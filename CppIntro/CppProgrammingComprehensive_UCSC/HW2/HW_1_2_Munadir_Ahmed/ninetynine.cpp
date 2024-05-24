/*
Author: Munadir Ahmed
Date: 4/13/18

*/

// file: ninetynine.cpp


#include "ninetynine.h"

// Definition of operations in p1 class

void ninetynine::ninetyninebottles() {
	int i = 0;

	// store the constant parts of the song in an array
	const char* constantLines[] = { " of beer on the wall", " of beer", "Take one down, pass it around", " of beer on the wall "};
	const unsigned short numOfLines = sizeof(constantLines) / sizeof(const char*);   // determine number of lines in the song
	
	// store the constant parts of the song in an array
	const char* grammer[] = { "bottle","bottles" };

	const char** grammerThisRoundPtr = &grammer[0];  // pointer 1-D array holding the grammer text for this round of the song
	const char** grammerNextRoundPtr = &grammer[0];  // pointer 1-D array holding the grammer text for this round of the song

	// store the first  ans second/only part of the numbers
	const char* firstNum[] = { "N/A", "N/A", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety" }; 
	const char* scndOrOnlyNum[] = { "zero", "one","two", "three", "four", "five", "six", "seven", "eight", "nine", "ten","eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};

	const char** firstNumPtr = &firstNum[0];  // pointer 1-D array holding the first number text for the song
	const char** scndOrOnlyNumPtr = &scndOrOnlyNum[0];  // pointer 1-D array holding the first number text for the song
	
	unsigned short NumOfBottlesOnWall = 99;   // number of bottles at beginning of the song

	while(NumOfBottlesOnWall >0) {   // interate through number of bottles
		for (int i = 0; i < numOfLines; i++) {   // iterate through lines of the song
			unsigned short quotient = NumOfBottlesOnWall / 10; // determine first part number
			unsigned short remainder = NumOfBottlesOnWall - quotient * 10;   // determine second part of the number

			firstNumPtr = &firstNum[quotient];   // get the text for te first part of the number
			
			// second part only exists for numbers greater than 19
			if (NumOfBottlesOnWall >= 20) {
				scndOrOnlyNumPtr = &scndOrOnlyNum[remainder];
			}
			else {   // below 20, there is only one part
				scndOrOnlyNumPtr = &scndOrOnlyNum[NumOfBottlesOnWall];
			}
			// Pick the right grammer for this round and next round
			if (NumOfBottlesOnWall == 1) {
				grammerThisRoundPtr = grammer;
			}
			else {
				grammerThisRoundPtr = &grammer[1];
			}

			if (NumOfBottlesOnWall == 2 || NumOfBottlesOnWall == 1) {
				grammerNextRoundPtr = grammer;
			}
			else {
				grammerNextRoundPtr = &grammer[1];
			}
		
		
			if (i == 2) {
				// print nothing in front of the 3rd line (only print the 3rd line by itself)
			}
			else {
				// else print number text in front for numbers above 19
				if (NumOfBottlesOnWall >= 20) {
					cout << *firstNumPtr <<" " ;
				}
				if (remainder != 0 || NumOfBottlesOnWall < 20) {
					cout << *scndOrOnlyNumPtr << " ";
				}
				else {
					// if the second number is zero, don't print anything
				}

				if (i == (numOfLines - 1)) {   // on last line
					cout << *grammerNextRoundPtr;
				}
				else if (i != 2) {
					cout << *grammerThisRoundPtr;
				}
				else {
					// do nothingnothing
				}
			}
			if (i == 2) {
				NumOfBottlesOnWall--;
			}
			cout << constantLines[i] << endl;
		}
		cout << endl;
	}

}