#pragma once
#include "UIElement.h"
#include "Planet.h"
#include "ShipBuildButton.h"


class PlanetUIElement :
	public UIElement
{

public:
	Planet*	OwningPlanet;

	void RenderPlanetUI(sf::RenderWindow* window);
	void SetUpPlanetUI();

	
	sf::Font TextFont;
	sf::Text Size;
	sf::Text Population;
	sf::Text PlanetClass;
	sf::Text PlanetLevel;


	sf::Text ShipyardLevel;



	float CurrentScroll = 0;

	std::vector<ShipBuildButton*> BuildableShips;

};

