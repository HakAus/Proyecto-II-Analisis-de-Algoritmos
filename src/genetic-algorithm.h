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
#include "wheel.h"
#include "vehicle.h"
#include "terrain.h"
#include "specification.h"
#include "sync-queue.h"
#include "LogManager.cpp"

class GeneticAlgorithm
{
private:
	// Algorithm
	LogManager * logManager;
	std::queue<Wheel*> population;
	std::unordered_map<int, Specification*> treadTable;
	std::unordered_map<int, Specification*> torqueTable;
	std::unordered_map<int, int> frequencyTable;
	std::priority_queue<Wheel*, std::vector<Wheel*>, WheelComparator>  rankedPopulation;
	std::vector<Terrain*> currentStretch;
	Terrain * currentTerrain;
	Vehicle * vehicle;
	float convergencePercentage;
	float fittestPopulationPercentage; //Funciona para el numero de extracciones
	int mutationPercentage;
	int populationAmount;
	int distanceProcessed;
	int totalDistance;
	int totalEnergy;
	int numberOfExtractions;
	int sensorWaitTime;
	int generation;
	int answer;

	unsigned short leftMask = 0b1111111100000000;	// Torque
	unsigned short rightMask = 0b0000000011111111;	// Tread

	// Concurrency
	std::thread consumer;
	SyncQueue * queue;

public:
	GeneticAlgorithm(Vehicle * pVehicle, const rapidjson::Document& pConfig, SyncQueue* pSharedQueue);
	GeneticAlgorithm();	// DEBUG
	~GeneticAlgorithm();
	void setSpecifications(const rapidjson::Document& pConfig);
	void loadSpecificationTable(const rapidjson::Document& pConfig, 
								std::unordered_map<int,Specification*>& pHashTable, 
								const char* pTableName
								);
	void startPopulation();
	void setCurrentTerrain();
	void setIndividualFitness(Wheel* pWheel);
	void setPopulationFitness();
	std::queue<Wheel*> selectFittestParents();
	void crossover(Wheel* pParent1, Wheel* pParent2, Wheel** pTwins);
	void generateMasks(int pBytePos);
	void mutate(Wheel* pChild);
	void tryMutation(Wheel* pChild);
	void evolve();
	void pushToPopulation(Wheel* pWheel);
	void startEvolution();
	bool checkConvergence();
	void addConfigurationToVehicle();
	void setNewGeneration();
	Wheel * getResult();
	void start();
	void join();
	void getStretch();
};