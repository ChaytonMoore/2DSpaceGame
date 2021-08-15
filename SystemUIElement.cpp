#include "SystemUIElement.h"




FleetStruct* SystemUIElement::RenderSystemUI(sf::RenderWindow* window, System* Current)
{

	FleetStruct* out = new FleetStruct;

	OwningSystem = Current;
	for (size_t i = 0; i < OwningSystem->SystemShips.size(); i++)
	{
		SystemShipButtons.push_back(new Button(sf::Vector2i(1600,i*50),OwningSystem->SystemShips.at(i)->Type->Name));
		window->draw(SystemShipButtons.at(i)->rect);
		window->draw(SystemShipButtons.at(i)->Text);

		if (SystemShipButtons.at(i)->isHovering(sf::Mouse::getPosition()) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			std::cout << "you cliked ship";
			out->Ships.push_back(OwningSystem->SystemShips.at(i));
		}

		


	}
	while (!SystemShipButtons.empty()) delete SystemShipButtons.back(), SystemShipButtons.pop_back();


	
	if (out->Ships.size()>0)
	{
		out = new FleetStruct;
		out->Location = OwningSystem->Location;
		out->CurrentSystem = OwningSystem;
		return out;
	}
	else
	{
		delete out;
		return nullptr;
	}
	

	//This is designed such that it will usually output a null instance
	
}
