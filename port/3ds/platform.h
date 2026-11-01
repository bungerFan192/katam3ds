#ifndef KATAM3DS_PLATFORM_H
#define KATAM3DS_PLATFORM_H
#include <3ds.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct { uint32_t held, down, up; } K3D_Input;
bool K3D_Init(void);
void K3D_Shutdown(void);
bool K3D_PollInput(K3D_Input *out);
void K3D_BeginFrame(void);
void K3D_EndFrame(void);
void K3D_DrawRect(int x,int y,int w,int h,uint32_t rgba);
void K3D_DrawText(int x,int y,const char *text,uint32_t rgba);
#endif
