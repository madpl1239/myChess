/*
 * sndManager.hpp
 * 
 * Simply sound manager for my Chess GUI.
 * 
 * 04-04-2025 by madpl
 */
#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <vector>


class SoundManager
{
public:
	SoundManager(){}
	
	~SoundManager()
	{
		if(not m_sounds.empty())
			m_sounds.clear();
		
		if(not m_activeSounds.empty())
			m_activeSounds.clear();
	}

	bool load(const std::string& name, const std::string& filePath)
	{
		auto soundBuffer = std::make_shared<sf::SoundBuffer>();
		
		if(not soundBuffer->loadFromFile(filePath))
		{
			std::cerr << "Error: Could not load audio from file " << filePath << "\n";
			
			return false;
		}
		
		m_sounds[name] = soundBuffer;
		
		return true;
	}

	void play(const std::string& name)
	{
		if(m_sounds.find(name) != m_sounds.end())
		{
			auto sound = std::make_shared<sf::Sound>();
			sound->setBuffer(*m_sounds[name]);
			sound->play();
			m_activeSounds.push_back(sound);
		}
		else
			std::cerr << "error: sound named " << name << " was not found.\n";
	}

	void update()
	{
		m_activeSounds.erase(std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
			[](const std::shared_ptr<sf::Sound>& sound)
			{
				return sound->getStatus() == sf::Sound::Stopped;
			}),
			m_activeSounds.end());
	}
	
	void update2()
	{
		static std::unordered_map<std::shared_ptr<sf::Sound>, sf::Clock> soundTimers;
		static const sf::Time MIN_DURATION = sf::milliseconds(100);
		
		auto now = sf::Clock();
		
		m_activeSounds.erase(std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
			[&](const std::shared_ptr<sf::Sound>& sound)
			{
				if(soundTimers.find(sound) == soundTimers.end())
					soundTimers[sound] = now; // we remember the sound start time
				
				sf::Time elapsed = now.getElapsedTime() - soundTimers[sound].getElapsedTime();
				
				// we only remove the sound if it has stopped 
				// and lasted longer than MIN_DURATION
				return (sound->getStatus() == sf::Sound::Stopped and elapsed > MIN_DURATION);
			}),
			m_activeSounds.end());
	}

private:
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> m_sounds{};
	std::vector<std::shared_ptr<sf::Sound>> m_activeSounds{};
};
