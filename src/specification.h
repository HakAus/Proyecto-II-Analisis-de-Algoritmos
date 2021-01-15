
class Specification
{
private:
	int id;
	int firmness[2];
	int humidity[2];
	int grip[2];
	int energy;

public:
	Specification(int pId, int pAttributes[2], int pEnergy);
	int* getFirmness();
	int* getHumidity();
	int* getGrip();

};