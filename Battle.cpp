#include "Battle.h"



sf::Vector2f GetDirection(sf::Vector2f Origin, sf::Vector2f Target)
{
	float x = Target.x - Origin.x;
	float y = Target.y - Origin.y;
	float l = sqrt(x * x + y * y);
	return sf::Vector2f(x/l,y/l);
}

float GetDistance(sf::Vector2f Origin, sf::Vector2f Target)
{
	float x = Target.x - Origin.x;
	float y = Target.y - Origin.y;
	return sqrt(x+y);

}


void FireKinetic(BattleShip* Parent, BattleShip* Target)
{
	Parent->WeaponElements.push_back(new KineticType(Parent->Kinetic));
}



void BattleShip::EventTick(std::vector<BattleShip*> Enemyships)
{
	//Get nearest for now we'll just use the first element
	sf::Vector2f Delta = GetDirection(Location, Enemyships.at(0)->Location);
	float Distance = GetDistance(Location, Enemyships.at(0)->Location);

	Delta.x = Delta.x * 0.005 * Speed;
	Delta.y = Delta.y * 0.005 * Speed;
	Location += Delta;

	//Weapon Firing

	//KineticFiring
	if (Distance<Kinetic->Range && KineticTickDelay<0)
	{
		KineticTickDelay = 100;

	}
	


	//Set the position of the sprite
	Sprite.setPosition(Location);

	//Decrease weapon ticks
	KineticTickDelay--;
	BeamTickDelay--;
	TorpedoTickDelay--;
}





std::vector<sf::Texture*> GetShipTypes(std::vector<ShipType*> input, std::vector<ShipType*> input2)
{
	std::vector<ShipType*> out;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (std::find(out.begin(), out.end(), input.at(i)) != out.end())
		{
			out.push_back(input.at(i));
		}
	}
	for (size_t i = 0; i < input2.size(); i++)
	{
		if (std::find(out.begin(), out.end(), input2.at(i)) != out.end())
		{
			out.push_back(input2.at(i));
		}
	}
	//^This isn't working right now
	//So just add the first thing automatically
	out.push_back(input.at(0));




	sf::Image* TempImage;
	std::vector<sf::Texture*> output;
	for (size_t i = 0; i < out.size(); i++)
	{
		TempImage = new sf::Image;
		TempImage->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Battle/" + out.at(i)->Name+".png");

		output.push_back(new sf::Texture);
		output.back()->loadFromImage(*TempImage);
	}


	return output;
}





BattleReturn Battle(Faction* Faction1, Faction* Faction2, std::vector<ShipType*> Faction1Ships, std::vector<ShipType*> Faction2Ships)
{
	std::vector<sf::Texture*> Textures;
	
	Textures = GetShipTypes(Faction1Ships,Faction2Ships);
	std::vector<BattleShip*> Ships1;
	std::vector<BattleShip*> Ships2;
	for (size_t i = 0; i < Faction1Ships.size(); i++)
	{
		Ships1.push_back(new BattleShip(Faction1Ships.at(i), Textures,true));
		Ships1.back()->Location = sf::Vector2f(130,10+i*100);
	}
	for (size_t i = 0; i < Faction2Ships.size(); i++)
	{
		Ships2.push_back(new BattleShip(Faction2Ships.at(i), Textures,false));
		Ships2.back()->Location = sf::Vector2f(1750, 10 + i * 100);
	}

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Battle", sf::Style::Fullscreen);

	
	BattleReturn output;

	//Summer school stock thing delte later
	/*
	std::ifstream file("C:/Users/User/Documents/2DSpaceGame/Assets/StockData.csv");
	std::string data;
	file >> data;
	std::vector<std::string> temp;
	temp = System::SplitString(data, ',');

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};
	std::vector<float> points;
	for (size_t i = 0; i < temp.size(); i++)
	{
		points.push_back((std::stof(temp.at(i))-138)*100);
	}
	*/

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
		
		/*
		for (size_t i = 0; i < points.size()-1; i++)
		{
			line[0].position = sf::Vector2f(i*1920/points.size(),points.at(i));
			line[1].position = sf::Vector2f((i+1) * 1920 / points.size(), points.at(i+1));

			window.draw(line, 2, sf::Lines);
		}
		*/
		for ( size_t i = 0;  i < Ships1.size();  i++)
		{
			window.draw(Ships1.at(i)->Sprite);
			Ships1.at(i)->EventTick(Ships2);
		}

		for (size_t i = 0; i < Ships2.size(); i++)
		{
			
			window.draw(Ships2.at(i)->Sprite);
			Ships2.at(i)->EventTick(Ships1);
		}


		window.display();
	}


	return output;
}
