#include "genetic-algorithm.h"


GeneticAlgorithm::GeneticAlgorithm()
{
	this->convergencePercentage = 0;
	this->mutationPercentage = 0;
	this->populationAmount = 0;
	this->distanceProcessed = 0;
	this->totalDistance = 0;
	this->totalEnergy = 0;
}

void GeneticAlgorithm::startPopulation()
{
	while (this->distanceProcessed < this->totalDistance)
	{
		int result = this->queue->pop();
		this->distanceProcessed++;
	}
}

bool GeneticAlgorithm::checkConvergence()
{
	return false; // dummy
}

void GeneticAlgorithm::startEvolution()
{
	this->startPopulation();
	// while (!this->checkConvergence())
	// {
	// 	this->evolve();
	// }
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
		Vehicle** twins;
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
	this->consumer = std::thread(&GeneticAlgorithm::startEvolution, this);
	this->totalDistance = pDistance;
}

void GeneticAlgorithm::join()
{
	this->consumer.join();
}

void GeneticAlgorithm::setSharedQueue(SyncQueue* pSharedQueue)
{
	this->queue = pSharedQueue;
}
