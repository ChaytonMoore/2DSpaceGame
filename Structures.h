#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
//This file will contain the headers for the functions and the main code for the structs

//Might be a better idea to inherit this data from the files instead of having it predefined
enum WaterData
{
	noWater,
	shallowWater,
	midWater,
	deepWater
};

enum TerrainType
{
	highMountain,
	lowMountain,
	deepForest,
	lightForest,
	hill,
	lowFerfile,
	midFertile,
	highFertile,
	wilderness
};

enum TileFeature
{
	non,
	river,
	ford,
	cliff
};

enum ClimateType
{
	mediterranean,
	sandDesert,
	rockDesert,
	alpine
};


struct SettlementStruct
{
	int Level;
	std::vector<std::string> Buildings;
	std::string CultureBase;

};

//This is the struct that defines a province
struct ProvinceStruct
{
	std::string RegionName;
	std::string SettlementName;
	sf::Color ColourValue;
	std::string Culture;

	std::vector<std::string> Resources;
	SettlementStruct Settlement;

	std::string RebelType;

	int VictoryPoints;
	int Fertility;
};

//This is compiled from a all the data from the various different maps
struct DataRichStruct
{

	int height[4]; //4 long because there a 4 sub files. top right, top left, bottom right then bottom left is the order
	int rough[4];
	WaterData water[4];
	TerrainType terrain[4];
	ClimateType climate[4];
	ProvinceStruct* Province;
	bool City, Port;

};

struct MapRow
{
	std::vector<DataRichStruct> Data;
};

std::vector<ProvinceStruct> CompileRegions();