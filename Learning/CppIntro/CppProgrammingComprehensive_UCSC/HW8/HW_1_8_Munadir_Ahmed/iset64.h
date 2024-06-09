/*
Author: Munadir Ahmed
Date: 5/19/18

*/

// file: dealOrNoDeal.h


// All includes
#ifndef ISET64_H   
#define ISET64_H


#include "../util/util.h"

#define MAX_NUM_ELEMENTS 64


// Declaration of class

/*--------------------------------------------------------
class declaration of iset64
----------------------------------------------------------*/
class iset64 {
public:
	iset64();
	iset64(const int aInit[], int aSizeOfArry);
	~iset64();
	iset64(const iset64 &aObjToCopyFrom);
	iset64& operator= (const iset64 &rhs);
	friend ostream& operator<<(ostream& o, const iset64& a);

	friend iset64 operator+(const iset64& a, const iset64& b);
	friend iset64 operator+(const iset64& a, int b);
	friend iset64 operator+(int b, const iset64& a);

	friend iset64& operator+=(iset64& a, iset64& b);
	friend iset64& operator+=(iset64& a, int b);
	friend iset64& operator+=(int b, iset64& a);

	friend iset64 operator-(const iset64& a, const iset64& b);
	friend iset64 operator-(const iset64& a, int b);
	friend iset64 operator-(int b, const iset64& a);

	friend iset64& operator-=(iset64& a, iset64& b);
	friend iset64& operator-=(iset64& a, int b);
	friend iset64& operator-=(int b, iset64& a);

	friend iset64 operator*(const iset64& a, const iset64& b);
	friend iset64 operator*(const iset64& a, int b);
	friend iset64 operator*(int b, const iset64& a);

	friend iset64& operator*=(iset64& a, iset64& b);
	friend iset64& operator*=(iset64& a, int b);
	friend iset64& operator*=(int b, iset64& a);

	friend bool operator==(const iset64& a, const iset64& b);
	friend bool operator==(const iset64& a, int b);
	friend bool operator==(int b, const iset64& a);

	friend iset64 operator~(const iset64& a);

	friend iset64& operator++(iset64& a);
	friend iset64 operator++(iset64& a,int i);

	friend iset64& operator--(iset64& a);
	friend iset64 operator--(iset64& a, int i);

	friend bool operator!(const iset64& a);
	operator bool();


	static void set_display(bool aSetDspOn);

private:
	void _release();
	void _copy(const iset64 &rhs);
	
	static bool _show;

	bool _inSet(int aNumToCheck, int& idx)const ;
	void _add(int aNumToAdd);
	void _remove(int aNumToRemove);

	int* _ptrToSet;
	int _sizeOfSet;
};

#endif // !ISET64_H