# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-09 on `b7-loop-09-11-26` (made by the user for
  this run), from 6497ef2f; summary `b7-loop-2026-11-09-1053.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the type 2 mode's dialog (`type2-dialog.md`,
  mapped and checked by the last run, c257b0c2, its migration not
  started; the last run's "Next"). Map checked again (0 errors, 21 in
  play, ready groups). Migration 415b58d5..e3269357 (the 10 steps, 4
  pages, `Draw`, `Update`, the 11 handlers, `Init` over its step table;
  28 of 28 ours, all Overridden: reached through functors and tables,
  `Init` by the still recompiled type 2 Init; `--native_type2_dialog`;
  races 38 s, 0 errors, its harnesses 0 differing; no subagents). Left:
  none. Validated `itc_4721` (race 68 s at 180, the harnesses ~110 s;
  0 errors, every harness 0 differing outside the expected groups, 28
  of 28 harness lines, no prints added; none in the race, 12 in the
  replay `itc_4722` (ok), 16 to drive). Recorded.
- Subsystem 2 of 3: the type 2 mode's machine at +0x23018 (`sub_82196520`
  and its transitions, UI movie 5, probably the garage; all 10 roots
  ready, the closest of the type 2 machines' leads; its line deleted
  from `leads.md`). Step: mapping; agent running.

## Done

- The type 2 dialog: `type2-dialog.md`, 28 of 28 (all Overridden),
  `--native_type2_dialog`, 415b58d5..e3269357.

## Rows to review

The run's section of `review.md` ("Run 2026-11-09 10:53").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5237, work left 1196 (wrapped 56, known 410,
  unknown 730).
- After the type 2 dialog's migration: ours 5265, work left 1175
  (wrapped 56, known 389, unknown 730).

## Stopped
