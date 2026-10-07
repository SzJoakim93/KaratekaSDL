#ifndef NATIVE_SPEAKER_H
#define NATIVE_SPEAKER_H

#include <stddef.h>
#include <stdint.h>

int native_speaker_open(void);
void native_speaker_play(const int16_t *pcm, size_t num_samples, int sample_rate);
void native_speaker_close(void);

#endif
