#pragma once
#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/writer.h"
#include <iostream>

class Terrain
{
private:
	const char * name;
	float attributes[3];
	int startKm;
	int endKm;

public:
	Terrain(const char * pName, float pAttributes[3],
			int pStartKm, int pEndKm);
	Terrain(const rapidjson::Value& pValue);
	const char * getName();
	const float* getAttributes();
	const int getStartKm();
	const int getEndKm();
	rapidjson::Value* toJsonObject(rapidjson::Document::AllocatorType& allocator);
};

#endif /* _TERRAIN_H_ */
