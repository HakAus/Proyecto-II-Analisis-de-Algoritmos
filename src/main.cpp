//#include "simulation.h"
#include <chrono>
#include "genetic-algorithm.h"

int main()
{
	srand((unsigned int)time(NULL));

	/*Simulation * simulation = new Simulation();
	simulation->start();
	simulation->join();
	return 0;*/

	//GA test
	GeneticAlgorithm* ga = new GeneticAlgorithm();
	Vehicle* testV1 = new Vehicle(0b1010001101100111);
	Vehicle* testV2 = new Vehicle(0b0101110110101011);
	Vehicle* testChilds[2];
	ga->crossover(testV1, testV2, testChilds);
	std::cout << std::endl;
	ga->tryMutation(testChilds[0]);


}
