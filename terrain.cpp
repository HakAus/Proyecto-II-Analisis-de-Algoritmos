#include "terrain.h"

Terrain::Terrain(const char * pName, float pAttributes[3],
				 int pStartKm, int pEndKm)
{				 
	this->name = pName;
	for (int index = 0; index < 3; index++)
		this->attributes[index] = pAttributes[index];
	this->startKm = pStartKm;
	this->endKm = pEndKm;
}

const char * Terrain::getName() { 
	return this->name; 
}
const float* Terrain::getAttributes() { 
	return this->attributes; 
}
const int Terrain::getStartKm() { 
	return this->startKm; 
}
const int Terrain::getEndKm() { 
	return this->endKm; 
}