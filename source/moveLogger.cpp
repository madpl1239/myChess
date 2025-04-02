/*
 * moveLogger.cpp
 * 
 * 02-02-2025 by madpl
 */
#include <iostream>
#include "moveLogger.hpp"


MoveLogger::MoveLogger(int x, int y):
	m_posX(x),
	m_posY(y)
{
	if(not m_font.loadFromFile(ARIAL))
		std::cout << "Error loading font!\n";

	sf::Text title;
	title.setFont(m_font);
	title.setString("myChess v0.5.0 - chess GUI by madpl (2025)");
	title.setCharacterSize(TEXTLOGO_HEIGHT);
	title.setFillColor(sf::Color::Black);
	// title.setStyle(sf::Text::Italic);
	title.setPosition(m_posX + TEXTON_RIGHT, m_posY);
	m_staticTexts.push_back(title);

	sf::Text player;
	player.setFont(m_font);
	player.setString("Player   : ");
	player.setCharacterSize(TEXTMOVE_HEIGHT);
	player.setFillColor(sf::Color(20, 28, 102, 255));
	player.setStyle(sf::Text::Bold);
	player.setPosition(m_posX + TEXTON_RIGHT, m_posY + 60);
	m_staticTexts.push_back(player);

	sf::Text engine;
	engine.setFont(m_font);
	engine.setString("Engine  : ");
	engine.setCharacterSize(TEXTMOVE_HEIGHT);
	engine.setFillColor(sf::Color(20, 28, 102, 255));
	engine.setStyle(sf::Text::Bold);
	engine.setPosition(m_posX + TEXTON_RIGHT, m_posY + 86);
	m_staticTexts.push_back(engine);
	
	#ifdef DEBUG
	std::cout  << "[DEBUG] ctor MoveLogger\n";
	#endif
}


MoveLogger::~MoveLogger()
{
	m_posX = 0;
	m_posY = 0;
	
	if(not m_staticTexts.empty())
		m_staticTexts.clear();
	
	#ifdef DEBUG
	std::cout  << "[DEBUG] dtor MoveLogger\n";
	#endif
}


void MoveLogger::updateMove(bool side, const std::string& move)
{
	if(side)
		m_staticTexts[1].setString("Player   : " + move);
	else
		m_staticTexts[2].setString("Engine  : " + move);
}


void MoveLogger::draw(sf::RenderWindow& window)
{
	for(const auto& text : m_staticTexts)
		window.draw(text);
}
