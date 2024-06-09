/*
Author: Munadir Ahmed
Date: 5/27/18

*/


#include "iset64.h"

bool iset64::_show = false;

// Function Definition//
/*
Constructor of iset64 class
@param - N/A

@return - N/A

*/
iset64::iset64() : _ptrToSet(nullptr), _sizeOfSet(0){

	if (iset64::_show) {
		cout << "in constructor of iset64 class" << endl;
	}
	_ptrToSet = new int[MAX_NUM_ELEMENTS];
	_sizeOfSet = 0;

	for (int i = 0; i < MAX_NUM_ELEMENTS; i++) {
		_ptrToSet[i] = -1;  // set the rest to -1
	}
};

/*
Constructor overload of iset64 class
@param - N/A

@return - N/A

*/
iset64::iset64(const int aInit[], int aSizeOfArry) : _ptrToSet(nullptr), _sizeOfSet(0) {
	if (iset64::_show) {
		cout << "in constructor of iset64 class" << endl;
	}
	_ptrToSet = new int[MAX_NUM_ELEMENTS];

	_sizeOfSet = aSizeOfArry;
	for (int i = 0; i < _sizeOfSet; i++) {
		_ptrToSet[i] = aInit[i];
	}
	for (int i = _sizeOfSet; i < MAX_NUM_ELEMENTS; i++) {
		_ptrToSet[i] = -1;  // set the rest to -1
	}
};

/*
Destructor of iset64 class
@param - N/A

@return - N/A

*/
iset64::~iset64() {
	if (iset64::_show) {
		cout << "in destructor of iset64 class" << endl;
	}
	_release();
};

/*
Copy constructor of iset64 class
@param - N/A

@return - N/A

*/
iset64::iset64(const iset64 &rhs) {
	if (iset64::_show) {
		cout << "in copy constructor of iset64 class" << endl;
	}
	_copy(rhs);
};

/*
Equal Operator overloading of class iset64
@param rhs - constant reference to object this object is being referred to

@return - reference to iset64 object (if returned by value the copy constructor will be called)

*/
iset64& iset64::operator= (const iset64 &rhs) {
	if (iset64::_show) {
		cout << "in equal operator of iset64 class" << endl;
	}
	if (this != &rhs) {
		// only if the address of THIS object isn't equal to the address of the object on the RHS
		_release();
		_copy(rhs);
	}

	return *this;

};

/*
Minus Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64 operator-(const iset64& a, const iset64& b) {
	iset64 tempVar(a); // copy a into temp variable
	
	for (int i = 0; i < b._sizeOfSet; i++) {
		tempVar._remove(b._ptrToSet[i]);
	}
	return tempVar;
};

/*
Minus Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64 operator-(const iset64& a, int b) {
	iset64 tempVar(a); // copy a into temp variable
	tempVar._remove(b);

	return tempVar;

};

/*
Minus Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64 operator-(int b, const iset64& a) {
	iset64 tempVar(a); // copy a into temp variable
	tempVar._remove(b);

	return tempVar;

};


/*
Add Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64 operator+(const iset64& a, const iset64& b) {
	iset64 tempVar(a); // copy a into temp variable

	for (int i = 0; i < b._sizeOfSet; i++) {
		tempVar._add(b._ptrToSet[i]);
	}
	return tempVar;
};

/*
Add Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64 operator+(const iset64& a, int b) {
	iset64 tempVar(a); // copy a into temp variable
	tempVar._add(b);

	return tempVar;

};

/*
Add Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64 operator+(int b, const iset64& a) {
	iset64 tempVar(a); // copy a into temp variable
	tempVar._add(b);

	return tempVar;

};

/*
Add Equal Operator overloading of class iset64
@param a - reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by reference
*/
iset64& operator+=(iset64& a, iset64& b) {
	for (int i = 0; i < b._sizeOfSet; i++) {
		a._add(b._ptrToSet[i]);
	}
	return a;
};

/*
Add Equal Operator overloading of class iset64
@param a - reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by reference
*/
iset64& operator+=(iset64& a, int b) {
	a._add(b);

	return a;

};

/*
Add Operator overloading of class iset64
@param a - reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by reference
*/
iset64& operator+=(int b, iset64& a) {
	a._add(b);

	return a;

};

/*
Minus Equal Operator overloading of class iset64
@param a - reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by reference
*/
iset64& operator-=(iset64& a, iset64& b) {
	for (int i = 0; i < b._sizeOfSet; i++) {
		a._remove(b._ptrToSet[i]);
	}
	return a;
};

/*
Minus Equal Operator overloading of class iset64
@param a - reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by reference
*/
iset64& operator-=(iset64& a, int b) {
	a._remove(b);

	return a;

};

/*
Minus Equal Operator overloading of class iset64
@param a - reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by reference
*/
iset64& operator-=(int b, iset64& a) {
	a._remove(b);

	return a;

};

/*
Multiplication Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64 operator*(const iset64& a, const iset64& b) {
	iset64 tempVar; // copy a into temp variable
	bool inSetA = false;
	int tempIdx = -1;

	for (int i = 0; i < b._sizeOfSet; i++) {
		inSetA = a._inSet(b._ptrToSet[i], tempIdx);
		if (inSetA) {
			tempVar._add(b._ptrToSet[i]);
		}
	}
	return tempVar;
};

/*
Multiplication Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64 operator*(const iset64& a, int b) {
	iset64 tempVar; // copy a into temp variable
	bool inSetA = false;
	int tempIdx = -1;
	inSetA = a._inSet(b,tempIdx);

	if (inSetA) {
		tempVar._add(b);
	}
	return tempVar;

};

/*
Multiplication Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64 operator*(int b, const iset64& a) {
	iset64 tempVar; // copy a into temp variable
	bool inSetA = false;
	int tempIdx = -1;
	inSetA = a._inSet(b, tempIdx);

	if (inSetA) {
		tempVar._add(b);
	}
	return tempVar;

};

/*
*= Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64& operator*=(iset64& a, iset64& b) {
	bool inSetB = false;
	int tempIdx = -1;

	for (int i = 0; i < a._sizeOfSet; i++) {
		inSetB = b._inSet(a._ptrToSet[i], tempIdx);
		if (!inSetB) {
			a._remove(a._ptrToSet[i]);
		}
	}
	return a;
};

/*
*= Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64& operator*=(iset64& a, int b) {
	bool inSetA = false;
	int tempIdx = -1;
	inSetA = a._inSet(b, tempIdx);
	a._release();
	a._ptrToSet = new int[MAX_NUM_ELEMENTS];

	if (inSetA) {
		a._add(b);
	}
	return a;
};

/*
*= Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
iset64& operator*=(int b, iset64& a) {
	bool inSetA = false;
	int tempIdx = -1;
	inSetA = a._inSet(b, tempIdx);
	a._release();
	a._ptrToSet = new int[MAX_NUM_ELEMENTS];

	if (inSetA) {
		a._add(b);
	}
	return a;

};

/*
Equal Equal  Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant reference to object of type iset64

@return - iset64 object returned by value
*/
bool operator==(const iset64& a, const iset64& b) {
	int tempIdx = 0;
	bool inSetA;
	for (int i = 0; i < b._sizeOfSet; i++) {
		inSetA = a._inSet(b._ptrToSet[i], tempIdx);
		if (!inSetA) {
			return false;
		}
	}
	return true;
};

/*
Equal Equal  Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
bool operator==(const iset64& a, int b) {
	int tempIdx = 0;
	bool inSetA = a._inSet(b, tempIdx);

	return inSetA;

};

/*
Equal Equal Operator overloading of class iset64
@param a - constant reference to object of type iset64
@param b - constant integer

@return - iset64 object returned by value
*/
bool operator==(int b, const iset64& a) {
	int tempIdx = 0;
	bool inSetA = a._inSet(b, tempIdx);

	return inSetA;

};

/*
Complement Operator overloading of class iset64
@param a - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64 operator~(const iset64& a) {
	iset64 tempVar;
	bool inSetA;
	tempVar._sizeOfSet = MAX_NUM_ELEMENTS;
	for (int i = 0; i < tempVar._sizeOfSet; i++) {
		tempVar._ptrToSet[i] = i;
	}
	for (int i = 0; i < a._sizeOfSet; i++) {
		tempVar._remove(a._ptrToSet[i]);
	}

	return tempVar;

};

/*
Pre increment overloading of class iset64
@param a - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64& operator++(iset64& a) {
	for (int i = 0; i < a._sizeOfSet; i++) {
		a._ptrToSet[i] = ((a._ptrToSet[i] + 1) % MAX_NUM_ELEMENTS);
	}

	return a;

};

/*
Post increment overloading of class iset64
@param a - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64 operator++(iset64& a, int i) {
	iset64 tempSet(a);
	for (int i = 0; i < a._sizeOfSet; i++) {
		a._ptrToSet[i] = ((a._ptrToSet[i] + 1) % MAX_NUM_ELEMENTS);
	}
	return tempSet;

};

/*
Pre increment overloading of class iset64
@param a - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64& operator--(iset64& a) {
	for (int i = 0; i < a._sizeOfSet; i++) {
		a._ptrToSet[i] = ((a._ptrToSet[i] - 1) % MAX_NUM_ELEMENTS);
		if (a._ptrToSet[i] < 0) {
			a._ptrToSet[i] = a._ptrToSet[i] + MAX_NUM_ELEMENTS;
		}
	}

	return a;

};

/*
Post increment overloading of class iset64
@param a - constant reference to object of type iset64

@return - iset64 object returned by value
*/
iset64 operator--(iset64& a, int i) {
	iset64 tempSet(a);
	for (int i = 0; i < a._sizeOfSet; i++) {
		a._ptrToSet[i] = ((a._ptrToSet[i] - 1) % MAX_NUM_ELEMENTS);
		if (a._ptrToSet[i] < 0) {
			a._ptrToSet[i] = a._ptrToSet[i] + MAX_NUM_ELEMENTS;
		}
	}
	return tempSet;

};
/*
Not Operator overloading of class iset64
@param a - constant reference to object of type iset64

@return - true or false
*/
bool operator!(const iset64& a) {
	if (a._sizeOfSet == 0) {
		return true;
	}
	else {
		return false;
	}

};

/*
() type conversion to bool
@param N/A
@return - true or false
*/
iset64::operator bool(){
	if (this->_sizeOfSet == 0) {
		return false;
	}
	else {
		return true;
	}

};

/*
prints the remaining cases in list
@param o - reference to ostream object
@param a - reference to iset64 object

@return - N/A

*/
ostream& operator<<(ostream& o, const iset64& a) {
	
	if (a._sizeOfSet == 0) {
		o << "{}" << endl;
		return o;
	}
	o << "{";
	for (int i = 0; i < a._sizeOfSet; i++) {
		o << a._ptrToSet[i];
		if (i == (a._sizeOfSet - 1)) {
			o << "}" << "; size of set is: " << a._sizeOfSet << endl; 
		}
		else {
			o << ", ";
		}
	}
	return o;


};

/*
public function to allow classes to enable or disable debug print statements
@param aSetDspOn - turn on or off display

@return - N/A

*/
void iset64::set_display(bool aSetDspOn) {
	iset64::_show = aSetDspOn;
}

//Private Functions
/*
 function that releases the stored memory space on the heap
@param - N/A

@return - N/A

*/
void iset64::_release() {
	if (iset64::_show) {
		cout << " in release helper function" << endl;
	}
	delete[] _ptrToSet;
	_sizeOfSet = 0;
};


/*
helper function that copies the object on the left to THIS object
@param aObjToCopyFrom - constant reference to object that copies the features of the object to THIS object without chaging the object.

@return - N/A

*/
void iset64::_copy(const iset64& aObjToCopyFrom) {
	if (iset64::_show) {
		cout << " in copy helper function" << endl;
	}
	iset64& thisOj = *this;  // renaming using reference
	thisOj._ptrToSet = new int[MAX_NUM_ELEMENTS];

	thisOj._sizeOfSet = aObjToCopyFrom._sizeOfSet;
	for (int i = 0; i < thisOj._sizeOfSet; i++) {
		thisOj._ptrToSet[i] = aObjToCopyFrom._ptrToSet[i];
	}
	for (int i = thisOj._sizeOfSet; i < MAX_NUM_ELEMENTS; i++) {
		_ptrToSet[i] = -1;  // set the rest to -1
	}

	
	
};

/*
helper function that deterines if number is within the set
@param aNumToCheck - interger to check and see if in set.

@return - returns true if number is in set, false otherwise

*/
bool iset64::_inSet(int aNumToCheck, int& idx)const {
	idx = -1;
	for (int i = 0; i < _sizeOfSet; i++) {
		if (_ptrToSet[i] == aNumToCheck) {
			idx = i;
			return true;
		}
	}
	return false;
}

/*
helper function that checks to see if number is in list and adds to the list
@param aNumAdd - interger to add to set.

@return - N/A
*/
void iset64::_add(int aNumAdd) {
	int tempIdx;
	bool numInSet = _inSet(aNumAdd, tempIdx);
	if (numInSet) {
		return;
	}
	_ptrToSet[_sizeOfSet] = aNumAdd;
	++_sizeOfSet;	
}

/*
helper function that removes number from set if it is there
@param aNumAdd - interger to remove to set.

@return - N/A
*/
void iset64::_remove(int aNumToRemove) {
	int tempIdx;
	bool numInSet = _inSet(aNumToRemove, tempIdx);
	if (!numInSet) {
		return;
	}
	_ptrToSet[tempIdx] = _ptrToSet[(_sizeOfSet-1)];   // replace index with last number in set
	_ptrToSet[(_sizeOfSet - 1)] = -1;   // set last number to -1
	--_sizeOfSet;
	
}