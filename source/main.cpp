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
#include "sndManager.hpp"


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
		
		sf::Clock engineMoveTimer;
		bool engineMovePending = false;
		
		MoveLogger moveLogger(SIZE + 10, 10);
		SoundManager sndManager;
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
		
		Highlighter highlighter;
		
		std::string position = "";
		std::string commPlayer = "";
		std::string commStockfish = "";
		std::string fen = "";
		
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
					
					if(event.key.code == sf::Keyboard::S)
						board.saveGame("./save_game.txt");
					
					if(event.key.code == sf::Keyboard::L)
					{
						board.loadGame("./save_game.txt");
						board.m_loaded = true;
						
						// transferring the new layout to Stockfish
						char sideToMove = board.getCurrentTurn();
						fen = board.generateFEN(sideToMove);
						std::cout << "Generated FEN after loading game: " << fen << "\n";
						// engine.sendCommand("ucinewgame");
						// engine.sendCommand("isready");
						// std::string response = engine.getResponse();
						// std::cout << "response after position fen isready?: " << response << std::endl;
					}
				}
				
				else if(event.type == sf::Event::MouseButtonPressed)
				{
					if(event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i pos = sf::Mouse::getPosition(window) - sf::Vector2i(OFFSET, OFFSET);
						int x = std::round(pos.x / TILE_SIZE);
						int y = 7 - std::round(pos.y / TILE_SIZE);
						
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
									moveLogger.updateInvalidStatus("");
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
									board.movePiece(selectedPiece.x, selectedPiece.y, x, y);
									
									if(isCastling and board.atBoard(rStart, rEnd))
										board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
									
									if(board.m_loaded)
									{
										engine.sendCommand("ucinewgame");
										engine.sendCommand("position fen " + fen + " moves " + commPlayer);
										engine.sendCommand("isready");
										
										std::string response = engine.getResponse();
										std::cout << "response after position fen isready?: " << response << std::endl;
										
										engine.sendCommand("go depth 2");
										
										board.m_loaded = false;
										commPlayer.clear();
										commStockfish.clear();
									}	
									
									commPlayer.clear();
									commStockfish.clear();
									commStockfish = getNextMove(engine, position);
									
									moveLogger.updateMove(false, commStockfish);
									
									engineMovePending = true;
									engineMoveTimer.restart();
									
									sndManager.play("move");
								}
								else
								{
									#ifdef DEBUG
									std::cout << "[DEBUG] Invalid move!\n";
									#endif
									
									moveLogger.updateInvalidStatus("Invalid Move!");
									sndManager.play("invalid");
								}
								
								highlighter.setDestination(x, y);
								highlighter.setSelectionActive(false);
								isPieceSelected = false;
							}
						}
						else
						{
							#ifdef DEBUG
							std::cout << "[DEBUG] x, y out of range!\n";
							#endif
						}
					}
				}
			}
			
			// checking for 1 seconds
			if(engineMovePending and engineMoveTimer.getElapsedTime().asSeconds() >= 1)
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
				{
					#ifdef DEBUG
					std::cout << "[DEBUG] commStockfish = " << commStockfish << "\n";
					std::cout << "[DEBUG] Invalid move from engine!\n";
					#endif
				}
				
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
