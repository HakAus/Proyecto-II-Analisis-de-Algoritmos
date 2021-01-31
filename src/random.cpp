#include <stdlib.h>
#include <time.h>

class Random {
public:
	static float RandomRange(int minValue, int maxValue) {
		return minValue + rand()%(maxValue - minValue);
	}
	static unsigned int RandomChromosome() {
		return (unsigned int) rand() % 4294967296;
	}

};