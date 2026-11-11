#ifndef KATAM3DS_CLOCK_3DS_H
#define KATAM3DS_CLOCK_3DS_H
#include <3ds.h>
#include <stdint.h>

/* Monotonic host clock. Never use wall-clock time for simulation. */
uint64_t K3D_ClockTicks(void);
uint64_t K3D_ClockMicroseconds(void);
void K3D_FramePaceBegin(void);
void K3D_FramePaceEnd(void);
#endif
