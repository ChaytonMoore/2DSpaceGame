#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Graphical.h"
#include "UIElement.h"


enum PlanetClasses
{
	D,H,J,K,L,M,N,R,T,Y
};

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


class Planet : public UIElement
{
public:
	float PlanetSize;
	int ArableLand = 0;
	uint64_t Population = 0;

	PlanetClasses PlanetType;

	Planet(float size, PlanetClasses type);


	sf::Sprite* PlanetSprite;

	MedicalLevels Medical;
	MiningLevels Mining;
	ShipyardLevels Shipyard;

};

