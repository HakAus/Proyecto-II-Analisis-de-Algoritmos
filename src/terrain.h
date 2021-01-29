#pragma once
#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include <iostream>
#include <vector>
#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/writer.h"

class Terrain
{
private:
	const char * name;
	std::vector<float> attributes;	// firmness, humidity, grip
	int startKm;
	int endKm;

public:
	Terrain(const char * pName, float pAttributes[3],
			int pStartKm, int pEndKm);
	Terrain(const rapidjson::Value& pValue);
	const char * getName();
	std::vector<float> getAttributes();
	const int getStartKm();
	const int getEndKm();
	rapidjson::Value* toJsonObject(rapidjson::Document::AllocatorType& allocator);
	void print();
	std::string toString();
};

#endif /* _TERRAIN_H_ */
