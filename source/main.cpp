/*
 * main.cpp
 *
 * Chess GUI for stockfish (testing version from branch working).
 *
 * 12-01-2025 by madpl (aka Bogdan Kułaga)
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
#include "game.hpp"
#include "scoreBar.hpp"


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
		
		MoveLogger moveLogger(SIZE + 10, 10);
		ScoreBar scoreBar(504, 505);
		
		SoundManager sndManager{};
		if(not initialSounds(sndManager))
			throw std::runtime_error("load sounds error!");
		
		sf::Texture boardTexture;
		if(not boardTexture.loadFromFile("./resources/board.png"))
			throw std::runtime_error("Could not load board.png");
		
		sf::Texture figuresTexture;
		if(not figuresTexture.loadFromFile("./resources/figures.png"))
			throw std::runtime_error("Could not load figures.png");
		
		sf::Texture bgTexture;
		if(not bgTexture.loadFromFile("./resources/background1.jpg"))
			throw std::runtime_error("Could not load background1.jpg");
		
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
		
		Highlighter highlighter{};
		
		Game game(window, board, engine, moveLogger,
				  highlighter, scoreBar, sndManager,
				  boardTexture, figuresTexture, bgTexture);
		
		game.run();
		
		window.close();
	}
	
	catch(std::exception& e)
	{
		std::cout << "throw exception: " << e.what() << "\n";
		
		return -1;
	}
	
	return 0;
}
