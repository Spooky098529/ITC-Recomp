# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-25 23:42 on `b7-loop-25-10-26-4` (the user's
  branch), from 45b1205; summary `b7-loop-2026-10-25-2342.md`.
- Limit for this run: 3 subsystems (the default).
- Subsystem 1 of 3: `leads.md`'s top lead, `sub_82115D98`,
  `sub_820CD5D8`, `sub_820CD7F0` (the race's set-up and a racer
  record's part set-up; 16 ready groups under them). Mapped as the
  race's set-up (`race-setup.md`, 52 rows, 7d2acaa; 3 leads added).
  Map checked (0 errors, 52 of 52 in play, 19 ready groups, no kernel
  calls; blocked by `RacerRecord::Init` `sub_820FF808` and
  `Vehicle::SetupFromCar` `sub_820C27E8`). Step: migration agent 1
  running.

## Done

(nothing yet)

## Rows to review

This run's rows: `review.md` ("Run 2026-10-25 23:42").

## Expected to differ

This run's groups: `expected-differences.md` (the "23:42 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 2152, work left 4299 (wrapped 104, known 540, unknown 3655).
  `b7_status.py`'s one check at the start: `movies.md:108`
  `sub_82108018`'s row says never called, but it ran (not this run's).
- After the race's set-up's map: ours 2152, work left 4299 (wrapped
  104, known 592, unknown 3603).

## Stopped

(running)
