/* KARATEKA - C replacements for original assembly routines. */
#include "karateka.h"
#include "font.h"
#include "render_text.h"

static void draw_cga_pixel(int x, int y, unsigned char color)
{
	if (x < 0 || x >= 320 || y < 0 || y >= 200)
		return;

	int byte_idx = y * 80 + (x / 4);
	int bit_shift = 6 - (2 * (x % 4));
	cga_buffer[byte_idx] &= (unsigned char)~(0x03u << bit_shift);
	cga_buffer[byte_idx] |= (unsigned char)((color & 0x03u) << bit_shift);
}

static const unsigned char digit_glyphs[10][7] = {
	{ 0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E },
	{ 0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E },
	{ 0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F },
	{ 0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E },
	{ 0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02 },
	{ 0x1F, 0x10, 0x10, 0x1E, 0x01, 0x01, 0x1E },
	{ 0x0E, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x0E },
	{ 0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08 },
	{ 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E },
	{ 0x0E, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x0E }
};

static int draw_font_char(int x, int y, char ch, unsigned char color)
{
	if (ch >= '0' && ch <= '9') {
		const unsigned char *glyph = digit_glyphs[ch - '0'];
		int row, column;

		for (row = 0; row < 7; row++) {
			for (column = 0; column < 5; column++) {
				if (glyph[row] & (0x10u >> column))
					draw_cga_pixel(x + column, y + row, color);
			}
		}
		return 6;
	}

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

void draw_text_block(int x, int y, const char *text, unsigned char color)
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

int text_width(const char *text)
{
	int width = 0;
	for (const char *p = text; *p != '\0'; p++) {
		char ch = *p;
		if (ch >= 'A' && ch <= 'Z')
			ch = (char)(ch - 'A' + 'a');
		if (ch >= '0' && ch <= '9') {
			width += 6;
			continue;
		}
		int idx = font_index(ch);
		width += idx < 0 ? 8 : font_advance_px[idx];
	}
	return width;
}
