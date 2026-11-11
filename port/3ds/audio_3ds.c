#include "audio_3ds.h"
#include <string.h>

#define K3D_AUDIO_CHANNEL 0
#define K3D_AUDIO_BUFFERS 3
#define K3D_AUDIO_MAX_FRAMES 2048

typedef struct {
    ndspWaveBuf wave;
    int16_t *samples;
} K3DAudioBuffer;

static K3DAudioBuffer sBuffers[K3D_AUDIO_BUFFERS];
static int sAudioReady;
static int sNdspStarted;

int K3D_AudioInit(unsigned sample_rate)
{
    int i;
    if (sAudioReady)
        return 1;
    if (ndspInit() != 0)
        return 0;
    sNdspStarted = 1;

    ndspChnReset(K3D_AUDIO_CHANNEL);
    ndspChnSetInterp(K3D_AUDIO_CHANNEL, NDSP_INTERP_LINEAR);
    ndspChnSetRate(K3D_AUDIO_CHANNEL, (float)sample_rate);
    ndspChnSetFormat(K3D_AUDIO_CHANNEL, NDSP_FORMAT_STEREO_PCM16);

    memset(sBuffers, 0, sizeof(sBuffers));
    for (i = 0; i < K3D_AUDIO_BUFFERS; ++i) {
        sBuffers[i].samples = (int16_t *)linearAlloc(
            K3D_AUDIO_MAX_FRAMES * 2 * sizeof(int16_t));
        if (!sBuffers[i].samples) {
            K3D_AudioExit();
            return 0;
        }
        sBuffers[i].wave.data_vaddr = sBuffers[i].samples;
        sBuffers[i].wave.nsamples = 0;
        sBuffers[i].wave.looping = false;
    }
    sAudioReady = 1;
    return 1;
}

void K3D_AudioExit(void)
{
    int i;
    if (sAudioReady)
        ndspChnReset(K3D_AUDIO_CHANNEL);
    for (i = 0; i < K3D_AUDIO_BUFFERS; ++i) {
        if (sBuffers[i].samples)
            linearFree(sBuffers[i].samples);
        memset(&sBuffers[i], 0, sizeof(sBuffers[i]));
    }
    if (sNdspStarted)
        ndspExit();
    sNdspStarted = 0;
    sAudioReady = 0;
}

size_t K3D_AudioQueue(const int16_t *samples, size_t frames)
{
    int i;
    size_t bytes;
    if (!sAudioReady || !samples || frames == 0 ||
        frames > K3D_AUDIO_MAX_FRAMES)
        return 0;

    for (i = 0; i < K3D_AUDIO_BUFFERS; ++i) {
        ndspWaveBuf *wave = &sBuffers[i].wave;
        if (wave->status == NDSP_WBUF_FREE || wave->status == NDSP_WBUF_DONE) {
            bytes = frames * 2 * sizeof(int16_t);
            memcpy(sBuffers[i].samples, samples, bytes);
            DSP_FlushDataCache(sBuffers[i].samples, bytes);
            wave->nsamples = (u32)frames;
            wave->looping = false;
            ndspChnWaveBufAdd(K3D_AUDIO_CHANNEL, wave);
            return frames;
        }
    }
    return 0;
}
