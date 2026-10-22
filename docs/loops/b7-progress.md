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
  (`vehicle-accessors.md`, 132 rows, 96 ready groups; checked). Migration 1
  done (04d1028..64cfe00: 127 rows ours, `--native_vehicle_accessors`,
  `--native_vector_library`; race 22 s, 0 errors). Step: validation
  (agent running).
- Known: `b7_status.py` checks `sub_825A9934` (`__chkstk`) "Dropped, but
  it ran", and every function made Native or Merged since the coverage
  session "..., but it ran" (`seen_in_play.txt` predates them; a new
  coverage session clears them).

## Done

## Rows to review

- `vehicle-accessors.md`: migration 1's 127 rows (2 now Native, adapters
  removed). For review: the two Wrapped rows (`sub_82593FB0`,
  `sub_820FFD88`) had their frame-state-diff wrappers moved into their
  adapters (out of `src/debug/frame_state_diff.cpp`).
- `vehicle-accessors.md` (new, 132 rows; 3 moved from `functions.md`),
  `known-data.md`'s new rows, 5 leads added.

## Expected to differ

- `vehicle-accessors.md` (migration 1): every vector, rigid-body and
  quaternion function has a "two or more NaN inputs" / "NaN inputs"
  (expected to differ) group; six differ, only in a NaN's sign or
  payload (which NaN wins depends on the operand order the compiler
  picked, in the translated code and ours alike):
  `sub_822938F0` Matrix::TransformPoint 5 of 99, `sub_82294170`
  Vec3::Dot 6 of 21, `sub_82294320` Vec3::AddTo 3 of 21, `sub_822944B0`
  Vec3::ScaledCopy 1 of 4, `sub_82294608` Vec3::DotOf 6 of 21,
  `sub_820B6968` RigidBody::AddForceAt 1 of 40.

## To check in a drive

Functions made native this run with no `native running (game):` line in
the validation agent's attract race; the user drives (with
`--log_level=info`) and checks the log for them when the loop ends.

## Totals

- Start: ours 422, work left 6331 (wrapped 142, known 313, unknown 5876).
- After mapping the vehicle accessors: ours 422, work left 6331
  (wrapped 142, known 442, unknown 5747).
- After migration 1: ours 549, work left 6204 (wrapped 140, known 317,
  unknown 5747).

## Stopped
