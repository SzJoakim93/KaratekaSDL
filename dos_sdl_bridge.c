/*
	KARATEKA - SDL Port Bridge
	Copyright 2026 Retro Porting Project
*/
#include <stdio.h>
#include <string.h>
#include "karateka.h"
#include "dos_sdl_bridge.h"

/* External reference to the CGA linear backbuffer */
extern char cga_buffer[16000];

#ifdef USE_SDL

static SDL_Surface* screen = NULL;
static int audio_open = 0;
static int audio_sample_rate = 22050;
static int speaker_start_frequency = 0;
static int speaker_end_frequency = 0;
static int speaker_samples_remaining = 0;
static int speaker_total_samples = 0;
static double speaker_phase = 0.0;

typedef struct {
	unsigned short start_frequency;
	unsigned short end_frequency;
	unsigned short duration_ms;
} SpeakerTone;

/* The original hit effects use changing speaker-toggle rates rather than fixed notes. */
static const SpeakerTone speaker_tones[0x1a] = {
	{ 0, 0, 0 },
	{ 7800, 270, 52 }, { 7800, 435, 23 },
	{ 3140, 3140, 3 }, { 1570, 1570, 10 },
	{ 980, 7800, 2 }, { 980, 980, 3 },
	{ 587, 587, 120 }, { 784, 784, 120 },
	{ 698, 698, 100 }, { 392, 392, 130 },
	{ 988, 988, 90 }, { 523, 523, 120 },
	{ 622, 622, 100 }, { 831, 831, 120 },
	{ 554, 554, 100 }, { 740, 740, 90 },
	{ 659, 659, 120 }, { 494, 494, 100 },
	{ 932, 932, 100 }, { 587, 587, 90 },
	{ 784, 784, 100 }, { 466, 466, 120 },
	{ 659, 659, 90 }, { 880, 880, 120 },
	{ 349, 349, 150 }
};

static void speaker_audio_callback(void *userdata, Uint8 *stream, int length)
{
	Sint16 *samples = (Sint16*)stream;
	int sample_count = length / (int)sizeof(*samples);
	int sample_index;
	int ramp_samples = audio_sample_rate / 200;
	double phase_step;
	double frequency;

	(void)userdata;
	memset(stream, 0, length);
	if (speaker_start_frequency <= 0 || speaker_end_frequency <= 0 || speaker_samples_remaining <= 0)
		return;
	if (ramp_samples > speaker_total_samples / 4)
		ramp_samples = speaker_total_samples / 4;
	if (ramp_samples < 1)
		ramp_samples = 1;

	for (sample_index = 0; sample_index < sample_count && speaker_samples_remaining > 0; sample_index++) {
		int elapsed = speaker_total_samples - speaker_samples_remaining;
		int edge_samples = elapsed;
		int amplitude = 5000;
		double progress = (double)elapsed / speaker_total_samples;

		if (speaker_samples_remaining < edge_samples)
			edge_samples = speaker_samples_remaining;
		if (edge_samples < ramp_samples && ramp_samples > 0)
			amplitude = amplitude * edge_samples / ramp_samples;

		frequency = speaker_start_frequency +
			(speaker_end_frequency - speaker_start_frequency) * progress;
		phase_step = 6.283185307179586 * frequency / audio_sample_rate;
		samples[sample_index] = speaker_phase < 3.141592653589793 ? (Sint16)amplitude : (Sint16)-amplitude;
		speaker_phase += phase_step;
		if (speaker_phase >= 6.283185307179586)
			speaker_phase -= 6.283185307179586;
		speaker_samples_remaining--;
	}
}

/* High intensity CGA Palette 1 colors */
static const unsigned int cga_palette[4] = {
	0xFF000000, /* Color 0: Black */
	0xFF55FFFF, /* Color 1: Cyan */
	0xFFFF55FF, /* Color 2: Magenta */
	0xFFFFFFFF  /* Color 3: White */
};

#endif

/* Initialize the SDL graphics subsystem and window*/
void init_sdl_graphics(void)
{
#ifdef USE_SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "SDL could not initialize: %s\n", SDL_GetError());
		return;
	}

	{
		SDL_AudioSpec desired_audio;
		memset(&desired_audio, 0, sizeof(desired_audio));
		desired_audio.freq = audio_sample_rate;
		desired_audio.format = AUDIO_S16SYS;
		desired_audio.channels = 1;
		desired_audio.samples = 512;
		desired_audio.callback = speaker_audio_callback;
		if (SDL_OpenAudio(&desired_audio, NULL) == 0) {
			audio_open = 1;
			SDL_PauseAudio(0);
		} else {
			fprintf(stderr, "SDL audio could not be opened: %s\n", SDL_GetError());
		}
	}

	SDL_WM_SetCaption("Karateka - Windows Port", 0);

	screen = SDL_SetVideoMode(
		640,
		400,
		32,
		SDL_SWSURFACE | SDL_ANYFORMAT
	);

	if (!screen) {
		fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
		return;
	}

	SDL_FillRect(screen, NULL, 0);
	SDL_Flip(screen);
#endif
}

/* Close SDL resources */
void close_sdl_graphics(void)
{
#ifdef USE_SDL
	if (audio_open) {
		SDL_CloseAudio();
		audio_open = 0;
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
	memset(cga_buffer, 0, 16000);
}

/* Present linear CGA backbuffer to 640x400 window with 2x integer scale */
void BB_flip(void)
{
#ifdef USE_SDL
	int x, y;
	if (!screen) return;
	if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);

	Uint32 *pixels = (Uint32*)screen->pixels;

	for (y = 0; y < 200; y++) {
		for (x = 0; x < 320; x++) {
			int byte_idx = y * 80 + (x / 4);
			int pixel_shift = 6 - (2 * (x % 4));
			int color_idx = (cga_buffer[byte_idx] >> pixel_shift) & 0x03;
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

	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

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
static unsigned int previous_script_frame_ticks = 0;

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

void script_frame_pace_reset(void)
{
#ifdef USE_SDL
	previous_script_frame_ticks = SDL_GetTicks();
#endif
}

void script_frame_pace(unsigned int frame_duration_ms)
{
#ifdef USE_SDL
	unsigned int now = SDL_GetTicks();
	unsigned int elapsed;

	elapsed = now - previous_script_frame_ticks;
	if (elapsed < frame_duration_ms) {
		SDL_Delay(frame_duration_ms - elapsed);
	}
	previous_script_frame_ticks = SDL_GetTicks();
#else
	(void)frame_duration_ms;
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
static unsigned char map_sdl_keycode(int sdl_key)
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
			unsigned char mappedKey = map_sdl_keycode(event.key.keysym.sym);
			if (mappedKey > 0) {
				pressedKey = mappedKey;
				isKeyPending = 1;
				return 1;
			}
		}
	}
#endif
	return isKeyPending ? 1 : 0;
}

unsigned char GetKey(void)
{
	isKeyPending = 0;
	return pressedKey;
}

void WaitKey(void)
{
	while (!isKeyPending) {
		DoInput(1);
		delay_ms(10);
	}
}

void WaitNoKey(void)
{
	isKeyPending = 0;
}

/* Bypassed Joystick presence check */
int C_46CC(void)
{
	return 0; /* Report no joystick connected to default to keyboard */
}

/* Play the sound ID as a short PC-speaker-style square wave. */
void sound(int id)
{
#ifdef USE_SDL
	if (!audio_open || id <= 0 || id >= (int)(sizeof(speaker_tones) / sizeof(speaker_tones[0])))
		return;

	SDL_LockAudio();
	speaker_start_frequency = speaker_tones[id].start_frequency;
	speaker_end_frequency = speaker_tones[id].end_frequency;
	speaker_total_samples = audio_sample_rate * speaker_tones[id].duration_ms / 1000;
	speaker_samples_remaining = speaker_total_samples;
	speaker_phase = 0.0;
	SDL_UnlockAudio();
#else
	(void)id;
#endif
}

void Beep(void)
{
	sound(5);
}

/* Stub/Bypass definitions for DOS/BIOS hardware setup */
void C_430A(void) {}
void C_42F4(void) {}
void C_48E2(void) {}
void C_4718(void) {}
int C_4300(void) { return 4; } /* Return CGA Mode 4 */
int C_440E(void) { return 1; } /* Return true (CGA graphics adapter present) */
