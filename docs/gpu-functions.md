# GPU functions

What's known about how the game drives the GPU: the render command replay,
the graphics library (the Xbox 360's Direct3D, linked into the game) and
its waits on the GPU. This is the groundwork for a native renderer that
replaces the emulated GPU (`librexgpu-xenos.so`). Game-side recording,
interpolation and everything else are in `functions.md`.

Addresses and names follow `functions.md`: names are our own, and
"likely" marks an identification from argument shapes that hasn't been
checked at runtime.

## The path from the game to the GPU

1. The main thread records render commands into the command list (see
   "Render command recording" in `functions.md`).
2. The render thread replays them: `sub_8258EE98` walks the item chains,
   and `sub_8258E698` dispatches each command through the table at
   `0x8265EDE0`.
3. Each handler calls the graphics library on the device at `0x8296BD90`
   (`lis rX,-32105` + `-17008`), which writes Xenos GPU packets (PM4) into
   the ring buffer.
4. The emulated GPU's thread ("GPU Commands", in the SDK) reads the ring
   buffer and translates to Vulkan.

A native renderer would replace steps 3 and 4: implement the handlers
below with Vulkan instead of the graphics library.

## Command format

`sub_8258E698(r4 = first slot)` loops over 16-byte slots:

- Slot header: `[id, arg1, arg2, 0]`; the payload starts in the next slot.
- The handler is called as `handler(r3 = payload, r4 = arg1, r5 = arg2)`
  and returns how many payload slots it used; the next command follows.
- Id 36 ends the chain.

Most handlers treat arg1 as a count of 16-byte items and call one library
function per item. The exceptions are marked with their item size.

## Command table (`0x8265EDE0`)

| Id | Handler | Item | What it does | Library call |
|---|---|---|---|---|
| 0, 36, 37 | `sub_82304258` | | Nothing (returns arg1). 36 ends a chain. | |
| 1 | `sub_8258FBD0` | 16 | **SetRenderState**: [state, value]. Calls the device's setter for the state at device + state + 56 (the Xbox 360 Direct3D keeps a setter per render state). | per-state setter |
| 2 | `sub_8258FC30` | 16 | **SetSamplerState**: [sampler, state, value]. Setter at device + state + 440. | per-state setter |
| 3 | `sub_8258FC90` | 16 | **SetBlendState**: [render target, blend control]. Writes the register shadow for RB_BLENDCONTROL0 (`0x2201`) for target 0, or `0x2201` + 7 + target (`0x2209`-`0x220B`, RB_BLENDCONTROL1-3) for the others. The value is the register's: `0x00010001` = colour and alpha source ONE, destination ZERO. | `sub_822A3B30` |
| 4 | `sub_8258FCE0` | 16 | **SetTexture**: [sampler, texture]. Merges the texture's fetch constant (+28..+48) into the fetch constant shadow at device + 1024 + 24 × sampler (see "Resources"), and fences the texture it replaces. | `sub_822A6008` |
| 5 | `sub_8258E9F8` | arg1 × 24 bytes | Inline vertex data for the helper object (below): stores payload and count at state +12, +16. | |
| 6 | `sub_8258EA20` | arg1 × 2 or 4 bytes | Inline index data for the helper object: payload, count, 32-bit flag (arg2) at state +20, +24, +28. | |
| 7 | `sub_8258FD48` | arg1 registers | **Set shader constants**. arg2 = stage << 24 \| type << 16 \| start << 8 \| count. Stage 0 = vertex, 1 = pixel; type 0-3 picks a setter. Wrapped by interpolation. | VS `sub_822A84C8`, `sub_822A81F8`, `sub_822A8418`; PS `sub_822A8518`, `sub_822A8308`, `sub_822A8470` |
| 8 | `sub_8258FF50` | 16 | **SetVertexShader**, likely: [shader]. Stores it at device +12436 (and +13156), takes a reference, sets dirty bit 51 and copies the shader's register patches (at shader +592) into the shadow at device +1024. Replayed as often as 9. | `sub_822A8958` → `sub_822A8808` |
| 9 | `sub_8258FFA0` | 16 | **SetPixelShader**, likely: [shader]. Stores it at device +12432, sets dirty bits 52 and 49, copies its register patches (at shader +56). | `sub_822A8618` |
| 10 | `sub_8258FFF0` | 16 | **SetVertexDeclaration**, likely: [declaration]. Stores it at device +11552 and sets dirty bit 51, the vertex shader's. | `sub_822A8960` |
| 11 | `sub_82590040` | 16 | **SetStreamSource**: [stream, buffer, offset, stride]. Writes the buffer's fetch constant (address + offset, size − offset) to vertex fetch constant 95 − stream (device + 1024 + 8 × (95 − stream)); the handler's dirty bit, (95 − stream) / 3 + 20, is the same slot. Buffer pointer at device + 12204 + 4 × stream, stride byte at device + 12272 + stream. | `sub_822A3758` |
| 12 | `sub_825900B8` | 16 | **SetIndices**: [index buffer]. Stores it at device +12180 and fences the previous one; replayed exactly once per indexed draw (command 18). | `sub_822A3908` |
| 13, 14 | `sub_8258EA58` | 16 | Helper object set-up: stores [a, b, c] at state +0..+8, calls the helper's vtable slot 2 with the device. | |
| 15 | `sub_82590108` | 16 | Helper object draw (vtable slot 3) from the stored inline data, with a device flag (+10444 bit 0) toggled around it. | `sub_822A2470`, `sub_822A2430` |
| 16 | `sub_825901B0` | 16 | Same, vtable slot 4, more arguments (indexed). | same |
| 17 | `sub_82590268` | 16 | **DrawVertices**, likely: [type, start, count]; the vertex count comes from a per-type table at `0x82000950` (count × a + b). | `sub_825BE300` |
| 18 | `sub_825902E0` | 12 | **DrawIndexedVertices**, likely: [type, base \| start (16 bits each), count], same table. | `sub_825BE708` |
| 19 | `sub_82590360` | 12 | **DrawPrimitiveUP**: [type, count, stride (16 bits each), data]; Begin, `memcpy` the vertices, End. | `sub_825BD8F0` (BeginVertices), `sub_825A7410` (memcpy), `sub_825BDDA0` (End) |
| 20 | `sub_825903D8` | 16 | Unknown: [object], skipped if null. | `sub_822A67B0` |
| 21 | `sub_8258EAD0` | arg1 bytes | Skips arg1 bytes of payload (inline data or padding). | |
| 22 | `sub_82591798` | 16 | Unknown: [object, flag]. Asks the object (vtable 2); if not ready and flag = 1, calls vtable 1, `sub_82591540` (with `0x8296BD8C`), vtable 3. | `sub_82591540` |
| 23 | `sub_82590428` | 16 | **SetRenderTarget**, likely: [index, surface]. | `sub_822A4340` → `sub_822A3D30` |
| 24 | `sub_82590478` | 16 | **SetDepthStencilSurface**, likely: [surface]. | `sub_822A4060` |
| 25 | `sub_825904C8` | 68 | **Resolve** (EDRAM to texture): flags, and by bit, source rect, destination texture, destination point, clear colour (4 bytes → floats), parameters; also level, slice, clear Z, stencil. | `sub_822AFA58` |
| 26 | `sub_82590620` | 280 | **Clear**: [count, rects (if count), flags, colour, Z, stencil]. | `sub_822A04F8` |
| 27 | `sub_82590698` | 32 | **DrawIndexedPrimitiveUP**: Begin, `memcpy` the indices (2 or 4 bytes each) and vertices, End. | `sub_825BDDC0`, `sub_825A7410`, `sub_825BDDA0` |
| 28 | `sub_8258EAE0` | 16 | Callback: [object]; calls its vtable slot 0 with the device. | |
| 29 | `sub_82590768` | 4 | Sets render targets 0-3 (bits 0-3) and depth (bit 7) from the saved set at `0x82970BC0`. | `sub_822A4340`, `sub_822A4060` |
| 30 | `sub_82590860` | 16 | **SetViewport**: [X, Y, width, height (16 bits each), min Z, max Z]. | `sub_822A3518` |
| 31 | `sub_825908E0` | 8 | **SetScissorRect**: [left, top, right, bottom (signed 16 bits)], with a device field (+11600) set to 1 first. | `sub_822A3D20`, `sub_822A3450` |
| 32 | `sub_82590968` | 16 | Unknown: [a, b, c]; a large function that sets viewport and scissor and copies memory. | `sub_825BED08` |
| 33 | `sub_825909E8` | 16 | Unknown: [a, b]; a large function that sets viewport and scissor and resolves. | `sub_825BF200` |
| 34 | `sub_82590A68` | | Saves the viewport (to `0x8296BD60`), render targets 0-3 and depth. | `sub_822A3740`, `sub_822A3998`, `sub_822A39E0` |
| 35 | `sub_82590AD0` | | Restores what 34 saved. | `sub_822A4340`, `sub_822A4060`, `sub_822A3518` |

**The helper object** (commands 5, 6, 13-16): the object at `0x82970BB8`,
with its state at `0x82970BBC`, draws geometry recorded inline in the
command list. What it draws, and which library calls its vtable reaches,
isn't known yet.

## Graphics library

The device pointer is at `0x8296BD90`. Fields used so far:

| Offset | What |
|---|---|
| +8, +16, +24 | Dirty masks (64-bit). Setters OR bits in; the draw functions write out what's dirty. |
| +56 + state | Render state setters (command 1). |
| +440 + state | Sampler state setters (command 2). |
| +1024 | Fetch constant shadow (register `0x4800` on): 32 texture fetch constants of 24 bytes, the same space seen as 96 vertex fetch constants of 8 bytes. Shaders' register patches are copied in here too. |
| +10424 + 4 × (reg − `0x2201`) | Shadow of the render backend and setup registers, from RB_BLENDCONTROL0 (`0x2201`). Checked for `0x2201`-`0x220B` (command 3) and `0x2206` below. |
| +10444 | PA_CL_VTE_CNTL (`0x2206`): bit 0 (viewport X scale enable) is read, cleared and restored around the helper object's draws, which are in screen space. |
| +10768 | Pointer to the GPU's ring buffer read pointer (written back by the GPU). |
| +10780 | Ring buffer position the waits compare against. |
| +11600 | Set to 1 before a scissor rect (command 31). |
| +11552 | Vertex declaration (command 10). |
| +12180 | Index buffer (command 12). |
| +12204 + 4 × stream | Stream source buffers (command 11). |
| +12432, +12436 | Pixel shader, vertex shader (commands 9, 8). |
| +13156 | Also set by command 8. |
| +13168, +13172 | Write pointer and limit of a small buffer of resource fences (written when a bound resource is replaced). |

## Render states and sampler states

States are byte offsets into the device's setter tables (command 1: device
+ 56 + state; command 2: device + 440 + state). Each setter writes fields of
the device's register shadow. These are all the states the game set in a
run through the menus and a race (`itc_351.log`, `--log_replay_commands`),
named by the register field their setter writes. "Changes" counts how often
the value differed from the previous one (capped at 1000).

| State | Setter | Writes | Name | Uses | Values seen |
|---|---|---|---|---|---|
| 40 | `sub_822A18D8` | RB_DEPTHCONTROL (+10420) bit 1, only with a depth buffer bound | Z enable | 116,071 | changes |
| 44 | `sub_822A1950` | RB_DEPTHCONTROL bits 4-6 | Z function | 96,156 | always 6 |
| 48 | `sub_822A1920` | RB_DEPTHCONTROL bit 2 | Z write enable | 196,087 | changes |
| 52 | `sub_822A1290` | PA_SU_SC_MODE_CNTL (+10440) bits 3-10 | Fill mode | 95,880 | always 0 |
| 56 | `sub_822A1260` | PA_SU_SC_MODE_CNTL bits 0-2 | Cull mode | 776,726 | changes |
| 60 | `sub_822A1300` | Blend enable (software copy +11596 bit 31); writes RB_BLENDCONTROL0-3 (+10424, +10456..+10464), `0x00010001` when off | Alpha blend enable | 335,797 | changes |
| 72 | `sub_822A1420` | Blend control bits 0-4 (and alpha's, unless separate alpha is on) | Source blend | 236,700 | changes |
| 76 | `sub_822A14B0` | Blend control bits 8-12 | Destination blend | 236,700 | changes |
| 80 | `sub_822A1390` | Blend control bits 5-7 | Blend op | 4,530 | always 0 |
| 96 | `sub_822A12C0` | RB_COLORCONTROL (+10428) bit 3 | Alpha test enable | 502,197 | changes |
| 100 | `sub_822A1728` | Float (value × a constant at `0x82002200`) at +10372 | Alpha reference | 623,676 | changes |
| 104 | `sub_822A1790` | RB_COLORCONTROL bits 0-2 | Alpha function | 83,895 | changes |
| 200 | `sub_822A3D20` | +11600, then reapplies the scissor rect | Scissor enable | 186,753 | always 0 |
| 204 | `sub_822A1CF0` | Polygon offset scale (+10704, +10712, value × a constant) and PA_SU_SC_MODE_CNTL bits 11-12 | Slope-scaled depth bias | 9,588 | changes |
| 208 | `sub_822A1DB8` | Polygon offset (+10708, +10716) and the same enables | Depth bias | 18,648 | changes |
| 212 | `sub_822A1EB0` | +10332 bits 0-3, only with render target 0 bound | Colour write mask | 23,970 | changes |
| 304 | `sub_822A2430` | PA_CL_VTE_CNTL (+10444) | Viewport transform enable | 34,236 | changes |
| 308-320 | `sub_822A2480`, `sub_822A2530`, `sub_822A25E0`, `sub_822A2690` | Per render target 0-3 (+12184.., +11772..): switches the bound surface's format between two variants (formats 2, 3, 10, 12) | Unknown; one per target | 89,328 each | changes |
| 324 | `sub_822A27E8` | +10560 bit 0 | Unknown | 14,193 | changes |
| 328 | `sub_822A2820` | PA_SU_SC_MODE_CNTL bit 21 (MULTI_PRIM_IB_ENA) | Primitive reset enable | 2,397 | always 1 |
| 332 | `sub_822A2850` | +10328 | Primitive reset index | 2,397 | always `0xFFFF` |

Sampler states write the sampler's texture fetch constant in the shadow
(device + 1024 + 24 × sampler):

| State | Setter | Writes | Name | Uses |
|---|---|---|---|---|
| 0 | `sub_822A31A0` | Word 0 bits 10-12 (clamp X) | Address U | 657,718 |
| 4 | `sub_822A31F0` | Word 0 bits 13-15 (clamp Y) | Address V | 657,718 |
| 16 | `sub_822A2BF8` | Word 3 bits 19-20 (mag filter) and 25-27 (anisotropic filter) | Mag filter | 715,677 |
| 20 | `sub_822A2A70` | Word 3 bits 21-22 (min filter) and 25-27 | Min filter | 714,021 |
| 24 | `sub_822A2D80` | Word 3 bits 23-24 | Mip filter | 825,638 |
| 28 | `sub_822A2F90` | Word 4 bits 12-21 (value × a constant) | LOD bias | 236,852 |
| 36 | `sub_822A2E70` | Word 3 bits 25-27 via a table at `0x82013628`, when anisotropic filtering is on; kept per sampler at +11668 | Max anisotropy | 102,775 |

So samplers map straight onto the GPU's fetch constant fields, and render
states onto a few registers (depth, blend, colour control, setup).

## Resources

Resources start with the Xbox 360 Direct3D resource header; the setters
write the ring buffer position into +8 (the fence) of a resource they
unbind, so the library knows when the GPU is done with it.

- **Texture**: the GPU texture fetch constant (6 words) at +28..+48:
  word 1 (+32) holds the format and base address, word 2 (+36) the size,
  word 5 (+48) the mip address. SetTexture converts the addresses to the
  GPU's physical form (`addr & 0x1FFFFFFF`, plus `0x1000` when the top
  bits of the address say so). The SDK's `xe_gpu_texture_fetch_t` describes
  the words.
- **Vertex buffer**: the vertex fetch constant at +24 (address) and +28
  (size).
- **Index buffer**, **shaders**, **vertex declaration**: layout not read
  yet. Shaders carry register patch lists: a list of (offset, count)
  followed by words, copied or masked into the device's shadow.

The draw functions (`sub_825BE300`, `sub_825BE708`, `sub_825BD8F0`,
`sub_825BDDC0`) share helpers `sub_8229EB10`, `sub_8229EF48` (73 call
sites), `sub_822ADC08`, `sub_822ADE30`, `sub_822ADEF0`, `sub_822AE070`,
`sub_822AE1D0` and `sub_822AED18`: most likely writing pending state and
packets into the ring buffer.

## Waits on the GPU

`sub_8229D8A0` is one step of a busy-wait: a few `db16cyc`, then it reads
a tick count (`sub_825B9A60`: r13+256 → +332) and gives up after 5000
ticks (`sub_822ACAA8`). It's called in loops from:

| Loop in | Callers | What it waits for |
|---|---|---|
| `sub_8229E908` (at `0x8229E98C`) | 10 | The GPU's read pointer (device +10768) to pass a position relative to +10780: **space in the ring buffer**. |
| `sub_8229DC28` (at `0x8229DC78`) | 2 | Unknown. |
| `sub_8229DCC0` (at `0x8229DD1C`) | 2 | Unknown. |
| `sub_822B64A0` (at `0x822B64E8`) | 1 | Unknown. |

In the race profile (see "Performance" in `functions.md`), `sub_8229D8A0`
was 46% of the render thread and `sub_8229E908` 2%: the render thread
spends about half its time waiting for the emulated GPU to make room in
the ring buffer. A native renderer has no ring buffer, so these waits go.

## What a frame replays

Counted with `--log_replay_commands` (2026-10-13, `itc_350.log`): calls
per frame, and items per frame where a call carries several. Menus were
measured over the main menu screens; the race over 72 two-second windows
of driving, including camera switches.

| Id | What | Race: calls (items) | Menus: calls (items) |
|---|---|---|---|
| 1 | Render states | 3,860 (7,709) | 212 (864) |
| 2 | Sampler states | 1,984 (9,070) | 115 (727) |
| 3 | Unknown | 1,407 | 10 |
| 4 | Textures | 1,898 (2,621) | 75 (80) |
| 5, 13, 15 | Helper object: data, set-up, draw | 153 each | 61 each |
| 7 | Shader constants | 3,663 (30,688 registers) | 95 (589 registers) |
| 8, 9 | Unknown (a pair) | 1,434 each | 22, 25 |
| 10 | Unknown | 299 | 12 |
| 11 | Stream sources | 279 (637) | 6 (18) |
| 12 | Unknown (one per indexed draw) | 2,177 | 11 |
| 18 | Indexed draws | 2,177 | 11 |
| 19 | Draws with inline vertices | 35 | 9 |
| 21 | Inline data (bytes) | 3 (2,579) | 9 (10,451) |
| 23, 24 | Render target, depth surface | 16.5, 12.5 | 5, 1.4 |
| 25 | Resolves | 24.6 | 1.7 |
| 26 | Clears | 9.5 | 2 |
| 29 | Set saved render targets | 1 | 1.5 |
| 30 | Viewports | 7.6 | 3.1 |
| 31 | Scissor rects | 5.9 | 25.5 |
| 32, 33 | Unknown | rare (one window) | 0.2 each |
| 34, 35 | Save, restore render targets | 8 each | 4 each |

Never replayed: 6, 16, 17, 20, 22, 27, 28 (14 shares a handler with 13
and isn't counted apart). A race frame is about 2,200 indexed draws and
~50,000 state items (render and sampler states, constant registers,
textures) that the graphics library turns into GPU packets one register at
a time. 25 resolves a frame cover the reflection, shadow map and
post-processing passes.

Sample arguments of the unknown commands (payload words):

- 3: `[0, 0x00010001]` in every sample: two 16-bit halves of 1.
- 8: a 4 KB-aligned address (`0x01BA1000`, `0x02571000`) or 0.
- 9: a heap object (`0x400C86D0`) or 0.
- 10: a heap object (`0x400C8310`, `0x40064310`).
- 12: an address (`0x066C13E0`, `0x07F61F90`).
- 32: `[0, 3, 0x82600360]` (a static object in the executable's data).
- 33: `[0x303, 0x40070350]`.

## Textures in use

167 distinct texture fetch constants (format, flags and size; the address
isn't part of the count) were bound in the same run:

- **Formats**: DXT4/5 85, DXT1 42, DXT2/3 17, 8_8_8_8 12, 4_4_4_4 3,
  2_10_10_10_AS_16_16_16_16 3, 16_16_16_16 2, 16_16 1, 24_8_FLOAT 1 (a
  depth buffer read as a texture), 32_FLOAT 1. Nearly all compressed, which
  Vulkan supports as BC1-BC3.
- **Tiled**: 163 of 167 (the 360's tiled layout; they need untiling).
- **Endianness**: 8-in-16 for 149, 8-in-32 for 18 (byte swaps).
- **Sizes**: mostly powers of two up to 1024x1024; two 1280x720 (screen
  copies, from resolves).

## Not yet known

- Commands 32 and 33 (rare), and what the helper object draws. (20, 22
  and 28 are never replayed.)
- Confirming the "likely" rows at runtime (8, 9, 10: which object is a
  vertex shader is a guess from its reference counting and dirty bits).
- Render states 308-320 and 324.
- Texture, vertex buffer and surface objects: their layout in guest memory
  (formats, tiling, sizes), which the native renderer needs to read.
- The shaders: how commands 8-10 and 12 relate to vertex and pixel shaders
  and vertex declarations.
- How present and the frame's start and end reach the library
  (`Renderer::Present`, `sub_82091BC0`).

## Tools

- `--log_replay_commands` logs, every 2 s, how often each command id is
  replayed per frame (and its items), plus the last arguments of each
  unknown command. It wraps every handler but 0/36/37
  (`sub_82304258` is used elsewhere too); command 7 is counted in its
  interpolation wrapper.

- `--dump_image=<path>` writes the loaded executable image
  (`0x82000000`-`0x82980000`, big-endian) and exits at the first frame.
  Read tables such as the command table from it; `tools/scan_ptrs.py` also
  takes it.
