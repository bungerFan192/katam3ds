#include "platform.h"
#include "video_3ds.h"
#include "audio_3ds.h"
#include <3ds.h>
#include <citro2d.h>

static C2D_TextBuf text_buf;
static C3D_RenderTarget *top_target;
static bool audio_ready;

static uint16_t ToGbaKeys(uint32_t keys)
{
    uint16_t out = 0;
    if (keys & KEY_A)      out |= (1u << 0);
    if (keys & KEY_B)      out |= (1u << 1);
    if (keys & KEY_SELECT) out |= (1u << 2);
    if (keys & KEY_START)  out |= (1u << 3);
    if (keys & KEY_RIGHT)  out |= (1u << 4);
    if (keys & KEY_LEFT)   out |= (1u << 5);
    if (keys & KEY_UP)     out |= (1u << 6);
    if (keys & KEY_DOWN)   out |= (1u << 7);
    if (keys & KEY_R)      out |= (1u << 8);
    if (keys & KEY_L)      out |= (1u << 9);
    return out;
}

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

    if (top_target == NULL || text_buf == NULL || !K3D_VideoInit()) {
        if (text_buf != NULL)
            C2D_TextBufDelete(text_buf);
        if (top_target != NULL)
            C3D_RenderTargetDelete(top_target);
        C2D_Fini();
        C3D_Fini();
        gfxExit();
        text_buf = NULL;
        top_target = NULL;
        return false;
    }

    /* Sound is optional during early port bring-up. */
    audio_ready = K3D_AudioInit(32768);
    return true;
}

void K3D_Shutdown(void)
{
    if (audio_ready) {
        K3D_AudioExit();
        audio_ready = false;
    }
    K3D_VideoExit();

    if (text_buf != NULL) {
        C2D_TextBufDelete(text_buf);
        text_buf = NULL;
    }
    if (top_target != NULL) {
        C3D_RenderTargetDelete(top_target);
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
    out->gba_held = ToGbaKeys(out->held);
    out->gba_down = ToGbaKeys(out->down);
    out->gba_up = ToGbaKeys(out->up);
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

void K3D_PresentGbaFramebuffer(const uint32_t *pixels)
{
    K3D_VideoPresent(pixels);
}
