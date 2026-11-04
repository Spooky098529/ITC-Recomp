# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-01 12:56 on `b7-loop-01-11-26`, from d8348c7b;
  summary `b7-loop-2026-11-01-1256.md`. Limit: 2 subsystems (the
  user's call, 2026-11-01; 3 before): the run stops after the car draw.
- Subsystem 1 of 2: the boot mode (`boot-mode.md`). Done.
- Subsystem 2 of 2: `Car::DrawUpdate` `sub_820DAC68` (`car-draw.md`).
  Map checked (24c2b1c1: 86 rows, 14 moved from `functions.md`; 0
  errors; 1 ready group; the second set's per-frame passes lead covered
  and deleted; the reading split over six subagents, one at a time).
  Migration paused by the user after part 1 of about 16 (the split's
  parts, one subagent each, one at a time): a036f833, the shared leaves
  (`Model::Draw`, `CarPart::Draw`, `Matrix::Shadow`,
  `CarLighting::Gather`, `LightKit::BlinkLevel`,
  `BlinkTimers::Advance`, `SceneModel::IsNearPlayerHeight`; harness
  car_draw 0 differing, scenery_lists too; `itc_3223`: race 35 s, 0
  errors, all seven ran in the race, frame dumps on and off the same
  2,430 draws), then a6ae298c, `Matrix::Shadow`'s adapter removed (the
  lead finished it at the pause: `itc_3228`, race 38 s, 0 errors,
  car_draw 0 differing). Not yet validated.
- Resumed 2026-11-03 on `b7-loop-03-11-26`, which the user made at
  f6472b69 (the paused run's last commit) and checked out: the run goes
  on there. The car draw's migration finished (2026-11-04: parts 2 to
  16, 64ca2adf..d0f3ca85, 15 subagents one at a time; 78 of 78 ours,
  every harness 0 differing, races 30-37 s, 0 errors). Validation
  halted the run (see "Stopped").
- **To pick up** (the user's call, 2026-11-01: the next loop continues
  this run on this branch, not a new one): start a migration agent for
  the rest of `car-draw.md` (71 of the group's 78 left, still one ready
  group under `sub_820DAC68`), split into parts handed to subagents one
  at a time, each following `car-draw-brief.md` (the first agent's
  brief: the flag, the harness, the wraps, the quirks, the report).
  The order planned, bottom up: the morph chain (`sub_8235A688`,
  `sub_8258A9E8`, `sub_82594E88`, `sub_82595110`); the light kit
  (`sub_820DCC10`, `sub_820DD0B0`, `sub_820DD5C0`, `sub_820DDAE8`,
  `sub_820DDB98`, `sub_820DDC48`, `sub_820DDE58`); the small parts'
  draws (`sub_820DE1C8` to `sub_820DF458`, `sub_820E0998`,
  `sub_820E1138`, `sub_820E1260`); the tail lights and headlights
  (`sub_820DF8B0`, `sub_820E1690`); the doors, the headlight body and
  the shadows (`sub_820E0B40`, `sub_820E0E38`, `sub_820E13E0`,
  `sub_820E1D88`); the parts' top (`sub_820E2448`, `sub_820E2760`,
  `sub_820E2EE8`); the interior (`sub_820E3170` to `sub_820E53F0`);
  the cockpit (`sub_820DB8C8` to `sub_820DC498`, after the interior:
  `sub_820DC350` calls `SceneInterior::Draw`); the traffic
  (`sub_820E55A0`, `sub_820E5668`, `sub_820E5B08`, `sub_820E61D8`,
  `sub_820E6820`, `sub_820E6A40`); the wheels (the tires, rims A, rim
  B, the brakes, then `sub_820ECA30`, `sub_820ECB68`, `sub_820ECE28`);
  the root `sub_820DAC68`. Part 1's notes: the plain C++ is in
  `src/car_draw/car_draw.h` (namespace `car_draw`), every function
  taking `stack` and keeping the original's frame layout;
  `ModelDraw` and `CarPartDraw` take the caller's r24 (and r28) for the
  recorder's context and return
  `std::optional<scenery_lists::SceneryDraw>` (nullopt: the original
  runs as a guest call); `car_draw::Calls` is built by
  `hooks::CarDrawCalls`; the native `SceneLights::Draw` must skip
  `BlinkTimersAdvance` when `hooks::CarDrawBlinkTimersHeld()` is true;
  `src/car_draw/*.cpp` build with `-ffp-model=strict`. Then the
  validation agent (with part 1's functions and `Matrix::Shadow`'s
  removal in its list), step 9's records, and the run stops at its
  limit (2).

## Done

- The boot mode: `boot-mode.md` (map 49161912, migration 49f60a69,
  `--native_boot_mode`, 12 of 12 in play with 7 in `movies.md`;
  validated `itc_3219`: race 53 s, 0 errors, 19 of 19 harness lines,
  16 ran in the race, replay `itc_3220` ok, 3 to drive).

## Rows to review

`review.md`, "Run 2026-11-01 12:56".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3962, work left 2441 (wrapped 82, known 490,
  unknown 1869). `b7_status.py`'s one check (`movies.md:117`
  `sub_82108018`: its row says never called, but it ran) was there
  before the run.
- After the boot mode's map: ours 3962, work left 2441 (wrapped 82,
  known 498, unknown 1861).
- After its migration (`itc_3218`: race 38 s, 0 errors, boot_mode 0
  differing): ours 3981, work left 2424 (wrapped 80, known 483,
  unknown 1861).
- After its validation (`itc_3219`, 120 s: race 53 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3981, work left
  2424.
- After the car draw's map: ours 3981, work left 2424 (wrapped 80,
  known 547, unknown 1797).
- After the car draw's first part and `Matrix::Shadow`'s removal
  (`itc_3228`: race 38 s, 0 errors, car_draw 0 differing): ours 3988,
  work left 2417 (wrapped 78, known 546, unknown 1793).
- After the car draw's migration (`itc_3409`: race 33 s, 0 errors,
  car_draw 0 differing): ours 4059, work left 2346 (wrapped 71, known
  482, unknown 1793).

## Stopped

2026-11-04, halted at the car draw's validation (a validation `missing:`
and a harness that doesn't finish are halts, "When to halt"):

- `attract_race.sh 120 --run_harnesses=all` (`itc_3410`, again
  `itc_3412`): the `car_draw` harness hangs in `PartsPoints::DrawShadows`
  `sub_820E1D88`'s cases (`src/harnesses/car_draw_shadows.cpp`): after
  its `native running (harness)` line, "Unhandled guest access
  violation: read of guest 0x7000006F" repeats ~6.4 million times on the
  harness thread until the timeout (each log ~1 GB); the harnesses after
  it and the race never run (0 s). Which side reads `0x7000006F` isn't
  known yet. Run alone (`--run_harnesses=car_draw`, `itc_3411`) every
  car_draw case matches and nothing loops: probably guest state left by
  the harnesses before it, meeting the gap in the shadows harness world
  the migration noted (part 9: more draws from the shared Random made
  the original read `0xA4` forever, case 70; avoided, not fixed). As
  the `ai_drivers` wild-address case (dbc8ee0), a harness fix, likely
  its own Random and a fully set-up world.
- So 34 of the 78 had no harness line in the all-harness run
  (`missing:`: the cockpit, the parts' and interior's tops, the traffic,
  the wheels, the root). Every summary line printed was 0 differing.
- The pad replay (`itc_3413`, `race_test.jsonl`, 127 s, 0 errors) was
  ok: 69 of the 78 ran in it; 9 not in game (`to-drive.md`).
- Nothing uncommitted; `check_function_statuses.py` 0 errors.

To pick up: fix the shadows harness's world, then validate again
(`attract_race.sh 120 --run_harnesses=all`), then step 9's records for
the car draw (its row under "B7's loop runs", the boundary over time);
the run's limit (2) is then reached.
