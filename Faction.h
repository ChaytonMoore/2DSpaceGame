#pragma once


enum PlanetClasses
{
	D, H, J, K, L, M, N, R, T, Y
};

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

	std::vector<PlanetClasses> Colonisable;
	std::vector<PlanetClasses> Terraformable;



	std::string type;

	int Capacity;
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
		sf::Image Temp;
		if (Temp.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Emblem/" + Name + "Emblem.png"))
		{
			GenericEmblem.loadFromImage(Temp);
		}
		else
		{
			Temp.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Emblem/GenericEmblem.png");
			GenericEmblem.loadFromImage(Temp);
		}


	}

	//Research coefficients
	float MedicalCoef;
	float MiningCoef;
	float Farmingcoef;
	float ConstructionCoef;
	float ShipBuildingCoef;
	float WeaponsCoef;
	float ShieldCoef;
	float EngineCoef;
	float SensorCoef;


	int Scientificness;
	int Industrialness;
	int Militaryness;
	int Xenophileness;


	struct FactionRelationEffect
	{
		std::string Name;
		Faction* TargetFaction;
		int Itensity;
		float TerminationDate;

	};

	std::vector<FactionRelationEffect> RelationEffects;


	void LoadShips();



	//Here some visual elements are stored
	sf::Texture GenericEmblem;


	int CalculateFactionRelation(Faction* TargetFaction);


	//void FactionTick(std::vector<Faction*> Factions, std::vector<System*> Systems);

	//extra data
	bool UnifiedPlanet;
	uint8_t Agressive;
	uint8_t Honour;
	std::string GovernmentType;
	uint8_t Colonial;
	uint8_t CivilMilitaryRatio;
	uint8_t Submissiveness;
	uint8_t Mercentile;
	char FavouriteDirection; //the direction they expand in, North, South, East, West, Centre

	


	
	
};

//faction is at the top so these go here


enum ShipActivity
{
	Standard, Transport, Terraform
};





PlanetClasses PlanetFromChar(char a);