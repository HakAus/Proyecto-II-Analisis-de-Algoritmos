#include <queue>
#include <vector>
#include "libs/rapidjson/document.h"
#include "terrain.h"
#include <iostream>
#include <string>

class TerrainGenerator 
{
private:
	std::queue<std::vector<Terrain*>>* sharedBuffer;
	std::vector<Terrain*> terrains;

public:
	TerrainGenerator(const rapidjson::Document& pConfig);
	void generateTerrains(int pDistance);
	//Segun una distancia se va a sacar el numero de terrenos
	//Esos terrenos se almacenan en listas
	//Los terrenos tienen que ser generados a partir de los archivos de configuracion
	std::vector<Terrain*> getStretch();
};