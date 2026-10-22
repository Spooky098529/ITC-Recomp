# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md` (its
rows to review and drive list are there), and this file's history.

## Now

- Run started 2026-10-22 14:39 on `b7-loop-22-10-26` (the user's branch,
  kept from the previous run), from f9216cf; summary
  `b7-loop-2026-10-22-1439.md`.
- Subsystem 1 of 3 (the vehicle accessors) finished; then the no-mapping
  lead (the race overlays' newly ready functions, 7c018ef..f986921)
  validated (21 of 21 harness lines).
- Subsystem 2 of 3 (the matrix math) finished. Now: the no-mapping
  lead (the race overlays' groups the matrix math made ready):
  migration (agent running).
- Known: `b7_status.py` checks `sub_825A9934` (`__chkstk`) "Dropped, but
  it ran", and every function made Native or Merged since the coverage
  session "..., but it ran" (`seen_in_play.txt` predates them; a new
  coverage session clears them).

## Done

- The matrix math (`matrix-math.md`, mapped in 7929f36; 28 rows): 28
  ours (20 Overridden, 5 Native, 3 Merged) in two migrations (18cafd4;
  6c81f15..9345920), each validated (18 and 8 harness lines, all ran in
  play); `--native_matrix_math`.

- No-mapping lead: the race overlays' functions made ready by the
  accessors (`race-overlays.md`): 14 rows ours (with FlagsKind and
  NoSaveFlags, no rows), 8 adapters removed (6 there, 2 in
  `vehicle-accessors.md`); under `--native_race_overlays`.
  7c018ef..f986921, validated. Still blocked there: `sub_820AB560`
  (Wrapped, 6 groups) and `sub_82126ED0` (a vtable call, 3 groups);
  UpdateSmoke (memcpy, unseen callees).

- The vehicle accessors (`vehicle-accessors.md`, mapped in 6846615; 132
  rows): 127 ours (125 Overridden, 2 Native) in one migration
  (04d1028..64cfe00), validated (127 of 127 harness lines);
  `--native_vehicle_accessors`, `--native_vector_library`. Left:
  `sub_820B8798` (needs `ctx`) and four behind `sub_820B94B0`,
  `sub_820B8AF8`, `sub_820DA180`, `sub_820CD5D8`.

## Rows to review

- `matrix-math.md` (new, 28 rows; 4 moved from `functions.md`), 1 lead
  added. Migration 1's 21 rows (15 Overridden, 3 of them were Wrapped;
  3 Native with no adapter ever; 3 Merged). For review: the three
  Wrapped functions' overrides (Multiply and Copy in
  `src/interpolation/blend_state.cpp`, Vec4::Transform in
  `src/debug/frame_state_diff.cpp`) now do the work natively, with the
  same hooks around it; frame dumps with the flag on and off look the
  same.
  Migration 2's 8 rows. For review: the turns' "one NaN" harness group
  keeps the angle within 1024 (a huge finite angle overflows the game's
  sine and cosine polynomials to NaNs of both signs, which then meet in
  Multiply); the "NaNs anywhere" group keeps every angle. The native
  turns note their multiply for interpolation through a new
  `interpolation::BlendState_OnMultiply` (as the Multiply wrapper does).
- `race-overlays.md`: the no-mapping lead's 14 rows (the view's body and
  wheel accessors, IsBehindCamera, the save flags and stage check,
  UpdateTrails, MarkerKind; FlagsKind `sub_82122DB8` and NoSaveFlags
  `sub_82127198` ported with MarkerKind but never ran in play, so no
  rows) and 6 adapters removed; `vehicle-accessors.md`: `sub_820C0170`,
  `sub_820C0180` Native. The new harnesses run the originals with the
  accessors' flags off too (an `Original<>` wrapper).
- `vehicle-accessors.md`: migration 1's 127 rows (2 now Native, adapters
  removed). For review: the two Wrapped rows (`sub_82593FB0`,
  `sub_820FFD88`) had their frame-state-diff wrappers moved into their
  adapters (out of `src/debug/frame_state_diff.cpp`).
- `vehicle-accessors.md` (new, 132 rows; 3 moved from `functions.md`),
  `known-data.md`'s new rows, 5 leads added.

## Expected to differ

- `matrix-math.md` (migration 1): every function has a "NaNs anywhere
  (expected to differ)" group; eight differ, only in a NaN's sign or
  payload (the operand order the compiler chose where two NaNs meet;
  every case with numbers alone or one NaN matches): Matrix::Multiply 5,
  Matrix::Inverse 10, MultiplyUnaligned 1, MultiplyTo 1,
  MultiplyReversed 1, ProjectPoint 3, Quaternion::FromMatrix 2,
  Quaternion::Multiply 1 (of 210 each).
  Migration 2: Matrix::RotateX 14, RotateY 20, RotateZ 19 (of 210; NaN
  or overflowing angles, the same reason through Multiply).

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

- Vehicle accessors (validation's race): `sub_820C0150`, `sub_820B6638`,
  `sub_820B6890`, `sub_820B6960`, `sub_820B7C90`, `sub_820B7D78`,
  `sub_820B8A48`, `sub_820B8CB0`, `sub_820B8CC0`, `sub_820B8CE0`,
  `sub_820B8D08`, `sub_820B8D10`, `sub_820B9668`, `sub_820B99B0`,
  `sub_820B99C0`, `sub_820FF938`, `sub_820FF9F8`, `sub_820FFA30`,
  `sub_820FFA50`, `sub_820FFAB0`, `sub_820FFC08`, `sub_820FFC68`,
  `sub_820FFC70`, `sub_820FFC98`, `sub_820FFCC8`, `sub_820FFD48`,
  `sub_820FFDD8`, `sub_820FFE08`, `sub_820FFE58`, `sub_820FFE98`,
  `sub_820FFF10`, `sub_82294348`, `sub_82294360`, `sub_822943C8`,
  `sub_822944B0`, `sub_82294568` (the racer records and stages probably
  run at a race's start or end, the contacts on collisions).
- Race overlays' newly ready functions (validation's race):
  `sub_82126BB0`, `sub_82126CB0`, `sub_82126C08`, `sub_82126C58`,
  `sub_82126B50` (the save flags and stage check), `sub_82122DB8`,
  `sub_82127198` (no rows: never seen in play), `sub_82129598`
  Trail::AddPoint (already on the previous run's list).

## Totals

- Start: ours 422, work left 6331 (wrapped 142, known 313, unknown 5876).
- After mapping the vehicle accessors: ours 422, work left 6331
  (wrapped 142, known 442, unknown 5747).
- After migration 1: ours 549, work left 6204 (wrapped 140, known 317,
  unknown 5747).
- After the race overlays' newly ready functions: ours 563, work left
  6190 (wrapped 140, known 303, unknown 5747).
- After mapping the matrix math: ours 563, work left 6190 (wrapped 140,
  known 327, unknown 5723).
- After the matrix math's migration 1: ours 584, work left 6169
  (wrapped 137, known 309, unknown 5723).
- After the matrix math's migration 2: ours 591, work left 6162
  (wrapped 137, known 302, unknown 5723).

## Stopped
