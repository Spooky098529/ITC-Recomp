# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-23 20:22 on `b7-loop-23-10-26-5` (the user's
  branch), from cf1f2cb; summary `b7-loop-2026-10-23-2022.md`.
- `leads.md`'s top lead (VirtualAlloc and VirtualFree, `sub_82295398`,
  `sub_82295330`) dropped: both already ours (native memory).
- Subsystem 1 of 3: the AI drivers (`ai-drivers.md`, mapped in
  1eeb084 from the route users' lead; 148 rows, 67 ready groups; the
  race states' route users went back to `leads.md`). Migration 1
  (c94d6be..91b8da2): 74 ours, 24 Native, `--native_ai_drivers`; 47
  ready groups left. Step: validation of migration 1 (agent running).

## Done

(None yet.)

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 20:22").

## Expected to differ

This run's groups: `expected-differences.md` (the "20:22 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1223, work left 5534 (wrapped 123, known 559, unknown 4852).
- After mapping the AI drivers: ours 1223, work left 5534 (wrapped 123,
  known 705, unknown 4706).
- After the AI drivers' migration 1: ours 1297, work left 5460 (wrapped
  123, known 631, unknown 4706). Race 22 s (`itc_1123`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (no
  "(expected to differ)" groups added).
