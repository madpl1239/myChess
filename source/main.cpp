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
#include "chessBoard.hpp"


int main(void) 
{
	sf::RenderWindow window(sf::VideoMode(504, 504), "Chess Game");

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
	const int frameOffset = 28;

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
					sf::Vector2i position = sf::Mouse::getPosition(window) - sf::Vector2i(frameOffset, frameOffset);
					int x = position.y / 56;
					int y = position.x / 56;
				
					if(!isPieceSelected)
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
			}
		}
		
		window.clear();
		board.draw(window, boardTexture, figuresTexture, frameOffset);
		window.display();
	}

	return 0;
}
