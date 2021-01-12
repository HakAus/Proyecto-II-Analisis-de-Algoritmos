#include <queue>
#include <vector>
#include <unordered_map>

#include "vehicle.h"
#include "terrain.h"
#include "specification.h"

class GeneticAlgorithm
{
private:
	std::queue<std::vector<Terrain*>>* sharedBuffer;
	std::vector<Vehicle*> population;
	std::unordered_map<int, Specification*> treadTable;
	std::unordered_map<int, Specification*> torqueTable;
	std::priority_queue<Vehicle*> rankedPopulation;
	std::vector<Terrain*> currentStretch;
	Terrain * currentTerrain;
	int convergencePercentage;
	int mutationPercentage;
	int populationAmount;

public:
	GeneticAlgorithm();
	void startPopulation();
	void calculateFitness();
	std::vector<Vehicle*> selectFittestParents();
	Vehicle** crossover(Vehicle * pParent1, Vehicle *pParent2);
	Vehicle* mutate(Vehicle* pVehicle);
	void evolve();
	bool checkConvergence();
	Vehicle * getResult();

};