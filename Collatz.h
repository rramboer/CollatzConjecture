#pragma once

class Collatz {

public:

	Collatz();

	Collatz(long long int num_in);

	void doCalculations();

	bool isPrime(long long int testNum);

	long long int iterate(long long int curr);

	void Print();

	std::string fileVers();

	int getChoice();

	void isMax(std::vector<long long int>& point);

	long long int maxVal, maxIter; // track maximum iteration count and index

private:

	int choice;
	long long int num;
	long long int multiples_of;
	std::vector<std::vector<long long int>> Arr;
	

};
