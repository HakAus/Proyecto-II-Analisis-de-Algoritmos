#include "wheel.h"
#include <iostream>

Wheel::Wheel(unsigned short pChromosome)
{
	this->chromosome = pChromosome;
	this->fitnessScore = 0.0;
	this->energeticCost = 0;
}

void showbits(std::string text, unsigned char x )
 {
     int i=0;
	 std::cout<<text;
     for (i = (sizeof(char) * 8) - 1; i >= 0; i--)
     {
        putchar(x & (1u << i) ? '1' : '0');
     }
     printf("\n");
 }

void showbits2(std::string text, unsigned short x )
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
	unsigned short torqueGenotype = floor(pTorqueId * CHROMOSOME_RANGE_SIZE);
	unsigned short treadGenotype = floor((11 - pTreadId) * CHROMOSOME_RANGE_SIZE);
	torqueGenotype = torqueGenotype << 8;

	this->chromosome = this->chromosome | torqueGenotype;
	this->chromosome = this->chromosome | treadGenotype;
	this->fitnessScore = 0.0;
	this->energeticCost = 0;
}


int Wheel::getTreadId() const
{
	unsigned char c = 0;
	c = c | this->chromosome;
	double num = c;
	num += 0.000000001;	// Para evitar que el numero sea cero
	return (int) (11 - ceil(num/CHROMOSOME_RANGE_SIZE));
}

int Wheel::getTorqueId() const
{
	unsigned short tmpChromosome = this->chromosome >> 8;
	unsigned char c = 0;
	c = c | tmpChromosome;
	double num = c;
	num += 0.000000001; // Para evitar que el numero sea cero
	return (int) ceil(num/CHROMOSOME_RANGE_SIZE);
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

unsigned short Wheel::getChromosome() const
{
	return this->chromosome;
}

void Wheel::print()
{
	std::cout << "**** Datos de la llanta ****" << std::endl;
	showbits2("Cromosoma: ", this->chromosome);
	std::cout << "fitnessScore: " << this->fitnessScore << std::endl;
	std::cout << "energeticCost: " << this->energeticCost << std::endl;
}