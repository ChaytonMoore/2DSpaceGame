#include "System.h"
#include <fstream>

std::vector<std::string> System::SplitString(std::string s, char delimiter)
{
	std::vector<std::string> out;
	out.push_back("");
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s.at(i)==delimiter)
		{
			out.push_back("");
		}
		else
		{
			out.back().push_back(s.at(i));
		}
	}

	

	return out;
}



void System::RenderSystem(sf::RenderWindow* window, sf::Vector2i CameraOffset, TextureStore* TStore)
{
	//Still a minor memory leak TODO, might just be more things created
	//TODO renders mulitple ships building over each other

	LocalPosition = Location - CameraOffset;
	Sprite.setPosition(sf::Vector2f(LocalPosition));
	window->draw(Sprite);
	SystemName.setPosition(sf::Vector2f(LocalPosition)+sf::Vector2f(32,64));
	window->draw(SystemName);

	bool inside;
	std::vector<std::string> existingEmblems;
	for (size_t i = 0; i < SystemShips.size(); i++)
	{
		inside = false;
		for (size_t j = 0; j < existingEmblems.size(); j++)
		{
			if (existingEmblems.at(j)==SystemShips.at(i)->OwningFaction->FactionName)
			{
				inside = true;
				
			}

		}
		if (!inside)
		{
			existingEmblems.push_back(SystemShips.at(i)->OwningFaction->FactionName);
			FleetsEmblems.push_back(new sf::Sprite);
			FleetsEmblems.back()->setPosition(sf::Vector2f(LocalPosition) + sf::Vector2f(32, -32));

			if (SystemShips.at(i)->OwningFaction->FactionName == "Human")
			{
				
				FleetsEmblems.back()->setTexture(*TStore->HumanEmblemT);
			}
		}

	}

	for (size_t i = 0; i < FleetsEmblems.size(); i++)
	{
		window->draw(*FleetsEmblems.at(i));
	}

	while (!FleetsEmblems.empty()) delete FleetsEmblems.front(), FleetsEmblems.pop_back();
	

}

int System::ClickedOn()
{
	std::cout << "system is clicked";

	return 1;
}

bool System::LoadSystem(std::string Name) 
{

	Planet* temp;


	//an entry for each planet using a txt
	// planet class, size, population, arable land
	std::string Line;
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/" + Name);
	std::vector<std::string> split;


	TextFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");
	SystemName.setFont(TextFont);
	SystemName.setString(SplitString(Name,'.').at(0));
	SystemName.setCharacterSize(16);


	int tempI;

	while (std::getline (File,Line))
	{
		if (Line.size() > 0)
		{


			Planets.push_back(new Planet(0, PlanetClasses::J));
			temp = Planets.back();

			temp->PlanetSprite = new sf::Sprite;



			split = SplitString(Line, ',');
			
			if (split.at(0) == "D")
			{
				temp->PlanetType = PlanetClasses::D;


				std::cout << "a";
			}

			
			if (split.at(0) == "H")
			{
				temp->PlanetType = PlanetClasses::H;
			}
			if (split.at(0) == "J")
			{
				temp->PlanetType = PlanetClasses::J;
			}
			if (split.at(0) == "K")
			{
				temp->PlanetType = PlanetClasses::K;
			}
			if (split.at(0) == "L")
			{
				temp->PlanetType = PlanetClasses::L;
			}
			if (split.at(0) == "M")
			{
				temp->PlanetType = PlanetClasses::M;
			}
			if (split.at(0) == "N")
			{
				temp->PlanetType = PlanetClasses::N;
			}
			if (split.at(0) == "R")
			{
				temp->PlanetType = PlanetClasses::R;
			}
			if (split.at(0) == "T")
			{
				temp->PlanetType = PlanetClasses::T;
			}
			if (split.at(0) == "Y")
			{
				temp->PlanetType = PlanetClasses::Y;
			}
			


			temp->PlanetSize = std::stof(split[1]);
			temp->Population = std::stof(split[2]);
			temp->ArableLand = std::stoi(split[3]);

			//TODO : perhaps make it so that other things than human can start of as player faction
			

		}

	}



	return true;
}




std::vector<System*> LoadSystems(Faction* PlayerFaction)
{

	std::vector<System*> output;
	//firstly load the address document
	std::string Line;
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Systems.txt");
	std::vector<std::string> split;


	while (std::getline(File, Line))
	{
		split = System::SplitString(Line,',');
		output.push_back(new System(sf::Vector2i(std::stoi(split.at(1)),std::stoi(split.at(2))),split.at(0)));
		output.back()->LoadSystem(split.at(0));
		//output.back()->AddUIPlanets();
		if (split.at(3)=="Human")
		{
			output.back()->OwningFaction = PlayerFaction;
		}
		//TODO ^ Make it so that other factions than humans can start with a system



	}


	return output;
}



void System::EventTick(double DeltaTime, std::vector<FleetStruct*> FreeFleets)
{
	for (size_t i = 0; i < ShipBuildQueue.size(); i++)
	{
		ShipBuildQueue.at(i).Remaining -= DeltaTime/100;
		if (ShipBuildQueue.at(i).Remaining<0)
		{
			SystemShips.push_back(new ShipInstance(ShipBuildQueue.at(i).Type,OwningFaction));
			ShipBuildQueue.erase(ShipBuildQueue.begin()+i);
			
		}
	}

	//Now handle the fleets in the system
	


}
