#pragma once
#include "planet.h"
#include "UIElement.h"
#include "ShipBuildButton.h"
#include "TextureLoader.h"
#include <FileSystem>




struct ShipConstructData
{
	ShipType* Type;
	float Remaining;
	Button* UIElement;
	std::string OverWriteName;

};



enum SelectEnum //Might want to move this even further up
{
	Basic, Fleet
};







//Might want to move these two things to shipbuildbutton.h

struct FleetStruct;




class System : public UIElement
{
	double CurrentTimeStamp = 0;
	int LastTime = 0;

public:
	std::vector<Planet*> Planets;
	sf::Vector2i Location;
	std::string Name;
	std::vector<ShipConstructData> ShipBuildQueue;
	std::vector<ShipInstance*> SystemShips;

	sf::Sprite Sprite;
	sf::Sprite OwnedSprite;
	sf::Sprite AllySprite;
	sf::Sprite EnemySprite;
	sf::Sprite NeutralSprite;
	sf::Sprite* CurrentSprite;
	sf::Sprite OwnerEmblem;

	sf::Text SystemName;
	sf::Font TextFont;

	Faction* OwningFaction;
	std::vector<sf::Sprite*> FleetsEmblems;
	

	System(sf::Vector2i loc, std::string name)
	{
		LoadImageSF(&Sprite,"SystemIcon.png");
		LoadImageSF(&OwnedSprite, "SystemIconOwned.png");
		LoadImageSF(&NeutralSprite, "SystemIconNeutral.png");
		Location = loc;
		Name = name;
		

		CurrentSprite = &Sprite;
		Sprite.setOrigin(Sprite.getPosition() + sf::Vector2f(32, 32));
		OwnedSprite.setOrigin(OwnedSprite.getPosition() + sf::Vector2f(32, 32));
		NeutralSprite.setOrigin(NeutralSprite.getPosition() + sf::Vector2f(32, 32));
	}

	void RenderSystem(sf::RenderWindow* window, sf::Vector2f CameraOffset, TextureStore* TStore, Faction* PlayerFaction, float Zoom);



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



	virtual void EventTick(double DeltaTime, std::vector<FleetStruct*>* FreeFleets, std::vector<System*> systems);


	void RemoveShipInstance(ShipInstance* inst)
	{
		for (size_t i = 0; i < SystemShips.size(); i++)
		{
			if (SystemShips.at(i)==inst)
			{
				//remove
				SystemShips.erase(SystemShips.begin()+i);
				break;
			}
		}
	}


	void IntraSystemMigration();


	std::string GetName();


	uint64_t GetPopulation();
	
	int GetShipyardPower();


	void LoadUpFreighter(ShipInstance* ship, System* Target);


	void FreighterDepart(ShipInstance* ship, std::vector<System*> Systems, std::vector<FleetStruct*>* FreeFleets);

	void FreighterArrive(ShipInstance* ship);

	void TerraformingTick();

	int GetBestObservatory();


	float DistanceTo(System* input);

	int Loyalty;

};

struct FleetStruct
{
	std::vector<ShipInstance*> Ships;
	sf::Vector2f Location; //TODO change to a float vector
	System* CurrentSystem;
	System* TargetSystem;

	sf::Sprite FleetEmblem;
	sf::Texture Texture;
	sf::Image Image;
	
	
	void RenderFleet(sf::RenderWindow* window, sf::Vector2f CameraLocation, float CameraZoom)
	{
		sf::Vector2f local = Location - CameraLocation;
		local = sf::Vector2f(local.x*CameraZoom,local.y*CameraZoom);
		FleetEmblem.setPosition(local);
		window->draw(FleetEmblem);
	}


	float Distance(sf::Vector2f a, sf::Vector2f b)
	{
		float out;
		a = a - b;
		a.x = abs(a.x);
		a.y = abs(a.y);
		out = sqrt(a.x * a.x + a.y * a.y);

		return out;
	}



	void EventTick(double DeltaTime)
	{
		//first we need to get the speed of the fleet which is the speed of the slowest ship
		//TODO evaluate speed elsewhere

		int speed = Ships.at(0)->Type->Speed;
		for (size_t i = 0; i < Ships.size(); i++)
		{
			if (speed>Ships.at(i)->Type->Speed)
			{
				speed = Ships.at(i)->Type->Speed;
			}
		}

		sf::Vector2f Direction;
		float x = (sf::Vector2f(TargetSystem->Location) - Location).x;
		float y = (sf::Vector2f(TargetSystem->Location) - Location).y;
		Direction = sf::Vector2f(x/(sqrt(x*x + y*y)),y/(sqrt(x*x + y*y)));
		Location = Location + sf::Vector2f(Direction.x * speed*DeltaTime, Direction.y * speed*DeltaTime);

		


	}


	FleetStruct()
	{

		Image.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Emblem/HumanEmblemFleet.png");
		Texture.loadFromImage(Image);
		FleetEmblem.setTexture(Texture);
		
	}

	
};


std::vector<System*> LoadSystems(std::vector<Faction*> Factions);



float SystemDistance(sf::Vector2f a, sf::Vector2f b);

//TODO organise header and content files so there is less stuff in header





