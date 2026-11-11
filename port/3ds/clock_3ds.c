#include "clock_3ds.h"

uint64_t K3D_ClockTicks(void)
{
    return (uint64_t)svcGetSystemTick();
}

uint64_t K3D_ClockMicroseconds(void)
{
    return (uint64_t)osGetTime();
}

void K3D_FramePaceBegin(void)
{
    /* The game owns simulation pacing; gfxSwapBuffers() / C3D_FrameEnd()
       provides the display synchronization point. */
}

void K3D_FramePaceEnd(void)
{
    /* Deliberately empty: avoid adding a second VBlank wait. */
}
