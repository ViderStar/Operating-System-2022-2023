#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
	int i;

	cout << "I am created" << endl;
	int numberToFind = atoi(argv[0]);
	int size = atoi(argv[1]);
	cout << "Number to find is " << numberToFind << endl;
	cout << "Size of array is " << size << endl;

	int* massive = new int[size];
	int counter = 0;
	for (i = 2; i < argc; i++) {
		massive[counter] = atoi(argv[i]);
		counter += 1;
	}

	int swapCounter = 0;
	for (int i = 0; i < size; i++) {
		if (massive[i] == numberToFind) {
			massive[i] = massive[swapCounter];
			massive[swapCounter] = numberToFind;
			swapCounter += 1;
		}
	}

	for (int i = 0; i < size; i++) {
		cout << massive[i] << " ";
	}
	cout << endl;

	system("pause");
	delete[]massive;
	return 0;
}