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
