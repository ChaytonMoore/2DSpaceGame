#include "ShipBuildButton.h"



//^ can be done elsewhere


bool ShipBuildButton::IsClicked(sf::Vector2i MouseLoc, float Scroll)
{


	
	return rect.getPosition().x < MouseLoc.x && rect.getPosition().x + rect.getSize().x > MouseLoc.x && rect.getPosition().y+35-Scroll/40 < MouseLoc.y && rect.getPosition().y + rect.getSize().y+35-Scroll/40 > MouseLoc.y;


}



