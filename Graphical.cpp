#include "Graphical.h"


void LoadImage(sf::Sprite* Image, std::string Name)
{
	//contains a generic location, that should be changed later in the code

	sf::Image* i = new sf::Image;

	sf::Texture* t = new sf::Texture;

	i->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/" + Name);
	
	t->loadFromImage(*i);

	Image->setTexture(*t);
}
