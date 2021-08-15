#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


//Very simple file to load different assets and be referenced


class TextureStore
{
public:
	sf::Image* HumanEmblemI;
	sf::Texture* HumanEmblemT;


	void MassLoadTextures();
};







//TODO : include more textures in this





