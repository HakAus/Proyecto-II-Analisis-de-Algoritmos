#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

const unsigned short m1 = 0b1110000000000000;
const unsigned short m2 = 0b0001111100000000;
const unsigned short m3 = 0b0000000011100000;
const unsigned short m4 = 0b0000000000011111;

using namespace std;

void showbits( unsigned short x )
{
    int i=0;
    for (i = (sizeof(short) * 8) - 1; i >= 0; i--)
    {
       putchar(x & (1u << i) ? '1' : '0');
    }
    printf("\n");
}

unsigned short mutation(unsigned short gene)
{
	unsigned short pos = rand() % 16;
	unsigned short mask = 1 << pos; 
	unsigned short has_bit = gene & mask;
	has_bit = has_bit > 0;
    return (gene & ~mask) | ((!has_bit << pos) & mask); 
}

int main()
{
	srand(time(NULL));

	unsigned short p1 = 0b1010000011100110;
	unsigned short p2 = 0b0110101101100000;
	unsigned short base = 0b0000000000000000;
	unsigned short tmp = 0b0000000000000000;
	unsigned short result = 0b0000000000000000;

	tmp = p1 & m1;
	result = result | tmp;
	tmp = base;

	tmp = p2 & m2;
	result = result | tmp;
	tmp = base;

	tmp = p2 & m3;
	result = result | tmp;
	tmp = base;

	tmp = p1 & m4;
	result = result | tmp;

	cout << "Original" << endl;
	showbits(result);
	cout << "Mutacion" << endl;
	showbits(mutation(result));

	return 0;
}