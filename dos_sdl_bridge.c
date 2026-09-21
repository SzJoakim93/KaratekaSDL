/*
	KARATEKA - SDL Port Bridge
	Copyright 2026 Retro Porting Project
*/
#include <stdio.h>
#include <string.h>
#include "karateka.h"
#include "dos_sdl_bridge.h"

/* External reference to the CGA linear backbuffer */
extern char D_0337[16000];

#ifdef USE_SDL

static SDL_Surface* screen = NULL;
static SDL_Surface* texture = NULL;
static unsigned int pixels[640 * 400];

/* High intensity CGA Palette 1 colors */
static const unsigned int cga_palette[4] = {
	0xFF000000, /* Color 0: Black */
	0xFF55FFFF, /* Color 1: Cyan */
	0xFFFF55FF, /* Color 2: Magenta */
	0xFFFFFFFF  /* Color 3: White */
};

#endif

/* Initialize the SDL graphics subsystem, window and upscaled texture */
void init_sdl_graphics(void)
{
#ifdef USE_SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "SDL could not initialize: %s\n", SDL_GetError());
		return;
	}

	SDL_WM_SetCaption("Karateka - Windows Port", 0);

	screen = SDL_SetVideoMode(
		640,
		400,
		8,
		SDL_SWSURFACE | SDL_ANYFORMAT
	);

	if (!screen) {
		fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
		return;
	}

	texture = SDL_CreateRGBSurface(
		0,
		640,
		400,
		8,
		0,
		0,
		0,
		0
	);

	if (!texture) {
		fprintf(stderr, "Texture could not be created: %s\n", SDL_GetError());
		return;
	}

	memset(pixels, 0, sizeof(pixels));
	SDL_FillRect(screen, NULL, 0);
	SDL_Flip(screen);
#endif
}

/* Close SDL resources */
void close_sdl_graphics(void)
{
#ifdef USE_SDL
	if (texture) {
		SDL_FreeSurface(texture);
		texture = NULL;
	}
	if (screen) {
		SDL_FreeSurface(screen);
		screen = NULL;
	}
	SDL_Quit();
#endif
}

/* Clear CGA backbuffer to Color 0 */
void BB_clear(void)
{
	memset(D_0337, 0, 16000);
}

/* Present linear CGA backbuffer to 640x400 window with 2x integer scale */
void BB_flip(void)
{
#ifdef USE_SDL
	int x, y;
	if (!texture) return;

	for (y = 0; y < 200; y++) {
		for (x = 0; x < 320; x++) {
			int byte_idx = y * 80 + (x / 4);
			int pixel_shift = 6 - (2 * (x % 4));
			int color_idx = (D_0337[byte_idx] >> pixel_shift) & 0x03;
			unsigned int color = cga_palette[color_idx];

			/* Upscale 320x200 pixel to 2x2 grid in 640x400 display */
			int dest_x = x * 2;
			int dest_y = y * 2;

			pixels[dest_y * 640 + dest_x] = color;
			pixels[dest_y * 640 + (dest_x + 1)] = color;
			pixels[(dest_y + 1) * 640 + dest_x] = color;
			pixels[(dest_y + 1) * 640 + (dest_x + 1)] = color;
		}
	}

	SDL_BlitSurface(texture, NULL, screen, NULL);
	SDL_Flip(screen);
#endif
}

/* Partial and Wipe screen copies fallback to standard flip */
void BB_flip_part(void)
{
	BB_flip();
}

void BB_flip_wipe(void)
{
	BB_flip();
}

/* Timer ticks mapping using SDL_GetTicks */
static unsigned int timer_start_ticks = 0;

void tim_strt(void)
{
#ifdef USE_SDL
	timer_start_ticks = SDL_GetTicks();
#endif
}

void tim_wait(void)
{
#ifdef USE_SDL
	/* Wait for at least 3 DOS ticks (165 ms) */
	while (SDL_GetTicks() - timer_start_ticks < 165) {
		SDL_Delay(1);
	}
	timer_start_ticks = SDL_GetTicks();
#endif
}

void C_1906(int ticks)
{
#ifdef USE_SDL
	/* 1 DOS Tick = 55 ms */
	unsigned int target = SDL_GetTicks() + (ticks * 55);
	while (SDL_GetTicks() < target) {
		SDL_Delay(1);
	}
#endif
}

void delay_ms(int ms)
{
#ifdef USE_SDL
	SDL_Delay(ms);
#endif
}

/* Key mappings converter */
static int map_sdl_keycode(int sdl_key)
{
	switch (sdl_key) {
		case SDLK_SPACE:  return ' ';
		case SDLK_LEFT:   return '4';
		case SDLK_RIGHT:  return '6';
		case SDLK_UP:     return '8';
		case SDLK_DOWN:   return '2';
		case SDLK_q:      return 'q';
		case SDLK_a:      return 'a';
		case SDLK_z:      return 'z';
		case SDLK_w:      return 'w';
		case SDLK_s:      return 's';
		case SDLK_x:      return 'x';
		case SDLK_b:      return 'b';
		case SDLK_0:      return '0';
		case SDLK_ESCAPE: return 0x1B; /* ESC */
		default:          return 0;
	}
}

/* Poll events and convert SDL inputs into Karateka character inputs */
int DoInput(int wait_for_key)
{
#ifdef USE_SDL
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			close_sdl_graphics();
			exit(0);
		}
		else if (event.type == SDL_KEYDOWN) {
			int mapped = map_sdl_keycode(event.key.keysym.sym);
			if (mapped > 0) {
				D_DE68 = (char)mapped;
				D_DE69 = 1;
				return 0xFF; /* Key pending */
			}
		}
	}
#endif
	return 0;
}

int GetKey(void)
{
	int key = D_DE68;
	D_DE69 = 0;
	return key;
}

void WaitKey(void)
{
	while (!D_DE69) {
		DoInput(1);
		delay_ms(10);
	}
}

void WaitNoKey(void)
{
	D_DE69 = 0;
}

/* Bypassed Joystick presence check */
int C_46CC(void)
{
	return 0; /* Report no joystick connected to default to keyboard */
}

/* Audio toggle sound routine stub */
void sound(int id)
{
	/* Audio synthesis will reside here */
}

/* Stub/Bypass definitions for DOS/BIOS hardware setup */
void C_430A(void) {}
void C_42F4(void) {}
void C_48E2(void) {}
void C_4718(void) {}
int C_4300(void) { return 4; } /* Return CGA Mode 4 */
int C_440E(void) { return 1; } /* Return true (CGA graphics adapter present) */
