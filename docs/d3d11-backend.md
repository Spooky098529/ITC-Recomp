# A Direct3D 11 backend (done: steps 1 to 6)

Written 2026-11-27 on `d3d11` (branched from `main` at `c37e7acd`, the PC
renderer's merge). Steps 2 and 3 are done (below): the renderer holds only
`gpu::Backend` and `gpu::` types, the Vulkan calls all in
`src/gpu/vulkan/`.

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

## Step 1: the Windows build again (done 2026-11-27)

The user's go-ahead, 2026-11-27 ("we need windows code"). `itc.exe` cross-
builds again and runs under Wine (details and checks in `docs/windows.md`,
"Status"): the game's memory blocks through `src/base/os_memory`
(VirtualAlloc on Windows), simde given `<cmath>` first, the executable's
folder found on Windows, and the Vulkan backend starting without a
swapchain until the window can show one (a fullscreen window under Wine).
The tick replay runs under Wine (`ITC_WINE=1`) and matches Linux's
baseline for race 0, tick for tick; CI packages `itc.exe` with the Visual
C++ runtime DLLs (and what those import). Found on the way, not this
plan's: a new career stops in the car dealer (`docs/known-issues.md`).

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
  sampler (`src/native/vulkan_types.h`, now `src/gpu/vulkan/types.h`). The pipeline disk cache is the
  driver's `VkPipelineCache` blob, not our keys, so it's unaffected.
  `passes.cpp`'s own copies of the blend factor and op maps had no callers:
  gone.
- Checked: an attract race's dump against the build before's at the same
  moment (2,654 draws; all 59 images 0.0000% of pixels off, `draws.txt` the
  same), the tick replay (both races match, 0 error lines).

## Step 3: the backend interface (done 2026-11-27)

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
- **Uploads (2026-11-27).** `gpu::ImageUpload` and `BufferUpload`, and
  `Backend::UploadImage` and `UploadBuffer`: copies from a client's staging
  buffer, recorded into one upload command buffer of the backend's per
  frame slot, submitted before the frame's (opened at the first upload,
  which for the arena's built-in index lists is before the first frame; a
  slot's stream is reset at its frame's start only when it isn't already
  open). An image goes from its tracked layout (undefined before its first
  upload, else sampled) to the copy and back to sampled, with the cache's
  old barriers; the buffer copies wait for earlier frames' reads once and
  end with the arena's old barrier for vertex input. The clients keep their
  own staging and its sizes ("staging full: next frame" unchanged); the
  texture cache's and the arena's own command pools are gone, with
  `Backend::SubmitBefore`. The movie planes' copies (made in the frame, as
  the movie is drawn) wait for the recording piece. Checked: the dump
  identical, the tick replay, the pad replay without errors.
- **Shaders (2026-11-27).** `gpu::ShaderDesc` (stage, SPIR-V, the
  textures' types and the sampler count of its stage's set, a vertex
  shader's vertex input) and `Backend::CreateShader`: the Vulkan backend's
  shader (`ShaderObject`) holds the module, the texture set layout (shared
  by stage, types and count, as `passes.cpp` shared them) and the vertex
  input. The game's shaders (`TranslatedShader::gpu`) are made through it
  at their first draw; `passes.cpp`'s pipeline code still reads the module,
  layouts and vertex input from the object (until pipelines move). The
  resolve, movie, gamma and ID pass shaders are still `passes.cpp`'s own
  modules. Checked: the dump identical, the tick replay, the pad replay
  without errors.
- **The pipeline disk cache (2026-11-27).** `Backend::OpenDiskCache`
  (where the backend keeps what it makes once for this device and driver);
  the Vulkan backend's `VkPipelineCache` and `cache/pipelines.bin`, read at
  start and saved at most every 10 s as it grows, moved from `passes.cpp`
  unchanged (`MaybeSaveDiskCache` is told the pipelines made until the
  backend makes them itself). Checked: the dump identical, the cache loaded
  and written.
- **Pipelines (2026-11-27).** `gpu::PipelineDesc` (`src/gpu/types.h`:
  the two shaders and the fixed-function state, field for field the
  draw's old `PipelineKey`, plain 32-bit fields hashed as bytes) and
  `Backend::GetPipeline`: the Vulkan backend owns sets 0 (empty) and 1 (the
  constant blocks), derives the pipeline layout from the shaders' texture
  sets (shared by layout pair, as before), and caches the pipelines by
  description; its `PipelineObject` gives `passes.cpp` the pipeline, its
  layout and its set layouts for binding (until binding moves). The draw
  ID pass makes its push-constant layouts from those set layouts. Gone from
  `passes.cpp`: `GetPipeline`, `GetPipelineLayout`, `GetModule`, the vertex
  inputs and the texture layouts. Checked: the dump identical (the ID pass's
  image too), the tick replay, the pad replay without errors.
- **Passes (2026-11-27).** `gpu::PassTarget` (an image, its load: kept,
  cleared or dropped, its store, the clear value as bits), `gpu::Rect`, and
  `Backend::BeginPass` and `EndPass`: the draws' passes, the resolve's, the
  movie's and the ID pass's (`vkCmdBeginRendering` and `vkCmdEndRendering`
  are the backend's alone). Checked: the dump identical (the ID pass's
  image too), the tick replay, the pad replay, the boot movie drawn.
- **Copies and clears in the frame (2026-11-27).** `Backend::ClearColor`,
  `ClearDepthStencil` (depth and/or stencil), `CopyImage`, `BlitImage`,
  `ResolveImage` (MSAA) and `CopyBufferToImage` (in the frame, unlike the
  uploads): the targets' clears, the copy back of a handed image, the
  movie planes' copies, and what's shown made from the front buffer or the
  main view (or cleared). Left for the dumps piece: the two debugging aids'
  (`--native_renderer_show_resolve`'s blit and the dump's read-back).
  Checked: the dump identical, the tick replay, the pad replay, the boot
  movie without errors.
- **The draw's binds and draws (2026-11-27).** `gpu::Viewport` and
  `Backend::SetViewport`, `SetScissor`, `SetDepthBias`,
  `SetBlendConstants`, `BindPipeline`, `BindVertexBuffers` (one buffer, an
  offset a binding), `BindIndexBuffer`, `Draw` and `DrawIndexed`: the game's
  draws record through them (the repeated-state skipping stays in
  `passes.cpp`, comparing what it asks for). Checked: the dump identical,
  the tick replay, the pad replay without errors.
- **Texture sets (2026-11-27).** `Backend::MakeTextureSet` (a stage's
  views and samplers for a shader, one set per distinct contents a frame):
  the per-frame set cache (`FrameSetCache`, keyed as before) and the
  frames' descriptor pools moved from `passes.cpp` into the Vulkan backend,
  reset at the slot's frame start. `passes.cpp` keeps the last-set reuse by
  fetch constants and generations (the renderer's logic). Checked: the dump
  identical, the tick replay, the pad replay without errors.
- **Binding the sets (2026-11-27).** `Backend::DescribeConstants` (a
  uniform buffer as the four constant blocks, their ranges given once) and
  `BindResources` (the pipeline's texture sets and constant blocks at their
  offsets): set 0's empty set and each block's set 1 are the backend's.
  Checked: the dump identical, the tick replay, the pad replay without
  errors.
- **The renderer's own passes (2026-11-27).** Decided: the resolve, movie
  and gamma passes are high-level operations each backend draws with its
  own shaders (made from the same GLSL, `src/native/glsl`), not a general
  full-screen-pass or compute interface: `Backend::DrawResolve`
  (`gpu::ResolveDraw`: source, destination, area, offset, samples to
  average, scale, red and blue exchanged), `DrawMovie` (three planes, a
  target, an area) and `ApplyGamma` (an image in its storage usage, its
  size, the table). The renderer says the images' uses before and after;
  the backend makes the pass, pipeline, set and push constants. The draw
  ID pass stays on the draw's calls: `GetIdPipeline` (the draw's
  `PipelineDesc`: its vertex shader, geometry and depth state, writing into
  R32Uint) and `SetDrawId` (D3D11: a small constant buffer); its draws
  bind no pixel textures (`BindResources` binds the empty set for a null
  texture set). The Vulkan backend owns the shaders, layouts, samplers,
  the pipelines by format, the per-frame pass sets (reset at the frame's
  start) and the gamma table (made at its first use); it saves its disk
  cache itself, counting every pipeline (`MaybeSaveDiskCache` and
  `pipeline_cache()` gone from the in-between accessors). Checked: the dump
  identical (the ID pass's image too), `--xbox_gamma=hdtv` at a reference
  moment (the images it doesn't touch identical, `shown` the reference's
  through the table, 0.0000% off), the boot movie drawn, the tick replay,
  the pad replay without errors.
- **The dumps and showing a resolve (2026-11-27).**
  `Backend::CopyImageToBuffer` (a depth image's depth): the dump's
  read-back goes through it, the resolves' images taken out of their
  sampled use and back with `Use` (no more hand-made transitions beside the
  tracked layout), and `--native_renderer_show_resolve` blits with
  `BlitImage`. The frame's resolves carry their `gpu::Image`
  (`TextureCache::HoldsHanded` takes one). `passes.cpp` makes no `vkCmd*`
  call now. Checked: the dump identical, a shown resolve equal to its own
  dumped image, the pad replay with dumps in the menus (the garage's
  frame drawn; the replay then drifted into the known `8219BCC0` stop, as
  dump stalls do) and without (no errors), the tick replay.
- **The in-between accessors gone (2026-11-27).** The renderer
  (`passes.cpp`, `textures.cpp`, `vertex_arena.cpp`, `renderer.cpp`) holds
  only `gpu::Backend` and `gpu::` types: formats are `gpu::Format`
  (`Target`, the texture cache's formats, the dump's), samples a count,
  extents, viewports and scissors `gpu::Extent2D`, `Viewport` and `Rect`,
  clears a value of our own, the shaders' declared texture types
  `gpu::ImageType`, views, samplers and texture sets the backend's objects
  (the placeholders' views made with `CreateView`); the texture cache's
  `ResolveDestination` and `HandedImage` carry the image and its format
  alone. No command buffer is passed around: the backend records. Three
  questions the renderer asked the device are the backend's:
  `ConstantsAlignment`, `CanFilter` (the depth hand-over) and
  `ResourceLayout` (which pipelines keep the bound resources, for the
  repeated-state skip). `vulkan_functions`, `vulkan_types.h` and
  `device_context.h` moved to `src/gpu/vulkan/` (`functions`, `types.h`,
  `device_context.h`); `renderer.cpp` names `gpu::vulkan::Backend` only
  where it makes it. Checked: the dump identical (`draws.txt`'s declared
  texture types now `gpu::ImageType`'s numbers: 2D arrays 1, not 5), the
  tick replay, the pad replay without errors, `--xbox_gamma=hdtv` at a
  reference moment (`shown` the reference's through the table), the boot
  movie drawn. A drive is welcome.
- *(D3D11 later: `UpdateSubresource` from the staging memory for the
  uploads.)*
- **Dump runs:** a run lands on one of at least four moments (2,654,
  2,669, 2,381 and 2,338 draws seen): take runs until one has the reference's
  line count. `draws.txt`'s inline vertex addresses may differ between
  runs of one build.

## Step 4: HLSL (done 2026-11-27)

- **SPIRV-Cross.** Not in Artix's repos: `itc_recomp/tools/build_spirv_cross.sh`
  builds its command-line tool (tag `vulkan-sdk-1.4.363.0`, the installed
  glslang's and spirv-tools' SDK) into `itc_recomp/out/tools/spirv-cross/`
  (the user's call, 2026-11-27). `compile_game.py` and `compile.sh` find it
  there (or `$SPIRV_CROSS`, or the PATH).
- **Output.** `compile_game.py` writes every game shader as Shader Model 5
  HLSL into `src/native/generated_game_shaders_hlsl.h` (`k_hlsl_game_<name>`,
  raw string literals, ~2.4 MB), cross-compiled from the same GLSL compiled
  again with its names (readable HLSL; the SPIR-V header is unchanged);
  `compile.sh` writes the backend's own (`generated_<name>_hlsl.h`,
  `k_hlsl_<name>`). Vertex shaders have Y flipped to D3D's clip space
  (`--flip-vert-y`: D3D's NDC has +Y up, Vulkan's down; Z is 0 to 1 in both,
  and the pixel position's origin is the top left in both).
- **The fit as defines.** Each specialization constant is
  `SPIRV_CROSS_CONSTANT_ID_n` (SPIRV-Cross's own, its value in the GLSL the
  default). The interpolators were first given semantics by define
  (`XE_INTERPOLATOR_n`); step 5 replaced that with fixed slots (below,
  "Interpolators").
- **Bindings as SPIRV-Cross writes them.** The constant blocks are `b0`
  (the draw constants), `b1` or `b2` (the vertex or pixel float constants),
  `b3` (bools and loops); a stage's views and samplers keep their bindings
  (`tN`, `sN`: views first, then samplers, the highest sampler `s13`, inside
  D3D11's 16). The own shaders' push constants are a `cbuffer Constants`
  with no register (the only one in each, so `b0`); combined samplers become
  a view and a sampler at the same number; the gamma's storage image is
  `u0`, its table a `ByteAddressBuffer` at `t1`.
- **Changed for HLSL** (no visible change: the frame dump identical, 59 of
  59 images 0.0000% off):
  - The draw constants (`XeDrawConstants`) were a scalar-layout block (a vec3
    across a 16-byte boundary, a float array with a 4-byte stride), which
    HLSL's packing can't express: now `vec4 xe_draw[11]` (std140) read at
    the same byte offsets through `xe_draw_*()` functions; `DrawConstants`
    (`draw_constants.h`) unchanged.
  - The ports' jumps were `pc = N; continue;` inside `switch (pc)` in a
    loop; HLSL has no `continue` in a switch (3 shaders failed). Now
    `pc = N; break;`, the end `pc = -1; break;`, and the loop leaves on
    `pc < 0`: the generator (`ucode_to_glsl.py`) and the 49 ports with jumps
    alike (the generator gives the ports' control flow exactly, checked on
    the 121 it ports before an older, unrelated stop on its texture tuples).
- **Checked: every shader compiles** with Microsoft's `D3DCompile`:
  `itc_recomp/tools/hlsl_check` (`hlsl_check.exe`, built with the cross
  toolchain by its `build.sh`) under Wine; `compile_game.py --check-hlsl` and
  `compile.sh --check-hlsl` run it (151 game shaders and 6 own ones, 0
  failed). Wine's own `d3dcompiler_47.dll` is vkd3d-shader's HLSL compiler,
  which refuses valid HLSL (139 of the 151: `SampleGrad` on a 2D array's
  float2 gradients, dynamic vector stores, constant expressions), so the test
  prefix has Microsoft's (`winetricks d3dcompiler_47`, a native override).
  That's also what the D3D11 backend compiles with under Wine in step 5,
  with DXVK running the result (or wined3d), as Windows' own
  `d3dcompiler_47.dll` is Microsoft's.
- **Next:** step 5.

## Step 5: the D3D11 backend (done 2026-11-28)

- **`gpu::d3d11::Backend`** (`src/gpu/d3d11/backend.{h,cpp}`, Windows only,
  linking `d3d11` and `dxgi`): `--renderer=d3d11` (`renderer.cpp`; anything
  else, or D3D11 on Linux, is Vulkan, named in the log). Direct3D 11.1:
  the draws' four constant blocks are ranges of one buffer
  (`*SetConstantBuffers1`, offsets of 256 bytes), copied from our memory
  into a dynamic buffer at each `BindResources` (`MAP_WRITE_NO_OVERWRITE`:
  no frame in flight reads the slot's ranges). The device and swapchain
  (flip discard, tearing allowed where DXGI has it) are made on the window's
  thread (`AttachWindow`), used on the render thread; frames in flight are
  event queries waited for at the slot's next `BeginFrame`. Uploads are
  `UpdateSubresource` in place (D3D11 orders them with the draws).
- **Shaders.** Each port's HLSL is found by its SPIR-V array
  (`kGamePortsHlsl`, written by `compile_game.py`; linked in on Windows
  only), compiled once with its fit's defines (`TranslatedShader::hlsl_defines`,
  carried on `gpu::ShaderDesc`) by `D3DCompile` from `d3dcompiler_47.dll`
  (loaded at start; Windows has it), kept in memory and on disk
  (`cache/d3d11_shaders/<hash>.dxbc`, by the hash of the HLSL, profile and
  defines). Vertex inputs are `TEXCOORD<location>`; the input layout is made
  with each pipeline.
- **Interpolators.** D3D links a vertex shader's outputs to a pixel shader's
  inputs by register, which follows declaration order (Vulkan by location):
  `to_hlsl` makes both stages declare the same 16 slots (`TEXCOORD0`-`15`,
  first), each interpolator copied to or from its slot (`XE_SLOT_n`, on with
  `XE_SLOT_n_ON`; an array input, the course's `v_mask[4]`, an element a
  slot). The renderer keeps the defaults, slot n for interpolator n:
  `SpecializePort` moves a pair's locations in both stages alike, so n
  always pairs with n (in the mask or not). Without the slots the buildings
  came out green and yellow.
- **Vertex and instance indices.** D3D's `SV_VertexID` and `SV_InstanceID`
  leave out the base vertex and first instance, which Vulkan's include (the
  shaders pass the index on as `r0`): the vertex shaders add them from a
  constant block at `b4` (SPIRV-Cross's
  `--hlsl-support-nonzero-basevertex-baseinstance`), set per draw (a
  non-indexed draw's vertex ID already counts from its first vertex).
- **What D3D11 lacks, done in the backend:**
  - views' swizzles (open question 4): a swizzled view is a copy of its
    image through `swizzle.frag`, drawn again (every level and layer) at the
    first bind after the image is written (each image counts its writes);
    compressed images' copies are RGBA8 (RG8 for BC5), a depth image's
    RGBA32F. Seen in races: red and blue exchanged (RGBA8,
    A2B10G10R10), RG16F with blue and alpha 1, a depth as RRRR;
  - scaled copies: `BlitImage` and the shown frame fitted to the window are
    draws (`blit.frag`), so an image that's a copy source is also sampled;
    a resolve or MSAA resolve between sizes goes through an image of the
    source's size; depth and multisampled copies are whole images (D3D11's
    rule);
  - triangle fans: non-indexed ones drawn as lists of indices made per draw;
    indexed ones aren't drawn (logged once; none seen in races or menus);
  - BC textures whose top level isn't a multiple of 4 (1x1 placeholders):
    kept decoded (RGBA8, RG8 for BC5), their uploads decoded on the CPU;
  - the alpha constant for color blending: the blend factor's color set to
    its alpha for such pipelines; alpha factors that name a color use its
    alpha; depth bias is whole units (D3D's `DepthBias` is an integer);
  - an upload's row length 0 is tightly packed, as Vulkan's (the movies were
    distorted without it).
- **The renderer's own passes** (`DrawResolve`, `DrawMovie`, `ApplyGamma`,
  the ID pass) use the HLSL of the same GLSL; the gamma needs typed UAV
  loads of RGBA16F (logged if missing).
- **Testing: DXVK, not wined3d.** Under Wine's own D3D11 (wined3d) the road
  (the projected course shader) draws black and the car darker; under DXVK
  (`winetricks dxvk`, 3.2, in the test prefix since 2026-11-28, the user's
  call) the frame matches Vulkan's. Windows players get their driver's
  D3D11, so wined3d's bug isn't ours to chase.
- **`--native_renderer_dump_at_tick=N`** (a debugging aid): the dump at race
  tick N, counted as `--state_digest` counts (`debug::RaceTicks`, counted
  in every run now), so two builds or backends dump the same moment (the
  frame-count trigger lands on different moments when frames carry more than
  one tick). Two Vulkan runs at tick 300: identical.
- **Checked (under Wine, DXVK):** an attract race at tick 300 against Vulkan
  (2,600 draws both; the frame differs in under 0.7% of pixels, edge pixels
  of depth ties and rounding; not chased, the user's call 2026-11-28: a
  second API is judged by eye and a drive, not pixel for pixel), the tick
  replay through the menus into race 0 (all ticks match, 0 error lines, no
  backend warnings), the boot movie with `--xbox_gamma=hdtv`, fullscreen.
  The user saw an attract race in the window. A drive is welcome.

## Step 6: clean-up (done 2026-11-28)

- `itc.toml` has `renderer = "vulkan"` (or `"d3d11"`, Windows only), with a
  comment; `--renderer=d3d11` elsewhere logs a warning and uses Vulkan.
- CLAUDE.md names this plan; `docs/windows.md` has DXVK in the test prefix.
- CI: both workflows build `d3d11` (runs 46143282902, Windows, and
  46143311543, Linux: success); the Windows build links `d3d11` and `dxgi`
  from the xwin SDK, nothing to add to the workflow or the package (Windows
  has `d3d11.dll`, `dxgi.dll` and `d3dcompiler_47.dll`).
- A Windows test zip: `itc_recomp/out/dist/itc-d3d11-win-amd64.zip` (packaged
  as `docs/windows.md` says).
- Drives by the user (2026-11-28, under Wine with DXVK): an attract race and a
  career race seen drawn; the movies were distorted (fixed: an upload's row
  length 0 is tightly packed); a parking area against Vulkan and v0.5.0
  alike. Two stops met on the way are the game's, not the renderer's
  (`docs/known-issues.md`: the new career's car dealer; a null pointer in the
  race-end camera). Left: one profile of each backend (no profiling until the
  end, as planned) and a test on Windows itself.

## Open questions

1. **Whether D3D11 is worth it over Vulkan on Windows.** Vulkan already runs
   there; D3D11 is for drivers or machines where Vulkan is poor. Not a
   blocker: the interface (step 3) helps either way.
2. **Feature gaps.** Things the Vulkan renderer uses that D3D11 lacks or
   does differently, to find in step 3: mirror-clamp-to-edge (D3D11 has
   `MIRROR_ONCE`), primitive restart (D3D11 always restarts on the strip
   cut index), rectangle lists (drawn as instances already), the depth
   formats, sampling a depth image. Answered in step 5 ("What D3D11 lacks").
3. **DXVK-native for Linux tests** (step 5). Not needed: the Windows build
   under Wine with DXVK tests the D3D11 backend.
4. **View swizzles.** D3D11's shader resource views have no component
   swizzle, which the texture cache's views carry (the fetch constant's
   swizzle, red and blue exchanged for handed images). Decided (the user,
   2026-11-27): the swizzle stays in `gpu::ViewDesc` and the renderer's
   logic and the shaders don't change; the D3D11 backend applies it itself
   (how, at step 5: for example a per-texture swizzle its own HLSL reads,
   or the common cases as format choices). Vulkan keeps its view swizzle.
   Done in step 5 as copies drawn through the swizzle.
