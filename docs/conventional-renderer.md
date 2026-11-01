# A conventional renderer (a plan; steps 1, 2 and 2b done)

Written 2026-11-01. This is a map and a plan only. No code has changed,
and nothing here was built, run or profiled for it. It is built from
the docs named in each section and from reading `src/` and `generated/`
on `b7-loop-01-11-26` (the B7 loop's branch, at `b287c74c`). Line
numbers are as of that commit. "Read" means read in the code,
"probably" means inferred, and "not read" means known only from a doc
or a call site. The work itself will be done on its own branch once the
B7 loop's current run wraps up (the user's call).

## The goal and the user's decisions (2026-11-01)

The aim is a PC renderer, not a translator of the 360's GPU state:

- passes draw straight into their textures, and readers sample them;
  no resolves out of EDRAM;
- our renderer is fed by native code through **our own interface**, not
  through the game's Direct3D. The game's Direct3D (its setters, the
  register shadow, its draws) stops being called on the render path;
- the guest memory mirrors and the register shadow go as their readers
  go.

The user's decisions:

1. **Resolves go because they're Xbox behavior, not for speed.** (They
   cost under 1% of the render thread: ~25 a frame, each one fullscreen
   triangle, "`Resolve` 0.8%" in `native-renderer.md`'s profiles.)
2. **The interface is ours, and backends are swappable behind it.**
   Native code calls our renderer directly, never the game's Direct3D.
   Vulkan comes first; a D3D11 backend (feature level 10_0, Windows only)
   could come later, so the interface doesn't assume Vulkan.
3. **Shaders for other backends may be translated offline**: the game's
   finite shader set into stored blobs per backend, as
   `src/graphics/compiled_shaders.h` did for the HLSL compiler.
4. **MSAA becomes a player option in `itc.toml`**, as in a typical PC
   game, instead of the sample counts the 360's EDRAM budget chose: one
   fixed value applied to every target the game drew with MSAA (the
   main view, the mirror, the reflections, the minimap, the menus' 3D
   view). Its default is still to pick.
7. **Precision differences are acceptable**: a pass drawn straight into
   its destination's format (more precise than the 360's 8-bit target
   then converted) may differ slightly in dumps, as an explained
   difference.
8. **The Vulkan backend translates shaders at runtime**, as now (the
   SDK's translator and the disk cache); the offline blobs are for other
   backends.
9. **GPU data may leave guest memory now** (step 2b), ahead of the
   decision on guest memory as a whole that B7 left "to the end", since
   most graphics code is native. The headers stay in guest memory, and
   so does any data the CPU also reads.
5. **Shadows and reflections once a tick** is interpolation's question,
   not the renderer's: left out of this plan.
6. **The work goes on its own branch** after the B7 loop's run; this
   doc is committed where the tree is.
10. **Not GPU emulation: the bar is frames that look correct** (the
   user's call, 2026-11-01). Nothing on the render path has to reproduce
   the game's Direct3D or the 360's device state (the register shadow,
   the packets). No harnesses compare render-path code with the
   recompiled version, and no step exists only to match device state.
   This isn't B7 work: B7's "match the recompiled code exactly" and its
   harness rule don't apply. Changes are checked by frame dumps against
   the current renderer (differences explained), interpolation's logs and
   the user's drives.

## Summary

- **Most of the graphics layer is native now** (confirmed):
  - the state setters (`device-state.md`: 129 of 131 Overridden or
    Native, 2 Dropped);
  - the shader setters and the render queue's replay
    (`device-shaders.md`);
  - the resources (`graphics-resources.md`, 46 of 46);
  - the command list, the kick and memcpy.

  **Still recompiled in the frame path**:
  - the 33 command handlers (wrapped, `src/native/replay_hooks.cpp:19`);
  - the shader load `sub_822AED18`, called from
    `src/native/draw_packets.cpp:215`;
  - the vertex shader patcher `sub_822AE340`;
  - BeginVertices and EndVertices;
  - Clear `sub_822A04F8` and Resolve `sub_822AFA58`;
  - predicated tiling;
  - `Renderer::DrawFrame` and `Scene::Draw`;
  - one resolve recorder (`sub_8209B3D0`).
- **The seam is the command handlers.** The game records a command list
  on the main thread and the render thread replays it. That's the game's
  own design, not a 360 mechanism, and it stays. Each handler decodes a
  command and calls the game's Direct3D. Native handlers that call our
  interface instead take the whole game Direct3D off the render path in
  one place, along with the register shadow's writes, the PM4 packets,
  the kick, the shader load and patch, and the resolve and clear code.
- **Frame interpolation is unaffected by the change of seam.** It blends
  in the command list's payload at command 7, before the values are
  passed on (`BlendState_ReplayConstants`,
  `src/interpolation/blend_state.cpp:3968`). A native command 7 that does
  the same substitution and then calls our interface keeps it exactly.
  The other rules to keep are the replay order and one list per frame;
  the glow's history pass must also stay a per-frame operation in order.
  Only the tick-replay lead (`tick-replay.md`) would need it rekeyed.
- **Resolves are not all the same thing.** Most are EDRAM artifacts that
  disappear when a pass draws into its texture. Under them are four kinds
  of real work that a PC renderer keeps in its own terms:
  - MSAA's resolve (now the player's option);
  - two mid-pass copies of the scene (a "grab pass", standard on PC);
  - depth sampled as a texture;
  - the glow's ping-pong.

  The 360's red and blue swap and the format changes are layout details
  that go away. The exponent bias needs a closer look (section 2).
- **The renderer can know a pass's destination before drawing it**
  without changing what the game records: the whole list is recorded
  before it's replayed, so a native replay can scan from a pass's
  render target set to its resolve and bind the destination texture from
  the start (section 5).
- **Recommended first step**: step 0 measures and logs. It gives a
  profile baseline, so nothing gets slower unseen, and a log of each
  pass's resolves: what each source is drawn into after, which
  destinations are sampled before being written (done: "Step 0's
  findings"). Step 1 then writes our own command handlers feeding our
  interface, and takes the game's Direct3D off the render path, checked
  by how the frames look (decision 10; it was three steps before).

## 1. How a frame is made today

Driven from `src/frame_pacing/renderer_hooks.cpp:173`
(`Renderer_ThreadLoop`): the render thread waits for the main thread's
pass, then `Renderer_DrawFrame` (`:157`) runs the frame. One thread
replays, records Vulkan and submits; there are no host worker threads
in `src/native/` (read).

| # | Layer | Written by | Read by | Where | Native? |
|---|---|---|---|---|---|
| L1 | **The command list**: 16-byte slots `[id, arg1, arg2, 0]` plus payload, double-buffered, one tick behind | Main thread, the game's recorders during Draw (every pass, render-only ones included) | L2 | Guest memory (scene + 8); `src/command_list/` | Native list; recorders partly native (B7) |
| L2 | **The render queue**: pass lists sorted by key, chunk chains walked, commands dispatched through `0x8265EDE0` (single commands through `0x8265ED48`) | Main thread (`RenderQueue::Add`) | Render thread, `RenderQueue::Replay` | `src/graphics/render_queue.cpp:382` (replay), `:349` (walk), `:254` (dispatch) | Native since 2026-11-01; calls the handlers as guest code (`src/hooks/render_queue.cpp:29`) |
| L3 | **Command handlers** (ids 1-35): decode the payload, call the game's Direct3D | — | — | `generated/`, wrapped by `REPLAY_COUNTED_HANDLER` (calls `Renderer_OnCommand` before and `Renderer_AfterCommand` after); command 7 wrapped in `blend_state.cpp:4348` | **Recompiled** |
| L4 | **The game's Direct3D state**: the register shadow (fetch constants at device +1024, float constants at +1792 and +5888, register blocks +10240 to +10552), the bound objects (+12180 to +12436), the dirty masks (+0 to +32) | Native setters (`src/graphics/device_state.cpp`, `device_shaders.cpp`, `resources.cpp`); the recompiled shader load | L5, L6 | Guest memory, the device at `[0x8296BD90]` | Native except the shader load and patcher |
| L5 | **The game's Direct3D draws**: DrawIndexedVertices without packets (`DrawPackets_IndexedDraw`, `draw_packets.cpp:173`), the shader load and patch, BeginVertices and End, Clear, Resolve, tiling | L3 | L6, through the shadow and the hooks | `draw_packets.cpp`, `replay_hooks.cpp`, `generated/` | Mixed |
| L6 | **Our renderer**. It rebuilds the register file from the shadow by the dirty masks (`ReadRegisters`, `src/native/state.cpp:139`) and finds the target (`GetTarget`), shaders (`ShaderLookupTable`), pipeline (`PipelineKey`), texture set (`BindingTable`), constants (from the shadow, byte-swapped into a ring, `PlaceConstants` `:2404`) and the mirrors. Then it records the Vulkan draw (`State::Draw`, `passes.cpp:3346`) | `Renderer_AfterCommand` → `Passes::AfterDraw` (`:4346`); `AfterClear` `:4404`, `AfterResolve` `:4449`, `InlineDraw` `:4366`, `FrontBufferResolve` `:4609`, `TiledResolve` `:4573` | The GPU | `src/native/` | Native; SDK headers (`rex/graphics/register_file.h`, `xenos.h`, `util/draw.h`) and the SDK's shader translator |
| L7 | **Submit and present**: the upload command buffer and the frame's in one `vkQueueSubmit`; a blit into the swapchain; present | `Renderer::EndFrame` | — | `src/native/renderer.cpp:790`, `:895`, `:905` | Native |
| side | **The PM4 command buffer** (device +40, segments, ring at +13496) and the kick | The game's Direct3D code still recompiled | Only `gpu_link.cpp`'s `ApplyBuffer` (`:347`): fences, MEM_WRITE, REG_TO_MEM, REG_RMW, COND_WRITE, INTERRUPT | Guest memory | Kick native (`gpu_link.cpp:485`); writers recompiled |

So today: native recorders → the command list → native replay →
**recompiled handler** → the game's Direct3D (mostly native) writing
**Xenos registers into guest memory** → our renderer reading those
registers back and rebuilding Vulkan state for every draw. In the target
design, L3 becomes native and calls our interface; L4, L5 and the side
row leave the render path; L6 and L7 become the Vulkan backend.

## 2. Every resolve

From `gpu-functions.md` ("A race frame's passes", one trace,
`itc_353.log`), `native-renderer.md` ("Progress on milestone 4"), the
recorders read in `generated/`, and `src/native/passes.cpp`. Command
25's items are allocated by `CommandList::AllocateResolve`
(`sub_8258CC30`), which has five recorders (`command-list.md:105`):

- `CameraPass::RecordEnd` (native, `src/cameras/cameras.cpp:1201`), and
  `sub_820A1CA8`, its type-3 chunk;
- `PostEffect::RecordResolve` (native,
  `src/post_effects/post_effects.cpp:346`);
- `sub_8209B3D0` (recompiled, no row; calls at `0x8209B8AC` and
  `0x8209BAD8` checked);
- `ScreenObject::Record` (not reached in play).

Two resolves go around command 25 and call `Device::Resolve` directly:
the front buffer's (`sub_8258D998`) and EndTiling's (one per tile).

**How a resolve is made now** (read): `State::Resolve` (`passes.cpp:4480`)
takes the source (render target `flags & 3`, or the depth surface with
`0x4`) as our `Target` image. `ResolveInto` (`:2806`) then:

- ends dynamic rendering;
- transitions both images;
- allocates a descriptor set;
- draws a fullscreen triangle with `glsl/resolve.frag`, which averages
  the samples, scales by `2^(flags >> 26)` and swaps red and blue when the
  destination's swizzle says so;
- writes into the destination's image (`TextureCache::GetResolveDestination`,
  one per 4 KB page of the base address and scale, `textures.cpp:1473`);
- applies the `0x100` and `0x200` clears afterwards.

Later draws whose fetch constant points at that base get that image
(`LookUpView`, `textures.cpp:1313`). Nothing is written back to guest
memory.

Targets are keyed by depth or color, EDRAM base, pitch, MSAA, Vulkan
format and tiling (`passes.cpp:2491`). Passes with the same key draw into
the same image, as the 360's passes share EDRAM. That's why a copy is
needed now: the next pass would overwrite the source before the reader
samples it.

**What a resolve does, and what each part becomes:**

| Part of a resolve | Xbox reason | In a PC renderer |
|---|---|---|
| The copy itself (EDRAM to memory) | Render targets live only in EDRAM | Gone: the pass draws into its texture |
| Clear after (`0x100`, `0x200`) | The next pass reuses the EDRAM | Gone: the next pass has its own target, cleared at load |
| Red and blue swap | The texture is laid out BGRA in memory and its swizzle swaps it back when sampled | Gone: one image with the swizzle in the view (probably the view of the destination's own swizzle, applied to an image drawn in RGBA order; to check per reader) |
| Format change (8-bit to 10/16-bit) | The destination's format is the readers'; EDRAM formats are few | Draw in the destination's format. More precision in some passes, so pixels may differ slightly (explained differences) |
| Depth to color (`24_8_FLOAT` to R32F) | EDRAM depth isn't a texture | Sample the depth image's depth view (`D32_SFLOAT_S8_UINT`, `passes.cpp:69`). The translator samples a depth texture's `.r` (probably) |
| MSAA average | EDRAM holds the samples; memory holds one | Standard MSAA resolve, at the player's MSAA setting (`itc.toml`); with MSAA off, nothing |
| Exponent bias (−3 on the main view's and the glow's copies) | The 7e3 HDR format's range, copied into 16_16_16_16 (probably a fixed-point −32..32 format scaled down) | **Open.** Either the readers sample with the bias folded in, or the pass draws into a float image and the bias is applied where it's read. The texture fetch constant has an `exp_adjust` field (`rex/graphics/xenos.h:1239`), but the SPIR-V translator was only seen applying the vertex fetch instruction's (`spirv_translator_fetch.cpp:445`), not the texture's (not checked further). If neither works, one small scale pass stays, as a PC tone mapper would have |
| A partial rect into a bigger texture (the main view's 1024x576 into the 1280x720 `1ECB9000`) | The upscale draw samples the 3D view from part of a frame-sized texture | The frame composed natively (`unlocked-res.md`, "Later"), or the 3D view drawn into that region. The upscale's pixel shader must be read first |
| A copy in the middle of a pass (the main view resolved, then ~350 more draws) | The game reads the scene so far (transparent or refracting objects, probably) | **Stays, as a copy**: a "grab pass" is a normal PC technique. An explicit `CopyTexture` in the interface |
| One EDRAM target reused by every step (the glow chain) | EDRAM budget | Each step draws into its own destination: an ordinary ping-pong |
| Tiles (predicated tiling, menus) | The target doesn't fit in EDRAM | One full target (already the case: `native-renderer.md`, "Predicated tiling") |
| The front buffer | Scan-out reads memory | Present from the frame's texture (already: `FrontBufferResolve` copies nothing, `passes.cpp:4609`) |

**The resolves of a race frame:**

| # | Recorder | Source | Destination | What it does | Read by, when | In a PC renderer | Interpolation near it |
|---|---|---|---|---|---|---|---|
| 1-3 | `RecordEnd`, camera pass type 11; then the boot's target filter (effect 14) twice into the same texture | 8_8_8_8, pitch 560, + depth | 8_8_8_8 512x512 (`[0x826BABC8]`) | Copy; then a blur in place | The projected texture (the underglow, probably) | The pass draws into the texture; the filter ping-pongs between it and a second image | The pass's camera (perspective, blended) |
| 4 | `RecordEnd` type 12, mode 0 | 8_8_8_8, pitch 1040 | 8_8_8_8 1024x1024 (`[0x826BABD4]`) | Copy of a target only cleared | Not known | The texture cleared | — |
| 5 | `RecordEnd` type 10, flags `0x204` | Depth only, pitch 1040 | 24_8_FLOAT 1024x1024 (`[0x826BABDC]`), R32F now | Depth to color, clear after | The shadow map (probably the headlights' or the car's), in the main view | The depth image sampled | Orthographic camera, never blended by design |
| 6 | `RecordEnd` type 20, flags `0x4` | Depth only, pitch 1040 | 24_8_FLOAT 1024x1024 (`[0x826BABC4]`) | Depth to color | A second depth map (use not known) | The depth image sampled | Not known |
| 7 | `RecordEnd` type 3, flags `0x300` | 8_8_8_8, pitch 520, 4x MSAA + depth | Format 54 (2_10_10_10_AS_16_16_16_16) 512x128 (`[0x826BABE8]`) | MSAA average, format change, swap, clears after | The rear-view mirror, in the HUD pass (some views only) | Drawn into the texture's format; MSAA per the option | The mirror's camera (mirrored, blended) |
| 8 | `RecordEnd` types 4-9 (mode 1: cube faces 1-5) | 2_10_10_10_FLOAT, pitch 280, 4x MSAA | Format 54 256x256 (`1EC39000`) | MSAA average, swap | The paint's reflection map (the paint shaders declare a cube at fetch 6 and get zeros now: `native-renderer.md`, "Textures of another dimension") | As 7. Whether the faces are meant as a cube map the paint should sample is worth a look later | Reflection cameras |
| 9 | `RecordEnd` type 19, flags `0x300` | 8_8_8_8, pitch 280, 4x MSAA | 8_8_8_8 256x256 | MSAA average | The minimap's road outline, in the HUD pass | As 7 | The minimap's camera |
| 10 | The motion blur's chunk (`sub_821363F0`'s; probably through `RecordResolve`) | 16_16, pitch 560, + depth, cleared to `0x80808000` | 16_16 512x288 (`1B053000`) | Copy | The motion blur's combine (`7EB16456AAD56CBA`) | The pass draws into the texture | **c100**, the previous view-projection (`--motion_blur_by_tick`) |
| 11-12 | `PostEffect::RecordResolve`, flags `0xF4000000`, rect 1024x576, twice (the glow's and the color filter's, probably) | **Main view**: 7e3 HDR, pitch 1040, 2x MSAA | 16_16_16_16 1024x576, twice | MSAA average, exponent bias −3; the second after ~350 more draws | The glow's bright pass, the color filter, and (the second) transparent draws, probably | MSAA resolve per the option; the first copy as the scene texture, the second a grab-pass copy; the bias as above (open) | — |
| 13-23 | `Glow::Record`, `RecordStar`, `RecordStarPass` (flags `0xF4000100`) | 2_10_10_10_FLOAT, pitch 320, one target for every step | Six 16_16_16_16 256x144 textures, plus the glow history (`*(+8408)`) | Exponent bias −3; clear after | The bloom chain, the star streaks, the history cross-fade (`E0D070FB58A4EA03`), the combine (`9750599DF5ED3B39`) | Each step into its own texture; the bias as above | **The glow history** (`glow.cpp`): forced on render-only passes, its weight scaled. It must stay a per-frame pass, in order |
| 24 | `sub_8209B3D0` (probably; not read) | The main view | Format 54 1280x720 (`1ECB9000`), rect 1024x576 at 0,0 | MSAA average, partial rect | The game's upscale draw; the menus resolve their own frame to the same address | The frame composed natively, or the 3D view drawn into that region | None |
| 25 | `sub_8258D998` → `Device::Resolve`, outside the list | 8_8_8_8, pitch 1280, no MSAA, EDRAM base 0 | The front buffer, 1280x720 | Scan-out copy | The window | Already present from the target | None |
| menus | EndTiling (command 33), one per tile | 1280x720, 4x MSAA, 32-bit + depth (one full target already) | The menu's 3D view texture | MSAA average per tile | The Free Run car, the Course In picture | MSAA resolve per the option, once | No render-only passes in menus |

## 3. The other 360-style machinery

With native handlers calling our interface (section 5), most of this
goes at once, as nothing on the render path calls the game's Direct3D
any more.

| Machinery | Who still writes it | Who still reads it | What replaces it | What can't go yet |
|---|---|---|---|---|
| **The register shadow and dirty masks** (device +0 to +12440) | The native setters; the recompiled shader load (SQ_PROGRAM_CNTL at +10400, and +10408, +10432, +10452, +12440) | `ReadRegisters` per draw (`state.cpp:139`), through `MarkDeviceDirty` (`draw_packets.cpp:221`) and the wrapped flush helpers (`:135-160`), then `State::Draw` throughout; the game's getters (one seen in play, `device-state.md`); interpolation's blends land there | The interface's state, set by the native handlers | The SDK translator takes registers (SQ_PROGRAM_CNTL, SQ_CONTEXT_MISC, fetch constants, system constants from PA and RB registers). The Vulkan backend makes the few it needs from its own state and the shader pair, inside the backend. Whether game code outside the replay reads the shadow: to log (section 7) |
| **The PM4 command buffer**, packets and kick | The shader load (IM_LOAD), BeginVertices (DRAW_INDX, fetch constant 95, the inline vertices' space), Resolve (RB_COPY_*), Clear (not read), tiling (BeginTiling, EndTiling, the worker callback insert `sub_822B6BF8`), DrawVertices, `sub_822AE1D0` on the library path, Swap's fences | `gpu_link.cpp`'s `ApplyBuffer`, for fences, memory writes and the tiling worker's interrupt; no draw is taken from it | Nothing: off the render path with the handlers | Resource locks and destruction outside the replay wait on fences (`graphics-resources.md`, Dropped-kind): with nothing writing packets, every fence is passed at once |
| **The single commands** (table `0x8265ED48`) | Main thread recorders | Native `RunSingles` (`render_queue.cpp:262`), recompiled handlers (15 wrapped: the minimap triangle) | Native handlers calling the interface | — |
| **The guest memory mirrors** (gone in step 2b: a device-local buffer arena, `native/buffer_arena.cpp`; were 512 MB host-visible buffers, one per frame in flight; front and back with `mirror_by_tick`; `passes.cpp:675`, `:1503`) | memcpy (`sub_825A7410`, `buffers.cpp:718`) and `Buffers_Memcpy` record writes; the render thread applies them at frame start | The GPU: the translated shaders fetch vertices by physical address from one storage buffer; the index buffer binds from it | Buffers made through the interface when the game creates and fills them (`Mesh::Parse`'s and the loaders' native writers, or memcpy's records), bound per stream | Fetch by address from one buffer is the SDK translator's model. The Vulkan backend can keep it over an arena of its own (addresses rebased) until vertex input is per-stream. The offline translation (decision 3) could emit per-stream vertex input |
| **The constant replay** (command 7, `sub_8258FD48`) | The recorded payloads; interpolation substitutes blends into them and puts them back (`blend_state.cpp:4100-4115`) | The native constant setters copy into the shadow; `State::Draw` copies each shader's used registers into the ring (`passes.cpp:3795-3830`) | A native command 7: substitution, then the interface's `SetConstants` | Interpolation's join is the payload address plus an exact value (`Substitute`, `blend_state.cpp:2656`); the substitution must stay at that point |
| **The vertex shader patch for the declaration** (`sub_822AE340`, the copy in the command buffer when "the GPU may still use it") | The game's Direct3D at the shader load | The renderer (`g_loaded_vertex_shader`, `replay_hooks.cpp:50-70`) | The backend translates each (vertex shader, declaration) pair itself: the declaration is part of the shader's key | Re-implementing the patch's result in the translation step (or offline): read the patcher first |
| **Textures reloaded on memcpy writes** (gone in step 2: was `MarkWritten`, views keyed by the fetch constant's six words) | — | — | Textures made, written and freed through the interface (`render/resources.h`), found by handle (step 2) | Uploaded from guest memory at first bind still; the data leaving guest memory is 2b |
| **Shader microcode translation** (the SDK's SPIR-V translator, `shaders.cpp:272`; disk cache `cache/`; lookups by the objects' fields) | Game data; the 14 stored boot shaders (`compiled_shaders.h`) | Our renderer, per pipeline | Vulkan: the runtime translator stays, inside the backend. Other backends: offline blobs of the game's shader and declaration pairs (decision 3) | — |
| **`replay_hooks.cpp`, `draw_packets.cpp`, `gpu_link.cpp`, `refcount.cpp`'s render-thread skip** | — | — | Native handlers | Go when the handlers are native and no longer call the game's Direct3D |

### GPU data out of guest memory (added 2026-11-01, the user's call)

The 360's CPU and GPU share one memory: the game writes GPU data into
physical memory and the GPU reads it in place. We copy it from guest
memory into Vulkan buffers and images: the mirrors for vertices and
indices, uploads for textures. The aim is for game code that writes GPU
data (native code, as B7 makes it so) to hand it to the renderer
directly (`UpdateBuffer`, `UpdateTexture`), into device-local memory,
without writing guest memory first.

**Where the time goes now.** Since `mirrors-fix`, draws copy nothing:
only memcpy's new writes (~0.1 MB a frame in a race) reach the mirrors,
at frame start. The mirrors, though, are host-visible and coherent with
no device-local bit (`passes.cpp:1336`). On a discrete GPU that's
probably system memory (unless the driver gives resizable BAR memory),
so every draw's vertex and index reads would cross PCIe. So the expected
gain is GPU time (most where the GPU is the limit, at 4K) and memory
(two or three 512 MB mirrors), more than render thread time. To be
measured: a GPU-bound run (4K) before and after, plus the render thread
profile.

**The game's writers of GPU data into guest memory** (from
`native-renderer.md`, "Buffers", `texture-handling-optimization.md` and
the B7 files; whether each is native is as of `b287c74c`):

| Writer | What | Native? | Into the renderer directly? |
|---|---|---|---|
| `Mesh::Parse` (`sub_825963F0`, `models.md`) and `sub_82596F68` | Vertex and index data, 1-3 MB copies from read buffers (~90% of the ranges); the headers | Native (B7) | Yes, once no CPU reader needs the data (below) |
| `sub_820E7598`, `sub_820E7CA0`, `sub_820E8028` | 2-4 ranges at a time | Not looked at | When native |
| XPR loading (`XPR::GetTexture`, the archive, the file layer) | Texture data | Native | Yes |
| The license plate (`CarModel::MakePlate`), the vinyl sheet and decals (`Vinyl::Compress`, `XGCompressSurface`), anything through `Resource::Lock` and `Unlock` | Textures the game makes and writes itself | Native | Yes: their native code calls `UpdateTexture` at the unlock or the write's end |
| Per-frame vertices built on the CPU: the light billboards (`sub_8209C6E0`), the world-space sprites' corners, the post effects' quads, every inline draw (commands 5, 19, 27) | Vertices in the command list's payload or in game objects | Mostly native | Into a per-frame upload ring, from the payload or the object, as now. The payload stays in guest memory: interpolation substitutes there |
| Shader microcode (`Model::Load`, the shader objects) | Read once, for translation | Native | No need: once per shader |

**What limits it:**

- **Recompiled writers keep writing guest memory.** memcpy's tracking
  stays as the fallback for any that are left.
- **The CPU may read the same data.** If game code reads a mesh's
  vertices or indices (collision, the minimap, culling, the course
  route), that data has to stay in guest memory as well, and only the
  GPU's copy moves. Not known yet. To check before anything moves: a
  read watch on resource data ranges after their first use by a draw, in
  a race and a drive. There's a precedent: a `--watch_input_reads`
  watch, tried and stashed on 2026-10-19.
- **The resource headers stay in guest memory** (fetch constants, sizes,
  formats) while recompiled code reads them. Only the data behind them
  moves.
- **The command list's payload stays** (interpolation's substitution)
  until tick replay.
- **Guest memory's future was left "to decide at the end"** (the user's
  call, B7 step 1: one native heap over guest memory, shared with the
  game). GPU data is the exception, decided now (decision 9): the first
  data deliberately kept out of it.

## 4. Frame interpolation's dependencies

Read in `src/interpolation/blend_state.cpp`, `glow.cpp`,
`src/frame_pacing/` and `src/hooks/cockpit_dials.cpp`; the doc is
`frame-interpolation.md`.

**Where it works.** A render-only pass reruns the game's Draw from the
same tick and records a new command list. On the main thread, every
camera product (world times view-projection) is noted as it's
multiplied or copied, and the products are published after Draw, keyed
by **command list payload address**. On the render thread, the command
7 wrapper writes the blend into the payload, calls the game's upload
(native, into the shadow) and puts the payload back. Everything else
interpolation does patches game state around game calls on the main
thread. **None of it reads the renderer's internals.** The renderer
branches on logic versus render-only lists only for the mirrors' swap
(`passes.cpp:1448`, `:1533`; `renderer.cpp:1283-1306`).

| Dependency | Keyed on | Touched by this plan? | How it could be keyed once the structure it relies on goes |
|---|---|---|---|
| Products noted while recording | The operands' guest addresses (often stack temporaries), the camera number, the handler, the recorder's caller and r31, the occurrence in the pass (`ProductKey`, `:1002`). Native recorders fake the original return addresses and stack frames to keep these stable | No (main thread, game side) | Real ids from native recorders (object, part, wheel, camera pass), which would also replace the matching by position. A B7 matter |
| **The join between recording and replay** | Payload address plus an exact value (`ProductTable`, `:441`; `Substitute`, `:2656`) | **Yes**: command 7 becomes native. Its substitution stays where it is and runs before `SetConstants` | If constants ever travel outside the command list (tick replay), the product's identity has to travel with the draw |
| Camera numbers (`CameraIds::Of`, `--cache_camera_ids`, `--sticky_camera_ids`, `--camera_ids_ignore_depth_offset`) | The projection's signature in 1% buckets; sticky by camera address | No | A camera pass index from the native camera code |
| `--pair_by_object`, wheels | Caller and r31, then position within a group | No | Object and part ids |
| The per-object extras (c4-c8) | The same upload as the product, by value | No (still in command 7) | From the native recorder's arguments |
| **Per-object pixel constants** (lighting, 63 registers) | **The last camera product replayed before them**: render thread order | **Kept**: the native replay runs commands in the same order. The backend may batch or reorder its Vulkan recording later, since values are captured at each call | Tie each lighting upload to its draw's object explicitly |
| The motion blur's previous view-projection (c100, the copy at `0x8213653C`) | The copy's return address and the view (`BlendState_MotionBlurNotedSource`, `:3949`) | No | A native call per view, blended as a camera |
| The light billboards' facing (`--cache_billboard_cameras`) | The camera pass address; the blended camera written in around `sub_8209C6E0` | No | The frame's camera passed to the native builder |
| World-space sprites, race overlays, the rival logo, UI movies and their tracks, the per-tick counters, the cockpit dials' live pose | Game objects, patched or replayed around game calls on the main thread; they assume Draw runs every frame | No | Unchanged while Draw reruns every frame |
| **The glow history** (`glow.cpp`) | The post effect object; its 256x144 history resolve forced every frame, weight scaled | **Yes**: the resolve becomes a pass into the history texture; it must still run every frame, in the same place | — |
| Logic or render-only list tag (`Renderer_OnListHandedOver`) | A queue of flags, one per list | Kept: the interface's `BeginFrame` takes the tag and the amount | — |

**What every step must keep:**

- command 7's substitution before the values leave the payload;
- the replay order;
- the glow history pass every frame;
- one list per frame, in lockstep.

The checks are `--log_blend_state` (unpaired or rejected counts in
steady driving) and `--log_stepping_constants` in an attract race, then
a drive by the user. These matter because a broken join fails silently:
the game just steps at 30 Hz, with no error.

## 5. The target design

### The seam: native command handlers

The command list's format stays (it's the game's, and interpolation's
join is its payload addresses). Its handlers become native and call our
interface:

- SetRenderState and SetSamplerState map the game's states to the
  interface's state blocks. The mapping today goes through the setters'
  registers; it becomes direct. The states used in play are listed in
  `gpu-functions.md`, "Render states and sampler states".
- SetTexture, SetStreamSource, SetIndices, SetVertexShader,
  SetPixelShader and SetVertexDeclaration map the game's object pointers
  to our handles (made when the objects were created, below).
- Command 7 does interpolation's substitution, then `SetConstants`.
- The draws (17, 18, 19, 27, the helper's 15 and 16) become `Draw`,
  `DrawIndexed` and `DrawInline`. Inline vertices come straight from the
  payload; BeginVertices's space in the command buffer goes.
- SetRenderTarget, SetDepthStencilSurface, 29, 34 and 35 bind surfaces.
  The saved set lives in our handler state, not device fields.
- Resolve (25) and EndTiling (33) become the renderer's own ops (below).
  Clear (26) becomes `Clear`.
- Commands 20, 22 and 28 call the game objects they name. Their
  callbacks may reach the game's Direct3D; to be read as part of the
  mapping.

The game's Direct3D still exists for code outside the replay (resource
creation, locks, the video output's set-up). Where that code makes or
fills a resource, its native version also tells our renderer (below).
The rest is left to B7.

### Knowing a pass's destination before drawing it

The list is fully recorded before the render thread replays it, and the
replay is native. When a render target is set, the replay can scan
forward in the chunk to the pass's resolve and learn the destination
texture, rect and flags. The pass then draws straight into that texture
(or, with MSAA on, into an MSAA image resolved into it at the end).
Nothing the game records changes.

The cases where it can't (a resolve in the middle of a pass, a target
reused after its resolve, a rect that isn't the whole target) are the
grab-pass copies, the ping-pong and the partial rect in section 2. Step
0's log says which those are.

### The interface

A header of plain C++ with no Vulkan types (call it
`src/render/device.h`), at the game's level of drawing:

- **Resources**, made by native code when the game makes them:
  - `CreateTexture(desc)`, where `desc` is format (an interface enum),
    size, levels, type, and whether it's drawn into;
  - `UpdateTexture(handle, level, data)`, given untiled, byte-swapped
    data (untiling stays above the interface);
  - `CreateBuffer` and `UpdateBuffer`;
  - `CreateShader(microcode or blob, stage)`;
  - `CreateVertexDeclaration(elements)`.

  Handles are ours, mapped from the guest headers.
- **State**, held by the interface and read by the backend only at a
  draw:
  - render targets 0-3 and depth (textures, plus the pass's clear);
  - viewport and scissor;
  - a blend state per target;
  - depth and stencil;
  - raster state (cull, fill, depth bias, the screen-space flag the
    helper's draws toggle);
  - textures and samplers per stage;
  - streams and indices;
  - the vertex declaration;
  - the shaders;
  - constants (vertex and pixel float c0-c255, bool, loop) as register
    files the backend reads by each shader's used ranges.
- **Operations**:
  - `BeginPass(targets, clears)` and `EndPass()`;
  - `Draw`, `DrawIndexed`, and `DrawInline(type, vertices)`;
  - `Clear`;
  - `CopyTexture(source target, rect, destination, point)` for grab
    passes;
  - `ResolveMultisample` (internal to `EndPass` when MSAA is on);
  - `Present(texture)`;
  - `DrawMovie`;
  - `BeginFrame(tick tag, amount)` and `EndFrame`.
- **Options**: MSAA (off, 2x, 4x, 8x; from `itc.toml`), the drawing
  scale, gamma. These are the backend's to honor; the game never sees
  them.

### Where Vulkan stops

Behind the interface live:

- pipelines and their key (kept and patched on state changes, not
  rebuilt from registers each draw);
- descriptor sets;
- dynamic rendering, barriers and layouts;
- rings and staging;
- the swapchain;
- the pipeline cache;
- SPIR-V and the SDK translator, with the registers it needs made from
  the backend's own state.

Today's `Passes::State::Draw` becomes the Vulkan backend's `Draw`.

### Above the interface (shared by backends)

- the command handlers and the destination scan;
- texture untiling and format conversion;
- shader analysis (which constants and fetches a shader uses);
- the frame's tag for interpolation;
- the dumps' meaning (`draws.txt`).

### What a second backend would supply

- the texture, buffer and target objects;
- the draws and clears;
- MSAA resolves;
- copies;
- present;
- its shaders, from the offline blobs (decision 3).

These are constraints to keep the interface from assuming Vulkan. They
come from the API's known limits and are not checked against D3D11:

- **Vertex fetch from one big storage buffer by address** doesn't map to
  feature level 10_0 (no UAVs or structured buffers in the vertex stage;
  typed buffer loads probably do). The interface takes streams and a
  declaration, never addresses.
- **No sampling of MSAA depth at 10_0**: the depth maps here aren't MSAA,
  so probably fine.
- **Shaders**: the SDK's DXBC translator targets Shader Model 5.1 for
  D3D12 (probably; not read), not SM 4.0, which is another reason for the
  offline blobs. The set is finite: 76 shaders through the menus and an
  attract race in milestone 2, all in the disk cache since.
- **Passes, not dynamic rendering**: `BeginPass` and `EndPass` map to
  render passes, framebuffers or OM bindings alike.

## 6. The plan

Each step ships on its own behind a flag (on by default once checked).
The checks:

- **Frame dumps** (`--native_renderer_dump_every`, or
  `--native_renderer_dump`) against the step before. `draws.txt` should
  be identical once dynamic addresses are masked, and pixels identical,
  or different only for a reason the step explains:
  - the precision of a pass drawn in its destination's format;
  - the MSAA option at a setting other than the game's.

  The attract race's random filter and frame timing make whole-frame
  comparisons noisy ("How to measure" in `native-renderer.md`), so
  compare resolve destinations and passes by draw IDs. Where a dump can't
  show a step's effect, use the doc's own method: a temporary check that
  computes both ways on every draw and logs the mismatches over a race.
- **No harnesses** (decision 10): render-path code isn't compared with
  the recompiled code; the dumps and the drives judge it.
- **Render thread profiles: not until the conventional renderer's work is
  all done** (the user's call, 2026-11-01). Steps are checked by dumps,
  logs and drives only; profiling (`profile_render_thread.sh`, two or
  three runs a build, alternating, against step 0's baseline) comes once
  at the end. Steps 1 and 2 were profiled before this call.
- **Interpolation**: `--log_blend_state` and `--log_stepping_constants`,
  then a drive by the user, for every step from 1 on.

| Step | What | Removes | Checked by | Size, risk |
|---|---|---|---|---|
| **0** | **Measure and log.** A render thread profile on the current code (a baseline: none since the setters, shaders and render queue went native). A pass log for one race frame and one menu frame. For each pass it records: the targets set; the draws; each resolve (rect, flags, destination); whether the source is drawn into again before the next clear; and whether each destination is sampled before it's resolved into, within the frame. Also whether game code outside the replay reads the register shadow (a watch on its blocks for a race) | Nothing; it settles the open rows of section 2 | The log; the profile | Small. **Needs the user to run it** (or the tree free of the B7 loop) |
| **1** | **Our command handlers, feeding our interface, the game's Direct3D off the render path** (steps 1-3 merged, decision 10). The interface defined (`src/render/device.h`, no Vulkan types). Native handlers decode each command (`functions/render-commands.md`) into it: state blocks, textures, streams, shaders and declarations, constants (command 7 keeps interpolation's substitution where it is), draws, targets, viewport and scissor, clears, resolves and tiling as the renderer's own operations. The primitive drawer's draw (`PrimitiveDrawer::Draw`, under command 15) feeds it too. The Vulkan renderer reads the interface instead of the registers, and makes the translator's registers and the (vertex shader, declaration) pairs itself. Resolves are still made as copies, as now. The old path stays behind a flag until this is checked | The register shadow's writes and reads on the render path, `ReadRegisters`, the dirty masks, the PM4 packets, the kick, the shader load and patch, the recompiled handlers, `replay_hooks.cpp`, `draw_packets.cpp`, `gpu_link.cpp` | Dumps against the current renderer (`draws.txt` and pixels; differences explained); the menus and tiling by the user; `--log_blend_state`, `--log_stepping_constants`; a drive; a profile against step 0's | Large. After it, the game's Direct3D is off the render path. Its parts can land one after another behind the flag |
| **2** | **Textures through the interface** (buffers moved to 2b, the user's call, 2026-11-01; design in "Step 2" below). Textures created, written and destroyed through the interface where the game makes, fills and frees them (the five creation paths, the four free points, the plate, the vinyl sheet), bound by handle; still uploaded at first bind (the user's call) | The fetch-constant-keyed texture lookups, memcpy's marking of written textures, the texture lifetimes' guesses (`RetireOverwritten`, `RetireMovedResolveDestinations`, the race-end flush) | Bound textures never created logged; `--native_renderer_verify_textures` over a drive (no unannounced changes); dumps against two runs' spread; the hitch log; `--log_gpu_memory` over races (no growth) | Medium |
| **2b** | **Buffers through the interface, and GPU data out of guest memory** (done 2026-11-01, as "Step 2b" below: buffers only, the mirrors replaced by a device-local arena, native writers still writing guest memory; the rest of this row is the plan as first written) ("GPU data out of guest memory" in section 3). Buffers' records first (created by `Mesh::Parse`'s relocations, `CreateVertexBuffer` and `CreateIndexBuffer`, the work memory's headers; destroyed where freed), the backend fetching by address over its own arena until vertex input is per-stream. First the read watch: which resource data the CPU reads after a draw first uses it. Then each native writer whose data the CPU doesn't read hands it to `UpdateBuffer` or `UpdateTexture` into device-local memory and leaves guest memory unwritten (headers kept); data the CPU reads is written to both; memcpy's tracking stays for recompiled writers. Buffers and textures device-local; per-frame vertices through an upload ring | The mirrors (for native writers' data), most of the guest memory physical allocations for GPU data, vertex reads across PCIe | The read watch over a race and a drive (0 reads of moved data); dumps identical; a GPU-bound run (4K) and the render thread profile, before and after | Medium; risk in data the CPU reads that the watch didn't see |
| **3** | **Passes into their textures.** The destination scan (section 5): each pass draws into its destination; depth maps sampled as depth; the swap and the format changes gone; the glow's steps into their own textures; grab passes as `CopyTexture`; tiling as one target. MSAA keeps the game's sample counts for now (resolved at the pass's end) | The resolve copies but the grab passes, `ResolveInto`'s conversions, the resolve destinations' separate images | Dumps: each destination against the step before; differences explained (precision) | Medium. The exponent bias (section 2) is decided here |
| **4** | **MSAA as a player option** (`itc.toml`; off, 2x, 4x, 8x): one value for every target the game drew with MSAA (decision 4; the default to pick) | The 360's fixed sample counts | Dumps at each setting; the user | Small |
| **5** | **The frame composed natively** (`unlocked-res.md`, "Later"): first read the upscale's pixel shader and check that nothing else reads `1ECB9000`; then the 3D view drawn into the frame (or its region) and the HUD over it | The partial-rect resolve and the upscale draw | Dumps; the user at 4K | Small to medium |
| **6** | **The Vulkan backend behind the interface**: the Vulkan code moved under `src/render/vulkan/`, the interface free of Vulkan types; the shader blobs' format defined for other backends | Nothing at runtime | Builds and dumps identical | Medium, mechanical |
| later | **Tick replay** (`tick-replay.md`): with draws in the interface's terms, a tick's draws kept and replayed on render-only frames | The per-draw work on ~14 of 15 frames | Its own diagnostic first | Large; needs interpolation's join keyed by draw |

Step 1 is where most of the 360 machinery leaves the render path. Step 3 is where the resolves go. The mirrors go in 2b.

The steps were numbered 0, 1, 4, 4b, 5, 6, 7, 8 until step 1 was done
(old steps 1-3 merged into it); renumbered 2026-11-01, so commit
messages before then name old 4-8 for today's 2-6.

## Step 0's findings (2026-11-01, on `conventional-renderer`)

The diagnostics (all off by default):

- `--native_renderer_log_passes` with `--native_renderer_dump`: the
  dumped frame's `passes.txt` beside `draws.txt` (same draw numbers):
  - its draws grouped into passes by the targets bound, with the clears
    (and why) and the resolves in order;
  - per resolve: the source's draws before its next clear, and the
    passes sampling the destination after it and before it;
  - the targets' EDRAM ranges and what first touched each
    (`Passes::State::DescribePasses`).
- `--watch_register_shadow`: the device's +0 to +12440 protected outside
  the render thread's replay. The main thread waits for each replay after
  `Renderer::SignalTick` (`sub_82091A58`, the list's hand-over, wrapped
  for it), so every access outside the replay faults and is noted
  (`src/debug/render_watch.cpp`). Other guest threads running during the
  replay aren't seen.
- `--watch_gpu_data_reads`: the vertex, index and texture data draws used
  (from `State::Draw` and `TextureCache::Upload`), protected in the
  guest's three virtual views of physical memory (`0xA0000000`,
  `0xC0000000`, `0xE0000000`). The renderer reads the physical view, so it
  isn't seen. A write, or a file read into the data (`fread`, hooked), stops
  watching a page until a draw uses it again. A self-test reads the first
  page armed through each view and logs whether each read was noted.
- `tools/resolve_watch_reads.py <build dir> <sdk lib dir> <log>` names the
  watches' instructions (addr2line; guest addresses for `generated/`).

### The passes of a race frame

Attract race, a logic frame and two render-only frames (`itc_3230` and
the runs after it; 2,430 draws in the logic frame). Each has 11 passes,
15 targets, and 25 resolves inside the list, plus the front buffer. The
render-only frames have the same passes and resolves. The numbers are
`passes.txt`'s, which differ from section 2's older trace:

| # | Source | Destination | Read by (this frame) | Notes |
|---|---|---|---|---|
| 0-2 | 8_8_8_8, pitch 560 | `1E73D000` 512x512 (format 6), three times | Pass 1 between them (the filter in place); the main view after the last | The source is drawn into again after 0 and 1: a ping-pong in one target |
| 3 | 8_8_8_8, pitch 1040 (3 draws, not only cleared) | `1E33D000` 1024x1024 | The main view, by 436 draws | Section 2's resolve 4: its reader is the main view |
| 4 | Depth, pitch 1040, flags `0x204` | `1E83D000` 1024x1024 (format 23) | The main view, by 436 draws | Cleared after |
| 5 | The same depth target (0 draws since 4's clear in the logic frame, 4 in render-only frames) | `1DBFD000` 1024x1024 | The main view, by 50 draws | Section 2's resolve 6 |
| 6 | 2_10_10_10_FLOAT, pitch 280, 4x MSAA, flags `0x300` | `1EC3D000` 256x256 (format 54) | The main view, by 102 draws | One reflection face a frame |
| 7 | 8_8_8_8, pitch 280, 4x MSAA, flags `0x300` | `1DFFD000` 256x256 | **None in the frame** | The minimap's (section 2's 9)? The attract race may not show it |
| 8 | 16_16, pitch 560 | `1B071000` 512x288 | The main view | The motion blur's |
| 9 | Main view (7e3, 2x MSAA), flags `F4000000`, rect 1024x576 | `1B44E000` 1024x576 | The main view's next 266 draws | **The grab pass** |
| 10 | The same, after 264 more draws | `1B44E000` again | The glow (pass 8) and pass 9 | |
| 11 | 2_10_10_10_FLOAT, pitch 320, EDRAM base 936 | `1B101000` 256x144 (`1B1AE000` in another run) | Pass 8, **before** it: the last frame's | **The glow history**: the only destination in the frame read before it's written |
| 12-19 | The same target, cleared after each | Four 256x144 textures, each twice | Pass 8 | The bloom chain's steps |
| 20-22 | The same, no clear between | `1B206000` three times | Pass 8, then pass 9 | A ping-pong in one target |
| 23 | Main view, flags 0 (no exponent bias), rect 1024x576 | `1B44E000` | Pass 9 | After the glow's combine |
| 24 | Main view, rect 1024x576 | `1ECBD000` 1280x720 (format 54) | Pass 10 (the upscale) | The partial rect |
| front | 8_8_8_8, pitch 1280 | — | The window | |

**EDRAM**: every color target at EDRAM base 0 is cleared (command 26)
before its first draw in the frame. Three targets start with a draw
instead: the glow's (base 936, overlapping the main view's depth), and
the frame's color and depth (pitch 1280). Each first draw neither blends
(`RB_BLENDCONTROL` `00010001`: one and zero) nor tests depth
(`RB_DEPTHCONTROL` `00700760`). So in this frame no pass reads what
another target left in EDRAM, and drawing each pass into its own texture
(step 3) keeps the same pixels. Ranges overlapping by EDRAM base only
mean shared EDRAM, not shared data.

**Section 5's cases**:
- copies in the middle of a pass: 9 (the grab pass) and 23;
- reused targets after their resolve: 0-2 and 20-22 (ping-pongs);
- the partial rect: 24;
- read before written: only the glow history (11).

### The passes of a menu frame (the user's drive, `itc_3240`)

A front-end frame with the 3D car (tiling), dumped at the first tiling: 3
passes, 377 draws, 12 targets, 15 resolves:

- resolves 0-11: the race scene's side targets (the projected texture,
  the 1024x1024 copy, both depth maps, the reflection, the 256x256
  copy), each cleared twice and resolved twice **with no draws**, and
  read by no pass. In a PC renderer these are nothing (or one clear of
  the texture, if a later frame reads it);
- pass 1: the 3D view, 1280x720, 4x MSAA, 2_10_10_10_FLOAT, 245 draws,
  cleared by BeginTiling and command 26, then resolved in three tiles
  (rects of 1280x256, flags `0x303`) into `1ECBD000` (1280x720, format
  54), the address the race frame's upscale reads;
- pass 2: the frame (pitch 1280), 118 draws, sampling `1ECBD000`, then
  the front buffer.

Every target is cleared before its first draw except the frame's (as in
the race). No grab pass, glow or partial rect in the menus.

### Register shadow readers (attract race, `itc_3233`)

Outside the replay, nothing reads or writes the register blocks, the
float, bool and loop constants or the fetch constants. Inside +0 to
+12440, only these were seen:
- `VideoOutput::LockDevice` and `UnlockDevice` write the device's thread
  word (+10760), in the front end;
- the library's worker thread (`sub_822B69A8`) reads its flag word
  (+10808, the bit at +10810), about 3 times a second;
- a kernel critical section in the device (+11356 to +11368) is entered
  and left from that thread.

None of these are render state. The user's drive (`itc_3240`, the menus
and a race) saw the same three and nothing else. So section 3's "whether
game code outside the replay reads the shadow" is answered no.

### CPU reads of GPU data (attract races, `itc_3234` to `itc_3237`)

The self-test passed on all three views. Over a 148 s attract session
(68,717 pages armed, across a race's end and the next load), no CPU code
read or wrote vertex, index or texture data after a draw had used it. The
only writes were 7 file reads into watched pages (the next load's data
over memory freed from the last), which dropped 34,604 pages.

In the user's drive (`itc_3240`, 40,604 pages armed), one reader of
GPU data: `texture_compression::DecodeA8R8G8B8Row`, on a guest thread
during the race (about 10,700 faults on texture pages and a few hundred
on pages also used for vertices or indices). That's `XGCompressSurface`
reading the vinyl sheet (A8R8G8B8, `Vinyl::Compress`, once per car
load). The sheet is drawn on the CPU (the decal path's rasterizer), so
probably its pages were used by a draw before, as the sheet or as other
data in memory reused for it; the page-level watch can't tell which. It
is a texture the game writes and reads itself (section 3's table): it
stays in guest memory, and the renderer gets a copy. One write by `libc`
(memcpy) to an index page, and 2 file reads, dropped pages as expected.
Nothing else read vertex, index or texture data after its first draw.

### The baseline profile (2026-11-01, `fa713794`'s build)

`profile_render_thread.sh`, uncapped attract race, three runs in a row
(profiles `itc_recomp/out/profiles/step0-baseline-{1,2,3}.data`):

| Run | Frames recorded | Render thread, Mcycles a frame | `BlendState_ReplayConstants` |
|---|---|---|---|
| 1 | 6,359 | 9.433 | 6.64%, 626 kcycles |
| 2 | 5,854 | 10.196 | 6.45%, 658 kcycles |
| 3 | 5,865 | 10.192 | 6.63%, 676 kcycles |

Steps from 1 on compare against these (alternating runs).

## Step 1 (2026-11-01, on `conventional-renderer`; done)

Built behind `--conventional_renderer`, checked (below), then made the
only path (`c65b3c3e`). The code:

- **The interface**, `src/render/device.h`: `render::State` (plain state
  with dirty bits; the game's objects' guest addresses as handles until
  step 2; enum values equal to the game's encodings) and
  `render::Backend` (`DrawIndexed`, `DrawInline`, `Clear`, `Resolve`,
  `BeginTiling`, `EndTiling`; each takes the state and clears the dirty
  bits it reads).
- **Our command handlers**, `src/render/commands.cpp`: every command and
  single command decoded into the state; render and sampler states
  dispatched by the setter's address in the image's tables (`0x826388F8`,
  `0x82638D78`); the game's defaults applied at the first replay. They
  keep the game's Direct3D's behavior where a frame shows it: render
  target 0 resets the viewport and scissor (through `0x82013600` and
  `0x82013618`); `SetViewport` clips; the blend render states set all
  four targets, command 3 one; command 34 AddRefs the saved surfaces;
  `[0x82970BC0]` points to the saved set; the drawer's commands 13-15 run
  `PrimitiveDrawer::Draw` natively. Command 7 goes through
  `BlendState_ReplayConstants` with our upload, so interpolation's
  substitution point is unchanged. Calls into game code that isn't
  Direct3D (`Release`, the retire list, callbacks, `AddRef`) are guest
  calls. Not drawn, logged once if met (none seen in play): commands 16,
  17 and 27 and some single commands.
- **The vertex shader's fetch patch**, `src/native/vertex_fetch_patch.cpp`
  (`sub_822AE340`'s port): a copy of the microcode patched per (shader,
  declaration, strides), with an id the renderer keys its translation on.
- **The translator's registers made from the state**,
  `src/native/interface_registers.cpp`, the way the game's setters made
  them: the texture fetch constants are the sampler's part merged with
  the texture's, as `SetTexture` does; stream `n` is vertex fetch
  constant `95 - n`; `SQ_PROGRAM_CNTL`, `SQ_CONTEXT_MISC` and the rest
  come from the shader headers, as `sub_822AED18` does; bits the game's
  device always has are set: `0x2205` bit 16 (`vtx_window_offset_enable`),
  `0x2204` bit 19 (`dx_clip_space_def`) and `0x2302` bit 2 (the round
  mode); the high-precision blend switches the
  format in `RB_COLOR_INFO` (2 and 10, 3 and 12).
- **The renderer reads the interface**, `src/native/passes.cpp`:
  `State::Bound` gives the bound objects from the interface (else the
  device's); `InterfaceDraw`, `InterfaceInlineDraw`, `InterfaceClear`,
  `InterfaceResolve` and `InterfaceBeginTiling` draw from it; the front
  buffer resolve and the movies take render target 0 from it. The
  backend in `src/native/renderer.cpp` forwards to these with the
  frame's command buffer.

The render queue (`src/graphics/render_queue.cpp`) calls only our
handlers (and `ReplayStats_OnCommand` and `Renderer_OnCommand`): the
recompiled handlers, the game's setters, the register shadow, the packets
and the shader load aren't called on the render path. Outside the command
list the frame's end (`sub_8258D998`) still calls the game's `Resolve`
into the front buffer; `src/native/replay_hooks.cpp` passes it on to the
renderer and logs `Resolve`'s callers once (the boot marker,
`scripted_run.sh`). `--native_render_queue=false` now draws nothing (the
recompiled replay's commands reach no renderer).

**The temporary check** (`--check_conventional_renderer`, with
`--check_vertex_fetch_patch`): our handlers run beside the game's on the
state alone, and at every old-path draw the bound objects, registers,
fetch constants of bound textures and streams, constants and patched
microcode are compared. An attract race: 5,830,965 draws, 40 differing,
all in `RB_DEPTH_INFO`'s EDRAM base (the renderer takes depth from the
depth surface's header, so harmless); the fetch patch 125,000 patches,
205 pairs, 0 differing. Left out as artifacts of the check, not
differences: the drawer's inline draws' `0x2204`/`0x2206` (our handler
puts the flag back before the game's draw runs), fetch constants 93-95
on inline draws, the device's texture and stream slots (the game's own
clears and resolves rebind them; the renderer reads the fetch constants)
and `RB_COLOR_MASK` but target 0's bits.

**Drawn from the interface** (attract races, `itc_3252` to `itc_3258`):

- every draw drawn: ~2,200 a frame, 0 unsupported, 0 failed; no unknown
  commands or drawer vtables logged; 0 error lines.
- Dumps with `--native_renderer_log_passes`, two runs each way:
  `passes.txt` the same passes, clears and resolves; `draws.txt` the same
  draws, shaders and states, but for two fields, both explained:
  - **The color mask**: `0000FFF1` against `00000001`. The game binds
    target 1 to target 0's surface; our register keeps slots 1-3's write
    masks (0xF, the default), the game's only target 0's bits. The
    renderer draws target 0 only and reads only its bits.
  - **The 2x MSAA pass's format label** (resolved into the 256x256
    format 54 texture): 3 against 12. The game's `HighPrecisionBlendEnable`
    and `SetRenderTarget` write the switched format into the bound
    surface's header (+28); ours don't (it's in our `RB_COLOR_INFO`), so
    the target's label, read from the header, is 3. Formats 3 and 12 are
    the same Vulkan format, the same extent (12 isn't 64bpp) and the same
    target key: the same image.
- Pixels: no run reproduces another's frame (the attract race's timing).
  Two old-path runs differ by 16.1% of the shown frame's pixels and two
  of ours by 15.5%; old run 2 against ours run 2 by 2.3%, the closest
  pair. Every pass's destination was within the spread of the same-path
  pairs; the frames and the reflection map look the same.
- Interpolation: `--log_blend_state` the same as the old path's (~342 to
  ~400 replayed camera products a draw, 0 mismatched, the same live
  poses), and `--log_stepping_constants` the same constants, interval by
  interval.
- **Render thread profile** (same build, flag off and on alternating,
  `step1-old-*` and `step1-conv-*` in `itc_recomp/out/profiles/`):

  | Run | Old path, Mcycles a frame | Ours, Mcycles a frame |
  |---|---|---|
  | 1 | 10.226 | 9.230 |
  | 2 | 10.586 | 9.352 |
  | 3 | 10.603 | 9.373 |

  -11% (step 0's baseline: 9.4-10.2). Ours' top symbols:
  `InterfaceRegisters::BuildTextureFetch` 13.1% (all 26 stages rebuilt
  on any texture change: a lead, per-stage dirty bits), `Passes::State::Draw`
  8.6%, `Commands::Run` 6.8%, `BlendState_ReplayConstants` 6.3%.

**The user's checks** (2026-11-01, `itc_3265`, flag on): the menus
(107-151 tilings per 5 s, all through our path), a race and interpolation
looked correct.

**The old path removed** (`c65b3c3e`): the recompiled handlers'
wrappers and the library's draw hooks (`replay_hooks.cpp`),
`draw_packets.cpp` (the indexed draw without its packets and
`--native_renderer_check_draw_writes`), `state.cpp` (`ReadRegisters`, the
dirty masks, `--native_renderer_log_register_reads`), the check
(`interface_check.cpp`, `--check_conventional_renderer`,
`--check_vertex_fetch_patch`), `Passes`' and `Renderer`'s entry points
for the device's state, the library's patched-copy and declaration
diagnostics, and interpolation's command 7 wrapper (its
`--log_frame_times` timing is in ours). The render queue harness's
`ChunkChain::Run` and `Replay` cases were dropped (they compared our
handlers with the recompiled ones; decision 10); its other cases pass.
`gpu_link.cpp` stays (Swap's fences outside the replay). An attract race
after (`itc_3267`): every draw drawn, `passes.txt` the same,
interpolation paired, pixels within the runs' spread.

## Step 2 (2026-11-01, on `conventional-renderer`; done)

Textures only (the user's calls, 2026-11-01): buffers go to 2b with the
mirrors, where their records are used; textures are still uploaded at
first bind, as now (no GPU memory for textures never drawn).

**Where the game makes and frees textures** (a survey of `src/`,
2026-11-01): there is no single place.

| Path | Where | Native? |
|---|---|---|
| `Device::CreateTexture` (`sub_822A5D08`): render targets (`cameras.cpp`'s `MakeRenderTarget`, `SceneTarget`), the video output's front buffers, `ScreenObject::Init` | `graphics/resources.cpp` | Yes (`ScreenObject::Init` recompiled, calling it) |
| `XGSetTextureHeader` (`sub_8235A920`) and `Ex` | `graphics/resources.cpp` | Yes |
| `Texture::Create` (`sub_820ECF08`): the vinyl sheet, the plates, the decals | `car_models/vinyl.cpp` | Yes |
| `RenderTexture::Create` (`sub_821330D0`): the post effects' pool (the glow chain, the color filter, the motion blur) | recompiled | No |
| XPR textures: headers relocated in place in the XPR's header block (`RelocateTexture`), data in the XPR's one physical block | `models/xpr.cpp` | Yes |

Freed at `Resource::Destroy` (the refcount path, render targets),
`Texture::Free` (through `DeferRelease`, two scene updates later),
`XPR::Release` and `Model::Release` (freed at `PendingFrees::FreeDue`)
and `RenderTexture::Release`. Written after creation: the plate
(`MakePlate`, memcpy rows), the vinyl sheet (the decal rasterizer's plain
stores, on a loader thread) and `Vinyl::Compress` (`XGCompressSurface`'s
byte stores, then the header given new data: a destroy and a create).

**The design:**

- **The interface gains** `TextureCreated`, `TextureWritten` and
  `TextureDestroyed`, by handle. The handle stays the header's guest
  address (what the command list binds; dumps stay comparable). The
  renderer keeps its own table from handle to image, filled only by
  these.
- **Callers**: the five creation paths (XPR textures at
  `RelocateTexture`, their data already copied; `RenderTexture::Create`
  through a thin wrapper), the four free points (where the memory is
  freed; images still destroyed after the frames in flight), the plate's
  and `Vinyl::Compress`'s ends, the vinyl sheet where it's drawn from.
- **Order**: main and loader threads queue the events; the render thread
  applies them before the next list it replays, so a list never binds a
  texture destroyed after it was recorded.
- **The renderer stops** looking textures up by the fetch constant's six
  words (by handle instead), marking textures written from memcpy's
  records, and guessing lifetimes (`RetireOverwritten`,
  `RetireMovedResolveDestinations`, `TextureCache_RaceLeft`'s flush).
  Resolve destinations are keyed by handle. The fetch constants are still
  built from the header (the translated shaders read them).
- **Checked by**: a bound texture never created is created there and
  logged once with its header (a path missed); `--native_renderer_verify_textures`
  over a drive (guest data changed with no `TextureWritten`: a writer
  missed); dumps against two runs' spread; the hitch log (texture time
  per 5 s, long frames); `--log_gpu_memory` over races (no growth without
  the race-end flush).
- **Interpolation** doesn't touch textures; the glow history's resolve is
  unchanged.

**What was built** (`f4753458`, `1eee2b06`, `a9897ae4`):

- **The interface**, `src/render/resources.h`: `TextureCreated`,
  `TextureWritten`, `TextureDestroyed` by handle, from any thread (a
  harness's calls are skipped), queued and tagged with the list handed
  over after them (`Renderer_OnListHandedOver`); the render thread hands
  them to the backend at the next replay's start (`Renderer_OnReplayStart`,
  and at the renderer's start). **Frees go one list later** than the rest:
  at a race's end two textures (`4_4_4_4`, `XGSetTextureHeaderEx`'s
  headers) were freed before the hand-over of a list that still drew them
  (`itc_3272`: "freed 1 frames before"). A texture made again while its
  free is held back is freed first. `render::Backend` has the three calls.
- **The callers** (all native): made at `Device::CreateTexture`,
  `XGSetTextureHeaderEx` (when it writes a header) and
  `TextureHeader::Relocate` (the XPR textures, and the second half of
  `Texture::Create` and `RenderTexture::Create`, so no wrapper of
  `sub_821330D0` was needed: it calls `XGSetTextureHeader` and
  `Resource::Relocate`, both native); freed at `Resource::Destroy`
  (type 3), `Texture::Free`, `RenderTexture::Release` and `XPR::Release`
  (each `TX` record of its header block, at the release rather than at
  `FreeDue`: lists recorded after the release don't draw them); written
  at `MakePlate`'s end, after each decal drawn into the vinyl sheet and
  its corner cleared (`OnDecalLoaded`), and after `XGCompressSurface` in
  `Vinyl::Compress` (whose new texture replaces the sheet: the old one is
  freed by `Texture::Free`). Model textures come through their XPR.
- **The texture cache by handle** (`src/native/textures.cpp`): a table
  from handle to its images: loaded from guest memory (one per mip range
  the sampler merges into word 4, nearly always one) and resolved into
  (one per scale; draws of that handle get the one written last). A stage
  with nothing bound keeps the last handle bound there, as
  `InterfaceRegisters` keeps its words (`StageTexture`). Each fetch
  constant's last view is kept by handle, words and generation. Images
  are retired when the handle is made again or freed, and destroyed
  after the frames in flight.
- **Removed**: the views' table by the fetch constant's words
  (`BindingTable` stays for samplers), `MarkWritten` and
  `Buffers_TakeNewWrites` with the marking's stats and time,
  `RetireOverwritten`, `RetireMovedResolveDestinations`,
  `TextureCache_RaceLeft` (and its call from `GameMessages_OnPassFinished`),
  `--log_gpu_memory`'s overlap counts, and the first-load memcpy coverage
  of `--native_renderer_log_texture_writes` (it now logs, with
  `--native_renderer_verify_textures`, which bytes of a texture changed
  with no `TextureWritten`).
- **Diagnostics kept or added**: a texture bound or resolved into that was
  never made is logged once with its header words and whether it was
  freed, and taken as made; a header changed with no `TextureCreated`, and
  a texture reading memory another handle's resolve destination stands
  for, are logged once (both drawn from guest memory); the textures line
  every 5 s counts made, made again, written, freed and held.

**Checked** (attract races):

- `itc_3270` (90 s, `--native_renderer_verify_textures`): no change of a
  texture's memory without `TextureWritten`; no header changed
  unannounced; no texture reading another's resolve destination.
- `itc_3273` (100 s, across a race's end, after frees were held back): 0
  bound never made. Held handles ~1,330-1,530 in a race; at the race's end
  1,302 freed, 320 held, then back to ~1,500: no growth without the old
  race-end flush (`--log_gpu_memory`: ~70 MB of loaded textures).
- Dumps (`--native_renderer_log_passes`), two runs of each build (part 2's
  `1eee2b06`, which draws as step 1, and `a9897ae4`): `passes.txt` and
  `draws.txt` the same once addresses are masked. Pixels: old run 2, new
  runs 1 and 2 are nearly the same frame (0.2-0.3% of the shown frame,
  every resolve destination 0-1%); old run 1 is the outlier of the four
  (40%, another glow chain address).
- Interpolation: `--log_blend_state` ~349 replayed camera products a
  draw, 0 mismatched, 240 live poses, the same as part 2's build;
  `--log_stepping_constants` the same constants.
- **Render thread profile** (`profile_render_thread.sh`, alternating,
  `step2-old-*` and `step2-new-*` in `itc_recomp/out/profiles/`):

  | Run | Part 2 (`1eee2b06`), Mcycles a frame | By handle (`a9897ae4`) |
  |---|---|---|
  | 1 | 9.579 | 9.813 |
  | 2 | 9.536 | 9.603 |
  | 3 | 9.479 | 9.681 |

  About +1.7%: a fetch constant's last answer missing now costs a lookup
  of the handle, its loaded map and its swizzled views
  (`GetSwizzledView` ~40 kcycles a frame, the handle table ~20). Left for
  later (the user's call, 2026-11-01: working first, optimized later); a
  one-entry memo per handle was tried in code but not measured.

**The user's drive** (2026-11-01, `itc_3276`; the plate and vinyl, the
glow and motion blur, a race change, the menus): "game looks fine". The
log: 0 error lines, none bound never made, no header changed unannounced,
no aliasing; loaded textures 23-83 MB, ~1,900-2,900 handles, no growth.

## Step 2b (2026-11-01, on `conventional-renderer`; done)

Buffers only (the user's call, 2026-11-01): texture data stays in guest
memory, uploaded at first bind as in step 2.

**What the survey found** (2026-11-01):

- **Draws read one storage buffer by address**: set 0 is the mirror, the
  translated shaders fetch at the fetch constant's address
  (`InterfaceRegisters` builds it from the stream header's +24,
  `interface_registers.cpp:317`), and indexed draws bind the same mirror
  at the index address. Past guest memory the mirror holds the built-in
  rectangle and quad index lists and the frame's inline vertices. With
  `mirror_by_tick`, two 512 MB host-visible mirrors.
- **Made**: `Device::CreateVertexBuffer` and `CreateIndexBuffer`;
  `Resource::Relocate` kinds 1, 2 and 8 (`Mesh::Parse`, its data copied
  before the relocation; the work memory below). **Freed**:
  `Resource::Destroy` types 1, 2, 7 and 8; `Model::Release` (the shapes'
  block and the vertex data at `model + 20`); the streamed track sections
  are freed and loaded again at the same header addresses.
- **The work memory makes buffers every frame**: `WorkMem::AllocVertexBuffer`
  (`sub_8258A9E8`, recompiled; `car-draw.md`) takes up to 1,280 headers
  of each of two buffers (`XGSetVertexBufferHeader`, then our
  `Relocate`), filled by `Morph::Blend` on a worker thread (a memcpy, then
  plain stores). The mirror gets the stores only because it copies the
  memcpy's range at the frame's start, after the job ran.
- **The CPU reads vertex data**: `Model::MorphShape` reads the base
  shapes' streams every frame, and the wheels' set-ups morph and write
  over shape data. The read watch (step 0) didn't see it: it arms pages
  only after a draw, and those base shapes are probably not drawn.

**The design, in three parts:**

1. **Buffers' records, nothing moved.** `BufferCreated`, `BufferWritten`
   and `BufferDestroyed` in `render/resources.h`, by the header's address,
   through the same queue as textures (frees one list later). Made at the
   two `Create*Buffer` and `Relocate` kinds 1, 2 and 8; freed at
   `Resource::Destroy` (1, 2, 7, 8) and `Model::Release` (each shape's
   headers, as `XPR::Release` does for textures); a section loaded again
   is the handle made again. The backend keeps handle to address and size;
   draws still read the mirrors. A buffer bound but never made is logged
   once with its header. Measured here: the bytes held in a race, and the
   work memory's bytes a tick (part 2's sizes).
2. **A device-local arena; the mirrors go.** One device-local buffer,
   space per handle (given back when freed or made again, reused after
   the frames in flight). `BufferCreated` and `BufferWritten` data is
   copied from guest memory into a staging ring at the next replay's
   start, then `vkCmdCopyBuffer` into the arena after a barrier (a write
   in place waits for earlier frames' reads: the queue orders it, so **no
   front and back mirrors**: `mirror_by_tick` and `mirror_to_both` go with
   the mirrors). `InterfaceRegisters` puts the handle's arena offset in
   fetch constant word 0 (the translated shaders unchanged); indexed draws
   bind the arena at the handle's offset. Inline vertices in a per-frame
   host-visible ring with its own set 0, swapped in for inline draws; the
   built-in index lists in a small static buffer. memcpy's records stay as
   the fallback for recompiled writers: a recorded write overlapping a
   handle's range is its `BufferWritten` (the work memory, `Morph::Blend`'s
   stores read at the replay's start, as the mirrors do now). Checked by:
   a temporary per-draw compare of the arena with guest memory; dumps
   against two runs of each build; `--log_blend_state`,
   `--log_stepping_constants`; `--log_gpu_memory` over races; a profile;
   a 4K run only if the user asks; a drive.
   **Why one buffer is enough** (the user's question, 2026-11-01): the
   mirrors went one per frame in flight (`fed38bb7`) because, with one
   shared, the next frame's CPU copies could overwrite data the GPU was
   still drawing the previous frame with. In the arena the CPU never writes
   memory the GPU draws from: it writes a staging slot (reused only after
   that frame's fence), and the GPU copies it into the arena at the frame's
   start after a barrier on earlier submissions' vertex and index reads.
   A list's draws see the writes made before its hand-over, as with
   `mirror_to_both` now.
3. **Native writers skip guest memory, narrowly** (dropped, the user's
   call, 2026-11-01: the copy into the arena is made on the render thread
   at a draw, while `Mesh::Parse` runs on a loader thread from a read
   buffer that's reused, so its data would still need a CPU copy of ours
   first: the same cost, and the risk of CPU readers not seen). `Mesh::Parse` hands its
   vertex data to `UpdateBuffer` from the read buffer, keeping the guest
   allocation (headers, addresses and frees unchanged), only for models
   whose shapes aren't morph or skin sources; anything else writes both.
   The gain is small (a memcpy at load); the risk is data the CPU reads.

**Part 1, built**: `render::BufferCreated` (with the data's address and
size taken at the call), `BufferWritten` and `BufferDestroyed`, through
the textures' queue; `graphics::BufferMade` and `BufferFreed` read the
header (`graphics/resources.cpp`), called from `CreateVertexBuffer`,
`CreateIndexBuffer`, `Resource::Relocate` (kinds 1, 2, 8),
`Resource::Destroy` and `Model::Release` (`models::MeshBuffersFreed`); the
backend's `BufferTable` (`native/buffer_table.cpp`) holds them, checks
each draw's bound streams and indices (a buffer never made logged once
and taken as made), and logs a line every 5 s. Nothing calls
`BufferWritten` yet (part 2: memcpy's records). Attract race `itc_3277`
(100 s, across a race's end): 0 bound never made, 0 freed never made, 0
made again while held; ~2,750-3,065 held, 32-37 MB of vertices and
2.5-3.3 MB of indices; at the race's end 3,045 freed and 0 held, then
~3,250 made for the next. The work memory made none (attract races draw
no morphed wheels): its size is for a drive. The user's drive
(`itc_3278`, ~95 s: the front end, a race): 0 error or warning lines, 0
bound never made, up to ~3,660 held (32 MB vertices, 2.9 MB indices); the
work memory made none there either. The wheels' morphs ran at their
set-up (`WheelPoints::SetUpTire` and the rest), writing over shape data
already made (memcpy through `Buffers_Memcpy`): part 2's memcpy records
make that a `BufferWritten`.

**Part 2, built** (`9079e07d`): `native/buffer_arena.cpp`, one
device-local buffer (128 MB for buffers, then the built-in index lists,
then 16 MB of inline vertices) with 64 MB of staging a frame and its own
copy command buffer, submitted before the texture uploads and the frame,
between two barriers (earlier submissions' reads before the copies; the
copies before this frame's index and shader reads). `BufferTable` gives
each buffer its space (freed space reused after the frames in flight) and
copies its data from guest memory at the first draw that uses it after it
was made or written; memcpy's records of writes into the physical views,
taken at each frame's start, mark the buffers they touch written. `Draw`
puts each used stream's arena offset (plus the stream's offset, the type
bits kept) into its vertex fetch constant's word 0 before the constants
are copied, and binds the arena as the index buffer at the indices'
offset; inline vertices are copied into the inline region. A draw whose
buffer has no space, or whose data doesn't fit this frame's staging, is
left out and counted. No measurement found a draw reading vertices past
its buffer's size (`itc_3279`, with the per-draw check), so separate
spaces are safe. The read watch doesn't watch buffers with the arena (the
fetch constants hold arena offsets).

Checked (attract races):

- `itc_3280` (100 s, across a race's end): 0 error lines; 0 draws left
  out; 38-41 MB of the arena used; 0 staging full; each buffer copied once
  at its first draw; no buffer marked written by memcpy (step 0's finding:
  nothing rewrites buffers there).
- Dumps (`--native_renderer_log_passes`), two runs each way
  (`itc_3281`-`itc_3284`): `passes.txt` and `draws.txt` the same once
  addresses are masked. Pixels of the shown frame: the arena's two runs
  0.6% apart, each 2.1-2.4% from the mirrors' second run; the mirrors'
  first run the outlier of the four (~40%, as in step 2). Every resolve
  destination within the same spread.
- Interpolation (`itc_3285`): ~316-454 replayed camera products a draw,
  0 mismatched in 39 of 40 intervals, 1 product in one (older runs show
  1-2 a run now and then: `itc_1754` to `itc_1775`); 240-300 live poses.
- The user: "the game looks fine". Profiles were left for later (the
  user's call, 2026-11-01).

**The mirrors removed** (`a2bac698`): the arena is the only path. Gone:
the mirrors and `--native_renderer_buffer_arena`, `_mirror_by_tick`,
`_mirror_to_both`, `_verify_mirror`, `_copy_all`, `_check_mirror`,
`_check_late_writes`, `_check_clobbers`, `_log_index_ranges`,
`_cache_index_ranges` and `_log_physical_writes` (its drawn ranges came
from the mirror), the index range cache and scans, `buffers.cpp`'s
segment map and coverage (memcpy's writes are now only taken for the
arena, `Buffers_TakeWrites`), `BufferTable::Bound` (`Use` logs a buffer
never made); ~1,775 lines. The read watch covers textures only. Attract
race `itc_3293` (60 s): 0 error lines, 0 draws left out, 0 bound never
made, ~39 MB used. Dump `itc_3294`: `passes.txt` and `draws.txt` the same
masked; the shown frame 4.0-5.1% from the arena's and the mirrors' runs
(within the runs' spread, the mirrors' two ~40% apart).

**The user's drive** (2026-11-01, `itc_3295`, ~2 minutes: the front end,
a race, back to the front end): 0 error or warning lines, none bound
never made, 0 draws left out, 0 staging full; ~9 MB of the arena used in
the front end, ~35 MB in the race, all 3,689 freed at its end. Nothing
was marked written by memcpy after its first draw (the wheels' morph at
set-up writes its shapes before they're drawn), so that path is still
unexercised in play.

## 7. Risks, and what's still unknown

- **Readers of the register shadow outside the replay.** Once the
  handlers stop calling the setters, nothing writes the shadow on the
  render path. The game's getters (one seen in play), the video output's
  set-up and the movie code (gone) may read it. Step 0's watch answers
  it. Probably little: the replay is where the state is set.
- **The command handlers' callbacks** (commands 20, 22, 28): read
  (`functions/render-commands.md`): 20 releases a resource, 22 retires
  objects onto the release list, 28 calls an object's slot 0 (not seen in
  play). None calls the draw functions itself; what 22's and 28's
  objects do in their slots isn't read.
- **The primitive drawer draws under command 15** (found mapping the
  handlers): `PrimitiveDrawer::Draw` (`sub_825A17A0`, recompiled) makes
  the screen-space quads and the minimap triangle through BeginVertices.
  Taking the game's Direct3D off the render path needs its draw feeding
  the interface too, not only the handlers.
- **The vertex declaration patch**: what the game's patcher changes in
  the microcode (fetch strides and formats, probably) has to be
  reproduced in our translation step. Read `sub_822AE340` first.
- **Render states and formats**: the interface's mapping from the
  game's states and Xenos formats must match what the registers gave.
  That includes the main view's format switching per draw by render
  states 308-320 (`gpu-functions.md`). Step 1's both-ways check
  (`--check_conventional_renderer`, removed) found none differing.
- **EDRAM aliasing.** Targets are keyed by EDRAM base, pitch, format and
  MSAA, and different keys don't share memory. Drawing is right that
  way, so the game probably doesn't read one surface's EDRAM through
  another. Drawing into destination textures (step 3) relies on it. To
  check: log surfaces sharing an EDRAM base with different keys within
  a frame (step 0).
- **The exponent bias** (section 2): how readers get the 1/8 scale
  without a copy is open.
- **Resolve 4** (the cleared 1024x1024 texture) and the second 1024x576
  copy: their readers aren't known (probably transparent objects for the
  copy).
- **The upscale draw's pixel shader**: not read.
- **Precision differences** when a pass draws in its destination's
  format (the mirror's 8-bit target into a 10/16-bit texture): visible
  only as explained pixel differences in dumps (accepted, decision 7).
- **CPU reads of GPU data** (step 2b): whether game code reads vertex,
  index or texture data after loading it isn't known. If it does and the
  watch misses a path (one a drive didn't take), moving that data out of
  guest memory would give the game zeros or stale bytes. Writing both
  copies for anything in doubt is the safe default.
- **Where the mirrors' memory lives**: host-visible without device-local,
  so probably system memory on a discrete GPU (not checked per driver);
  the size of step 2b's GPU-side gain depends on it.
- **D3D11 at 10_0**: everything said about it is from the API's known
  limits, not checked.
- **Speed**: not a goal, but the steps change the per-draw path. Step 0's
  baseline and a profile per step keep it seen.

Confidence: the frame's layers, the native and recompiled split, the
resolve's implementation, the targets' keying, the front buffer's path
and interpolation's join point were all read in the code. The resolves'
destinations and readers come from the docs' trace and dumps (one trace,
2026-10-13 and -14). The cost shares are from profiles 2-3 weeks old.

## 8. Questions still open

Answered 2026-11-01 (decisions 4, 7, 8 and 9 above). Left open:

1. **MSAA's default value** in `itc.toml` (off, 2x, 4x or 8x), to pick
   at step 4.
