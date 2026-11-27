# A Direct3D 11 backend (a plan; not started)

Written 2026-11-27 on `d3d11` (branched from `main` at `c37e7acd`, the PC
renderer's merge). This is a plan: no code has changed for it.

## The goal

The user, 2026-11-27: a Direct3D 11 renderer beside the Vulkan one. The PC
renderer (`docs/pc-renderer.md`) made it possible: the game no longer talks
to anything 360-shaped, its replay handlers call our own interface
(`render::`), every shader is our GLSL, and the ReXGlue SDK is gone. What a
second backend needs is below that interface.

Vulkan stays the default on Linux and Windows; D3D11 is chosen with a flag
(and `itc.toml`), Windows only.

## How it's built today

Above the seam, backend-neutral already:

- the game's command list and its replay (`src/command_list/`,
  `src/graphics/render_queue.cpp`, `replay_commands.cpp`,
  `single_commands.cpp`);
- `render::` (`src/render/state.{h,cpp}`): what the handlers set, as values
  (render states, blend, samplers, the bound shaders, declaration, streams,
  index buffer, targets, viewport, constants), and the draws, clears and
  resolves passed on. Its values are Vulkan's enums (16 Vulkan types in
  `state.h`), by the PC renderer plan's design.

Below it, one Vulkan renderer (`src/native/`), its logic and its API calls
mixed:

| File | Lines | Vulkan references | What |
|---|---|---|---|
| `passes.cpp` | 4,917 | 957 | the draw (`State::Draw`, ~800 lines), targets keyed by surface, resolves (hand-over, copies, MSAA), tiling, pipelines and their disk cache, descriptor sets, the constant ring, the gamma, movie and resolve passes, the dumps and diagnostics |
| `textures.cpp` | 2,047 | 187 | the texture cache: layouts, untiling, byte swaps, formats, uploads, XPR files, resolve destinations, samplers |
| `renderer.cpp` | 1,251 | 168 | the window, instance, device, swapchain, frames in flight, present |
| `vertex_arena.cpp` | 769 | 70 | the vertex and index data in one device-local buffer, the held models |
| `buffers.cpp`, `shaders.cpp`, `shader_ports.cpp`, `port_spirv.cpp`, `draw_registers.cpp` | ~1,400 | ~0 | memcpy's write tracking, the shader table, fitting a port to its draw (by editing its SPIR-V), the draw's fetch block |

The shaders are GLSL compiled offline to SPIR-V (`generated_game_shaders.h`)
and fitted at run time by writing specialization constants and
interpolator locations into a copy of the SPIR-V (`port_spirv.cpp`).

## The design

### A backend interface under the renderer's logic

The renderer's logic (what to draw into, when a resolve hands its image
over or copies, which pipeline state a draw needs, constant packing,
vertex placement, texture decoding) stays one piece of code. Its API calls
go through an interface (`src/gpu/backend.h`, new) with a Vulkan
implementation (today's calls, moved) and a D3D11 one. Roughly:

- **Resources:** images (2D, 2D array, cube; color or depth; sampled,
  render target or both; MSAA), buffers (vertex/index, uniform ring,
  staging), uploads, views with a swizzle, samplers from a description,
  destruction deferred past the frames in flight.
- **Shaders and state:** a shader from a port and its fit; the draw's state
  as one description (shaders, vertex input, topology, blend, depth,
  raster, target formats, samples), which Vulkan makes a pipeline and D3D11
  a set of state objects and an input layout.
- **Recording:** begin and end a pass (targets, clears), viewport, scissor,
  depth bias, blend constants, bind state, vertex and index buffers,
  textures and samplers by stage, constant blocks by offset, draw, draw
  indexed, copy an image (or a region), resolve MSAA, a full-screen pass,
  a compute dispatch (the gamma).
- **Frames:** begin a frame (its slot's work finished), submit, present,
  the window's size changes.

Vulkan's image layouts and barriers stay inside its implementation; D3D11
has none. `render::`'s Vulkan enums become our own (`src/gpu/types.h`),
mapped by each backend.

### Shaders for D3D11

From the SPIR-V we already make: SPIRV-Cross (its command line tool, like
glslangValidator an offline tool from the Vulkan SDK) writes each shader as
HLSL (Shader Model 5) in `compile_game.py`, checked in beside the SPIR-V.
The run-time fit becomes defines: SPIRV-Cross turns each specialization
constant into a `SPIRV_CROSS_CONSTANT_ID_n` macro, and the interpolators'
places become their semantics. D3D11 compiles each fitted shader once
(`D3DCompile`, `d3dcompiler_47.dll`, part of Windows 10 and later and of
Wine), kept in the disk cache as the pipelines are.

### Testing

There is no Windows machine (`docs/windows.md`): the D3D11 backend is
tested under Wine, whose D3D11 is wined3d (to OpenGL or Vulkan) or DXVK (to
Vulkan). Both are worth a run: they differ in what they accept.
DXVK-native (a Linux build of DXVK) could run the D3D11 backend in the
Linux build too, but would be a test-only dependency: decided at step 5.

Frames are checked against the Vulkan backend's with the frame dumps
(`--native_renderer_dump`, made backend-neutral in step 3) at the same
moment (`--attract_seed`, `--max_fps=30`, no interpolation), and by a drive.

## The steps

| Step | What | Checked by |
|---|---|---|
| **1** | **The Windows build again.** It has failed on CI since the E steps, whose code is Linux-only by the user's call (memory mapping, `madvise`, `mincore`, and what else the cross-build finds). Make it build and run under Wine with the Vulkan backend | CI; an attract race and the pad replay under Wine |
| **2** | **`render::`'s own types.** Our enums in place of Vulkan's in `render::` and the handlers, mapped to Vulkan's in the renderer | Frame dumps pixel for pixel; the tick replay |
| **3** | **The backend interface.** The Vulkan calls moved behind it in pieces: the device, swapchain and frames; images, buffers, uploads and samplers (the texture cache, the arena, targets); shaders and draw state; recording (passes, binds, draws, copies, resolves); the full-screen and compute passes; the dumps. No visible change | Frame dumps pixel for pixel after each piece; the tick and pad replays; a drive at the end |
| **4** | **HLSL.** `compile_game.py` writes every shader as HLSL too (SPIRV-Cross), with the fit as defines; checked by compiling them all (`D3DCompile` under Wine, or vkd3d-shader on Linux) | Every shader compiles; a sample compared by eye |
| **5** | **The D3D11 backend.** `--renderer=d3d11` (Windows), the device and swapchain, resources, state objects, shaders, recording, the special passes | Under Wine: an attract race's frame against Vulkan's at the same moment, the pad replay, a drive by the user |
| **6** | **Clean-up.** The docs, `itc.toml`'s setting, CI, a Windows test zip | |

No profiling until the end, as with the PC renderer: then one profile of
each backend.

## Open questions

1. **Whether D3D11 is worth it over Vulkan on Windows.** Vulkan already runs
   there; D3D11 is for drivers or machines where Vulkan is poor. Not a
   blocker: the interface (step 3) helps either way.
2. **Feature gaps.** Things the Vulkan renderer uses that D3D11 lacks or
   does differently, to find in step 3: mirror-clamp-to-edge (D3D11 has
   `MIRROR_ONCE`), primitive restart (D3D11 always restarts on the strip
   cut index), rectangle lists (drawn as instances already), the depth
   formats, sampling a depth image.
3. **DXVK-native for Linux tests** (step 5).
