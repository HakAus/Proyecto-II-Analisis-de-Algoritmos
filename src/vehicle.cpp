#include "vehicle.h"

Vehicle::Vehicle(unsigned short pChromosome)
{
	this->chromosome = pChromosome;
	this->fitnessScore = 0.0;
}

short Vehicle::getTreadId()
{
	unsigned char c = 0;
	c = c | this->chromosome;
	double num = c;
	return 11 - ceil(num/CHROMOSOME_RANGE_SIZE);
}

short Vehicle::getTorqueId()
{
	this->chromosome = this->chromosome >> 8;
	unsigned char c = 0;
	c = c | this->chromosome;
	double num = c;
	return ceil(num/CHROMOSOME_RANGE_SIZE);
}

void Vehicle::setFitnessScore(float pScore)
{
	this->fitnessScore = pScore;
}

float Vehicle::getFitnessScore()
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
