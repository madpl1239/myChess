/*
 * piece.hpp
 *
 * 12-01-2025 by madpl
 */
#pragma once


enum class PieceType
{
	ROOK = 0,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING,
	PAWN,
	NONE
};


struct Piece
{
public:
    Piece(PieceType t = PieceType::NONE, char c = ' '):
		m_type(t),
		m_color(c)
	{}
    
    PieceType m_type;
    char m_color; // 'W' for white, 'B' for black
};
