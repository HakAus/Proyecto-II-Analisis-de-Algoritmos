#include "simulation.h"

Simulation::Simulation()
{
	this->reader = new Reader();
	reader->read("config.json");
	this->terrainGenerator = new TerrainGenerator(reader->getDocument());
	this->geneticAlgorithm = new GeneticAlgorithm();
}

void Simulation::start()
{

}

void Simulation::showSolution()
{
	
}