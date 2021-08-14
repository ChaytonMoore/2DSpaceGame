#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "Graphical.h"
#include "System.h"
#include "PlanetUIElement.h"

//#include "Faction.h"

sf::Vector2i CameraLocation = sf::Vector2i(0,0);

System* CurrentSystem;
Planet* CurrentPlanet;

double GlobalTime = 0;


//Factions 




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
			FactionShips.back()->Crew = std::stoi(split.at(13));
			FactionShips.back()->ConstructTime = std::stof(split.at(14));
			
		}
		
	}
}


//Might: make it so this works generically 
std::vector<ShipType*> GetBuildableShips(Faction* PlayerFaction)
{
	std::vector<ShipType*> output;
	//first check if the system is the same as the player faction
	if (PlayerFaction == CurrentSystem->OwningFaction)
	{
		bool valid;
		for (size_t i = 0; i < PlayerFaction->FactionShips.size(); i++)
		{

			valid = CurrentSystem->SimpleMetal > PlayerFaction->FactionShips.at(i)->RequiredMetal   &&   CurrentSystem->Titanium > PlayerFaction->FactionShips.at(i)->RequiredTitanium   &&   CurrentSystem->RareMetal > PlayerFaction->FactionShips.at(i)->RequiredRare;
			valid = valid && PlayerFaction->EngineTech >= PlayerFaction->FactionShips.at(i)->EngineTech && PlayerFaction->ShipbuildingTech >= PlayerFaction->FactionShips.at(i)->ShipTech && PlayerFaction->ShieldTech >= PlayerFaction->FactionShips.at(i)->ShieldTech && PlayerFaction->SensorTech >= PlayerFaction->FactionShips.at(i)->Sensortech && PlayerFaction->WeaponsTech >= PlayerFaction->FactionShips.at(i)->WeaponTech;

			if (valid)
			{
				output.push_back(PlayerFaction->FactionShips.at(i));
			}
		}
	}


	return output;
}

//*****End of Factions*******


int FindMostPopulated()
{
	int pop = 0;
	int current = 0;
	for (size_t i = 0; i < CurrentSystem->Planets.size(); i++)
	{
		if (CurrentSystem->Planets.at(0)->Population>pop)
		{
			pop = CurrentSystem->Planets.at(0)->Population;
			current = i;
		}
	}

	return current;

}



void ClickHandle(std::vector<UIElement*> Potentials)
{
	sf::Vector2i MouseLoc = sf::Mouse::getPosition();

	
	for (size_t i = 0; i < Potentials.size(); i++)
	{

			

		//re arrange the if statements

		if (dynamic_cast<System*>(Potentials.at(i)) != nullptr)
		{

			if (dynamic_cast<System*>(Potentials.at(i))->IsClicked(MouseLoc))
			{



				CurrentSystem = ((System*)(Potentials.at(i)));
				if (CurrentSystem->Planets.size() > 0)
				{
					CurrentPlanet = CurrentSystem->Planets.at(0);
				}
				((System*)(Potentials.at(i)))->ClickedOn();
			}
		}
		else if (dynamic_cast<Planet*>(Potentials.at(i)) != nullptr)
		{
			if (dynamic_cast<Planet*>(Potentials.at(i))->IsClicked(MouseLoc))
			{

				CurrentPlanet = ((Planet*)(Potentials.at(i)));
			}
		}
		else if(dynamic_cast<ShipBuildButton*>(Potentials.at(i)) != nullptr)
		{
			if (dynamic_cast<ShipBuildButton*>(Potentials.at(i))->IsClicked(MouseLoc))
			{
				
				dynamic_cast<ShipBuildButton*>(Potentials.at(i))->ClickedOn();
				
			}
		}


			


	}




}



void RenderPlanets(sf::RenderWindow* window,System* system)
{
	for (size_t i = 0; i < system->Planets.size(); i++)
	{
		window->draw(*system->Planets.at(i)->PlanetSprite);
		
	}


}

void RenderShipQueue(sf::RenderWindow* window)
{
	for (size_t i = 0; i < CurrentSystem->ShipBuildQueue.size(); i++)
	{
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



int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game");
	sf::Sprite Background;
	LoadImage(&Background, "Background.png");
	


	//Create the factions


	Faction* PlayerFaction = new Faction;
	PlayerFaction->LoadShips();

	
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





	std::chrono::high_resolution_clock::time_point TimeBegin = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point TimeEnd = std::chrono::high_resolution_clock::now();
	double DeltaTime = std::chrono::duration<double, std::milli>(TimeEnd - TimeBegin).count();
	sf::Text GlobalClock;
	sf::Font ClockFont;
	ClockFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/Protector.ttf");
	GlobalClock.setFont(ClockFont);
	GlobalClock.setCharacterSize(24);

	int adddd = 0;
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
						
						
						for (size_t i = 0; i < GetBuildableShips(PlayerFaction).size(); i++)
						{
							PlanetUI->BuildableShips.push_back(new ShipBuildButton(sf::Vector2i(30, 200), GetBuildableShips(PlayerFaction).at(i)->Name, GetBuildableShips(PlayerFaction).at(i)));
						}

						//^Might be a memory leak
						
						for (size_t i = 0; i < PlanetUI->BuildableShips.size(); i++)
						{
							UIAspects.push_back(PlanetUI->BuildableShips.at(i));
						}


						CurrentSystem->AddUIPlanets(&UIAspects);
						
					}
					else
					{
						PlanetUI->BuildableShips.clear();

						// TODO: Remove UI planets when planet viewer is closed
					}
					SystemTabOpen = !SystemTabOpen;
				}

			}


			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					ClickHandle(UIAspects);
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
			Systems.at(i)->RenderSystem(&window, CameraLocation);
		}


		if (SystemTabOpen && CurrentSystem)
		{
			RenderPlanets(&window, CurrentSystem);
			PlanetUI->OwningPlanet = CurrentPlanet;
			PlanetUI->RenderPlanetUI(&window);


			RenderShipQueue(&window);
			
		}

		


		GlobalClock.setString("YEAR: "+std::to_string(GlobalTime));
		window.draw(GlobalClock);
		window.display();






		TimeEnd = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration<double>(TimeEnd - TimeBegin).count();
		GlobalTime += DeltaTime/100;
		
	}

	

	return 0;
}
