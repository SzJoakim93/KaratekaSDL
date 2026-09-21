/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022

*/
#include "karateka.h"

/*---- ----*/
int D_D43A = 0;
int D_D43C = 0;
int D_D43E = 0;
int D_D440 = 0;

int D_D444[][5] = {
	0xFF,0xFF,0xC0,0xB0,0x90,
	0xFF,0xFF,0xFF,0xA0,0xA0,
	0xFF,0xFF,0xFF,0x80,0x80,
	0xFF,0xFF,0xFF,0x40,0x40
};
int D_D46C[5] = {
	0xFF,0xFF,0xFF,0x40,0x40
};
int D_D476[][5] = {
	0xC0,0xA0,0x30,0x90,0x90,
	0xC0,0x80,0x80,0x80,0x80,
	0xC0,0x80,0x60,0x40,0x40,
	0xC0,0xA0,0x40,0x20,0x20
};
int D_D49E[5] = {
	0xC0,0xA0,0x40,0x20,0x20
};
int D_D4A8[][5] = {
	0x00,0x00,0x00,0x90,0x90,
	0x00,0x00,0x40,0x80,0x80,
	0x00,0x00,0x20,0x40,0x40,
	0x00,0x00,0x20,0x20,0x20
};
int D_D4D0[5] = {
	0x00,0x00,0x20,0x20,0x20
};
int __D4DA;
int D_D4DC;
int __D4DE;
/*---- ----*/

C_268A()
{
	int _p04;
	int bp06;
	int _p08;
	int bp0a;

	if(D_0110 == 0x46) {
		if(D_011A > 0)
			return 0xe;
		if(D_0118 > 0)
			return 0;
	}
	bp06 = D_010E - D_BB65;
	if(D_0110 == 0x1c) {
		D_016C = 1;
		D_016E = 1;
	}
	if(D_00F0 == 1) {
		if(D_0110 == 2)
			return 0;
		if(D_010C > 0x3f && D_00F8 == 0)
			return 1;
		if(bp06 < 0x38 && D_00F8 == 0)
			return 2;
		return 0;
	}
	if(D_00FA == 1)
		return 0x1e;
	if(D_D43A == 1) {/*else 2838*/
		if(D_0110 == 0x22) {/*else 2786*/
			D_D43A = 0;
			D_00FA = 2;
			D_B9BA = -1;
			D_D4E2 = -1;
			D_D4DC = 1;
			if(k_StrL > 9) {
				k_StrL = 9;
				D_0124 = 0;
			} else if(k_StrL < 9) {
				D_0124 = 9 - k_StrL;
			}
			return 0;
		}
		if(D_00FE == 1) {
			D_00FE = 0;
			return 0x20;
		}
		if(D_0110 == 0x1f) {
			if(bp06 > 0xc)
				return 0x1f;
			return 0x21;
		}
		if(D_0110 == 0x21)
			return 0x20;
		if(D_00EA == 3) {/*else 280E*/
			if(D_010E > 300) {/*else 2806*/
				if(D_00F8 > 0) {
					D_D43A = 0;
					D_00FA = 2;
					return 0;
				}
				D_D43C = k_rand(2) * 12;
				return 0x1f;
			}
			return 0x20;
		}
		if(D_010E - D_BB65 < 250)
			return 0x20;
		D_010E = 1920;
		D_D43A = 0;
		D_00FA = 1;
		return 0;
	}
	if(D_00F8 > 0) {
		if(D_0110 == 0x41)
			return 0x13;
		if(D_0110 == 0x40)
			return 0x13;
		return 9;
	}
	if(D_00FE == 1) {
		D_00FE = 0;
		D_010E += 8;
		if(D_0110 > 4 && D_0110 < 8)
			return 0x1b;
		return 0;
	}
	if(D_00F6 == 1) {/*else 299C*/
		if(D_010C > 0x3f) {
			if(bp06 < 0x30)
				return 5;
			return C_29C6(bp06);
		}
		if(D_0110 == 0xb)
			return 0;
		if(bp06 > 0x2c)
			return C_29C6(bp06);
		bp0a = k_rand(0xff);
		if(bp06 < 0x1c)
			bp06 = 0x1c;
		if(bp0a >= D_D444[D_00EA][(bp06 - 0x1c) / 4])
			return 0;
		if(bp0a >= D_D476[D_00EA][(bp06 - 0x1c) / 4]) {
			if(D_0110 < 8 && D_0110 > 4)
				return C_2A94() + 0x14;
			return C_2A94();
		}
		if(bp0a >= D_D4A8[D_00EA][(bp06 - 0x1c) / 4])
			return C_2A6E();
		return C_29C6(bp06);
	}
	if(bp06 > 0x6e) {
		if(D_010C == 0x42)
			return 0xb;
		return 0x13;
	}
	D_00F6 = 1;

	return 0;
}

C_29C6(bp0a)
int bp0a;
{
	int _p02;
	int bp04;

	if(bp0a > 0x12c)
		return 0xe;
	if(bp0a > 0x4b)
		bp04 = D_00F4;
	else
		bp04 = (D_00F4 * 3) / 4;
	if(k_rand(0xff) > bp04)
		return 0;
	if(D_00EA == 0 && D_010E < D_0102 + 15)
		return 0;
	if(D_010E < D_0102 + 5)
		return 0;
	if(D_0110 == 7 || D_0110 == 0xa)
		if(bp0a > 0x1c)
			return 0xc;
	return 7;
}

C_2A6E()
{
	return k_rand(2) + 2;
}

__2A8F()
{
}

C_2A94()
{
	if(D_00F2 == 1)
		return k_rand(1) + 4;
	else
		return k_rand(2) + 4;
}

C_2ACE()
{
	int _p02;
	int bp04;
	int _p06;
	int bp08;

	if(D_00F8 == 1)
		return 0x1e;
	bp04 = D_010E - D_BB65;
	if(D_010C == 0x08) {
		sound(0xb);
		return 0xf;
	}
	if(D_00FA == 1 && D_010C < 0x3f) {
		if(D_0110 == 0x43)
			return k_rand(1) + 0x11;
		else
			return 0;
	}
	if(D_00FA == 1)
		return 0xf;
	if(D_00FC == 1) {
		D_00FC = 0;
		D_BB65 -= 4;
		if(D_010C > 0x04 && D_010C < 0x08)
			return 0x1b;
		else
			return 0;
	}
	if(bp04 > 0x2c)
		return C_2C19(bp04);
	bp08 = k_rand(0xff);
	if(bp04 < 0x1c)
		bp04 = 0x1c;
	if(bp08 >= D_D46C[(bp04 - 0x1c) / 4])
		return C_2C19(bp04);
	if(bp08 >= D_D49E[(bp04 - 0x1c) / 4])
		return C_2A94();
	if(bp08 >= D_D4D0[(bp04 - 0x1c) / 4])
		return C_2A6E();
	return C_2C19(bp04);
}

C_2C19(bp06)
int bp06;
{
	if(k_rand(0xff) > 0x32 && bp06 < 0x4b)
		return 0;
	if(D_010C == 0x08 || D_010C == 0x0b)
		return 0xd;
	return 8;
}

C_2C62()
{
	int bp04;
	int bp06;

	if(D_00F8 > 0)
		return 0;
	bp04 = D_010E - D_BB65;
	if(D_00EC == 1) {/*else 2E35*/
		if(D_D43A == 1) {/*else 2D0A*/
			if(D_0110 == 0x1f && bp04 > 0x13 && bp04 < 0x1c) {/*else 2E35*/
				if((D_010C == 0x04 || D_010C == 0x07) && D_D43C == 0x18) {
					C_301D();
					/*goto 2E35*/
				} else if((D_010C == 0x03 || D_010C == 0x06) && D_D43C == 0xc) {
					C_301D();
					/*goto 2E35*/
				} else if((D_010C == 0x02 || D_010C == 0x05) && D_D43C == 0) {
					C_301D();
					/*goto 2E35*/
				}
			}
		} else {
			if(D_00FA > 0) {/*else 2D80*/
				if(D_012E == 0 && D_BB65 > 0xec && D_00EA > 2) {
					if(D_010C < 0x05) {
						D_0154 = 1;
						/*goto 2e35*/
					} else {
						C_2FB2();
						C_44F4(0, D_010C - 0x02);/*damage related?*/
						if(D_0130 == 5) {
							C_3BAC();
							k_StrR = 5;
							D_0120 = D_0122;
							D_010E = D_0104 + 16;
							D_C262 = D_C264[0xc];
							/*goto 2e35*/
						}
					}
				}
			} else {
				bp06 = C_445D(D_010C - 0x02, bp04, D_D440);/*collision related?*/
				if(bp06 == 4 && D_0130 != 6)
					D_CC78 = D_CC7A[8];
				if(bp06 == 2 || bp06 == 3) {/*else 2E35*/
					D_0154 = 1;
					if(-- k_StrR == 0) {/*else 2DFE*/
						D_0154 = 2;
						D_011E = 0;
						D_00FA = 1;
						D_0174 = 1;
						if(D_00F2 == 1)
							D_0170 = 1;
						if(D_0124 > 0) {
							D_0120 = 2;
							D_0126 = 2;
						}
						D_CC78 = D_CC7A[0x1c];
						/*goto 2E27*/
					} else {
						if(D_0110 > 0xb && D_0110 < 0x12 && D_0130 != 6)
							D_CC78 = D_CC7A[8];
						D_0128 += 3;
						D_011E ++;
						D_00FE = 1;
					}
					C_44F4(0, D_010C - 0x02);/*damage related?*/
				}
			}
		}
	}
/*2e35*/
	if(D_011E > 0) {
		if(k_StrR + k_StrL < 26) {
			if(--D_0128 == 0) {
				k_StrR ++;
				D_011E --;
				D_0128 = D_011C;
			}
		}
	}
	if(D_0112 == 1) {/*else 2F64*/
		if(D_0130 == 7) {
			k_StrL = 1;
			bp06 = 3;
		} else if(D_0110 != 0x21) {
			if(D_0110 == 7)
				bp04 -= 4;
			bp06 = C_445D(D_0110 - 2, bp04, D_D43E);/*collision related?*/
			if(bp06 == 4 && D_BB65 > D_0102)
				D_C262 = D_C264[7];
		}
		if(D_0110 == 0x21 || bp06 == 2 || bp06 == 3 || D_010C > 0x3f) {/*else 2F64*/
			if(D_0154 == 0)
				D_0154 = 1;
			if((-- k_StrL == 0 || D_010C > 0x3f) && D_010C != 0x44) {
				D_00F8 = 1;
				D_C262 = D_C264[0x1c];
				D_010C = 0x44;
				/*goto 2F3E*/
			} else {
				if((D_010C > 0x0e && D_010C < 0x12) || D_0110 == 0x21)
					D_C262 = D_C264[7];
				D_0126 += 3;
				D_0124 ++;
				D_00FC = 1;
			}
			if(D_0130 == 7)
				C_44F4(1, 9);/*damage related?*/
			else
				C_44F4(1, D_0110 + 4);/*damage related?*/
		}
	}
	if(D_00F8 > 0)
		return 0;
/*2F72*/
	if(D_0124 > 0) {
		if(k_StrR + k_StrL < 26) {
			if(--D_0126 == 0) {
				k_StrL ++;
				if(--D_0124 == 0)
					D_0120 = D_0122;
				D_0126 = D_0120;
			}
		}
	}
}

C_2FB2()
{
	D_0104 += 0x14;
	D_0142[D_0130] = 1;
	D_012E = 1;
	D_C232 = D_C246[++D_C236];
	D_C230 = D_C238[D_C236];
	D_C234 = D_C254[D_C236];
	D_0154 = 1;
	D_00EE = 2;
	D_B9BA = -1;
	D_D4E2 = -1;
}

C_301D()
{
	D_0154 = 3;
	if(D_00EA == 3) {/*else 3088*/
		if(--k_StrR == 0) {/*else 3080*/
			D_0154 = 4;
			D_CC78 = D_CC7A[0x22];
			if(D_D43C == 0)
				D_D43C = 0xc;
			if(k_StrL > 10) {
				k_StrL = 10;
				D_0124 = 0;
				/*goto 308E*/
			} else {
				D_0124 = 10 - k_StrL;
				/*goto 308E*/
			}
		} else {
			D_00FE = 1;
			/*goto 308E*/
		}
	} else {
		D_00FE = 1;
	}
	C_44F4(0, D_010C - 0x02);/*damage related?*/
}
