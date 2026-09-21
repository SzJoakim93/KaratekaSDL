/*
	KARATEKA - Assembly Function Replacements in C
	Copyright 2026 Retro Porting Project
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "karateka.h"
#include "assembly_replacements.h"

/* Global references to external variables */
extern int D_0158; /* Holds active shift for enemy rendering */

/* RLE state variables */
static int rleDataCount = 0;
static unsigned char rleDataByte = 0;
static int rleMaskCount = 0;
static unsigned char rleMaskByte = 0;
static int figDataIndex = 0;
static int figMaskIndex = 0;
static int fig_stride = 0;
static int fig_height = 0;
static int fig_width = 0;
static int v_offset = 0;

/* Helper to reverse bits in a byte for horizontal mirroring */
static unsigned char reverse_bits(unsigned char b)
{
	unsigned char r = 0;
	if (b & 0x01) r |= 0x80;
	if (b & 0x02) r |= 0x40;
	if (b & 0x04) r |= 0x20;
	if (b & 0x08) r |= 0x10;
	if (b & 0x10) r |= 0x08;
	if (b & 0x20) r |= 0x04;
	if (b & 0x40) r |= 0x02;
	if (b & 0x80) r |= 0x01;
	return r;
}

/* Helper to read next RLE data byte */
static unsigned char get_rle_data_byte(void)
{
	if (rleDataCount > 0) {
		rleDataCount--;
		return rleDataByte;
	}
	unsigned char b = ks_data[figDataIndex++];
	if (b == 0x7B) {
		rleDataByte = ks_data[figDataIndex++];
		rleDataCount = ks_data[figDataIndex++] - 1;
		return rleDataByte;
	}
	return b;
}

/* Helper to read next RLE mask byte */
static unsigned char get_rle_mask_byte(void)
{
	if (rleMaskCount > 0) {
		rleMaskCount--;
		return rleMaskByte;
	}
	unsigned char b = km_data[figMaskIndex++];
	if (b == 0x7B) {
		rleMaskByte = km_data[figMaskIndex++];
		rleMaskCount = km_data[figMaskIndex++] - 1;
		return rleMaskByte;
	}
	return b;
}

/* Helper to write 2bpp color pixel to backbuffer */
static void write_pixel(int px, int py, unsigned char color_val)
{
	if (px < 0 || px >= 320 || py < 0 || py >= 200) return;
	int byte_idx = py * 80 + (px / 4);
	int pixel_shift = 6 - (2 * (px % 4));
	
	/* Clear the 2 bits for this pixel */
	D_0337[byte_idx] &= ~(0x03 << pixel_shift);
	/* Set the new 2 bits */
	D_0337[byte_idx] |= (color_val & 0x03) << pixel_shift;
}

/* Pseudo-random number generator mapping to original Prime LCG */
int k_rand(int range)
{
	int r = C_414A();
	r = (r * 2) & 0xFFFF;
	return (int)(((unsigned long)(range + 1) * r) >> 16);
}

/* Blits standard sprite to backbuffer */
void putFig(int fig, int x, int y)
{
	unsigned short data_offset = (unsigned char)ks_index[fig * 2] | ((unsigned char)ks_index[fig * 2 + 1] << 8);
	unsigned short mask_offset = (unsigned char)km_index[fig * 2] | ((unsigned char)km_index[fig * 2 + 1] << 8);

	fig_stride = ks_data[data_offset];
	fig_height = ks_data[data_offset + 1];
	fig_width = fig_stride * 4;

	y -= fig_height;
	x -= D_00E4;

	rleDataCount = 0;
	rleMaskCount = 0;
	figDataIndex = data_offset + 3;
	figMaskIndex = mask_offset + 3;

	for (int row = 0; row < fig_height; row++) {
		int py = y + row;
		for (int b = 0; b < fig_stride; b++) {
			unsigned char m, d;
			if (fig < 200) {
				m = get_rle_mask_byte();
				d = get_rle_data_byte();
			} else {
				m = km_data[figMaskIndex++];
				d = ks_data[figDataIndex++];
			}

			for (int p = 0; p < 4; p++) {
				int px = x + b * 4 + p;
				int shift = 6 - 2 * p;
				unsigned char mask_val = (m >> shift) & 0x03;
				unsigned char data_val = (d >> shift) & 0x03;

				if (mask_val != 0) {
					write_pixel(px, py, data_val);
				}
			}
		}
	}
}

/* Blits horizontally mirrored sprite to backbuffer */
void putFig_flipx(int fig, int x, int y)
{
	unsigned short data_offset = (unsigned char)ks_index[fig * 2] | ((unsigned char)ks_index[fig * 2 + 1] << 8);
	unsigned short mask_offset = (unsigned char)km_index[fig * 2] | ((unsigned char)km_index[fig * 2 + 1] << 8);

	fig_stride = ks_data[data_offset];
	fig_height = ks_data[data_offset + 1];
	fig_width = fig_stride * 4;

	y -= fig_height;
	x -= D_00E4;

	rleDataCount = 0;
	rleMaskCount = 0;
	figDataIndex = data_offset + 3;
	figMaskIndex = mask_offset + 3;

	for (int row = 0; row < fig_height; row++) {
		int py = y + row;
		for (int b = 0; b < fig_stride; b++) {
			unsigned char m, d;
			if (fig < 200) {
				m = get_rle_mask_byte();
				d = get_rle_data_byte();
			} else {
				m = km_data[figMaskIndex++];
				d = ks_data[figDataIndex++];
			}

			/* Mirror both mask and pixel layout inside the byte */
			m = reverse_bits(m);
			d = reverse_bits(d);

			for (int p = 0; p < 4; p++) {
				int px = x + (fig_stride - 1 - b) * 4 + p;
				int shift = 6 - 2 * p;
				unsigned char mask_val = (m >> shift) & 0x03;
				unsigned char data_val = (d >> shift) & 0x03;

				if (mask_val != 0) {
					write_pixel(px, py, data_val);
				}
			}
		}
	}
}

/* Helper to clear ground scanlines */
static void C_0E39(void)
{
	if (D_00EA < 2) {
		/* Clear 42 rows starting at row 114 */
		memset(&D_0337[114 * 80], 0, 42 * 80);
	} else {
		/* Clear 48 rows starting at row 108 */
		memset(&D_0337[108 * 80], 0, 48 * 80);
	}
}

/* Background graphics renderer */
void renderBG(int bg)
{
	if (D_00EE == 1) {
		if (D_00EA < 2) {
			/* Fill outdoor sky with Cyan (0x55 = color index 1) */
			memset(D_0337, 0x55, 80 * 80);
		} else {
			BB_clear();
		}
		if (D_BB60 == 0) {
			DrMeters();
		}
	}

	if (D_00EA < 2) {
		/* Outdoor rendering */
		if (D_00E4 != D_B9BA) {
			/* Fill sky */
			memset(D_0337, 0x55, 80 * 80);
			/* Copy Fuji background mountain graphics */
			memcpy(&D_0337[80 * 80], &D_A606[1500], 34 * 80);
			/* Draw white snow line */
			memset(&D_0337[106 * 80], 0xFF, 1 * 80);
			memset(&D_0337[107 * 80], 0x00, 3 * 80);
		}
		C_0E39();
		
		/* Draw dithered floor pattern based on scroll phase */
		unsigned char floor_pattern = (D_00E4 & 1) ? 0x66 : 0x99;
		memset(&D_0337[150 * 80], floor_pattern, 15 * 80);
		memset(&D_0337[165 * 80], ~floor_pattern, 15 * 80);
	} else {
		/* Indoor palace rendering */
		C_0E39();
		if (D_00EA == 2) {
			if (D_00E4 != D_B9BA) {
				memset(&D_0337[80 * 80], 0x00, 30 * 80);
				memcpy(&D_0337[80 * 80], &D_A606[1500], 22 * 80);
				memset(&D_0337[90 * 80], 0x00, 1 * 80);
			}
			/* Draw dithered floor pattern */
			memset(&D_0337[150 * 80], 0xAA, 15 * 80);
			memset(&D_0337[165 * 80], 0x00, 15 * 80);
		} else {
			if (D_00E4 != D_B9BA) {
				memset(D_0337, 0x00, 114 * 80);
			}
			memset(&D_0337[150 * 80], 0x55, 15 * 80);
			memset(&D_0337[165 * 80], 0x00, 15 * 80);
		}
	}
}

/* Master scene compositor */
void render(void)
{
	/* 1. Render Background */
	unsigned char bg = D_B9C0[2];
	renderBG(bg);

	/* 2. Render Actors */
	RenderEntry *entries = (RenderEntry*)&D_B9C0[3];
	int idx = 0;
	while (entries[idx].f_00 != 0xFF) {
		int fig = entries[idx].f_00;
		int x = entries[idx].f_01;
		int y = entries[idx].f_03;

		if (x & 0x4000) {
			x &= ~0x4000;
			putFig_flipx(fig, x, y);
		} else {
			putFig(fig, x, y);
		}
		idx++;
	}

	/* 3. Present backbuffer frame */
	if (D_B9C0[1] != 0) {
		BB_flip_wipe();
	} else {
		if (D_00EE == 0) {
			BB_flip_part();
		} else {
			BB_flip();
		}
	}

	/* 4. Play frame SFX */
	if (D_B9C0[0] != 0) {
		sound(D_B9C0[0]);
	}
}

/* Draws intro title screen */
int C_0F57(void)
{
	BB_clear();
	memcpy(&D_0337[0x15E0], D_A606, 0x1040);
	BB_flip();
	if (C_191C(0x12)) return 1;
	putFig(0x5C, 40, 180);
	BB_flip();
	if (C_191C(0x48)) return 1;
	return 0;
}

/* Draws Broderbund logo screen */
int C_0F90(void)
{
	BB_clear();
	putFig(0x5B, 106, 115);
	BB_flip();
	D_0168 = 1;
	int ret = C_191C(0x48);
	D_0168 = 0;
	return ret;
}

/* Stub/Null exit routines */
void C_4426(void) {}

/* Collision coordinate detection checks */
int C_445D(int state, int offset_x, int index)
{
	int val_x = offset_x;
	int val_state = state;
	
	if (val_state > 2) {
		val_state += 3;
	} else {
		val_state -= 3;
		if (val_state != 0) {
			val_x -= 4;
		}
	}
	
	val_x &= 0xFFFC;
	
	int bl = D_E032[index];
	int dx = D_E018[index];
	
	if (val_x < dx) {
		return 4;
	}
	if (val_x != dx) {
		dx += 17;
		if (val_x > dx) {
			return 0;
		}
		if (val_state == 1) {
			return D_E056[bl];
		}
		if (val_state > 2) {
			dx -= 4;
			if (val_x < dx) {
				if (val_state <= 4) {
					return D_E060[bl];
				} else {
					return D_E06A[bl];
				}
			}
			return D_E074[bl];
		}
		int al = D_E04C[bl];
		if (bl == 1 && val_state == 2) {
			return 0;
		}
		return al;
	}
	if (val_state > 4) {
		return D_E06A[bl];
	}
	if (val_state >= 3) {
		return D_E060[bl];
	}
	return 3;
}

/* Queue damage modifier entries into backbuffer draw queue */
void C_44F4(int type, int offset)
{
	int bx = offset * 2;
	if (D_00F2 == 1 && bx >= 12) {
		bx += 12;
	}
	
	unsigned short val_x;
	unsigned char cl;
	if (type == 0) {
		val_x = D_BB65;
		cl = 0x61;
		if (offset < 3) {
			cl++;
		}
	} else {
		val_x = D_010E;
		cl = 0x63;
		if (D_D43A == 0) {
			bx = 14;
		}
		if (bx < 18) {
			cl++;
		}
	}
	
	int si = D_B9BE;
	int di = si + 4;
	while (si >= D_D442) {
		D_B9C0[di] = D_B9C0[si];
		D_B9C0[di+1] = D_B9C0[si+1];
		D_B9C0[di+2] = D_B9C0[si+2];
		D_B9C0[di+3] = D_B9C0[si+3];
		si -= 4;
		di -= 4;
	}
	
	si += 4;
	val_x += D_E07E[bx];
	if (D_0130 == 7) {
		val_x -= 0x23;
	}
	
	D_B9C0[si] = cl;
	D_B9C0[si+1] = val_x & 0xFF;
	D_B9C0[si+2] = val_x >> 8;
	
	unsigned char cl_val = D_E0A2[bx / 2];
	if (D_0130 == 7) {
		cl_val += 8;
	}
	D_B9C0[si+3] = cl_val;
	
	D_B9BE += 4;
	D_D442 += 4;
}

/* Copy Protection check bypass */
int C_1705(char* buffer)
{
	return 1; /* Always report copy check success */
}

/* Script player interpreter */
int C_177B(void)
{
	D_BB60 = 1;
	/* SCRIPT_12/init_sal behavior */
	D_B9BA = -1;
	D_00EE = 1;
	D_B9C0[0] = 0;
	D_B9C0[1] = 0;
	D_B9C0[2] = 0;
	D_B9C0[3] = 0xFF;
	fprintf(stderr, "D_B9BE before increment is %d\n", D_B9BE);
	D_B9BE = 3;

	int script_idx = 0;
	int D_BB67 = 0;

	while (D_BB94[script_idx] != 0xFF) {
		unsigned char cmd = D_BB94[script_idx];
		switch (cmd) {
			case 0x00: /* SCRIPT_00/set_tune */
				D_B9C0[0] = D_BB94[script_idx + 1];
				script_idx += 2;
				break;
			case 0x02: /* SCRIPT_02/set_bg */
				D_B9C0[2] = D_BB94[script_idx + 1];
				script_idx += 2;
				break;
			case 0x04: { /* SCRIPT_04/set_fig */
				int di = D_B9BE;
				D_B9C0[di] = D_BB94[script_idx + 1];
				unsigned short fx = D_BB94[script_idx + 2] | (D_BB94[script_idx + 3] << 8);
				if (D_BB67 != 0) {
					D_BB67--;
					fx += D_BB65;
				}
				D_B9C0[di + 1] = fx & 0xFF;
				D_B9C0[di + 2] = fx >> 8;
				D_B9C0[di + 3] = D_BB94[script_idx + 4];
				D_B9C0[di + 4] = 0xFF;
				D_B9BE += 4;
				script_idx += 5;
				break;
			}
			case 0x06: { /* SCRIPT_06/chg_fig */
				int chg_idx = D_BB94[script_idx + 1];
				unsigned short val2 = D_BB94[script_idx + 2] | (D_BB94[script_idx + 3] << 8);
				unsigned short val4 = D_BB94[script_idx + 4] | (D_BB94[script_idx + 5] << 8);
				unsigned char *ptr = (unsigned char*)&D_B9C0[3];
				ptr[chg_idx * 4 + 3] = val2 & 0xFF;
				ptr[chg_idx * 4 + 4] = val2 >> 8;
				ptr[chg_idx * 4 + 5] = val4 & 0xFF;
				ptr[chg_idx * 4 + 6] = val4 >> 8;
				script_idx += 6;
				break;
			}
			case 0x08: /* SCRIPT_08/do_scr */
				render();
				script_idx += 1;
				break;
			case 0x0A: { /* SCRIPT_0a/del_fig */
				int del_idx = D_BB94[script_idx + 1];
				int target_offset = 3 + del_idx * 4;
				D_B9BE -= 4;
				for (int i = target_offset; i < D_B9BE; i++) {
					D_B9C0[i] = D_B9C0[i + 4];
				}
				D_B9C0[D_B9BE] = 0xFF;
				script_idx += 2;
				break;
			}
			case 0x0C: /* SCRIPT_0c/set_wipe */
				D_B9C0[1] = 1;
				script_idx += 1;
				break;
			case 0x0E: /* SCRIPT_0e/set_nowipe */
				D_B9C0[1] = 0;
				script_idx += 1;
				break;
			case 0x10: /* SCRIPT_10/wait */
				C_1906(D_BB94[script_idx + 1]);
				script_idx += 2;
				break;
			case 0x12: /* SCRIPT_12/init_sal */
				D_B9BA = -1;
				D_00EE = 1;
				D_B9C0[0] = 0;
				D_B9C0[1] = 0;
				D_B9C0[2] = 0;
				D_B9C0[3] = 0xFF;
				D_B9BE = 3;
				script_idx += 1;
				break;
			case 0x14: /* SCRIPT_14/set_pos */
				D_BB67 = 2;
				script_idx += 3;
				break;
			case 0x16: /* SCRIPT_16/inc_x */
				D_BB65 += (char)D_BB94[script_idx + 1];
				script_idx += 2;
				break;
			case 0x18: /* SCRIPT_18/loop */
				C_1906(D_BB94[script_idx + 1]);
				script_idx += 2;
				break;
		}

		/* Keyboard interrupt check (demo mode abort hook) */
		if (D_0156 != 0) {
			DoInput(0);
			if (D_DE69 != 0) {
				GetKey();
				D_BB60 = 0;
				return 1;
			}
		}
	}
	D_BB60 = 0;
	return 0;
}

/* Tick delay + input checker wrapper */
int C_191C(int ticks)
{
	int count = ticks / 2;
	if (count <= 0) count = 1;
	for (int i = 0; i < count; i++) {
		C_1906(2);
		DoInput(0);
		if (D_DE69 != 0) {
			return 1;
		}
	}
	return 0;
}

/* Plays specific cutscene sequence */
void Cutscene(char* script_name, int idx)
{
	int prev_D_00EA = D_00EA;
	int prev_D_00E4 = D_00E4;

	load_animation_script(script_name, D_BB94);
	load_sprite_assets(idx);

	D_00E4 = 0;
	BB_clear();
	D_00EE = 1;
	D_00EA = 4;

	C_177B();

	D_D4E2 = 0xFFFF;
	D_00EA = prev_D_00EA;
	D_00E4 = prev_D_00E4;
}

/* Parses text scripts and maps indexes offsets */
static void parse_script_indices(char *script_data, int *index_array)
{
	int si = 0;
	for (int cl = 0; cl < 0x2A; cl++) {
		index_array[cl] = si;
		while ((unsigned char)script_data[si] != 0xFF) {
			if ((unsigned char)script_data[si] == 0x18) {
				si += 2;
			} else {
				si += 0x11;
			}
		}
		si++;
	}
}

void C_19BD(void)
{
	parse_script_indices(D_C2B8, D_C264);
}

void C_19E9(void)
{
	parse_script_indices(D_CCCE, D_CC7A);
}

/* Local memcpy utility */
static void C_2329(char *dst, char *src, char *src_end)
{
	int len = src_end - src;
	memcpy(dst, src, len);
}

/* Shift drawing coordinates helper */
static void C_2341(RenderEntry *dst, char *src, int offset)
{
	memcpy(&dst[0], src + 1, 4);
	memcpy(&dst[1], src + 6, 4);
	dst[0].f_01 += offset;
	dst[1].f_01 += offset;
}

/* Tick scripting and animation engine */
void C_2366(void)
{
	D_B9C0[0] = 0;
	D_B9C0[1] = 0;

	D_BB65 = (D_BB65 + 3) & ~3;
	D_010E = (D_010E + 3) & ~3;

	int len = D_C232 - D_C230;
	memcpy(&D_B9C0[3], (void*)D_C230, len);
	int current_offset = len;

	if (D_00F8 <= 1) {
		int si = D_C262;
		while ((unsigned char)D_C2B8[si] == 0xFF) {
			int action;
			if (D_0156 != 0) {
				action = C_2ACE();
			} else {
				action = C_20E3();
			}
			if (action >= 0x2A) action = 0;
			si = D_C264[action];
			D_C262 = si;
		}

		D_010C = (unsigned char)D_C2B8[si + 1];
		unsigned char scale_flags = D_C2B8[si + 2];
		D_D43E = scale_flags >> 1;
		D_00EC = scale_flags & 1;

		int player_x = D_BB65;
		if (D_011A == 0) {
			char dx = D_C2B8[si + 4];
			D_BB65 += dx;
			player_x = D_BB65;
			if (player_x > D_0102) {
				D_BB65 = D_0102;
				player_x = D_0102;
			}
			if (player_x < D_0104) {
				D_BB65 = D_0104;
				player_x = D_0104;
			}
		}

		if (D_00FA <= 0) {
			int cx_bound = player_x + 8;
			if (D_D43A != 0) cx_bound -= 4;
			if (D_010C == 0x0B) {
				if (cx_bound - D_010E >= -0x10) {
					D_010C = 0;
					D_C262 = D_C264[0];
				}
			}
			if (cx_bound > D_010E) {
				int over = cx_bound - D_010E;
				D_BB65 -= over;
			}
		}

		int cam_x;
		if (D_011A != 0) {
			cam_x = D_010E - 170;
			D_00E4 = cam_x;
		} else {
			cam_x = D_BB65 - 150;
			D_00E4 = cam_x;
		}
		if (cam_x > D_0108) {
			D_00E4 = D_0108;
		}
		if (cam_x < D_010A) {
			D_00E4 = D_010A;
		}

		if (D_011A == 0) {
			D_B9C0[0] = D_C2B8[si + 6];
		}

		C_2341((RenderEntry*)&D_B9C0[3 + current_offset], &D_C2B8[si + 7], D_BB65);
		current_offset += 8;
		D_C262 += 0x11;
	}

	if (D_00FA <= 1) {
		int si = D_CC78;
		while ((unsigned char)D_CCCE[si] == 0xFF) {
			int action = C_268A();
			si = D_CC7A[action];
			D_CC78 = si;
		}

		D_0110 = (unsigned char)D_CCCE[si + 1];
		unsigned char scale_flags = D_CCCE[si + 2];
		D_D440 = scale_flags >> 1;
		D_0112 = scale_flags & 1;

		if (D_0118 == 0) {
			char dx = D_CCCE[si + 4];
			D_010E += dx;
			if (D_012E == 0 && D_00F0 == 0 && D_D43A == 0) {
				if (D_010E < D_0104) {
					D_010E = D_0104;
				}
			}
		}

		if (D_00FA <= 0) {
			int player_boundary = D_BB65 + 8;
			if (D_D43A != 0) player_boundary -= 4;
			if (D_0110 == 0x0A) {
				if (D_010E - player_boundary <= 0x10) {
					D_0110 = 0;
					D_CC78 = D_CC7A[0];
				}
			}
			if (D_010E < player_boundary) {
				D_010E = player_boundary;
			}
		}

		if (D_010E < D_0100) {
			D_010E = D_0100;
		}

		unsigned char snd = D_CCCE[si + 6];
		if (snd != 0) {
			D_B9C0[0] = snd;
		}

		RenderEntry *e1 = (RenderEntry*)&D_B9C0[3 + current_offset];
		e1->f_00 = D_CCCE[si + 8];
		int val_x = D_CCCE[si + 9] | (D_CCCE[si + 10] << 8);
		if (D_00F2 != 0) val_x += 4;
		if (D_D43A != 0) val_x += 12;
		val_x += D_010E;
		if (D_00F2 == 0 && D_D43A == 0) {
			val_x |= 0x4000;
		}
		e1->f_01 = val_x;
		unsigned char val_f03 = D_CCCE[si + 11];
		if (D_D43A != 0) val_f03 += D_D43C;
		e1->f_03 = val_f03;

		RenderEntry *e2 = (RenderEntry*)&D_B9C0[3 + current_offset + 4];
		unsigned char f00_2 = D_CCCE[si + 13];
		if (D_D43A == 0) {
			f00_2 += D_0158;
		}
		e2->f_00 = f00_2;
		int val_x2 = D_CCCE[si + 14] | (D_CCCE[si + 15] << 8);
		if (D_00F2 != 0) val_x2 += 4;
		if (D_D43A != 0) val_x2 += 12;
		val_x2 += D_010E;
		e2->f_01 = val_x2;
		e2->f_03 = D_CCCE[si + 16];

		current_offset += 8;
		D_CC78 += 0x11;
	}

	int len_rem = D_C234 - D_C232;
	D_D442 = current_offset + 3;
	memcpy(&D_B9C0[3 + current_offset], (void*)D_C232, len_rem);
	current_offset += len_rem;

	D_B9C0[3 + current_offset] = 0xFF;
	D_B9BE = 3 + current_offset;
}

/* Clears bottom 16 scanlines in backbuffer */
void ClearBottom(void)
{
	memset(&D_0337[0x3980], 0, 1280);
}

static const unsigned char triangle_left[14] = {
	0xA0, 0x00,
	0xA8, 0x00,
	0xAA, 0x00,
	0xAA, 0x80,
	0xAA, 0x00,
	0xA8, 0x00,
	0xA0, 0x00
};

static const unsigned char triangle_right[14] = {
	0x00, 0x05,
	0x00, 0x15,
	0x00, 0x55,
	0x01, 0x55,
	0x00, 0x55,
	0x00, 0x15,
	0x00, 0x05
};

static void clear_8x8_at(int offset)
{
	for (int i = 0; i < 7; i++) {
		D_0337[offset + i * 80] = 0;
		D_0337[offset + i * 80 + 1] = 0;
	}
}

static void draw_triangle_left(int idx)
{
	int offset = 0x3C50 + (idx * 3 - 2);
	for (int i = 0; i < 7; i++) {
		D_0337[offset + i * 80] = triangle_left[i * 2];
		D_0337[offset + i * 80 + 1] = triangle_left[i * 2 + 1];
	}
}

static void draw_triangle_right(int idx)
{
	int offset = 0x3CA0 - (idx * 3);
	for (int i = 0; i < 7; i++) {
		D_0337[offset + i * 80] = triangle_right[i * 2];
		D_0337[offset + i * 80 + 1] = triangle_right[i * 2 + 1];
	}
}


/* Life meters drawing dummy stub */
void DrMeters(void)
{
	if (D_00F8 != 0) {
		k_StrL = 0;
	}
	int prev_L = k_StrL;
	int prev_R = k_StrR;

	if (D_0170 != 0) {
		k_StrL = 0;
	}
	if (D_0118 != 0) {
		k_StrR = 0;
		ClearBottom();
	} else if (D_011A != 0) {
		k_StrL = 0;
		ClearBottom();
	} else {
		int diff_l = k_StrL - D_D4E2;
		if (diff_l == 1) {
			draw_triangle_left(k_StrL);
		} else if (diff_l != 0) {
			ClearBottom();
			for (int i = 1; i <= k_StrL; i++) {
				draw_triangle_left(i);
			}
			for (int i = 1; i <= k_StrR; i++) {
				draw_triangle_right(i);
			}
		}

		int diff_r = k_StrR - D_D4E0;
		if (diff_r == 1) {
			draw_triangle_right(k_StrR);
		} else if (diff_r != 0) {
			ClearBottom();
			for (int i = 1; i <= k_StrL; i++) {
				draw_triangle_left(i);
			}
			for (int i = 1; i <= k_StrR; i++) {
				draw_triangle_right(i);
			}
		}
	}

	D_D4E2 = k_StrL;
	D_D4E0 = k_StrR;

	if (D_D500 != 0) {
		D_D500--;
	} else if (k_StrL <= 2 && k_StrL > 0) {
		D_D500 = 1;
		clear_8x8_at(0x3C51);
		clear_8x8_at(0x3C54);
		D_D4E2 = 0;
	}

	if (D_D501 != 0) {
		D_D501--;
	} else if (k_StrR <= 2 && k_StrR > 0) {
		D_D501 = 1;
		clear_8x8_at(0x3C9D);
		clear_8x8_at(0x3C9A);
		D_D4E0 = 0;
	}

	k_StrL = prev_L;
	k_StrR = prev_R;
}

/* Draws life status points */
