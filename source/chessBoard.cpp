/*
 * chessBoard.cpp
 *
 * 12-01-2025 by madpl
 */
#include <iostream>
#include <cmath>
#include "defines.hpp"
#include "chessBoard.hpp"
#include "moveLogger.hpp"


ChessBoard::ChessBoard(sf::RenderWindow& window, MoveLogger& logger, SoundManager& sndManager):
	m_window(window),
	m_moveLogger(logger),
	m_sndManager(sndManager),
	m_enPassantTarget(-1, -1)
{
	#ifdef DEBUG
	std::cout << "[DEBUG] ctor ChessBoard\n";
	#endif
	
	m_board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
	m_fullMoveNumber = 1;
}


ChessBoard::~ChessBoard()
{
	#ifdef DEBUG
	std::cout << "[DEBUG] dtor ChessBoard\n";
	#endif
	
	if(not m_board.empty())
	{
		for(int y = 0; y < 8; ++y)
			if(not m_board[y].empty())
				m_board[y].clear();
		
		m_board.clear();
	}
	
	m_fullMoveNumber = 0;
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

	// diagonal move
	if(dx == 1 and dy == 1)
	{
		// if the target square contains an opponent's piece - standard capture
		if(m_board[endY][endX].m_type != PieceType::NONE)
			return (m_board[endY][endX].m_color != pawn.m_color);
		else
		{
			// try beating test en passant:
			// we check if the target field corresponds to the en passant position.
			if(sf::Vector2i(endX, endY) == m_enPassantTarget)
			{
				// position of a pawn that could have been captured en passant is "behind" the target square,
				// i.e. towards the opponent – ​​we calculate his Y coordinate.
				int capturedPawnY = endY - direction;
				if(capturedPawnY >= 0 and capturedPawnY < 8)
				{
					if(m_board[capturedPawnY][endX].m_type == PieceType::PAWN and
					   m_board[capturedPawnY][endX].m_color != pawn.m_color)
						return true;
				}
			}
		}
	}

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
				
				break;
			}
		}
		
		if(kingX != -1)
			break;
	}

	if(kingX == -1 or kingY == -1)
	{
		#ifdef DEBUG
		std::cout << "[DEBUG] Error: King not found on m_board!\n";
		#endif
		
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
					#ifdef DEBUG
					std::cout << "[DEBUG] Piece (" << pieceTypeToString(m_board[y][x].m_type) 
								<< ") at (" << x << ", " << y << ") attack king!\n";
					#endif
					
					return true;
				}
			}
		}
	}

	return false;
}


std::string ChessBoard::pieceTypeToString(PieceType type) const
{
	std::string result = "Unknown";
	
	switch(type)
	{
		case PieceType::PAWN:
			result = "Pawn";
			break;
		
		case PieceType::ROOK:
			result = "Rook";
			break;
		
		case PieceType::KNIGHT:
			result = "Knight";
			break;
		
		case PieceType::BISHOP:
			result = "Bishop";
			break;
		
		case PieceType::QUEEN:
			result = "Queen";
			break;
		
		case PieceType::KING:
			result = "King";
			break;
		
		default:
			result = "Unknown";
			break;
	}
	
	return result;
}


char ChessBoard::pieceTypeToChar(PieceType type) const
{
	char result = ' ';
	
	switch(type)
	{
		case PieceType::PAWN:
			result = 'p';
			break;
			
		case PieceType::ROOK:
			result = 'r';
			break;
			
		case PieceType::KNIGHT:
			result = 'n';
			break;
			
		case PieceType::BISHOP:
			result = 'b';
			break;
			
		case PieceType::QUEEN:
			result = 'q';
			break;
			
		case PieceType::KING:
			result = 'k';
			break;
			
		default:
			result = ' ';
			break;
	}
	
	return result;
}


void ChessBoard::movePiece(int startX, int startY, int endX, int endY)
{
	Piece movingPiece = m_board[startY][startX];
	int dx = abs(endX - startX);
	int dy = abs(endY - startY);
	int direction = (movingPiece.m_color == 'W') ? 1 : -1;
	
	m_enPassantTarget = sf::Vector2i(-1, -1);
	bool enPassantCapture = false;

	// update en passant target before moving the piece
	if(movingPiece.m_type == PieceType::PAWN and dy == 2)
		m_enPassantTarget = sf::Vector2i(startX, startY + direction);
	else
		m_enPassantTarget = sf::Vector2i(-1, -1);

	// check en passant capture validity
	if(movingPiece.m_type == PieceType::PAWN and dx == 1 and dy == 1 and
		sf::Vector2i(endX, endY) == m_enPassantTarget)
	{
		int capturedPawnY = endY - direction;
		
		if(m_board[capturedPawnY][endX].m_type == PieceType::PAWN and
			m_board[capturedPawnY][endX].m_color != movingPiece.m_color)
		{
			enPassantCapture = true;
		}
	}

	// move piece to new position
	m_board[endY][endX] = movingPiece;
	m_board[startY][startX] = Piece();

	// remove opponent's pawn if en passant occurred
	if(enPassantCapture)
	{
		int capturedPawnY = endY - direction;
		m_board[capturedPawnY][endX] = Piece();
	}

	// setting the en passant possibilities:
	if(movingPiece.m_type == PieceType::PAWN and dy == 2)
		m_enPassantTarget = sf::Vector2i(startX, startY + direction);
	else
		m_enPassantTarget = sf::Vector2i(-1, -1);

	char currentPlayerColor = m_board[endY][endX].m_color;
	char opponentColor = (currentPlayerColor == 'W') ? 'B' : 'W';

	if(isInCheck(opponentColor))
	{
		m_moveLogger.updateCheckStatus("King is check!");
		m_sndManager.play("check");
		
		#ifdef DEBUG
		std::cout << (opponentColor == 'W' ? "[DEBUG] black" : "[DEBUG] white") 
					<< " king is in check!\n";
		#endif
	}
	else
		m_moveLogger.updateCheckStatus("");

	// increment full move number after Black's move
	if(movingPiece.m_color == 'B')
		++m_fullMoveNumber;
}


void ChessBoard::movePieceForce(int startX, int startY, int endX, int endY)
{
	Piece movingPiece = m_board[startY][startX];
	
	m_board[endY][endX] = movingPiece;
	m_board[startY][startX] = Piece();
}


std::string ChessBoard::toChess(int x, int y) const
{
	std::string str;

	str += static_cast<char>(97 + x);
	str += static_cast<char>(7 - y + 49);

	return str;
}


sf::Vector2i ChessBoard::toCoords(char col, char row) const
{
	int x = static_cast<int>(col - 'a');
	int y = 7 - static_cast<int>(row - '1');
	
	return sf::Vector2i(x, y);
}


std::string ChessBoard::boardToString(int x, int y) const
{
	std::string str;

	str += static_cast<char>(97 + x);
	str += static_cast<char>(y + 49);

	return str;
}


bool ChessBoard::atBoard(sf::Vector2i Start, sf::Vector2i End)
{
	if((Start.x >= 0 and Start.x < 8) and (Start.y >= 0 and Start.y < 8) and
	   (End.x >= 0 and End.x < 8) and (End.y >= 0 and End.y < 8))
		return true;
	
	return false;
}


bool ChessBoard::castling(std::string& str, std::string& position, sf::Vector2i& rookStart, sf::Vector2i& rookEnd)
{
	bool result = false;
	
	// castling if the king has not yet moved
	if(str == "e1g1") // king's move
		if(position.find("e1") == -1)
		{
			// "h1f1"
			rookStart = toCoords('h', '1');
			rookEnd = toCoords('f', '1');
			result = true;
		}

	if(str == "e8g8")
		if(position.find("e8") == -1)
		{
			// "h8f8"
			rookStart = toCoords('h', '8');
			rookEnd = toCoords('f', '8');
			result = true;
		}

	if(str == "e1c1")
		if(position.find("e1") == -1)
		{
			// "a1d1"
			rookStart = toCoords('a', '1');
			rookEnd = toCoords('d', '1');
			result = true;
		}

	if(str == "e8c8") 
		if(position.find("e8") == -1)
		{
			// "a8d8"
			rookStart = toCoords('a', '8');
			rookEnd = toCoords('d', '8');
			result = true;
		}
	
	return result;
}


std::string ChessBoard::generateFEN(char currentTurn)
{
	std::string fen = "";

	// 1. generation layout pieces
	for(int y = 7; y >= 0; --y)
	{
		int emptyCount = 0;
		for(int x = 0; x < 8; ++x)
		{
			const Piece& piece = m_board[y][x];
			
			if(piece.m_type == PieceType::NONE)
				++emptyCount;
			else
			{
				if(emptyCount > 0)
				{
					fen += std::to_string(emptyCount);
					emptyCount = 0;
				}
				
				char pieceChar = pieceTypeToChar(piece.m_type);
				fen += (piece.m_color == 'W') ? toupper(pieceChar) : tolower(pieceChar);
			}
		}
		
		if(emptyCount > 0)
			fen += std::to_string(emptyCount);
		
		if(y > 0)
			fen += "/";
	}

	// 2. current player
	fen += " ";
	fen += (currentTurn == 'W') ? "w" : "b";

	// 3. castling
	std::string castling = "";
	if(m_board[7][4].m_type == PieceType::KING && m_board[7][4].m_color == 'W')
	{
		if(m_board[7][7].m_type == PieceType::ROOK)
			castling += "K";
		
		if(m_board[7][0].m_type == PieceType::ROOK)
			castling += "Q";
	}

	if(m_board[0][4].m_type == PieceType::KING && m_board[0][4].m_color == 'B')
	{
		if(m_board[0][7].m_type == PieceType::ROOK)
			castling += "k";
		
		if(m_board[0][0].m_type == PieceType::ROOK)
			castling += "q";
	}
	
	fen += " " + (castling.empty() ? "-" : castling);

	// 4. en passant
	fen += " ";
	if(m_enPassantTarget.x != -1)
		fen += boardToString(m_enPassantTarget.x, m_enPassantTarget.y);
	else
		fen += "-";

	// 5. half move counter without capture
	fen += " 0";

	// 6. move counter
	fen += " " + std::to_string(m_fullMoveNumber);

	return fen;
}


void ChessBoard::saveGame(const std::string& filename)
{
	std::ofstream file(filename);

	if(not file)
	{
		std::cerr << "Error: Unable to open file for saving!\n";
		
		return;
	}
	
	for(int y = 0; y < 8; ++y)
	{
		for(int x = 0; x < 8; ++x)
		{
			const Piece& piece = m_board[y][x];
			char color = (piece.m_type == PieceType::NONE) ? 'N' : piece.m_color;
			
			file << static_cast<int>(piece.m_type) << " " << color << " " << x << " " << y << "\n";
		}
		
		file << "*\n";
	}
	
	file << "ENPASSANT " << m_enPassantTarget.x << " " << m_enPassantTarget.y << "\n";
	
	file.close();
}


void ChessBoard::loadGame(const std::string& filename)
{
	std::ifstream file(filename);
	
	if(not file)
	{
		std::cerr << "Error: Unable to open file for loading!\n";
		
		return;
	}

	m_board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
	std::string line;
	char currentTurn = 'W'; // White's move by default

	while(std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string type;
		int x, y;
		char color;
		
		iss >> type;
		if(type == "ENPASSANT")
		{
			iss >> x >> y;
			m_enPassantTarget = sf::Vector2i(x, y);
		}
		
		else if(type == "TURN") 
			iss >> currentTurn;
		
		else if(type == "*")
			continue;
		
		else
		{
			try
			{
				int pieceType = std::stoi(type);
				iss >> color >> x >> y;
				
				if(color == 'N')
					continue;
				
				if(x < 0 or x >= 8 or y < 0 or y >= 8)
					continue;
				
				m_board[y][x] = Piece(static_cast<PieceType>(pieceType), color);
			}
			
			catch(const std::exception& e)
			{
				std::cerr << "Error parsing line: " << line << std::endl;
			}
		}
	}

	file.close();
}


const Piece& ChessBoard::getPiece(int x, int y) const
{
	return m_board[y][x];
}


const sf::Vector2i& ChessBoard::getEnPassantTarget() const
{
	return m_enPassantTarget;
}


void ChessBoard::draw(sf::Texture& boardTexture, sf::Texture& figuresTexture) 
{
	sf::Sprite boardSprite(boardTexture);
	sf::Sprite pieceSprite(figuresTexture);
	
	boardSprite.setPosition(0, 0);
	m_window.draw(boardSprite);
	
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
			pieceSprite.setPosition(x * TILE_SIZE + OFFSET, y * TILE_SIZE + OFFSET);
			pieceSprite.setOrigin(0, 1);
			
			m_window.draw(pieceSprite);
		}
	}
}
