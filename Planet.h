#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Graphical.h"
#include "UIElement.h"
#include <fstream>
#include "Faction.h"









enum MedicalLevels
{
	NoMedical,Infirmary, Hospital, HospitalNetwork, MedicalLabs, AdvancedMedicalLabs
};

enum MiningLevels
{
	NoMines, BasicMines, GlobalMines, StripMining, AdvancedStripMining
};

enum ShipyardLevels
{
	NoShipyard, SurfaceShipyard, BasicShipyard, SpaceElevator, AdvancedShipyards
};

enum ScienceLevels
{
	NoLab, BasicLab, University, UniversityNetwork, ScienceComplex, GlobalScienceComplex
};

enum Observatory
{ 
	NoObservatory, BasicTelescope, SataliteTelescope, SataliteNetwork,SubspaceScanner, SubspaceNetwork
};









class Planet : public UIElement
{
public:
	float PlanetSize;
	int ArableLand = 0;
	uint64_t Population = 0;
	uint64_t MaxPop = 0;

	PlanetClasses PlanetType;

	Planet(float size, PlanetClasses type);


	sf::Sprite* PlanetSprite;

	MedicalLevels Medical;
	MiningLevels Mining;
	ShipyardLevels Shipyard;
	ScienceLevels Lab;
	Observatory Observatory;


	int CalculateMetalOutput();
	int CalculateTitaniumOutput();
	int CalculateRareOutput();

	

	float TerraformTime;
	void PlanetYearTick(Faction* OwningFaction);
};


struct ShipInstance
{
	ShipType* Type;
	Faction* OwningFaction;
	int RemainingRange;
	int HullStrength;

	ShipInstance(ShipType* SType, Faction* Fac);

	ShipActivity Activity = Standard;
	std::map<std::string, int> Resources;

	Planet* TerraformingPlanet;

};

std::string LargeToString(uint64_t num);


struct PlanetTexture
{
	sf::Texture* texture;
	char type;
	PlanetClasses typeEnum;
};


std::vector<PlanetTexture*> LoadPlanetTextures();


