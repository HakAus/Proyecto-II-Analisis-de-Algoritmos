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
	std::queue<rapidjson::Value*> queue;
	std::mutex mutex;
	std::condition_variable condition;
	int maxSize;

public:
	SyncQueue(const rapidjson::Document& pConfig);
	void push(rapidjson::Value* pValue);
	rapidjson::Value* pop();
	bool empty();
	rapidjson::Value* front();
	int size();

};

#endif /* _SYNCQUEUE_H_ */