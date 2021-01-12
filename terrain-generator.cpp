#include "terrain-generator.h"




TerrainGenerator::TerrainGenerator(const rapidjson::Document& pConfig)
{
	for (auto const& te : pConfig.GetObject())
	{
		std::cout << te.name.GetString()<<std::endl;
	}
	for (auto const& te : pConfig["terrains"].GetArray().Begin()->GetObject())
	{
		std::cout << te.name.GetString()<<":";
		if (te.value.IsString()) {
			std::cout << te.value.GetString();
		}
		else {
			std::cout << te.value.GetArray()[0].GetInt()<<":"<< te.value.GetArray()[1].GetInt();
		}
		std::cout<<std::endl;
	}
}
