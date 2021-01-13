#include <queue>
#include <vector>
#include "libs/rapidjson/document.h"
#include "terrain.h"
#include "terrainPrototype.h"
#include <iostream>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "terrain.h"
// #include "sync-queue.h"

class TerrainGenerator 
{
private:
	const float minimunTerrainDistancePercentage = 0.10;
	const float maximunTerrainDistancePercentage = 0.20;
	std::vector<TerrainPrototype*> terrains;
	int distanceGenerated;
	int totalDistance;
	int maxQueueSize = 10;

	// Concurrency
	std::thread producer;
	std::mutex * mutex;
	std::condition_variable * condition;
	std::queue<int/*std::vector<Terrain*>*/>* sharedQueue;

public:
	TerrainGenerator(const rapidjson::Document& pConfig);
	void generate(int pDistance);
	std::vector<Terrain*> getStretch();
	void start(int pDistance);
	void join();
	void setSharedQueue(std::queue<int/*std::vector<Terrain*>*/>* pSharedQueue, std::mutex* pMutex, std::condition_variable* pCondition);

};
