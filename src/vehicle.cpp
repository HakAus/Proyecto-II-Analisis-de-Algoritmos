#include "vehicle.h"
#include <iostream>
Vehicle::Vehicle(unsigned short pChromosome)
{
	this->chromosome = pChromosome;
	this->fitnessScore = 0.0;
}

int Vehicle::getTreadId()
{
	unsigned char c = 0;
	c = c | this->chromosome;
	double num = c;
	num += 0.000000001;
	return (int) (11 - ceil(num/CHROMOSOME_RANGE_SIZE));
}

int Vehicle::getTorqueId()
{
	unsigned short tmpChromosome = this->chromosome >> 8;
	unsigned char c = 0;
	c = c | tmpChromosome;
	double num = c;
	num += 0.000000001;
	return (int) ceil(num/CHROMOSOME_RANGE_SIZE);
}

void Vehicle::setFitnessScore(double pScore)
{
	this->fitnessScore = pScore;
}

double Vehicle::getFitnessScore()
{
	return this->fitnessScore;
}

void Vehicle::setChromosome(unsigned short pChromosome)
{
	this->chromosome = pChromosome;
}

unsigned short Vehicle::getChromosome()
{
	return this->chromosome;
}
