#include "genetic-algorithm.h"
#include "random.cpp"
#include <typeinfo>


// PARA IMPRIMIR

// rapidjson::StringBuffer sb;
// rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
// stretch->Accept(writer);
// auto str = sb.GetString();
// printf("%s\n", str);

GeneticAlgorithm::GeneticAlgorithm(Vehicle* pVehicle, const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	logManager = new LogManager();
	this->vehicle = pVehicle;
	this->fittestPopulationPercentage = 0.40;
	this->sensorWaitTime = 4;
	this->convergencePercentage = 0.85;
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
	}
	std::cout << "New stretch fetched!" << std::endl;
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
	logManager->addLine("INITIAL POPULATION GENOTYPES");
	for (int amount = 0; amount < this->populationAmount; amount++)
	{
		Wheel* wheel = new Wheel(Random::RandomChromosome());
		this->population.push(wheel);
		logManager->addLine("VEHICLE#", amount);
		logManager->addLine(wheel->getChromosome());
	}
}

void GeneticAlgorithm::setIndividualFitness(Wheel* pWheel)
{
	logManager->addLine("WHEEL ATRIBUTES");
	Specification* torque = this->torqueTable[pWheel->getTorqueId()];
	Specification* tread = this->treadTable[pWheel->getTreadId()];
	logManager->addLine(torque->toString());
	logManager->addLine(tread->toString());
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
	logManager->addLine("WHEEL FITNESSSCORE");
	logManager->addLine(fitnessScore);
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
	// int highestConvergence = 0;
	int convergencePoint = this->populationAmount * this->convergencePercentage;
	//std::cout << "Convergence Point: " << convergencePoint << std::endl;
	for (auto & frequency : this->frequencyTable)
	{
		// if (frequency.second > highestConvergence)
		// 	highestConvergence = frequency.second;
		if (frequency.second >= convergencePoint)
		{
			this->answer = frequency.first;
			this->generation = 0;
			return true;
		}
		frequency.second = 0;
	}
	// std::cout << "highestConvergence = " << highestConvergence << std::endl;
	return false;
}

void GeneticAlgorithm::addConfigurationToVehicle()
{
	std::cout << "Winning configuration: " << this->answer << std::endl;
	logManager->addLine("Winning configuration:", answer);
	int torqueId = this->answer%10;
	int treadId = this->answer/10;
	Wheel * wheel = new Wheel(torqueId, treadId);
	this->vehicle->addConfiguration(this->currentTerrain, wheel);
}

void GeneticAlgorithm::startEvolution()
{
	srand(time(0));
	do 
	{
		this->getStretch();
		while (!this->currentStretch.empty()) 
		{
			logManager->createLog("Stretch");
			this->setCurrentTerrain();
			logManager->addLine(currentTerrain->toString());
			this->startPopulation();
			this->setPopulationFitness();
			this->generation = 0;
			do
			{		
				this->evolve();
			} 
			while (!this->checkConvergence());

			for (auto& frequency : this->frequencyTable)
			{
				frequency.second = 0;
			}

			addConfigurationToVehicle();
			

			 while (!this->population.empty())
			 {
			 	Wheel* wheel = this->population.front();
			 	this->rankedPopulation.push(wheel);
			 	this->population.pop();
			 }

			 while (!rankedPopulation.empty())
			 {
			 	//std::cout << "[Torque: " << rankedPopulation.top()->getTorqueId() <<
			 	//	", Tread: " << rankedPopulation.top()->getTreadId() <<
			 	//	", Fitness Score: " << rankedPopulation.top()->getFitnessScore()
			 	//	<< "]" << std::endl;
			 	rankedPopulation.pop();
			 }
			 logManager->saveLog();
			std::this_thread::sleep_for(std::chrono::seconds(this->sensorWaitTime));
		}
	} 
	while (!this->queue->empty());
	//vehicle->printRouteConfiguration(); DESCOMENTAR
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
	// printTest("Pos:", pos);
	// showbits("OldChromosome", childChromosome);
	// showbits("NewChromosome", newChromosome);
	pChild->setChromosome(newChromosome);
}

void GeneticAlgorithm::tryMutation(Wheel * pChild)
{
	int mutationSucces = Random::RandomRange(0, 100);
	// std::cout << mutationSucces << std::endl;
	if (mutationSucces < mutationPercentage) 
		mutate(pChild);
	// else printTest("No mutation", 0);
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
		logManager->addLine(wheel->toString());
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
	logManager->addLine("NEW GENERATION");
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
