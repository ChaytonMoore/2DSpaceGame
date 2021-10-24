#pragma once
#include "System.h"
#include "FactionData.h"
#include <fstream>

struct SystemProximity
{
	System* main;
	std::vector<System*> nearby;


};


class Border
{
public:
	std::vector<SystemProximity> Proximities;

	void LoadProximityData(std::vector<System*> systems);
	
	
	sf::RenderTexture tex;
	sf::Sprite spr;

	Border(std::vector<System*> systems, sf::Vector2i ScreenSize);

	void Update(std::vector<FactionData*> factions, std::vector<System*> systems);


};
