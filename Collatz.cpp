#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "Collatz.h"
using namespace std;

Collatz::Collatz() : num(1000), multiples_of(1), choice(1) { 
	doCalculations();
	Print();
}

Collatz::Collatz(long long int num_in) : num(num_in) {
	getChoice();
	doCalculations();
	Print();
}

void Collatz::doCalculations() {

	switch (choice) {
	case 1: // all
		for (long long int i = 1; i <= num; ++i) {
			long long int iter = iterate(i);
			vector<long long int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 2: // odds
		for (long long int i = 1; i <= num; i += 2) {
			long long int iter = iterate(i);
			vector<long long int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 3: // evens
		for (long long int i = 2; i <= num; i += 2) {
			long long int iter = iterate(i);
			vector<long long int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 4: // powers of 2
		for (long long int i = 1; i <= num; i += i) {
			long long int iter = iterate(i);
			vector<long long int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 5: // primes

		return;
	case 6: // multiples of <X>
		for (long long int i = multiples_of; i <= num; i += multiples_of) {
			long long int iter = iterate(i);
			vector<long long int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	default:
		return;
	}
}

long long int Collatz::iterate(long long int curr) {
	long long int numIter = 0;
	while (curr != 1) {
		numIter++;
		if (curr % 2 == 0) { // even
			curr = curr / 2;
		}
		else if (curr % 2 != 0) { // odd
			curr = 3 * curr + 1;
		}
	}
	return numIter;
}

void Collatz::Print() {
	ofstream fout;
	string fileName = fileVers();
	fout.open(fileName);
	for (long long int i = 0; i < (long long int)Arr.size(); ++i) {
		fout << Arr[i][0] << " " << Arr[i][1] << endl;
	}
	fout.close();
	return;
}

string Collatz::fileVers() {
	switch (choice) {
	case 1:
		return "output.txt";
	case 2:
		return "odds.txt";
	case 3:
		return "evens.txt";
	case 4:
		return "pow_two.txt";
	case 5:
		return "primes.txt";
	case 6:
		return "multiples_of_" + to_string(multiples_of) + ".txt";
	default:
		return "output.txt";
	}
}

int Collatz::getChoice() {
	cout << "Select a mode:	";
	cin >> choice;
	cout << endl;
	if (choice == 6) {
		cout << "Which integer multiples?	";
		cin >> multiples_of;
	}
	cout << endl;
	return choice;
}