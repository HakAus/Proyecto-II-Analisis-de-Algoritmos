#pragma once
#ifndef _WHEEL_H_
#define _WHEEL_H_

#include <cmath>

class Wheel
{
private:
	const double CHROMOSOME_RANGE_SIZE = 36.45;
	unsigned short chromosome;
	double fitnessScore;

public:
	Wheel(unsigned short pChromosome);
	Wheel(int pTorqueId, int pTreadId);
	int getTreadId() const;
	int getTorqueId() const;
	void setFitnessScore(double pScore);
	double getFitnessScore() const;
	void setChromosome(unsigned short pChromosome);
	unsigned short getChromosome() const;
	void print();

};

class WheelComparator {
public:
  bool operator()(Wheel *a, Wheel *b) {
    return a->getFitnessScore() > b->getFitnessScore();
  }
};

#endif /* _WHEEL_H_ */
