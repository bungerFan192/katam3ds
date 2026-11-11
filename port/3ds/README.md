# katam3ds — Nintendo 3DS platform layer

The 3DS work lives in this directory and is kept separate from the original
GBA ROM build. This branch is based on `3ds-port-foundation`.

## Implemented adapters in this branch

- `platform.c/.h`: libctru input polling, active-high GBA button-mask mapping,
  Citro2D frame setup and a framebuffer presentation entry point.
- `video_3ds.c/.h`: uploads a 240x160 RGBA framebuffer to a tiled Citro3D
  texture and draws it on the top screen.
- `audio_3ds.c/.h`: NDSP stereo signed-16-bit PCM queue with reusable linear
  buffers. The game's M4A mixer is not connected yet.
- `dma_3ds.c/.h`: immediate copies and queued VBlank/HBlank transfers with a
  VBlank byte budget. GBA DMA register emulation is not connected yet.
- `save_3ds.c/.h`: SDMC file read/write adapter for save data.
- `clock_3ds.c/.h`: system tick and millisecond clock.
- `wireless_3ds.c/.h`: four-player packet API. The UDS backend deliberately
  reports unavailable until discovery, host/join and packet queues are real.
- `task_runtime.c/.h`: fixed-capacity scheduler, still separate from the
  original GBA `Task` ABI.

## Important: this is not yet a playable port

The standalone target does not compile the original game sources, render a
Kirby room, load the original assets, or provide local wireless multiplayer.
The 3DS framebuffer presenter is not the GBA PPU; the DMA scheduler is not yet
wired to the GBA DMA registers; and the NDSP queue is not yet fed by the M4A
mixer.

The largest blocker is the GBA memory map. The reference port reserves memory
at the original GBA virtual addresses so existing pointer arithmetic keeps
working. We have not established that this is safe or possible in the 3DS
process. Do not add original game sources until that strategy is solved.

See [UPSTREAM_PORT_PLAN.md](UPSTREAM_PORT_PLAN.md) for the system-by-system
analysis and next integration steps.

## Build

```sh
make -C port/3ds
```

The CI target is a platform-adapter smoke test, not a game build.
