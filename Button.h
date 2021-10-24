#pragma once
#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include <iostream>
#include <SFML/Audio.hpp>


class Button : public UIElement
{




public:
	sf::Vector2f LocalPosition;
	sf::RectangleShape rect;
	sf::Text Text;
	sf::Font TextFont;
	bool Active = true;

	sf::SoundBuffer soundFXB;
	sf::Sound* soundFX;

	Button(sf::Vector2i Position,std::string TextString)
	{
		rect.setPosition(Position.x,Position.y);
		rect.setSize(sf::Vector2f(100,40));
		rect.setOutlineColor(sf::Color(74, 89, 57));
		rect.setFillColor(sf::Color(37, 37, 38));
		rect.setOutlineThickness(2);


		
		TextFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");
		Text = *new sf::Text;
		Text.setString(TextString);
		Text.setFont(TextFont);
		Text.setCharacterSize(14);
		Text.setPosition(Position.x,Position.y);

		soundFXB.loadFromFile("button.wav");
		soundFX = new sf::Sound;
		soundFX->setBuffer(soundFXB);




	}

	~Button()
	{
		delete soundFX;
	}


	void Render(sf::RenderWindow* window)
	{

		window->draw(rect);
		window->draw(Text);
	}


	bool isHovering(sf::Vector2i MouseLoc);

	bool isClicked(sf::Vector2i MouseLoc);

};



