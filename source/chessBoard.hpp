/*
 * chessBoard.hpp
 *
 * 12-01-2025 by madpl (aka madpl1239)
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "piece.hpp"


class ChessBoard 
{
public:
	ChessBoard();
	void setInitialPositions();
	void draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int frameOffset);
	bool isPieceAt(int x, int y) const;
	bool isValidMove(int startX, int startY, int endX, int endY) const;
	void movePiece(int startX, int startY, int endX, int endY);

private:
	std::vector<std::vector<Piece>> board;
};
