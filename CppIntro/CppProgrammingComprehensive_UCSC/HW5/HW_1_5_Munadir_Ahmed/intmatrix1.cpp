/*
Author: Munadir Ahmed
Date: 5/5/18

*/

// file: intmatrix1.cpp
#define SCALAR_MULT_SUPPORTED 0
#define SCALAR_ADD_SUPPORTED 0

#include "intmatrix1.h"

// Definition of operations in intmatrixph1 class
/*

Initializes an empty matrix on the heap
@param aNumOfRows - number of rows in the matrix
@param aNumOfColns - number of columns in the matrix
@param aDefaultValue - what each element of the matrix should be filled with

@return - N/A

*/
void intmatrix1::init(unsigned int aNumOfRows, unsigned int aNumOfColns, int aDefaultValue) {

	intmatrix1::_numOfRows = aNumOfRows;
	intmatrix1::_numOfColumns = aNumOfColns;
	intmatrix1::_totalNumOfElements = (aNumOfRows * aNumOfColns);   

	intmatrix1::_ptrToMatrix = new int[intmatrix1::_totalNumOfElements];

	
	for (unsigned int i = 0; i < aNumOfRows; i++) {
		for (unsigned int j = 0; j < aNumOfColns; j++) {

			setElement(i, j, aDefaultValue);
		}
	}
}

/*

Initializes an empty matrix on the heap
@param aNumOfRows - number of rows in the matrix
@param aNumOfColns - number of columns in the matrix
@param aDefaultValue - what each element of the matrix should be filled with

@return - N/A

*/
void intmatrix1::init(const char* aStringRepOfMatrix) {
	_parseStringMatrix(aStringRepOfMatrix);
	return;
}

/*

Initializes an empty matrix on the heap
@param aNumOfRows - number of rows in the matrix
@param aNumOfColns - number of columns in the matrix
@param aDefaultValue - what each element of the matrix should be filled with

@return - N/A

*/
void intmatrix1::_parseStringMatrix(const char* aStringRepOfMatrix) {
	unsigned int numOfColnsInRow0 = 0;
	unsigned int numOfColnsInRowi = 0;
	unsigned int numOfRows = 1;
	unsigned int i = 0;
	
	unsigned int MaxNumOfElements = 0;
	

	

	char tempChar = *aStringRepOfMatrix;

	while (tempChar) {
		MaxNumOfElements++;
		tempChar = *(aStringRepOfMatrix + MaxNumOfElements);
	}

	int* tempHeapMemory = new int[MaxNumOfElements];
	int tempMemIdx = 0;

	bool seenNum = false;
	bool seenSpaceAfterNum = false;
	bool seenRowSeparator = false;
	bool validMatrix = true;
	int numberStored = 0;

	tempChar = *aStringRepOfMatrix;

	while (tempChar && validMatrix) {
		

		if (tempChar >= '0' && tempChar <= '9') {
			numberStored = numberStored*10 + (tempChar - '0');
			seenNum = true;
			seenSpaceAfterNum = false;
			if (seenRowSeparator) {
				numOfRows++;
			}
			seenRowSeparator = false;
		}
		if (tempChar == ' ') {
			
			if (seenNum) {
				seenSpaceAfterNum = true;
				if (numOfRows == 1) {
					numOfColnsInRow0++;
				}
				numOfColnsInRowi++;
				*(tempHeapMemory + tempMemIdx) = numberStored;
				tempMemIdx++;

			}
			seenNum = false;
			numberStored = 0;
		}

		if (tempChar == '|') {
			
			if (seenNum) {
				if (numOfRows == 1) {
					numOfColnsInRow0++;
				}
				numOfColnsInRowi++;
				*(tempHeapMemory + tempMemIdx) = numberStored;
				tempMemIdx++;
			}
			seenNum = false;
			numberStored = 0;

			if (numOfColnsInRow0 != numOfColnsInRowi) {
				validMatrix = false;
			}
			numOfColnsInRowi = 0;

			
			seenRowSeparator = true;
		}

		
		i++;
		tempChar = *(aStringRepOfMatrix + i);
		
	}

	if (!tempChar) {
		if (seenNum) {
			if (numOfRows == 1) {
				numOfColnsInRow0++;
			}
			numOfColnsInRowi++;
			*(tempHeapMemory + tempMemIdx) = numberStored;
			tempMemIdx++;
		}
		seenNum = false;
		numberStored = 0;

		if (numOfColnsInRow0 != numOfColnsInRowi) {
			validMatrix = false;
		}
		numOfColnsInRowi = 0;
	}

	if (!validMatrix || (MaxNumOfElements ==0)) {
		intmatrix1::_numOfColumns = 0;
		intmatrix1::_numOfRows = 0;
		intmatrix1::_totalNumOfElements = 0;
		intmatrix1::_ptrToMatrix = new int[1];
		*intmatrix1::_ptrToMatrix = NULL;
		delete[] tempHeapMemory;
		return;
	}

	intmatrix1::_numOfColumns = numOfColnsInRow0;
	intmatrix1::_numOfRows = numOfRows;

	intmatrix1::_totalNumOfElements = (intmatrix1::_numOfColumns * intmatrix1::_numOfRows);   // +1 to hold the null character

	intmatrix1::_ptrToMatrix = new int[intmatrix1::_totalNumOfElements];
	
	for (i = 0; i < _totalNumOfElements; i++) {
		*(intmatrix1::_ptrToMatrix + i) = *(tempHeapMemory + i);
	}
	
	delete[] tempHeapMemory;
	return;
}

/*
Determines the index in the heap memory that the matrix coordinate points to
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested

@return - N/A

*/
unsigned int  intmatrix1::_memoryIdxManager(unsigned int aRowIdx, unsigned int aColnIdx) {

	if (isEmpty()) {
		// nothing to store as it is an empty matrix
		return 0;
	}
	if (!(this->_inRange(aRowIdx, aColnIdx))) {
		// nothing to store as it is an empty matrix
		return 0;
	}
	unsigned int memoryIdx = (aRowIdx * _numOfColumns) + aColnIdx;// aRowIdx*aColnIdx gives index to end of row, and + aColnIdx give element in the column of interest
	return memoryIdx;
}


/*

Sets the element in the desired coordinate to the desired value
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested
$param aValue - value to be set in the desired location

@return - N/A

*/
void intmatrix1::setElement(unsigned int aRowIdx, unsigned int aColnIdx, int aValue) {

	if (isEmpty()) {
		// nothing to store as it is an empty matrix
		return;
	}
	if (!(this->_inRange(aRowIdx, aColnIdx))) {
		// nothing to store as it is an empty matrix
		return;
	}

	unsigned int memoryIdx = _memoryIdxManager(aRowIdx, aColnIdx);

	*(intmatrix1::_ptrToMatrix + memoryIdx) = aValue; // initialize aNumOfRows*aNumOfColns elements to default
	
	return;
}


/*

Gets the element in the desired coordinate to the desired value
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested

@return - returns the element at the desired location

*/
int intmatrix1::getElement(unsigned int aRowIdx, unsigned int aColnIdx) {

	if (this->isEmpty()) {
		// nothing to store as it is an empty matrix
		return 0;
	}

	if (!(this->_inRange(aRowIdx, aColnIdx))) {
		// nothing to store as it is an empty matrix
		return 0;
	}

	int valueStored;
	unsigned int memoryIdx = _memoryIdxManager(aRowIdx, aColnIdx);

	valueStored = *(intmatrix1::_ptrToMatrix + memoryIdx); // gets the value at the memory index

	return valueStored;
}

/*

Returns if the matrix is empty or not
@param N/A

@return - N/A

*/
bool intmatrix1::isEmpty() {

	if (intmatrix1::_totalNumOfElements == 0) {
		// nothing to store as it is an empty matrix
		return true;
	}
	else {
		return false;
	}	
}

/*

Returns if this matrix is equal to the matrix object passed to function
@param aMatrixToCompare -  matrix object to compare to

@return - true if both matrices are equal, false otherwise

*/
bool intmatrix1::isEqual(intmatrix1 aMatrixToCompare) {
	int othrMtrxRows = aMatrixToCompare.getNumOfRows();
	int othrMtrxColns = aMatrixToCompare.getNumOfColumns();

	//Row and Column dimensions must be equal
	if (othrMtrxRows != _numOfRows) {
		return false;
	}
	if (othrMtrxColns != _numOfColumns) {
		return false;
	}

	int valueAtCoordinate_OthrMatrix = 0;
	int valueAtCoordinate_ThisMatrix = 0;

	for (unsigned int rowIdx = 0; rowIdx < _numOfRows; rowIdx++) {
		for (unsigned int colnIdx = 0; colnIdx <_numOfColumns; colnIdx++) {
			valueAtCoordinate_OthrMatrix = aMatrixToCompare.getElement(rowIdx, colnIdx);
			valueAtCoordinate_ThisMatrix = this->getElement(rowIdx, colnIdx);
			if (valueAtCoordinate_OthrMatrix != valueAtCoordinate_ThisMatrix) {
				return false;
			}
		}
	}


	return true;  // passed all other tests, must be equal
}

/*

Determines if a coordinate is in rage or not
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested

@return - returns if the desired location is in range or not


*/
bool intmatrix1::_inRange(unsigned int aRowIdx, unsigned int aColnIdx) {

	bool rowInRange = true;
	bool colInRange = true;

	if (aRowIdx >= _numOfRows) {
		cout << "The desired row index is out of range" << endl;
		rowInRange = false;
	}

	if (aColnIdx >= _numOfColumns) {
		cout << "The desired column index is out of range" << endl;
		colInRange = false;
	}

	return (rowInRange && colInRange);

}

/*

Prints the matrix
@param aStatement - pointer to character array

@return - N/A

*/
void intmatrix1::print(const char* aStatement) {
	cout << aStatement << endl;  // cout iterates through statement till it finds null pointer

	if (this->isEmpty()) {

		// Empty matrix, break out of function
		cout << "Empty Matrix" << endl;
		return;
	}

	unsigned int elementIdx;

	for (unsigned int i = 0; i < intmatrix1::_numOfRows; i++) {
		for (unsigned int j = 0; j < intmatrix1::_numOfColumns; j++) {
			
			elementIdx = _memoryIdxManager(i, j);
			int num = *(intmatrix1::_ptrToMatrix + elementIdx);

			cout << num << "\t";
		}
		cout << endl;
	}
	return;
}


/*

Delete memory allocated on heap
@param N/A

@return - N/A

*/
void intmatrix1::fini(void) {
	//cout << "Deleting matrix on heap of size: "<< _numOfRows << "x" <<_numOfColumns << endl;
	delete [] intmatrix1::_ptrToMatrix;
}

/*

Gets the number of rows in the matrix 
@param N/A

@return - returns the number of rows

*/
unsigned int  intmatrix1::getNumOfRows() {

	return _numOfRows;
}

/*

Gets the number of columns in the matrix
@param N/A

@return - returns the number of columns

*/
unsigned int  intmatrix1::getNumOfColumns() {

	return _numOfColumns;
}

/*

Function that determines if addition or subtraction is legal
@param aMtrxToOprtWth - matrix with which we are trying to do an operation
@param aTypOfOpr - the type of operation of interest

@return - returns true if the type of operation can be performed with the matrix of interest, else false

*/
bool intmatrix1::_isOperationLegal(intmatrix1 aMtrxToOprtWth, _operation aTypOfOpr){

	int othrMtrxRows = aMtrxToOprtWth.getNumOfRows();
	int othrMtrxColns = aMtrxToOprtWth.getNumOfColumns();

	bool canDoOperation = false;
	if (aMtrxToOprtWth.isEmpty() || this->isEmpty()) {
		cout << "Can't do operation as on matrix is empty" << endl;
		return canDoOperation;   // can't do operation with empty matrix
	}
	if (aTypOfOpr == addition) {
		if ((othrMtrxRows == _numOfRows) && (othrMtrxColns == _numOfColumns)) {

			// number of rows and columns of two matrices are equal, can do operation
			canDoOperation = true;
		}
		else if ((this->isScalar()) || (aMtrxToOprtWth.isScalar())) {
			// other matrix is really a scalar so we can do operation
			canDoOperation = SCALAR_ADD_SUPPORTED ? true : false;
		}
		else {
			cout << "Can't do addition as matrix dimensions don't match" << endl;
		}
	}

	if(aTypOfOpr == multiply){
		if ((othrMtrxRows == _numOfColumns)) {

			// assumes that THIS matrix is on the left side of multiplication so the number of rows of THIS matrix must equal the number of columns of OTHER matrix
			canDoOperation = true;
		}
		else if ( (this->isScalar()) || (aMtrxToOprtWth.isScalar())) {
			// other matrix is really a scalar so we can do operation
			canDoOperation = SCALAR_MULT_SUPPORTED ? true:false;
		}
		else {
			cout << "Can't do multiplication as matrix dimensions aren't correct" << endl;
		}
	}
	return canDoOperation;
}

/*

Performs matrix addition of this Matrix object with aMatrixToAdd object
@param aMatrixToAdd - matrix object which is to be added with this matrix object (if dimensions are right)

@return - returns a intmatrix object with the right dimensions

*/
intmatrix1  intmatrix1::add(intmatrix1 aMatrixToAdd) {

	bool canDoOperation = _isOperationLegal(aMatrixToAdd, addition);

	intmatrix1 resultMatrix;

	if (!canDoOperation) {
		resultMatrix.init(); // create empty matrix and return
		return resultMatrix;
	}

	unsigned int numOfRowsOfResultMatrix = 0;
	unsigned int numOfColnsOfResultMatrix = 0;
	int scalarValueToAdd = 0;
	int elementValue;


	// Determine number of rows and columsn of resulting matrix and initialize resulting matrix to 0
	if (aMatrixToAdd.isScalar()) {
		// other matrix object is a scalar so will be same dimension as this matrix object
		numOfColnsOfResultMatrix = _numOfColumns;
		numOfRowsOfResultMatrix = _numOfRows;
		scalarValueToAdd = aMatrixToAdd.getElement(0,0);
	}
	else if (this->isScalar()) {
		// this matrix object is a scalar so will be same dimension as other matrix object
		numOfColnsOfResultMatrix = aMatrixToAdd.getNumOfColumns();
		numOfRowsOfResultMatrix = aMatrixToAdd.getNumOfRows();
		scalarValueToAdd = this->getElement(0, 0);
		
	}
	else { // neither are scalar matrices
		
		numOfColnsOfResultMatrix = aMatrixToAdd.getNumOfColumns();
		numOfRowsOfResultMatrix = aMatrixToAdd.getNumOfRows();
		
	}

	resultMatrix.init(numOfRowsOfResultMatrix, numOfColnsOfResultMatrix);
	//resultMatrix.print();  // for debug

	for (unsigned int rowIdx = 0; rowIdx < numOfRowsOfResultMatrix; rowIdx++) {
		for (unsigned int colnIdx = 0; colnIdx <numOfColnsOfResultMatrix; colnIdx++) {
			if (aMatrixToAdd.isScalar()) {
				elementValue = this->getElement(rowIdx, colnIdx) + scalarValueToAdd;
			}
			else if (this->isScalar()) {
				elementValue = aMatrixToAdd.getElement(rowIdx, colnIdx) + scalarValueToAdd;
			}
			else {
				elementValue = aMatrixToAdd.getElement(rowIdx, colnIdx) + this->getElement(rowIdx, colnIdx);
			}
			resultMatrix.setElement(rowIdx, colnIdx, elementValue);
		}
	}


	return resultMatrix;
}

/*
Returns if this matrix object is scalar or not
@param N/A

@return - returns the number of columns

*/
bool  intmatrix1::isScalar() {

	if (isEmpty()) {
		return false;
	}

	if ((_numOfRows == 1) && (_numOfColumns == 1)) {
		return true;
	}
	else {
		return false;
	}
}


/*

Performs matrix multiplication of this Matrix object with aMatrixToAdd object. Assumes this matrix object is on the left
@param aMatrixToAdd - matrix object which is to be multiplied with this matrix object (if dimensions are right). aMatrixToAdd assumed to be on the right 

@return - returns a intmatrix object with the right dimensions after performing multiplication operation

*/
intmatrix1  intmatrix1::mult(intmatrix1 aMatrixToAdd) {

	bool canDoOperation = _isOperationLegal(aMatrixToAdd, multiply);

	intmatrix1 resultMatrix;

	if (!canDoOperation) {
		resultMatrix.init(); // create empty matrix and return
		return resultMatrix;
	}

	unsigned int numOfRowsOfResultMatrix = 0;
	unsigned int numOfColnsOfResultMatrix = 0;
	int scalarValueToMultiply = 0;
	int elementValue=0;


	// Determine number of rows and columsn of resulting matrix and initialize resulting matrix to 0
	if (aMatrixToAdd.isScalar()) {
		// other matrix object is a scalar so will be same dimension as this matrix object
		numOfColnsOfResultMatrix = _numOfColumns;
		numOfRowsOfResultMatrix = _numOfRows;
		scalarValueToMultiply = aMatrixToAdd.getElement(0, 0);
	}
	else if (this->isScalar()) {
		// this matrix object is a scalar so will be same dimension as other matrix object
		numOfColnsOfResultMatrix = aMatrixToAdd.getNumOfColumns();
		numOfRowsOfResultMatrix = aMatrixToAdd.getNumOfRows();
		scalarValueToMultiply = this->getElement(0, 0);

	}
	else { // neither are scalar matrices

		numOfColnsOfResultMatrix = aMatrixToAdd.getNumOfColumns();
		numOfRowsOfResultMatrix = _numOfRows;

	}

	resultMatrix.init(numOfRowsOfResultMatrix, numOfColnsOfResultMatrix);
	//resultMatrix.print(); //for debugging
	// Handles scalar multiplication
	if (aMatrixToAdd.isScalar() || this->isScalar()) {
		for (unsigned int rowIdx = 0; rowIdx < numOfRowsOfResultMatrix; rowIdx++) {
			for (unsigned int colnIdx = 0; colnIdx < numOfColnsOfResultMatrix; colnIdx++) {
				if (aMatrixToAdd.isScalar()) {
					elementValue = (this->getElement(rowIdx, colnIdx)) * scalarValueToMultiply;
				}
				else if (this->isScalar()) {
					elementValue = (aMatrixToAdd.getElement(rowIdx, colnIdx))* scalarValueToMultiply;
				}
				else {
					//
				}
				resultMatrix.setElement(rowIdx, colnIdx, elementValue);
			}

		}
	}
	else {   // handles matrix multiplication
		
		elementValue = 0;
		for (unsigned int rowIdx = 0; rowIdx < numOfRowsOfResultMatrix; rowIdx++) {
			for (unsigned int colnIdx = 0; colnIdx < numOfColnsOfResultMatrix; colnIdx++) {
				for (unsigned int iterator = 0; iterator < _numOfColumns; iterator++) {
					elementValue += (this->getElement(rowIdx, iterator)) * (aMatrixToAdd.getElement(iterator,colnIdx) );
				}
				resultMatrix.setElement(rowIdx, colnIdx, elementValue);
				elementValue = 0;
			}

		}
	}





	return resultMatrix;
}