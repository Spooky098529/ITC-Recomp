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
- Subsystem 1 of 3 (the action mappers) finished: the rest waits on
  the input manager's vtable targets and `sub_8258CE60`.
- Subsystem 2 of 3: the game's data tables at `[0x826C5CA0]`
  (`data-tables.md`, mapped in d8f876f: 18 rows, 9 ready groups).
  Migration 1 (db90dc4..4344730) passed the checks: 16 of 18 ours.
  Step: validation (agent running).

## Done

- The action mappers (`action-mappers.md`, mapped in 422b250; 40 rows):
  36 ours in one migration (006e5e4..c67265f), validated (35 of 35
  harness lines, one Merged; all ran in the attract race or the replay,
  replay ok); `--native_action_mappers`. Left: BindAll and Bind (the
  input manager's vtable targets), the mapper's constructor and Init
  (`sub_8258CE60`).

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 11:06").

## Expected to differ

This run's groups: `expected-differences.md` (the "11:06 run" lines).

## To check in a drive

None so far: the action mappers' functions all ran in the attract
race or the replay.

## Totals

- Start: ours 971, work left 5782 (wrapped 137, known 459, unknown 5186).
- After mapping the action mappers: ours 971, work left 5782 (wrapped
  137, known 484, unknown 5161).
- After the action mappers' migration 1: ours 1007, work left 5746
  (wrapped 129, known 456, unknown 5161). Its races ran 19 s at 30
  ticks/s with `--run_harnesses=all` (36 new harnesses at boot); 23 s
  without, with `--native_action_mappers` on and off (`itc_913`,
  `itc_914`).
- After mapping the data tables: ours 1007, work left 5746 (wrapped 129,
  known 474, unknown 5143).
- After the data tables' migration 1: ours 1023, work left 5730 (wrapped
  129, known 458, unknown 5143).
