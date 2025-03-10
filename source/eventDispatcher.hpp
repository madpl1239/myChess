/*
 * eventDispatcher.hpp
 * 
 * 9-03-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include "chessBoard.hpp"
#include "stockHandle.hpp"
#include "moveLogger.hpp"
#include "highLighter.hpp"
#include "sndManager.hpp"


class EventDispatcher
{
public:
	EventDispatcher(sf::RenderWindow& window, ChessBoard& board, Stockfish& engine, MoveLogger& moveLogger,
					Highlighter& highlighter, SoundManager& sndManager):
	m_window(window),
	m_board(board),
	m_engine(engine),
	m_moveLogger(moveLogger),
	m_highlighter(highlighter),
	m_sndManager(sndManager)
	{}

	bool dispatchEvents(bool& quit, bool& engineMovePending, sf::Clock& engineMoveTimer, std::string& position,
						std::string& commPlayer, std::string& commStockfish, std::string& fen,
						bool& isPieceSelected, sf::Vector2i& selectedPiece)
	{
		sf::Event event;
		while(m_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				quit = true;
			
			else if(event.type == sf::Event::KeyPressed)
				handleKeyPress(event, quit, position, commPlayer, commStockfish, fen);
			
			else if(event.type == sf::Event::MouseButtonPressed)
				handleMousePress(event, engineMovePending, engineMoveTimer, position, commPlayer, 
								 commStockfish, isPieceSelected, selectedPiece, fen);
		}
		
		return quit;
	}

private:
	void handleKeyPress(sf::Event& event, bool& quit, std::string& position, std::string& commPlayer,
						std::string& commStockfish, std::string& fen)
	{
		if(event.key.code == sf::Keyboard::Escape)
			quit = true;
		
		else if(event.key.code == sf::Keyboard::S)
			m_board.saveGame("./save_game.txt");
		
		else if(event.key.code == sf::Keyboard::L)
		{
			m_board.loadGame("./save_game.txt");
			m_board.m_loaded = true;
			position.clear();
			commPlayer.clear();
			commStockfish.clear();
			m_engine.sendCommand("ucinewgame");
			char sideToMove = m_board.getCurrentTurn();
			fen = m_board.generateFEN(sideToMove);
			std::cout << "Generated FEN after loading game: " << fen << "\n";
		}
	}

	void handleMousePress(sf::Event& event, bool& engineMovePending, sf::Clock& engineMoveTimer,
						  std::string& position, std::string& commPlayer, std::string& commStockfish,
							bool& isPieceSelected, sf::Vector2i& selectedPiece, std::string& fen)
	{
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i pos = sf::Mouse::getPosition(m_window) - sf::Vector2i(OFFSET, OFFSET);
			int x = std::round(pos.x / TILE_SIZE);
			int y = 7 - std::round(pos.y / TILE_SIZE);
			
			if(x < 8 and y < 8)
			{
				if(not isPieceSelected)
				{
					if(m_board.isPieceAt(x, y))
					{
						commPlayer = m_board.toChess(x, y);
						selectedPiece = sf::Vector2i(x, y);
						m_highlighter.setSelection(x, y);
						isPieceSelected = true;
						m_moveLogger.updateInvalidStatus("");
					}
				}
				else
				{
					if(m_board.isValidMove(selectedPiece.x, selectedPiece.y, x, y))
					{
						commPlayer += m_board.toChess(x, y);
						m_moveLogger.updateMove(true, commPlayer);
						
						sf::Vector2i rStart;
						sf::Vector2i rEnd;
						bool isCastling = m_board.castling(commPlayer, position, rStart, rEnd);
						
						position += " " + commPlayer;
						m_board.movePiece(selectedPiece.x, selectedPiece.y, x, y);
						
						if(isCastling and m_board.atBoard(rStart, rEnd))
							m_board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
						
						if(m_board.m_loaded)
						{
							commPlayer.clear();
							commStockfish.clear();
							commStockfish = getNextMoveAfterFEN(m_engine, fen, position);
						}
						else
						{
							commPlayer.clear();
							commStockfish.clear();
							commStockfish = getNextMove(m_engine, position);
						}
						
						m_moveLogger.updateMove(false, commStockfish);
						engineMovePending = true;
						engineMoveTimer.restart();
						m_sndManager.play("move");
					}
					else
					{
						m_moveLogger.updateInvalidStatus("Invalid move!");
						m_sndManager.play("invalid");
					}
					
					m_highlighter.setDestination(x, y);
					m_highlighter.setSelectionActive(false);
					isPieceSelected = false;
				}
			}
		}
	}

	sf::RenderWindow& m_window;
	ChessBoard& m_board;
	Stockfish& m_engine;
	MoveLogger& m_moveLogger;
	Highlighter& m_highlighter;
	SoundManager& m_sndManager;
};
