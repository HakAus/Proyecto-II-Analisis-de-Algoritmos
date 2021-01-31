#pragma once
#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include <vector>
#include <utility>
#include "terrain.h"
#include "wheel.h"

class Vehicle 
{
private:
	int batteryLevel;
	float kilometersTravelled;
	std::vector<std::pair<Terrain*, Wheel*>> configurations;

public:
	Vehicle();
	void updateBatteryLevel(int pEnergyUnits);
	void updateKilometersTravelled(float pKilometers);
	int getBatteryLevel();
	bool hasEnergy();
	void addWheelForTerrain(Terrain* pTerrain, Wheel* pWheel);
	void printRouteConfiguration();

};

#endif /*__VEHICLE_H_*/