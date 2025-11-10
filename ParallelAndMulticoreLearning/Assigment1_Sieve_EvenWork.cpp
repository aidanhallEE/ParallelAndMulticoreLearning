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
vector <int> rootPrimes;

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
void RootPrimeFinder(int n);
void ResultsToFile(float time);

int main() {
	// Prompts
	cout << "Enter Range: ";
	cin >> rangeInput;

	cout << "Enter # of Threads: ";
	cin >> threadInput;

	// Set up array
	compositeNumArray.resize(rangeInput + 1);

	// Get primes up to sqrt n
	RootPrimeFinder(rangeInput);

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
	int size = (n/threadNum)-2;

	for (int t = 0; t < threadNum; t++) {
		if (t == (threadNum - 1)) {
			threads.emplace_back(thread(PrimeFinder, start, n));
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
	// Make local vector
	int segmentSize = end - start;
	vector <bool> localNumArray(segmentSize + 1, 0);
	vector <int> localintArray(0);

	int startMultiplier = 0;
	int prime = 0;

	for (int p = 0; p < size(rootPrimes); p++) { // USE THIS FOR LOOP TO INSTEAD CYCLE THROUGH THE FOUND PRIMES
		prime = rootPrimes[p];

		startMultiplier = start / prime + start % prime; // Get the starting value to be multiplied by the prime number
		if (startMultiplier < 2) {
			startMultiplier = start;
		}
		//cout << "Inside the for loop\n";

		// Set all the multiples
		for (int j = startMultiplier; prime * (j) < (end + 1); j++) {
			//cout << "Inside the second for loop\n";
			//cout << "Position of multiplied: " << prime * j - start << " j: " << j << "\n";
			localNumArray[(prime * j)-start] = 1; // Fix this to fit inside the bounds
		}
	}
	//cout << "After thread prime finder\n";

	//Collect the results
	// how to do......
	// Array for numbers
	for (int i = 0; i < segmentSize + 1; i++) { //ADD MUTEX TO MAKE IT WORKKK!!
		if (localNumArray[i] == 0) {
			localintArray.push_back(i + start);
		}
	}

	// Add too final
	lock_guard<std::mutex> lock(finalArray_mutex);
	finalArray.insert(finalArray.end(), localintArray.begin(), localintArray.end());
}

void RootPrimeFinder(int n) { // n is the sqrt of the range
	// make local array
	vector <bool> localNumArray(sqrt(n)+1, 0);

	// prime sieve
	for (int i = 2; i < sqrt(n); i++) {
		if (localNumArray[i] == 0) { 
			// Update allllll of the multiples of the number up to n
			for (int j = i; i * j < sqrt(n); j++) {
				//std::cout << "do multiple of " << i << ": " << i * j << '\n'; //Debug Print
				localNumArray[i * j] = 1; // Actual thing to use
			}
		}
	}

	cout << "After Root Primes\n";

	// Add it to the int list
	for (int i = 2; i < sqrt(n); i++) {
		if (localNumArray[i] == false) {
			cout << i << "\n";
			rootPrimes.push_back(i);
		}
	}
	cout << "After adding root primes to list\n";
}


void ResultsToFile(float time) {
	unsigned long long sum = 0;
	int total = 0;

	//get sum and total (cycle through the entire array)
	
	for (int i = 0; i < finalArray.size(); i++) {
			//DEBUG
			cout << "num lol " << finalArray[i] << '\n';
			total++;
			sum = sum + finalArray[i];
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