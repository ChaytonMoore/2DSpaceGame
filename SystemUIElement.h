#pragma once
#include "UIElement.h"
#include "ShipActionsUI.h"
#include "DetailsPanel.h"









class SystemUIElement : public UIElement
{

public:
	System* OwningSystem;

	//This contains the visual elements for the ships in the system

	std::vector<Button*> SystemShipButtons;
	
	std::vector<sf::Text> TerraformTimes;

	FleetStruct* RenderSystemUI(sf::RenderWindow* window, System* Current, DetailsPanel* Panel);

	SystemUIElement();

	sf::RectangleShape ResourcePanel;
	sf::Text SimpleMetal;
	sf::Text Titanium;
	sf::Text RareMetal;


};

