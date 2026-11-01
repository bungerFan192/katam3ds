# 3DS port foundation

This directory is the first platform-side layer for the Nintendo 3DS port. The original GBA build remains untouched.

## Current status

- `task_runtime.h/.c`: a small fixed-capacity task scheduler that does not use GBA IWRAM/EWRAM offsets.
- Not yet wired into the original game loop. The original `Task` ABI and memory layout are still used by the decompilation, so replacing them safely requires a later compatibility adapter.
- Renderer, room resource upload, and multiplayer transport are not implemented yet.

## Build target

Compile these files with devkitARM and link with libctru/Citro3D in a separate 3DS target. Do not add them to the existing GBA source wildcard; that target must continue to build the ROM.
