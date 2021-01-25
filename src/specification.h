#include <iostream>
#include <vector>
#include <cmath>

class Specification
{
private:
	int id;
	std::vector<int> firmness;
	std::vector<int> humidity;
	std::vector<int> grip;
	int energy;

public:
	Specification(int pId, std::vector<int>& pAttributes, int pEnergy);
	std::vector<int> getFirmness();
	std::vector<int> getHumidity();
	std::vector<int> getGrip();
	int getEnergy();
	void getClosestAttributesTo(std::vector<float>& pTerrainAttributes, std::vector<int>& pAttributes);

	int getClosestFirmness(float pValue);
	int getClosestHumidity(float pValue);
	int getClosestGrip(float pValue);

	void print();

};