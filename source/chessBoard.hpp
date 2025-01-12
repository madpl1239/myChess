/*
 * chessBoard.hpp
 *
 * 12-01-2025 by madpl (aka madpl1239)
 */
#pragma once

#include <SFML/Graphics.hpp>
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
