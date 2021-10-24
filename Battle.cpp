#include "Battle.h"



float GetDistance(sf::Vector2f Origin, sf::Vector2f Target)
{
	float x = Target.x - Origin.x;
	float y = Target.y - Origin.y;
	return sqrt(pow(x,2)+pow(y,2));

}

void BattleShip::SetLookAtRotation()
{
	float x = Target->Location.x - Location.x;
	float y = Target->Location.y - Location.y;
	
	Direction = atan(x/y);
	
}

void BattleShip::MoveShip()
{
	if (MovementMode == "attack")
	{
		
		float x = Target->GetCentreLocation().x - Location.x;
		float y = Target->GetCentreLocation().y - Location.y;
		float length = sqrt(x * x + y * y);
		float xM = (x / length) * Speed * 0.01;
		float yM = (y / length) * Speed * 0.01;

		Location += sf::Vector2f(xM, yM);
		
		if (GetDistance(Location,Target->GetCentreLocation())<GetLowestRangedWeapon()->Range)
		{
			//run away
			MovementMode = "back";
		}
		if (Location.x < 0 || Location.x>1920 || Location.y > 1080 || Location.y < 0)
		{
			MovementMode = "random";
			MovementTarget = sf::Vector2f(rand() % 1920, rand() % 1080);
		}
	}
	if (MovementMode == "back")
	{
		
		float x = Target->GetCentreLocation().x - Location.x;
		float y = Target->GetCentreLocation().y - Location.y;
		float length = sqrt(x * x + y * y);
		float xM = (x / length) * Speed * 0.01;
		float yM = (y / length) * Speed * 0.01;

		Location -= sf::Vector2f(xM, yM); //inverse to attack
		if (GetDistance(Location, Target->GetCentreLocation()) > GetLongestRangedWeapon()->Range)
		{
			
			MovementMode = "attack";
		}
		if (GetCentreLocation().x <0 || GetCentreLocation().x>1920 || GetCentreLocation().y>1080 || GetCentreLocation().y<0)
		{

			MovementMode = "random";
			MovementTarget = sf::Vector2f(rand() % 1920, rand() % 1080);
			
		}
	}
	if (MovementMode == "random")
	{
		float x = MovementTarget.x - Location.x;
		float y = MovementTarget.y - Location.y;
		float length = sqrt(x * x + y * y);
		float xM = (x / length) * Speed * 0.01;
		float yM = (y / length) * Speed * 0.01;

		Location += sf::Vector2f(xM, yM);
		if (GetDistance(Location, MovementTarget) < 10)
		{
			MovementMode = "attack";
		}
		
	}

	
}

sf::Vector2f BattleShip::GetDirection(sf::Vector2f Origin, sf::Vector2f Target)
{
	float x = Target.x - Origin.x;
	float y = Target.y - Origin.y;
	float l = sqrt(x * x + y * y);
	return sf::Vector2f(x/l,y/l);
}




void FireKinetic(BattleShip* Parent, BattleShip* Target)
{
	Parent->WeaponElements.push_back(new KineticType(static_cast<KineticType*>(Parent->WeaponElements.at(1))));
}

Weapon* BattleShip::GetLowestRangedWeapon()
{
	Weapon* lowest = nullptr;
	int range = MAXINT;
	for (size_t i = 0; i < WeaponElements.size(); i++)
	{
		if (WeaponElements.at(i)->Range < range)
		{
			lowest = WeaponElements.at(i);
			range = WeaponElements.at(i)->Range;
		}
	}
	return lowest;

}

Weapon* BattleShip::GetLongestRangedWeapon()
{
	Weapon* lognest = nullptr;
	int range =0;
	for (size_t i = 0; i < WeaponElements.size(); i++)
	{
		if (WeaponElements.at(i)->Range > range)
		{
			lognest = WeaponElements.at(i);
			range = WeaponElements.at(i)->Range;
		}
	}
	return lognest;

}

void BattleShipEventTick(std::vector<BattleShip*> Enemyships, std::vector<TorpedoType*>* FreeTorpedos, BattleShip* owner, std::vector<KineticType*>* FreeKinetics, std::vector<BeamType*>* FreeBeams, BattleSounds* Sounds)
{
	


	//first of make sure that there is a target
	if (owner->Target == nullptr)
	{
		owner->Target = Enemyships.at(rand() % Enemyships.size());
		
	}
	
	owner->MoveShip();
	

	//Get nearest for now we'll just use the first element
	sf::Vector2f Delta = BattleShip::GetDirection(owner->Location, owner->Target->Location);
	float Distance = GetDistance(owner->Location, owner->Target->Location);
	


	owner->Sprite.setRotation(owner->Direction);
	owner->Sprite.setPosition(owner->Location);


	//Weapon Firing

	//KineticFiring
	if (Distance< owner->WeaponElements.at(1)->Range && owner->KineticTickDelay<0)
	{
		owner->KineticTickDelay = 4500 + rand() % 1000;
		FreeKinetics->push_back(new KineticType(static_cast<KineticType*>(owner->WeaponElements.at(1))));
		FreeKinetics->back()->Location = owner->GetCentreLocation();
		FreeKinetics->back()->Origin = owner;
		float x = owner->Target->GetCentreLocation().x - FreeKinetics->back()->Location.x;
		float y = owner->Target->GetCentreLocation().y - FreeKinetics->back()->Location.y;
		float length = sqrt(x * x + y * y);
		FreeKinetics->back()->Vector = sf::Vector2f(x/length,y/length);
		FreeKinetics->back()->line->color == sf::Color(175,175,190);

		owner->Railgun->play();

	}
	if (Distance < owner->WeaponElements.at(0)->Range && owner->TorpedoTickDelay < 0)
	{
		owner->TorpedoTickDelay = 9500 + rand() % 1000;
		FreeTorpedos->push_back(new TorpedoType(static_cast<TorpedoType*>(owner->WeaponElements.at(0))));
		FreeTorpedos->back()->Location = owner->GetCentreLocation();
		FreeTorpedos->back()->Target = owner->Target;
		FreeTorpedos->back()->Circle = new sf::CircleShape(3);
		FreeTorpedos->back()->Circle->setPosition(owner->Location);

	}
	if (Distance < owner->WeaponElements.at(2)->Range && owner->BeamTickDelay < 0)
	{
		owner->BeamTickDelay = 3500 +rand() % 1000;
		FreeBeams->push_back(new BeamType(static_cast<BeamType*>(owner->WeaponElements.at(2))));
		FreeBeams->back()->Location = owner->GetCentreLocation();
		FreeBeams->back()->Target = owner->Target;
		FreeBeams->back()->Origin = owner;
		FreeBeams->back()->line[0].color = sf::Color(255,0,0);
		FreeBeams->back()->line[1].color = sf::Color(230,100,100);
		owner->Laser->play();
		std::cout << Distance << std::endl;

	}
	


	//Set the position of the sprite
	//Sprite.setPosition(Location);

	//Decrease weapon ticks
	owner->KineticTickDelay--;
	owner->BeamTickDelay--;
	owner->TorpedoTickDelay--;

	//health indicators
	owner->HullInd->setPosition(owner->Location+sf::Vector2f(20,20));
	if (owner->Parent->Shields > 0)
	{
		owner->ShieldInd->setPosition(owner->Location + sf::Vector2f(20, 40));
		owner->ShieldInd->setRadius(5 * owner->ShieldStrength / owner->Parent->Shields);
	}

	owner->HullInd->setRadius(5 * owner->HullStrength/owner->Parent->Hull);



	
}


bool TorpedoType::EventTick()
{
	float x = Target->GetCentreLocation().x - Location.x;
	float y = Target->GetCentreLocation().y - Location.y;
	float length = sqrt(x * x + y * y);
	float xM = (x / length) * Speed * 0.01;
	float yM = (y / length) * Speed * 0.01;
	Location += sf::Vector2f(xM, yM);
	Circle->setPosition(Location);
	if (GetDistance(Target->GetCentreLocation(),Location)<15)
	{
		//perform an attack
		if (Target->ShieldStrength>0)
		{
			Target->ShieldStrength -= Damage;
			if (Target->ShieldStrength < 0)
			{
				Target->HullStrength += Target->ShieldStrength;
			}
		}
		else
		{
			Target->HullStrength -= Damage;
		}
		delete this;
		return true;
	}

	

	return false;

}

bool KineticType::EventTick(std::vector<BattleShip*>* ships1, std::vector<BattleShip*>* ships2)
{

	float xM = Vector.x * Speed * 0.01;
	float yM = Vector.y * Speed * 0.01;
	Location += sf::Vector2f(xM, yM);
	line[0].position = Location;
	line[1].position = Location - sf::Vector2f(Vector.x*3, (Vector.y * 5));

	for (size_t i = 0; i < ships1->size(); i++)
	{
		if (GetDistance(Location,ships1->at(i)->GetCentreLocation())<3 && ships1->at(i) != Origin)
		{
			if (ships1->at(i)->ShieldStrength > 0)
			{
				ships1->at(i)->ShieldStrength -= Damage;
			}
			else
			{
				ships1->at(i)->HullStrength -= Damage;
			}
			return true;
		}
	}

	for (size_t i = 0; i < ships2->size(); i++)
	{
		if (GetDistance(Location, ships2->at(i)->GetCentreLocation()) < 3 && ships2->at(i) != Origin)
		{
			if (ships2->at(i)->ShieldStrength > 0)
			{
				ships2->at(i)->ShieldStrength -= Damage;
			}
			else
			{
				ships2->at(i)->HullStrength -= Damage;
			}
			return true;
		}
	}

	return (Location.x < 0 || Location.x>1920 || Location.y > 1080 || Location.y < 0);

}


std::vector<ShipInstance*> BattleShip::ToInstances(Faction* fac, std::vector<BattleShip*> AllShips, std::vector<ShipInstance*> Original)
{
	std::vector<ShipInstance*> out;
	for (size_t i = 0; i < Original.size(); i++)
	{
		if (AllShips.at(i))
		{
			out.push_back(Original.at(i));
		}
	}


	return out;
}