/*
 * chessBoard.cpp
 *
 * 12-01-2025 by madpl
 */
#include <iostream>
#include <cmath> // For abs()
#include "defines.hpp"
#include "chessBoard.hpp"


ChessBoard::ChessBoard()
{
	m_board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
}


void ChessBoard::setInitialPositions()
{
	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			if(y == 1)
				m_board[y][x] = Piece(PieceType::PAWN, 'W');
			
			else if(y == 6)
				m_board[y][x] = Piece(PieceType::PAWN, 'B');
			
			else if(y == 0 or y == 7)
			{
				char color = (y == 0) ? 'W' : 'B';
				
				if(x == 0 or x == 7)
					m_board[y][x] = Piece(PieceType::ROOK, color);
				
				if(x == 1 or x == 6)
					m_board[y][x] = Piece(PieceType::KNIGHT, color);
				
				if(x == 2 or x == 5)
					m_board[y][x] = Piece(PieceType::BISHOP, color);
				
				if(x == 3)
					m_board[y][x] = Piece(PieceType::QUEEN, color);
				
				if(x == 4)
					m_board[y][x] = Piece(PieceType::KING, color);
			}
		}
	}
}


bool ChessBoard::isPieceAt(int x, int y) const
{
	return m_board[y][x].m_type != PieceType::NONE;
}


bool ChessBoard::isValidMove(int startX, int startY, int endX, int endY) const
{
	if(startX == endX and startY == endY)
		return false;
	
	if(m_board[startY][startX].m_color == m_board[endY][endX].m_color)
		return false;

	Piece piece = m_board[startY][startX];
	int dx = abs(endX - startX);
	int dy = abs(endY - startY);

	switch(piece.m_type)
	{
		case PieceType::PAWN:
			return validatePawnMove(piece, startX, startY, endX, endY, dx, dy);
		
		case PieceType::ROOK:
			return (dx == 0 or dy == 0) and isPathClear(startX, startY, endX, endY);
		
		case PieceType::KNIGHT:
			return (dx == 2 and dy == 1) or (dx == 1 and dy == 2);
		
		case PieceType::BISHOP:
			return dx == dy and isPathClear(startX, startY, endX, endY);
		
		case PieceType::QUEEN:
			return (dx == dy or dx == 0 or dy == 0) and isPathClear(startX, startY, endX, endY);
		
		case PieceType::KING:
			return validateKingMove(piece, startX, startY, endX, endY, dx, dy);
		
		default:
			return false;
	}
}


bool ChessBoard::validatePawnMove(const Piece& pawn, int startX, int startY, int endX, int endY, int dx, int dy) const
{
	int direction = (pawn.m_color == 'W') ? 1 : -1;

	// Normal move forward
	if(dx == 0)
	{
		if(dy == 1 and m_board[endY][endX].m_type == PieceType::NONE)
			return startY + direction == endY;
		
		if(dy == 2 and ((pawn.m_color == 'W' and startY == 1) or (pawn.m_color == 'B' and startY == 6)))
			return (m_board[startY + direction][startX].m_type == PieceType::NONE and
					m_board[endY][endX].m_type == PieceType::NONE);
	}

	// Capture diagonally
	if(dx == 1 and dy == 1)
		return (m_board[endY][endX].m_type != PieceType::NONE and
				m_board[endY][endX].m_color != pawn.m_color);

	return false;
}


bool ChessBoard::validateKingMove(const Piece& king, int startX, int startY, int endX, int endY, int dx, int dy) const
{
	if(dx <= 1 and dy <= 1)
		return true;

	// Castling check (basic version)
	if(dx == 2 and dy == 0 and !isInCheck(king.m_color))
	{
		int rookX = (endX > startX) ? 7 : 0;
		
		if(m_board[startY][rookX].m_type == PieceType::ROOK and m_board[startY][rookX].m_color == king.m_color)
			return isPathClear(startX, startY, rookX, startY);
	}

	return false;
}


bool ChessBoard::isPathClear(int startX, int startY, int endX, int endY) const
{
	int xDir = (endX - startX) ? (endX - startX) / abs(endX - startX) : 0;
	int yDir = (endY - startY) ? (endY - startY) / abs(endY - startY) : 0;

	int x = startX + xDir;
	int y = startY + yDir;

	while(x != endX or y != endY)
	{
		if(m_board[y][x].m_type != PieceType::NONE)
			return false;
		
		x += xDir;
		y += yDir;
	}

	return true;
}


bool ChessBoard::isInCheck(char kingColor) const
{
	int kingX = -1, kingY = -1;

	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			if(m_board[y][x].m_type == PieceType::KING and m_board[y][x].m_color == kingColor)
			{
				kingX = x;
				kingY = y;
				
				std::cout << "King (" << (kingColor == 'W' ? "White" : "Black") 
							<< ") found at position (" << kingX << ", " << kingY << ")\n";
				
				break;
			}
		}
		
		if(kingX != -1)
			break;
	}

	if(kingX == -1 or kingY == -1)
	{
		std::cerr << "Error: King not found on m_board!\n";
		
		return false;
	}

	// Check if any opponent's piece can attack the king
	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			if(m_board[y][x].m_color != kingColor and m_board[y][x].m_type != PieceType::NONE)
			{
				if(isValidMove(x, y, kingX, kingY))
				{
					std::cout << "Piece (" << pieceTypeToString(m_board[y][x].m_type) 
								<< ") at (" << x << ", " << y << ") attack king!\n";
					
					return true;
				}
			}
		}
	}

	std::cout << "King (" << (kingColor == 'W' ? "White" : "Black") << ") is not in check.\n";

	return false;
}


std::string ChessBoard::pieceTypeToString(PieceType type) const
{
	switch(type)
	{
		case PieceType::PAWN:
			return "Pawn";
		
		case PieceType::ROOK:
			return "Rook";
		
		case PieceType::KNIGHT:
			return "Knight";
		
		case PieceType::BISHOP:
			return "Bishop";
		
		case PieceType::QUEEN:
			return "Queen";
		
		case PieceType::KING:
			return "King";
		
		default:
			return "Unknown";
	}
}


void ChessBoard::movePiece(int startX, int startY, int endX, int endY)
{
	m_board[endY][endX] = m_board[startY][startX];
	m_board[startY][startX] = Piece();

	char currentPlayerColor = m_board[endY][endX].m_color;
	char opponentColor = (currentPlayerColor == 'W') ? 'B' : 'W';

	if(isInCheck(opponentColor))
		std::cout << (opponentColor == 'W' ? "black" : "white") << " king is in check!\n";
}


void ChessBoard::draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int frameOffset) 
{
	sf::Sprite boardSprite(boardTexture);
	sf::Sprite pieceSprite(figuresTexture);
	
	boardSprite.setPosition(0, 0);
	window.draw(boardSprite);
	
	for(int y = 0; y < 8; ++y) 
	{
		for(int x = 0; x < 8; ++x) 
		{
			Piece piece = m_board[y][x];
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
