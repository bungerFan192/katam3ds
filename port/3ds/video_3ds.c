#include "video_3ds.h"
#include <citro2d.h>
#include <string.h>

#define GBA_W 240
#define GBA_H 160
#define TEX_W 256
#define TEX_H 256

static C3D_Tex sFrameTexture;
static Tex3DS_SubTexture sFrameSubtex = {
    GBA_W, GBA_H, 0.0f, 0.0f,
    (float)GBA_W / TEX_W, (float)GBA_H / TEX_H
};
static int sVideoReady;
static uint8_t sTiled[TEX_W * TEX_H * 4] __attribute__((aligned(0x80)));
static C2D_Image sFrameImage;

static void PutPixel(unsigned x, unsigned y, uint32_t rgba)
{
    unsigned tile = (y >> 3) * (TEX_W >> 3) + (x >> 3);
    unsigned in_tile = (y & 7) * 8 + (x & 7);
    unsigned offset = (tile * 64 + in_tile) * 4;
    /* GBA-port framebuffer pixels are 0xFFBBGGRR in little-endian memory. */
    sTiled[offset + 0] = (uint8_t)(rgba & 0xFF);
    sTiled[offset + 1] = (uint8_t)((rgba >> 8) & 0xFF);
    sTiled[offset + 2] = (uint8_t)((rgba >> 16) & 0xFF);
    sTiled[offset + 3] = (uint8_t)((rgba >> 24) & 0xFF);
}

int K3D_VideoInit(void)
{
    if (!C3D_TexInit(&sFrameTexture, TEX_W, TEX_H, GPU_RGBA8))
        return 0;
    C3D_TexSetFilter(&sFrameTexture, GPU_NEAREST, GPU_NEAREST);
    sFrameImage.tex = &sFrameTexture;
    sFrameImage.subtex = &sFrameSubtex;
    memset(sTiled, 0, sizeof(sTiled));
    sVideoReady = 1;
    return 1;
}

void K3D_VideoExit(void)
{
    if (sVideoReady) {
        C3D_TexDelete(&sFrameTexture);
        sVideoReady = 0;
    }
}

void K3D_VideoPresent(const uint32_t *framebuffer)
{
    unsigned x, y;
    if (!sVideoReady || !framebuffer)
        return;

    memset(sTiled, 0, sizeof(sTiled));
    for (y = 0; y < GBA_H; ++y)
        for (x = 0; x < GBA_W; ++x)
            PutPixel(x, y, framebuffer[y * GBA_W + x]);

    C3D_TexUpload(&sFrameTexture, sTiled);
    C3D_TexFlush(&sFrameTexture);
    C2D_DrawImageAt(sFrameImage, 0.0f, 0.0f, 0.0f, NULL, 1.0f, 1.0f);
}
