#pragma once
#include "Button.h"
//#include "System.h"

//^ don't strickly need to include can be done elsewhere



struct ShipType
{
	std::string Name;
	int Length;
	int Attack;
	int Hull;
	int Shields;
	int Speed; // measured in c
	int Range; //measured in lightyears


	//Requirments
	int ShipTech;
	int WeaponTech;
	int ShieldTech;
	int EngineTech;
	int Sensortech;

	//std::vector<PlanetClasses> Colonisable;
	int Crew;
	float ConstructTime;


	int RequiredMetal;
	int RequiredTitanium;
	int RequiredRare;


};




class ShipBuildButton :
	public Button
{
public:


	ShipType* ShipToBuild;

	virtual void Render(sf::RenderWindow* window, float Scroll)
	{

		rect.setPosition(LocalPosition - sf::Vector2f(0,Scroll));
		Text.setPosition(LocalPosition - sf::Vector2f(0, Scroll));

		window->draw(rect); 
		window->draw(Text);


		
	}

	ShipBuildButton(sf::Vector2i Position, std::string TextString, ShipType* varient) : Button(sf::Vector2i(0,0), "Ship")
	{
		
		rect.setPosition(Position.x, Position.y);
		rect.setSize(sf::Vector2f(180, 35));
		rect.setOutlineColor(sf::Color(74, 89, 57));
		rect.setFillColor(sf::Color(37, 37, 38));
		rect.setOutlineThickness(2);



		Text = *new sf::Text();
		Text.setString(TextString);
		Text.setFont(TextFont);
		Text.setCharacterSize(12);

		ShipToBuild = varient;


		LocalPosition = sf::Vector2f(Position.x, Position.y);
	}

	
	virtual int ClickedOn();

	virtual bool IsClicked(sf::Vector2i MouseLoc, float Scroll);


};





class Faction
{
public:
	//Each faction has a load of tech levels
	int MedicalTech = 1;
	int MiningTech = 1;
	int FarmingTech = 1;
	int ConstructionTech = 1;
	int ShipbuildingTech = 1;
	int WeaponsTech = 1;
	int ShieldTech = 0;
	int EngineTech = 1;
	int SensorTech = 1;

	std::vector<ShipType*> FactionShips;
	std::string FactionName;
	Faction(std::string Name)
	{
		FactionName = Name;
	}


	void LoadShips();
};



