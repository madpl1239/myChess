/*
 * chessFont.hpp
 * 
 * 24-03-2024 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <iostream>


class ChessFont
{
public:
	ChessFont(const std::string& fontPath)
	{
		if(not m_font.loadFromFile(fontPath))
			throw std::runtime_error("Error: Unable to load font!");
	}

	void addCapturedPiece(const std::string& pieceName, char col)
	{
		if(m_pieceSymbols.find(pieceName) != m_pieceSymbols.end())
		{
			sf::Text piece;
			piece.setFont(m_font);
			piece.setCharacterSize(PIECEFONT_SIZE);
			piece.setFillColor(col == 'W' ? sf::Color::White : sf::Color::Black);
			piece.setString(m_pieceSymbols[pieceName]);
			
			if(col == 'W')
			{
				int index = capturedWhite.size();
				int row = index / MAX_PIECES_PER_ROW;
				int colPos = index % MAX_PIECES_PER_ROW;
				piece.setPosition(START_X + colPos * SPACING, START_Y + row * LINE_HEIGHT);
				capturedWhite.push_back(piece);
			}
			
			else if(col == 'B')
			{
				int index = capturedBlack.size();
				int row = index / MAX_PIECES_PER_ROW;
				int colPos = index % MAX_PIECES_PER_ROW;
				piece.setPosition(START_X + colPos * SPACING, START_Y + 2 * LINE_HEIGHT + row * LINE_HEIGHT);
				capturedBlack.push_back(piece);
			}
		}
		else
			std::cerr << "Error: Unknown chess piece!\n";
	}
	
	std::string pieceTypeToSymbol(PieceType type, char col)
	{
		std::string str = "none_none";
		
		if(col == 'W')
			switch(type)
			{
				case PieceType::KING:
					str = "king_white";
					break;
				
				case PieceType::QUEEN:
					str = "queen_white";
					break;
				
				case PieceType::ROOK:
					str = "rook_white";
					break;
				
				case PieceType::BISHOP:
					str = "bishop_white";
					break;
				
				case PieceType::KNIGHT:
					str = "knight_white";
					break;
				
				case PieceType::PAWN:
					str = "pawn_white";
					break;
				
				default:
					str = "none_none";
					break;
			}
		
		else if(col == 'B')
			switch(type)
			{
				case PieceType::KING:
					str = "king_black";
					break;
				
				case PieceType::QUEEN:
					str = "queen_black";
					break;
				
				case PieceType::ROOK:
					str = "rook_black";
					break;
				
				case PieceType::BISHOP:
					str = "bishop_black";
					break;
				
				case PieceType::KNIGHT:
					str = "knight_black";
					break;
				
				case PieceType::PAWN:
					str = "pawn_black";
					break;
				
				default:
					str = "none_none";
					break;
			}
		
		return str;
	}

	void draw(sf::RenderWindow& window)
	{
		for(const auto& piece: capturedWhite)
			window.draw(piece);
		
		for(const auto& piece: capturedBlack)
			window.draw(piece);
	}

private:
	sf::Font m_font;
	std::vector<sf::Text> capturedWhite;
	std::vector<sf::Text> capturedBlack;

	const int PIECEFONT_SIZE = 36;
	const int SPACING = 32;
	const int START_X = 540;
	const int START_Y = 180;
	const int LINE_HEIGHT = 40;
	const int MAX_PIECES_PER_ROW = 8;

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
		{"pawn_black", L'\u265F'},
		{"none_none", L'\u2699'}
	};
};
