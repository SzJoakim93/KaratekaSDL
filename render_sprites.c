/* KARATEKA - C replacements for original assembly routines. */
#include "karateka.h"
#include "render_sprites.h"

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
