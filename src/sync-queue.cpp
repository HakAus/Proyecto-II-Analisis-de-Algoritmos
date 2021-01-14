#include "sync-queue.h"

SyncQueue::SyncQueue(const rapidjson::Document& pConfig)
{

	this->maxSize = pConfig["simulationConfig"].GetObject()["maxQueueItems"].GetInt();
}

void SyncQueue::push(rapidjson::Value* pValue)
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return this->queue.size() < this->maxSize;});
	this->queue.push(pValue);
	std::cout << "Se produjo " << pValue << std::endl;
	locker.unlock();
	this->condition.notify_one();
}

rapidjson::Value* SyncQueue::pop()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return !this->queue.empty();});
	rapidjson::Value* result = this->queue.front();
	this->queue.pop();
	std::cout << "Se consumio " << result << std::endl;
	locker.unlock();
	this->condition.notify_one();
	return result;
}

rapidjson::Value* SyncQueue::front()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return !this->queue.empty();});
	rapidjson::Value* result = this->queue.front();
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