#include "reader.h"

Reader::Reader(){}

void Reader::read(const char * pJSON)
{
	this->DOM.Parse(pJSON);
	assert(this->DOM.IsObject());
}

rapidjson::Document* Reader::getDocument()
{
	return &this->DOM;
}