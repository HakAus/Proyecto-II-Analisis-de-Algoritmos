#include "specification.h"

Specification::Specification(int pId, std::vector<int> &pAttributes, int pEnergy)
{
	this->id = pId;
	for (int index = 0; index < 2; index++)
	{
		this->firmness.push_back(pAttributes[index]);
		this->humidity.push_back(pAttributes[index+2]); 
		this->grip.push_back(pAttributes[index+4]);
	}
	this->energy = pEnergy;
}

std::vector<int> Specification::getFirmness() { return this->firmness; }
std::vector<int> Specification::getHumidity() { return this->humidity; }
std::vector<int> Specification::getGrip() { return this->grip; }
int Specification::getEnergy() { return this->energy; }

void Specification::getClosestAttributesTo(std::vector<float> &pTerrainAttributes, std::vector<int> &pAttributes)
{
	pAttributes.push_back(getClosestFirmness(pTerrainAttributes[0]));
	pAttributes.push_back(getClosestHumidity(pTerrainAttributes[1]));
	pAttributes.push_back(getClosestGrip(pTerrainAttributes[2]));
}

int Specification::getClosestFirmness(int pValue) 
{
	/*+ 
			    (pValue * (this->firmness[0] <= pValue && pValue <= this->firmness[1])) + 
			    (this->firmness[1] * (this->firmness[1] < pValue));*/
	return this->firmness.at(0) /** (int)(pValue < this->firmness.at(0))*/;
}

int Specification::getClosestHumidity(int pValue) 
{
	/*+ 
			    (pValue * (this->humidity[0] <= pValue && pValue <= humidity[1])) + 
			    (this->humidity[1] * (this->humidity[1] < pValue));*/
	return this->humidity[0] /** (int)(pValue < this->humidity[0])*/;
}

int Specification::getClosestGrip(int pValue) 
{
	/*+ 
			    (pValue * (this->grip[0] <= pValue && pValue <= this->grip[1])) + 
			    (this->grip[1] * (this->grip[1] < pValue));*/
	return this->grip[0] /** (int)(pValue < this->grip[0])*/;
}
