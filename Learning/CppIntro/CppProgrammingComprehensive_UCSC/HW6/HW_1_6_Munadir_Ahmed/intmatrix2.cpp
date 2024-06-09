/*
Author: Munadir Ahmed
Date: 5/12/18

*/

// file: intmatrix2.cpp
#define SCALAR_MULT_SUPPORTED 1
#define SCALAR_ADD_SUPPORTED 0

#include "intmatrix2.h"

// Definition of operations in intmatrixph1 class
/*

Constructor of intmatrix2 class
@param aNumOfRows - number of rows in the matrix
@param aNumOfColns - number of columns in the matrix
@param aDefaultValue - what each element of the matrix should be filled with

@return - N/A

*/
intmatrix2::intmatrix2(unsigned int aNumOfRows, unsigned int aNumOfColns, int aDefaultValue)
		:_numOfRows(aNumOfRows), _numOfColumns(aNumOfColns),_matrix(nullptr), _totalNumOfElements(aNumOfRows * aNumOfColns){
	//cout << "Calling Constructor of intmatrix2" << endl;   //debug helper

	if (_totalNumOfElements == 0) {   // invalid matrix
		_matrix = nullptr;
		_numOfRows = 0;
		_numOfColumns = 0;
	}
	else {
		_matrix = new int* [aNumOfRows];
		for (unsigned int i = 0; i < aNumOfRows; i++) {
			_matrix[i] = new int[aNumOfColns];
		}
	}

	

	//Initialize to default value
	for (unsigned int i = 0; i < aNumOfRows; i++) {
		for (unsigned int j = 0; j < aNumOfColns; j++) {

			_matrix[i][j] = aDefaultValue;
		}
	}
}

/*

Constructor for string input

@return - N/A

*/
intmatrix2::intmatrix2(const char* aStringRepOfMatrix) {
	_parseStringMatrix(aStringRepOfMatrix);
	return;
}

/*

Destructor for intmatrix2
@param N/A

@return - N/A

*/
intmatrix2::~intmatrix2(void) {

	//cout << "Calling Destructor"<<endl;   //debug helper
	_release();

}

/*

Releases memory on the heap.


@return - N/A

*/
void intmatrix2::_release(void) {
	//cout << "in release heap memory helper function" << endl;   // debug 
	if (_matrix != nullptr) {
		//cout << "Deleting matrix on heap of size: " << _numOfRows << "x" << _numOfColumns << endl;
		for (unsigned int rowIdx = 0; rowIdx < _numOfRows; rowIdx++) {
			delete[] _matrix[rowIdx];   // delete each of the rows stored on the heap memory
		}
		delete[] _matrix;   // delete the array of pointer to rows on heap
	}
	else {
		//cout << "No memory stored on heap for matrix of size: " << _numOfRows << "x" << _numOfColumns << endl;
	}

	return;
}
/*

Copy constructor for intmatrix2
@param aMatrixToCopyFrom - matrix to copy from. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).

@return - N/A

*/
intmatrix2::intmatrix2(const intmatrix2& aMatrixToCopyFrom) {
	//cout << "In copy constructor of intmatrix2" << endl;   //debug helper
	_copy(aMatrixToCopyFrom);

}

/*

Equal operator overloading function for THIS class
@param aMtrxOnRHS - matrix on right hand side. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).

@return - returns reference to THIS object

*/
intmatrix2& intmatrix2::operator=(const intmatrix2& aMtrxOnRHS) {
	
	// guarding against equating to self (otherwise _release would cause crash). THIS is a pointer holding an address. So checking to see if the address helf by THIS pointer is the same as the address of aMtrxOnRHS object. If yes, it must be the same object as THIS (e.g. a=a)
	if (this != &aMtrxOnRHS) {   
		_release();   // first release memory on heap
		_copy(aMtrxOnRHS);   // then copy all aspects of aMtrxOnRHS to THIS
	}

	return *this;   // returns THIS object (* as this is pointer to object, so dereferencing)
}

/*
Equal operator overloading function for THIS class
@param aMtrxOnRHS - matrix on right hand side.passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).

@return - returns reference to THIS object

*/
ostream& operator<<(ostream & o, const intmatrix2 & aMtrx)
{

	if (aMtrx.isEmpty()) {

		// Empty matrix, break out of function
		o << "Empty Matrix" << endl;
		return o;
	}

	for (unsigned int rowIdx = 0; rowIdx < aMtrx._numOfRows; rowIdx++) {
		for (unsigned int colIdx = 0; colIdx < aMtrx._numOfColumns; colIdx++) {


			int num = aMtrx._matrix[rowIdx][colIdx];

			o << num << "\t";
		}
		o << endl;
	}
	return o;
}


/*

Copy helper functon
@param aMatrixToCopyFrom - matrix to copy from. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).

@return - N/A

*/
void intmatrix2::_copy(const intmatrix2& aMatrixToCopyFrom) {
	//cout << "In copy helper function of intmatrix2" << endl;
	
	_numOfColumns = aMatrixToCopyFrom._numOfColumns;
	_numOfRows = aMatrixToCopyFrom._numOfRows;
	_totalNumOfElements = aMatrixToCopyFrom._totalNumOfElements;


	if (_totalNumOfElements == 0) {   // invalid matrix
		_matrix = nullptr;
		_numOfRows = 0;
		_numOfColumns = 0;

		return;
	}
	else {
		_matrix = new int*[_numOfRows];
		for (unsigned int i = 0; i < _numOfRows; i++) {
			_matrix[i] = new int[_numOfColumns];
		}
	}

	for (unsigned int rowIdx = 0; rowIdx < _numOfRows; rowIdx++) {
		for (unsigned int colnIdx = 0; colnIdx <_numOfColumns; colnIdx++) {
			this->_matrix[rowIdx][colnIdx] = aMatrixToCopyFrom._matrix[rowIdx][colnIdx];   // class write has access to all private members
		}
	}

	return;
}

/*

Initializes an empty matrix on the heap
@param aNumOfRows - number of rows in the matrix
@param aNumOfColns - number of columns in the matrix
@param aDefaultValue - what each element of the matrix should be filled with

@return - N/A

*/
void intmatrix2::_parseStringMatrix(const char* aStringRepOfMatrix) {
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
			numberStored = numberStored * 10 + (tempChar - '0');
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
	
	if (!validMatrix || (MaxNumOfElements == 0)) {
		intmatrix2::_numOfColumns = 0;
		intmatrix2::_numOfRows = 0;
		intmatrix2::_totalNumOfElements = 0;
		intmatrix2::_matrix = nullptr;

		delete[] tempHeapMemory;
		return;
	}

	intmatrix2::_numOfColumns = numOfColnsInRow0;
	intmatrix2::_numOfRows = numOfRows;

	intmatrix2::_totalNumOfElements = (intmatrix2::_numOfColumns * intmatrix2::_numOfRows);   // +1 to hold the null character

	_matrix = new int*[_numOfRows];
	for (unsigned int i = 0; i < _numOfRows; i++) {
		_matrix[i] = new int[_numOfColumns];
	}
	unsigned int tempHeapMemoryIdx = 0;
	//Initialize to default value
	for (unsigned int i = 0; i < _numOfRows; i++) {
		for (unsigned int j = 0; j < _numOfColumns; j++) {

			_matrix[i][j] = *(tempHeapMemory + tempHeapMemoryIdx);
			tempHeapMemoryIdx++;
		}
	}

	delete[] tempHeapMemory;
	return;
}

/*

Sets the element in the desired coordinate to the desired value. This function changes THIS object.
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested
$param aValue - value to be set in the desired location

@return - N/A

*/
void intmatrix2::setElement(unsigned int aRowIdx, unsigned int aColnIdx, int aValue) {

	if (isEmpty()) {
		// nothing to store as it is an empty matrix
		return;
	}
	if (!(this->_inRange(aRowIdx, aColnIdx))) {
		// nothing to store as it is desired location is out of range
		return;
	}

	

	_matrix[aRowIdx][aColnIdx] = aValue; // is not empty and in range

	return;
}


/*

Gets the element in the desired coordinate to the desired value. This function DOES NOT changes THIS object.
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested

@return - returns the element at the desired location

*/
int intmatrix2::getElement(unsigned int aRowIdx, unsigned int aColnIdx) const {

	if (this->isEmpty()) {
		// nothing to store as it is an empty matrix
		return 0;
	}

	if (!(this->_inRange(aRowIdx, aColnIdx))) {
		// nothing to store as it is an empty matrix
		return 0;
	}

	int valueStored;

	valueStored = _matrix[aRowIdx][aColnIdx]; // gets the value at the memory index

	return valueStored;
}

/*

Returns if the matrix is empty or not. This function DOES NOT changes THIS object.
@param N/A

@return - N/A

*/
bool intmatrix2::isEmpty() const {

	if (_totalNumOfElements == 0) {
		// nothing to store as it is an empty matrix
		return true;
	}
	else {
		return false;
	}
}

/*

Returns if this matrix is equal to the matrix object passed to function. This function DOES NOT changes THIS object.
@param aMatrixToCompare - matrix to compare. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).

@return - true if both matrices are equal, false otherwise

*/
bool intmatrix2::isEqual(const intmatrix2& aMatrixToCompare) const {
	int othrMtrxRows = aMatrixToCompare._numOfRows;    // class writer has access to private members

	int othrMtrxColns = aMatrixToCompare._numOfColumns;    // class writer has access to private members

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
			valueAtCoordinate_OthrMatrix = aMatrixToCompare._matrix[rowIdx][colnIdx];
			valueAtCoordinate_ThisMatrix = _matrix[rowIdx][colnIdx];
			if (valueAtCoordinate_OthrMatrix != valueAtCoordinate_ThisMatrix) {
				return false;
			}
		}
	}

	return true;  // passed all other tests, must be equal
}

/*

Determines if a coordinate is in rage or not. This function DOES NOT changes THIS object.
@param aRowIdx - row index of the matrix being requested
@param aColnIdx - column index of the matrix being requested

@return - returns if the desired location is in range or not


*/
bool intmatrix2::_inRange(unsigned int aRowIdx, unsigned int aColnIdx)const {

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

Gets the number of rows in the matrix. This function DOES NOT changes THIS object.
@param N/A

@return - returns the number of rows

*/
unsigned int  intmatrix2::getNumOfRows() const {

	return _numOfRows;
}

/*

Gets the number of columns in the matrix. This function DOES NOT changes THIS object.
@param N/A

@return - returns the number of columns

*/
unsigned int  intmatrix2::getNumOfColumns() const {

	return _numOfColumns;
}

/*

Function that determines if addition or subtraction is legal
@param aMtrxToOprtWth - matrix with which we are trying to do an operation. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).
@param aTypOfOpr - the type of operation of interest

@return - returns true if the type of operation can be performed with the matrix of interest, else false

*/
bool intmatrix2::_isOperationLegal(const intmatrix2& aMtrxToOprtWth, _operation aTypOfOpr) const {

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

	if (aTypOfOpr == multiply) {
		if ((othrMtrxRows == _numOfColumns)) {

			// assumes that THIS matrix is on the left side of multiplication so the number of rows of THIS matrix must equal the number of columns of OTHER matrix
			canDoOperation = true;
		}
		else if ((this->isScalar()) || (aMtrxToOprtWth.isScalar())) {
			// other matrix is really a scalar so we can do operation
			canDoOperation = SCALAR_MULT_SUPPORTED ? true : false;
		}
		else {
			cout << "Can't do multiplication as matrix dimensions aren't correct" << endl;
		}
	}
	return canDoOperation;
}

/*

Performs matrix addition of this Matrix object with aMatrixToAdd object. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).
@param aMatrixToAdd - matrix object which is to be added with this matrix object (if dimensions are right)

@return - returns a intmatrix object with the right dimensions (copy constructor is called before this local object is destroyed)

*/
intmatrix2  intmatrix2::add(const intmatrix2& aMatrixToAdd) const {

	bool canDoOperation = _isOperationLegal(aMatrixToAdd, addition);

	if (!canDoOperation) {
		intmatrix2 EmptyMatrix;
		return EmptyMatrix;
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
		scalarValueToAdd = aMatrixToAdd.getElement(0, 0);
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

	// create local object to hold value of addition
	intmatrix2 resultMatrix(numOfRowsOfResultMatrix, numOfColnsOfResultMatrix);

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


	return resultMatrix;   // copy constructor is called before this object is destroyed
}

/*
Returns if this matrix object is scalar or not
@param N/A

@return - returns the number of columns

*/
bool  intmatrix2::isScalar() const {

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
@param aMatrixToMultiplyWith - matrix object which is to be multiplied with this matrix object (if dimensions are right). aMatrixToAdd assumed to be on the right. passed by const reference. const as we don't want this function to change the other matrix. passed by reference as otherwise (i.e. with pass by value) the constructor will be called for the local object in the function call (bitwise copy, i.e. shallow copy -> will lead to crash).

@return - returns a intmatrix object with the right dimensions after performing multiplication operation (copy constructor is called before this local object is destroyed)

*/
intmatrix2  intmatrix2::mult(const intmatrix2& aMatrixToMultiplyWith)const {

	bool canDoOperation = _isOperationLegal(aMatrixToMultiplyWith, multiply);

	if (!canDoOperation) {
		intmatrix2 emptyMatrix; // create empty matrix and return
		return emptyMatrix;
	}

	unsigned int numOfRowsOfResultMatrix = 0;
	unsigned int numOfColnsOfResultMatrix = 0;
	int scalarValueToMultiply = 0;
	int elementValue = 0;


	// Determine number of rows and columsn of resulting matrix and initialize resulting matrix to 0
	if (aMatrixToMultiplyWith.isScalar()) {
		// other matrix object is a scalar so will be same dimension as this matrix object
		numOfColnsOfResultMatrix = _numOfColumns;
		numOfRowsOfResultMatrix = _numOfRows;
		scalarValueToMultiply = aMatrixToMultiplyWith.getElement(0, 0);
	}
	else if (this->isScalar()) {
		// this matrix object is a scalar so will be same dimension as other matrix object
		numOfColnsOfResultMatrix = aMatrixToMultiplyWith.getNumOfColumns();
		numOfRowsOfResultMatrix = aMatrixToMultiplyWith.getNumOfRows();
		scalarValueToMultiply = this->getElement(0, 0);

	}
	else { // neither are scalar matrices

		numOfColnsOfResultMatrix = aMatrixToMultiplyWith.getNumOfColumns();
		numOfRowsOfResultMatrix = _numOfRows;

	}

	intmatrix2 resultMatrix(numOfRowsOfResultMatrix, numOfColnsOfResultMatrix);

	// Handles scalar multiplication
	if (aMatrixToMultiplyWith.isScalar() || this->isScalar()) {
		for (unsigned int rowIdx = 0; rowIdx < numOfRowsOfResultMatrix; rowIdx++) {
			for (unsigned int colnIdx = 0; colnIdx < numOfColnsOfResultMatrix; colnIdx++) {
				if (aMatrixToMultiplyWith.isScalar()) {
					elementValue = (this->getElement(rowIdx, colnIdx)) * scalarValueToMultiply;
				}
				else if (this->isScalar()) {
					elementValue = (aMatrixToMultiplyWith.getElement(rowIdx, colnIdx))* scalarValueToMultiply;
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
					elementValue += (this->getElement(rowIdx, iterator)) * (aMatrixToMultiplyWith.getElement(iterator, colnIdx));
				}
				resultMatrix.setElement(rowIdx, colnIdx, elementValue);
				elementValue = 0;
			}

		}
	}

	return resultMatrix; // copy constructor is called before this local object is destroyed
}