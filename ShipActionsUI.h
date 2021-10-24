#pragma once
#include "System.h"
#include "FactionData.h"
class ShipActionsUI
{
public:


	//Buttons for the various possible things to do.
	Button* Colonise;
	Button* Terraform;
	Button* AutoTransport;
	

	ShipActionsUI()
	{
		Colonise = new Button(sf::Vector2i(1450,100),"Colonise");
		Terraform = new Button(sf::Vector2i(1450, 150), "Terraform");
		AutoTransport = new Button(sf::Vector2i(1450,200),"Auto Transport");
	}

	void RenderShipActions(sf::RenderWindow* window, FleetStruct* CurrentFleet, System* CurrentSystem, Planet* CurrentPlanet, SelectEnum* select, FactionData* PlayerFaction);
	


};

