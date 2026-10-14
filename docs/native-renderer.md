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
- **Shaders: reuse the SDK's Xenos-to-SPIR-V translator** for now, to be
  replaced with our own later. This is the one part that leans on the SDK. The plugin builds it with hidden symbols, so
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
| 6 | **Exclusive mode**: stop calling the graphics library (and so the emulated GPU), keeping only what our renderer needs from it; then measure. | The emulated GPU is no longer needed; frame rate measured against it. | |

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
- **The boot movie** (2026-10-14; see "Movies" in `gpu-functions.md`):
  the user saw the Ubisoft movie missing from our window. It's the
  DrawVertices outside the command list that logging found. DrawVertices
  is now wrapped: outside the command list, on the render thread (the one
  ending frames; others are only logged), it's drawn after the library's
  call (`Passes::DirectDraw`) as an auto-indexed draw whose vertex buffers
  are copied from their fetch constants like an indexed draw's
  (`DrawCall::auto_indexed`; inline draws are auto-indexed too). Its
  textures change every frame, so the texture cache checks every texture
  of these draws each frame (its verify-every-frame mode, for that draw
  only): ~40 reloads a second while the movie plays.
- Seen on the way (fixed above): in the menus our window drew the 2D
  interface right but not the 3D car on the Free Run screen (car, floor
  reflection, light beams) nor the Course In screen's night city picture
  (black in ours). The menus also log ~6,000 lookups per 5 s of a texture
  of another dimension than declared.

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
- `--native_renderer_check_mirror`: log the vertex and index ranges whose
  contents change between frames.
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

- **Gamma**: our window is lighter than the game's (seen side by side,
  2026-10-14): the Xbox 360's output gamma isn't applied yet. A known
  issue, not part of milestones 5 or 6 (the user's call).

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
- No Vulkan validation layer here yet (`--native_renderer_validation` falls
  back without it); install `vulkan-validation-layers` before drawing gets
  complicated.
