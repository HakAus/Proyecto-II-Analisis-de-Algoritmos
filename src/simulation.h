#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "reader.h"
#include "terrain.h"
#include "vehicle.h"
#include "sync-queue.h"
#include "terrain-generator.h"
#include "genetic-algorithm.h"


class Simulation
{

private:
	Reader * reader;
	SyncQueue * queue;
	TerrainGenerator * terrainGenerator;
	GeneticAlgorithm * geneticAlgorithm;
	std::map<Terrain*, Vehicle*> solution;
	
public:
	Simulation();
	void start();
	void join();
	void showSolution();
};