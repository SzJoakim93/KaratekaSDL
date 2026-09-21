/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022

*/
#include "karateka.h"

/*---- ----*/
int D_D502 = 0;
int D_D504 = 0;
int D_D506 = 0;
int D_D508 = 1;
int D_D50A = 0;
int D_D50C = 0;
int D_D50E = 0;
/*D510	"cal07a"*/
/*D517	"fuji.bcg"*/
/*D520	"cal04"*/
/*D526	"cal05"*/
/*D52C	"cal06"*/
/*D532	"allval"*/
/*D539	"cal03"*/
/*D53F	"prngal"*/
/*D546	"cal07"*/
/*---- ----*/

C_3233()
{
	int bp04;

	/*-- --*/
	if(D_D502 == 6) {
		sound(0xc);
		D_D502 = 0;
	}
	if(D_D502 > 2)
		D_D502 ++;
	if(D_D502 == 2 && D_010C == 0x01)
		D_D502 = 3;
	/*-- --*/
	if(D_D504 > 0) {
		if(D_D43A == 0) {/*else 32BA*/
			if(D_010C == 0x1c)
				return 0;
			sound(0xa);
			if(C_3F83() != 0)/*"the end"*/
				D_41B9 = 1;
			D_D504 = 0;

			return 1;
		}

		return 0;
	}
	/*-- --*/
	if(D_010C == 0x46 && D_DE68 == 0x20) {
		D_C262 = D_C264[0x10];
		D_DE69 = 0;
	}
	if(D_00EA == 2) {/*else 33E2*/
		bp04 = D_0106 - D_BB65;
		if(D_D50C > 0) {
			D_D50C --;
		} else if(D_012C == 0) {
			if(bp04 < 0x5e && bp04 > 0x20) {
				if(bp04 < 0x3e || (D_010C > 0x01 && D_010C < 0x08 && D_00FA > 0)) {
					if(D_00F8 == 0)
						D_D50A = 1;
				}
			}
		}
		if(bp04 < 0x3e && bp04 > 0x20 && D_D50A == 1 && D_00F8 == 0) {
			D_00F8 = 1;
			D_C262 = D_C264[0x1d];
		}
		if(D_D50A == 1) {/*else 3391*/
			if(++D_012C == 5) {
				D_D50A = 0;
				D_D50C = 3;
			}
			if(D_012C == 1)
				D_012C += 2;
			C_3B63();
			/*goto 33BD*/
		} else if(D_012C > 0 && D_D50C == 0) {
			D_D508 = -D_D508;
			if(D_D508 > 0) {
				if(--D_012C == 0)
					D_D50C = 0xa;
				C_3B63();
			}
		}
		if(D_012C > 1 && bp04 > 0x20 && D_00F8 == 0)
			D_0104 = D_0106 - 0x44;
		else
			D_0104 = D_0106;
	}
	if(D_00F8 > 0) {/*else 3452*/
		if(D_0166 == 0) {
			D_0166 = 0x14;

			return 0;
		}
		if(--D_0166 == 0) {/*else 3437*/
			if(D_010C == 0x44)
				sound(3);
			for(bp04 = 0; bp04 < 1000; bp04 ++);
			k_StrL = 0;
			D_D504 = 1;
		} else if(D_0166 == 0xb && D_010C == 0x44) {
			D_0166 = 1;
		}

		return 0;
	}
	if(D_BB65 == D_0104) {/*else 37C0*/
		if(D_0152 == 3) {
			D_0152 = 0;
			D_00EA = 2;
			load_level_assets(++D_00EA);
			D_0130 = 6;
			D_012E = 1;
			D_0142[D_0130] = 1;
			D_B9BA = -1;
			D_00FA = 2;

			return 0;
		}
		D_00EA += D_0152;
		D_0152 = 0;
		switch(D_00EA) {
			case 0:
				/*akuma sends a fighter*/
				Cutscene(/*D510*/"cal07a", 7);
				load_level_assets(++D_00EA);
				load_background_graphics(/*D517*/"fuji.bcg", 1500);
				D_0118 = 0x20;
				D_0110 = 0x46;
				D_CC78 = D_CC7A[0xe];
				D_B9BA = -1;

				return 0;
			case 1:
				/*mariko waits in cell*/
				Cutscene(/*D520*/"cal04", 8);
				load_level_assets(++D_00EA);
				load_background_graphics(/*D517*/"fuji.bcg", 1500);
				D_010E = 200;
				D_00E2 = 1;
				D_0166 = 9;

				return 0;
			case 2:
				if(D_0104 < D_0106) {
					C_3BEF();

					return 0;
				}
				/*mariko waits in cell and stands*/
				Cutscene(/*D526*/"cal05", 0xa);
				D_0130 = 1;
				D_B9BA = -1;
				load_level_assets(++D_00EA);
				D_0102 = D_BB65;
				D_012E = 1;
				D_00FA = 0;
				D_D50E = 0;
				D_D502 = 0;
				D_00F6 = 0;

				return 0;
			case 3:
				if(D_012E == 1 && (D_D43A == 0 || D_0130 < 5)) {/*else 36FD*/
					if(++D_0130 == 7) {/*else 366A*/
						/*akuma kicks, mariko stands*/
						Cutscene(/*D52C*/"cal06", 0xc);
						load_level_assets(++D_00EA);
						load_animation_script(/*D532*/"allval", D_CCCE);
						C_19E9();/*make indexes for D_CCCE*/
						D_0130 = 6;
						C_3AC0(D_0130);
						D_010E = 276;
						D_BB65 = D_010E - 30;
						D_0110 = 6;
						D_00F2 = 1;
						k_StrR = 26 - k_StrL;
						if(D_010C < 0x3f) {
							D_010C = 0x0a;
							D_C262 = D_C264[0xc];
							D_0112 = 1;
						} else {
							k_StrL = 0;
							D_00F8 = 1;
							D_C262 = D_C264[0x1c];
						}
						D_011E = 18 - k_StrR;
						if(D_011E < 0)
							D_011E = 0;

						return 0;
					}
					if(D_0130 == 2) {
						D_0104 -= 0x140;
						D_010A -= 0x140;
					}
					D_012E = D_0142[D_0130];
					if(D_00FA > 0) {/*else 36D2*/
						D_00FA = 2;
						if(D_0132[D_0130] > 0) {
							C_1C9F(D_00EA);
							D_0132[D_0130] = 0;
						}
						if(D_0130 == 6 && D_D50E == 0) {
							D_D502 = 1;
							D_D50E = 1;
						}
					} else {
						D_010E -= 276;
					}
					if(D_012E == 0)
						D_0104 = 0xfc;
					else
						D_0104 = 0x130;
					C_3AC0(D_0130);

					return 0;
				}
				C_3BEF();

				return 0;
			case 4:
				if(D_0130 == 7) {
					if(D_010C > 0x40) {
						C_177B();/*execute script/animation?*/
						C_3F9E();/*victory scroll?*/

						return 1;
					}

					return 0;
				}
				D_0130 ++;
				if(D_00FA == 1)
					D_00FA ++;
				D_012E = D_0142[D_0130];
				C_3AC0(D_0130);
				if(D_0130 == 7) {/*else 37A1*/
					D_0104 = 0xac;
					load_animation_script(/*D539*/"cal03", D_BB94);
					load_sprite_assets(0xb);/*load ks/km files*/
					load_animation_script(/*D53F*/"prngal", D_CCCE);
					C_19E9();/*make indexes for D_CCCE*/
					D_00FA = 0;
					D_010E = 200;
					D_0110 = 1;
					D_00F0 = 1;
					D_CC78 = D_CC7A[0];
				}

				return 0;
		}/*end switch*/
	}
/*37c0*/
	if(D_00FA > 0 && D_0130 == 5 && D_0172 == 0) {
		D_0172 = 1;
		D_0124 = 21 - k_StrL;
		D_0120 = 2;
		D_0126 = 2;
	}
	if(D_00FA == 1 && (D_00EA == 1 || D_00EA == 2) && D_012C == 0 && D_0104 - D_BB65 > 0x3c) {/*else 3986*/
		if(D_0166 == 0) {
			D_0120 = 2;
			D_0126 = 2;
			D_0166 = 0x28;
			if(D_0104 - D_BB65 < 0xfa)
				D_0166 = 0x3c;
			if(D_00E2 == 1)
				D_0166 = 0x32;

			return 0;
		}
		if(D_0166 == 1 && D_010C == 0x08)
			D_0166 ++;
		if(--D_0166 == 0) {/*else 397F*/
			if(D_00E2 == 1 && D_00EA == 2) {/*else 38E4*/
				/*akuma sends a fighter*/
				Cutscene(/*D546*/"cal07", 9);
				load_sprite_assets(2);/*load ks/km files*/
				load_background_graphics(/*D517*/"fuji.bcg", 1500);
				C_1C9F(D_00EA);
				D_0118 = 0x20;
				D_0110 = 0x46;
				D_CC78 = D_CC7A[0xe];
				D_B9BA = -1;
				D_00E2 = 0;
				D_D506 = 0;

				return 0;
			}
			if(D_0104 - D_BB65 > 0x190) {/*else 396A*/
				if(D_00EA == 2 && D_D506 == 0) {
					D_D506 = 1;
					sound(0x18);
					C_3BAC();
					k_StrR = 0;
					D_011E = 0;
					D_00FE = 0;
					D_010E = D_BB65 + 296;
					D_0166 = 2;
					/*goto 397f*/
				} else {
					C_1C9F(D_00EA);
					D_D506 = 0;
					if(D_00EA == 2) {/*else 395C*/
						if(D_010E - D_BB65 > 320)
							D_010E = D_BB65 + 300;
					} else {
						D_011A = 0x20;
					}
					D_D4E2 = 0;
					/*goto 397f*/
				}
			} else {
				if(D_0104 - D_BB65 > 0x1e)
					C_1C9F(D_00EA);
			}
		}

		return 0;
	}
	if(D_BB65 == D_0102) {/*else 39F2*/
		if(D_00EA == 0) {
			D_BB65 = 0x2c;
			D_0102 = 0x28;
			D_C262 = D_C264[0x20];

			return 0;
		}
		if(D_00EA > 2) {
			if(D_0130 > 2) {/*else 39F2*/
				D_0130 --;
				D_012E = 1;
				C_3AC0(D_0130);
				if(D_0130 == 2) {
					D_C236 = 2;
					C_3B1D();
				}
				D_BB65 = 0x114;
				D_0104 = D_BB65 + 2;
				D_010E += 276;
			}
		}
	}
	bp04 = D_010E - D_BB65;
	if(bp04 < 330) {
		D_011A =
		D_0118 = 0;
		if(D_0110 > 0x3f && D_00EA > 0)
			D_0110 = 1;

		return 0;
	}
	if(D_0118 > 1) {
		D_0118 --;

		return 0;
	}
	if(D_0118 == 1) {
		D_0118 = 0;
		D_011A = 0x20;
		D_0110 = 0x46;
		D_CC78 = D_CC7A[0xe];
		D_B9BA = -1;

		return 0;
	}
	if(D_011A > 1) {
		D_011A --;

		return 0;
	}
	if(D_011A == 1) {
		D_011A = 0;
		if(D_00EA == 1) {
			D_0118 = 0x20;
			D_B9BA = -1;
			if(bp04 > 800)
				D_010E -= bp04 / 2;
		} else {
			D_00FA = 1;
			D_0158 --;
			k_StrR = 0;
		}
	}

	return 0;
}

C_3AC0(bp06)
int bp06;
{
	if(D_D502 == 1)
		D_D502 ++;
	if(bp06 == 2) {
		D_0102 = 0x20;
		D_BB65 = 0x20;
	} else {
		D_0102 = 0;
		D_BB65 = 4;
	}
	D_C236 = 3;
	if(bp06 == 7)
		D_C236 = 5;
	else
		D_C236 += D_012E;
	C_3B1D();
}

C_3B1D()
{
	D_B9BA = -1;
	D_D4E2 = 0;
	D_00EE = 1;
	D_C230 = D_C238[D_C236];
	D_C232 = D_C246[D_C236];
	D_C234 = D_C254[D_C236];
}

C_3B63()
{
	D_D4E2 = 0;
	D_00EE = 2;
	D_B9BA = -1;
	D_C236 = D_012C;
	D_C230 = D_C238[D_C236];
	D_C232 = D_C246[D_C236];
	D_C234 = D_C254[D_C236];
}

C_3BAC()
{
	D_D43A = 1;
	D_0110 = 0x1f;
	D_CC78 = D_CC7A[0x1f];
	D_D43C = k_rand(2) * 12;
	D_D4E0 = 0;
	D_00FA = 0;
}

C_3BEF()
{
	sound(1);
	D_BB65 -= 4;
	if(--k_StrL == 0) {
		D_00F8 = 1;
		D_C262 = D_C264[0x1c];

		return 0;
	}
	D_0124 ++;
	D_010C = 0;
	D_C262 = D_C264[0];
}
