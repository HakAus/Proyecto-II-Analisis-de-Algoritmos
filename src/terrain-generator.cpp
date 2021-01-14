#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->currentDistance = 0;
	this->distance = 200;
	this->totalTerrains = 0;
	this->minimunTerrainLength = 5;
	this->maximunTerrainLength = 20;
	this->minStretchLength = 40;
	this->maxStretchLength = 80;
	this->maxQueueSize = 10;
	this->generationWaitTime = 4;
	this->queue = pSharedQueue;
	this->readTerrainData(pConfig);
}

void TerrainGenerator::readTerrainData(const rapidjson::Document& pConfig)
{
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
	std::cout << "Terrain Data Read" << std::endl;
}

void TerrainGenerator::generateStretch()//Puede encender un flag para no producir mas genera un tramo revisar que llegue al numero de distancia necesario o que se pase un poco
{
	// std::cout << "1";
	int stretchLength = Random::RandomRange(minStretchLength,maxStretchLength);
	rapidjson::Document stretch;
	stretch.SetNull();
	rapidjson::Document::AllocatorType& stretchAllocator = stretch.GetAllocator();
	// std::cout << "2";
	while (currentDistance < stretchLength)
	{
		// std::cout << "3";
		int terrainDistance = Random::RandomRange(minimunTerrainLength, maximunTerrainLength);
		int distanceLeft = distance - currentDistance;
		if (currentDistance + terrainDistance > distanceLeft)
			if (distanceLeft < minimunTerrainLength)	
				break;
			else
				terrainDistance = distanceLeft;
		if (stretch.IsNull())
			stretch.SetArray();
		// std::cout << "4";
		int pos = Random::RandomRange(0, terrains.size());
		Terrain * terrain = terrains.at(pos)->getTerrain(currentDistance,terrainDistance,minimunTerrainLength, maximunTerrainLength);
		stretch.PushBack(terrain->toJsonObject(), stretchAllocator);
		currentDistance = terrain->getEndKm();
	}
	// std::cout << "5";
	if (!stretch.IsNull()) {
		// std::cout << "5.1";
		this->queue->push(&stretch);
	}
	// std::cout << "6";
}

void TerrainGenerator::showTerrains()
{
	for (auto const& terru : terrains)
	{
		std::cout << terru->toString();
	}	
}

void TerrainGenerator::generate()
{
	std::cout << "Called generate" << std::endl;
	while (currentDistance < distance)
	{
		this->generateStretch();
		this->showQueue();
		std::this_thread::sleep_for (std::chrono::seconds(this->generationWaitTime));	// para frenar la genearacion de terrenos
	}
}

void TerrainGenerator::showQueue()
{
	std::cout << queue->size();
	for(int i = 0;i <queue->size();i++)
	{
		rapidjson::Value* doc = queue->front();
		std::cout<<doc->GetArray().Begin()->GetString();
	}
}

std::vector<Terrain*> TerrainGenerator::getStretch()
{
	std::vector<Terrain*> dummy;
	return dummy;
}

void TerrainGenerator::start()
{
	this->producer = std::thread(&TerrainGenerator::generate,this);
}

void TerrainGenerator::join()
{
	this->producer.join();
}
