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
- **Mutex for a critical section:** the user's call (2026-10-24) to
  replace a kernel critical section with a `std::mutex`: the recompiled
  code initializes the critical section in the guest object, ours leaves
  those bytes as the allocation cleared them. Everything else in the
  object matches.
- **Host semaphore:** the user's call (2026-10-24) to run the video
  output's device lock over a host `std::binary_semaphore`: the native
  constructor no longer writes the guest semaphore's 40 bytes at +224
  (an event and a critical section); nothing else reads them (its
  release `sub_8258DBA8` never runs).
- **Packets nothing reads:** the graphics rule (the user's, standing):
  a packet written straight into the command buffer that nothing reads
  is Dropped, so the command buffer's words and its write position (+40)
  differ where the original writes one (here: the window-scissor packets
  of `Device::SetScreenScissor`, reached from the scissor and viewport
  setters). Every other output matches.
- **Flush packet dropped:** the user's call (2026-10-24) to drop
  `Resource::Lock`'s cache flush packet: the recompiled code queues the
  flush (widening the device's pending flush range at +11560 through
  `sub_8229DDD8`, or writing the packet past the command buffer's write
  position); ours leaves them. Only cases taking the packet path (flags
  without 0x2 or 0x10) differ; the header, ranges, lock count and r3
  match. The renderer skips coherency packets.
- **Uninitialized stack byte:** the original fills a pad byte from an
  uninitialized stack byte, the native code writes 0; how many cases
  differ depends on what the callers left on the stack (0 while the run
  loop was recompiled, every case since `main` and `App::Run` are native,
  2026-10-25: the byte is now 0x33).
- **Unwritten stack bytes copied:** a function builds a record on its
  stack, its callees leave some of its bytes unwritten, and it copies
  the whole record into a guest object; in the original those bytes are
  what earlier calls left there (their register saves), which native
  callees don't write on the guest stack. Here: `Racers::SetStart`'s 16
  traffic records, 42 bytes each (the car object's +34, the record's
  +3928 to +3935 and others) copied into the race data. The harness
  finds the bytes by running the callees natively over two fills and
  compares them in their own group; every other output matches.
  Whether the game reads those bytes isn't known.
- **Unwritten header bytes in the frame:** a function reads a record
  into its own stack frame and its callee leaves some of its bytes
  unwritten; in the original they hold what earlier calls left there
  (their register saves), which native callees don't write on the guest
  stack. Nothing reads them, and nothing copies them out. Here:
  `CarModel::OnDecalLoaded`'s TGA headers (the reader never writes +3
  and +9; in `Vinyl::DrawDecals`' frame at +192 and +224), `DrawDecal`
  reading only the width and height.
- **Constant order:** the HLSL compiler (`sub_822B8F10`) places the HSV
  gradient's literal constants (1 to 6, source `82071528`) in another
  order when it compiles during a race than at boot, where every compile
  gave the order stored (`--use_hardcoded_shaders`, the boot's bytes);
  probably state of its own, such as heap addresses. The same program
  either way: only the constants' registers differ.

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

### `graphics-resources.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_822B5EF8` Edram::Alloc | the allocator made: its critical section | 8 of 8 | Mutex for a critical section (bytes +0 to +27) | for review (03:25 run) |
| `sub_822A63F8` Resource::Lock | its command buffer | 448 of 1000 | Flush packet dropped (the pending range at +11560) | for review (03:25 run) |

### `video-output.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_8258DB18` VideoOutput::VideoOutput | its semaphore at +224 | 4 of 4 | Host semaphore | for review (03:25 run) |

### `device-state.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_822A3450` Device::SetScissorRect | the command buffer | 300 of 300 | Packets nothing reads | for review (03:25 run) |
| `sub_822A3D20` Device::SetRenderState_ScissorTestEnable | the command buffer | 200 of 200 | Packets nothing reads | for review (03:25 run) |
| `sub_822A3518` Device::SetViewport | the command buffer | 352 of 400 | Packets nothing reads | for review (03:25 run) |
| `sub_822A3C88` Device::SetSurfaceInfo | the command buffer | 175 of 200 | Packets nothing reads | for review (03:25 run) |
| `sub_822A3D30` Device::SetRenderTarget | the command buffer | 131 of 600 | Packets nothing reads | for review (03:25 run) |
| `sub_822A4340` Device::SetRenderTarget_External | the command buffer | 25 of 100 | Packets nothing reads | for review (03:25 run) |
| `sub_822A4060` Device::SetDepthStencilSurface | the command buffer | 123 of 500 | Packets nothing reads | for review (03:25 run) |
| `sub_8229B7D8` | the command buffer | 200 of 200 | Packets nothing reads | for review (03:25 run) |
| `sub_822A3B90` Device::DirtyAllState | the command buffer | 200 of 200 | Packets nothing reads | for review (03:25 run) |
| `sub_822A8A60` Device::SetShaderGprAllocation | the command buffer | 300 of 300 | Packets nothing reads | for review (03:25 run) |
| `sub_822A3A20` Device::SetGammaRamp | the command buffer | 40 of 60 | Packets nothing reads | for review (03:25 run) |
| `sub_822ACB90` Device::SetDefaultState | the command buffer | 40 of 40 | Packets nothing reads | for review (03:25 run) |
| `sub_822A6A88` VertexDeclaration::FromFvf | the end marker's pad byte | 400 of 400 | Uninitialized stack byte | for review (01:17 run) |
| `sub_822AA0E8` Device::SetFVF | the end marker's pad byte | 100 of 100 | Uninitialized stack byte | for review (01:17 run) |

### `functions.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_822B8F10` D3DXCompileShader | the HSV gradient's constant order | 1 of 1 | Constant order | for review (2026-10-24) |

### `race-setup.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_82115D98` Race::SetUp | the traffic records' unwritten bytes | 46 of 48 (45 or 46 from run to run) | Unwritten stack bytes copied | for review (23:42 run) |

### `car-models.md`

| Function | Group | Differ | Reason | Review |
|---|---|---|---|---|
| `sub_820D5750` CarModel::OnDecalLoaded | the TGA headers' unwritten bytes | 16 of 16 | Unwritten header bytes in the frame | for review (08:36 run) |
