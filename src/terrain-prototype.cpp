#include "terrainPrototype.h"
#include <iostream>

TerrainPrototype::TerrainPrototype(std::string pName, int* ranges)
{
	name = pName;
	for (int i = 0; i < 2; i++)
	{
		firmessRange[i] = ranges[i];
		humidityRange[i] = ranges[i+2];
		gripRange[i] = ranges[i+4];
	}

}

Terrain * TerrainPrototype::getTerrain(int currentDistance,int finalDistance, int minimunTerrainLength,int maximunTerrainLength)
{
	int terrainDistance = Random::RandomRange(minimunTerrainLength, maximunTerrainLength);
	int distanceLeft = finalDistance - currentDistance;
	if (currentDistance + terrainDistance > distanceLeft)
		terrainDistance = distanceLeft;
	float values[3] = {Random::RandomRange(firmessRange[0],firmessRange[1]),Random::RandomRange(humidityRange[0],humidityRange[1]),Random::RandomRange(gripRange[0],gripRange[1])};
	return new Terrain(name.c_str(),values,currentDistance, currentDistance + terrainDistance);
}

std::string TerrainPrototype::toString()
{
	return "Terreno: " + name + "Firmeza: "+std::to_string(firmessRange[0])+ " " + std::to_string(firmessRange[1])+ "Humedad: " +std::to_string(humidityRange[0])+" " + std::to_string(humidityRange[1])+ "Agarre: " + std::to_string(gripRange[0])
		+" " + std::to_string(gripRange[1])+"\n";
}
