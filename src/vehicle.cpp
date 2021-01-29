#include "vehicle.h"

Vehicle::Vehicle()
{
	this->batteryLevel = 7000;
	this->kilometersTravelled = 0.0;
}

void Vehicle::updateBatteryLevel(int pEnergyUnits)
{
	this->batteryLevel -= pEnergyUnits;
}

void Vehicle::updateKilometersTravelled(float pKilometers)
{
	this->kilometersTravelled += pKilometers;
}

int Vehicle::getBatteryLevel()
{
	return this->batteryLevel;
}

void Vehicle::addConfiguration(Terrain* pTerrain, Wheel* pWheel)
{
	this->configurations[pTerrain] = pWheel;
}

void Vehicle::printRouteConfiguration()
{
	std::cout << std::endl;
	std::cout << "*** Configuration for terrains in detected route ***" << std::endl;
	for (const auto& configuration : this->configurations)
	{
		configuration.first->print();
		configuration.second->print();
	}
	std::cout << "****************************************************" << std::endl;
}