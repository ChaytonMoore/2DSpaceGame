#include "Button.h"



bool Button::isHovering(sf::Vector2i MouseLoc)
{

	return rect.getPosition().x < MouseLoc.x && rect.getPosition().x + rect.getSize().x > MouseLoc.x && rect.getPosition().y < MouseLoc.y && rect.getPosition().y + rect.getSize().y > MouseLoc.y;
}

bool Button::isClicked(sf::Vector2i MouseLoc)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isHovering(MouseLoc))
	{
		if (!soundFX->getStatus())
		{
			soundFX->play();
		}
		
	}

	return sf::Mouse::isButtonPressed(sf::Mouse::Left) && isHovering(MouseLoc);
}
