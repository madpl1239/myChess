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
	bool isPieceAt(int x, int y) const;
	bool isValidMove(int startX, int startY, int endX, int endY) const;
	void movePiece(int startX, int startY, int endX, int endY);
	bool validatePawnMove(const Piece& pawn, int startX, int startY, int endX, int endY, int dx, int dy) const;
	bool validateKingMove(const Piece& king, int startX, int startY, int endX, int endY, int dx, int dy) const;
	bool isPathClear(int startX, int startY, int endX, int endY) const;
	bool isInCheck(char kingColor) const;
	std::string pieceTypeToString(PieceType type) const;
	
	void draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int frameOffset);

private:
	std::vector<std::vector<Piece>> board;
};
