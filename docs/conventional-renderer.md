# A conventional renderer (a plan, not started)

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
   game, instead of the sample counts the 360's EDRAM budget chose.
5. **Shadows and reflections once a tick** is interpolation's question,
   not the renderer's: left out of this plan.
6. **The work goes on its own branch** after the B7 loop's run; this
   doc is committed where the tree is.

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
  destinations are sampled before being written. Step 1 then ports the
  command handlers as they are (same Direct3D calls) to have them native.
  Step 2 makes them feed our interface beside the Direct3D path and
  compares the two.

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
| **The guest memory mirrors** (512 MB host-visible buffers, one per frame in flight; front and back with `mirror_by_tick`; `passes.cpp:675`, `:1503`) | memcpy (`sub_825A7410`, `buffers.cpp:718`) and `Buffers_Memcpy` record writes; the render thread applies them at frame start | The GPU: the translated shaders fetch vertices by physical address from one storage buffer; the index buffer binds from it | Buffers made through the interface when the game creates and fills them (`Mesh::Parse`'s and the loaders' native writers, or memcpy's records), bound per stream | Fetch by address from one buffer is the SDK translator's model. The Vulkan backend can keep it over an arena of its own (addresses rebased) until vertex input is per-stream. The offline translation (decision 3) could emit per-stream vertex input |
| **The constant replay** (command 7, `sub_8258FD48`) | The recorded payloads; interpolation substitutes blends into them and puts them back (`blend_state.cpp:4100-4115`) | The native constant setters copy into the shadow; `State::Draw` copies each shader's used registers into the ring (`passes.cpp:3795-3830`) | A native command 7: substitution, then the interface's `SetConstants` | Interpolation's join is the payload address plus an exact value (`Substitute`, `blend_state.cpp:2656`); the substitution must stay at that point |
| **The vertex shader patch for the declaration** (`sub_822AE340`, the copy in the command buffer when "the GPU may still use it") | The game's Direct3D at the shader load | The renderer (`g_loaded_vertex_shader`, `replay_hooks.cpp:50-70`) | The backend translates each (vertex shader, declaration) pair itself: the declaration is part of the shader's key | Re-implementing the patch's result in the translation step (or offline): read the patcher first |
| **Textures reloaded on memcpy writes** (`textures.cpp`; `MarkWritten` `:1236`; views keyed by the fetch constant's six words) | memcpy into texture memory (the archive and XPR loaders) | `TextureCache::BeginFrame` marks the textures overlapped; they reload on next use | Textures made and uploaded through the interface when the game creates or loads them (`XPR::GetTexture`, `Resource::Lock` and Unlock are native), bound by handle | Textures the game writes after creating them (the license plate, the vinyl sheet: `car-models.md`) need their native writers to call `UpdateTexture`, or the write tracking kept for them |
| **Shader microcode translation** (the SDK's SPIR-V translator, `shaders.cpp:272`; disk cache `cache/`; lookups by the objects' fields) | Game data; the 14 stored boot shaders (`compiled_shaders.h`) | Our renderer, per pipeline | Vulkan: the runtime translator stays, inside the backend. Other backends: offline blobs of the game's shader and declaration pairs (decision 3) | — |
| **`replay_hooks.cpp`, `draw_packets.cpp`, `gpu_link.cpp`, `refcount.cpp`'s render-thread skip** | — | — | Native handlers | Go when the handlers are native and no longer call the game's Direct3D |

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
- **The harnesses** (`--run_harnesses`) for every game function a step
  makes native, with cases in the same change.
- **Render thread profiles** (`profile_render_thread.sh`, two or three
  runs a build, alternating): a step may be faster, but none should be
  slower unseen.
- **Interpolation**: `--log_blend_state` and `--log_stepping_constants`,
  then a drive by the user, for every step from 1 on.

| Step | What | Removes | Checked by | Size, risk |
|---|---|---|---|---|
| **0** | **Measure and log.** A render thread profile on the current code (a baseline: none since the setters, shaders and render queue went native). A pass log for one race frame and one menu frame. For each pass it records: the targets set; the draws; each resolve (rect, flags, destination); whether the source is drawn into again before the next clear; and whether each destination is sampled before it's resolved into, within the frame. Also whether game code outside the replay reads the register shadow (a watch on its blocks for a race) | Nothing; it settles the open rows of section 2 | The log; the profile | Small. **Needs the user to run it** (or the tree free of the B7 loop) |
| **1** | **Native command handlers, as they are**: the 33 handlers and the single commands in plain C++, still calling the game's (native) Direct3D, command 7 with interpolation's substitution where it is. The renderer's hooks called directly | The guest round trip per command; `REPLAY_COUNTED_HANDLER` | Harness cases per handler; `draws.txt` identical; `--log_blend_state`; a drive | Medium (a B7-style port, so B7's rules) |
| **2** | **The interface, fed beside the Direct3D path.** The interface defined; the native handlers also call it; the Vulkan renderer builds each draw's state both ways (from the interface, and from the registers as now) and logs mismatches | Nothing yet | 0 mismatches over an attract race, a drive and the menus | Large: the render states' and formats' mapping is the work |
| **3** | **The renderer switched to the interface.** `State::Draw` reads the interface's state. The handlers stop calling the game's Direct3D. The Vulkan backend makes the translator's registers itself, and translates (vertex shader, declaration) pairs itself in place of the game's patch. Resolves and clears are still made by the renderer as now (copies), from the handlers' arguments | The register shadow's writes and reads on the render path, `ReadRegisters`, the dirty masks, the PM4 packets, the kick, the shader load and patch, `replay_hooks.cpp`, `draw_packets.cpp`, `gpu_link.cpp` | Dumps identical (`draws.txt` and pixels); the menus and tiling by the user; profile; a drive | Large. After it, the game's Direct3D is off the render path |
| **4** | **Resources through the interface.** Textures and buffers made and filled when the game makes them (`XPR::GetTexture`, `Resource::Lock` and Unlock, `Mesh::Parse`, the loaders, the native texture writers), bound by handle. The Vulkan backend keeps fetch by address over its own arena until vertex input is per-stream | The mirrors, the fetch-constant-keyed texture lookups, most of memcpy's tracking | A check of every bound buffer and texture against guest memory over a drive (as `--native_renderer_verify_mirror` did); dumps identical; the hitch log | Medium |
| **5** | **Passes into their textures.** The destination scan (section 5): each pass draws into its destination; depth maps sampled as depth; the swap and the format changes gone; the glow's steps into their own textures; grab passes as `CopyTexture`; tiling as one target. MSAA keeps the game's sample counts for now (resolved at the pass's end) | The resolve copies but the grab passes, `ResolveInto`'s conversions, the resolve destinations' separate images | Dumps: each destination against the step before; differences explained (precision) | Medium. The exponent bias (section 2) is decided here |
| **6** | **MSAA as a player option** (`itc.toml`; off, 2x, 4x, 8x, plus a default matching the game, to decide). Applied to the targets the game drew with MSAA, or to all (to decide) | The 360's fixed sample counts | Dumps at each setting; the user | Small |
| **7** | **The frame composed natively** (`unlocked-res.md`, "Later"): first read the upscale's pixel shader and check that nothing else reads `1ECB9000`; then the 3D view drawn into the frame (or its region) and the HUD over it | The partial-rect resolve and the upscale draw | Dumps; the user at 4K | Small to medium |
| **8** | **The Vulkan backend behind the interface**: the Vulkan code moved under `src/render/vulkan/`, the interface free of Vulkan types; the shader blobs' format defined for other backends | Nothing at runtime | Builds and dumps identical | Medium, mechanical |
| later | **Tick replay** (`tick-replay.md`): with draws in the interface's terms, a tick's draws kept and replayed on render-only frames | The per-draw work on ~14 of 15 frames | Its own diagnostic first | Large; needs interpolation's join keyed by draw |

Steps 1 and 3 are where most of the 360 machinery leaves the render
path. Step 5 is where the resolves go.

## 7. Risks, and what's still unknown

- **Readers of the register shadow outside the replay.** Once the
  handlers stop calling the setters, nothing writes the shadow on the
  render path. The game's getters (one seen in play), the video output's
  set-up and the movie code (gone) may read it. Step 0's watch answers
  it. Probably little: the replay is where the state is set.
- **The command handlers' callbacks** (commands 20, 22, 28) call game
  objects that may reach the game's Direct3D; not read.
- **The vertex declaration patch**: what the game's patcher changes in
  the microcode (fetch strides and formats, probably) has to be
  reproduced in our translation step. Read `sub_822AE340` first.
- **Render states and formats**: the interface's mapping from the
  game's states and Xenos formats must match what the registers gave.
  That includes the main view's format switching per draw by render
  states 308-320 (`gpu-functions.md`). Step 2's both-ways check is there
  for this.
- **EDRAM aliasing.** Targets are keyed by EDRAM base, pitch, format and
  MSAA, and different keys don't share memory. Drawing is right that
  way, so the game probably doesn't read one surface's EDRAM through
  another. Drawing into destination textures (step 5) relies on it. To
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
  only as explained pixel differences in dumps.
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

1. **MSAA's default** in `itc.toml`: the game's own counts (2x main view,
   4x mirror, reflections, minimap and menus), or one setting such as 4x?
   And should the option apply to every target the game drew with MSAA,
   or only the main view and the menus' 3D view?
2. **Pixel differences from precision** (a pass drawn straight into a
   10/16-bit texture instead of 8-bit then converted): accept as
   explained differences, or keep the 360's precision where it shows?
3. **The Vulkan backend's shaders**: keep the runtime translator (as
   now), or also move Vulkan to the offline blobs, so every backend works
   the same way?
