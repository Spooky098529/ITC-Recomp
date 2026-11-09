# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-08 on `b7-loop-08-11-26-2` (made by the user for
  this run), from fa0e6c51; summary `b7-loop-2026-11-08-2040.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the type 2 mode's other screen machines
  (`sub_82190B18`, `sub_8218CE10`, `sub_821E7568`, `sub_821B90D8`,
  `sub_821B6FC0`, `sub_821A16C0`, `sub_82196520`, `sub_8219B2E8`; the
  last run's recommendation, 8 ready roots; the top lead `sub_82562FF0`
  no longer runs in play and the next ones wait on the core's sync; its
  line deleted from `leads.md`). Mapped `type2-screens.md` (eb666c7f:
  150 rows, 121 in play, 1 moved from `functions.md`; four of the eight
  machines in full: the hub at +0, the spec sheet at +56624, the build
  screen's machine at +0x1BF20, the license plate editor at +0x20A70;
  97 ready groups; 5 leads added for the rest (+9640, +0x23018,
  +0x173D0, the tuning screen's Init, the mode's Init), two leads
  deleted as covered; no subagents); map checked. Migration
  e134d538..caa092b0 (the license plate editor whole: set-up, cells,
  menus, transitions, pages, update; the four machines' InitStates; the
  spec sheet's and the build screen machine's handlers; 12 adapters
  removed; `--native_type2_screens`; races 33-38 s, 0 errors, its
  harnesses 0 differing; two harness world fixes; no subagents). Left:
  the hub whole, the build screen's Enter and its pages (ready).
  Validated `itc_4654` (race 78 s at 180, the harnesses ~100 s; 0
  errors, every harness 0 differing outside the expected groups, 61 of
  61 harness lines, no prints added; none in the race, 20 in the replay
  `itc_4655` (ok), 41 to drive). Second migration 36e01983..67b6d2c4
  (the hub: start, notices, images, pages, transitions, dialogs;
  `WaitForLoad` keeps the patched 1; the build screen's `Enter` with
  `Fill`, `RemoveAt`, `SetUpStatus`; 3 adapters removed; 110 of 121
  ours; races 34-37 s, 0 errors, its harnesses 0 differing; no
  subagents). Left: `Type2Hub::Update` with `PutCarOnStand` (needs the
  stand car's harness world), the build screen's three pages with
  their Jumps (ready). Validated `itc_4668` (race 75 s at 180, the
  harnesses ~105 s; 0 errors, every harness 0 differing outside the
  expected groups, 49 of 49 harness lines, no prints added; 1 in the
  race, 28 in the replay `itc_4669` (ok), 20 to drive). Third
  migration be6b79e6..c18081f3 (the build screen's three pages with
  their six jumps, `Type2Hub::Update` with `PutCarOnStand` over a
  stand car harness world; `ShowBusy`, `HideBusy` adapters removed;
  121 of 121 in play ours; races 34-38 s, 0 errors, its harnesses 0
  differing; no subagents). Left: none. Validated `itc_4681` (race 75
  s at 180; 0 errors, every harness 0 differing outside the expected
  groups, 13 of 13 harness lines, no prints added; 4 in the replay
  `itc_4682` (ok), 9 to drive). Recorded.
- Subsystem 2 of 3: the type 6 quit's save screen (`sub_821A2608`,
  `sub_821A1D10`, `sub_821A26F0`, `sub_821A6608`, `sub_821A6AF0`,
  `sub_821A64A0`, `sub_821A2760`, `sub_821A4C58`, `sub_821A1CA0`,
  `sub_821A6650`: `Type6SaveScreen::Init`'s transition handlers; all 10
  roots ready, the closest lead; its line deleted from `leads.md`).
  Mapped `save-screen.md` (ce16c61a: 61 rows, 37 in play: the save
  screen machine (UI movie 44) five modes' Inits share, its eight
  kinds, `Draw`'s 27 pages and `Update`'s 7 steps; 37 ready groups; the
  save screens' lead trimmed, 1 lead added (the type 10 mode's Init);
  no subagents); map checked. Migration d393b07d..36674493 (the steps
  1 and 2, 22 pages, `StoreSettings`, the machine's handlers, `Update`,
  `Draw`, the type 2, 4 and 10 Inits; `StartCheck`, `Start` adapters
  removed; 42 of 42 in play ours; `--native_save_screen`; races 31-37
  s, 0 errors, its harnesses 0 differing; no subagents). Left: none.
  Validated `itc_4711` (race 69 s at 180; 0 errors, every harness 0
  differing outside the expected groups, 43 of 43 harness lines, no
  prints added; 6 in the race, 12 in the replay `itc_4712` (ok), 25 to
  drive). Recorded.
- Subsystem 3 of 3: the front-end screen around the menu helpers
  (`sub_82188178`, `sub_821869B8`, `sub_82186428`, `sub_82186490`,
  `sub_821864A0`, `sub_82187160`, `sub_82187330`, `sub_82187508`,
  `sub_82187EC8`: the type 2 mode's +0x1E4C8 machine; all roots ready;
  its line deleted from `leads.md`). Step: mapping (agent running).

## Done

- `save-screen.md`: 42 of 42 in play native (the save screen machine
  five modes share; 2 adapters removed), `--native_save_screen`;
  ce16c61a, d393b07d..36674493.

- `type2-screens.md`: 121 of 121 in play native (the hub, the spec
  sheet, the build screen's machine, the license plate editor; 17
  adapters removed), `--native_type2_screens`; eb666c7f,
  e134d538..c18081f3.

## Rows to review

`review.md`, "Run 2026-11-08 20:40".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5074, work left 1354 (wrapped 56, known 390,
  unknown 908).
- After the type 2 screens' map: ours 5074, work left 1354 (wrapped
  56, known 510, unknown 788).
- After its migration: ours 5135, work left 1293 (wrapped 56, known
  449, unknown 788).
- After its second migration: ours 5184, work left 1244 (wrapped 56,
  known 400, unknown 788).
- After its third migration: ours 5195, work left 1233 (wrapped 56,
  known 389, unknown 788).
- After the save screen's map: ours 5195, work left 1233 (wrapped 56,
  known 426, unknown 751).
- After its migration: ours 5237, work left 1196 (wrapped 56, known
  389, unknown 751).

## Stopped
