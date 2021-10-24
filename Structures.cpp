#include "Structures.h"
#include <fstream>
//This file contains all the functions regarding the the structures.
struct SubTileRow
{
	std::vector<int> Data;
};

std::vector<std::string> split(std::string input, char delim)
{
	std::vector<std::string> output;
	bool writing = false;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == delim)
		{
			writing = false;
		}

		if (input[i] != delim)
		{
			if (!writing)
			{
				output.push_back(*new std::string);
			}
			writing = true;
			output.back().push_back(input[i]);
		}
	}


	return output;
}


std::vector<ProvinceStruct> CompileRegions()
{

	std::vector<ProvinceStruct> Output;

	std::string temp[8];
	int idx = 0;

	Output.push_back(*new ProvinceStruct);
	std::ifstream RegionsFile("descr_regions.txt");
	while (!RegionsFile.eof())
	{
		std::getline(RegionsFile,temp[idx]);
		idx++;



		if (idx==8)
		{
			Output.back().RegionName = temp[0];
			Output.back().SettlementName = temp[1];
			Output.back().Culture = temp[2];
			Output.back().RebelType = temp[3];

			Output.back().ColourValue.r = std::stoi(split(temp[4],' ')[0],nullptr,10);
			Output.back().ColourValue.b = std::stoi(split(temp[4],' ')[1], nullptr, 10);
			Output.back().ColourValue.g = std::stoi(split(temp[4],' ')[2], nullptr, 10);

			Output.back().Resources = split(temp[5], ',');
			

			idx = 0;
			//Create new struct
			Output.push_back(*new ProvinceStruct);
		}
		


	}
	RegionsFile.close();
	




	return Output;
}

std::vector<MapRow> ComposeMapData(sf::Image GroundData, sf::Image RegionsData)
{
	//Firstly create the actual 2d list from the regions data

	std::vector<MapRow> Output;
	for (size_t i = 0; i < RegionsData.getSize().x; i++)
	{
		Output.push_back(*new MapRow);
		for (size_t i = 0; i < RegionsData.getSize().y; i++)
		{
			Output.back().Data.push_back(*new DataRichStruct);
			//Output.back().Data.back().Province = 
		}
	}



	return Output;
}


