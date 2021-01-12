#include "simulation.h"

Simulation::Simulation()
{
	this->reader = new Reader();
	this->terrainGenerator = new TerrainGenerator();
	this->geneticAlgorithm = new GeneticAlgorithm();
}

void Simulation::start()
{

}

void Simulation::showSolution()
{
	
}