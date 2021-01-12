#include <queue>
#include <vector>

#include "terrain.h"

class TerrainGenerator 
{
private:
	std::queue<std::vector<Terrain*>>* sharedBuffer;
	std::vector<Terrain*> terrains;

public:
	TerrainGenerator();
	void generateTerrains(int pDistance);
	std::vector<Terrain*> getStretch();
};