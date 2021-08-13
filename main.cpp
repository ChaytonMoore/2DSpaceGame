#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Graphical.h"
#include "System.h"
#include "PlanetUIElement.h"

//#include "Faction.h"

sf::Vector2i CameraLocation = sf::Vector2i(0,0);

System* CurrentSystem;
Planet* CurrentPlanet;



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

			

			FactionShips.push_back(*new ShipType);

			FactionShips.back().Name = split.at(0);
			FactionShips.back().Length = std::stoi(split.at(1));
			FactionShips.back().Attack = std::stoi(split.at(2));
			FactionShips.back().Hull = std::stoi(split.at(3));
			FactionShips.back().Shields = std::stoi(split.at(4));
			FactionShips.back().Speed = std::stoi(split.at(5));
			FactionShips.back().Range = std::stoi(split.at(6));
			FactionShips.back().ShipTech = std::stoi(split.at(7));
			FactionShips.back().WeaponTech = std::stoi(split.at(8));
			FactionShips.back().ShieldTech = std::stoi(split.at(9));
			FactionShips.back().EngineTech = std::stoi(split.at(10));
			FactionShips.back().Sensortech = std::stoi(split.at(11));

			col = split.at(12);
			FactionShips.back().Crew = std::stoi(split.at(13));
			FactionShips.back().ConstructTime = std::stof(split.at(14));

		}
	}
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
		if (Potentials.at(i)->IsClicked(MouseLoc))
		{
			



			if (dynamic_cast<System*>(Potentials.at(i)) != nullptr)
			{
				CurrentSystem = ((System*)(Potentials.at(i)));
				if (CurrentSystem->Planets.size()>0)
				{
					CurrentPlanet = CurrentSystem->Planets.at(0);
				}
				((System*)(Potentials.at(i)))->ClickedOn();
			}
			else if (dynamic_cast<Planet*>(Potentials.at(i)) != nullptr)
			{
				
				CurrentPlanet = ((Planet*)(Potentials.at(i)));
			}

			break;
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


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game");
	sf::Sprite Background;
	LoadImage(&Background, "Background.png");
	
	
	System* Sol = new System(sf::Vector2i(960,540),"Sol");
	std::vector<UIElement*> UIAspects;
	UIAspects.push_back(Sol);
	Sol->LoadSystem("Sol.csv");
	Sol->AddUIPlanets(&UIAspects);
	
	std::vector<sf::Texture*> PlanetTextures;
	std::vector<sf::Image*> PlanetImages;


	bool SystemTabOpen = false;
	
	//Load the planets
	//Must make sure that the other planets are loaded

	PlanetImages.push_back(new sf::Image);
	PlanetImages.at(0)->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/KPlanet.png");

	PlanetTextures.push_back(new sf::Texture);
	PlanetTextures.at(0)->loadFromImage(*PlanetImages.at(0));
	SetUpPlanetSprites(Sol,PlanetTextures);

	PlanetUIElement* PlanetUI = new PlanetUIElement;
	PlanetUI->SetUpPlanetUI();



	//Create the factions
	

	Faction* PlayerFaction = new Faction;
	PlayerFaction->LoadShips();



	while (window.isOpen())
	{
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
						PlanetUI->BuildableShips.push_back(new ShipBuildButton(sf::Vector2i(30,200),"Sputnik Class",PlayerFaction->FactionShips.at(0)));


						
					}
					else
					{

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
		
		Sol->RenderSystem(&window,CameraLocation);

		if (SystemTabOpen && CurrentSystem)
		{
			RenderPlanets(&window, CurrentSystem);
			PlanetUI->OwningPlanet = CurrentPlanet;
			PlanetUI->RenderPlanetUI(&window);
			
		}

		


		window.display();

	}

	

	return 0;
}
