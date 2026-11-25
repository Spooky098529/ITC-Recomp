# Car draw migration: the brief every part's agent follows

(Written by the car draw's first migration agent, 2026-11-01, for the
parts' agents it hands the work to, one at a time; kept here since the
run was paused after part 1, so the next migration agent uses it.)

You are one part of the B7 loop's migration of the car draw
(`docs/functions/car-draw.md`, mapped in 24c2b1c1), in this repo,
branch `b7-loop-01-11-26` (stay on it; never
make branches; never touch `main` or tags; never force-push). The
migration was split into parts handed to agents one at a time; nothing
else runs while you work. Earlier parts' commits are on the branch:
read `git log --oneline -15` and the code they added (`src/car_draw/`,
`src/hooks/car_draw.*`, `src/harnesses/car_draw*.cpp`, or wherever part 1
put them) and follow their structure, flag and harness.

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
- `docs/functions/car-draw.md`: the intro (the wraps and the behavior to
  match) and your functions' rows. The status definitions are at the top.
- A recent migration for the patterns: the load poll (`src/load_poll/`,
  `src/hooks/load_poll.cpp`, `src/harnesses/load_poll.cpp`,
  `git log --oneline -8 -- itc_recomp/src/load_poll`), the boot mode
  (49f60a69), and the scenery model draws
  (`src/scenery_lists/model_draws.cpp`: native draws that call the
  recorder `RecordObject` with `BlendState_BeforeRecordObject`,
  `BlendState_SetRecordContext` and `BlendState_AfterRecordObject`, and
  hand material groups to the render queue with the original's return
  addresses).

## Rules for this migration

- Flag `--native_car_draw` (one flag for the whole subsystem; part 1
  defines it, on by default once checked), falling back to
  `__imp__sub_X`. Harness name `car_draw` (registered in `harness.cpp`'s
  list and the flag's help text); more files under `src/harnesses/` are
  fine if one gets big, all under the `car_draw` harness name (or
  `car_draw_<part>` names if the earlier parts did so; follow them).
- Each plain C++ function starts with `ITC_NATIVE_RUNNING("sub_X Name");`.
- `grep -rn sub_X itc_recomp/src` for every member first (one override
  per function). Existing wraps stay around native bodies, calling the
  native body where they called `__imp__` (the load poll's precedent:
  `ITC_FRAME_STATE_WRAP_LOAD_POLL` in `src/debug/frame_state_diff.cpp`):
  - `ITC_FRAME_STATE_WRAP` (`frame_state_diff.cpp`) wraps
    `Car::DrawUpdate` 820DAC68, `SceneModel::IsNearPlayerHeight`
    820DAB58, `SceneCockpit::Draw` 820DC498, `SceneLights::Draw`
    820DDE58, `SceneParts::Draw` 820E2EE8, `SceneInterior::Draw`
    820E53F0, `SceneTraffic::Draw` 820E6A40, `SceneWheels::Draw`
    820ECE28: the wrap calls the native body under the flag (native
    callers call the plain C++, as the load poll's).
  - `BlinkTimers::Advance` 820DC958 is skipped on render-only passes
    (`--per_tick_blink_timers`, `src/frame_pacing/per_frame_logic.cpp`):
    that skip must hold on the native path too (the native caller
    `SceneLights::Draw` must not advance them on a render-only pass when
    the flag is on).
  - `CarPart::Draw` 820D06F0 is called through its adapter by the native
    cockpit dials (`src/hooks/cockpit_dials.cpp`, around
    `BlendState_SetLivePose`); keep that working (its adapter stays).
  - Frame interpolation pairs the recorder's products by the world
    matrix's guest address and, with `--pair_by_object`, by the record
    context (`lr` 0x820D0564 and r24-r31 at `Model::Draw`'s call of the
    recorder, r31 the model): a native `Model::Draw` sets the same context
    (`BlendState_SetRecordContext`, values as the original's registers
    hold at that call). Native callers build their matrices at the same
    guest stack addresses as the original (the original's frame sizes and
    offsets, back chain written as `stwu` does), so the world addresses
    the recorder sees don't change. Any return address a diagnostic or
    the render queue reads is the original's.
- Copy the original's quirks (B7 changes no game behavior): stack
  leftovers and unwritten words as the original leaves them; the rear
  bumper's draw writing guest words 432 and 456 with no plate model; the
  race shadow draws reading lighting words left unwritten outside a race;
  the state left across passes (door and headlight matrices, plate
  textures, the glow model's materials, the morph memory and job).
  Float math in the original's order and precision (single-precision
  rounding where the original rounds, `fmadds` as fused where the
  original fuses; look at how `src/matrix_math/` and
  `src/second_set/` do it).
- Callees that are already ours: call their plain C++ (grep `src/` for
  the address). Callees still recompiled (for example the morph's worker
  job `sub_820AC7F0` through `[0x8296B914]`, or a callee "Not seen in
  play") are made as guest calls from the original's frame, as other
  native code does (a `Calls` struct of `std::function`s, see
  `load_poll.h`), unless your part ports them.
- Harness cases for every function made native, in the same change:
  random inputs over the branches, edge cases, real inputs where known;
  new code uses `harness::Outputs` (floats via `Float`/`Floats`, the rest
  exact). Compare everything the function writes (its guest memory, the
  command list and render queue entries it makes, r3). Every case
  matches outside "(expected to differ)" groups (with a reason in a
  comment and the row).
- Build: `cmake --build itc_recomp/out/build/linux-amd64-release
  -j$(nproc)`. Race: `itc_recomp/tools/attract_race.sh 40
  --run_harnesses=car_draw[,<harnesses of other subsystems whose src/
  files you touched>]`: ~20 s at 30 ticks/s (fewer only when the
  harnesses took the time), 0 new error lines, every harness 0 differing
  outside "(expected to differ)". A stalled boot is restarted once.
  If your functions run in an attract race, a temporary counter override
  (or the `ITC_NATIVE_RUNNING` line in the log) shows they ran, removed
  before committing. For anything drawn: a frame dump
  (`--native_renderer_dump=<dir in the scratchpad>`) with the flag on
  against one with `--native_car_draw=false` (attract races put a random
  sepia or grayscale tint over the screen: compare what's drawn, not the
  tint; look at the images with Read). No profiling.
- On a failure: add logging, run, read the log; no fixes by guessing. If
  that doesn't settle it, leave the work uncommitted and say so.
- Records: the rows in `docs/functions/car-draw.md` (Status Overridden
  for an adapter, Merged for work done inside a native caller with no
  plain C++ function of its own, saying where; note the flag and the
  harness), then `itc_recomp/tools/check_function_statuses.py`; what was
  learned in the subsystem's doc (the intro or rows). Keep row text in
  the file's style.
- Commit by explicit path only (`git add <paths>`), check
  `git diff --cached | grep -n /home/` is empty, message
  `B7 loop: <what> native under --native_car_draw (harness car_draw)`
  ending with the line
  `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`, then
  `git push origin b7-loop-01-11-26`. Commit several times within the
  part if it's big (each commit builds).
- Then removable adapters: `python3 itc_recomp/tools/b7_groups.py
  --removable <the file's functions>`; for each candidate follow
  "Removing an adapter" (callers' code read, adapter not called
  elsewhere, harness calls plain C++ and `__imp__` directly, temporary
  counter shows it's not called in a race), remove it in its own commit
  with its row set to Native, race again. Keep adapters that wrap
  (diagnostics, the cockpit dials' caller) or are reached through
  pointers.
- Never use subagents yourself.

## Your report (your last message)

```
part: <the part's name>
commits: <hash: subject, one a line>
functions: <made native, each "sub_X Name: Overridden|Merged|Native">
harnesses: <each "harness: <function>: N cases, M differ" line, verbatim from the log>
expected to differ: none | <function: the group's line, verbatim; the reason>
race: <seconds at 30 ticks/s>, <error lines>; ran in the race: <functions seen>
frames: <what the dumps showed, on against off>
removed adapters: <functions> | none
uncommitted: none | <what and why>
problems: none | <what failed and what the log showed>
notes for the next parts: <structure, helpers, anything they must know>
```
