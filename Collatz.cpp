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

Collatz::Collatz(int num_in) : num(num_in) {
	getChoice();
	doCalculations();
	Print();
}

void Collatz::doCalculations() {

	switch (choice) {
	case 1: // all
		for (int i = 1; i <= num; ++i) {
			int iter = iterate(i);
			vector<int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 2: // odds
		for (int i = 1; i <= num; i += 2) {
			int iter = iterate(i);
			vector<int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 3: // evens
		for (int i = 2; i <= num; i += 2) {
			int iter = iterate(i);
			vector<int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 4: // powers of 2
		for (int i = 1; i <= num; i += i) {
			int iter = iterate(i);
			vector<int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	case 5: // primes

		return;
	case 6: // multiples of <X>
		for (int i = multiples_of; i <= num; i += multiples_of) {
			int iter = iterate(i);
			vector<int> point = { i , iter };
			Arr.push_back(point);
		}
		return;
	default:
		return;
	}
}

int Collatz::iterate(int curr) {
	int numIter = 0;
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
	fstream fout;
	string fileName = fileVers();
	fout.open("output.txt");
	for (int i = 0; i < (int)Arr.size(); ++i) {
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