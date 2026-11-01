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

## Stopped

2026-11-01, paused by the user (working on something else) in the car
draw's migration, after its part 1 (a036f833) and `Matrix::Shadow`'s
adapter removal (a6ae298c). Not a halt: nothing failed (every harness
run 0 differing, no attract race errors), nothing uncommitted,
`check_function_statuses.py` 0 errors. The next loop continues this
run from "To pick up" under "Now" (the user's call): it doesn't start
the progress file afresh, and it finishes the car draw (migration,
validation, records), then stops at the run's limit of 2.
