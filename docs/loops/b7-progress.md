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
  same as the fallback. Validation agent running.

## Done

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

## Stopped

(not stopped)
