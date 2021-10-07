#pragma once

class Collatz {

public:

	Collatz();

	Collatz(long long int num_in);

	void doCalculations();

	long long int iterate(long long int curr);

	void Print();

	std::string fileVers();

	int getChoice();

private:

	int choice;
	long long int num;
	long long int multiples_of;
	std::vector<std::vector<long long int>> Arr;

};
