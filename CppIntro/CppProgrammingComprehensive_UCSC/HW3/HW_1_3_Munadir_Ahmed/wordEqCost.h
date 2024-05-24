/*
Author: Munadir Ahmed
Date: 4/21/18

*/

// file: wordEqCost.h


// All includes
#ifndef WORDEQCOST_H   
#define WORDEQCOST_H



#include "../util/util.h"

// Declaration of class

class wordEqCost {
public:
	void printWordsEqCost(double aCostToMatchInDollars);
private:

	double _costOfWord(const char* aWord);
};


#endif // !WORDEQCOST_H