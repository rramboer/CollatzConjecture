#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "Collatz.h"
using namespace std;

int Collatz(int num) {
	int numIter = 0;
	while (num != 1) {
		numIter++;
		if (num % 2 == 0) { // even
			num = num / 2;
		}
		else if (num % 2 != 0) { // odd
			num = 3 * num + 1;
		}
	}
	return numIter;
}

