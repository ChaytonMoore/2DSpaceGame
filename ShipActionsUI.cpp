#include "ShipActionsUI.h"
#include "FactionData.h"


void ShipActionsUI::RenderShipActions(sf::RenderWindow* window,FleetStruct* CurrentFleet, System* CurrentSystem, Planet* CurrentPlanet, SelectEnum* select, FactionData* PlayerFaction)
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

	CanActivity = CanActivity && (CurrentPlanet->Population < 1 || (CurrentSystem->OwningFaction == PlayerFaction->OwningFaction && CurrentPlanet->Population < 100000));
	
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
			PlayerFaction->OwnedSystems.push_back(CurrentSystem);
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
		if (Terraform->isClicked(sf::Mouse::getPosition()) && Terraform->Active)
		{
			if (CurrentShip->Activity == ShipActivity::Terraform)
			{
				CurrentShip->Activity = ShipActivity::Standard;
				CurrentShip->TerraformingPlanet = nullptr;
				Terraform->rect.setOutlineColor(sf::Color(74, 89, 57));
			}
			else 
			{
				CurrentShip->Activity = ShipActivity::Terraform;
				CurrentShip->TerraformingPlanet = CurrentPlanet;
				Terraform->rect.setOutlineColor(sf::Color::Red);
			}
			Terraform->Active = false;
			
		}
		else if(Terraform->isClicked(sf::Mouse::getPosition()))
		{
			Terraform->Active = true;
		}
	}
	
	//TODO can only do either if the population is 0 of atleast one valid planet
	//TODO right now it appears that sputnik can colonise that shouldn't be


	//auto transport
	CanActivity = false;
	if (CurrentShip->Type->Capacity>0)
	{
		AutoTransport->Render(window);
		if (AutoTransport->isClicked(sf::Mouse::getPosition()))
		{
			CurrentShip->Activity = ShipActivity::Transport;
		}
	}

}