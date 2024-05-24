/*
Author: Munadir Ahmed
Date: 5/18/18

*/


#include "dealOrNoDeal.h"

// Suitcase Class: Function Definition//
/*
Constructor of suitcase class
@param aAmount - amount in suitcase

@return - N/A

*/
suitcase::suitcase(unsigned int aAmount) : _amount(aAmount) {
	if (SHOW) {
		cout << "in constructor of suitcase class" << endl;
	}
};

/*
Destructor of suitcase class
@param - N/A

@return - N/A

*/
suitcase::~suitcase() {
	if (SHOW) {
		cout << "in destructor of suitcase class" << endl;
	}
	_release();
};

/*
Copy constructor of suitcase class
@param - N/A

@return - N/A

*/
suitcase::suitcase(const suitcase &rhs) {
	if (SHOW) {
		cout << "in copy constructor of suitcase class" << endl;
	}
	_copy(rhs);
};

/*
Equal Operator overloading of class suitcase
@param rhs - constant reference to object this object is being referred to

@return - reference to suitcase object (if returned by value the copy constructor will be called)

*/
suitcase& suitcase::operator= (const suitcase &rhs) {
	if (SHOW) {
		cout << "in equal operator of suitcase class" << endl;
	}
	if (this != &rhs) {
		// only if the address of THIS object isn't equal to the address of the object on the RHS
		_release();
		_copy(rhs);
	}

	return *this;

};

/*
prints the remaining cases in list
@param o - reference to ostream object
@param a - reference to suitcase object

@return - N/A

*/
ostream& operator<<(ostream& o, const suitcase& a) {

	o << a._amount << endl;

	return o;


};

/*
Public function that returns the amount heald in the suitcase
@param - N/A

@return - amount held in the suitcase

*/
unsigned int suitcase::getAmount() {
	return _amount; 
};

/*
Public function that allows external classes to set the amout after construction
@param aNewAmount - new amount we want to set in the suitcase

@return - N/A

*/
void suitcase::setAmount(unsigned int aNewAmount) {
	_amount = aNewAmount;
};

//Private Functions
/*
 function that returns the amount heald in the suitcase
@param - N/A

@return - N/A

*/
void suitcase::_release() {
	if (SHOW) {
		cout << " in release helper function" << endl;
	}
	// nothing to do here
};


/*
helper function that copies the object on the left to THIS object
@param aObjToCopyFrom - constant reference to object that copies the features of the object to THIS object without chaging the object.

@return - N/A

*/
void suitcase::_copy(const suitcase& aObjToCopyFrom) {
	if (SHOW) {
		cout << " in copy helper function" << endl;
	}

	_amount = aObjToCopyFrom._amount;   // class writer has access to private members of the class (even of other objects of the class)
};

// SuitcaseCollection Class: Function Definition//
/*
Constructor of suitcase collection class
@param aNumOfCasesInColn - total number of cases in collection

@return - N/A

*/
suitcaseCollection::suitcaseCollection(unsigned int aNumOfCasesInColn):_listOfCases(nullptr), _numOfCasesLeft(aNumOfCasesInColn), _listOfSuitcaseNumbers(nullptr),_originalNumOfCases(aNumOfCasesInColn) {

	if (SHOW) {
		cout << "in constructor of suitcase collection class" << endl;
	}
	if (aNumOfCasesInColn > 0) {
		_listOfCases = new suitcase[aNumOfCasesInColn];
		_listOfSuitcaseNumbers = new unsigned int[aNumOfCasesInColn];
	}

	unsigned int tempAmt = 0;
	suitcase tempSuitCase(0);
	for (unsigned int i = 0; i < aNumOfCasesInColn; i++) {
		_listOfSuitcaseNumbers[i] = 0;
		_listOfCases[i] = (tempSuitCase);
	}
};

/*
Destructor of suitcase collection class
@param - N/A

@return - N/A

*/
suitcaseCollection::~suitcaseCollection() {
	if (SHOW) {
		cout << "in destructor of suitcase collection class" << endl;
	}
	_release();
};

// Public Functions
/*
Public function that returns the amount heald in the suitcase
@param aSuitcaseNumber - suitcase number of interest

@return - amount held in the suitcase

*/
unsigned int suitcaseCollection::getAmountInCase(unsigned int aSuitcaseNumber) {
	unsigned int tempIdx = 0;
	bool inList = _caseInList(tempIdx, aSuitcaseNumber);

	if (!inList) {
		cout << "case selected not in list" << endl;
		return 0;
	}

	suitcase tempCase;
	tempCase = _listOfCases[aSuitcaseNumber-1];
	return tempCase.getAmount();
};

/*
Public function that resets the collection to the original length
@param - N/A

@return - N/A

*/
void suitcaseCollection::resetAllCases() {
	_numOfCasesLeft = _originalNumOfCases;
	
	return;
};

/*
Public function that returns the amount heald in the suitcase
@param N/A

@return - average amount held in the remaining suitcases

*/
unsigned int suitcaseCollection::getAverage() {
	unsigned int sum = 0;

	if (_numOfCasesLeft < 1) {
		return sum;
	}

	for (unsigned int i = 0; i < _numOfCasesLeft; i++) {
		sum += _listOfCases[i].getAmount();
	}

	return (sum/ _numOfCasesLeft);
	
};

/*
Public function that sets the amount heald in the suitcase and the case number
@param aAmntToSet - amount to set in the suitcase
@param aSuitcaseNumber - suitcase number of interest

@return - amount held in the suitcase

*/
void suitcaseCollection::setCaseAmntAndNum(unsigned int aAmntToSet, unsigned int aSuitcaseNumber) {
	unsigned int tempIdx = 0;

	if (aSuitcaseNumber<1 || aSuitcaseNumber > TOTAL_CASE_NUM) {
		cout << "number of cases must be within 1 to TOTAL_CASE_NUM" << endl;
		return;
	}
	_listOfSuitcaseNumbers[aSuitcaseNumber-1] = aSuitcaseNumber;
	_listOfCases[aSuitcaseNumber-1].setAmount(aAmntToSet);
};

/*
Public function that allows external classes to set the amout after construction
@param aSuitcaseNumber - case number to be removed

@return - N/A

*/
void suitcaseCollection::removeSuitcase(unsigned int aSuitcaseNumber) {
	unsigned int tempIdx = 0;
	bool inList = _caseInList(tempIdx, aSuitcaseNumber);

	if (!inList) {
		cout << "case selected not in list" << endl;
		return;
	}

	_numOfCasesLeft--;
	unsigned int tempArray[TOTAL_CASE_NUM] = { 0 };

	for (unsigned int i = 0; i < _numOfCasesLeft; i++) {
		if (i < tempIdx) {
			tempArray[i] = _listOfSuitcaseNumbers[i];
		}
		else {
			tempArray[i] = _listOfSuitcaseNumbers[i+1];
		}
	}

	for (unsigned int i = 0; i < _numOfCasesLeft; i++) {
		_listOfSuitcaseNumbers[i] = tempArray[i];
	}

	for (unsigned int i = _numOfCasesLeft; i < TOTAL_CASE_NUM; i++) {
		_listOfSuitcaseNumbers[i] = 0;
	}
};

/*
function that checks to see if case number is in the current list of cases available
@param aSuitcaseNumber - case number to be checked

@return - N/A

*/
bool suitcaseCollection::selectedCaseInList(unsigned int aSuitcaseNumber) {
	unsigned int tempIdx = 0;
	bool inList = false;
	inList = _caseInList(tempIdx, aSuitcaseNumber);

	return inList;


};

/*
prints the remaining cases in list
@param aSuitcaseNumber - case number to be checked

@return - N/A

*/
ostream& operator<<(ostream& o, const suitcaseCollection& a) {
	
	for (unsigned int i = 0; i < a._numOfCasesLeft; i++) {
		o << a._listOfSuitcaseNumbers[i] << " ";
	}
	o << endl;
	return o;


};

/*
function that returns the number of cases left in the game
@param N/A

@return - N/A

*/
unsigned int suitcaseCollection::getNumOfCasesLeft() {
	
	return _numOfCasesLeft;


};

/*
function that returns a random case number from those left in the game
@param N/A

@return - N/A

*/
unsigned int suitcaseCollection::genRandCaseNum() {

	Random a;
	unsigned int idxNum = a.get_random_number(1, _numOfCasesLeft);
	unsigned int caseNum = _listOfSuitcaseNumbers[idxNum];

	return caseNum;


};


//Private Functions
/*
function that returns the amount heald in the suitcase
@param - N/A

@return - N/A

*/
void suitcaseCollection::_release() {
	if (SHOW) {
		cout << " in suitcase collection class release helper function" << endl;
	}
	delete[] _listOfCases;
	delete[] _listOfSuitcaseNumbers;
};

/*
function that checks to see if case number is in the current list of cases available
@param indxOfCase - returns the index number if case is in list and 0 otherwise
@param aSuitcaseNumber - case number to be checked

@return - N/A

*/
bool suitcaseCollection::_caseInList(unsigned int& indxOfCase, unsigned int aSuitcaseNumber) {
	unsigned int tempIdx = 0;
	bool inList = false;
	for (unsigned int i = 0; i < _numOfCasesLeft; i++) {
		if (_listOfSuitcaseNumbers[i] == aSuitcaseNumber) {
			tempIdx = i;
			inList = true;
			break;
		}
	}
	indxOfCase = tempIdx;

	return inList;


};

// Deal Or No Deal Class: Function Definition//
/*
Constructor of suitcase collection class
@param aTypeOfGame - type of game being played

@return - N/A

*/
dealOrNoDeal::dealOrNoDeal(typeOfGame aTypeOfGame):_userSelectedCaseNum(0),_userSuitcase(0), _typeOfGame(aTypeOfGame), _numOfTurnsPlayed(0), _numberOfGamesPlayed(0){

	if (SHOW) {
		cout << "in constructor of deal or no deal class" << endl;
	}
	
	_initialize();

	_play();
};

/*
Destructor of dealOrNoDeal class
@param - N/A

@return - N/A

*/
dealOrNoDeal::~dealOrNoDeal() {
	if (SHOW) {
		cout << "in destructor of dealOrNoDeal class" << endl;
	}
	_release();
};

void dealOrNoDeal::_randomizePrices(unsigned int arrayOfPrices[], unsigned int size) {
	Random rndNum;

	for (unsigned int i = 0; i < 1000; i++) {
		unsigned int tempRndNumA = rndNum.get_random_number(0, size);
		unsigned int tempRndNumB = rndNum.get_random_number(0, size);

		unsigned int tempPrice = arrayOfPrices[tempRndNumA];
		arrayOfPrices[tempRndNumA] = arrayOfPrices[tempRndNumB];
		arrayOfPrices[tempRndNumB] = tempPrice;
	}

	return;
}

/*
function that reinitializes/ initializes the game 
@param - N/A

@return - N/A

*/
void dealOrNoDeal::_initialize() {
	unsigned int arrayOfPrices[TOTAL_CASE_NUM] = { 1,	100,	500,	1000,	2500,	5000,	7500,	10000,	20000,	30000,	40000,	50000,	75000,	100000,	500000,	1000000,	2500000,	5000000,	7500000,	10000000,	20000000,	30000000,	40000000,	50000000,	75000000,	100000000 };
	_numOfTurnsPlayed = 0;
	_caseColln.resetAllCases();
	_randomizePrices(arrayOfPrices, TOTAL_CASE_NUM);
	unsigned int tempAmt = 0;
	for (unsigned int i = 0; i < TOTAL_CASE_NUM; i++) {
		tempAmt = arrayOfPrices[i];
		_caseColln.setCaseAmntAndNum(tempAmt, i + 1);
	}

	if (_typeOfGame == humanInteractive) {
		do {
			cout << "Enter Suitcase to select between 1 and 26" << endl;
			cin >> _userSelectedCaseNum;
		} while (_userSelectedCaseNum > TOTAL_CASE_NUM || _userSelectedCaseNum<1);


	}
	else {
		Random randNum;
		_userSelectedCaseNum = randNum.get_random_number(1, 26);
	}

	unsigned int amntInCase = _caseColln.getAmountInCase(_userSelectedCaseNum);
	_userSuitcase.setAmount(amntInCase);
	_caseColln.removeSuitcase(_userSelectedCaseNum);
	return;
}

/*
function that returns the amount heald in the suitcase
@param - N/A

@return - N/A

*/
void dealOrNoDeal::_release() {
	if (SHOW) {
		cout << " in dealOrNoDeal release helper function" << endl;
	}
	// nothing to do
};

/*
function that returns the amount heald in the suitcase
@param - N/A

@return - N/A

*/
void dealOrNoDeal::_play() {
	bool runGame = true;

	while (runGame) {
		unsigned int numOfGameBeforeBankerCall = 6;
		Random rndNum;
		unsigned int numberOfGamesPlayed = 0;
		unsigned int casesSelected;
		while (_getNumOfCasesLeft() != 1) {


			if (numberOfGamesPlayed >= numOfGameBeforeBankerCall) {
				numberOfGamesPlayed = 0;
				numOfGameBeforeBankerCall = (numOfGameBeforeBankerCall == 1) ? 1 : (numOfGameBeforeBankerCall - 1);
				_callBanker();
			}

			if (_typeOfGame == humanInteractive) {
				do {
					cout << "Select from the following cases:" << endl;
					cout << _caseColln << endl;
					cin >> casesSelected;
				} while (!_caseColln.selectedCaseInList(unsigned int(casesSelected)));

			}
			else {
				casesSelected = _caseColln.genRandCaseNum();
			}

			cout << "Amount in case #" << casesSelected <<" is $" << (_caseColln.getAmountInCase(casesSelected)) / 100 << endl;
			_caseColln.removeSuitcase(casesSelected);

			numberOfGamesPlayed++;
		}

		// assume choseing user held case and no Deal taken at any time
		cout << "Amount in your case $" << (_userSuitcase.getAmount() / 100) << endl;

		_numberOfGamesPlayed++;
		bool amBroke = ((_userSuitcase.getAmount()) == 1);
		bool amMillionaire = (_userSuitcase.getAmount() == 100000000);
		if (_typeOfGame == playUntilBroke && !amBroke) {
			_initialize();
			runGame = true;
		}
		else if (_typeOfGame == playUntilBroke && amBroke) {
			cout << "Number of games played to go broke = " << _numberOfGamesPlayed << endl;
			runGame = false;
		}
		else if (_typeOfGame == playUntilMillionaire && !amMillionaire) {
			_initialize();
			runGame = true;
		}
		else if (_typeOfGame == playUntilMillionaire && amMillionaire) {
			cout << "Number of games played to become millionaire = " << _numberOfGamesPlayed << endl;
			runGame = false;
		}
		else {
			runGame = false;   // (get here when single game selected)
		}
	}
	
	

};

/*
function that returns the number of cases left in the game
@param N/A

@return - N/A

*/
unsigned int dealOrNoDeal::_getNumOfCasesLeft() {

	return _caseColln.getNumOfCasesLeft();


};

/*
returns the bankers offer to the contestent 
@param N/A

@return - N/A

*/
void dealOrNoDeal::_callBanker() {
	_numOfTurnsPlayed++;
	unsigned int bankerOffer = 0;
	bankerOffer = unsigned int(_caseColln.getAverage() /100) * _numOfTurnsPlayed/10;

	cout << "Banker is offering $" << bankerOffer << endl;

	return;


};