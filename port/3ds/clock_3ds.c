#include "clock_3ds.h"

uint64_t K3D_ClockTicks(void)
{
    return (uint64_t)svcGetSystemTick();
}

uint64_t K3D_ClockMilliseconds(void)
{
    return (uint64_t)osGetTime();
}
