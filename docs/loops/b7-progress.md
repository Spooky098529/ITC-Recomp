# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-23 13:44 on `b7-loop-23-10-26-3` (the user's
  branch), from 97be4fc; summary `b7-loop-2026-10-23-1344.md`.
- Subsystem 1 of 3: the model animation (the top lead): mapped in
  c425049 (36 rows, 18 ready groups), checked; migration 1 done
  (6f51fc2, 6714e72; nothing ready left); validation agent running.

## Done

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 13:44").

## Expected to differ

This run's groups: `expected-differences.md` (the "13:44 run" lines).

## To check in a drive

`to-drive.md` (what's left from earlier runs).

## Totals

- Start: ours 1023, work left 5730 (wrapped 129, known 586, unknown 5015).
- After mapping the model animation: ours 1023, work left 5730 (wrapped
  129, known 622, unknown 4979).
- After the model animation's migration 1: ours 1060, work left 5696
  (wrapped 129, known 588, unknown 4979). Race 19 s at 30 ticks/s with
  `--run_harnesses=all`, 0 errors, every harness 0 differing outside
  the earlier "(expected to differ)" groups (none added).

## Stopped
