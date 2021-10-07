#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include "Collatz.h"
using namespace std;

void doCalculations(int num) {
	vector<vector<int>> numArr;
	for (int i = 1; i <= num; ++i) {
		int iter = Collatz(i);
		vector<int> point = { i , iter };
		numArr.push_back(point);
	}
	Print(numArr, num);
	int selection = getChoice();
	return;
}

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

void Print(vector<vector<int>> Arr, int& choice) {
	fstream fout;
	fout.open(fileVers(choice));
	for (int i = 0; i < (int)Arr.size(); ++i) {
		fout << Arr[i][0] << " " << Arr[i][1] << endl;
	}
	fout.close();
	return;
}

string fileVers(int& choice) {
	switch(choice)
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		default:
			return "output.txt"
}

int getChoice() {
	cout << "Select a mode:	";
	
}