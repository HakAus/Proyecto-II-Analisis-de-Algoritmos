#include "simulation.h"

Simulation::Simulation()
{
	this->reader = new Reader();
	reader->read("../input/config.json");
	this->queue = new SyncQueue(reader->getDocument());	
	this->vehicle = new Vehicle();
	this->terrainGenerator = new TerrainGenerator(this->reader, reader->getDocument(), this->queue);
	this->geneticAlgorithm = new GeneticAlgorithm(this->vehicle, reader->getDocument(), this->queue);
}

void Simulation::start()
{
	std::cout << "Simulation started" << std::endl;
	terrainGenerator->start();
	geneticAlgorithm->start();
}

void Simulation::join()
{
	terrainGenerator->join();
	geneticAlgorithm->join();
	std::cout << "Simulation finished" << std::endl;
}

void Simulation::showSolution()
{
	
}
