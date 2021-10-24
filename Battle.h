#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "System.h"



class Weapon
{
public:
	sf::Vector2f Location;
	int Range;
	int Damage;
	int Speed;
	Weapon(int D, int R, int S)
	{

		Damage = D;
		Range = R;
		Speed = S;
	}
	
};



struct BattleReturn
{
	bool Winner;
	std::vector<ShipInstance*> Ships1;
	std::vector<ShipInstance*> Ships2;
};


struct BattleSounds
{


	BattleSounds()
	{

		RailgunB.loadFromFile("railgun.wav");
		LaserB.loadFromFile("laser.wav");
		
	}

	sf::SoundBuffer RailgunB;
	sf::SoundBuffer LaserB;
	sf::SoundBuffer TorpedoB;
	sf::SoundBuffer ExplosionB;


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

	BattleShip* Target;

	//so the class will contain inert weapon classes


	bool FactionI;
	sf::Sound* Railgun = new sf::Sound;
	sf::Sound* Laser = new sf::Sound;
	sf::Sound* Torpedo = new sf::Sound;
	sf::Sound* Explosion = new sf::Sound;

	BattleShip(ShipType* ParentL, sf::Texture* Texture, bool FactionIn, BattleSounds* Sounds) //TODO make the textures thing really work, maybe a dictionary
	{
		//load sounds
		Railgun->setBuffer(Sounds->RailgunB);
		Laser->setBuffer(Sounds->LaserB);


		std::string temp;
		std::vector<std::string> temp2;

		std::ifstream file("C:/Users/User/Documents/2DSpaceGame/Assets/Battle/" + ParentL->Name+ ".txt");

		Parent = ParentL;
		
		file >> temp;
		std::cout << ParentL->Name;
		temp2 = System::SplitString(temp,',');
		
		HullStrength = std::stoi(temp2[0]); //Later might make it so that the hull is from the instance
		ShieldStrength = std::stoi(temp2[1]);
		Speed = std::stoi(temp2[2]);

		file >> temp;
		temp2 = System::SplitString(temp, ',');
		Weapon* Torpedo;
		Weapon* Kinetic;
		Weapon* Beam;
		if (temp[0]!=char("0")) //If it has a torpedo
		{
			Torpedo = new Weapon(std::stoi(temp2[0]), std::stoi(temp2[1]), std::stoi(temp2[2]));
		}

		file >> temp;
		temp2 = System::SplitString(temp, ',');
		if (temp[0] != char("0")) //If it has a kinetic
		{
			Kinetic = new Weapon(std::stoi(temp2[0]), std::stoi(temp2[1]), std::stoi(temp2[2]));
		}

		file >> temp;
		temp2 = System::SplitString(temp, ',');
		if (temp[0] != char("0")) //If it has a beam
		{
			Beam = new Weapon(std::stoi(temp2[0]), std::stoi(temp2[1]), std::stoi(temp2[2]));
		}

		WeaponElements.push_back(Torpedo);
		WeaponElements.push_back(Kinetic);
		WeaponElements.push_back(Beam);

		Sprite.setTexture(*Texture);
		FactionI = FactionIn;


		//Set sprite
		Sprite.setScale(pow((float)Parent->Length/300,0.5), pow((float)Parent->Length / 300, 0.5));

		Direction = FactionIn * 180;
		

		//health inds
		ShieldInd->setFillColor(sf::Color::Blue);
		HullInd->setFillColor(sf::Color::Green);

	}
	
	

	int KineticTickDelay = 0;
	int TorpedoTickDelay = 0;
	int BeamTickDelay = 0;

	std::vector<Weapon*> WeaponElements;

	float Direction = 0;

	void SetLookAtRotation();

	void MoveShip();

	std::string MovementMode = "attack";

	Weapon* GetLowestRangedWeapon();
	Weapon* GetLongestRangedWeapon();


	sf::Vector2f GetSpriteSize(sf::Sprite sprite)
	{
		float a = sprite.getTexture()->getSize().x * sprite.getScale().x;
		float b = sprite.getTexture()->getSize().y * sprite.getScale().y;
		return sf::Vector2f(a, b);
	}

	sf::Vector2f MovementTarget;

	static sf::Vector2f GetDirection(sf::Vector2f Origin, sf::Vector2f Target);

	sf::Vector2f GetCentreLocation()
	{
		
		
		sf::Vector2f out;
		out = GetSpriteSize(Sprite) * cos(Direction);
		out.x /= 2;
		out.y /= 2;
		out += Location;

		return out;
	}

	//health indicators
	sf::CircleShape*ShieldInd = new sf::CircleShape(5);
	sf::CircleShape* HullInd = new sf::CircleShape(5);
	

	static std::vector<ShipInstance*> ToInstances(Faction* fac, std::vector<BattleShip*> AllShips, std::vector<ShipInstance*> Original);

};

class TorpedoType : public Weapon
{
public:

	TorpedoType(int D, int R, int S) : Weapon(D, R, S)
	{
		Damage = D;
		Range = R;
		Speed = S;
	}



	TorpedoType(TorpedoType* in) : Weapon(in->Damage, in->Range, in->Speed)
	{
		Damage = in->Damage;
		Range = in->Range;
		Speed = in->Speed;
	}

	sf::CircleShape* Circle;
	BattleShip* Target;
	bool EventTick();

};



class KineticType : public Weapon
{
public:

	KineticType(int D, int R, int S) : Weapon(D, R, S)
	{
		Damage = D;
		Range = R;
		Speed = S;
	}


	KineticType(KineticType* in) : Weapon(in->Damage, in->Range, in->Speed)
	{
		Damage = in->Damage;
		Range = in->Range;
		Speed = in->Speed;
	}

	BattleShip* Origin;
	sf::Vector2f Vector;
	sf::Vertex line[2] =
	{
		sf::Vertex(sf::Vector2f(Location.x, Location.y)),
		sf::Vertex(sf::Vector2f(Location.x - Vector.x, Location.y - Vector.y))
	};


	bool EventTick(std::vector<BattleShip*>* ships1, std::vector<BattleShip*>* ships2);
};

class BeamType : public Weapon
{
public:

	BeamType(int D, int R, int S) : Weapon(D, R, S)
	{
		Damage = D;
		Range = R;
		Speed = S;
	}
	BeamType(BeamType* in) : Weapon(in->Damage, in->Range, in->Speed)
	{
		Damage = in->Damage;
		Range = in->Range;
		Speed = in->Speed;
	}
	sf::Vertex line[2] =
	{
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(0, 0))
	};
	BattleShip* Origin;
	BattleShip* Target;
	float length= 1000;


};


void BattleShipEventTick(std::vector<BattleShip*> Enemyships, std::vector<TorpedoType*>* FreeTorpedos,BattleShip* owner, std::vector<KineticType*>* FreeKinetics, std::vector<BeamType*>* FreeBeams, BattleSounds* Sounds);
