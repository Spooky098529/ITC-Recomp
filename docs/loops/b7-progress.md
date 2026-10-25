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
- The user's call (2026-10-25): `_mtinit` (kernel TLS) stays; raise it
  only if it becomes a blocker (`b7-loop.md`'s "Precedent first").
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
  `Texture::Create`. Validated (`itc_1696`: 24 of 24 harness lines, 22
  ran in the attract race, 2 in the replay `itc_1697`, ok; nothing to
  drive). Finished.
- The "no mapping" lead (not counted): `UiMovie::DrawTexts`
  `sub_8211CD90` and what it unblocks (`UiMovie::Update`
  `sub_8211D2F8`), with `sub_82106540` (blocking `_init_pointers`
  `sub_825AA398`). Migration 1 done after six batches (c4141e4..b8b7f8a:
  `DrawTexts` with `ScrollOffset`, `UiMovie::Update`, `UiMovie::Draw`,
  `UiMovie::Run`, `_init_pointers`, `sub_82106540`; 9 adapters removed;
  no new flags; 27 s, 0 errors, every harness 0 differing; the title
  screen's text frame dump identical with the fallback). Left ready:
  `sub_822141E8`, `sub_8211DF20`; `_mtinit` `sub_825AA708` not taken
  (kernel TLS, no precedent). Validated (`itc_1722`: 16 of 16 harness
  lines, 8 prints added in e517b98; replay `itc_1723` ok; 1 to
  `to-drive.md`). Migration 2 done (81a1ede, dc97a73: `UiMovie::Finish`,
  `FinishNamed`, `FinishAll`; `RaceHud::RunMovie` `sub_822141E8` with
  `sub_82212908` Merged, the rival logo's draw `sub_82211A28`, in the
  new `race-hud.md`; `--native_race_hud`; `itc_1726`: 26 s, 0 errors,
  every harness 0 differing). Left: 10 ready groups in
  `ui-movies-game.md` outside this lead (a new "no mapping" lead).
  Validated (`itc_1729`: 5 of 5 harness lines; replay `itc_1730` ok; 1
  to `to-drive.md`). Finished.
- The "no mapping" lead (not counted): the UI movies' 10 ready groups
  left in `ui-movies-game.md` (the constructor and destructor, the named
  animations' walk, the small vectors). Migration done after five
  batches (7d83fb5..ad1dbcb: all 10 groups and the 6 text setters they
  unblocked, 26 rows; no adapter removable yet; no new flag, harness
  group `ui_movie_objects`; 27 s, 0 errors, every harness 0 differing;
  the HUD mirror's replay frame dumps the same with the fallback).
  Nothing ready left; the rest waits on `sub_82108860`, the string
  table's formatting, the archive load's events and the texture set.
  Validated (`itc_1740`: 19 of 19 harness lines, 7 Merged skipped;
  replay `itc_1741` ok; 4 to `to-drive.md`). Finished.

## Done

- The UI movies' ready groups (no mapping; 7d83fb5..ad1dbcb): 26 rows,
  19 plain C++, 7 Merged; validated (`itc_1740`, `itc_1741`);
  `--native_ui_movies_game`.
- `UiMovie::DrawTexts` and what it unblocked (no mapping; two
  migrations, c4141e4..dc97a73): 13 made native, 9 adapters removed, 1
  Merged; `race-hud.md` new (3 rows); validated (`itc_1722`,
  `itc_1729`); `--native_race_hud`. Left: `_mtinit` (kernel TLS).
- The scene models (`scene-models.md`, mapped in 9a490b3; 48 rows): 23
  ours in one migration (8c076d0..88bdea2), 1 Native with no adapter;
  validated (`itc_1696`, `itc_1697`); `--native_scene_models`. Left:
  `SceneModel::SetUp`, `Build`, `Update`, the loaders.

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
- After the scene models' validation: race 27 s (`itc_1696`), 0
  errors, every harness 0 differing; replay 126 s (`itc_1697`), 0
  errors.
- After the UI movies' ready groups' validation: race 23 s (`itc_1740`,
  the harnesses' time), 0 errors, every harness 0 differing; replay
  (`itc_1741`), 0 errors.
- After the UI movies' ready groups' migration: ours 2076, work left
  4375 (wrapped 105, known 582, unknown 3688).
- After the DrawTexts lead's validation 2: race 26 s (`itc_1729`), 0
  errors, every harness 0 differing; replay (`itc_1730`), 0 errors.
- After the DrawTexts lead's migration 2: ours 2050, work left 4401
  (wrapped 105, known 608, unknown 3688).
- After the DrawTexts lead's migration 1: ours 2044, work left 4407
  (wrapped 108, known 609, unknown 3690).
- After the DrawTexts lead's validation: race 27 s (`itc_1722`), 0
  errors, every harness 0 differing; replay 127 s (`itc_1723`), 0
  errors.
