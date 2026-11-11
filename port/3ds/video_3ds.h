#ifndef KATAM3DS_VIDEO_3DS_H
#define KATAM3DS_VIDEO_3DS_H
#include <3ds.h>
#include <stdint.h>
int K3D_VideoInit(void);
void K3D_VideoExit(void);
/* Source is 240x160, row-major RGBA8888 (0xAABBGGRR on ARM). */
void K3D_VideoPresent(const uint32_t *framebuffer);
#endif
