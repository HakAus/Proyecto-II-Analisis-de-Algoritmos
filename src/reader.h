#pragma once
#include "../libs/rapidjson/document.h"

class Reader
{
private:
	rapidjson::Document DOM;

public:
	Reader();
	void read(const char * pJSON);
	rapidjson::Document& getDocument();

};