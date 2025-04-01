/*
 * game.hpp
 * 
 * 9-03-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.hpp"
#include "chessBoard.hpp"
#include "stockHandle.hpp"
#include "moveLogger.hpp"
#include "highLighter.hpp"
#include "sndManager.hpp"
#include "scoreBar.hpp"


class Game
{
public:
	Game(sf::RenderWindow& window, ChessBoard& board, Stockfish& engine,
		 MoveLogger& moveLogger, TextFader& fader, Highlighter& highlighter, 
		 ScoreBar& scoreBar, SoundManager& sndManager,
		 sf::Texture& boardTexture, sf::Texture& figuresTexture, sf::Texture& bgTexture):
	m_window(window),
	m_board(board),
	m_engine(engine),
	m_moveLogger(moveLogger),
	m_textFader(fader),
	m_highlighter(highlighter),
	m_scoreBar(scoreBar),
	m_sndManager(sndManager),
	m_boardTexture(boardTexture),
	m_figuresTexture(figuresTexture),
	m_bgTexture(bgTexture),
	m_mate(false)
	{
		#ifdef DEBUG
		std::cout << "[DEBUG] ctor Game\n";
		#endif
	}
	
	~Game()
	{
		m_engineMoveTimer = {};
		m_engineMovePending = false;
		
		m_position = "";
		m_commPlayer = "";
		m_commStockfish = "";
		m_fen = "";
		
		m_selectedPiece = {};
		m_isPieceSelected = false;
		
		m_mate = false;
		m_quit = false;
		
		#ifdef DEBUG
		std::cout << "[DEBUG] dtor Game\n";
		#endif
	}

	void run()
	{
		m_quit = false;
		
		while(m_window.isOpen() and !m_quit) 
		{
			sf::Time deltaTime = m_loggerTimer.restart();
			
			gameLoop();
			
			if(not m_engineMovePending)
				m_highlighter.setDestination(-5, -5);
			
			m_textFader.update();
			m_moveLogger.updateSaveLoad();
			
			m_window.clear(sf::Color(0x7F, 0xAC, 0x7F, 0xFF));
			
			m_board.draw(m_boardTexture, m_figuresTexture, m_bgTexture);
			m_highlighter.draw(m_window);
			m_scoreBar.draw(m_window);
			m_moveLogger.draw(m_window);
			m_textFader.draw(m_window);
			
			m_window.display();
		}
	}

private:
	void gameLoop()
	{
		sf::Event event;
		while(m_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				m_quit = true;
			
			else if(event.type == sf::Event::KeyPressed)
				handleKeyPress(event);
			
			else if(event.type == sf::Event::MouseButtonPressed)
				handleMousePress(event); // player move
		}
		
		// engine move
		if(m_engineMovePending and m_engineMoveTimer.getElapsedTime().asSeconds() >= 1 and !m_mate)
		{
			if(m_board.m_loaded)
			{
				m_commStockfish.clear();
				m_commStockfish = getNextMoveAfterFEN(m_engine, m_fen, m_position);
				m_evaluation = -getEvaluation(m_engine.getFinalResponse(), m_mateEvaluation);
				m_mateEvaluation *= -1; 
			}
			else
			{
				m_commStockfish.clear();
				m_commStockfish = getNextMove(m_engine, m_position);
				m_evaluation = -getEvaluation(m_engine.getFinalResponse(), m_mateEvaluation);
				m_mateEvaluation *= -1;
			}
			
			if(abs(m_mateEvaluation) == 0)
				m_mate = true;
			
			m_moveLogger.updateMove(false, m_commStockfish);
			
			sf::Vector2i rStart;
			sf::Vector2i rEnd;
			bool isCastling = false;
			
			// m_commStockfish, m_position not updated in castling() method
			sf::Vector2i kingPos = m_board.toCoords(m_commStockfish[0], m_commStockfish[1]);
			if(m_board.getPiece(kingPos.x, kingPos.y).m_type == PieceType::KING)
				isCastling = m_board.castling(m_commStockfish, m_position, rStart, rEnd);
			
			m_position += " " + m_commStockfish;
			sf::Vector2i posStart = m_board.toCoords(m_commStockfish[0], m_commStockfish[1]);
			sf::Vector2i posEnd = m_board.toCoords(m_commStockfish[2], m_commStockfish[3]);
			
			if(m_board.atBoard(posStart, posEnd))
				m_board.movePiece(posStart.x, posStart.y, posEnd.x, posEnd.y);
			else
			{
				#ifdef DEBUG
				std::cout << "[DEBUG] commStockfish = " << m_commStockfish << "\n";
				std::cout << "[DEBUG] length: " << m_commStockfish.length() << "\n";
				std::cout << "[DEBUG] Invalid move from engine!\n";
				#endif
				
				m_moveLogger.updateInvalidStatus("Invalid engine move!");
				m_sndManager.play("invalid");
			}
			
			// move of rook
			if(isCastling and m_board.atBoard(rStart, rEnd))
			{
				m_board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
				m_board.setCurrentTurn('W');
				m_board.setFullMoveNumber(m_board.getFullMoveNumber() - 1);
			}
			
			m_sndManager.play("move");
			m_engineMovePending = false;
			
			#ifdef DEBUG
			std::cout << "[DEBUG] m_board.m_fullMoveNumber = " << m_board.getFullMoveNumber() << "\n";
			std::cout << "[DEBUG] m_board.m_currentTurn = " << m_board.getCurrentTurn() << "\n";
			std::cout << "[DEBUG] m_evaluation = " << m_evaluation << "\n";
			std::cout << "[DEBUG] m_mateEvaluation = " << m_mateEvaluation << "\n";
			#endif
			
			if(abs(m_mateEvaluation) == 1)
			{
				// checkmate
				m_moveLogger.updateInvalidStatus("Checkmate!");
				
				#ifdef DEBUG
				std::cout << "[DEBUG] Checkmate!\n"; 
				#endif 
			}
		}
		
		m_scoreBar.update(m_evaluation);
	}

	void handleKeyPress(sf::Event& event)
	{
		if(event.key.code == sf::Keyboard::Escape)
			m_quit = true;
		
		else if(event.key.code == sf::Keyboard::S)
		{
			m_board.saveGame("./save_game.txt");
			m_moveLogger.showSaveLoadMessage("save game");
			m_sndManager.play("check");
		}
		
		else if(event.key.code == sf::Keyboard::L)
		{
			m_board.loadGame("./save_game.txt");
			m_moveLogger.showSaveLoadMessage("load game");
			m_board.m_loaded = true;
			m_position.clear();
			m_commPlayer.clear();
			m_commStockfish.clear();
			m_engine.sendCommand("ucinewgame");
			char sideToMove = m_board.getCurrentTurn();
			m_fen = m_board.generateFEN(sideToMove);
			
			#ifdef DEBUG
			std::cout << "Generated FEN after loading game: " << m_fen << "\n";
			#endif
			
			m_sndManager.play("check");
		}
	}

	// player move
	void handleMousePress(sf::Event& event)
	{
		if(event.mouseButton.button == sf::Mouse::Left and !m_mate)
		{
			m_engineMoveTimer.restart();
			
			sf::Vector2i pos = sf::Mouse::getPosition(m_window) - sf::Vector2i(OFFSET, OFFSET);
			int x = std::floor(pos.x / TILE_SIZE);
			int y = 7 - std::floor(pos.y / TILE_SIZE);
			
			if(x < 8 and y < 8)
			{
				if(not m_isPieceSelected)
				{
					if(m_board.isPieceAt(x, y))
					{
						m_commPlayer = m_board.toChess(x, y);
						
						m_selectedPiece = {x, y};
						m_highlighter.setSelection(x, y);
						m_moveLogger.updateInvalidStatus("");
						m_isPieceSelected = true;
					}
				}
				else
				{
					if(m_board.isValidMove(m_selectedPiece.x, m_selectedPiece.y, x, y))
					{
						m_commPlayer += m_board.toChess(x, y);
						
						m_moveLogger.updateMove(true, m_commPlayer);
						
						sf::Vector2i rStart;
						sf::Vector2i rEnd;
						bool isCastling = false;
						
						sf::Vector2i kingPos = m_board.toCoords(m_commPlayer[0], m_commPlayer[1]);
						if(m_board.getPiece(kingPos.x, kingPos.y).m_type == PieceType::KING)
							isCastling = m_board.castling(m_commPlayer, m_position, rStart, rEnd);
						
						m_position += " " + m_commPlayer;
						if(m_board.atBoard(m_selectedPiece, {x, y}))
							m_board.movePiece(m_selectedPiece.x, m_selectedPiece.y, x, y);
						
						if(isCastling and m_board.atBoard(rStart, rEnd))
						{
							m_board.movePiece(rStart.x, rStart.y, rEnd.x, rEnd.y);
							m_board.setCurrentTurn('B');
						}
						
						m_sndManager.play("move");
						
						m_commPlayer.clear();
						m_engineMovePending = true;
					}
					else
					{
						m_moveLogger.updateInvalidStatus("Invalid move!");
						m_sndManager.play("invalid");
					}
					
					m_highlighter.setDestination(x, y);
					m_highlighter.setSelectionActive(false);
					m_isPieceSelected = false;
				}
			}
		}
	}

	sf::RenderWindow& m_window;
	ChessBoard& m_board;
	Stockfish& m_engine;
	MoveLogger& m_moveLogger;
	TextFader& m_textFader;
	Highlighter& m_highlighter;
	ScoreBar& m_scoreBar;
	SoundManager& m_sndManager;
	sf::Texture& m_boardTexture;
	sf::Texture& m_figuresTexture;
	sf::Texture& m_bgTexture;

	sf::Clock m_loggerTimer{};
	sf::Clock m_engineMoveTimer{};
	bool m_engineMovePending = false;

	std::string m_position = "";
	std::string m_commPlayer = "";
	std::string m_commStockfish = "";
	std::string m_fen = "";

	sf::Vector2i m_selectedPiece{};
	bool m_isPieceSelected = false;

	bool m_mate = false;
	bool m_quit = false;
	
	int m_mateEvaluation = 50;
	float m_evaluation = 0.0f;
};
