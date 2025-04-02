/*
 * textFader.hpp
 * 
 * 30-03-2025 by madpl
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "defines.hpp"

using time_point_t = std::chrono::steady_clock::time_point;
using Clock = std::chrono::steady_clock;


class TextFader
{
public:
	TextFader(float displayTime, float fadeTime);
	~TextFader();

	void showMessage(const std::string& message, sf::Vector2f position,
					 sf::Color col = sf::Color::White, int size = 24,
					 sf::Uint32 style = sf::Text::Regular);

	void update();
	void draw(sf::RenderWindow& window);

private:
	struct Message
	{
		sf::Text text;
		sf::Color col;
		time_point_t startTime;
		bool isActive;
	};

	sf::Font m_font;
	std::vector<Message> m_messages;
	float m_displayTime;
	float m_fadeTime;
};
