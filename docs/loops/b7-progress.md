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
- Subsystem 2 of 3: the course scenery's model lists (`sub_82147FB8`,
  `sub_82147D88` and the others at `82146AA8`-`82148650`), picked as
  closest to ready (5 of its 9 roots ready, 64 functions left under
  them; `Minimap::Draw` waits on them). Mapped as the scenery lists
  (`scenery-lists.md`, 27 rows, a373941: the six lists' load, points
  and release, the draw decisions, `PointList::Draw`, the scenery and
  shared model draws, the texture provider's slots 2 and 4). Map checked
  (0 errors, 27 of 27 in play, 14 ready groups; no kernel calls;
  `Recorder::RecordObject` Wrapped for interpolation, its hooks kept).
  Migration 1 done after five batches (e3295c1..c96cd1b: the lists'
  release and picks, the draw state, the texture provider's slots, the
  visibility test, `RecordObject` inside its interpolation wrapper,
  `RecordSceneLights`, the depth draws, `SceneryModel::DrawShape` and
  `RecordShape`, `PointList::Draw`, and `models.md`'s five accessors
  under `--native_models`; flush-to-zero set off in the draws' adapters,
  59d655c; 1 adapter removed, 2 Native with none, 3 Merged;
  `--native_scenery_lists`; `itc_1768`: 24 s, 0 errors, every harness 0
  differing; frame dumps matched the fallback). Left ready:
  `Model::DrawShape` `sub_82596B18` with `Model::RecordShape`
  `sub_82595550` (1,063 lines). Validated (`itc_1769`: 26 of 26 harness
  lines, 3 Merged skipped; replay `itc_1770` ok; 2 to `to-drive.md`).
  Migration 2 done (343035d: `Model::DrawShape` under
  `--native_scenery_lists`, `Model::RecordShape` under `--native_models`;
  every `Model::DrawShape` call in a race native, from `Scenery::Draw`;
  frame dumps and `--log_blend_state` counts matched the fallback; 24 s,
  0 errors, every harness 0 differing). Nothing ready left: the lists'
  loads wait on `Model::Load`. Validated (`itc_1777`: 2 of 2 harness
  lines; replay `itc_1778` ok; `Model::RecordShape` to `to-drive.md`:
  its work runs inside `Model::DrawShape`, past its print). Finished.
- The user's call (2026-10-25): fallbacks to the original and native
  code calling into `generated/` left as they are for now, to
  investigate later (the functions will be made native anyway).
- Subsystem 3 of 3: the shader objects' set-up under `Mesh::LoadShaders`
  (`sub_822A8740`, `sub_822A85B8`, `sub_82295890`, `sub_82296FE0`),
  picked as all 4 roots ready and blocking `Model::Load` (which blocks
  the scenery lists' loads and the race mode). Mapped as the shader
  objects (`shader-objects.md`, 7 rows, and `XMemCpy` with its line copy
  in `c-runtime.md`; 4047496: the shader creates and inits, the memory
  blocks' `AllocVirtual`, `AllocPhysicalEx` and `PendingFrees::FreeDue`).
  Map checked (0 errors, all in play, 6 ready groups, no kernel imports;
  the memory blocks' lock a host mutex by the user's call). Migration 1
  done after six batches (7bc02b9..840ecff: `XMemCpy` (its step word
  logged as 1), the shader inits and creates, the memory blocks' makers
  and put-off frees, the header copies, the texture and resource
  relocation, `XPR::Create`, `Mesh::LoadShaders`, the skeleton set-up
  and bind, `Mesh::Parse` and `Model::Load`; 3 adapters removed, 2
  Native with none; `--native_shader_objects`; `itc_1794`: 24 s, 0
  errors, every harness 0 differing; race frame dumps the same 2,412
  draws as the fallback). Left ready: `DataFile::GetEntry`
  `sub_82294730`, which blocks `ModelList::Load` and `LoadPoints`.
  Validated (`itc_1795`: 21 of 21 harness lines, all 21 ran in the
  attract race; no replay needed). Migration 2 done after five batches
  (800b20c..49bad70: `DataFile::GetEntry` under the new
  `--native_data_files`, `ModelList::Load` and `LoadPoints`, the course
  set's parsers, pool objects, resets and `AreaModel::~Model`,
  `Strings::Get`, a slot's `XPR::Release`; 4 adapters removed, 3 Native
  with none; `itc_1828`: 24 s, 0 errors, every harness 0 differing;
  `draws.txt` the same as the fallback within one of the attract race's
  two variants, 2,412 or 2,414 draws). Left ready:
  `Scenery::CollectAreas`, `CollectModels`, `DrawSetLists`,
  `Strings::FormatV`. Validated (`itc_1831`: 17 of 17 harness lines,
  15 in the attract race, 2 in the replay `itc_1832`, ok; nothing to
  drive). Migration 3 done (220ea7f..a5141f5: `Scenery::CollectAreas`,
  `CollectModels`, `DrawSetLists`, `Strings::FormatV` (the font number's
  `sub_82108A70`, never seen, left a guest call) and `Strings::Format`;
  8 adapters removed; 21-24 s, 0 errors, every harness 0 differing;
  `DrawSetLists`' dumps the same as the fallback but for the texture
  animations' frames, as two fallback runs differ). Nothing ready left
  in `course-scenery.md` or `strings-and-font.md`. The first validation
  3 was stopped when the user saw the menus run too fast: its replay
  (`itc_1847`, `--skip_intro`) ran unpaced from boot (150-188 ticks/s,
  later over 1,000), as `TitleScreen_SkipTickWait` does until the first
  race is seen, so the race at boot wasn't. Not reproduced: the same
  replay with `--log_race_changes=true` (`itc_1851`) and two boots ran
  at 30. Left to debug later (the user's call). Validated again
  (`itc_1852`: 13 of 13 harness lines; replay `itc_1853` ok, menus at
  30 ticks/s, the race changes logged as expected; 1 to
  `to-drive.md`). Finished.

## Done

- The shader objects (`shader-objects.md`, mapped in 4047496; 7 rows):
  7 ours, and 38 in six other files in three migrations
  (7bc02b9..a5141f5: the chain up to `Model::Load`, `DataFile::GetEntry`
  and what it unblocked), 15 adapters removed; validated (`itc_1795`,
  `itc_1831`, `itc_1852`); `--native_shader_objects`,
  `--native_data_files`.
- The scenery lists (`scenery-lists.md`, mapped in a373941; 27 rows):
  25 ours and 6 in `models.md` in two migrations (e3295c1..343035d), 1
  adapter removed, 2 Native with none, 3 Merged; validated (`itc_1769`,
  `itc_1777`); `--native_scenery_lists`. Left: the lists' loads (on
  `Model::Load`).
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
- After the scenery lists' validation: race 24 s (`itc_1769`), 0
  errors, every harness 0 differing; replay 127 s (`itc_1770`), 0
  errors.
- At the end (after the shader objects' validation 3, `itc_1852`: race
  24 s, 0 errors, every harness 0 differing; replay `itc_1853`): ours
  2152, work left 4299 (wrapped 104, known 540, unknown 3655).
- After the shader objects' migration 3: ours 2152, work left 4299
  (wrapped 104, known 540, unknown 3655).
- After the shader objects' validation 2: race 24 s (`itc_1831`), 0
  errors, every harness 0 differing; replay (`itc_1832`), 0 errors.
- After the shader objects' migration 2: ours 2147, work left 4304
  (wrapped 104, known 545, unknown 3655).
- After the shader objects' validation: race 24 s (`itc_1795`), 0
  errors, every harness 0 differing.
- After the shader objects' migration 1: ours 2130, work left 4321
  (wrapped 104, known 562, unknown 3655).
- After the shader objects' map: ours 2107, work left 4344 (wrapped
  104, known 585, unknown 3655).
- After the scenery lists' migration 2 and validation: ours 2107, work
  left 4344 (wrapped 104, known 576, unknown 3664); race 24 s
  (`itc_1777`), 0 errors, every harness 0 differing; replay 127 s
  (`itc_1778`).
- After the scenery lists' migration 1: ours 2105, work left 4346
  (wrapped 104, known 578, unknown 3664).
- After the scenery lists' map: ours 2076, work left 4375 (wrapped 105,
  known 606, unknown 3664).
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

## Stopped

2026-10-25, at the run's limit: 3 subsystems (the scene models, the
scenery lists, the shader objects; the DrawTexts lead and the UI
movies' ready groups, without mapping, not counted).
`check_function_statuses.py`: 0 errors. Earlier in the run: the first
validation of the shader objects' migration 3 was stopped when the user
saw the menus run too fast (one replay, not reproduced; left for
later).
