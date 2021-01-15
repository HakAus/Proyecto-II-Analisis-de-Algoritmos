#include "simulation.h"

Simulation::Simulation()
{
	this->reader = new Reader();
	reader->read("../input/config.json");
	this->queue = new SyncQueue(reader->getDocument());	
	this->terrainGenerator = new TerrainGenerator(reader->getDocument(), this->queue);
	this->geneticAlgorithm = new GeneticAlgorithm(reader->getDocument(), this->queue);
}

void Simulation::start()
{
	std::cout << "start" << std::endl;
	terrainGenerator->start();
	// geneticAlgorithm->start();
}

void Simulation::join()
{
	terrainGenerator->join();
	// geneticAlgorithm->join();
}

void Simulation::showSolution()
{
	
}
