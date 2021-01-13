#include "sync-queue.h"

SyncQueue::SyncQueue(int pMaxSize)
{
	maxSize = pMaxSize;
}

void SyncQueue::push(int pValue)
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return this->queue.size() < this->maxSize;});
	this->queue.push(pValue);
	std::cout << "Se produjo " << pValue << std::endl;
	locker.unlock();
	this->condition.notify_one();
}

int SyncQueue::pop()
{
	std::unique_lock<std::mutex> locker(this->mutex);
	this->condition.wait(locker, [this](){return !this->queue.empty();});
	int result = this->queue.front();
	this->queue.pop();
	std::cout << "Se consumio " << result << std::endl;
	locker.unlock();
	this->condition.notify_one();
	return result;
}

bool SyncQueue::empty()
{
	return this->queue.empty();
}