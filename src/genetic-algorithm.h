#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

#include "../libs/rapidjson/document.h"
#include "vehicle.h"
#include "terrain.h"
#include "specification.h"
#include "sync-queue.h"

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

	unsigned short leftMask = 0b1111111100000000;
	unsigned short rightMask = 0b0000000011111111;

	// Concurrency
	std::thread consumer;
	SyncQueue * queue;

public:
	GeneticAlgorithm(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue);
	GeneticAlgorithm();
	void startPopulation();
	void calculateFitness();
	std::queue<Vehicle*> selectFittestParents();
	void crossover(Vehicle * pParent1, Vehicle *pParent2, Vehicle** pTwins);
	void generateMask(int pBytePos);
	void mutate(Vehicle* pChild);
	void tryMutation(Vehicle* pChild);
	void evolve();
	void startEvolution();
	bool checkConvergence();
	Vehicle * getResult();
	void start();
	void join();
	void getData();
	void setSpecifications(const rapidjson::Document& pConfig);
	void loadSpecificationTable(const rapidjson::Document& pConfig, std::unordered_map<int,Specification*> pHashTable, 
								const char* pTableName);
};