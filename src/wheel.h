#pragma once
#ifndef _WHEEL_H_
#define _WHEEL_H_

#include <cmath>

class Wheel
{
private:
	double CHROMOSOME_RANGE_SIZE = 9362.15;
	unsigned int chromosome;
	double fitnessScore;
	int energeticCost;

	//debug
	double overallSimilarity;
	double torqueSimilarity;
	double treadSimilarity;
	double treadEnergyPercentile;
	double torqueEnergyPercentile;

public:
	Wheel(unsigned int pChromosome);
	Wheel(int pTorqueId, int pTreadId);
	int getTreadId() const;
	int getTorqueId() const;
	void setFitnessScore(double pScore);
	double getFitnessScore() const;
	void setEnergeticCost(int pEnergy);
	int getEnergeticCost() const;
	void setChromosome(unsigned short pChromosome);
	unsigned int getChromosome() const;
	void print();

	//debug
	void setOverallSimilarity(double pSimilarity);
	double getOverallSimilarity() const;
	void setTorqueSimilarity(double pSimilarity);
	double getTorqueSimilarity() const;
	void setTreadSimilarity(double pSimilarity);
	double getTreadSimilarity() const;
	void setTreadEnergyPercentile(int pEnergy);
	double getTreadEnergyPercentile() const;
	void setTorqueEnergyPercentile(int pEnergy);
	double getTorqueEnergyPercentile() const;

};

class WheelComparator {
public:
  bool operator()(Wheel *a, Wheel *b) {
    return a->getFitnessScore() > b->getFitnessScore();
  }
};

#endif /* _WHEEL_H_ */
