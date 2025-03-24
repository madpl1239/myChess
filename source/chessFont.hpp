/*
 * chessFont.hpp
 * 
 * 24-03-2024 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>


class ChessFont
{
public:
	ChessFont(const std::string& fontPath)
	{
		if(not font.loadFromFile(fontPath))
			throw std::runtime_error("Error: Unable to load font!");
		
		piece.setFont(font);
		piece.setCharacterSize(100);
		piece.setFillColor(sf::Color::White);
	}

	void setPiece(const std::string& pieceName, float x, float y)
	{
		if(pieceSymbols.find(pieceName) != pieceSymbols.end())
		{
			piece.setString(pieceSymbols[pieceName]);
			piece.setPosition(x, y);
		}
		else
			std::cerr << "Error: Unknown chess piece!\n";
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(piece);
	}
	
private:
	sf::Font font;
	sf::Text piece;

	std::unordered_map<std::string, wchar_t> pieceSymbols
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
