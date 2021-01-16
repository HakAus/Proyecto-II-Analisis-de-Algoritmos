#include "genetic-algorithm.h"
#include "random.cpp"

// PARA IMPRIMIR

// rapidjson::StringBuffer sb;
//    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
//    stretch->Accept(writer);
//    auto str = sb.GetString();
//    printf("%s\n", str);

GeneticAlgorithm::GeneticAlgorithm(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->convergencePercentage = 0;
	this->mutationPercentage = 0;
	this->populationAmount = 200;
	this->distanceProcessed = 0;
	this->totalDistance = 200;
	this->totalEnergy = 0;
	this->queue = pSharedQueue;	
	//generateMask(11);
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

void GeneticAlgorithm::loadSpecificationTable(const rapidjson::Document& pConfig, std::unordered_map<int,Specification*> pHashTable, 
											  const char* pTableName)
{
	for (auto const& torqueItr : pConfig[pTableName].GetArray())
	{
		int id =  torqueItr.GetObject()["id"].GetInt();
		int energy = torqueItr.GetObject()["energy"].GetInt();
		int ranges[6];
		int counter = 0;

		for (auto & attribute : torqueItr.GetObject()["ranges"].GetObject())
		{
			ranges[counter] = attribute.value.GetArray()[0].GetInt();
			ranges[counter+1] = attribute.value.GetArray()[1].GetInt();
			counter += 2;
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
	while (this->distanceProcessed < this->totalDistance)
	{
		rapidjson::Document* stretch = this->queue->pop();

		int distance = 0;

		for (const auto& terrainJSONObject : stretch->GetArray())
		{
			Terrain* terrain = new Terrain(terrainJSONObject);
			this->currentStretch.push_back(terrain);
			distance += (terrain->getEndKm() - terrain->getStartKm());
			std::cout << terrain->getEndKm() << std::endl;
			std::cout << terrain->getStartKm() << std::endl;
			for (int i = 0; i < 3; i++)
				std::cout << terrain->getAttributes()[i] << std::endl;
		}
		this->distanceProcessed += distance;
		std::cout << "Distance processed (Genetic) : " << this->distanceProcessed << std::endl;
	}
}

void GeneticAlgorithm::startPopulation()
{
	for (int amount = 0; amount < this->populationAmount; amount++)
	{
		this->population.push_back(new Vehicle(Random::RandomChromosome()));
	}
}


bool GeneticAlgorithm::checkConvergence()
{
	return false; // dummy
}

void GeneticAlgorithm::startEvolution()
{
	this->getStretch();
 	this->startPopulation();
 	
 	while (!this->checkConvergence())
 		this->evolve();
	
}

void GeneticAlgorithm::calculateFitness()
{
	//Pop a poblacion cuando fitness hacia priority
}

std::queue<Vehicle*> GeneticAlgorithm::selectFittestParents()
{
	std::queue<Vehicle*> dummy;
	return dummy;
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
		population.push_back(parents[i]);
		population.push_back(pTwins[i]);
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
	if (mutationSucces < mutationPercentage)mutate(pChild);
	else printTest("No mutation", 0);
}

void GeneticAlgorithm::setNewGeneration()
{
	int currentPopulation = population.size();
	int requieredPopulation = populationAmount - currentPopulation;
	for (int i = 0; i < requieredPopulation; i++)
	{
		population.push_back(rankedPopulation.top());
		rankedPopulation.pop();
	}
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
		}
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
