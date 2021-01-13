#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator()
{
	this->distanceGenerated = 0;	
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
