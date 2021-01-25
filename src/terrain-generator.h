#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../libs/rapidjson/pointer.h"
#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/prettywriter.h"
#include "terrain.h"
#include "terrain-prototype.h"
#include "sync-queue.h"

class TerrainGenerator 
{
private:
	int currentDistance;	// Distance travelled so far
	int distanceGoal; 		// Total distance needed to generate
	int totalTerrains;		// Amount of terrains generated
	int minimunTerrainLength;
	int maximunTerrainLength;
	int minStretchLength; 
 	int maxStretchLength;
 	int generationWaitTime;	// Time lapse ins seconds between each generation iteration
	std::vector<TerrainPrototype*> terrains;

	// Concurrency
	bool producing;
	std::thread producer;
	SyncQueue * queue;

public:
	TerrainGenerator(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue);
	void readTerrainData(const rapidjson::Document& pConfig);
	void generateStretch();
	void generate();
	
	void start();
	void join();
	void pushToQueue(rapidjson::Document* value);
	
	void showTerrains();
	void showQueue();

	std::vector<Terrain*> getStretch();	// not implemented yet
	
};
