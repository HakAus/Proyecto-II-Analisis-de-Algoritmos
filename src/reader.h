#pragma once
#include "../libs/rapidjson/document.h"

class Reader
{
private:
	rapidjson::Document DOM;

public:
	Reader();
	void read(const char * pJSON);
	void readInto(const char * pJSON, rapidjson::Document& pDocument);
	rapidjson::Document& getDocument();
};