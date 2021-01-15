#include "specification.h"

Specification::Specification(int pId, int pAttributes[6], int pEnergy)
{
	this->id = pId;
	for (int index = 0; index < 2; index++)
	{
		this->firmness[index] = pAttributes[index];
		this->humidity[index] = pAttributes[index+2]; 
		this->grip[index] = pAttributes[index+4];
	}
	this->energy = pEnergy;
}

int* Specification::getFirmness() { return this->firmness; }
int* Specification::getHumidity() { return this->humidity; }
int* Specification::getGrip() { return this->grip; }