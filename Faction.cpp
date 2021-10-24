#include "System.h"

int Faction::CalculateFactionRelation(Faction* TargetFaction)
{
	//Calculates the relation between different factions.
	//The relation is closeness of faction types
	//The effects on the factions will also be examined.
	int output = 50;
	//we start at 0

	output -= abs(TargetFaction->Scientificness - Scientificness);
	output -= abs(TargetFaction->Industrialness - Industrialness);
	output -= abs(TargetFaction->Militaryness - Militaryness);
	output += TargetFaction->Xenophileness;

	for (size_t i = 0; i < TargetFaction->RelationEffects.size(); i++)
	{
		if (TargetFaction->RelationEffects.at(i).TargetFaction == this)
		{
			if (TargetFaction->RelationEffects.at(i).Name == "War")
			{
				output -= TargetFaction->RelationEffects.at(i).Itensity;
			}
			if (TargetFaction->RelationEffects.at(i).Name == "Gift")
			{
				output += TargetFaction->RelationEffects.at(i).Itensity;
			}
		}
	}


	return output;
}

//void Faction::FactionTick(std::vector<Faction*> Factions, std::vector<System*> Systems)
//{

//}

ShipInstance::ShipInstance(ShipType* SType, Faction* Fac)
{
	Type = SType;
	RemainingRange = Type->Range;
	HullStrength = Type->Hull;
	OwningFaction = Fac;

}

PlanetClasses PlanetFromChar(char a)
{
	PlanetClasses out = D;

	if (a == 'H')
	{
		out = PlanetClasses::H;
	}
	if (a == 'J')
	{
		out = PlanetClasses::J;
	}
	if (a == 'K')
	{
		out = PlanetClasses::K;
	}
	if (a == 'L')
	{
		out = PlanetClasses::L;
	}
	if (a == 'M')
	{
		out = PlanetClasses::M;
	}
	if (a == 'N')
	{
		out = PlanetClasses::N;
	}
	if (a == 'R')
	{
		out = PlanetClasses::R;
	}
	if (a == 'T')
	{
		out = PlanetClasses::T;
	}
	if (a == 'Y')
	{
		out = PlanetClasses::Y;
	}


	//The switch function wasn't working correctly for some reason

	return out;
}





