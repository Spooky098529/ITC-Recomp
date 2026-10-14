# Native renderer

A renderer that draws the game's render commands with Vulkan directly,
replacing the emulated GPU (`librexgpu-xenos.so`) and moving the project
away from the ReXGlue SDK. Why: the emulated GPU limits the frame rate
(see "Performance" in `functions.md`), and the game's command list is a
narrow boundary to draw from (see `gpu-functions.md`, which holds what's
known about it; this file is the plan and its progress).

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
  every frame; tracking which ranges changed is for later.
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
| 6 | **Exclusive mode**: the emulated GPU gets no work and then isn't loaded at all; the graphics library is kept for what our renderer needs from it (see "Plan for milestone 6"); then measure. | The GPU plugin isn't loaded; frame rate measured against the emulated GPU. | In progress: the plugin isn't loaded (`run.sh` runs exclusive mode); measured: 135 fps mean uncapped against the emulated GPU's 155, then 215 after the first speed work (step 5, in progress; holds the 144 cap in an attract race). The rest of step 5 and the frame's end (step 6) to come. |
| 7 | **Known issues cleanup**: what's under Known issues below (the boot movie, gamma, textures replaced in memory, textures never freed, window resizing). | The known issues are fixed or accepted. | |

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
  doesn't fit waits for the next frame. Loaded textures' memory is hashed
  again every 240 frames (8 a frame at most) and reloaded if it changed.
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
   nothing waits on the swap counter (+4).

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
- `--native_renderer_copy_all`: copy every vertex and index range into the
  mirror every frame, as before memcpy's writes were tracked.
- `--native_renderer_verify_mirror`: compare every range not copied with
  guest memory, every frame, logging how many were checked and those that
  differ (slow: ~20 fps in a race, since the mirror is uncached memory).
- `--native_renderer_log_buffers`: log the vertex and index buffer objects
  draws bind (header, data, later changes), the library's memcpy into
  their headers, and for each range copied into the mirror whether memcpy
  wrote it before its first use and again afterwards (see "Buffers").
- `--native_renderer_check_mirror`: log the vertex and index ranges whose
  contents change between frames; and per frame, the bytes copied whose
  contents differ from that range's last copy, the bytes of ranges new,
  and how much of the vertex buffers' copies lies within the draws'
  vertices (lowest to highest).
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
- Testing: build, run the game in the background with
  `--native_renderer=true --skip_intro --attract_after_s=1`. With no input,
  an attract race starts about 1 s after launch (where `fps` goes above 30 in the
  log; see "The title screen" in `functions.md`), so a run can be fully
  automatic: launch under `timeout`, delete the `/dev/shm/xenia_memory_*`
  files afterwards, and read the `native renderer:` lines in the log and
  any dump. Screenshots: `spectacle -b -n -f -o <file>.png` while the game
  runs (a script that launches, sleeps and fires it); it captures the
  whole screen, so the windows must be visible (tell the user first).
  Periodic dumps (`--native_renderer_dump_every`) need no screenshots.
  To compare with the emulated GPU, add `--motion_blur=true --glow=true
  --readback_resolve=full` and a full dump (`--native_renderer_dump`
  without `_every`, 10 s into the race), and put each `guest_resolve_*`
  next to the `resolve_*` of the same name (a small PIL script).

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

## Known issues

To be fixed (or accepted) in milestone 7.

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

- **Textures replaced in memory show their old contents for a while.**
  The game streams track sections in during a race, and textures are
  rewritten in place (in a 75 s attract race, several a few seconds apart,
  59 in one 5 s window). A texture past its first 240 frames is hashed
  again at most every 240 frames and at most 8 a frame, so a change was
  noticed 240-3,200 frames late (logged as "textures changed in memory
  after their first frames"). `--native_renderer_verify_textures` checks
  every texture used each frame; something cheaper (watching writes) is
  needed before this is the default.

- Textures are never freed: memory reused for different textures (another
  race) leaves the old images behind.

- **Resizing the window doesn't resize the swapchain** on Wayland: the
  compositor leaves the size to us and never reports the swapchain out of
  date, so the image stays 1280x720. Fix later by watching the window's
  size-change events (on the main thread) and recreating the swapchain.
- **Performance: shaders are found by hashing their microcode every
  draw** (~5% of the render thread, 2026-10-14 profile, before the caches
  became hash maps). Translations are cached by content hash, so each
  patched version of a vertex shader is translated once; the cost is
  finding which one a draw uses. Each draw hashes the same microcode
  several times: `ShaderCache::GetInterface` (both shaders, through
  `Analyzed`), `ShaderCache::Get` (each again), and the two diagnostic
  maps `param_gen_by_shader` and `interpolators_by_shader` in
  `passes.cpp` (the pixel shader twice more). Cheap fix: hash each once a
  draw and pass the hash along, and hash for the diagnostics only when
  they're logged. Further: key by what the library tracks (the shader
  object and the declaration it's patched for; `sub_822AE340` is already
  wrapped) instead of hashing, but a freed object's memory reused for
  another shader would then draw with a stale translation unless shader
  creation and release are hooked too.
- **A freeze while driving** (seen once by the user, 2026-10-14, in a
  driven race after `4410ce8`, the indexed draw without packets; to look
  into later, the user's call). The log (`itc_560.log`) shows no slow
  second in the race (slowest 131 fps at the 144 cap) and the game's ticks
  stayed at 30, so either the hitch was under ~0.1 s or the picture
  stopped changing while frames still went out. The same race had **failed
  draws** in its busiest 5 s windows (36 and 74 a frame, with 3,000-3,300
  draws and up to 824 inline draws a frame; earlier logs had them once, 7
  a frame), not known to be related. Since `895055b` the log names each
  failed draw's reason ("failed draws a frame by reason") and any frame
  taking 100 ms or more ("long frame"); the next run that shows either
  should say which.
- **Performance: locked instructions are slow on the render thread** (a
  few hundred cycles each, 2026-10-14: the thread's many stores have to
  drain first). The shader reference count's compare-and-swap still costs
  ~1%, and any atomic added to per-draw code (counters included) shows up:
  keep per-draw counters per thread.
- No Vulkan validation layer here yet (`--native_renderer_validation` falls
  back without it); install `vulkan-validation-layers` before drawing gets
  complicated.
