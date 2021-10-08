/*
	Collatz Conjecture

	The Collatz conjecture is a conjecture in mathematics that concerns sequences defined as follows: 
	start with any positive integer n. Then each term is obtained from the previous term as follows: 
	if the previous term is even, the next term is one half of the previous term. 
	If the previous term is odd, the next term is 3 times the previous term plus 1. 
	The conjecture is that no matter what value of n, the sequence will always reach 1.

	(https://en.wikipedia.org/wiki/Collatz_conjecture)


	"output.txt" contains the list of resulting iterations in the format:

	STARTING NUMBER  -  NUMBER OF ITERATIONS BEFORE REACHING 1
*/

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "Collatz.h"
using namespace std;

void printMenu() {
	cout << "Options:\n" << "1. All\n" << "2. Odds\n" << "3. Evens\n"
		<< "4. Powers of Two\n" << "5. Primes\n" << "6. Multiples of <X>" << endl;
	return;
}

int main() {
	long long int initNum;
	cout << "Enter the upper bound between 1 and 9,223,372,036,854,775,807:   ";
	cin >> initNum;
	printMenu();
	Collatz c(initNum);

	cout << "Maximum iteration count " << c.maxIter << " at index " << c.maxVal << "." << endl;

	cout << "\nCalculations completed. Results are in the corresponding '*txt' file." << endl;

	return 0;
}
