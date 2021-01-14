#include "simulation.h"
#include <chrono>

int main()
{
	srand((unsigned int)time(NULL));
	Simulation * simulation = new Simulation();
	simulation->start();
	simulation->join();
	return 0;
}
