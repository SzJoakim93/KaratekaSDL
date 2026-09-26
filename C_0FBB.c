/*
	KARATEKA
	Copyright 1986 Jordan Mechner
	IBM version by The Connelley Group
	reverse-coded by ergonomy_joe 2022

*/
#include <stdio.h>

#include "fcntl.h"
#include "karateka.h"

/*bb08	"castle.bcg"*/
/*bb13	"r"*/
/*bb15	"%d"*/
/*bb18	"%d %d %d"*/
/*bb21	"%d %d %d %d"*/
/*bb2d	"%d %d"*/
/*bb33	0*/

/*0FBB*/load_background_graphics(fname, bp14)
char *fname;
int bp14;
{
	int h;
	int bp08;
	int _p0a[2];

	h = open_file_safe(fname, O_RDONLY|O_RAW);
	read(h, D_A606 + bp14, 2);
	bp08 = D_A606[bp14] + (D_A606[bp14 + 1] << 8);
	read(h, D_A606 + bp14, bp08);
	close(h);

	return bp08;
}


/*draw "castle.bcg"*/
load_castle_bg()
{
	int h;
	int bp06;
	int _p08[2];

	BB_clear();

	h = open_file_safe(/*bb08*/"castle.bcg", O_RDONLY|O_RAW);
	read(h, D_0337, 2);
	bp06 = D_0337[0] + (D_0337[1] << 8);
	read(h, D_0337, bp06);
	close(h);

	BB_flip();
	sound(0x19);
}

/*load ks/km files*/
load_sprite_assets(idx)
int idx;
{
	int h;
	int sz;
	int i;
	int _p0e;
	int data_sz;
	int offset;
	int index;
	char fname[20];

	/*-- ".ind" --*/
	strcpy(fname, D_0264[idx]);
	strcat(fname, D_032D);
	h = open_file_safe(fname, O_RDONLY|O_RAW);
	read(h, ks_data + ks_base, 0x2a8);
	close(h);
	for(i = 0; i < 0x2a8; i += 4) {
		index = ks_data[ks_base + i];
		offset = ks_data[ks_base + i + 2] + (ks_data[ks_base + i + 3] << 8) + ks_base;
		ks_index[index * 2] = offset & 0xff;
		ks_index[index * 2 + 1] = offset >> 8;
		if(index == 0xff) {
			sz = ks_data[ks_base + i + 2] + (ks_data[ks_base + i + 3] << 8);
			i = 0x2a8;
		}
	}/*end for*/
	/*-- ".dat" --*/
	strcpy(fname, D_0264[idx]);
	strcat(fname, D_0332);
	h = open_file_safe(fname, O_RDONLY|O_RAW);
	data_sz = sz;
	read(h, ks_data + ks_base, data_sz);
	close(h);
	/*-- --*/
	if(ks_base == 0)
		ks_base = sz;
	/*-- ".ind" --*/
	strcpy(fname, D_02C0[idx]);
	strcat(fname, D_032D);
	h = open_file_safe(fname, O_RDONLY|O_RAW);
	read(h, km_data + km_base, 0x2a8);
	close(h);
	for(i = 0; i < 0x2a8; i += 4) {
		index = km_data[km_base + i];
		offset = km_data[km_base + i + 2] + (km_data[km_base + i + 3] << 8) + km_base;
		km_index[index * 2] = offset & 0xff;
		km_index[index * 2 + 1] = offset >> 8;
		if(index == 0xff) {
			sz = km_data[km_base + i+2] + (km_data[km_base + i+3] << 8);
			i = 0x2a8;
		}
	}/*end for*/
	/*-- ".dat" --*/
	strcpy(fname, D_02C0[idx]);
	strcat(fname, D_0332);
	h = open_file_safe(fname, O_RDONLY|O_RAW);
	read(h, km_data + km_base, sz);
	close(h);
	/*-- --*/
	if(km_base == 0)
		km_base = sz;
	/*-- --*/
}

/*12E0*/load_animation_script(path, bp6c)
char *path;
char *bp6c;
{
	char bp02[0x50];
	char *bp52;
	int _p58[3];
	int bp5a;/*index in text*/
	int _p5c;
	int bp5e;/*error flag*/
	int _p62[2];
	FILE *f;

	bp5e =
	bp5a = 0;

	while(!(f = fopen(path, /*bb13*/"r")))
		C_402A();/*int24h handler*/
	while((bp52 = fgets(bp02, 0x50, f)) != 0)
		parse_script_line(bp02, bp6c, &bp5a, &bp5e);
	fclose(f);
	D_BB92 = bp5a;

	return bp5e;
}

parse_script_line(bp16, bp18, bp1a, bp1c)
char *bp16;
char *bp18;
int *bp1a;
int *bp1c;
{
	int bp08;
	int bp0a;
	int bp0c;
	int bp0e;
	char bp10;
	char *comma;
	int parsed_val1, parsed_val2;

	comma = strchr(bp16, ',');
	if (!comma) {
		return;
	}
	bp08 = comma - bp16;
	bp16[bp08] = 0;

	for(bp0a = 0; bp0a < 0xe; bp0a ++) {
		bp10 = strncmp(bp16, D_01E8[bp0a], 5);
		if(bp10 == 0)
			break;
	}/*end for*/
	if(bp10 != 0) {
		(*bp1c) ++;
	} else
	switch(bp0a) {
		case 0:/*set_tune*/
			bp18[(*bp1a) ++] = 0;
			if (sscanf(&(bp16[bp08 + 1]), "%d", &parsed_val1) == 1) {
				bp18[(*bp1a) ++] = parsed_val1;
			} else {
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 1:/*set_bg*/
			bp18[(*bp1a) ++] = 2;
			if (sscanf(&(bp16[bp08 + 1]), "%d", &parsed_val1) == 1) {
				bp18[(*bp1a) ++] = parsed_val1;
			} else {
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 2:/*set_fig*/
			bp18[(*bp1a) ++] = 4;
			if (sscanf(&(bp16[bp08 + 1]), "%d %d %d", &parsed_val1, &bp0c, &bp0e) == 3) {
				bp18[(*bp1a) ++] = parsed_val1;
				bp18[(*bp1a) ++] = bp0c & 0xff;
				bp18[(*bp1a) ++] = bp0c >> 8;
				bp18[(*bp1a) ++] = bp0e;
			} else {
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 3:/*chg_fig*/
			bp18[(*bp1a) ++] = 6;
			if (sscanf(&(bp16[bp08 + 1]), "%d %d %d %d", &parsed_val1, &parsed_val2, &bp0c, &bp0e) == 4) {
				bp18[(*bp1a) ++] = parsed_val1;
				bp18[(*bp1a) ++] = parsed_val2;
				bp18[(*bp1a) ++] = bp0c & 0xff;
				bp18[(*bp1a) ++] = bp0c >> 8;
				bp18[(*bp1a) ++] = bp0e;
			} else {
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 4:/*do_scr*/
			bp18[(*bp1a) ++] = 8;
		break;
		case 5:/*del_fig*/
			bp18[(*bp1a) ++] = 0xa;
			if (sscanf(&(bp16[bp08 + 1]), "%d", &parsed_val1) == 1) {
				bp18[(*bp1a) ++] = parsed_val1;
			} else {
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 6:/*set_wipe*/
			bp18[(*bp1a) ++] = 0xc;
		break;
		case 7:/*set_nowipe*/
			bp18[(*bp1a) ++] = 0xe;
		break;
		case 8:/*wait*/
			bp18[(*bp1a) ++] = 0x10;
			if (sscanf(&(bp16[bp08 + 1]), "%d", &parsed_val1) == 1) {
				bp18[(*bp1a) ++] = parsed_val1;
			} else {
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 9:/*init_sal*/
			bp18[(*bp1a) ++] = 0x12;
		break;
		case 10:/*set_pos*/
			bp18[(*bp1a) ++] = 0x14;
			if (sscanf(&(bp16[bp08 + 1]), "%d %d", &parsed_val1, &parsed_val2) == 2) {
				bp18[(*bp1a) ++] = parsed_val1;
				bp18[(*bp1a) ++] = parsed_val2;
			} else {
				bp18[(*bp1a) ++] = 0;
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 11:/*inc_x*/
			bp18[(*bp1a) ++] = 0x16;
			if (sscanf(&(bp16[bp08 + 1]), "%d", &bp0c) == 1) {
				bp18[(*bp1a) ++] = bp0c & 0xff;
			} else {
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 12:/*loop*/
			bp18[(*bp1a) ++] = 0x18;
			if (sscanf(&(bp16[bp08 + 1]), "%d", &parsed_val1) == 1) {
				bp18[(*bp1a) ++] = parsed_val1;
			} else {
				bp18[(*bp1a) ++] = 0;
			}
		break;
		case 13:/*end_animation*/
			bp18[(*bp1a) ++] = 0xff;
		break;
	}/*end switch*/
}

/*16D6*/open_file_safe(fname, attr)
char *fname;
int attr;
{
#ifdef USE_SDL
	return open(fname, attr);
#else
	int h;

	while((h = open(fname, attr)) == -1)
		C_402A();/*int24h handler*/

	return h;
#endif
}
