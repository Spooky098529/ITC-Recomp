# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-13 on `b7-loop-13-11-26` (made by the lead at
  the user's call, from `rexglue-removal-project`), from 59e05585;
  summary `b7-loop-2026-11-13-1231.md`. Limit: 10 subsystems (the
  user's call for this run).
- First: two leads without mapping (not counted toward the limit):
  `RaceStates::OnRaceEnd` `sub_821644D8` and the adapter fix of
  `CarList::DrawEntryPowerGraph` `sub_82250670`; their lines deleted
  from `leads.md`. Migrated 98f4fe26..52b87de7: `DrawEntryPowerGraph`'s
  r31 sign-extended (8 new cases, differing before the fix);
  `OnRaceEnd` native under `--native_race_states` with its 4
  never-seen callees and theirs ported (the course record, `Time::Add`;
  by the user's 2026-11-12 call to minimize recompiled code), 5
  adapters removed (`RacerRecords::Add`, `Stats::AddRaceResult`,
  `Achievements::Check`, `Write`, `Replay::Keep`); every harness 0
  differing, race 37 s at 40, 0 errors; no subagents. Validated:
  `itc_5235` (race 50 s at 180, 0 errors, every harness 0 differing
  outside the expected groups), 15 of 15 harness lines, no prints
  added; none ran in the race or the replay `itc_5236` (ok): 15 to
  drive. Recorded.
- Subsystem 1 of 10: the morph jobs (`sub_820ACC08` and the
  `820AC998`-`820ACC08` vtable slots: the 224-byte job the second
  worker pool runs for `Model::MorphShape`, the wheels' per-draw
  morphs; ready, run every frame); its line deleted from `leads.md`.
  Mapped `morph-jobs.md` (3b635fc5: 21 rows, 14 in play, with the
  shared 36-byte work item class at vtable `0x820066DC`; 13 ready
  groups; the pool's post `sub_82095A28` a guest call by the
  `DebugText`/`FramePasses` precedent; no leads added). Check passed.
  Migrated 752011b3..63bc5ef5: all 21 rows ours (13 ready groups and 7
  never-seen functions in one batch), `--native_morph_jobs`;
  `Model::MorphShape` calls the maker's plain C++; `Morph::Blend`'s
  adapter removed; 19 adapters kept (vtable slots); two harness faults
  fixed on the way (r3's high word); every harness 0 differing, race
  33 s at 40, 0 errors; no subagents. Validated: `itc_5244` (race
  50 s at 180, 0 errors, every harness 0 differing outside the
  expected groups), 22 of 22 harness lines, 3 prints added (3123f921);
  the work item's slots and `Morph::Blend` ran in the race, none in
  the replay `itc_5245` (ok): 15 to drive. Recorded; subsystem 1
  done.
- Subsystem 2 of 10: the save devices' constructor (`sub_820A6160`,
  `sub_820A4560`, run at boot, ready); its line deleted from
  `leads.md`. Mapped into `save-interface.md` (1edd6e03: 5 new rows,
  2 in play; 1 ready group; the listener stored as
  `user::kNoListener` with no call, the 2026-10-30 precedent; 1 lead
  added, the slot handlers). Check passed. Migrated f0ecffdd..32f5b0d9:
  all 5 rows ours (3 never-seen ported), with `SaveDevices::Create`
  (`framework.md`) so the constructor's adapter could go; under
  `--native_save_interface`; every harness 0 differing, race 38 s at
  40, 0 errors; the three ran in the game at boot; no subagents.
  Validated: `itc_5250` (race 49 s at 180, 0 errors, every harness 0
  differing outside the expected groups), 6 of 6 harness lines, no
  prints added; the constructor, `AllocBuffers` and `Create` ran in
  play, the teardown's 3 to drive (replay `itc_5251` ok). Recorded;
  subsystem 2 done.
- Subsystem 3 of 10: the save devices' slot handlers (the lead the
  save devices' map added: the states `SaveDevices::Update` runs every
  frame; 3 ready, state 6 blocked by `sub_82295C98` and
  `sub_822970A0`); its line deleted from `leads.md`. Mapped into
  `save-interface.md` (2c70e8c8: 14 new rows, shared rows updated in
  `files-and-saves.md` and `system-services.md`; 6 ready groups; the
  overlapped-result waits `sub_82295C98`, `sub_82295F20` to be made
  native with their never-taken wait path a guest call to
  `WaitForSingleObject`, the 2026-10-24 and 2026-10-29 precedents;
  B4's `File::`/content steps in `src/hooks/saves.cpp` to be split
  into plain C++; no leads added). Check passed. Migrated
  b41c696e..796dcddf: the 14 rows, `SaveStep`, `LoadStep`, both
  overlapped results (adapters removed), `Framework::DeviceList` and
  `SaveDevices::Update`'s plain C++ (B5's adapter kept), B4's 13 steps
  split into plain C++; under `--native_save_interface`; every harness
  0 differing, race 38 s at 40, 0 errors; no subagents. Validated:
  `itc_5258` (race 50 s at 180, 0 errors, every harness 0 differing
  outside the expected groups), 33 of 33 harness lines, 13 prints
  added to B4's steps (319af0df); 5 ran in the race, 20 more in the
  replay `itc_5259` (ok), 8 to drive. Recorded; subsystem 3 done.
- Subsystem 4 of 10: three small ready leads taken together (each a
  few functions, seen in play): the render manager's slot 8
  (`sub_820944D8`, `sub_82094338`), the texture provider's getter
  (`sub_825946B8`, `sub_82594648`) and the static zero matrix at boot
  (`sub_825D8F88`, `sub_82090000`); their lines deleted from
  `leads.md`. Mapped (1572447f: 13 new rows in `app-start-up.md`,
  `models.md` and `matrix-math.md`, with the render manager's slot 7,
  the provider's slots 5-10 and the identity matrix's set-up
  `sub_825D9060`; 6 ready groups; no leads added). Check passed.
  Migrated 87dda153..ee595da5: 13 of 13 ours (3 not seen, ported),
  `Matrix::FromFloats`' adapter removed (`camera-modes.md`), under the
  existing `--native_app_start_up`, `--native_models`,
  `--native_matrix_math`; every harness 0 differing, race 38 s at 40,
  0 errors; a cleanup lead added to `leads.md` (native callers'
  guest calls into these slots); no subagents. Validated: `itc_5265`
  (race 49 s at 180, 0 errors, every harness 0 differing outside the
  expected groups), 14 of 14 harness lines, no prints added; 10 ran in
  the race, 4 to drive (replay `itc_5266` ok). Recorded; subsystem 4
  done.
- Next: the cleanup lead the migration added (no mapping, not
  counted): native callers' guest calls into the render manager's
  slots and the texture provider's getters made direct calls; its line
  deleted from `leads.md`. Migrated 4577728d..bc20b2d4: ~30 native
  callers in 16 subsystems call the plain C++ directly when the vtable
  holds the known target (`hooks/direct_slots.h`), guest call
  otherwise; two new harness groups for the direct paths; every harness
  0 differing, races 38, 24, 38, 23 s at 40 (the short ones the
  harnesses' time), 0 errors; no adapters removable; no subagents.
  Validated: `itc_5274` (race 49 s at 180, 0 errors, every harness 0
  differing outside the expected groups), 4 of 4 harness lines, all
  four ran in the race. Recorded.
- Subsystem 5 of 10: the sticker editor's input pages (the tuning
  screen's states 38 to 46 and 49, `tuning-pages.md`; none seen in
  play, ported by the 2026-11-12 call; the previous run's "Next"); its
  line deleted from `leads.md`. Mapped `sticker-editor.md` (680da961:
  25 rows, none in play, so `b7_status.py` doesn't list it, as for the
  never-seen files of the last run; 23 ready groups judged by reading
  their callees, `UiMovie::Unloop` `sub_8211DE28` to port with them;
  the update's sticker commands 32 to 51 ready after; no leads added).
  Check passed. Migrated eb20ef60..d84fa8b8: 25 of 25 ours (13
  Native), with `UiMovie::Unloop`; `HandleInput` calls the pages'
  plain C++; under `--native_tuning_pages`; every harness 0 differing,
  race 31 s at 40 (the harnesses' time), 0 errors; a harness fault
  fixed on the way (`InputStickerView`'s held buttons moved to the
  stand world); two subagents, one at a time. Left: the update's
  sticker commands 32 to 51 (ready, a second migration). Validated:
  `itc_5289` (race 49 s at 180, 0 errors, every harness 0 differing
  outside the expected groups), 26 of 26 harness lines, 1 print added
  (3d4fc53f); none ran in the race or the replay `itc_5290` (ok): 26 to
  drive. Second migration 662861e6..7fbeadd2: the sticker commands 32
  to 51 (17 functions, `tuning-screen.md`) native, called by the
  update's plain C++; the last 12 helper adapters removed (all 25
  rows Native); every harness 0 differing, race 29 s at 40 (the
  harnesses' time), 0 errors; no subagents. Validated: `itc_5299`
  (race 49 s at 180, 0 errors, every harness 0 differing outside the
  expected groups), 29 of 29 harness lines, no prints added; none ran
  in the race or the replay `itc_5300` (ok): 17 more to drive.
  Recorded; subsystem 5 done.
- Subsystem 6 of 10: the stand's setters the tuning screen reaches
  (`sub_8217B078` to `sub_8217B3D8`, `sub_8217DBC0`; none seen in
  play; `UpdateStand`'s `ApplyAreaDecals` waits on `sub_8217B3D8`); its
  line deleted from `leads.md`. Mapped into `stand-models.md`
  (0ced7d4b: 8 new rows and `SetDecalWord` completed, none in play; 9
  ready groups judged by reading their callees; with them
  `SetColor`, `ApplyAreaDecals`, `ApplyDecalWord`, `UpdateStand` and
  `PreviewParts` in `tuning-screen.md` become ready; no leads added).
  Check passed. Migrated 8d0e0496..0e74c60a: the 9 setters,
  `SetColor`, `ApplyAreaDecals`, `ApplyDecalWord`, `PreviewParts`,
  `SetParts` native (`UpdateStand` calls them), 17 adapters removed
  (13 more functions Native); under `--native_stand_models`; every
  harness 0 differing, races 33 s at 40 and 84 s at 90, 0 errors; a
  harness fault fixed on the way (new groups advanced the shared
  random generator); no subagents. Validated: `itc_5306` (race 47 s
  at 180, the harnesses ~130 s of it; 0 errors, every harness 0
  differing outside the expected groups), 31 of 31 harness lines, no
  prints added; 6 ran in the replay `itc_5307` (ok), 25 to drive.
  Recorded; subsystem 6 done.
- Next: a lead without mapping (not counted): the 22 never-seen rows
  left without Status in `stand-models.md` and `tuning-screen.md`
  (`SetChoice`, `SetAero2`, `SetDoorKind`, `ClearLightKit`,
  `SetAccessory`, `SetCaliper`, `RefitAxles`,
  `PreviewInteriorChoices`, the swatch setter and the rest), by the
  2026-11-12 call. Migrated abc17bc2..5db16791: 18 functions native
  (with `SceneInterior::SetRecord`, a new row, and the refresh's
  `RefreshPaint`, `RefreshLightKits`, `RefreshMovie14`); no adapters
  (callers native); `RefreshInterior` and commands 10, 13, 14 blocked
  by four `tuning-pages.md` functions (a lead added, with one for
  `MakeModel`'s and `FitSelectedCar`'s guest callers); every harness 0
  differing, race 33 s at 40, 0 errors; no subagents. Validated:
  `itc_5316` (race 48 s at 180, the harnesses ~130 s; 0 errors, every
  harness 0 differing outside the expected groups), 19 of 19 harness
  lines; none in the race or the replay `itc_5317` (ok): 19 to drive.
  Recorded.
- Next: two leads without mapping (not counted), one migration agent:
  the four `tuning-pages.md` blockers (`IsSlotAvailable`,
  `SetPaintGradients`, `SetLightKind0Bars`, `SetLightKind1Bar`) with
  what they unblock (`RefreshInterior`, commands 10, 13, 14), and
  `MakeModel`'s and `FitSelectedCar`'s native guest callers made
  direct; their lines deleted from `leads.md`. Migrated
  bc2ed8af..0f0b959d: the four blockers, `RefreshInterior` and commands
  10, 13, 14 Native (no adapters); `MakeModel`'s and
  `FitSelectedCar`'s callers direct, both adapters removed (Native);
  every harness 0 differing, races 29 and 23 s at 40 (the harnesses'
  time), 0 errors; a lead added (`sub_82208738`'s adapter); no
  subagents. Validation (`itc_5322`, race 48 s, 0 errors): 10 of 10
  harness lines, but `TuningMachine::Start`'s harness (tuning_machine,
  which the migration didn't run) differs in 40 of 40 cases, one byte
  (recompiled 01, native 00 at entries 47, 51, 55, 59). By "Precedent
  first" (harness faults found with logging, fixed, validated again; a
  real difference halts), the migration agent diagnosed it: a harness
  fault (the dead register saves the original `FitSelectedCar` writes
  under `PickCameraPreset`'s frame, left out of `tuning_refresh` in
  0f0b959d but not of `tuning_machine`'s copy); fixed edb5225e, 40 of
  40 at 0 differing. Validated again: `itc_5325` (race 48 s at 180,
  the harnesses ~130 s; 0 errors, every harness 0 differing outside
  the expected groups), 10 of 10 harness lines; 8 to drive. Recorded.
- Subsystem 7 of 10: the type 6 and 7 modes' small never-seen pieces
  (the controller pages' callees `sub_821BB290`, `sub_821BB3B8`,
  `sub_821BB590`, `sub_821C0238`; the small functions beside the group
  pair and the car list `sub_82249BE8` and the rest), with the adapter
  lead `sub_82208738` for its migration; their lines deleted from
  `leads.md`. Mapped (707fe4ba: 16 new rows in `type6-type7-modes.md`,
  `type67-screens.md`, `car-list.md`, none in play; 10 ready groups
  judged by reading their callees; six destructors wait on three small
  recompiled blockers to take along; 2 leads added). Check passed.
  Migrated f83e3ee2..6cb9126c: all 16 rows and the 3 blockers
  ours (adapters kept: their other callers are recompiled or vtables),
  `DecalImages::UnloadAll` `sub_82208738`'s adapter removed (Native);
  every harness 0 differing, races 30-38 s at 40, 0 errors; no
  subagents. Validated: `itc_5333` (race 47 s at 180, the harnesses
  ~131 s; 0 errors, every harness 0 differing outside the expected
  groups), 20 of 20 harness lines; `UnloadAll` ran in the replay
  `itc_5334` (ok), 19 to drive. Recorded; subsystem 7 done.
- Subsystem 8 of 10: the type 6 and 7 list screens' and node screen's
  pages and steps (called through tables built on first use,
  `type67-screens.md`), with the screen blocks' probable destructors
  `sub_822367C0`, `sub_82246BE0`; none seen in play; their lines
  deleted from `leads.md`. Mapped (e3d8b9a9: 77 new rows and 25
  changed in `type67-screens.md`, none in play; one ready piece of ~175
  functions with the file's 97 status-less rows, judged by reading
  callees; `CourseRecords::Of` `sub_82105BF8` to take along; 1 lead
  added, the type 8 mode's list screens). Check passed. Migration
  part 1 a15c7b09..87d4c82e: 102 rows ours (101 Overridden, `AddRacers`
  Merged), five batches by subagents one at a time; every harness 0
  differing but one new expected group (`Type6ListScreen::SetUpRace`'s
  traffic records' unwritten bytes, 11 of 16, as `Race::SetUp`'s;
  `expected-differences.md`); races 34-38 s at 40, 0 errors. Left: 72
  rows (the color list, `Type7ListScreen`, the node screen, the panel
  leftovers, the screen blocks' destructors). Validation agent running
  on part 1.

## Done

- The type 6 and 7 modes' small pieces: 16 rows and 3 blockers ours,
  `UnloadAll` Native, f83e3ee2..6cb9126c; validated.

- Cleanup leads (no mapping): the tuning pages' blockers with
  `RefreshInterior` and commands 10, 13, 14; `MakeModel` and
  `FitSelectedCar` direct, bc2ed8af..edb5225e; validated (a harness
  fault fixed).

- Cleanup lead (no mapping): the stand's and tuning screen's never-seen
  rows, 18 Native, abc17bc2..5db16791; validated.

- The stand's setters: 9 rows in `stand-models.md` and 5 callers in
  `tuning-screen.md` ours, 17 adapters removed, 8d0e0496..0e74c60a;
  validated.

- The sticker editor: `sticker-editor.md` 25 of 25 Native, with the
  sticker commands 32 to 51 and `UiMovie::Unloop`, under
  `--native_tuning_pages` and `--native_tuning_screen`,
  eb20ef60..7fbeadd2; validated.

- Cleanup lead (no mapping): direct calls to the render manager's and
  texture provider's slots from ~30 native callers,
  4577728d..bc20b2d4; validated.

- Three small leads: 13 rows in `app-start-up.md`, `models.md`,
  `matrix-math.md` ours, `Matrix::FromFloats` Native,
  87dda153..ee595da5; validated.

- The save devices' slot handlers: 14 rows in `save-interface.md` ours,
  with `SaveDevices::Update` and 5 shared, B4's steps split, under
  `--native_save_interface`, b41c696e..319af0df; validated.

- The save devices' constructor: 5 rows in `save-interface.md` ours,
  with `SaveDevices::Create`, under `--native_save_interface`,
  f0ecffdd..32f5b0d9; validated.

- The morph jobs: `morph-jobs.md` 21 of 21 ours,
  `--native_morph_jobs`, 752011b3..3123f921; validated.

- Cleanup leads (no mapping): `RaceStates::OnRaceEnd` with the course
  record (`race-states.md`), `DrawEntryPowerGraph`'s r31
  (`power-graph.md`), 5 adapters removed, 98f4fe26..52b87de7;
  validated.

## Rows to review

The run's section of `review.md` ("Run 2026-11-13 12:31").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5989, work left 718 (wrapped 51, known 361,
  unknown 306).
- After the cleanup leads: ours 5998, work left 717 (wrapped 51,
  known 360, unknown 306).
- After the morph jobs' map: ours 5998, work left 717 (wrapped 51,
  known 374, unknown 292).
- After the morph jobs: ours 6019, work left 703 (wrapped 51, known
  360, unknown 292).
- After the save devices' map: ours 6019, work left 703 (wrapped 51,
  known 362, unknown 290).
- After the save devices: ours 6025, work left 700 (wrapped 51, known
  359, unknown 290).
- After the slot handlers: ours 6044, work left 686 (wrapped 51, known
  354, unknown 281).
- After the three small leads: ours 6057, work left 676 (wrapped 51,
  known 354, unknown 271).
- After the sticker editor: ours 6100, work left 676 (wrapped 51,
  known 354, unknown 271).
- After the stand's setters: ours 6114, work left 676.
- After the type 6 and 7 small pieces: ours 6160, work left 676.

## Stopped
