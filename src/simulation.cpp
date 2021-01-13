#include "simulation.h"

Simulation::Simulation()
{
	this->reader = new Reader();
	reader->read("config.json");
	this->terrainGenerator = new TerrainGenerator(reader->getDocument());
	this->terrainGenerator->setSharedQueue(&this->sharedQueue, &this->mutex, &this->condition);
	this->geneticAlgorithm = new GeneticAlgorithm();
	this->geneticAlgorithm->setSharedQueue(&this->sharedQueue, &this->mutex, &this->condition);

}

void Simulation::start(int pDistance)
{
	terrainGenerator->start(pDistance);
	geneticAlgorithm->start(pDistance);
}

void Simulation::join()
{
	terrainGenerator->join();
	geneticAlgorithm->join();
}

void Simulation::showSolution()
{
	
}
