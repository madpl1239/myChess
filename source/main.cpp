/*
 * main.cpp
 *
 * Validating of chess moves.
 *
 * 12-01-2025 by madpl
 */
#include <SFML/Graphics.hpp>
#include <vector>
#include "chessBoard.hpp" 


int main(void)
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Game");

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

	const int offset = 28; // Offset for the board
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		board.draw(window, boardTexture, figuresTexture, offset);
		window.display();
	}

	return 0;
}
