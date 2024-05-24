/*
Author: Munadir Ahmed
Date: 5/12/18

*/

// file: intmatrix2.h


// All includes
#ifndef INTMATRIX2_H   
#define INTMATRIX2_H


#include "../util/util.h"

// Declaration of class

class intmatrix2 {
public:
	intmatrix2(unsigned int aNumOfRows = 0, unsigned int aNumOfColns = 0, int aDefaultValue = 0);
	intmatrix2(const char* aStringRepOfMatrix);
	intmatrix2(const intmatrix2& aMatrixToCopyFrom);
	~intmatrix2(void);

	// Operator overload functions
	intmatrix2& operator=(const intmatrix2& aMtrxOnRHS);
	friend ostream& operator<<(ostream& o, const intmatrix2& aMtrx);

	bool isEmpty() const;
	bool isScalar() const;
	unsigned int getNumOfRows() const;
	unsigned int getNumOfColumns() const;
	int getElement(unsigned int aRowIdx, unsigned int aColnIdx) const;
	
	bool isEqual(const intmatrix2& aMatrixToCompare) const;

	void setElement(unsigned int aRowIdx, unsigned int aColnIdx, int aValue);

	// Return by value functions
	intmatrix2 add(const intmatrix2& aMatrixToAdd) const;
	intmatrix2 mult(const intmatrix2& aMatrixToMultiplyWith) const;

private:
	int** _matrix;   // pointer to pointer of matrix on heap
	unsigned int _numOfRows;
	unsigned int _numOfColumns;
	enum _operation { addition = 0, multiply };
	unsigned int _totalNumOfElements;

	bool _isOperationLegal(const intmatrix2& aMtrxToOprtWth, _operation aTypOfOpr) const;
	void _parseStringMatrix(const char* aStringRepOfMatrix);
	bool _inRange(unsigned int aRowIdx, unsigned int aColnIdx)const;
	void _copy(const intmatrix2& aMatrixToCopyFrom);
	void _release(void);
};


#endif // !INTMATRIX2_H