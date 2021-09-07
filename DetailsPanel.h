#include "UIElement.h"
#include "Planet.h"
#include "ShipBuildButton.h"
#pragma once

//The widget is for displaying details about a specific thing.
enum PanelSetting
{
	Ship,Building,TechAdvance
};

class DetailsPanel : UIElement
{
public:
	PanelSetting CurrentSetting;
	
	std::string CurrentObject;

	void RenderPanel(sf::RenderWindow* window);
	void Reload();

	sf::Sprite MainImage;
	sf::Image ImageRef;
	sf::Texture TextureRef;

};

