#include "menu_scene.h"

#include <stdio.h>

#include "assembly_replacements.h"
#include "dos_sdl_bridge.h"
#include "settings.h"

typedef struct {
	int width;
	int height;
} Resolution;

static const Resolution resolutions[] = {
	{ 640, 400 },
	{ 640, 480 },
	{ 800, 600 },
	{ 1024, 768 },
	{ 1280, 720 },
	{ 1280, 800 },
	{ 1366, 768 },
	{ 1600, 900 },
	{ 1920, 1080 }
};

static const char *const main_items[] = {
	"resume",
	"settings",
	"quit"
};

static const char *const setting_names[] = {
	"resolution",
	"fullscreen",
	"native pc speaker",
	"back"
};

static void draw_menu_item(int y, const char *label, int selected)
{
	int color = selected ? 1 : 3;
	draw_text_block((320 - text_width(label)) / 2, y, label, (unsigned char)color);
}

static void draw_main_menu(int selected)
{
	int i;

	BB_clear();
	draw_menu_item(52, "pause menu", 0);
	for (i = 0; i < (int)(sizeof(main_items) / sizeof(main_items[0])); i++)
		draw_menu_item(82 + i * 20, main_items[i], i == selected);
	BB_flip();
}

static void draw_settings_menu(int selected)
{
	char resolution_value[32];
	char fullscreen_value[32];
	char speaker_value[40];
	char line[80];
	const char *values[] = {
		resolution_value,
		fullscreen_value,
		speaker_value,
		""
	};
	int i;

	snprintf(resolution_value, sizeof(resolution_value), "%dx%d",
		settings.resWidth, settings.resHeight);
	snprintf(fullscreen_value, sizeof(fullscreen_value), "%s",
		settings.fullscreen ? "yes" : "no");
	snprintf(speaker_value, sizeof(speaker_value), "%s",
		settings.use_native_pc_speaker ? "yes" : "no");

	BB_clear();
	draw_menu_item(30, "settings", 0);
	for (i = 0; i < (int)(sizeof(setting_names) / sizeof(setting_names[0])); i++) {
		if (i == 3) {
			snprintf(line, sizeof(line), "%s", setting_names[i]);
		} else {
			snprintf(line, sizeof(line), "%s: %s", setting_names[i], values[i]);
		}
		draw_menu_item(58 + i * 22, line, i == selected);
	}
	draw_text_block(13, 162, "left right change  enter select", 3);
	draw_text_block(89, 178, "esc back", 3);
	BB_flip();
}

static unsigned char wait_for_menu_input(void)
{
	for (;;) {
		if (DoInput(1)) {
			unsigned char key = GetKey();
			if (key)
				return key;
		}
		delay_ms(10);
	}
}

static void wait_for_input_release(void)
{
	while (DoInput(0)) {
		GetKey();
		delay_ms(10);
	}
}

static int is_select_key(unsigned char key)
{
	return key == ' ' || key == '\r' || key == 'a';
}

static int find_resolution(void)
{
	int i;

	for (i = 0; i < (int)(sizeof(resolutions) / sizeof(resolutions[0])); i++) {
		if (settings.resWidth == resolutions[i].width &&
			settings.resHeight == resolutions[i].height)
			return i;
	}
	return -1;
}

static void change_resolution(int direction)
{
	int index = find_resolution();
	int count = (int)(sizeof(resolutions) / sizeof(resolutions[0]));

	if (index < 0) {
		index = direction > 0 ? -1 : 0;
	}
	index = (index + direction + count) % count;
	settings.resWidth = resolutions[index].width;
	settings.resHeight = resolutions[index].height;
	settings_changed = 1;
}

static void adjust_setting(int selected, int direction)
{
	switch (selected) {
		case 0:
			change_resolution(direction);
			break;
		case 1:
			settings.fullscreen = !settings.fullscreen;
			settings_changed = 1;
			break;
		case 2:
			settings.use_native_pc_speaker = !settings.use_native_pc_speaker;
			settings_changed = 1;
			break;
	}
}

static void run_settings_menu(void)
{
	int selected = 0;

	for (;;) {
		unsigned char key;

		draw_settings_menu(selected);
		key = wait_for_menu_input();
		if (key == '8') {
			selected = (selected + 3) % 4;
		} else if (key == '2') {
			selected = (selected + 1) % 4;
		} else if (key == '4' || key == '6') {
			adjust_setting(selected, key == '4' ? -1 : 1);
		} else if (is_select_key(key)) {
			if (selected == 3) {
				wait_for_input_release();
				break;
			}
			adjust_setting(selected, 1);
		} else if (key == 0x1B || key == 'q') {
			wait_for_input_release();
			break;
		} else {
			continue;
		}

		wait_for_input_release();
	}

	if (settings_changed) {
		apply_settings();
		if (write_settings())
			settings_changed = 0;
	}
}

int quit_confirmation_scene(void)
{
	int selected = 0;

	for (;;) {
		unsigned char key;

		draw_main_menu(selected);
		key = wait_for_menu_input();
		if (key == '8') {
			selected = (selected + 2) % 3;
		} else if (key == '2') {
			selected = (selected + 1) % 3;
		} else if (key == 0x1B || key == 'q') {
			return 0;
		} else if (is_select_key(key)) {
			if (selected == 0)
				return 0;
			if (selected == 1) {
				run_settings_menu();
			} else {
				return 1;
			}
		} else {
			continue;
		}

		wait_for_input_release();
	}
}
