# A PC renderer (a plan, not started)

Written 2026-11-26 on `pc-renderer` (branched from `main` at `a56e837b`,
after E5's step 10 was merged). This is a plan: no code has changed for it.
It is built from three surveys of `src/` (the game's graphics library, our
Vulkan renderer, and what reads and writes GPU data) and from SBX-Recomp's
shader work (`../SBX-Recomp` at `b128953`, read on the user's word).

The first attempt, `docs/conventional-renderer.md` (the `conventional-renderer`
branch, 2026-11-01 to 03), is a dead end (the user's call): its branch is
1,344 commits behind and was never merged. Its research still holds and is
cited here (its section 2, every resolve of a race frame; its section 3,
the 360 machinery), and so are its lessons (frees applied one list later;
the wheels' morphs writing over made shapes; the vinyl sheet read by the
CPU). Its code isn't reused.

## The goal

The user, 2026-11-26: turn the game's graphics code into a regular PC-style
renderer. No EDRAM, no GPU results written back to system RAM, and no GPU
data held in system RAM unless the CPU needs it. The motive is the memory
footprint: today every vertex, index and texture sits in the 512 MB
physical memory block, and the renderer keeps two more 512 MB copies of
that block (host-visible, so in system RAM on a discrete GPU) because the
translated shaders fetch vertices from it by address.

The user's decisions so far:

1. **The game's shaders become GLSL with PC conventions**, made by an
   offline tool from the microcode (not by hand), its output committed as
   source. SBX-Recomp has such a tool and hand-written GLSL for all 135
   shaders; we start from theirs (below).
2. Earlier calls that still apply: frames that look correct are the bar,
   not matching the 360's device state, and no harnesses on the render
   path; no EDRAM behavior (a pass starts cleared or with what its image
   holds); no profiling until all of this is done.

## How a frame is made today

From the surveys (paths under `itc_recomp/src/`):

- **The main thread records a command list** (`command_list/`): 16-byte
  slots `[id, arg1, arg2, 0]` plus payload, double-buffered, about 50
  recorders (`docs/functions/command-list.md`). This is the game's own
  design, not a 360 mechanism, and frame interpolation is built on it
  (logic and render-only lists, command 7's constants). **It stays.**
- **The render thread replays it** (`graphics/render_queue.cpp`,
  `RunCommands`) through 33 command handlers (`graphics/replay_commands.cpp`;
  the table in `docs/gpu-functions.md`). Each handler calls the game's
  Direct3D, which writes the **register shadow**: render states, fetch
  constants (textures and vertex streams), float constants, dirty masks
  (`graphics/device_state.cpp`, 131 setters).
- **Our renderer reads that shadow back** after each command
  (`native/replay_hooks.cpp`, `native/state.cpp`'s `ReadRegisters`) and
  rebuilds a Vulkan draw (`native/passes.cpp`'s `State::Draw`): the
  target keyed by EDRAM base, pitch, MSAA and format; the shaders
  translated from the microcode by the SDK's translator; vertices and
  indices from the 512 MB copies by guest address; textures keyed by the
  fetch constant's words, reloaded when memcpy writes over them.
- **Resolves** copy a target into an image looked up by the destination
  texture's 4 KB page; later draws find it by address. About 25 a race
  frame (`conventional-renderer.md`, section 2). Only the GPU reads them:
  nothing rendered is written to guest memory, and no shader uses
  memexport.
- **Faked GPU side effects:** `native/gpu_link.cpp` walks the command
  buffers the game kicks and makes their memory writes (fences, MEM_WRITE,
  REG_TO_MEM, interrupts calling the library's callbacks), which the
  library's waits read.

## What the CPU needs of GPU data

From the old branch's read watch (attract races, a drive) and the code:

- **Read by the CPU, must stay in system memory** (or keep a CPU copy):
  - the vinyl sheet (CPU-drawn by the decal rasterizer, read by
    `XGCompressSurface`; the compressed result is GPU-only);
  - the license plate's and decal images' textures while they're written
    (`Resource::Lock` users);
  - the morph sources: the wheels' set-up morphs read a shape's base
    streams and write them back; the per-draw morphs read base and target
    streams on worker threads and write blended vertices into the frame's
    work memory;
  - the billboards' and inline draws' vertices (written per frame; already
    copied per frame).
- **Never read by the CPU after loading:** model vertex and index data
  (`Mesh::Parse`, `Model::Load`), XPR textures (`XPR::Create`), resolve
  destinations, render targets, front buffers. Collision has its own mesh
  data; the minimap reads only matrices.
- **Shader microcode** is read once (for translation now; for the shader
  lookup later).

## The design

### The seam: the handlers call our renderer

The command list and its replay stay. The 33 handlers (and the single
commands that run in play: 5, 7, 13/14, 15, 28) stop calling the game's
Direct3D and call **our renderer's interface** instead (`render/`, new),
with PC-style calls:

- state: blend, depth, stencil, raster and sampler states as values (the
  game's D3D render state numbers mapped once, in the handler);
- `SetTexture(stage, Texture)`, `SetVertexBuffer(stream, Buffer, offset,
  stride)`, `SetIndexBuffer`, `SetVertexLayout` (from the game's vertex
  declaration), `SetShaders(vs, ps)`;
- `SetConstants(stage, first, count, data)`: the game's float constant
  registers (c0-c255, D3D9-style) uploaded as a uniform block. Command 7's
  interpolation substitution stays where it is, before this call;
- `SetRenderTarget(slot, Texture)`, `SetDepthTarget`, `Clear`, `Draw`,
  `DrawIndexed`, `CopyTexture` (for a grab pass), `Present`.

The handlers are plain C++ on the command list's payload; nothing on the
render path writes the register shadow, the dirty masks, PM4 packets or
fences. The game's Direct3D stays only where game code outside the replay
needs it (resource creation, below).

### Resources: created once, held by the GPU

Textures and buffers become renderer objects, made where the game makes
them (`graphics/resources.cpp`'s `CreateTexture`, `CreateVertexBuffer`,
`CreateIndexBuffer`; `Mesh::Parse` and `Model::Load`; `XPR::Create`) and
filled where the game fills them (the loaders' copies, `Resource::Unlock`,
the morphs). The header (the game's small heap block) keeps a handle to
the object; the handlers bind by handle.

- Data the CPU never reads goes **straight to device-local memory** through
  a staging upload, and is never written to (or is freed from) the
  physical block.
- Data the CPU reads keeps its system memory copy, and its writer tells
  the renderer when it changed (`Texture::Written`, `Buffer::Written`).
- Per-frame vertices (billboards, inline draws, post-effect quads, the
  per-draw morphs' output) go through a per-frame upload ring.
- Frees are applied one list later (the old branch's lesson: a list
  handed over can still draw a texture freed after it).

With this, memcpy's write tracking (`native/buffers.cpp`), the texture
reloads by overlap, and the 512 MB copies go.

### Shaders: GLSL with PC inputs

From SBX-Recomp (`docs/shaders.md` there; `sbx_pc/tools/shader_port/` and
`sbx_pc/src/renderer/vulkan/glsl/`):

- a microcode-to-GLSL tool (`ucode_to_glsl.py`, `xenos_ucode.py`,
  `port_all.py`) that found all 135 shaders (30 vertex, 105 pixel) and
  wrote a register-by-register port of each, checked bit for bit against
  the SDK's translation (`tools/shader_port/check`);
- **hand-written GLSL for all 135**, which is what they ship: `lit.frag`,
  `unlit.frag` (car parts), `course.frag`, `course_projected.frag`, the
  post effects, the boot sprites and font, and two vertex shaders,
  `model.vert` and `screen.vert`, with defines per shader
  (`variants.txt`).

They kept the translator's interface on purpose, so theirs still read
vertices through `xe_vfetch` from guest memory as one storage buffer, with
the translator's constant packing and system constants. **We change the
interface**:

- vertex shaders take **vertex attributes** (`layout(location = n) in`),
  their formats and strides from the game's vertex declaration as a Vulkan
  vertex input state; the game's shader patcher (`sub_822AE340`) and its
  fetch fields go;
- constants as uniform blocks by name (the shader's constant table gives
  the names and registers);
- textures as ordinary combined image samplers, already in RGBA order and
  in PC formats (the loader converts or the view swizzles); no signed and
  unsigned views picked by system constants;
- the exponent bias, the alpha test and the gamma conversions as uniforms
  or defines where the game uses them.

Shaders are found by the microcode's hash (pixel) or the layout-free hash
(vertex), as SBX does. A shader the table lacks stops the program, named,
so a missing one is found at once.

### Render targets: passes into their textures

EDRAM surfaces become render target textures. Each resolve in
`conventional-renderer.md`'s section 2 is classified once (the old plan's
table holds most of the answers):

- a pass whose result is only resolved and then cleared **draws straight
  into its destination texture** (most of them: the camera passes, the
  glow chain, the motion blur's velocity, the minimap);
- a depth resolve becomes the depth image sampled as depth;
- an MSAA resolve stays as a standard MSAA resolve (`vkCmdResolveImage` or
  a resolve attachment);
- a copy in the middle of a pass (the main view, then ~350 more draws)
  stays as a `CopyTexture` (a grab pass);
- predicated tiling is drawn once into one target (as now);
- the front buffer is our own frame image, presented.

The EDRAM allocator (`graphics/edram.cpp`), `Device::Resolve`'s register
work, and the tiling machinery go.

### What else goes

`native/state.cpp` (the shadow read), `native/draw_packets.cpp`,
`native/replay_hooks.cpp`, `native/gpu_link.cpp` (the library's fence
waits and callbacks are dropped or answered at once instead), the command
buffer's packet writers, `Device::LoadShaders` (lifted), the shader patch,
and **the SDK's shader translator**: with every shader ours, the
`rexglue-sdk/` submodule leaves the build, which ends the ReXGlue removal.

### What stays

The window, device and swapchain code (`native/renderer.cpp`), frames in
flight, the constant ring, the pipeline cache (and its disk cache), the
texture upload and deferred retirement, the gamma and movie passes, and
frame interpolation's four joins: command 7's constant substitution, the
logic and render-only lists, the glow's history pass, the motion blur's
previous view-projection.

## The steps

Each step keeps the game running, is checked by frame dumps (against the
step before, differences explained), interpolation's logs
(`--log_blend_state`), an attract race and the pad replay, and a drive by
the user before the next. No profiles until the end.

| Step | What | Removes | Size |
|---|---|---|---|
| **0** | **Measure.** Log resident memory by source (the physical block's touched pages, the two copies, texture staging) and the renderer's GPU memory, in a race and the menus; a frame dump baseline | Nothing | Small |
| **1** | **The shaders.** Bring in SBX's tool (its author's OK given, below); run it over our `BUILD_OUT` and course archives and the boot shaders, and compare hashes with their table; take their hand-written GLSL and give it PC inputs (vertex attributes, constants by name, plain textures). In this step the renderer binds the 512 MB copy itself as the vertex buffer with each stream's guest address as the offset, so the shaders change before the buffers do | The SDK translator on the draw path, the shader patch, `xe_vfetch` | Large |
| **2** | **Resources by handle.** Textures and buffers made, filled and freed where the game does it; device-local, uploaded once; bound by handle. CPU-read data keeps its system copy and its writers announce changes | The 512 MB copies, memcpy's tracking, texture reloads by overlap; most GPU data in the physical block | Medium |
| **3** | **The handlers call our interface.** The 33 handlers and the singles in play call `render/` directly | The register shadow on the render path, `ReadRegisters`, the dirty masks, `replay_hooks.cpp`, `draw_packets.cpp`, `gpu_link.cpp`, the packets, the lifted shader load | Large |
| **4** | **Passes into their textures.** The resolves classified and replaced (above); render targets as textures | The EDRAM allocator, resolve copies but grab passes, the tiling machinery | Medium |
| **5** | **Clean-up.** The SDK translator and the `rexglue-sdk/` submodule out of the build; the physical block shrunk to what's left in it; step 0's measures again, and the one profile | ReXGlue entirely | Small |
| later | MSAA as a player option; a second backend; tick replay (`docs/tick-replay.md`) | | |

## Open questions

1. **SBX-Recomp's license.** Answered 2026-11-26: its author is OK with
   the user using its code. Files taken from it say so in a header comment
   (where they came from, at which SBX-Recomp commit), as
   `src/lifted/vector_helpers.h` does for the SDK's.
2. **Whether our shaders are SBX's.** Their doc says SBX's graphics library
   is a different revision from ours, and the Japanese release's data may
   differ: step 1 starts by running their tool over our files.
3. **The exponent bias** on the main view's and the glow's copies (−3):
   folded into the readers' sampling (a uniform), or a float target with
   the bias applied where it's read. Decided in step 4.
4. **The upscale draw and the 1280x720 `1ECB9000` texture** (the main
   view's partial-rect resolve): drawn into that region, or the frame
   composed natively (`docs/unlocked-res.md`, "Later"). Decided in step 4.
