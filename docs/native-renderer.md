# Native renderer

A renderer that draws the game's render commands with Vulkan directly,
replacing the emulated GPU (`librexgpu-xenos.so`) and moving the project
away from the ReXGlue SDK. Why: the emulated GPU limits the frame rate
(see "Performance" in `functions.md`), and the game's command list is a
narrow boundary to draw from (see `gpu-functions.md`, which holds what's
known about it; this file is the plan and its progress, and its last
section, "Performance: the render thread", the work on the renderer's
speed).

## Decisions

- **Our own window and swapchain**, in `itc_recomp/src/native/`, not the
  SDK's presenter. SDL is used for the window: it's third-party, and the
  process has one copy of it (in `librexruntime.so`), whose event loop the
  SDK runs on the main thread. Window calls go to the main thread through
  `SDL_RunOnMainThread`; events for our window are ignored by the SDK's
  loop (it dispatches by window id).
- **Vulkan loaded at runtime** through SDL (`VK_NO_PROTOTYPES`, our own
  function table), so the build needs no Vulkan library. Headers come from
  the SDK's third-party copy of the Khronos headers.
- **Shaders: reuse the SDK's Xenos-to-SPIR-V translator**, to stay (the
  user's call, 2026-10-14: it works well). It's SDK source compiled into
  our own library and doesn't need the emulated GPU. The plugin builds it with hidden symbols, so
  `itc_recomp/CMakeLists.txt` compiles its sources into a static library of
  ours (`itc_xenos_shaders`: the shader, translator and SPIR-V sources,
  `format/ucode.cpp`, `util/draw.cpp`, `registers.cpp`). The translator
  reads the `dump_shaders` setting, which only the plugin defines; we
  supply an empty one in `src/native/shaders.cpp`. `util/draw.cpp` defines
  two GPU settings of its own (`half_pixel_offset`,
  `resolve_resolution_scale_fill_half_pixel_offset`), which are then
  registered twice; setting them on the command line may reach only one
  copy.
- **Guest memory is mirrored, not imported** (a mirror per frame in flight
  since milestone 4). The translated shaders read
  vertices (and the index buffer is read) from one storage buffer laid out
  like guest physical memory. Importing guest memory itself
  (`VK_EXT_external_memory_host`) was the plan, but RADV refuses it:
  `vkAllocateMemory` returns `VK_ERROR_INVALID_EXTERNAL_HANDLE` both for
  the whole 512 MB and for a single 64 KB range in use (shader microcode),
  so it's the kind of memory (a shared mapping of the emulator's memory
  file), not unallocated pages (2026-10-13). Instead, a 512 MB host-visible
  buffer mirrors it at the same offsets, and before each draw its index
  range and the vertex buffers its vertex shader fetches from (the shader's
  vertex fetch constants: address in word 0, size in words in word 1 bits
  2-25) are copied in, each range once a frame. This copies static meshes
  every frame; tracking which ranges changed is for later. (Later: only
  ranges memcpy wrote since, see "Buffers"; since v0.3.8, with
  `--native_renderer_mirror_by_tick` and `--native_renderer_mirror_to_both`,
  on in `run.sh`, the mirrors take memcpy's writes at frame starts and
  draws don't copy at all, see "Hypothesis to test: the mirrors updated
  once a tick".)
- **Development mode first**: with `--native_renderer`, the replay hooks
  draw natively *and* still call the graphics library, so the emulated GPU
  keeps rendering the game's own window and the two can be compared side by
  side; the game stays playable throughout. Input goes to the game's
  window, not ours. An exclusive mode, which stops calling the library,
  comes at milestone 6.

## Milestones

| # | Milestone | Done when | Status |
|---|---|---|---|
| 1 | **Skeleton**: `--native_renderer` creates our own Vulkan device, window and swapchain; the replay hooks pass each frame's commands to it. | A window clears and presents every frame. | Done (2026-10-13): presents every frame in menus (30 fps) and races (113-128 fps, ~26,800 commands and ~2,900 draws a frame), the game unaffected. |
| 2 | **Main view geometry**: vertex and index buffers read from guest memory, shaders translated, render states mapped to pipelines. | Race geometry is visible in our window, untextured. | Done (2026-10-13): the cockpit view's geometry, the right way up, solid, with the game's coloured lighting on white placeholder textures (screenshot checked). |
| 3 | **Textures**: untile and upload (mostly BC1-BC3), samplers from the fetch constants. | The main view looks right without effects. | Done (2026-10-13): signs, gauges, fences, buildings and road textured correctly (screenshot checked); what comes from resolves is still missing. |
| 4 | **The other passes**: each pass renders into a Vulkan image of its resolve's size and format; resolves become copies. | Shadows, reflections and the mirror work. | Done (2026-10-14): every pass draws, inline vertex draws included; clears and resolves work; the user saw the car's shadow, the mirror and the reflections right in our window (cockpit and third person), and the resolves match the emulated GPU's (`--readback_resolve=full`), except its blocky bloom. Menus not checked. |
| 5 | **The final frame and the menus** (still in development mode): post-processing, HUD, the frame's end, then every menu screen. | Races and menus look as in the game's window, side by side. | In progress: the final frame with the HUD shows (checked by the user in a race); menus' tiled 3D views fixed. |
| 6 | **Exclusive mode**: the emulated GPU gets no work and then isn't loaded at all; the graphics library is kept for what our renderer needs from it (see "Plan for milestone 6"); then measure. | The GPU plugin isn't loaded; frame rate measured against the emulated GPU. | Done (2026-10-14): the plugin isn't loaded (`run.sh` runs exclusive mode); measured: 135 fps mean uncapped against the emulated GPU's 155, then ~235 after the speed work (step 5; holds the 144 cap in an attract race). The library's remaining packet writers moved to milestone 7 (the user's call); the frame's end (step 6) measured at ~0.2% of the render thread and left as it is. |
| 7 | **Known issues cleanup**: what's under Known issues below (the boot movie, an in-game movie locking up the game, the bloom's ghosting (fixed 2026-10-15), motion blur at the tick's length, gamma, textures replaced in memory, textures never freed, the library's remaining packet writers, further speed work on our renderer, the freezes while driving to re-examine, no window without Vulkan surface support). | The known issues are fixed or accepted. | |

## Progress on milestone 2

- After each draw's library call (`Renderer_AfterCommand`), the bound vertex
  and pixel shaders are read from the device (+12436, +12432), their
  microcode read through physical memory, and translated to SPIR-V
  (`src/native/shaders.cpp`, cached by hash and register count). In a run
  through the menus and attract mode: 76 shaders translated, none failed,
  every draw had its shaders. Their hashes match the plugin's dumps (whose
  `.bin` files hold the words byte-swapped to little-endian).
- `src/native/main_view.cpp` draws the main view (the render target with
  pitch 1040, 2x MSAA, format 12), command 18 with one item only, into a
  1280x720 RGBA16F target of ours, copied to the window at the end of the
  frame. Per draw: the register file is rebuilt from the device's shadow
  (`src/native/state.cpp`), the shaders translated, the constants written
  to a per-frame ring (system constants as the plugin computes them for
  what these draws use, the shader's float constants packed, bool/loop and
  fetch constants as is), a pipeline made from the registers (topology,
  primitive restart, cull, depth, blend of target 0, colour mask), and the
  draw recorded with the base vertex as a system constant (the shader adds
  it after byte-swapping the index). Textures are white 1x1 placeholders
  of the declared type. The scissor comes from the device (render state 200 at
  +11600, rectangle at +12416), not the registers: the window scissor
  registers aren't in the shadow's flushed blocks and read as zero (which
  first skipped every draw as invisible).
- First result (attract-mode race, 2026-10-13): ~950 main view draws a
  frame drawn, ~80 not yet supported (other draw commands, several items),
  41 pipelines, 39 shaders, no failures; ~15 MB a frame copied into the
  mirror; the game at 100-125 fps instead of 130-150 with the renderer on.
  The viewport comes out as 0,0 1024x576, as expected. Seen in the window: the
  dashboard, gauges, pillars, walls, overpass, lamps, buildings and road all
  in place and solid; the game's coloured light (magenta, green, blue) on
  the buildings and the opponent's red tail lights. The target was then cut
  to the view's 1024x576.

## Progress on milestone 3

- `src/native/textures.cpp` loads textures itself (not the SDK's texture
  cache): the fetch constant's layout (as the SDK reads it: mip range, base
  and mip addresses), the 360's tiled addressing, mips padded as levels of
  a power-of-two texture, the packed mip tail, rows and row counts padded
  to 32 blocks, layers to 4 KB; byte swaps per the fetch constant;
  `4_4_4_4` converted to RGBA8, DXT1/2_3/4_5 as BC1-BC3. 2D (and stacked)
  and cube only. Images are keyed by the fetch constant's data fields (not
  swizzle or sampling); a view per swizzle (the translator expects the
  swizzle in the view, with missing components repeating the last one).
  Uploads go through a 64 MB staging buffer per frame in flight, into a
  command buffer submitted before the frame's; a texture whose staging
  doesn't fit waits for the next frame. Loaded textures are reloaded when
  the library's memcpy writes their memory (since 2026-10-15; before, they
  were hashed again now and then: see `texture-handling-optimisation.md`).
- Samplers from the fetch constant (filters, clamp modes, border, mip
  range, anisotropy) with the shader's fetch instruction overriding the
  filters. The texture's post-swizzle signs go into the system constants;
  signed bindings get the unsigned view (no signed textures expected).
- Texture descriptor sets are allocated per draw from a per-frame pool,
  shared between draws with the same views and samplers.
- First result (race, 2026-10-13): 408 textures, 50.7 MB, loaded in the
  race's first frame without deferring, none reloaded; 18 samplers; ~101
  fps. Unsupported: `24_8_FLOAT` (the shadow depth map). About 2,200
  lookups a second find a texture of another dimension than the shader
  declares (now logged with an example). Still wrong, all for milestone 4:
  the mirror is black, and some buildings show the rainbow effect (not
  resolve targets, as guessed then: see "The rainbow effect").

## Plan for milestone 4

What's known is in `gpu-functions.md`: the passes of a race frame ("A race
frame's passes"), the command items ("Pass command items") and surface and
texture objects ("Resources"). The approach:

1. **Track the bound targets**: handle commands 23, 24, 29, 34/35 (or
   read device +12184.. and +12200 per draw, as the main view check does
   now) and draw every pass, not only the main view: a render target per
   distinct surface setup (EDRAM base, pitch, format, MSAA), with its
   depth. Begin and end dynamic rendering when the targets change.
   Pipelines then need the target's format and sample count in their key.
2. **Clears** (command 26) as attachment clears or load ops (decode the
   flags first).
3. **Resolves** (command 25): copy (or resolve, for MSAA) the source rect
   of the bound target into a Vulkan image of the destination texture's
   size and format, and register that image in the texture cache under the
   destination's base address, so later draws sampling that fetch constant
   get it instead of loading guest memory (which holds garbage: the
   emulated GPU doesn't write resolves back). Depth resolves
   (`24_8_FLOAT`) need a depth-to-colour copy (a small shader) into a
   format the shaders sample (R32_SFLOAT, as the translator expects for
   depth textures: check). Format conversions on resolve (7e3 to
   16_16_16_16, the `0xF4000000` flags' likely exponent bias) need
   checking against the emulated GPU's output.
4. The black mirror went once resolves fed the texture cache; the rainbow
   effect didn't (it was the vertex shader patching, see above).
5. The textures logged as "of another dimension than declared": car
   paint shaders (see "Progress on milestone 4").

## Progress on milestone 4

- `src/native/passes.cpp` (was `main_view.cpp`) draws every pass. A target
  per bound surface, keyed by colour or depth, EDRAM base, pitch, MSAA and
  the format drawn in (7e3 and its blending variant share RGBA16F; the
  fixed-point -32..32 formats are drawn as floats); sized to the pitch and
  as many rows as the EDRAM holds from its base. Bound surfaces are read
  from the surface objects (device +12184.., +12200; +24 surface info, +28
  colour or depth info). Render target slots 1-3 hold target 0's surface
  in the menus (not extra targets).
- **Clears** (command 26): flags decoded from the library (`sub_8229FC40`):
  bits 0-3 render targets 0-3, `0x10` Z, `0x20` stencil; colour is a
  D3DCOLOR (ARGB). The library clips the clear to the viewport and scissor;
  we clear the whole target (see "Clears' rectangles" below). Rects at +12
  (16 bytes each), not used.
- **Resolves** (command 25): the handler calls `Resolve(flags, src rect
  (+8, field bit 0), dest texture (+24, bit 1), dest point (+28, bit 2),
  level +36, slice +40, clear colour bytes +44 (bit 3), Z +48, stencil +52,
  parameters +56 (bit 4))`, the XDK's argument order. Flags as the XDK's
  D3DRESOLVE: bits 0-1 render target, `0x4` depth, `0x100` clear the
  target after, `0x200` clear depth and stencil, bits 26-31 a signed
  exponent bias (`0xF4000000` = -3). No other bits seen in a race. Drawn
  with our shaders (`src/native/glsl/`, compiled to checked-in headers by
  `compile.sh`): samples averaged, depth as R32_SFLOAT, scaled by the
  bias, into an image per destination texture (keyed by base address),
  which the texture cache hands out instead of guest memory.
- What the resolves are, from dumps (2026-10-13): 0-2 the 512x512
  projected texture (empty: its draws aren't drawn, see below); 3 the
  1024x1024 cleared one (zero); 4 a depth map seen from low on the car,
  looking down the road (the headlights' shadow map, likely); 5 a second
  depth map; 6 the mirror (rear view, right); 7 the paint's reflection map
  (a road view, right); 8 **the minimap's road outline** (not "processing
  7" as guessed in `gpu-functions.md`); 9 the 16_16 pass; 10-23 the HDR
  copy and bloom chain (bloom empty: drawn with inline vertices); 24 the
  1280x720 frame. The post-processing destinations sit at different heap
  addresses each race.
- Shaders are now translated per vertex/pixel pair with the interpolators
  the pair connects and PsParamGen, as the plugin's pipeline cache does;
  with all 16 on, a pixel shader reading one its vertex shader doesn't
  write got undefined values. Depth bias (render states 204/208: registers
  `0x2380`-`0x2383` at device +10704, now read) is set per draw as the
  plugin sets it (~110-130 draws a frame use it).
- **Inline vertex draws** (commands 15 and 19, 2026-10-14; see "Inline
  vertex draws" in `gpu-functions.md`): BeginVertices and End are wrapped
  (`replay_hooks.cpp`), and each draw is drawn at End
  (`Passes::InlineDraw`), when the vertices are in place and the shadow
  holds its state (command 15 changes render state 304 between items, so
  after the command would be too late). The vertices are copied into the
  mirror past guest memory (each mirror is 512 MB plus built-in index
  buffers plus 4 MB of inline vertices), and vertex fetch constant 95 is
  pointed there (the translator doesn't mask the address), with 93-94 and
  VGT_INDX_OFFSET as BeginVertices writes them. Rectangle lists use the
  translator's vertex shader expansion (`kRectangleListAsTriangleStrip`:
  each invocation runs the guest shader for the rectangle's 3 vertices and
  makes the 4th) drawn as two-triangle strips with a built-in index
  buffer, as the plugin's primitive processor does without a geometry
  shader; quad lists use a built-in index buffer as triangle lists
  (v0 v1 v2, v0 v2 v3). ~100-140 inline draws a frame in a race; no draws
  unsupported any more. Compared with the emulated GPU's resolves
  (`--readback_resolve=full`, see Tools): the projected texture (resolves
  0-2, the underglow), the 1024x1024 one (3), the paint reflection map
  and the 16_16 pass match; the bloom chain (11-22) is smooth in ours and
  blocky in the emulated GPU's, and the user judged ours (and our motion
  blur) the more correct.
- The minimap's player triangle comes from the second command table (see
  "Single commands" in `gpu-functions.md`): its helper draw
  (`sub_8258F5C0`) is wrapped as command 15, and drawn since.
- **Red and blue swapped in resolves** (fixed 2026-10-14): the mirror,
  the paint reflections and the minimap triangle showed red and blue
  exchanged (an orange lane line blue in the mirror; seen by the user side
  by side with the game's window). The library's Resolve (`sub_822AFA58`)
  writes RB_COPY_DEST_INFO straight into the ring, not the shadow
  (endian | format << 7 | number << 13 | exponent bias << 16 | swap <<
  24), and sets copy_dest_swap when the destination texture's swizzle
  takes X from another component than the first (fetch word 3 bits 1-3):
  memory holds the texture as BGRA and its swizzle swaps it back when
  sampled. Our resolve shader now swaps red and blue then too. (Earlier
  this was put down to the read-back copy, wrongly: the read-back data is
  stored swapped, as the GPU stores it, and ours now matches it.) The
  resolve's format remap, for reference: destination 54 → colour format 7,
  55 → 16, 56 → 17, 27 → 30, 28 → 31, 29 → 32, 22 and 23 → 6.
- **Clears' rectangles** (2026-10-14): in a race no clear has rects, and
  every clear's viewport starts at 0,0 and covers the pass's drawn area
  (1280x720, 1024x576, 512x512, 1024x1024, 256x256, 512x288; the bloom
  target's 8192x8192, i.e. unbounded) with the scissor off. So clipping
  to them would clear only what's drawn anyway, and clearing the whole
  target is kept. Menus not checked.
- **Textures of another dimension than declared** (2026-10-14): the
  ~800-1,800 lookups per 5 s are car paint pixel shaders
  (`919CB09ADFA6FA77`, `7ECFFC514C780F01`, main view, one to eight draws
  a frame) declaring a cube map at fetch constant 6 while it holds the 2D
  256x256 paint reflection map (resolve destination `1EC39000`). (The
  earlier example, a DXT1 cube in a 2D slot, wasn't seen again.) The
  plugin binds a null descriptor for a dimension it can't use (a 2D fetch
  takes 1D, 2D or 3D, a cube fetch only a cube), which reads as zero; we
  now bind zero (transparent black) placeholders for these too, instead of
  the white ones kept for textures that can't be loaded. Hardly visible:
  in 14 snapshots of an attract race those draws wrote 37 pixels in one
  and 2 in another. The log names each shader and texture once
  ("dimension mismatch").
- Render targets 1-3 and resolves to a level or slice other than 0: none
  in a race (the per-5 s log shows 0 draws with targets 1-3 bound to
  another surface than 0, and 0 resolves skipped; 2026-10-14), so nothing
  to do for races. Menus not checked.

### The rainbow effect (fixed)

Buildings, pillars and overpasses showed smooth rainbow gradients (and
green or red tints) instead of their own colours, on and off; the game's own
window showed them right. **Fixed 2026-10-14 (`88f2c28`)**: those draws ran
a vertex shader patched for another vertex declaration.

- The library patches a vertex shader's vertex fetch instructions for the
  bound declaration (see "Vertex declaration" in `gpu-functions.md`). When
  the GPU may still be using the shader's microcode, it can't patch it in
  place: `sub_822AEBF0` copies the microcode into the command buffer
  (IM_LOAD_IMMEDIATE) and patches the copy. The object's own microcode
  (+48) stays patched for the previous declaration, and +24 still names
  that one. We translated the object's microcode, so ~100-400 draws a
  frame fetched vertices with another declaration's layout: in the case
  looked at, the colour and texture coordinate stream (12 bytes a vertex)
  read with an 8-byte stride, giving garbage vertex colours and texture
  coordinates.
- Now `sub_822AE340` (the patcher) is wrapped: when it patches a copy, the
  patched bytes are kept with the shader object and the declaration id, and
  a draw whose object is patched for another declaration than the bound
  one uses them (`g_loaded_vertex_shader`, `replay_hooks.cpp`). The log
  counts these draws, and logs draws still mismatched (none since).
- Why it depended on timing: the copy is made only while the emulated GPU
  hasn't finished with the shader, so frames recorded slowly (dumps,
  `--native_renderer_serialize`) mostly patched in place, and looked right.
- The same fix ended the draws reading vertices past their fetch
  constant's size (25-70 a frame): those were wrong strides too.
- Misleading leads, for the record: leaving out pixel shader `F3C0D413`
  (the cockpit's gauges) with `--native_renderer_cycle` seemed to remove
  the effect, but that was a single frame of an effect that comes and goes;
  attract races (no cockpit) showed it all the same. The colour write mask
  (now normalized as the plugin does it), PsParamGen (no draw uses it), the
  base vertex and index range, mirror copies overwriting each other
  (`--native_renderer_check_clobbers`: none) and stale textures
  (`--native_renderer_verify_textures`: no change) were all ruled out on
  the way. What found it: periodic dumps at full speed
  (`--native_renderer_dump_every`) with the draw IDs naming the draws;
  `--native_renderer_base_level_only` showing the gradients didn't come from
  the texture; then the shader dumps showing the same program with
  different vertex fetch strides on one vertex buffer.

## Progress on milestone 5

Restructured on 2026-10-14 at the user's request: the menus moved into
milestone 5, and milestone 6 is now the full disconnect from the emulated
GPU (exclusive mode) and the frame rate measurement.

- **The final frame is shown** (2026-10-14): `sub_8258D998` (the front
  buffer resolve at the frame's end, outside the command list) is wrapped
  (`replay_hooks.cpp`) and the window shows what render target 0 holds
  then (`Passes::FrontBufferResolve`), instead of the main view. Logged
  in a race: `Resolve(device, flags 0, no rect, front buffer, no point,
  level 0, slice 0)` from render target 0 = the pitch 1280, no-MSAA,
  8_8_8_8 surface at EDRAM base 0, into a 1280x720 8_8_8_8 (format 6)
  front buffer. The device is the video output's (+140, `40021B00`);
  the device pointer at `0x8296BD90` reads 0 there, so it seems to be set
  only during the replay. Frames without a front buffer resolve still
  show the main view.
- The last pass (saved targets, viewport 1280x720) in an attract race is
  two inline strips: the upscale (sampling the 1280x720 format 54
  resolve destination, the view in its top-left 1024x576) and the
  "Import Tuner Challenge" logo (a 208x128 texture). Attract races have
  no HUD; the greyscale in some attract shots is the attract mode's own
  effect (the user confirmed).
- **Checked by the user in a free run race (2026-10-14, cockpit view,
  side by side):** the whole HUD shows in our window as in the game's
  (minimap with its triangle and place name, mirror, gauges, gear, speed,
  ASSIST bar); the only difference seen is the known gamma (ours lighter).
  81-98 fps in the race with the renderer in development mode.
- **Predicated tiling** (2026-10-14; see "Predicated tiling" in
  `gpu-functions.md`): the menus' 3D views were missing (black) because
  they're drawn with predicated tiling, whose resolves (command 33) we
  didn't make, into a surface too tall for its EDRAM-sized target. Found
  by logging the library's draws and resolves made outside the commands
  that usually make them (`replay_hooks.cpp`, once per caller): Resolve
  from command 33. Now targets made while tiling (between commands 32 and
  33) are keyed apart and as large as the tiles together (1280x720), are
  cleared at command 32 with its clear values, and the library's Resolve
  is wrapped: in command 33 each tile's resolve is made from its
  arguments, with the clears after it only on the last tile (we draw the
  whole surface at once). The user saw the menus right after this. Races
  unaffected (same snapshots).
- **The boot movie isn't drawn** (left for later, the user's call,
  2026-10-14): see Known issues.
- Seen on the way (fixed above): in the menus our window drew the 2D
  interface right but not the 3D car on the Free Run screen (car, floor
  reflection, light beams) nor the Course In screen's night city picture
  (black in ours). The menus also log ~6,000 lookups per 5 s of a texture
  of another dimension than declared.

## Plan for milestone 6

Agreed with the user on 2026-10-14. **Kept**: the game's graphics library
(the Xbox 360's Direct3D, recompiled guest code, not the SDK): its shadow
is what we translate, and it does work we'd otherwise rewrite (combining
SQ_PROGRAM_CNTL, patching vertex shaders for their declaration, inline
vertex space). The SDK's shader translator and runtime also stay.
**Dropped**: the emulated GPU (`librexgpu-xenos.so`), not loaded at all by
the milestone's end.

What's known of how the library reaches the GPU (2026-10-14, from the
generated code):

- **The kick**: `sub_8229E3F0(device, pairs, count)` writes an
  INDIRECT_BUFFER packet (`0xC0013F00`, address, size) per (address,
  size) pair into the primary ring (base device +13496, size mask +13500,
  write position +10820, waiting for room with `sub_8229DCC0`), then
  writes CP_RB_WPTR (`0x7FC80714`). That write is the only place the
  emulated GPU is handed work. (With +10809 bit 1 set it calls an object
  at +19980 instead: not looked at.)
- **The GPU's progress** is a block the GPU writes, pointed to by device
  +10768, read by every wait: +0 against the command buffer position
  +10780 (`sub_8229E908`, the ring space wait, 46% of the render thread);
  +4, a counter in `sub_8229DC28` (likely swaps); +60 the primary ring's
  read pointer (`sub_8229DCC0`).
- **VdSwap** (the SDK's kernel, `xboxkrnl_video.cpp`) only writes a fetch
  constant and an XE_SWAP packet into the ring; the swap happens when the
  GPU processes it.

Steps (reordered 2026-10-14, the user's call: dropping the plugin and
measuring come before the speed work):

1. **Log first, with the GPU running**: at each kick, walk the indirect
   buffers and log the packets with effects outside the GPU (MEM_WRITE,
   EVENT_WRITE*, COND_WRITE, REG_TO_MEM, WAIT_REG_MEM, INTERRUPT,
   XE_SWAP, VIZ_QUERY), and how the progress block changes over a frame.
   Find what clears device +10868 (the per-frame full wait
   `sub_8229F1B0` spins on it; likely an interrupt callback), where vblank
   interrupts come from, and whether the game waits on query results.
2. **Exclusive mode, step 1** (`--native_renderer_exclusive`): replace the
   kick. CP_RB_WPTR isn't written; the effects step 1 finds are made by us,
   and the progress block is set so the library sees everything consumed
   at once. Everything else the library does stays, so the renderer is
   unchanged; with every fence passed, vertex shaders get patched in place
   (the patched-copy path should go quiet). Measure.
3. **No GPU plugin**: supply what it still provides (likely vblank
   interrupts and swap completion) from a timer and our present, and run
   without loading it (find out whether the SDK runs with no plugin or
   needs a stub).
4. **Check** the menus and predicated tiling, then measure with
   `--log_fps` and `perf` at the same moments of an attract race: the
   emulated GPU alone, development mode, exclusive mode.
5. **Step 2**: from a profile of the render thread, skip the library's
   work that only writes packets (the draws' state flush `sub_822ADE30`
   and friends, Resolve's and Clear's packets, the swap), keeping what
   fills the shadow and the four things above (SQ_PROGRAM_CNTL, shader
   patching, inline vertex space, fences).
6. **The frame's end**: the front buffer resolve stays (it's what's shown);
   the full GPU wait and the swap become no-ops, once it's checked that
   nothing waits on the swap counter (+4). **Not done**: measured at
   ~0.2% of the render thread, so left as it is (see "The frame's end,
   measured" below).

Risks: interrupts the game relies on, and occlusion queries it reads back;
step 1's logs are there to find them before anything is stubbed.

## Progress on milestone 6

- **What the GPU does outside itself** (step 1, 2026-10-14,
  `--native_renderer_log_gpu`, `src/native/gpu_link.cpp`; boot and an
  attract race):
  - The library kicks ~45,000 times a second (one buffer each, ~500 words
    on average). Every segment ends with two EVENT_WRITE_SHD packets
    (initiator 3): the fence (the command position before the kick) to
    progress block +0, and the command buffer address the GPU reached
    (`BFxxxxxx`, a virtual alias) to +4. The block is at `FFC93000`
    (physical `1FC94000`); +60 is the ring's read pointer write-back
    (`VdEnableRingBufferRPtrWriteBack`, in `sub_822AD190`).
  - Every other kick is 11 words: COHER_STATUS/SIZE/BASE (`0A31`, `0A2F`,
    `0A30`) for the command buffer range `1FB90000` (1 MB) and a
    WAIT_REG_MEM on COHER_STATUS_HOST: making the GPU see the CPU's
    writes. Also in the stream: WAIT_UNTIL (`05C8`), cache flush events
    (EVENT_WRITE 6 and 15), `0D00`, the binning packets (`60`-`63`),
    XE_SWAP with display registers (`1844`, `0F01`). None writes memory.
  - At boot only: the scratch registers' set-up (SCRATCH_ADDR `1FC93000`,
    which is the interrupt callback's block, device +10772 → `FFC92000`;
    SCRATCH_UMSK `0x33`), the display's gamma table (COND_WRITE on `1925`,
    registers `19xx`), and one inserted callback: scratch 4 and 5 =
    `82481B68`, `400D1DA0`, scratch 0 = 2, WAIT_REG_MEM on its write-back,
    INTERRUPT (CPU mask 2), wait for 0, scratch 4 = `0BADF00D`. The
    library's interrupt callback (`sub_8229DEA0`, source 1) calls the
    function in callback block +16 with +20 and clears its CPU's bit in
    +0.
  - Vblank interrupts (source 0, 60 a second) come from the SDK's vsync
    thread, not the command stream: `sub_8229DEA0` counts them (+15148),
    counts down +15156 (zeroing callback block +4 at 0) and calls a
    callback at +15144 (set by `sub_824F0030`, in the movie code).
  - No MEM_WRITE, REG_TO_MEM, queries (EVENT_WRITE_ZPD, VIZ_QUERY) or
    screen extents in a race. Device +10868 (the full wait's flag) stayed
    0: it counts deferred kicks (`sub_8229E848`). `sub_822ACAA8` is the
    library's "GPU hung" handler (after a wait times out: sets +10809 bit
    1, progress = position − 2).
- **Exclusive mode, step 1** (`--native_renderer_exclusive`, 2026-10-14):
  the kick (`sub_8229E3F0`) no longer calls the library's: it walks the
  buffers and makes their effects itself, as the SDK's command processor
  would (EVENT_WRITE_SHD, MEM_WRITE, REG_TO_MEM, COND_WRITE, REG_RMW, the
  scratch registers' write-back, INTERRUPT by calling the callback block's
  function), and counts what it doesn't handle (logged every 5 s).
  CP_RB_WPTR is never written, so the emulated GPU gets nothing (its
  window stays black); the primary ring stays empty, so +60 needs no
  update. First run: no hang, the boot callback made, nothing unhandled,
  every draw drawn; attract race frames look right (the sepia and
  greyscale shots are the attract mode's, the user confirmed).
- **Measured** (2026-10-14, the same 46 s of an attract race,
  `--max_fps=144`, no dumps or extra logging; fps per second of the
  race):

  | Setup | Mean | 10th percentile | Median | Max |
  |---|---|---|---|---|
  | Emulated GPU alone (no `--native_renderer`) | 141 | 123 | 147 | 151 |
  | Development mode | 87 | 73 | 90 | 112 |
  | Exclusive mode, step 1 | 127 | 104 | 128 | 149 |

  So exclusive mode is still **slower than the emulated GPU alone** (which
  sits at the cap much of the time; an uncapped comparison is still to be
  made). With the waits gone, the render thread does two jobs: the game's
  Direct3D still writes every GPU packet (~45,000 kicks a second, now
  read by nothing but our kick's walk), and our renderer's own work per
  draw (e.g. ~15 MB of vertex data copied into the mirror every frame).
  Neither is profiled yet. (An uncapped try failed: `--max_fps` given
  twice, and the log then said "no GPU emulation loaded (gpu_plugin not
  set)" despite `--gpu_plugin=xenos`; the game sat at boot with a window
  open. Likely the repeated flag broke the command line; not checked.)
- **What still depends on the SDK's GPU plugin** in exclusive mode: its
  window (the SDK app's window, black now, and the one that gets input:
  the user found two windows confusing, rightly) and the vblank
  interrupts from its vsync thread (`GraphicsSystem::MarkVblank`). Also
  the MMIO registers at `0x7FC80000` (CP_RB_WPTR `0x714`, interrupt
  status `0x6544` read by the callback, `0x6110` written by Swap, others
  read by `sub_822A0688`) are mapped by the plugin's GraphicsSystem
  (`AddVirtualMappedRange`). Without the plugin the kernel ignores
  `VdSetGraphicsInterruptCallback`, `VdInitializeRingBuffer` and
  `VdEnableRingBufferRPtrWriteBack`.
- **The SDK's "detached" mode** (`rexglue-sdk/src/ui/rex_app.cpp`,
  around line 360): with no graphics system, ReXApp still makes its window
  (`rex::ui::Window`, an SDL window: `WindowSDL::sdl_window_`) and calls
  `OnCreateImmediateDrawer()`, leaving the surface and the present cadence
  to the app. Plan (agreed direction, 2026-10-14): our renderer draws into
  that window instead of opening its own, so there's one window and input
  works as it does now.

- **One window, ours** (2026-10-14, `src/native/app_window.cpp`; the
  user's call: our renderer's window stays, the SDK's goes, rather than
  drawing into the SDK's "detached" window). ReXApp always opens its
  window, and the SDK's input (the keyboard driver of `--mnk_mode`, the
  binds) and its close path hang off that window object. In exclusive mode
  `ItcApp::SetupPresentation` hides it (`SDL_GetWindows`: it's the only
  window then), and when our window is made it's registered in the SDK's
  event table (`SDLWindowedAppContext::RegisterWindow`) against the SDK's
  window object, so our window's keyboard, focus and close events reach the
  SDK as before. Mouse capture would act on the SDK's own window, but the
  game has no mouse controls (`--mnk_mouse` is off); gamepads come from
  SDL events whatever the window. No SDK patch. Checked by the user:
  keyboard, gamepad, one window.
- **Interrupts are made at the library's level, not the GPU's.** Running
  the command stream's INTERRUPT callbacks inside the kick hung loading
  screens. EndTiling (`sub_825BF200`) ends with `sub_822B6BF8`, which
  inserts the callback `sub_822B6A78`: it wakes the worker thread of the
  CPU it runs on (`sub_822B69A8`, event at device +11244 + 56 × CPU, CPU
  from PCR +268), which submits the library's **deferred kicks**:
  `sub_8229E848(device, r4, address, size, interrupts, list)` queues a
  buffer on the list at device +13036 while device +10868 (interrupts
  still to come from the GPU) isn't 0, and otherwise adds to +10868 and
  kicks. Calling the callback on the render thread first woke the wrong
  CPU's worker (the PCR's CPU is now set to the packet's for the call);
  with that fixed, the worker kicked at once and raced the render thread:
  fences went backwards and +10868 dropped below 0, so the full wait spun
  forever. Our GPU never lags, so in exclusive mode `sub_8229E848` now
  kicks at once and leaves +10868 at 0, and the worker's wake is skipped
  (counted as "deferred kick worker wakes skipped"). The user went through
  loading screens and raced (2026-10-14) and found the rendering "more
  faithful to the real thing" overall. `--native_renderer_log_gpu` also
  logs the tiling worker's calls and kicks from threads other than the
  render thread. The approach (the user's direction): remove what makes
  the library need a GPU, rather than imitate the GPU's timing.
- **The boot stall without the plugin comes from `--skip_intro`**, not
  from the plugin's absence (2026-10-14). It happened in about 1 boot in 6
  to 1 in 25, always right after the first front buffer resolve. A stalled
  run's game threads all sleep, so it's a wait, not a spin. gdb showed:
  - the render thread inside the boot movie's start (the replay handler
    `sub_8258E9B8` → `sub_820AE958`, which starts the 1280x720 movie
    through `sub_82481A70`), asleep in the movie player's loop
    `sub_82491C50`. That loop waits with no timeout on a kernel event at
    player +84 or +132 (`sub_824909A0`, `sub_824909E8`), set by
    `sub_82490A60` and `sub_82490A90`;
  - the movie's decoder threads asleep (`sub_824908A0`, `sub_824908D0`);
  - the main thread already in `Boot::EndMovie` (`sub_82107BF8` →
    `sub_820AE528` → `sub_8258D8C8`), waiting on the render thread.

  `--skip_intro` makes the movie step report the movie finished at once,
  so the movie is stopped while it's still starting. The user's call: it's
  fine without `--skip_intro`, so players aren't affected and nothing is
  changed. For automated runs, a boot that never shows "called from
  8248466C" in the log has stalled; restart it. (The vblank interrupts
  and the MMIO range at `0x7FC80000`, the earlier candidates, weren't
  needed to explain it.)

- **No GPU plugin** (step 3, 2026-10-14): `run.sh` (and so `run-debug.sh`)
  runs `--native_renderer=true --native_renderer_exclusive=true` with no
  `--gpu_plugin`, and the zips (CI's and the local ones) leave out
  `librexgpu-xenos.so`; the game doesn't link it, it was only loaded by
  `--gpu_plugin`. Nothing had to stand in for the plugin: no vblank
  interrupts and no GPU registers at `0x7FC80000` are needed for boot,
  menus or races. A packaged zip run without `--skip_intro` played the
  boot movie, the menus and an attract race.
- **Measured without the plugin** (step 4, 2026-10-14): the same 45 s of
  an attract race (from the second after the race starts), fps per
  second, `--skip_intro --attract_after_s=1`, no extra logging:

  | Setup | Cap | Mean | 10th percentile | Median | Min | Max |
  |---|---|---|---|---|---|---|
  | Emulated GPU alone (`--gpu_plugin=xenos`) | 144 | 143 | 127 | 149 | 122 | 151 |
  | Exclusive mode, no plugin | 144 | 129 | 104 | 133 | 100 | 149 |
  | Emulated GPU alone | none | 155 | 126 | 159 | 121 | 191 |
  | Exclusive mode, no plugin | none | 135 | 106 | 134 | 101 | 184 |

  Our renderer is about 13% slower on average, and about 18% slower in the
  busy seconds (the 10th percentile, 106 against 126 uncapped). It matches
  the earlier measurement with the plugin loaded (127 against 141), so
  loading the plugin wasn't costing anything. Step 5 is the speed work.
  The user then went through menus, loading screens (predicated tiling)
  and a race from the packaged zip with no plugin: all fine, no errors in
  the log. Steps 3 and 4 are done.

- **Speed, first changes** (step 5, 2026-10-14; uncapped attract race, mean
  and 10th percentile of fps per second; one run each). A profile of the
  render thread (`perf record -e cycles:u --call-graph dwarf`) showed our
  renderer's per-draw work, not the library, as most of it (`AfterDraw`
  71%):

  | Change | Mean | 10th pct |
  |---|---|---|
  | Before (emulated GPU alone: 155 / 126) | 135 | 106 |
  | Index range scan as one vectorised loop per format (was ~20%; checked equal on 10 million draws) | 167 | 130 |
  | Register file cleared once, not 80 KB a draw (was ~10%) | 182 | 144 |
  | Texture set layouts cached per shader; texture set key and infos reuse their buffers | 190 | 149 |
  | Shader caches as hash maps instead of ordered maps of tuples | 198 | 156 |
  | Mirror copies only ranges memcpy wrote since (see "Buffers") | 215 | 170 |

  At `--max_fps=144` the last holds the cap through the race (mean 150,
  slowest second 145).

  What's left (second profile): the mirror copies ~12%, texture sets ~9%
  (string keys), our blending ~8%, `Draw` itself ~8%, shader lookups ~4%
  (see Known issues), the guest's global lock in the library ~3.5%.
- **What the mirror copies** (step 5, 2026-10-14,
  `--native_renderer_check_mirror`, an uncapped attract race, 55 s): of the
  13-19 MB copied a frame (vertex buffers 11-17 MB, the rest indices),
  **nothing changed between frames**: 0 bytes a frame copied with contents
  different from that range's previous copy, in every 5 s window, and
  under 0.01 MB a frame of ranges never seen before (ranges grew from
  1,301 to 2,643 as the race went on: track sections streamed in at new
  addresses or sizes). Dynamic geometry goes through inline vertex draws
  (BeginVertices), which are copied separately, not through these ranges.
  Copying only each draw's vertices (lowest to highest) instead of the
  whole buffer would save only 1-8%. So almost all of the mirror's copying
  (~12% of the render thread) is redundant; what's missing is a cheap way
  to know when a range's memory is rewritten (streaming reuses memory).
  Only an attract race was measured.
- **The library's share of the render thread** (step 5, 2026-10-14, after
  the mirror change; uncapped attract race, 4 s profiled 20 s in, ~230
  fps). Each sample is given to the innermost frame that is guest code or
  ours (libc and driver time goes to its caller): **our renderer 72%**,
  the replay handlers (`0x8258xxxx`-`0x8259xxxx`) 9%, **the game's
  Direct3D 19%**. The library's part, by what the replay calls:

  | Library call | Share | What it is |
  |---|---|---|
  | `sub_825BE708` (DrawIndexedVertices) | 10.4% | Constants into packets (`sub_822AE1D0`, 3.1%: copies the dirty constants' 64-byte blocks), state flush (`sub_822ADE30`, 1.9%), the DRAW_INDX packet itself (1.9%), shader loads (`sub_822AED18`, 1.3%), `sub_8229EF48` 0.8%, `sub_822ADC08` 0.4%, fetch constants (`sub_822AE070`, 0.4%) |
  | `sub_822A8808` (SetVertexShader) | 6.0% | Almost all the shader's AddRef `sub_822A6738` (4.6%) and Release `sub_822A67B0` (1.2%): an atomic add on the reference count (+4), which the recompiler wraps in a process-wide mutex (its `mfmsr`/`mtmsrd` become `REX_ENTER/LEAVE_GLOBAL_LOCK`). Not packet writing. |
  | Pixel shader constants, SetIndices, SetTexture, SetPixelShader | ~2% together | |
  | Inline draws (`sub_825A17A0`) | 0.5% | (1.8% with our `Renderer_InlineDraw` inside End) |
  | The kick, the ring space wait | ~0.2% | |

  So skipping the packet writing could save at most ~10%, nearly all of it
  inside the indexed draw; the AddRef/Release mutex is a separate ~6%.
- **The indexed draw without its packets** (step 5, 2026-10-14,
  `draw_packets.cpp`). DrawIndexedVertices (`sub_825BE708`) works through
  five 64-bit dirty masks at device +0 (vertex constants), +8 (pixel
  constants), +16 (shaders, fetch constants, some registers), +24 and +32
  (register blocks), calling a helper per set bit, clears them, then writes
  DRAW_INDX. From the code, only the shader load `sub_822AED18` writes
  more than packets: the shadow (SQ_PROGRAM_CNTL and +10404 at +10400,
  +10408, +10432, +10452, +12440), vertex shader patching, the shaders'
  fences (+8), and it returns more dirty bits. The draw itself also sets
  device +11584 when +24 bit 1 is dirty, and in predicated tiling records
  each DRAW_INDX's address in a list at +12976 for EndTiling.
  `--native_renderer_check_draw_writes` confirmed it (a 45 s attract race,
  device snapshot around each call): `sub_822AE1D0`, `sub_822ADE30`,
  `sub_822ADEF0`, `sub_822AE070` and `sub_822ADC08` change no device field
  but the command pointer (+40); the draw without a shader load changes
  only the masks; no predicated draws in a race. (+13564-+13572 changed
  under every helper a few times in 5 s, of a million calls: another
  thread.)

  So in exclusive mode the draw now calls only the shader load and clears
  the masks (predicated draws keep the library's draw). First result: the
  same draws, 8 times fewer kicks, but **draws using the library's patched
  shader copy rose from 9 to 136 a frame**: a shader's fence is the command
  position it was loaded at, and the library patches in place only once
  the GPU has passed it; with so few kicks the position stays unkicked
  longer. Nothing reads the shaders behind the library, so the bound
  shaders' fences are cleared before the shader load, and copies went to
  0. Uncapped attract race, same build:

  | Setup | Mean | 10th pct | Median | Min |
  |---|---|---|---|---|
  | Library's packets (`--native_renderer_library_packets`) | 209 | 165 | 209 | 159 |
  | Draw without packets | 219 | 172 | 222 | 167 |
  | ... and shader fences cleared | 232 | 181 | 233 | 176 |

  At `--max_fps=144`: mean 150, slowest second 147. Kicks went from ~100,000
  to ~9,000 a second. Frames dumped at the same moments
  (`--native_renderer_dump_every`) had identical `draws.txt` (shaders,
  render state, fetch constants of every draw); the pixels differ only by
  the interpolated camera's position (edges everywhere, from frame
  timing).
- **The shader reference count** (step 5, 2026-10-14, `refcount.cpp`).
  The library's AddRef `sub_822A6738` and Release `sub_822A67B0` are a
  lwarx/stwcx loop on the object's count at +4; the recompiler turns the
  `mfmsr`/`mtmsrd` around it into the process-wide global lock, taken
  twice per pass (`REX_CHECK_GLOBAL_LOCK` locks it as well as
  `REX_ENTER_GLOBAL_LOCK`). Profile before (shares of the render thread's
  samples with a usable stack, about half of them; same method as above):
  AddRef 5.5% (two thirds waiting on the mutex), Release 1.0%. AddRef has
  7 callers, SetVertexShader's `sub_822A8808` the hot one; Release ~40,
  many in game code on other threads. Release at 0 releases +24 (type 4
  objects with +0 bit `0x40000000`, as AddRef does at 1), calls
  `sub_822A6210` on the object (likely its destroy) and returns 0.

  Safe without the lock, from the code: the stwcx is already a
  compare-and-swap on the guest word, and no global-lock region anywhere
  in `generated/` does a plain store (the only regions without lwarx are
  two 64-bit range trackers on other data, `sub_8229DE48` and
  `sub_8229DDD8`); the SDK's own users of the lock (filesystem, kernel
  objects, memory) don't touch the library's objects; the library's other
  atomics on these objects (`sub_822A63F8`, `sub_822A6630`, +-256) are on
  +0. So both are overridden with the same loop as a native
  compare-and-swap, behaviour otherwise kept.

  That took the mutex out of the profile but gained only ~2%: the locked
  instruction itself costs a few hundred cycles on the render thread
  (perf puts almost all of Release's samples right after `lock cmpxchg`).
  SetVertexShader AddRefs the new shader and Releases the bound one
  (device +12436) even when they're the same, which **52% of its ~300,000
  calls a second** are (logged every 5 s); the pair then changes nothing,
  and the device's own reference keeps the count above 0, so both are
  skipped (a thread-local "unchanged shader" the two overrides check;
  the rest of SetVertexShader runs as before). A first version counted
  those calls with shared atomic counters, which alone cost 5%: counters
  on this thread are kept per thread now.

  Uncapped attract race, alternating runs of the same build:

  | Setup | Mean | 10th pct | Median | Min |
  |---|---|---|---|---|
  | Global lock (`--refcount_global_lock=true`), run 1 | 222 | 172 | 223 | 168 |
  | Global lock, run 2 | 233 | 185 | 234 | 180 |
  | Lock-free, same shader skipped, run 1 | 234 | 184 | 227 | 179 |
  | Lock-free, same shader skipped, run 2 | 241 | 191 | 242 | 185 |

  About +4%, within run-to-run noise of ~5 fps. AddRef, Release and
  SetVertexShader together went from ~6.5% of the render thread to ~1%;
  the library's share from 12% to 9%. Not measured at 144 (already held).
  Frames dumped every 10 s (`--native_renderer_dump_every`) with the lock
  and without: `draws.txt` identical once the addresses of dynamically
  allocated textures and vertex buffers (`0x1Bxxxxxx`) are masked; two
  runs of the same build differ more (they differ in those addresses
  too). The user saw no visual errors.
- **The frame's end, measured** (step 6, 2026-10-14, on `e6d1d50`;
  uncapped attract race, 4 s of the render thread profiled 20 s in, ~210
  fps; inclusive shares, nearly all samples had a usable stack): the
  front buffer listener `sub_82091BC0` 0.06% (the full GPU wait
  `sub_8229F1B0` 0.04%, the front buffer resolve 0.03%), Present
  `sub_82091A60` 0.13% (`sub_822A8A60` 0.05%, Swap `sub_822A08A0` 0.06%
  with `VdSwap` too small to show, the unbinding `sub_8258D9F8` 0.01%).
  `--log_frame_times` puts Present under 0.01 ms a frame on average (the
  frame takes ~4.5 ms), so nothing there blocks either: in exclusive mode
  the full wait passes at once (our kick shows everything consumed and
  +10868 stays 0). Making them no-ops would save ~8 µs a frame, below
  run-to-run noise, while Swap does more than `VdSwap`: it records fences
  at device +13540 and +13544 (the position at the swap and the previous
  one) and calls `sub_822B5C58` (with the front buffer), `sub_822B2388`,
  `sub_8229EC00` (with +13512), `sub_822B20D8` and `sub_822A3B90`, none
  identified, so stubbing it would need what reads those worked out first.
  Left as it is (agreed with the user, 2026-10-14); the "swap counter"
  question (progress block +4) wasn't pursued. That ends milestone 6.

## Buffers (where vertex and index data comes from)

Found 2026-10-14 with `--native_renderer_log_buffers` (`buffers.cpp`), in
uncapped attract races of 70 s, to replace the per-frame mirror copies
with uploads when the game fills its buffers (the user's direction: draw
from buffers made like a regular program's, not from a copy of guest
memory).

- **Buffer objects** are Xbox 360 Direct3D resource headers in the game's
  heap (`0x04xxxxxx`-`0x0Axxxxxx`): +0 type (`00000001` vertex,
  `20000002` index: 16-bit, 8-in-16), +24 and +28 the GPU's own words:
  for a vertex buffer the vertex fetch constant's two words (address with
  type 3 in the low bits, then `0x10000000 | size in words << 2 | endian`),
  for an index buffer the address and the size in bytes. Data addresses
  are in the physical views (`F7xxxxxx`-`FCxxxxxx`). Draws bind them with
  SetStreamSource (command 11) and SetIndices (command 12).
- **They appear as track sections stream in**: 1,301 objects (934 index)
  bound in the first 5 s, 2,810 after 70 s. Some objects' headers change
  later (205 in a 50 s race), in groups, data address moved by the same
  amount and size unchanged. Those aren't moves: the section's memory is
  freed and allocated again (the kernel's `NtAllocateVirtualMemory`,
  called from the game's `sub_82295398` in its streaming code
  `sub_820D07C8`, zeroes the headers; caught with a gdb watchpoint) and
  the same section is loaded again at the same header addresses, its data
  elsewhere.
- **Everything is written by the library's memcpy (`sub_825A7410`)**:
  - Headers: copied in whole blocks (25-315 KB) from a read buffer at
    `002B0000` by the resource loader `sub_825963F0` (call at
    `0x82596448`), a few by `sub_8258B678` (`0x8258B6B4`) from the heap.
    The final data addresses must then be written into the headers by
    plain stores (not seen by the memcpy wrapper).
  - Data: 1-3 MB copies from read buffers (`002B5500`, `003EBFBC`) into
    the physical views by `sub_825963F0` (`0x8259676C`, ~90% of the
    ranges) and `sub_82596F68` (`0x82597014`); 2-4 ranges at a time by
    game code not yet looked at (`sub_820E7598`, `sub_820E7CA0`,
    `sub_820E8028`).
  - **Every vertex and index range a draw used was written whole by
    memcpy before its first use** (2,900 ranges, none partly or not at
    all). Ranges written again after first use (30-70 per 5 s, ~0.2 MB)
    were rewritten by the same loaders: a section loaded again into the
    same memory (with the same contents, as the mirror check shows: no
    range's contents changed between frames).
- What this doesn't see: writes by plain stores rather than memcpy, and
  data read from disc straight into place. Neither showed up (no range
  was used before memcpy wrote it, and the mirror check saw no contents
  change), but a race the user drives may differ.
- So a range only needs copying when memcpy has written it since its
  last copy. **Done** (2026-10-14): the memcpy wrapper (`buffers.cpp`)
  records each copy into the physical views once it's finished (a pending
  list, taken by the render thread into a map of the newest write per
  segment), and each mirror keeps, per range, the write sequence when it
  last got it; a draw copies a range only if a newer write overlaps it
  (`Buffers_NewestWrite`). Copies fell from 15-21 MB a frame to about
  0.1 MB; `--native_renderer_verify_mirror` (every skipped range compared
  with guest memory, every frame) found none different in a 70 s attract
  race. In that race even ignoring memcpy's writes altogether gave no
  differences (sections are loaded again with the same contents). The
  user then drove two races with the check on, the second at another time
  of day (the game has one world map that all races use; the time of day
  changes the lighting): 4.8 million ranges checked, none different, and
  nothing wrong on screen. So the tracking hasn't been seen catching a
  real change, but nothing in the game was found that changes vertex or
  index data after loading it. With that, the data could live in VRAM
  (see "What the mirror copies").
- **Since v0.3.8** (2026-10-15, opt-in, on in `run.sh`): with
  `--native_renderer_mirror_by_tick` and `--native_renderer_mirror_to_both`
  the mirrors are given memcpy's writes at each frame's start instead, and
  draws skip the index range, the vertex sizes and `Mirror`: -14.7% of the
  render thread's cycles a frame. Why that's safe (no write lands in data a
  frame in flight reads) and how it was checked: "Hypothesis to test: the
  mirrors updated once a tick" and what follows it.

## Tools (all debugging aids)

- `--native_renderer_show_resolve=<n>`: the window shows the frame's nth
  resolve destination instead of the main view; the log lists the frame's
  resolves by index every 1,200 frames.
- `--native_renderer_dump=<dir>`: dumps 10 s into a race and whenever a
  file named `now` appears in `<dir>`, into `<dir>/<n>`: the frame as PFM
  files (what's shown and each resolve destination) with `draws.txt`
  (each draw's target, shader hashes and textures), then a frame per
  pixel shader the main view uses with its draws left out, then one with
  textures from memory hidden and one with resolve destinations hidden.
  `draws.txt` also records each draw's render state and the vertex fetch
  constants it reads, and `draw_ids.pfm` holds which draw wrote each
  pixel of the main view. `tools/pfm_to_png.py <dir>` makes PNGs;
  `tools/rainbow_draws.py <dump>` names the draws covering most of the
  rainbow-coloured pixels (its saturation test also counts real colour,
  e.g. orange signs: look at the pictures). The game pauses while it
  writes; the frame itself is recorded at full speed.
- With `--readback_resolve=full` (an SDK setting: the emulated GPU writes
  each resolve back to guest memory), a dump with resolves also writes
  `guest_resolve_<n>_<address>_format<f>.pfm`: each resolve destination as
  guest memory holds it at the end of the frame (the last resolve to that
  address), i.e. the emulated GPU's result, decoded from memory
  (uncompressed formats, first level). Pair them with ours
  (`resolve_<n>_...`) to compare. Run such comparisons with
  `--motion_blur=true --glow=true`, so the emulated GPU draws the motion
  blur and the light bloom that `patches/rexglue-sdk/0001` and `0002`
  otherwise leave out (ours always draws them). The emulated GPU isn't
  always the right one: its bloom chain comes out blocky.
- `--native_renderer_dump_every=<s>` (with `--native_renderer_dump`): dump
  only the frame (shown image, draw IDs, `draws.txt`; no resolves or
  left-out steps) every that many seconds of a race. Attract races are
  deterministic, so two runs' dumps show the same moments and can be
  compared with a flag on and off.
- `--native_renderer_cycle=<dir>`: when a file named `now` appears in
  `<dir>`, leaves out the main view's draws of each pixel shader in turn
  for 3 s, saving a frame of each; unlike the dump, frames are recorded at
  their usual speed.
- `--native_renderer_skip_pixel_shader=<hash>`: leave out the draws of one
  pixel shader (hex).
- `--native_renderer_log_glow`: once a second, by logic and render-only
  frames: the glow history's cross-fades (pixel shader
  `E0D070FB58A4EA03`), the texture they read, the 256x144 resolves without
  a clear and which of them write that texture back, and the cross-fade's
  pixel constants and vertex colour (see the bloom under Known issues).
- `--native_renderer_log_shader_ucode=<hash>`: log a shader's microcode
  disassembly when it's first used.
- `--native_renderer_watch_draws`: log what changes in the main view's
  draws' inputs between frames (with the camera still, only flickering
  draws should change). Ignores float changes under 0.1% and compares only
  the fetch constants a draw's shaders read.
  `--native_renderer_watch_pixel_shader=<hash>` limits it to one pixel
  shader.
- `--native_renderer_check_late_writes`: log vertex and index ranges
  written after we copied them, by the end of the frame.
- `--native_renderer_base_level_only`: sample only each texture's first
  level.
- `--native_renderer_check_clobbers`: log copies into the mirror that change
  bytes an earlier draw's copy put there in the same frame.
- `--native_renderer_verify_textures`: hash every texture used again each
  frame, reloading the changed ones.
- `--native_renderer_hide_cubes`: give cube maps the white placeholder.
- `--native_renderer_no_index_clamp`: don't clamp indices to
  `VGT_MIN/MAX_VTX_INDX`.
- Logged every 5 s: draws a frame with PsParamGen and each pixel shader's
  interpolator mask; draws reading vertices past their fetch constant's
  size; and, when there are any, draws whose base vertex differs from
  their item's and draws with indices outside `VGT_MIN/MAX_VTX_INDX`.
- `--native_renderer_serialize`: wait for the GPU before recording each
  frame.
- `--native_renderer_log_gpu`: every 5 s, the packets in the library's
  kicks with effects outside the GPU (with an example of each), register
  writes below the context registers, the interrupts the library's
  callback got, and the progress block; a trace of the first 60 kicks,
  40 every 10 s, and those holding rare packets (scratch registers,
  INTERRUPT, COND_WRITE, queries...). Works with the emulated GPU too.
- `--native_renderer_check_draw_writes`: keep the library's indexed draw,
  and log every 5 s the device fields its packet-writing helpers and the
  draw itself change besides the command pointer (calls ending a command
  segment aren't compared), and how many draws were predicated or loaded
  shaders.
- `--native_renderer_library_packets`: in exclusive mode, let the
  library's indexed draw write its packets as before.
- `--refcount_global_lock`: let the library's AddRef and Release take the
  recompiler's global lock as before, and SetVertexShader take and drop a
  reference when binding the shader already bound.
- `--native_renderer_skip_repeated_state=false`: record every draw's
  viewport, scissor, depth bias, blend constants, pipeline and index
  buffer binding, as before they were skipped when repeated (see
  "Performance: repeated state calls").
- `--native_renderer_reuse_constants=false`: write every draw's constant
  blocks into the ring and bind its descriptor sets, as before blocks the
  same as the last written were reused (see "Performance: constants
  written every draw").
- `--native_renderer_log_constant_changes`: in the 5 s stats, why the
  fetch block differs from the last written (texture or vertex fetch
  constants the draw reads, or only unread ones), and, for draws repeating
  the last draw's descriptor sets with other offsets, which blocks'
  offsets differ. Off by default (a memcmp of the block per draw).
- `--native_renderer_copy_all`: copy every vertex and index range into the
  mirror every frame, as before memcpy's writes were tracked.
- `--native_renderer_verify_mirror`: compare every range not copied with
  guest memory, every frame, logging how many were checked and those that
  differ (slow: ~20 fps in a race, since the mirror is uncached memory).
  With `--native_renderer_mirror_by_tick` it instead looks each drawn range
  up in the writes the mirror draws read hasn't had (fast, exact as long as
  memcpy is the only writer), logging the ranges missing one.
- `--native_renderer_mirror_by_tick`: draws read a mirror swapped as a
  logic pass's list starts replaying, memcpy's writes applied at frame
  starts to the other; draws do no mirror work. With
  `--native_renderer_mirror_to_both` the writes go into the mirror draws
  read as well (both on in `run.sh` since v0.3.8; the first alone leaves new
  data out for the rest of a tick). Fixed at start.
- `--native_renderer_log_physical_writes`: each memcpy into the physical
  views by thread, pass (logic or render-only) and caller, checked against
  the vertex and index ranges drawn from two frames before it to one after,
  the list being replayed and the texture cache; a summary every 5 s, and
  each write changing drawn data on a render-only pass or under the replay
  on its own line.
- `--native_renderer_log_buffers`: log the vertex and index buffer objects
  draws bind (header, data, later changes), the library's memcpy into
  their headers, and for each range copied into the mirror whether memcpy
  wrote it before its first use and again afterwards (see "Buffers").
- `--native_renderer_check_mirror`: log the vertex and index ranges whose
  contents change between frames; and per frame, the bytes copied whose
  contents differ from that range's last copy, the bytes of ranges new,
  and how much of the vertex buffers' copies lies within the draws'
  vertices (lowest to highest).
- `--native_renderer_log_index_ranges`: per frame, how often draws' index
  ranges are scanned, how many are distinct and how many memcpy wrote
  since their last scan, and how the mirror's checks go (see "Profile of
  2026-10-15, after `read-registers`").
- `--native_renderer_cache_index_ranges=false`: scan every draw's indices
  again instead of keeping each range's lowest and highest index until
  memcpy writes it (on by default; see "Done: the index range cache").
- `--native_renderer_log_register_reads`: how often the register shadow is
  read a frame, the words copied (those marked dirty) and what changed
  since the last read (see "Register reads").
- `--native_renderer_log_texture_writes`: which code writes textures'
  memory: on first load, and when a texture is found changed, what the
  library's memcpy wrote there (with `--native_renderer_verify_textures`
  every change is found; see `texture-handling-optimisation.md`).
- `--native_renderer_disk_cache=false`: don't keep translated shaders and
  pipelines in `cache/` next to the executable (see "Hitches from
  first-seen shaders" in Known issues).
- `--native_renderer_validation`: turn on the Vulkan validation layer, if
  installed (it isn't on this PC yet; see Known issues).
- Texture loads of uncompressed formats are logged (format, size,
  endianness, tiling).

## Code map (`itc_recomp/src/native/`)

- `replay_hooks.cpp`: wraps each replay handler; calls
  `Renderer_OnCommand` before and `Renderer_AfterCommand` after the
  library call (command 7 is wrapped in `interpolation/blend_state.cpp`).
- `renderer.cpp`: window, device (`DeviceContext` in `device_context.h`),
  swapchain, per-frame command buffers and fences, submit (texture uploads
  first) and present (blits what `Passes` shows to the window), stats
  every 5 s, the dump's sequence. Draws, clears (26) and resolves (25)
  reach `Passes`.
- `passes.cpp`: everything per draw: register file from the shadow
  (`state.cpp`), shaders (`shaders.cpp`), constants ring, guest memory
  mirrors, pipelines, texture descriptor sets; targets, clears and
  resolves; what's shown (the main view, pitch 1040 and 2x MSAA, resolved
  to 1024x576) and the dumps.
- `gpu_link.cpp`: the kick (`sub_8229E3F0`), the deferred kick
  (`sub_8229E848`) and the library's interrupt callback:
  `--native_renderer_log_gpu`, and exclusive mode's kick.
- `buffers.cpp`: `--native_renderer_log_buffers` (buffer objects, and the
  library's memcpy `sub_825A7410` wrapped to record what it writes).
- `draw_packets.cpp`: exclusive mode's indexed draw without packets (the
  library's `sub_825BE708` is wrapped in `replay_hooks.cpp` and calls
  it), and `--native_renderer_check_draw_writes`.
- `refcount.cpp`: the library's AddRef (`sub_822A6738`) and Release
  (`sub_822A67B0`) without the global lock, and SetVertexShader
  (`sub_822A8808`) skipping both when the shader is already bound.
- `app_window.cpp`: exclusive mode's one window (hides the SDK's, routes
  our window's events to it).
- `glsl/`: our own shaders (resolves), compiled into `generated_*.h`.
- `textures.cpp`: the texture cache (milestone 3).
- `disk_cache.cpp`: the disk caches' directory (`cache/` next to the
  executable); `shaders.cpp` keeps translations there, `passes.cpp` the
  pipeline cache.
- `frame_costs.h`: what a frame spent on texture loads, shader
  translations and pipeline creation, for the long frame log.
- `log_enabled.h`: whether info logging is on. With it off (`run.sh`'s
  `--log_level=off`), the 5 s stats, the frame cost timing and the
  dimension mismatch descriptions do no work.
- Testing: build, run the game in the background with
  `--native_renderer=true --skip_intro --attract_after_s=1`. With no input,
  an attract race starts about 1 s after launch (where `fps` goes above 30 in the
  log; see "The title screen" in `functions.md`), so a run can be fully
  automatic: launch under `timeout`, delete the `/dev/shm/xenia_memory_*`
  files afterwards, and read the `native renderer:` lines in the log and
  any dump. Never screenshot the user's desktop: look at frames with the
  renderer's own dump (`--native_renderer_dump`, 10 s into a race or when
  `<dir>/now` appears; `--native_renderer_dump_every` for periodic ones).
  To compare with the emulated GPU, add `--motion_blur=true --glow=true
  --readback_resolve=full` and a full dump (`--native_renderer_dump`
  without `_every`, 10 s into the race), and put each `guest_resolve_*`
  next to the `resolve_*` of the same name (a small PIL script).

## Known issues

To be fixed (or accepted) in milestone 7.

- **The bloom's ghosting (fixed 2026-10-15)**: not interpolated, it
  ghosted with frame interpolation (seen by the user, 2026-10-14). Fixed
  on `glow-interpolation-fix` (merged): the user drove a race uncapped
  and saw no ghosting. The bloom is the glow chain whose combine is
  pixel shader `9750599DF5ED3B39` (see `--glow` in `functions.md`). What
  the logs show (2026-10-15, attract races):
  - **The glow keeps a history, written back at tick rate.** In the
    256x144 target, two draws shrink the HDR copy, then the cross-fade
    (pixel shader `E0D070FB58A4EA03`, blendcontrol `00010706`: colour =
    src x src alpha + dst x (1 - src alpha)) blends in the history texture
    (`1B0E3000` in these races), and then the blur steps and the combine
    run. `--native_renderer_log_glow`: every frame cross-fades with the
    history, but only logic frames resolve into it (30 a second; 4
    non-clearing 256x144 resolves on a logic frame, 3 on a render-only
    one).
  - Why: the post-processing objects' draw (`sub_821379D8`, recorded for
    each object by `sub_82133960` from the race draw step) records the
    history resolve (`sub_82133F58` into `*(+8408)`) only when `+48` is
    set and `+44` isn't, and `sub_82133960` clears `+48` after each draw
    (and toggles `+8060`). `+48` and `+44` are written by `sub_821351F0`
    (not yet checked: presumably once a tick, which render-only passes
    don't run).
  - So between ticks every frame blends in the history as the last logic
    frame left it, while the scene moves smoothly: the glow's trail
    likely moves in 30 Hz steps (the ghosting, not yet confirmed).
  - **The glow's strength steps at 30 Hz too**: `--log_stepping_constants`
    lists pixel c33 recorded by `82133D28` (`sub_82133D00`, from
    `sub_821379D8`, twice a frame) changing ~30 times a second. c33 is
    {`+8440` (0.7) or `+8436` (~0.8-0.86, every tick), `+8452` (the glow
    strength, ~2.7-3.2), `+8456` (0.75), `+8476` (0)} of the object;
    `+8452` is written by `sub_82136F18`. Nothing else stepping looks like
    the glow's (the rest are the known c14, c70, c74 and c100).
  - **The cross-fade's weight is its quad's vertex colour alpha**: the
    shader (`--native_renderer_log_shader_ucode=E0D070FB58A4EA03`) is
    `tfetch2D r0.xyz, tf0; oC0.w = r1.w; oC0.xyz = r0 * r1`, no
    constants. `sub_821379D8` builds the quad (`sub_82133E30`) with colour
    (8, 8, 8, `+8460`), or a fixed alpha when `+40` (a reset) is set; the
    8 undoes the history's 1/8 (its resolve's exponent bias). `+8460` was
    ~0.55 in attract races (0.545-0.58, changing every tick): each tick,
    history = 0.55 x history + 0.45 x this frame's glow.
  - **The fix** (`src/interpolation/glow.cpp`, wrapping `sub_821379D8`, in
    races with interpolation on): c33's fields and `+8460` are blended
    between the last two ticks by the tick progress (as world-space
    sprites); on render-only passes `+48` is set as the last logic pass
    had it, so the game records the history resolve every frame; and the
    weight is raised to the pass's share of a tick (time since the
    object's last pass over 1/30 s), so the history fades as fast in time
    as at 30 Hz. Checked in an attract race at the 144 cap: the history
    written on all 120 render-only frames a second, per-frame weights
    0.85-0.91 for shares of 0.16-0.29, c33 no longer listed by
    `--log_stepping_constants`, 150 fps as before. The user then drove
    uncapped and judged it fixed.
    `--blend_glow=false` and `--glow_history_every_frame=false` turn the
    two parts off.
- **Motion blur reads the 30 Hz tick.** The game's velocity-based motion
  blur (pixel shader `7EB16456AAD56CBA`) gets its velocities from the
  tick, so at interpolated frame rates each frame smears a whole tick of
  motion, more than intended. `run.sh` turned it off with the emulated GPU
  (`--motion_blur=false`, which our renderer ignores); ours always draws
  it. Scaling the velocities to the frame's share of a tick, or an option
  to skip the pass, would fix it.
- **An in-game movie locked up the whole game** (seen by the user in
  v0.3.0, 2026-10-14; not the Ubisoft logo; the user knows where it plays).
  Not investigated yet (milestone 7, the user's call). Plan: launch the
  game, have the user trigger the movie, and if it hangs take every
  thread's stack with gdb to see what it waits on. One lead to check, not
  a finding: the movie code sets a callback the library calls on vblank
  interrupts (`sub_824F0030`, device +15144; see "What the GPU does outside
  itself"), and with no GPU plugin there are no vblank interrupts. The game
  ships 12 movies in `DATA/TXR/MOVIE` (`OPENING`, `STAGE1-3_OPENING`,
  `SHIBUYA_OPEN`, `SHINJUKU_OPEN`, endings, staff rolls).
- **The boot movie (`UBI_LOGO.wmv`) isn't drawn** in our window (seen by
  the user). It's drawn outside the command list (see "Movies" in
  `gpu-functions.md`), which the replay hooks don't see; the log names it
  ("DrawVertices ... called from 8248466C ... outside the command list").
  Tried 2026-10-14 (`2433232`, then removed): wrapping DrawVertices and
  drawing it after the library's call as an auto-indexed draw (vertex
  buffers copied from the fetch constants, `vkCmdDraw` from the start
  vertex), with the texture cache checking that draw's textures every
  frame. It drew on the render thread and showed the logo, but the user
  saw the playback judder in our window, and judged it not worth the
  effort for now: for milestone 7. Not yet known why it juddered; to find out, log per
  movie draw the frame number, the three planes' addresses and a hash of
  the Y plane, and look for repeated or out-of-order frames. The code is
  in `2433232` to start from.

- **Gamma**: our window is lighter than the game's (seen side by side,
  2026-10-14): the Xbox 360's output gamma isn't applied yet. A known
  issue, for milestone 7 (the user's call).

- **Textures replaced in memory showed their old contents for a while**
  (fixed 2026-10-15). The game rewrites textures in place as it streams
  track sections; they were hashed again only every 240 frames, so a
  change was noticed up to 3,200 frames late. Now the library's memcpy,
  which writes them (as it does vertex and index data), marks them to be
  reloaded at the next frame; `--native_renderer_verify_textures` checks
  that nothing else changes them (none in an attract race). Five textures
  a race aren't written by memcpy at all, and their writer isn't known;
  they didn't change. Details in `texture-handling-optimisation.md`.

- Textures are never freed: memory reused for different textures (another
  race) leaves the old images behind.

- **Resizing the window and the swapchain** on Wayland: the compositor
  leaves the size to us and never reports the swapchain out of date, so the
  swapchain likely stays 1280x720. The user sees the image follow the
  window's size (2026-10-14), so to players it isn't a problem (maybe the
  compositor scales it); not a player-facing limit any more. Recreating
  the swapchain on the window's size-change events would still give a
  sharper image at other sizes.
- **Performance: shaders are found by hashing their microcode every
  draw.** Translations are cached by content hash, so each patched version
  of a vertex shader is translated once; the cost is finding which one a
  draw uses. Since 2026-10-14 each microcode is hashed once a draw
  (`MicrocodeHash`, passed to `ShaderCache::GetInterface` and `Get`, and
  used by the diagnostic maps), not four to six times. Further: key by
  what the library tracks (the shader object and the declaration it's
  patched for; `sub_822AE340` is already wrapped) instead of hashing, but
  a freed object's memory reused for another shader would then draw with a
  stale translation unless shader creation and release are hooked too.
- **Hitches from first-seen shaders** (fixed 2026-10-15: the disk
  caches). Translating a shader took ~1 ms, and new scenery brings 15-25
  at once: pinned to one CPU (`taskset -c 0`, a stand-in for the Steam
  Deck), slow frames spent 21-26 ms translating, every session, since
  translations were kept only in memory; pipelines were made without a
  `VkPipelineCache` (Mesa's own disk cache likely covered the driver's
  compile on Linux). Now both are kept in `cache/` next to the executable
  (`disk_cache.h`, `--native_renderer_disk_cache=false` to turn off):
  - `shaders.bin`: each translation (key: stage, microcode hash, register
    count, interface; value: the SPIR-V and the bindings) and each vertex
    and pixel shader pair's interface, appended as made and flushed (the
    game exits without warning). A header holds a version
    (`kDiskVersion` in `shaders.cpp`: **bump it whenever what a
    translation produces changes**, the SDK's translator, the
    modification `ShaderCache::Get` sets or `TranslatedShader`'s fields)
    and the device features translations depend on; a mismatch starts a
    new file; a cut-short last record is dropped. Interfaces are now also
    kept in memory per shader pair instead of worked out every draw.
  - `pipelines.bin`: the `VkPipelineCache` for all pipelines, loaded if
    its header is this device and driver's, saved (to `.tmp`, renamed
    over) at most every 10 s after new pipelines.
  Second session, pinned to one CPU: all 69 shaders from the cache, none
  translated; 3 slow frames instead of 8, none translating, 17 pipelines
  made in 0.3 ms; the one slow frame in the race was texture loading
  (34.5 ms for 44 textures, 11 MB). Texture streaming is the hitch left.
- **Hitches from texture streaming** (improved 2026-10-15). New track
  sections bring textures loaded on the render thread in the frame that
  first uses them. Untiling went block by block (an address computed and
  a `memcpy` per 8- or 16-byte block); now `GatherTiledRow` copies whole
  runs (the tiled layout keeps 16 bytes contiguous, aligned in x; 8 for
  1-byte blocks), checked equal to the old gather on 180,224 rows of a
  race, and the byte swap uses SSSE3 shuffles (x86-64-v2 has them;
  scalar elsewhere). Pinned to one CPU, the race's first frame (44
  textures, 11 MB) went from 33.2 ms to 9.8 ms; during the race the
  frame spending the most on textures in each 5 s took 3-11 ms. The log
  now gives hashing time in "long frame" lines and, every 5 s, the time
  spent loading and hashing textures and the worst frame. A per-frame
  time budget (deferring loads to later frames) was considered and not
  done (the user's call): deferred first loads show white placeholders.
  Since then hashing is gone too (textures are marked written by the
  library's memcpy instead): texture time per 5 s pinned went from 30-40
  ms to 1-11 ms. What's left to try, in order, is in
  `texture-handling-optimisation.md`.
- **The attract race's grayscale filter flickers with the game pinned to
  one CPU** (`taskset -c 0`; seen by the user, 2026-10-15). Since the
  frame pacer fix (`d8c10c5`, a due tick always gets the next pass); the
  build before it doesn't flicker pinned (it runs in slow motion
  instead), and nothing flickers unpinned or on the Steam Deck. Not
  investigated (the user's call): likely the filter is drawn from
  something that follows logic passes, and the pass pattern under one
  CPU differs. A known issue for now.
- **Performance: string-keyed caches** (fixed 2026-10-14). Pipelines, the
  ID pass's pipelines and pipeline layouts were keyed by `std::string`
  copies of their keys (a 96-byte `PipelineKey`: a `malloc` and a byte
  hash and compare every draw), and texture sets by a string of their
  contents in a map cleared every frame (a node and a key allocated per
  set). Now `PipelineKey` and handle pairs are the keys, and texture sets
  use `FrameSetCache` (an open-addressed table and one key buffer, kept
  between frames). With the shader hashing above: uncapped attract race
  239 to 258 fps mean (10th percentile 191 to 206; two runs each). The
  profile before (2026-10-14, uncapped, render thread): recompiled game
  code and graphics library 21%, libc 16.5% (`memcmp` ~5%, `memcpy`
  ~2.5%, `malloc` 2%+), `Passes::State::Draw` 12%, the Vulkan driver
  12%, blending 7%, textures 6.6%, shader lookup 5.2%, the mirror 5.1%,
  `ReadRegisters` 3.7%.
- **A freeze while driving** (to re-examine in milestone 7, the user's
  call; seen by the user, 2026-10-14, in a
  driven race after `4410ce8`, the indexed draw without packets; to look
  into later, the user's call). The log (`itc_560.log`) shows no slow
  second in the race (slowest 131 fps at the 144 cap) and the game's ticks
  stayed at 30, so either the hitch was under ~0.1 s or the picture
  stopped changing while frames still went out. The same race had **failed
  draws** in its busiest 5 s windows (36 and 74 a frame, with 3,000-3,300
  draws and up to 824 inline draws a frame; earlier logs had them once, 7
  a frame), not known to be related. Since `895055b` the log names each
  failed draw's reason ("failed draws a frame by reason") and any frame
  taking 100 ms or more ("long frame", since 2026-10-15 40 ms or more,
  with the frame's texture loads, shader translations and pipeline
  creations and the time each took: `frame_costs.h`); the next run that
  shows either should say which.
  The user then saw, in v0.3.0, the **HUD dropping out and flickering in
  busy scenes**, with the game stuttering and freezing (2026-10-14). The
  HUD is drawn last, so it's what a full per-frame buffer loses first: the
  constants ring (32 MB, ~9 KB a draw, so ~3,500 draws; the failures above
  came at ~3,050-3,400 attempted draws a frame), the texture descriptor
  pool (4,096 sets) or the inline vertices (4 MB). Now (`renderer-fixes`)
  the constants come in 64 MB blocks per frame (~7,000 draws each) and a
  frame that fills its blocks gets another, kept for later frames; texture
  sets come in pools of 8,192, grown the same way; the inline vertices,
  part of the guest memory mirror, are fixed at 32 MB, and running out is
  its own failed-draw reason. Each growth is logged ("block N added",
  "pool N added"). Checked with 2 MB blocks and 256-set pools (up to 14
  blocks a frame): no failed draws, the frame dumped looked right. A
  driven race on v0.3.0 (`run-debug.sh`) then logged "constants ring full"
  in its busiest 5 s (3,122 draws a frame on average), and nothing else
  failing, so the constants ring is the one that runs out; the HUD didn't
  visibly drop out that time. A driven race on the fix (2,867 draws a
  frame at most) grew nothing. The stutter and freezes weren't seen in
  either run: their cause isn't known.
- **No window when Vulkan has no surface support**: with `XDG_DATA_DIRS`
  missing `/usr/share` (as in VS Code's terminal), the Vulkan loader finds
  no drivers, SDL fails to make the window ("Installed Vulkan doesn't
  implement the VK_KHR_surface extension"), and the renderer logs "couldn't
  start; continuing without it": the game runs with no window and no
  message to the player. It should stop with an error instead.
- **Performance: locked instructions are slow on the render thread** (a
  few hundred cycles each, 2026-10-14: the thread's many stores have to
  drain first). The shader reference count's compare-and-swap still costs
  ~1%, and any atomic added to per-draw code (counters included) shows up:
  keep per-draw counters per thread.
- **Performance, to do: the library's remaining packet writers** (moved
  here from milestone 6's step 5, the user's call, 2026-10-14: not clearly
  worth it now). Besides the indexed draw (done, see "The indexed draw
  without its packets"), the library still writes GPU packets nothing
  reads in exclusive mode: the shader loads' own packets (~1% of the
  render thread), inline draws (`sub_825A17A0`), SetTexture, SetIndices,
  pixel shader constants and others, ~3-4% together (2026-10-14 profile,
  before the reference count change). Each needs the same check as the
  indexed draw (`--native_renderer_check_draw_writes`: which device fields
  it changes besides the command pointer) before its packets are skipped.
- **First-person views are slower on the Steam Deck** (2026-10-15):
  cockpit 42-52 fps against 55-71 in third person, the render thread
  flat out in all views, at about 7 µs a draw, and the cockpit drawing
  ~600 more a frame. Measurements and next steps (a profile in cockpit
  view) in "Performance: the render thread" below; the fix is the item below.
- **Performance: repeated state calls** (fixed 2026-10-15). Every draw
  recorded its viewport, scissor, depth bias, blend constants, pipeline,
  descriptor sets and index buffer, then the draw. A profile of the
  render thread (uncapped attract race, branch records `perf record -j
  any_call,u` to charge library time to its caller: unwinding through
  `itc` fails, and the release build has no line tables) put ~12% in the
  Vulkan driver, 9% of it from `Draw`, and ~5% in `memcmp`, some of it
  called by the driver. Counted over an attract race (the "following
  another draw" line every 5 s), of ~2,200 draws a frame each repeated the
  previous draw's viewport, scissor, depth bias and blend constants
  ~100%, its pipeline 70-74%, its descriptor sets (offsets aside) 44-51%,
  and its index buffer binding 1% (the same buffer and type 93-97%, at
  another offset). Now `Draw` records only what changed since the last
  draw (`RecordedState`, forgotten at a frame's start, after a resolve's
  draw and after the ID pass), and indexed draws bind the mirror at offset
  0 and start with `firstIndex` (97% then repeat the binding). Descriptor
  sets were still bound every draw, the constants' dynamic offsets
  changing; now only when they change (see "Performance: constants
  written every draw").
  `--native_renderer_skip_repeated_state=false` brings back the old calls.
  Frames dumped every 5 s of an attract race at 30 fps
  (`--native_renderer_dump_every=5`, interpolation off) were
  pixel-identical with and without. Uncapped attract race, alternating
  runs: 257 and 260 fps mean without, 275 and 276 with (10th percentile
  205/204 to 218/224). Pinned to one CPU: 51, 51, 59 without and 53, 62,
  62 with; noisy, so a small gain there at best.
- **Performance: constants written every draw** (improved 2026-10-15,
  branch `descriptor-reuse`). Each draw wrote its five constant blocks
  (system, vertex floats, pixel floats, bool/loop, fetch) to new space in
  the constants ring and bound its descriptor sets with the new offsets,
  though the game sets only the constants that change (on the 360 they
  stayed in the GPU's registers between draws). Each draw reserves ~9 KB
  of the ring (4 KB per float stage), but writes less: only the float
  registers its shaders read are packed in, ~2 KB a draw on average.
  Now `PlaceConstants` compares each block with the last one written in
  the block of the ring in use (`LastConstants`, a host copy; the ring
  itself is uncached memory, not read back) and, when the same, points the
  draw at it; the float registers are compared as they're packed, in the
  same pass. Descriptor sets are bound only when they or their offsets
  differ from the last draw's. Forgotten at a frame's start and when the
  ring moves to another block. `--native_renderer_reuse_constants=false`
  writes every block as before. Over an attract race (the "given
  constants" line every 5 s), blocks reused: system 88-90%, vertex floats
  74-75%, pixel floats 74-75%, bool/loop 100%, fetch 39-43%; constants
  written a frame 4.5 MB to 1.5 MB; descriptor sets bound again for 62% of
  draws instead of all ("with offsets" in the "following another draw"
  line). Frames dumped every 5 s at 30 fps were pixel-identical with and
  without. Uncapped attract race, alternating: 272.7 and 268.6 fps mean
  without, 276.4 and 272.6 with (~+1.4%; 10th percentile 222/215 against
  221/218).
  **Why the fetch block changes** (measured 2026-10-15,
  `--native_renderer_log_constant_changes`, uncapped attract race). The
  block (all 32 texture fetch constants, 768 bytes) differs from the last
  one written for ~60% of draws, and always in a fetch constant the draw's
  shaders read: a texture fetch constant read ~50% of draws, a vertex fetch
  constant read ~15% (its address alone ~5%), only unread ones 0%. So
  comparing or packing only the slots read would gain nothing. (The
  shaders read the block only at the fetch constants their instructions
  name, which `textures`, `samplers` and `vertex_fetch_constants` list,
  except `getWeights`, which reads a texture's size with no binding.) And
  the bind skips it could add are few: the texture descriptor sets
  themselves repeat for only ~47% of draws, and of those, the offsets
  differ for ~10% of draws, the fetch block's alone for 7-9%. Even a fetch
  block never changing would raise binds skipped from ~38% to ~46% of
  draws; the first 38% gained ~1.4%, so this is likely ~0.3%, within run
  to run noise. Not pursued: the texture changes rebind the sets anyway.
- **Performance: reading constants from guest memory** (fixed
  2026-10-15, branch `descriptor-reuse`). Profile of the render thread
  (uncapped attract race, user-time samples): packing each draw's float
  constants took ~14% (vertex 7.8%, pixel 6.6%) and `ReadRegisters` ~4.7%.
  In the packing loop ~85% of the samples were on testing each of the 256
  bits of the shader's `float_constants` mask in turn, a mispredicted
  branch; the loads were `REX_LOAD_U32`, a volatile load and byte swap per
  word, which can't be vectorized. Now the mask is walked run by run of
  consecutive registers (`countr_zero`/`countr_one`) and each run, like
  each of `ReadRegisters`' blocks, is copied by `guest::CopyU32s`, one
  plain loop the compiler vectorizes (`pshufb`) that also reports whether
  any word changed (for the constants reuse). It takes the host address
  once per range, so a range crossing 0xE0000000 (where the SDK's host
  offset changes; the device is at `0x40021B00`, far from it) is copied in
  two parts; tested against `LoadU32` word by word around the line. Kept
  out of a recursive call: that stopped the copy being inlined and
  vectorized. A temporary check computing
  every draw's constants both ways over a whole race: 0 mismatches in
  ~3.6 billion float words and ~12 billion register words. Uncapped
  attract race, alternating, 20 s runs: 284.6 and 284.9 fps mean before,
  329.1 and 319.9 after (~+14%; 10th percentile 247/244 against 284/277).
  (Frames dumped at the same moments can't check such changes any more:
  two runs of the same build already differ, the attract race allocating
  its vertex buffers at other addresses.)
- **Performance: diagnostics on every constant replay** (fixed
  2026-10-15). `BlendState_ReplayConstants` ran, for each vertex constant
  upload of 4+ registers (~1,500 a frame) and with logging off: a clock
  read, a matrix load and a status string (allocated) for a
  `--log_replayed_constants` line, and `SlotOwner`, a mutex and a map
  lookup in a table filled only with `--log_slot_callers`, for
  `--log_upload_owners`. Both now run only with their flag. Uncapped
  attract race, alternating: 276 and 275 fps mean before, 284 and 279
  after.
- **Performance, to do: our renderer's own code** (milestone 7, the
  user's call, 2026-10-14). After milestone 6's speed work our renderer is
  ~78% of the render thread, the replay handlers ~11%, the library ~9%.
  Biggest by the last profile (2026-10-14, after `d74d923`; uncapped
  attract race, ~235 fps; libc and driver time counted in the caller):
  `Passes::State::Draw` 27% (much of it in the Vulkan driver),
  `BlendState_ReplayConstants` 9% (1.3% waiting on its mutex), texture
  sets (`GetTextureSet`) 7%, `ReadRegisters` 3.6%, `TextureCache::GetView`
  3.4%, the mirror (`Mirror`, `IndexRange`, `Buffers_NewestWrite`) ~6%,
  shader lookups (`ShaderCache::Get`, `GetInterface`) 3.7%, `GetTarget`
  2%, `GetPipeline` 1.6%. Known leads: the shader hashing and the
  mirror's redundant copies (both above); texture sets keyed by strings.
  Profile again before picking one.
  Profiled again 2026-10-15 (the method, the breakdown and the next steps
  are in "Performance: the render thread", "Profile of 2026-10-15", below); the
  repeated state calls and the replay's diagnostics are done (above).
  The shares in this item are from 2026-10-14 and out of date: the latest
  breakdown is "Profile of 2026-10-15, after `read-registers`", and what's
  been done since is in that section's status.
- No Vulkan validation layer here yet (`--native_renderer_validation` falls
  back without it; still missing on 2026-10-16); install `vulkan-validation-layers` before drawing gets
  complicated.

## Performance: the render thread

The render thread is the frame-rate limit (flat out in every view on the
Steam Deck), and its cost follows the number of draws: ~7 µs a draw on
the Deck before this work. The first-person views (cockpit, bumper) are
where it shows, as they draw more (the interior, the rear-view mirror),
so the work is making every draw cheaper, in any view. This section was
`first-person-performance.md` until 2026-10-15; the "Performance:" items
in Known issues above are the fixes from before it.

Status (2026-10-15, `main` after v0.3.7): skipping repeated state calls
and diagnostics left running (about +9% uncapped together, see "Profile
of 2026-10-15"); reusing constant blocks and skipping descriptor binds
(~+1.4%) and copying constants from guest memory vectorized (~+14%), both
from `descriptor-reuse`, released in v0.3.7 (uncapped ~285 to ~320 fps). A
fresh profile after v0.3.7 is in "Profile of 2026-10-15, after v0.3.7",
with where libc and driver time comes from and the next steps. Since
v0.3.7 (merged 2026-10-15 from `replay-allocations`, `blend-math` and
`read-registers`): interpolation's replay without allocations, blends
planned once a tick and cameras blended once a frame, the byte swap by
hand, and register reads of only what the library marked dirty; together
~12% fewer render thread cycles a draw than v0.3.7 (~7,660 to ~6,760, by
profile; see "Against v0.3.7"). Then the mirrors updated once a frame
(`mirrors-fix`, opt-in, on in `run.sh`): -14.7% render thread cycles a
frame ("Hypothesis to test: the mirrors updated once a tick" and what
follows). All of it released in v0.3.8 (uncapped attract race with
`run.sh`'s flags ~430 fps). Since v0.3.9 (2026-10-16, on `main`):
`Buffers_OnCommand` and `ReplayStats_OnCommand` check their flag before
setting up their logging's stack frame (~-2% of the render thread's cycles
a frame), and `GetRenderer` no longer sets up `Init`'s stack frame on every
call (~-150 thousand cycles a frame); see "`Buffers_OnCommand`'s cost". None
of it checked on the Deck yet, and no profile taken in cockpit view itself.

### What the user reported: first-person views slower on the Deck

On the Steam Deck, with the build of `47484c8` (texture streaming fixed,
see `texture-handling-optimisation.md`), the game is smooth in the third
person view, but in the two first-person views (cockpit and bumper) a
thread sits at 100% and the game feels laggy. In third person a thread
"still spikes occasionally, up to 80% or so".

### The log

`itc_045.log` from the Deck (`run-debug.sh`, a race the user drove: about
50 s in cockpit, 20 s in bumper, then 30 s in third person, in that order;
not an attract race). The log isn't in the repo; the numbers below are
from it. "Per 5 s" values are the renderer's stats lines; frame times are
`--log_frame_times` (2 s windows).

| View (time in the log) | Draws a frame | Frames/s | Render thread CPU a frame (`render thread draw CPU`, avg) | Ticks released over 8 ms late, a second |
|---|---|---|---|---|
| Cockpit (01:35:02-01:35:52) | 2,570-3,050 | 42-52 | 17.5-23.7 ms | 12-22 |
| Bumper (01:35:52-01:36:12) | 2,030-2,360 | 57-65 | 14.5-17.8 ms | 9-18 |
| Third person (01:36:17-end) | 1,875-2,480 | 55-71 | 13.4-18.3 ms | 10-28 |

- **The busy thread is the render thread** (`XThreadA1FFD6C0`, entry
  `82091EF8`, `Renderer::ThreadLoop` in `functions.md`), at 97-98% of a
  core in **all three views**. With `--max_fps=144` it's always the limit
  on the Deck, so it's always busy; what changes between views is how
  many frames it manages. (The user's 100% against 80% was likely seen
  in another tool's sampling; the log doesn't show a difference in
  busyness.) The main thread is at 46-61%, everything else under 15%.
- **The cost follows the draw count**: about 7-7.5 µs of render thread
  time per draw in every view (cockpit ~21 ms / ~2,800 draws, third
  person ~15 ms / ~2,100). The cockpit draws ~600 more a frame (likely
  the car's interior: not checked), so it runs ~25% slower.
- **A 512x128 resolve** (`1EC79000`, format 54, flags `0x300`) is in the
  frame's resolve list in cockpit and bumper (lists logged at 01:35:23,
  01:35:50, 01:36:09) and gone in third person (01:36:28, 01:36:47);
  resolves per frame fell from 25.5 to 24.5 then. Its shape suggests the
  rear-view mirror; not confirmed. Bumper and third person draw about the
  same number of draws and run at about the same rate, so the mirror
  alone doesn't explain the bumper view feeling worse.
- **Late ticks**: the 30 Hz game ticks are often released over 8 ms late
  (the "late" count in the fps lines; see "frame pacer" in
  `functions.md`), 12-22 a second in cockpit. Ticks per second stay at
  30 and there are no grid restarts, so this is uneven motion (judder),
  likely what the user feels as lag, rather than slow motion.
- **Not textures, shaders or pipelines**: no frame over 40 ms in the race
  (the "long frame" lines are all at loads), texture loading at most
  4.7 ms in any frame, all shaders from the disk cache.

### What to do next

The fix is making each draw cheaper on the render thread (or doing fewer
of them), which is the item "Performance, to do: our renderer's own code"
in Known issues above. Find where the time goes first,
with a profile of the render thread **in the cockpit view**:

- The attract race (`--skip_intro --attract_after_s=1`) chooses its own
  camera, so the cockpit view needs the user: start the game (from the
  repo root, the command in `CLAUDE.md`), ask the user to drive a race in
  cockpit view, and while they do, record the render thread:
  `perf record -e cycles:u --call-graph dwarf,16384 -t <tid> -- sleep 15`
  (the tid is the `XThread...` with `[guest 8258DE50 -> 82091EF8 ...]` in
  the `--log_thread_cpu` lines, or from `ps -L`). Then the same in third
  person, to compare. `perf` can only attach to a process it started or
  with `ptrace_scope` allowing it; check, and if attaching isn't
  allowed, start the game under `perf record` instead.
- The last profile (2026-10-14, uncapped attract race on the PC, third
  person): `Passes::State::Draw` 27% (much of it in the Vulkan driver),
  `BlendState_ReplayConstants` 9%, `GetTextureSet` 7%, the guest memory
  mirror ~6%, shader lookups 3.7%, `ReadRegisters` 3.6%,
  `TextureCache::GetView` 3.4%, `GetTarget` 2%, `GetPipeline` 1.6%.
- Check what the extra cockpit draws are (the interior, the mirror's
  scene) with the existing per-pass tools in "Tools" above, e.g. a dump
  (`--native_renderer_dump`) in cockpit view.
- If the mirror pass is a real cost, see whether its draws can be
  cheaper (it may redraw the scene with the same states).

Measure on the PC before and after any change. For changes under a few
percent (most of them now), profile the render thread instead of counting
frames (`itc_recomp/tools/profile_render_thread.sh`, see "Measuring small
changes" and "Against v0.3.7"). For larger ones, an uncapped attract race
(`--max_fps=0`, ~430 fps mean with `run.sh`'s flags since v0.3.8;
`timeout 20`, mean over
seconds 3-18 of the race, as in `CLAUDE.md`), alternating old and new
builds (keep a copy of the old binary outside the build directory; a copy
writes its logs to `logs/` beside itself, so read the newest log there and
check its time), and, only when the user agrees, a pinned one (`taskset -c
0`) as a stand-in for the Deck. Correctness: frames dumped at the same
moments (`--native_renderer_dump_every`) no longer match even between two
runs of the same build (the attract race allocates its buffers at other
addresses), so they can't check a change. Instead, for a change that
should give the same results by a faster path, add a temporary check that
computes both ways on every draw and logs the mismatches, run a whole race,
then remove it (as for the constant copies: 0 mismatches). Then a build for
the user's Deck: local builds need a newer glibc than SteamOS has, so
push the branch and run CI on it (`gh workflow run build-linux.yml --ref
perf-debug`, then `gh run download <id> -D itc_recomp/out/dist/<name>`;
the release preset is `-O3` with ThinLTO and `-march=x86-64-v2`). The
user unzips it on the Deck (`chmod +x itc run.sh run-debug.sh`) and
repeats the three views in order with `run-debug.sh`.

### Ideas for lightening the render load

Written 2026-10-15, before any profile in cockpit view: estimates, to be
checked. The game draws 2,000-3,000 draws a frame, little for a modern
GPU; the cost is our per-draw overhead from emulating how the 360 drew:
about 7 µs a draw on the Deck, roughly 1.5 µs on the user's PC (258 fps
uncapped at roughly 2,500 draws; a rough figure), where native renderers
manage well under 1 µs. Roughly by payoff for the effort:

1. **Redraw fewer passes on interpolated frames.** A race frame has ~25
   resolves: the shadow maps (two 1024x1024 depth passes), the 512x512
   and 1024x1024 reflection passes, the 256x144 glow chain, the 512x128
   one that may be the mirror. With interpolation, every in-between frame
   redraws all of them from the same tick. Shadows and reflections
   refreshed once a tick (30 Hz) likely wouldn't be noticed, and reusing
   the tick's images could remove a large share of the draws on most
   frames. Count draws per pass first (the dump tools in
   "Tools" above can) to see what's at stake.
2. **Upload less constant data.** (Done in part: blocks the same as the
   last written are reused, see "Profile of 2026-10-15", Next. The
   estimate below was too high: ~9 KB a draw is reserved in the ring, but
   only ~2 KB written, the floats being packed.) About 9 KB of shader constants is
   uploaded per draw (Known issues above, the constants
   ring): at 2,800 draws ~25 MB a frame, ~1.25 GB/s of copying at 50 fps,
   on the render thread. The translated shaders know which constants they
   read: upload only those, or only what changed since the last draw.
3. **Re-derive state only when it changed.** Every draw reads the whole
   register file, looks up shaders, builds a pipeline key and finds a
   texture set. The game's graphics library keeps dirty flags for exactly
   this; following them would skip most lookups, as consecutive draws
   usually change little (constants, maybe a texture). **Register reads:
   done** (`read-registers`, see "Register reads"): the register file is
   now read only where the dirty masks say. The lookups (shaders,
   pipeline key, texture set) don't use the masks yet.
4. **Split the render thread in two.** One thread replays the game's
   command list and runs the library, and also translates and records
   Vulkan. Recording on a second thread, pipelined a frame behind, could
   nearly double what the Deck can feed (its 4 cores are ~30% busy). The
   biggest structural change and the most work.
5. **Cheaper Vulkan calls**: push descriptors or descriptor buffers
   instead of allocating descriptor sets, more dynamic state, skipping
   rebinds of unchanged state (the last profile had `Passes::State::Draw`
   at 27%, much of it in the driver). **Skipping repeated state: done**
   (2026-10-15, `--native_renderer_skip_repeated_state`, see "Performance:
   repeated state calls" in Known issues): +6.5% uncapped. The
   descriptor sets have since been bound only when they change (62% of
   draws).
6. **Smaller items**: the interpolation blending (~2.5-3 ms a frame on
   the Deck in cockpit; SIMD; largely done since: no allocations, blends
   planned once a tick, the byte swap vectorized, see "Done on
   `blend-math`"), and the library's remaining packet writers (~3-4%, in
   Known issues).

Start with the profile in cockpit view and a draws-per-pass count: they
show whether 1 and 2 are as big as estimated.

### Profile of 2026-10-15 (PC, attract race)

The user's view (2026-10-15): the cockpit just draws more, so general
render optimisations are the way; a cockpit profile isn't needed first.

**How it was profiled, with the release build as is** (no debug build:
`-O0` changes what's slow). Start the game uncapped in the background, take
the render thread's tid (the busiest `XThread...` in `ps -L -o
tid=,pcpu=,comm= -p <pid>`), then `perf record -e cycles:u -t <tid>`.
- A flat profile (`perf report --no-children --sort dso,sym`) is reliable.
- Call stacks don't work: `--call-graph dwarf` drops every sample whose
  stack passes through `itc` (unwinding fails there), and the release build
  has no line tables (`addr2line` gives `passes.cpp:?`), though perf does
  name inlined functions.
- To charge libc and driver time to the function of ours that called it,
  record the CPU's branch records instead: `perf record -e cycles:u -j
  any_call,u -t <tid>`, then `perf script -F period,ip,sym,dso,brstacksym`
  (or `brstack` for addresses) and take the first branch whose source is in
  `itc`. Calls go through the GOT (`call *(%r15,%rbx,1)`), so disassembly
  doesn't name the target, but `brstack`'s target addresses do. Tail
  jumps aren't recorded, so a function entered by `jmp` (e.g. `memcmp`)
  is charged to the call before it: treat those with care.
- libc has no symbols perf reads, but `objdump -d /usr/lib/libc.so.6`
  names its internal functions (`__memcmp_evex_movbe`, ...); the load
  address is in `perf script --show-mmap-events`.
- Within a big function (`Draw` is ~15 KB, much of it inlined), `perf
  annotate` doesn't work on this build: it shows other functions'
  instructions under the name. Instead take `perf script -F ip,sym,symoff`
  (the `ip` alone is a run-time address, the executable being
  position-independent), add each sample's `symoff` to the function's
  address from `nm -S`, and count the samples in address ranges of `objdump
  -d --start-address=... --stop-address=...` of it: e.g. the loop between
  two branches. That's how the float constant loop was found.
- After changing code meant to be vectorized, check the disassembly for
  it (`pshufb` for byte swaps on x86-64-v2): a change can quietly stop the
  compiler inlining or vectorizing it.

**Render thread, before the changes below** (uncapped, ~260 fps):

| Part | Share |
|---|---|
| Our renderer (`native::`) | ~37%: `Passes::State::Draw` 13%, textures ~8%, mirror/buffers ~5%, shader lookups ~3.5%, `ReadRegisters` 3% |
| The game's Direct3D library and replay handlers (guest code) | ~21% |
| Vulkan driver (`libvulkan_radeon`) | ~12%, 9% of it called from `Draw` |
| `memcmp` | ~5%, some of it called by the driver |
| `memcpy` | ~4.4%: 2.9% `Draw` writing constants into the ring, 1.1% the mirror |
| Interpolation | ~10%: `BlendState_ReplayConstants` 6%, its mutex ~1.6%, malloc/free ~1% |
| malloc/free elsewhere, other mutexes | ~2-3% |

**Render thread, after v0.3.6, before the constant copies** (uncapped,
~285 fps, `perf record -e cycles:u`, user time only; 2026-10-15): `Draw`
~20% of which the float constant loops ~14% (vertex 7.8%, pixel 6.6%),
`BlendState_ReplayConstants` ~6-7%, `ReadRegisters` ~4.7%,
`TextureCache::GetView` 3.2%, `IndexRange` 2.2%, `GetTarget` 2.1%, the
library's AddRef/Release and packet writers ~1-2% each; libc ~11% and the
Vulkan driver ~8.5% of all samples. The float loops and `ReadRegisters`
are now much cheaper (see Next, 1); no profile has been taken since, so
take one before choosing what's next.

The matrix math in `interpolation/transform.cpp` is only ~1.5% (~137 ns
a product, ~470 blended products a frame, measured with a benchmark):
not worth SIMD.

**Done** (v0.3.6; details under "Performance: repeated state calls" and
"Performance: diagnostics on every constant replay" in
Known issues):
- Draws record viewport, scissor, depth bias, blend constants, pipeline
  and index buffer binding only when changed
  (`--native_renderer_skip_repeated_state`); indexed draws use
  `firstIndex`. Pixel-identical frames; uncapped 258 to 275 fps.
- The constant replay's diagnostics (a clock read and a status string,
  and a mutex-guarded lookup) run only with their flags: 275 to ~281.
- Pinned to one CPU the gain didn't show clearly (runs vary ~15%); the
  Deck's result is still to be seen.

**Next, in order of payoff for the effort:**
1. **Bind descriptor sets only when they change: done** (2026-10-15,
   branch `descriptor-reuse`; "Performance: constants written every draw"
   in Known issues). A block the same as the last one written is
   reused; sets are bound for 62% of draws instead of all, constants
   written fell from 4.5 to 1.5 MB a frame, and uncapped went up ~1.4%
   (272.7/268.6 to 276.4/272.6 fps), less than hoped. The fetch block,
   reused for only ~40% of draws, limits the binds skipped, but not
   worth more work (measured 2026-10-15): it changes only where the draw
   reads it (a texture ~50% of draws, a vertex buffer ~15%), mostly along
   with the texture descriptor sets, so reusing it more could skip binds
   for at most ~8% more draws (~0.3% by the gain above). Details in
   "Performance: constants written every draw".
   **Reading constants from guest memory: done** (2026-10-15, same
   branch): packing the float constants walked all 256 bits of the
   shader's mask with a mispredicted branch each, and every word was a
   volatile load; ~19% of the render thread with `ReadRegisters`. Now run
   by run, copied with a vectorized byte swap: uncapped ~285 to ~325 fps
   (+14%). See "Performance: reading constants from guest memory" in
   Known issues.
2. **Allocations in interpolation**: `PixelConstantBlender::Apply`
   returns a `std::vector` per lighting upload, and a `shared_ptr` is freed
   in the blending path. Check with a new profile whether any mutex is
   still taken per upload (the per-upload `SlotOwner` lock is gone).
3. The bigger ideas above: fewer passes redrawn on interpolated frames
   (1), following the library's dirty flags (3), splitting the render
   thread (4). Discussed with the user (2026-10-15), not tried: a host-side
   copy of the device's constants and state, kept up to date by hooking
   whatever sets them, so draws don't read guest memory. Guest memory has
   to be written as now regardless (the library reads its own device
   struct), and a write missed by the hooks gives wrong rendering, so the
   byte swaps it would save (now cheap) don't justify it alone; the
   payoff would be knowing what changed since the last draw without
   comparing, which is (3) and could skip shader, pipeline and texture
   set lookups too.

### Profile of 2026-10-15, after v0.3.7 (PC, attract race)

On `8ee1120` (v0.3.7 plus docs), uncapped attract race, ~318 fps mean
over the race's seconds 3-18; the render thread at ~98% of a core, the
main thread ~65%. Flat profile (`perf record -e cycles:u -t <tid>`, 12 s
from 14 s after launch, `--log_level=off` as `run.sh` runs; a run with
info logging gave the same picture plus ~1% of its interface maps):
`itc` 77%, libc 12%, the Vulkan driver 9.4%, libstdc++ 0.9%. Nothing
dominates any more:

| Part | Share | Biggest items |
|---|---|---|
| `Passes::State::Draw` itself | 10.8% | |
| Vulkan driver | 9.4% | see below |
| libc | 12% | see below |
| Texture binding | ~8% | `GetView` 3.5, `GetSwizzledView` 1.4, `GetTextureSet` 1.4, `GetSampler` 1.2, `SwizzledSigns`, `GetTextureLayout` |
| Mirror | ~7% | `IndexRange` 2.8, `Buffers_NewestWrite` 1.4, the per-frame `mirrored` set's insert 1.3 (`unordered_set<uint64_t>`), `Mirror` 0.8, `TakePending` |
| Interpolation | ~7% (+ ~1.3% of libc) | `BlendState_ReplayConstants` 7.1, `QuaternionFromRotation` 0.5 |
| Reading state | ~6% | `ReadRegisters` 2.9, `GetTarget` 2.6, `GetHostViewportInfo` 0.7 |
| Shader lookup | ~4% | `ShaderCache::Get` 1.7, XXH3 of the microcode 1.4, `GetInterface` |
| Replay handlers and library (guest) | ~15% | render states 2.0, sampler states 1.7, the shader reference count ~3 (below), SetIndices 0.8 |

**Where libc and driver time comes from** (branch records, `-j any_call,u`,
8 s; each sample outside `itc` charged to the source of the first branch
from `itc`, by a script over `perf script -F period,ip,dso,brstack` with
the load addresses from `--show-mmap-events`; the flat profile of a
branch-record run is skewed, so take shares from a plain run):

- `Draw` calls 10.3% of the thread's time outside `itc`: the driver 6.6%
  (call sites, by the function table offset they call through and their
  arguments: `vkCmdBindDescriptorSets` 2.3%, `vkCmdDrawIndexed` 2.2%,
  `vkCmdBindPipeline` 1.3%, one more 0.8%), `memcpy` 0.7% (one site),
  `memcmp` ~2% spread over ~10 sites, some of it the driver's (tail jumps
  aren't recorded, so a driver function that jumps to `memcmp` is charged
  to the call before).
- `PlaceConstants`: `memcpy` 0.9% and `memcmp` 0.8% (writing and
  comparing constant blocks).
- Interpolation's allocations: `_int_free` from a `shared_ptr`'s control
  block 0.9%, malloc/free from `BlendState_ReplayConstants` ~0.6%,
  `vector<float>` copies 0.2%: ~1.7% with libstdc++.
- `BeginFrame` freeing 0.4%; `GetTextureSet` into the driver 0.6% (sets
  allocated); `Resolve` into the driver 0.4%.
- **The "Release" samples (`sub_822A67B0`, 2.3%)** follow a call to AddRef
  from SetVertexShader (`__imp__sub_822A8808`) in every sample: it calls
  AddRef on the new shader, then reaches Release of the old one by a tail
  jump (not recorded). So that's the locked compare-and-swap of binding a
  different vertex shader (the ~48% of binds that aren't the same one),
  ~3% with AddRef's own 0.8%.

**Next, from this** (estimates):
1. **Interpolation's allocations and the replay's lookups: done**
   (2026-10-15, branch `replay-allocations`). The lighting blender
   (`PixelConstantBlender::Apply`, inlined into
   `BlendState_ReplayConstants` with `Substitute` and `BlendProduct`; the
   matrix math in `transform.cpp` keeps its own symbols, ~1.5%) allocated a
   252-float vector per upload and copied it three times, allocated a
   `vector<bool>`, looked objects up in two `std::map`s (one cleared every
   frame, so a node per upload) and compared the upload with a copy of
   itself. It now reuses its buffers, keys both maps by hash (the
   occurrence counts go stale by frame instead of being cleared, and are
   pruned with the entries), skips that compare when blending from the
   latest value (the default), and copies what `--log_pixel_outcomes`
   reads only with it. The per-object extras are a fixed array, and the
   replay frame the render thread drops (~1,500 products, a node each) is
   freed by the main thread when it publishes the next. Checked over two
   whole attract races (~30 million lighting uploads each) against the old
   blender run side by side: every value uploaded and put back the same;
   outcomes differed only for uploads holding a NaN at the race's end
   (old "not as recorded", since NaN never equals itself; new "rejected";
   both left as recorded). Uncapped attract race, alternating, 20 s runs:
   312.7, 321.8, 320.5 fps mean before, 330.8, 331.4, 330.2 after (~+4%;
   10th percentile 271/281/281 against 289/289/285). Render thread:
   `BlendState_ReplayConstants` 7.1% to 6.0%, libc's `free` ~1.6% to
   ~0.5%.
   Then, the same day: lighting uploads loaded with `guest::CopyFloats` (a
   byte-swapping loop meant to vectorize; it didn't once inlined, see
   "Done on `blend-math`") straight into the entry, without the
   copy, stopping early when equal to last tick's bit for bit. Checked the
   same way (also with `--pixel_blend_from_latest=false`): every value
   the same. **No measurable gain**: `BlendState_ReplayConstants` stayed at
   6.0%, fps within noise (328.2, 328.2, 320.7 before; 330.4, 330.8, 331.6
   after). So the lighting uploads' loads and copies weren't where the
   remaining 6% goes; what is isn't known (the camera products'
   `Substitute`, matrix loads and stores, or the ~20 cvar reads an upload
   are the candidates): split it with a profile (functions marked
   `noinline`, or sample offsets within the function) before more work.
   **The split** (a build with `Apply`, `Substitute`, `BlendProduct`, the
   matrix loads and stores and the logs' upload hooks `noinline`):
   `PixelConstantBlender::Apply` 3.6% (~24% its loop sorting the 252
   components, ~17% the loop storing the blend, ~15% at its entry),
   `BlendState_ReplayConstants` itself 1.8% (~20% a locked increment per
   camera product, ~25% putting the lighting upload back float by
   float), `ReplayCameras::Substitute` 1.1% (~55% the `unordered_map`
   lookup: a divide, then a chain of nodes), the matrix math ~1.5%, the
   logs' hooks ~0.6%. Fixes, one at a time:
   1. **The locked increment: done.** `counts_.replayed_products`, a
      `std::atomic` read only by `--log_blend_state`, is counted only with
      it. `BlendState_ReplayConstants` 6.01% to 5.75%; fps within noise
      (325.0 to 327.4 mean over three alternating pairs, one pair the
      other way).
   2. **`Substitute`'s lookup: done.** The frame's products are in an
      open-addressed table (`ProductTable`: slots of address and index, at
      most half full, a multiplicative hash; the products stored together)
      built by the main thread. Checked over a whole attract race against
      the `unordered_map` kept beside it: the same product for all 33.8
      million lookups. Measured by profile, not fps (the user's call: fps
      runs vary more than such a change; see below): `BlendState_
      ReplayConstants` 919 to 822 thousand cycles a frame (two runs each),
      ~0.6% of the render thread.
   3. **`Apply`'s loops through local pointers: done.** The flags it
      writes are bytes, which may alias anything, so the loops reloaded
      the vectors' start and end and recomputed the size every iteration
      (seen in the disassembly: `mov (%r12)`, `mov 0x8(%r12)`, `sar`,
      before; a compare with a register after). Also switched slots by a
      per-register table instead of a scan of six ranges per component.
      Checked against the old blender as before (both blend modes; only
      the NaN uploads' outcome differs). `BlendState_ReplayConstants`
      834, 809, 823, 854, 833 thousand cycles a frame (step 2's runs) to
      813, 783, 761 (~-45 thousand, ~0.3% of the render thread; less than
      the split's ~0.9% for the whole loop: the reloads were only part of
      it).
   4. **Storing the blend and putting it back: done.** The blender
      computes the whole upload into a buffer (the blend where it blends,
      the recorded bits elsewhere) and writes it with `guest::StoreU32s`
      (the store counterpart of `CopyU32s`); the restore after the upload
      uses it too. The select loop vectorized only once written without
      branches (the blend computed for every component, chosen with `&`
      rather than `&&`): check the disassembly for `mulps`/`subps`.
      Checked against the old blender as before, bit for bit (both blend
      modes). `BlendState_ReplayConstants` 796, 790, 818 thousand cycles
      a frame (step 3, alternating) to 733, 746, 731 (~-60 thousand, ~0.4%
      of the render thread).

   Over steps 1-4, `BlendState_ReplayConstants` went from ~920 to ~737
   thousand cycles a frame (5.9% to 4.8% of the render thread), on top of
   the allocations removed before (7.1% to 6.0%, and libc's `free` ~1.6%
   to ~0.5%): from ~1.1 million cycles a frame to ~0.74, about 2.3% of
   the render thread, plus ~1.1% of `free`.

   **Measuring small changes.** Fps in alternating runs varies ~±1.5%
   between runs of one build, and so does the render thread's total
   cycles per frame (15.55 and 15.79 million for one build), so changes
   under a few percent don't show in either. Instead, profile both builds
   at the same point of an attract race and compare the changed
   function's **cycles per frame**: its share of the profile times the
   thread's cycles (perf's "Event count"), over the frames drawn in the
   profiled seconds (the `--log_fps` lines stamped within them). Two runs
   each agreed within ~3%.
2. **The mirror's per-draw work** (~7%): cache each draw's index range
   (lowest, highest) by index buffer, count and format, dropped on a
   memcpy write as the mirror's copies are; a flat table instead of the
   `mirrored` `unordered_set`.
3. **Texture binding** (~8%): cache what `GetView`, `GetSampler` and
   `SwizzledSigns` work out per fetch constant (its 6 words).
4. **Descriptor binds** (2.3% in the driver plus our share of `Draw`):
   bound for ~62% of draws, mostly for new constant offsets. Push
   descriptors, or constants reached through one buffer with offsets in
   push constants, would cut them; a change to how the translated shaders
   read constants.
5. **The shader reference count** (~3%): the counts' atomics can't simply
   go (Release has ~40 callers, some on other threads). One option: hold a
   reference of our own to each vertex shader once bound, so SetVertexShader
   needn't touch counts; shaders would then never be destroyed (the user's
   call).
6. **Shader lookup** (~4%): key by object and declaration (Known issues).

### Next: interpolation's blending logic and matrix math

After `replay-allocations` (merged 2026-10-15), measured with
`itc_recomp/tools/profile_render_thread.sh` (uncapped attract race, render
thread ~15.5 million cycles a frame): `BlendState_ReplayConstants` ~772
thousand cycles a frame (5.0%; `PixelConstantBlender::Apply`,
`ReplayCameras::Substitute` and `BlendProduct` are inlined into it), and
the matrix math in `transform.cpp` ~290 thousand (1.9%):
`QuaternionFromRotation` ~100, `SplitViewProjection` ~53, the transform
decompositions ~60, blends ~45, `Multiply` ~16.

Leads, not yet measured:

- **Decompositions repeated every frame** (done, see "Done on
  `blend-math`"). `BlendProduct` decomposes both
  ticks' world matrices (`ScaledTransformFromWorldMatrix`, quaternions) and
  splits both ticks' cameras (`SplitViewProjection`) for every product on
  every frame, though they depend only on the tick's inputs; at ~330 fps a
  tick is ~11 frames. And ~1,500 products a frame share a handful of
  cameras, each split again per product. Computing them once per tick (or
  per distinct matrix) would leave only the blend and the multiply per
  frame.
- **The lighting blender's per-component sorting** (in `Apply`): within a
  tick, render-only frames upload the same values, so its outcome and
  which components keep the tick's value could be kept per entry and
  reused while the upload is bit for bit the one sorted.
- A fresh split first (done on `blend-math`, below): mark `Apply`,
  `Substitute`, `BlendProduct` and the logs' upload hooks
  `[[gnu::noinline]]` in a temporary commit (as `adf4a8f` did), profile,
  revert (`76fe176`'s markers were only reverted on `read-registers`).

#### Done on `blend-math` (2026-10-15)

Measured with `profile_render_thread.sh`, two or three runs a build,
alternating. Builds with `PixelConstantBlender::Apply`, `Substitute`,
`BlendProduct` and the logs' upload hooks `[[gnu::noinline]]` (a temporary
commit, `76fe176`, removed on `read-registers`: all of this section's and
"Register reads"' profiles were taken with it), so the parts show apart. **The split** (thousand cycles a frame):
`Apply` ~370-410, `BlendState_ReplayConstants` itself ~250, `Substitute`
~85, and the math that depends only on the tick ~235:
`QuaternionFromRotation` ~130, `SplitViewProjection` ~50, the
decompositions ~60. All of `interpolation::` ~1,170.

1. **Blends planned once a tick.** The main thread, after `MatchTick`,
   works out each paired product's `ProductPlan` (both world matrices
   decomposed, which path the world takes: blend, wheel or the tick's) and
   splits each distinct pair of view-projections once (`CameraPair`,
   keyed by both matrices' bits); the replay frame shares the tick's
   plans. The render thread only blends and multiplies. Checked over two
   whole attract races (17 million products) against the old
   `BlendProduct` run beside it: the same matrix, parts and stats for
   every one. `interpolation::` ~1,170 to ~950 thousand cycles a frame.
   (`--log_blend_state`'s sample of rejected cameras is now logged once a
   tick per distinct pair rather than per product.)
2. **Each camera blended once a frame** (`CameraBlends`, by the plan's
   camera index, stamped by frame). Checked the same way (15.5 million
   camera products, the same view-projection and eye). ~993 to ~920.
3. **The byte swap by hand.** An annotated profile of `Apply` (by
   address: the build has no line info, and `perf annotate -s <name>`
   silently annotated another function) showed the sorting loop ~28%
   (mostly the per-component "unchanged" test), loading the upload
   (`CopyFloats`) ~16% and storing it (`StoreU32s`) ~8.5%, both a scalar
   `bswap` per word: the loops in `guest.h` vectorize alone but stayed
   scalar once inlined. They now swap four words with SSSE3's `pshufb`
   (`_mm_shuffle_epi8`, part of x86-64-v2), the rest one by one; scalar
   without SSSE3. Checked against the scalar loops for counts 0-70 at
   every alignment. `CopyU32s` is also how the renderer reads registers
   and constants. Its old loop did have `pshufb` in it, but only on the
   path for a range crossing 0xE0000000, which never happens: the
   ordinary path was the scalar one (~60% of `ReadRegisters`' samples in
   it). The hand-written swap serves both paths. Per symbol, thousand
   cycles a frame, three runs each: `Passes::State::Draw` (constants)
   ~1,733 to ~1,501, `Apply` ~409 to ~351, `BlendState_ReplayConstants`
   ~221 to ~184 (putting the lighting upload back), `ReadRegisters` ~453
   to ~417 (now ~46 plus ~371 in `CopyBlock`, no longer inlined: filter
   profiles by more than one name). The render thread ~330 thousand
   cycles a frame lower overall (~2.2%).
   `ReadRegisters` copies ~350 words a draw and is still ~417 with the
   copy vectorized (most samples on the `pshufb` after the load): see
   "Register reads" below for why, and the fix.
   Uncapped attract races, alternating: 341.8, 345.7, 346.1 fps mean
   against 332.1, 337.9, 335.6 before (~+2.8%).

#### Register reads (`read-registers`, 2026-10-15)

`ReadRegisters` (`native/state.cpp`) fills the renderer's `RegisterFile`
from the device's shadow: 7 register blocks and the fetch, bool and loop
constants, 349 words. Measured with `--native_renderer_log_register_reads`
(uncapped attract race):

- **It runs once per draw**, ~2,150 times a frame (`AfterDraw`; command
  18's items come one per command), copying ~750,000 words a frame
  (~3 MB). Only ~9,000 of them change (~4 a read); 40% of reads change
  nothing. By block, the share of reads changing it: fetch constants
  ~60%, 0x2180 ~27%, 0x2200 ~12%, 0x2100 ~10%, the rest ~1% or 0.
- **Not mainly memory stalls.** Timed with the time stamp counter, a copy
  took ~300 cycles and the same copy repeated right after ~233, so
  ~three quarters of it was the copy itself, not waiting on the shadow
  (the guest's library, which writes the shadow, runs on the render
  thread too, so the shadow should be in cache). The timing itself proved
  unreliable for small copies (it read ~200 whatever the code did, so
  the fences dominate), so changes were judged by profile; `perf stat`
  cache-miss counts weren't taken.
- **What changes is known already**: the library's setters mark what they
  write in three dirty masks (device +16, +24, +32), and its draw flushes
  the marked units and clears the masks (layout in "The register shadow
  as a whole", `docs/gpu-functions.md`). In exclusive mode our
  `DrawPackets_IndexedDraw` clears them itself, so it now hands the masks
  (with the bits the shader load returns) to `MarkDeviceDirty` first. Other
  library paths that flush (other draws, clears, predicated draws) go
  through the flush helpers, whose hooks mark what they're given
  (`sub_822ADE30`, `sub_822AE070`, and `sub_822ADC08`, which writes 0x2203
  itself), and InlineDraw marks the words it overwrites in `regs`.
  `ReadRegisters` copies only what's marked: ~15 words a read.

Checked over a whole attract race (boot, title, race; ~2,200 reads a
frame) against reading every block whole after each read: 0 words
differ. The first try differed only in 0x2203 (~1 a frame), until
`sub_822ADC08`'s hook marked it. Menus, other modes and the cockpit
weren't covered by the check.

Profiles, three runs each, alternating, thousand cycles a frame:

| Build | `ReadRegisters` (+`CopyBlock`) | `DrawPackets_IndexedDraw` | Marking | Render thread |
|---|---|---|---|---|
| `blend-math` | 431, 403, 478 | 53, 49, 56 | | 14,594, 14,929, 14,657 |
| dirty runs, by block (first try) | 239, 228, 239 | 74, 79, 83 | ~3 | 14,486, 14,696, 14,377 |
| dirty bits through a table | 141, 167, 151 | 75, 73, 87 | 30, 22, 26 | 14,133, 14,380, 14,528 |

So about −215 thousand cycles a frame (~1.5% of the render thread); the
thread's total fell ~380 on average, within its noise. The first try
converted the masks per block with `__builtin_bitreverse64` (the library
numbers units from the top bit) and looped over all 10 blocks, finding
runs: ~110 cycles a read for ~15 words, spread over the loop's
bookkeeping (no x86 instruction reverses bits, so each is a long run of
shifts and masks). Now the draw only ORs the masks, and the read walks
each mask's set bits through a 3×64 table (shadow offset, register,
words).

Left, unexplained or not done: `ReadRegisters` is still ~150 thousand
cycles a frame (~70 a read) for ~15 words, and the marking in the draw
~25 more; `MarkRegistersDirty` grew from ~3 to ~25 between the two builds
for no reason found (it's called from the flush helpers' hooks only).
Readers of `RegisterFile` could also learn what changed (the masks say
it), e.g. to skip comparing the fetch block when no fetch bit was set;
not tried.

#### Against v0.3.7 (2026-10-15)

v0.3.7 built from its tag in the same tree (only `itc_recomp/src`
differs; same compiler, ThinLTO) against `read-registers` at `4f8f436`
(`blend-math`, the register reads, the temporary noinline markers
removed). `profile_render_thread.sh`, three runs each, alternating, after
a warm-up run of v0.3.7 for its shader cache. The draws are the same
(~2,100 a frame in the attract race, from the log), so cycles a draw
compare directly; the cockpit's lower frame rate is its larger number of
draws at this cost a draw.

| Build | Render thread, million cycles a frame | Per draw | Frames in the 12 s |
|---|---|---|---|
| v0.3.7 | 16.10, 16.12, 16.02 (16.08) | ~7,660 | 3,793, 3,798, 3,815 |
| current | 14.20, 14.07, 14.29 (14.19) | ~6,760 | 4,284, 4,333, 4,241 |

−11.8% of the render thread's cycles a draw, ~+13% frames. Biggest
changes by symbol (one pair, thousand cycles a frame):
`BlendState_ReplayConstants` 1,092 to 633 (planned blends, cameras once a
frame, the byte swap, and its parts inlined again), `State::Draw` 1,801
to 1,425 (constants' byte swap), `ReadRegisters` 427 to 148, `GetTarget`
453 to 225 (not changed on purpose; probably `CopyU32s` inlined into it,
not checked), `QuaternionFromRotation` and `SplitViewProjection` ~130 to
~11 (once a tick). Larger: `IndexRange` +46, `ShaderCache::GetInterface`
+35, `Buffers_OnCommand` +36 (noise or inlining, not looked into).

**To investigate: reusing the lighting blender's decisions within a tick.**
After the above, `Apply`'s per-component loop (does each component blend,
keep the tick's value, or reject the upload) is still ~28% of it, and the
light slot checks ~11%. The decisions depend only on the previous tick's
values (fixed within a tick), the upload as recorded and a few options, so
an entry could keep them with the upload they were made on and reuse them
while a frame's upload is bit for bit the same (the blend itself is still
computed per frame with that frame's amount). Not keyed by tick alone: the
light slots hold the previous tick's lights on a logic pass and the tick's
own after, so the first frame after it must decide again. Check against
the old blender over whole attract races, both blend modes; keep
`--log_blend_state`'s rejection counts per frame. Unmeasured: comparing
~1 KB per upload may eat most of the gain.

### Profile of 2026-10-15, after `read-registers` (`mirrors-fix`)

On `843af98` (`main` with `blend-math` and `read-registers`),
`profile_render_thread.sh`, two runs: 14.43 and 14.13 million cycles a
frame (4,193 and 4,295 frames in the 12 s). `itc` 77%, libc 11.3%, the
Vulkan driver 10.7%, libstdc++ 0.8%. By group (share of the render
thread, two runs):

| Part | Share | Biggest items (run a) |
|---|---|---|
| Replay handlers and library (guest, `sub_8...`) | ~26% | Release 2.4, `8258FBD0` 2.1, `8258FC30` 1.7 |
| `Passes::State::Draw` itself | ~10.2% | |
| Texture binding | ~8.9% | `GetView` 3.5, `GetTextureSet` 1.6, `GetSwizzledView` 1.5, `GetSampler` 1.3 |
| Mirror | ~8.4% | `IndexRange` 3.4 (~485 thousand cycles a frame), `Buffers_NewestWrite` 1.5 (~215), the `mirrored` set's insert 1.5, `Mirror` 1.2 (~172), `TakePending` 0.7 (~100) |
| Shader lookup | ~4.7% | XXH3 of the microcode 1.6, `ShaderCache::Get` 1.3, `GetInterface` 1.0 |
| Interpolation | ~4.8% | `BlendState_ReplayConstants` 4.4 |
| Reading state | ~3.1% | `GetTarget` 1.6, `ReadRegisters` 1.1 |
| Pipeline lookup | ~1.0% | the `PipelineKey` `unordered_map` 1.1 |

Also: memcpy's write tracking (`RecordWrite`: the `std::map` of segments,
its `operator[]` and tree rebalancing) ~1%. The script runs with info
logging (it counts frames from the `fps` lines), so its profiles include
`collect_interfaces`' `std::map` (`interpolators_by_shader`, 1.0%), which
`run.sh` (`--log_level=off`) doesn't pay; the same in every build, so
comparisons hold.

So the mirror is now the largest of our own parts after `Draw`, ~1.2
million cycles a frame (~560 a draw at ~2,150 draws). Where it goes, per
draw:
- `IndexRange` scans every index of the draw (vectorized, `passes.cpp`)
  to find the lowest and highest vertex, though most index buffers are
  static and drawn again every frame (and several times a frame).
- `Mirror` inserts each range into `mirrored` (an `unordered_set`, a
  node allocated per new range and all freed at the frame's start) to copy
  a range at most once a frame, then looks it up in the frame's
  `GuestMirror::copied` and asks `Buffers_NewestWrite` (a `std::map`
  lookup, after `TakePending`) whether memcpy wrote it since.

Plan, to measure first (`CLAUDE.md`: logs before changes): count per
frame the index ranges seen, the distinct ones, how many were written by
memcpy since last seen, and the memcpy writes taken. Then:
1. Cache `IndexRange`'s result by (address, count, format, endian,
   restart), valid while memcpy hasn't written the range (its sequence, as
   the mirror keeps).
2. Replace `mirrored` with a flat open-addressed table stamped by frame
   (as `ProductTable`), so nothing is allocated or cleared.
3. Answer "written since?" cheaply when nothing was written since the last
   question (`Buffers_Sequence` unchanged), before the `std::map` lookup.

**Measured** (`--native_renderer_log_index_ranges`, uncapped attract race,
`itc_744.log`; per frame, the race's 5 s lines):
- **Index ranges are almost never rewritten.** ~1,700-2,200 scans a frame
  (~1.5-2.5 million indices), ~650-830 distinct ranges. Of the scans,
  ~61% repeat a range already scanned this frame, and ~100% find it
  unwritten by memcpy since its last scan (first seen and written since:
  0.0% when rounded). An unwritten range's kept result was the same as the
  scan for every one (0 mismatches over the race), so nothing but memcpy
  writes them. A cache would remove nearly all of `IndexRange`.
- **memcpy writes ~130-260 times a frame**, so the sequence is never
  unchanged between a range's copy and the mirror's next question (0% of
  ~1,100 questions a frame): plan item 3 as written gains nothing.
- **The mirror** is called ~4,800-6,100 times a frame (index and vertex
  ranges), 81% for a range already copied that frame (the `mirrored`
  insert finds it), and asks `Buffers_NewestWrite` ~950-1,150 times.

So a cache's check matters: `IndexRange` costs ~230 cycles a draw (~485
thousand a frame over ~2,100 draws), and `Buffers_NewestWrite` ~195 a
question (~215 thousand over ~1,100), about the same. Checking each cached
range with `Buffers_NewestWrite` on every draw would gain little. Instead:
- check each cached index range once a frame, as the mirror already does
  for the same range (same address and size; the mirror already assumes
  memcpy doesn't write a range between two draws of a frame, which
  `--native_renderer_check_late_writes` checks): ~61% of draws then need
  only the lookup;
- and make the "written since?" question itself cheaper, e.g. a table of
  the newest write's sequence per 4 KB page (1 MB for 512 MB), so a range
  is a few loads instead of a `std::map` search; a write in the same page
  but outside the range only costs an extra scan or copy. Count how often
  that happens before choosing it.

**Done: the index range cache** (`IndexRangeCache` in `passes.cpp`,
`--native_renderer_cache_index_ranges`, on by default). An open-addressed
table by address, count and format (emptied if it reaches 32,768 ranges)
keeps each range's lowest and highest index, the write sequence when
scanned, and the frame it was last found unwritten in. A draw whose range
was found this frame takes the result; otherwise `Buffers_NewestWrite`
decides once a frame whether it's scanned again. Checked with
`--native_renderer_check_index_cache` (scanned anyway and compared;
removed 2026-10-15 once the mirror work was done, in its last version
`2b7f2a2`): two whole attract races, ~62 million cached results, 0 differ.
Profiles, one binary with the cache off and on, three runs each,
alternating (`ITC_ARGS`, a new option of `profile_render_thread.sh`),
thousand cycles a frame:

| Cache | Render thread | `IndexRange` | `DrawIndexRange` | `Buffers_NewestWrite` |
|---|---|---|---|---|
| off | 14,463, 14,539, 14,207 (14,403) | 495, 458, 470 | 16, 17, 11 | 204, 193, 199 |
| on | 14,016, 13,452, 13,793 (13,754) | 0 | 83, 83, 86 | 313, 308, 308 |

So `IndexRange`'s ~475 is gone, for ~70 more in the lookup and ~110 more
in `Buffers_NewestWrite` (the once-a-frame checks): ~−290 directly, ~2% of
the thread (the total fell ~650, ~4.5%, but that's within its noise added
to the direct gain). Next: the mirror asks the same question of the same
range in the same draw (`Mirror` of the index range), so the cache's
answer can be passed to it, removing the added ~110.

**Done: the cache's answer passed to the mirror.** `DrawIndexRange`
keeps what `Buffers_NewestWrite` said of the draw's index range when it
asked (`draw_index_newest`, reset at each draw's start), and `Draw` passes
it to `Mirror`, which asks itself only without it (or when the range was
clipped at the end of physical memory). This relies on what the mirror
already did: no memcpy write to an index range between two points of one
draw. Checked over an attract race with `--native_renderer_verify_mirror`
(every range not copied compared with guest memory: 2.1 million, 0
differ) and `--native_renderer_check_index_cache` (4 million, 0 differ).
Profiles against the previous commit (separate binaries, three runs each,
alternating), thousand cycles a frame:

| Build | Render thread | `Buffers_NewestWrite` | `DrawIndexRange` |
|---|---|---|---|
| cache only | 13,795, 13,758, 12,584 | 338, 354, 311 | 139, 129, 121 |
| answer passed | 13,441, 13,656, 13,613 | 230, 285, 272 | 77, 94, 93 |

So ~−115 for the two together, as expected (the thread's totals are too
noisy to show it: the third "cache only" run drew ~8% more frames than
any other). Over both steps the mirror's per-draw work is ~−400 thousand
cycles a frame (~3% of the render thread).

### Hypothesis to test: the mirrors updated once a tick (the user's, 2026-10-15)

**Outcome:** done and merged (`mirrors-fix`, released in v0.3.8, opt-in
and on in `run.sh`); the handover below is kept as written, with the
results after it ("Test results", "The experiment", "Draws' mirror work
removed", "A race the user drove").

For a new session to test. Read "Buffers" and the "Guest memory is
mirrored" decision above first, and "Render command recording" and
"Interpolation" in `functions.md`.

**Where things stand.** Branch `mirrors-fix` (not merged; whether to
start from it or merge it first is the user's call; ask before creating
branches) has, on top of `main`:
- the render thread profile after `read-registers`: the mirror ~8.4% of
  the render thread, the largest of our parts after `Draw`;
- `--native_renderer_log_index_ranges` (counts below);
- the index range cache (`IndexRangeCache`, `DrawIndexRange`), and its
  write check passed to `Mirror` (`draw_index_newest`): ~-400 thousand
  cycles a frame of ~14,000 together (~3%), checked over attract races;
- `--native_renderer_check_index_cache`, temporary (removed after the
  mirror work, see below);
- `profile_render_thread.sh` takes extra game flags in `ITC_ARGS`.

What the mirror still costs a frame on that branch, thousand cycles:
"written since?" (`Buffers_NewestWrite`, a `std::map` search of every
segment memcpy ever wrote) ~260 and `TakePending` ~95, `Mirror`'s own
time ~140 (mostly, by inference, the inlined lookup in
`GuestMirror::copied`), the `mirrored` set's inserts ~1.8%. All of it is
bookkeeping: ~6,000 `Mirror` calls a frame (81% for a range already handled
that frame) decide what to copy, and ~0.1 MB a frame is copied.

**Why the question is asked at all.** The mirrors (two 512 MB host-visible
buffers made once at start, one per frame in flight, used in turn) are
copies of guest memory, since RADV won't import it. The game writes vertex
and index data with plain memcpy (as on the 360, where the GPU read the
same memory) and never says what changed, so every copy is checked before
use: has memcpy written its range since? In every race checked, no range's
contents changed after loading (sections reloaded in place carry the same
data); the check guards memory reused for other data.

**The hypothesis.** The game was written for a fixed 30 Hz tick: logic
(and, the hypothesis says, everything that writes what the GPU reads)
runs only on ticks. So guest memory the GPU reads only changes on logic
passes, and the mirrors need updating once a tick, not checked draw by
draw. Then the mirrors work as front and back buffers alternating by tick:
the GPU draws from a mirror that doesn't change while it's in use, and the
other takes the writes. Draws do no mirror work at all.

**What's known that bears on it** (`functions.md`):
- Every frame is a main loop pass. Logic passes (a 30 Hz tick due) run
  Update and Draw; render-only passes (in races) skip Update, and Draw
  records a new command list from the same game state. The command list is
  double-buffered and flipped every pass; the render thread replays the
  previous pass's list while the main thread runs the next pass. So drawing
  runs one pass behind, overlapping the next pass's logic, as it did one
  tick behind on the 360 (not a new hazard from interpolation; an earlier
  claim in this session that it was is wrong).
- memcpy writes into the physical views (`F7xxxxxx`-`FCxxxxxx`) ~130-260
  times **per render frame** (`--native_renderer_log_index_ranges`'s
  "memcpy writes taken", uncapped attract race, ~330 fps), yet index ranges
  drawn were 0% rewritten. So something writes physical memory every frame,
  most likely not vertex or index data (textures are written by the same
  memcpy, see `texture-handling-optimisation.md`; or dynamic data). If
  those writes happen on render-only passes and include ranges draws read,
  the hypothesis fails for them.
- Pass type and number: `frame_pacing::GameMessages_IsRenderOnlyPass()`,
  `GameMessages_PassNumber()` (`game_messages.h`; main thread). The memcpy
  wrapper is `sub_825A7410` in `native/buffers.cpp` (writes into the
  physical views are queued in `g_pending` with their caller).

**Test first, with logs** (CLAUDE.md: logs, not guesses). Add a flag that,
for each memcpy into the physical views, records: the calling thread (main
or which other), the pass number and whether it's a logic or render-only
pass, the caller (`lr`), and whether the range overlaps a vertex or index
range drawn in the last two frames (the mirror's ranges; a per-frame list
is enough) or a texture. Summarize every 5 s by caller: writes and bytes
on logic and on render-only passes, by thread, and how many overlapped
drawn ranges. Run attract races and, with the user, a driven race (new
sections stream in as the car moves; attract races may not cover it).
- Hypothesis holds if every write overlapping drawn vertex or index data
  is on a logic pass (or is the same contents again). Writes on render-only
  passes into other memory (textures, non-drawn data) don't matter to the
  mirror.
- Also count writes landing in a range the render thread is drawing from
  at that moment (the list being replayed): if any, the game relies on the
  overlap being harmless, and the snapshot point below matters.

**If it holds, the design** (estimates; work out each before building):
- Each mirror is guest memory as of a pass's recording. A write goes into
  the back mirror at once (from the memcpy wrapper, on its thread; the
  mirrors are mapped, any thread can write) or is queued, and the front
  gets the writes it missed when it becomes the back.
- The swap: at the first pass after a logic pass whose list the render
  thread replays. Before writing into the old front, wait for the fences
  of frames still drawing from it (at most two frames in flight).
- Inline vertices are in the mirror too (`inline_offset`, written every
  frame by the render thread): move them to a small per-frame-slot buffer,
  or they'd change under a frame in flight.
- The index range cache (`mirrors-fix`) drops entries overlapping each
  applied write instead of asking `Buffers_NewestWrite`; the `mirrored`
  set, `GuestMirror::copied` and the per-draw `Buffers_NewestWrite` go.
  `g_writes` (the segment map) stays for the logging tools.
- At start: copy every segment memcpy wrote before the mirrors existed.
- It still assumes vertex and index data only arrives by memcpy (as now).

**Checking it**: `--native_renderer_verify_mirror` (every range not copied
compared with guest memory; adapt it to compare each drawn range with
guest memory as of the snapshot), `--native_renderer_check_late_writes`,
and the pictures (a driven race by the user). **Measuring it**:
`profile_render_thread.sh`, old and new binaries in their own directories,
three runs each, alternating; expect the mirror's ~1 million cycles a
frame mostly gone.

**A cheaper fallback** if it doesn't hold: a table of each 4 KB page's
newest write sequence instead of the `std::map` search (count first how
often a write lands in a cached range's page but outside the range), and a
flat table stamped by frame instead of the `mirrored` set.

**Test results, attract races (2026-10-15).** `--native_renderer_log_physical_writes`
(`buffers.cpp`): each memcpy into the physical views with its thread
(main, or numbered by first write), pass number and type, caller, the
render frame and whether a list was being replayed then, and whether the
bytes were already the same; checked a frame later against the vertex and
index ranges the mirror got in the frames from two before the write's to
one after, against the replayed frame's ranges, and against the texture
cache. Every 5 s a summary, overall and by caller and thread; each write
changing drawn data on a render-only pass or under the replay is logged
on its own line with the frames that drew it. Two uncapped attract races
(40 s and 70 s, logs `itc_753`, `itc_754`):
- ~75,000 writes a second (~250 a frame at ~300 fps, ~10-15 MB a second).
  ~95% are from one thread (thread 3; callers `822AE900`, `825A17FC`,
  `822B0278`, `825903B4`, `822A00AC`, `822A0060`, `8229F8D8`, `8229F88C`),
  steadily on every pass, never into drawn ranges or textures: by its
  steadiness most likely audio (not checked). Another thread (2; `820ED6B8`,
  `820ED910`, `820ED920`) writes textures in bursts, on any pass.
- **Drawn data does change on render-only passes**: 35 writes in the
  70 s race, in 4 bursts (one pass each, e.g. pass 1785, 12761), on the
  main thread from game code (`820E78E8`, `820E7DDC`, `820E7F5C`,
  `820E82B0`, `820E8368`, `820E8420`, `820E84D8`, `820E8590`: in
  `sub_820E7598`, `sub_820E7CA0`, `sub_820E8028`, see "Buffers"; all three
  called from one function near `820E86F0`-`820E8890`; small vertex
  ranges, 64 bytes to 26 KB, in pairs of the same size; what they hold
  isn't known). So with the mirrors swapped only on ticks, those ranges
  would be drawn from a mirror without them.
- But **every one of those ranges was drawn only in the frame after the
  write** (new data, drawn first by that pass's own list), never in the
  frames before it; logic passes' writes into drawn ranges likewise. **No
  write landed in a range the list being replayed reads** (0 in both
  races). So no range was changed while a frame could be drawing it.
- Not yet covered: a race the user drives (sections streamed as the car
  moves).

So the hypothesis's core holds in these races (nothing a frame in flight
reads is ever written), and only its timing doesn't (new data also
arrives on render-only passes).

**The experiment (2026-10-15).** `--native_renderer_mirror_by_tick` builds
the design above: draws read the front mirror, swapped as a logic pass's
list starts replaying (the main thread queues each list's pass type as
`Renderer::WaitForFrame` hands it over, `Renderer_OnListHandedOver`; the
render thread takes one per frame, `Renderer_OnReplayStart`); memcpy's
writes are applied at frame starts to the back mirror (free with two
frames in flight as long as it wasn't the front in the frame before);
inline vertices get half their region per frame slot. Draws do no
copying. `--native_renderer_verify_mirror` is no use with it (it slows
frames below 30, so every frame swaps), so drawn ranges are instead looked
up in the writes the front hasn't had (exact, given memcpy is the only
writer). 70 s attract race (`itc_756`): 150 swaps per 5 s (one a tick) in
~1,700 frames; drawn ranges missing a write: none in logic passes' lists;
in render-only passes' lists, exactly the loader bursts above (vertex
ranges of 768-25856 bytes, `820E7xxx` callers), each stale for the 2
frames left in its tick. Pictures otherwise right.

`--native_renderer_mirror_to_both` (with it): the writes taken at each
frame's start also go into the front mirror. 70 s race (`itc_757`): no
drawn range missing a write; the write log saw 45 writes changing drawn
data, every one drawn only in the frame after, none under the replay.
Speed, uncapped attract races alternating (seconds 3-18): default 360 and
358 fps, to both 372 and 366 (~+3%, near the run-to-run noise; draws
still make the `mirrored` set's inserts and the index range work, which
this would let go).

**Draws' mirror work removed (2026-10-15, `7d62381`).** With
`--native_renderer_mirror_by_tick` and none of the mirror's diagnostics,
draws skip the index range (`DrawIndexRange`), the vertex sizes and
`Mirror` (the `mirrored` set) altogether, as the mirrors need nothing from
them; the segment map (`g_writes`) isn't kept either (only diagnostics
and `--native_renderer_log_texture_writes` read it). With the mode,
`--native_renderer_verify_mirror` looks each drawn range up in the writes
the mirror hasn't had (fast; none missing over a 60 s race with
`--native_renderer_mirror_to_both`). Render thread profiles, same build,
alternating, `ITC_ARGS` off and on (`--native_renderer_mirror_by_tick=true
--native_renderer_mirror_to_both=true`), Mcycles a frame: off 13.643,
13.679, 13.622; on 11.649, 11.537, 11.722: **-14.7%** (frames drawn in the
12 s window 4,445 against 5,113). Gone: `Buffers_NewestWrite` ~245k,
`Mirror` ~185k, `TakePending` ~90k, `DrawIndexRange` ~70k cycles a frame,
and the inlined set and map work. Left: `Buffers_OnCommand` ~90k a frame
(see "To check" below).

**`Buffers_OnCommand`'s cost (fixed 2026-10-16).** Perf's attribution
was right; the `perf annotate` above was likely read against a rebuilt
binary. In a fresh profile the samples sat on the function's own prologue
and epilogue: ThinLTO had inlined `Seen` and the summary into it, so it
saved six registers, set up a 0x258-byte frame and a stack protector on
every replayed command (~27,000 a frame) before checking the flag.
`debug::ReplayStats_OnCommand` (`--log_replay_commands`) did the same,
~0.6% of the render thread. Both now check their flag and call a
`[[gnu::noinline]]` function for the logging; the check inlines into the
call site. Render thread, Mcycles a frame, alternating: before 11.442,
11.979, 11.883, 11.867, 11.796; after both fixes 11.699, 11.367, 11.592
(~-2%); neither symbol shows in the profile any more. Looking for the same pattern elsewhere (the render
thread's hot functions ranked by the share of their samples in their
first 64 bytes) found `GetRenderer()` (`renderer.cpp`): 1.3-2% of the
thread, 58% of it at entry, as `new Renderer()` and `Init` were inlined
into it with a 0x948-byte frame, set up on every call (several per
command). The start is now `[[gnu::noinline]] StartRenderer()` and
`GetRenderer` inlines into its callers: one profile each, 11.732 before,
11.402 after (~150 thousand cycles a frame, its share before). The rest
with many samples at entry (`sub_8258FD48`, `sub_8258FF50`) are wrappers
with small frames, not this pattern. What was written before:

**To check: `Buffers_OnCommand`'s cost.** `Buffers_OnCommand`
(`native/buffers.cpp`) is called from `Renderer_OnCommand`
(`native/renderer.cpp`) for every replayed command, and without
`--native_renderer_log_buffers` it returns at once (one cvar read). Yet
profiles give it 78-105 thousand cycles a frame (~0.7% of the render
thread; the six profiles above, mode off and on alike), and it was +36
thousand against v0.3.7 ("Against v0.3.7"). `perf annotate` of it
(`by_tick_2`) put most of its samples at addresses inside other
functions' code (`Seen`, `Buffers_OnFrameStart`), so perf's symbol
attribution in this ThinLTO build (code in `.ltext`) may be wrong, and
the cycles may belong elsewhere. To find out: profile a build with the
check moved to the call site (`if (Buffers_Logging())` around the call in
`Renderer_OnCommand`), or with `-DITC_LTO=OFF`, and see whether the 90k
goes or moves to another symbol; `perf report --sort sym,srcline` may
also show where the samples really are.

**A race the user drove (2026-10-15, `itc_769`)**, with both flags and
`--native_renderer_verify_mirror`, capped at 144: ~3.5 minutes, ~140 s
racing; no drawn range missing a write in any 5 s summary, and the
pictures looked right to the user.

Was suggested first (for the user to decide): since writes only
land in memory no frame in flight reads, each write could be copied into
both mirrors when memcpy makes it (on its thread), and draws would do no
mirror work at all; no swap and no fence wait. Inline vertices would
still need their own per-slot buffer, and the start-up catch-up copy
stays. Its risk is the same assumption as now (vertex and index data only
by memcpy), plus that nothing rewrites memory a frame in flight reads,
which `--native_renderer_check_late_writes` and
`--native_renderer_verify_mirror` would catch. Separately, thread 3's
writes are ~95% of what `TakePending` and the segment map handle each
frame for nothing.

Checking a change: run the old code beside the new over a whole attract
race and compare every result, as for steps 1-4 (the lighting blender:
cherry-pick `2c82ae6` and `5942f31`, then `--check_pixel_blender=true`; the
known difference is the outcome label of NaN uploads at the race's end;
the product table: `13ac56f`, which checked every lookup).

## Resolution (branch `unlocked-resolution`, 2026-10-16)

Goal: `--width`/`--height` and fullscreen. The game draws its 3D view at
1024x576 (2x MSAA) and its final frame at 1280x720; the plan is to scale
every render target and resolve destination by a factor per axis, for
any 16:9 size, and to letterbox other shapes (a real widescreen mode would
mean changing the game's projections, HUD and culling: not planned).

**The translator's scale.** The SDK's `SpirvShaderTranslator` takes
`draw_resolution_scale_x/y` as whole numbers, but uses them as floats
(pixel position `floor(gl_FragCoord) / scale`; scaled textures'
unnormalized coordinates and sizes times the scale), except for memory
export (eM writes, made once per guest pixel by `fragcoord mod scale`) and
the EDRAM emulation's tile sizes, which we don't use. So a fractional
scale only needs memory export to be absent.

**No shader writes memory** (2026-10-16, `itc_783.log`): with
`--native_renderer_disk_cache=false` so every shader is analyzed again,
the "analyzed ... shader" lines (`shaders.cpp`) show eM 00 and no stream
constants for all 74 shaders of the boot, title screen and an attract
race; the user's menu session (`itc_784.log`, 4 shaders more) found
none either. Races in cockpit and bumper view aren't covered yet.

**Nor does any fetch the translator would scale** (`itc_785.log`, the
same run with the line extended): no shader uses unnormalized texture
coordinates, getWeights or texel offsets, and no draw uses the pixel
position (`0.0 draws a frame with PsParamGen`). With these, the
translator's scale changes nothing in this game's translations, so
shaders are translated at scale 1 and the scaling is done in our renderer
alone (no SDK patch). A shader that does use them shows in the
"analyzed" lines.

**Done: the scale** (2026-10-16). `--width`/`--height` set the window
in pixels (one alone keeps 16:9; neither, 1280x720), and the drawing
scale is the largest that fits the window's pixels (the swapchain's size
at start), so 1920x1080 draws at 1.5x. (A `--resolution_scale` flag was
dropped as redundant, the user's call: fullscreen with `--width`/`--height`
already draws at a resolution other than the screen's.) `Passes` keeps each target's guest extent and
draws into an image of it times the scale; viewports are scaled, scissors
and resolve rectangles scaled with each edge rounded the same way in
source and destination (so resolves stay one-to-one copies), resolve
destinations are made at the scaled size by the texture cache
(`set_resolution_scale`), and the depth bias slope is multiplied by the
scale (as the plugin does). Shown frame and main view sizes scale with it.
Checked in attract races: at 1920x1080 every target and resolve at 1.5x
(3D view 1536x864, glow 384x216, shadow maps 1536x1536, frame 1920x1080),
the dumped frame right (`itc_786.log`; the user agreed it looks fine); at
the default, the same sizes as before, the 144 cap held, no failed draws
(`itc_787.log`).

**Done: fullscreen and black bars** (2026-10-16). Our window follows the
SDK's `--fullscreen` (default true, as the SDK's own window; `run.sh` and
the test command in `CLAUDE.md` pass `--fullscreen=false`, and a bare
`--fullscreen` or `--no-fullscreen` works): borderless at the desktop's
resolution. In fullscreen `--width`/`--height` set only the drawing
resolution; without them the scale fits the screen.
**High pixel density**: the first fullscreen run (`itc_789.log`) got a
2648x1490 swapchain on this PC's 3840x2160 screen at 145% desktop scaling:
without `SDL_WINDOW_HIGH_PIXEL_DENSITY`, SDL gives the window's size in
the desktop's units and the compositor stretches it, so a 3x frame was
shrunk and stretched back. The window now asks for real pixels, and a
windowed `--width`/`--height` is divided by the pixel density for SDL
(`itc_790.log`: density 1.45, a 1920x1080 window got a 1920x1080
swapchain).
At present the frame is blitted as large as fits, centred, the rest of
the swapchain image cleared black (`FitOffsets` in `renderer.cpp`). The
swapchain is made again when SDL reports our window's pixel size changed
(an event watch), since Wayland never reports it out of date. A 1600x1000
window (`itc_788.log`): swapchain 1600x1000, drawn at 1.25x (1600x900),
the cap held. Not done: toggling fullscreen while running, and the drawing
scale following a resized window (it's fixed at start).
