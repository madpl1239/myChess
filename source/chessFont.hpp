/*
 * chessFont.hpp
 * 
 * 24-03-2024 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>


class ChessFont
{
public:
	enum color_t
	{
		White,
		Black
	};
	
	ChessFont(const std::string& fontPath)
	{
		if(not m_font.loadFromFile(fontPath))
			throw std::runtime_error("Error: Unable to load font!");
		
		m_piece.setFont(m_font);
		m_piece.setCharacterSize(62);
		m_piece.setFillColor(sf::Color::White);
	}

	void setPiece(const std::string& pieceName, float x, float y, color_t col)
	{
		if(m_pieceSymbols.find(pieceName) != m_pieceSymbols.end())
		{
			m_piece.setString(m_pieceSymbols[pieceName]);
			m_piece.setPosition(x, y);
			
			if(col == color_t::White)
				m_piece.setFillColor(sf::Color::White);
			
			else if(col == color_t::Black)
				m_piece.setFillColor(sf::Color::Black);
		}
		else
			std::cerr << "Error: Unknown chess piece!\n";
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(m_piece);
	}
	
private:
	sf::Font m_font;
	sf::Text m_piece;

	std::unordered_map<std::string, wchar_t> m_pieceSymbols
	{
		{"king_white", L'\u2654'},
		{"queen_white", L'\u2655'},
		{"rook_white", L'\u2656'},
		{"bishop_white", L'\u2657'},
		{"knight_white", L'\u2658'},
		{"pawn_white", L'\u2659'},
		{"king_black", L'\u265A'},
		{"queen_black", L'\u265B'},
		{"rook_black", L'\u265C'},
		{"bishop_black", L'\u265D'},
		{"knight_black", L'\u265E'},
		{"pawn_black", L'\u265F'}
	};
};
