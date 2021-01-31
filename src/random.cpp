#include <stdlib.h>
#include <time.h>
#include <random>

class Random {

public:
	static float RandomRangeFloat(int minValue, int maxValue)
	{
		static thread_local std::mt19937 engine(1729);
		std::uniform_real_distribution<float> distribution(minValue, maxValue);
		return distribution(engine);
	}
	
	static int RandomRangeInt(int minValue, int maxValue)
	{
		static thread_local std::mt19937 engine(1729);
		std::uniform_int_distribution<int> distribution(minValue, maxValue);
		return distribution(engine);
	}
	static float RandomRange(int minValue, int maxValue) {
		return minValue + rand()%(maxValue - minValue);
	}
	static unsigned int RandomChromosome() {
		static thread_local std::mt19937 engine(1729);
		std::uniform_int_distribution<unsigned int> distribution;
		return distribution(engine);
	}

};