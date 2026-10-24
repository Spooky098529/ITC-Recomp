# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`,
`b7-loop-2026-10-23-2022.md`, `b7-loop-2026-10-24-0325.md`, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-24 16:01 on `b7-loop-24-10-26-2` (the user's
  branch), from f122c5f; summary `b7-loop-2026-10-24-1601.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead (`sub_820900C0`,
  `sub_820911A8`, probably the C runtime's array destructor and
  constructor helpers): mapped into `c-runtime.md` (2 new rows, 3a15cda;
  probably the compiler's `vector constructor iterator` and `vector
  destructor iterator`; 2 ready groups; 2 leads added); map checked (0
  errors, 2 of 2 in play). Migration 1 done (f624a82, 93e739f: both
  helpers under `--native_c_runtime`, and wcslen, mbtowc and the wide
  write_char's adapters removed), validated (`itc_1342`, `itc_1343`;
  replay ok; 1 to drive; prints added in 9b878fd). Finished.
- Subsystem 2 of 3: `leads.md`'s `820FB8B8`-`820FC0E8` lead
  (`sub_820FBBE8` and others, called only from `820F1138`-`820F4558`):
  closest to ready (all 6 of its roots ready, 8 functions left).
  Mapped as the course scenery (`course-scenery.md`, 71 rows: 3 moved
  from `functions.md`, 68 new; 043a0d2; 29 ready groups; 5 leads added);
  map checked (0 errors, 71 of 71 in play). Migration agent running.

## Done

- The compiler's array helpers (`c-runtime.md`, mapped in 3a15cda; 2
  new rows): 2 ours in one migration (f624a82), with wcslen's, mbtowc's
  and the wide write_char's adapters removed (93e739f); validated
  (`itc_1342`, `itc_1343`); `--native_c_runtime`. Left: none of the
  two.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-24 16:01").

## Expected to differ

This run's groups: `expected-differences.md` (the "16:01 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1672, work left 5088 (wrapped 123, known 555, unknown 4410).
- After mapping the array helpers: ours 1672, work left 5088 (wrapped
  123, known 557, unknown 4408).
- After the array helpers' migration 1: ours 1674, work left 5086
  (wrapped 123, known 555, unknown 4408). Race 27 s (`itc_1341`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing.
- After mapping the course scenery: ours 1674, work left 5086 (wrapped
  123, known 623, unknown 4340).

## Stopped

(not stopped)
