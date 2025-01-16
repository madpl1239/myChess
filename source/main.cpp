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
		
		ChessBoard board;
		board.setInitialPositions();
		
		bool isPieceSelected = false;
		sf::Vector2i selectedPiece;
		const int frameOffset = OFFSET;
		
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
						sf::Vector2i position = sf::Mouse::getPosition(window) - sf::Vector2i(OFFSET, OFFSET);
						
						int x = std::round(position.x / TILE_SIZE);
						int y = std::round(position.y / TILE_SIZE);
						
						if(x < 8 and y < 8)
						{
							if(not isPieceSelected)
							{
								if(board.isPieceAt(x, y))
								{
									selectedPiece = sf::Vector2i(x, y);
									isPieceSelected = true;
								}
							}
							else
							{
								if(board.isValidMove(selectedPiece.x, selectedPiece.y, x, y))
									board.movePiece(selectedPiece.x, selectedPiece.y, x, y);
								else
									std::cout << "Invalid move!" << std::endl;
								
								isPieceSelected = false;
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
