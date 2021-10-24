#include "Planet.h"


Planet::Planet(float size, PlanetClasses type)
{
	PlanetSize = size;
	PlanetType = type;


	TerraformTime = sqrt(size) * 8;
	
}


int Planet::CalculateMetalOutput()
{
	//The base is the tenth root of the population
	int base = pow(Population,0.1);
	base += Mining;
	return base;
}

int Planet::CalculateTitaniumOutput()
{
	return Mining * 2;
}

int Planet::CalculateRareOutput()
{
	return (Mining > 1) * (Mining - 1);

}




float trunc_decs(float f,int decs)
{
	int i1 = floor(f);
	float rmnd = f - i1;
	int i2 = static_cast<int> (rmnd * pow(10, decs));
	float f1 = i2 / pow(10, decs);

	return i1 + f1;
}


std::string LargeToString(uint64_t num)
{

	

	std::string out;

	

	if (num>1000000000)
	{
		out = std::to_string(trunc_decs(((float)num / 1000000000), 2)).substr(0,4) + " B";
	}
	else if(num>1000000)
	{
		out = std::to_string(trunc_decs(((float)num / 1000000), 2)).substr(0,4) + " M";
	}
	else if (num > 1000)
	{
		out = std::to_string(trunc_decs(((float)num / 1000), 2)).substr(0,4) + " K";
	}
	else
	{
		out = std::to_string(num);
	}



	return out;
}


std::vector<PlanetTexture*> LoadPlanetTextures()
{
	std::vector<PlanetTexture*> output;


	std::string string;

	
	std::ifstream File("C:/Users/User/Documents/2DSpaceGame/Textures/PlanetRegistry.txt");

	bool Completed = true;

	sf::Image* Temp = new sf::Image;
	while (std::getline(File, string))
	{
		Completed = Temp->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/"+string+".png") * Completed;
		output.push_back(new PlanetTexture);
		output.back()->texture = new sf::Texture; //This line might be redundant and creates a duplicte file
		output.back()->texture->loadFromImage(*Temp);
		output.back()->type = string[0];
		
	}

	if (!Completed)
	{
		int msgboxID = MessageBox(
			NULL,
			("Not all of the planets could be loaded."), //contents
			"File Error", //title
			MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);
		

		switch (msgboxID)
		{
		case IDCANCEL:
			exit(-1);
			
		case IDTRYAGAIN:
			output = LoadPlanetTextures();
			//This might cause a memory leak
			//TODO fix memory leak
			//It does but it's a fairly small memory leak

		
			
			
		}

	}


	return output;
}


float PopulationCurve(int Population, int max)
{
	float out = 0;
	if (Population > 50 && Population < 200)
	{
		out = 0.00033 * ((float)Population) - 6.62;
	}
	if (Population >= 200 && Population < 100000)
	{
		out = 0.05 - (3 * pow(10, -7)) * (float)Population;
	}
	if (Population >= 100000)
	{
		out = 0.02;
	}

	//The output is for delta population
	return out;

}

void Planet::PlanetYearTick(Faction* OwningFaction)
{

	MaxPop = (ArableLand * (float)(9 + OwningFaction->FarmingTech) / 10) * 1000000 + OwningFaction->FarmingTech * 100000 * PlanetSize;


	//population can't increase more than the arable land *(farmingtech+9)/10
	if (Population < MaxPop)
	{
		Population += Population * PopulationCurve(Population, (ArableLand * (float)(9 + OwningFaction->FarmingTech) / 10));
	}


}