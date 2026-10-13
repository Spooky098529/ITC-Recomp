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
- **Guest memory is mirrored, not imported.** The translated shaders read
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
  every frame; tracking which ranges changed is for later. Frames in flight
  share the mirror, so a range rewritten while the GPU still reads the
  previous frame's copy could glitch.
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
| 4 | **The other passes**: each pass renders into a Vulkan image of its resolve's size and format; resolves become copies. | Shadows, reflections and the mirror work. | |
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

## Code map (`itc_recomp/src/native/`)

- `replay_hooks.cpp`: wraps each replay handler; calls
  `Renderer_OnCommand` before and `Renderer_AfterCommand` after the
  library call (command 7 is wrapped in `interpolation/blend_state.cpp`).
- `renderer.cpp`: window, device (`DeviceContext` in `device_context.h`),
  swapchain, per-frame command buffers and fences, submit (texture uploads
  first) and present (blits `MainView`'s target to the window), stats
  every 5 s. Only draw ids reach `MainView::AfterDraw` today.
- `main_view.cpp`: everything per draw: register file from the shadow
  (`state.cpp`), shaders (`shaders.cpp`), constants ring, guest memory
  mirror, pipelines, texture descriptor sets, and the one render target.
  It skips every draw that isn't the main view (pitch 1040, 2x MSAA,
  format 3 or 12: `other_passes` in the stats).
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
