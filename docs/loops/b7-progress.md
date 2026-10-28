# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-28 08:36 on `b7-loop-28-10-26` (the user's
  branch), from 4ffc2cd; summary `b7-loop-2026-10-28-0836.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3 done: texture compression (below). Recorded in
  the plan's table and boundary log.
- Now: a "no mapping" lead, not counted: `Vinyl::Compress`
  `sub_820ED090` (`car-models.md`, 110 lines) with its two ready
  callees `sub_820ECF08`, `sub_820ECFC8` (204 lines in all), then
  `XGCompressSurface`'s adapter, which only it calls. Migration done
  (689b0c8, 53ea064): the three ours under `--native_car_models`
  (adapters stay: recompiled callers in play), `XGCompressSurface`
  Native; `itc_2106`: 19 s, 0 errors, every harness 0 differing but
  the expected groups; frame dumps the same as the fallback. Left
  under `CarModel::OnDecalLoaded` `sub_820D5750`: 7 ready groups (the
  sheet's software rasterizer). Validated (`itc_2107`: 4 of 4
  harness lines, all 4 in the attract race; nothing to drive).
- Now: another "no mapping" lead, not counted: the vinyl sheet's
  decal path under `CarModel::OnDecalLoaded` `sub_820D5750`
  (`car-models.md`; 19 functions, 2,257 lines, 7 ready groups: the
  software rasterizer `sub_820EE638` (1,246 lines) and the rest; only
  `sub_820EED38` and `sub_820EED58` without rows). Migration done in
  four batches (de91953..c9f00d1): all 19 ours under
  `--native_car_models`, `Vinyl::Compress` Native; `itc_2121`: 19 s,
  0 errors, every harness 0 differing but the expected groups (one
  new, `OnDecalLoaded`'s headers' unwritten bytes); frame dumps the
  same as the fallback. Validated (`itc_2122`): 19 of 20 harness
  lines, all 20 in the attract race; `Vinyl::ClearCorner`
  `sub_820D3C80` missing (its harness doesn't reach it): halted.

- Resumed 2026-10-28 by the user's call after the stop below: the
  harness fixed (9dc3c72; `OnDecalLoaded`'s compress cases now reach
  `ClearCorner` and the compress, 3 of 16 compressing; a double free in
  the harness's cleanup they exposed fixed; `itc_2126`: 17 s, 0
  errors, every harness 0 differing but the expected groups, the
  harnesses ~1 s longer).
- Subsystem 2 of 3: `leads.md`'s top lead, `sub_82170550`,
  `sub_82170668`, `sub_82171250`, `sub_82165E20`, `sub_821662E0`,
  `sub_82173A90`, `sub_82174BC0` (probably the race's states and
  manager, the callers of `TrackCollision::Load`, `ResetWorld`,
  `Unload`, `Tick`; 16+ ready groups under them). Mapped as the race
  world (`race-world.md`, 54 rows, all new, 45 in play: the race
  mode's state machine at +116812, the racers' manager at
  `[0x826D82F4]` with its streaming of vehicles by distance, the
  start placements, the save's distance on unload; 5b6d76c; 1 lead
  added). Map checked (0 errors, 22 ready groups, blocked outside by
  `TrackCollision::Tick` `sub_820FF050` (ready) and the archive's
  `sub_8209DE48`, `sub_820A0678`; no kernel calls). Migration 1 done
  (4824fa1..1472716, four batches): the leaves, the manager's tables,
  the vehicles' tick with `TrackCollision::Tick` (`collision-world.md`),
  `RaceWorld::Init`; 24 ours in `race-world.md`, 2 adapters removed;
  `--native_race_world`; `itc_2138`: 17 s (the harnesses 21 s), 0
  errors, every harness 0 differing but the expected groups; frame
  dumps the same as the fallback (`itc_2133`, `itc_2134`). Left ready:
  `ResetWorld`, `ResetVehicles`, `Unload`, the event's start
  placements, `RaceStart::Place`, `AddRacers`, `ListRacers` and the
  big ticks (split). Validated (`itc_2139`, replay `itc_2140` ok: 24
  of 24 harness lines, 13 in the attract race, 8 in the replay, 3 to
  drive). Migration 2 done (1be0160..b3013b9, five batches, the tick
  tree split in three; subagents one at a time): `ResetWorld`,
  `ResetVehicles`, `Unload`, the event's start placements,
  `ListRacers`, `AddRacers`, `OnLoaded`, `RaceStart::Place` and
  `PlaceRolling`, the vehicle streaming and `RaceWorld::Tick`; 48 of
  50 in play ours, 25 Native; `itc_2202`: 9 s (the harnesses 29 s,
  `race_world`'s 8.3 s of it), 0 errors, every harness 0 differing but
  the expected groups (one new); frame dumps the same as the fallback
  (`itc_2197`, `itc_2198`). Left ready: `RaceManager::Refresh`
  `sub_82171978`. Validated (`itc_2203`, replay `itc_2204` ok: 33 of
  33 harness lines, 14 in the attract race, 13 in the replay, 6 to
  drive). Migration 3 (981658f): `RaceManager::Refresh`; validated
  (`itc_2206`, replay `itc_2207`: ran in the replay). Finished: 49 of
  50 in play ours; recorded in the plan's table and boundary log.

## Done

- The race world (`race-world.md`, mapped in 5b6d76c; 54 rows, 50 in
  play): 49 ours in three migrations (4824fa1..981658f), 25 adapters
  removed; `TrackCollision::Tick` (`collision-world.md`); validated
  (`itc_2139`/`2140`, `itc_2203`/`2204`, `itc_2206`/`2207`; 8 to
  drive); `--native_race_world`.

- The vinyl sheet's decal path (no mapping; de91953..c9f00d1):
  `car-models.md` 27 of 46 ours under `--native_car_models`,
  `Vinyl::Compress` Native; validation (`itc_2122`) found
  `ClearCorner`'s harness gap.

- `Vinyl::Compress` (no mapping; 689b0c8, 53ea064): `car-models.md`
  3 more ours under `--native_car_models`, `XGCompressSurface` Native;
  validated (`itc_2107`).

- Texture compression (`texture-compression.md`, mapped in f1c484c;
  157 rows, 24 in play): 24 ours in one migration (cdf8494..af2c1ee),
  18 adapters removed; validated (`itc_2099`, all 24 in the attract
  race); `--native_texture_compression`.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-28 08:36").

## Expected to differ

This run's groups: `expected-differences.md` (the "08:36 run" lines).

## To check in a drive

`to-drive.md` (this run's heading, when it has any).

## Totals

- At the start: ours 2400, work left 4052 (wrapped 103, known 522,
  unknown 3427).
- After the texture compression's map: ours 2400, work left 4052
  (wrapped 103, known 546, unknown 3403).
- After the texture compression's migration: ours 2424, work left
  4028 (wrapped 103, known 522, unknown 3403).
- After its validation: race 19 s (`itc_2099`), 0 errors, every
  harness 0 differing but the expected groups; no replay needed.
- After `Vinyl::Compress`'s migration: ours 2427, work left 4025
  (wrapped 103, known 520, unknown 3402).
- After the decal path's migration: ours 2447, work left 4006
  (wrapped 103, known 503, unknown 3400).
- At the end (after the decal path's validation, `itc_2122`: race 19
  s, 0 errors, every harness 0 differing but the expected groups):
  ours 2447, work left 4006 (wrapped 103, known 503, unknown 3400).
- After the race world's map: ours 2447, work left 4006 (wrapped 103,
  known 548, unknown 3355).
- After the race world's migration 1: ours 2472, work left 3982
  (wrapped 103, known 524, unknown 3355).
- After the race world's migration 2: ours 2496, work left 3962
  (wrapped 103, known 504, unknown 3355).
- After the race world (finished): ours 2497, work left 3961 (wrapped
  103, known 503, unknown 3355).

## Stopped

(resumed; see "Now") 2026-10-28, by "When to halt": the decal path's validation agent
reported `Vinyl::ClearCorner` `sub_820D3C80` `missing:` (no `native
running (harness):` line; it ran in play). `CarModel::OnDecalLoaded`'s
compress cases (`c % 6 == 1` in `DecalLoadCases`,
`src/harnesses/car_models.cpp`) give the slots not yet arrived a random
state with `r.Below(3)`, so a slot is almost always loading and it
returns before `ClearCorner`. To fix: compress cases with no slot
loading, or a `ClearCorner` case of its own. 1 subsystem of 3 done
(texture compression), with two leads without mapping (the vinyl
sheet). `check_function_statuses.py`: 0 errors. Nothing for
`to-drive.md`.
