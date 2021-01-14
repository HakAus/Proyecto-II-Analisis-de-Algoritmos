#include <queue>
#include <vector>
#include "libs/rapidjson/document.h"
#include "terrain.h"
#include "terrainPrototype.h"
#include <iostream>
#include <string>
#include "libs/rapidjson/writer.h"
#include <fstream>
#include <ctime>

class TerrainGenerator 
{
private:
	int currentDistance = 0;
	int distance = 200; //Valor de prueba
	int minStretchLength = 40; //Largo del tramo
	int maxStretchLength = 80;
	int numeroDeTerrenos = 0;
	int minimunTerrainLength = 5; //Largo del terreno
	int maximunTerrainLength = 20;
	std::queue<rapidjson::StringBuffer*>* sharedBuffer;
	std::vector<TerrainPrototype*> terrains;

public:
	TerrainGenerator(const rapidjson::Document& pConfig, std::queue<rapidjson::StringBuffer*>* pSharedBuffer);
	void generateTerrains();
	void showTerrains();
	void showQueue();
};