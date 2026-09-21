/*
	KARATEKA - SDL Port Bridge
	Copyright 2026 Retro Porting Project
*/
#ifndef DOS_SDL_BRIDGE_H
#define DOS_SDL_BRIDGE_H

#ifdef USE_SDL
#include <SDL/SDL.h>
#endif

/* Graphics Setup */
void init_sdl_graphics(void);
void close_sdl_graphics(void);

/* Backbuffer Presentation */
void BB_clear(void);
void BB_flip(void);
void BB_flip_part(void);
void BB_flip_wipe(void);

/* Timer Abstractions */
void tim_strt(void);
void tim_wait(void);
void C_1906(int ticks); /* BIOS timer tick loop replacement */
void delay_ms(int ms);

/* Input Abstractions */
int DoInput(int wait_for_key);
int GetKey(void);
void WaitKey(void);
void WaitNoKey(void);
int C_46CC(void); /* Joystick presence check: returns 0 (no joystick) */

/* Audio Abstraction */
void sound(int id);

/* Stub/Bypass definitions for DOS/BIOS hardware setup */
void C_430A(void); /* getDefaultDrive */
void C_42F4(void); /* selectDisk_0 */
void C_48E2(void); /* set int 24h */
void C_4718(void); /* speed related function */
int C_4300(void);  /* get current video mode */
int C_440E(void);  /* check graphics adapter */

#endif /* DOS_SDL_BRIDGE_H */
