#pragma once
#include <vector>
#include <iostream>
//#include "Planet.h"

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

	std::vector<ShipType> FactionShips;


	void LoadShips();
};

