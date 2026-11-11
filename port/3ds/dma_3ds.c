#include "dma_3ds.h"
#include <string.h>

static K3D_DMA_Transfer sQueue[K3D_DMA_QUEUE_CAPACITY];
static size_t sCount;

void K3D_DMA_Init(void)
{
    sCount = 0;
    memset(sQueue, 0, sizeof(sQueue));
}

int K3D_DMA_Copy(const void *source, void *destination, size_t size,
                 K3D_DMA_Timing timing)
{
    K3D_DMA_Transfer *item;
    if (size == 0)
        return 1;
    if (!source || !destination || timing > K3D_DMA_HBLANK)
        return 0;
    if (timing == K3D_DMA_IMMEDIATE) {
        memmove(destination, source, size);
        return 1;
    }
    if (sCount >= K3D_DMA_QUEUE_CAPACITY)
        return 0;
    item = &sQueue[sCount++];
    item->source = source;
    item->destination = destination;
    item->size = size;
    item->timing = timing;
    return 1;
}

static size_t Flush(K3D_DMA_Timing timing, size_t budget)
{
    size_t i = 0, copied = 0;
    while (i < sCount) {
        K3D_DMA_Transfer *item = &sQueue[i];
        if (item->timing == timing && item->size <= budget - copied) {
            memmove(item->destination, item->source, item->size);
            copied += item->size;
            sQueue[i] = sQueue[--sCount];
            continue;
        }
        ++i;
    }
    return copied;
}

size_t K3D_DMA_FlushVBlank(size_t byte_budget)
{
    return Flush(K3D_DMA_VBLANK, byte_budget);
}

size_t K3D_DMA_FlushHBlank(void)
{
    return Flush(K3D_DMA_HBLANK, (size_t)-1);
}

size_t K3D_DMA_Pending(void)
{
    return sCount;
}
