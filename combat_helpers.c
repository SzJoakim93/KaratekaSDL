/* KARATEKA - C replacements for original assembly routines. */
#include "karateka.h"
#include "combat_helpers.h"

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
