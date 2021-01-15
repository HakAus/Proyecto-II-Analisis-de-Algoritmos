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

Terrain::Terrain(const rapidjson::Value& pValue){
	this->name = pValue.GetObject()["Name"].GetString();
	this->attributes[0] = pValue.GetObject()["Firmeza"].GetDouble();
	this->attributes[1] = pValue.GetObject()["Humedad"].GetDouble();
	this->attributes[2] = pValue.GetObject()["Agarre"].GetDouble();
	this->startKm = pValue.GetObject()["KmStart"].GetInt();
	this->endKm = pValue.GetObject()["KmEnd"].GetInt();
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

rapidjson::Value* Terrain::toJsonObject(rapidjson::Document::AllocatorType& pAllocator)
{
	rapidjson::Value* terrain = new rapidjson::Value(rapidjson::kObjectType);
	terrain->AddMember("Name", rapidjson::Value(this->name,pAllocator), pAllocator);
	terrain->AddMember("KmStart", rapidjson::Value(this->startKm), pAllocator);
	terrain->AddMember("KmEnd", rapidjson::Value(this->endKm), pAllocator);
	terrain->AddMember("Firmeza", rapidjson::Value(this->attributes[0]), pAllocator);
	terrain->AddMember("Humedad", rapidjson::Value(this->attributes[1]), pAllocator);
	terrain->AddMember("Agarre", rapidjson::Value(this->attributes[2]), pAllocator);
	// rapidjson::Writer<rapidjson::StringBuffer> writer(*pStrBuffer);
	// writer.StartObject();
	// //std::string strName = name;
	// writer.Key("KmStart");
	// writer.Uint(startKm);
	// writer.Key("KmEnd");
	// writer.Uint(endKm);
	// writer.Key("Firmeza");
	// writer.Uint(attributes[0]);
	// writer.Key("Humedad");
	// writer.Uint(attributes[1]);
	// writer.Key("Agarre");
	// writer.Uint(attributes[2]);
	// writer.EndObject();
	return terrain;

}
