/*
	KARATEKA - Assembly Function Replacements in C
	Copyright 2026 Retro Porting Project
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "karateka.h"
#include "assembly_replacements.h"
#include "font.h"

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

/* Helper to reverse 2bpp pixels in a byte for horizontal mirroring */
static unsigned char reverse_2bpp(unsigned char b)
{
	return ((b & 0x03) << 6) | ((b & 0x0C) << 2) | ((b & 0x30) >> 2) | ((b & 0xC0) >> 6);
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
		rleDataCount = ks_data[figDataIndex++];
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
		rleMaskCount = km_data[figMaskIndex++];
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
	cga_buffer[byte_idx] &= ~(0x03 << pixel_shift);
	/* Set the new 2 bits */
	cga_buffer[byte_idx] |= (color_val & 0x03) << pixel_shift;
}

/* Blits standard sprite to backbuffer (Column-major) */
void putFig(int fig, int x, int y)
{
	unsigned short data_offset = (unsigned char)ks_index[fig * 2] | ((unsigned char)ks_index[fig * 2 + 1] << 8);
	unsigned short mask_offset = (unsigned char)km_index[fig * 2] | ((unsigned char)km_index[fig * 2 + 1] << 8);

	fig_stride = ks_data[data_offset];
	fig_height = ks_data[data_offset + 1];
	fig_width = fig_stride * 4;

	y -= fig_height;
	x -= cameraClamp;

	rleDataCount = 0;
	rleMaskCount = 0;
	figDataIndex = data_offset + 3;
	figMaskIndex = mask_offset + 3;

	for (int col = 0; col < fig_stride; col++) {
		for (int row = 0; row < fig_height; row++) {
			int py = y + row;
			unsigned char m, d;
			if (fig < 200) {
				m = get_rle_mask_byte();
				d = get_rle_data_byte();
			} else {
				m = 0xFF;
				d = ks_data[figDataIndex++];
			}

			for (int p = 0; p < 4; p++) {
				int px = x + col * 4 + p;
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

/* Blits horizontally mirrored sprite to backbuffer (Column-major) */
void putFig_flipx(int fig, int x, int y)
{
	unsigned short data_offset = (unsigned char)ks_index[fig * 2] | ((unsigned char)ks_index[fig * 2 + 1] << 8);
	unsigned short mask_offset = (unsigned char)km_index[fig * 2] | ((unsigned char)km_index[fig * 2 + 1] << 8);

	fig_stride = ks_data[data_offset];
	fig_height = ks_data[data_offset + 1];
	fig_width = fig_stride * 4;

	y -= fig_height;
	x -= cameraClamp;

	rleDataCount = 0;
	rleMaskCount = 0;
	figDataIndex = data_offset + 3;
	figMaskIndex = mask_offset + 3;

	for (int col = 0; col < fig_stride; col++) {
		for (int row = 0; row < fig_height; row++) {
			int py = y + row;
			unsigned char m, d;
			if (fig < 200) {
				m = get_rle_mask_byte();
				d = get_rle_data_byte();
			} else {
				m = 0xFF;
				d = ks_data[figDataIndex++];
			}

			m = reverse_2bpp(m);
			d = reverse_2bpp(d);

			for (int p = 0; p < 4; p++) {
				int px = x + (fig_stride - 1 - col) * 4 + p;
				int shift = 6 - 2 * p;
				unsigned char mask_val = (m >> shift) & 0x03;
				unsigned char data_val = (d >> shift) & 0x03;

				if (data_val == 2) {
					data_val = 1;
				}

				if (mask_val != 0) {
					write_pixel(px, py, data_val);
				}
			}
		}
	}
}

/* Pseudo-random number generator mapping to original Prime LCG */
int k_rand(int range)
{
	int r = C_414A();
	r = (r * 2) & 0xFFFF;
	return (int)(((unsigned long)(range + 1) * r) >> 16);
}

/* Helper to clear ground scanlines */
static void C_0E39(void)
{
	if (D_00EA < 2) {
		/* Clear 42 rows starting at row 114 */
		memset(&cga_buffer[114 * 80], 0, 42 * 80);
	} else {
		/* Clear 48 rows starting at row 108 */
		memset(&cga_buffer[108 * 80], 0, 48 * 80);
	}
}

/* Background graphics renderer */
void renderBG(int bg)
{
	if (D_00EE == 1) {
		if (D_00EA < 2) {
			/* Fill outdoor sky with Cyan (0x55 = color index 1) */
			memset(cga_buffer, 0x55, 80 * 80);
		} else {
			BB_clear();
		}
		if (D_BB60 == 0) {
			DrMeters();
		}
	}

	if (D_00EA < 2) {
		/* Outdoor rendering */
		if (cameraClamp != D_B9BA) {
			/* Fill sky */
			memset(cga_buffer, 0x55, 80 * 80);
			/* Copy Fuji background mountain graphics */
			memcpy(&cga_buffer[80 * 80], &D_A606[1500], 34 * 80);
			/* Draw white snow line */
			memset(&cga_buffer[106 * 80], 0xFF, 1 * 80);
			memset(&cga_buffer[107 * 80], 0x00, 3 * 80);
		}
		C_0E39();
		
		/* Draw dithered floor pattern based on scroll phase */
		unsigned char floor_pattern = (cameraClamp & 1) ? 0x66 : 0x99;
		for (int row = 154; row < 184; row++) {
			memset(&cga_buffer[row * 80], floor_pattern, 80);
			floor_pattern = ~floor_pattern;
		}
	} else {
		/* Indoor palace rendering */
		C_0E39();
		if (D_00EA == 2) {
			if (cameraClamp != D_B9BA) {
				memset(&cga_buffer[80 * 80], 0x00, 30 * 80);
				memcpy(&cga_buffer[80 * 80], &D_A606[1500], 22 * 80);
				memset(&cga_buffer[90 * 80], 0x00, 1 * 80);
			}
			/* Draw dithered floor pattern */
			for (int row = 154; row < 184; row++) {
				memset(&cga_buffer[row * 80], row % 2 ? 0xAA : 0x00, 15 * 80);
			}
		} else {
			if (cameraClamp != D_B9BA) {
				memset(cga_buffer, 0x00, 114 * 80);
			}
			for (int row = 154; row < 184; row++) {
				memset(&cga_buffer[row * 80], row % 2 ? 0x55 : 0x00, 15 * 80);
			}
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
	while (entries[idx].fig_id != 0xFF) {
		int fig = entries[idx].fig_id;
		int x = entries[idx].x_pos;
		int y = entries[idx].y_pos;

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

	D_B9BA = cameraClamp;
}

static void draw_cga_pixel(int x, int y, unsigned char color)
{
	if (x < 0 || x >= 320 || y < 0 || y >= 200)
		return;

	int byte_idx = y * 80 + (x / 4);
	int bit_shift = 6 - (2 * (x % 4));
	cga_buffer[byte_idx] &= (unsigned char)~(0x03u << bit_shift);
	cga_buffer[byte_idx] |= (unsigned char)((color & 0x03u) << bit_shift);
}

static int draw_font_char(int x, int y, char ch, unsigned char color)
{
	if (ch >= 'A' && ch <= 'Z') ch = (char)(ch - 'A' + 'a');
	int idx = font_index(ch);
	if (idx < 0)
		return 8;

	int height = font_height[idx];
	int width_bytes = font_width_bytes[idx];
	const uint8_t *glyph = font_glyph(idx);
	for (int byte_col = 0; byte_col < width_bytes; byte_col++) {
		for (int row = 0; row < height; row++) {
			uint8_t bits = glyph[byte_col * height + row];
			for (int bit = 0; bit < 8; bit++) {
				if (bits & (0x80u >> bit))
					draw_cga_pixel(x + byte_col * 8 + (8 - bit), y + row, color);
			}
		}
	}

	return font_advance_px[idx];
}

static void draw_text_block(int x, int y, const char *text, unsigned char color)
{
	int cursor_x = x;
	for (const char *p = text; *p != '\0'; p++) {
		unsigned char ch = (unsigned char)*p;
		if (ch == '\n' || ch == '\r') {
			y += 12;
			cursor_x = x;
			continue;
		}
		cursor_x += draw_font_char(cursor_x, y, (char)ch, color);
	}
}

static int text_width(const char *text)
{
	int width = 0;
	for (const char *p = text; *p != '\0'; p++) {
		char ch = *p;
		if (ch >= 'A' && ch <= 'Z')
			ch = (char)(ch - 'A' + 'a');
		int idx = font_index(ch);
		width += idx < 0 ? 8 : font_advance_px[idx];
	}
	return width;
}

static void draw_intro_text_screen(const char *top_line, const char *bottom_line)
{
	BB_clear();
	draw_text_block((320 - text_width(top_line)) / 2, 78, top_line, 3);
	draw_text_block((320 - text_width(bottom_line)) / 2, 96, bottom_line, 3);
	BB_flip();
}

int quit_confirmation_scene(void)
{
	static const char *question = "Are you sure want to quit?";
	static const char *choices = "Y / N";

	for (;;) {
		BB_clear();
		draw_text_block((320 - text_width(question)) / 2, 86, question, 3);
		draw_text_block((320 - text_width(choices)) / 2, 106, choices, 3);
		BB_flip();

		WaitKey();
		switch (GetKey()) {
			case 'y':
			case 'Y':
				return 1;
			case 'n':
			case 'N':
				return 0;
		}
	}
}

/* Draws intro title screen */
int intro_karateka_title(void)
{
	BB_clear();
	memcpy(&cga_buffer[0x15E0], D_A606, 0x1040);
	BB_flip();
	if (C_191C(0x12)) return 1;
	putFig(0x5C, 40, 180);
	BB_flip();
	if (C_191C(0x48)) return 1;
	return 0;
}

/* Draws Broderbund logo screen */
int intro_publisher(void)
{
	BB_clear();
	putFig(0x5B, 106, 115);
	BB_flip();
	D_0168 = 1;
	int ret = C_191C(0x48);
	D_0168 = 0;
	return ret;
}

/* "a game by" / "jordan mechner" */
int intro_developer(void)
{
	draw_intro_text_screen("a game by", "jordan mechner");
	return C_191C(0x48);
}

/* "ibm version by" / "the connelley group" */
int intro_ibm_port(void)
{
	draw_intro_text_screen("ibm version by", "the connelley group");
	return C_191C(0x48);
}

/* Opening story scroll */
int intro_story_scroll(void)
{
	static const char *story_lines[] = {
		"high atop a craggy cliff",
		"guarded by an army of",
		"fierce warriors stands the",
		"fortress of the evil",
		"warlord akuma deep in the",
		"darkest dungeon of the",
		"castle akuma gloats over",
		"his lovely captive the",
		"princess mariko",
		"",
		"you are one trained in the",
		"way of karate a karateka",
		"alone and unarmed you must",
		"defeat akuma and rescue the",
		"beautiful mariko",
		"",
		"put fear and self concern",
		"behind you focus your will",
		"on your objective accepting",
		"death as a possibility this",
		"is the way of the karateka"
	};

	const int line_count = (int)(sizeof(story_lines) / sizeof(story_lines[0]));
	const int scroll_end = 200 + (line_count - 1) * 14 + 7;
	BB_clear();
	BB_flip();
	script_frame_pace_reset();

	for (int scroll_offset = 0; scroll_offset <= scroll_end; scroll_offset++) {
		BB_clear();
		for (int line = 0; line < line_count; line++) {
			int y = 200 + line * 14 - scroll_offset;
			if (y >= -7 && y < 200 && story_lines[line][0] != '\0')
				draw_text_block(18, y, story_lines[line], 3);
		}
		BB_flip();
		script_frame_pace(55);
		DoInput(0);
		if (isKeyPending != 0) {
			GetKey();
			return 1;
		}
	}
	return 0;
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
		val_x = playerPosClamp;
		cl = 0x61;
		if (offset < 3) {
			cl++;
		}
	} else {
		val_x = enemyClamp;
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
	val_x += D_E07E[bx / 2];
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
	script_frame_pace_reset();
	/* SCRIPT_12/init_sal behavior */
	D_B9BA = -1;
	D_00EE = 1;
	D_B9C0[0] = 0;
	D_B9C0[1] = 0;
	D_B9C0[2] = 0;
	D_B9C0[3] = 0xFF;
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
					fx += playerPosClamp;
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
				int entry_offset = 3 + chg_idx * 4;
				D_B9C0[entry_offset] = D_BB94[script_idx + 2];
				D_B9C0[entry_offset + 1] = D_BB94[script_idx + 3];
				D_B9C0[entry_offset + 2] = D_BB94[script_idx + 4];
				D_B9C0[entry_offset + 3] = D_BB94[script_idx + 5];
				script_idx += 6;
				break;
			}
			case 0x08: /* SCRIPT_08/do_scr */
				render();
				script_frame_pace(165);
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
				playerPosClamp += (char)D_BB94[script_idx + 1];
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
			if (isKeyPending != 0) {
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
		if (isKeyPending != 0) {
			return 1;
		}
	}
	return 0;
}

/* Plays specific cutscene sequence */
void Cutscene(char* script_name, int idx)
{
	int prev_D_00EA = D_00EA;
	int prev_cameraClamp = cameraClamp;

	load_animation_script(script_name, D_BB94);
	load_sprite_assets(idx);

	cameraClamp = 0;
	BB_clear();
	D_00EE = 1;
	D_00EA = 4;

	C_177B();

	D_D4E2 = 0xFFFF;
	D_00EA = prev_D_00EA;
	cameraClamp = prev_cameraClamp;
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
	dst[0].x_pos += offset;
	dst[1].x_pos += offset;
}

/* Tick scripting and animation engine */
void C_2366(void)
{
	D_B9C0[0] = 0;
	D_B9C0[1] = 0;

	playerPosClamp = (playerPosClamp + 3) & ~3;
	enemyClamp = (enemyClamp + 3) & ~3;

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

		int player_x = playerPosClamp;
		if (D_011A == 0) {
			char dx = D_C2B8[si + 4];
			playerPosClamp += dx;
			player_x = playerPosClamp;
			if (player_x < min_boundary) {
				playerPosClamp = min_boundary;
				player_x = min_boundary;
			}
			if (player_x > max_boundary) {
				playerPosClamp = max_boundary;
				player_x = max_boundary;
			}
		}

		if (D_00FA <= 0) {
			int cx_bound = player_x + 8;
			if (D_D43A != 0) cx_bound -= 4;
			if (D_010C == 0x0B) {
				if (cx_bound - enemyClamp >= -0x10) {
					D_010C = 0;
					D_C262 = D_C264[0];
				}
			}
			if (cx_bound > enemyClamp) {
				int over = cx_bound - enemyClamp;
				playerPosClamp -= over;
			}
		}

		int cam_x;
		if (D_011A != 0) {
			cam_x = enemyClamp - 170;
		} else {
			cam_x = playerPosClamp - 150;
		}
		if (cam_x < minCameraScroll) {
			cam_x = minCameraScroll;
		}
		if (cam_x > maxCameraScroll) {
			cam_x = maxCameraScroll;
		}
		cameraClamp = cam_x;

		if (D_011A == 0) {
			D_B9C0[0] = D_C2B8[si + 6];
		}

		C_2341((RenderEntry*)&D_B9C0[3 + current_offset], &D_C2B8[si + 7], playerPosClamp);
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
			enemyClamp += dx;
			if (D_012E == 0 && D_00F0 == 0 && D_D43A == 0) {
				if (enemyClamp > max_boundary) {
					enemyClamp = max_boundary;
				}
			}
		}

		if (D_00FA <= 0) {
			int player_boundary = playerPosClamp + 8;
			if (D_D43A != 0) player_boundary -= 4;
			if (D_0110 == 0x0A) {
				if (enemyClamp - player_boundary <= 0x10) {
					D_0110 = 0;
					D_CC78 = D_CC7A[0];
				}
			}
			if (enemyClamp < player_boundary) {
				enemyClamp = player_boundary;
			}
		}

		if (enemyClamp < D_0100) {
			enemyClamp = D_0100;
		}

		unsigned char snd = D_CCCE[si + 6];
		if (snd != 0) {
			D_B9C0[0] = snd;
		}

		RenderEntry *e1 = (RenderEntry*)&D_B9C0[3 + current_offset];
		e1->fig_id = D_CCCE[si + 8];
		int val_x = D_CCCE[si + 9] | (D_CCCE[si + 10] << 8);
		if (D_00F2 != 0) val_x += 4;
		if (D_D43A != 0) val_x += 12;
		val_x += enemyClamp;
		if (D_00F2 == 0 && D_D43A == 0) {
			val_x |= 0x4000;
		}
		e1->x_pos = val_x;
		unsigned char val_y = D_CCCE[si + 11];
		if (D_D43A != 0) val_y += D_D43C;
		e1->y_pos = val_y;

		RenderEntry *e2 = (RenderEntry*)&D_B9C0[3 + current_offset + 4];
		unsigned char f00_2 = D_CCCE[si + 13];
		if (D_D43A == 0) {
			f00_2 += D_0158;
		}
		e2->fig_id = f00_2;
		int val_x2 = D_CCCE[si + 14] | (D_CCCE[si + 15] << 8);
		if (D_00F2 != 0) val_x2 += 4;
		if (D_D43A != 0) val_x2 += 12;
		val_x2 += enemyClamp;
		e2->x_pos = val_x2;
		e2->y_pos = D_CCCE[si + 16];

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
	memset(&cga_buffer[0x3980], 0, 1280);
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
		cga_buffer[offset + i * 80] = 0;
		cga_buffer[offset + i * 80 + 1] = 0;
	}
}

static void draw_triangle_left(int idx)
{
	int offset = 0x3C50 + (idx * 3 - 2);
	for (int i = 0; i < 7; i++) {
		cga_buffer[offset + i * 80] = triangle_left[i * 2];
		cga_buffer[offset + i * 80 + 1] = triangle_left[i * 2 + 1];
	}
}

static void draw_triangle_right(int idx)
{
	int offset = 0x3CA0 - (idx * 3);
	for (int i = 0; i < 7; i++) {
		cga_buffer[offset + i * 80] = triangle_right[i * 2];
		cga_buffer[offset + i * 80 + 1] = triangle_right[i * 2 + 1];
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
