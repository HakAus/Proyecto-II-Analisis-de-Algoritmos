#include <queue>
#include <map>

#include "reader.h"
#include "terrain.h"
#include "vehicle.h"
#include "terrain-generator.h"
#include "genetic-algorithm.h"

class Simulation
{

private:
	Reader * reader;
	TerrainGenerator * terrainGenerator;
	GeneticAlgorithm * geneticAlgorithm;
	std::queue<rapidjson::StringBuffer *> sharedBuffer;
	std::map<Terrain*, Vehicle*> solution;

public:
	Simulation();
	void start();
	void showSolution();
};