#include "terrain-generator.h"




TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig)
{
	for (auto const& te : pConfig.GetObject())
	{
		std::cout << te.name.GetString()<<std::endl;
	}
	for (auto const& te : pConfig["terrains"].GetArray())
	{
		for (auto & itr : te.GetObject())
		{
			std::cout << itr.name.GetString()<<std::endl;
			if (itr.value.IsString())
				std::cout << itr.value.GetString();
			else
				std::cout << itr.value.GetArray()[0].GetInt() << ":" << itr.value.GetArray()[1].GetInt();
			std::cout << std::endl;
		}
	}
}
