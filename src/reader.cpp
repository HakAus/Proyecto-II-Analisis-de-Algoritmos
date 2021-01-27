#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "reader.h"

Reader::Reader(){}

void Reader::read(const char * pJSON)
{
	std::cout << "Reading " << pJSON << "... " << std::endl;
	std::ifstream stream(pJSON);
	std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	this->DOM.Parse(str.c_str());
	assert(this->DOM.IsObject());
}

void Reader::readInto(const char * pJSON, rapidjson::Document& pDocument)
{
	std::cout << "Reading " << pJSON << "... " << std::endl;
	std::ifstream stream(pJSON);
	std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	pDocument.Parse(str.c_str());
	assert(pDocument.IsObject());
}

rapidjson::Document& Reader::getDocument()
{
	return this->DOM;
}