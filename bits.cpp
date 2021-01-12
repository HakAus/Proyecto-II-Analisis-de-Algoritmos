// #include <iostream>
// #include <ctime>
// #include <cstdlib>
// #include <cmath>

// const unsigned short mask1 = 0b1110000000000000;
// const unsigned short mask2 = 0b0001111100000000;
// const unsigned short mask3 = 0b0000000011100000;
// const unsigned short mask4 = 0b0000000000011111;

// const double CHROMOSOME_RANGE_SIZE = 36.45;

// using namespace std;

// void showByte(char c)
// {
// 	for (int i = 7; i >=0; i--)
// 	{
// 		putchar(c & (1u << i) ? '1' : '0');
// 	}
// 	printf("\n");
// }

// void showbits( unsigned short x )
// {
//     int i=0;
//     for (i = (sizeof(short) * 8) - 1; i >= 0; i--)
//     {
//        putchar(x & (1u << i) ? '1' : '0');
//     }
//     printf("\n");
// }

// unsigned short mutation(unsigned short chromosome)
// {
// 	unsigned short pos = rand() % 16;
// 	unsigned short mask = 1 << pos; 
// 	bool has_bit = chromosome & mask;
//     return (chromosome & ~mask) | ((!has_bit << pos) & mask); 
// }

// unsigned short crossover(unsigned short parent1, unsigned short parent2)
// {
// 	unsigned short base = 0b0000000000000000;
// 	unsigned short tmp = 0b0000000000000000;
// 	unsigned short result = 0b0000000000000000;

// 	tmp = parent1 & mask1;
// 	result = result | tmp;
// 	tmp = base;

// 	tmp = parent2 & mask2;
// 	result = result | tmp;
// 	tmp = base;

// 	tmp = parent2 & mask3;
// 	result = result | tmp;
// 	tmp = base;

// 	tmp = parent1 & mask4;
// 	result = result | tmp;

// 	return result;
// }

// int getFold(unsigned short chromosome)
// {
// 	unsigned char c = 0;
// 	c = c | chromosome;
// 	cout << "Byte del pliegue" << endl;
// 	showByte(c);
// 	printf("Byte en decimal %d\n", c);
// 	double num = c;
// 	return 11 - ceil(num/CHROMOSOME_RANGE_SIZE);
// }

// int getTorque (unsigned short chromosome)
// {
// 	chromosome = chromosome >> 8;
// 	unsigned char c = 0;
// 	c = c | chromosome;
// 	cout << "Byte del torque" << endl;
// 	showByte(c);
// 	printf("Byte en decimal %d\n", c);
// 	double num = c;
// 	return ceil(num/CHROMOSOME_RANGE_SIZE);	// ese numero magico 36.45 es para que al redondear los numeros si del rango si den el tipo de pliegue o torque correcto.
// }


// int main()
// {
// 	srand(time(NULL));

// 	unsigned short parent1 = 0b1011011010101011;
// 	unsigned short parent2 = 0b0110101101100000;

// 	cout << "Padre 1" << endl;
// 	showbits(parent1);
// 	// cout << "Cruce original" << endl;
// 	// unsigned short result = crossover(parent1,parent2);
// 	// showbits(result);
// 	// cout << "Cruce con mutacion" << endl;
// 	// showbits(mutation(result));

// 	cout << getFold(parent1) << endl;
// 	cout << getTorque(parent1) << endl;
// 	return 0;
// }