# First-person views: lower frame rate on the Steam Deck

Status (2026-10-15, v0.3.7): measured on the Deck; the cause is the cost
of each draw on the render thread, and the cockpit simply draws more (its
interior and the rear-view mirror). So the work is making every draw
cheaper, in any view. Done so far: skipping repeated state calls and
diagnostics left running (about +9% uncapped together, see "Profile of
2026-10-15" below); reusing constant blocks and skipping descriptor binds
(~+1.4%) and copying constants from guest memory vectorized (~+14%), both
from `descriptor-reuse`, released in v0.3.7 (uncapped ~285 to ~320 fps); not
yet checked on the Deck. A fresh profile after v0.3.7 is in "Profile of
2026-10-15, after v0.3.7", with where libc and driver time comes from and
the next steps. Read `CLAUDE.md` first
(rules: find problems with logs, commit and push work branches after each
commit, keep docs up to date; no Windows code on branches off `main`).

## What the user reported

On the Steam Deck, with the build of `47484c8` (texture streaming fixed,
see `texture-handling-optimisation.md`), the game is smooth in the third
person view, but in the two first-person views (cockpit and bumper) a
thread sits at 100% and the game feels laggy. In third person a thread
"still spikes occasionally, up to 80% or so".

## The log

`itc_045.log` from the Deck (`run-debug.sh`, a race the user drove: about
50 s in cockpit, 20 s in bumper, then 30 s in third person, in that order;
not an attract race). The log isn't in the repo; the numbers below are
from it. "Per 5 s" values are the renderer's stats lines; frame times are
`--log_frame_times` (2 s windows).

| View (time in the log) | Draws a frame | Frames/s | Render thread CPU a frame (`render thread draw CPU`, avg) | Ticks released over 8 ms late, a second |
|---|---|---|---|---|
| Cockpit (01:35:02-01:35:52) | 2,570-3,050 | 42-52 | 17.5-23.7 ms | 12-22 |
| Bumper (01:35:52-01:36:12) | 2,030-2,360 | 57-65 | 14.5-17.8 ms | 9-18 |
| Third person (01:36:17-end) | 1,875-2,480 | 55-71 | 13.4-18.3 ms | 10-28 |

- **The busy thread is the render thread** (`XThreadA1FFD6C0`, entry
  `82091EF8`, `Renderer::ThreadLoop` in `functions.md`), at 97-98% of a
  core in **all three views**. With `--max_fps=144` it's always the limit
  on the Deck, so it's always busy; what changes between views is how
  many frames it manages. (The user's 100% against 80% was likely seen
  in another tool's sampling; the log doesn't show a difference in
  busyness.) The main thread is at 46-61%, everything else under 15%.
- **The cost follows the draw count**: about 7-7.5 µs of render thread
  time per draw in every view (cockpit ~21 ms / ~2,800 draws, third
  person ~15 ms / ~2,100). The cockpit draws ~600 more a frame (likely
  the car's interior: not checked), so it runs ~25% slower.
- **A 512x128 resolve** (`1EC79000`, format 54, flags `0x300`) is in the
  frame's resolve list in cockpit and bumper (lists logged at 01:35:23,
  01:35:50, 01:36:09) and gone in third person (01:36:28, 01:36:47);
  resolves per frame fell from 25.5 to 24.5 then. Its shape suggests the
  rear-view mirror; not confirmed. Bumper and third person draw about the
  same number of draws and run at about the same rate, so the mirror
  alone doesn't explain the bumper view feeling worse.
- **Late ticks**: the 30 Hz game ticks are often released over 8 ms late
  (the "late" count in the fps lines; see "frame pacer" in
  `functions.md`), 12-22 a second in cockpit. Ticks per second stay at
  30 and there are no grid restarts, so this is uneven motion (judder),
  likely what the user feels as lag, rather than slow motion.
- **Not textures, shaders or pipelines**: no frame over 40 ms in the race
  (the "long frame" lines are all at loads), texture loading at most
  4.7 ms in any frame, all shaders from the disk cache.

## What to do next

The fix is making each draw cheaper on the render thread (or doing fewer
of them), which is the item "Performance, to do: our renderer's own code"
in `native-renderer.md`'s Known issues. Find where the time goes first,
with a profile of the render thread **in the cockpit view**:

- The attract race (`--skip_intro --attract_after_s=1`) chooses its own
  camera, so the cockpit view needs the user: start the game (from the
  repo root, the command in `CLAUDE.md`), ask the user to drive a race in
  cockpit view, and while they do, record the render thread:
  `perf record -e cycles:u --call-graph dwarf,16384 -t <tid> -- sleep 15`
  (the tid is the `XThread...` with `[guest 8258DE50 -> 82091EF8 ...]` in
  the `--log_thread_cpu` lines, or from `ps -L`). Then the same in third
  person, to compare. `perf` can only attach to a process it started or
  with `ptrace_scope` allowing it; check, and if attaching isn't
  allowed, start the game under `perf record` instead.
- The last profile (2026-10-14, uncapped attract race on the PC, third
  person): `Passes::State::Draw` 27% (much of it in the Vulkan driver),
  `BlendState_ReplayConstants` 9%, `GetTextureSet` 7%, the guest memory
  mirror ~6%, shader lookups 3.7%, `ReadRegisters` 3.6%,
  `TextureCache::GetView` 3.4%, `GetTarget` 2%, `GetPipeline` 1.6%.
- Check what the extra cockpit draws are (the interior, the mirror's
  scene) with the existing per-pass tools in `native-renderer.md`
  ("Tools"), e.g. a dump (`--native_renderer_dump`) in cockpit view.
- If the mirror pass is a real cost, see whether its draws can be
  cheaper (it may redraw the scene with the same states).

Measure on the PC before and after any change: an uncapped attract race
(`--max_fps=0`, about 320 fps mean since `descriptor-reuse`; `timeout 20`, mean over
seconds 3-18 of the race, as in `CLAUDE.md`), alternating old and new
builds (keep a copy of the old binary outside the build directory; a copy
writes its logs to `logs/` beside itself, so read the newest log there and
check its time), and, only when the user agrees, a pinned one (`taskset -c
0`) as a stand-in for the Deck. Correctness: frames dumped at the same
moments (`--native_renderer_dump_every`) no longer match even between two
runs of the same build (the attract race allocates its buffers at other
addresses), so they can't check a change. Instead, for a change that
should give the same results by a faster path, add a temporary check that
computes both ways on every draw and logs the mismatches, run a whole race,
then remove it (as for the constant copies: 0 mismatches). Then a build for
the user's Deck: local builds need a newer glibc than SteamOS has, so
push the branch and run CI on it (`gh workflow run build-linux.yml --ref
perf-debug`, then `gh run download <id> -D itc_recomp/out/dist/<name>`;
the release preset is `-O3` with ThinLTO and `-march=x86-64-v2`). The
user unzips it on the Deck (`chmod +x itc run.sh run-debug.sh`) and
repeats the three views in order with `run-debug.sh`.

## Ideas for lightening the render load

Written 2026-10-15, before any profile in cockpit view: estimates, to be
checked. The game draws 2,000-3,000 draws a frame, little for a modern
GPU; the cost is our per-draw overhead from emulating how the 360 drew:
about 7 µs a draw on the Deck, roughly 1.5 µs on the user's PC (258 fps
uncapped at roughly 2,500 draws; a rough figure), where native renderers
manage well under 1 µs. Roughly by payoff for the effort:

1. **Redraw fewer passes on interpolated frames.** A race frame has ~25
   resolves: the shadow maps (two 1024x1024 depth passes), the 512x512
   and 1024x1024 reflection passes, the 256x144 glow chain, the 512x128
   one that may be the mirror. With interpolation, every in-between frame
   redraws all of them from the same tick. Shadows and reflections
   refreshed once a tick (30 Hz) likely wouldn't be noticed, and reusing
   the tick's images could remove a large share of the draws on most
   frames. Count draws per pass first (the dump tools in
   `native-renderer.md` can) to see what's at stake.
2. **Upload less constant data.** (Done in part: blocks the same as the
   last written are reused, see "Profile of 2026-10-15", Next. The
   estimate below was too high: ~9 KB a draw is reserved in the ring, but
   only ~2 KB written, the floats being packed.) About 9 KB of shader constants is
   uploaded per draw (`native-renderer.md`, Known issues, the constants
   ring): at 2,800 draws ~25 MB a frame, ~1.25 GB/s of copying at 50 fps,
   on the render thread. The translated shaders know which constants they
   read: upload only those, or only what changed since the last draw.
3. **Re-derive state only when it changed.** Every draw reads the whole
   register file, looks up shaders, builds a pipeline key and finds a
   texture set. The game's graphics library keeps dirty flags for exactly
   this; following them would skip most lookups, as consecutive draws
   usually change little (constants, maybe a texture).
4. **Split the render thread in two.** One thread replays the game's
   command list and runs the library, and also translates and records
   Vulkan. Recording on a second thread, pipelined a frame behind, could
   nearly double what the Deck can feed (its 4 cores are ~30% busy). The
   biggest structural change and the most work.
5. **Cheaper Vulkan calls**: push descriptors or descriptor buffers
   instead of allocating descriptor sets, more dynamic state, skipping
   rebinds of unchanged state (the last profile had `Passes::State::Draw`
   at 27%, much of it in the driver). **Skipping repeated state: done**
   (2026-10-15, `--native_renderer_skip_repeated_state`, see "Performance:
   repeated state calls" in `native-renderer.md`): +6.5% uncapped. The
   descriptor sets have since been bound only when they change (62% of
   draws).
6. **Smaller items**: the interpolation blending (~2.5-3 ms a frame on
   the Deck in cockpit; SIMD), and the library's remaining packet writers
   (~3-4%, in Known issues).

Start with the profile in cockpit view and a draws-per-pass count: they
show whether 1 and 2 are as big as estimated.

## Profile of 2026-10-15 (PC, attract race)

The user's view (2026-10-15): the cockpit just draws more, so general
render optimisations are the way; a cockpit profile isn't needed first.

**How it was profiled, with the release build as is** (no debug build:
`-O0` changes what's slow). Start the game uncapped in the background, take
the render thread's tid (the busiest `XThread...` in `ps -L -o
tid=,pcpu=,comm= -p <pid>`), then `perf record -e cycles:u -t <tid>`.
- A flat profile (`perf report --no-children --sort dso,sym`) is reliable.
- Call stacks don't work: `--call-graph dwarf` drops every sample whose
  stack passes through `itc` (unwinding fails there), and the release build
  has no line tables (`addr2line` gives `passes.cpp:?`), though perf does
  name inlined functions.
- To charge libc and driver time to the function of ours that called it,
  record the CPU's branch records instead: `perf record -e cycles:u -j
  any_call,u -t <tid>`, then `perf script -F period,ip,sym,dso,brstacksym`
  (or `brstack` for addresses) and take the first branch whose source is in
  `itc`. Calls go through the GOT (`call *(%r15,%rbx,1)`), so disassembly
  doesn't name the target, but `brstack`'s target addresses do. Tail
  jumps aren't recorded, so a function entered by `jmp` (e.g. `memcmp`)
  is charged to the call before it: treat those with care.
- libc has no symbols perf reads, but `objdump -d /usr/lib/libc.so.6`
  names its internal functions (`__memcmp_evex_movbe`, ...); the load
  address is in `perf script --show-mmap-events`.
- Within a big function (`Draw` is ~15 KB, much of it inlined), `perf
  annotate` doesn't work on this build: it shows other functions'
  instructions under the name. Instead take `perf script -F ip,sym,symoff`
  (the `ip` alone is a run-time address, the executable being
  position-independent), add each sample's `symoff` to the function's
  address from `nm -S`, and count the samples in address ranges of `objdump
  -d --start-address=... --stop-address=...` of it: e.g. the loop between
  two branches. That's how the float constant loop was found.
- After changing code meant to be vectorized, check the disassembly for
  it (`pshufb` for byte swaps on x86-64-v2): a change can quietly stop the
  compiler inlining or vectorizing it.

**Render thread, before the changes below** (uncapped, ~260 fps):

| Part | Share |
|---|---|
| Our renderer (`native::`) | ~37%: `Passes::State::Draw` 13%, textures ~8%, mirror/buffers ~5%, shader lookups ~3.5%, `ReadRegisters` 3% |
| The game's Direct3D library and replay handlers (guest code) | ~21% |
| Vulkan driver (`libvulkan_radeon`) | ~12%, 9% of it called from `Draw` |
| `memcmp` | ~5%, some of it called by the driver |
| `memcpy` | ~4.4%: 2.9% `Draw` writing constants into the ring, 1.1% the mirror |
| Interpolation | ~10%: `BlendState_ReplayConstants` 6%, its mutex ~1.6%, malloc/free ~1% |
| malloc/free elsewhere, other mutexes | ~2-3% |

**Render thread, after v0.3.6, before the constant copies** (uncapped,
~285 fps, `perf record -e cycles:u`, user time only; 2026-10-15): `Draw`
~20% of which the float constant loops ~14% (vertex 7.8%, pixel 6.6%),
`BlendState_ReplayConstants` ~6-7%, `ReadRegisters` ~4.7%,
`TextureCache::GetView` 3.2%, `IndexRange` 2.2%, `GetTarget` 2.1%, the
library's AddRef/Release and packet writers ~1-2% each; libc ~11% and the
Vulkan driver ~8.5% of all samples. The float loops and `ReadRegisters`
are now much cheaper (see Next, 1); no profile has been taken since, so
take one before choosing what's next.

The matrix math in `interpolation/transform.cpp` is only ~1.5% (~137 ns
a product, ~470 blended products a frame, measured with a benchmark):
not worth SIMD.

**Done** (v0.3.6; details under "Performance: repeated state calls" and
"Performance: diagnostics on every constant replay" in
`native-renderer.md`):
- Draws record viewport, scissor, depth bias, blend constants, pipeline
  and index buffer binding only when changed
  (`--native_renderer_skip_repeated_state`); indexed draws use
  `firstIndex`. Pixel-identical frames; uncapped 258 to 275 fps.
- The constant replay's diagnostics (a clock read and a status string,
  and a mutex-guarded lookup) run only with their flags: 275 to ~281.
- Pinned to one CPU the gain didn't show clearly (runs vary ~15%); the
  Deck's result is still to be seen.

**Next, in order of payoff for the effort:**
1. **Bind descriptor sets only when they change: done** (2026-10-15,
   branch `descriptor-reuse`; "Performance: constants written every draw"
   in `native-renderer.md`). A block the same as the last one written is
   reused; sets are bound for 62% of draws instead of all, constants
   written fell from 4.5 to 1.5 MB a frame, and uncapped went up ~1.4%
   (272.7/268.6 to 276.4/272.6 fps), less than hoped. The fetch block,
   reused for only ~40% of draws, limits the binds skipped, but not
   worth more work (measured 2026-10-15): it changes only where the draw
   reads it (a texture ~50% of draws, a vertex buffer ~15%), mostly along
   with the texture descriptor sets, so reusing it more could skip binds
   for at most ~8% more draws (~0.3% by the gain above). Details in
   "Performance: constants written every draw".
   **Reading constants from guest memory: done** (2026-10-15, same
   branch): packing the float constants walked all 256 bits of the
   shader's mask with a mispredicted branch each, and every word was a
   volatile load; ~19% of the render thread with `ReadRegisters`. Now run
   by run, copied with a vectorized byte swap: uncapped ~285 to ~325 fps
   (+14%). See "Performance: reading constants from guest memory" in
   `native-renderer.md`.
2. **Allocations in interpolation**: `PixelConstantBlender::Apply`
   returns a `std::vector` per lighting upload, and a `shared_ptr` is freed
   in the blending path. Check with a new profile whether any mutex is
   still taken per upload (the per-upload `SlotOwner` lock is gone).
3. The bigger ideas above: fewer passes redrawn on interpolated frames
   (1), following the library's dirty flags (3), splitting the render
   thread (4). Discussed with the user (2026-10-15), not tried: a host-side
   copy of the device's constants and state, kept up to date by hooking
   whatever sets them, so draws don't read guest memory. Guest memory has
   to be written as now regardless (the library reads its own device
   struct), and a write missed by the hooks gives wrong rendering, so the
   byte swaps it would save (now cheap) don't justify it alone; the
   payoff would be knowing what changed since the last draw without
   comparing, which is (3) and could skip shader, pipeline and texture
   set lookups too.

## Profile of 2026-10-15, after v0.3.7 (PC, attract race)

On `8ee1120` (v0.3.7 plus docs), uncapped attract race, ~318 fps mean
over the race's seconds 3-18; the render thread at ~98% of a core, the
main thread ~65%. Flat profile (`perf record -e cycles:u -t <tid>`, 12 s
from 14 s after launch, `--log_level=off` as `run.sh` runs; a run with
info logging gave the same picture plus ~1% of its interface maps):
`itc` 77%, libc 12%, the Vulkan driver 9.4%, libstdc++ 0.9%. Nothing
dominates any more:

| Part | Share | Biggest items |
|---|---|---|
| `Passes::State::Draw` itself | 10.8% | |
| Vulkan driver | 9.4% | see below |
| libc | 12% | see below |
| Texture binding | ~8% | `GetView` 3.5, `GetSwizzledView` 1.4, `GetTextureSet` 1.4, `GetSampler` 1.2, `SwizzledSigns`, `GetTextureLayout` |
| Mirror | ~7% | `IndexRange` 2.8, `Buffers_NewestWrite` 1.4, the per-frame `mirrored` set's insert 1.3 (`unordered_set<uint64_t>`), `Mirror` 0.8, `TakePending` |
| Interpolation | ~7% (+ ~1.3% of libc) | `BlendState_ReplayConstants` 7.1, `QuaternionFromRotation` 0.5 |
| Reading state | ~6% | `ReadRegisters` 2.9, `GetTarget` 2.6, `GetHostViewportInfo` 0.7 |
| Shader lookup | ~4% | `ShaderCache::Get` 1.7, XXH3 of the microcode 1.4, `GetInterface` |
| Replay handlers and library (guest) | ~15% | render states 2.0, sampler states 1.7, the shader reference count ~3 (below), SetIndices 0.8 |

**Where libc and driver time comes from** (branch records, `-j any_call,u`,
8 s; each sample outside `itc` charged to the source of the first branch
from `itc`, by a script over `perf script -F period,ip,dso,brstack` with
the load addresses from `--show-mmap-events`; the flat profile of a
branch-record run is skewed, so take shares from a plain run):

- `Draw` calls 10.3% of the thread's time outside `itc`: the driver 6.6%
  (call sites, by the function table offset they call through and their
  arguments: `vkCmdBindDescriptorSets` 2.3%, `vkCmdDrawIndexed` 2.2%,
  `vkCmdBindPipeline` 1.3%, one more 0.8%), `memcpy` 0.7% (one site),
  `memcmp` ~2% spread over ~10 sites, some of it the driver's (tail jumps
  aren't recorded, so a driver function that jumps to `memcmp` is charged
  to the call before).
- `PlaceConstants`: `memcpy` 0.9% and `memcmp` 0.8% (writing and
  comparing constant blocks).
- Interpolation's allocations: `_int_free` from a `shared_ptr`'s control
  block 0.9%, malloc/free from `BlendState_ReplayConstants` ~0.6%,
  `vector<float>` copies 0.2%: ~1.7% with libstdc++.
- `BeginFrame` freeing 0.4%; `GetTextureSet` into the driver 0.6% (sets
  allocated); `Resolve` into the driver 0.4%.
- **The "Release" samples (`sub_822A67B0`, 2.3%)** follow a call to AddRef
  from SetVertexShader (`__imp__sub_822A8808`) in every sample: it calls
  AddRef on the new shader, then reaches Release of the old one by a tail
  jump (not recorded). So that's the locked compare-and-swap of binding a
  different vertex shader (the ~48% of binds that aren't the same one),
  ~3% with AddRef's own 0.8%.

**Next, from this** (estimates):
1. **Interpolation's allocations and the replay's lookups: done**
   (2026-10-15, branch `replay-allocations`). The lighting blender
   (`PixelConstantBlender::Apply`, inlined into
   `BlendState_ReplayConstants` with `Substitute` and `BlendProduct`; the
   matrix math in `transform.cpp` keeps its own symbols, ~1.5%) allocated a
   252-float vector per upload and copied it three times, allocated a
   `vector<bool>`, looked objects up in two `std::map`s (one cleared every
   frame, so a node per upload) and compared the upload with a copy of
   itself. It now reuses its buffers, keys both maps by hash (the
   occurrence counts go stale by frame instead of being cleared, and are
   pruned with the entries), skips that compare when blending from the
   latest value (the default), and copies what `--log_pixel_outcomes`
   reads only with it. The per-object extras are a fixed array, and the
   replay frame the render thread drops (~1,500 products, a node each) is
   freed by the main thread when it publishes the next. Checked over two
   whole attract races (~30 million lighting uploads each) against the old
   blender run side by side: every value uploaded and put back the same;
   outcomes differed only for uploads holding a NaN at the race's end
   (old "not as recorded", since NaN never equals itself; new "rejected";
   both left as recorded). Uncapped attract race, alternating, 20 s runs:
   312.7, 321.8, 320.5 fps mean before, 330.8, 331.4, 330.2 after (~+4%;
   10th percentile 271/281/281 against 289/289/285). Render thread:
   `BlendState_ReplayConstants` 7.1% to 6.0%, libc's `free` ~1.6% to
   ~0.5%.
   Then, the same day: lighting uploads loaded with `guest::CopyFloats` (a
   byte-swapping loop meant to vectorize; it didn't once inlined, see
   "Done on `blend-math`") straight into the entry, without the
   copy, stopping early when equal to last tick's bit for bit. Checked the
   same way (also with `--pixel_blend_from_latest=false`): every value
   the same. **No measurable gain**: `BlendState_ReplayConstants` stayed at
   6.0%, fps within noise (328.2, 328.2, 320.7 before; 330.4, 330.8, 331.6
   after). So the lighting uploads' loads and copies weren't where the
   remaining 6% goes; what is isn't known (the camera products'
   `Substitute`, matrix loads and stores, or the ~20 cvar reads an upload
   are the candidates): split it with a profile (functions marked
   `noinline`, or sample offsets within the function) before more work.
   **The split** (a build with `Apply`, `Substitute`, `BlendProduct`, the
   matrix loads and stores and the logs' upload hooks `noinline`):
   `PixelConstantBlender::Apply` 3.6% (~24% its loop sorting the 252
   components, ~17% the loop storing the blend, ~15% at its entry),
   `BlendState_ReplayConstants` itself 1.8% (~20% a locked increment per
   camera product, ~25% putting the lighting upload back float by
   float), `ReplayCameras::Substitute` 1.1% (~55% the `unordered_map`
   lookup: a divide, then a chain of nodes), the matrix math ~1.5%, the
   logs' hooks ~0.6%. Fixes, one at a time:
   1. **The locked increment: done.** `counts_.replayed_products`, a
      `std::atomic` read only by `--log_blend_state`, is counted only with
      it. `BlendState_ReplayConstants` 6.01% to 5.75%; fps within noise
      (325.0 to 327.4 mean over three alternating pairs, one pair the
      other way).
   2. **`Substitute`'s lookup: done.** The frame's products are in an
      open-addressed table (`ProductTable`: slots of address and index, at
      most half full, a multiplicative hash; the products stored together)
      built by the main thread. Checked over a whole attract race against
      the `unordered_map` kept beside it: the same product for all 33.8
      million lookups. Measured by profile, not fps (the user's call: fps
      runs vary more than such a change; see below): `BlendState_
      ReplayConstants` 919 to 822 thousand cycles a frame (two runs each),
      ~0.6% of the render thread.
   3. **`Apply`'s loops through local pointers: done.** The flags it
      writes are bytes, which may alias anything, so the loops reloaded
      the vectors' start and end and recomputed the size every iteration
      (seen in the disassembly: `mov (%r12)`, `mov 0x8(%r12)`, `sar`,
      before; a compare with a register after). Also switched slots by a
      per-register table instead of a scan of six ranges per component.
      Checked against the old blender as before (both blend modes; only
      the NaN uploads' outcome differs). `BlendState_ReplayConstants`
      834, 809, 823, 854, 833 thousand cycles a frame (step 2's runs) to
      813, 783, 761 (~-45 thousand, ~0.3% of the render thread; less than
      the split's ~0.9% for the whole loop: the reloads were only part of
      it).
   4. **Storing the blend and putting it back: done.** The blender
      computes the whole upload into a buffer (the blend where it blends,
      the recorded bits elsewhere) and writes it with `guest::StoreU32s`
      (the store counterpart of `CopyU32s`); the restore after the upload
      uses it too. The select loop vectorized only once written without
      branches (the blend computed for every component, chosen with `&`
      rather than `&&`): check the disassembly for `mulps`/`subps`.
      Checked against the old blender as before, bit for bit (both blend
      modes). `BlendState_ReplayConstants` 796, 790, 818 thousand cycles
      a frame (step 3, alternating) to 733, 746, 731 (~-60 thousand, ~0.4%
      of the render thread).

   Over steps 1-4, `BlendState_ReplayConstants` went from ~920 to ~737
   thousand cycles a frame (5.9% to 4.8% of the render thread), on top of
   the allocations removed before (7.1% to 6.0%, and libc's `free` ~1.6%
   to ~0.5%): from ~1.1 million cycles a frame to ~0.74, about 2.3% of
   the render thread, plus ~1.1% of `free`.

   **Measuring small changes.** Fps in alternating runs varies ~±1.5%
   between runs of one build, and so does the render thread's total
   cycles per frame (15.55 and 15.79 million for one build), so changes
   under a few percent don't show in either. Instead, profile both builds
   at the same point of an attract race and compare the changed
   function's **cycles per frame**: its share of the profile times the
   thread's cycles (perf's "Event count"), over the frames drawn in the
   profiled seconds (the `--log_fps` lines stamped within them). Two runs
   each agreed within ~3%.
2. **The mirror's per-draw work** (~7%): cache each draw's index range
   (lowest, highest) by index buffer, count and format, dropped on a
   memcpy write as the mirror's copies are; a flat table instead of the
   `mirrored` `unordered_set`.
3. **Texture binding** (~8%): cache what `GetView`, `GetSampler` and
   `SwizzledSigns` work out per fetch constant (its 6 words).
4. **Descriptor binds** (2.3% in the driver plus our share of `Draw`):
   bound for ~62% of draws, mostly for new constant offsets. Push
   descriptors, or constants reached through one buffer with offsets in
   push constants, would cut them; a change to how the translated shaders
   read constants.
5. **The shader reference count** (~3%): the counts' atomics can't simply
   go (Release has ~40 callers, some on other threads). One option: hold a
   reference of our own to each vertex shader once bound, so SetVertexShader
   needn't touch counts; shaders would then never be destroyed (the user's
   call).
6. **Shader lookup** (~4%): key by object and declaration (Known issues
   in `native-renderer.md`).

## Next: interpolation's blending logic and matrix math

After `replay-allocations` (merged 2026-10-15), measured with
`itc_recomp/tools/profile_render_thread.sh` (uncapped attract race, render
thread ~15.5 million cycles a frame): `BlendState_ReplayConstants` ~772
thousand cycles a frame (5.0%; `PixelConstantBlender::Apply`,
`ReplayCameras::Substitute` and `BlendProduct` are inlined into it), and
the matrix math in `transform.cpp` ~290 thousand (1.9%):
`QuaternionFromRotation` ~100, `SplitViewProjection` ~53, the transform
decompositions ~60, blends ~45, `Multiply` ~16.

Leads, not yet measured:

- **Decompositions repeated every frame.** `BlendProduct` decomposes both
  ticks' world matrices (`ScaledTransformFromWorldMatrix`, quaternions) and
  splits both ticks' cameras (`SplitViewProjection`) for every product on
  every frame, though they depend only on the tick's inputs; at ~330 fps a
  tick is ~11 frames. And ~1,500 products a frame share a handful of
  cameras, each split again per product. Computing them once per tick (or
  per distinct matrix) would leave only the blend and the multiply per
  frame.
- **The lighting blender's per-component sorting** (in `Apply`): within a
  tick, render-only frames upload the same values, so its outcome and
  which components keep the tick's value could be kept per entry and
  reused while the upload is bit for bit the one sorted.
- A fresh split first: mark `Apply`, `Substitute`, `BlendProduct` and
  the logs' upload hooks `[[gnu::noinline]]` in a temporary commit (as
  `adf4a8f` did), profile, revert.

### Done on `blend-math` (2026-10-15)

Measured with `profile_render_thread.sh`, two or three runs a build,
alternating. Builds with `PixelConstantBlender::Apply`, `Substitute`,
`BlendProduct` and the logs' upload hooks `[[gnu::noinline]]` (a temporary
commit), so the parts show apart. **The split** (thousand cycles a frame):
`Apply` ~370-410, `BlendState_ReplayConstants` itself ~250, `Substitute`
~85, and the math that depends only on the tick ~235:
`QuaternionFromRotation` ~130, `SplitViewProjection` ~50, the
decompositions ~60. All of `interpolation::` ~1,170.

1. **Blends planned once a tick.** The main thread, after `MatchTick`,
   works out each paired product's `ProductPlan` (both world matrices
   decomposed, which path the world takes: blend, wheel or the tick's) and
   splits each distinct pair of view-projections once (`CameraPair`,
   keyed by both matrices' bits); the replay frame shares the tick's
   plans. The render thread only blends and multiplies. Checked over two
   whole attract races (17 million products) against the old
   `BlendProduct` run beside it: the same matrix, parts and stats for
   every one. `interpolation::` ~1,170 to ~950 thousand cycles a frame.
   (`--log_blend_state`'s sample of rejected cameras is now logged once a
   tick per distinct pair rather than per product.)
2. **Each camera blended once a frame** (`CameraBlends`, by the plan's
   camera index, stamped by frame). Checked the same way (15.5 million
   camera products, the same view-projection and eye). ~993 to ~920.
3. **The byte swap by hand.** An annotated profile of `Apply` (by
   address: the build has no line info, and `perf annotate -s <name>`
   silently annotated another function) showed the sorting loop ~28%
   (mostly the per-component "unchanged" test), loading the upload
   (`CopyFloats`) ~16% and storing it (`StoreU32s`) ~8.5%, both a scalar
   `bswap` per word: the loops in `guest.h` vectorize alone but stayed
   scalar once inlined. They now swap four words with SSSE3's `pshufb`
   (`_mm_shuffle_epi8`, part of x86-64-v2), the rest one by one; scalar
   without SSSE3. Checked against the scalar loops for counts 0-70 at
   every alignment. `CopyU32s` is also how the renderer reads registers
   and constants, which were slow too (`ReadRegisters` already had
   `pshufb` in its disassembly; why the time went anyway wasn't looked
   into): `ReadRegisters` ~453 to ~46,
   `Passes::State::Draw` (constants) ~1,730 to ~1,500, `Apply` ~408 to
   ~350, all of `interpolation::` ~923 to ~837 thousand cycles a frame.
   Uncapped attract races, alternating: 341.8, 345.7, 346.1 fps mean
   against 332.1, 337.9, 335.6 before (~+2.8%).

Checking a change: run the old code beside the new over a whole attract
race and compare every result, as for steps 1-4 (the lighting blender:
cherry-pick `2c82ae6` and `5942f31`, then `--check_pixel_blender=true`; the
known difference is the outcome label of NaN uploads at the race's end;
the product table: `13ac56f`, which checked every lookup).
