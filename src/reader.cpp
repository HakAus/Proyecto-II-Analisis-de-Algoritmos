#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "reader.h"

Reader::Reader(){}

void Reader::read(const char * pJSON)
{
	std::ifstream stream(pJSON);
	std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	this->DOM.Parse(str.c_str());
	assert(this->DOM.IsObject());
}

rapidjson::Document& Reader::getDocument()
{
	return this->DOM;
}