#pragma once

class Collatz {

public:

	Collatz();

	Collatz(long long int num_in);

	void doCalculations();

	bool isPrime(long long int testNum); // Determines if an input is prime

	long long int iterate(long long int curr); // Calculates the number of iterations

	void Print(); // Writes the output to the corresponding file

	std::string fileVers(); // Returns correct file name based on user selection

	int getChoice(); // Retrieves user input

	void isMax(std::vector<long long int>& point); // Sets and updates the maximum number of iterations

	long long int maxVal, maxIter; // Tracks maximum iteration count and index

private:

	int choice;
	long long int num;
	long long int multiples_of;
	std::vector<std::vector<long long int>> Arr;

};
