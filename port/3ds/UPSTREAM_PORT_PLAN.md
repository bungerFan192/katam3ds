# katam-port → Nintendo 3DS: system port notes

Reference project: https://github.com/sh1ftmaker/katam-port

This branch contains **new 3DS-side adapters**, not copied upstream implementation
code. The reference repository's platform files are being used to understand
the contracts. Before importing any upstream implementation, its license and
attribution requirements must be verified.

## System-by-system map

| Reference system | 3DS target | Current state |
|---|---|---|
| `platform/ppu.c` | `video_3ds.c` + a future GBA software PPU | Texture upload/presentation exists; GBA register/VRAM/OAM renderer is not ported |
| `platform/dma.c` | `dma_3ds.c` | Immediate, VBlank-budgeted and HBlank-queued copy adapter exists; GBA DMA register decoding and IRQ semantics are not connected |
| `platform/audio.c`, `audio_out.c`, `m4a_mixer.c` | `audio_3ds.c` | NDSP stereo PCM queue exists; upstream mixer and its per-frame block clock are not connected |
| `platform/sio.c`, `mp.c`, `port/mp.h` | `wireless_3ds.c` | Four-player packet API is defined, but the UDS backend intentionally returns unavailable |
| `platform/main.c`, backend frame/input seam | `platform.c`, `clock_3ds.c` | 3DS frame, input polling and GBA key-mask translation exist |
| `platform/sram.c` | `save_3ds.c` | SDMC file read/write adapter exists; SRAM byte-address emulation is not connected |
| `platform/rollback.c` | future rollback adapter | Not started; requires deterministic game-state snapshots and a real transport |
| GBA fixed memory map | future `memory_3ds` layer | Critical blocker; no unsafe fixed-address mapping has been attempted |

## Contracts discovered in the reference

### PPU / video

The reference PPU renders one 240x160 scanline at a time from GBA VRAM, palette
RAM, OAM and display registers. It supports the game's text/affine backgrounds,
sprites, priorities, windows, blending, mosaic and HBlank effects. The
3DS-side presenter accepts a finished 240x160 RGBA framebuffer, converts it to
a tiled 256x256 Citro3D texture, and draws it on the top screen.

**Still required:** reproduce the GBA register and memory semantics, port the
scanline renderer, then call `K3D_PresentGbaFramebuffer` from the game frame
boundary. The presenter alone is not a PPU.

### DMA

The reference DMA layer distinguishes immediate, VBlank and HBlank transfers.
The initial 3DS scheduler supports those timing classes and a VBlank byte
budget. It currently accepts ordinary host pointers; it does not decode the
GBA DMA registers or validate addresses against a GBA memory map.

### Audio

The reference sound engine renders one interleaved stereo signed-16-bit block
per game frame and sends it to a host audio device. The 3DS backend queues
stereo PCM blocks through NDSP with three reusable linear-memory buffers.
The mixer still needs to be built and connected so its sample count, sample
rate and one-frame latency match the game's sound driver.

### Multiplayer

The reference MultiSio layer expects four 16-bit words per transfer and
schedules 16 transfer slots per frame. A 3DS UDS transport is packet-based and
asynchronous, so it must collect complete frame packets and expose them to a
SIO compatibility layer without changing the game's MultiSio protocol.

The current `wireless_3ds.c` is an explicit unavailable backend, not a fake
loopback and not working wireless. Next step is to implement UDS discovery,
host/join, peer identity, packet send/receive, timeout handling and then the
frame-slot bridge. Only after that should lobby recognition and the game's
MultiSio interrupt flow be connected.

## The biggest blocker: GBA address space

The reference project reserves the GBA regions at their original virtual
addresses so existing pointer arithmetic and memory-mapped register access
continue to work. The 3DS process has its own address-space and memory-layout
constraints. We must not assume that casting `0x02000000`, `0x04000000` or
`0x06000000` to pointers is safe on 3DS.

Before compiling original game sources, investigate a supported 3DS mapping
strategy or refactor the decompilation's address macros into region-aware
accessors. This decision affects tasks, DMA, PPU, save, interrupts and every
direct GBA register access.

## Build / integration status

The standalone 3DS target now compiles the platform adapters alongside the
existing smoke-test program. It still does **not** compile the original
decompilation, launch a Kirby level, or support wireless multiplayer.
