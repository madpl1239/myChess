/*
 * moveLogger.cpp
 * 
 * 02-02-2025 by madpl
 */
#include "moveLogger.hpp"
#include <iostream>


MoveLogger::MoveLogger(float x, float y, float width, float height):
	m_posX(x),
	m_posY(y)
{
	m_background.setSize(sf::Vector2f(width, height));
	m_background.setPosition(x, y);
	m_background.setFillColor(sf::Color(200, 200, 200));

	if(not m_font.loadFromFile("./resources/Vera.ttf"))
		std::cout << "Error loading font!\n";
}


void MoveLogger::addMove(const std::string& move)
{
	sf::Text text;
	
	text.setFont(m_font);
	text.setString(move);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setPosition(m_posX + 10, m_posY + 10 + m_moveTexts.size() * 25);
	
	m_moveTexts.push_back(text);
}


void MoveLogger::draw(sf::RenderWindow& window)
{
	window.draw(m_background);
	
	for(const auto& text : m_moveTexts)
		window.draw(text);
}
