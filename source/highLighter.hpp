/*
 * highLighter.hpp
 * 
 * 04-02-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>


class Highlighter
{
public:
    Highlighter();
    
	void setSelection(int x, int y);
	void setSelectionActive(bool _switch);
    void setDestination(int x, int y);
	void clear();
	
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_selection;
    sf::RectangleShape m_destination;
	bool m_selectionActive;
    bool m_destinationActive;
};
