#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "reader.h"
#include "terrain.h"
#include "vehicle.h"
// #include "sync-queue.h"
#include "terrain-generator.h"
#include "genetic-algorithm.h"


class Simulation
{

private:
	Reader * reader;
	TerrainGenerator * terrainGenerator;
	GeneticAlgorithm * geneticAlgorithm;
	std::map<Terrain*, Vehicle*> solution;

	std::thread producer;
	std::thread consumer;
	std::mutex mutex;
	std::condition_variable condition;
	std::queue<int/*std::vector<Terrain*>*/> sharedQueue;

public:
	Simulation();
	void start(int pDistance);
	void join();
	void showSolution();
};