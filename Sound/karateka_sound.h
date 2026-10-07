/*
 * Karateka (IBM version) PC-speaker sound -> PCM renderer.
 *
 * The original game has no audio samples for most sounds: it flips the
 * speaker bit (port 61h, bit 1) from timed CPU loops.  This module replays
 * that logic, records *when* the speaker would have flipped, and converts
 * those flip times into ordinary PCM so it can be played through SDL (or
 * anything else).
 *
 * Sound numbers are the argument of sound(n) in the original code (1..26).
 */
#ifndef KARATEKA_SOUND_H
#define KARATEKA_SOUND_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KS_SOUND_COUNT 26

/* Render sound n (1..KS_SOUND_COUNT) to mono signed 16-bit PCM.
 * Returns a malloc()'d buffer (free() it) and stores the number of samples
 * in *num_samples.  Returns NULL if n is invalid or on allocation failure. */
int16_t *ks_render(int n, int sample_rate, size_t *num_samples);

/* Same gating as the original sound(): mode is the byte at D_D54C.
 *   0 = everything, 1 = sound effects only (no melodies), other = silent. */
int ks_sound_allowed(int n, int mode);

/* Short description, e.g. "hit (asm sound_02)"; NULL if n is invalid. */
const char *ks_sound_name(int n);

#ifdef __cplusplus
}
#endif
#endif
