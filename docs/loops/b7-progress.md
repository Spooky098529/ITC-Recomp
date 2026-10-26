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
  lead. Validated (`itc_1909`: 8 of 8 harness lines, 7 ran in the
  attract race, `Car::SetupVehicle` in the replay `itc_1911`, ok;
  nothing to drive). Finished.
- The "no mapping" lead (not counted): the car physics' ready groups
  left in `car-physics.md` (`sub_820B0E70`, `sub_820B7998`,
  `sub_820B7A00`, `sub_820B9570`, `sub_820B8798`, `sub_8259D5A8` and
  what they unblock under `sub_820B62E8`, `sub_820B5670`,
  `sub_820B5B08`, `sub_820B6498`), picked as closest to ready.
  Migration 1 done after five batches (f8a45a7..b488eb2:
  `Vehicle::DestroyPhysics`, `ProbeGround`, `Unflip`, `RefreshFromBody`,
  `PhysicsStep`, `SetUpPhysics`, `ResetPhysics`, the collision objects'
  init and add, `CollisionWorld::Setup`, `Create`, `SetQueryCapacity`;
  74 adapters removed in four files, 3 Native with none; no new flag;
  23 s, 0 errors, every harness 0 differing; frame dumps matched the
  fallback). `car-physics.md` 129 of 134 ours. Left ready:
  `CollisionWorld::Update` `sub_820B92C8` (5 functions, 774 lines).
  Validated (`itc_1939`: 84 of 84 harness lines, 83 ran in the attract
  race, `Vehicle::DestroyPhysics` in the replay `itc_1940`, ok; nothing
  to drive). Migration 2 done (3a5e3bd, bd3e362: `CollisionWorld::Update`
  with `SolvePairs`, `CollideAll`, `Collide`, `Respond`; the two
  queries in play, `WallHits` and `GroundHits`, called natively; 10
  adapters removed; 23 s, 0 errors, every harness 0 differing; frame
  dumps the same as the fallback). `car-physics.md` 134 of 134 ours.
  Validated (`itc_1946`: 15 of 15 harness lines, 4 ran in the attract
  race, 11 in the replay `itc_1947`, ok; nothing to drive). Finished.
- Subsystem 2 of 3: the vehicle code above the physics (`sub_820BFD30`,
  `sub_820C3020`, `sub_820C3608`, `sub_820C3150`, `sub_820C33B8`,
  `sub_820C0118`, `sub_820C21E0`, `sub_820C20E0`, `sub_820C30D8`),
  picked as closest to ready (5 of its 9 roots ready since the car
  physics). Mapped as the vehicle (`vehicle.md`, 25 rows, ebba856; 2
  leads added). Map checked (0 errors, 25 of 25 in play, 15 ready
  groups, no blockers, no kernel calls). Step: migration agent 1
  running.

## Done

- The car physics' ready groups (no mapping; f8a45a7..bd3e362):
  `car-physics.md` 134 of 134 ours, 84 adapters removed in four files;
  validated (`itc_1939`, `itc_1946`); `--native_car_physics`.
- The race's set-up (`race-setup.md`, mapped in 7d2acaa; 52 rows): 52
  ours, and `RacerRecord::Init` and 4 in `car-physics.md`, in three
  migrations (2e9d8eb..2f54d5a), 27 adapters removed; validated
  (`itc_1867`, `itc_1890`, `itc_1909`); `--native_race_setup`. One
  expected group for review (`Race::SetUp`'s traffic records).

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
- After the vehicle's map: ours 2230, work left 4222 (wrapped 104,
  known 541, unknown 3577).
- After the car physics' validation 2: race 23 s (`itc_1946`), 0
  errors, every harness 0 differing but the expected groups; replay
  127 s (`itc_1947`), 0 errors.
- After the car physics' migration 2: ours 2230, work left 4222
  (wrapped 104, known 516, unknown 3602).
- After the car physics' validation: race 23 s (`itc_1939`), 0 errors,
  every harness 0 differing but the expected groups; replay 127 s
  (`itc_1940`), 0 errors.
- After the car physics' migration 1: ours 2224, work left 4227
  (wrapped 104, known 521, unknown 3602).
- After the race's set-up's validation 3: race 24 s (`itc_1909`), 0
  errors, every harness 0 differing but the expected groups; replay
  127 s (`itc_1911`), 0 errors.
- After the race's set-up's migration 3: ours 2209, work left 4242
  (wrapped 104, known 535, unknown 3603).

## Stopped

(running)
