/*
	KARATEKA - SDL Port Bridge
	Copyright 2026 SzJoakim93
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "karateka.h"
#include "dos_sdl_bridge.h"

#ifdef USE_SDL
#include "Sound/karateka_sound.h"
#include "native_speaker.h"
#endif

/* External reference to the CGA linear backbuffer */
extern char cga_buffer[16000];

#ifdef USE_SDL
static SDL_Surface* screen = NULL;
static int windowed_width = 640;
static int windowed_height = 400;
static int fullscreen_mode = 0;
static int audio_open = 0;
static int native_speaker_opened = 0;
static int audio_sample_rate = 22050;
static int16_t *sound_pcm[KS_SOUND_COUNT + 1];
static size_t sound_pcm_lengths[KS_SOUND_COUNT + 1];
static int playing_sound = 0;
static size_t playback_position = 0;
static SDL_Joystick *joystick = NULL;

static void free_sound_bank(void)
{
	int n;

	for (n = 1; n <= KS_SOUND_COUNT; n++) {
		free(sound_pcm[n]);
		sound_pcm[n] = NULL;
		sound_pcm_lengths[n] = 0;
	}
}

static int render_sound_bank(void)
{
	int n;

	free_sound_bank();
	for (n = 1; n <= KS_SOUND_COUNT; n++) {
		sound_pcm[n] = ks_render(n, audio_sample_rate, &sound_pcm_lengths[n]);
		if (!sound_pcm[n]) {
			fprintf(stderr, "Could not render sound %d (%s)\n", n, ks_sound_name(n));
			free_sound_bank();
			return 0;
		}
	}
	return 1;
}

static void speaker_audio_callback(void *userdata, Uint8 *stream, int length);

static int open_sdl_audio(void)
{
	SDL_AudioSpec desired_audio;
	SDL_AudioSpec obtained_audio;

	memset(&desired_audio, 0, sizeof(desired_audio));
	desired_audio.freq = audio_sample_rate;
	desired_audio.format = AUDIO_S16SYS;
	desired_audio.channels = 1;
	desired_audio.samples = 512;
	desired_audio.callback = speaker_audio_callback;
	if (SDL_OpenAudio(&desired_audio, &obtained_audio) != 0) {
		fprintf(stderr, "SDL audio could not be opened: %s\n", SDL_GetError());
		return 0;
	}
	if (obtained_audio.freq <= 0 || obtained_audio.format != AUDIO_S16SYS ||
		obtained_audio.channels != 1) {
		fprintf(stderr, "SDL audio returned an unsupported format\n");
		SDL_CloseAudio();
		return 0;
	}

	audio_sample_rate = obtained_audio.freq;
	if (!render_sound_bank()) {
		SDL_CloseAudio();
		return 0;
	}
	audio_open = 1;
	SDL_PauseAudio(0);
	return 1;
}

static void speaker_audio_callback(void *userdata, Uint8 *stream, int length)
{
	Sint16 *samples = (Sint16*)stream;
	size_t sample_count = (size_t)length / sizeof(*samples);
	size_t available;
	size_t copy_count;

	(void)userdata;
	memset(stream, 0, length);
	if (playing_sound <= 0)
		return;
	available = sound_pcm_lengths[playing_sound] - playback_position;
	copy_count = sample_count < available ? sample_count : available;
	memcpy(samples, sound_pcm[playing_sound] + playback_position, copy_count * sizeof(*samples));
	playback_position += copy_count;
	if (playback_position == sound_pcm_lengths[playing_sound]) {
		playing_sound = 0;
		playback_position = 0;
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
	read_settings(); /* Load settings from settings.ini */

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
		int native_audio_open = 0;
		if (settings.use_native_pc_speaker) {
			if (render_sound_bank()) {
				native_audio_open = native_speaker_open();
				native_speaker_opened = native_audio_open;
				if (!native_audio_open)
					free_sound_bank();
			}
		}
		if (!native_audio_open)
			open_sdl_audio();
	}

	SDL_WM_SetCaption("Karateka SDL", 0);
	SDL_ShowCursor(SDL_DISABLE);

	if (!set_video_mode(settings.resWidth, settings.resHeight, settings.fullscreen)) {
		fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
		return;
	}

	SDL_FillRect(screen, NULL, 0);
	SDL_Flip(screen);
#endif
}

/* Apply the values changed in the in-game settings menu. */
void apply_settings(void)
{
#ifdef USE_SDL
	if (!screen)
		return;

	if (!settings.fullscreen) {
		windowed_width = settings.resWidth;
		windowed_height = settings.resHeight;
	}
	if (screen->w != settings.resWidth || screen->h != settings.resHeight ||
		fullscreen_mode != settings.fullscreen) {
		set_video_mode(settings.resWidth, settings.resHeight, settings.fullscreen);
	}

	if (settings.use_native_pc_speaker) {
		if (!native_speaker_opened) {
			if (!sound_pcm[1] && !render_sound_bank())
				return;
			native_speaker_opened = native_speaker_open();
			if (native_speaker_opened && audio_open) {
				SDL_CloseAudio();
				audio_open = 0;
			}
		}
	} else {
		if (native_speaker_opened) {
			native_speaker_close();
			native_speaker_opened = 0;
		}
		if (!audio_open)
			open_sdl_audio();
	}
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
	if (native_speaker_opened) {
		native_speaker_close();
		native_speaker_opened = 0;
	}
	free_sound_bank();
	playing_sound = 0;
	playback_position = 0;
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
		case SDLK_RETURN: return '\r';
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

/* Play the pre-rendered PCM for a Karateka sound ID. */
void sound(int id)
{
#ifdef USE_SDL
	if (id < 1 || id > KS_SOUND_COUNT)
		return;

	if (settings.use_native_pc_speaker && native_speaker_opened) {
		native_speaker_play(sound_pcm[id], sound_pcm_lengths[id], audio_sample_rate);
		return;
	}
	if (!audio_open)
		return;

	SDL_LockAudio();
	playing_sound = id;
	playback_position = 0;
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
