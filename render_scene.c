/* KARATEKA - C replacements for original assembly routines. */
#include <string.h>
#include "karateka.h"
#include "meters.h"
#include "render_scene.h"
#include "render_sprites.h"

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
				memset(&cga_buffer[60 * 80], 0x00, 50 * 80);
				memcpy(&cga_buffer[80 * 80], &D_A606[1500], 22 * 80);
				memset(&cga_buffer[90 * 80], 0x00, 1 * 80);
			}
			/* Draw dithered floor pattern */
			for (int row = 154; row < 184; row++) {
				memset(&cga_buffer[row * 80], row % 2 ? 0x00 : 0xAA, 80);
			}
		} else {
			if (cameraClamp != D_B9BA) {
				memset(cga_buffer, 0x00, 114 * 80);
			}
			for (int row = 154; row < 184; row++) {
				memset(&cga_buffer[row * 80], row % 2 ? 0x00 : 0x55, 80);
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

		if (x & 0x8000) {
			x -= 0x10000;
		}

		if (x >= 0 && (x & 0x4000)) {
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
