#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig,std::queue<rapidjson::Document*> *pSharedBuffer)
{
	this->sharedBuffer = pSharedBuffer;
	for (auto const& te : pConfig["terrains"].GetArray())
	{
		std::string name;
		int ranges[6];
		int counter = 0;
		for (auto & itr : te.GetObject())
		{
			if (itr.value.IsString())
				name = itr.value.GetString();
			else
			{
					ranges[counter] = itr.value.GetArray()[0].GetInt();
					ranges[counter+1] = itr.value.GetArray()[1].GetInt();	
					counter+=2;
			}
		}
		terrains.push_back(new TerrainPrototype(name,ranges));
	}
	generateTerrains();
	//showQueue();
}

void TerrainGenerator::generateTerrains()//Puede encender un flag para no producir mas genera un tramo revisar que llegue al numero de distancia necesario o que se pase un poco
{
	std::cout << "1";
	int stretchLength = Random::RandomRange(minStretchLength,maxStretchLength);
	rapidjson::Document stretch;
	stretch.SetNull();
	rapidjson::Document::AllocatorType& stretchAllocator = stretch.GetAllocator();
	std::cout << "2";
	while (currentDistance < stretchLength)
	{
		std::cout << "3";
		int terrainDistance = Random::RandomRange(minimunTerrainLength, maximunTerrainLength);
		int distanceLeft = distance - currentDistance;
		if (currentDistance + terrainDistance > distanceLeft)
			if (distanceLeft < minimunTerrainLength)	
				break;
			else
				terrainDistance = distanceLeft;
		if (stretch.IsNull())
			stretch.SetArray();
		std::cout << "4";
		int pos = Random::RandomRange(0, terrains.size());
		Terrain * terrain = terrains.at(pos)->getTerrain(currentDistance,terrainDistance,minimunTerrainLength, maximunTerrainLength);
		stretch.PushBack(terrain->toJsonObject(), stretchAllocator);
		currentDistance = terrain->getEndKm();
	}
	std::cout << "5";
	if (!stretch.IsNull()) {
		std::cout << "5.1";
		sharedBuffer->push(&stretch);
	}
	std::cout << "6";
}

void TerrainGenerator::showTerrains()
{
	for (auto const& terru : terrains)
	{
		std::cout << terru->toString();
	}
}

void TerrainGenerator::showQueue()
{
	std::cout << sharedBuffer->size();
	for(int i = 0;i <sharedBuffer->size();i++)
	{
		rapidjson::Document* doc = sharedBuffer->front();
		std::cout<<doc->GetArray().Begin()->GetString();
	}
}

