#include <iostream>
#include <vector>

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
	void getClosestAttributesTo(std::vector<float>& pTerrainAttributes, int pResult[3]);

	int getClosestFirmness(int pValue);
	int getClosestHumidity(int pValue);
	int getClosestGrip(int pValue);

	void print();

};