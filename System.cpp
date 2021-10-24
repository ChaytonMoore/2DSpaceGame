#include "System.h"
#include <sys/stat.h>
#include <fstream>
#include <algorithm>



std::vector<std::string> System::SplitString(std::string s, char delimiter)
{
	std::vector<std::string> out;
	out.push_back("");
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s.at(i)==delimiter)
		{
			out.push_back("");
		}
		else
		{
			out.back().push_back(s.at(i));
		}
	}

	

	return out;
}



void System::RenderSystem(sf::RenderWindow* window, sf::Vector2f CameraOffset, TextureStore* TStore, Faction* PlayerFaction, float Zoom)
{
	//Still a minor memory leak TODO, might just be more things created
	//TODO renders mulitple ships building over each other
	
	//Setting up the render components
	LocalPosition = Location - sf::Vector2i(CameraOffset);
	LocalPosition = sf::Vector2i(LocalPosition.x * Zoom, LocalPosition.y * Zoom);
	CurrentSprite->setPosition(sf::Vector2f(LocalPosition)+sf::Vector2f(32,32));

	if (OwningFaction == PlayerFaction)
	{
		CurrentSprite = &OwnedSprite;
	}
	else if (OwningFaction) //TODO make it so that different states if factions can be expressed
	{
		
		CurrentSprite = &NeutralSprite;
		OwnerEmblem.setTexture(OwningFaction->GenericEmblem);
		OwnerEmblem.setPosition(sf::Vector2f(LocalPosition));
		window->draw(OwnerEmblem);
		
	}
	else
	{
		CurrentSprite = &Sprite;
	}


	//Rendering the components
	window->draw(*CurrentSprite);
	SystemName.setPosition(sf::Vector2f(LocalPosition)+sf::Vector2f(32,64));
	window->draw(SystemName);

	bool inside;
	std::vector<std::string> existingEmblems;
	for (size_t i = 0; i < SystemShips.size(); i++)
	{
		inside = false;
		for (size_t j = 0; j < existingEmblems.size(); j++)
		{
			if (existingEmblems.at(j)==SystemShips.at(i)->OwningFaction->FactionName)
			{
				inside = true;
				
			}

		}
		if (!inside)
		{
			existingEmblems.push_back(SystemShips.at(i)->OwningFaction->FactionName);
			try
			{
				FleetsEmblems.push_back(new sf::Sprite); //this sometimes causes an exception for an unknown reason
			}
			catch (const std::exception&)
			{

			}
			
			FleetsEmblems.back()->setPosition(sf::Vector2f(LocalPosition) + sf::Vector2f(32, -32));

			if (SystemShips.at(i)->OwningFaction->FactionName == "Human")
			{
				
				FleetsEmblems.back()->setTexture(*TStore->HumanEmblemT);
			}
		}

	}

	for (size_t i = 0; i < FleetsEmblems.size(); i++)
	{
		window->draw(*FleetsEmblems.at(i));
	}

	while (!FleetsEmblems.empty()) delete FleetsEmblems.front(), FleetsEmblems.pop_back();
	

}

int System::ClickedOn()
{
	std::cout << "system is clicked";

	return 1;
}

bool System::LoadSystem(std::string Name) 
{

	Planet* temp;


	//an entry for each planet using a txt
	// planet class, size, population, arable land
	std::string Line;
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/" + Name);
	std::vector<std::string> split;


	TextFont.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Assets/EC.ttf");
	SystemName.setFont(TextFont);
	SystemName.setString(SplitString(Name,'.').at(0));
	SystemName.setCharacterSize(16);


	int tempI;

	while (std::getline (File,Line))
	{
		if (Line.size() > 0)
		{






			split = SplitString(Line, ',');

			//START// These three lines used to be above the split, probably not important
			Planets.push_back(new Planet(std::stof(split[1]), PlanetClasses::J));
			temp = Planets.back();

			temp->PlanetSprite = new sf::Sprite;

			//END//
			
			if (split.at(0) == "D")
			{
				temp->PlanetType = PlanetClasses::D;


				std::cout << "a";
			}

			
			if (split.at(0) == "H")
			{
				temp->PlanetType = PlanetClasses::H;
			}
			if (split.at(0) == "J")
			{
				temp->PlanetType = PlanetClasses::J;
			}
			if (split.at(0) == "K")
			{
				temp->PlanetType = PlanetClasses::K;
			}
			if (split.at(0) == "L")
			{
				temp->PlanetType = PlanetClasses::L;
			}
			if (split.at(0) == "M")
			{
				temp->PlanetType = PlanetClasses::M;
			}
			if (split.at(0) == "N")
			{
				temp->PlanetType = PlanetClasses::N;
			}
			if (split.at(0) == "R")
			{
				temp->PlanetType = PlanetClasses::R;
			}
			if (split.at(0) == "T")
			{
				temp->PlanetType = PlanetClasses::T;
			}
			if (split.at(0) == "Y")
			{
				temp->PlanetType = PlanetClasses::Y;
			}
			


			//temp->PlanetSize = std::stof(split[1]); //this is now taken care of in the constructor call
			temp->Population = std::stof(split[2]);
			temp->ArableLand = std::stoi(split[3]);

			//TODO : perhaps make it so that other things than human can start of as player faction
			
			
		}

	}



	return true;
}




std::vector<System*> LoadSystems(std::vector<Faction*> Factions)
{

	std::vector<System*> output;
	//firstly load the address document
	std::string Line;
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Assets/Systems.txt");
	std::vector<std::string> split;


	int facnum = 0;
	while (std::getline(File, Line))
	{
		split = System::SplitString(Line,',');
		output.push_back(new System(sf::Vector2i(std::stoi(split.at(1)),std::stoi(split.at(2))),split.at(0)));
		output.back()->LoadSystem(split.at(0));
		//output.back()->AddUIPlanets();



		//make this thing a lot more automated
		if (split.at(3)=="Human")
		{
			output.back()->OwningFaction = Factions.at(0);
		}
		//TODO ^ Make it so that other factions than humans can start with a system
		if (split.at(3) == "Svethi")
		{
			facnum = 1;
			output.back()->OwningFaction = Factions.at(1);
		}
		if (split.at(3) == "Taenos")
		{
			facnum = 2;
			output.back()->OwningFaction = Factions.at(2);
		}


	}
	
	for (size_t i = 0; i < output.size(); i++)
	{
		for (size_t j = 0; j < output.at(i)->Planets.size(); j++)
		{

			if (output.at(i)->OwningFaction)
			{ //TODO make it so J class planets can't have max populations unless I want moons to be colonisable
				output.at(i)->Planets.at(j)->MaxPop = (output.at(i)->Planets.at(j)->ArableLand * (float)(9 + output.at(i)->OwningFaction->FarmingTech) / 10) * 1000000 + output.at(i)->OwningFaction->FarmingTech * 100000 * output.at(i)->Planets.at(j)->PlanetSize;
			}
			else
			{
				output.at(i)->Planets.at(j)->MaxPop = (output.at(i)->Planets.at(j)->ArableLand * (float)(9 + Factions.at(facnum)->FarmingTech) / 10) * 1000000 + Factions.at(facnum)->FarmingTech * 100000 * output.at(i)->Planets.at(j)->PlanetSize;
			}
		}
	}



	//new build the proximity systems
	//if the file is missing then make it
	FILE* filepoint;
	errno_t err;
	if ((err = fopen_s(&filepoint, "C:/Users/User/Documents/2DSpaceGame/Assets/proximity.nrst", "r")) != 0)
	{
		std::vector<System*> nearest;
		
		std::ofstream nrst("C:/Users/User/Documents/2DSpaceGame/Assets/proximity.nrst", std::ios::out | std::ios::binary);
		int sz = output.size();
		nrst.write((char *)&sz,sizeof(sz));


		uint16_t writeInt;
		for (size_t i = 0; i < output.size(); i++)
		{
			nearest.clear();
			//clear before algorithm begins
			for (size_t j = 0; j < output.size(); j++)
			{
				if (i != j && !(std::find(nearest.begin(), nearest.end(), output.at(j)) != nearest.end()))
				{
					if (SystemDistance(sf::Vector2f(output.at(i)->Location), sf::Vector2f(output.at(j)->Location)) < 500)
					{
						nearest.push_back(output.at(j));
						writeInt = j;
						//write to file
						nrst.write((char*)& writeInt, sizeof(writeInt));
					}

				}
			}

			//now write the terminating byte
			//only allows so many starsystems
			writeInt = 65535;
			nrst.write((char*)& writeInt, sizeof(writeInt));
			std::cout << writeInt << std::endl;
			

		}
	}

	//The /nrst file's format is
	//int
	//for each system
	// 2byte ints for each near system and then terminate 2byte



	return output;
}


int System::GetShipyardPower()
{
	int out = 0;
	for (size_t i = 0; i < Planets.size(); i++)
	{
		out += int(Planets.at(i)->Shipyard);
	}

	return out;
}


void System::EventTick(double DeltaTime, std::vector<FleetStruct*>* FreeFleets, std::vector<System*> systems)
{
	if (ShipBuildQueue.size())
	{
		ShipBuildQueue.at(0).Remaining -= DeltaTime/100;
		if (ShipBuildQueue.at(0).Remaining<0)
		{
			SystemShips.push_back(new ShipInstance(ShipBuildQueue.at(0).Type,OwningFaction));
			ShipBuildQueue.erase(ShipBuildQueue.begin());
			
		}
	}

	//Now handle the fleets in the system



	//Now handle events that take place every so often
	if (CurrentTimeStamp> float(LastTime)/10) //10th of year
	{
		if (OwningFaction)
		{


			//next tenth year tick
			LastTime += 1;
			//Calculate the resources for the planets
			for (size_t i = 0; i < Planets.size(); i++)
			{
				SimpleMetal += Planets.at(i)->CalculateMetalOutput();
				Titanium += Planets.at(i)->CalculateTitaniumOutput();
				RareMetal += Planets.at(i)->CalculateRareOutput();



			}
			if (LastTime % 10 == 0)
			{
				for (size_t i = 0; i < Planets.size(); i++)
				{

					//The year tick
					Planets.at(i)->PlanetYearTick(OwningFaction);
				}
				TerraformingTick();
				//for some reason it only really works if terraforming is done every year
			}
			IntraSystemMigration();
		}


		


		
	}

	//for all of the ships that are in auto transport depart them
	for (size_t i = 0; i < SystemShips.size(); i++)
	{
		if (SystemShips.at(i)->Activity == ShipActivity::Transport)
		{
			//TODO makes the ship disappear rather than anything else
			FreighterDepart(SystemShips.at(i),systems,FreeFleets);
		}
	}



	CurrentTimeStamp += DeltaTime/100;
}



void System::IntraSystemMigration()
{
	//At least 2 populated planets
	int populated = 0;
	for (size_t i = 0; i < Planets.size(); i++)
	{
		if (Planets.at(i)->Population>100) //bigger than 100 is classed as populated
		{
			populated += 1;
		}
	}


	if (populated>1)
	{
		
		for (size_t i = 0; i < Planets.size(); i++)
		{
			for (size_t j = 0; j < Planets.size(); j++)
			{
				if (i!=j)
				{
					if (Planets.at(i)->Population > Planets.at(j)->Population && Planets.at(j)->Population < (Planets.at(i)->ArableLand * (float)(9 + OwningFaction->FarmingTech) / 10) * 1000000 + OwningFaction->FarmingTech * 100000 * Planets.at(i)->PlanetSize)
					{
						Planets.at(i)->Population -= Planets.at(j)->Population * 0.01 * pow(Planets.at(j)->PlanetSize,0.5) * OwningFaction->ShipbuildingTech;
						Planets.at(j)->Population += Planets.at(j)->Population * 0.01 * pow(Planets.at(j)->PlanetSize,0.5) * OwningFaction->ShipbuildingTech;
					
					}


				}
			}
		}
	}

}


std::string System::GetName()
{
	//This function exists such that the name of a system can be returned as the name not including .csv
	std::string out = Name;

	size_t loc = out.find(".");

	out.erase(loc,out.size()-loc);
	
	//Also change under scores to spaces
	for (size_t i = 0; i < out.size(); i++)
	{
		if (out.at(i) == '_')
		{
			out.at(i) = ' ';
		}
	}

	return out;
}


uint64_t System::GetPopulation()
{
	uint64_t out = 0;

	for (size_t i = 0; i < Planets.size(); i++)
	{
		out += Planets.at(i)->Population;
	}
	return out;
}

void System::LoadUpFreighter(ShipInstance* ship, System* Target)
{
	//20:5:1 ratio of resources is ideal
	float NeedSimple, NeedTit, NeedRare;
	int TotalR = Target->SimpleMetal + Target->RareMetal + Target->Titanium;
	if (TotalR == 0)
	{
		NeedSimple = 1;
		NeedTit = 1;
		NeedRare = 1;
	}
	else
	{
		NeedSimple = (TotalR * 0.769) / (Target->SimpleMetal + 1);
		NeedTit = (TotalR * 0.192) / (Target->Titanium + 1);
		NeedRare = (TotalR * 0.0385) / (Target->RareMetal+1);
	}

	std::cout << "res" << NeedSimple << " " << NeedTit << " " << NeedRare << std::endl;
	//now normalise the needs
	float TotalNeed = NeedSimple + NeedTit + NeedRare;
	NeedSimple = NeedSimple / TotalNeed;
	NeedTit = NeedTit / TotalNeed;
	NeedRare = NeedRare / TotalNeed;

	NeedSimple = ship->Type->Capacity * NeedSimple * (SimpleMetal*3<Target->SimpleMetal); //Coopt these values for the total stuff in the hold
	NeedTit = ship->Type->Capacity * NeedTit * (Titanium * 3 < Target->Titanium);
	NeedRare = ship->Type->Capacity * NeedRare * (RareMetal * 3 < Target->RareMetal);


	ship->Resources.clear(); //firstly make sure the hold is cleared


	if (SimpleMetal>=NeedSimple)
	{
		ship->Resources.insert({ "Simple",NeedSimple }); //float -> int so it will be floored
		SimpleMetal -= NeedSimple;
	}
	
	if (Titanium>=NeedTit)
	{
		ship->Resources.insert({ "Titanium",NeedTit });
		Titanium -= NeedTit;
	}
	
	if (RareMetal>= NeedRare)
	{
		ship->Resources.insert({ "Rare",NeedRare });
		RareMetal -= NeedRare;
	}

}



void System::FreighterDepart(ShipInstance* ship, std::vector<System*> Systems, std::vector<FleetStruct*>* FreeFleets)
{

	//really hate having to go through every single system like this
	std::vector<System*> validSystems;
	for (size_t i = 0; i < Systems.size(); i++)
	{
		if (Systems.at(i)->OwningFaction == ship->OwningFaction && SystemDistance(sf::Vector2f(Systems.at(i)->Location),sf::Vector2f(Location))<= ship->Type->Range && Systems.at(i) != this)
		{
			//don't know if doing two ifs would be better or not for performance
			validSystems.push_back(Systems.at(i));
			std::cout << " " << Systems.at(i)->OwningFaction << " " << ship->OwningFaction << std::endl;

		}
	}
	if (validSystems.size()==0) 
	{
		//if there are no valid systems the transport will stop auto transporting
		ship->Activity = ShipActivity::Standard;
		return;
	}
	//get a random system
	int systemIdx = rand() % validSystems.size();
	LoadUpFreighter(ship,validSystems.at(systemIdx));
	
	//now set the freighter on a course to that system
	FleetStruct* transportFleet = new FleetStruct();
	transportFleet->CurrentSystem = this;
	transportFleet->TargetSystem = validSystems.at(systemIdx);
	transportFleet->Ships.push_back(ship);
	this->RemoveShipInstance(ship); //remove it from the system
	//transportFleet->Ships.back()->Activity = ShipActivity::Standard;
	transportFleet->Location = sf::Vector2f(this->Location);
	FreeFleets->push_back(transportFleet);

}

float SystemDistance(sf::Vector2f a, sf::Vector2f b)
{
	float x, y;
	x = abs(a.x - b.x);

	y = abs(a.y - b.y);



	return (sqrt(x * x + y * y) / 75);
}



void System::FreighterArrive(ShipInstance* ship)
{
	//add population from freighter
	for (size_t i = 0; i < Planets.size(); i++)
	{
		if (Planets.at(i)->Population < Planets.at(i)->MaxPop && Planets.at(i)->Population)
		{
			Planets.at(i)->Population += ship->Type->Crew;
			break;
		}
	}
	SimpleMetal += ship->Resources["Simple"];
	Titanium += ship->Resources["Titanium"];
	RareMetal += ship->Resources["Rare"];
	
}

void System::TerraformingTick()
{
	
	for (size_t i = 0; i < SystemShips.size(); i++)
	{
		if (SystemShips.at(i)->Activity == ShipActivity::Terraform)
		{
			
			//do the terraforming for this planet
			SystemShips.at(i)->TerraformingPlanet->TerraformTime -= (float(SystemShips.at(i)->Type->Crew)/1000); // it's 1000 divided by 10 as it happens 10 times a year
			if (SystemShips.at(i)->TerraformingPlanet->TerraformTime<0)
			{
				if (SystemShips.at(i)->TerraformingPlanet->PlanetType == PlanetClasses::Y)
				{
					SystemShips.at(i)->TerraformingPlanet->PlanetType = PlanetClasses::H;
				}
				if (SystemShips.at(i)->TerraformingPlanet->PlanetType == PlanetClasses::H)
				{
					SystemShips.at(i)->TerraformingPlanet->ArableLand += SystemShips.at(i)->TerraformingPlanet->PlanetSize;
					SystemShips.at(i)->TerraformingPlanet->PlanetType = PlanetClasses::L;
				}
				if (SystemShips.at(i)->TerraformingPlanet->PlanetType == PlanetClasses::L)
				{
					SystemShips.at(i)->TerraformingPlanet->ArableLand += 1;
					SystemShips.at(i)->TerraformingPlanet->ArableLand *= 50;
					SystemShips.at(i)->TerraformingPlanet->PlanetType = PlanetClasses::K;
				}
				if (SystemShips.at(i)->TerraformingPlanet->PlanetType == PlanetClasses::K)
				{
					SystemShips.at(i)->TerraformingPlanet->ArableLand *= 300;
					SystemShips.at(i)->TerraformingPlanet->PlanetType = PlanetClasses::M;
					SystemShips.at(i)->Activity = ShipActivity::Standard;
					//this is the end state 
				}
				SystemShips.at(i)->TerraformingPlanet->TerraformTime = sqrt(SystemShips.at(i)->TerraformingPlanet->PlanetSize) * 8;
			}
		}
	}

}

//basicly a planet function

int System::GetBestObservatory()
{
	int out = 0;
	for (size_t i = 0; i < Planets.size(); i++)
	{
		if (Planets.at(i)->Observatory>out)
		{
			out = Planets.at(i)->Observatory;
		}
	}

	return out;
}

float System::DistanceTo(System* input)
{
	return SystemDistance(sf::Vector2f(Location), sf::Vector2f(input->Location));

}


