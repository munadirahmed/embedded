/*
Author: Munadir Ahmed
Date: 5/19/18

*/

// file: dealOrNoDeal.h


// All includes
#ifndef DEAL_OR_NO_DEAL_H   
#define DEAL_OR_NO_DEAL_H


#include "../util/util.h"

static const int SHOW = 0;
static const unsigned int TOTAL_CASE_NUM = 26;

// Declaration of class

/*--------------------------------------------------------
class declaration of suitcase
----------------------------------------------------------*/
class suitcase {
public:
	suitcase(unsigned int aAmount = 0);
	~suitcase();
	suitcase(const suitcase &aObjToCopyFrom);
	suitcase& operator= (const suitcase &rhs);
	friend ostream& operator<<(ostream& o, const suitcase& a);

	unsigned int getAmount();
	void setAmount(unsigned int aNewAmount = 0);

private:
	unsigned int _amount;

	void _release();
	void _copy(const suitcase &rhs);
};

/*--------------------------------------------------------
class for suitcase collection
----------------------------------------------------------*/

class suitcaseCollection {
public:
	suitcaseCollection(unsigned int aNumOfCasesInColn = TOTAL_CASE_NUM);
	~suitcaseCollection();

	unsigned int getAmountInCase(unsigned int aSuitcaseNumber = 1);
	void removeSuitcase(unsigned int aSuitcaseNumber = 1);
	void setCaseAmntAndNum(unsigned int aAmntToSet = 0, unsigned int aSuitcaseNumber = 1);
	bool selectedCaseInList(unsigned int aSuitcaseNumber = 1);
	void resetAllCases();
	unsigned int genRandCaseNum();
	unsigned int getAverage();
	unsigned int getNumOfCasesLeft();

private:
	suitcase* _listOfCases;
	unsigned int* _listOfSuitcaseNumbers;
	unsigned int _numOfCasesLeft;
	unsigned int _originalNumOfCases;
	
	friend ostream& operator<<(ostream& o,const suitcaseCollection& a);

	void _release();
	bool _caseInList(unsigned int& indxOfCase,unsigned int aSuitcaseNumber = 1);

	/* no body can copy or equal suitcaseCollection */
	suitcaseCollection(const suitcaseCollection& x) = delete;
	suitcaseCollection& operator=(const suitcaseCollection& x) = delete;
};

/*--------------------------------------------------------
class for suitcase collection
----------------------------------------------------------*/

class dealOrNoDeal {
public:
	enum typeOfGame { humanInteractive = 0, singleComp, playUntilMillionaire, playUntilBroke};
	dealOrNoDeal(typeOfGame aTypeOfGame = humanInteractive);
	~dealOrNoDeal();

private:
	suitcaseCollection _caseColln;
	unsigned int _userSelectedCaseNum;
	suitcase _userSuitcase;
	typeOfGame _typeOfGame;
	unsigned int _numOfTurnsPlayed;
	unsigned int _numberOfGamesPlayed;
	
	unsigned int _getNumOfCasesLeft();
	void _play();
	void _initialize();
	void _callBanker();
	void _release();
	void _randomizePrices(unsigned int arrayOfPrices[], unsigned int size = TOTAL_CASE_NUM);

	/* no body can copy or equal dealOrNoDeal */
	dealOrNoDeal(const dealOrNoDeal& x) = delete;
	dealOrNoDeal& operator=(const dealOrNoDeal& x) = delete;
};

/*--------------------------------------------------------
class random number generator
----------------------------------------------------------*/
class Random {
public:
	Random() { srand((unsigned)time(0)); }
	int get_random_number(int a = 0, int b = 10000) const {
		int upper_bound, lower_bound;
		if (a < b) {
			upper_bound = b - a; lower_bound = a;
		}
		else if (a >= b) {
			upper_bound = a - b; lower_bound = b;
		}
		return(lower_bound + rand() % upper_bound);
	}

private:
	/* no body can copy random or equal random */
	Random(const Random& x) = delete;
	Random& operator=(const Random& x) = delete;
};


#endif // !DEAL_OR_NO_DEAL_H