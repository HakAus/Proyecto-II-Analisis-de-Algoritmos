#pragma once
#ifndef _SYNCQUEUE_H_
#define _SYNCQUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>

class SyncQueue
{
private:
	std::queue<int/*std::vector<Terrain*>*/> queue;
	std::mutex mutex;
	std::condition_variable condition;
	int maxSize;

public:
	SyncQueue(int pMaxSize);
	void push(int pValue);
	int pop();
	bool empty();
};

#endif /* _SYNCQUEUE_H_ */