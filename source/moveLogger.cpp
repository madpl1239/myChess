/*
 * moveLogger.cpp
 * 
 * 02-02-2025 by madpl
 */
#include <iostream>
#include "moveLogger.hpp"


MoveLogger::MoveLogger(int x, int y):
	m_posX(x),
	m_posY(y),
	m_saveLoadVisible(false),
	m_saveLoadAlpha(255)
{
	if(not m_font.loadFromFile(ARIAL))
		std::cout << "Error loading font!\n";

	sf::Text title;
	title.setFont(m_font);
	title.setString("myChess v1.0 - GUI for chess by madpl (2025)");
	title.setCharacterSize(TEXTLOGO_HEIGHT);
	title.setFillColor(sf::Color::Black);
	// title.setStyle(sf::Text::Italic);
	title.setPosition(m_posX + TEXTON_RIGHT, m_posY + 10);
	m_staticTexts.push_back(title);

	sf::Text player;
	player.setFont(m_font);
	player.setString("Player   : ");
	player.setCharacterSize(TEXTMOVE_HEIGHT);
	player.setFillColor(sf::Color(20, 28, 102, 255));
	player.setStyle(sf::Text::Bold);
	player.setPosition(m_posX + TEXTON_RIGHT, m_posY + 60);
	m_staticTexts.push_back(player);

	sf::Text engine;
	engine.setFont(m_font);
	engine.setString("Engine  : ");
	engine.setCharacterSize(TEXTMOVE_HEIGHT);
	engine.setFillColor(sf::Color(20, 28, 102, 255));
	engine.setStyle(sf::Text::Bold);
	engine.setPosition(m_posX + TEXTON_RIGHT, m_posY + 86);
	m_staticTexts.push_back(engine);

	sf::Text checkInfo;
	checkInfo.setFont(m_font);
	checkInfo.setString("");
	checkInfo.setCharacterSize(TEXTCHECK_HEIGHT);
	checkInfo.setFillColor(sf::Color::Red);
	checkInfo.setStyle(sf::Text::Italic | sf::Text::Bold);
	checkInfo.setPosition(m_posX + TEXTON_RIGHT, m_posY + 430);
	m_checkText = checkInfo;
	
	sf::Text invalidInfo;
	invalidInfo.setFont(m_font);
	invalidInfo.setString("");
	invalidInfo.setCharacterSize(TEXTCHECK_HEIGHT);
	invalidInfo.setFillColor(sf::Color::Red);
	invalidInfo.setStyle(sf::Text::Italic | sf::Text::Bold);
	invalidInfo.setPosition(m_posX + TEXTON_RIGHT, m_posY + 430);
	m_invalidText = invalidInfo;
	
	sf::Text saveLoadText;
	saveLoadText.setFont(m_font);
	saveLoadText.setString("save game");
	saveLoadText.setCharacterSize(TEXTCHECK_HEIGHT);
	saveLoadText.setFillColor(sf::Color(0, 0, 0, m_saveLoadAlpha));
	saveLoadText.setStyle(sf::Text::Italic);
	saveLoadText.setPosition(m_posX + TEXTON_RIGHT, m_posY + 430);
	m_saveLoadText = saveLoadText;
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
	m_invalidText.setString("");
	m_checkText.setString(checkMessage);
}


void MoveLogger::updateInvalidStatus(const std::string& checkMessage)
{
	m_checkText.setString("");
	m_invalidText.setString(checkMessage);
}


void MoveLogger::showSaveLoadMessage(const std::string& message)
{
	m_saveLoadText.setString(message);
	m_saveLoadVisible = true;
	m_saveLoadClock.restart();
	m_saveLoadAlpha = 255;
	m_saveLoadText.setFillColor(sf::Color(0x7F, 0xFF, 0xD4, m_saveLoadAlpha));
}


void MoveLogger::updateSaveLoad()
{
	if(m_saveLoadVisible)
	{
		float elapsed = m_saveLoadClock.getElapsedTime().asSeconds();
		if(elapsed >= 4.0f)
			m_saveLoadVisible = false;
		
		else if(elapsed >= 3.0f)
		{
			m_saveLoadAlpha = static_cast<sf::Uint8>(255 * (4.0f - elapsed));
			m_saveLoadText.setFillColor(sf::Color(0x7F, 0xFF, 0xD4, m_saveLoadAlpha));
		}
	}
}


void MoveLogger::draw(sf::RenderWindow& window)
{
	for(const auto& text : m_staticTexts)
		window.draw(text);

	window.draw(m_checkText);
	window.draw(m_invalidText);

	if(m_saveLoadVisible)
		window.draw(m_saveLoadText);
}
