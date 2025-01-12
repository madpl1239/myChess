/*
 * chessBoard.cpp
 *
 * 12-01-2025 by madpl
 */
#include "chessBoard.hpp"


void ChessBoard::setInitialPositions()
{
	// Place rooks
	board[0][0] = Piece(PieceType::ROOK, 'W');
	board[0][7] = Piece(PieceType::ROOK, 'W');
	board[7][0] = Piece(PieceType::ROOK, 'B');
	board[7][7] = Piece(PieceType::ROOK, 'B');

	// Place knights
	board[0][1] = Piece(PieceType::KNIGHT, 'W');
	board[0][6] = Piece(PieceType::KNIGHT, 'W');
	board[7][1] = Piece(PieceType::KNIGHT, 'B');
	board[7][6] = Piece(PieceType::KNIGHT, 'B');

	// Place bishops
	board[0][2] = Piece(PieceType::BISHOP, 'W');
	board[0][5] = Piece(PieceType::BISHOP, 'W');
	board[7][2] = Piece(PieceType::BISHOP, 'B');
	board[7][5] = Piece(PieceType::BISHOP, 'B');

	// Place queens
	board[0][3] = Piece(PieceType::QUEEN, 'W');
	board[7][3] = Piece(PieceType::QUEEN, 'B');

	// Place kings
	board[0][4] = Piece(PieceType::KING, 'W');
	board[7][4] = Piece(PieceType::KING, 'B');

	// Place pawns
	for(int i = 0; i < 8; ++i)
	{
		board[1][i] = Piece(PieceType::PAWN, 'W');
		board[6][i] = Piece(PieceType::PAWN, 'B');
	}
}


void ChessBoard::draw(sf::RenderWindow& window, sf::Texture& boardTexture, sf::Texture& figuresTexture, int offset)
{
	sf::Sprite boardSprite(boardTexture);
	boardSprite.setPosition(offset, offset);
	window.draw(boardSprite);

	int pieceSize = figuresTexture.getSize().y / 2; // Assuming 2 rows (white, black)
	sf::Sprite pieceSprite(figuresTexture);

	for(int x = 0; x < 8; ++x)
	{
		for(int y = 0; y < 8; ++y)
		{
			Piece piece = board[x][y];
			if(piece.type == PieceType::NONE)
				continue;

			int pieceIndex = static_cast<int>(piece.type);
			int colorOffset = (piece.color == 'B') ? pieceSize : 0;

			pieceSprite.setTextureRect(sf::IntRect(pieceIndex * pieceSize, colorOffset, pieceSize, pieceSize));
			pieceSprite.setPosition(offset + y * pieceSize, offset + x * pieceSize);
			window.draw(pieceSprite);
		}
	}
}
