/*
Author: Munadir Ahmed
Date: 4/28/18

*/

// file: nqueen.h


// All includes
#ifndef NQUEEN_H   
#define NQUEEN_H



#include "../util/util.h"

// Declaration of class

class nqueen {
public:
	void findAllSoln(int aDimensionOfBoard);
	enum dimensionToCheck { rows = 0, columns };
	enum diagonalToCheck { nw_se = 0, sw_ne };
	enum queenPlacement { remove = 0, place };
private:

	const int MAX_SIZE_OF_BOARD = 100;
	bool _checkRequestDimension(int aDimensionOfBoard);
	//bool _checkValidSquare(const int aMatrix[100][100], int aDimensionOfBoard, int* aMagicNumberPtr);   // must be same as MAX_SIZE_OF_SQUARE
	bool _isRowOrColnClear(const int aMatrix[100][100], int aDimensionOfBoard, int aDimensionIdx, nqueen::dimensionToCheck aDirection);
	bool _isDiagonalClear(const int aMatrix[100][100], int aDimensionOfBoard, int aRowStartIdx, int aColnStartIdx, nqueen::diagonalToCheck aDirection);
	void _clearMatrix(int aMatrix[100][100], int aDimensionOfBoard);
	bool _queenLocationManager(int aMatrix[100][100], int aDimensionOfBoard, int aRowStartIdx, int aColnStartIdx, nqueen::queenPlacement aPlacement);
	bool _queensPathClear(const int aMatrix[100][100], int aDimensionOfBoard, int aRowStartIdx, int aColnStartIdx);
	void _printBoard(const int aMatrix[100][100], int aDimensionOfBoard, int aSolutionNumber);
};


#endif // !NQUEEN_H