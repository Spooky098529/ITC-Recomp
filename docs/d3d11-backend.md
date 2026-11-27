# A Direct3D 11 backend (a plan; step 2 done, step 3 in progress)

Written 2026-11-27 on `d3d11` (branched from `main` at `c37e7acd`, the PC
renderer's merge). Step 2 is done (below, "Step 2: `render::`'s own types").

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
  resolves passed on. Its values were Vulkan's enums, by the PC renderer
  plan's design; since step 2 they're our own (`src/gpu/types.h`).

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

How that's done (decided at step 3's start, 2026-11-27):

- **Usage, not layouts.** The renderer says what an image is about to be
  used for (`Use(image, Usage)`: sampled, color target, depth target, copy
  source, copy destination, shown), as D3D12's resource states do; the
  Vulkan backend keeps each image's layout and records a barrier only when
  the usage changes, and the D3D11 backend does nothing. Tracking it
  wholly inside the backend (as DXVK does) can't work here without moving
  the renderer's pass logic too: a texture bound inside a dynamic rendering
  pass can't be transitioned there, so the renderer must say it before the
  pass, as it does today with its `Transition` calls.
- **Handles:** `gpu::Image`, `gpu::Buffer`, `gpu::Sampler`, `gpu::Shader`,
  `gpu::Pipeline` are opaque objects each backend derives from (both
  backends are built into the Windows executable, so no shared
  definitions).
- **One command stream:** the backend owns the frame's recording; the
  renderer's calls don't pass a command buffer. Uploads (the texture cache's
  and the arena's) go through the backend, which orders them before the
  frame's drawing (Vulkan: their own command buffers submitted first;
  D3D11: `UpdateSubresource` or a staging copy in place).
- **In between:** the Vulkan implementation is a class of its own
  (`gpu::vulkan::Backend`, `src/gpu/vulkan/`). While pieces move, code not
  yet moved reaches Vulkan through a few Vulkan-only accessors on that class
  (the device context, the frame's command buffer), which shrink as pieces
  move, until the renderer holds only a `gpu::Backend`.

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

## Step 2: `render::`'s own types (done 2026-11-27)

- `src/gpu/types.h` holds the renderer's own values for what `render::`
  held as Vulkan's: `CompareOp` (in the GPU's order, D3D's minus one: the
  draw constants' alpha test flag passes it to the shaders as is),
  `BlendFactor`, `BlendOp`, `CullMode` (bits: front 1, back 2; both culls
  everything, which D3D11 lacks: its backend will skip such draws, as the
  draw already takes no depth bias for them), `Filter`, `MipMode`,
  `AddressMode` (`kMirrorClampToEdge` is D3D11's `MIRROR_ONCE`) and
  `BorderColor`.
- `render::` (`state.{h,cpp}`) and the handlers no longer include anything
  of Vulkan's. The pipeline key carries the `gpu::` values; the Vulkan code
  maps them where it makes a pipeline (the draw's and the ID pass's) or a
  sampler (`src/native/vulkan_types.h`). The pipeline disk cache is the
  driver's `VkPipelineCache` blob, not our keys, so it's unaffected.
  `passes.cpp`'s own copies of the blend factor and op maps had no callers:
  gone.
- Checked: an attract race's dump against the build before's at the same
  moment (2,654 draws; all 59 images 0.0000% of pixels off, `draws.txt` the
  same), the tick replay (both races match, 0 error lines).

## Step 3: the backend interface (in progress)

- **The device, swapchain and frames (2026-11-27).** `src/gpu/backend.h`
  (`gpu::Backend`: the window's flags, what's made on the main thread once
  the window is, the device, the output size, a frame's begin and end with
  the shown image fitted with black bars, waiting for the GPU) and its
  Vulkan implementation `src/gpu/vulkan/backend.{h,cpp}`
  (`gpu::vulkan::Backend`): the instance and surface, the device and its
  features (`native::DeviceContext`, still handed to the code not yet
  moved), the swapchain, the frames' command buffers, fences and
  semaphores, the blit to the swapchain image, the submit (the uploads'
  command buffers first) and the present, moved from `renderer.cpp`
  unchanged. `renderer.cpp` keeps the window (made with the backend's
  flags), the dumps, the cycle and the stats; it reaches the frame's
  command buffer through the backend (`commands()`, for the passes not yet
  moved). Checked: an attract race's dump against the build before's at
  the same moment (all 59 images 0.0000% off; `draws.txt` the same but for
  inline draws' vertex addresses, 16 MB apart: which of the game's two
  command list buffers the frame used), the tick replay (both races
  match).
- **Samplers (2026-11-27).** `gpu::SamplerDesc` (`src/gpu/types.h`) and
  `Backend::CreateSampler`; the Vulkan backend applies the device's limits
  (no anisotropy where it's missing, then linear filters with it; the most
  anisotropy; mirrored repeat for a missing mirror clamp to edge), which the
  texture cache did. The texture cache keeps its sampler cache, keyed by the
  description; `passes.cpp`'s three fixed samplers (the resolve's, the
  movie's, the linear placeholder's) come from the backend too.
  `Passes::Init` and `TextureCache::Init` take the backend. Checked as
  before (dump 0.0000%, the tick replay matching).
- **Images, first part (2026-11-27).** `gpu::Format`, `ImageDesc` and
  `Usage` (`src/gpu/types.h`); `Backend::CreateImage` (with initial texels,
  then ready to sample), `DestroyImage`, `Describe`, `MemorySize` and
  `Use`. The Vulkan backend's image (`gpu::vulkan::ImageObject`) holds its
  image, memory, views (all of it; a depth image's depth alone) and its
  layout; `Use` records the renderer's full barrier (all stages, all
  access) only when the layout changes, as `Transition` did, and
  `Usage::kUndefined` forgets the layout without a barrier (the ID pass's
  images, drawn from nothing each frame). On it so far: the six
  placeholders (made with their texels, no longer cleared by hand), the
  shown image (`Backend::EndFrame` takes it), the ID pass's images, the
  movie planes, and the render targets with the images they hand to
  resolve destinations: the texture cache carries a handed image as its
  `gpu::Image` (`HandedImage::object`), gives it back as such, and has the
  backend destroy it when retired. The cache's own images (textures,
  resolve destinations' own) are still its raw Vulkan ones; the dump's and
  `--native_renderer_show_resolve`'s transitions of resolve images still go
  out and back by hand (they leave the tracked layout as it was). Checked:
  the dump identical, the tick replay, the pad replay with dumps (the
  tuning shop's and garage's tiled views; 8 hand-overs and 4 copy-backs a
  frame in the menus), the boot movie, `--xbox_gamma=hdtv`.
- **Images, second part (2026-11-27).** The texture cache's own images
  (loaded textures, resolve destinations) come from the backend too
  (`TextureCache::State::CreateTexture`); a resolve destination's own view
  is its object's, and `ResolveDestination::object` lets the resolve say
  its uses (`Use`) instead of working out its layout from `fresh`. Every
  retired texture's image is the backend's to destroy. Uploads still
  record their own barriers in the cache's upload command buffer and set
  the object's layout after (until uploads move); the swizzled views are
  still the cache's own Vulkan views. Checked: the dump identical, the tick
  replay, the pad replay (no errors).
- **Views (2026-11-27).** `gpu::ViewDesc` (a view type, a swizzle of the
  image's components or constants, a depth image's depth alone) and
  `Backend::CreateView` and `DestroyView`: the texture cache's swizzled
  views are made and destroyed through them. Checked: the dump identical,
  the pad replay without errors.
- **Buffers (2026-11-27).** `gpu::BufferDesc` (size; vertex, index,
  uniform, storage, copy source and destination; host-visible and mapped
  for its life, or device-local) and `Backend::CreateBuffer`,
  `DestroyBuffer`, `Mapped`, `MemorySize`: every buffer the renderer made
  itself (the arena, its and the texture cache's staging, the constant
  blocks, the movie staging, the gamma table, the dump buffer) comes from
  them; `VertexArena::Init` takes the backend. Checked: the dump identical,
  `--xbox_gamma=hdtv` and the boot movie without errors.
- **Next (as planned 2026-11-27):** uploads, then the recording.
  *Uploads:* each client keeps
  its own staging buffers and their sizes (so "staging full: try next
  frame" stays as it is); the backend records `UploadImage` (old layout:
  the image's tracked one, undefined before its first upload, then ready to
  sample, as the cache's barriers do now) and `UploadBuffer` into one upload
  command buffer of its own submitted before the frame (today the cache's
  and the arena's, in that order), and `CopyBufferToImage` in the frame
  (the movie planes). D3D11: `UpdateSubresource` from the staging memory.
  *Recording:* passes, binds and draws (step 3's next piece), then the
  full-screen and compute passes, then the dumps.
- **Dump runs:** a run lands on one of at least three moments (2,654,
  2,381 and 2,338 draws seen): take runs until one has the reference's
  line count. `draws.txt`'s inline vertex addresses may differ between
  runs of one build.

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
4. **View swizzles.** D3D11's shader resource views have no component
   swizzle, which the texture cache's views carry (the fetch constant's
   swizzle, red and blue exchanged for handed images). With D3D11 the
   shaders must apply it (a per-texture constant), and then perhaps with
   Vulkan too, so both draw alike: decided at step 4 or 5.
