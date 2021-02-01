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

class GeneticAlgorithm
{
private:
	// Algorithm
	std::queue<Wheel*> population;
	std::unordered_map<int, Specification*> treadTable;
	std::unordered_map<int, Specification*> torqueTable;
	std::unordered_map<int, int> frequencyTable;
	std::priority_queue<Wheel*, std::vector<Wheel*>, WheelComparator>  rankedPopulation;
	std::vector<Terrain*> currentStretch;
	Terrain * currentTerrain;
	Vehicle * vehicle;
	float fittestExtractionPercentage; //Funciona para el numero de extracciones
	int numberOfExtractions;
	int sensorWaitTime;
	float convergencePercentage;
	int mutationPercentage;
	int populationAmount;
	int distanceProcessed;
	bool specificationWiseFitness;
	int totalDistance;
	int generation;
	int answer;

	unsigned int leftMask = 0b11111111111111110000000000000000;	// Torque
	unsigned int rightMask =0b00000000000000001111111111111111; // Tread

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
	// std::vector<Wheel*> selectFittestParents();
	// Wheel* crossover(Wheel* pParent1, Wheel* pParent2);
	void crossover(Wheel* pParent1, Wheel* pParent2, Wheel** pTwins);
	void generateMasks(int pBytePos);
	void mutate(Wheel* pChild);
	void tryMutation(Wheel* pChild);
	void evolve();
	void pushToPopulation(Wheel* pWheel);
	void startEvolution();
	bool checkConvergence();
	void addWheelToVehicle(int pWheelConfiguration);
	void setNewGeneration();
	void resetContainers();
	Wheel * getResult();
	void start();
	void join();
	void getStretch();
};