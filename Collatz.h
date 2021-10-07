#pragma once

class Collatz {
public:

	Collatz();

	Collatz(int num_in);

	void doCalculations();

	int iterate(int curr);

	void Print();

	std::string fileVers();

	int getChoice();

private:

	int choice;
	int num;
	int multiples_of;
	std::vector<std::vector<int>> Arr;

};
