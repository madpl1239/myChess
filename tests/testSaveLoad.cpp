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

	ChessBoard board(sf::RenderWindow(), MoveLogger(), SoundManager());
	board.setInitialPositions();
	board.saveGame("test_save.txt");

	ChessBoard loadedBoard(sf::RenderWindow(), MoveLogger(), SoundManager());
	loadedBoard.loadGame("test_save.txt");

	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			assert(board.m_board[y][x].m_type == loadedBoard.m_board[y][x].m_type);
			assert(board.m_board[y][x].m_color == loadedBoard.m_board[y][x].m_color);
		}
	}
	
	assert(board.m_enPassantTarget == loadedBoard.m_enPassantTarget);

	std::cout << "Test passed: Save & Load Game ✅\n";
}


int main(void)
{
	testSaveLoadGame();

	return 0;
}
