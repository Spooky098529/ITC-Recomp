# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-08 on `b7-loop-08-11-26` (made by the user for
  this run), from 2bf66687; summary `b7-loop-2026-11-08-1300.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the UI movie helpers without rows (two leads taken
  together: `sub_8211BC18`, whether an animation is running or queued,
  which 19 of the car list's groups wait on, the top recommendation of
  the last run; and `sub_8211CC50`, the node text block's clear beside
  the UI movie text calls; both lines deleted from `leads.md`). Mapped
  into the existing `ui-movies-game.md` (9f12739f: 8 new rows, 3 in
  play, `sub_8211BC18`, `sub_8211DE30`, `sub_8211CC50`, each a ready
  group; 5 never run; no leads added; no subagents); map checked.
  Migration e052912e..e73bf7db (the three native under
  `--native_ui_movies_game`, with `sub_8211DCE8`'s adapter removed;
  cleanup in `car-list.md` they unblocked: the panels' answers, shows,
  selections and moves, eight steps, `SetUpRows`, `SetUpCarPage`, under
  `--native_car_list`; 22 adapters removed; resumed once after an API
  sign-in error cut it off mid-batch; race 36 s, 0 errors, its
  harnesses 0 differing; no subagents). Left: `Step17` `sub_8224C450`
  and `CarPage` `sub_8224B8D8` (need a harness world with the stand's
  car), and `sub_8211E500`, `sub_8211E2C8`, `sub_8211E370`,
  `sub_8211CBD8`, `sub_8211CA18` in `ui-movies-game.md`, ready, not
  taken. Validated `itc_4536` (race 85 s at 180; 0 errors, every
  harness 0 differing outside the expected groups, 34 of 34 harness
  lines, no prints added; `sub_8211DCE8` in the race, none more in the
  replay `itc_4537` (ok), 33 to drive). Second migration
  f2cce057..97ac1b9f (`UiMovies::DeleteAll`, `SetNodeTextV`,
  `SetNodeNumberText`, `OnLoaded` with `File::GetSize`
  (`--native_file_layer`); the car list's `Step17` and `CarPage` with a
  stand's car harness world (`harnesses/stand_car_world.h`); 12
  adapters removed; race 35 s, 0 errors, its harnesses 0 differing; no
  subagents). Left: none in either file; newly ready in other files:
  `sub_82211B98`, `sub_82246038`, `sub_82218838`, `sub_82090CC0` (with
  `sub_8258BD58`, `sub_8258BE48`), and 12 callers without rows. Step:
  its validation. Validated `itc_4552` (race 85 s at 180; 0 errors,
  every harness 0 differing outside the expected groups, 19 of 19
  harness lines; a print added for `sub_8211E370`, c87035a6; 4 in the
  race, 1 more in the replay `itc_4553` (ok), 14 to drive). Recorded.
- Cleanup lead (no mapping, not counted): the groups the UI movie
  helpers made ready in mapped files: `RaceScreens::~RaceScreens`
  `sub_82211B98` (`race-hud-object.md`), `StandMode::Release`
  `sub_82246038` (`type6-type7-modes.md`), `RaceDialog::SetText`
  `sub_82218838` (`race-screens.md`), the debug font loader
  `sub_82090CC0` with `sub_8258BD58`, `sub_8258BE48`
  (`files-and-saves.md`). Its callers without rows went to `leads.md`.
  Migration 9ca12391..c33350d7 (the four, with `DebugFont::Init`,
  `sub_8258BE48` Merged, and what they made ready: `RaceScreens::Destroy`,
  `RaceDialog::Open`; 11 adapters removed: `RaceHud::TearDown`,
  `AltHud::Destroy`, `~RaceScreens`, the eight in-race screens'
  Destroys; `File::GetSize` keeps its adapter, called through vtables;
  races 26-38 s, 0 errors, its harnesses 0 differing outside the known
  `Type6Mode::Init` group; no subagents). Left: `sub_8216C208`
  (`race-hud-machine.md`) and `sub_82218C10` (`race-screens.md`), ready.
  Validated `itc_4564` (race 85 s at 180; 0 errors, every harness 0
  differing outside the expected groups, 17 of 17 harness lines, no
  prints added; 14 in the race, 2 more in the replay `itc_4565` (ok), 1
  to drive). Second migration cd0cb488..2c188a95 (`DestroyScreens`,
  `ShowMessage`, its message 0-11 first calls left on the original,
  their callees never ran; `RaceScreens::Destroy` and `SetText`
  adapters removed; a harness gap fixed: `PauseMenu::Draw`'s set-up
  cleared the pause list's items on every case once seeds moved; races
  28-34 s, 0 errors, its harnesses 0 differing; no subagents). Left
  from these: none (other ready groups in `race-screens.md` and
  `race-hud-machine.md`, probably freed by `SetNodeTextV`). Validated
  `itc_4571` (race 85 s at 180; 0 errors, every harness 0 differing
  outside the expected groups, 4 of 4 harness lines; 2 in the race, 2 in
  the replay `itc_4572` (ok), none to drive). Step: third migration (the
  ready groups left in `race-screens.md` and `race-hud-machine.md`:
  `sub_8220D898`, `sub_8221A4A8`, `sub_8216E5B0`, `sub_8216CC60`,
  `sub_8220E418`, `sub_8220E810`, `sub_8220E908`). Migration
  def0337f..53434c3a (the pause menu's main and adjust pages and set-up,
  the menu after the race's main page and `Open`, the race HUD's mode,
  timer and racers set-ups, `StartCountdown`, `OnRaceEnd` with
  `AwardPrize` and the career results; 18 adapters removed; two batches
  by subagents, one at a time; races 27-33 s, 0 errors, its harnesses 0
  differing). Left: the pause menu's transitions they made ready
  (`sub_8220E988`, `sub_8220F3C0`, `sub_8220F500`, `sub_8220ED90`,
  `sub_8220EFA8`, `sub_8220F6D0`, `sub_8220F0A0`, `sub_8220F788`,
  `sub_8220F2A8`). Validated `itc_4603` (race 83 s at 180, the
  harnesses ~96 s; 0 errors, every harness 0 differing outside the
  expected groups, 26 of 26 harness lines; none in the race, 7 in the
  replay `itc_4604` (ok, a free start: no countdown), 19 to drive).
  Fourth migration 6709c224..f6ad7444 (the pause menu's transitions,
  volume and adjust pages' set-ups, `VolumePage` with `SaveVolumes`,
  never seen, ported; 9 adapters removed after counter runs; race 32 s,
  0 errors, its harnesses 0 differing; no subagents). Left: none (the
  race screens' rows without a status never ran). Validated
  `itc_4610` (race 82 s at 180; 0 errors, every harness 0 differing
  outside the expected groups, 17 of 17 harness lines; 11 in the replay
  `itc_4611` (ok), 6 to drive). Recorded.
- Subsystem 2 of 3: the UI movie helpers' ready callers without rows
  (`sub_82196EC0`, `sub_82224AB8`, `sub_8222FB90`, `sub_8225D590`,
  `sub_821C0BF0`, `sub_821C0CD0`, `sub_821C0D70`, `sub_821A51C8`,
  `sub_821A52C0`, `sub_821A5360`, `sub_821821C8`, `sub_821827E0`; ready
  since this run's first subsystem; its line deleted from `leads.md`).
  Mapped `front-end-screen-helpers.md` (200c6832: 34 rows, 32 in play;
  the modes' releases (types 2, 4, 5, 9, 10, 12), the save and options
  dialogs' text setters with the steps they unblocked, the car cards;
  15 ready groups; 1 lead added, four leads' lines trimmed; no
  subagents); map checked. Migration 92fe4c38..99d3bf29 (the 15
  groups and what they made ready: the save screen's starts and dialog
  steps, the options' save and category 6 pages and dialogs, the car
  cards' fill; 27 of 32 ours, 5 Merged case-0 tails; 6 adapters
  removed; `--native_front_end_screen_helpers`; race 36 s, 0 errors,
  its harnesses 0 differing; no subagents). Left: none. Validated
  `itc_4619` (race 81 s at 180; 0 errors, every harness 0 differing
  outside the expected groups, 27 of 27 harness lines; 1 in the race,
  10 in the replay `itc_4620` (ok), 16 to drive). Recorded.
- Subsystem 3 of 3: the front-end screen with the car cards
  (`sub_82183DE0`, `sub_82184488`, `sub_82182DD0`, `sub_82185920`; the
  lead this run's second map added, the closest to ready: two roots
  ready, the others with ready groups under them; its line deleted from
  `leads.md`). Mapped `car-dealer.md` (c96dfffd: 75 rows, 57 in play:
  the car dealer, one of the type 2 mode's 13 screen machines, its 23
  transitions, 6 pages, the color pick and the purchase, the career's
  first car; 38 ready groups; the paint color lead taken whole, four
  leads' lines trimmed, 1 lead added (the type 2 mode's other screen
  machines); no subagents); map checked. Migration 3480d4e6..f51e5073
  (41 of 58 in play: `InitStates`, `Create`, the pages 0-4, `Update`,
  `Start`, the lists, `Buy`, the info and swatches, the first race's
  start; 5 adapters removed; `--native_car_dealer`; race 36 s, 0
  errors, its harnesses 0 differing; no subagents). Left: the
  transitions 1, 6, 7, 9, 11, 15, 16, 17, 19 and the set-ups
  `OpenListMenu`, `OpenCarList`, `OpenCar`, `OpenColors` (ready). Step:
  validation, agent running.

## Done

- `front-end-screen-helpers.md`: 27 of 32 in play native (5 Merged;
  the modes' releases, the save and options dialogs, the car cards; 6
  adapters removed), `--native_front_end_screen_helpers`; 200c6832,
  92fe4c38..99d3bf29.

- Cleanup lead after the UI movie helpers (not counted): the in-race
  screens, the race HUD machine and object, the pause menu's pages and
  transitions, the career results (`race-screens.md`,
  `race-hud-machine.md`, `race-hud-object.md`, `career.md`,
  `type6-type7-modes.md`, `files-and-saves.md`), four migrations, ~60
  adapters removed; 9ca12391..f6ad7444.

- `ui-movies-game.md`: 8 new rows, 92 of 92 ours (the UI movie
  helpers), with the car list's groups they unblocked (`car-list.md`,
  94 of 94; 34 adapters removed in all), `--native_ui_movies_game`,
  `--native_car_list`, `--native_file_layer`; 9f12739f,
  e052912e..c87035a6.

## Rows to review

`review.md`, "Run 2026-11-08 13:00".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4920, work left 1506 (wrapped 56, known 448,
  unknown 1002).
- After the UI movie helpers' map: ours 4920, work left 1506 (wrapped
  56, known 451, unknown 999).
- After its migration: ours 4944, work left 1482 (wrapped 56, known
  427, unknown 999).
- After its second migration: ours 4952, work left 1474 (wrapped 56,
  known 419, unknown 999).
- After the cleanup lead's migration: ours 4960, work left 1466
  (wrapped 56, known 413, unknown 997).
- After its second migration: ours 4962, work left 1464 (wrapped 56,
  known 411, unknown 997).
- After its third migration: ours 4979, work left 1447 (wrapped 56,
  known 394, unknown 997).
- After its fourth migration: ours 4989, work left 1438 (wrapped 56,
  known 385, unknown 997).
- After the front-end screen helpers' map: ours 4989, work left 1438
  (wrapped 56, known 417, unknown 965).
- After its migration: ours 5016, work left 1411 (wrapped 56, known
  390, unknown 965).
- After the car dealer's map: ours 5016, work left 1411 (wrapped 56,
  known 447, unknown 908).
- After its migration: ours 5057, work left 1371 (wrapped 56, known
  407, unknown 908).

## Stopped
