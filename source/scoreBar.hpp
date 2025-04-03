/*
 * scoreBar.hpp
 * 
 * 18-03-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.hpp"


class ScoreBar
{
public:
	explicit ScoreBar(float boardHeight, float xPosition);
	~ScoreBar();
	
	void update(float evaluation);
	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape m_topBar;
	sf::RectangleShape m_bottomBar;
	float m_boardHeight;
	const int m_width = 24;
};
