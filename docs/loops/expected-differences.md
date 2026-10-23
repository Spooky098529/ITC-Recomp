# Harness groups expected to differ

Every harness group named "... (expected to differ)" whose cases came out
different, with why. Kept across the B7 loop's runs (not started afresh
like `b7-progress.md`): the migration agents' reports add to it, and the
other loop docs point here instead of listing them. A group at 0 differing
isn't listed. Counts are from the run that added the line; a later run
may show other counts for the same group (random inputs).

Since 2a7c804 (2026-10-23, the user's call: NaN signs and payloads
aren't a goal), new harness code compares floats with `harness::Outputs`,
where two NaNs match ("Comparing floats" in `b7-loop.md`), and gets no
NaN-only groups; only real behavior differences (like Stack leftovers)
are added here. The existing harnesses weren't switched over, so their
NaN groups below stay as recorded.

## Reasons

- **Two NaNs:** where two NaNs meet in one operation, which one comes out
  follows the operand order the compiler picked, in the translated code
  and ours alike. Only a NaN's sign or payload differs; the cases with
  numbers alone, or one NaN, match. The game's data holds no NaNs.
- **Own negation:** one NaN reaches an operation both as it is and
  negated (`a + -a`), so it's the two NaNs case with a single input; only
  the sign differs.
- **Newton step:** the translated code's Newton step, as the compiler
  rewrote it, quiets NaNs differently; only the NaNs' sign and payload
  differ.
- **Stack leftovers:** the original `RigidBody::ContactImpulseWith`
  (`car-physics.md`) reads its stack's leftovers for a fixed body, and
  native callees leave different leftovers there (they save no registers
  on the guest stack). The game's cars aren't fixed; whether anything
  fixed meets a car in play isn't known.

- **Negated NaN:** where the vector unit's normalize meets a NaN or an
  infinity, the translated code keeps the NaN's sign where the source
  negates it (the compiler built -(x - 0.5) as 0.5 - x); ours negates it.
  Only a NaN's sign differs. The game's normals are numbers.
- **Unaligned out:** the normalize reads the words around an out pointer
  that isn't 16-aligned, which hold random values in the harness (with
  Negated NaN where they're NaNs). The game's outs are 16-aligned frame
  temporaries.

## By function file

Review: "accepted" once the user has agreed, else "for review" (the run
that added it).

### `race-overlays.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_82129598` Trail::AddPoint | NaN or infinite inputs | 5 of 126 | Newton step | accepted (2026-10-22) |

### `vehicle-accessors.md`

Every vector, rigid body and quaternion function has a "two or more NaN
inputs" or "NaN inputs" group (run 2026-10-22 14:39, migration 1).

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_822938F0` Matrix::TransformPoint | NaN inputs | 5 of 99 | Two NaNs | for review (14:39 run) |
| `sub_82294170` Vec3::Dot | NaN inputs | 6 of 21 | Two NaNs | for review (14:39 run) |
| `sub_82294320` Vec3::AddTo | NaN inputs | 3 of 21 | Two NaNs | for review (14:39 run) |
| `sub_822944B0` Vec3::ScaledCopy | NaN inputs | 1 of 4 | Two NaNs | for review (14:39 run) |
| `sub_82294608` Vec3::DotOf | NaN inputs | 6 of 21 | Two NaNs | for review (14:39 run) |
| `sub_820B6968` RigidBody::AddForceAt | NaN inputs | 1 of 40 | Two NaNs | for review (14:39 run) |

### `matrix-math.md`

Every function has a "NaNs anywhere" group (run 2026-10-22 14:39). The
turns' angles overflow the game's sine and cosine polynomials to NaNs of
both signs, which then meet in Multiply.

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| Matrix::Multiply | NaNs anywhere | 5 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::Inverse | NaNs anywhere | 10 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::MultiplyUnaligned | NaNs anywhere | 1 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::MultiplyTo | NaNs anywhere | 1 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::MultiplyReversed | NaNs anywhere | 1 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::ProjectPoint | NaNs anywhere | 3 of 210 | Two NaNs | for review (14:39 run) |
| Quaternion::FromMatrix | NaNs anywhere | 2 of 210 | Two NaNs | for review (14:39 run) |
| Quaternion::Multiply | NaNs anywhere | 1 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::RotateX | NaNs anywhere | 14 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::RotateY | NaNs anywhere | 20 of 210 | Two NaNs | for review (14:39 run) |
| Matrix::RotateZ | NaNs anywhere | 19 of 210 | Two NaNs | for review (14:39 run) |

### `car-physics.md`

Every function has a "NaNs and infinities anywhere" group (run
2026-10-22 14:39; migration 3's were 0 to 12 differing, not listed one by
one; SyncFromBody's "one NaN meeting its own negation" was 0 of 0 in the
run, 23 sign-only cases in a temporary test).

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| RigidBody::UpdateOrientation | NaNs and infinities anywhere | 1 | Two NaNs | for review (14:39 run) |
| RigidBody::SetOrientation | NaNs and infinities anywhere | 2 | Two NaNs | for review (14:39 run) |
| RigidBody::SetPosition | NaNs and infinities anywhere | 2 | Two NaNs | for review (14:39 run) |
| RigidBody::SetAngularVelocity | NaNs and infinities anywhere | 2 | Two NaNs | for review (14:39 run) |
| RigidBody::SetInertia | NaNs and infinities anywhere | 1 | Two NaNs | for review (14:39 run) |
| RigidBody::Integrate | NaNs and infinities anywhere | 3 | Two NaNs | for review (14:39 run) |
| RigidBody::EffectiveMassAt | NaNs and infinities anywhere | 7 | Two NaNs | for review (14:39 run) |
| RigidBody::ContactImpulse | NaNs and infinities anywhere | 87 | Two NaNs | for review (14:39 run) |
| RigidBody::ContactImpulse | one NaN meeting its own negation | 2 of 3 | Own negation | for review (14:39 run) |
| RigidBody::FrictionImpulse | NaNs and infinities anywhere | 1 | Two NaNs | for review (14:39 run) |
| RigidBody::ContactImpulseWith | NaNs and infinities anywhere | 1 | Two NaNs | for review (14:39 run) |
| Vehicle::AccumulateTireForces | NaNs and infinities anywhere | 8 | Two NaNs | for review (14:39 run) |
| Vehicle::BrakeTorques | NaNs and infinities anywhere | 4 | Two NaNs | for review (14:39 run) |
| Vehicle::SuspensionTorques | NaNs and infinities anywhere | 3 | Two NaNs | for review (14:39 run) |
| Vehicle::EngineTorque | NaNs and infinities anywhere | 1 | Two NaNs | for review (14:39 run) |
| Vehicle::AxleReactionTorque | NaNs and infinities anywhere | 4 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateTireGrip | NaNs and infinities anywhere | 1 of 100 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateBrakeHeat | NaNs and infinities anywhere | 1 of 100 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateTireWear | NaNs and infinities anywhere | 4 of 100 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateTireForces | NaNs and infinities anywhere | 38 of 100 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateAero | NaNs and infinities anywhere | 8 of 100 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateWheelAxes | NaNs and infinities anywhere | 79 of 100 | Two NaNs | for review (14:39 run) |
| Vehicle::UpdateWheelAxes | one NaN meeting its own negation | 1 of 1 | Own negation | for review (14:39 run) |
| `sub_820B2CC0` Vehicle::ApplyForces | NaNs and infinities anywhere | 23 of 100 | Two NaNs | for review (22:11 run) |
| `sub_820B7B60` CollisionObject::Translate | NaNs and infinities anywhere | 32 of 200 | Two NaNs | for review (22:11 run) |
| `sub_820B7B60` CollisionObject::Translate | one NaN meeting its own negation | 1 of 1 | Own negation | for review (22:11 run) |

### `collision.md`

Every function has a "NaNs and infinities anywhere" group (run
2026-10-22 22:11).

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| BoxShape::FaceDirection | NaNs and infinities anywhere | 2 of 200 | Two NaNs | for review (22:11 run) |
| BoxShape::Translate | NaNs and infinities anywhere | 2 of 200 | Two NaNs | for review (22:11 run) |
| BoxShape::Place | NaNs and infinities anywhere | 10 of 200 | Two NaNs | for review (22:11 run) |
| CollisionShape::UpdateMotion | NaNs and infinities anywhere | 21 of 200 | Two NaNs | for review (22:11 run) |
| CollisionShape::UpdateMotion | one NaN meeting its own negation | 1 of 1 | Own negation (row +96) | for review (22:11 run) |
| CollisionShape::Place | NaNs and infinities anywhere | 85 of 200 | Two NaNs | for review (22:11 run) |
| CollisionShape::Translate | NaNs and infinities anywhere | 33 of 200 | Two NaNs | for review (22:11 run) |
| CollisionShape::Translate | one NaN meeting its own negation | 1 of 1 | Own negation (row +96) | for review (22:11 run) |
| CollisionShape::SetUp | NaNs and infinities anywhere | 7 of 100 | Two NaNs | for review (22:11 run) |
| BoxShape::RelativePlacement | NaNs and infinities anywhere | 17 of 200 | Two NaNs | for review (22:11 run) |
| BoxShape::RelativeCenter | NaNs and infinities anywhere | 1 of 200 | Two NaNs | for review (22:11 run) |
| BoxBox::BlockCorners | NaNs and infinities anywhere | 1 of 200 | Two NaNs | for review (22:11 run) |
| BoxBox::Separated | NaNs and infinities anywhere | 96 of 200 | Two NaNs | for review (22:11 run) |
| BoxBox::SeparatedAfterMove | NaNs and infinities anywhere | 17 of 200 | Two NaNs | for review (22:11 run) |
| Query::Separated | NaNs and infinities anywhere | 96 of 200 | Two NaNs | for review (22:11 run) |
| Query::SeparatedAfterMove | NaNs and infinities anywhere | 17 of 200 | Two NaNs | for review (22:11 run) |
| BoxBox::FindImpact | NaNs and infinities anywhere | 1 of 200 | Two NaNs | for review (22:11 run) |
| Query::FindImpact | NaNs and infinities anywhere | 1 of 200 | Two NaNs | for review (22:11 run) |
| `sub_820BB868` CollisionPair::Respond | a fixed body meeting the impulse | 10 of 44 | Stack leftovers | for review (22:11 run) |
| `sub_820BB868` CollisionPair::Respond | one NaN anywhere | 4 of 158 | Two NaNs (signs only) | for review (22:11 run) |
| `sub_820BB868` CollisionPair::Respond | one NaN anywhere, a fixed body meeting the impulse | 6 of 42 | Stack leftovers, Two NaNs | for review (22:11 run) |
| `sub_820BBE30` CollisionPair::Solve | one NaN anywhere, a fixed body meeting the impulse | 2 of 42 | Stack leftovers, Two NaNs | for review (22:11 run) |

### `collision-world.md`

The functions going through a normal have "one NaN in a vertex" and "out
not 16-aligned" groups (run 2026-10-22 22:11, migration 1); those of
TrackMesh::Ground, GroundProbe, WheelGround and TrackCollision::Ground
were at 0.

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_820F7468` TrackMesh::TriangleNormal | out not 16-aligned | 11 of 28 | Unaligned out | for review (22:11 run) |
| `sub_820F7468` TrackMesh::TriangleNormal | one NaN in a vertex | 4 of 91 | Negated NaN | for review (22:11 run) |
| `sub_820F7468` TrackMesh::TriangleNormal | one NaN in a vertex, out not 16-aligned | 4 of 9 | Negated NaN, Unaligned out | for review (22:11 run) |
| `sub_820F7590` TrackMesh::QuadHeight | out not 16-aligned | 11 of 28 | Unaligned out | for review (22:11 run) |
| `sub_820F7590` TrackMesh::QuadHeight | one NaN in a vertex | 9 of 91 | Negated NaN | for review (22:11 run) |
| `sub_820F7590` TrackMesh::QuadHeight | one NaN in a vertex, out not 16-aligned | 6 of 9 | Negated NaN, Unaligned out | for review (22:11 run) |
| `sub_820FDC18` WallQuery::TestCell | one NaN in a vertex | 1 of 300 | Two NaNs (the word +1464's sign) | for review (22:11 run) |
| `sub_820FDF38` TrackCollision::QueryWalls | one NaN in a vertex | 1 of 300 | Two NaNs (the word +1464's sign) | for review (22:11 run) |
