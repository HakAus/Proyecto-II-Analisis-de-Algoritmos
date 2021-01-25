#include "terrain.h"
#include <stdlib.h>
#include "Random.cpp"
#include <stdio.h>
#include <string>

class TerrainPrototype 
{
public:
	std::string name;
	int firmessRange[2];
	int humidityRange[2];
	int gripRange[2];

	TerrainPrototype(std::string pName,int * ranges);
	Terrain * getTerrain(int currentDistance, int finalDistance);
	std::string toString();
};