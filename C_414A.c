/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022

*/
#include "karateka.h"

int D_DE66 = 1;

/*random0*/
int C_414A()
{
	unsigned bp02;

	getSetSeed(0, &bp02);/*set/get random seed*/
	bp02 = (bp02 * 3677 + 3) % 0x8000;/*3677(0xe5d) is prime*/
	getSetSeed(1, &bp02);/*set/get random seed*/

	return bp02;
}

/*set random seed*/
void __4186(bp04)
int bp04;
{
	getSetSeed(1, &bp04);/*set/get random seed*/
}

/*set/get random seed*/
/*4198*/void getSetSeed(bp06, bp08)
char bp06;
int *bp08;
{
	if(bp06 == 0)
		*bp08 = D_DE66;
	else
		D_DE66 = *bp08;
}
