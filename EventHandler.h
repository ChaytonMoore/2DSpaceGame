#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>



#include "System.h"



struct EventOutput
{
	std::string type;
	
	//Not all variables are required
	Faction* fac;
	System* sys;

};


//probably didn't need a whole class for this.
class EventHandler
{
	sf::RenderWindow* window;
	sf::Sprite Display;
	


public:
	void Run(std::string name, std::vector<std::string> strArgs);
	//^ Originally event output but it's just the visual display
	static EventOutput RandomGenericEvent(Faction* PlayerFaction, std::vector<System*> AllSystems);
};





