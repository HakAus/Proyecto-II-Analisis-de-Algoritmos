#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig)
{
	this->distanceGenerated = 0;
	
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
					std::cout << counter;
					counter+=2;
			}
		}
		terrains.push_back(new TerrainPrototype(name,ranges));
	}
	for (auto const& terru : terrains) 
	{
		std::cout << terru->toString();
	}	
}

void TerrainGenerator::generate(int pDistance)
{
	while (distanceGenerated < pDistance)
	{
		this->queue->push(distanceGenerated);
		distanceGenerated++;
	}
}

std::vector<Terrain*> TerrainGenerator::getStretch()
{
	std::vector<Terrain*> dummy;
	return dummy;
}

void TerrainGenerator::start(int pDistance)
{
	this->producer = std::thread(&TerrainGenerator::generate,this,pDistance);

}

void TerrainGenerator::join()
{
	this->producer.join();
}

void TerrainGenerator::setSharedQueue(SyncQueue* pSharedQueue)
{
	this->queue = pSharedQueue;
}
