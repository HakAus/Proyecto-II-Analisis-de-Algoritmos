#pragma once
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <cmath>

class Vehicle
{
private:
	const double CHROMOSOME_RANGE_SIZE = 36.45;
	unsigned short chromosome;
	float fitnessScore;

public:
	Vehicle(unsigned short pChromosome);
	short getTreadId();
	short getTorqueId();
	void setFitnessScore(float pScore);
	float getFitnessScore();
	void setChromosome(unsigned short pChromosome);
	unsigned short getChromosome();
};

#endif /* _VEHICLE_H_ */
