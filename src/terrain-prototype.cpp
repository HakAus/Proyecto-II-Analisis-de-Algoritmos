#include "terrain-prototype.h"
#include <iostream>

TerrainPrototype::TerrainPrototype(std::string pName, int* ranges)
{
	name = pName;
	for (int i = 0; i < 2; i++)
	{
		firmnessRange[i] = ranges[i];
		humidityRange[i] = ranges[i+2];
		gripRange[i] = ranges[i+4];
	}

}

Terrain* TerrainPrototype::getTerrain(int currentDistance, int finalDistance)
{
	float attributes[3] = { 
						Random::RandomRange(firmnessRange[0],firmnessRange[1]),
						Random::RandomRange(humidityRange[0],humidityRange[1]),
						Random::RandomRange(gripRange[0],gripRange[1]) 
					  };
	return new Terrain(name.c_str(), attributes, currentDistance, currentDistance + finalDistance);
}

std::string TerrainPrototype::toString()
{
	return "Terreno: " + name + "Firmeza: "+std::to_string(firmnessRange[0])+ " " + std::to_string(firmnessRange[1])+ "Humedad: " +std::to_string(humidityRange[0])+" " + std::to_string(humidityRange[1])+ "Agarre: " + std::to_string(gripRange[0])
		+" " + std::to_string(gripRange[1])+"\n";
}
