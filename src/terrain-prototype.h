#include "terrain.h"
#include <stdlib.h>
#include "random.cpp"
#include <stdio.h>
#include <string>

class TerrainPrototype 
{
public:
	std::string name;
	int firmnessRange[2];
	int humidityRange[2];
	int gripRange[2];

	TerrainPrototype(std::string pName,int * ranges);
	Terrain * getTerrain(int currentDistance, int finalDistance);
	std::string toString();
};