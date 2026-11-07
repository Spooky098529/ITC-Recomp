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
  Validated `itc_4330` (race 90 s at 180, the harnesses ~88 s; 0
  errors, every harness 0 differing, 23 of 23 harness lines; none in
  the race, 9 in the replay `itc_4331` (ok), 14 to drive). Fourth
  migration 85a8c44b..43d9b8ec (`MoveType`, `InputTypes`; the adapters
  of `PageItemCount`, `PageCursor`, `IsPickOwned` and `MoveType`
  removed; races 35-36 s, 0 errors, its harnesses 0 differing; two
  subagents, one at a time); left: none in the file (72 of 74 ours;
  `HandleInput` waits on the editors' leads). Validated `itc_4346`
  (race 90 s at 180, the harnesses ~88.5 s; 0 errors, every harness 0
  differing, 5 of 5 harness lines; 4 in the replay `itc_4347` (ok),
  `IsPickOwned` to drive, already listed). Recorded.
- Cleanup leads (no mapping, not counted), taken together: seven
  removable adapters in `front-end-car-draw.md` (`sub_8217D2B0`,
  `sub_8217D120`, `sub_82179678`, `sub_8217AFE8`, `sub_8217BA20`,
  `sub_8217CAB8`, `sub_820C8480`) and six in `tuning-screen.md`
  (`sub_821F4BF8`, `sub_821F2068`, `sub_821EE880`, `sub_821E0E00`,
  `sub_821EE6D0`, `sub_821EE760`); `b7_groups.py --removable` lists all
  13; both lines deleted from `leads.md`. Migration c1cd4b85..99c60056
  (the 13, with `CategoryItemCount` `sub_821CC6C0` and `WheelNumber`
  `sub_821CDED8` that became removable: 15 Native; races 34-35 s, 0
  errors, the four files' harnesses 0 differing; the callers' harnesses
  leave out the callees' register saves below their frames; no
  subagents); left: none removable. Validated `itc_4356` (race 90 s at
  180, the harnesses ~88.5 s; 0 errors, every harness 0 differing, 15
  of 15 harness lines; 7 in the replay `itc_4357` (ok), 8 to drive).
- Subsystem 2 of 3: the front-end screens' constructors that make
  state machines (lead picked: `sub_822366F0`, `sub_82246B28`,
  `sub_8225A3D8`, `sub_82252290`, `sub_82235BD8`, `sub_82245FF8`,
  `sub_8224CF58`; the most ready groups in `leads.md`, 29; its line
  deleted from `leads.md`). Map checked (fbb76124:
  `screen-constructors.md`, 41 rows, all new, all in play: the type 6
  and 7 modes' screen blocks' constructors, `ListScreen`, `NodeScreen`,
  `MachineScreen`, the panel sets; 0 errors; 13 ready groups; 2 leads
  added (the type 6 and 7 modes' slot 0 with their Inits); no
  subagents). Migration c0ce0aec..bc2f1e67 (`--native_screen_constructors`,
  harness `screen_constructors`: all 41, the two blocks' constructors
  Overridden (their callers, the modes' slot 0, recompiled), 39 Native,
  24 adapters removed; the stack leftovers copied; races 38 s, 0
  errors, its harnesses 0 differing; no subagents); left: none.
  Validation agent running.

## Done

- Cleanup: 15 adapters removed in `front-end-car-draw.md` and
  `tuning-screen.md` (now Native); c1cd4b85..99c60056.

- `tuning-pages.md`: 72 of 74 native (the tuning screen's input pages,
  the vinyl editor's entry and exit, the decal images; 26 adapters
  removed), `--native_tuning_pages`; 92fea740, 94da1ea6..43d9b8ec.

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
- After its fourth migration: ours 4548, work left 1872 (wrapped 57,
  known 420, unknown 1395).
- After the cleanup leads (`itc_4356`): ours 4548, work left 1872 (15
  Overridden -> Native).
- After the screen constructors' map: ours 4548, work left 1872
  (wrapped 57, known 461, unknown 1354).

## Stopped
