/*
Author: Munadir Ahmed
Date: 4/21/18

*/

// file: magicSquaretest.cpp


/*----------------------------------------------------------------
All includes here
-----------------------------------------------------------------*/
#include "magicSquare.h"


/*----------------------------------------------------------------
test bed
-----------------------------------------------------------------*/
void testbed() {
	magicSquare a;
	
	for (int i = 3; i < 100; i = i + 2) {
		a.printMagicSquare(i);
	}
}

void main() {
	testbed();
	cout << endl << "Press Enter to continue..." << endl;
	cin.get();
}

/*
Generated 3x3 matrix is given below:
6       1       8
7       5       3
2       9       4
Generated 3x3 is a valid COXETER MAGIC SQUARE with magic number 15

Generated 5x5 matrix is given below:
15      8       1       24      17
16      14      7       5       23
22      20      13      6       4
3       21      19      12      10
9       2       25      18      11
Generated 5x5 is a valid COXETER MAGIC SQUARE with magic number 65

Generated 7x7 matrix is given below:
28      19      10      1       48      39      30
29      27      18      9       7       47      38
37      35      26      17      8       6       46
45      36      34      25      16      14      5
4       44      42      33      24      15      13
12      3       43      41      32      23      21
20      11      2       49      40      31      22
Generated 7x7 is a valid COXETER MAGIC SQUARE with magic number 175

Generated 9x9 matrix is given below:
45      34      23      12      1       80      69      58      47
46      44      33      22      11      9       79      68      57
56      54      43      32      21      10      8       78      67
66      55      53      42      31      20      18      7       77
76      65      63      52      41      30      19      17      6
5       75      64      62      51      40      29      27      16
15      4       74      72      61      50      39      28      26
25      14      3       73      71      60      49      38      36
35      24      13      2       81      70      59      48      37
Generated 9x9 is a valid COXETER MAGIC SQUARE with magic number 369

Generated 11x11 matrix is given below:
66      53      40      27      14      1       120     107     94      81      68
67      65      52      39      26      13      11      119     106     93      80
79      77      64      51      38      25      12      10      118     105     92
91      78      76      63      50      37      24      22      9       117     104
103     90      88      75      62      49      36      23      21      8       116
115     102     89      87      74      61      48      35      33      20      7
6       114     101     99      86      73      60      47      34      32      19
18      5       113     100     98      85      72      59      46      44      31
30      17      4       112     110     97      84      71      58      45      43
42      29      16      3       111     109     96      83      70      57      55
54      41      28      15      2       121     108     95      82      69      56
Generated 11x11 is a valid COXETER MAGIC SQUARE with magic number 671

Generated 13x13 is a valid COXETER MAGIC SQUARE with magic number 1105

Generated 15x15 is a valid COXETER MAGIC SQUARE with magic number 1695

Generated 17x17 is a valid COXETER MAGIC SQUARE with magic number 2465

Generated 19x19 is a valid COXETER MAGIC SQUARE with magic number 3439

Generated 21x21 is a valid COXETER MAGIC SQUARE with magic number 4641

Generated 23x23 is a valid COXETER MAGIC SQUARE with magic number 6095

Generated 25x25 is a valid COXETER MAGIC SQUARE with magic number 7825

Generated 27x27 is a valid COXETER MAGIC SQUARE with magic number 9855

Generated 29x29 is a valid COXETER MAGIC SQUARE with magic number 12209

Generated 31x31 is a valid COXETER MAGIC SQUARE with magic number 14911

Generated 33x33 is a valid COXETER MAGIC SQUARE with magic number 17985

Generated 35x35 is a valid COXETER MAGIC SQUARE with magic number 21455

Generated 37x37 is a valid COXETER MAGIC SQUARE with magic number 25345

Generated 39x39 is a valid COXETER MAGIC SQUARE with magic number 29679

Generated 41x41 is a valid COXETER MAGIC SQUARE with magic number 34481

Generated 43x43 is a valid COXETER MAGIC SQUARE with magic number 39775

Generated 45x45 is a valid COXETER MAGIC SQUARE with magic number 45585

Generated 47x47 is a valid COXETER MAGIC SQUARE with magic number 51935

Generated 49x49 is a valid COXETER MAGIC SQUARE with magic number 58849

Generated 51x51 is a valid COXETER MAGIC SQUARE with magic number 66351

Generated 53x53 is a valid COXETER MAGIC SQUARE with magic number 74465

Generated 55x55 is a valid COXETER MAGIC SQUARE with magic number 83215

Generated 57x57 is a valid COXETER MAGIC SQUARE with magic number 92625

Generated 59x59 is a valid COXETER MAGIC SQUARE with magic number 102719

Generated 61x61 is a valid COXETER MAGIC SQUARE with magic number 113521

Generated 63x63 is a valid COXETER MAGIC SQUARE with magic number 125055

Generated 65x65 is a valid COXETER MAGIC SQUARE with magic number 137345

Generated 67x67 is a valid COXETER MAGIC SQUARE with magic number 150415

Generated 69x69 is a valid COXETER MAGIC SQUARE with magic number 164289

Generated 71x71 is a valid COXETER MAGIC SQUARE with magic number 178991

Generated 73x73 is a valid COXETER MAGIC SQUARE with magic number 194545

Generated 75x75 is a valid COXETER MAGIC SQUARE with magic number 210975

Generated 77x77 is a valid COXETER MAGIC SQUARE with magic number 228305

Generated 79x79 is a valid COXETER MAGIC SQUARE with magic number 246559

Generated 81x81 is a valid COXETER MAGIC SQUARE with magic number 265761

Generated 83x83 is a valid COXETER MAGIC SQUARE with magic number 285935

Generated 85x85 is a valid COXETER MAGIC SQUARE with magic number 307105

Generated 87x87 is a valid COXETER MAGIC SQUARE with magic number 329295

Generated 89x89 is a valid COXETER MAGIC SQUARE with magic number 352529

Generated 91x91 is a valid COXETER MAGIC SQUARE with magic number 376831

Generated 93x93 is a valid COXETER MAGIC SQUARE with magic number 402225

Generated 95x95 is a valid COXETER MAGIC SQUARE with magic number 428735

Generated 97x97 is a valid COXETER MAGIC SQUARE with magic number 456385

Generated 99x99 is a valid COXETER MAGIC SQUARE with magic number 485199


Press Enter to continue...




*/