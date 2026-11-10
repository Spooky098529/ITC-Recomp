# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-10 on `b7-loop-10-11-26` (made by the user for
  this run), from abc87b1d; summary `b7-loop-2026-11-10-2102.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the type 2 mode's machine at +9640 (`sub_8218CE10`
  and its transitions, UI movie 1, probably the career's race menu; all
  seen in play, its groups ready from the bottom up; first in the last
  run's "Next" after the small `Garage::Sell`; its line deleted from
  `leads.md`). Mapped `race-menu.md` (e6f5b641: 38 rows, 31 in play:
  the race mode, event and variant pages, the confirm dialog, the
  start through `Career::SetUpEvent` and `Race::SetUp`; 21 ready
  groups, `Draw` over 1,500 lines to split; no leads added, the race
  set-up callers' lead deleted, two trimmed; no subagents); map
  checked. Migration a65e90d7..4414719e (Init and the handlers,
  `SetUpEvents`, `Refresh`, `Start`, the transitions and debug pages,
  `Update`, the pages, `Draw` with `StartPage` and `StartEvent`; 38 of
  38 ours: 14 Overridden, 3 Merged, 21 Native; `--native_race_menu`;
  races 37-38 s at 40, 0 errors, its harnesses 0 differing; 7
  subagents, one at a time). Left: none. Step: validation (agent
  running).

## Done

## Rows to review

The run's section of `review.md` ("Run 2026-11-10 21:02").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5364, work left 1133 (wrapped 56, known 390,
  unknown 687).
- After the race menu's map: ours 5364, work left 1133 (wrapped 56,
  known 421, unknown 656).
- After its migration: ours 5402, work left 1102 (wrapped 56, known
  390, unknown 656).

## Stopped
