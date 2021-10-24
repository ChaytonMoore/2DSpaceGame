#include "SystemUIElement.h"




FleetStruct* SystemUIElement::RenderSystemUI(sf::RenderWindow* window, System* Current, DetailsPanel* Panel)
{
	
	FleetStruct* out = new FleetStruct();
	
	
	OwningSystem = Current;
	
	
	


	for (size_t i = 0; i < OwningSystem->SystemShips.size(); i++)
	{
		SystemShipButtons.push_back(new Button(sf::Vector2i(1600,i*50),OwningSystem->SystemShips.at(i)->Type->Name));
		window->draw(SystemShipButtons.at(i)->rect);
		window->draw(SystemShipButtons.at(i)->Text);

		if (SystemShipButtons.at(i)->isHovering(sf::Mouse::getPosition()) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			
			out->Ships.push_back(OwningSystem->SystemShips.at(i));
		}

		if (SystemShipButtons.at(i)->isHovering(sf::Mouse::getPosition()))
		{
			Panel->CurrentObject = OwningSystem->SystemShips.at(i)->Type->Name;
			Panel->CurrentSetting = PanelSetting::Ship;
			Panel->Reload(OwningSystem->SystemShips.at(i)->Type);
		}
		else
		{
			Panel->CurrentObject = "";
		}
		


	}
	while (!SystemShipButtons.empty()) delete SystemShipButtons.back(), SystemShipButtons.pop_back();
	
	

	int inc1 = 0, inc2 = 0, inc3 = 0;
	for (size_t i = 0; i < OwningSystem->Planets.size(); i++)
	{
		inc1 += OwningSystem->Planets.at(i)->CalculateMetalOutput();
		inc2 += OwningSystem->Planets.at(i)->CalculateTitaniumOutput();
		inc3 += OwningSystem->Planets.at(i)->CalculateRareOutput();

	}

	//TODO make the resources appear only when this system is owned by the player
	SimpleMetal.setString("Metal: "+std::to_string(OwningSystem->SimpleMetal) + " + " + std::to_string(inc1));
	Titanium.setString("Titanium: " + std::to_string(OwningSystem->Titanium) + " + " + std::to_string(inc2));
	RareMetal.setString("Rare Metal: " + std::to_string(OwningSystem->RareMetal) + " + " + std::to_string(inc3));

	window->draw(ResourcePanel);
	window->draw(SimpleMetal);
	window->draw(Titanium);
	window->draw(RareMetal);

	
	
	if (out->Ships.size()>0)
	{
		
		out->Location = sf::Vector2f(OwningSystem->Location);
		out->CurrentSystem = OwningSystem;
		
		return out;
	}
	else
	{
		delete out;
		return nullptr;
	}
	


	//^This is designed such that it will usually output a null instance
	
	
	

	
	
	
}


SystemUIElement::SystemUIElement()
{

	
	sf::Font* FontData = new sf::Font;
	FontData->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/Protector.ttf");

	SimpleMetal.setCharacterSize(22);
	SimpleMetal.setFont(*FontData);
	SimpleMetal.setPosition(15,970);

	Titanium.setCharacterSize(22);
	Titanium.setFont(*FontData);
	Titanium.setPosition(15, 1010);

	RareMetal.setCharacterSize(22);
	RareMetal.setFont(*FontData);
	RareMetal.setPosition(15, 1050);

	ResourcePanel.setPosition(sf::Vector2f(0,960));
	ResourcePanel.setSize(sf::Vector2f(250,120));
	ResourcePanel.setOutlineColor(sf::Color(74, 89, 57));
	ResourcePanel.setFillColor(sf::Color(37, 37, 38));
	ResourcePanel.setOutlineThickness(2);


}
