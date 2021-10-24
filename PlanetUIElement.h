#pragma once
#include "UIElement.h"
#include "Planet.h"
#include "ShipBuildButton.h"


class PlanetUIElement :
	public UIElement
{

public:
	Planet*	OwningPlanet;

	void RenderPlanetUI(sf::RenderWindow* window, Faction* OwningFaction, bool isPlayer); //Might want to change the isplayer variable to a player faction reference
	void SetUpPlanetUI();

	
	sf::Font TextFont;
	sf::Text Size;
	sf::Text Population;
	sf::Text PlanetClass;
	sf::Text PlanetLevel;
	sf::Text TerraformTime;
	


	sf::Text ShipyardLevel;
	Button* ShipyardUpgrade = new Button(sf::Vector2i(250,175),"Upgrade");

	sf::Text MineLevel;
	Button* MineUpgrade = new Button(sf::Vector2i(250, 400), "Upgrade");

	sf::Text ObservatoryLevel;
	Button* ObservatoryUpgrade = new Button(sf::Vector2i(250, 500), "Upgrade");

	float CurrentScroll = 0;

	std::vector<ShipBuildButton*> BuildableShips;

	void ResetBuildableships();

};

