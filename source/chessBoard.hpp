/*
 * chessBoard.hpp
 *
 * 12-01-2025 by madpl (aka madpl1239)
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "piece.hpp"
#include "moveLogger.hpp"
#include "sndManager.hpp"


class ChessBoard 
{
public:
	ChessBoard(sf::RenderWindow& window, MoveLogger& logger, SoundManager& sndManager);
	~ChessBoard();

	void setInitialPositions();
	bool isPieceAt(int x, int y) const;
	bool isValidMove(int startX, int startY, int endX, int endY) const;
	void movePiece(int startX, int startY, int endX, int endY);
	std::string toChess(int x, int y);
	sf::Vector2i toCoords(char col, char row);
	bool atBoard(sf::Vector2i Start, sf::Vector2i End);
	bool castling(std::string& str, std::string& position, sf::Vector2i& rookStart, sf::Vector2i& rookEnd);

	void draw(sf::Texture& boardTexture, sf::Texture& figuresTexture);

private:
	bool validatePawnMove(const Piece& pawn, int startX, int startY, int endX, int endY, int dx, int dy) const;
	bool validateKingMove(const Piece& king, int startX, int startY, int endX, int endY, int dx, int dy) const;
	bool isPathClear(int startX, int startY, int endX, int endY) const;
	bool isInCheck(char kingColor) const;
	std::string pieceTypeToString(PieceType type) const;

	std::vector<std::vector<Piece>> m_board;
	sf::Vector2i m_enPassantTarget;
	MoveLogger& m_moveLogger;
	SoundManager& m_sndManager;
	sf::RenderWindow& m_window;
};
