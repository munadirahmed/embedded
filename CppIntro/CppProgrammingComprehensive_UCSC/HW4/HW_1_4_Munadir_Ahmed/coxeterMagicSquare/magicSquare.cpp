/*
Author: Munadir Ahmed
Date: 4/21/18

*/

// file: wordEqCost.cpp


#include "magicSquare.h"

/*

Prints the Magic Square of the dimension of interest
@param aDimensionOfCube - dimension of magic square of interest

@return - N/A

*/
void magicSquare::printMagicSquare(int aDimension) {
	int(*Matrix)[100] = new int[100][100];// pointer to heap memory
	
	bool validSquare = 0;
	/* begin test of hand entered matrix
	const int dim = 3;

	Matrix[100][100] = { { 6, 1, 8 },{ 7,5,3 },{ 2,9,4 } };


	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			cout << Matrix[i][j] << "\t";
		}
		cout << endl;
	}

	validSquare = _checkValidSquare(dummyMatrix, dim);

	cout << endl << validSquare << endl;
	
	begin test of hand entered matrix*/

	_clearMatrix(Matrix, 100);

	_generateMagicSquare(Matrix, aDimension);

	if (aDimension <= 11) {
		cout << "Generated "<< aDimension<<"x"<< aDimension<<" matrix is given below:" << endl;

		for (int i = 0; i < aDimension; i++) {
			for (int j = 0; j < aDimension; j++) {
				cout << Matrix[i][j] << "\t";
			}
			cout << endl;
		}
	}
	int magicNumber = 0;
	int* magicNumberPtr = &magicNumber;
	validSquare = _checkValidSquare(Matrix, aDimension, magicNumberPtr);

	const char* a[] = { " is not", " is" };
	cout << "Generated " << aDimension << "x" << aDimension << a[validSquare] << " a valid COXETER MAGIC SQUARE with magic number " << magicNumber << endl;
	

	cout << endl;
	delete[] Matrix; // deleting from heap memory
	return;
}

/*

Checks to make sure that the matrix being passed is a valid magic square (private function)
@param aMatrix[][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfSquare - dimension of square to be checked

@return - returns true if the matrix passed is a magic cube and false otherwise

*/
bool magicSquare::_checkRequestDimension(const int aMatrix[100][100], int aDimensionOfSquare) {
	if (aDimensionOfSquare > magicSquare::MAX_SIZE_OF_SQUARE) {
		cout << "The dimension of the square being request is greater than the maximum space allocated. Please increase size of MAX_SIZE_OF_SQUARE" << endl;
		return 0;
	}

	// check to see if DimensionOfSquare is even and break from function if it is. Magic square must have odd dimension
	if (aDimensionOfSquare % 2 == 0) {
		cout << "The dimension of the square being request must be odd" << endl;

		return 0;
	}

	return 1;

	
}

/*

Checks to make sure that the matrix being passed is a valid magic square (private function)
@param aMatrix[][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfSquare - dimension of square to be checked
@param aMagicNumberPtr - pointer to magic number for this dimension

@return - returns true if the matrix passed is a magic cube and false otherwise

*/
bool magicSquare::_checkValidSquare(const int aMatrix[100][100], int aDimensionOfSquare, int* aMagicNumberPtr) {
	bool isValidDimension = _checkRequestDimension(aMatrix, aDimensionOfSquare);
		if (!isValidDimension) {
			return 0;
	}

	// check to see if the the middle column of row 0 is 1, otherwise it is not a valid magic square
	int middleColnIdx = aDimensionOfSquare / 2;   // using inherent flooring to get middle index row 0
	if (aMatrix[0][middleColnIdx] != 1) {
		return 0;
	}

	int sumOfFirstRow = 0;
	for(int i = 0; i < aDimensionOfSquare; i++) {
		sumOfFirstRow += aMatrix[0][i];   // calculate sum of first row to check against for all other rows, columnsand diaonals
	}

	bool rowValid = false;
	bool columnValid = false;
	bool diagonalValid = false;

	rowValid = _checkRowAnColn(aMatrix, aDimensionOfSquare, sumOfFirstRow, rows);
	columnValid = _checkRowAnColn(aMatrix, aDimensionOfSquare, sumOfFirstRow, columns);
	diagonalValid = _checkDiagonals(aMatrix, aDimensionOfSquare, sumOfFirstRow, diagonals);

	if (!rowValid || !columnValid || !diagonalValid) {
		return false;
	}
	else {
		*aMagicNumberPtr = sumOfFirstRow;
		return true;
	}
}

/*

Checks all row of the matrix to see if it is a magic square
@param aMatrix[][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfSquare - dimension of square to be checked
@param aSumOfFirstRow - sum to be checked against
@param aDirection - direction of matrix to check

@return - returns true if all rows are equal sum

*/
bool magicSquare::_checkRowAnColn(const int aMatrix[100][100], int aDimensionOfSquare,int aSumOfFirstRow, magicSquare::dimensionToCheck aDirection) {
	
	int tempSum = 0;
	for (int i = 0; i < aDimensionOfSquare; i++) {  // iterate over rows
		
		if (aDirection == diagonals) {
			return false;
		}
		else {
			for (int j = 0; i < aDimensionOfSquare; i++) { // iterate over columns

				switch (aDirection) {
				case rows:
					tempSum += aMatrix[i][j];   // check rows
					break;
				case columns:
					tempSum += aMatrix[j][i];   // check columns
					break;
				default:
					// shouldn't come here
					break;
				}

			}
		}
		

		if (tempSum != aSumOfFirstRow) {
			return false;
		}
		tempSum = 0;   // reset for the next row/column/diagonal iteration
	}

	return true;  // all row/column/diagonal have been iterated and they are equal to aSumOfFirstRow
}

/*

Checks all major and minor diagonal of the matrix to see if it is a magic square
@param aMatrix[][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfSquare - dimension of square to be checked
@param aSumOfFirstRow - sum to be checked against
@param aDirection - direction of matrix to check

@return - returns true if all rows are equal sum

*/
bool magicSquare::_checkDiagonals(const int aMatrix[100][100], int aDimensionOfSquare, int aSumOfFirstRow, magicSquare::dimensionToCheck aDirection) {

	if (aDirection != diagonals) {
		return false;
	}

	int tempSumMainDiag = 0;
	int tempSumMinorDiag = 0;
	for (int i = 0; i < aDimensionOfSquare; i++) {  // iterate over major and minor
		tempSumMainDiag += aMatrix[i][i];   // check major diagonals going 
		tempSumMinorDiag += aMatrix[(aDimensionOfSquare -1) - i][i];   // check minor diagonals going 


		
	}
	if (tempSumMainDiag != aSumOfFirstRow) {
		return false;
	}
	if (tempSumMinorDiag != aSumOfFirstRow) {
		return false;
	}


	return true;  // all row/column/diagonal have been iterated and they are equal to aSumOfFirstRow
}

/*

Clears the matrix
@param aMatrix[][100] - 2-d array of matrix to be cleared
@param aDimensionOfSquare - dimension of square to be checked


@return - N/A

*/
void magicSquare::_clearMatrix(int aMatrix[100][100], int aDimensionOfSquare) {

	for (int i = 0; i < aDimensionOfSquare; i++) {  // iterate over rows
		for (int j = 0; j< aDimensionOfSquare; j++) {  // iterate over columns
			aMatrix[i][j] = 0;
		}
	}
	return;
}

/*

Generate Magic square
@param aMatrix[][100] - 2-d array of matrix where the magic number should be generated
@param aDimensionOfSquare - dimension of square to be generated


@return - N/A

*/
void magicSquare::_generateMagicSquare(int aMatrix[100][100], int aDimensionOfSquare) {

	bool isValidDimension = _checkRequestDimension(aMatrix, aDimensionOfSquare);
	if (!isValidDimension) {
		return ;
	}

	int middleColnIdx = aDimensionOfSquare / 2;   // using inherent flooring to get middle index row 0
	aMatrix[0][middleColnIdx] = 1;  // initialize the middle index of first row to 1

	bool done = false;
	int maxCount = aDimensionOfSquare * aDimensionOfSquare;
	int count = 2;   // already initialized first element (1) so start with 2

	int rowIdx = 0;   // start on first row
	int* rowIdxPtr = &rowIdx;   // pointer to row index
	int columnIdx = middleColnIdx;  // start in the middle column
	int* columnIdxPtr = &columnIdx;   // pointer to column index

	while (!done) {

		_moveInSquare(aMatrix, aDimensionOfSquare, rowIdxPtr, columnIdxPtr);

		aMatrix[rowIdx][columnIdx] = count;
		count++;
		if (count > maxCount) {   // finished filling aDimensionOfSquare x aDimensionOfSquare matrix
			done = true;
		}
	}
	return;
}

/*

Move the row and column index pointers across the square
@param aMatrix[][100] - 2-d array of matrix where the magic number should be generated
@param aDimensionOfSquare - dimension of square to be generated


@return - N/A

*/
void magicSquare::_moveInSquare(int aMatrix[100][100], int aDimensionOfSquare, int* aRowIdx, int* aColnIdx) {

	int beginingRowIdx = *aRowIdx;  // where we are starting in the row
	int beginingColumnIdx = *aColnIdx;  // where we are starting in the column
	int finalRowIdx = *aRowIdx;  // where we are going to end in the row
	int finalColumnIdx = *aColnIdx;  // where we are going to end in the column

	// first go diagonal
	finalRowIdx = (beginingRowIdx == 0) ? (aDimensionOfSquare - 1) : (beginingRowIdx - 1);   // wrap around top
	finalColumnIdx = (beginingColumnIdx == 0) ? (aDimensionOfSquare - 1) : (finalColumnIdx - 1);  // wrap around left

	// If cell is not empty then change location to below the original start point
	if (aMatrix[finalRowIdx][finalColumnIdx] != 0) {
		finalRowIdx = (beginingRowIdx == (aDimensionOfSquare - 1)) ? 0 : (beginingRowIdx + 1);  // wrap around bottom
		finalColumnIdx = beginingColumnIdx;
	}

	*aRowIdx = finalRowIdx;
	*aColnIdx = finalColumnIdx;

	return;
}