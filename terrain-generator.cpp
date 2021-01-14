#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig,std::queue<rapidjson::Document> *pSharedBuffer)
{
	this->sharedBuffer = pSharedBuffer;
	for (auto const& te : pConfig["terrains"].GetArray())
	{
		std::string name;
		int ranges[6];
		int counter = 0;
		for (auto & itr : te.GetObject())
		{
			if (itr.value.IsString())
				name = itr.value.GetString();
			else
			{
					ranges[counter] = itr.value.GetArray()[0].GetInt();
					ranges[counter+1] = itr.value.GetArray()[1].GetInt();	
					counter+=2;
			}
		}
		terrains.push_back(new TerrainPrototype(name,ranges));
	}
	generateTerrains();
	showQueue();
}

void TerrainGenerator::generateTerrains()//Puede encender un flag para no producir mas genera un tramo revisar que llegue al numero de distancia necesario o que se pase un poco
{
	int stretchLength = Random::RandomRange(minStretchLength,maxStretchLength);
	rapidjson::Document stretch;
	stretch.SetArray();
	rapidjson::StringBuffer sb;
	rapidjson::StringBuffer * strbuf = &sb;
	rapidjson::Writer<rapidjson::StringBuffer> writer(*strbuf);
	writer.StartArray();
	while (currentDistance < stretchLength)
	{
		int terrainDistance = Random::RandomRange(minimunTerrainLength, maximunTerrainLength);
		int distanceLeft = distance - currentDistance;
		if (currentDistance + terrainDistance > distanceLeft)
			if (distanceLeft < minimunTerrainLength)	
				break;
			else
				terrainDistance = distanceLeft;
		int pos = Random::RandomRange(0, terrains.size());
		Terrain * terrain = terrains.at(pos)->getTerrain(currentDistance,terrainDistance,minimunTerrainLength, maximunTerrainLength);
		terrain->toJsonObject(strbuf);
		currentDistance = terrain->getEndKm();
	}
	writer.EndArray();
	rapidjson::Writer<rapidjson::StringBuffer> newWriter(*strbuf);
	stretch.Accept(newWriter);
	assert(stretch.IsArray());
	//sharedBuffer->push(stretch);
	std::cout<<("JsonData1 = %s", strbuf->GetString());
}

void TerrainGenerator::showTerrains()
{
	for (auto const& terru : terrains)
	{
		std::cout << terru->toString();
	}
}

void TerrainGenerator::showQueue()
{
	std::cout << sharedBuffer->size();
	for(int i = 0;i <sharedBuffer->size();i++)
	{
		assert(sharedBuffer->front().IsArray());
	}
}

