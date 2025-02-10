/*
 * testSaveLoad.cpp
 * 
 * Testing save, load abilities.
 * 
 * 09-02-2025 by madpl
 */
#include "chessBoard.hpp"
#include <iostream>
#include <cassert>


void testSaveLoadGame()
{
	std::cout << "Running test: Save & Load Game\n";

	sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	SoundManager sndManager;
	
	ChessBoard board(window, logger, sndManager);
	board.setInitialPositions();
	board.saveGame("test_save.txt");

	ChessBoard loadedBoard(window, logger, sndManager);
	loadedBoard.loadGame("test_save.txt");

	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			assert(board.getPiece(y, x).m_type == loadedBoard.getPiece(y, x).m_type);
			assert(board.getPiece(y, x).m_color == loadedBoard.getPiece(y, x).m_color);
		}
	}
	
	assert(board.getEnPassantTarget() == loadedBoard.getEnPassantTarget());

	std::cout << "Test passed: Save & Load Game ✅\n";
}


int main(void)
{
	testSaveLoadGame();

	return 0;
}
