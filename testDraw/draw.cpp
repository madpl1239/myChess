/*
 * draw.cpp
 *
 * 26-02-202 by madpl
 */
#include <iostream>
#include "chessBoard.hpp"
#include "moveLogger.hpp"
#include "sndManager.hpp"


int main(void)
{
	std::cout << "Running test: draw board\n";
	
	sf::RenderWindow win(sf::VideoMode(SIZE + 400, SIZE), "");
	win.setPosition({200, 100});
	
	MoveLogger logger(SIZE + 10, 10);
	SoundManager snd{};
	
	sf::Texture boardTexture;
	if(not boardTexture.loadFromFile("../resources/board.png"))
		throw std::runtime_error("Could not load board.png");
		
	sf::Texture figuresTexture;
	if(not figuresTexture.loadFromFile("../resources/figures.png"))
		throw std::runtime_error("Could not load figures.png");
	
	ChessBoard board(win, logger, snd);
	board.setInitialPositions();
	
	bool quit = false;
	while(win.isOpen() and not quit)
	{
		sf::Event e;
		while(win.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				quit = true;
		}
		
		win.clear(sf::Color(0x7F, 0xAC, 0x7F, 0xFF));
		board.draw(boardTexture, figuresTexture);
		win.display();
	}
	
	std::cout << "done.\n";
	win.close();
	
	return 0;
}
