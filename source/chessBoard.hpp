/*
 * chessBoard.hpp
 *
 * 12-01-2025 by madpl
 */
#pragma once

#include "piece.hpp"


class ChessBoard
{
public:
	ChessBoard()
	{
		// Initialize an empty 8x8 board
		board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
	}

	void setInitialPositions();
	void draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int offset);
	
public:
	std::vector<std::vector<Piece>> board;
};
