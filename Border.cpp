#include "Border.h"

void Border::LoadProximityData(std::vector<System*> systems)
{
	//firstly load the data from the .nrst file
	std::vector<uint16_t> data;
	std::ifstream nrst("C:/Users/User/Documents/2DSpaceGame/Assets/proximity.nrst", std::ios::in | std::ios::binary);
	int Quan;
	nrst.read((char*)&Quan,sizeof(Quan));
	for (size_t i = 0; i < Quan; i++)
	{
		data.clear();
		while (true)
		{
			
			data.push_back(0);
			nrst.read((char*)&data.back(),sizeof(data.back()));
			
			if (data.back() == 65535)
			{
				break;
			}


		}
		//remove the last data element
		data.pop_back();


		Proximities.push_back(*new SystemProximity);
		Proximities.back().main = systems.at(i);
		for (size_t j = 0; j < data.size(); j++)
		{
			Proximities.back().nearby.push_back(systems.at(data.at(j)));
		}



	}



}

Border::Border(std::vector<System*> systems, sf::Vector2i ScreenSize)
{
	LoadProximityData(systems);
	spr.setTexture(tex.getTexture());
	tex.create(ScreenSize.x,ScreenSize.y);


	
}

void Border::Update(std::vector<FactionData*> factions, std::vector<System*> systems)
{
	//first of we'll be drawing a cirle at the centre of screen
	tex.clear(sf::Color(10,10,10));
	sf::CircleShape a(100);
	a.setPosition(1000,500);
	a.setFillColor(sf::Color::Green);
	tex.draw(a);
	
	spr.setTexture(tex.getTexture());

	
}