#include "simulation.h"

Simulation::Simulation()
{
	this->queue = new SyncQueue(100);
	this->reader = new Reader();
	reader->read("../input/config.json");
	this->terrainGenerator = new TerrainGenerator(reader->getDocument(), this->queue);
	this->geneticAlgorithm = new GeneticAlgorithm(reader->getDocument(), this->queue);
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
