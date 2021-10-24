#pragma once
#include <SFML/Graphics.hpp>

class UIElement
{
public: virtual ~UIElement() {}
	int ClickRadius = 8;
	int UISize = 64;
	sf::Vector2i LocalPosition;

	int ClickedOn();

	bool IsClicked(sf::Vector2i MouseLoc);


	void EventTick(double DeltaTime);

	


};

