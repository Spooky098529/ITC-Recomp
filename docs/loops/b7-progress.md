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
  Step: migration 1 (agent running).

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

- `collision-world.md` (new, 43 rows), `known-data.md`'s mesh, route,
  course table and track position rows; corrections: `car-physics.md`'s
  CollisionObject::Collide (the first query's arguments: r3 the
  records, r4 the shape record), `collision.md`'s box words +160 (the
  lower corners' track positions), `sub_820C0168` probably the car's own
  track position; 5 leads added.

- `collision.md` (new, 64 rows), `known-data.md`'s shape, box, pair
  query and table entries, `car-physics.md`'s intro; 1 lead added (the
  track's collision queries). Guesses to confirm: the floats at
  `0x82002B90`..`0x82002B98` and the impulse scale at `0x82600E84`.
  Migration 1's 53 rows (35 Overridden, 18 Native) and the intro (the
  box's vtable slots, read at run time). For review: the native code
  reads the constants at `0x82002B90` and `0x820021E8` from guest memory,
  leaves out the box's type calls (slot 4 is `li r3,2`) and runs the
  original for any shape that isn't a box or when the impact table's
  box-box entry isn't `sub_820BF038`; the box copies' memcpys are native
  only where `c_runtime::Memcpy` records nothing. Map corrections:
  LowerCorner's ties and NaNs (k^2), NextCheckTime's radii (the +20
  shape), Shape::Shape zeroes +108's w.
  Migration 2's rows: 23 in `collision.md` (all 64 ours: 35 Native, 29
  Overridden), `car-physics.md`'s `sub_820B7B60` CollisionObject::Translate
  and `sub_820B2CC0` Vehicle::ApplyForces, `vehicle-accessors.md`'s new
  `sub_820988C0` RigidBody::Force (keeps its adapter: reached
  indirectly in play) and `sub_82379328` RigidBody::Torque. For review:
  the lifetime functions use `memory::crt` only while `--native_heap` is
  on, checking error paths first with a new `memory::IsHeapBlock`;
  PushOutCorners does the work of `sub_820BE0D8` and `sub_820BC8A8`
  (never ran in play, no rows); Respond's and Solve's "a fixed body
  meeting the impulse" groups (below).

- `heap.md`: 15 rows read in full and rewritten (no status changed), the
  intro's paragraph on native versions; migration 1's 15 rows (12
  Overridden, 3 Native; XMemAlloc's attribute bits corrected by the
  harness: protection `(attr >> 28) & 3`, default alignment field 12). For
  review: the adapters run the plain C++ only while `--native_heap` is
  active (`hooks::NativeHeapActive()`), and the error paths (errno, the
  new handler, `bad_alloc`, `_invalid_parameter`) run `__imp__` from the
  start; `known-data.md`'s heap globals;
  1 lead added (`sub_8235AAC0`), operator new taken from the
  VirtualAlloc lead.

## Expected to differ

- `collision.md` (migration 1): every function has a "NaNs and
  infinities anywhere (expected to differ)" group (two NaNs meeting;
  sign or payload only); nonzero: FaceDirection 2, BoxShape::Translate
  2, BoxShape::Place 10, UpdateMotion 21, CollisionShape::Place 85,
  CollisionShape::Translate 33, RelativePlacement 17, RelativeCenter 1,
  BlockCorners 1, BoxBox::Separated 96, SeparatedAfterMove 17,
  Query::Separated 96, Query::SeparatedAfterMove 17, BoxBox::FindImpact
  1, Query::FindImpact 1 (of 200). UpdateMotion, CollisionShape::Place
  and CollisionShape::Translate also have "one NaN meeting its own
  negation" groups (1, 0, 1 differ: a sign in row +96).
- `collision.md` (migration 2): "NaNs and infinities anywhere":
  Vehicle::ApplyForces 23 of 100, CollisionObject::Translate 32 of 200,
  CollisionShape::SetUp 7 of 100; Translate "one NaN meeting its own
  negation" 1 of 1. CollisionPair::Respond "a fixed body meeting the
  impulse" 10 of 44, "one NaN anywhere" 4 of 158, both 6 of 42; Solve
  "one NaN anywhere, a fixed body meeting the impulse" 2 of 42. For
  review: the fixed-body groups differ because the original
  RigidBody::ContactImpulseWith reads its stack's leftovers for a fixed
  body, and native callees leave different leftovers (the game's cars
  aren't fixed; whether anything fixed meets a car in play isn't known).

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
