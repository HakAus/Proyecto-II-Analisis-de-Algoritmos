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
	int currentDistance = 0;
	int distance = 200; //Valor de prueba
	int numeroDeTerrenos = 0;
	int minimunTerrainLength = 5;
	int maximunTerrainLength = 20;
	std::queue<std::vector<Terrain*>>* sharedBuffer;
	std::vector<TerrainPrototype*> terrains;

public:
	TerrainGenerator(const rapidjson::Document& pConfig);
	void generateTerrains();
	void showTerrains();
	void increasePercentage(float* percentage);
};