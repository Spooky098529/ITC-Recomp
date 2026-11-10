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
  subagents, one at a time). Left: none. Validation halted: the
  all-harness race `itc_4804` never raced: `RaceMenu::Transition3`'s
  harness (`src/harnesses/race_menu.cpp`) looped on "Unhandled guest
  access violation: read of guest 0x701FAF40" (~6.96M lines, then the
  title exited), after 15 race menu groups at 0 differing and every
  earlier harness at 0; 20 of 35 harness lines, 15 missing (the race
  menu's groups after Transition3). No prints added. The replay
  `itc_4805` was ok (127 s, 0 errors): 28 ran in it, 7 to drive.
- After the halt, by the user's call (2026-11-10, "fix it"): the
  harness's world left each node's first scene animation entry (+116)
  random, so `UiMovie::Play`'s lookups read anywhere in guest memory
  (`itc_4806`: case 34, the recompiled side, sub-scene 20's entry
  0x701FAF20 + 8 x 4); `MakeWorld` now gives every node `EventWords`'
  set-up (4438f1c3). Validated again `itc_4807` (race 65 s at 180, the
  harnesses ~113 s; 0 errors, every harness 0 differing outside the
  expected groups, 35 of 35 harness lines). Recorded.

## Done

- The race menu: `race-menu.md`, 38 of 38 (21 Native, 3 Merged),
  `--native_race_menu`, a65e90d7..4414719e; its validation halted, its
  harness fixed (4438f1c3) and validated again.

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

2026-11-10, halted at the race menu's validation (subsystem 1 of 3):
`RaceMenu::Transition3`'s harness cases read the wild guest address
0x701FAF40 in the all-harness race (`itc_4804`), the access violation
repeated until the title exited, so the race never started and 15 of
the race menu's functions have no harness line ("When to halt": a
harness failing, functions missing). Which side read the address isn't
known (probably a world word the harness leaves unset, as `dbc8ee0`'s
`ai_drivers` case); the migration agent's own runs of the race menu's
harnesses (at 40 s) were at 0 differing. Not debugged by the loop. The
replay `itc_4805` ran 28 of the 35 as native code with 0 errors.
Nothing uncommitted. `itc_4804.log` is ~1.1 GB (the repeated line).
Fixed after the stop (4438f1c3, the user's call): the harness's random
+116 words; validated again (`itc_4807`, all 0 differing, 35 of 35).
Then the run was ended there by the user's call (2026-11-10, after 1
of 3 subsystems) and merged into `rexglue-removal-project`.
