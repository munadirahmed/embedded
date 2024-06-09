/*
Author: Munadir Ahmed
Date: 4/28/18

*/

// file: nqueen.cpp


#include "nqueen.h"

/*

Determines all solutions for board of the desired dimension
@param aDimension - dimension of board

@return - N/A

*/
void nqueen::findAllSoln(int aDimension) {
	bool validBoard = 0;

	validBoard = _checkRequestDimension(aDimension);

	if (!validBoard) {   // not a valid board so don't bother moding foward
		return;
	}
	
	int(*Matrix)[100] = new int[100][100];// pointer to heap memory

	_clearMatrix(Matrix, aDimension);  // clear the matrix and initialize all to zero

	int* posOfQueenInEacRow = new int[aDimension];  // holds the location (coln idx) of the queen in each row 
	int activeRowIdx = 0;   // holds the active row
	int activeColnIdx = 0;   // holds the active column
	int previousRow = 0;  // used when clearing queen in previous row (temp variable)

	posOfQueenInEacRow[0] = 0;

	bool endOfSearch = false;
	bool placeQinRow = false;
	bool skipPlacement = false;
	int numberOfSolutions = 0;
	while (!endOfSearch) {
		//cout << "Active Row is " << activeRowIdx << endl;   // print for debugging
		//cout << "Active column is " << activeColnIdx << endl;   // print for debugging

		if (skipPlacement) {
			// do nothing
			skipPlacement = false;
		}
		else {
			// In each iteration, check to see if queen can be placed in current location
			placeQinRow = _queensPathClear(Matrix, aDimension, activeRowIdx, activeColnIdx);
			if (placeQinRow) {
				// able to place queen in active row and column index
				_queenLocationManager(Matrix, aDimension, activeRowIdx, activeColnIdx, place);
				posOfQueenInEacRow[activeRowIdx] = activeColnIdx;  // record where the queen is in this row before moving on
				activeRowIdx++;  // increment active row as queen hase been placed
				activeColnIdx = 0;  // go to left most column for next active row
			}
			else { // unable to place queen at this location so increment column index
				activeColnIdx++;
			}
		}
		

		if (activeColnIdx > (aDimension - 1)) {  // exceeded the board dimension (must not be able to place queen in this row
			if (activeRowIdx == 0) {
				//beyond the board and on the first row, must be done with search and not found a solution in current solution
				endOfSearch = true;
			}
			else {
				// with current combination in previous rows can't find a place for queen in current active row
				previousRow = (activeRowIdx - 1);
				_queenLocationManager(Matrix, aDimension, previousRow, posOfQueenInEacRow[previousRow], remove);
				activeRowIdx = previousRow;
				activeColnIdx = posOfQueenInEacRow[previousRow] + 1;
				skipPlacement = activeColnIdx > (aDimension - 1);
			}
		}

		if (activeRowIdx > (aDimension - 1)) {  // must have found a solution
			numberOfSolutions++;
			_printBoard(Matrix, aDimension, numberOfSolutions);   // print solution
			
			previousRow = (activeRowIdx - 1);
			_queenLocationManager(Matrix, aDimension, previousRow, posOfQueenInEacRow[previousRow], remove);
			activeRowIdx = previousRow;
			activeColnIdx = posOfQueenInEacRow[previousRow] + 1;
			// below gives only one solutions per first row placement of queen! works for 4x4 but 8x8 has issues!
			/*_clearMatrix(Matrix, aDimension);  // clear the matrix and initialize all to zero
			activeRowIdx = 0;
			activeColnIdx = posOfQueenInEacRow[0] + 1;*/   
		}

		if (posOfQueenInEacRow[0] > (aDimension - 1)) {  // exceeded the board dimension (must not be able to place queen in this row
			//beyond the board and on the first row, must be done with search and not found a solution in current solution
			endOfSearch = true;
		}
		
//		_printBoard(Matrix, aDimension);   // print for debugging
		//cout << endl;// print for debugging
	}
	
	cout << endl;
	cout << "Total NQueen problem solution for " << aDimension << "x" << aDimension << " board is: " << numberOfSolutions << endl;

	delete[] posOfQueenInEacRow;   // memory management: delete memory on heap before exiting function
	delete[] Matrix;   // memory management: delete memory on heap before exiting function
	return;
}



/*

Places or remove queen in desired location
@param aMatrix - array reference for chess board
@param aDimensionOfBoard - dimension of square to be checked
@param aRowStartIdx - the row index we are currently in
@param aColnStartIdx - the column index we are currently in

@return - returns false if requested location is beyond dimension of board

*/
bool nqueen::_queenLocationManager(int aMatrix[100][100], int aDimensionOfBoard, int aRowStartIdx, int aColnStartIdx, nqueen::queenPlacement aPlacement) {
	if (aRowStartIdx > (aDimensionOfBoard - 1)) {
		cout << "Row index is beyond range of board" << endl;
		return false;
	}
	if (aColnStartIdx > (aDimensionOfBoard - 1)) {
		cout << "Column index is beyond range of board" << endl;
		return false;
	}
	if (aPlacement == place) {
		aMatrix[aRowStartIdx][aColnStartIdx] = 1;
	}
	else {
		aMatrix[aRowStartIdx][aColnStartIdx] = 0;
	}
	

	return true;
}


/*

Checks to make sure that the the dimension being request is valid
@param aDimensionOfSquare - dimension of square to be checked

@return - returns boolean indicating if board is of proper size

*/
bool nqueen::_checkRequestDimension(int aDimensionOfBoard) {
	if (aDimensionOfBoard >= nqueen::MAX_SIZE_OF_BOARD) {
		cout << "The dimension of the board being request must be less than the maximum space allocated. Please increase size of MAX_SIZE_OF_BOARD" << endl;
		return 0;
	}
	
	return 1;


}



/*

Checks all row of the matrix to see if it is a magic square
@param aMatrix[100][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfBoard - dimension of square to be checked
@param aDimensionIdx - which index of the desired dimension to check
@param aDirection - dimension to be traversed over (rows or columns)

@return - returns true if all rows are equal sum

*/
bool nqueen::_isRowOrColnClear(const int aMatrix[100][100], int aDimensionOfBoard, int aDimensionIdx, nqueen::dimensionToCheck aDirection) {

	for (int i = 0; i < aDimensionOfBoard; i++) {  // iterate over rows

		switch (aDirection) {
		case rows:
			//cout << "(R,C) = " << i << ", " << aDimensionIdx << " value = " << aMatrix[i][aDimensionIdx] << endl;
			if (aMatrix[i][aDimensionIdx]) {// check in row direction
				// returns false if 1 is found in path
				
				return false;
			}
			break;
		case columns:
			//cout << "(R,C) = " << aDimensionIdx << ", " << i << " value = " << aMatrix[aDimensionIdx][i] << endl;
			if (aMatrix[aDimensionIdx][i]) {// check in column direction
				// returns false if 1 is found in path
				
				return false;
			}
			break;
		default:
			// shouldn't come here
			break;
		}
		
	}

	return true;  // traversed dimension and didn't find a 1
}

/*

Checks all major and minor diagonal of the matrix to see if it is a magic square
@param aMatrix[100][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfBoard - dimension of board to be checked
@param aRowStartIdx - the row index we are currently in
@param aColnStartIdx - the column index we are currently in
@param aDirection - direction of diagonal to check

@return - returns true if all rows are equal sum

*/
bool nqueen::_isDiagonalClear(const int aMatrix[100][100], int aDimensionOfBoard, int aRowStartIdx, int aColnStartIdx, nqueen::diagonalToCheck aDirection) {
	
	int rowIdx = 0;
	int colnIdx = 0;

	bool reachedOtherCorner = false;
	bool dontstop = true;
	//Derive starting point depending on direction traversing
	switch (aDirection) {
	case nw_se:
		//rowIdx = (aRowStartIdx > aColnStartIdx) ? (aRowStartIdx - aColnStartIdx) : (aRowStartIdx - aRowStartIdx);   // go to top left of diagonal
		//colnIdx = (aRowStartIdx > aColnStartIdx) ? (aColnStartIdx - aColnStartIdx) : (aColnStartIdx - aRowStartIdx);   // go to top left of diagonal
		rowIdx = aRowStartIdx;
		colnIdx = aColnStartIdx;

		while (dontstop) {
			if (rowIdx == (0)) {
				dontstop = false;
			}
			else if (colnIdx == 0) {
				dontstop = false;
			}
			else {
				rowIdx--;  // move to top left of board
				colnIdx--;  // move to top left of diagonal
			}

		}
		break;
	case sw_ne:
		rowIdx = aRowStartIdx;
		colnIdx = aColnStartIdx;
		
		while (dontstop) {
			if (rowIdx == (aDimensionOfBoard-1)) {
				dontstop = false;
			}
			else if (colnIdx == 0) {
				dontstop = false;
			}
			else {
				rowIdx++;  // move to bottom left of diagonal
				colnIdx--;  // move to bottom left of diagonal
			}
			
		}

		break;
	default:
		// shouldn't come here
		break;
	}

	

	while (!reachedOtherCorner) {

		//cout << "(R,C) = " << rowIdx << ", " << colnIdx << " value = " << aMatrix[rowIdx][colnIdx] << endl;
		if (aMatrix[rowIdx][colnIdx]) {  // found a 1 in the queens path
			return false;
		}

		switch (aDirection) {
		case nw_se:
			if ((rowIdx == (aDimensionOfBoard - 1)) || (colnIdx == (aDimensionOfBoard - 1))) {
				reachedOtherCorner = true;
			}
			else {
				rowIdx++;
				colnIdx++;
			}
			break;
		case sw_ne:
			if ((rowIdx == 0) || (colnIdx == (aDimensionOfBoard - 1))) {
				reachedOtherCorner = true;
			}
			else {
				rowIdx--;
				colnIdx++;
			}
			break;
		default:
			// do nothing
			break;

		}
	}

	return true;  // diagonal in desired direction is empty
}

/*

Clears the matrix
@param aMatrix[][100] - 2-d array of matrix to be cleared
@param aDimensionOfBoard - dimension of board to be checked


@return - N/A

*/
void nqueen::_clearMatrix(int aMatrix[100][100], int aDimensionOfBoard) {

	for (int i = 0; i < aDimensionOfBoard; i++) {  // iterate over rows
		for (int j = 0; j< aDimensionOfBoard; j++) {  // iterate over columns
			aMatrix[i][j] = 0;
		}
	}
	return;
}

/*

Checks to see if queen can be placed at the desired coordinate
@param aMatrix[100][100] - 2-d array of matrix to be checked (const to prevent function from changing elements)
@param aDimensionOfBoard - dimension of board to be checked
@param aRowStartIdx - the row index we are currently in
@param aColnStartIdx - the column index we are currently in


@return - N/A

*/
bool nqueen::_queensPathClear(const int aMatrix[100][100], int aDimensionOfBoard, int aRowStartIdx, int aColnStartIdx) {
	
	bool rowClear = false;
	bool columnClear = false;
	bool majorDiagonalClear = false;
	bool minorDiagonalClear = false;
	rowClear = _isRowOrColnClear(aMatrix, aDimensionOfBoard, aColnStartIdx, rows);
	columnClear = _isRowOrColnClear(aMatrix, aDimensionOfBoard, aRowStartIdx, columns);
	majorDiagonalClear = _isDiagonalClear(aMatrix, aDimensionOfBoard, aRowStartIdx, aColnStartIdx, nw_se);
	minorDiagonalClear = _isDiagonalClear(aMatrix, aDimensionOfBoard, aRowStartIdx, aColnStartIdx, sw_ne);

	//cout << majorDiagonalClear << endl;

	//cout << minorDiagonalClear << endl;

	if (rowClear && columnClear && majorDiagonalClear && minorDiagonalClear) {
		return true;
	}
	else {
		return false;
	}

}

/*
Prints the solution
@param aMatrix - array reference for chess board
@param aDimensionOfBoard - dimension of board
@param aSolutionNumber - solution number

@return - N/A

*/
void nqueen::_printBoard(const int aMatrix[100][100], int aDimensionOfBoard, int aSolutionNumber) {
	cout << "Solution #" << aSolutionNumber << " is:" << endl;
	for (int i = 0; i < aDimensionOfBoard; i++) {
		for (int j = 0; j < aDimensionOfBoard; j++) {
			if (aMatrix[i][j]) {
				cout << "1" ;
			}
			else {
				cout << "-";
			}
		}
		cout << endl;
	}
	return;
}