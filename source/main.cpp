/*
 * main.cpp
 *
 * Validating of chess moves.
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


std::string getNextMove(Stockfish& engine, std::string& position)
{
	std::string command = "position startpos moves" + position;
	
	engine.sendCommand(command);
	engine.sendCommand("go depth 2");
	
	std::string response = engine.getResponse();
		
	size_t bestmoveIdx = response.find("bestmove");
	if(bestmoveIdx != std::string::npos)
	{
		size_t endIdx = response.find(' ', bestmoveIdx + 9);
		
		return (endIdx != std::string::npos) ? response.substr(bestmoveIdx + 9, endIdx - (bestmoveIdx + 9))
												: response.substr(bestmoveIdx + 9);
	}
	
	return "no response";
}


bool castling(ChessBoard& board, std::string& str, std::string& position, sf::Vector2i& rookStart, sf::Vector2i& rookEnd)
{
	bool result = false;
	
	// castling if the king has not yet moved
	if(str == "e1g1") // king's move
		if(position.find("e1") == -1)
		{
			// h1f1
			rookStart = board.toCoords('h', '1');
			rookEnd = board.toCoords('f', '1');
			result = true;
		}

	if(str == "e8g8")
		if(position.find("e8") == -1)
		{
			// h8f8
			rookStart = board.toCoords('h', '8');
			rookEnd = board.toCoords('f', '8');
			result = true;
		}

	if(str == "e1c1")
		if(position.find("e1") == -1)
		{
			// a1d1
			rookStart = board.toCoords('a', '1');
			rookEnd = board.toCoords('d', '1');
			result = true;
		}

	if(str == "e8c8") 
		if(position.find("e8") == -1)
		{
			// a8d8
			rookStart = board.toCoords('a', '8');
			rookEnd = board.toCoords('d', '8');
			result = true;
		}
		
	return result;
}

	
int main(void) 
{
	try
	{
		// logo
		std::cout << "guiChess v0.5 - by madpl 2025\n";
		
		sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "guiChess by madpl 2025");
		window.setPosition(sf::Vector2i(600, 200));
		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);
		
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
		
		engine.sendCommand("uci");
		if(engine.getResponse().find("uciok") == std::string::npos)
		{
			engine.sendCommand("quit");
			std::cerr << "engine: not uciok!\n";
			
			return -1;
		}
		
		engine.sendCommand("uci");
		engine.sendCommand("setoption name Skill Level value 1");
		std::cout << engine.getResponse();
		
		engine.sendCommand("isready");
		if(engine.getResponse().find("readyok") != std::string::npos)
			std::cout << "readyok\n";
		else
		{
			engine.sendCommand("quit");
			std::cerr << "not readyok!\n";
			
			return -1;
		}
		
		engine.sendCommand("ucinewgame");
		engine.sendCommand("isready");
		
		std::string resp = engine.getResponse();
		if(resp.find("readyok") == std::string::npos)
			throw std::runtime_error("response error");
		
		#ifdef DEBUG
		std::cout << "[DEBUG] ucinewgame\n";
		std::cout << "[DEBUG] readyok\n";
		#endif
		
		ChessBoard board;
		board.setInitialPositions();
		
		std::string position = "";
		std::string commPlayer = "";
		std::string commStockfish = "";
		
		const int frameOffset = OFFSET;
		sf::Vector2i selectedPiece;
		bool isPieceSelected = false;
		
		while(window.isOpen()) 
		{
			sf::Event event;
			while(window.pollEvent(event)) 
			{
				if(event.type == sf::Event::Closed)
					window.close();
				
				if(event.type == sf::Event::MouseButtonPressed)
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
									isPieceSelected = true;
								}
							}
							else
							{
								if(board.isValidMove(selectedPiece.x, selectedPiece.y, x, y))
								{
									commPlayer += board.toChess(x, y);
									
									#ifdef DEBUG
									std::cout << "[DEBUG] commPlayer = " << commPlayer << "\n";
									#endif
									
									sf::Vector2i rStart;
									sf::Vector2i rEnd;
									bool isCastling = castling(board, commPlayer, position, rStart, rEnd);
									
									position += " " + commPlayer;
									commPlayer.clear();
									
									board.movePiece(selectedPiece.x, selectedPiece.y, x, y);
									if(isCastling)
										board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
								}
								else
									std::cout << "Invalid move!\n";
								
								isPieceSelected = false;
								
								// engine response
								commStockfish.clear();
								commStockfish = getNextMove(engine, position);
								
								#ifdef DEBUG
								std::cout << "[DEBUG] commStockfish = " << commStockfish << "\n";
								#endif
								
								sf::Vector2i rStart;
								sf::Vector2i rEnd;
								bool isCastling = castling(board, commStockfish, position, rStart, rEnd);
								
								position += " " + commStockfish;
								sf::Vector2i posStart = board.toCoords(commStockfish[0], commStockfish[1]);
								sf::Vector2i posEnd = board.toCoords(commStockfish[2], commStockfish[3]);
								
								if(board.atBoard(posStart, posEnd))
									board.movePiece(posStart.x, posStart.y, posEnd.x, posEnd.y);
								else
									std::cout << "Invalid move from engine!\n";
								
								if(isCastling)
									board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
							}
						}
						else
							std::cout << "x, y out of range!\n";
					}
				}
			}
			
			window.clear();
			board.draw(window, boardTexture, figuresTexture, frameOffset);
			window.display();
		}
	}
	
	catch(std::exception& e)
	{
		std::cerr << "throw exception: " << e.what() << "\n";
		
		return -1;
	}

	return 0;
}
