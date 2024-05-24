/*
Author: Munadir Ahmed
Date: 5/5/18

*/

// file: intmatrix1.h


// All includes
#ifndef INTMATRIX1_H   
#define INTMATRIX1_H


#include "../util/util.h"

// Declaration of class

class intmatrix1 {
public:
	void init(unsigned int aNumOfRows=0, unsigned int aNumOfColns=0, int aDefaultValue=0);
	void init(const char* aStringRepOfMatrix);
	void fini(void);
	void print(const char* aStatement = "\0");
	bool isEmpty();
	bool isScalar();
	intmatrix1 add(intmatrix1 aMatrixToAdd);
	intmatrix1 mult(intmatrix1 aMatrixToMultiplyWith);
	unsigned int getNumOfRows();
	unsigned int getNumOfColumns();
	int getElement(unsigned int aRowIdx, unsigned int aColnIdx);
	void setElement(unsigned int aRowIdx, unsigned int aColnIdx, int aValue);
	bool isEqual(intmatrix1 aMatrixToCompare);

private:
	int* _ptrToMatrix;   // pointer to matrix on heap
	unsigned int _numOfRows;
	unsigned int _numOfColumns;
	enum _operation { addition = 0, multiply };
	unsigned int _totalNumOfElements;
	
	unsigned int _memoryIdxManager(unsigned int aRowIdx, unsigned int aColnIdx);
	bool _isOperationLegal(intmatrix1 aMtrxToOprtWth, _operation aTypOfOpr);
	void _parseStringMatrix(const char* aStringRepOfMatrix);
	bool _inRange(unsigned int aRowIdx, unsigned int aColnIdx);
};


#endif // !INTMATRIX1_H