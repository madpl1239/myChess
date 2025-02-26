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
	win.setPosition(200, 100);
	
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
		while(win.poolEvent(e))
		{
			if(e.event.)
				guit = true;
		}
		
		board.draw(boardTexture, figuresTexture);
		win.display();
	}
	
	win.close();
	
	std::cout << "done.\n";
	
	return 0;
}
