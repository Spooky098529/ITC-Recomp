# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-25 23:42 on `b7-loop-25-10-26-4` (the user's
  branch), from 45b1205; summary `b7-loop-2026-10-25-2342.md`.
- Limit for this run: 3 subsystems (the default).
- Subsystem 1 of 3: `leads.md`'s top lead, `sub_82115D98`,
  `sub_820CD5D8`, `sub_820CD7F0` (the race's set-up and a racer
  record's part set-up; 16 ready groups under them). Mapped as the
  race's set-up (`race-setup.md`, 52 rows, 7d2acaa; 3 leads added).
  Map checked (0 errors, 52 of 52 in play, 19 ready groups, no kernel
  calls; blocked by `RacerRecord::Init` `sub_820FF808` and
  `Vehicle::SetupFromCar` `sub_820C27E8`). Migration 1 done after
  five batches (2e9d8eb..357f247: the car object's resets, `Car::Init`,
  `Clear`, `SetModel`, `FromBuild`, the race data's small pieces, the
  event's getters, `CourseRecord::Reset`, `CourseRoutes::Get` by one
  subagent; 8 Native with no adapter, 5 Merged; `--native_race_setup`;
  `itc_1866`: 22 s, 0 errors, every harness 0 differing). Left ready:
  `Race::StartPosition`, `Route::Set`, `Route::Reset`, `Stages::Reset`;
  `RacerRecord::Init` (`vehicle-accessors.md`) now ready, blocking 10
  groups. Validated (`itc_1867`: 26 of 26 harness lines, 18 ran in the
  attract race, 6 in the replay `itc_1868`, ok; 2 to `to-drive.md`).
  Migration 2 done after five batches (9b93272..482c90c:
  `RacerRecord::Init`, the stage list's and route's resets and sets,
  `Race::StartPosition`, the racers' adds and starts, `RaceData::Reset`,
  `Race::PlaceRacers`, `Race::SetUpRival` and `Race::SetUp`; 14
  adapters removed, 1 Native with none, 2 Merged; one new expected
  group, `Race::SetUp`'s traffic records' unwritten bytes; `itc_1889`:
  24 s, 0 errors, every other harness 0 differing). Left ready:
  `Vehicle::SetupFromCar` `sub_820C27E8` (`car-physics.md`, 4,148
  lines, to split), blocking `Car::SetupVehicle`. Validated
  (`itc_1890`: 24 of 24 harness lines, 17 ran in the attract race, 4 in
  the replay `itc_1891`, ok; 2 more to `to-drive.md`). Migration 3 done
  in four parts by subagents, one at a time (62e7b9a..2f54d5a:
  `CarData::Derive`, `ApplySettings`, `Vehicle::ApplyTuning`,
  `Vehicle::SetupFromCar` under `--native_car_physics`,
  `Car::SetupVehicle`; 6 adapters removed in `car-physics.md`; 24 s, 0
  errors, every harness 0 differing). Nothing left in `race-setup.md`
  (52 of 52 ours); car physics' other ready groups to a "no mapping"
  lead. Step: validation agent 3 running.

## Done

(nothing yet)

## Rows to review

This run's rows: `review.md` ("Run 2026-10-25 23:42").

## Expected to differ

This run's groups: `expected-differences.md` (the "23:42 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 2152, work left 4299 (wrapped 104, known 540, unknown 3655).
  `b7_status.py`'s one check at the start: `movies.md:108`
  `sub_82108018`'s row says never called, but it ran (not this run's).
- After the race's set-up's map: ours 2152, work left 4299 (wrapped
  104, known 592, unknown 3603).
- After the race's set-up's migration 1: ours 2183, work left 4268
  (wrapped 104, known 561, unknown 3603).
- After the race's set-up's validation: race 24 s (`itc_1867`), 0
  errors, every harness 0 differing; replay 127 s (`itc_1868`), 0
  errors.
- After the race's set-up's migration 2: ours 2204, work left 4247
  (wrapped 104, known 540, unknown 3603).
- After the race's set-up's validation 2: race 24 s (`itc_1890`), 0
  errors, every harness 0 differing but the expected group; replay
  127 s (`itc_1891`), 0 errors.
- After the race's set-up's migration 3: ours 2209, work left 4242
  (wrapped 104, known 535, unknown 3603).

## Stopped

(running)
