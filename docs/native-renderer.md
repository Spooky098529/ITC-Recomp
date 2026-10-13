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
  comes at milestone 5.

## Milestones

| # | Milestone | Done when | Status |
|---|---|---|---|
| 1 | **Skeleton**: `--native_renderer` creates our own Vulkan device, window and swapchain; the replay hooks pass each frame's commands to it. | A window clears and presents every frame. | Done (2026-10-13): presents every frame in menus (30 fps) and races (113-128 fps, ~26,800 commands and ~2,900 draws a frame), the game unaffected. |
| 2 | **Main view geometry**: vertex and index buffers read from guest memory, shaders translated, render states mapped to pipelines. | Race geometry is visible in our window, untextured. | Done (2026-10-13): the cockpit view's geometry, the right way up, solid, with the game's coloured lighting on white placeholder textures (screenshot checked). |
| 3 | **Textures**: untile and upload (mostly BC1-BC3), samplers from the fetch constants. | The main view looks right without effects. | Done (2026-10-13): signs, gauges, fences, buildings and road textured correctly (screenshot checked); what comes from resolves is still missing. |
| 4 | **The other passes**: each pass renders into a Vulkan image of its resolve's size and format; resolves become copies. | Shadows, reflections and the mirror work. | In progress: every pass draws, clears and resolves work; the shadow map, mirror and reflection map come out right (dumped and checked); buildings sometimes show the rainbow effect (see below). |
| 5 | **Post-processing, HUD, present**, then exclusive mode. | A race is fully native; measure the frame rate. | |
| 6 | **Menus**. | The emulated GPU is no longer needed. | |

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
  the mirror is black, and some buildings show rainbow smears, likely
  textures that are resolve targets (whatever is in guest memory there),
  to be confirmed.

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
  we clear the whole target. Rects at +12 (16 bytes each), not used.
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
- Not yet: draws other than command 18 (inline vertices, 19; the helper
  object, 15: post-processing, bloom, the projected texture's draws),
  clear rects, render targets 1-3, resolves to a level or slice other
  than 0.

### The rainbow effect

Buildings sometimes show a rainbow-coloured texture (smooth colour
gradients) instead of theirs, for a while, then their own again; the game's
own window shows them right. What's known (2026-10-13):

- It depends on timing: frames recorded slowly don't have it (dumps that
  hashed every vertex range or formatted a line per draw; the frames after
  a dump's pause), waiting for the GPU before each frame makes it rarer.
- Not frames in flight sharing the mirror (a mirror each now, still there).
- Not vertex or index data: none of ~2,000 ranges changed between frames
  or within a frame.
- Not textures: none changed after loading (checked every 4 frames for
  their first 240), none reloaded.
- Not resolve destinations: no lookup got one of another size or format.
- Not z-fighting: no draw uses an equal depth test; depth bias didn't help.
- Smooth gradients look like a texture's small mips sampled with stretched
  coordinates, which would point at a constant (texture coordinate
  transform) read wrong; not checked.

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
  `tools/pfm_to_png.py <dir>` makes PNGs. The game pauses while it writes.
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
  `--native_renderer=true`; the user starts a race, sends a screenshot of
  the "ITC native renderer" window and quits; read the `native renderer:`
  lines in the log.

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
4. The rainbow smears on buildings and the black mirror should go once
   resolves feed the texture cache; check that guess first by logging
   which texture addresses are resolve destinations.
5. Open: the textures logged as "of another dimension than declared"
   (a log line names an example since `2c13992`; not run yet).

## Known issues

- Textures are never freed: memory reused for different textures (another
  race) leaves the old images behind.

- **Resizing the window doesn't resize the swapchain** on Wayland: the
  compositor leaves the size to us and never reports the swapchain out of
  date, so the image stays 1280x720. Fix later by watching the window's
  size-change events (on the main thread) and recreating the swapchain.
- No Vulkan validation layer here yet (`--native_renderer_validation` falls
  back without it); install `vulkan-validation-layers` before drawing gets
  complicated.
