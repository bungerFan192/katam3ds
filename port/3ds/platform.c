#include "platform.h"
#include <3ds.h>
#include <citro2d.h>

static C2D_TextBuf text_buf;
static C3D_RenderTarget *top_target;

bool K3D_Init(void)
{
    gfxInitDefault();

    if (!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE)) {
        gfxExit();
        return false;
    }

    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    top_target = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    text_buf = C2D_TextBufNew(128);

    if (top_target == NULL || text_buf == NULL) {
        if (text_buf != NULL)
            C2D_TextBufDelete(text_buf);
        if (top_target != NULL)
            C2D_TargetDelete(top_target);
        C2D_Fini();
        C3D_Fini();
        gfxExit();
        text_buf = NULL;
        top_target = NULL;
        return false;
    }

    return true;
}

void K3D_Shutdown(void)
{
    if (text_buf != NULL) {
        C2D_TextBufDelete(text_buf);
        text_buf = NULL;
    }
    if (top_target != NULL) {
        C2D_TargetDelete(top_target);
        top_target = NULL;
    }

    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

bool K3D_PollInput(K3D_Input *out)
{
    if (out == NULL)
        return false;

    hidScanInput();
    out->held = hidKeysHeld();
    out->down = hidKeysDown();
    out->up = hidKeysUp();
    return aptMainLoop();
}

void K3D_BeginFrame(void)
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(text_buf);
    C2D_TargetClear(top_target, C2D_Color32(0, 0, 0, 255));
    C2D_SceneBegin(top_target);
}

void K3D_EndFrame(void)
{
    C3D_FrameEnd(0);
}

void K3D_DrawRect(int x, int y, int w, int h, uint32_t rgba)
{
    C2D_DrawRectSolid((float)x, (float)y, 0.0f,
                      (float)w, (float)h, rgba);
}

void K3D_DrawText(int x, int y, const char *s, uint32_t rgba)
{
    C2D_Text text;
    C2D_TextParse(&text, text_buf, s);
    C2D_TextOptimize(&text);
    C2D_DrawText(&text, C2D_WithColor,
                 (float)x, (float)y, 0.0f, 0.5f, 0.5f, rgba);
}
