/*
 * chessBoard.cpp
 *
 * 12-01-2025 by madpl
 */
#include <iostream>
#include "defines.hpp"
#include "chessBoard.hpp"


ChessBoard::ChessBoard() 
{
	board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
}


void ChessBoard::setInitialPositions() 
{
	for(int y = 0; y < 8; ++y) 
	{
		for(int x = 0; x < 8; ++x) 
		{
			if(y == 1)
				board[y][x] = Piece(PieceType::PAWN, 'W');
			
			else if(y == 6)
				board[y][x] = Piece(PieceType::PAWN, 'B');
			
			else if(y == 0 || y == 7) 
			{
				char color = (y == 0) ? 'W' : 'B';
				
				if(x == 0 || x == 7)
					board[y][x] = Piece(PieceType::ROOK, color);
				
				if(x == 1 || x == 6)
					board[y][x] = Piece(PieceType::KNIGHT, color);
				
				if(x == 2 || x == 5)
					board[y][x] = Piece(PieceType::BISHOP, color);
				
				if(x == 3)
					board[y][x] = Piece(PieceType::QUEEN, color);
				
				if(x == 4)
					board[y][x] = Piece(PieceType::KING, color);
			}
		}
	}
}


bool ChessBoard::isPieceAt(int x, int y) const 
{
	if(x < 8 and y < 8)
		return board[y][x].m_type != PieceType::NONE;
	else
	{
		std::cout << "x, y is higher than 7!\n";
		
		return false;
	}
}


bool ChessBoard::isValidMove(int startX, int startY, int endX, int endY) const 
{
	if(startX > 7 or startY > 7 or endX > 7 or endY > 7)
	{
		std::cout << "x, y is higher than 7!\n";
		
		return false;
	}
	
	if(startX == endX and startY == endY)
		return false;
	
	if(startX < 8 and startY < 8 and endX < 8 and endY < 8)
		if(board[startY][startX].m_color == board[endY][endX].m_color)
			return false;

	return true;
}


void ChessBoard::movePiece(int startX, int startY, int endX, int endY) 
{
	board[endY][endX] = board[startY][startX];
	board[startY][startX] = Piece();
}


void ChessBoard::draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int frameOffset) 
{
	sf::Sprite boardSprite(boardTexture);
	boardSprite.setPosition(0, 0);
	window.draw(boardSprite);

	sf::Sprite pieceSprite(figuresTexture);

	for(int y = 0; y < 8; ++y) 
	{
		for(int x = 0; x < 8; ++x) 
		{
			Piece piece = board[y][x];
			if(piece.m_type == PieceType::NONE)
				continue;
			
			int pieceIndex = static_cast<int>(piece.m_type);
			int colorOffset = (piece.m_color == 'B') ? TILE_SIZE : 0;
			
			pieceSprite.setTextureRect(sf::IntRect(pieceIndex * TILE_SIZE, colorOffset, TILE_SIZE, TILE_SIZE));
			pieceSprite.setPosition((x * TILE_SIZE) + frameOffset, (y * TILE_SIZE) + frameOffset);
			pieceSprite.setOrigin(0, 1);
			window.draw(pieceSprite);
		}
	}
}
