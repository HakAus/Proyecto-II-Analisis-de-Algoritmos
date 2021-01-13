
class Specification
{
private:
	int firmness[2];
	int humidity[2];
	int grip[2];
	int energy;

public:
	Specification(int pFirmness[2], int pHumidity[2], int pGrip[2], int pEnergy);
	int* getFirmness();
	int* getHumidity();
	int* getGrip();

};