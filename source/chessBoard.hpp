/*
 * chessBoard.hpp
 *
 * 12-01-2025 by madpl (aka madpl1239, aka Bogdan Kułaga)
 */
#pragma once

#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "piece.hpp"
#include "moveLogger.hpp"
#include "sndManager.hpp"
#include "chessFont.hpp"


class ChessBoard
{
public:
	ChessBoard(sf::RenderWindow& window, MoveLogger& logger, SoundManager& sndManager, ChessFont& pieceFont);
	~ChessBoard();

	void setInitialPositions();
	
	bool isPieceAt(int x, int y) const;
	bool isValidMove(int startX, int startY, int endX, int endY) const;
	
	void movePiece(int startX, int startY, int endX, int endY);
	void movePieceForce(int startX, int startY, int endX, int endY);
	
	std::string toChess(int x, int y) const;
	sf::Vector2i toCoords(char col, char row) const;
	std::string boardToString(int x, int y) const;
	
	bool atBoard(sf::Vector2i Start, sf::Vector2i End);
	bool castling(std::string& str, std::string& position, sf::Vector2i& rookStart, sf::Vector2i& rookEnd);
	
	std::string generateFEN(char currentTurn);
	
	void saveGame(const std::string& filename);
	void loadGame(const std::string& filename);
	
	// getters
	const Piece& getPiece(int x, int y) const;
	const sf::Vector2i& getEnPassantTarget() const;
	const char getCurrentTurn() const;
	const int getFullMoveNumber() const;
	
	// setters
	void setCurrentTurn(char side);
	void setFullMoveNumber(int number);
	
	void draw(sf::Texture& boardTexture, sf::Texture& figuresTexture, sf::Texture& bgTexture);
	
	bool m_loaded;

private:
	bool validatePawnMove(const Piece& pawn, int startX, int startY, int endX, int endY, int dx, int dy) const;
	bool validateKingMove(const Piece& king, int startX, int startY, int endX, int endY, int dx, int dy) const;
	bool isPathClear(int startX, int startY, int endX, int endY) const;
	bool isInCheck(char kingColor) const;
	std::string pieceTypeToString(PieceType type) const;
	char pieceTypeToChar(PieceType type) const;

	std::vector<std::vector<Piece>> m_board;
	sf::Vector2i m_enPassantTarget;
	int m_fullMoveNumber;
	char m_currentTurn;
	MoveLogger& m_moveLogger;
	SoundManager& m_sndManager;
	sf::RenderWindow& m_window;
	ChessFont& m_pieceFont;
};
