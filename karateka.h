/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022
*/

#include "dos_sdl_bridge.h"

/*---- ----*/
extern int D_00E2;
extern int D_00E4;
extern int D_00E6;
extern int D_00E8;
extern int D_00EA;
extern int D_00EC;
extern int D_00EE;
extern int D_00F0;
extern int D_00F2;
extern int D_00F4;
extern int D_00F6;
extern int D_00F8;
extern int D_00FA;
extern int D_00FC;
extern int D_00FE;
extern int D_0100;
extern int D_0102;
extern int D_0104;
extern int D_0106;
extern int D_0108;
extern int D_010A;
extern int D_010C;
extern int D_010E;
extern int D_0110;
extern int D_0112;
/*0114*/extern int k_StrR;
/*0116*/extern int k_StrL;
extern int D_0118;
extern int D_011A;
extern int D_011C;
extern int D_011E;
extern int D_0120;
extern int D_0122;
extern int D_0124;
extern int D_0126;
extern int D_0128;
extern int D_012A;
extern int D_012C;
extern int D_012E;
extern int D_0130;
extern int D_0132[];
extern int D_0142[];
extern int D_0152;
extern int D_0154;
extern unsigned int D_0156;
extern int D_0158;
extern int D_015A;
extern int D_015C;
extern int D_015E;
/*0160*/extern int ks_base;
extern int D_0162;
/*0164*/extern int km_base;
extern int D_0166;

extern int D_016A;
extern int D_016C;
extern int D_016E;
extern int D_0170;
extern int D_0172;
extern int D_0174;

extern char *D_01E8[];
extern char *D_021C[];
extern char *D_0264[];
extern char *D_02C0[];
extern int D_02DC[];
extern int D_02E6[];
extern int D_02F0[];
extern int D_02FA[];
extern int D_0304[];
extern int D_030E[];
extern int D_0318[];
extern char D_0322[];
extern char D_032D[];/*".ind"*/
extern char D_0332[];/*".dat"*/
extern char D_0337[/*0x3e80=16000*/];

extern int D_41B9;

extern int D_B9BA;

extern int D_BB92;

extern unsigned char D_BF92[];

extern unsigned char *D_C230;
extern unsigned char *D_C232;
extern unsigned char *D_C234;
extern int D_C236;
extern unsigned char *D_C238[/*7*/];
extern unsigned char *D_C246[/*7*/];
extern unsigned char *D_C254[/*7*/];
extern int D_C262;
extern int D_C264[/*0x2a*/];
extern unsigned char D_C2B8[0x4000];
extern int D_CC78;
extern int D_CC7A[/*0x2a*/];
extern unsigned char D_CCCE[0x4000];

extern int D_D43A;
extern int D_D43C;
extern int D_D43E;
extern int D_D440;

extern unsigned char D_E0CB;
extern unsigned char D_E0CC;
extern unsigned char D_E0CD;
extern unsigned char D_E0CE;
/*---- ----*/
/* Include central table/variable declarations */
#include "tables.h"

