#include "vehicle.h"

Vehicle::Vehicle()
{
	this->batteryLevel = 100;
	this->kilometersTravelled = 0.0;
}

void Vehicle::updateBatteryLevel(int pEnergyUnits)
{
	if ((this->batteryLevel - pEnergyUnits) > 0)
		this->batteryLevel -= pEnergyUnits;
	else
		this->batteryLevel = 0;
}

void Vehicle::updateKilometersTravelled(float pKilometers)
{
	this->kilometersTravelled += pKilometers;
}

bool Vehicle::hasEnergy()
{
	return this->batteryLevel > 0;
}

int Vehicle::getBatteryLevel()
{
	return this->batteryLevel;
}

void Vehicle::addWheelForTerrain(Terrain* pTerrain, Wheel* pWheel)
{
	this->configurations[pTerrain] = pWheel;
	updateBatteryLevel(pWheel->getEnergeticCost());

}

void Vehicle::printRouteConfiguration()
{
	std::cout << std::endl;
	std::cout << "*** Configuration for terrains in detected route ***" << std::endl;
	std::cout << "Batery Units Left: " << this->batteryLevel << std::endl;
	for (const auto& configuration : this->configurations)
	{
		configuration.first->print();
		configuration.second->print();
	}
	std::cout << "****************************************************" << std::endl;
}