# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-04 08:44 on `b7-loop-03-11-26` (the thirtieth
  run's branch, merged; the user's call, 2026-11-04: the new run stays
  on it), from c9283680; summary `b7-loop-2026-11-04-0844.md`. Limit: 3
  subsystems.
- Subsystem 1 of 3: the race flow's set-up `sub_82160AE0` (lead picked;
  the two leads above it deleted as covered by existing files). Map
  checked (ef6eef64: `race-flow.md`, 5 rows, all new; 0 errors; 1 ready
  group, `sub_82160AE0`, 1,661 lines, split the migration). Migration
  1e19350e (`--native_race_flow`, 1 of 1 in play; `itc_3417`: race 38
  s, 0 errors, race_flow 12 cases 0 differing; no subagents). Done.
- Subsystem 2 of 3: the car scene's draw `sub_820C5DE8` (lead picked:
  runs every frame, 11 ready groups under it, 21 functions left in its
  subtree; the thirtieth run's car draw waits on it). Map checked
  (c98bcea2: `car-scene-draw.md`, 21 rows, 11 moved from
  `functions.md`; 0 errors; 11 ready groups; 9 Wrapped by
  `frame_state_diff.cpp`, kept around native bodies). Migration
  d5d7449c..6e318bb5 (`--native_car_scene_draw`, 21 of 21 ours, 12
  adapters removed; four parts, subagents one at a time, brief
  `car-scene-draw-brief.md`; `itc_3444`: race 33 s, 0 errors, every
  harness 0 differing; frame dumps on and off the same 2,430 draws).
  Done.
- Subsystem 3 of 3: the front end's car draw `sub_8217FE60` (lead
  picked, with the model draws' lead `sub_820C8480` it calls: 4 of 4
  roots ready, 28 ready groups; `CarScene::Draw`'s caller in the front
  end). Map checked (3beadd5a: `front-end-car-draw.md`, 24 rows, all
  new, 15 in play; 0 errors; 9 ready groups; the root waits on
  `CarScene::Update` `sub_820C9608`). Migration 38b4a134..43fa606c
  (`--native_front_end_car_draw`, 17 of 18 in play ours, 7 adapters
  removed, with `ApplyLightKit` in `load-poll.md`; the root waits on
  `CarScene::Update`; one subagent for `PoseCar`; `itc_3474`: race 38
  s, 0 errors, every harness 0 differing; replays reach the garage and
  the tuning shop, dumps there pixel-identical on and off). Step:
  validation, agent running.

## Done

- The car scene's draw: `car-scene-draw.md` (map c98bcea2, migration
  d5d7449c..6e318bb5, `--native_car_scene_draw`, 21 of 21; validated
  `itc_3448`: race 49 s, 0 errors, 21 of 21 harness lines, 20 ran in
  the race, `Projector::BuildOrtho` in the replay `itc_3449` (ok);
  none to drive).
- The race flow's set-up: `race-flow.md` (map ef6eef64, migration
  1e19350e, `--native_race_flow`, 1 of 1 in play; validated `itc_3418`:
  race 49 s, 0 errors, 1 of 1 harness lines, ran in the race, replay
  `itc_3419` ok, none to drive).

## Rows to review

`review.md`, "Run 2026-11-04 08:44".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4059, work left 2346 (wrapped 71, known 482,
  unknown 1793).
- After the race flow's map: ours 4059, work left 2346 (wrapped 71,
  known 483, unknown 1792).
- After its migration (`itc_3417`): ours 4060, work left 2345 (wrapped
  71, known 482, unknown 1792).
- After its validation (`itc_3418`, 120 s: race 49 s, 0 errors, every
  harness 0 differing but the expected groups): ours 4060, work left
  2345.
- After the car scene's draw's map: ours 4060, work left 2345
  (wrapped 71, known 492, unknown 1782).
- After its migration (`itc_3444`): ours 4081, work left 2324
  (wrapped 62, known 480, unknown 1782).
- After its validation (`itc_3448`, 120 s: race 49 s, 0 errors, every
  harness 0 differing but the expected groups): ours 4081, work left
  2324.
- After the front end's car draw's map: ours 4081, work left 2324
  (wrapped 62, known 495, unknown 1767).
- After its migration (`itc_3474`): ours 4099, work left 2309 (wrapped
  62, known 480, unknown 1767).
