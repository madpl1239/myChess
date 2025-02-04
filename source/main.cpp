/*
 * main.cpp
 *
 * Chess GUI for stockfish (any version).
 *
 * 12-01-2025 by madpl
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


int main(void) 
{
	try
	{
		// Logo
		std::cout << "myChess v1.0\n";
		
		sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "myChess v1.0");
		window.setPosition(sf::Vector2i(200, 200));
		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);
		
		sf::Clock engineMoveTimer;
		bool engineMovePending = false;
		
		MoveLogger moveLogger(SIZE + 10, 10);
		
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
		
		ChessBoard board(window, moveLogger);
		board.setInitialPositions();
		
		Highlighter highlighter;
		
		std::string position = "";
		std::string commPlayer = "";
		std::string commStockfish = "";
		
		sf::Vector2i selectedPiece;
		bool isPieceSelected = false;
		bool quit = false;
		
		while(window.isOpen() and !quit) 
		{
			sf::Event event;
			while(window.pollEvent(event)) 
			{
				if(event.type == sf::Event::Closed)
					quit = true;
				
				else if(event.type == sf::Event::KeyPressed)
				{
					if(event.key.code == sf::Keyboard::Escape)
						quit = true;
				}
				
				else if(event.type == sf::Event::MouseButtonPressed)
				{
					if(event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i pos = sf::Mouse::getPosition(window) - sf::Vector2i(OFFSET, OFFSET);
						int x = std::round(pos.x / TILE_SIZE);
						int y = std::round(pos.y / TILE_SIZE);
						
						if(x < 8 and y < 8)
						{
							if(not isPieceSelected)
							{
								if(board.isPieceAt(x, y))
								{
									commPlayer = board.toChess(x, y);
									selectedPiece = sf::Vector2i(x, y);
									highlighter.setSelection(x, y);
									isPieceSelected = true;
								}
							}
							else
							{
								if(board.isValidMove(selectedPiece.x, selectedPiece.y, x, y))
								{
									commPlayer += board.toChess(x, y);
									moveLogger.updateMove(true, commPlayer);
									
									sf::Vector2i rStart;
									sf::Vector2i rEnd;
									bool isCastling = board.castling(commPlayer, position, rStart, rEnd);
									
									position += " " + commPlayer;
									commPlayer.clear();
									board.movePiece(selectedPiece.x, selectedPiece.y, x, y);
									
									if(isCastling and board.atBoard(rStart, rEnd))
										board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
									
									commStockfish.clear();
									commStockfish = getNextMove(engine, position);
									moveLogger.updateMove(false, commStockfish);
									
									engineMovePending = true;
									engineMoveTimer.restart();
								}
								else
									std::cout << "Invalid move!\n";
								
								highlighter.setDestination(x, y);
								isPieceSelected = false;
							}
						}
						else
							std::cout << "x, y out of range!\n";
					}
				}
			}
			
			// checking for 2 seconds
			if(engineMovePending and engineMoveTimer.getElapsedTime().asSeconds() >= 2)
			{
				engineMovePending = false;
				
				sf::Vector2i rStart;
				sf::Vector2i rEnd;
				bool isCastling = board.castling(commStockfish, position, rStart, rEnd);
				
				position += " " + commStockfish;
				sf::Vector2i posStart = board.toCoords(commStockfish[0], commStockfish[1]);
				sf::Vector2i posEnd = board.toCoords(commStockfish[2], commStockfish[3]);
				
				if(board.atBoard(posStart, posEnd))
					board.movePiece(posStart.x, posStart.y, posEnd.x, posEnd.y);
				else
					std::cout << "Invalid move from engine!\n";
				
				if(isCastling and board.atBoard(rStart, rEnd))
					board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
			}
			
			if(engineMovePending == false)
				highlighter.clear();
			
			window.clear(sf::Color(0x8F, 0xBC, 0x8F, 0xFF));
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
