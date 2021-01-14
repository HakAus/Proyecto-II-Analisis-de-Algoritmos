#include "terrain.h"

Terrain::Terrain(const char * pName, float pAttributes[3],
				 int pStartKm, int pEndKm)
{				 
	this->name = pName;
	for (int index = 0; index < 3; index++)
		this->attributes[index] = pAttributes[index];
	this->startKm = pStartKm;
	this->endKm = pEndKm;
}

const char * Terrain::getName() { 
	return this->name; 
}
const float* Terrain::getAttributes() { 
	return this->attributes; 
}
const int Terrain::getStartKm() { 
	return this->startKm; 
}
const int Terrain::getEndKm() { 
	return this->endKm; 
}

void Terrain::toJsonObject(rapidjson::StringBuffer* pStrBuffer)
{
	rapidjson::Writer<rapidjson::StringBuffer> writer(*pStrBuffer);
	writer.StartObject();
	std::string strName = name;
	writer.Key("KmStart");
	writer.Uint(startKm);
	writer.Key("KmEnd");
	writer.Uint(endKm);
	writer.Key("Firmeza");
	writer.Uint(attributes[0]);
	writer.Key("Humedad");
	writer.Uint(attributes[1]);
	writer.Key("Agarre");
	writer.Uint(attributes[2]);
	writer.EndObject();
}

