/*
 * textFader.cpp
 * 
 * 30-03-2025 by madpl
 */
#include "textFader.hpp"


TextFader::TextFader(float displayTime, float fadeTime)
{
	if(fadeTime > displayTime)
		fadeTime = displayTime;
	
	this->m_displayTime = displayTime;
	this->m_fadeTime = fadeTime;
	
	m_font.loadFromFile("./resources/arial.ttf");
	
	#ifdef DEBUG
	std::cout << "[DEBUG] ctor TextFader\n";
	#endif
}


TextFader::~TextFader()
{
	m_displayTime = 0.0f;
	m_fadeTime = 0.0f;
	
	if(not m_messages.empty())
		m_messages.clear();
	
	#ifdef DEBUG
	std::cout << "[DEBUG] dtor TextFader\n";
	#endif
}


void TextFader::showMessage(const std::string& message, sf::Vector2f position,
							sf::Color color, int size, sf::Uint32 style)
{
	Message newMessage;
	
	newMessage.text.setFont(m_font);
	newMessage.text.setCharacterSize(size);
	newMessage.text.setFillColor(color);
	newMessage.text.setString(message);
	newMessage.text.setPosition(position);
	newMessage.text.setStyle(style);
	newMessage.col = color;
	newMessage.isActive = true;
	newMessage.startTime = Clock::now();
	
	m_messages.push_back(newMessage);
}


void TextFader::update()
{
	auto now = Clock::now();
	
	for(auto& msg : m_messages)
	{
		if(not msg.isActive)
			continue;
		
		float elapsed = std::chrono::duration<float>(now - msg.startTime).count();
		
		if(elapsed >= m_displayTime)
			msg.isActive = false;
		
		// elapsed wchodzi w faze m_fadeTime
		else if(elapsed >= (m_displayTime - m_fadeTime))
		{
			float alpha = 255 * (1 - (elapsed - (m_displayTime - m_fadeTime)) / m_fadeTime);
			msg.text.setFillColor(sf::Color(msg.col.r, msg.col.g, msg.col.b, static_cast<sf::Uint8>(alpha)));
		}
	}

	// usuwa nieaktywne komunikaty
	m_messages.erase(std::remove_if(m_messages.begin(), m_messages.end(),
									[](const Message& msg){return !msg.isActive;}),
						m_messages.end());
}


void TextFader::draw(sf::RenderWindow& window)
{
	for(const auto& msg : m_messages)
	{
		if(msg.isActive)
			window.draw(msg.text);
	}
}
