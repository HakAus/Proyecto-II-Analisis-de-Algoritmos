#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->totalDistance = 0;
	this->distance = 200;
	this->totalTerrains = 0;
	this->minimunTerrainLength = 5;
	this->maximunTerrainLength = 20;
	this->minStretchLength = 40;
	this->maxStretchLength = 80;
	this->maxQueueSize = 10;
	this->generationWaitTime = 4;
	this->queue = pSharedQueue;
	this->producing = true;
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
	int stretchLength = Random::RandomRange(minStretchLength, maxStretchLength);
	int currentDistance = 0;
	rapidjson::Document stretch;
	stretch.SetArray();
	rapidjson::StringBuffer sb;
	rapidjson::StringBuffer* strbuf = &sb;
	rapidjson::Writer<rapidjson::StringBuffer> writer(*strbuf);
	writer.StartArray();
	while (currentDistance < stretchLength)
	{
		int terrainDistance = Random::RandomRange(minimunTerrainLength, maximunTerrainLength);
		int distanceLeft = distance - totalDistance;
		if (totalDistance + terrainDistance > distanceLeft)
			if (distanceLeft < minimunTerrainLength)
			{
				producing = false;
				break;
			}
			else
				terrainDistance = distanceLeft;
		int pos = Random::RandomRange(0, terrains.size());
		Terrain* terrain = terrains.at(pos)->getTerrain(totalDistance, terrainDistance, minimunTerrainLength, maximunTerrainLength);
		terrain->toJsonObject(strbuf);
		currentDistance += terrainDistance;
		totalDistance += terrainDistance;
	}
	writer.EndArray();
	rapidjson::Writer<rapidjson::StringBuffer> newWriter(*strbuf);
	stretch.Accept(newWriter);
	/*rapidjson::Pointer("Value").Set(stretch, stretch.GetArray());
	rapidjson::Value* object = rapidjson::Pointer("Value").Get(stretch);*/
	rapidjson::Value* object = &stretch;
	pushToQueue(object);
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
	while (totalDistance < distance)
	{
		this->generateStretch();
		this->showQueue();
		std::this_thread::sleep_for (std::chrono::seconds(this->generationWaitTime));	// para frenar la genearacion de terrenos
	}
}

void TerrainGenerator::showQueue()
{
	for(int i = 0;i <queue->size();i++)
	{
		rapidjson::Value* val = queue->front();
		//const rapidjson::Value::ConstArray& array = val->GetArray();
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

void TerrainGenerator::pushToQueue(rapidjson::Value* value)
{
	if (producing)
	{
		queue->push(value);
		std::cout << "Generatorpushed" << std::endl;
	}
	else
		std::cout << "DidntPush" << std::endl;
}