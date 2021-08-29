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
				std::cout << FactionShips.back()->Name << " " << FactionShips.back()->Colonisable.back() << std::endl;
			}

			FactionShips.back()->Crew = std::stoi(split.at(13));
			FactionShips.back()->ConstructTime = std::stof(split.at(14));
			col = split.at(15);
			for (size_t i = 0; i < col.size(); i++)
			{
				FactionShips.back()->Terraformable.push_back(ShipType::FromChar(col.at(i)));

			}
			
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



void ClickHandle(std::vector<UIElement*> Potentials, PlanetUIElement* PlanetUI)
{
	sf::Vector2i MouseLoc = sf::Mouse::getPosition();

	
	for (size_t i = 0; i < Potentials.size(); i++)
	{

			

		//re arrange the if statements
		if (Potentials.at(i) != nullptr)
		{

			std::cout << i;
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

							CurrentFleet->TargetSystem = (System*)(Potentials.at(i));
							CurrentSelect = SelectEnum::Basic;
							std::cout << "Basic" << std::endl;

							for (size_t j = 0; j < CurrentFleet->Ships.size(); j++)
							{
								std::cout <<" "<<CurrentFleet->Ships.size() <<"s"<< std::endl;

								if (CurrentFleet->Ships.at(j)) //TODO just leaving the ships in might cause a memory leak
								{
									CurrentSystem->RemoveShipInstance(CurrentFleet->Ships.at(j));
								}
							}
							FreeFleets.push_back(CurrentFleet);
							CurrentFleet = nullptr;
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
			else if (dynamic_cast<ShipBuildButton*>(Potentials.at(i)) != nullptr)
			{
				if (dynamic_cast<ShipBuildButton*>(Potentials.at(i))->IsClicked(MouseLoc, PlanetUI->CurrentScroll))
				{

					dynamic_cast<ShipBuildButton*>(Potentials.at(i))->ClickedOn();

				}
			}
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

void SetUpPlanetSprites(System* system, std::vector<sf::Texture*> PlanetTextures)
{


	
	for (size_t i = 0; i < system->Planets.size(); i++)
	{
		system->Planets.at(i)->PlanetSprite->setTexture(*PlanetTextures.at(0));

		system->Planets.at(i)->PlanetSprite->setScale(sf::Vector2f(pow(system->Planets.at(i)->PlanetSize,0.05)*0.75, pow(system->Planets.at(i)->PlanetSize, 0.05) * 0.75));
		system->Planets.at(i)->PlanetSprite->setPosition(sf::Vector2f(1720,32+i*128));
		system->Planets.at(i)->LocalPosition = (sf::Vector2i(1720, 32 + i * 128));
	}
}


int ShipBuildButton::ClickedOn()
{


	std::cout << ShipToBuild->Name << "build" << std::endl;
	
	CurrentSystem->ShipBuildQueue.push_back(*new ShipConstructData());
	CurrentSystem->ShipBuildQueue.back().Type = ShipToBuild;
	CurrentSystem->ShipBuildQueue.back().Remaining = ShipToBuild->ConstructTime;
	CurrentSystem->ShipBuildQueue.back().UIElement = new Button(sf::Vector2i(180,120), ShipToBuild->Name);

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

void LoadFactionData(Faction* Faction, std::vector<System*>* Systems)
{
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Profiles/" + Faction->FactionName + ".txt");
	std::vector<std::string> split;
	std::string a;
	File >> a;
	split = System::SplitString(a,',');

	int pop;


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

void TechnologyTick(double DeltaTime,std::vector<Faction*> Factions, std::vector<System*> Systems)
{
	//Should only happen once a year

	//For each faction we'll just the same variables
	int TotalResearch;
	srand(time(NULL));

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
		if (TotalResearch*Factions.at(i)->MedicalCoef > Factions.at(i)->MedicalTech * 50) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->MedicalTech++;
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->MedicalCoef > rand()% Factions.at(i)->MedicalTech * 50)
			{
				Factions.at(i)->MedicalTech++;
			}
		}

		
		if (TotalResearch * Factions.at(i)->MiningCoef > Factions.at(i)->MiningTech * 50) //Might want to increase this at a different rate perhaps by a power of 2.
		{
			//If the tech is higher it'll just advance the tech
			Factions.at(i)->MiningTech++;
		}
		else
		{
			//Random chance it is advanced
			if (TotalResearch * Factions.at(i)->MiningCoef > rand() % Factions.at(i)->MiningTech * 50)
			{
				Factions.at(i)->MiningTech++;
			}
		}


	}

	


}


int main()
{
	
	
	CentralPanel = new DetailsPanel;

	//The very first thing is to load the generic textures.
	
	TextureStore* TStore = new TextureStore;
	TStore->MassLoadTextures();

	

	//Load the planet focus texture
	LoadImage(&PlanetFocus,"PlanetSelect.png");


	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Fullscreen);
	sf::Sprite Background;
	LoadImage(&Background, "Background.png");
	


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
		SetUpPlanetSprites(Systems.at(i), PlanetTextures);
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
							}

							//^Might be a memory leak

							for (size_t i = 0; i < PlanetUI->BuildableShips.size(); i++)
							{
								UIAspects.push_back(PlanetUI->BuildableShips.at(i));
							}

						}
						CurrentSystem->AddUIPlanets(&UIAspects);
						
					}
					else
					{
						//Remove shipsbuild buttons from UI aspects
						for (size_t i = 0; i < UIAspects.size(); i++)
						{		
							
								//Not the best or cleanest solution but it works
								for (size_t j = 0; j < PlanetUI->BuildableShips.size(); j++)
								{
									if (PlanetUI->BuildableShips.at(j) == UIAspects.at(i))
									{
										UIAspects.erase(UIAspects.begin()+i);
										std::cout << "erased" << std::endl;
										
									}

								}

						}

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
			Systems.at(i)->EventTick(DeltaTime*DialationSettings[TimeDialation],FreeFleets);
		}


		if (SystemTabOpen && CurrentSystem)
		{
			RenderPlanets(&window, CurrentSystem);
			PlanetUI->OwningPlanet = CurrentPlanet;
			PlanetUI->RenderPlanetUI(&window);


			RenderShipQueue(&window);
			TempFleet = SystemUI->RenderSystemUI(&window, CurrentSystem);
			if (TempFleet != nullptr)
			{
				std::cout << "fleet enum" << std::endl;
				CurrentFleet = TempFleet;
				CurrentSelect = SelectEnum::Fleet;
			}
			if (CurrentFleet != nullptr)
			{
				if (CurrentFleet->Ships.size() == 1)
				{
					ActionsUIShip->RenderShipActions(&window,CurrentFleet,CurrentSystem,CurrentPlanet, &CurrentSelect);
				}
			}
			
		}


		//Draw and tick free fleets
		for (size_t i = 0; i < FreeFleets.size(); i++)
		{
			FreeFleets.at(i)->RenderFleet(&window,CameraLocation);
			FreeFleets.at(i)->EventTick(DeltaTime* DialationSettings[TimeDialation]);

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
		CentralPanel->RenderPanel(&window);


		//Technology tick
		if (GlobalTime > CurrentTimeStamp)
		{
			CurrentTimeStamp++;
			TechnologyTick(DeltaTime, Factions, Systems);
		}
		
		
		


		GlobalClock.setString("YEAR: "+std::to_string(GlobalTime));
		window.draw(GlobalClock);
		window.display();






		TimeEnd = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration<double>(TimeEnd - TimeBegin).count();
		GlobalTime += DeltaTime* DialationSettings[TimeDialation] /100;
		
	}

	

	return 0;
}
