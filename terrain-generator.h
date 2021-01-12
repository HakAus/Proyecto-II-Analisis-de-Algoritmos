#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "terrain.h"
// #include "sync-queue.h"

class TerrainGenerator 
{
private:
	// SyncQueue<int/*std::vector<Terrain*>*/> * queue;
	std::vector<Terrain*> terrains;
	int distanceGenerated;
	int totalDistance;
	int maxQueueSize = 10;

	// Concurrency
	std::thread producer;
	std::mutex * mutex;
	std::condition_variable * condition;
	std::queue<int/*std::vector<Terrain*>*/>* sharedQueue;

public:
	TerrainGenerator();
	void generate(int pDistance);
	std::vector<Terrain*> getStretch();
	void start(int pDistance);
	void join();
	void setSharedQueue(std::queue<int/*std::vector<Terrain*>*/>* pSharedQueue, std::mutex* pMutex, std::condition_variable* pCondition);

};