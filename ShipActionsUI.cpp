#include "ShipActionsUI.h"



void ShipActionsUI::RenderShipActions(sf::RenderWindow* window,FleetStruct* CurrentFleet, System* CurrentSystem, Planet* CurrentPlanet, SelectEnum* select)
{
	ShipInstance* CurrentShip = CurrentFleet->Ships.at(0);
	//Only draw the things that the ship can actually do
	bool CanActivity = false;

	for (size_t j = 0; j < CurrentShip->Type->Colonisable.size(); j++)
	{
		if (CurrentPlanet->PlanetType == CurrentShip->Type->Colonisable.at(j))
		{
				
			CanActivity = true;
			

		}
	}

	CanActivity = CanActivity && CurrentPlanet->Population < 1;
	
	//^Checks if there is a colonisable planet in the system
	if (CanActivity)
	{
		Colonise->Render(window);
		
		//Now for click functionality
		if (Colonise->isClicked(sf::Mouse::getPosition())) 
		{
			
			//The selected planet will be colonised
			CurrentPlanet->Population += CurrentShip->Type->Crew;
			for (size_t i = 0; i < CurrentSystem->SystemShips.size(); i++)
			{
				if (CurrentShip == CurrentSystem->SystemShips.at(i))
				{
					CurrentSystem->SystemShips.erase(CurrentSystem->SystemShips.begin()+i);
				}
			}
			CurrentSystem->OwningFaction = CurrentShip->OwningFaction;
			delete CurrentFleet;
			delete CurrentShip;
			*select = SelectEnum::Basic;
			
			return;
			

		}
	}
	CanActivity = false;

	for (size_t j = 0; j < CurrentShip->Type->Terraformable.size(); j++)
	{
		if (CurrentPlanet->PlanetType == CurrentShip->Type->Terraformable.at(j))
		{
			CanActivity = true;
		}
	}
	//Seems to not work properly for class D planets TODO

	CanActivity = CanActivity && CurrentPlanet->Population < 1;

	
	if (CanActivity)
	{
		Terraform->Render(window);
		*select = SelectEnum::Basic;
	}
	
	//TODO can only do either if the population is 0 of atleast one valid planet
	//TODO right now it appears that sputnik can colonise that shouldn't be

}
