# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md` (their rows to review and drive lists are
there), and this file's history.

## Now

- Run started 2026-10-22 22:11 on `b7-loop-22-10-26-2` (the user's
  branch), from 91f40be; summary `b7-loop-2026-10-22-2211.md`.
- Subsystem 1 of 3 (the C runtime heap) finished.
- Subsystem 2 of 3 (the collision narrow phase) finished.
- Subsystem 3 of 3: the collision world's owner and the track's
  collision queries (two leads joined: `sub_820FF050`, `sub_820FF368`,
  ..., and `sub_820FDF38`, `sub_820FDC18`, ...; the collision update
  every tick, above `collision.md` and `car-physics.md`), mapped in
  `collision-world.md` (a28943d; 43 rows; checked: 14 ready groups).
  Migration 1 done (323cd79..ee7d5e1, 29 of 43 ours). Validated (29 of
  29 harness lines, all ran in play or the replay; replay ok). Migration 2
  done (027d891..4fc6646: the wall query, Unload and the teardown; 37 of
  43 ours). Step: the harness NaN comparison change (the user's call),
  then migration 2's validation.

## Done

- The collision narrow phase (`collision.md`, mapped in dd758b3; 64
  rows): all 64 ours (29 Overridden, 35 Native) in two migrations
  (f5b59e8..e634cfd, df767d1..b83c56a), each validated (53 and 27
  harness lines; replays ok); `--native_collision`; with
  `car-physics.md`'s CollisionObject::Translate and Vehicle::ApplyForces
  and `vehicle-accessors.md`'s RigidBody::Force and Torque. Left: none.

- The C runtime heap (`heap.md`, its existing rows read in full in
  1f48eaf): 15 functions ours (12 Overridden, 3 Native) in one migration
  (510aae9..34d8c7b), validated (15 of 15 harness lines, all ran in
  play); `--native_c_runtime_heap`. Left: none (the game's own heap
  internals aren't ported).

## Rows to review

This run's rows: `review.md` ("Run 2026-10-22 22:11").

## Expected to differ

This run's groups: `expected-differences.md` (the "22:11 run" lines).

## To check in a drive

Functions made native this run with no `native running (game):` line in
the validation agent's attract race or pad replay; the user drives (with
`--log_level=info`) and checks the log for them when the loop ends.

- Collision (migration 2's validation, attract race `itc_799` and replay
  `itc_800`, replay ok): `sub_820BACB8` CollisionShape::Type. Migration
  1's 18 impact-path functions all ran in that replay (the first replay,
  `itc_779`, had no deep enough contact).

## Totals

- Start: ours 714, work left 6039 (wrapped 137, known 336, unknown 5566).
- After mapping the heap: the same (rows rewritten in place).
- After the heap's migration 1: ours 729, work left 6024 (wrapped 137,
  known 321, unknown 5566).
- After mapping the collision narrow phase: ours 729, work left 6024
  (wrapped 137, known 385, unknown 5502).
- After the collision's migration 1: ours 782, work left 5971 (wrapped
  137, known 332, unknown 5502).
- After the collision's migration 2: ours 797, work left 5956 (wrapped
  137, known 319, unknown 5500).
- After mapping the collision world: ours 797, work left 5956 (wrapped
  137, known 362, unknown 5457).
- After the collision world's migration 1: ours 826, work left 5927
  (wrapped 137, known 333, unknown 5457).
- After the collision world's migration 2: ours 837, work left 5916
  (wrapped 137, known 322, unknown 5457).
