/*
Author: Munadir Ahmed
Date: 4/28/18

*/

// file: magicSquare.h


// All includes
#ifndef MAGICSQUARE_H   
#define MAGICSQUARE_H



#include "../util/util.h"

// Declaration of class

class magicSquare {
public:
	void printMagicSquare(int aDimension);
	enum dimensionToCheck { rows = 0, columns, diagonals };
private:
	
	const int MAX_SIZE_OF_SQUARE = 100;
	bool _checkRequestDimension(const int aMatrix[100][100], int aDimensionOfSquare);
	bool _checkValidSquare(const int aMatrix[100][100],int aDimensionOfSquare, int* aMagicNumberPtr);   // must be same as MAX_SIZE_OF_SQUARE
	bool _checkRowAnColn(const int aMatrix[100][100], int aDimensionOfSquare, int aSumOfFirstRow, magicSquare::dimensionToCheck aDirection);
	bool _checkDiagonals(const int aMatrix[100][100], int aDimensionOfSquare, int aSumOfFirstRow, magicSquare::dimensionToCheck aDirection);
	void _clearMatrix(int aMatrix[100][100], int aDimensionOfSquare);
	void _generateMagicSquare(int aMatrix[100][100], int aDimensionOfSquare);
	void _moveInSquare(int aMatrix[100][100], int aDimensionOfSquare, int* aRowIdx, int* aColnIdx);
};


#endif // !MAGICSQUARE_H