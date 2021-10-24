#pragma once
#include "Button.h"
#include "FactionData.h" 

class TopMenu
{
public:
	void Render(sf::RenderWindow* window, std::vector<FactionData*>* Factions, FactionData* PlayerFaction);
	Button* DiplomacyDropDown = new Button(sf::Vector2i(300,5),"Diplomacy");
	Button* TechnologyDropDown = new Button(sf::Vector2i(410, 5), "Technology");
	Button* GovernmentDropDown = new Button(sf::Vector2i(520, 5), "Government");

	TopMenu(std::vector<FactionData*> facs)
	{
		DiplomacyDropDown->Text.setCharacterSize(11);
		TechnologyDropDown->Text.setCharacterSize(11);
		GovernmentDropDown->Text.setCharacterSize(11);
		SlideBack.setPosition(300,40);
		SlideBack.setSize(sf::Vector2f(1200,600));
		SlideBack.setFillColor(sf::Color(42,42,42));


		SpeciesImage.setPosition(400,40);
		LoadImageSF(&SpeciesImage,"Species/Taenos.png");

		SpeciesText.setPosition(1000,40);
		sf::Font* Font = new sf::Font;
		Font->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/Protector.ttf");
		SpeciesText.setFont(*Font);
		SpeciesText.setCharacterSize(14);


		TechText.setFont(*Font);
		TechText.setPosition(400,100);
		TechText.setCharacterSize(16);

		OpinionText.setFont(*Font);
		OpinionText.setPosition(1450,100);
		OpinionText.setCharacterSize(25);

		GovType.setFont(*Font);
		GovType.setPosition(600, 40);
		GovType.setCharacterSize(25);

		ReleaseRebellious->Text.setCharacterSize(8);
		ReleaseRebellious->rect.setSize(sf::Vector2f(200, 25));
		
		
	}
	std::vector<FactionData*> DiploSpecies;

	int CurrentSlide = 0;
	sf::RectangleShape SlideBack;
	FactionData* CurrentSpecies;


	//diplomacy elements
	std::vector<Button*> SpeciesButtons;
	sf::Sprite SpeciesImage;
	sf::Text SpeciesText;
	sf::Text OpinionText;

	sf::Text TechText;


	void SetSpeciesText();
	void SetSpeciesImage();
	sf::Color GenerateOpinionTextColour(int opinion);


	Button* TradeDeal = new Button(sf::Vector2i(1000, 250), "Trade Deal");
	Button* Friendship = new Button(sf::Vector2i(1000, 300), "Friendship");
	Button* Alliance = new Button(sf::Vector2i(1000, 350), "Alliance");
	Button* Vassalise = new Button(sf::Vector2i(1000, 400), "Vassalise");


	sf::Text GovType;
	Button* ReleaseRebellious = new Button(sf::Vector2i(1000,250),"Release Rebellious Systems");

};

