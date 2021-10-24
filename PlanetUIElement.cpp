#include "PlanetUIElement.h"


void PlanetUIElement::RenderPlanetUI(sf::RenderWindow* window, Faction* OwningFaction, bool isPlayer)
{

	//TODO currently the planets will be rendered from the previous system


	Size.setString("Size: "+std::to_string(OwningPlanet->PlanetSize));
	Population.setString("Population: "+LargeToString(OwningPlanet->Population) + " / " + LargeToString(OwningPlanet->MaxPop));
	switch (OwningPlanet->PlanetType)
	{
	case D:PlanetClass.setString("Class: D"); break;
	case H:PlanetClass.setString("Class: H"); break;
	case J:PlanetClass.setString("Class: J"); break;
	case K:PlanetClass.setString("Class: K"); break;
	case L:PlanetClass.setString("Class: L"); break;
	case M:PlanetClass.setString("Class: M"); break;
	case N:PlanetClass.setString("Class: N"); break;
	case R:PlanetClass.setString("Class: R"); break;
	case T:PlanetClass.setString("Class: T"); break;
	case Y:PlanetClass.setString("Class: Y"); break;
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

	if (OwningPlanet->Mining == MiningLevels::BasicMines)
	{
		MineLevel.setString("Basic Mines");
	}
	else if (OwningPlanet->Mining == MiningLevels::GlobalMines)
	{
		MineLevel.setString("Global Mines");
	}
	else if (OwningPlanet->Mining == MiningLevels::StripMining)
	{
		MineLevel.setString("Strip Mines");
	}
	else if (OwningPlanet->Mining == MiningLevels::AdvancedStripMining)
	{
		MineLevel.setString("Advanced Strip Mines");
	}
	else
	{
		MineLevel.setString("No Mines");
	}


	if (OwningPlanet->Observatory == Observatory::BasicTelescope)
	{
		ObservatoryLevel.setString("Basic Telescope");
	}
	else if (OwningPlanet->Observatory == Observatory::SataliteTelescope)
	{
		ObservatoryLevel.setString("Satalite Telescope");
	}
	else if (OwningPlanet->Observatory == Observatory::SataliteNetwork)
	{
		ObservatoryLevel.setString("Satalite Network");
	}
	else if (OwningPlanet->Observatory == Observatory::SubspaceScanner)
	{
		ObservatoryLevel.setString("Subspace Scanner");
	}
	else if (OwningPlanet->Observatory == Observatory::SubspaceNetwork)
	{
		ObservatoryLevel.setString("Subspace Network");
	}
	else
	{
		ObservatoryLevel.setString("No Observatory");
	}




	//this is probably redundant since the menu no longer moves
	Size.setPosition(10, 100 - CurrentScroll);
	Population.setPosition(10, 120 - CurrentScroll);
	PlanetClass.setPosition(10, 140 - CurrentScroll);
	PlanetLevel.setPosition(25, 160 - CurrentScroll);
	ShipyardLevel.setPosition(15, 175 - CurrentScroll);
	MineLevel.setPosition(15, 400 - CurrentScroll);
	ObservatoryLevel.setPosition(15,500 - CurrentScroll);
	

	window->draw(Size);
	window->draw(Population);
	window->draw(PlanetClass);
	window->draw(PlanetLevel);
	window->draw(ShipyardLevel);
	window->draw(MineLevel);
	window->draw(ObservatoryLevel);

	if ((OwningPlanet->PlanetType == PlanetClasses::D || OwningPlanet->PlanetType == PlanetClasses::H || OwningPlanet->PlanetType == PlanetClasses::L || OwningPlanet->PlanetType == PlanetClasses::K)&& OwningPlanet->Population<1)
	{
		TerraformTime.setString(std::to_string(OwningPlanet->TerraformTime).substr(0,4));
		window->draw(TerraformTime);
	}
	

	for (size_t i = 0; i < BuildableShips.size(); i++)
	{
		
		BuildableShips.at(i)->Render(window,CurrentScroll,i);
	}
	


	//render building upgrade buttons

	//check to make sure the upgrade is possible
	if (OwningPlanet->Shipyard<4 && OwningPlanet->Population> 1000 * int(OwningPlanet->Shipyard) && isPlayer && OwningFaction->ConstructionTech > OwningPlanet->Shipyard * 3) //include it in systme build queue
	{
		//Show the button
		ShipyardUpgrade->Render(window); //Make sure this is relative
		if (ShipyardUpgrade->isClicked(sf::Mouse::getPosition()))
		{
			// upgrade the shipyard
			
			OwningPlanet->Shipyard = ShipyardLevels(int(OwningPlanet->Shipyard) + 1);

			
			
			
		}
	}

	//upgrade mining
	if (OwningPlanet->Shipyard<4 && OwningPlanet->Population > 3000 * (int(OwningPlanet->Mining)+1) && isPlayer && OwningFaction->MiningCoef > OwningPlanet->Mining * 3)
	{
		MineUpgrade->Render(window);
		if (MineUpgrade->isClicked(sf::Mouse::getPosition()))
		{
			OwningPlanet->Mining = MiningLevels(int(OwningPlanet->Shipyard)+1 );
		}
	}

	//upgrade observatory
	if (OwningPlanet->Observatory<4 && isPlayer && OwningFaction->SensorTech >= int(OwningPlanet->Observatory))
	{
		ObservatoryUpgrade->Render(window);
		if (ObservatoryUpgrade->isClicked(sf::Mouse::getPosition()))
		{
			OwningPlanet->Observatory = Observatory(int(OwningPlanet->Observatory)+1);
		}
	}


}

void PlanetUIElement::SetUpPlanetUI()
{
	TextFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");

	Size.setFont(TextFont);
	Size.setCharacterSize(10);

	Population.setFont(TextFont);
	Population.setCharacterSize(12);

	PlanetClass.setFont(TextFont);
	PlanetClass.setCharacterSize(10);

	PlanetLevel.setFont(TextFont);
	PlanetLevel.setCharacterSize(12);

	TerraformTime.setFont(TextFont);
	TerraformTime.setCharacterSize(12);
	TerraformTime.setPosition(200,160);

	ShipyardLevel.setFont(TextFont);
	ShipyardLevel.setCharacterSize(16);

	MineLevel.setFont(TextFont);
	MineLevel.setCharacterSize(16);


	ShipyardUpgrade->rect.setScale(1,0.5);
	MineUpgrade->rect.setScale(1,0.5);
	ObservatoryUpgrade->rect.setScale(1,0.5);

	ObservatoryLevel.setFont(TextFont);
	ObservatoryLevel.setCharacterSize(12);
	ObservatoryLevel.setPosition(200, 300);
}
