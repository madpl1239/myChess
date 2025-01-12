/*
 * chessBoard.cpp
 *
 * 12-01-2025 by madpl
 */
#include "chessBoard.hpp"
#include <iostream>


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
				board[y][x] = Piece(PieceType::PAWN, 'B');
			
			else if(y == 6)
				board[y][x] = Piece(PieceType::PAWN, 'W');
			
			else if(y == 0 || y == 7) 
			{
				char color = (y == 0) ? 'B' : 'W';
				
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


void ChessBoard::draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int frameOffset) 
{
	sf::Sprite boardSprite(boardTexture);
	boardSprite.setPosition(0, 0);
	window.draw(boardSprite);

	int pieceWidth = figuresTexture.getSize().x / 6;
	int pieceHeight = figuresTexture.getSize().y / 2;

	sf::Sprite pieceSprite(figuresTexture);

	for(int y = 0; y < 8; ++y) 
	{
		for(int x = 0; x < 8; ++x) 
		{
			Piece piece = board[y][x];
			if(piece.type == PieceType::NONE)
				continue;
			
			int pieceIndex = static_cast<int>(piece.type);
			int colorOffset = (piece.color == 'B') ? pieceHeight : 0;
			
			pieceSprite.setTextureRect(sf::IntRect(pieceIndex * pieceWidth, colorOffset, pieceWidth, pieceHeight));
			pieceSprite.setPosition(frameOffset + x * 56, frameOffset + y * 56);
			window.draw(pieceSprite);
		}
	}
}


bool ChessBoard::isPieceAt(int x, int y) const 
{
	return board[y][x].type != PieceType::NONE;
}


bool ChessBoard::isValidMove(int startX, int startY, int endX, int endY) const 
{
	if(startX == endX and startY == endY)
		return false;
	
	if(board[startY][startX].color == board[endY][endX].color)
		return false;

	return true;
}


void ChessBoard::movePiece(int startX, int startY, int endX, int endY) 
{
	board[endY][endX] = board[startY][startX];
	board[startY][startX] = Piece();
}
