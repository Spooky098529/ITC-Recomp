# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-04 08:44 on `b7-loop-03-11-26` (the thirtieth
  run's branch, merged; the user's call, 2026-11-04: the new run stays
  on it), from c9283680; summary `b7-loop-2026-11-04-0844.md`. Limit: 3
  subsystems.
- Subsystem 1 of 3: the race flow's set-up `sub_82160AE0` (lead picked;
  the two leads above it deleted as covered by existing files). Map
  checked (ef6eef64: `race-flow.md`, 5 rows, all new; 0 errors; 1 ready
  group, `sub_82160AE0`, 1,661 lines, split the migration). Migration
  1e19350e (`--native_race_flow`, 1 of 1 in play; `itc_3417`: race 38
  s, 0 errors, race_flow 12 cases 0 differing; no subagents). Step:
  validation, agent running.

## Done

(none yet)

## Rows to review

`review.md`, "Run 2026-11-04 08:44".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4059, work left 2346 (wrapped 71, known 482,
  unknown 1793).
- After the race flow's map: ours 4059, work left 2346 (wrapped 71,
  known 483, unknown 1792).
- After its migration (`itc_3417`): ours 4060, work left 2345 (wrapped
  71, known 482, unknown 1792).
