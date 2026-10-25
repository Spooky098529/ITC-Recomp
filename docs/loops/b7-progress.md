# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-25 16:38 on `b7-loop-25-10-26-3` (the user's
  branch), from c67ef00; summary `b7-loop-2026-10-25-1638.md`.
- Limit for this run: 3 subsystems (the default).
- Subsystem 1 of 3: `leads.md`'s top lead, `sub_820DA180`,
  `sub_820D97B8`, `sub_820D9870` (probably the scene models' update,
  from `sub_820C5A60` for each model in the scene's pool 7). Mapped as
  the scene models (`scene-models.md`, 48 rows, 9a490b3: the set-up
  and build side only; the rest, ~227 functions, in 6 new leads). Map
  checked (0 errors, 48 of 48 in play, 18 ready groups; only kernel use
  `sub_820C3CA0`'s VirtualAlloc, the allocation call). Migration 1 done
  after three batches (8c076d0..88bdea2: all 18 ready groups and 5
  they unblocked, 23 of 48 ours, `sub_820D0A20` Native with no adapter;
  `--native_scene_models`; `itc_1695`: 26 s, 0 errors, every harness 0
  differing). Nothing ready left; the rest waits on the load slots,
  `Archive::Load`, `ScenePools::SetUp`, the asset's size, `sprintf`,
  `Texture::Create`. Step: validation, agent running.

## Done

(none yet)

## Rows to review

This run's rows: `review.md` ("Run 2026-10-25 16:38").

## Expected to differ

This run's groups: `expected-differences.md` (the "16:38 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 2013, work left 4438 (wrapped 110, known 590, unknown 3738).
  `b7_status.py`'s one check at the start: `movies.md:108`
  `sub_82108018`'s row says never called, but it ran (not this run's).
- After the scene models' map: ours 2013, work left 4438 (wrapped 110,
  known 638, unknown 3690).
- After the scene models' migration 1: ours 2037, work left 4414
  (wrapped 110, known 614, unknown 3690).
