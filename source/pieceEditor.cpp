/*
 * pieceEditor.cpp
 * 
 * 07-04-2025 by madpl
 */
#include "pieceEditor.hpp"


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
	return sf::Vector2i(std::floor(pixelPos.x / TILE_SIZE), 7 - std::floor(pixelPos.y / TILE_SIZE));
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
		
		int selectionBarX = (10 * TILE_SIZE) - OFFSET;
		int selectionBarY = 4 * TILE_SIZE;
		
		if(mousePos.y > selectionBarY and mousePos.y < selectionBarY + TILE_SIZE)
		{
			int index = std::floor((mousePos.x - selectionBarX) / TILE_SIZE);
			if(index >= 0 and index < static_cast<int>(m_pieceIcons.size()))
			{
				m_selectedType = static_cast<PieceType>(index);
				
				return;
			}
		}
		
		auto coords = getBoardCoordinates(mousePos);
		if(coords.x >= 0 and coords.y >= 0 and coords.x < 8 and coords.y < 8)
			placePiece(coords.y, coords.x);
	}
	
	if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::N)
		m_selectedType = PieceType::NONE;

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
	
	drawSelectionBar(window);
}


bool PieceEditor::isActive() const
{
	return m_isActive;
}


void PieceEditor::drawSelectionBar(sf::RenderWindow& window)
{
	int baseX = 10 * TILE_SIZE;
	int baseY = OFFSET + 4 * TILE_SIZE;
	
	for(size_t i = 0; i < 6; ++i)
	{
		sf::Sprite& icon = m_pieceIcons[(PieceType)i];
		icon.setPosition(baseX + (i * TILE_SIZE), baseY);
		window.draw(icon);
	}
}


void PieceEditor::loadPieceIcons()
{
	m_textureAtlas.loadFromFile("./resources/figures.png");
	m_pieceIcons.clear();

	int tileSize = TILE_SIZE;
	int row = (m_selectedColor == 'W') ? 1 : 0;

	m_pieceIcons[PieceType::PAWN].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::PAWN].setTextureRect(sf::IntRect(tileSize * 5, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::KING].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::KING].setTextureRect(sf::IntRect(tileSize * 4, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::QUEEN].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::QUEEN].setTextureRect(sf::IntRect(tileSize * 3, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::BISHOP].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::BISHOP].setTextureRect(sf::IntRect(tileSize * 2, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::KNIGHT].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::KNIGHT].setTextureRect(sf::IntRect(tileSize * 1, tileSize * row, tileSize, tileSize));

	m_pieceIcons[PieceType::ROOK].setTexture(m_textureAtlas);
	m_pieceIcons[PieceType::ROOK].setTextureRect(sf::IntRect(tileSize * 0, tileSize * row, tileSize, tileSize));
}
