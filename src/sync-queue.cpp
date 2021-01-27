#include "sync-queue.h"

SyncQueue::SyncQueue(const rapidjson::Document& pConfig)
{
	this->maxSize = pConfig["simulationConfig"].GetObject()["maxQueueItems"].GetInt();
}

SyncQueue::~SyncQueue()
{
	while(!this->queue.empty())
	{
		this->queue.pop();
	}
}

std::string SyncQueue::getInfo()
{
	return this->message;
}

void SyncQueue::toString()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	std::queue<rapidjson::Document*> copy;
	while(!this->queue.empty())
	{
		rapidjson::Document* doc = this->queue.front();
		copy.push(doc);
		for (auto const& tramo : doc->GetArray())
		{
			this->message += "--------------------\n";
			int kmStart = tramo.GetObject()["KmStart"].GetInt();
			this->message += "KmStart: " + std::to_string(kmStart) + "\n";
			int kmEnd = tramo.GetObject()["KmEnd"].GetInt();
			this->message += "KmEnd: " + std::to_string(kmEnd) + "\n";
			int firmeza = tramo.GetObject()["Firmeza"].GetInt();
			this->message += "Firmeza: " + std::to_string(firmeza) + "\n";
			int humedad = tramo.GetObject()["Humedad"].GetInt();
			this->message += "Humedad: " + std::to_string(humedad) + "\n";
			int agarre = tramo.GetObject()["Agarre"].GetInt();
			this->message += "Agarre: " + std::to_string(agarre) + "\n";
			this->message += "--------------------\n";
		}
		this->queue.pop();
	}
	while (!copy.empty())
	{
		this->queue.push(copy.front());
		copy.pop();
	}	
	locker.unlock();
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
	bool result = this->queue.empty();
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