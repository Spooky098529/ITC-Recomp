# First-person views: lower frame rate on the Steam Deck

Status (2026-10-15): measured on the Deck, cause narrowed down, not fixed.
Branch `perf-debug`. Read `CLAUDE.md` first (rules: find problems with
logs, commit and push work branches after each commit, keep docs up to
date; no Windows code on this branch).

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
(`--max_fps=0`, about 258 fps mean, seconds 3-48 of the race), and a
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
2. **Upload less constant data.** About 9 KB of shader constants is
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
