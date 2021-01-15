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
	int currentDistance;
	int distanceGoal; //Valor de prueba
	int totalTerrains;
	int minimunTerrainLength;
	int maximunTerrainLength;
	int minStretchLength; //Largo del tramo
 	int maxStretchLength;
 	int maxQueueSize;
 	int generationWaitTime;
	std::vector<TerrainPrototype*> terrains;

	// Concurrency
	bool producing;
	std::thread producer;
	SyncQueue * queue;

public:
	TerrainGenerator(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue);
	void generate();
	std::vector<Terrain*> getStretch();
	void start();
	void join();

	void pushToQueue(rapidjson::Document* value);

	void generateStretch();
	void showTerrains();
	void increasePercentage(float* percentage);
	void showQueue();
	void readTerrainData(const rapidjson::Document& pConfig);
};
