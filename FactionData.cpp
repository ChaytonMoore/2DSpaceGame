#include "FactionData.h"

void FactionData::FactionTick(std::vector<System*> Systems, std::vector<FactionData*> Factions)
{
	//This part of the alghorithm will find which systems can be seen by a faction

	for (size_t i = 0; i < Systems.size(); i++)
	{
		//also check if it has been explored
		if (!(std::find(KnownSystems.begin(), KnownSystems.end(), Systems.at(i)) != KnownSystems.end()))
		{
			for (size_t j = 0; j < OwnedSystems.size(); j++)
			{
				
				if (OwnedSystems.at(j)->GetBestObservatory()*3 > SystemDistance(sf::Vector2f(Systems.at(i)->Location), sf::Vector2f(OwnedSystems.at(j)->Location)))
				{
					
					KnownSystems.push_back(Systems.at(i));
					break;
				}
			}
		}


	}
	

}

FactionData::FactionData(std::string name, bool rebel)
{
	OwningFaction = new Faction(name);
	isRebel = rebel;
	OwningFaction->LoadShips();


}

FactionData::FactionData(std::string name)
{
	OwningFaction = new Faction(name);
	OwningFaction->LoadShips();


}

std::vector<Faction*> FactionData::GetFactions(std::vector<FactionData*> in)
{
	std::vector<Faction*> out;
	for (size_t i = 0; i < in.size(); i++)
	{
		out.push_back(in.at(i)->OwningFaction);
	}



	return out;
}


int GetBestPlanet(ShipType* a,System* system)
{
	int output = -1; //-1 means no valid planet

	//critera is that the planet must be unoccupied 
	for (size_t i = 0; i < system->Planets.size(); i++)
	{
		if (output == -1)
		{

			if (std::find(a->Colonisable.begin(), a->Colonisable.end(), system->Planets.at(i)->PlanetType) != a->Colonisable.end())
			{
				output = i;
			}
		}
		else
		{

			if (system->Planets.at(i)->Population == 0 && system->Planets.at(i)->MaxPop > system->Planets.at(output)->MaxPop && std::find(a->Colonisable.begin(), a->Colonisable.end(), system->Planets.at(i)->PlanetType) != a->Colonisable.end())
			{
				output = i;
			}
		}
	}

	return output;
}

void FactionData::MonthlyAITick()
{
	//attempt expansion
	if (OwnedSystems.size() < OwningFaction->EngineTech * 3 && OwningFaction->UnifiedPlanet && OwningFaction->Colonial)
	{
		//if this is true it means the faction can expand
		
		//Now the decision for the AI to make
		if (OwnedSystems.size()>(OwningFaction->Colonial/10))
		{
			//if the faction wants to colonise
			//now need to choose the system, the algorithm will go through each owned system and go through the known systems within range.
			for (size_t i = 0; i < OwnedSystems.size(); i++)
			{
				if (GetColonyShip(OwnedSystems.at(i)))
				{
					bool found = false;
					//there is a valid colony ships that can be sent
					//now search for a valid system to send it for
					for (size_t j = 0; j < KnownSystems.size(); j++)
					{
						if (GetColonyShip(OwnedSystems.at(i))->RemainingRange <= OwnedSystems.at(i)->DistanceTo(KnownSystems.at(j)))
						{
							//Then there is a valid system it can move to
							//Move Ship
							found = true;
							break;
						}
					}
					if (!found)
					{
						for (size_t j = 0; j < OwnedSystems.size(); j++)
						{
							if (i!=j&&GetColonyShip(OwnedSystems.at(i))->RemainingRange <= OwnedSystems.at(i)->DistanceTo(OwnedSystems.at(j)))
							{
								//Then there is a valid system it can move to
								//Move Ship
								
								break;
							}
						}
					}


					break;
				}
				else
				{
					//There is no colony ship but they can build one and so shall
					bool CanBuild = true;
					//firstly check that there is no ship already being built
					for (size_t j = 0; j < OwnedSystems.size(); j++)
					{
						for (size_t n = 0; n < OwnedSystems.at(j)->ShipBuildQueue.size(); n++)
						{
							if (OwnedSystems.at(j)->ShipBuildQueue.at(n).Type->type == "colony" || OwnedSystems.at(j)->ShipBuildQueue.at(n).Type->type == "colonyBig")
							{
								CanBuild = false;
							}
						}
					}

					if (CanBuild)
					{
						//start building the ship
						OwnedSystems.at(i)->ShipBuildQueue.push_back(*new ShipConstructData());
						OwnedSystems.at(i)->ShipBuildQueue.back().Type = GetColonyShipType(OwningFaction,OwnedSystems.at(i));
						OwnedSystems.at(i)->ShipBuildQueue.back().Remaining = OwnedSystems.at(i)->ShipBuildQueue.back().Type->ConstructTime;

						break;
					}

				}
			}
		}



		//now execute all the fleet comamnds in the system
		for (size_t i = 0; i < KnownSystems.size(); i++)
		{
			if (std::find(OwnedSystems.begin(), OwnedSystems.end(), KnownSystems.at(i)) != OwnedSystems.end())
			{


				for (size_t j = 0; j < KnownSystems.at(i)->SystemShips.size(); j++)
				{
					//firstly do all the functions for a colony ship
					if (KnownSystems.at(i)->SystemShips.at(j)->Type->type == "colony" || KnownSystems.at(i)->SystemShips.at(j)->Type->type == "colonyBig" && GetBestPlanet(KnownSystems.at(i)->SystemShips.at(j)->Type, KnownSystems.at(i)) != -1)
					{

						KnownSystems.at(i)->OwningFaction = OwningFaction;

						KnownSystems.at(i)->Planets.at(GetBestPlanet(KnownSystems.at(i)->SystemShips.at(j)->Type, KnownSystems.at(i)))->Population += KnownSystems.at(i)->SystemShips.at(j)->Type->Crew;
						delete KnownSystems.at(i)->SystemShips.at(j);
						KnownSystems.at(i)->SystemShips.erase(KnownSystems.at(i)->SystemShips.begin()+j);

					}
				}
			}

		}



	}



}

std::vector<ShipType*> FactionData::GetBuildableShips(Faction* PlayerFaction, System* CurrentSystem)
{
	std::vector<ShipType*> output;
	if (CurrentSystem)
	{


		//first check if the system is the same as the player faction
		if (PlayerFaction == CurrentSystem->OwningFaction)
		{
			bool valid;
			for (size_t i = 0; i < PlayerFaction->FactionShips.size(); i++)
			{

				valid = CurrentSystem->SimpleMetal > PlayerFaction->FactionShips.at(i)->RequiredMetal && CurrentSystem->Titanium > PlayerFaction->FactionShips.at(i)->RequiredTitanium && CurrentSystem->RareMetal > PlayerFaction->FactionShips.at(i)->RequiredRare;
				valid = valid && PlayerFaction->EngineTech >= PlayerFaction->FactionShips.at(i)->EngineTech && PlayerFaction->ShipbuildingTech >= PlayerFaction->FactionShips.at(i)->ShipTech && PlayerFaction->ShieldTech >= PlayerFaction->FactionShips.at(i)->ShieldTech && PlayerFaction->SensorTech >= PlayerFaction->FactionShips.at(i)->Sensortech && PlayerFaction->WeaponsTech >= PlayerFaction->FactionShips.at(i)->WeaponTech;
				valid = valid && CurrentSystem->GetPopulation() > PlayerFaction->FactionShips.at(i)->Crew * 100;
				valid = valid && CurrentSystem->SimpleMetal >= PlayerFaction->FactionShips.at(i)->RequiredMetal && CurrentSystem->Titanium >= PlayerFaction->FactionShips.at(i)->RequiredTitanium && CurrentSystem->RareMetal >= PlayerFaction->FactionShips.at(i)->RequiredRare;

				if (valid)
				{
					output.push_back(PlayerFaction->FactionShips.at(i));
				}
			}
		}

	}

	std::string varients[8] = { "scout","frigate","destroyer","cruiser","dreadnought","transport","colony","colonyBig" };
	ShipType* LastInstance;
	int idx;
	for (size_t i = 0; i < 8; i++)
	{
		LastInstance = nullptr;
		idx = output.size();
		idx--;
		while (idx > 0)
		{

			if (LastInstance && output.at(idx)->type == varients[i])
			{
				output.erase(output.begin() + idx);

			}
			else if (output.at(idx)->type == varients[i])
			{
				LastInstance = output.at(idx);
			}
			idx--;

		}
	}

	return output;
}

ShipType* FactionData::GetColonyShipType(Faction* PlayerFaction, System* CurrentSystem)
{
	std::vector<ShipType*> initial = GetBuildableShips(PlayerFaction, CurrentSystem);
	std::vector<ShipType*> valid;
	for (size_t i = 0; i < initial.size(); i++)
	{
		if (initial.at(i)->type == "colony"|| initial.at(i)->type == "colonyBig")
		{
			valid.push_back(initial.at(i));
		}
	}
	if (valid.size()==1)
	{
		return valid.at(0);
	}
	else if(valid.size() == 0)
	{
		return nullptr;
	}
	else
	{
		return valid.at(rand() % valid.size());
	}


}

ShipInstance* FactionData::GetColonyShip(System* input)
{
	ShipInstance* output = nullptr;
	for (size_t i = 0; i < input->SystemShips.size(); i++)
	{
		if ((input->SystemShips.at(i)->Type->type == "colony" || input->SystemShips.at(i)->Type->type == "colonyBig") && input->SystemShips.at(i)->OwningFaction == OwningFaction)
		{
			output = input->SystemShips.at(i);
			break;
		}
	}



	return output;
}

void FactionData::FactionEventTick(double deltatime, std::vector<System*> Systems, std::vector<FactionData*> Factions, FactionData* PlayerFaction)
{
	//called for every faction every tick
	
	//The ai tick is only done every tenth of a year
	if (CurrentTimeStamp > float(LastTime) / 10) //10th of year
	{
		if (PlayerFaction != this)
		{
			MonthlyAITick();
		}
	}

	CurrentTimeStamp += deltatime / 100;
}


int FactionData::GetSystemLoyalty(System* in)
{
	//TODO fill out this function
	int out = 100;
	if (in->GetName() != "Sol")
	{
		out -= 150;
	}
	
	
	
	return out;
}

void FactionData::ReleaseRebelliousSystems(std::vector<FactionData*>* factions)
{
	
	for (size_t i = 0; i < OwnedSystems.size(); i++)
	{
		if (FactionData::GetSystemLoyalty(OwnedSystems.at(i))<10)
		{
			
			//release them
			factions->push_back(new FactionData(OwnedSystems.at(i)->GetName(),true));
			factions->back()->OriginFaction = this;
			factions->back()->isRebel = true; //to make sure
			//^ creates a new faction
			factions->back()->OwnedSystems.push_back(OwnedSystems.at(i));
			//^adds the system
			OwnedSystems.at(i)->OwningFaction = factions->back()->OwningFaction;
			OwnedSystems.at(i)->CurrentSprite = &OwnedSystems.at(i)->NeutralSprite;
			//system local stuff

			OwnedSystems.erase(OwnedSystems.begin()+i);
			//^ remove from other faction's owned systems
		}
	}
	

}