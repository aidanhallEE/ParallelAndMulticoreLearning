#include <iostream>
#include <math.h> // For: Sqrt
#include <vector>    // For: Arrays but better
#include <thread>
#include <fstream>   // For: File operations
#include <mutex>

// Simplification
using namespace std;
std::vector<std::thread> threads;

// Declare Variables
int rangeInput;
int threadInput;

// Starting Value
vector <bool> compositeNumArray;

atomic <int> valueToCheck = 2;

atomic<int> finalTotal = 0;
atomic<unsigned long long> finalSum = 0;
float finalTime;

vector <int> finalArray;
mutex finalArray_mutex;

// File
ofstream File("primes.txt", ios::app);

// Prototypes
bool isPrime(int number);

int main() {
	// Define Inputs
	int nInput;
	int threadInput;

	// Prompts
	std::cout << "n: ";
	std::cin >> rangeInput;

	//array
	compositeNumArray.resize(rangeInput+1);

	// Going Up to sqrt(n), check each number that hasn't been marked prime yet and update all it's multiples

	for (int i = 2; i < sqrt(rangeInput); i++) {
		// Add another if statement containing the rest checking if the array[i] is 0 <- This makes sure we don't do unneeded checks
		if (isPrime(i) && compositeNumArray[i] == 0) {
		// Update allllll of the multiples of the number up to n
			for (int j = i; i*j < (rangeInput + 1); j++) {
				std::cout << "do multiple of " << i << ": " << i*j << '\n'; //Debug Print
				compositeNumArray[i * j] = 1; // Actual thing to use
			}
		}
	}

	for (int i = 2; i < rangeInput+1; i++) {
		//std::cout << compositeNumArray[i] << "\n\n";

		if (compositeNumArray[i] == 0) {
			std::cout << i << " ";
		}

	}

	// Probably will have to end up making a new print!!!!



	return -1;
}

bool isPrime(int number) {
	bool prime = 1;

	for (int i = 2; i < number; i++) {
		if ((number % i) == 0) {
			prime = 0;
		}
	}

	return prime;
}