# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 23:03 on `b7-loop-30-10-26-2`, from ac55473;
  summary `b7-loop-2026-10-30-2303.md`.
- Limit for this run: 3 subsystems; 3 taken.
- Subsystem 1 of 3: the car models (`sub_820D5EC8`...`sub_820ED090`,
  the top of `leads.md`). Already mapped (`car-models.md`, 46 rows in
  play, 27 ours, 19 known); the map checked by the lead, not remapped
  (as `car-specs.md`'s, the seventh run): 0 status errors, ready groups
  (`sub_820D59D8`, `sub_820D0860`, `sub_82295F18`, `sub_820D8BA8`,
  `sub_820D8C50`, `sub_820ED6D0`, `sub_82599C58`, `sub_820C3CE0`).
  Migration 1 checked (9f71b49..233c175: the loads, `LoadExterior`
  with the emblem, decals and vinyl textures, the wheels' set-up, the
  interior's set c body and parts, with `GetLastError` from
  `functions.md` beside SetLastError's, no flag; 4 adapters removed;
  under `--native_car_models`; left: `CarInterior::Load`,
  `CarModel::ReloadDecals`, unmapped `sub_820ED6D0` and `sub_82599C58`
  blocking the file-loaded callbacks). Validated (`itc_2834`, replay
  `itc_2835` ok): 15 of 15 harness lines (`sub_82295F18`, an adapter
  with no plain C++, through `GetLastError`'s shared print); 12 ran in
  the race, 3 in the replay, none to drive. Migration 2 checked
  (27d7e64..9260d11: `CarInterior::Load`, `Skeleton::LocalMatrices`
  (`models.md`, `--native_models`), `CarModel::MakePlate` with its two
  helpers (new rows), the file-loaded callbacks, `ReloadDecals`; 5
  adapters removed; car models 48 of 50 in play ours; the +3724 = 1
  image branch `sub_820C4E68`, found running in an attract race though
  not in the coverage list, left on the originals and made a lead, so
  `Vinyl::DrawDecal`'s adapter stays; the groups left belong to other
  files (`sub_820C4338`'s, those under `sub_820C8DE8`)). Validated
  (`itc_2852`, replay `itc_2853` broke on the intermittent SDK heap
  error, ran to the end): 13 of 13 harness lines; `ReloadDecals` to
  drive. Subsystem 1 finished.

- Subsystem 2 of 3: the part models' material passes
  (`sub_820D0EA8`...`sub_820D3318`; picked as closest to ready, 6 of
  its 9 roots ready, and blocking the rest of the scene loaders).
  Mapped (2da185e, `material-passes.md`, 23 new rows, 22 in play, 15
  ready groups, no stop; nothing outside blocks it; `PaintPalette::Init`
  `sub_820D2330` taken in from the scene's lead). Migration 1 checked
  (579077c..a02c2b2: the color conversions, the small passes,
  `SetMaterials`, the paint passes, `Projector::Matrices` with
  `LookAt`; 1 adapter removed; `--native_material_passes`; frame dumps
  with the flag on and off alike; left: `PaintPalette::Init`, stopped at
  ~five batches). Validated (`itc_2866`, replay `itc_2867` ok): 21 of
  21 harness lines; 20 ran in the race, 1 in the replay, none to drive.
  Migration 2 checked (4795aab: `PaintPalette::Init`, as data copied
  through lfs/stfs as the original; 22 of 22 in play ours; no adapter
  removable yet, nothing else ready). Validated (`itc_2871`): 1 of 1
  harness line, ran in play. Subsystem 2 finished.
- Subsystem 3 of 3: the race's track zone signs, probably a HUD object
  (`sub_822122B0`...`sub_822141A0`; picked as run every race frame,
  its 5 roots all ready). Mapped (ac07446, `race-hud-object.md`, 86
  new rows, 57 in play, 30 ready groups, no stop: the race's HUD at +16
  of the race screens' holder `[0x826D84F0]`; the lead's texture 56
  corrected, the mirror's; blocked in part by the menu sound, two UI
  movie functions, the other in-race screens and the holder's +1004
  object, all leads). Migration 1 checked (003ad8d..6fe8bac: 29
  functions in five batches, the gauges, dashboard, set-ups, signs and
  tick; 12 adapters removed; `--native_race_hud_object`; none of it
  runs in an attract race; 14 ready groups left). Validated
  (`itc_2883`, replay `itc_2884` ok): 29 of 29 harness lines; 24 ran
  in the replay, 5 to drive; the harnesses left 2 s of race in 60.
  Migration 2 checked (653a14e..e4aaf1e: 17 more, the movie calls, the
  HUD's draw (frame dumps alike), the warning, the item popup,
  `RaceScreens::Init` with its +1004 object's maker `sub_82216A60`
  (new row); 3 adapters removed; nothing ready left: the rest calls the
  menu sound, `sub_8211D930`, `sub_8211E370` or the other in-race
  screens). Validated (`itc_2893`, replay `itc_2894` broke on the
  intermittent SDK heap error, ran to the end): 15 of 15 harness lines;
  4 ran in the race, 9 in the replay, 2 to drive; but `App::App` and
  `App::Create`'s harnesses differ 8 of 8 (since 83b2b76): the loop
  stopped.

## Done

- The race HUD object (subsystem 3): `race-hud-object.md`, 45 of 58 in
  play, two migrations; `--native_race_hud_object`; ac07446..e4aaf1e.
  Validated (`itc_2883`, `itc_2893`; the second with the app's harnesses
  differing).

- The material passes (subsystem 2): `material-passes.md`, 22 of 22 in
  play, two migrations; `--native_material_passes`; 2da185e..4795aab.
  Validated (`itc_2866`, `itc_2871`).

- The car models (subsystem 1): `car-models.md`, 48 of 50 in play, two
  migrations, with `GetLastError` and `Skeleton::LocalMatrices`;
  `--native_car_models`; 9f71b49..9260d11. Validated (`itc_2834`,
  `itc_2852`).

## Rows to review

`review.md`, "Run 2026-10-30 23:03".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3423, work left 2974 (wrapped 103, known 487,
  unknown 2384).
- After the car models' migration 1 (`itc_2833`: race 35 s, 0
  errors, car_models and last_error 0 differing): ours 3438, work left
  2959 (wrapped 103, known 472, unknown 2384).
- After its validation (`itc_2834`, 60 s: race 6 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3438, work left
  2959.
- After the car models' migration 2 (`itc_2851`: race 36 s, 0 errors,
  car_models and scene_loaders 0 differing): ours 3447, work left 2950
  (wrapped 103, known 467, unknown 2380).
- After its validation (`itc_2852`, 60 s: race 7 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3447, work left
  2950.
- After the material passes' map: ours 3447, work left 2950 (wrapped
  103, known 489, unknown 2358).
- After the material passes' migration 1 (`itc_2863`: race 38 s, 0
  errors, material_passes 0 differing): ours 3468, work left 2929
  (wrapped 103, known 468, unknown 2358).
- After its validation (`itc_2866`, 60 s: race 7 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3468, work left
  2929.
- After the material passes' migration 2 (`itc_2870`: race 38 s, 0
  errors, material_passes 0 differing): ours 3469, work left 2928
  (wrapped 103, known 467, unknown 2358).
- After its validation (`itc_2871`, 60 s: race 7 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3469, work left
  2928.
- After the race HUD object's map: ours 3469, work left 2928 (wrapped
  103, known 524, unknown 2301).
- After the race HUD object's migration 1 (`itc_2882`: race 32 s, 0
  errors, race_hud_object, track_zones, ui_movies_game and app_start_up
  0 differing): ours 3498, work left 2899 (wrapped 103, known 495,
  unknown 2301).
- After its validation (`itc_2883`, 60 s: race 2 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3498, work left
  2899.
- After the race HUD object's migration 2 (`itc_2892`: race 31 s, 0
  errors, race_hud_object and the touched harnesses 0 differing): ours
  3514, work left 2883 (wrapped 103, known 480, unknown 2300).
- After its validation (`itc_2893`, 60 s: race 2 s, 0 errors, the
  harnesses done in 56.7 s; `App::App` and `App::Create` 8 of 8
  differing): ours 3514, work left 2883.

## Stopped

2026-10-31, at the race HUD object's second validation (`itc_2893`),
the run's limit reached (3 subsystems: the car models, the material
passes, the race HUD object). Halted because two harnesses outside the
subsystem differ: `App::App` (`sub_82094590`) and `App::Create`
(`sub_82094AB8`), 8 of 8 each, at 0 in `itc_2883` before the
migration. Probable cause (the validation agent's reading of the log,
not fixed): 83b2b76 made `operator new`'s adapter (`sub_825A6880`,
`hooks/c_runtime_heap.cpp`) report its calls to the shared harness
recorder `hooks::g_start_up_recorder`, which those harnesses also use;
the recompiled `App::App` and `App::Create` call `operator new` through
the adapter (recorded), their native versions call it directly (not
recorded): 240 entries against 210, 17 against 11. Nothing
uncommitted; `check_function_statuses.py`: 0 errors. Also for the user:
the harnesses took 56.7 of the validation's 60 s (2 s of race), close
to outgrowing it. Left for the user: that harness, the run's section
in `review.md`, `to-drive.md`, and the merge.

Fixed after the stop, at the user's go-ahead (2026-10-31): the
`App::App` and `App::Create` harnesses' recorders let `operator new`
allocate on both sides (`itc_2895`: app_start_up, race_hud_object and
c_runtime_heap 0 differing, race 28 s, no errors). Merged into
`rexglue-removal-project` at the user's call, before a drive.
