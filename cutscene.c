/* KARATEKA - C replacements for original assembly routines. */
#include <string.h>
#include "karateka.h"
#include "cutscene.h"
#include "render_scene.h"

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
