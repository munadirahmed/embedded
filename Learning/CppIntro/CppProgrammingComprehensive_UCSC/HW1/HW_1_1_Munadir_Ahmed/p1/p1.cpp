/*
Author: Munadir Ahmed
Date: 4/7/18

*/

// file: p1.cpp


#include "p1.h"

// Definition of operations in p1 class

void p1::print_usa() {
	unsigned short NumOfColumn = 27;
	unsigned short NumOfRow = 10;

	unsigned short rowIdx, colnIdx;

	for (rowIdx = 0; rowIdx < NumOfRow; rowIdx++) {
		for (colnIdx = 0; colnIdx < NumOfColumn; colnIdx++) {
			if (rowIdx == 0) {
				if ((colnIdx <= 2) || (colnIdx >= 6 && colnIdx <= 8)) {  //U
					cout << "X";
				}
				else if ((colnIdx >= 11 && colnIdx <= 14) || (colnIdx == 16)) { //S
					cout << "X";
				}
				else if ((colnIdx >= 20 && colnIdx <= 22 )) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 1)
			{
				if ((colnIdx == 1) || (colnIdx == 7)) {
					cout << "X";
				}
				else if ((colnIdx == 9+1 ) || (colnIdx >= 14 + 1 && colnIdx <= 15 + 1)) { //S
					cout << "X";
				}
				else if ((colnIdx == 20 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 2)
			{
				if ((colnIdx == 1) || (colnIdx == 7 ) || (false)) {
					cout << "X";
				}
				else if ((colnIdx == 9 + 1) || (colnIdx == 15 + 1)) { //S
					cout << "X";
				}
				else if ((colnIdx == 19 + 2 || colnIdx == 21 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 3)
			{
				if ((colnIdx == 1) || (colnIdx == 7) || (false)) {
					cout << "X";
				}
				else if ((colnIdx == 9 + 1) ) { //S
					cout << "X";
				}
				else if ((colnIdx == 19 + 2 || colnIdx == 21 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 4)
			{
				if ((colnIdx == 1) || (colnIdx == 7) || (false)) {
					cout << "X";
				}
				else if ((colnIdx >= 10 + 1 && colnIdx <= 12 + 1) ) { //S
					cout << "X";
				}
				else if ((colnIdx == 18 + 2 || colnIdx == 22 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 5)
			{
				if ((colnIdx == 1) || (colnIdx == 7) || (false)) {
					cout << "X";
				}
				else if ((colnIdx >= 13 + 1 && colnIdx <= 14 + 1)) { //S
					cout << "X";
				}
				else if ((colnIdx == 18 + 2 || colnIdx == 22 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 6)
			{
				if ((colnIdx == 1) || (colnIdx == 7) || (false)) {
					cout << "X";
				}
				else if (colnIdx == 15 + 1) { //S
					cout << "X";
				}
				else if ((colnIdx >= 18 + 2 && colnIdx <= 22 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 7)
			{
				if ((colnIdx == 1) || (colnIdx == 7) || (false)) {
					cout << "X";
				}
				else if (colnIdx == 15 + 1 || colnIdx == 9 + 1) { //S
					cout << "X";
				}
				else if ((colnIdx == 17 + 2 || colnIdx == 23 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else if (rowIdx == 8)
			{
				if ((colnIdx >= 1 && colnIdx <= 2) || (colnIdx >= 6 && colnIdx <= 7)) {
					cout << "X";
				}
				else if (colnIdx == 15 + 1 || colnIdx == 9 + 1 || colnIdx == 10 + 1) { //S
					cout << "X";
				}
				else if ((colnIdx == 17 + 2 || colnIdx == 23 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
			else // rowIdx == 9
			{
				if ((colnIdx >= 3 && colnIdx <= 5) ||  (false)) {
					cout << "X";
				}
				else if (colnIdx == 9 + 1 || (colnIdx >= 11 + 1 && colnIdx <= 14 + 1)) { //S
					cout << "X";
				}
				else if ((colnIdx >= 16 + 2 && colnIdx <=18 + 2)|| (colnIdx >= 22 + 2 && colnIdx <= 24 + 2)) { //A
					cout << "X";
				}
				else {
					cout << " ";
				}
			}
		}
		cout << endl;
	}
}

void p1::print_n_n2_n3() {
	for (int i = 0; i <= 9; i++) {
		if (i == 0) {
			// print N N^2 N^3 on the first line
			cout << std::right << std::setw(1) << "n" << std::right << std::setw(2) << "   n^2" << std::right << std::setw(3) << "   n^3" << endl;
		}
		else
		{
			// print i i^2 and i^3 with right justifiction
			cout << i << "   " << std::right << std::setw(2) << pow(i, 2) << "   " << std::right << std::setw(3) << pow(i, 3) << endl;
		}
	}
}

void p1::a_power_b() {
	for (int i = 0; i <= 7; i++) {
		if (i == 0) {
			// print a b a^b on the first line
			cout << std::right << std::setw(1) << "a" << std::right << std::setw(1) << "   b" << std::setw(13) << "   a^b" << endl;
		}
		else
		{
			// print i i^2 and i^3 with right justifiction
			cout << std::right << std::setprecision(0) << std::fixed << i << "   "  << i+1 << "   " << std::setw(10)  <<  pow(i, i+1)  << endl;
		}
	}
}

void p1::two_power_n() {
	// print a b a^b on the first line
	cout << std::right << std::setw(1) << "n" << std::setw(10) << "2^n" << endl;
	for (int i = 0; i <= 20; i++) {
		
			// print i i^2 and i^3 with right justifiction
			cout << std::right << std::setprecision(0) << std::fixed << i  << std::setw(10) << pow(2, i) << endl;
		
	}
}

void p1::a1(unsigned int  aMinNum, unsigned int  aMaxNum) {
	if (aMaxNum < aMinNum) {
		cout << "Minimum Number must be less than or equal Maximum Number" << endl;
		return;
	}
	unsigned int i,j;
	for (i = aMinNum; i <= aMaxNum; i++) {
		for (j = aMinNum; j <= i; j++) {
			cout << j << " ";
		}
		cout << endl;
	}
}

void p1::a2(unsigned int  aMinNum, unsigned int  aMaxNum) {
	if (aMaxNum < aMinNum) {
		cout << "Minimum Number must be less than or equal Maximum Number" << endl;
		return;
	}
	unsigned int i, j, itertionNum, iterator;
	
	itertionNum = aMaxNum - aMinNum;
	cout << std::setw(itertionNum * 2 + 1);
	iterator = 0;
	for (i = aMinNum; i <= aMaxNum; i++) {
		
		for (j = i; j >= aMinNum; j--) {
			
			cout << j << " ";
		}
		
		cout << endl;
		iterator++;
		cout << std::setw(2*itertionNum - 2*iterator+ 1);
	}
}

void p1::a3(unsigned int  aMinNum, unsigned int  aMaxNum) {
	if (aMaxNum < aMinNum) {
		cout << "Minimum Number must be less than or equal Maximum Number" << endl;
		return;
	}
	unsigned int i, j;
	cout << std::setw(0);
	for (i = aMaxNum; i >=aMinNum ; i--) {
		for (j = aMinNum; j <= i; j++) {
			cout << j << " ";
		}
		cout << endl;
	}
}

void p1::a4(unsigned int  aMinNum, unsigned int  aMaxNum) {
	unsigned int i, j, itertionNum, iterator;

	if (aMaxNum < aMinNum) {
		cout << "Minimum Number must be less than or equal Maximum Number" << endl;
		return;
	}
	itertionNum = aMaxNum - aMinNum;
	cout << std::setw(0);
	iterator = itertionNum;
	for (i = aMaxNum; i >= aMinNum; i--) {

		for (j = aMinNum; j <= i; j++) {
			cout << j << " ";
		}

		cout << endl;
		iterator--;
		cout << std::setw(2 * itertionNum - 2 * iterator + 1);
		
	}
}