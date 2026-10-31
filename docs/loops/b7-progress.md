# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 23:03 on `b7-loop-30-10-26-2`, from ac55473;
  summary `b7-loop-2026-10-30-2303.md`.
- Limit for this run: 3 subsystems; 1 taken.
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

## Done

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

## Stopped
