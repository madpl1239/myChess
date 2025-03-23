/*
 * scoreBar.cpp
 * 
 * 18-03-2025 by madpl
 */
#include "scoreBar.hpp"
#include <algorithm>


ScoreBar::ScoreBar(float boardHeight, float xPosition):
	m_boardHeight(boardHeight)
{
	m_topBar.setSize(sf::Vector2f(m_width, boardHeight / 2));
	m_topBar.setFillColor(sf::Color(255, 0, 0, 80));
	m_topBar.setPosition(xPosition, 0);
	
	m_bottomBar.setSize(sf::Vector2f(m_width, boardHeight / 2));
	m_bottomBar.setFillColor(sf::Color(0, 255, 0, 80));
	m_bottomBar.setPosition(xPosition, boardHeight / 2);
	
	#ifdef DEBUG
	std::cout << "[DEBUG] ctor ScoreBar\n";
	#endif
}


ScoreBar::~ScoreBar()
{
	m_boardHeight = 0;
	m_topBar = sf::RectangleShape{};
	m_bottomBar = sf::RectangleShape{};
	
	#ifdef DEBUG
	std::cout << "[DEBUG] dtor ScoreBar\n";
	#endif
}


void ScoreBar::update(float evaluation)
{
	float evalClamped = std::clamp(evaluation, -5.0f, 5.0f);
	float ratio = (evalClamped + 5.0f) / 10.0f;
	float topHeight = (1.0f - ratio) * m_boardHeight;
	float bottomHeight = ratio * m_boardHeight;
	
	m_topBar.setSize(sf::Vector2f(m_width, topHeight));
	m_bottomBar.setSize(sf::Vector2f(m_width, bottomHeight));
	m_bottomBar.setPosition(m_topBar.getPosition().x, topHeight);
}


void ScoreBar::draw(sf::RenderWindow& window)
{
	window.draw(m_topBar);
	window.draw(m_bottomBar);
}
