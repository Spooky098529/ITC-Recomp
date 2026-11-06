# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-06 11:15 on `b7-loop-06-11-26` (the previous
  run's branch, checked out by the user for this run), from 6eac80c0;
  summary `b7-loop-2026-11-06-1115.md`. Limit: 3 subsystems.
- Subsystem 1 of 3: the power graph (lead picked: `sub_8217C708`,
  `sub_821DF350`, the "sprite UI element"; its line deleted from
  `leads.md`). Map checked (64372b27: `power-graph.md`, 6 rows, all
  new, 4 in play; 0 errors; 1 ready group, `sub_8217C708` with
  `sub_8217B508`; 2 leads added; no subagents). Migration f97f2857
  (`--native_power_graph`: `PowerGraph::Draw`, `Line::RecordFlat`,
  `TuningScreen::DrawPowerGraph`; `itc_3662`: race 38 s, 0 errors,
  both harnesses 300 cases, 0 differ; front end only, the replay
  `race_test.jsonl` didn't reach the tuning screen; no subagents);
  left: none, `CarList::DrawEntryPowerGraph` `sub_82250670` blocked by
  the car object makers' lead. Validated `itc_3663` (race 39 s, 0
  errors, every harness 0 differing, 3 of 3 harness lines; none in the
  attract race, all three in the replay `itc_3665` (ok); none to
  drive). Recorded.
- Subsystem 2 of 3: the car objects' makers (lead picked: `sub_82179A60`,
  `sub_82182C90`, `sub_82182D30`, `sub_821A1380`, `sub_82110448`; ready
  groups under all five roots, and it unblocks the power graph's
  `sub_82250670`; its line deleted from `leads.md`). Map checked
  (aca0ef24: `car-makers.md`, 25 rows, all new, 24 in play; 0 errors;
  11 ready groups; 3 leads added, two lead lines trimmed; no
  subagents). Migration 9d2ce651..1276cd9a (`--native_car_makers`:
  the record's setters and `FromCar`, the garage cars, `FromGarageCar`
  with the tuning screen's set-up, `FitCar`, `CopyRecord`, the busy
  marker; 4 adapters removed; `itc_3674`: race 38 s, 0 errors, 14
  harnesses 0 differing; no subagents); left: none ready, the rest
  waits on `SceneModel::ApplyRecord`/`ReloadDecals`, `ScenePools::SetUpB`,
  `CarScene::Update`, `GarageStage::Draw` and `sub_821A1380`'s handlers;
  `CarList::DrawEntryPowerGraph` `sub_82250670` now ready (a cleanup
  lead). Validated `itc_3677` (race 38 s, 0 errors, every harness 0
  differing, 15 of 15 harness lines; 9 ran in the race, 4 in the
  replay `itc_3678` (ok); the busy marker's two to drive). Recorded.

## Done

- `car-makers.md`: 16 of 24 in play native (the record from a car and
  its setters, the garage cars, `FromGarageCar`, the tuning screen's
  set-up, `FitCar`, `CopyRecord`, the busy marker),
  `--native_car_makers`; aca0ef24, 9d2ce651..1276cd9a.
- `power-graph.md`: 3 of 4 in play native (`PowerGraph::Draw`,
  `Line::RecordFlat`, `TuningScreen::DrawPowerGraph`),
  `--native_power_graph`; 64372b27, f97f2857.

## Rows to review

`review.md`, "Run 2026-11-06 11:15".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4230, work left 2181 (wrapped 57, known 458,
  unknown 1666).
- After the power graph's map: ours 4230, work left 2181 (wrapped 57,
  known 462, unknown 1662).
- After its migration: ours 4233, work left 2178 (wrapped 57, known
  459, unknown 1662).
- After its validation (`itc_3663`): ours 4233, work left 2178.
- After the car makers' map: ours 4233, work left 2178 (wrapped 57,
  known 483, unknown 1638).
- After its migration: ours 4249, work left 2162 (wrapped 57, known
  467, unknown 1638).
- After its validation (`itc_3677`): ours 4249, work left 2162.

## Stopped
