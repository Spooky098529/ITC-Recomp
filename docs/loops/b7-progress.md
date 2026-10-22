# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md` (its
rows to review and drive list are there), and this file's history.

## Now

- Run started 2026-10-22 14:39 on `b7-loop-22-10-26` (the user's branch,
  kept from the previous run), from f9216cf; summary
  `b7-loop-2026-10-22-1439.md`.
- Subsystem 1 of 3: the vehicle accessors (the leads' vehicle, wheel,
  body, contact and scene-model accessors, with `sub_825C8388`'s lead
  folded in: both block the race overlays' updates). Mapped in 6846615
  (`vehicle-accessors.md`, 132 rows, 96 ready groups; checked). Step:
  migration 1 (agent running).
- Known: `b7_status.py` checks `sub_825A9934` (`__chkstk`) "Dropped, but
  it ran", and every function made Native or Merged since the coverage
  session "..., but it ran" (`seen_in_play.txt` predates them; a new
  coverage session clears them).

## Done

## Rows to review

- `vehicle-accessors.md` (new, 132 rows; 3 moved from `functions.md`),
  `known-data.md`'s new rows, 5 leads added.

## Expected to differ

## To check in a drive

Functions made native this run with no `native running (game):` line in
the validation agent's attract race; the user drives (with
`--log_level=info`) and checks the log for them when the loop ends.

## Totals

- Start: ours 422, work left 6331 (wrapped 142, known 313, unknown 5876).
- After mapping the vehicle accessors: ours 422, work left 6331
  (wrapped 142, known 442, unknown 5747).

## Stopped
