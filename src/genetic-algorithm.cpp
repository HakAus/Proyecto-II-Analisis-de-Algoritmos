#include "genetic-algorithm.h"


GeneticAlgorithm::GeneticAlgorithm(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->convergencePercentage = 0;
	this->mutationPercentage = 0;
	this->populationAmount = 0;
	this->distanceProcessed = 0;
	this->totalDistance = 0;
	this->totalEnergy = 0;
	this->queue = pSharedQueue;
	
	this->setSpecifications(pConfig);
}

void GeneticAlgorithm::setSpecifications(const rapidjson::Document& pConfig)
{
// 	for (auto const& te : pConfig["terrains"].GetArray())
// 	{
// 		int energy;
// 		int ranges[6];
// 		int counter = 0;
// 		for (auto & itr : te.GetObject())
// 		{
// 			if (itr.value.IsInt())
// 				energy = itr.value.GetInt();
// 			else if (itr.value.IsArray())
// 			{
				
// 				ranges[counter] = itr.value.GetArray()[0].GetInt();
// 				ranges[counter+1] = itr.value.GetArray()[1].GetInt();
// 				counter += 2;
// 			}
// 		}
// 		terrains.push_back(new TerrainPrototype(name,ranges));
// 	}
// 	for (auto const& terru : terrains) 
// 	{
// 		std::cout << terru->toString();
// 	}	
// }

// void GeneticAlgorithm::getData()
// {

// 	while (this->distanceProcessed < this->totalDistance)
// 	{
// 		int result = this->queue->pop();
// 		this->distanceProcessed++;
// 	}
}

void GeneticAlgorithm::startPopulation()
{
	// for ()
	// 	new Vehicle(random);
}


bool GeneticAlgorithm::checkConvergence()
{
	return false; // dummy
}

void GeneticAlgorithm::startEvolution()
{
	/* 
 	this->startPopulation();
 	while (!this->checkConvergence())
 		this->evolve();
	*/
}

void GeneticAlgorithm::calculateFitness()
{

}

std::queue<Vehicle*> GeneticAlgorithm::selectFittestParents()
{
	std::queue<Vehicle*> dummy;
	return dummy;
}

void GeneticAlgorithm::crossover(Vehicle * pParent1, Vehicle * pParent2, Vehicle ** pTwins)
{
	for (int i = 0; i < 2; i++)
		pTwins[i] = new Vehicle(1); //dummy
}

void GeneticAlgorithm::mutate(Vehicle * pChild)
{

}

void GeneticAlgorithm::tryMutation(Vehicle * pChild)
{
	// if ...
	mutate(pChild);
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
			// put into population
		}
	}
}

void GeneticAlgorithm::start(int pDistance)
{
	this->totalDistance = pDistance;
	// this->consumer = std::thread(&GeneticAlgorithm::startEvolution, this);
}

void GeneticAlgorithm::join()
{
	this->consumer.join();
}
