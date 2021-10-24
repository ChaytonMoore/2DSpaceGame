#pragma once
#include "System.h"
#include <algorithm>

struct FactionData
{
	Faction* OwningFaction;
	std::vector<System*> OwnedSystems; //this needs to be a complete list
	std::vector<System*> KnownSystems;
	std::vector<System*> ExploredSystems;
	bool isRebel = false;

	std::vector<Faction*> Traders;
	std::vector<Faction*> Friends;
	std::vector<Faction*> Allies;

	std::vector<FactionData*> GetKnownFactions(std::vector<FactionData*> in)
	{
		std::vector<Faction*> t;
		std::vector<FactionData*> out;
		
		for (size_t i = 0; i < KnownSystems.size(); i++)
		{
			if (KnownSystems.at(i)->OwningFaction)
			{
				continue;
			}
			t = FactionData::GetFactions(out);
			if (KnownSystems.at(i)->OwningFaction != OwningFaction && std::find(t.begin(), t.end(), KnownSystems.at(i)->OwningFaction) != t.end())
			{
				for (size_t j = 0; j < in.size(); j++)
				{
					if (in.at(j)->OwningFaction == KnownSystems.at(i)->OwningFaction)
					{
						out.push_back(in.at(j));
						break;
					}
				}
			}
		}
		
		return out;
	}
	
	std::vector<Faction*> AtWar;


	//This should happen every 10th of a year
	void FactionTick(std::vector<System*> Systems,std::vector<FactionData*> Factions);
	void FactionEventTick(double deltatime,std::vector<System*> Systems, std::vector<FactionData*> Factions, FactionData* PlayerFaction);
	

	FactionData(std::string name, bool rebel);
	FactionData(std::string name);

	operator Faction*()
	{
		return OwningFaction;
	}

	static std::vector<Faction*> GetFactions(std::vector<FactionData*> in);

	//This will only happen if the faction isn't classified as inert
	void MonthlyAITick(); //not really a month more a tenth of a year


	//This is duplicate code from main.cpp
	std::vector<ShipType*> GetBuildableShips(Faction* PlayerFaction, System* CurrentSystem);

	ShipType* GetColonyShipType(Faction* PlayerFaction, System* CurrentSystem);


	ShipInstance* GetColonyShip(System* input);

	void ReleaseRebelliousSystems(std::vector<FactionData*>* factions);

	static int GetSystemLoyalty(System* in);


	//only relavent if it's a rebel faction
	FactionData* OriginFaction;


private:
	double CurrentTimeStamp = 0;
	int LastTime = 0;
};