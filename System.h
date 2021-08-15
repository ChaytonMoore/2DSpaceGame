#pragma once
#include "planet.h"
#include "UIElement.h"
#include "ShipBuildButton.h"
#include "TextureLoader.h"

struct ShipConstructData
{
	ShipType* Type;
	float Remaining;
	Button* UIElement;

};


struct FleetStruct
{
	std::vector<ShipInstance*> Ships;
	sf::Vector2i Location;
	System* CurrentSystem;
	System* TargetSystem;

};


struct ShipInstance
{
	ShipType* Type;
	Faction* OwningFaction;
	int RemainingRange;
	int HullStrength;

	ShipInstance(ShipType* SType, Faction* Fac)
	{
		Type = SType;
		RemainingRange = Type->Range;
		HullStrength = Type->Hull;
		OwningFaction = Fac;

	}
};
//Might want to move these two things to shipbuildbutton.h


class System : public UIElement
{
public:
	std::vector<Planet*> Planets;
	sf::Vector2i Location;
	std::string Name;
	std::vector<ShipConstructData> ShipBuildQueue;
	std::vector<ShipInstance*> SystemShips;

	sf::Sprite Sprite;
	sf::Text SystemName;
	sf::Font TextFont;

	Faction* OwningFaction;
	std::vector<sf::Sprite*> FleetsEmblems;
	

	System(sf::Vector2i loc, std::string name)
	{
		LoadImage(&Sprite,"SystemIcon.png");
		Location = loc;
		Name = name;
		
	}

	void RenderSystem(sf::RenderWindow* window, sf::Vector2i CameraOffset, TextureStore* TStore);



	virtual int ClickedOn();


	bool LoadSystem(std::string Name);

	void AddUIPlanets(std::vector<UIElement*>* aspects)
	{
		for (size_t i = 0; i < Planets.size(); i++)
		{
			aspects->push_back(Planets.at(i));
		}
	}

	static std::vector<std::string> SplitString(std::string s, char delimiter);


	int SimpleMetal = 0;
	int Titanium = 0;
	int RareMetal = 0;



	virtual void EventTick(double DeltaTime, std::vector<FleetStruct*> FreeFleets);

};



std::vector<System*> LoadSystems(Faction* PlayerFaction);
