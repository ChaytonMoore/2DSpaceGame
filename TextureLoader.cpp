#include "TextureLoader.h"

void TextureStore::MassLoadTextures()
{
	HumanEmblemI = new sf::Image;
	HumanEmblemT = new sf::Texture;
	HumanEmblemI->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Emblem/HumanEmblem.png");
	HumanEmblemT->loadFromImage(*HumanEmblemI);


}
