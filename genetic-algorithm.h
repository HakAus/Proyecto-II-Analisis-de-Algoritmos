#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


#include "vehicle.h"
#include "terrain.h"
#include "specification.h"
// #include "sync-queue.h"

class GeneticAlgorithm
{
private:
	// Algorithm
	std::vector<Vehicle*> population;
	std::unordered_map<int, Specification*> treadTable;
	std::unordered_map<int, Specification*> torqueTable;
	std::priority_queue<Vehicle*> rankedPopulation;
	std::vector<Terrain*> currentStretch;
	Terrain * currentTerrain;
	int convergencePercentage;
	int mutationPercentage;
	int populationAmount;
	int distanceProcessed;
	int totalDistance;
	int totalEnergy;

	// Concurrency
	std::thread consumer;
	std::mutex * mutex;
	std::condition_variable * condition;
	std::queue<int/*std::vector<Terrain*>*/>* sharedQueue;
	// SyncQueue<int/*std::vector<Terrain*>*/> * queue;

public:
	GeneticAlgorithm();
	void startPopulation();
	void calculateFitness();
	std::queue<Vehicle*> selectFittestParents();
	void crossover(Vehicle * pParent1, Vehicle *pParent2, Vehicle** pTwins);
	void mutate(Vehicle* pChild);
	void tryMutation(Vehicle* pChild);
	void evolve();
	void startEvolution();
	bool checkConvergence();
	Vehicle * getResult();
	void start(int pDistance);
	void join();
	void setSharedQueue(std::queue<int/*std::vector<Terrain*>*/>* pSharedQueue, std::mutex* pMutex, std::condition_variable* pCondition);

};