#include "TopMenu.h"


void TopMenu::SetSpeciesText()
{
	std::ifstream file("C:/Users/User/Documents/2DSpaceGame/Assets/Profiles/Desc/" + CurrentSpecies->OwningFaction->FactionName +".txt");
	std::string a;
	std::string line;
	while (std::getline(file, line))
	{
		a += line + '\n';
	}
	SpeciesText.setString(a);
	
}

void TopMenu::SetSpeciesImage()
{
	//firstly flush the existing image and texture data
	SpeciesImage.getTexture()->~Texture();
	sf::Image Temp; //keeping the image around isn't necessary
	if (CurrentSpecies->isRebel)
	{
		//the image will get an image via name hashing of the species
		int NameValue = 0;
		for (size_t i = 0; i < CurrentSpecies->OwningFaction->FactionName.size(); i++)
		{
			NameValue += CurrentSpecies->OwningFaction->FactionName.at(i);
		}
		Temp.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Species/" + CurrentSpecies->OriginFaction->OwningFaction->FactionName+"/"+std::to_string(NameValue%5)+ ".png");
	}
	else
	{
		Temp.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Species/" + CurrentSpecies->OwningFaction->FactionName + ".png");
	}
	sf::Texture* Temp2 = new sf::Texture;
	Temp2->loadFromImage(Temp);
	SpeciesImage.setTexture(*Temp2);

	//This code remarkedly doesn't cause a memory leak
}

sf::Color TopMenu::GenerateOpinionTextColour(int opinion)
{
	//100 is green
	//0 is red
	return sf::Color(255-(opinion*2.55),opinion*2.55,0);
	
}

void TopMenu::Render(sf::RenderWindow* window, std::vector<FactionData*>* Factions, FactionData* PlayerFaction)
{
	DiplomacyDropDown->Render(window);
	TechnologyDropDown->Render(window);
	GovernmentDropDown->Render(window);
	DiploSpecies = PlayerFaction->GetKnownFactions(*Factions);



	if (DiplomacyDropDown->isClicked(sf::Mouse::getPosition()) && DiplomacyDropDown->Active)
	{
		DiplomacyDropDown->Active = false;
		if (CurrentSlide == 1)
		{
			CurrentSlide = 0;
		}
		else
		{
			CurrentSlide = 1;
			//Clear the species buttons
			while (!SpeciesButtons.empty()) delete SpeciesButtons.back(), SpeciesButtons.pop_back();

			//repopulate
			if (DiploSpecies.size())
			{


				for (size_t i = 0; i < Factions->size(); i++)
				{
					if (Factions->at(i) != PlayerFaction)
					{
						SpeciesButtons.push_back(new Button(sf::Vector2i(300, 10 + i * 35), Factions->at(i)->OwningFaction->FactionName));
						DiploSpecies.push_back(Factions->at(i));
					}
				}
				CurrentSpecies = DiploSpecies.back();
				if (CurrentSpecies)
				{
					SetSpeciesText();
				}
			}
		}
	}
	else if(!DiplomacyDropDown->isClicked(sf::Mouse::getPosition()))
	{
		//This is done so that it doesn't just flicker
		DiplomacyDropDown->Active = true;
	}

	if (TechnologyDropDown->isClicked(sf::Mouse::getPosition()) && TechnologyDropDown->Active)
	{
		TechnologyDropDown->Active = false;
		if (CurrentSlide == 2)
		{
			CurrentSlide = 0;
			
		}
		else
		{
			//reset the tech text
			TechText.setString("");
			std::string TechTemp;
			TechTemp.append("Medical: " + std::to_string(PlayerFaction->OwningFaction->MedicalTech));
			TechTemp.append("\nMining: " + std::to_string(PlayerFaction->OwningFaction->MiningTech));
			TechTemp.append("\nFarming: " + std::to_string(PlayerFaction->OwningFaction->FarmingTech));
			TechTemp.append("\nConstruction: " + std::to_string(PlayerFaction->OwningFaction->ConstructionTech));
			TechTemp.append("\nShip Building: " + std::to_string(PlayerFaction->OwningFaction->ShipbuildingTech));
			TechTemp.append("\nWeapons: " + std::to_string(PlayerFaction->OwningFaction->WeaponsTech));
			TechTemp.append("\nEngine: " + std::to_string(PlayerFaction->OwningFaction->EngineTech));
			TechTemp.append("\nSensor: " + std::to_string(PlayerFaction->OwningFaction->SensorTech));
			TechText.setString(TechTemp);

			CurrentSlide = 2;
		}

	}
	else if (!TechnologyDropDown->isClicked(sf::Mouse::getPosition()))
	{
		//This is done so that it doesn't just flicker
		TechnologyDropDown->Active = true;
	}

	if (GovernmentDropDown->isClicked(sf::Mouse::getPosition()) && GovernmentDropDown->Active)
	{
		GovernmentDropDown->Active = false;
		if (CurrentSlide == 3)
		{
			CurrentSlide = 0;
		}
		else
		{
			CurrentSlide = 3;
			GovType.setString("Government: "+PlayerFaction->OwningFaction->GovernmentType);
		}
	}
	else if (!GovernmentDropDown->isClicked(sf::Mouse::getPosition()))
	{
		//This is done so that it doesn't just flicker
		GovernmentDropDown->Active = true;
	}

	
	if(CurrentSlide != 0)
	{
		
		window->draw(SlideBack);
	}
	if (CurrentSlide == 1) //factions
	{
		
		if (DiploSpecies.size())
		{
			for (size_t i = 0; i < SpeciesButtons.size(); i++)
			{
				SpeciesButtons.at(i)->Render(window);
			}
			window->draw(SpeciesImage);
			window->draw(SpeciesText);

			for (size_t i = 0; i < SpeciesButtons.size(); i++)
			{
				if (SpeciesButtons.at(i)->isClicked(sf::Mouse::getPosition()))
				{
					CurrentSpecies = DiploSpecies.at(i);
				}
				//reset the text and images of the ui
				SetSpeciesText();
				SetSpeciesImage();
			}

			//draw the opinion text

			int RelationValue = PlayerFaction->OwningFaction->CalculateFactionRelation(CurrentSpecies->OwningFaction);
			OpinionText.setFillColor(GenerateOpinionTextColour(RelationValue));
			OpinionText.setString(std::to_string(RelationValue)); //TODO make the text display properly
			window->draw(OpinionText);

			//Render the buttons for different operations
			//Only make the button available if it's possible
			if (PlayerFaction->OwningFaction->CalculateFactionRelation(CurrentSpecies->OwningFaction) > 40)
			{
				TradeDeal->rect.setOutlineColor(sf::Color(74, 89, 57));
				//the button click call should be here


			}
			else
			{
				TradeDeal->rect.setOutlineColor(sf::Color(255, 0, 0));
				//set the colour to something less distinctive

			}
			TradeDeal->Render(window);

			if (PlayerFaction->OwningFaction->CalculateFactionRelation(CurrentSpecies->OwningFaction) > 50)
			{
				Friendship->rect.setOutlineColor(sf::Color(74, 89, 57));
				//the button click call should be here


			}
			else
			{
				Friendship->rect.setOutlineColor(sf::Color(255, 0, 0));
			}
			Friendship->Render(window);

			if (PlayerFaction->OwningFaction->CalculateFactionRelation(CurrentSpecies->OwningFaction) > 70)
			{
				Alliance->rect.setOutlineColor(sf::Color(74, 89, 57));
				//the button click call should be here


			}
			else
			{
				Alliance->rect.setOutlineColor(sf::Color(255, 0, 0));
			}
			Alliance->Render(window);

			if (PlayerFaction->OwningFaction->CalculateFactionRelation(CurrentSpecies->OwningFaction) > 90)
			{
				Vassalise->rect.setOutlineColor(sf::Color(74, 89, 57));
				//the button click call should be here


			}
			else
			{
				Vassalise->rect.setOutlineColor(sf::Color(255, 0, 0));
			}
			Vassalise->Render(window);
		}
		
	}
	if (CurrentSlide == 2) //tech
	{
		window->draw(TechText);
		
	}
	if (CurrentSlide == 3)
	{
		window->draw(GovType);
		ReleaseRebellious->Render(window);
		if (ReleaseRebellious->isClicked(sf::Mouse::getPosition()))
		{
			PlayerFaction->ReleaseRebelliousSystems(Factions);
		}
	}


}