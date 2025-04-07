/*
 * pieceEditor.hpp
 * 
 * 07-04-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "defines.hpp"
#include "chessBoard.hpp"
#include "piece.hpp"


class PieceEditor
{
public:
	PieceEditor(ChessBoard& board);
	~PieceEditor();
	
	void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	
	bool isActive() const;

private:
	void toggleEditor();
	sf::Vector2i getBoardCoordinates(const sf::Vector2i& pixelPos);
	void placePiece(int row, int col);
	void drawUI(sf::RenderWindow& window);
	void loadPieceIcons();
	
	ChessBoard& m_board;
	bool m_isActive = false;
	sf::Vector2i m_selectedSquare;
	PieceType m_selectedType = PieceType::NONE;
	char m_selectedColor = 'W';
	std::map<PieceType, sf::Sprite> m_pieceIcons;
	sf::Texture m_textureAtlas;
};
