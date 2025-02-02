/*
 * moveLogger.hpp
 * 
 * 02-02-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


class MoveLogger
{
public:
	MoveLogger(float x, float y, float width, float height);
	
	void addMove(const std::string& move);
	void draw(sf::RenderWindow& window);
	
private:
	sf::RectangleShape m_background;
	std::vector<sf::Text> m_moveTexts;
	sf::Font m_font;
	float m_posX, m_posY;
};
