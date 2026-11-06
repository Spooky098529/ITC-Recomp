# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-06 14:33 on `b7-loop-06-11-26` (the previous
  run's branch, checked out by the user for this run), from 105ba4b9;
  summary `b7-loop-2026-11-06-1433.md`. Limit: 5 subsystems (the
  user's call, 2026-11-06, raised from 3 during the run).
- Subsystem 1 of 5: the course scenery's race-side owners (lead picked:
  `sub_8215B790`, `sub_8215D2B0`, `sub_821514A0`, `sub_821516F0`, the top
  of `leads.md`; ready groups under all four; its line deleted from
  `leads.md`). Map checked (db8e723a: `race-update.md`, 9 rows, 1
  moved from `functions.md`, 8 new, 7 in play; 0 errors; 8 ready groups,
  2 in the new file (`sub_8215B330`; `sub_8215D008` with `sub_8215C698`),
  6 in other files (`course-scenery.md`, `heap.md`, `vehicle-accessors.md`,
  `scene-models.md`, `scene.md`); no leads added; no subagents).
  Migration b30ad3c0..4a42fcc8 (`--native_race_update`: `VaryCar`, the
  race lights; under `--native_course_scenery`: `Scenery::PoolSizes`,
  `SceneryList::LoadAll`, `Stream`; under `--native_vehicle_accessors`
  and `--native_scene_models`: `ScenePools::SetUp` with its loaders;
  races 38, 38, 38, 36 s, 0 errors, all harnesses 0 differing; no
  subagents); left: `Scenery::Update` `sub_820F4558` (ready, read, its
  stack reads noted in its row) and the second set's set-ups; held:
  `XPhysicalSize` `sub_822965C0` (`MmQueryAllocationSize`, no
  precedent: asked the user) with `Scenery::AllocPhysical`/`FreePhysical`
  and their callers. Validated `itc_3702` (race 38 s, 0 errors, every
  harness 0 differing, 12 of 12 harness lines; 11 ran in the race,
  `CarParts::LoadSetA` in the replay `itc_3703` (ok); none to drive).
  The user's call (2026-11-06): `XPhysicalSize` answers a physical
  block's size from our allocation layer (`AllocPhysical` recording each
  block's size), for recompiled and native callers. Second migration
  7efd9cff..01089192 (`XPhysicalSize` from `memory::PhysicalSize`, its
  adapter removed; `Scenery::AllocPhysical`, `FreePhysical`, `Create`,
  `Setup`, `SetupPools`, `RaceScene::LoadScenery`; the second set's
  set-ups and `SceneInterior::SetUp`; races 38 and 36 s, 0 errors, all
  harnesses 0 differing; no subagents); left: `Scenery::Update` (ready,
  its harness needs enabled camera passes: its row) and `sub_820C5B98`
  (SetUpB's pool 0 case). Validated `itc_3722` (race 39 s, the
  harnesses ~80 s of the 120; 0 errors, every harness 0 differing, 13 of
  13 harness lines, all 13 ran in the race; none to drive). Third
  migration ba4bf51f..3aae586e (`Scenery::Update`; `ScenePools::SetUpB`
  with `sub_820C5B98` Merged, `SceneCockpit::SetUp`, `SceneModel::SetUp`,
  `SceneModel::Build`; `RaceScene::RemakeCarModel`; `RaceScene::Update`
  with `Car::TickUpdate` (one subagent, alone); 25 adapters removed;
  races 38 and 34 s, 0 errors, all harnesses 0 differing); left: none
  in `race-update.md` (7 of 7 ours). Validated `itc_3747` (race 36 s,
  0 errors, every harness 0 differing, 32 of 32 harness lines, all ran
  in the race; none to drive). Recorded.
- Subsystem 2 of 5: the scene models' holders (two leads taken together:
  what's left of the scene models' owners, `sub_8217FCB0`, `sub_82179998`,
  and the front end's re-applies of a scene model's record,
  `sub_82178DF8`, `sub_82178E60`, `sub_821799D8`, `sub_820E7380`,
  `sub_820E7470`, `sub_820D8F18`; ready groups under every root, next to
  the scene model work just done; both lines deleted from `leads.md`).
  Map checked (d33966e3: `stand-models.md`, 39 rows, all new, 28 in
  play; 0 errors; 12 ready groups, `TuningScreen::PreviewWheel`'s 2,171
  lines to split; 2 leads added; no subagents). Migration
  e8b7c8b2..ed0605b9 (`--native_stand_models`: all 12 ready groups, with
  `SceneModel::ApplyRecord` and `ReloadDecals` under
  `--native_scene_models`; races 34-38 s, 0 errors, all harnesses 0
  differing; one subagent, alone, for `SetWheel`/`PreviewWheel`); left:
  none in the file; `GarageStage::ReapplyRecord` `sub_82178DF8` and
  `ReapplyRecordNoDecals` `sub_82178E60` (`car-makers.md`) now ready (a
  cleanup lead). Validated `itc_3761` (race 36 s, 0 errors, every
  harness 0 differing, 32 of 32 harness lines; none in the attract race,
  13 in the replay `itc_3762` (ok), 19 to drive). Recorded.
- Cleanup lead (no mapping, not counted): `GarageStage::ReapplyRecord`
  `sub_82178DF8` and `ReapplyRecordNoDecals` `sub_82178E60` in
  `car-makers.md`; its line deleted from `leads.md`. Migration cea27e18,
  6fbff1bb (both native under `--native_car_makers`; the adapters of
  `SceneModel::ApplyRecord`, `ReloadDecals`, `ApplyLightKit` and
  `ApplyPaint`'s wrap removed; races 33 and 28 s, 0 errors, all
  harnesses 0 differing; no subagents); left: `GarageStage::SetCar`
  `sub_8217FD58` and `SetCarClass` `sub_8217FDC8` now ready, and the
  `ApplyWheelPaint` wrap. Validated `itc_3768` (race 36 s, 0 errors,
  every harness 0 differing, 6 of 6 harness lines from the plain C++;
  `ApplyPaint` in the race, four in the replay `itc_3769` (ok),
  `ReapplyRecordNoDecals` to drive). Recorded.
- Cleanup lead (no mapping, not counted): `GarageStage::SetCar`
  `sub_8217FD58` and `SetCarClass` `sub_8217FDC8` in `car-makers.md`,
  ready since the `ReapplyRecord` lead, with the makers by id above them
  and `SceneModel::ApplyWheelPaint`'s wrap. Migration 17174c73..c5e36a97
  (`SetCar`, `SetCarClass`, `ShowCarById`, `ShowCarByIdWithClass` under
  `--native_car_makers`; `CopyRecord`, `ReapplyRecord`,
  `ReapplyRecordNoDecals` adapters and `ApplyWheelPaint`'s wrap removed;
  races 37, 37, 33 s, 0 errors, all harnesses 0 differing; no
  subagents); left: none in `car-makers.md` and `stand-models.md`.
  Validation agent running.

## Done

- Cleanup: `GarageStage::ReapplyRecord`, `ReapplyRecordNoDecals` native
  (`car-makers.md`); `SceneModel::ApplyRecord`, `ReloadDecals`,
  `ApplyLightKit`, `ApplyPaint` Native; cea27e18, 6fbff1bb.

- `stand-models.md`: 28 of 28 in play native (the garage stage's
  holders and setters, the wheels' setters, the tuning screen's
  helpers; with `SceneModel::ApplyRecord`, `ReloadDecals` in
  `scene-models.md`), `--native_stand_models`; d33966e3,
  e8b7c8b2..ed0605b9.

- `race-update.md`: 7 of 7 native (`RaceScene::LoadScenery`, `Update`,
  `RemakeCarModel`, `VaryCar`, the lights, `Car::TickUpdate`; with
  `XPhysicalSize`, the scenery's set-up and update, `ScenePools::SetUpB`
  and `SceneModel::Build` in their files; 25 adapters removed),
  `--native_race_update`; db8e723a, b30ad3c0..3aae586e.

## Rows to review

`review.md`, "Run 2026-11-06 14:33".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4252, work left 2159 (wrapped 57, known 466,
  unknown 1636).
- After the race update's map: ours 4252, work left 2159 (wrapped 57,
  known 472, unknown 1630).
- After its migration: ours 4264, work left 2147 (wrapped 57, known
  460, unknown 1630).
- After its validation (`itc_3702`): ours 4264, work left 2147.
- After its second migration: ours 4277, work left 2134 (wrapped 57,
  known 447, unknown 1630).
- After its validation (`itc_3722`): ours 4277, work left 2134.
- After its third migration: ours 4286, work left 2125 (wrapped 57,
  known 438, unknown 1630).
- After its validation (`itc_3747`): ours 4286, work left 2125.
- After the stand models' map: ours 4286, work left 2125 (wrapped 57,
  known 466, unknown 1602).
- After its migration: ours 4319, work left 2092 (wrapped 57, known
  433, unknown 1602).
- After its validation (`itc_3761`): ours 4319, work left 2092.
- After the `ReapplyRecord` cleanup lead (`itc_3768`): ours 4321, work
  left 2090 (wrapped 57, known 431, unknown 1602).
