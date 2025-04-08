/*
 * pieceEditor.cpp
 * 
 * 07-04-2025 by madpl
 */
#include "pieceEditor.hpp"
#include <iostream>


PieceEditor::PieceEditor(ChessBoard& board):
	m_board(board)
{
	loadPieceIcons();
}


PieceEditor::~PieceEditor()
{
	
}


void PieceEditor::toggleEditor()
{
	m_isActive = !m_isActive;
	
	std::cout << "Editor " << (m_isActive ? "ON" : "OFF") << std::endl;
}


sf::Vector2i PieceEditor::getBoardCoordinates(const sf::Vector2i& pixelPos)
{
	return sf::Vector2i(std::round(pixelPos.x / TILE_SIZE), 7 - std::round(pixelPos.y / TILE_SIZE));
}


void PieceEditor::placePiece(int row, int col)
{
	m_board.getBoard()[row][col] = Piece(m_selectedType, m_selectedColor);
}


void PieceEditor::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::E)
		toggleEditor();

	if(not m_isActive)
		return;

	if(event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window) - sf::Vector2i(OFFSET, OFFSET);
		
		int y = mousePos.y;
		if(y > 504 and y < 720)
		{
			int index = mousePos.x / TILE_SIZE;
			
			if(index >= 0 and index <= 5)
			{
				m_selectedType = static_cast<PieceType>(index + 1);
				
				return;
			}
		}
		
		auto coords = getBoardCoordinates(mousePos);
		if(coords.x >= 0 and coords.y >= 0 and coords.x < 8 and coords.y < 8)
			placePiece(coords.y, coords.x);
	}

	if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::C)
	{
		m_selectedColor = (m_selectedColor == 'W') ? 'B' : 'W';
		
		loadPieceIcons();
	}
}


void PieceEditor::draw(sf::RenderWindow& window)
{
	if(not m_isActive)
		return;
	
	drawUI(window);
}


bool PieceEditor::isActive() const
{
	return m_isActive;
}


void PieceEditor::drawUI(sf::RenderWindow& window)
{
	int y = 4 * TILE_SIZE;
	
	for(int i = 1; i <= 6; ++i)
	{
		auto sprite = m_pieceIcons[(PieceType)i];
		sprite.setPosition(504 + (OFFSET * 2) + ((i - 1) * TILE_SIZE), y);
		
		window.draw(sprite);
	}
}


void PieceEditor::loadPieceIcons()
{
	m_textureAtlas.loadFromFile("./resources/figures.png");
	m_pieceIcons.clear();

	int tileSize = TILE_SIZE;
	int row = (m_selectedColor == 'W') ? 0 : 1;

	m_pieceIcons[PieceType::PAWN].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::PAWN].setTextureRect(sf::IntRect(tileSize * 5, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::KNIGHT].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::KNIGHT].setTextureRect(sf::IntRect(tileSize * 4, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::BISHOP].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::BISHOP].setTextureRect(sf::IntRect(tileSize * 3, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::ROOK].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::ROOK].setTextureRect(sf::IntRect(tileSize * 2, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::QUEEN].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::QUEEN].setTextureRect(sf::IntRect(tileSize * 1, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::KING].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::KING].setTextureRect(sf::IntRect(tileSize * 0, tileSize * row, tileSize, tileSize));
}
