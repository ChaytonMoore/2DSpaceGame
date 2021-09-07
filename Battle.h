#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "System.h"



class Weapon
{
	sf::Vector2f Location;

};

class TorpedoType: public Weapon
{
public:
	
	int Damage;
	int Range;
	int Speed;

	TorpedoType(int D, int R, int S)
	{
		Damage = D;
		Range = R;
		Speed = S;
	}





};



class KineticType: public Weapon
{
public:
	int Damage;
	int Range;
	int Speed;

	KineticType(int D,int R, int S)
	{
		Damage = D;
		Range = R;
		Speed = S;
	}

	KineticType(KineticType* in)
	{
		Damage = in->Damage;
		Range = in->Range;
		Speed = in->Speed;
	}

};

class BeamType: public Weapon
{
public:
	int Damage;
	int Range;

	BeamType(int D, int R)
	{
		Damage = D;
		Range = R;
	}

};





class BattleShip
{
public:
	
	int HullStrength;
	int ShieldStrength;
	int Speed;

	sf::Vector2f Location;
	sf::Sprite Sprite;

	ShipType* Parent;

	//so the class will contain inert weapon classes
	TorpedoType* Torpedo;
	KineticType* Kinetic;
	BeamType* Beam;

	bool Faction;


	BattleShip(ShipType* Parent, std::vector<sf::Texture*> Textures,bool FactionIn) //TODO make the textures thing really work, maybe a dictionary
	{

		std::string temp;
		std::vector<std::string> temp2;

		std::ifstream file("C:/Users/User/Documents/2DSpaceGame/Assets/Battle/" + Parent->Name+ ".txt");

		
		
		file >> temp;
		
		temp2 = System::SplitString(temp,',');
		
		HullStrength = std::stoi(temp2[0]); //Later might make it so that the hull is from the instance
		ShieldStrength = std::stoi(temp2[1]);
		Speed = std::stoi(temp2[2]);

		file >> temp;
		temp2 = System::SplitString(temp, ',');
		if (temp[0]!=char("0")) //If it has a torpedo
		{
			Torpedo = new TorpedoType(std::stoi(temp2[0]), std::stoi(temp2[1]), std::stoi(temp2[2]));
		}

		file >> temp;
		temp2 = System::SplitString(temp, ',');
		if (temp[0] != char("0")) //If it has a kinetic
		{
			Kinetic = new KineticType(std::stoi(temp2[0]), std::stoi(temp2[1]), std::stoi(temp2[2]));
		}

		file >> temp;
		temp2 = System::SplitString(temp, ',');
		if (temp[0] != char("0")) //If it has a beam
		{
			Beam = new BeamType(std::stoi(temp2[0]), std::stoi(temp2[1]));
		}


		Sprite.setTexture(*Textures.at(0));
		Faction = FactionIn;


		//Set sprite
		Sprite.setScale(pow((float)Parent->Length/300,0.5), pow((float)Parent->Length / 300, 0.5));
		
	}
	
	void EventTick(std::vector<BattleShip*> Enemyships);

	int KineticTickDelay = 0;
	int TorpedoTickDelay = 0;
	int BeamTickDelay = 0;

	std::vector<Weapon*> WeaponElements;

};

struct BattleReturn
{
	bool Winner;
	std::vector<ShipType*> WiningShips;
	std::vector<ShipType*> LosingShips;
};


BattleReturn Battle(Faction* Faction1, Faction* Faction2, std::vector<ShipType*> Faction1Ships, std::vector<ShipType*> Faction2Ships);
