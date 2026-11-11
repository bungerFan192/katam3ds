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

/* Stable FIFO flush: DMA order is observable when transfers overlap. */
static size_t Flush(K3D_DMA_Timing timing, size_t budget, int one_only)
{
    size_t i, kept = 0, copied = 0;
    K3D_DMA_Transfer pending[K3D_DMA_QUEUE_CAPACITY];

    for (i = 0; i < sCount; ++i) {
        K3D_DMA_Transfer item = sQueue[i];
        if (item.timing == timing && (!one_only || copied == 0)) {
            if (item.size <= budget - copied) {
                memmove(item.destination, item.source, item.size);
                copied += item.size;
                continue;
            }
            /* Do not overtake an earlier transfer of the same timing class. */
            if (item.timing == timing) {
                pending[kept++] = item;
                ++i;
                for (; i < sCount; ++i)
                    pending[kept++] = sQueue[i];
                break;
            }
        }
        pending[kept++] = item;
    }

    memcpy(sQueue, pending, kept * sizeof(pending[0]));
    sCount = kept;
    return copied;
}

size_t K3D_DMA_FlushVBlank(size_t byte_budget)
{
    return Flush(K3D_DMA_VBLANK, byte_budget, 0);
}

size_t K3D_DMA_FlushHBlank(void)
{
    return Flush(K3D_DMA_HBLANK, (size_t)-1, 1);
}

size_t K3D_DMA_Pending(void)
{
    return sCount;
}
