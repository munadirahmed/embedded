/*
Author: Munadir Ahmed
Date: 4/7/18

*/

// file: reverse.cpp


#include "reverse.h"
static unsigned long firstFailedNum = 0;
// Definition of operations in reverse class

unsigned long reverse1::r(unsigned long n) {
	unsigned short digits[32] = {0};  // assumes it is a 32 bit integer as it is an unsigned long input
	unsigned short  index,numOfDigits, remainder;
	unsigned long long tempReverse;
	unsigned long quotient, reverse;
	unsigned long num = n;
	
	static bool firstFailFlag;
	
	for (index = 0; index <= 31; index++) {
		if (num == 0) {
			break;
		}
		quotient = num / 10;
		remainder = (unsigned short)(num - (quotient * 10));

		digits[index] = remainder;
		num = quotient;
	}

	numOfDigits = index;
	tempReverse = 0;

	for (index = 0; index < numOfDigits; index++) {
		tempReverse += digits[index] * (unsigned long long)(pow(10, numOfDigits - index - 1));
	}
	bool flag = tempReverse > (0xFFFFFFFF);
	if (flag) {
		reverse = 0;
		if (firstFailFlag == false) {
			firstFailFlag = true;
			firstFailedNum = n;
		}
	}
	else {
		reverse = (unsigned long) tempReverse;
	}

	return reverse;

}

unsigned long reverse1::firstFail() {
	return firstFailedNum;
}