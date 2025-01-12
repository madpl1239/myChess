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
	sf::RenderWindow window(sf::VideoMode(600, 600), "Chess Game");

	sf::Texture boardTexture;
	if(!boardTexture.loadFromFile("board.png"))
	{
		std::cerr << "Failed to load board texture" << std::endl;
		
		return -1;
	}

	sf::Texture figuresTexture;
	if(!figuresTexture.loadFromFile("figures.png"))
	{
		std::cerr << "Failed to load figures texture" << std::endl;
		
		return -1;
	}

	ChessBoard board;
	board.setInitialPositions();

	const int offset = 48;
	bool isPieceSelected = false;
	sf::Vector2i selectedPiece;

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
					sf::Vector2i position = sf::Mouse::getPosition(window) - sf::Vector2i(offset, offset);
					int x = position.y / (boardTexture.getSize().x / 8);
					int y = position.x / (boardTexture.getSize().y / 8);
					
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
						
						isPieceSelected = false;
					}
				}
			}
		}
		
		window.clear();
		board.draw(window, boardTexture, figuresTexture, offset);
		window.display();
	}

	return 0;
}
