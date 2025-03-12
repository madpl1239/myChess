/*
 * highLighter.cpp
 * 
 * 04-02-2025 by madpl
 */
#include <iostream>
#include "defines.hpp"
#include "highLighter.hpp"


Highlighter::Highlighter():
	m_selectionActive(false),
	m_destinationActive(false)
{
	m_selection.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	m_selection.setFillColor(sf::Color(255, 255, 0, 40));

	m_destination.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	m_destination.setFillColor(sf::Color(0, 255, 0, 40));
}


void Highlighter::setSelection(int x, int y)
{
	m_selection.setPosition(x * TILE_SIZE + OFFSET, (7 - y) * TILE_SIZE + OFFSET);
	m_selectionActive = true;
}


void Highlighter::setSelectionActive(bool _switch)
{
	m_selectionActive = _switch;
}


void Highlighter::setDestination(int x, int y)
{
	m_destination.setPosition(x * TILE_SIZE + OFFSET, (7 - y) * TILE_SIZE + OFFSET);
	m_destinationActive = true;
}


void Highlighter::clear()
{
    m_selectionActive = false;
    m_destinationActive = false;
}


void Highlighter::draw(sf::RenderWindow& window)
{
	if(m_selectionActive)
		window.draw(m_selection);

	if(m_destinationActive)
		window.draw(m_destination);
}
