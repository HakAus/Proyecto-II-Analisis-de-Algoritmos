#include "simulation.h"

Simulation::Simulation()
{
	this->reader = new Reader();
	reader->read("config.json");
	this->terrainGenerator = new TerrainGenerator(reader->getDocument(),this->queue);
	this->geneticAlgorithm = new GeneticAlgorithm(reader->getDocument(),this->queue);
}

void Simulation::start()
{

}

void Simulation::showSolution()
{
	
}