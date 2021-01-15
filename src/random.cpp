#include <stdlib.h>
#include <time.h>

class Random {
public:
	static float RandomRange(int minValue, int maxValue) {
		return minValue + rand()%(maxValue - minValue);
	}
	static unsigned short RandomChromosome() {
		return (unsigned short) rand() % 65536;
	}

};