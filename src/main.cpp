#include "simulation.h"
#include <chrono>
#include <random>

int main()
{
	srand(time(0));
	Simulation * simulation = new Simulation();
	simulation->start();
	simulation->join();
	return 0;
}
