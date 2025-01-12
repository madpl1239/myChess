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
#include "defines.hpp"
#include "chessBoard.hpp"


int main(void) 
{
	sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "Chess Game");

	sf::Texture boardTexture;
	if(not boardTexture.loadFromFile("./resources/board.png"))
	{
		std::cerr << "Failed to load board texture" << std::endl;
		
		return -1;
	}

	sf::Texture figuresTexture;
	if(not figuresTexture.loadFromFile("./resources/figures.png"))
	{
		std::cerr << "Failed to load figures texture" << std::endl;
		
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
					int x = position.y / TILE_SIZE;
					int y = position.x / TILE_SIZE;
					
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
					
					std::cout << "mouse clicked\n";
				}
			}
		}
		
		window.clear();
		board.draw(window, boardTexture, figuresTexture, frameOffset);
		window.display();
	}

	return 0;
}
