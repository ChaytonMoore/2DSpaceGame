#pragma once
#include "System.h"
class ShipActionsUI
{
public:


	//Buttons for the various possible things to do.
	Button* Colonise;
	Button* Terraform;

	ShipActionsUI()
	{
		Colonise = new Button(sf::Vector2i(1450,100),"Colonise");
		Terraform = new Button(sf::Vector2i(1450, 150), "Terraform");
	}

	void RenderShipActions(sf::RenderWindow* window, FleetStruct* CurrentFleet, System* CurrentSystem, Planet* CurrentPlanet, SelectEnum* select, Faction* PlayerFaction);
	


};

