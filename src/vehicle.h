#pragma once
#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include <unordered_map>
#include "terrain.h"
#include "wheel.h"

class Vehicle 
{
private:
	int batteryLevel;
	float kilometersTravelled;
	std::unordered_map<Terrain*, Wheel*> configurations;

public:
	Vehicle();
	void updateBatteryLevel(int pEnergyUnits);
	void updateKilometersTravelled(float pKilometers);
	int getBatteryLevel();
	void addConfiguration(Terrain* pTerrain, Wheel* pWheel);
	void printRouteConfiguration();

};

#endif /*__VEHICLE_H_*/