#include <iostream>  // For: Input and Output 
#include <fstream>   // For: File operations
#include <chrono>    // For: Execution time
#include <atomic>    // For: Atomic opperations
#include <thread>    // For: Spawning threads
#include <math.h>    // For: Sqrt()
#include <iomanip>   // For: Decimal Precision
#include <vector>    // For: Arrays but better
#include <algorithm> // For: Needed for sort and nth_element
#include <mutex>     // For: Thread locking

// Simplification
using namespace std;
std::vector<std::thread> threads;

// Declare Variables
int rangeInput;
int threadInput;

// Starting Value
atomic <int> valueToCheck = 2; 

atomic<int> finalTotal = 0; 
atomic<unsigned long long> finalSum = 0;
float finalTime;

vector <int> finalArray;
mutex finalArray_mutex;

// File
ofstream File("primes.txt", ios::app);

// Declare Functions
void ResultsToFile(float time, int total, unsigned long long sum, vector <int> array);
vector <int> SortAndConvertTopTen(vector <int> array);
void ThreadSorter(int n, int threadNum);
void PrimeFinder(int n) {
	int sen = 0;
	unsigned long long threadSum = 0;
	int threadTotal = 0;
	vector <int> threadArray;

	while(1){
		//This thread is now checking the next number
		int i = valueToCheck.fetch_add(1);

		// Check if all numbers are already checked
		if (i > n) {
			break; 
		}

		// Check for primes
		for (int j = 2; j < sqrt(i); j++) { //Sqrt of i reduces time
			if (i % j == 0) {
				sen = 1;
				break;
			}
		}

		// If one of the values was prime
		if (sen == 0) {
			// Update thread output data
			threadTotal++;
			threadSum = threadSum + i;
			threadArray.push_back(i);
		}
		else {
			sen = 0;
		}
	}

	// Output
	finalTotal.fetch_add(threadTotal);
	finalSum.fetch_add(threadSum);

	lock_guard<std::mutex> lock(finalArray_mutex);
	finalArray.insert(finalArray.end(), threadArray.begin(), threadArray.end());//Append array to final array
}
// MAIN
int main() {
	// Input for range and thread
	cout << "Enter Range: ";
	cin >> rangeInput;

	cout << "Enter # of Threads: ";
	cin >> threadInput;

	// Start Clock
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Do function
	ThreadSorter(rangeInput, threadInput);

	// End clock
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	//Finalize Data
	finalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()/(float)1000);
	finalArray = SortAndConvertTopTen(finalArray);

	// Print results
	ResultsToFile(finalTime, finalTotal, finalSum, finalArray);

	// Makes VS happy
	return 1;
}

//======================================================
//             Define the other functions
//======================================================

void ResultsToFile(float time, int total, unsigned long long sum, vector <int> array) {
	//<execution time> <total number of primes found> <sum of all primes found> <top ten maximum primes, listed in order from lowest to highest>
	File << "Inputs: (range: " << rangeInput << " | threads: " << threadInput << ") Outputs: " << fixed << setprecision(3) << time << "s | Total #: " << total << " | Sum: " << sum << " | ";
	
	// Put in the last 10 primes
	for (int i = 0; i < array.size(); i++) {

		File << array[i] << " ";
	}
	File << "\n";
	//close the file
	File.close();
}


void ThreadSorter(int n, int threadNum) {
	//Clear out threads from previous run
	threads.clear();

	for (int t = 0; t < threadNum; t++) {
		// Every thread runs the same function and grabs the next avaliable number
		threads.emplace_back(std::thread(PrimeFinder, n));
	}

	// Join the threads
	for (int i = 0; i < threadNum; i++)
	{
		threads[i].join();
	}
}

vector <int> SortAndConvertTopTen(vector <int> array) {
	// Determine how many elements to select (K = 10, or fewer if the vector is smaller)
	const size_t K = 10;
	size_t count = std::min(K, array.size());

	//Find 10th value
	auto nth_position = array.begin() + (array.size() - count);

	// Partial sort
	std::nth_element(array.begin(), nth_position, array.end());

	//Sort rest of values
	std::sort(nth_position, array.end());


	// Init return vector and fill it with the last 10 pieces of data
	std::vector<int> top_10_sorted;
	std::copy(nth_position, array.end(), std::back_inserter(top_10_sorted));

	//return the vector
	return top_10_sorted;
}