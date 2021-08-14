#include "PlanetUIElement.h"


void PlanetUIElement::RenderPlanetUI(sf::RenderWindow* window)
{
	Size.setString("Size: "+std::to_string(OwningPlanet->PlanetSize));
	Population.setString("Population: "+std::to_string(OwningPlanet->Population));
	switch (OwningPlanet->PlanetType)
	{
	case D:PlanetClass.setString("Class: D"); break;
	}

	if (OwningPlanet->Population>50000000000)
	{
		PlanetLevel.setString("City Planet");
	}
	else if (OwningPlanet->Population > 1000000000)
	{
		PlanetLevel.setString("Developed Planet");
	}
	else if (OwningPlanet->Population > 100000000)
	{
		PlanetLevel.setString("Major Colony");
	}
	else if (OwningPlanet->Population > 1000000)
	{
		PlanetLevel.setString("Large Colony");
	}
	else if (OwningPlanet->Population > 100000)
	{
		PlanetLevel.setString("Colony");
	}
	else if (OwningPlanet->Population > 1000)
	{
		PlanetLevel.setString("Small Colony");
	}
	else if (OwningPlanet->Population > 0)
	{
		PlanetLevel.setString("Outpost");
	}
	else
	{
		PlanetLevel.setString("Uninhabitated");
	}

	if (OwningPlanet->Shipyard == ShipyardLevels::SurfaceShipyard)
	{
		ShipyardLevel.setString("Surface Shipyard");
	}
	else if (OwningPlanet->Shipyard == ShipyardLevels::BasicShipyard)
	{
		ShipyardLevel.setString("Basic Shipyard");
	}
	else if (OwningPlanet->Shipyard == ShipyardLevels::SpaceElevator)
	{
		ShipyardLevel.setString("Space Elevator");
	}
	else if (OwningPlanet->Shipyard == ShipyardLevels::AdvancedShipyards)
	{
		ShipyardLevel.setString("Advanced Shipyard");
	}
	else
	{
		ShipyardLevel.setString("No Shipyard");
	}


	Size.setPosition(10, 100 - CurrentScroll);
	Population.setPosition(10, 120 - CurrentScroll);
	PlanetClass.setPosition(10, 140 - CurrentScroll);
	PlanetLevel.setPosition(25, 160 - CurrentScroll);
	ShipyardLevel.setPosition(15, 175 - CurrentScroll);


	window->draw(Size);
	window->draw(Population);
	window->draw(PlanetClass);
	window->draw(PlanetLevel);
	window->draw(ShipyardLevel);

	for (size_t i = 0; i < BuildableShips.size(); i++)
	{
		
		BuildableShips.at(i)->Render(window,CurrentScroll);
	}
	

}

void PlanetUIElement::SetUpPlanetUI()
{
	TextFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");

	Size.setFont(TextFont);
	Size.setCharacterSize(10);

	Population.setFont(TextFont);
	Population.setCharacterSize(10);

	PlanetClass.setFont(TextFont);
	PlanetClass.setCharacterSize(10);

	PlanetLevel.setFont(TextFont);
	PlanetLevel.setCharacterSize(12);

	ShipyardLevel.setFont(TextFont);
	ShipyardLevel.setCharacterSize(16);


}
