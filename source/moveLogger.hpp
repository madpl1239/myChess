/*
 * moveLogger.hpp
 * 
 * 02-02-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "defines.hpp"


class MoveLogger
{
public:
	MoveLogger(int x, int y);
	~MoveLogger();
	
	void updateMove(bool side, const std::string& move);
	void draw(sf::RenderWindow& window);
	
private:
	int m_posX, m_posY;
	sf::Font m_font;
	std::vector<sf::Text> m_staticTexts; 
};
