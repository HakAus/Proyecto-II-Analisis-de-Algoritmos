#include "genetic-algorithm.h"
#include "random.cpp"
#include <typeinfo>

// PARA IMPRIMIR

// rapidjson::StringBuffer sb;
// rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
// stretch->Accept(writer);
// auto str = sb.GetString();
// printf("%s\n", str);

// while (!this->population.empty())
// {
// 	Wheel* wheel = this->population.front();
// 	this->rankedPopulation.push(wheel);
// 	this->population.pop();
// }

// while (!rankedPopulation.empty())
// {
// 	std::cout << "[Torque: " << rankedPopulation.top()->getTorqueId() <<
// 		", Tread: " << rankedPopulation.top()->getTreadId() <<
// 		", Fitness Score: " << rankedPopulation.top()->getFitnessScore()
// 		<< "]" << std::endl;
// 	rankedPopulation.pop();
// }

GeneticAlgorithm::GeneticAlgorithm(Vehicle* pVehicle, const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->vehicle = pVehicle;
	this->fittestPopulationPercentage = 0.20;
	this->sensorWaitTime = 4;
	this->convergencePercentage = 0.95;
	this->mutationPercentage = 0;
	this->populationAmount = 400;
	this->distanceProcessed = 0;
	this->totalDistance = pConfig["simulationConfig"].GetObject()["distanceGoal"].GetInt();
	this->totalEnergy = 0;
	this->queue = pSharedQueue;	
	this->numberOfExtractions = populationAmount * fittestPopulationPercentage;
	this->generation = 0;
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
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	while (!this->population.empty())
	{
		this->population.pop();
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
		std::cout << "A new stretch was popped from the queue!" << std::endl;
	}
}

void GeneticAlgorithm::setCurrentTerrain()
{
	if (!this->currentStretch.empty())
	{
		this->currentTerrain = this->currentStretch[0];
		this->currentStretch.erase(this->currentStretch.begin());
		this->currentTerrain->print();
	}
}

void GeneticAlgorithm::startPopulation()
{
	this->generation = 0;
	for (int amount = 0; amount < this->populationAmount; amount++)
	{
		this->population.push(new Wheel(Random::RandomChromosome()));
	}
}

void GeneticAlgorithm::setIndividualFitness(Wheel* pWheel)
{
	Specification* torque = this->torqueTable[pWheel->getTorqueId()];
	Specification* tread = this->treadTable[pWheel->getTreadId()];

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
		torqueSimilarity += (torqueAttributes[fitnessIndex] * terrainAttributes[fitnessIndex]);
		treadSimilarity +=  (treadAttributes[fitnessIndex] * terrainAttributes[fitnessIndex]);
	}

	torqueSimilarity = torqueSimilarity / (terrainNorm * torqueNorm);
	treadSimilarity = treadSimilarity / (terrainNorm * treadNorm);

	// fitnessScore = (1/torqueSimilarity)*(torque->getEnergy()-torque->getEnergy()*torqueSimilarity) + 
	// 			   (1/treadSimilarity)*(tread->getEnergy()-tread->getEnergy()*treadSimilarity);
   	fitnessScore = (1/torqueSimilarity)*torque->getEnergy() + (1/treadSimilarity)*tread->getEnergy();
	pWheel->setFitnessScore(fitnessScore);
	pWheel->setEnergeticCost(torque->getEnergy() + tread->getEnergy());
}

void GeneticAlgorithm::setPopulationFitness()
{
	int wheelsInserted = 0;
	while (wheelsInserted < this->populationAmount)
	{
		Wheel * wheel = this->population.front();
		setIndividualFitness(wheel);
		this->rankedPopulation.push(wheel);
		this->population.pop();
		wheelsInserted++;
	}
}

bool GeneticAlgorithm::checkConvergence()
{
	int convergencePoint = this->populationAmount * this->convergencePercentage;
	for (auto & frequency : this->frequencyTable)
	{
		if (frequency.second >= convergencePoint)
		{
			addWheelToVehicle(frequency.first);
			resetContainers();
			return true;
		}
		frequency.second = 0;
	}
	return false;
}

void GeneticAlgorithm::addWheelToVehicle(int pWheelConfiguration)
{
	int torqueId = pWheelConfiguration % 10;
	int treadId = pWheelConfiguration / 10;
	Wheel * wheel = new Wheel(torqueId, treadId);
	setIndividualFitness(wheel);
	this->vehicle->addWheelForTerrain(this->currentTerrain, wheel);
}

void GeneticAlgorithm::resetContainers()
{
	// frequency table and ranked population must be cleared for next genetic cycle

	for (auto& frequency : this->frequencyTable)
	{
		frequency.second = 0;
	}

	while (!this->rankedPopulation.empty())
	{
		rankedPopulation.pop();
	}
}

void GeneticAlgorithm::startEvolution()
{
	srand(time(0));
	do
	{
		getStretch();
		while (!currentStretch.empty() && vehicle->hasEnergy()) 
		{
			setCurrentTerrain();
			startPopulation();
			setPopulationFitness();
			do
			{				
				evolve();
			} 
			while (!checkConvergence());

			std::this_thread::sleep_for(std::chrono::seconds(sensorWaitTime));
		}
	} 
	while (!queue->empty() && vehicle->hasEnergy());

	if (vehicle->hasEnergy())
		std::cout << "You have arrived succesfully at destination!!" << std::endl;
	else
		std::cout << "Bummer ... The battery ran out before reaching your destination" << std::endl;

	vehicle->printRouteConfiguration();
}

std::queue<Wheel*> GeneticAlgorithm::selectFittestParents()
{
	std::queue<Wheel*> fittest;
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

void GeneticAlgorithm::crossover(Wheel* pParent1, Wheel* pParent2, Wheel** pTwins)
{
	generateMasks(Random::RandomRange(4,12));
	Wheel* parents[2] = { pParent1,pParent2 };
	int parentIndexControll = 1;
	unsigned short childGenotype = 0;
	unsigned short temp = 0;
	// showbits("P1 Genotype:",pParent1->getChromosome());
	// showbits("P2 Genotype:",pParent2->getChromosome());
	for (int i = 0; i < 2; i++)
	{
		temp = parents[i]->getChromosome() & leftMask;
		// showbits("Temp", temp);
		childGenotype = childGenotype | temp;
		temp = 0;

		temp = parents[i+parentIndexControll]->getChromosome() & rightMask;
		// showbits("Temp", temp);
		childGenotype = childGenotype | temp;
		temp = 0;

		pTwins[i] = new Wheel(childGenotype);
		parentIndexControll = -parentIndexControll;
		// showbits("Child:", childGenotype);
		childGenotype = 0;
	}
}

void GeneticAlgorithm::generateMasks(int pBytePos)
{
	int rightMaskShift = 16 - pBytePos;
	unsigned short base = 0b1111111111111111;
	leftMask = base << pBytePos;
	base = 0b1111111111111111;
	rightMask = base >> rightMaskShift;
	// showbits("LM", leftMask);
	// showbits("RM", rightMask);
}

void GeneticAlgorithm::mutate(Wheel * pChild)
{
	unsigned short pos = Random::RandomRange(0, 16);
 	unsigned short mask = 1 << pos; 
	unsigned short childChromosome = pChild->getChromosome();
 	bool has_bit = childChromosome & mask;
	unsigned short newChromosome = (childChromosome & ~mask) | ((!has_bit << pos) & mask);
	pChild->setChromosome(newChromosome);
}

void GeneticAlgorithm::tryMutation(Wheel * pChild)
{
	int mutationSucces = Random::RandomRange(0, 100);
	if (mutationSucces < mutationPercentage) 
		mutate(pChild);
}

void GeneticAlgorithm::setNewGeneration()
{
	int remainingPopulation = populationAmount - population.size();
	for (int wheelCount = 0; wheelCount < remainingPopulation; wheelCount++)
	{
		pushToPopulation(rankedPopulation.top());
		rankedPopulation.pop();
	}

	while (!rankedPopulation.empty())
	{
		rankedPopulation.pop();
	}

	while (!this->population.empty())
	{
		Wheel * wheel = this->population.front();
		this->rankedPopulation.push(wheel);
		this->population.pop();
	}
	//rankedPopulation = priority_queue <Vehicle*>(); //Evitar el while, pero no se si afecta la memoria.
}

void GeneticAlgorithm::pushToPopulation(Wheel * pWheel)
{
	int configurationId = pWheel->getTreadId()*10 + pWheel->getTorqueId();
	this->frequencyTable[configurationId] += 1; 
	this->population.push(pWheel);
}

void GeneticAlgorithm::evolve()
{
	std::cout << "Generation: " << this->generation << std::endl;
	std::queue<Wheel*> fittest = this->selectFittestParents();
	std::vector<Wheel*> children;
	while (!fittest.empty())
	{
		Wheel * mom = fittest.front();
		fittest.pop();
		Wheel * dad = fittest.front();
		fittest.pop();
		Wheel* twins[2];
		crossover(mom,dad,twins);
		for (int twinIndex = 0; twinIndex < 2; twinIndex++)
		{
			tryMutation(twins[twinIndex]);
			setIndividualFitness(twins[twinIndex]);
			pushToPopulation(twins[twinIndex]);
		}
		pushToPopulation(mom);
		pushToPopulation(dad);
	}
	setNewGeneration();
	this->generation++;
}

void GeneticAlgorithm::start()
{
	this->consumer = std::thread(&GeneticAlgorithm::startEvolution, this);
}

void GeneticAlgorithm::join()
{
	this->consumer.join();
}
