/*----------------------------------------------------------------
This file test dealOrNoDeal objects
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
All includes here
-----------------------------------------------------------------*/
#include "dealOrNoDeal.h"

/*----------------------------------------------------------------
test init and fini
-----------------------------------------------------------------*/
void test_suitcase() {
	suitcase a;
	suitcase b(20);

	suitcase c = b;

	suitcase d;

	d = a;

	suitcase* ptrToCase = nullptr;
	
	ptrToCase = new suitcase[10];

	a.setAmount(11);
	delete[] ptrToCase;
}

void test_suitcasecollection() {
	suitcaseCollection a(26);
	
	for (unsigned int i = 0; i < 26; i++) {
		a.setCaseAmntAndNum(i, i + 1);
	}
	cout << a.getAmountInCase(24) << endl;

	a.removeSuitcase(23);
	a.removeSuitcase(23);


}

void test_dealornodeal() {
	//dealOrNoDeal c;  // uncomment to play user interacting game

	dealOrNoDeal a(dealOrNoDeal::typeOfGame::playUntilBroke);
	cout << endl;
	cout << "Press enter key to continue"<<endl;
	cin.get();
	dealOrNoDeal b(dealOrNoDeal::typeOfGame::playUntilMillionaire);


}


/*----------------------------------------------------------------
test bed
-----------------------------------------------------------------*/
void testbed() {

	//test_suitcase();
	//test_suitcasecollection();
	test_dealornodeal();
}

/*----------------------------------------------------------------
main
-----------------------------------------------------------------*/
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	testbed();
	cin.get();
	return 0;
}