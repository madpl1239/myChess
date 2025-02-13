/*
 * testFEN.cpp
 * 
 * Testing generateFEN() method.
 * 
 * compilation:
 * g++ -o testFEN testFEN.cpp ../source/chessBoard.cpp ../source/moveLogger.cpp \
 * 		-lgtest -lgtest_main -lpthread -I../source -lsfml-system -lsfml-window \
 * 		-lsfml-graphics -lsfml-audio
 * 
 * 11-02-2025 by madpl
 */
#include <gtest/gtest.h>
#include "chessBoard.hpp"
#include "moveLogger.hpp"


// Test 1: check correctness of FEN for the starting position
TEST(ChessBoardTest, InitialPosition)
{
	sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	ChessBoard board(window, logger);
	board.setInitialPositions();
	
	std::string expectedFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
	EXPECT_EQ(board.generateFEN('W'), expectedFEN);
}


// Test 2: check correctness of FEN after the moves e2e4 and d7d5
TEST(ChessBoardTest, AfterPawnMoves)
{
	sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	ChessBoard board(window, logger);
	board.setInitialPositions();
	
	// move e2e4
	board.movePiece(4, 1, 4, 3);
	// move d7d5
	board.movePiece(3, 6, 3, 4);
	
	std::string expectedFEN = "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w - d6 0 2";
	EXPECT_EQ(board.generateFEN('W'), expectedFEN);
}


// Test 3: check white's kingside castling
TEST(ChessBoardTest, CastlingKingSideWhite)
{
	sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	ChessBoard board(window, logger);
	board.setInitialPositions();
	
	// manually setting up kingside castling for white
	board.movePieceForce(4, 0, 6, 0); // king e1 -> g1
	board.movePieceForce(7, 0, 5, 0); // rook h1 -> f1
	
	std::string expectedFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 b - - 0 1";
	EXPECT_EQ(board.generateFEN('B'), expectedFEN);
}


// Test 4: check the capture en passant
TEST(ChessBoardTest, EnPassantCapture)
{
	sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	ChessBoard board(window, logger);
	board.setInitialPositions();
	
	// movements to set the en passant position
	board.movePiece(4, 1, 4, 3); // e2 -> e4
	board.movePiece(3, 6, 3, 4); // d7 -> d5
	board.movePiece(4, 3, 3, 4); // e4 bije d5 en passant
	
	std::string expectedFEN = "rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR b - - 0 2";
	EXPECT_EQ(board.generateFEN('B'), expectedFEN);
}


// Test 5: check correctness of the FEN after capturing the figure
TEST(ChessBoardTest, CapturePiece)
{
	sf::RenderWindow window(sf::VideoMode(SIZE + 400, SIZE), "");
	MoveLogger logger(SIZE + 10, 10);
	ChessBoard board(window, logger);
	board.setInitialPositions();
	
	// white pawn e2 -> e4
	board.movePiece(4, 1, 4, 3);
	// black pawn d7 -> d5
	board.movePiece(3, 6, 3, 4);
	// white pawn e4 -> d5 taking the black pawn
	board.movePiece(4, 3, 3, 4);
	
	std::string expectedFEN = "rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR b - - 0 2";
	EXPECT_EQ(board.generateFEN('B'), expectedFEN);
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}
