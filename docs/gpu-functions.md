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
| 8 | `sub_8258FF50` | 16 | **SetVertexShader**: [shader]. Stores it at device +12436 (and +13156), takes a reference, sets dirty bit 51 and copies the shader's register patches (at shader +592) into the shadow at device +1024. Replayed as often as 9. | `sub_822A8958` → `sub_822A8808` |
| 9 | `sub_8258FFA0` | 16 | **SetPixelShader**: [shader]. Stores it at device +12432, sets dirty bits 52 and 49, copies its register patches (at shader +56). | `sub_822A8618` |
| 10 | `sub_8258FFF0` | 16 | **SetVertexDeclaration**: [declaration]. Stores it at device +11552 and sets dirty bit 51, the vertex shader's. | `sub_822A8960` |
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
| 23 | `sub_82590428` | 16 | **SetRenderTarget**: [index, surface]. Stores the surface at device +12184 + 4 × index and its colour info (+28) in the RB_COLOR_INFO shadow (target 0: `0x2001`; 1-3: `0x2003`-`0x2005`); target 0 also sets the surface info and a full viewport (`sub_822A3C88`). | `sub_822A4340` → `sub_822A3D30` |
| 24 | `sub_82590478` | 16 | **SetDepthStencilSurface**: [surface]. Stores it at device +12200; its +28 goes to RB_DEPTH_INFO (`0x2002`), +32 to the HiZ control. | `sub_822A4060` |
| 25 | `sub_825904C8` | 68 | **Resolve** (EDRAM to texture): flags, and by bit, source rect, destination texture, destination point, clear colour (4 bytes → floats), parameters; also level, slice, clear Z, stencil. | `sub_822AFA58` |
| 26 | `sub_82590620` | 280 | **Clear**: [count, rects (if count), flags, colour, Z, stencil]. | `sub_822A04F8` |
| 27 | `sub_82590698` | 32 | **DrawIndexedPrimitiveUP**: Begin, `memcpy` the indices (2 or 4 bytes each) and vertices, End. | `sub_825BDDC0`, `sub_825A7410`, `sub_825BDDA0` |
| 28 | `sub_8258EAE0` | 16 | Callback: [object]; calls its vtable slot 0 with the device. | |
| 29 | `sub_82590768` | 4 | Sets render targets 0-3 (bits 0-3) and depth (bit 7) from the saved set at `0x82970BC0`. | `sub_822A4340`, `sub_822A4060` |
| 30 | `sub_82590860` | 16 | **SetViewport**: [X, Y, width, height (16 bits each), min Z, max Z]. | `sub_822A3518` |
| 31 | `sub_825908E0` | 8 | **SetScissorRect**: [left, top, right, bottom (signed 16 bits)], with a device field (+11600) set to 1 first. | `sub_822A3D20`, `sub_822A3450` |
| 32 | `sub_82590968` | 16 | **BeginTiling** (predicated tiling, below): [flags, tile count, tile rects]. | `sub_825BED08` |
| 33 | `sub_825909E8` | 16 | **EndTiling**: [flags, destination texture]; resolves each tile. | `sub_825BF200` |
| 34 | `sub_82590A68` | | Saves the viewport (to `0x8296BD60`), render targets 0-3 and depth. | `sub_822A3740`, `sub_822A3998`, `sub_822A39E0` |
| 35 | `sub_82590AD0` | | Restores what 34 saved. | `sub_822A4340`, `sub_822A4060`, `sub_822A3518` |

**The helper object** (commands 5, 6, 13-16): the object at `0x82970BB8`,
with its state at `0x82970BBC`, draws geometry recorded inline in the
command list. In a race its draw (vtable slot 3) is `sub_825A17A0`, which
draws the stored vertices (command 5's 24-byte vertices) through
BeginVertices: 4-vertex triangle strips (screen-space quads, with
PA_CL_VTE_CNTL's bit toggled around them by command 15).
`sub_8258F5C0`, command 15 of the second table (below), makes the same
draw for the minimap's player triangle (a 3-vertex triangle list).

### Single commands

The chain walker `sub_8258EE98` takes items of kinds 0-6 (the item's first
halfword) through a jump table: kind 1 is `sub_8258E698` (the command
lists above), kind 0 `sub_8258E630`, which walks single commands, a
16-byte slot each (`[id, arguments]`), through a **second table at
`0x8265ED48`** with the same ids (2026-10-14): e.g. 15 `sub_8258F5C0`, 17
`sub_8258F770` (DrawVertices: type +4, start +8, count +12), 18
`sub_8258F7C8` (indexed: type +4, base | start +8, count +12), 19
`sub_8258F828` (inline: type +4, count +8, stride +10, data +12), 25
`sub_8258F9A0`, 26 `sub_8258FAC8`, 27 `sub_8258FB20`; 0 and 29-36 are
`sub_825C8388`. Counted in a race: only 5, 7, 13 and 15, several a frame,
for the minimap's player triangle. Kinds 2-5 (`sub_8258E718`,
`sub_8258E780`, `sub_8258E7E8`, `sub_8258E868`) aren't looked at yet.

### Inline vertex draws

Commands 15 and 19 (and `sub_8258F5C0`) reach BeginVertices
(`sub_825BD8F0(device, primitive type, vertex count, stride in bytes)`,
2026-10-14). It flushes the dirty state as a draw does (with the stream 0
stride byte at +12272 set to the stride meanwhile, so the vertex shader is
patched for it), reserves `count × stride` bytes in the command buffer
(`sub_8229E308`) and writes, straight into the ring rather than the
shadow: VGT_INDX_OFFSET (`0x2102`) = 0, vertex fetch constants 93 and 94 =
`[1, 0]` and **95 (stream 0) = the reserved space** (`address | 3`,
`0x10000002 | size in words << 2`: 8-in-32), and an auto-indexed DRAW_INDX
of `count` vertices of the type. It returns the reserved space; the
caller copies the vertices there, then calls End (`sub_825BDDA0`). Types
seen in a race: 6 (strips, the helper's quads, 70-115 a frame), 8
(rectangle lists, 19 a frame: post-processing and bloom passes, 40-byte
vertices), 13 (quad lists, 7-14 a frame), 4 (triangle lists).
`sub_825BDDC0` (the indexed variant, command 27) and DrawVertices
(`sub_825BE300`) weren't reached in a race except DrawVertices once
outside the command list.

## Graphics library

The device pointer is at `0x8296BD90`. Fields used so far:

| Offset | What |
|---|---|
| +8, +16, +24 | Dirty masks (64-bit). Setters OR bits in; the draw functions write out what's dirty. |
| +56 + state | Render state setters (command 1). |
| +440 + state | Sampler state setters (command 2). |
| +1024 | Fetch constant shadow (register `0x4800` on): 32 texture fetch constants of 24 bytes, the same space seen as 96 vertex fetch constants of 8 bytes. Shaders' register patches are copied in here too. |
| +1792 + 16 × c | Vertex shader float constants c0-c255 (registers `0x4000` on; command 7 type 1, `sub_822A81F8`). |
| +5888 + 16 × c | Pixel shader float constants c0-c255 (registers `0x4400` on; `sub_822A8308`). |
| +9984, +10000 | Bool constants, vertex and pixel, one bit each (command 7 type 2; `sub_822A8418`, `sub_822A8470`). |
| +10016, +10080 | Loop (integer) constants, vertex and pixel, one word each (command 7 type 0; `sub_822A84C8`, `sub_822A8518`). |
| +10240 + 4 × (reg − `0x2000`) | Surface registers: RB_SURFACE_INFO, RB_COLOR_INFO, RB_DEPTH_INFO, RB_COLOR1-3_INFO (`0x2000`-`0x2005`). |
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

### The register shadow as a whole

At each draw, the library writes the dirty parts of the shadow into the
ring buffer: `sub_822ADE30(device, dirty mask, first register, shadow)`
writes each run of set bits as a type-0 register write, register = first
+ k from shadow + 4k. Its callers (in the draw path, around
`0x822AFEEC`-`0x822B0030`, and in `0x8229F6E8`-) give the blocks:

| Registers | Shadow |
|---|---|
| `0x2000`- | +10240 |
| `0x2100`- | +10316 |
| `0x2180`- | +10400 |
| `0x2200`- | +10420 |
| `0x2280`- | +10468 |
| `0x2300`- | +10552 |

Fetch constants are flushed by `sub_822AE070` (from +1024 to `0x4800`).
**Dirty masks.** The setters mark what they write in three 64-bit masks
of the device, and DrawIndexedVertices flushes the marked units and
clears the masks. `sub_822ADE30` and `sub_822AE070` take the top bit
(`cntlzd`) as unit 0, so a block taking mask bits lo..lo+n-1 has unit k
at bit lo+n-1-k:

| Mask | Bits | Units |
|---|---|---|
| +16 shader state | 0-11 | `0x2200`-`0x220B` (bit 11 is `0x2200`) |
| | 8 | also has `sub_822ADC08` called, which writes `0x2203` (+10432) itself |
| | 12-43 | fetch constants 0-31, 6 words each (bit 43 is constant 0) |
| | 44-48 | `0x2180`-`0x2184` |
| | 49-52 | shaders: the shader load `sub_822AED18`, which writes the shadow and returns the mask with more bits |
| +24 render state | 1 | bool and loop constants, all 40 words (`0x4900`-, +9984) |
| | 2-22 | `0x2100`-`0x2114` |
| | 23-38 | `0x2000`-`0x200F` |
| | 39-59 | `0x2280`-`0x2294` |
| +32 other state | 0-37 | `0x2300`-`0x2325` |
| | 38-45 | `0x2380`-`0x2387` |
| | 49-54 | `sub_822ADEF0` (other registers) |

The native renderer reads only the marked units (see "Register reads" in
"Performance: the render thread" in `docs/native-renderer.md`).

So the device holds every register and constant a draw needs, in guest
memory: a native renderer can read the GPU's state at each draw straight
from it. It must read it after the library's draw call: some of it is
filled in at draw time (the combined SQ_PROGRAM_CNTL at +10400, the vertex
shader patched for its declaration).

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
| 308-320 | `sub_822A2480`, `sub_822A2530`, `sub_822A25E0`, `sub_822A2690` | Per render target 0-3 (surface at device +12184.., value kept at +11772..): switches the bound surface's colour format between 2_10_10_10 (2) and 2_10_10_10_AS_10_10_10_10 (10), or 2_10_10_10_FLOAT (3) and 2_10_10_10_FLOAT_AS_16_16_16_16 (12), and updates RB_COLOR*_INFO | High-precision blend, per target | 89,328 each | changes |
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

**GPU addresses.** The library converts a guest address to the GPU's
physical address as `(addr & 0x1FFFFFFF) + (((addr >> 20) + 0x200) & 0x1000)`
(IM_LOAD in `sub_822AED18`, SetTexture, SetStreamSource). Read what the GPU
reads through guest physical memory (one 512 MB mapping at the guest
virtual base + 4 GB), not through the CPU's view of the address: for
`0xE...` addresses the two differ, and the CPU view of a pixel shader's
microcode gave the start of a vertex shader instead (2026-10-13).

Resources start with the Xbox 360 Direct3D resource header: +0 is the
type and flags (low bits: 2 index buffer, 5 vertex declaration, 6 vertex
shader, 7 pixel shader), +4 the reference count; the setters
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
- **Surface** (render target or depth): +24 is RB_SURFACE_INFO (`0x2000`,
  device +10240): pitch, and the MSAA mode in bits 16-17, from which the
  library sets the AA config (+10556: 0, `0x8001` or `0xC003`); +28 is
  RB_COLOR_INFO or RB_DEPTH_INFO (EDRAM base in bits 0-11, format in bits
  16-19); +32 is the depth surface's HiZ control. A surface is EDRAM only:
  it reaches a texture through a resolve.
- **Index buffer**: word +0 bit 31 set = 32-bit indices (else 16-bit), and
  bits 29-30 give the endianness of the draw packet; +24 is the address,
  +28 the size in bytes. All 3,189 index buffers seen in a run were
  `0x20000002`: 16-bit, 8-in-16 byte swap. Read by the indexed draw `sub_825BE708`, which writes a
  `DRAW_INDX` packet (`0xC0032201`).
- **Shaders** are loaded by the draw helper `sub_822AED18` with the GPU's
  `IM_LOAD` packet (`0xC0012700`: address with the shader type in bit 0,
  then size in words):

  | | Vertex shader (device +12436) | Pixel shader (device +12432) |
  |---|---|---|
  | Microcode address | +48 | +24 |
  | Microcode size (bytes) | +600 | +64 |
  | Register patches (SetXShader) | +592 + *(+604) | +56 + *(+68) |
  | Program control words (draw) | +592 + *(+612): words 0, 1 → device +10400, +10404 | +56 + *(+76): ORed into the same |
  | Vertex declaration bound for | +24 (declaration id), +32/+40 (stream masks) | |

  The program control words are SQ_PROGRAM_CNTL and its neighbours; the
  vertex and pixel halves are combined at draw time.
- **Vertex declaration** (device +11552): type 5; +24 is likely the
  element count (1-7 seen) and +28 the stream count; the id at +48 is 0
  until first bound, then taken from a counter. The game used 16. When the bound declaration (its
  id at +48) differs from the one the vertex shader was last bound for,
  `sub_822AE340` patches the vertex shader's microcode in place, writing
  three-word groups (the size of a vertex fetch instruction). So vertex
  formats end up in the shader's fetch instructions, as on the GPU, and the
  vertex shader's microcode depends on the declaration: translate it as
  bound at draw time, not once per object.
- **Patched copies** (2026-10-14): `sub_822AED18` patches in place only
  when the GPU is done with the shader's last use (shader +8 against the
  GPU's progress). Otherwise `sub_822AEBF0` reserves room in the command
  buffer, writes a type-3 packet with opcode `0x2B` (IM_LOAD_IMMEDIATE:
  `0xC0002B00 | count << 16`, 0, size in words, then the microcode), copies
  the object's microcode there and patches the copy (`sub_822AE340` with
  the copy's address in r4). The object's microcode and +24 stay as they
  were, and the GPU keeps the copy loaded until the shader is loaded
  again. In a race 100-400 draws a frame run such a copy. After a patch
  in place, the caller stores the declaration's id at shader +24 (giving
  the declaration one from a counter if it has none) and the stream masks
  (device +12272, +12280) at +32, +40. Patching happens only when the
  draw's dirty flags have bit 51 (SetVertexShader) and shader +592 bit
  `0x40` is clear.

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

### Checked at runtime

`--log_replay_commands` hashes each shader's microcode when it's bound, the
way the SDK names its dumps (XXH3-64 of the raw guest memory), in a run
through the menus and a race (`itc_352.log`):

- **Pixel shaders**: 46 distinct microcodes, all 46 among the SDK's
  dumped shaders, sizes matching (36-204 bytes). The layout above is right.
- **Vertex shaders**: 58 distinct microcodes at bind time, 37 of them among
  the dumps. Every vertex shader object was seen with more than one hash:
  its microcode changes as declarations are patched in (the others are
  states never drawn).
- ~390 objects of each kind share these microcodes (an object per
  material).

## A race frame's passes

Traced with `--log_replay_commands` (17 race frames, `itc_353.log`). Each
row is a render target (or depth-only) pass and the resolves out of it,
in order; sizes are the destination textures'. "Probably" marks a guess
from the pass's shape, not checked by hiding it.

| # | Target (EDRAM) | Draws | Resolved to | Probably |
|---|---|---|---|---|
| 1 | 8_8_8_8, pitch 560, + depth | 2-27 | 8_8_8_8 512x512 | A projected texture (headlights or the car's shadow) |
| 2 | 8_8_8_8, pitch 560, no depth (twice) | 1 each | the same 512x512 texture | Blurring it |
| 3 | 8_8_8_8, pitch 1040, + depth | 0 (clear only) | 8_8_8_8 1024x1024 | Cleared every frame, never drawn |
| 4 | Depth only, pitch 1040 | 80-168 | 24_8_FLOAT 1024x1024 (flags `0x204`) | Shadow map |
| 5 | Depth only, pitch 1040 | 4 | 24_8_FLOAT 1024x1024 (flags `0x4`) | A second depth map |
| 6 | 8_8_8_8, pitch 520, 4x MSAA + depth | ~730 | 2_10_10_10_AS_16_16_16_16 512x128 (flags `0x300`) | Rear-view mirror: only in some frames (camera views) |
| 7 | 2_10_10_10_FLOAT, pitch 280, 4x MSAA + depth | 420-520 | 2_10_10_10_AS_16_16_16_16 256x256 | Reflection map for the cars' paint |
| 8 | 8_8_8_8, pitch 280, 4x MSAA | 1 | 8_8_8_8 256x256 | The minimap's road outline (seen in a native renderer dump) |
| 9 | 16_16, pitch 560, + depth, cleared to `0x80808000` | 350-470 | 16_16 512x288 | Half-resolution velocity (motion blur) or depth |
| 10 | **Main view**: 2_10_10_10_FLOAT_AS_16_16_16_16 (7e3 HDR), pitch 1040, **2x MSAA**, + depth at EDRAM tile 936 | 700-860, then ~350 more | 16_16_16_16 1024x576 (rect 0,0-1024,576; flags `0xF4000000`), twice | The scene, then a copy before more drawing (transparent or refracting objects reading it) |
| 11 | 2_10_10_10_FLOAT, pitch 320 | 1 per step | 16_16_16_16 256x144: 0-3 draws, then 8 resolves (flags `0xF4000100`), then 3 | Bloom: downsample and blur chain (the "GrowBlend" passes; see patch 0001) |
| 12 | Main view again | 1 | 16_16_16_16 1024x576, then 2_10_10_10_AS_16_16_16_16 **1280x720** (rect 1024x576) | Combining the post-processing, then the frame |
| 13 | The saved targets (mask `0x8F`), viewport 1280x720 | 29-49 | | Upscale and HUD |

About 24-26 resolves a frame, matching the counts. Findings:

- **The main view's format changes per draw**: the trace shows its colour
  format as 12 (2_10_10_10_FLOAT_AS_16_16_16_16), as bound, but draws
  switch it with render states 308-320: in an attract-mode race, 747,509
  of its indexed draws went to format 3 (2_10_10_10_FLOAT) and 29,733 to
  format 12 (2026-10-13). Identify it by pitch 1040 and 2x MSAA with
  either format.
- **The 3D view renders at 1024x576 with 2x MSAA**, not 1280x720: its
  surface pitch is 1040 (1024 rounded up to whole EDRAM tiles of 80
  pixels), its resolves copy 1024x576, and its depth starts at tile 936 =
  13 tiles across x 72 rows (576 lines x 2 samples / 16). In 7e3 colour
  plus depth that's 9.4 MB of the 10 MB EDRAM. The viewport is still given
  as 1280x720; how it maps to 1024x576 isn't known yet.
- The menus resolve their own 1280x720 frame (54 format) the same way.
- Every surface is EDRAM only, and every texture the passes produce comes
  from a resolve: a native renderer can render each pass straight into a
  Vulkan image of the resolve's size and format, and treat the resolve as
  a copy or a no-op.

### Pass command items

As `--log_replay_commands`' frame trace decodes them
(`src/debug/replay_stats.cpp`, `TraceCommand`); arg1 is the item count;
words are big-endian.

- **23 SetRenderTarget**, 16 bytes an item: +0 index (0-3), +4 surface.
- **24 SetDepthStencil**, 16 bytes: +0 surface (0 = none).
- **25 Resolve**, 68 bytes: +0 which optional fields are present (bit 0
  rect, bit 1 destination, bit 2 point, bit 3 clear colour); +4 flags;
  +8 source rect (4 words: left, top, right, bottom); +24 destination
  texture object (its fetch constant at +28..+48, so the base address is
  the one later draws sample it by); +28 destination point (2 words); +36
  level; +40 slice; +44 clear colour; +48 clear Z; +52 clear stencil. The
  resolve copies from the bound render target (or the depth surface: the
  `24_8_FLOAT` resolves of depth-only passes).
- **26 Clear**, 280 bytes: +4 rect count (rects follow, if any), +8
  flags, +268 colour, +272 Z, +276 stencil.
- **29**: +0 mask (bits 0-3 render targets, bit 7 depth).
- **30 SetViewport**: words as the frame trace prints them (x, y, width,
  height, then Z range).

## Predicated tiling

(2026-10-14.) The menus' 3D views (the car on the Free Run screen, the
Course In city picture) are drawn into a 1280x720, 4x MSAA, 32-bit
surface with depth: about 14 MB, more than the 10 MB of EDRAM. So the
menus use the XDK's predicated tiling: the GPU replays the commands once
per tile.

- **Command 32**, BeginTiling (`sub_825BED08(device, flags, count, rects,
  clear colour, Z, stencil)`): item +0 flags (0 seen), +4 tile count, +8
  the tiles' rectangles (16 bytes each). The handler passes a clear colour
  (4 floats) and Z all equal to the float at `0x82000A34` (0 seen), stencil
  0. Seen: 3 tiles, `0,0-1280,256`, `0,256-1280,512`, `0,512-1280,720`.
  The library keeps the count at device +12488 and the rectangles at
  +12492.
- **Command 33**, EndTiling (`sub_825BF200(device, flags, no rects,
  destination, clear colour, ...)`): item +0 flags (`0x303` seen), +4 the
  destination texture. For each tile it calls Resolve (from `0x825BF348`,
  and `0x825BF378` for the last) with the tile's rectangle as source and
  its top-left as the destination point; on every tile but the last it
  adds `0x100` (if a render target is bound) and `0x200` (if depth is)
  so the next tile starts cleared.
- Not seen in races (32 and 33 came up in one window of the replay counts,
  probably a menu).

## Movies

(2026-10-14.) The boot movie (`UBI_LOGO.wmv`) is drawn outside the command
list, on the render thread: `sub_824842C8` (reached through a function
pointer) sets three textures, shaders, sampler states and stream 0 (a
buffer at the movie object's +64, stride 20) through the library directly,
then calls DrawVertices (`sub_825BE300(device = movie object +56, type 4,
start 0, 6 vertices)`: two triangles) from `0x8248466C`. The textures are
the frame's planes, format 2 (8-bit), linear, not byte-swapped: Y
1280x720 (pitch 1280) and two 640x360 (pitch 768), in three rotating sets
of buffers rewritten as the movie decodes. DrawVertices takes the vertex
count, not the primitive count (command 17's handler converts).

## The end of a frame

After the command list's last pass (row 13 above), the render thread's
draw listeners finish the frame outside the command list:

1. `sub_82091BC0` (the listener between `Scene::Draw` and present):
   - `sub_8258D998(video output = renderer + 64)` resolves the EDRAM
     frame into one of two front buffer textures,
     video output + 4 × (68 + *(+268)), with Resolve (`sub_822AFA58`) on the
     device at video output +140.
   - `sub_8229F1B0(device)` writes one register (`0x5C8`), then waits until
     the GPU has read everything written so far (`sub_8229E908` with the
     current write position) and spins until device +10868 is 0: **a full
     wait for the GPU every frame**, on top of the ring buffer waits.
2. `Renderer::Present` (`sub_82091A60`):
   - `sub_8258DC58`: `sub_822A8A60` (writes a few registers, `0x5C8` and
     `0xD00`; not identified), the vblank wait (`sub_8258DAA8`, which we
     reduced to its device call), then **Swap** `sub_822A08A0(device, front
     buffer)`, which calls the kernel's `VdSwap` and
     `VdGetSystemCommandBuffer`; then flips the front buffer index
     (+268, two buffers).
   - `sub_8258D9F8`: unbinds every texture (16), stream (4), the index
     buffer and both shaders, dropping the frame's references.

The upscale from the 1024x576 view: the command list's last resolve copies
the view (rect 1024x576) into the top-left of a 1280x720 texture, and the
saved 1280x720 targets are set again with a 1280x720 viewport; the draw
after that samples the view across the screen (the upscale; seen in an
attract race, 2026-10-14), and the rest are the HUD (in attract races, only
the game's logo). The front buffer resolve copies render target 0 (that
pitch 1280, 8_8_8_8 surface) whole into a 1280x720 8_8_8_8 texture.

For a native renderer, present is: take the final 1280x720 image (or the
1024x576 view and HUD, at any resolution) and present it with a Vulkan
swapchain; Swap and `VdSwap` aren't called, and the per-frame GPU wait
goes.

## Not yet known

- (20, 22 and 28 are never replayed. 32 and 33 are predicated tiling,
  below.)
- Confirming the "likely" rows at runtime.
- Render state 324.
- How the 1280x720 viewport maps onto the 1024x576 main view, and whether
  the draw after the last resolve is the upscale.
- `sub_822A8A60` at present.
- The resolve flags: bits 8-9 (`0x300`, `0x100`, `0x204`), and the top
  bits `0xF4000000` (likely an exponent bias for the HDR copies; Resolve
  puts flags bits 26-31 into RB_COPY_DEST_INFO's exponent bias). The copy
  registers are written to the ring, not the shadow; RB_COPY_DEST_INFO's
  swap bit comes from the destination's swizzle (see "Red and blue
  swapped in resolves" in `native-renderer.md`).
- Which passes the guesses above really are (hiding each with
  `--debug_zero_owners` or a resolve skip would settle it).
- The vertex declaration's element list (past word 16); a native renderer
  may not need it, since the formats are patched into the vertex shader.
- (Decoded since: the clear flags and the resolve's arguments and flags;
  see "Progress on milestone 4" in `native-renderer.md`.)

## Tools

- `--log_replay_commands` also traces one whole frame every 10 s
  ("frame trace" lines): each surface, resolve, clear, viewport and
  save/restore command in order, with its arguments decoded and the number
  of draws since the previous one. It logs each new shader (with its
  microcode hash), vertex declaration and index buffer, every render and
  sampler state id with its setter, and every new texture fetch constant.
- `--log_replay_commands` logs, every 2 s, how often each command id is
  replayed per frame (and its items), plus the last arguments of each
  unknown command. It wraps every handler but 0/36/37
  (`sub_82304258` is used elsewhere too); command 7 is counted in its
  interpolation wrapper.

- `--dump_image=<path>` writes the loaded executable image
  (`0x82000000`-`0x82980000`, big-endian) and exits at the first frame.
  Read tables such as the command table from it; `tools/scan_ptrs.py` also
  takes it.
