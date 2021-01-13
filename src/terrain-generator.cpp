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
		std::unique_lock<std::mutex> locker(*this->mutex);
		this->condition->wait(locker, [this](){return this->sharedQueue->size() < this->maxQueueSize;});
		this->sharedQueue->push(distanceGenerated);
		std::cout << "Se produjo " << distanceGenerated << std::endl;
		distanceGenerated++;
		locker.unlock();
		this->condition->notify_one();
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

void TerrainGenerator::setSharedQueue(std::queue<int/*std::vector<Terrain*>*/>* pSharedQueue, std::mutex* pMutex, std::condition_variable* pCondition)
{
	this->sharedQueue = pSharedQueue;
	this->mutex = pMutex;
	this->condition = pCondition;
}
