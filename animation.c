/* KARATEKA - C replacements for original assembly routines. */
#include <string.h>
#include "karateka.h"
#include "animation.h"
#include "render_scene.h"

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
