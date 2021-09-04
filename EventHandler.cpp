#include "EventHandler.h"
#include "Button.h"
#include <iostream>



std::string EventTextModify(std::string text, std::vector<std::string> args)
{
	
	int idx = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		if (text[i]=='#')
		{
			text.insert(i,args.at(idx));
			i += args.at(idx).size();
			idx++;
			text.erase(text.begin()+i);
		}

		//split the string up
		if (i%60 == 0)
		{
			text.insert(i,"\n");
			
		}
	}

	
	return text;
}





void EventHandler::Run(std::string name, std::vector<std::string> strArgs)
{
	window = (new sf::RenderWindow(sf::VideoMode(800, 600), "Game",sf::Style::None));
	

	sf::Image tI;
	tI.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Events/"+name+".png");
	sf::Texture tT; //TODO make it so that a text file can be used to redirect to an image with a different name.
	tT.loadFromImage(tI);
	Display.setTexture(tT);
	sf::Font font;
	font.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");


	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Events/"+name+".txt");
	std::string TempString;

	
	std::getline(File, TempString);
	sf::Text Title(TempString,font);
	

	

	std::getline(File, TempString);
	TempString = EventTextModify(TempString,strArgs);
	
	sf::Text Body(TempString,font);

	Title.setCharacterSize(24);
	Title.setStyle(sf::Text::Bold);
	Title.setFillColor(sf::Color::Black);
	Title.setPosition(300,0);

	Body.setFillColor(sf::Color::Black);
	Body.setPosition(10,400);
	Body.setCharacterSize(12);

	Button* Exit = new Button(sf::Vector2i(0,0),"Exit");
	

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				//TODO this is only a quick method of exiting
				if (event.key.code == sf::Keyboard::Escape)
				{
					window->close();
				}
			}

		}


		window->clear();
		window->draw(Display);
		window->draw(Title);
		window->draw(Body);
		Exit->Render(window);
		window->display();

		if (Exit->isClicked(sf::Mouse::getPosition()+sf::Vector2i(-560,-240)))
		{
			window->close();
		}
		
	}


	
}



System* Chosensystem(Faction* fac,std::vector<System*> systems)
{
	std::vector<System*> valid;
	for (size_t i = 0; i < systems.size(); i++)
	{
		if (systems.at(i)->OwningFaction == fac)
		{
			valid.push_back(systems.at(i));
			std::cout << "^^^^^^^^^";
		}
	}
	if ((valid.size() - 1) != 0)
	{
		return valid.at(rand() % (valid.size() - 1));
	}
	return valid.at(0);
}


int GetRandomPopulatedPlanetIdx(System* sys)
{
	//this function ignores planets with less than 1000 population
	std::vector<int> tP;
	for (size_t i = 0; i < sys->Planets.size(); i++)
	{
		if (sys->Planets.at(i)->Population > 1000)
		{
			tP.push_back(i);
		}
	}


	return tP.at(rand() % (tP.size()-1));

}



EventOutput EventHandler::RandomGenericEvent(Faction* PlayerFaction, std::vector<System*> AllSystems)
{
	EventOutput out;

	srand(time(NULL));
	std::ifstream file("C:/Users/User/Documents/2DSpaceGame/Assets/Events/EventReg.txt");
	int FSize = 1;
	std::string content((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));

	for (size_t i = 0; i < content.size(); i++)
	{
		if(content.at(i) == '\n')
		{
			FSize++;
		}
	}

	file.close();
	std::ifstream fileS("C:/Users/User/Documents/2DSpaceGame/Assets/Events/EventReg.txt");


	int random = rand() % FSize;
	std::string type;
	int idx = 0;
	while (std::getline(fileS,type))
	{
		if (idx==random)
		{
			break;
		}
		idx++;
		std::cout << type << std::endl;
	}


	std::vector<std::string> strArgs;
	if (type == "PopDecrease")
	{
		System* tSys = Chosensystem(PlayerFaction,AllSystems);
		//Set the display
		int planetIdx = GetRandomPopulatedPlanetIdx(tSys);
		int randomT = (rand() % 10 + 1);
		strArgs.push_back(LargeToString(randomT*tSys->Planets.at(planetIdx)->Population*0.001));
		strArgs.push_back((tSys->GetName()+" "+std::to_string(planetIdx+1)));


		//Do the thing

		tSys->Planets.at(planetIdx)->Population -= randomT * tSys->Planets.at(planetIdx)->Population * 0.01;

	}

	EventHandler EventWindow;
	EventWindow.Run(type,strArgs);



	return out;
}
