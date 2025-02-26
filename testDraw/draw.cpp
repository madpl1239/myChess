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
	
	ChessBoard board(win, logger, snd);
	board.setInitialPositions();
	
	bool quit = false;
	while(not quit)
	{
		sf::Event e;
		while(win.poolEvent(e))
		{
			if(e.)
		}
		
		board.draw();
		win.display();
	}
	
	std::cout << "done.\n";
	
	return 0;
}
