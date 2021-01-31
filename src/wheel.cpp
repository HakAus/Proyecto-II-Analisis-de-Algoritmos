#include "wheel.h"
#include <iostream>

Wheel::Wheel(unsigned int pChromosome)
{
	this->chromosome = pChromosome;
	this->fitnessScore = 0.0;
	this->energeticCost = 0;

	//debug
	this->overallSimilarity = 0.0;
	this->torqueEnergyPercentile = 0.0;
	this->treadEnergyPercentile = 0.0;
}

void showbitsA(std::string text, unsigned short x )
{
 int i=0;
 std::cout<<text;
 for (i = (sizeof(short) * 8) - 1; i >= 0; i--)
 {
    putchar(x & (1u << i) ? '1' : '0');
 }
 printf("\n");
}

Wheel::Wheel(int pTorqueId, int pTreadId)
{
	unsigned int torqueGenotype = floor(pTorqueId * CHROMOSOME_RANGE_SIZE);
	unsigned int treadGenotype = floor((11.0 - pTreadId) * CHROMOSOME_RANGE_SIZE);
	torqueGenotype = torqueGenotype << 16;

	this->chromosome = this->chromosome | torqueGenotype;
	this->chromosome = this->chromosome | treadGenotype;
	this->fitnessScore = 0.0;
	this->energeticCost = 0;

	// debug
	this->overallSimilarity = 0.0;
	this->torqueEnergyPercentile = 0.0;
	this->treadEnergyPercentile = 0.0;
}


int Wheel::getTreadId() const
{
	// std::cout << "TREAD ID" << std::endl;
	unsigned short c = 0;
	// showbitsA("c: ",c);
	c = c | this->chromosome;
	// showbitsA("c: ",c);
	double num = c;
	num += 0.0000000001;	// Para evitar que el numero sea cero
	// std::cout << "num: " << std::to_string(num) << std::endl;
	// double division = num / CHROMOSOME_RANGE_SIZE;
	// std::cout << "division: " << std::to_string(division) << std::endl;
	// double rounded = ceil(num/ CHROMOSOME_RANGE_SIZE);
	// std::cout << "rounded: " << std::to_string(rounded) << std::endl;
	// double substraction = 11.0 - ceil(num / CHROMOSOME_RANGE_SIZE);
	// std::cout << "substraction: " << std::to_string(substraction) << std::endl;
	// unsigned int result =  (11.0 - ceil(num / CHROMOSOME_RANGE_SIZE));
	// std::cout << "result: " << result << std::endl;
	return (unsigned int) (11.0 - ceil(num / CHROMOSOME_RANGE_SIZE));
}

int Wheel::getTorqueId() const
{
	// std::cout << "TORQUE ID" << std::endl;
	unsigned int c = this->chromosome >> 16;
	// showbitsA("c: ",c);
	double num = c;
	num += 0.0000000001; // Para evitar que el numero sea cero
	// std::cout << "num: " << std::to_string(num) << std::endl;
	// double division = num/CHROMOSOME_RANGE_SIZE;
	// std::cout << "division: " << std::to_string(division) << std::endl;
	// double rounded = ceil(num/CHROMOSOME_RANGE_SIZE);
	// std::cout << "rounded: " << std::to_string(rounded) << std::endl;
	// unsigned int result =  ceil(num/CHROMOSOME_RANGE_SIZE);
	// std::cout << "result: " << result << std::endl;
	return (unsigned int) ceil(num/CHROMOSOME_RANGE_SIZE);
}

void Wheel::setFitnessScore(double pScore)
{
	this->fitnessScore = pScore;
}

double Wheel::getFitnessScore() const
{
	return this->fitnessScore;
}

void Wheel::setEnergeticCost(int pEnergy)
{
	this->energeticCost = pEnergy;
}

int Wheel::getEnergeticCost() const
{
	return this->energeticCost;
}

void Wheel::setChromosome(unsigned short pChromosome)
{
	this->chromosome = pChromosome;
}

unsigned int Wheel::getChromosome() const
{
	return this->chromosome;
}

void Wheel::print()
{
	std::cout << "**** Datos de la llanta ****" << std::endl;
	std::cout << "torqueId: " << getTorqueId() << std::endl;
	std::cout << "treadId: " << getTreadId() << std::endl;
	std::cout << "fitnessScore: " << this->fitnessScore << std::endl;
	std::cout << "overallSimilarity: " << std::to_string(this->overallSimilarity) << std::endl;
	std::cout << "torqueSimilarity: " << std::to_string(this->torqueSimilarity) << std::endl;
	std::cout << "treadSimilarity: " << std::to_string(this->treadSimilarity) << std::endl;
	std::cout << "torqueEnergyPercentile: " << std::to_string(this->torqueEnergyPercentile) << std::endl;
	std::cout << "treadEnergyPercentile: " << std::to_string(this->treadEnergyPercentile) << std::endl;
	std::cout << "energeticCost: " << this->energeticCost << std::endl;
}

// debug
void Wheel::setOverallSimilarity(double pSimilarity)
{
	this->overallSimilarity = pSimilarity;	
}

double Wheel::getOverallSimilarity() const
{
	return this->overallSimilarity;
}

void Wheel::setTorqueSimilarity(double pSimilarity)
{
	this->torqueSimilarity = pSimilarity;
}

double Wheel::getTorqueSimilarity() const
{
	return this->torqueSimilarity;
}

void Wheel::setTreadSimilarity(double pSimilarity)
{
	this->treadSimilarity = pSimilarity;
}

double Wheel::getTreadSimilarity() const
{
	return this->treadSimilarity;
}

void Wheel::setTreadEnergyPercentile(int pEnergy)
{
	this->treadEnergyPercentile = (100 / 15) * pEnergy;
}

double Wheel::getTreadEnergyPercentile() const
{
	return this->treadEnergyPercentile;
}

void Wheel::setTorqueEnergyPercentile(int pEnergy)
{
	this->torqueEnergyPercentile = (100 / 12) * pEnergy;
}

double Wheel::getTorqueEnergyPercentile() const
{
	return this->torqueEnergyPercentile;
}