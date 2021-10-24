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
	void Reload(ShipType* ship); //nothing wrong with having the ship ref being nullptr

	sf::Sprite MainImage;
	sf::Image ImageRef;
	sf::Texture TextureRef;
	sf::Text Name;
	sf::Text Desc;
	sf::Text Stats; //This has to be formated and generated manually
	sf::Font TitleFont;
	sf::Font BodyFont;
	sf::RectangleShape Background;
	sf::RectangleShape ImgOutline; //only exists to be an outline for an image

};

