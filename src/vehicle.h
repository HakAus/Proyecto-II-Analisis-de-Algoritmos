#pragma once
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <cmath>

class Vehicle
{
private:
	const double CHROMOSOME_RANGE_SIZE = 36.45;
	unsigned short chromosome;
	double fitnessScore;

public:
	Vehicle(unsigned short pChromosome);
	int getTreadId() const;
	int getTorqueId() const;
	void setFitnessScore(double pScore);
	double getFitnessScore() const;
	void setChromosome(unsigned short pChromosome);
	unsigned short getChromosome() const;

};

class VehicleComparator {
public:
  bool operator()(Vehicle *a, Vehicle *b) {
    return a->getFitnessScore() > b->getFitnessScore();
  }
};

#endif /* _VEHICLE_H_ */
