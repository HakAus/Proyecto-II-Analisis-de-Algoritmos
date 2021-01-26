#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <cmath>

#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/prettywriter.h"
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
	std::unordered_map<int, int> frequencyTable;
	std::priority_queue<Vehicle*, std::vector<Vehicle*>, VehicleComparator>  rankedPopulation;
	std::vector<Terrain*> currentStretch;
	Terrain * currentTerrain;
	float convergencePercentage;
	float fittestPopulationPercentage; //Funciona para el numero de extracciones
	int mutationPercentage;
	int populationAmount;
	int distanceProcessed;
	int totalDistance;
	int totalEnergy;
	int numberOfExtractions;

	unsigned short leftMask = 0b1111111100000000;	// Torque
	unsigned short rightMask = 0b0000000011111111;	// Tread

	// Concurrency
	std::thread consumer;
	SyncQueue * queue;

public:
	GeneticAlgorithm(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue);
	GeneticAlgorithm();	// DEBUG
	~GeneticAlgorithm();
	void setSpecifications(const rapidjson::Document& pConfig);
	void loadSpecificationTable(const rapidjson::Document& pConfig, 
								std::unordered_map<int,Specification*>& pHashTable, 
								const char* pTableName
								);
	void startPopulation();
	void setCurrentTerrain();
	void setVehicleFitness(Vehicle* pVehicle);
	void setPopulationFitness();
	std::queue<Vehicle*> selectFittestParents();
	void crossover(Vehicle * pParent1, Vehicle *pParent2, Vehicle** pTwins);
	void generateMask(int pBytePos);
	void mutate(Vehicle* pChild);
	void tryMutation(Vehicle* pChild);
	void evolve();
	void pushToPopulation(Vehicle * pVehicle);
	void startEvolution();
	bool checkConvergence();
	void setNewGeneration();
	Vehicle * getResult();
	void start();
	void join();
	void getStretch();
};