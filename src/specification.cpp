#include "specification.h"

Specification::Specification(int pId, std::vector<int>& pAttributes, int pEnergy)
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

void Specification::getClosestAttributesTo(std::vector<float>& pTerrainAttributes, std::vector<int>& pAttributes)
{
	pAttributes.push_back(getClosestAttribute(pTerrainAttributes[0], this->firmness));
	pAttributes.push_back(getClosestAttribute(pTerrainAttributes[1], this->humidity));
	pAttributes.push_back(getClosestAttribute(pTerrainAttributes[2], this->grip));
}

int Specification::getClosestAttribute(float pTarget, std::vector<int>& pAttributeContainer)
{
	return (int) (pAttributeContainer[0] * (pTarget < pAttributeContainer[0])) +	// Caso en que target es inferior al minimo del rango
				 (pTarget * (pAttributeContainer[0] <= pTarget && pTarget <= pAttributeContainer[1])) + // Caso en que target esta dentro del rango
				 (pAttributeContainer[1] * (pTarget > pAttributeContainer[1]));
}

void Specification::print()
{
	for (int i = 0; i < 2; i++)
	{
		std::cout << "Firmness: " << this->firmness[i] << std::endl;
		std::cout << "Humidity: " << this->humidity[i] << std::endl;
		std::cout << "Grip: " << this->grip[i] << std::endl;
	}
}
