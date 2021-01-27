#include "terrain-generator.h"

TerrainGenerator::TerrainGenerator(Reader* pReader, const rapidjson::Document& pConfig, SyncQueue* pSharedQueue)
{
	this->reader = pReader;
	this->currentDistance = 0;
	this->distanceGoal = pConfig["simulationConfig"].GetObject()["distanceGoal"].GetInt();
	this->totalTerrains = 0;
	this->minimunTerrainLength = 5;
	this->maximunTerrainLength = 20;
	this->minStretchLength = 40;
	this->maxStretchLength = 80;
	this->generationWaitTime = pConfig["simulationConfig"].GetObject()["stretchGenerationWaitSeconds"].GetInt();
	this->queue = pSharedQueue;
	this->producing = true;
	this->randomGeneration = pConfig["simulationConfig"].GetObject()["randomTerrainGeneration"].GetBool();
	this->readTerrainData(pConfig);

	if (!randomGeneration)
	{
		std::string filepath = pConfig["simulationConfig"].GetObject()["manualRouteFilePath"].GetString();
		readRouteData(filepath.c_str());
	}

}

void TerrainGenerator::readRouteData(std::string pFilepath)
{

	this->reader->readInto(pFilepath.c_str(), this->manualRoute);
	auto stretchObject = this->manualRoute["stretches"].GetObject();

	for (const auto& stretchItr : stretchObject)
	{
		std::queue<Terrain*> stretch;	
		for (const auto& terrainItr : stretchItr.value.GetArray())
		{
			Terrain * terrain = new Terrain(terrainItr);
			stretch.push(terrain);
		}
		this->stretches.push(stretch);
	}
}

void TerrainGenerator::readTerrainData(const rapidjson::Document& pConfig)
{
	for (auto const& terrain : pConfig["terrains"].GetArray())
	{
		std::string name;
		int ranges[6];
		int counter = 0;
		for (auto & attribute : terrain.GetObject())
		{
			if (attribute.value.IsString())
				name = attribute.value.GetString();
			else
			{
					ranges[counter] = attribute.value.GetArray()[0].GetInt();
					ranges[counter+1] = attribute.value.GetArray()[1].GetInt();	
					counter+=2;
			}
		}
		terrains.push_back(new TerrainPrototype(name,ranges));
	}
	std::cout << "Terrain Data Read" << std::endl;
}

void TerrainGenerator::generateRandomStretch()
{
	int stretchLength = Random::RandomRange(minStretchLength, maxStretchLength);
	int stretchCurrentDistance = 0;
	rapidjson::Document* stretch = new rapidjson::Document();
	stretch->SetArray();

	while (stretchCurrentDistance < stretchLength)
	{
		int terrainDistance = Random::RandomRange(this->minimunTerrainLength, this->maximunTerrainLength);
		int distanceLeft = this->distanceGoal - this->currentDistance;
		if ((this->currentDistance + terrainDistance) > distanceLeft)
		{
			if (distanceLeft < this->minimunTerrainLength)
			{
				producing = false;
				break;
			}
			else
				terrainDistance = distanceLeft;
		}
		int pos = Random::RandomRange(0, terrains.size());
		Terrain* terrain = terrains.at(pos)->getTerrain(this->currentDistance, terrainDistance);
		rapidjson::Value* terrainJSONObject = terrain->toJsonObject(stretch->GetAllocator());
		stretch->PushBack(*terrainJSONObject,stretch->GetAllocator());
		stretchCurrentDistance += terrainDistance;
		this->currentDistance += terrainDistance;
	}
	std::cout << "currentDistance= " << this->currentDistance << " - distanceGoal= " << this->distanceGoal << std::endl;
	this->queue->push(stretch);
}

void TerrainGenerator::generatePredefinedStretch()
{
	if (!this->stretches.empty())
	{
		std::queue<Terrain*> currentStretch = this->stretches.front();
		this->stretches.pop();
		rapidjson::Document* stretch = new rapidjson::Document();
		stretch->SetArray();

		while (!currentStretch.empty())
		{
			Terrain* terrain = currentStretch.front();
			int terrainDistance = terrain->getEndKm() - terrain->getStartKm();
			currentStretch.pop();
			rapidjson::Value* terrainJSONObject = terrain->toJsonObject(stretch->GetAllocator());
			stretch->PushBack(*terrainJSONObject,stretch->GetAllocator());
			this->currentDistance += terrainDistance;
		}
		std::cout << "currentDistance= " << this->currentDistance << " - distanceGoal= " << this->distanceGoal << std::endl;
		this->queue->push(stretch);
	}
}

void TerrainGenerator::generateStretch() 
{
	if (this->randomGeneration)
		generateRandomStretch();
	else
		generatePredefinedStretch();
}

void TerrainGenerator::generate()
{
	srand(time(0));
	while (this->currentDistance < this->distanceGoal)
	{
		this->generateStretch();
		std::this_thread::sleep_for (std::chrono::seconds(this->generationWaitTime));	// para frenar la genearacion de terrenos
	}
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
	for(int i = 0;i <queue->size();i++)
	{
		rapidjson::Document* val = queue->front();
		for (auto const& tramo : val->GetArray())
		{
			std::cout << "--------------------" << std::endl;
			int kmStart = tramo.GetObject()["KmStart"].GetInt();
			std::cout << "KmStart: " << kmStart << std::endl;
			int kmEnd = tramo.GetObject()["KmEnd"].GetInt();
			std::cout << "KmEnd: " << kmEnd << std::endl;
			int firmeza = tramo.GetObject()["Firmeza"].GetInt();
			std::cout << "Firmeza: " << firmeza << std::endl;
			int humedad = tramo.GetObject()["Humedad"].GetInt();
			std::cout << "Humedad: " << humedad << std::endl;
			int agarre = tramo.GetObject()["Agarre"].GetInt();
			std::cout << "Agarre: " << agarre << std::endl;
			std::cout << "--------------------" << std::endl;
		}
	}
}

std::vector<Terrain*> TerrainGenerator::getStretch()
{
	std::vector<Terrain*> dummy;
	return dummy;
}

void TerrainGenerator::start()
{
	this->producer = std::thread(&TerrainGenerator::generate,this);
}

void TerrainGenerator::join()
{
	this->producer.join();
}

void TerrainGenerator::pushToQueue(rapidjson::Document* value)
{
	if (producing)
	{
		queue->push(value);
	}
	else
		std::cout << "DidntPush" << std::endl;
}