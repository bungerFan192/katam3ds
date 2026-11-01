#include "platform.h"
#include "task_runtime.h"
int main(void){
 KatamTaskRuntime tasks; K3D_Input input={0};
 if(!K3D_Init()) return 1;
 KatamTaskRuntime_Init(&tasks);
 while(K3D_PollInput(&input)){
   K3D_BeginFrame();
   K3D_DrawRect(20,20,280,40,0x202060FF);
   K3D_DrawText(28,28,"KATAM 3DS PORT - renderer smoke test",0xFFFFFFFF);
   K3D_DrawText(28,70,"Press START to exit",0xFFFFFFFF);
   K3D_EndFrame();
   if(input.down & KEY_START) break;
   KatamTaskRuntime_Update(&tasks);
 }
 K3D_Shutdown(); return 0;
}
