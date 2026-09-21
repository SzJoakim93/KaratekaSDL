/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022

*/
#include "karateka.h"

/*---- ----*/
char D_BF92[0x29E];
char *D_C230 = D_BF92;
char *D_C232 = D_BF92;
char *D_C234 = D_BF92;
int D_C236 = 0;
char *D_C238[7];
char *D_C246[7];
char *D_C254[7];
/*-- --*/
int D_C262 = 0;
int D_C264[0x2A];/*indexes in script D_C2B8*/
char D_C2B8[0x9C0];
/*-- --*/
int D_CC78 = 0;
int D_CC7A[0x2A];/*indexes in script D_CCCE*/
char D_CCCE[0x75E];
/*-- --*/
/*d42c	"allpal"*/
/*d433	0*/
/*---- ----*/

/*load 'allpal' script*/
C_19F5()
{
	char dummy[4];

	load_animation_script(/*D42C*/"allpal", D_C2B8);
	C_19BD();/*make indexes for D_C2B8*/
	D_015E = 0;
	km_base = 0;
	D_0162 = 0;
	ks_base = 0;
	D_0172 = 0;
	load_sprite_assets(5);/*load ks/km files*/
}

C_1A35(bp10)
int bp10;
{
	int bp04;
	int bp06;
	int bp08;
	int bp0a;

	for(bp0a = bp04 = bp06 = 0; bp0a < bp10; bp04 ++, bp0a ++) {
		D_C238[bp0a] = &(D_BF92[bp06]);
/*1A6C*/
		while(D_BF92[bp04] != 0xff) {/*else 1AD4*/
			if(D_BF92[bp04] == 0x12) {
				D_C246[bp0a] = &(D_BF92[bp06]);
				bp04 ++;
			}
			if(D_BF92[bp04] == 0xff)
				break;
			for(bp08 = 1; bp08 < 5; bp08 ++)
				D_BF92[bp06 ++] = D_BF92[bp04 + bp08];
			bp04 += 5;
		}/*end while*/
		D_C254[bp0a] = &(D_BF92[bp06]);
	}/*end for*/
	D_C230 = D_C238[0];
	D_C232 = D_C246[0];
	D_C234 = D_C254[0];
}

load_level_assets(bp08)
int bp08;
{
	int dummy;

	load_sprite_assets(bp08);/*load ks/km files*/
	if(bp08 < 4) {
		load_animation_script(D_021C[bp08], D_BF92);
		if(bp08 < 2)
			C_1A35(1);
		else
			C_1A35(6);
	}
	/*-- --*/
	if(bp08 == 2)
		CodeChecksum();
	/*-- --*/
	C_1B6E(bp08);
	WaitNoKey();
}

C_1B6E(bp0a)
int bp0a;
{
	int bp04;

	D_0158 = -1;
	D_00E4 = 0;
	D_012E = 0;
	if(bp0a == 2 || bp0a == 4)
		D_012E = 1;
	D_00EE = 1;
	D_B9BA = -1;
	D_BB65 = D_02E6[bp0a];
	D_0102 = D_02F0[bp0a];
	D_0106 =
	D_0104 = D_02FA[bp0a];
	D_0108 = D_0304[bp0a];
	D_010A = D_030E[bp0a];
	D_0100 = D_0318[bp0a];
	C_1C9F(bp0a);
	if(bp0a == 3) {/*else 1C34*/
		D_0132[0] =
		D_0132[1] =
		D_0132[3] =
		D_0132[4] =
		D_0132[5] = 1;
		D_0132[6] = 0;
		D_0132[7] = 3;
		for(bp04 = 2; bp04 < 7; bp04 ++)
			D_0142[bp04] = 0;
		D_0142[1] = 1;
	}
	if(bp0a != 1 && bp0a != 4) {
		D_C262 = D_C264[0x13];/*C28A*/
		D_010C = 0x41;
	}
	D_00EE = 1;
	D_00F8 = 0;

	if(bp0a < 2)
		k_StrL = 13;
	else if(bp0a == 2)
		k_StrL = 12;
	else if(bp0a == 3)
		k_StrL = 10;

	D_D4E2 = 0;
	D_0124 = 0;
	D_0126 = D_0120;
	if(bp0a == 0)
		D_00F6 = 0;
}

C_1C9F(bp06)
int bp06;
{
	D_00E8 = 1;
	D_010E = D_02DC[bp06];
	if(bp06 == 3 && D_0130 > 1)
		D_010E -= 320;
	if(bp06 < 2) {
		D_0110 = 0x41;
		D_CC78 = D_CC7A[0x13];
	} else {
		D_0110 = 1;
		D_CC78 = D_CC7A[0];
	}
	D_00F2 = 0;
	D_00FA = 0;
	if(bp06 < 2)
		D_00F4 = 0x78;
	else
		D_00F4 = 0xa0;
	k_StrR = 13;
	if(bp06 > 1)
		k_StrR = 26 - k_StrL - D_0124;
	D_D4E0 = 0;
	D_011E = 0;
	D_0128 = D_011C;
	if(D_0158 < 4)
		D_0158 ++;
}

C_1D4E()
{
	int bp02;
	int bp04;

	if(D_016E > 0)
		return -1;
	if(DoInput(0)) {/*else 1DC9*/
		bp04 = C_1DD1();
		bp02 = C_1E1C(bp04);
		if(bp02 != -1) {
			bp02 = C_1FD3(bp02, bp04);
			if(bp02 == -1) {
				D_DE68 = bp04;
				D_DE69 = 1;
			}
		}
		if(bp02 != -1) {
			D_C262 = D_C264[bp02];
			D_00E6 = 1;

			return bp02;
		}
	}

	return -1;
}

C_1DD1()
{
	int bp02;

	bp02 = 0;
	if(D_DE72 == 1) {
		while(DoInput(0))
			bp02 = GetKey();
	} else {
		if(DoInput(0))
			bp02 = GetKey();
	}

	return bp02;
}

C_1E1C(bp0a)
int bp0a;
{
	int bp02;
	int dummy;

	/*-- to lower case --*/
	if(bp0a >= 'A' && bp0a <= 'Z')
		bp0a += 0x20;
	/*-- for joystick settings --*/
	if(bp0a < 0x20) {/*else 1EAD*/
		if(bp0a == 0x18)/*CTRL+X*/
			D_E0CC ^= 0xff;
		else if(bp0a == 0x19)/*CTRL+Y*/
			D_E0CB ^= 0xff;
		else if(bp0a == 0x05)/*CTRL+E*/
			D_E0CD ^= 0xff;
		else if(bp0a == 0x02)/*CTRL+B*/
			D_E0CE ^= 0xff;
		else if(bp0a == 0x0e) {/*CTRL+N*/
			D_E0CE =
			D_E0CD =
			D_E0CB =
			D_E0CC = 0;
		}

		return -1;
	}
	/*-- --*/
	if(bp0a == '&'/*0x26*/) {
		if(D_010C < 0x40)
			D_010C = 0x41;

		return 8;
	}
	/*-- --*/
	for(bp02 = 0; bp02 < 0xb; bp02 ++) {
		if(bp0a == D_0322[bp02])
			break;
	}/*end for*/
	if(bp02 == 0xb)
		return -1;
	if(bp02 == 6 && D_BB65 == D_0102)
		return -1;
	if(D_010C > 0x3f) {
		if(bp02 < 6)
			return -1;

		return bp02 + 1;
	} else {
		if(bp02 > 8)
			return -1;

		return bp02 + 1;
	}
}

/*joystick calibration?*/
C_1F43()
{
	Beep();
	if(C_46CC() == 0) {
		Question(D_DE2B);/*" no joystick{  press any key{"*/
		D_DE72 = 1;
	} else {
		Question(D_DDD0);/*"center joystick and press a key{"*/
		C_46CC();
		D_DE72 = 0;
	}
}

/*"CTRL+R menu"?*/
C_1F86() {
	int bp02;

	Beep();
	bp02 = Question(D_DE49);/*" press q to quit| d for demo"*/
	if(bp02 == 'Q' || bp02 == 'q') {
		C_4426();
		exit(0);
	} else if(bp02 == 'D' || bp02 == 'd') {
		D_016A =
		D_DE70 = 1;
	}
}

C_1FD3(bp06)
int bp06;
{
	switch(D_010C) {
		case 0x01:
			return bp06;
		case 0x02: case 0x03: case 0x04:
		case 0x2a: case 0x2b: case 0x2c:
			if(bp06 < 4)
				return bp06;
		break;
		case 0x05: case 0x06: case 0x07:
			if(bp06 > 3 && bp06 < 7)
				return bp06 + 0x14;
		break;
		case 0x0a: case 0x0b:
			if(bp06 != 7 && bp06 != 8)
				break;
			return bp06 + 5;
		case 0x41:/*standing*/
			if(bp06 == 0x08)
				return 0x14;
			if(bp06 == 0x09)
				return 0x0a;
			if(bp06 == 0x0a)
				return 0x0b;/*bow?*/
		break;
		case 0x46:/*running*/
			if(bp06 == 0x07)
				return 0x13;
			if(bp06 == 0x09)
				return 0x10;
			if(bp06 == 0x0b)
				return 0x13;/*stop?*/
		break;
	}/*end switch*/

	return -1;
}

C_20E3()
{
	int bp02;

	if(D_0130 == 2 && D_C236 == 3)
		D_012A = 1;
	if(D_00F8 == 1) {
		if(D_010C == 0x1d)
			return 0x1f;
		else
			return 0x1e;
	}
	if(D_010C == 0x08) {
		if(D_0170 == 0)
			sound(0xb);
		else
			sound(0x14);
		D_DE69 =
		D_0174 =
		D_016E = 0;
	} else if(D_010C == 0x44) {
		D_00F8 = 2;

		return 0;
	}
	if(D_00FC == 1) {
		D_00FC = 0;
		D_BB65 -= 8;
		if(D_010C > 0x04 && D_010C < 0x08)
			return 0x1b;

		return 0;
	}
	if(D_016C == 1) {
		D_016C = 0;

		return k_rand(1) + 0x11;
	}
	if(D_00FA > 0 && D_0174 == 1)
		return 0;
	switch(D_010C) {
		case 0x01:
		case 0x08:/*fight mode?*/
			bp02 = C_1D4E();
			if(bp02 == -1)
				bp02 = 0;
			return bp02;
		case 0x02: case 0x03: case 0x04:
			bp02 = C_1D4E();
			if(bp02 == -1)
				bp02 = 0x29;
			else
				bp02 += 0x22;
			return bp02;
		case 0x2a: case 0x2b: case 0x2c:
			bp02 = C_1D4E();
			if(bp02 == -1)
				bp02 = 0x29;
			else
				bp02 += 0x25;
			return bp02;
		case 0x05: case 0x06: case 0x07:
			bp02 = C_1D4E();
			if(bp02 > 0x17 && bp02 < 0x1b) {/*else 2247*/
				D_015A ++;
				if(D_015A == 3) {
					D_015A = 0;
					bp02 = -1;
				}
			} else {
				D_015A = 0;
			}
			if(bp02 == -1)
				bp02 = 0x1b;

			return bp02;
		case 0x0a: case 0x0b:
			bp02 = C_1D4E();
			if(bp02 == 0xc && D_010C == 0x0a)
				return 0xc;
			if(bp02 == 0xd && D_010C == 0x0b)
				return 0xd;
			return 0;
		case 0x0f:
			return 0;
		case 0x41: case 0x42:
			bp02 = C_1D4E();
			if(bp02 == -1)
				return 0x13;
			return bp02;
		case 0x46:
			bp02 = C_1D4E();
			if(bp02 == -1)
				bp02 = 0xf;
			return bp02;
	}/*end switch*/
}
