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



void System::RenderSystem(sf::RenderWindow* window, sf::Vector2i CameraOffset)
{

	LocalPosition = Location - CameraOffset;
	Sprite.setPosition(sf::Vector2f(LocalPosition));
	window->draw(Sprite);
	SystemName.setPosition(sf::Vector2f(LocalPosition)+sf::Vector2f(32,64));
	window->draw(SystemName);

	

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

			

		}

	}



	return true;
}
