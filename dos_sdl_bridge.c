/*
	KARATEKA - SDL Port Bridge
	Copyright 2026 SzJoakim93
*/
#include <stdio.h>
#include <string.h>
#include "karateka.h"
#include "dos_sdl_bridge.h"

/* External reference to the CGA linear backbuffer */
extern char cga_buffer[16000];

#ifdef USE_SDL

static SDL_Surface* screen = NULL;
static int windowed_width = 640;
static int windowed_height = 400;
static int fullscreen_mode = 0;
static int audio_open = 0;
static int audio_sample_rate = 22050;
static int speaker_start_frequency = 0;
static int speaker_end_frequency = 0;
static int speaker_samples_remaining = 0;
static int speaker_total_samples = 0;
static double speaker_phase = 0.0;
static SDL_Joystick *joystick = NULL;

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

static int set_video_mode(int width, int height, int fullscreen)
{
	Uint32 flags = SDL_SWSURFACE | SDL_ANYFORMAT;
	SDL_Surface *new_screen;

	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	} else {
		flags |= SDL_RESIZABLE;
	}

	new_screen = SDL_SetVideoMode(width, height, 32, flags);
	if (!new_screen) {
		fprintf(stderr, "Video mode could not be changed: %s\n", SDL_GetError());
		return 0;
	}

	screen = new_screen;
	fullscreen_mode = fullscreen;
	return 1;
}

static void toggle_fullscreen(void)
{
	if (fullscreen_mode) {
		set_video_mode(windowed_width, windowed_height, 0);
	} else {
		const SDL_VideoInfo *video_info = SDL_GetVideoInfo();

		if (!video_info || video_info->current_w <= 0 || video_info->current_h <= 0) {
			fprintf(stderr, "Desktop resolution could not be determined: %s\n", SDL_GetError());
			return;
		}
		set_video_mode(video_info->current_w, video_info->current_h, 1);
	}
}

#endif

/* Initialize the SDL graphics subsystem and window*/
void init_sdl_graphics(void)
{
#ifdef USE_SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0) {
		fprintf(stderr, "SDL could not initialize: %s\n", SDL_GetError());
		return;
	}

	SDL_JoystickEventState(SDL_ENABLE);
	if (SDL_NumJoysticks() > 0) {
		joystick = SDL_JoystickOpen(0);
		if (!joystick)
			fprintf(stderr, "SDL joystick could not be opened: %s\n", SDL_GetError());
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

	SDL_WM_SetCaption("Karateka SDL", 0);

	if (!set_video_mode(windowed_width, windowed_height, 0)) {
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
	if (joystick) {
		SDL_JoystickClose(joystick);
		joystick = NULL;
	}
	screen = NULL;
	SDL_Quit();
#endif
}

/* Clear CGA backbuffer to Color 0 */
void BB_clear(void)
{
	memset(cga_buffer, 0, 16000);
}

/* Present the CGA backbuffer scaled to fit the current video surface. */
void BB_flip(void)
{
#ifdef USE_SDL
	int x, y, scaled_width, scaled_height, offset_x, offset_y;
	int surface_stride;
	Uint32 mapped_palette[4];
	unsigned int color_index;
	if (!screen) return;

	if (screen->w * 200 >= screen->h * 320) {
		scaled_height = screen->h;
		scaled_width = scaled_height * 320 / 200;
	} else {
		scaled_width = screen->w;
		scaled_height = scaled_width * 200 / 320;
	}
	if (scaled_width < 1 || scaled_height < 1)
		return;

	offset_x = (screen->w - scaled_width) / 2;
	offset_y = (screen->h - scaled_height) / 2;
	surface_stride = screen->pitch / (int)sizeof(Uint32);
	for (color_index = 0; color_index < 4; color_index++) {
		mapped_palette[color_index] = SDL_MapRGB(screen->format,
			(Uint8)(cga_palette[color_index] >> 16),
			(Uint8)(cga_palette[color_index] >> 8),
			(Uint8)cga_palette[color_index]);
	}

	SDL_FillRect(screen, NULL, mapped_palette[0]);
	if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);

	Uint32 *pixels = (Uint32*)screen->pixels;

	for (y = 0; y < scaled_height; y++) {
		int source_y = y * 200 / scaled_height;
		for (x = 0; x < scaled_width; x++) {
			int source_x = x * 320 / scaled_width;
			int byte_idx = source_y * 80 + (source_x / 4);
			int pixel_shift = 6 - (2 * (source_x % 4));
			unsigned char source_byte = (unsigned char)cga_buffer[byte_idx];
			int color_idx = (source_byte >> pixel_shift) & 0x03;

			pixels[(offset_y + y) * surface_stride + offset_x + x] =
				mapped_palette[color_idx];
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

#ifdef USE_SDL
static void delay_with_input(unsigned int duration_ms)
{
	unsigned int start_ticks = SDL_GetTicks();

	while (SDL_GetTicks() - start_ticks < duration_ms) {
		DoInput(0);
		SDL_Delay(1);
	}
}
#endif

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
		DoInput(0);
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
		delay_with_input(frame_duration_ms - elapsed);
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
	if (ticks > 0)
		delay_with_input((unsigned int)ticks * 55);
#endif
}

void delay_ms(int ms)
{
#ifdef USE_SDL
	if (ms > 0)
		delay_with_input((unsigned int)ms);
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
		case SDLK_y:      return 'y';
		case SDLK_n:      return 'n';
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

static unsigned char map_sdl_joystick_button(int button)
{
	static const unsigned char button_keys[] = {
		' ', 'a', 'z', 'x', 'w', 's', 'q', 'b', '0'
	};

	if (button < 0 || button >= (int)(sizeof(button_keys) / sizeof(button_keys[0])))
		return 0;
	return button_keys[button];
}

static unsigned char map_sdl_joystick_direction(void)
{
	int hat;
	int axis;
	Sint16 value;

	if (!joystick)
		return 0;

	SDL_JoystickUpdate();
	for (hat = 0; hat < SDL_JoystickNumHats(joystick); hat++) {
		Uint8 position = SDL_JoystickGetHat(joystick, hat);
		if (position & SDL_HAT_LEFT) return '4';
		if (position & SDL_HAT_RIGHT) return '6';
		if (position & SDL_HAT_UP) return '8';
		if (position & SDL_HAT_DOWN) return '2';
	}

	for (axis = 0; axis < SDL_JoystickNumAxes(joystick) && axis < 2; axis++) {
		value = SDL_JoystickGetAxis(joystick, axis);
		if (value < -8000) return axis == 0 ? '4' : '8';
		if (value > 8000) return axis == 0 ? '6' : '2';
	}

	return 0;
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
			if (event.key.keysym.sym == SDLK_RETURN &&
				(event.key.keysym.mod & KMOD_ALT)) {
				toggle_fullscreen();
				continue;
			}
			unsigned char mappedKey = map_sdl_keycode(event.key.keysym.sym);
			if (mappedKey > 0) {
				pressedKey = mappedKey;
				isKeyPending = 1;
				return 1;
			}
		}
		else if (event.type == SDL_JOYBUTTONDOWN && joystick &&
			event.jbutton.which == SDL_JoystickIndex(joystick)) {
			unsigned char mappedKey = map_sdl_joystick_button(event.jbutton.button);
			if (mappedKey > 0) {
				pressedKey = mappedKey;
				isKeyPending = 1;
				return 1;
			}
		}
		else if (event.type == SDL_VIDEORESIZE && !fullscreen_mode) {
			if (set_video_mode(event.resize.w, event.resize.h, 0)) {
				windowed_width = event.resize.w;
				windowed_height = event.resize.h;
			}
		}
	}

	{
		unsigned char mappedKey = map_sdl_joystick_direction();
		if (mappedKey > 0) {
			pressedKey = mappedKey;
			isKeyPending = 1;
			return 1;
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

/* Report whether an SDL joystick is connected and open. */
int C_46CC(void)
{
#ifdef USE_SDL
	return joystick != NULL;
#else
	return 0;
#endif
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
