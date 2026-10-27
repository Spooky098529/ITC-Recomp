# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-27 22:31 on `b7-loop-27-10-26` (the user's
  branch), from d7b8a6b; summary `b7-loop-2026-10-27-2231.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead, `sub_820AE010`,
  `sub_820AE348`, `sub_820AE498` (`820AE010`-`820AEC10`, objects made
  with `sub_825A6880` from `sub_82094590`; 4 ready groups under them).
  Mapped as the app's update and its state stack (`app-states.md`, 56
  rows: 2 moved from `functions.md`, 54 new; ac92c3b; 1 lead added).
  Map checked (0 errors, 56 of 56 in play, 26 ready groups, no
  blockers outside it, no kernel calls). Migration 1 done after five
  batches (7bb4dfd..b29697d: the containers, the operation list, the
  layer's push, the message vector, `App::IsRunning`, the walks,
  handlers, requests and add slots, `App::Update`; 5 adapters removed,
  4 Native with none, 1 Merged; `--native_app_states`; `itc_1995`: 23
  s, 0 errors, every harness 0 differing but the expected groups;
  frame dumps the same as the fallback). Left ready: `App::Start`,
  `AppStates::Create` and the state-layer tree's ready groups.
  Validated (`itc_1996`: 30 of 30 harness lines, 24 ran in the attract
  race, 6 in the replay `itc_1997`, ok; nothing to drive). Step:
  migration agent 2 running.

## Done

## Rows to review

This run's rows: `review.md` ("Run 2026-10-27 22:31").

## Expected to differ

This run's groups: `expected-differences.md` (the "22:31 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 2255, work left 4197 (wrapped 104, known 516, unknown 3577).
  `b7_status.py`'s one check at the start: `movies.md:108`
  `sub_82108018`'s row says never called, but it ran (not this run's).
- After the app states' map: ours 2255, work left 4197 (wrapped 104,
  known 570, unknown 3523).
- After the app states' migration 1: ours 2286, work left 4166
  (wrapped 103, known 540, unknown 3523).
- After the app states' validation: race 23 s (`itc_1996`), 0 errors,
  every harness 0 differing but the expected groups; replay 127 s
  (`itc_1997`), 0 errors.

## Stopped
