#include <iostream> //output
#include <fstream> // write to file
#include <chrono> //time
#include <atomic> //threads
#include <thread> //Need this for threads i think?
#include <math.h> // Sqrt
#include <tuple> // For multi output
#include <iomanip>
#include <vector> // i dont even know anymore
#include <algorithm> // Needed for std::sort and std::nth_element
#include <mutex>     // Needed for thread-safe access to the primes list

using namespace std;
std::vector<std::thread> threads;

// Declare Variables
int rangeInput;
int threadInput;

atomic <int> valueToCheck = 2; // Starting Value

atomic<int> finalTotal = 0; 
atomic<long long int> finalSum = 0;
float finalTime;

vector <int> finalArray; // Placeholder for array
mutex finalArray_mutex;


// File
ofstream File("Output.txt", ios::app);

// Declare Functions
void ResultsToFile(float time, int total, int sum, vector <int> array);
void ThreadSorter(int n, int threadNum);
void PrimeFinder(int n) {
	int sen = 0;
	long long int threadSum = 0;
	int threadTotal = 0;
	vector <int> threadArray;

	while(1){
		int i = valueToCheck.fetch_add(1);

		if (i > n) {
			break; // All numbers checked
		}


		for (int j = 2; j < sqrt(i); j++) { //Sqrt of i reduces time
			if (i % j == 0) {
				sen = 1;
				break;
			}
		}
		if (sen == 0) {
			threadTotal++;
			threadSum = threadSum + i;
			threadArray.push_back(i);
			//cout << i << ", ";
		}
		else {
			sen = 0;
		}
	}
	// Output
	finalTotal.fetch_add(threadTotal);
	finalSum.fetch_add(threadSum);
	lock_guard<std::mutex> lock(finalArray_mutex);
	finalArray.insert(finalArray.end(), threadArray.begin(), threadArray.end());
	//Append array to final array
}

int main() {
	// Input
	cout << "Enter Range: ";
	cin >> rangeInput;

	cout << "Enter # of Threads: ";
	cin >> threadInput;

	// DO IT
	// Start Clock
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Do function
	ThreadSorter(rangeInput, threadInput);

	// End clock
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	//Finalize Data
	finalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()/(float)1000);

	//int cutdownArray[finalArray.size()]
	//std::copy(v.begin(), v.end(), cutdownArray);
	// Print results
	ResultsToFile(finalTime, finalTotal, finalSum, finalArray);
	// Fix Array or we are fucked!

	// Makes VS happy
	return 1;
}


// Define the other functions
//======================================================

void ResultsToFile(float time, int total, int sum, vector <int> array) {
	//<execution time> <total number of primes found> <sum of all primes found> <top ten maximum primes, listed in order from lowest to highest>
	File << "Inputs: (range: " << rangeInput << " | threads: " << threadInput << ") Outputs: " << fixed << setprecision(3) << time << "s | Total #: " << total << " | Sum: " << sum << " | ";
	for (int i = 0; i < array.size(); i++) {
		File << array[i] << " ";
	}
	File << "\n";
	File.close();
}


void ThreadSorter(int n, int threadNum) {
	//Clear it out
	threads.clear();

	for (int t = 0; t < threadNum; t++) {
		// Every thread runs the same function, racing for the next number
		threads.emplace_back(std::thread(PrimeFinder, n));
	}

	// Join the threads
	for (int i = 0; i < threadNum; i++)
	{
		threads[i].join();
	}
}