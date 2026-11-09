# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-09 on `b7-loop-09-11-26` (made by the user for
  this run), from 6497ef2f; summary `b7-loop-2026-11-09-1053.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the type 2 mode's dialog (`type2-dialog.md`,
  mapped and checked by the last run, c257b0c2, its migration not
  started; the last run's "Next"). Map checked again (0 errors, 21 in
  play, ready groups). Migration 415b58d5..e3269357 (the 10 steps, 4
  pages, `Draw`, `Update`, the 11 handlers, `Init` over its step table;
  28 of 28 ours, all Overridden: reached through functors and tables,
  `Init` by the still recompiled type 2 Init; `--native_type2_dialog`;
  races 38 s, 0 errors, its harnesses 0 differing; no subagents). Left:
  none. Validated `itc_4721` (race 68 s at 180, the harnesses ~110 s;
  0 errors, every harness 0 differing outside the expected groups, 28
  of 28 harness lines, no prints added; none in the race, 12 in the
  replay `itc_4722` (ok), 16 to drive). Recorded.
- Subsystem 2 of 3: the type 2 mode's machine at +0x23018 (`sub_82196520`
  and its transitions, UI movie 5, probably the garage; all 10 roots
  ready, the closest of the type 2 machines' leads; its line deleted
  from `leads.md`). Mapped `garage-screen.md` (5c43d083: 78 rows, 22
  in play: the garage screen, the player's garage cars over the stand,
  each made current, sold, its plate changed, viewed or its spec sheet
  opened; Init's 20 transitions, 5 pages, 18 table transitions,
  `Update`; 15 ready groups; no leads added, four leads' lines trimmed;
  no subagents); map checked. Migration e10e29dd..52c21938 (the
  helpers, transitions 0, 1, 3, the list and leave pages, `UpdatePage`,
  `Update`, the handlers, `InitStates`; 7 helper adapters removed; 22
  of 22 in play ours, the 56 never seen kept recompiled;
  `--native_garage_screen`; race 37 s, 0 errors, its harnesses 0
  differing; no subagents). Left: none. Validated `itc_4733` (race 68
  s at 180, the harnesses ~110 s; 0 errors, every harness 0 differing
  outside the expected groups, 22 of 22 harness lines, no prints added;
  none in the race, 6 in the replay `itc_4734` (ok), 16 to drive).
  Recorded.
- Subsystem 3 of 3: the tuning screen's machine (the type 2 mode's
  +19792, `sub_821E7568` and its transitions; all 8 roots ready, one of
  the type 2 Init's unmapped Inits; its line deleted from `leads.md`).
  Mapped `tuning-machine.md` (8db41a33: 22 rows, 19 in play: the
  tuning screen's machine handlers, its creation, movies, `Start` and
  `SetUp` from the garage car with the part tables' loaders, the type 2
  dialog's replies, unload, close and teardown; 10 ready groups, `Start`
  blocked by the refresh `sub_821E3AD0`; two leads taken whole and
  deleted, three given notes; no subagents); map checked. Migration
  529f68d7..9a1c1037 (the Init and handlers, `SetUp` and its five
  callees split over five subagents one at a time; 5 adapters removed,
  3 Native without one; 18 of 19 in play ours, `Start` blocked by the
  refresh; `--native_tuning_machine`; race 38 s, 0 errors, its
  harnesses 0 differing). Left: none. Step: validation; agent running.

## Done

- The garage screen: `garage-screen.md`, 22 of 22 in play (7 Native),
  `--native_garage_screen`, e10e29dd..52c21938.
- The type 2 dialog: `type2-dialog.md`, 28 of 28 (all Overridden),
  `--native_type2_dialog`, 415b58d5..e3269357.

## Rows to review

The run's section of `review.md` ("Run 2026-11-09 10:53").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5237, work left 1196 (wrapped 56, known 410,
  unknown 730).
- After the type 2 dialog's migration: ours 5265, work left 1175
  (wrapped 56, known 389, unknown 730).
- After the garage screen's map: ours 5265, work left 1175 (wrapped 56,
  known 411, unknown 708).
- After its migration: ours 5287, work left 1153 (wrapped 56, known
  389, unknown 708).
- After the tuning machine's map: ours 5287, work left 1153 (wrapped
  56, known 408, unknown 689).
- After its migration: ours 5305, work left 1135 (wrapped 56, known
  390, unknown 689).

## Stopped
