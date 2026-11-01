#include "platform.h"
#include <3ds.h>
#include <citro2d.h>
static C2D_TextBuf text_buf;
bool K3D_Init(void){ gfxInitDefault(); C3D_Init(C3D_DEFAULT_CMDBUF_SIZE); C2D_Init(C2D_DEFAULT_MAX_OBJECTS); C2D_Prepare(); text_buf=C2D_TextBufNew(128); return text_buf!=NULL; }
void K3D_Shutdown(void){ if(text_buf) C2D_TextBufDelete(text_buf); C2D_Fini(); C3D_Fini(); gfxExit(); }
bool K3D_PollInput(K3D_Input *out){ if(!out) return false; hidScanInput(); out->held=hidKeysHeld(); out->down=hidKeysDown(); out->up=hidKeysUp(); return aptMainLoop(); }
void K3D_BeginFrame(void){ C3D_FrameBegin(C3D_FRAME_SYNCDRAW); C2D_TargetClear(C2D_GetScreenTarget(GFX_TOP),C2D_Color32(0,0,0,255)); C2D_SceneBegin(C2D_GetScreenTarget(GFX_TOP)); }
void K3D_EndFrame(void){ C3D_FrameEnd(0); }
void K3D_DrawRect(int x,int y,int w,int h,uint32_t rgba){ C2D_DrawRectSolid((float)x,(float)y,0,(float)w,(float)h,rgba); }
void K3D_DrawText(int x,int y,const char *s,uint32_t rgba){ C2D_Text t; C2D_TextParse(&t,text_buf,s); C2D_TextOptimize(&t); C2D_DrawText(&t,C2D_WithColor, (float)x,(float)y,0,0.5f,0.5f,rgba); }
