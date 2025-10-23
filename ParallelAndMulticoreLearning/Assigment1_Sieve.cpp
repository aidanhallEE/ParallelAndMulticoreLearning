#include <iostream>

// Prototypes
bool isPrime(int number);

int main() {
	// Define Inputs
	int nInput;
	int threadInput;

	// Prompts
	std::cout << "n: ";
	std::cin >> nInput;

	// test output
	std::cout << isPrime(nInput);

	// Make array size of input
	// Going Up to sqrt(n), check each number that hasn't been marked prime yet and update all it's multiples

	for (int i = 2; i < nInput; i++) {
		//if (isPrime(nInput)){
		// Update allllll of the multiples of the number up to n
		// }
	}

	// Probably will have to end up making a new


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