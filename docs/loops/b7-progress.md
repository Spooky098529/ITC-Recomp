# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md` (their drive lists are there), and this
file's history.

## Now

- Run started 2026-10-23 11:06 on `b7-loop-23-10-26-2` (the user's
  branch), from 5b4cecd; summary `b7-loop-2026-10-23-1106.md`.
- Subsystem 1 of 3: the action mappers (`action-mappers.md`, mapped in
  422b250 from the list helpers' lead: 40 rows, 15 ready groups). Migration 1
  (006e5e4..c67265f) passed the checks: 36 of 40 ours. Step:
  validation (agent running).

## Done

None yet.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 11:06").

## Expected to differ

This run's groups: `expected-differences.md` (the "11:06 run" lines).

## To check in a drive

None yet.

## Totals

- Start: ours 971, work left 5782 (wrapped 137, known 459, unknown 5186).
- After mapping the action mappers: ours 971, work left 5782 (wrapped
  137, known 484, unknown 5161).
- After the action mappers' migration 1: ours 1007, work left 5746
  (wrapped 129, known 456, unknown 5161). Its races ran 19 s at 30
  ticks/s with `--run_harnesses=all` (36 new harnesses at boot); 23 s
  without, with `--native_action_mappers` on and off (`itc_913`,
  `itc_914`).
