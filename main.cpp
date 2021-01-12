#include "simulation.h"

int main()
{
	Simulation simulation;
	simulation.start(100);
	simulation.join();
	return 0;
}