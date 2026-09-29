/*
	KARATEKA - Assembly Function Replacements in C
	Copyright 2026 Retro Porting Project
*/
#ifndef ASSEMBLY_REPLACEMENTS_H
#define ASSEMBLY_REPLACEMENTS_H

#pragma pack(push, 1)
typedef struct {
	unsigned char fig_id;  /* Figure/sprite ID */
	unsigned short x_pos;  /* 16-bit X world coordinate */
	unsigned char y_pos;   /* Y screen coordinate */
} RenderEntry;
#pragma pack(pop)

/* Drawing & Blitting (from __0671.asm) */
int k_rand(int range);
void render(void);
void putFig(int fig, int x, int y);
void putFig_flipx(int fig, int x, int y);
int C_0F57(void); /* Draws intro KARATEKA text */
int C_0F90(void); /* Draws Broderbund screen */

/* Low level helpers (from C_41BE.ASM) */
void Beep(void);
void C_4426(void); /* Restore video & disk default settings on exit */
int C_445D(int index, int offset, int height); /* Collision helper */
void C_44F4(int type, int offset); /* Damage handler logic */

/* Timing & Cutscene Scripts (from C_1705.asm) */
int C_1705(char* buffer); /* Copy protection check bypass */
int C_177B(void); /* Run cutscene logic loop */
int C_191C(int ticks); /* Wait ticks + poll inputs */
void Cutscene(char* script_name, int idx); /* Initiate cutscene */
void C_19BD(void); /* Build script offset arrays */
void C_19E9(void); /* Build script offset arrays */

/* Animation Script Update Engine (from C_2329.ASM) */
void C_2366(void); /* tick animation sequencer */

/* Meters (from C_30A1.ASM) */
void ClearBottom(void); /* Clears bottom 16 scanlines of cga_buffer */
void DrMeters(void); /* Life bars */

#endif /* ASSEMBLY_REPLACEMENTS_H */
