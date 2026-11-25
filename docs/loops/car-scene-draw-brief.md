# Car scene draw migration: the brief every part's agent follows

(Written by the car scene draw's migration agent, 2026-11-04, for the
parts' agents it hands the work to, one at a time; modeled on
`car-draw-brief.md`, the previous run's.)

You are one part of the B7 loop's migration of the car scene's draw
(`docs/functions/car-scene-draw.md`, mapped in c98bcea2: `CarScene::Draw`
`sub_820C5DE8` and its subtree), in this repo, branch `b7-loop-03-11-26`
(stay on it; never make branches; never touch `main` or tags; never
force-push). The migration is split into parts handed to agents one at
a time; nothing else runs while you work. Earlier parts' commits are on
the branch: read `git log --oneline -15` and the code they added
(`src/car_scene_draw/`, `src/hooks/car_scene_draw.*`,
`src/harnesses/car_scene_draw*.cpp`) and follow their structure, flag and
harness. Your prompt names your part's functions.

## Read first

- `CLAUDE.md` (rules: never edit `generated/`, plain C++ behind thin
  adapters, American English, no absolute paths in the repo, stage only
  your own files by path, commit before any revert, find problems with
  logs not guesses).
- `docs/loops/b7-loop.md`: "The migration agent" (with "Comparing floats"
  and "Removing an adapter"), "The tooling", "Rules that still hold",
  "When to halt", "Precedent first". Follow them exactly.
- `docs/rexglue-removal.md`: "How a native override is written and
  checked" and "Checking with a harness".
- `docs/functions/car-scene-draw.md`: the intro (the wraps, what outlives
  the draw, frame interpolation's reads) and your functions' rows. The
  status definitions are at the top.
- The previous migration for the patterns: the car draw (`src/car_draw/`,
  namespace `car_draw`; `src/hooks/car_draw.cpp`;
  `src/harnesses/car_draw*.cpp` with `car_draw_shared.h`;
  `docs/loops/car-draw-brief.md`), and its wraps in
  `src/debug/frame_state_diff.cpp` (`Car::DrawUpdate` 820DAC68 and the
  load poll's `ITC_FRAME_STATE_WRAP_LOAD_POLL`). Matrix helpers already
  ours: `src/matrix_math/` (`Matrix::Multiply` `sub_8209BD50`,
  `Matrix::Copy`, `Matrix::Inverse`, `Vec4::Transform` `sub_820AB560`).

## Rules for this migration

- Plain C++ in `itc_recomp/src/car_scene_draw/` (namespace
  `car_scene_draw`; a `car_scene_draw.h` with the declarations), adapters
  in `itc_recomp/src/hooks/car_scene_draw.cpp`, one flag for the whole
  subsystem: `--native_car_scene_draw` (part 1 defines it with
  `ITC_FLAG_BOOL`, on by default once checked; later parts declare it),
  falling back to `__imp__sub_X`. Add new files to
  `itc_recomp/CMakeLists.txt` beside the car draw's; float-heavy files
  get `-ffp-model=strict;-ffp-contract=off` as the car draw's do (see
  the `COMPILE_OPTIONS` near line 709) and use explicit `std::fma` where
  `generated/` has `fmadds`/`fmsubs`/`fnmsubs` (check how
  `src/matrix_math/` and `src/second_set/` do it: single-precision
  rounding where the original rounds with `frsp` or a single op).
- Harness name `car_scene_draw` (registered in `harness.cpp`'s list and
  the `--run_harnesses` help text); more files
  (`src/harnesses/car_scene_draw_<part>.cpp`) are fine, all under the one
  harness name.
- Each plain C++ function starts with `ITC_NATIVE_RUNNING("sub_X Name");`.
- `grep -rn sub_X itc_recomp/src` (and the bare address) for every member
  first: one override per function. Wrapped functions keep their wrap:
  the root, `sub_820C4EB8`, `sub_820C4FB8`, `sub_820DAA38`,
  `sub_820DAAC8`, `sub_820DAB10`, `sub_82597518`, `sub_82597610`,
  `sub_82598768` are `ITC_FRAME_STATE_WRAP(address)` in
  `src/debug/frame_state_diff.cpp` (`--find_per_frame_state`,
  `--debug_hide`). Change each one's wrap to call the native adapter
  under the flag and `__imp__` otherwise (as `Car::DrawUpdate`'s and
  `ITC_FRAME_STATE_WRAP_LOAD_POLL` do; a new macro
  `ITC_FRAME_STATE_WRAP_CAR_SCENE_DRAW(address, adapter)` is fine), with
  no second `REX_FUNC` for them in `hooks/car_scene_draw.cpp` (the
  adapter there is a plain function `hooks::CarSceneDraw<Name>(ctx,
  base)` declared in `hooks/car_scene_draw.h`). Native callers call the
  plain C++ directly, not the wrap (as the car draw's callers do).
- Other users of these functions stay working: `sub_820DAA38`,
  `sub_820DAAC8`, `sub_820DAB10` and `Projector::Build` `sub_82597610`
  are also called by `Scenery::RecordHeadlightProjectors` `sub_820F34C8`
  (`course-scenery.md`, recompiled), and `Matrix::Perspective` by
  `TextureProjection::Fit`: their adapters stay while such callers do.
- Frame interpolation must keep working (`docs/frame-interpolation.md`):
  it follows `Matrix::Copy` and `Matrix::Multiply` into cameras and pairs
  products by guest addresses, so native code builds its matrices at the
  same guest stack addresses as the original (the original's frame
  sizes and offsets, the back chain written as `stwu` does, the same
  `Matrix::Copy`/`Multiply` calls in the same order with the same
  arguments), and whatever it records into the command list (the root's
  chunks, the command 7 uploads by return address `0x820C6B14`,
  `0x820C725C`, `0x820C7640`, `0x820C7A38`) is identical to the
  original's, return addresses included where anything reads them.
- B7 changes no game behavior: match `generated/` exactly, float math in
  the original's order and precision, stack leftovers and unwritten words
  as the original leaves them (`Plane::Intersect3` and
  `Plane::SegmentCrossing` leave out unwritten on 0; the box's NaN
  handling; the vector's growth).
- Callees already ours: call their plain C++ (grep `src/` for the
  address). Callees still recompiled or never seen in play (`_Xlen`
  `sub_825981C8`, `bad_alloc`'s copy `sub_82091200`, the vtables' slots:
  the render manager `[0x826BABA0]`, the render queue, the texture
  provider at the scene's +432) are guest calls from the original's
  frame, as other native code does (a `Calls` struct or `GuestCall`,
  see `src/hooks/guest_call.h`, `load_poll.h`), unless your part ports
  them. `operator new`/`delete`: `heap.md`'s plain C++.
- Harness cases for every function made native, in the same change:
  random inputs over the branches, edge cases (zero lengths, NaN, the
  0-returns, a vector growing past its capacity), real inputs where
  known; new code uses `harness::Outputs` (floats via `Float`/`Floats`,
  the rest exact). Compare everything the function writes (its out
  buffers, its frame's leftovers where a caller reads them, the guest
  globals, the command list chunks and render queue entries, r3). Every
  case matches outside "(expected to differ)" groups (with a reason in a
  comment and the row).
- Build: `cmake --build itc_recomp/out/build/linux-amd64-release
  -j$(nproc)`. Race: `itc_recomp/tools/attract_race.sh 40
  --run_harnesses=car_scene_draw[,<harnesses of other subsystems whose
  src/ files you touched, e.g. car_draw, course_scenery>]`: ~20 s at 30
  ticks/s (fewer only when the harnesses took the time), 0 new error
  lines, every harness 0 differing outside "(expected to differ)". A
  stalled boot is restarted once. A temporary counter (or the
  `ITC_NATIVE_RUNNING` line in the log) shows your functions ran in the
  race; a counter is removed before committing. For anything drawn (the
  root, the projectors, the fit): a frame dump
  (`--native_renderer_dump=<dir in the scratchpad>`) with the flag on
  against one with `--native_car_scene_draw=false` (attract races put a
  random sepia or grayscale tint over the screen: compare what's drawn,
  not the tint; look at the images with Read). No profiling.
- On a failure: add logging, run, read the log; no fixes by guessing. If
  that doesn't settle it, leave the work uncommitted and say so.
- Records: the rows in `docs/functions/car-scene-draw.md` (Status
  Overridden for an adapter, Wrapped stays Wrapped only while the wrap
  calls `__imp__` for the work: a wrap that calls a native body under the
  flag is Overridden, as `car-draw.md`'s rows for its wrapped functions
  say; Merged for work done inside a native caller with no plain C++
  function of its own, saying where; note the flag, the harness, and what
  you learned reading it in full), then
  `python3 itc_recomp/tools/check_function_statuses.py`. Keep row text in
  the file's style.
- Commit by explicit path only (`git add <paths>`), check
  `git diff --cached | grep -n /home/` is empty, message
  `B7 loop: <what> native under --native_car_scene_draw (harness
  car_scene_draw)` ending with the line
  `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`, then
  `git push origin b7-loop-03-11-26`. Commit several times within the
  part if it's big (each commit builds and was raced).
- Then removable adapters: `python3 itc_recomp/tools/b7_groups.py
  --removable <the file's functions>`; for each candidate follow
  "Removing an adapter" (callers' code read, adapter not called
  elsewhere, harness calls plain C++ and `__imp__` directly, temporary
  counter shows it's not called in a race), remove it in its own commit
  with its row set to Native, race again. Keep adapters that wrap
  (diagnostics), have recompiled callers in play, or are reached through
  pointers.
- Never use subagents yourself.

## Your report (your last message)

```
part: <the part's name>
commits: <hash: subject, one a line>
functions: <made native, each "sub_X Name: Overridden|Merged|Native">
harnesses: <each "harness: <function>: N cases, M differ" line, verbatim from the log>
expected to differ: none | <function: the group's line, verbatim; the reason>
race: <log itc_<n>>, <seconds at 30 ticks/s>, <error lines>; ran in the race: <functions seen>
frames: <what the dumps showed, on against off> | not drawn
removed adapters: <functions> | none
uncommitted: none | <what and why>
problems: none | <what failed and what the log showed>
notes for the next parts: <structure, helpers, anything they must know>
```
