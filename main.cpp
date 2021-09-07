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



//#include "Faction.h"

sf::Vector2i CameraLocation = sf::Vector2i(0,0);

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



void Faction::LoadShips()
{
	std::string Line;
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Ships.csv");
	std::vector<std::string> split;

	std::string col;
	
	while (std::getline(File, Line)) 
	{
		split = System::SplitString(Line, ',');
		if (split.at(0) != "Title" && split.size()>12)
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
				FactionShips.back()->Colonisable.push_back(ShipType::FromChar(col.at(i)));
				
			}

			FactionShips.back()->Crew = std::stoi(split.at(13));
			FactionShips.back()->ConstructTime = std::stof(split.at(14));
			FactionShips.back()->RequiredMetal = std::stof(split.at(15));
			FactionShips.back()->RequiredTitanium = std::stof(split.at(16));
			FactionShips.back()->RequiredRare = std::stof(split.at(17));

			col = split.at(18);
			for (size_t i = 0; i < col.size(); i++)
			{
				FactionShips.back()->Terraformable.push_back(ShipType::FromChar(col.at(i)));

			}

			FactionShips.back()->type = split.at(19);


			
			
		}
		
	}
}


//Might: make it so this works generically 
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

float SystemDistance(sf::Vector2f a, sf::Vector2f b)
{
	float x,y;
	x = abs(a.x - b.x);
	
	y = abs(a.y - b.y);
	
	
	
	return (sqrt(x * x + y * y)/75);
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
	std::cout << " " << CurrentSystem->SimpleMetal << " " << ShipToBuild->RequiredMetal << std::endl;
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

void PlanetUIElement::ResetBuildableships()
{

	//Clear the potentials of uneeded elements


	while (!BuildableShips.empty()) delete BuildableShips.back(), BuildableShips.pop_back();

	

	for (size_t i = 0; i < GetBuildableShips(CurrentSystem->OwningFaction).size(); i++)
	{
		BuildableShips.push_back(new ShipBuildButton(sf::Vector2i(30, 200), GetBuildableShips(CurrentSystem->OwningFaction).at(i)->Name, GetBuildableShips(CurrentSystem->OwningFaction).at(i)));
	}



	//Add to potentials

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

void LoadFactionData(Faction* Faction, std::vector<System*>* Systems)
{
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Profiles/" + Faction->FactionName + ".txt");
	std::vector<std::string> split;
	std::string a;
	File >> a;
	split = System::SplitString(a,',');

	int pop;

	try {

		for (size_t i = 0; i < Systems->size(); i++)
		{
			Systems->at(i)->SimpleMetal = std::stoi(split.at(0));
			Systems->at(i)->Titanium = std::stoi(split.at(1));
			Systems->at(i)->RareMetal = std::stoi(split.at(2));

			if (Systems->at(i)->OwningFaction == Faction)
			{
				//Now find the most populated planet
				pop = FindMostPopulated(Systems->at(i));
				Systems->at(i)->Planets.at(pop)->Medical = MedicalLevels(std::stoi(split.at(3)));
				Systems->at(i)->Planets.at(pop)->Mining = MiningLevels(std::stoi(split.at(4)));
				Systems->at(i)->Planets.at(pop)->Shipyard = ShipyardLevels(std::stoi(split.at(5)));
				Systems->at(i)->Planets.at(pop)->Lab = ScienceLevels(std::stoi(split.at(6)));

			}

		}

		Faction->MedicalCoef = std::stof(split.at(7));
		Faction->MiningCoef = std::stof(split.at(8));
		Faction->Farmingcoef = std::stof(split.at(9));

		Faction->ConstructionCoef = std::stof(split.at(10));
		Faction->ShipBuildingCoef = std::stof(split.at(11));
		Faction->WeaponsCoef = std::stof(split.at(12));

		Faction->ShieldCoef = std::stof(split.at(13));
		Faction->EngineCoef = std::stof(split.at(14));
		Faction->SensorCoef = std::stof(split.at(15));
	}
	catch(std::out_of_range)
	{
		//throw std::out_of_range("Insufficient number of values in profile of: ");
		std::string message;
		message = "Insufficient number of values in profile of: " + Faction->FactionName;
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

void TechnologyTick(double DeltaTime,std::vector<Faction*> Factions, std::vector<System*> Systems)
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
			if (Systems.at(j)->OwningFaction == Factions.at(i))
			{
				for (size_t n = 0; n < Systems.at(j)->Planets.size(); n++)
				{
					//The research level is proportional to the lab level squared to put priority on the larger systems
					TotalResearch += pow((int)Systems.at(j)->Planets.at(n)->Lab, 2); 



				}
			}
		}
		
		//Each of the techs have its own value 
		if (TotalResearch*Factions.at(i)->MedicalCoef > Factions.at(i)->MedicalTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->MedicalTech++;
			EventHandler::TechAdvanceNotification("Medical", Factions.at(i)->MedicalTech);
		}
		else
		{
			//Random chance it is advanced
			
			if (TotalResearch * Factions.at(i)->MedicalCoef -5 > rand() % (int(pow(Factions.at(i)->MedicalTech, 2)+1) * 16))
			{
				
				Factions.at(i)->MedicalTech++;
				EventHandler::TechAdvanceNotification("Medical", Factions.at(i)->MedicalTech);
			}
		}
		
		
		if (TotalResearch * Factions.at(i)->MiningCoef > Factions.at(i)->MiningTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->MiningTech++;
			EventHandler::TechAdvanceNotification("Mining", Factions.at(i)->MiningTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->MiningCoef -5 > rand() % (int(pow(Factions.at(i)->MiningTech, 2)+1) * 16))
			{
				
				Factions.at(i)->MiningTech++;
				EventHandler::TechAdvanceNotification("Mining", Factions.at(i)->MiningTech);
			}
		}

		if (TotalResearch * Factions.at(i)->Farmingcoef > Factions.at(i)->FarmingTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->FarmingTech++;
			EventHandler::TechAdvanceNotification("Farming", Factions.at(i)->FarmingTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->Farmingcoef-5 > rand() % (int(pow(Factions.at(i)->FarmingTech, 2)+1) * 16))
			{
				Factions.at(i)->FarmingTech++;
				EventHandler::TechAdvanceNotification("Farming", Factions.at(i)->FarmingTech);
			}
		}

		if (TotalResearch * Factions.at(i)->ConstructionCoef > Factions.at(i)->ConstructionTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->ConstructionTech++;
			EventHandler::TechAdvanceNotification("Contruction", Factions.at(i)->ConstructionTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->ConstructionCoef-5 > rand() % (int(pow(Factions.at(i)->ConstructionTech, 2)+1) * 16))
			{
				Factions.at(i)->ConstructionTech++;
				EventHandler::TechAdvanceNotification("Contruction", Factions.at(i)->ConstructionTech);
			}
		}

		if (TotalResearch * Factions.at(i)->ShipBuildingCoef > Factions.at(i)->ShipbuildingTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->ShipbuildingTech++;
			EventHandler::TechAdvanceNotification("Ship Building", Factions.at(i)->ShipbuildingTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->ShipBuildingCoef-5 > rand() % (int(pow(Factions.at(i)->ShipbuildingTech, 2)+1) * 16))
			{
				Factions.at(i)->ShipbuildingTech++;
				EventHandler::TechAdvanceNotification("Ship Building", Factions.at(i)->ShipbuildingTech);
			}
		}

		if (TotalResearch * Factions.at(i)->WeaponsCoef > Factions.at(i)->WeaponsTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->WeaponsTech++;
			EventHandler::TechAdvanceNotification("Weapon", Factions.at(i)->WeaponsTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->WeaponsCoef-5 > rand() % (int(pow(Factions.at(i)->WeaponsTech, 2)+1) * 16))
			{
				Factions.at(i)->WeaponsTech++;
				EventHandler::TechAdvanceNotification("Weapon", Factions.at(i)->WeaponsTech);
			}
		}


		if (TotalResearch * Factions.at(i)->ShieldCoef > Factions.at(i)->ShieldTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->ShieldTech++;
			EventHandler::TechAdvanceNotification("Shield", Factions.at(i)->ShieldTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->ShieldCoef-5 > rand() % int(pow(Factions.at(i)->ShieldTech, 2)+1 * 16))
			{
				Factions.at(i)->ShieldTech++;
				EventHandler::TechAdvanceNotification("Shield", Factions.at(i)->ShieldTech);
			}
		}


		if (TotalResearch * Factions.at(i)->EngineCoef > Factions.at(i)->EngineTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->EngineTech++;
			EventHandler::TechAdvanceNotification("Engine", Factions.at(i)->EngineTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->EngineCoef-5 > rand() % int(pow(Factions.at(i)->EngineTech, 2)+1 * 16))
			{
				Factions.at(i)->EngineTech++;
				EventHandler::TechAdvanceNotification("Engine", Factions.at(i)->EngineTech);
			}
		}

		if (TotalResearch * Factions.at(i)->SensorCoef > Factions.at(i)->SensorTech * TechConst) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->SensorTech++;
			EventHandler::TechAdvanceNotification("Sensor", Factions.at(i)->SensorTech);
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->SensorCoef-5 > rand() % int(pow(Factions.at(i)->SensorTech, 2)+1 * 16))
			{
				Factions.at(i)->SensorTech++;
				EventHandler::TechAdvanceNotification("Sensor", Factions.at(i)->SensorTech);
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


int main()
{
	
	
	//MenuRender();
	CentralPanel = new DetailsPanel;

	//The very first thing is to load the generic textures.
	
	TextureStore* TStore = new TextureStore;
	TStore->MassLoadTextures();

	

	//Load the planet focus texture
	LoadImageSF(&PlanetFocus,"PlanetSelect.png");
	

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Fullscreen); //This should be fullscreen but for testing windowed is easier
	sf::Sprite Background;
	LoadImageSF(&Background, "Background.png");
	


	//Create the factions


	Faction* PlayerFaction = new Faction("Human");
	PlayerFaction->LoadShips();

	std::vector<Faction*> Factions;
	Factions.push_back(PlayerFaction);

	

	
	//System* Sol = new System(sf::Vector2i(960,540),"Sol");
	std::vector<UIElement*> UIAspects;
	
	//UIAspects.push_back(Sol);
	//Sol->LoadSystem("Sol.csv");


	std::vector<System*> Systems;
	Systems = LoadSystems(PlayerFaction);
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
	LoadFactionData(PlayerFaction, &Systems);


	

	while (window.isOpen())
	{
		TimeBegin = std::chrono::high_resolution_clock::now();

		

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

				if (event.key.code == sf::Keyboard::W)
				{
					CameraLocation.y -= 16;
				}

				if (event.key.code == sf::Keyboard::S)
				{
					CameraLocation.y += 16;
				}

				if (event.key.code == sf::Keyboard::D)
				{
					CameraLocation.x += 16;
				}

				if (event.key.code == sf::Keyboard::A)
				{
					CameraLocation.x -= 16;
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
							for (size_t i = 0; i < GetBuildableShips(PlayerFaction).size(); i++)
							{
								PlanetUI->BuildableShips.push_back(new ShipBuildButton(sf::Vector2i(30, 200), GetBuildableShips(PlayerFaction).at(i)->Name, GetBuildableShips(PlayerFaction).at(i)));

								std::cout << GetBuildableShips(PlayerFaction).at(i)->Name << std::endl;
							}

							//^Might be a memory leak

							//for (size_t i = 0; i < PlanetUI->BuildableShips.size(); i++)
							//{
							//	UIAspects.push_back(PlanetUI->BuildableShips.at(i));

							//}

						}
						CurrentSystem->AddUIPlanets(&UIAspects);
						
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
				PlanetUI->CurrentScroll += event.mouseWheel.delta*16;
				
			}

		}
		window.clear();

		//Here we will paste the background
		window.draw(Background);
		
		//Sol->RenderSystem(&window,CameraLocation);
		for (size_t i = 0; i < Systems.size(); i++)
		{
			Systems.at(i)->RenderSystem(&window, CameraLocation, TStore, PlayerFaction);
			Systems.at(i)->EventTick(DeltaTime*DialationSettings[TimeDialation]*Paused,FreeFleets);
		}


		if (SystemTabOpen && CurrentSystem)
		{
			//reset only if the current metals is insufficient
			if(GetShouldResetShipBuild(PlanetUI->BuildableShips, CurrentSystem)) //passing system might not be necessary but keep it
			{
				PlanetUI->ResetBuildableships();
			}
			 


			RenderPlanets(&window, CurrentSystem);
			PlanetUI->OwningPlanet = CurrentPlanet;
			PlanetUI->RenderPlanetUI(&window,CurrentSystem->OwningFaction, (PlayerFaction == CurrentSystem->OwningFaction));

			
			RenderShipQueue(&window);
			TempFleet = SystemUI->RenderSystemUI(&window, CurrentSystem);
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
			FreeFleets.at(i)->RenderFleet(&window,CameraLocation);
			FreeFleets.at(i)->EventTick(DeltaTime* DialationSettings[TimeDialation]*Paused);

			if (FreeFleets.at(i)->Distance(FreeFleets.at(i)->Location,sf::Vector2f(FreeFleets.at(i)->TargetSystem->Location))<0.1)
			{
				//Add the ships to the system
				for (size_t j = 0; j < FreeFleets.at(i)->Ships.size(); j++)
				{
					FreeFleets.at(i)->TargetSystem->SystemShips.push_back(FreeFleets.at(i)->Ships.at(j));

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
					CentralPanel->Reload();

				}
				break;
			}
			else
			{
				CentralPanel->CurrentObject = "";
			}
		}



		


		//Technology tick
		if (GlobalTime > CurrentTimeStamp)
		{
			CurrentTimeStamp++;
			

			//This is also the tick for generic events
			if (GlobalTime>0.5)
			{
				TechnologyTick(DeltaTime, Factions, Systems);
				//Very important that the time setting is set to 0

				 //I might keep it this line solely so that players can focus on any task.
				EventHandler::RandomGenericEvent(PlayerFaction, Systems);
				Paused = false;
			}

		}
		
		
		


		GlobalClock.setString("YEAR: "+std::to_string(GlobalTime));
		window.draw(GlobalClock);
		window.display();






		TimeEnd = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration<double>(TimeEnd - TimeBegin).count();
		GlobalTime += DeltaTime* DialationSettings[TimeDialation] /100 * Paused;
		
	}

	

	return 0;
}
