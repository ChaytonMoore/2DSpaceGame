#include <SFML/Graphics.hpp>


#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <stdlib.h> 
#include <stdio.h>  


#include "Graphical.h"
#include "System.h"
#include "PlanetUIElement.h"
#include "SystemUIElement.h"
#include "DetailsPanel.h"
#include "MainMenu.h"
#include "Battle.h"
#include "EventHandler.h"
#include "TopMenu.h"
#include "Border.h"
#include "MusicPlayer.h"




//#include "Faction.h"

sf::Vector2f CameraLocation = sf::Vector2f(0,0);

System* CurrentSystem;
Planet* CurrentPlanet;
FleetStruct* CurrentFleet;
std::vector<FleetStruct*> FreeFleets;

//Probably best to move the variable below out of global
sf::Sprite PlanetFocus;


double GlobalTime = 0;


//Factions 



SelectEnum CurrentSelect = SelectEnum::Basic;
DetailsPanel* CentralPanel;

float CurrentZoom = 1;

void Faction::LoadShips()
{

	
	std::string Line;
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Ships.csv");
	std::vector<std::string> split;

	std::string col;
	
	while (std::getline(File, Line)) 
	{
		split = System::SplitString(Line, ',');

		//if ((split.at(0) != "Title") && split.size() > 12);
		if(split.at(0) != "Title" && FactionName == split.at(21))
		{

			FactionShips.push_back(new ShipType);

			FactionShips.back()->Name = split.at(0);
			FactionShips.back()->Length = std::stoi(split.at(1));
			FactionShips.back()->Attack = std::stoi(split.at(2));
			FactionShips.back()->Hull = std::stoi(split.at(3));
			FactionShips.back()->Shields = std::stoi(split.at(4));
			FactionShips.back()->Speed = std::stoi(split.at(5));
			FactionShips.back()->Range = std::stoi(split.at(6));
			FactionShips.back()->ShipTech = std::stoi(split.at(7));
			FactionShips.back()->WeaponTech = std::stoi(split.at(8));
			FactionShips.back()->ShieldTech = std::stoi(split.at(9));
			FactionShips.back()->EngineTech = std::stoi(split.at(10));
			FactionShips.back()->Sensortech = std::stoi(split.at(11));

			col = split.at(12);
			for (size_t i = 0; i < col.size(); i++)
			{
				FactionShips.back()->Colonisable.push_back(PlanetFromChar(col.at(i)));
				
			}

			FactionShips.back()->Crew = std::stoi(split.at(13));
			FactionShips.back()->ConstructTime = std::stof(split.at(14));
			FactionShips.back()->RequiredMetal = std::stof(split.at(15));
			FactionShips.back()->RequiredTitanium = std::stof(split.at(16));
			FactionShips.back()->RequiredRare = std::stof(split.at(17));

			col = split.at(18);
			for (size_t i = 0; i < col.size(); i++)
			{
				FactionShips.back()->Terraformable.push_back(PlanetFromChar(col.at(i)));

			}

			FactionShips.back()->type = split.at(19);
			FactionShips.back()->Capacity = std::stoi(split.at(20));

			
			
		}
		
	}
}


//Might: make it so this works generically instead as just for player faction
//it actually probably does work generically and so it's only in the name
std::vector<ShipType*> GetBuildableShips(Faction* PlayerFaction)
{
	std::vector<ShipType*> output;
	if (CurrentSystem)
	{


		//first check if the system is the same as the player faction
		if (PlayerFaction == CurrentSystem->OwningFaction)
		{
			bool valid;
			for (size_t i = 0; i < PlayerFaction->FactionShips.size(); i++)
			{

				valid = CurrentSystem->SimpleMetal > PlayerFaction->FactionShips.at(i)->RequiredMetal && CurrentSystem->Titanium > PlayerFaction->FactionShips.at(i)->RequiredTitanium && CurrentSystem->RareMetal > PlayerFaction->FactionShips.at(i)->RequiredRare;
				valid = valid && PlayerFaction->EngineTech >= PlayerFaction->FactionShips.at(i)->EngineTech && PlayerFaction->ShipbuildingTech >= PlayerFaction->FactionShips.at(i)->ShipTech && PlayerFaction->ShieldTech >= PlayerFaction->FactionShips.at(i)->ShieldTech && PlayerFaction->SensorTech >= PlayerFaction->FactionShips.at(i)->Sensortech && PlayerFaction->WeaponsTech >= PlayerFaction->FactionShips.at(i)->WeaponTech;
				valid = valid && CurrentSystem->GetPopulation() > PlayerFaction->FactionShips.at(i)->Crew*100;
				valid = valid && CurrentSystem->SimpleMetal >= PlayerFaction->FactionShips.at(i)->RequiredMetal && CurrentSystem->Titanium >= PlayerFaction->FactionShips.at(i)->RequiredTitanium && CurrentSystem->RareMetal >= PlayerFaction->FactionShips.at(i)->RequiredRare;
				
				if (valid)
				{
					output.push_back(PlayerFaction->FactionShips.at(i));
				}
			}
		}

	}

	std::string varients[8] = {"scout","frigate","destroyer","cruiser","dreadnought","transport","colony","colonyBig"};
	ShipType* LastInstance;
	int idx;
	for (size_t i = 0; i < 8; i++)
	{
		LastInstance = nullptr;
		idx = output.size();
		idx--;
		while (idx>0)
		{
			
			if (LastInstance && output.at(idx)->type == varients[i])
			{
				output.erase(output.begin() + idx);
				
			}
			else if (output.at(idx)->type == varients[i])
			{
				LastInstance = output.at(idx);
			}
			idx--;
			
		}
	}

	return output;
}

//*****End of Factions*******


int FindMostPopulated(System* sys)
{
	int pop = 0;
	int current = 0;
	for (size_t i = 0; i < sys->Planets.size(); i++)
	{
		if (sys->Planets.at(i)->Population>pop)
		{
			pop = sys->Planets.at(i)->Population;
			current = i;
		}
	}

	return current;

}






void PlanetUIElement::ResetBuildableships()
{

	//Clear the potentials of uneeded elements


	while (!BuildableShips.empty()) delete BuildableShips.back(), BuildableShips.pop_back();


	if (CurrentSystem->OwningFaction)
	{
		for (size_t i = 0; i < GetBuildableShips(CurrentSystem->OwningFaction).size(); i++)
		{
			
			BuildableShips.push_back(new ShipBuildButton(sf::Vector2i(30, 200), GetBuildableShips(CurrentSystem->OwningFaction).at(i)->Name, GetBuildableShips(CurrentSystem->OwningFaction).at(i)));
			//delete if already present
		}
	}

}

void ClickHandle(std::vector<UIElement*> Potentials, PlanetUIElement* PlanetUI)
{
	sf::Vector2i MouseLoc = sf::Mouse::getPosition();

	System* Temp;

	for (size_t i = 0; i < Potentials.size(); i++)
	{
		


		//re arrange the if statements
		if (Potentials.at(i) != nullptr && Potentials.size()>i)
		{


			


		
			
			//clean up uneeded elements




			if (dynamic_cast<System*>(Potentials.at(i)) != nullptr)
				{

					if (dynamic_cast<System*>(Potentials.at(i))->IsClicked(MouseLoc))
					{

						if (CurrentSelect == SelectEnum::Fleet && CurrentFleet != nullptr)
						{

							if ((System*)(Potentials.at(i)) == CurrentFleet->CurrentSystem)
							{
								delete CurrentFleet;
								CurrentSelect = SelectEnum::Basic;
							}
							else
							{
								//Need to check to make sure that the ship can actually get to the location
								if (CurrentFleet->Ships.at(0)->RemainingRange >= round(SystemDistance(sf::Vector2f(CurrentSystem->Location), sf::Vector2f((dynamic_cast<System*>(Potentials.at(i))->Location)))))
								{
									//TODO make it so that each ship is tested to find the one with the shortest range.

									CurrentFleet->TargetSystem = (System*)(Potentials.at(i));
									CurrentSelect = SelectEnum::Basic;


									for (size_t j = 0; j < CurrentFleet->Ships.size(); j++)
									{


										if (CurrentFleet->Ships.at(j)) //TODO just leaving the ships in might cause a memory leak
										{
											CurrentSystem->RemoveShipInstance(CurrentFleet->Ships.at(j));
										}
									}
									FreeFleets.push_back(CurrentFleet);
									CurrentFleet = nullptr;


								}


							}
						}



						CurrentSystem = ((System*)(Potentials.at(i)));
						if (CurrentSystem->Planets.size() > 0)
						{
							CurrentPlanet = CurrentSystem->Planets.at(FindMostPopulated(CurrentSystem));
						}
						((System*)(Potentials.at(i)))->ClickedOn();
						//Reset the ships as it's a new system
						PlanetUI->ResetBuildableships();


						//move up
					}
				}
			else if (dynamic_cast<Planet*>(Potentials.at(i)) != nullptr)
				{
					if (dynamic_cast<Planet*>(Potentials.at(i))->IsClicked(MouseLoc))
					{

						CurrentPlanet = ((Planet*)(Potentials.at(i)));
					}
				}
			/*
			else if (dynamic_cast<ShipBuildButton*>(Potentials.at(i)) != nullptr)
				{
					if (dynamic_cast<ShipBuildButton*>(Potentials.at(i))->IsClicked(MouseLoc, PlanetUI->CurrentScroll))
					{

						dynamic_cast<ShipBuildButton*>(Potentials.at(i))->ClickedOn();
						PlanetUI->ResetBuildableships(Potentials);
						//checks to see if the central panel should still be up
						bool CentralPanelUp = false;
						for (size_t n = 0; n < PlanetUI->BuildableShips.size(); n++)
						{
							if (PlanetUI->BuildableShips.at(n)->ShipToBuild->Name == CentralPanel->CurrentObject)
							{
								CentralPanelUp = true;
							}
						}
						if (!CentralPanelUp)
						{
							CentralPanel->CurrentObject = "";
						}

					}
				}
				*/

			


			


		}
		

			


	}




}



void RenderPlanets(sf::RenderWindow* window,System* system)
{
	for (size_t i = 0; i < system->Planets.size(); i++)
	{
		window->draw(*system->Planets.at(i)->PlanetSprite);
		if (system->Planets.at(i) == CurrentPlanet)
		{
			//Render the focus graphic
			PlanetFocus.setPosition(system->Planets.at(i)->PlanetSprite->getPosition());
			PlanetFocus.setScale(system->Planets.at(i)->PlanetSprite->getScale());
			window->draw(PlanetFocus);
		}
		
	}


}

void RenderShipQueue(sf::RenderWindow* window)
{
	for (size_t i = 0; i < CurrentSystem->ShipBuildQueue.size(); i++)
	{
		CurrentSystem->ShipBuildQueue.at(i).UIElement->rect.setPosition(sf::Vector2f(240,150 + i*50));
		CurrentSystem->ShipBuildQueue.at(i).UIElement->Text.setPosition(sf::Vector2f(240, 150 + i * 50));
		window->draw(CurrentSystem->ShipBuildQueue.at(i).UIElement->rect);
		CurrentSystem->ShipBuildQueue.at(i).UIElement->Text.setString(CurrentSystem->ShipBuildQueue.at(i).Type->Name+ std::to_string(CurrentSystem->ShipBuildQueue.at(i).Remaining));
		window->draw(CurrentSystem->ShipBuildQueue.at(i).UIElement->Text);
	}
}

void SetUpPlanetSprites(System* system, std::vector<PlanetTexture*> PlanetTextures)
{


	
	for (size_t i = 0; i < system->Planets.size(); i++)
	{
		if (system->Planets.at(i)->PlanetType == K)
		{
			system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(0)->texture);
		}
		else if (system->Planets.at(i)->PlanetType == M && system->Name == "Sol.csv") //Note system names have .csv at the end
		{
			//This is used for unique planets such as earth

			// TODO find a better way of assigning planets
			system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(1)->texture);
		}
		else if (system->Planets.at(i)->PlanetType == D)
		{
			system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(2)->texture);
		}
		else if (system->Planets.at(i)->PlanetType == J)
		{
			system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(3)->texture);
		}
		else if (system->Planets.at(i)->PlanetType == M)
		{
			system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(4)->texture);
		}
		else
		{
			
			system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(0)->texture);
		}
		

		system->Planets.at(i)->PlanetSprite->setScale(sf::Vector2f(pow(system->Planets.at(i)->PlanetSize,0.05)*0.75, pow(system->Planets.at(i)->PlanetSize, 0.05) * 0.75));
		system->Planets.at(i)->PlanetSprite->setPosition(sf::Vector2f(1720,32+i*128));
		system->Planets.at(i)->LocalPosition = (sf::Vector2i(1720, 32 + i * 128));
	}
}

int ShipBuildButton::ClickedOn()
{
	//Remove resources
	
	CurrentSystem->SimpleMetal = CurrentSystem->SimpleMetal - ShipToBuild->RequiredMetal;
	CurrentSystem->Titanium = CurrentSystem->Titanium - ShipToBuild->RequiredTitanium;
	CurrentSystem->RareMetal = CurrentSystem->RareMetal - ShipToBuild->RequiredRare;
	
	
	
	CurrentSystem->ShipBuildQueue.push_back(*new ShipConstructData());
	CurrentSystem->ShipBuildQueue.back().Type = ShipToBuild;
	CurrentSystem->ShipBuildQueue.back().Remaining = ShipToBuild->ConstructTime;
	CurrentSystem->ShipBuildQueue.back().UIElement = new Button(sf::Vector2i(180,120), ShipToBuild->Name);
	CurrentSystem->Planets.at(FindMostPopulated(CurrentSystem))->Population -= ShipToBuild->Crew;
	
	//reset the current system buildable ships

	

	return 1;
}


void PlayerFactionStart(Faction* PlayerFaction, std::vector<System*> systems)
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		if (systems.at(i)->OwningFaction == PlayerFaction)
		{
			systems.at(i)->Titanium = 100;
			systems.at(i)->SimpleMetal = 100;
			systems.at(i)->RareMetal = 100;
		}
	}
	
}

void LoadFactionData(FactionData* Faction, std::vector<System*>* Systems)
{
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Profiles/" + Faction->OwningFaction->FactionName + ".txt");
	std::vector<std::string> split;
	std::string a;
	File >> a;
	split = System::SplitString(a,',');

	int pop;

	try {

		for (size_t i = 0; i < Systems->size(); i++)
		{
			

			if (Systems->at(i)->OwningFaction == Faction->OwningFaction)
			{
				Systems->at(i)->SimpleMetal = std::stoi(split.at(0));
				Systems->at(i)->Titanium = std::stoi(split.at(1));
				Systems->at(i)->RareMetal = std::stoi(split.at(2));


				//Now find the most populated planet
				pop = FindMostPopulated(Systems->at(i));
				Systems->at(i)->Planets.at(pop)->Medical = MedicalLevels(std::stoi(split.at(3)));
				Systems->at(i)->Planets.at(pop)->Mining = MiningLevels(std::stoi(split.at(4)));
				Systems->at(i)->Planets.at(pop)->Shipyard = ShipyardLevels(std::stoi(split.at(5)));
				Systems->at(i)->Planets.at(pop)->Lab = ScienceLevels(std::stoi(split.at(6)));

				//This is fragmented and bad
				Systems->at(i)->Planets.at(pop)->Observatory = Observatory(std::stoi(split.at(20)));

				Faction->OwnedSystems.push_back(Systems->at(i));
			}

		}

		Faction->OwningFaction->MedicalCoef = std::stof(split.at(7));
		Faction->OwningFaction->MiningCoef = std::stof(split.at(8));
		Faction->OwningFaction->Farmingcoef = std::stof(split.at(9));

		Faction->OwningFaction->ConstructionCoef = std::stof(split.at(10));
		Faction->OwningFaction->ShipBuildingCoef = std::stof(split.at(11));
		Faction->OwningFaction->WeaponsCoef = std::stof(split.at(12));

		Faction->OwningFaction->ShieldCoef = std::stof(split.at(13));
		Faction->OwningFaction->EngineCoef = std::stof(split.at(14));
		Faction->OwningFaction->SensorCoef = std::stof(split.at(15));
		Faction->OwningFaction->Scientificness = std::stof(split.at(16));
		Faction->OwningFaction->Industrialness = std::stof(split.at(17));
		Faction->OwningFaction->Militaryness = std::stof(split.at(18));
		Faction->OwningFaction->Xenophileness = std::stof(split.at(19));
		//call 20 is above in planet facilities
		
		
	}
	catch(std::out_of_range)
	{
		//throw std::out_of_range("Insufficient number of values in profile of: ");
		std::string message;
		message = "Insufficient number of values in profile of: " + Faction->OwningFaction->FactionName;
		std::wstring s = std::wstring(message.begin(), message.end());

		int msgboxID = MessageBox(
			NULL,
			(message.c_str()), //contents
			"File Error", //title
			MB_ICONWARNING |  MB_DEFBUTTON2
		);
		exit(-1);
	}
	
}

void TechnologyTick(double DeltaTime,std::vector<FactionData*> Factions, std::vector<System*> Systems)
{
	//Yes this function is terrible yandere level code


	//Should only happen once a year

	//For each faction we'll just the same variables
	int TotalResearch;
	srand(time(NULL));


	const int TechConst = 1250; //normally 50

	for (size_t i = 0; i < Factions.size(); i++)
	{
		TotalResearch = 0;
		for (size_t j = 0; j < Systems.size(); j++)
		{
			if (Systems.at(j)->OwningFaction == Factions.at(i)->OwningFaction)
			{
				for (size_t n = 0; n < Systems.at(j)->Planets.size(); n++)
				{
					//The research level is proportional to the lab level squared to put priority on the larger systems
					TotalResearch += pow((int)Systems.at(j)->Planets.at(n)->Lab, 2); 



				}
			}
		}
		
		//Each of the techs have its own value 
		if (TotalResearch*Factions.at(i)->OwningFaction->MedicalCoef > Factions.at(i)->OwningFaction->MedicalTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->MedicalTech++;
			EventHandler::TechAdvanceNotification("Medical", Factions.at(i)->OwningFaction->MedicalTech);
		}
		else
		{
			//Random chance it is advanced
			
			if (TotalResearch * Factions.at(i)->OwningFaction->MedicalCoef -5 > rand() % (int(pow(Factions.at(i)->OwningFaction->MedicalTech, 2)+1) * 16))
			{
				
				Factions.at(i)->OwningFaction->MedicalTech++;
				EventHandler::TechAdvanceNotification("Medical", Factions.at(i)->OwningFaction->MedicalTech);
			}
		}
		
		
		if (TotalResearch * Factions.at(i)->OwningFaction->MiningCoef > Factions.at(i)->OwningFaction->MiningTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->MiningTech++;
			EventHandler::TechAdvanceNotification("Mining", Factions.at(i)->OwningFaction->MiningTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->MiningCoef -5 > rand() % (int(pow(Factions.at(i)->OwningFaction->MiningTech, 2)+1) * 16))
			{
				
				Factions.at(i)->OwningFaction->MiningTech++;
				EventHandler::TechAdvanceNotification("Mining", Factions.at(i)->OwningFaction->MiningTech);
			}
		}

		if (TotalResearch * Factions.at(i)->OwningFaction->Farmingcoef > Factions.at(i)->OwningFaction->FarmingTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->FarmingTech++;
			EventHandler::TechAdvanceNotification("Farming", Factions.at(i)->OwningFaction->FarmingTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->Farmingcoef-5 > rand() % (int(pow(Factions.at(i)->OwningFaction->FarmingTech, 2)+1) * 16))
			{
				Factions.at(i)->OwningFaction->FarmingTech++;
				EventHandler::TechAdvanceNotification("Farming", Factions.at(i)->OwningFaction->FarmingTech);
			}
		}

		if (TotalResearch * Factions.at(i)->OwningFaction->ConstructionCoef > Factions.at(i)->OwningFaction->ConstructionTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->ConstructionTech++;
			EventHandler::TechAdvanceNotification("Contruction", Factions.at(i)->OwningFaction->ConstructionTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->ConstructionCoef-5 > rand() % (int(pow(Factions.at(i)->OwningFaction->ConstructionTech, 2)+1) * 16))
			{
				Factions.at(i)->OwningFaction->ConstructionTech++;
				EventHandler::TechAdvanceNotification("Contruction", Factions.at(i)->OwningFaction->ConstructionTech);
			}
		}

		if (TotalResearch * Factions.at(i)->OwningFaction->ShipBuildingCoef > Factions.at(i)->OwningFaction->ShipbuildingTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->ShipbuildingTech++;
			EventHandler::TechAdvanceNotification("Ship Building", Factions.at(i)->OwningFaction->ShipbuildingTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->ShipBuildingCoef-5 > rand() % (int(pow(Factions.at(i)->OwningFaction->ShipbuildingTech, 2)+1) * 16))
			{
				Factions.at(i)->OwningFaction->ShipbuildingTech++;
				EventHandler::TechAdvanceNotification("Ship Building", Factions.at(i)->OwningFaction->ShipbuildingTech);
			}
		}

		if (TotalResearch * Factions.at(i)->OwningFaction->WeaponsCoef > Factions.at(i)->OwningFaction->WeaponsTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->WeaponsTech++;
			EventHandler::TechAdvanceNotification("Weapon", Factions.at(i)->OwningFaction->WeaponsTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->WeaponsCoef-5 > rand() % (int(pow(Factions.at(i)->OwningFaction->WeaponsTech, 2)+1) * 16))
			{
				Factions.at(i)->OwningFaction->WeaponsTech++;
				EventHandler::TechAdvanceNotification("Weapon", Factions.at(i)->OwningFaction->WeaponsTech);
			}
		}


		if (TotalResearch * Factions.at(i)->OwningFaction->ShieldCoef > Factions.at(i)->OwningFaction->ShieldTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->ShieldTech++;
			EventHandler::TechAdvanceNotification("Shield", Factions.at(i)->OwningFaction->ShieldTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->ShieldCoef-5 > rand() % int(pow(Factions.at(i)->OwningFaction->ShieldTech, 2)+1 * 16))
			{
				Factions.at(i)->OwningFaction->ShieldTech++;
				EventHandler::TechAdvanceNotification("Shield", Factions.at(i)->OwningFaction->ShieldTech);
			}
		}


		if (TotalResearch * Factions.at(i)->OwningFaction->EngineCoef > Factions.at(i)->OwningFaction->EngineTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->EngineTech++;
			EventHandler::TechAdvanceNotification("Engine", Factions.at(i)->OwningFaction->EngineTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->EngineCoef-5 > rand() % int(pow(Factions.at(i)->OwningFaction->EngineTech, 2)+1 * 16))
			{
				Factions.at(i)->OwningFaction->EngineTech++;
				EventHandler::TechAdvanceNotification("Engine", Factions.at(i)->OwningFaction->EngineTech);
			}
		}

		if (TotalResearch * Factions.at(i)->OwningFaction->SensorCoef > Factions.at(i)->OwningFaction->SensorTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->OwningFaction->SensorTech++;
			EventHandler::TechAdvanceNotification("Sensor", Factions.at(i)->OwningFaction->SensorTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->OwningFaction->SensorCoef-5 > rand() % int(pow(Factions.at(i)->OwningFaction->SensorTech, 2)+1 * 16))
			{
				Factions.at(i)->OwningFaction->SensorTech++;
				EventHandler::TechAdvanceNotification("Sensor", Factions.at(i)->OwningFaction->SensorTech);
			}
		}


	}

	


}


bool GetShouldResetShipBuild(std::vector<ShipBuildButton*> ships, System* system)
{
	//It might not be necessary to use a system input as current system is global but keep it in incase this function is used elsewhere.
	bool out = false;

	for (size_t i = 0; i < ships.size(); i++)
	{
		if (ships.at(i)->ShipToBuild->RequiredMetal > system->SimpleMetal || ships.at(i)->ShipToBuild->RequiredTitanium > system->Titanium || ships.at(i)->ShipToBuild->RequiredRare > system->RareMetal )
		{
			out = true;
		}
	}

	return out;
}

void CreateShips(std::vector<BattleShip*>* Ships1, std::vector<BattleShip*>* Ships2, std::vector<ShipInstance*> F1, std::vector<ShipInstance*> F2, BattleSounds* Sounds)
{
	//the cross over in textures will be minor.
	std::vector<sf::Image*> temp ;
	std::vector<sf::Texture*> tempT;
	for (size_t i = 0; i < F1.size(); i++)
	{
		temp.push_back(new sf::Image);
		tempT.push_back(new sf::Texture);
		temp.back()->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Battle/" + F1.at(i)->Type->Name + ".png");
		tempT.back()->loadFromImage(*temp.back());
		Ships1->push_back(new BattleShip(F1.at(i)->Type, tempT.back(), 0, Sounds));
		Ships1->back()->Location = sf::Vector2f(10,100 * (i+1));
		
	}

	for (size_t i = 0; i < F2.size(); i++)
	{
		temp.push_back(new sf::Image);
		tempT.push_back(new sf::Texture);
		temp.back()->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Battle/" + F2.at(i)->Type->Name + ".png");
		tempT.back()->loadFromImage(*temp.back());
		Ships2->push_back(new BattleShip(F2.at(i)->Type, tempT.back(), 1, Sounds));
		Ships2->back()->Location = sf::Vector2f(1800, 100 * (i+1));
	}



}






BattleReturn Battle(Faction* Faction1, Faction* Faction2, std::vector<ShipInstance*> Faction1Ships, std::vector<ShipInstance*> Faction2Ships)
{
	BattleSounds* Sounds = new BattleSounds();

	

	std::vector<BattleShip*> Ships1;
	std::vector<BattleShip*> Ships2;

	std::vector<TorpedoType*> FreeTorpedos;
	std::vector<KineticType*> FreeKinetics;
	std::vector<BeamType*> FreeBeams;
	Faction* Facs[2];
	Facs[0] = Faction1Ships.front()->OwningFaction;
	Facs[1] = Faction2Ships.front()->OwningFaction;


	std::vector<BattleShip*> Store1;
	std::vector<BattleShip*> Store2;
	Store1 = Ships1;
	Store2 = Ships2;
	CreateShips(&Ships1, &Ships2, Faction1Ships, Faction2Ships, Sounds);


	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Battle", sf::Style::Fullscreen);


	BattleReturn output;


	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
		}
		window.clear();


		

		for (size_t i = 0; i < Ships1.size(); i++)
		{
			window.draw(Ships1.at(i)->Sprite);
			window.draw(*Ships1.at(i)->HullInd);
			if (Ships1.at(i)->Parent->Shields>0)
			{
				window.draw(*Ships1.at(i)->ShieldInd);
			}
			
			BattleShipEventTick(Ships2, &FreeTorpedos, Ships1.at(i), &FreeKinetics, &FreeBeams, Sounds);
			if (Ships1.at(i)->HullStrength<0)
			{
				delete Ships1.at(i);
				Ships1.erase(Ships1.begin() + i);
				if (Ships1.size() == 0 || Ships2.size() == 0)
				{
					output.Winner = Ships2.size();
					output.Ships1 = BattleShip::ToInstances(Faction1,Store1,Faction1Ships);
					output.Ships2 = BattleShip::ToInstances(Faction2, Store1, Faction2Ships);
					return output;
				}
			}
		}

		for (size_t i = 0; i < Ships2.size(); i++)
		{

			window.draw(Ships2.at(i)->Sprite);
			window.draw(*Ships2.at(i)->HullInd);
			if (Ships2.at(i)->Parent->Shields > 0)
			{
				window.draw(*Ships2.at(i)->ShieldInd);
			}

			BattleShipEventTick(Ships1, &FreeTorpedos, Ships2.at(i), &FreeKinetics, &FreeBeams, Sounds);
			if (Ships2.at(i)->HullStrength < 0)
			{
				delete Ships2.at(i);
				Ships2.erase(Ships2.begin() + i);
				if (Ships1.size() == 0 || Ships2.size() == 0)
				{
					output.Winner = Ships2.size();
					output.Ships1 = BattleShip::ToInstances(Faction1, Store1, Faction1Ships);
					output.Ships2 = BattleShip::ToInstances(Faction2, Store1, Faction2Ships);
					return output;
				}
			}
		}


		//draw weapons
		for (size_t i = 0; i < FreeTorpedos.size(); i++)
		{
			window.draw(*FreeTorpedos.at(i)->Circle);
			if (FreeTorpedos.at(i)->EventTick())
			{
				//remove
				FreeTorpedos.erase(FreeTorpedos.begin()+i);
			}
		}

		for (size_t i = 0; i <FreeKinetics.size(); i++)
		{
			window.draw(FreeKinetics.at(i)->line, 2, sf::Lines);
			if (FreeKinetics.at(i)->EventTick(&Ships1,&Ships2))
			{
				FreeKinetics.erase(FreeKinetics.begin() + i);
			}
		}
		
		for (size_t i = 0; i < FreeBeams.size(); i++)
		{
			FreeBeams.at(i)->line[0].position = FreeBeams.at(i)->Origin->GetCentreLocation();
			FreeBeams.at(i)->line[1].position = FreeBeams.at(i)->Target->GetCentreLocation();
			window.draw(FreeBeams.at(i)->line, 2, sf::Lines);
			FreeBeams.at(i)->length--;
			if (FreeBeams.at(i)->length < 0)
			{
				if (FreeBeams.at(i)->Target->ShieldStrength > 0)
				{
					FreeBeams.at(i)->Target->ShieldStrength -= FreeBeams.at(i)->Damage;
				}
				else
				{
					FreeBeams.at(i)->Target->HullStrength -= FreeBeams.at(i)->Damage;
				}
				FreeBeams.erase(FreeBeams.begin() + i);
			}
		}

		window.display();

	}




}

int main()
{

	



	
	//MenuRender();
	CentralPanel = new DetailsPanel;


	//The very first thing is to load the generic textures.
	
	TextureStore* TStore = new TextureStore;
	TStore->MassLoadTextures();

	

	//Load the planet focus texture
	LoadImageSF(&PlanetFocus,"PlanetSelect.png");
	

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::None); //This should be fullscreen but for testing windowed is easier
	sf::Sprite Background;
	LoadImageSF(&Background, "Background.png");
	


	//Create the factions




	
	
	std::vector<FactionData*> Factions;
	Factions.push_back(new FactionData("Human"));
	//This will need to be done differently for different factions
	FactionData* PlayerFaction = Factions.back();

	Factions.push_back(new FactionData("Svethi"));
	Factions.push_back(new FactionData("Taenos"));


	


	
	//System* Sol = new System(sf::Vector2i(960,540),"Sol");
	std::vector<UIElement*> UIAspects;

	//top UI Aspect
	TopMenu HorizMenu(Factions);
	
	//UIAspects.push_back(Sol);
	//Sol->LoadSystem("Sol.csv");


	std::vector<System*> Systems;
	Systems = LoadSystems(FactionData::GetFactions(Factions));
	for (size_t i = 0; i < Systems.size(); i++)
	{
		UIAspects.push_back(Systems.at(i));
	}


	//Fleets declaration
	

	//Sol->AddUIPlanets(&UIAspects);
	
	std::vector<sf::Texture*> PlanetTextures;
	std::vector<sf::Image*> PlanetImages;

	

	bool SystemTabOpen = false;
	
	//Load the planets
	//Must make sure that the other planets are loaded

	PlanetImages.push_back(new sf::Image);
	PlanetImages.at(0)->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/KPlanet.png");

	PlanetTextures.push_back(new sf::Texture);
	PlanetTextures.at(0)->loadFromImage(*PlanetImages.at(0));
	//SetUpPlanetSprites(Sol,PlanetTextures);
	for (size_t i = 0; i < Systems.size(); i++)
	{
		SetUpPlanetSprites(Systems.at(i), LoadPlanetTextures());
		
	}

	PlanetUIElement* PlanetUI = new PlanetUIElement;
	PlanetUI->SetUpPlanetUI();

	SystemUIElement* SystemUI = new SystemUIElement;
	ShipActionsUI* ActionsUIShip = new ShipActionsUI();





	std::chrono::high_resolution_clock::time_point TimeBegin = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point TimeEnd = std::chrono::high_resolution_clock::now();
	double DeltaTime = std::chrono::duration<double, std::milli>(TimeEnd - TimeBegin).count();
	sf::Text GlobalClock;
	sf::Font ClockFont;
	ClockFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/Protector.ttf");
	GlobalClock.setFont(ClockFont);
	GlobalClock.setCharacterSize(24);
	int TimeDialation = 1;
	const float DialationSettings[7] = { 0,0.5,1,2,6,15,32 };
	bool Paused = true;


	FleetStruct* TempFleet;

	int CurrentTimeStamp = 0;
	//used for controlling what year it is


	//This code should only be run when a new game is created
	for (size_t i = 0; i < Factions.size(); i++)
	{
		LoadFactionData(Factions.at(i), &Systems);
	}
	

	
	//Create the border graphic
	//Border* BorderGraphic = new Border(Systems,sf::Vector2i(1920,1080));

	//create music
	MusicPlayer* Music = new MusicPlayer();




	while (window.isOpen())
	{
		//begin time
		TimeBegin = std::chrono::high_resolution_clock::now();
		//play music
		Music->Tick();
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			CameraLocation.y -= 1 * pow(1 / CurrentZoom, 1);

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			CameraLocation.y += 1 * pow(1 / CurrentZoom, 1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			CameraLocation.x += 1 * pow(1 / CurrentZoom, 1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			CameraLocation.x -= 1 * pow(1 / CurrentZoom, 1);

		}


		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				//TODO this is only a quick method of exiting
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}



				//Time dialation
				if (event.key.code == sf::Keyboard::Up && TimeDialation < 6)
				{
					//Increase game speed
					TimeDialation++;
					
				}
				else if(event.key.code == sf::Keyboard::Down && TimeDialation >0)
				{
					//Decrease game speed
					TimeDialation--;
				}
				

				if (event.key.code == sf::Keyboard::Space)
				{
					Paused = !Paused;
				}



				if (event.key.code == sf::Keyboard::Tab)
				{

					if (!SystemTabOpen)
					{
						//when it is opened
						
						//later make it so it validates what types of ship can be built
						

						//TODO : crashes if there is no selected system
						if (CurrentSystem)
						{
							//for (size_t i = 0; i < GetBuildableShips(PlayerFaction->OwningFaction).size(); i++)
							//{
								//PlanetUI->BuildableShips.push_back(new ShipBuildButton(sf::Vector2i(30, 200), GetBuildableShips(PlayerFaction->OwningFaction).at(i)->Name, GetBuildableShips(PlayerFaction->OwningFaction).at(i)));

								
							//}
							PlanetUI->ResetBuildableships();

							//^Might be a memory leak

							//for (size_t i = 0; i < PlanetUI->BuildableShips.size(); i++)
							//{
							//	UIAspects.push_back(PlanetUI->BuildableShips.at(i));

							//}
							CurrentSystem->AddUIPlanets(&UIAspects);
						}
						
						
					}
					else
					{

						while (!PlanetUI->BuildableShips.empty()) delete PlanetUI->BuildableShips.back(), PlanetUI->BuildableShips.pop_back();
						

						// TODO: Remove UI planets when planet viewer is closed
					}
					SystemTabOpen = !SystemTabOpen;
				}

			}




			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					ClickHandle(UIAspects,PlanetUI);
				}
				
				
			}

			//scrolling was for the planet UI
			if (event.type == sf::Event::MouseWheelMoved)
			{
				//PlanetUI->CurrentScroll += event.mouseWheel.delta*16;
				//^ the old use of the mouse wheel
				CurrentZoom = (event.mouseWheel.delta==1)*CurrentZoom*1.1 + (event.mouseWheel.delta == -1) * CurrentZoom * 0.9;
				CameraLocation += sf::Vector2f(50* event.mouseWheel.delta,50* event.mouseWheel.delta);
			}

		}
		window.clear();

		//Here we will paste the background
		window.draw(Background);
		
		//Sol->RenderSystem(&window,CameraLocation);
		for (size_t i = 0; i < Systems.size(); i++)
		{
			//systems are now rendered based on who can see them
			//Systems.at(i)->RenderSystem(&window, CameraLocation, TStore, PlayerFaction->OwningFaction, CurrentZoom);
			Systems.at(i)->EventTick(DeltaTime*DialationSettings[TimeDialation]*Paused,&FreeFleets,Systems);
		}

		//This event tick of the faction
		for (size_t i = 0; i < Factions.size(); i++)
		{
			Factions.at(i)->FactionEventTick(DeltaTime,Systems,Factions,PlayerFaction);
		}

		for (size_t i = 0; i < PlayerFaction->KnownSystems.size(); i++)
		{
			PlayerFaction->KnownSystems.at(i)->RenderSystem(&window, CameraLocation, TStore, PlayerFaction->OwningFaction, CurrentZoom);
		
		}
		for (size_t i = 0; i < PlayerFaction->ExploredSystems.size(); i++)
		{
			PlayerFaction->ExploredSystems.at(i)->RenderSystem(&window, CameraLocation, TStore, PlayerFaction->OwningFaction, CurrentZoom);
		}
		for (size_t i = 0; i < PlayerFaction->OwnedSystems.size(); i++)
		{
			PlayerFaction->OwnedSystems.at(i)->RenderSystem(&window, CameraLocation, TStore, PlayerFaction->OwningFaction, CurrentZoom);
		}

		//rotate current system
		if (CurrentSystem)
		{
			CurrentSystem->CurrentSprite->rotate(DeltaTime*30);
		}


		if (SystemTabOpen && CurrentSystem)
		{
			//reset only if the current metals is insufficient
			if(GetShouldResetShipBuild(PlanetUI->BuildableShips, CurrentSystem)) //passing system might not be necessary but keep it
			{
				PlanetUI->ResetBuildableships();
				CentralPanel->CurrentObject = "";
			}
			 


			RenderPlanets(&window, CurrentSystem);
			PlanetUI->OwningPlanet = CurrentPlanet;
			PlanetUI->RenderPlanetUI(&window,CurrentSystem->OwningFaction, (PlayerFaction->OwningFaction == CurrentSystem->OwningFaction));

			
			RenderShipQueue(&window);
			TempFleet = SystemUI->RenderSystemUI(&window, CurrentSystem, CentralPanel);
			if (TempFleet != nullptr)
			{
				
				CurrentFleet = TempFleet;
				CurrentSelect = SelectEnum::Fleet;
			}
			if (CurrentFleet != nullptr)
			{
				if (CurrentFleet->Ships.size() == 1)
				{
					ActionsUIShip->RenderShipActions(&window,CurrentFleet,CurrentSystem,CurrentPlanet, &CurrentSelect, PlayerFaction);
				}
			}
			

			CentralPanel->RenderPanel(&window); //only want to render the central panel if the system is open, atleast right now
			
		}


		//Draw and tick free fleets
		for (size_t i = 0; i < FreeFleets.size(); i++)
		{
			FreeFleets.at(i)->RenderFleet(&window,CameraLocation,CurrentZoom);
			FreeFleets.at(i)->EventTick(DeltaTime* DialationSettings[TimeDialation]*Paused);
			
			if (FreeFleets.at(i)->Distance(FreeFleets.at(i)->Location,sf::Vector2f(FreeFleets.at(i)->TargetSystem->Location))<0.1)
			{
				//Add the ships to the system
				for (size_t j = 0; j < FreeFleets.at(i)->Ships.size(); j++)
				{
					FreeFleets.at(i)->TargetSystem->SystemShips.push_back(FreeFleets.at(i)->Ships.at(j));
					//Freighter arrival script
					if (FreeFleets.at(i)->Ships.at(j)->Type->Capacity) // freighter if the capacity is larger than 1
					{
						FreeFleets.at(i)->TargetSystem->FreighterArrive(FreeFleets.at(i)->Ships.at(j));
					}
				}


				delete FreeFleets.at(i);
				FreeFleets.erase(FreeFleets.begin()+i);
			}
		}



		for (size_t i = 0; i < PlanetUI->BuildableShips.size(); i++)
		{
			
			if (PlanetUI->BuildableShips.at(i)->isClicked(sf::Mouse::getPosition()) && PlanetUI->BuildableShips.at(i)->CanClick)
			{
				
				PlanetUI->BuildableShips.at(i)->ClickedOn();
				PlanetUI->BuildableShips.at(i)->CanClick = false;
				CentralPanel->CurrentObject = "";
				
			}
			//need to make sure that the button has to be clicked each time a ship is built
			if(!PlanetUI->BuildableShips.at(i)->isClicked(sf::Mouse::getPosition()))
			{
				PlanetUI->BuildableShips.at(i)->CanClick = true;
				
			}
			


		}
		//This needs to be done twice as some are removed
		for (size_t i = 0; i < PlanetUI->BuildableShips.size(); i++)
		{
			if (PlanetUI->BuildableShips.at(i)->isHovering(sf::Mouse::getPosition()))
			{
				if (CentralPanel->CurrentObject != PlanetUI->BuildableShips.at(i)->ShipToBuild->Name)
				{
					//Change the current image and details
					CentralPanel->CurrentObject = PlanetUI->BuildableShips.at(i)->ShipToBuild->Name;
					CentralPanel->CurrentSetting = PanelSetting::Ship;
					CentralPanel->Reload(PlanetUI->BuildableShips.at(i)->ShipToBuild);

				}
				break;
			}
			else
			{
				CentralPanel->CurrentObject = "";
			}
		}


		//border stuff doesn't work
		//BorderGraphic->Update(Factions,Systems);
		//window.draw(BorderGraphic->spr);
		


		//Technology tick
		if (GlobalTime > CurrentTimeStamp)
		{
			CurrentTimeStamp++;
			//the faction tick should happen at the start as well
			for (size_t i = 0; i < Factions.size(); i++)
			{
				Factions.at(i)->FactionTick(Systems,Factions);
			}
			//This is the yearly tick, monthly ticks are handled in function



			//This is also the tick for generic events
			if (GlobalTime>0.5)
			{
				TechnologyTick(DeltaTime, Factions, Systems);
				//Very important that the time setting is set to 0

				 //I might keep it this line solely so that players can focus on any task.



				//Currently disable events as they're annoying
				// EventHandler::RandomGenericEvent(PlayerFaction, Systems);
				// Paused = false;
				
			}

		}
		
		
		//render the top menu last thing as it's the very top ui element
		HorizMenu.Render(&window, &Factions, PlayerFaction);
		std::cout << "s" << Factions.size() << std::endl;

		GlobalClock.setString("YEAR: "+std::to_string(GlobalTime));
		window.draw(GlobalClock);
		window.display();






		TimeEnd = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration<double>(TimeEnd - TimeBegin).count();
		GlobalTime += DeltaTime* DialationSettings[TimeDialation] /100 * Paused;
		
	}

	

	return 0;
}
