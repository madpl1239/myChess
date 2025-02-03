/*
 * moveLogger.cpp
 * 
 * 02-02-2025 by madpl
 */
#include <iostream>
#include "moveLogger.hpp"


MoveLogger::MoveLogger(float x, float y, float width, float height):
	m_posX(x),
	m_posY(y)
{
	if(not m_font.loadFromFile("./resources/Vera.ttf"))
		std::cout << "Error loading font!\n";

	// Inicjalizacja napisów statycznych
	sf::Text title;
	title.setFont(m_font);
	title.setString("myChess v1.0 - GUI for chess by madpl (2025)");
	title.setCharacterSize(TEXTLOGO_HEIGHT);
	title.setFillColor(sf::Color(0, 100, 100));
	title.setStyle(sf::Text::Bold);
	title.setPosition(m_posX + 10, m_posY + 10);
	
	m_staticTexts.push_back(title);

	sf::Text player;
	player.setFont(m_font);
	player.setString("Player   : ");
	player.setCharacterSize(TEXTMOVE_HEIGHT);
	player.setFillColor(sf::Color::Black);
	player.setPosition(m_posX + 10, m_posY + 60);
	
	m_staticTexts.push_back(player);

	sf::Text engine;
	engine.setFont(m_font);
	engine.setString("Engine  : ");
	engine.setCharacterSize(TEXTMOVE_HEIGHT);
	engine.setFillColor(sf::Color::Black);
	engine.setPosition(m_posX + 10, m_posY + 90);
	m_staticTexts.push_back(engine);

	sf::Text checkInfo;
	checkInfo.setFont(m_font);
	checkInfo.setString("");
	checkInfo.setCharacterSize(TEXTCHECK_HEIGHT);
	checkInfo.setFillColor(sf::Color::Red);
	checkInfo.setPosition(m_posX + 10, m_posY + 130);
	m_checkText = checkInfo;
}


void MoveLogger::updateMove(bool side, const std::string& move)
{
	if(side)
		m_staticTexts[1].setString("Player   : " + move);
	else
		m_staticTexts[2].setString("Engine  : " + move);
}


void MoveLogger::updateCheckStatus(const std::string& checkMessage)
{
	m_checkText.setString(checkMessage);
}


void MoveLogger::draw(sf::RenderWindow& window)
{
	for(const auto& text : m_staticTexts)
		window.draw(text);
	
	window.draw(m_checkText);
}
