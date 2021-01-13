#include <stdlib.h>
#include <time.h>

class Random {
public:
	static float RandomRange(int minValue, int maxValue) {
		srand(time(NULL));
		return 1 + minValue + rand() % (maxValue - minValue);
	}

};