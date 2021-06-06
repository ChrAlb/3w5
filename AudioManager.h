#pragma once
#include "ResourceManager.h"
#include <SFML/Audio/SoundBuffer.hpp>

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer>{
public:
	AudioManager() : ResourceManager("audio.cfg.txt"){}

	sf::SoundBuffer* Load(const std::string& l_path){
		sf::SoundBuffer* sound = new sf::SoundBuffer();
		if (!sound->loadFromFile(l_path))
		{
			delete sound;
			sound = nullptr;
			std::cerr << "! Failed to load sound: " << l_path << std::endl;
		}
		return sound;
	}
};