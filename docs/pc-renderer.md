# A PC renderer (a plan; steps 0 and 1 done, 2, 2b and 4 in progress)

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
| **1** | **The vertex arena** (done 2026-11-26, below; moved ahead of the shaders: the translated shaders read each stream at its fetch constant's address, which a draw can point anywhere). One device-local buffer holding the ranges draws use, copied from physical memory at their first draw and again after memcpy writes them | The 512 MB mirrors, their diagnostics and flags | Medium |
| **2** | **The shaders.** Bring in SBX's tool (its author's OK given, below); run it over our `BUILD_OUT` and course archives and the boot shaders, and compare hashes with their table; take their hand-written GLSL and give it PC inputs (vertex attributes, constants by name, plain textures), with the vertex data byte-swapped and its components in PC order where it's uploaded | The SDK translator on the draw path, the shader patch, `xe_vfetch` | Large |
| **2b** | **Resources by handle.** Textures and buffers made, filled and freed where the game does it; uploaded once into GPU memory, bound by handle; the GPU data written into the physical block only where the CPU reads it | memcpy's tracking, texture reloads by overlap; most GPU data in the physical block | Medium |
| **3** | **The handlers call our interface.** The 33 handlers and the singles in play call `render/` directly | The register shadow on the render path, `ReadRegisters`, the dirty masks, `replay_hooks.cpp`, `draw_packets.cpp`, `gpu_link.cpp`, the packets, the lifted shader load | Large |
| **4** | **Passes into their textures.** The resolves classified and replaced (above); render targets as textures | The EDRAM allocator, resolve copies but grab passes, the tiling machinery | Medium |
| **5** | **Clean-up.** The SDK translator and the `rexglue-sdk/` submodule out of the build; the physical block shrunk to what's left in it; step 0's measures again, and the one profile | ReXGlue entirely | Small |
| later | MSAA as a player option; a second backend; tick replay (`docs/tick-replay.md`) | | |

## Step 0's measures (2026-11-26)

`--log_memory_use` (new; `memory::DescribeResident`, logged by the
renderer every 5 s with the GPU memory line): each block's touched pages
(`mincore`), the process's resident set (`/proc/self/status`), and on
amdgpu the driver's VRAM and GTT in use (sysfs, the whole card). GTT is
system memory the driver pins for the GPU: it isn't in the process's
resident set, which is why the mirrors never showed there. The renderer
also logs which memory type each mirror got.

On this PC (amdgpu; idle before the runs: VRAM 3,017 MB, GTT 346 MB),
attract races `itc_5953` (60 s) and the pad replay `itc_5954` (menus, then a
race, 126 s):

| | Menus | Race |
|---|---|---|
| The process's resident set | ~510-580 MB | ~530-645 MB |
| of which the physical block (touched pages) | ~205-211 MB | ~220-255 MB |
| of which the heap | 70 MB | 71-84 MB |
| GTT, whole card (idle 346 MB) | ~1,690 MB | ~1,690 MB |
| The renderer's host-visible buffers in it | 1,344 MB: the two mirrors (2 × 544 MB, memory type 2, not device-local), texture staging (128 MB), the constant rings (128 MB) | the same |
| The renderer's device memory | textures 63-82 MB, resolve destinations 25 MB, images (targets) 208 MB | the same |

So the game holds about **1.9 GB of system memory**: ~0.6 GB resident in the
process and ~1.35 GB pinned by the driver, against ~0.3 GB of GPU memory.
The mirrors are 1.09 GB of it, and the physical block's ~250 MB is mostly
GPU data the CPU never reads again. The physical block's pages stay
resident after its data is freed (its allocator doesn't give pages back).
The other host-visible buffers (staging, constants, 256 MB) are sized
generously and can shrink with the steps.

The frame dump baseline is an attract race's dump (`--native_renderer_dump`,
10 s in), kept locally in `itc_recomp/out/pc-renderer-baseline/`: 2,390
draws, 24 resolves.

## Step 1: the vertex arena (2026-11-26)

`src/native/vertex_arena.{h,cpp}`. One device-local buffer (224 MB: the
built-in index lists for rectangle and quad lists, 16 MB of inline vertices
a frame slot, 192 MB of ranges) in place of the two 544 MB host-visible
mirrors:

- A draw places each stream its vertex shader reads, by its fetch
  constant's address and size (no draw reads past the size: checked with
  the old per-draw mirror's overrun count, none in an attract race), and
  its index range; the fetch constants handed to the shaders get the
  arena offsets in place of the addresses, and the index buffer is bound
  at its range's offset. Inline draws copy their vertices into this frame's
  slot.
- Copies go through per-frame staging (16 MB, more chunks while a load
  needs them) and a command buffer submitted after the texture uploads,
  before the frame; it starts with a barrier on earlier frames' reads, so
  a range copied again in place waits for the draws still reading it.
- memcpy's writes into physical memory (`Buffers_TakeMirrorWrites`, at each
  frame's start) mark the ranges they overlap; those are copied again at
  their next draw. Ranges not drawn for 900 frames are given back (once no
  frame in flight can read them), sooner when it's full.
- Gone with the mirrors: `--native_renderer_mirror_by_tick`, `_to_both`,
  `_verify_mirror`, `_check_mirror`, `_copy_all`, `_check_late_writes`,
  `_check_clobbers`, `_cache_index_ranges`, `_log_index_ranges` (the run
  scripts and CLAUDE.md no longer pass the first two).

Checked: attract races (`itc_5957`, 60 s; 0 error lines, 0 draws left
out), the pad replay (`itc_5959`, menus and a race, 126 s, 0 error
lines), a frame dump against step 0's (`draws.txt` the same length, 2,390
draws; the shown frame 0.08% of pixels off by more than 0.02, within runs'
spread; a resolve identical). The arena holds ~14-25 MB of ranges in ~1,500.
**GTT ~1,690 -> ~630 MB** (whole card; idle 346 MB): the game's pinned
system memory from ~1,345 to ~285 MB (texture staging 128 MB, the constant
rings 128 MB, the arena's staging 32 MB).

## Step 2: the shaders (in progress)

**Part 1 (2026-11-26): every shader drawn from GLSL.** SBX-Recomp's tool,
ports, hand-written shaders and renderer code brought in
(`docs/shaders.md` says what came and what differs here). In an attract
race all 74 shaders used come from GLSL and none is translated; the
translator now runs only for a shader with no GLSL (logged). Our graphics
library's vertex fetch patch differs from SBX's (sorted, merged into
mini-fetches, swizzles rewritten), so `RemapVertexFetches` rebuilds each
vertex shader's file microcode and fetch fields from its object
(`shader_ports.h`), and a new swizzle constant gives the declaration's
component order. Checked: attract races (0 error lines), a frame dump
against step 0's (0.27% of pixels off by more than 0.02: GLSL's arithmetic
against the console's), and a race frame from the pad replay with the
colors right.

Found on the way and fixed: two calls by address from lifted code to hand
ports with no dispatch entry (`known-issues.md`, "Calls by address with no
dispatch entry"; `--check_call_targets`).

Left: the PC-style inputs (vertex attributes, constants by name, plain
textures), with step 3, where the draw state stops coming from the
registers.

**The order from here (2026-11-26).** The faked GPU (`gpu_link.cpp`) only
makes fences in play now (~3,000 a second, two a kick; no memory writes,
interrupts or callbacks in a race), and it goes with step 3, when the
handlers stop calling the game's Direct3D and so stop writing packets: an
interim for the fences alone would be thrown away. So step 4 (the render
targets, the EDRAM part of the goal) comes next, then step 3, then 2b and 5.
Step 4's approach:

- Targets keyed by the game's surface object instead of its EDRAM base
  (the renderer stops knowing EDRAM).
- A resolve that is a plain copy hands the target's image to the
  destination texture (the pass has drawn into the texture: no copy). A
  target drawn into again without a clear first (a grab pass, the main
  view after its first resolve) gets the image copied back once.
- MSAA resolves, format changes and the exponent bias (-3 on the main
  view's and the glow's copies) stay copies, as PC renderers do them.

## Step 4: render targets (first part done 2026-11-26)

- **Targets keyed by the surface object.** `Passes::State::GetTarget` keys
  an image by the game's surface object, its size (the object's +36:
  width - 1 from bit 18, height - 1 from bit 3), samples and format; the
  EDRAM base and the "as tall as the EDRAM left" height are gone from the
  renderer.
- **Surfaces take no EDRAM.** `graphics::CreateSurface` no longer calls the
  EDRAM allocator (base 0, or the parameters'), `DestroySurface` frees
  none, and `graphics/edram.{h,cpp}` are removed.
- **Explained difference:** the camera pass of type 20 (resolve 6 in
  `conventional-renderer.md`'s table: depth only, into `[0x826BABC4]`,
  use unknown) has its own depth surface sharing EDRAM with the shadow
  map's (type 10); on the 360 it starts with the shadow map's depth, and
  its 4 draws in an attract race write nothing. It now starts from its own
  image (cleared: all 1.0), as the no-EDRAM rule has it. The shown frame
  doesn't change (an attract race's dump: 0.30% of pixels off by more
  than 0.02, as step 2's).
- **What stays:** the ~25 resolves a race frame are copies, because none
  is a plain copy: each changes format, averages MSAA samples or applies
  the exponent bias (the old table's "What a resolve does"), as a PC
  renderer's StretchRect or MSAA resolve does. Handing images over instead
  of copying would save no copy today. Drawing passes straight into their
  destinations needs the bias moved into the readers and a pass plan (the
  old branch's 3a), for later.

Order of a race frame's passes (one frame traced, 2026-11-26): each camera
pass draws, then resolves (the shadow map's 66 draws, then its depth
resolve); the main view draws, is resolved twice, the glow's steps draw
and resolve on one 256x144 target in turn, then the main view draws on
(the grab pass) and is resolved for the scene target, then the front.

## Step 2b: XPR textures held by the GPU (first part done 2026-11-26)

`--log_memory_use` now also lists the physical memory's live blocks by who
asked (`memory::PhysicalTag`): in the pad replay's menus, XPR textures
109.4 MB (53 files), model vertices 9.5 MB, the rest 42.9 MB; the XPR
files are where the GPU data is.

- **XPR textures uploaded at load, their pages given back**
  (`--give_back_xpr_data`, on): `XPR::Create` hands the renderer its data
  block and each texture's fetch constant (`TextureCache_XprLoaded`); at
  the next frame's start each is uploaded as a draw would, and once all are
  on the GPU the block's pages are `madvise(MADV_DONTNEED)`d. Those
  textures stay through race ends (they can't be loaded again) and leave
  when `XPR::Release` says so (`TextureCache_XprReleased`). Only files whose
  records are all textures.
- **Freed physical blocks' pages given back** (`memory::FreePhysical`,
  always): a freed block's data stayed resident until something else took
  the space.
- **The fault found on the way:** the headers' base and mip addresses
  (fetch words 1 and 5) are the CPU's view of the physical memory
  (`0xE0000000`, 4 KB back), as `RelocateTexture` writes them, and
  `SetTexture` binds the GPU's. Uploaded under the headers' words, no draw
  found them, and draws read the given-back zeros (the menus' 2D elements
  and much of a race, the user's report). They're translated
  (`GpuPhysicalAddress`) before the upload. The sampler fields `SetTexture`
  merges in (clamps, filters, LOD) aren't in the texture's key (`KeyOf`),
  so every variant shares the one image. A draw reaching a given-back XPR
  with a key not uploaded, and memcpy writing into one, are logged as
  errors: none in the checks.

Checked: an attract race's frame against step 0's (2,390 draws, 0.08% of
pixels off by more than 0.02), attract races (0 error lines), the tick
replay (both races' digests match), and the pad replay (menus, the garage,
the tuning shop, both races with their HUD, the pause menu, the return to
the garage: frames right, 0 error lines). **The physical block's resident
pages: ~205 -> ~33 MB in the menus, ~250 -> ~64 MB in a race; the process's
resident set ~510-645 -> ~400-490 MB.**

Left for 2b: the model vertices and indices (~10-40 MB) by handle, which
waits on step 3 (draws find their streams by address until then).

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
