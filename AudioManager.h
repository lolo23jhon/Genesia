#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SFML/Audio/SoundBuffer.hpp>
#include "ResourceManager.h"

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
	////////////////////////////////////////////////////////////
	AudioManager() : ResourceManager("audio.cfg") {}

	////////////////////////////////////////////////////////////
	bool load(sf::SoundBuffer* t_resource, const std::string& t_path) {
		return t_resource->loadFromFile(utilities::getWorkingDirectory() + t_path);
	}
};

#endif // !AUDIO_MANAGER_H