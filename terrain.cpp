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

rapidjson::Document Terrain::toJsonObject()
{
	rapidjson::Document objValue;
	objValue.SetObject();
	rapidjson::Value strName(name, objValue.GetAllocator());
	rapidjson::Document::AllocatorType& allocator = objValue.GetAllocator();
	objValue.AddMember("name", strName, allocator);
	objValue.AddMember("KmStart", startKm, allocator);
	objValue.AddMember("KmEnd", endKm, allocator);
	objValue.AddMember("Firmeza", attributes[0], allocator);
	objValue.AddMember("Humedad", attributes[1], allocator);
	objValue.AddMember("Agarre", attributes[2], allocator);
	return objValue;
}

