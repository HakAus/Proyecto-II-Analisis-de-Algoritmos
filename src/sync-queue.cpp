#include "sync-queue.h"

SyncQueue::SyncQueue(const rapidjson::Document& pConfig)
{

	this->maxSize = pConfig["simulationConfig"].GetObject()["maxQueueItems"].GetInt();
}
void SyncQueue::print()
{
	std::queue<rapidjson::Document*> copy;
	while(!this->queue.empty())
	{
		rapidjson::Document* doc = this->queue.front();
		copy.push(doc);
		for (auto const& tramo : doc->GetArray())
		{
			std::cout << "--------------------" << std::endl;
			int kmStart = tramo.GetObject()["KmStart"].GetInt();
			std::cout << "KmStart: " << kmStart << std::endl;
			int kmEnd = tramo.GetObject()["KmEnd"].GetInt();
			std::cout << "KmEnd: " << kmEnd << std::endl;
			int firmeza = tramo.GetObject()["Firmeza"].GetInt();
			std::cout << "Firmeza: " << firmeza << std::endl;
			int humedad = tramo.GetObject()["Humedad"].GetInt();
			std::cout << "Humedad: " << humedad << std::endl;
			int agarre = tramo.GetObject()["Agarre"].GetInt();
			std::cout << "Agarre: " << agarre << std::endl;
			std::cout << "--------------------" << std::endl;
		}
		this->queue.pop();
	}
	while (!copy.empty())
	{
		this->queue.push(copy.front());
		copy.pop();
	}	
	
}

void SyncQueue::push(rapidjson::Document* pValue)
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return this->queue.size() < this->maxSize;});
	this->queue.push(pValue);
	locker.unlock();
	this->condition.notify_one();
}

rapidjson::Document* SyncQueue::pop()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return !this->queue.empty();});
	rapidjson::Document* result = this->queue.front();
	this->queue.pop();
	locker.unlock();
	this->condition.notify_one();
	return result;
}

rapidjson::Document* SyncQueue::front()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return !this->queue.empty();});
	rapidjson::Document* result = this->queue.front();
	locker.unlock();
	this->condition.notify_one();
	return result;
}

bool SyncQueue::empty()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	bool result =  this->queue.empty();
	locker.unlock();
	return result;
}

int SyncQueue::size()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	int size = this->queue.size();
	locker.unlock();
	return size;
}