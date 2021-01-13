#include "specification.h"

Specification::Specification(int pFirmness[2], int pHumidity[2], int pGrip[2], int pEnergy)
{
	for (int index = 0; index < 2; index++)
	{
		this->firmness[index] = pFirmness[index];
		this->humidity[index] = pHumidity[index];
		this->grip[index] = pGrip[index];
	}
	this->energy = pEnergy;
}

int* Specification::getFirmness() { return this->firmness; }
int* Specification::getHumidity() { return this->humidity; }
int* Specification::getGrip() { return this->grip; }