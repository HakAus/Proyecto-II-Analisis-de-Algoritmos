#include "terrain.h"

Terrain::Terrain(const char * pName, float pAttributes[3],
				 int pStartKm, int pEndKm)
{				 
	this->name = pName;
	for (int index = 0; index < 3; index++)
		this->attributes.push_back(pAttributes[index]);
	this->startKm = pStartKm;
	this->endKm = pEndKm;
}

Terrain::Terrain(const rapidjson::Value& pValue)
{
	this->name = pValue.GetObject()["name"].GetString();
	this->attributes.push_back(pValue.GetObject()["firmness"].GetDouble());
	this->attributes.push_back(pValue.GetObject()["humidity"].GetDouble());
	this->attributes.push_back(pValue.GetObject()["grip"].GetDouble());
	this->startKm = pValue.GetObject()["kmStart"].GetInt();
	this->endKm = pValue.GetObject()["kmEnd"].GetInt();
}

const char * Terrain::getName() { return this->name; }
std::vector<float> Terrain::getAttributes() { return this->attributes; }
const int Terrain::getStartKm() { return this->startKm; }
const int Terrain::getEndKm() { return this->endKm; }

rapidjson::Value* Terrain::toJsonObject(rapidjson::Document::AllocatorType& pAllocator)
{
	rapidjson::Value* terrain = new rapidjson::Value(rapidjson::kObjectType);
	terrain->AddMember("name", rapidjson::Value(this->name,pAllocator), pAllocator);
	terrain->AddMember("firmness", rapidjson::Value(this->attributes[0]), pAllocator);
	terrain->AddMember("humidity", rapidjson::Value(this->attributes[1]), pAllocator);
	terrain->AddMember("grip", rapidjson::Value(this->attributes[2]), pAllocator);
	terrain->AddMember("kmStart", rapidjson::Value(this->startKm), pAllocator);
	terrain->AddMember("kmEnd", rapidjson::Value(this->endKm), pAllocator);
	return terrain;
}

void Terrain::print()
{
	std::cout << "**** Datos del terreno ****" << std::endl;
	std::cout << "Nombre: " << this->name << std::endl;
	std::cout << "Atributos: (";
	int pos = 0;
	for (; pos < 2; pos++)
	{
		std::cout << this->attributes[pos] << ",";
	} 
	std::cout << this->attributes[pos] << ")" << std::endl;
	std::cout << "KmStart: " << this->startKm << std::endl;
	std::cout << "KmEnd: " << this->endKm << std::endl;
	std::cout << "***************************" << std::endl;
}