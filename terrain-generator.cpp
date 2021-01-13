#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig)
{
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
}

void TerrainGenerator::generateTerrains()
{
	while (currentDistance < distance)
	{
		int pos = Random::RandomRange(0, terrains.size());
		Terrain * terrain = terrains.at(pos)->getTerrain(currentDistance,distance,minimunTerrainLength, maximunTerrainLength);
		currentDistance = terrain->getEndKm();
		numeroDeTerrenos++;
	}
	std::cout << numeroDeTerrenos;
}

void TerrainGenerator::showTerrains()
{
	for (auto const& terru : terrains)
	{
		std::cout << terru->toString();
	}
}


