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

float finalTime;

vector <int> finalArray;
mutex finalArray_mutex;

// File
ofstream File("primes.txt", ios::app);

// Prototypes
bool isPrime(int number);
void ThreadSorter(int n, int threadNum);
void PrimeFinder(int n);

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
	/*
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
	*/
	ThreadSorter(rangeInput, 16);

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


void ThreadSorter(int n, int threadNum) {
	//Clear out threads from previous run
	threads.clear();

	for (int t = 0; t < threadNum; t++) {
		// Every thread runs the same function and grabs the next avaliable number
		threads.emplace_back(thread(PrimeFinder, n));
	}

	// Join the threads
	for (int i = 0; i < threadNum; i++)
	{
		threads[i].join();
	}
}

void PrimeFinder(int n) {
	unsigned long long threadSum = 0;
	int threadTotal = 0;
	vector <int> threadArray;

	while (1) {
		//This thread is now checking the next number
		int i = valueToCheck.fetch_add(1);

		// Check if all numbers are already checked
		if (i > sqrt(n)) {
			break;
		}

		// Add another if statement containing the rest checking if the array[i] is 0 <- This makes sure we don't do unneeded checks
		if (isPrime(i) && compositeNumArray[i] == 0) {
			// Update allllll of the multiples of the number up to n
			for (int j = i; i * j < (rangeInput + 1); j++) {
				std::cout << "do multiple of " << i << ": " << i * j << '\n'; //Debug Print
				compositeNumArray[i * j] = 1; // Actual thing to use
			}
		}
	}
}