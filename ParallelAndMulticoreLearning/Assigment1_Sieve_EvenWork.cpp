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
void PrimeFinder(int start, int end);
void ResultsToFile(float time);

int main() {
	// Prompts
	cout << "Enter Range: ";
	cin >> rangeInput;

	cout << "Enter # of Threads: ";
	cin >> threadInput;

	// Set up array
	compositeNumArray.resize(rangeInput + 1);

	// Start Clock
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Do function
	ThreadSorter(rangeInput, threadInput);

	// End clock
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	// Get the time
	finalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / (float)1000);

	// Print results
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

	int start = 2;
	int size = ((sqrt(n+1)))/threadNum-2;

	for (int t = 0; t < threadNum; t++) {
		if (t == (threadNum -1)) {
			threads.emplace_back(thread(PrimeFinder, start, sqrt(n)));
			break;
		}
		// Every thread runs the same function and grabs the next avaliable number
		threads.emplace_back(thread(PrimeFinder, start, start + size));
		start = start + size;
		// if last one then go up to sqrt n
	}

	// Join the threads
	for (int i = 0; i < threadNum; i++)
	{
		threads[i].join();
	}
}

void PrimeFinder(int start, int end) {
	for (int i = start; i < end+1; i ++) {
		// Add another if statement containing the rest checking if the array[i] is 0 <- This makes sure we don't do unneeded checks
		if (compositeNumArray[i] == 0) { // Wait I don't even need isPrime(i)
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
	for (int i = 2; i < rangeInput+1; i++) {
		if (compositeNumArray[i] == 0) {
			//DEBUG
			//cout << compositeNumArray[i] << '\n';
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