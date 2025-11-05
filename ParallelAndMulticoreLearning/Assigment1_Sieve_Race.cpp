#include <iostream>
#include <math.h>    // For: Sqrt
#include <vector>    // For: Arrays but better
#include <thread>
#include <fstream>   // For: File operations
#include <mutex>
#include <chrono>    // For: Execution time
#include <iomanip>   // For: Decimal Precision when printing

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
void ResultsToFile(float time);

int main() {
	// Prompts
	cout << "Enter Range: ";
	cin >> rangeInput;

	cout << "Enter # of Threads: ";
	cin >> threadInput;

	//array
	compositeNumArray.resize(rangeInput+1);
	
	// Start Clock
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Do function
	ThreadSorter(rangeInput, threadInput);

	// End clock
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	// Get the time
	finalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / (float)1000);

	/*
	// Print result
	for (int i = 2; i < rangeInput+1; i++) {
		//std::cout << compositeNumArray[i] << "\n\n";

		if (compositeNumArray[i] == 0) {
			std::cout << i << " ";
		}

	}
	*/

	// Probably will have to end up making a new print function!!!!

	ResultsToFile(finalTime);

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
				//std::cout << "do multiple of " << i << ": " << i * j << '\n'; //Debug Print
				compositeNumArray[i * j] = 1; // Actual thing to use
			}
		}
	}
}

void ResultsToFile(float time) {
	unsigned long long sum = 0;
	int total = 0;

	//get sum and total (cycle through the entire array)
	for (int i = 2; i < rangeInput; i++) {
		if (compositeNumArray[i] == 0) {
			total++;
			sum = sum + i;
		}
	}

	//<execution time> <total number of primes found> <sum of all primes found> <top ten maximum primes, listed in order from lowest to highest>
	File << "Inputs: (range: " << rangeInput << " | threads: " << threadInput << ") Outputs: " << fixed << setprecision(3) << time << "s | Total #: " << total << " | Sum: " << sum << " | ";

	/*
	// Put in the last 10 primes
	for (int i = 0; i < array.size(); i++) {

		File << array[i] << " ";
	}
	*/

	File << "\n";
	//close the file
	File.close();
}