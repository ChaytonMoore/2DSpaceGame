#include <iostream>
#include <SFML/Audio.hpp>
#include <fstream>
#pragma once
class MusicPlayer
{
	std::vector<sf::SoundBuffer*> buffers;
	sf::Sound* Music;

	int Current = -1;

	void LoadMusic();
public:
	MusicPlayer()
	{
		LoadMusic();
	}

	void Tick();
};

