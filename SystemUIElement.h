#pragma once
#include "UIElement.h"
#include "System.h"










class SystemUIElement : public UIElement
{

public:
	System* OwningSystem;

	//This contains the visual elements for the ships in the system

	std::vector<Button*> SystemShipButtons;


	FleetStruct* RenderSystemUI(sf::RenderWindow* window, System* Current);



};
