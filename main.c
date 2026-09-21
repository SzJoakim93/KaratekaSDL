/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022

*/
/*
some alpha/beta versions can be found at:
	http://ascii.textfiles.com/archives/5039
*/

#include "karateka.h"

/*-- --*/
int _stack = 300;
/*-- --*/
int D_00E2 = 0;
int D_00E4 = 0;
int D_00E6 = 0;
int D_00E8 = 0;
int D_00EA = 0;/*bg info?*/
int D_00EC = 0;
int D_00EE = 0;/*bg info?*/
int D_00F0 = 0;
int D_00F2 = 0;
int D_00F4 = 0;
int D_00F6 = 0;
int D_00F8 = 0;
int D_00FA = 0;
int D_00FC = 0;
int D_00FE = 0;
int D_0100 = 0;
int D_0102 = 0;
int D_0104 = 0;
int D_0106 = 0;
int D_0108 = 0;
int D_010A = 0;
int D_010C = 0;
int D_010E = 0;
int D_0110 = 0;
int D_0112 = 0;
/*0114*/int k_StrR = 0;/*enemy life*/
/*0116*/int k_StrL = 0;/*player life*/
int D_0118 = 0;
int D_011A = 0;
int D_011C = 0x30;
int D_011E = 0;
int D_0120 = 0;
int D_0122 = 0x30;
int D_0124 = 0;
int D_0126 = 0x19;
int D_0128 = 0x19;
int D_012A = 0;
int D_012C = 0;
int D_012E = 0;
int D_0130 = 0;
int D_0132[8];
int D_0142[8];
int D_0152 = 0;
int D_0154 = 0;
unsigned int D_0156 = 0;
int D_0158 = 0;
int D_015A = 0;
int D_015C = 0;
int D_015E = 0;
/*0160*/int ks_base = 0;
int D_0162 = 0;
/*0164*/int km_base = 0;
int D_0166 = 0;
int D_0168 = 0;
int D_016A = 0;
int D_016C = 0;
int D_016E = 0;
int D_0170 = 0;
int D_0172 = 0;
int D_0174 = 0;
/*-- --*/
char *D_01E8[] = {
	/*0176*/"set_tune",
	/*017F*/"set_bg",
	/*0186*/"set_fig",
	/*018E*/"chg_fig",
	/*0196*/"do_scr",
	/*019D*/"del_fig",
	/*01A5*/"set_wipe",
	/*01AE*/"set_nowipe",
	/*01B9*/"wait",
	/*01BE*/"init_sal",
	/*01C7*/"set_pos",
	/*01CF*/"inc_x",
	/*01D5*/"loop",
	/*01DA*/"end_animation"
};
char *D_021C[] = {
	/*0204*/"bal00",
	/*020A*/"bal01",
	/*0210*/"bal02",
	/*0216*/"bal03"
};
char *D_0264[] = {
	/*0224*/"ks0",
	/*0228*/"ks1",
	/*022C*/"ks2",
	/*0230*/"ks3",
	/*0234*/"ks4",
	/*0238*/"ksc",
	/*023C*/"ksi0",
	/*0241*/"ksi1",
	/*0246*/"ksi2",
	/*024B*/"ksj2",
	/*0250*/"ksi3",
	/*0255*/"ksi4",
	/*025A*/"ksj4",
	/*025F*/"ksi"
};
char *D_02C0[] = {
	/*0280*/"km0",
	/*0284*/"km1",
	/*0288*/"km2",
	/*028C*/"km3",
	/*0290*/"km4",
	/*0294*/"kmc",
	/*0298*/"kmi0",
	/*029D*/"kmi1",
	/*02A2*/"kmi2",
	/*02A7*/"kmj2",
	/*02AC*/"kmi3",
	/*02B1*/"kmi4",
	/*02B6*/"kmj4",
	/*02BB*/"kmi"
};
/*---- ----*/
int D_02DC[5] = {0x140,0x884,0x780,0x230,0x014};
int D_02E6[5] = {0x05C,0x028,0x02C,0x050,0x020};
int D_02F0[5] = {0x058,0x020,0x020,0x020,0x020};
int D_02FA[5] = {0x1A0,0x880,0x778,0x250,0x110};
int D_0304[5] = {0x000,0x000,0x000,0x000,0x000};
int D_030E[5] = {0x060,0x740,0x640,0x140,0x000};
int D_0318[5] = {0x060,0x024,0x020,0x000,0x000};
/*---- ----*/
char D_0322[0xb] = "qazwsx46 b0";
char D_032D[] = ".ind";
char D_0332[] = ".dat";
char D_0337[16000];
int D_41B7 = 1;
int D_41B9 = 0;
/*41bb	"disk is in drive a."*/
/*41cf	"allgal"*/
/*41d6	"fuji.bcg"*/
/*41DF	">>> Press the F1 key if you have a monochrome graphics <<<"*/
/*421A	">>>    system and Karateka is not visible on screen.   <<<"*/
/*4255	"Karateka needs a graphics adapter card to operate correctly."*/
/*4292	"title.bcg"*/
/*429C	"cal00"*/
/*42A2	"cal01"*/
/*---- ----*/

/*0255*/main()
{
	int bp02;

	bp02 = 0;
	C_430A();/*getDefaultDrive*/
	C_42F4();/*selectDisk_0*/
	C_48E2();/*set int 24h*/
	C_4718();/*speed related function*/
	D_DE72 = 1;
	for(;;) {
		if(C_1705(D_0337))/*check copy*/
			break;
		if(C_1705(D_0337+0x400))/*check copy*/
			break;
		puts(D_DD8A);/*"make sure your karateka"*/
		puts(/*41BB*/"disk is in drive a.");
		puts(D_DDB6);/*"press any key to continue"*/
		WaitKey();
	}/*end for*/
	/*-- main loop --*//*02B7*/
	for(;;) {
		D_016A = 1;
		C_19F5();/*load 'allpal' script*/
		D_0118 =
		D_011A =
		D_00E2 =
		D_00F0 =
		D_00EC =
		D_0170 =
		D_016C =
		D_016E =
		D_0172 =
		D_0174 =
		D_012A =
		D_00E6 =
		D_00E8 =
		D_00F2 =
		D_00F6 =
		D_00F8 =
		D_00FA =
		D_012C =
		D_0130 =
		D_0166 =
		D_D43A = 0;
		k_StrR = 13;
		if(D_41B9)
			C_0616();/*little intro*/
		else
			C_03DE();/*intro and demo*/
		D_41B9 = 0;
		D_00FC =
		D_0130 = 0;
		tim_strt();
		D_016A = 0;
		/*-- game loop --*//*032A*/
		while(D_016A != 1 && D_DE70 <= 0) {
			DoInput(1);
			tim_wait();
			C_2366();
			C_2C62();
			render();
			/*-- --*/
			if(D_0154 > 0) {
				if(D_0154 == 2)
					sound(8);
				else if(D_0154 == 3)
					sound(0x18);
				else if(D_0154 == 4)
					sound(0x16);
				else
					sound(1);
				D_0154 = 0;
			}
			/*-- --*/
			D_00EE = 0;
			if(D_016A == 0)
				D_016A = C_3233();
			if(D_012A == 2) {
				D_012A = 0;
				sound(1);
			}
			if(D_012A == 1) {
				D_012A = 2;
				D_C236 = 1;
				C_3B1D();
			}
			/*-- --*/
			DrMeters();
		}/*end while*/
	}/*end for*/
}

/*intro and demo*/
C_03DE()
{
	int bp02;

	WaitNoKey();
	D_00F6 =
	D_015C =
	D_015A = 0;
	load_animation_script(/*41CF*/"allgal", D_CCCE);
	C_19E9();/*make indexes for D_CCCE*/
	load_background_graphics(/*41D6*/"fuji.bcg", 1500);
	D_00EA = 0;
	load_level_assets(D_00EA);/*load level data*/
	/*intro loop*/
	for(;;) {/*0429*/
		D_DE70 = 0;
		load_sprite_assets(0xd);/*load ks/km files*/
		/*-- first time --*/
		if(D_41B7) {
			if(C_4300() == 7) {
				puts(/*41DF*/">>> Press the F1 key if you have a monochrome graphics <<<");
				puts(/*421A*/">>>    system and Karateka is not visible on screen.   <<<");
			}
			if(!C_440E()) {
				puts(/*4255*/"Karateka needs a graphics adapter card to operate correctly.");
				exit(1);
			}
			if(!C_46CC()) {
				D_DE72 = 1;
			} else {
				C_4055();
				D_DE72 = 0;
			}
			D_41B7 = 0;
		}
		/*-- --*/
		if(C_0F90()) {/*"broderbund software presents"*/
			C_0616();/*little intro*/
			break;
		}
		load_background_graphics(/*4292*/"title.bcg", 0);
		if(C_3FE4()) {/*"a game by jordan mechner"*/
			C_0616();/*little intro*/
			break;
		}
		if(C_4007()) {/*"IBM version by the connelley group"*/
			C_0616();/*little intro*/
			break;
		}
		if(C_0F57()) {/*"KARATEKA"*/
			C_0616();/*little intro*/
			break;
		}
		if(C_3F6C()) {/*story scroll*/
			C_0616();/*little intro*/
			break;
		}
		D_00E4 = 0;
		BB_clear();
		D_00EE = 1;
		D_00EA = 4;
		load_animation_script(/*429C*/"cal00", D_BB94);
		load_sprite_assets(6);/*load ks/km files*/
		if(C_177B()) {/*execute script/animation?*/
			C_0616();/*little intro*/
			break;
		}
		D_00EA = 0;
		load_animation_script(/*42A2*/"cal01", D_BB94);
		load_background_graphics(/*41D6*/"fuji.bcg", 1500);
		load_level_assets(0);/*load level data*/
		if(C_177B()) {/*execute script/animation?*/
			C_064E();
			break;
		}
		D_00EA = 0;
		D_00F4 = 0xFA;
		D_0156 = 1;
		tim_strt();
		/* */
		/*-- demo loop --*/
		for(bp02 = 0; bp02 != 1; bp02 = C_05D2()) {
			/**/
			tim_wait();
			C_2366();
			C_2C62();
			render();
			/*-- --*/
			if(D_0154 > 0) {
				if(D_0154 == 2)
					sound(8);
				/**/
					/**/
				/**/
					/**/
				else
					sound(1);
				D_0154 = 0;
			}
			/*-- --*/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/**/
			/*-- --*/
			DrMeters();
		}/*end for*/
		C_1B6E(0);
	}/*end for*/
	/*-- --*/
	D_010C = 0x41;
	WaitNoKey();
	D_DE69 = D_00EA = 0;
	D_0174 = D_016C = D_016E = D_DE69;
	D_0156 = 0;
}

/*"input" for demo*/
C_05D2()
{
	if(
		(D_00F8 == 1 && D_010C == 0x43) ||
		D_BB65 == D_0104 ||
		DoInput(1)
	) return 1;

	return 0;
}

/*little intro*/
C_0616()
{
	load_castle_bg();/*draw "castle.bcg"*/
	load_background_graphics(/*41D6*/"fuji.bcg", 1500);
	load_level_assets(0);/*load level data*/
	BB_clear();
	D_00EA = 0;
	D_DE70 = 0;
	load_animation_script(/*42A2*/"cal01", D_BB94);
	C_177B();/*execute script/animation?*/
}

C_064E()
{
	load_background_graphics(/*41D6*/"fuji.bcg", 1500);
	load_level_assets(0);/*load level data*/
	BB_clear();
	D_DE70 = 0;
}
