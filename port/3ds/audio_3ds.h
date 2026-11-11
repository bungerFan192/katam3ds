#ifndef KATAM3DS_AUDIO_3DS_H
#define KATAM3DS_AUDIO_3DS_H
#include <3ds.h>
#include <stddef.h>
#include <stdint.h>
int K3D_AudioInit(unsigned sample_rate);
void K3D_AudioExit(void);
/* Queue interleaved signed 16-bit stereo PCM. Returns frames queued, or 0. */
size_t K3D_AudioQueue(const int16_t *samples, size_t frames);
#endif
