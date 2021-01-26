#include "genetic-algorithm.h"
#include "random.cpp"
#include <typeinfo>

// PARA IMPRIMIR

// rapidjson::StringBuffer sb;
// rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
// stretch->Accept(writer);
// auto str = sb.GetString();
// printf("%s\n", str);

GeneticAlgorithm::GeneticAlgorithm(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->fittestPopulationPercentage = 0.40;
	this->convergencePercentage = 0;
	this->mutationPercentage = 0;
	this->populationAmount = 200;
	this->distanceProcessed = 0;
	this->totalDistance = 200;
	this->totalEnergy = 0;
	this->queue = pSharedQueue;	
	this->numberOfExtractions = populationAmount * fittestPopulationPercentage;
	this->setSpecifications(pConfig);
}

// DEBUG
GeneticAlgorithm::GeneticAlgorithm()
{
	this->convergencePercentage = 0;
	this->mutationPercentage = 80;
	this->populationAmount = 0;
	this->distanceProcessed = 0;
	this->totalDistance = 0;
	this->totalEnergy = 0;
	generateMask(8);
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	while (!this->population.empty())
	{
		this->population.pop_back();
	}
}

void GeneticAlgorithm::loadSpecificationTable(const rapidjson::Document &pConfig, 
											  std::unordered_map<int,Specification*> &pHashTable, 
											  const char* pTableName)
{
	for (auto& atribute : pConfig[pTableName].GetArray())
	{
		int id = atribute.GetObject()["id"].GetInt();
		int energy = atribute.GetObject()["energy"].GetInt();
		std::vector<int> ranges;
		for (auto& magnitude : atribute.GetObject()["ranges"].GetObject())
		{
			ranges.push_back(magnitude.value.GetArray()[0].GetInt());
			ranges.push_back(magnitude.value.GetArray()[1].GetInt());
		}
		pHashTable[id] = new Specification(id,ranges,energy);
		demo[id] = new Specification(rand() % 10, ranges, rand() % 10);
	}
}

void GeneticAlgorithm::setSpecifications(const rapidjson::Document& pConfig)
{
	loadSpecificationTable(pConfig, this->torqueTable, "torqueTable");
	loadSpecificationTable(pConfig, this->treadTable, "treadTable");
}

void GeneticAlgorithm::getStretch()
{
	if (this->distanceProcessed < this->totalDistance)
	{
		rapidjson::Document* stretch = this->queue->pop();
		int distance = 0;

		for (const auto& terrainJSONObject : stretch->GetArray())
		{
			Terrain* terrain = new Terrain(terrainJSONObject);
			this->currentStretch.push_back(terrain);
			distance += (terrain->getEndKm() - terrain->getStartKm());
		}
		this->distanceProcessed += distance;
	}
}

void GeneticAlgorithm::setCurrentTerrain()
{
	if (!this->currentStretch.empty())
	{
		this->currentTerrain = this->currentStretch[0];
		this->currentStretch.erase(this->currentStretch.begin());
	}
}

void GeneticAlgorithm::startPopulation()
{
	for (int amount = 0; amount < this->populationAmount; amount++)
	{
		this->population.push_back(new Vehicle(Random::RandomChromosome()));
	}
}

void GeneticAlgorithm::calculateFitness()
{
	for (Vehicle* vehicle : this->population)
	{
		std::cout << "Vehicle chromosome: " << vehicle->getChromosome() << std::endl;

		Specification* torque = this->torqueTable[vehicle->getTorqueId()];
		Specification* tread = this->treadTable[vehicle->getTreadId()];

		std::vector<float> terrainAttributes = this->currentTerrain->getAttributes();
		std::vector<int> torqueAttributes;
		torque->getClosestAttributesTo(terrainAttributes, torqueAttributes);
		std::vector<int> treadAttributes;
		tread->getClosestAttributesTo(terrainAttributes, treadAttributes);

		double terrainNorm = sqrt(
							 pow(terrainAttributes[0], 2) + 
							 pow(terrainAttributes[1], 2) + 
						 	 pow(terrainAttributes[2], 2)
							 );
		double torqueNorm = sqrt(
							pow(torqueAttributes[0], 2) + 
							pow(torqueAttributes[1], 2) + 
							pow(torqueAttributes[2], 2)
							);
		double treadNorm = sqrt(
						   pow(treadAttributes[0], 2) + 
						   pow(treadAttributes[1], 2) + 
						   pow(treadAttributes[2], 2)
						   );

		double fitnessScore = 0.0;
		double torqueSimilarity = 0.0;
		double treadSimilarity = 0.0;

		for (int fitnessIndex = 0; fitnessIndex < 3; fitnessIndex++)
		{
			torqueSimilarity += (torqueAttributes[fitnessIndex] * terrainAttributes[fitnessIndex]) / (terrainNorm * torqueNorm);
			treadSimilarity +=  (treadAttributes[fitnessIndex] * terrainAttributes[fitnessIndex]) / (terrainNorm * treadNorm);
		}

		fitnessScore = (1/torqueSimilarity)*torque->getEnergy() + (1/treadSimilarity)*tread->getEnergy();
		vehicle->setFitnessScore(fitnessScore);
		std::cout << "Fitness: " << fitnessScore << std::endl;
	}
	//Pop a poblacion cuando fitness hacia priority

}

bool GeneticAlgorithm::checkConvergence()
{
	return false; // dummy
}

void GeneticAlgorithm::startEvolution()
{
	this->getStretch();
	this->setCurrentTerrain();
 	this->startPopulation();
 	this->calculateFitness();
 	// while (!this->checkConvergence())
 	 	//this->evolve();
	
}

std::queue<Vehicle*> GeneticAlgorithm::selectFittestParents()
{
	std::queue<Vehicle*> fittest;
	for (int i = 0; i < numberOfExtractions; i++)
	{
		fittest.push(rankedPopulation.top());
		rankedPopulation.pop();
	}
	return fittest;
}

 void showbits(std::string text, unsigned short x )
 {
     int i=0;
	 std::cout<<text;
     for (i = (sizeof(short) * 8) - 1; i >= 0; i--)
     {
        putchar(x & (1u << i) ? '1' : '0');
     }
     printf("\n");
 }

 void printTest(std::string text, int value)
 {
	 std::cout << text << value <<std::endl;
 }

void GeneticAlgorithm::crossover(Vehicle * pParent1, Vehicle * pParent2, Vehicle ** pTwins)
{
	Vehicle* parents[2] = { pParent1,pParent2 };
	int parentIndexControll = 1;
	unsigned short childGenotype = 0;
	unsigned short temp = 0;
	showbits("P1 Genotype:",pParent1->getChromosome());
	showbits("P2 Genotype:",pParent2->getChromosome());
	for (int i = 0; i < 2; i++)
	{
		temp = parents[i]->getChromosome() & leftMask;
		showbits("Temp", temp);
		childGenotype = childGenotype | temp;
		temp = 0;

		temp = parents[i+parentIndexControll]->getChromosome() & rightMask;
		showbits("Temp", temp);
		childGenotype = childGenotype | temp;
		temp = 0;

		pTwins[i] = new Vehicle(childGenotype);
		parentIndexControll = -parentIndexControll;
		showbits("Child:", childGenotype);
		childGenotype = 0;
	}

}

void GeneticAlgorithm::generateMask(int pBytePos)
{
	int rightMaskShift = 16 - pBytePos;
	unsigned short base = 0b1111111111111111;
	leftMask = base << pBytePos;
	base = 0b1111111111111111;
	rightMask = base >> rightMaskShift;
	showbits("LM", leftMask);
	showbits("RM", rightMask);
}

void GeneticAlgorithm::mutate(Vehicle * pChild)
{
	unsigned short pos = Random::RandomRange(0, 16);
 	unsigned short mask = 1 << pos; 
	unsigned short childChromosome = pChild->getChromosome();
 	bool has_bit = childChromosome & mask;
	unsigned short newChromosome = (childChromosome & ~mask) | ((!has_bit << pos) & mask);
	printTest("Pos:", pos);
	showbits("OldChromosome", childChromosome);
	showbits("NewChromosome", newChromosome);
	pChild->setChromosome(newChromosome);
}

void GeneticAlgorithm::tryMutation(Vehicle * pChild)
{
	int mutationSucces = Random::RandomRange(0, 100);
	std::cout << mutationSucces << std::endl;
	if (mutationSucces < mutationPercentage) mutate(pChild);
	else printTest("No mutation", 0);
}

void GeneticAlgorithm::setNewGeneration()
{
	int populationFill = populationAmount - population.size(); //Verificar que queden 200 de poblacion
	for (int i = 0; i < populationAmount/*populationFill*/;i++)
	{
		if(i < populationFill)//Se puede quitar el if y el while haria psize - pfill
			population.push_back(rankedPopulation.top());
		rankedPopulation.pop();
	}

	/*while (!rankedPopulation.empty())
	{
		rankedPopulation.pop();
	}*/

	//rankedPopulation = priority_queue <Vehicle*>(); //Evitar el while, pero no se si afecta la memoria.
}

void GeneticAlgorithm::evolve()
{
	this->calculateFitness();
	std::queue<Vehicle*> fittest = this->selectFittestParents();
	std::vector<Vehicle*> children;
	while (!fittest.empty())
	{
		Vehicle * mom = fittest.front();
		fittest.pop();
		Vehicle * dad = fittest.front();
		fittest.pop();
		Vehicle* twins[2];
		crossover(mom,dad,twins);
		for (int twinIndex = 0; twinIndex < 2; twinIndex++)
		{
			tryMutation(twins[twinIndex]);
			population.push_back(twins[twinIndex]);
		}
		population.push_back(mom);
		population.push_back(dad);
	}
	setNewGeneration();
}

void GeneticAlgorithm::start()
{
	this->consumer = std::thread(&GeneticAlgorithm::startEvolution, this);
}

void GeneticAlgorithm::join()
{
	this->consumer.join();
}
