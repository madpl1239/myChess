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
	window(window),
	board(board),
	engine(engine),
	moveLogger(moveLogger),
	highlighter(highlighter),
	sndManager(sndManager)
	{}

	bool dispatchEvents(bool& quit, bool& engineMovePending, sf::Clock& engineMoveTimer, std::string& position, std::string& commPlayer,
							std::string& commStockfish, std::string& fen, bool& isPieceSelected, sf::Vector2i& selectedPiece)
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				quit = true;
			}

			else if(event.type == sf::Event::KeyPressed)
			{
				handleKeyPress(event, quit, position, commPlayer, commStockfish, fen);
			}

			else if(event.type == sf::Event::MouseButtonPressed)
			{
				handleMousePress(event, engineMovePending, engineMoveTimer, position, commPlayer, commStockfish, isPieceSelected, selectedPiece);
			}
		}

		return quit;
	}

private:
	void handleKeyPress(sf::Event& event, bool& quit, std::string& position, std::string& commPlayer,
						std::string& commStockfish, std::string& fen)
	{
		if(event.key.code == sf::Keyboard::Escape)
		{
			quit = true;
		}

		else if(event.key.code == sf::Keyboard::S)
		{
			board.saveGame("./save_game.txt");
		}

		else if(event.key.code == sf::Keyboard::L)
		{
			board.loadGame("./save_game.txt");
			board.m_loaded = true;
			position.clear();
			commPlayer.clear();
			commStockfish.clear();
			engine.sendCommand("ucinewgame");
			char sideToMove = board.getCurrentTurn();
			fen = board.generateFEN(sideToMove);
			std::cout << "Generated FEN after loading game: " << fen << "\n";
		}
	}

	void handleMousePress(sf::Event& event, bool& engineMovePending, sf::Clock& engineMoveTimer, std::string& position, std::string& commPlayer,
							std::string& commStockfish, bool& isPieceSelected, sf::Vector2i& selectedPiece)
	{
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i pos = sf::Mouse::getPosition(window) - sf::Vector2i(OFFSET, OFFSET);
			int x = std::round(pos.x / TILE_SIZE);
			int y = 7 - std::round(pos.y / TILE_SIZE);
			
			if(x < 8 and y < 8)
			{
				if(!isPieceSelected)
				{
					if(board.isPieceAt(x, y))
					{
						commPlayer = board.toChess(x, y);
						selectedPiece = sf::Vector2i(x, y);
						highlighter.setSelection(x, y);
						isPieceSelected = true;
						moveLogger.updateInvalidStatus("");
					}
				}
				else
				{
					if(board.isValidMove(selectedPiece.x, selectedPiece.y, x, y))
					{
						commPlayer += board.toChess(x, y);
						moveLogger.updateMove(true, commPlayer);
						
						sf::Vector2i rStart;
						sf::Vector2i rEnd;
						bool isCastling = board.castling(commPlayer, position, rStart, rEnd);
						
						position += " " + commPlayer;
						board.movePiece(selectedPiece.x, selectedPiece.y, x, y);
						
						if(isCastling and board.atBoard(rStart, rEnd))
						{
							board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
						}
						
						if(board.m_loaded)
						{
							commPlayer.clear();
							commStockfish.clear();
							commStockfish = getNextMoveAfterFEN(engine, fen, position);
						}
						else
						{
							commPlayer.clear();
							commStockfish.clear();
							commStockfish = getNextMove(engine, position);
						}

						moveLogger.updateMove(false, commStockfish);
						engineMovePending = true;
						engineMoveTimer.restart();
						sndManager.play("move");
					}
					else
					{
						moveLogger.updateInvalidStatus("Invalid move!");
						sndManager.play("invalid");
					}

					highlighter.setDestination(x, y);
					highlighter.setSelectionActive(false);
					isPieceSelected = false;
				}
			}
		}
	}

	sf::RenderWindow& window;
	ChessBoard& board;
	Stockfish& engine;
	MoveLogger& moveLogger;
	Highlighter& highlighter;
	SoundManager& sndManager;
};
