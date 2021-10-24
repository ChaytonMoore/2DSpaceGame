#include "MusicPlayer.h"

void MusicPlayer::LoadMusic()
{
	//firstly load the index.txt
	std::string text;
	std::ifstream file("Music/index.txt");

	while (std::getline(file,text))
	{
		buffers.push_back(new sf::SoundBuffer);
		buffers.back()->loadFromFile("Music/"+text);
	}
	file.close();


}

void MusicPlayer::Tick()
{
	if (!Music->Playing)
	{
		Current++;
		if (Current>buffers.size()-1)
		{
			Current = 0;
		}//there is a better way to do this
		Music->setBuffer(*buffers.at(Current));
		Music->play();

	}


}