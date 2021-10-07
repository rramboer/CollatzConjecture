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


int main() {
	fstream fout;
	fout.open("output.txt");
	vector<vector<int>> numArr;
	int initNum;
	cout << "Enter the upper bound:   ";
	cin >> initNum;
	for (int i = 1; i <= initNum; ++i) {
		int iter = Collatz(i);
		vector<int> point = { i , iter };
		numArr.push_back(point);
	}
	for (int i = 0; i < (int)numArr.size(); ++i) {
		fout << numArr[i][0] << " " << numArr[i][1] << endl;
	}
	cout << "Calculations completed. Results are in 'output.txt'." << endl;
	fout.close();
	return 0;
}
