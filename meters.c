/* KARATEKA - C replacements for original assembly routines. */
#include <string.h>
#include "karateka.h"
#include "meters.h"

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
