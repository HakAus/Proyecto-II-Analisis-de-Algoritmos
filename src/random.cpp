#include <stdlib.h>
#include <time.h>

class Random {
public:
	static float RandomRange(int minValue, int maxValue) {
		return minValue + rand()%(maxValue - minValue);
	}

};