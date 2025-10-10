#include <thread>   // For: Threads
#include <iostream> // For: Input/Output
#include <vector>   // For: Better Arrays
#include <mutex>    // For: Mututal Exclusion
#include <chrono>   // For: Time

int N; //Number of philosphers/threads

std::vector<std::unique_ptr<std::mutex>> chopsticks; //mutex array for chopsticks
std::vector<std::thread> threads; // array for threads

//functions
void phillospher(int i);

int main() {
	// Input
	std::cout << "Number of philosphers: ";
	// Program will break if less than 2
	std::cin >> N;

	// Set number of chopsticks
	//chopsticks.resize(N);
	for (int i = 0; i < N; ++i) {
		// make unique mutex pointer for each chopstick
		chopsticks.emplace_back(std::make_unique<std::mutex>());
	}

	// Make philosphers
	threads.clear();
	for (int t = 0; t < N; t++) {
		// Give each thread their thread index
		threads.emplace_back(std::thread(phillospher, t));
	}

	// Join the threads
	for (int i = 0; i < N; i++)
	{
		threads[i].join();
	}

	// Return success
	return 0;
}

void phillospher(int i) {
	// get index of the sticks this thread will be using
	int leftStick = i;
	int rightStick = ((i + 1) % chopsticks.size());	// Instead of having an extra if statment I can just use modulo to make the last phillospher pick up the first chopstick as his right hand one

	// set which stick is picked first
	int leftLock = std::min(leftStick, rightStick);
	int rightLock = std::max(leftStick, rightStick);

	//
	std::mutex& m1 = *chopsticks[leftLock];
	std::mutex& m2 = *chopsticks[rightLock];

	while (1) {
		// think
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 5000));

		// go eat
		std::lock(m1, m2); //attempts to grab both chopsticks at the same time, or waits until it can

		//Adopts the lock and releases when the loop restarts
		std::lock_guard<std::mutex> lock_first(m1, std::adopt_lock);
		std::lock_guard<std::mutex> lock_second(m2, std::adopt_lock);


		std::cout << "Philospher " << i + 1 << " is eating\n";

		// Spend time eating
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 5000));

		std::cout << "Philospher " << i + 1 << " has finished eating\n";
	}
}