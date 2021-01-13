#include <queue>
#include <vector>
#include "libs/rapidjson/document.h"
#include "terrain.h"
#include "terrainPrototype.h"
#include <iostream>
#include <string>

class TerrainGenerator 
{
private:
	const float minimunTerrainDistancePercentage = 0.10;
	const float maximunTerrainDistancePercentage = 0.20;
	int currentDistance = 0;
	int distance;
	std::queue<std::vector<Terrain*>>* sharedBuffer;
	std::vector<TerrainPrototype*> terrains;

public:
	TerrainGenerator(const rapidjson::Document& pConfig);
	void generateTerrains();
	void showTerrains();
};