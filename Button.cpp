#include "Button.h"

bool Button::isHovering(sf::Vector2i MouseLoc)
{

	return rect.getPosition().x < MouseLoc.x && rect.getPosition().x + rect.getSize().x > MouseLoc.x && rect.getPosition().y + 35 < MouseLoc.y && rect.getPosition().y + rect.getSize().y + 35 > MouseLoc.y;
}
