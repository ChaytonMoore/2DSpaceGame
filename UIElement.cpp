#include "UIElement.h"
#include <iostream>

int UIElement::ClickedOn()
{
	std::cout << "a";
	return 0;
}


bool UIElement::IsClicked(sf::Vector2i MouseLoc)
{
	float Distance = sqrt(abs(MouseLoc.x - (LocalPosition.x+UISize/2)) + abs(MouseLoc.y - (LocalPosition.y+UISize/2)));
	
	

	return (Distance < ClickRadius);
}


void UIElement::EventTick(double DeltaTime)
{

}

