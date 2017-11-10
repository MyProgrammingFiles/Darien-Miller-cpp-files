#include <iostream>
#include <cstddef>
#include <conio.h>
using namespace std;

void findNumpowers(int &numberOfpowers, int &decimal);
void initializeArrays(int *powersOf2, int *binary, int numberOfpowers);
void assignPowers(int *powerOf2, int numberOfpowers);
void findBinary(int *powersOf2, int *binary, int decimal, int numberOfpowers);
int *adjustPowersArray(int *powersOf2, int numberOfpowers);
int *adjustBinaryArray(int *binary, int numberOfpowers);
void resetArrays(int *powersOf2, int *binary);

int main()
{
	int decimal, numberOfpowers = 0;
	char choice;
	
	findNumpowers(numberOfpowers, decimal);//first step: find the number of powers of 2 in the number the user entered
	int *powersOf2 = new int [numberOfpowers];//second step: create dynamic arrays exactly as big as the number of powers in the decimal value
	int *binary = new int [numberOfpowers];
	while(decimal != -1){
		initializeArrays(powersOf2, binary, numberOfpowers);//third step: initialize each element in both arrays to 0 
		assignPowers(powersOf2, numberOfpowers);//fourth step: assign the powers of 2 to dynamic array
		findBinary(powersOf2, binary, decimal, numberOfpowers);//Lastly, use the subtraction method, by subtracting each power of two from the decimal value
		numberOfpowers = 0;
		findNumpowers(numberOfpowers, decimal);
		powersOf2 = adjustPowersArray(powersOf2, numberOfpowers);
		binary = adjustBinaryArray(binary, numberOfpowers);
	}
	
	delete [] powersOf2;
	delete [] binary;
	
	powersOf2 = NULL, binary = NULL;
	return 0;
}

int *adjustPowersArray(int *powersOf2, int numberOfpowers)
{
	int *copy = new int[numberOfpowers];
	
	for(int i = 0; i < numberOfpowers; i++)
		copy[i] = powersOf2[i];
	
	delete [] powersOf2;//de-allocate the memory used to create the old array pointed to by powersOf2
	
	powersOf2 = copy;
	
	return powersOf2;
}

int *adjustBinaryArray(int *binary, int numberOfpowers)
{
	int *copy = new int[numberOfpowers];
	
	for(int i = 0; i < numberOfpowers; i++)
		copy[i] = binary[i];
	
	delete [] binary;//de-allocate the memory used to create the old array pointed to by binary
	
	binary = copy;
	
	return binary;
}

void findNumpowers(int &numberOfpowers, int &decimal)
{
	cout << "Enter the number you wish to convert to binary, or -1 to quit: ";
	cin >> decimal;

	for(int k = 1; k <= decimal; k*=2)
		numberOfpowers++;
}

void initializeArrays(int *powersOf2, int *binary, int numberOfpowers)
{
	for(int intialize = 0; intialize < numberOfpowers; intialize++){
		powersOf2[intialize] = 0;
		binary[intialize] = 0;
	}
}

void assignPowers(int *powersOf2, int numberOfpowers)
{
	for(int i = 0, j = 1; i < numberOfpowers; i++, j*=2)
		powersOf2[i] = j;
}

void findBinary(int *powersOf2, int *binary, int decimal, int numberOfpowers)
{
	int k = 0;
	
	cout << decimal << " converted to binary is: \n"; 
	do{
		if (decimal >= powersOf2[numberOfpowers-1-k]){
			decimal -= powersOf2[numberOfpowers-1-k];
			binary[k] = 1;
		}else
			binary[k] = 0;
		k++;
	}while(decimal > 0);
	
	for(int i = 0; i < numberOfpowers; i++)
		cout << binary[i] << " ";
	cout << endl << endl;	
}
