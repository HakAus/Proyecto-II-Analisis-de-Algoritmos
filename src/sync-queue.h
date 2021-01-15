#pragma once
#ifndef _SYNCQUEUE_H_
#define _SYNCQUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include "../libs/rapidjson/document.h"

class SyncQueue
{
private:
	std::queue<rapidjson::Document*> queue;
	std::mutex mutex;
	std::condition_variable condition;
	int maxSize;
	std::string message;

public:
	SyncQueue(const rapidjson::Document& pConfig);
	~SyncQueue();
	void push(rapidjson::Document* pValue);
	rapidjson::Document* pop();
	bool empty();
	rapidjson::Document* front();
	int size();
	void toString();
	std::string getInfo();

};

#endif /* _SYNCQUEUE_H_ */