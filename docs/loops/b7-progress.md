# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-07 10:56 on `b7-loop-07-11-26` (made by the user
  for this run), from 48e2b333; summary `b7-loop-2026-11-07-1056.md`.
  Limit: 3 subsystems (`b7-loop.md`'s setting; the previous run's 8 was
  the user's call for that run).
- Subsystem 1 of 3: the tuning screen's other vtable slots and pages
  (lead picked: `sub_821D6AD0` and the 17 others, the most ready groups
  in `leads.md`, 45, beside the tuning screen just finished; the top two
  leads skipped: `sub_82562FF0` inside the movie library's range, the
  worker pools blocked only by the core's event and critical section
  wrappers; its line deleted from `leads.md`). Map checked (92fea740:
  `tuning-pages.md`, 106 rows, all new, 67 in play: `TuningScreen::HandleInput`
  `sub_821E6950` and its pages for states 0, 3-8, 15-22, 53-62; 0
  errors; 48 ready groups; 4 leads added (the vinyl and sticker editors'
  pages, the screen's other helper callers, `sub_8224B248`), the menu
  helpers' lead trimmed; three subagents, one at a time). Migration
  94da1ea6..5fd92bc3 (`--native_tuning_pages`, harness `tuning_pages`:
  the counts, owned flags, wheel and inch list ends, the decal images'
  textures and movie restores, `EnterEditor`, `StoreVinyls`,
  `LoadVinyls`, `ShowPricePanel`, `SetBrandLogo`, `Color::HsvToRgb`; 22
  Overridden, 6 Merged, 6 adapters removed; race 38 s, 0 errors, all
  its harnesses 0 differing; no subagents); left: 27 ready groups
  (`DecalImages::UnloadAll`/`Unload` first; the pages; `sub_821D74A0`
  with `sub_821D32C8` over 1,500 lines, to split). Validated
  `itc_4251` (race 92 s at 180, the harnesses ~86 s; 0 errors, every
  harness 0 differing, 28 of 28 harness lines; none in the race, 10 in
  the replay `itc_4252` (ok), 18 to drive). Second migration
  8e5ecf93..bd66f968 (`SetAeroAvailability`, `SetCurrentChoice`, the
  decal images' update, load and unload with `Texture::FromTga`,
  `Menu::StepList`, `LeaveEditor`, `InputCategories`, `BackToItems`,
  `InputWheelParts`, `InputInches`, `OnDialogYes` with the sticker
  dialog's callees; 11 Overridden, 11 Native without adapters, 1
  Merged, 4 adapters removed; races 37-38 s, 0 errors, all its
  harnesses 0 differing; four subagents, one at a time); left: 9 ready
  groups in the file (`sub_821D6E20`, `sub_821D74A0` with `sub_821D32C8`,
  `sub_821F6688`, `sub_821DFD48`, `sub_821D8160`, `sub_821E25B0` each
  over 1,100 lines; `sub_821FFB20`, `sub_82202530`, `sub_82201B40`).
  Validated `itc_4287` (race 91 s at 180, the harnesses ~86.6 s; 0
  errors, every harness 0 differing, 26 of 26 harness lines; none in the
  race, 8 in the replay `itc_4288` (ok), 18 to drive). Third migration
  d36330ef..ecb9b381 (`InputItems` with `MoveItem`, `InputOptions` with
  `MoveOption`, `Leave` with `StoreOwnedParts`, `InputWheels` with
  `AnimateWheels` and `FillPartRowsRight`/`Left`, `InputLevels` with
  `MoveLevel`; 12 adapters removed; races 36-37 s, 0 errors, all its
  harnesses 0 differing; six subagents, one at a time); left:
  `InputTypes` `sub_821D74A0` with `MoveType` (1,766 lines); six
  removable adapters in `tuning-screen.md` (a cleanup lead added).
  Validation agent running.

## Done

## Rows to review

`review.md`, "Run 2026-11-07 10:56".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4476, work left 1937 (wrapped 57, known 418,
  unknown 1462).
- After the tuning pages' map: ours 4476, work left 1937 (wrapped 57,
  known 485, unknown 1395).
- After its migration: ours 4510, work left 1904 (wrapped 57, known
  452, unknown 1395).
- After its second migration: ours 4534, work left 1885 (wrapped 57,
  known 433, unknown 1395).
- After its third migration: ours 4546, work left 1874 (wrapped 57,
  known 422, unknown 1395).

## Stopped
