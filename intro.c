/* KARATEKA - C replacements for original assembly routines. */
#include <string.h>
#include "karateka.h"
#include "cutscene.h"
#include "intro.h"
#include "render_scene.h"
#include "render_sprites.h"
#include "render_text.h"

static void draw_intro_text_screen(const char *top_line, const char *bottom_line)
{
	BB_clear();
	draw_text_block((320 - text_width(top_line)) / 2, 78, top_line, 3);
	draw_text_block((320 - text_width(bottom_line)) / 2, 96, bottom_line, 3);
	BB_flip();
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
