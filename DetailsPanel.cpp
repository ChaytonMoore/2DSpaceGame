#include "DetailsPanel.h"


void DetailsPanel::RenderPanel(sf::RenderWindow* window)
{
	if (CurrentObject != "")
	{
		window->draw(Background);
		window->draw(ImgOutline);
		window->draw(MainImage);
		window->draw(Name);
		window->draw(Desc);
		window->draw(Stats);
	}
}


void DetailsPanel::Reload(ShipType* ship)
{
	TitleFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");
	Name.setFont(TitleFont);
	Name.setString(CurrentObject);
	Name.setPosition(400,100);
	Name.setOutlineColor(sf::Color(0, 0, 0));
	Name.setOutlineThickness(1);

	BodyFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/monofonto.otf");
	Desc.setFont(BodyFont);
	std::ifstream file("C:/Users/User/Documents/2DSpaceGame/Assets/Desc/"+CurrentObject+".txt");
	std::string fileContents;
	std::string Temp;
	while (std::getline(file,Temp))
	{
		fileContents += '\n'+Temp;
	}
	Desc.setString(fileContents);
	Desc.setPosition(400,130);
	Desc.setCharacterSize(11);

	std::string statString = "";

	if (CurrentSetting == PanelSetting::Ship)
	{
		statString.append("Type: " + ship->type);
		statString.append("\nLength: " + std::to_string(ship->Length));
		statString.append("\nAttack: " + std::to_string(ship->Attack));
		statString.append("\nHull: " + std::to_string(ship->Hull));
		statString.append("\nShields: " + std::to_string(ship->Shields));
		statString.append("\nRange: " + std::to_string(ship->Range));
		statString.append("\nCrew Size: " + std::to_string(ship->Crew));
		statString.append("\nCapacity: " + std::to_string(ship->Capacity));
		statString.append("\nRequired Simple Metal: " + std::to_string(ship->RequiredMetal));
		statString.append("\nRequired Titanium: " + std::to_string(ship->RequiredTitanium));
		statString.append("\nRequired Rare Metal: " + std::to_string(ship->RequiredRare));
	}
	else if (CurrentSetting == PanelSetting::Building)
	{

	}
	else if (CurrentSetting == PanelSetting::TechAdvance)
	{

	}
	else
	{
		//if it's not one we'll assume ship, TODO might want to change this later
		statString += "Length: " + ship->Length;
		statString += "\n Attack: " + ship->Attack;
		statString += "\n Hull: " + ship->Hull;
	}
	Stats.setString(statString);
	Stats.setFont(BodyFont);
	Stats.setPosition(1055, 400);
	Stats.setCharacterSize(13);

	//The image will need to be reloaded.
	ImageRef.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Ships/"+CurrentObject+".png");
	TextureRef.loadFromImage(ImageRef);
	MainImage.setTexture(TextureRef);
	MainImage.setPosition(400,400);

	Background.setPosition(400,100);
	Background.setSize(sf::Vector2f(1000,780));
	Background.setOutlineColor(sf::Color(45,45,48));
	Background.setFillColor(sf::Color(30,30,30));
	Background.setOutlineThickness(2);
	
	ImgOutline.setPosition(403, 400);
	ImgOutline.setSize(sf::Vector2f(643, 480));
	ImgOutline.setOutlineColor(sf::Color(62,62,62));
	ImgOutline.setFillColor(sf::Color(62, 62, 62));
	ImgOutline.setOutlineThickness(6);
}

