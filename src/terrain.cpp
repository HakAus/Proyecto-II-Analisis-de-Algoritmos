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
	this->name = pValue.GetObject()["Name"].GetString();
	this->attributes.push_back(pValue.GetObject()["Firmeza"].GetDouble());
	this->attributes.push_back(pValue.GetObject()["Humedad"].GetDouble());
	this->attributes.push_back(pValue.GetObject()["Agarre"].GetDouble());
	this->startKm = pValue.GetObject()["KmStart"].GetInt();
	this->endKm = pValue.GetObject()["KmEnd"].GetInt();
}

const char * Terrain::getName() { return this->name; }
std::vector<float> Terrain::getAttributes() { return this->attributes; }
const int Terrain::getStartKm() { return this->startKm; }
const int Terrain::getEndKm() { return this->endKm; }

rapidjson::Value* Terrain::toJsonObject(rapidjson::Document::AllocatorType& pAllocator)
{
	rapidjson::Value* terrain = new rapidjson::Value(rapidjson::kObjectType);
	terrain->AddMember("Name", rapidjson::Value(this->name,pAllocator), pAllocator);
	terrain->AddMember("KmStart", rapidjson::Value(this->startKm), pAllocator);
	terrain->AddMember("KmEnd", rapidjson::Value(this->endKm), pAllocator);
	terrain->AddMember("Firmeza", rapidjson::Value(this->attributes[0]), pAllocator);
	terrain->AddMember("Humedad", rapidjson::Value(this->attributes[1]), pAllocator);
	terrain->AddMember("Agarre", rapidjson::Value(this->attributes[2]), pAllocator);
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