#ifndef KATAM3DS_DMA_3DS_H
#define KATAM3DS_DMA_3DS_H
#include <stddef.h>
#include <stdint.h>
#define K3D_DMA_QUEUE_CAPACITY 64
typedef enum {
    K3D_DMA_IMMEDIATE = 0,
    K3D_DMA_VBLANK = 1,
    K3D_DMA_HBLANK = 2
} K3D_DMA_Timing;
typedef struct {
    const void *source;
    void *destination;
    size_t size;
    K3D_DMA_Timing timing;
} K3D_DMA_Transfer;
void K3D_DMA_Init(void);
int K3D_DMA_Copy(const void *source, void *destination, size_t size,
                 K3D_DMA_Timing timing);
size_t K3D_DMA_FlushVBlank(size_t byte_budget);
size_t K3D_DMA_FlushHBlank(void);
size_t K3D_DMA_Pending(void);
#endif
