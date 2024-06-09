/*
Author: Munadir Ahmed
Date: 4/21/18

*/

// file: wordEqCost.cpp


#include "wordEqCost.h"

/*

 * Calculate the cost of the word (private function)
 * @param aWord - pointer to the word (1-D array of constant characters) -> prevents the content being changed from within

 * @return - returns the cost of the word that is passes

*/
double wordEqCost::_costOfWord(const char* aWord) {
	
	/* 
	Define variables to hold the cost array corresponding to a character, 
	holds the total integer value of the cost (in cents), and the total cost
	in dollar

	*/
	unsigned int costIdx = 0;
	unsigned int totalCostOfWordInt = 0;
	double totalCostOfWord = 0;

	double costs[26] = { 1,  //a
						0.01,  //b
						0.02,  //c
						0.03,  //d
						2,  //e
						0.04,  //f
						0.05,  //g
						0.06,  //h
						3,  //i
						0.07,  //j
						0.08,  //k
						0.09,  //l
						0.1,  //m
						0.11,  //n
						4,  //o
						0.12,  //p
						0.13,  //q
						0.14,  //r
						0.15,  //s
						0.16,  //t
						5,  //u
						0.17,  //v
						0.18,  //w
						0.19,   //x
						6.00,   //y
						0.20   //z
	};

	char ch = '/0';   // define temp character variable to hold each character during iteration and initialize to null

	while (*aWord) {
	// while not the null, iterate through each character in the string
		ch = *aWord;
		//cout << ch << endl;
		
		if (ch >= 'A' && ch <= 'Z') {
			ch = ch + 'a' - 'A';   // convert to lower case
		};
		// determine index in the cost array to use
		costIdx = unsigned int(ch -'a');
		//cout << costIdx << " is the cost index"<<endl;

		totalCostOfWordInt += (unsigned int)(100*costs[costIdx]);   // increment cost in cents during iteration
		aWord++;  // iterate pointer
	}

	// derive cost in dollars and return as output of function
	totalCostOfWord =  ((double) totalCostOfWordInt )/100;
	//cout << "Total Cost = $" << totalCostOfWord << endl;
	return totalCostOfWord;
}

/*

* Public function to the wordEqCost class. Prints the list of words in the dictionary that was found to be equal to the desired cost
* @param aCostToMatchInDollars - cost in dollars to be matched with

* @return - N/A

*/
void wordEqCost::printWordsEqCost(double aCostToMatchInDollars) {
	const int MAX_NUM_WORDS = 19;
	unsigned int costOfWordInCents;
	unsigned int costToMatchInCents = (unsigned int)(aCostToMatchInDollars *100);
	unsigned int numOfWordsStored = 0;
	
	double costOfWordInDollars;

	char word[128];
	char* p = &word[0];

	char listOfWord[MAX_NUM_WORDS][128] = {};  // define a list to store words in

	// an array of pointers. each element points to a 1-d array of 128 characters (i.e. each element holds the address of the 0th index 
	// of a 1-D array of characters
	char(*listOfWordPtr)[128] = listOfWord;

	ifstream in("C:\\Users\\mahmed\\Documents\\Personal\\Classes\\CppProgrammingComprehensive\\code\\objects\\wordEqual25\\dictionary.txt");

	if (!in) {
		cout << "Cannot open in.dat for reading" << endl;
	}
	while (!in.eof() && numOfWordsStored<MAX_NUM_WORDS) {
		in.getline(word, 128);
		costOfWordInDollars = _costOfWord(p);
		costOfWordInCents = (unsigned int)(costOfWordInDollars * 100);

		if (costOfWordInCents == costToMatchInCents) {
			numOfWordsStored++;
			for (int i = 0; i < 128; i++) {
				if (p[i]) {   // if not null then transfer character to 
					//cout << p[i] << endl;

					// first dereference to get the address of the 0th index of the 1-D array of characters being pointed to
					// next increment to the i th address, and store the character in this address corresponding to the ith address 
					// that is held in p (i.e. copy the character the pointer p is pointing to to this address
					*((*listOfWordPtr) + i) = p[i];
				}
				else {  // if pointing to null then break out of loop
					break;
				}

			}
			listOfWordPtr++;   // once done copying increment the pointer to 1-D array of character
			cout << numOfWordsStored << ". " << word << endl;
		}
		
	}
	return;

}