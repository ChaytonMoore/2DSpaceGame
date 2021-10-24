#pragma once
#include "Button.h"
#include "Planet.h"











class ShipBuildButton :
	public Button
{
public:


	ShipType* ShipToBuild;
	bool CanClick = false;

	virtual void Render(sf::RenderWindow* window, float Scroll, int idx)
	{

		rect.setPosition(LocalPosition - sf::Vector2f(0, Scroll-idx*25));
		Text.setPosition(LocalPosition - sf::Vector2f(0, Scroll-idx*25));

		window->draw(rect); 
		window->draw(Text);


		
	}

	ShipBuildButton(sf::Vector2i Position, std::string TextString, ShipType* varient) : Button(sf::Vector2i(0,0), "Ship")
	{
		
		rect.setPosition(Position.x, Position.y);
		rect.setSize(sf::Vector2f(180, 15));
		rect.setOutlineColor(sf::Color(74, 89, 57));
		rect.setFillColor(sf::Color(37, 37, 38));
		rect.setOutlineThickness(2);



		Text = *new sf::Text();
		Text.setString(TextString);
		Text.setFont(TextFont);
		Text.setCharacterSize(12);

		ShipToBuild = varient;


		LocalPosition = sf::Vector2f(Position.x, Position.y);
	}
	
	
	virtual int ClickedOn();

	virtual bool IsClicked(sf::Vector2i MouseLoc, float Scroll);


};









