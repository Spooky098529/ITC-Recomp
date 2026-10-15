# First-person views: lower frame rate on the Steam Deck

Status (2026-10-15, v0.3.6): measured on the Deck; the cause is the cost
of each draw on the render thread, and the cockpit simply draws more (its
interior and the rear-view mirror). So the work is making every draw
cheaper, in any view. Done so far: skipping repeated state calls and
diagnostics left running (about +9% uncapped together, see "Profile of
2026-10-15" below); reusing constant blocks and skipping descriptor binds
(~+1.4%, branch `descriptor-reuse`); not yet checked on the Deck. Read `CLAUDE.md` first
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
(`--max_fps=0`, about 280 fps mean since v0.3.6, seconds 3-48 of the race), and a
pinned one (`taskset -c 0`) as a stand-in for the Deck. Then a build for
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
   descriptor sets are still bound every draw.
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
   reused for only ~40% of draws, limits the binds skipped; packing only
   the fetch constants a shader reads is the next step there, if worth it.
2. **Allocations in interpolation**: `PixelConstantBlender::Apply`
   returns a `std::vector` per lighting upload, and a `shared_ptr` is freed
   in the blending path. Check with a new profile whether any mutex is
   still taken per upload (the per-upload `SlotOwner` lock is gone).
3. The bigger ideas above: fewer passes redrawn on interpolated frames
   (1), following the library's dirty flags (3), splitting the render
   thread (4).
