/*
 * saveLoad.cpp
 * 
 * tests new possibility: save and load game.
 * 
 * 10-02-2025 by madpl
 */
#include "chessBoard.hpp"
#include <fstream>
#include <cassert>


void testSaveLoadGame()
{
	std::cout << "Running test: Save & Load Game\n";
	
	sf::RenderWindow win(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	SoundManager sndManager;
	
	ChessBoard board(win, logger, sndManager);
	board.setInitialPositions();
	board.saveGame("test_save.txt");
	
	ChessBoard loadedBoard(win, logger, sndManager);
	loadedBoard.loadGame("test_save.txt");
	
	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			assert(board.getPiece(x, y).m_type == loadedBoard.getPiece(x, y).m_type);
			assert(board.getPiece(x, y).m_color == loadedBoard.getPiece(x, y).m_color);
		}
	}
	
	assert(board.getEnPassantTarget() == loadedBoard.getEnPassantTarget());
	std::cout << "Test passed: Save and Load Game\n";
}


int main(void)
{
	testSaveLoadGame();
	
	return 0;
}
