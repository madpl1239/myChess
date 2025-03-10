/*
 * main.cpp
 *
 * Chess GUI for stockfish (testing version from branch working).
 *
 * 12-01-2025 by madpl (aka Bogdan Kułaga)
 */
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "defines.hpp"
#include "chessBoard.hpp"
#include "stockHandle.hpp"
#include "utils.hpp"
#include "moveLogger.hpp"
#include "highLighter.hpp"
#include "sndManager.hpp"
#include "eventDispatcher.hpp"


int main(void) 
{
	try
	{
		// Logo
		std::cout << "myChess testing version\n";
		
		sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "myChess - testing");
		window.setPosition(sf::Vector2i(200, 200));
		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);
		
		sf::Clock engineMoveTimer{};
		bool engineMovePending = false;
		
		MoveLogger moveLogger(SIZE + 10, 10);
		SoundManager sndManager{};
		if(not initialSounds(sndManager))
			throw std::runtime_error("load sounds error!");
		
		sf::Texture boardTexture;
		if(not boardTexture.loadFromFile("./resources/board.png"))
			throw std::runtime_error("Could not load board.png");
		
		sf::Texture figuresTexture;
		if(not figuresTexture.loadFromFile("./resources/figures.png"))
			throw std::runtime_error("Could not load figures.png");
		
		#ifdef LINUX
		Stockfish engine("./stockfish");
		#endif
		
		#ifdef WINDOWS
		Stockfish engine("./stockfish.exe");
		#endif
		
		if(initialCommand(engine) == -1)
			throw std::runtime_error("engine error!");
		
		ChessBoard board(window, moveLogger, sndManager);
		board.setInitialPositions();
		
		Highlighter highlighter{};
		
		std::string position = "";
		std::string commPlayer = "";
		std::string commStockfish = "";
		std::string fen = "";
		
		sf::Vector2i selectedPiece;
		bool isPieceSelected = false;
		bool quit = false;
		
		EventDispatcher dispatcher(window, board, engine, moveLogger, highlighter, sndManager);
		
		while(window.isOpen() and !quit) 
		{
			quit = dispatcher.dispatchEvents(quit, engineMovePending, engineMoveTimer, position, commPlayer, commStockfish, fen, isPieceSelected, selectedPiece);
			
			// checking for 1 seconds
			if(engineMovePending and engineMoveTimer.getElapsedTime().asSeconds() >= 1)
			{
				engineMovePending = false;
				
				sf::Vector2i rStart;
				sf::Vector2i rEnd;
				
				// commStockfish, position not updated in castling() method
				bool isCastling = board.castling(commStockfish, position, rStart, rEnd);
				
				position += " " + commStockfish;
				sf::Vector2i posStart = board.toCoords(commStockfish[0], commStockfish[1]);
				sf::Vector2i posEnd = board.toCoords(commStockfish[2], commStockfish[3]);
				
				if(board.atBoard(posStart, posEnd))
					board.movePiece(posStart.x, posStart.y, posEnd.x, posEnd.y);
				else
				{
					#ifdef DEBUG
					std::cout << "[DEBUG] commStockfish = " << commStockfish << "\n";
					std::cout << "[DEBUG] Invalid move from engine!\n";
					#endif
					
					moveLogger.updateInvalidStatus("Invalid engine move!");
					sndManager.play("invalid");
				}
				
				// move of rook
				if(isCastling and board.atBoard(rStart, rEnd))
					board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
				
				sndManager.play("move");
			}
			
			if(not engineMovePending)
				highlighter.setDestination(-5, -5);
				
			window.clear(sf::Color(0x7F, 0xAC, 0x7F, 0xFF));
			board.draw(boardTexture, figuresTexture);
			highlighter.draw(window);
			moveLogger.draw(window);
			window.display();
		}
		
		window.close();
	}
	
	catch(std::exception& e)
	{
		std::cout << "throw exception: " << e.what() << "\n";
		
		return -1;
	}
	
	return 0;
}
