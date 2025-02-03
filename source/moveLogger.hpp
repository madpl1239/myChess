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
	MoveLogger(float x, float y, float width, float height);
	
	void updateMove(bool side, const std::string& move);
	void updateCheckStatus(const std::string& checkMessage);
	void applyEngineDelay();
	
	void draw(sf::RenderWindow& window);
	
private:
	float m_posX, m_posY;
	sf::Font m_font;
	std::vector<sf::Text> m_staticTexts; 
	sf::Text m_checkText;
};
