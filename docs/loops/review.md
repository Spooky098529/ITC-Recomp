# For the user's review

What the B7 loop's runs left for the user to review before a merge: the
function files' rows added or changed, and the choices made in them
(behavior kept or left on the recompiled path, corrections to a map).
Kept across runs (not started afresh like `b7-progress.md`), newest run
first; the lead adds to the current run's section as reports come in.
Harness groups expected to differ are in `expected-differences.md`, the
functions to check in a drive in the progress file.

When the user has reviewed a run's section, its heading gets
"(reviewed <date>)"; the section stays for the record.

## Run 2026-10-23 01:52 (`b7-loop-23-10-26`)

- `sound-cues.md` (new, 87 rows; 4 moved from `functions.md`:
  `sub_8259D1A8`, `sub_821184E8`, `sub_82118FB0`, `sub_82118C18`), 8 new
  `known-data.md` rows (the sound manager, the audio engine, the bank
  pair and holders, the sound, the emitter, the listener, the XACT and
  XAudio engines); 4 leads added (XACT's body, X3DAudio and the engine
  sounds, the data tables, a file-layer helper). For review: XACT's
  vtable slots named only "probably <method>"; the intro records the
  user's call that XACT will probably be translated (it replaces "keep
  the game's XACT" in `native-audio.md`'s decisions, not yet edited
  there); the 3D path never ran in play.
  Migration 1's 30 rows (7 Native; the intro's "Native so far"). For
  review: X3DAudioInitialize (`sub_82480710`, a library entry point,
  pure math) translated; it doesn't store the speed at r1+28 in its
  caller's frame as the original does (no caller reads it);
  SoundBankHolderDelete leaves out the call to the empty `sub_82106540`;
  the bank slot functions run the original where it traps, and the
  allocating ones unless the native heap is on; `sub_8259D438` runs the
  original when a handle is to be closed (never in play). The message
  handler `sub_82119188` left recompiled (guest vtable calls); the rest
  waits on XACT (the cue and engine calls through its vtables), the
  file layer and the string helpers; `sub_82092B78` (a std allocator)
  has no row anywhere.
  `xact.md` (new, 149 rows: the engine with Initialize, DoWork, the
  bank creation and categories, its global settings and audio callback
  `sub_825C0248` run by XAudio on its own thread, the sound and wave
  banks, the cue, sound and fader), `known-data.md`'s XACT objects, the
  `.xsb` and `.xwb` layouts, volume tables and clock globals; 4 leads
  (the rest of XACT, about 265 functions; the voice class with XAudio;
  shared helpers). For review: names follow the XACT3 interfaces where
  slots and arguments make them clear, else "probably"; the boundary
  named in the intro (XAudio's voices and callback, the kernel's
  critical sections and clock, XMemAlloc); a correction to
  `sound-cues.md` and `known-data.md` (the cue's slot +60 is
  GetProperties, writing the emitter's +136 block's first 14 bytes, not
  3D settings). The mapping agent read the vtables from `default.xex`
  decrypted and decompressed into its scratchpad with Python's
  `cryptography` and the retail key (nothing of it in the repo).
  Migration 1's 55 rows (46 Overridden, 9 Native; the intro's
  paragraphs on how the translation is set up). For review: a function
  calling its own object's vtable slot runs natively only while the
  slot holds the already-native function (GetHeader at sound bank +64,
  GetEntry and GetSeekTable at wave bank +16 and +20, AddListener at
  the bank interface's +0), else hands off to the original before
  changing anything; partial ports taking only the paths seen in play
  (`sub_825CA980` only a sound without tracks, `sub_825CC1D0` only an
  idle fader, `sub_825C8230` hands complex cues to the original);
  allocating functions run the original unless `--native_heap` is on.
  Left recompiled: the engine's critical section (no native lock in
  `src/`; the core's), the global settings' unmapped vtable slots,
  calls through vtables into recompiled code, unmapped callees.

## Run 2026-10-22 22:11 (`b7-loop-22-10-26-2`)

- `collision-world.md` (new, 43 rows), `known-data.md`'s mesh, route,
  course table and track position rows; corrections: `car-physics.md`'s
  CollisionObject::Collide (the first query's arguments: r3 the
  records, r4 the shape record), `collision.md`'s box words +160 (the
  lower corners' track positions), `sub_820C0168` probably the car's own
  track position; 5 leads added. Migration 1's 29 rows (and the intro:
  its constants; the up vector at `0x82638280`, not `0x82628280`). For
  review: a new `memory::IsPages`; the native normalize reproduces the
  vector unit's; `TrackPosition::SearchLinked` (never ran in play) writes
  -1 past its six halfwords onto the route's first at `0x826C5B10`, as
  the game does, kept; four functions ported that never ran in play are
  described in FindCell's row; the "Negated NaN" groups
  (`expected-differences.md`): ours flips a NaN's sign where the
  translated code doesn't. In four functions (`sub_820F6940`,
  `sub_820F8618`, `sub_820FEBD0`, `sub_820FC188`) the print comes after
  a guard that falls back to the recompiled code (the validation agent
  didn't count them as missing).
  Migration 2's rows: 11 in `collision-world.md` (the wall query,
  Unload; `sub_820FC870` named WallQuery::ModeSkips, ours), 3 in
  `car-physics.md` (CollisionWorld::Destroy, CollisionObject's
  destructors, under `--native_collision_world`: their only caller is
  Unload). For review: the wall hits are reached only by the harness (the
  AI cars don't touch walls; every path but EdgesAcrossWall's
  equal-lengths sum); TestCell and QueryWalls' harnesses don't compare r3
  (their callers don't read it); TestCell's up vector at `0x826382B0`,
  not `0x826282B0`; prints moved before the guards in GatherCells,
  TrackMesh::Free, TrackRoute::Free; a new `CollisionShapeDestroyable`
  in `collision.h`.

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
  meeting the impulse" groups (`expected-differences.md`).

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

## Run 2026-10-22 14:39 (`b7-loop-22-10-26`)

- `car-physics.md`: migration 1's 41 rows (names marked "ours" where the
  map had none) and the intro (the identity quaternion is at
  `0x826381B0`, not the map's `0x825F81B0`); `vehicle-accessors.md`:
  5 rows. For review: the vector library fixed (ee810a9): ThinLTO could
  move `dpps` out of its flush-to-zero window, found by the car physics'
  harness ("tiny values" groups added, at 0); ContactImpulse's harness
  has a "one NaN meeting its own negation (expected to differ)" group
  (3 cases, 2 differ); the "numbers" groups keep values below 10^6 (no
  infinities made). ContactImpulseWith runs the original for a fixed
  body (whose term the original reads from its stack's leftovers).
  Migration 2's 19 rows. For review: UpdateSteering's harness doesn't
  compare r3 (on the tan path the original leaves `_ctrlfp`'s word
  there; the step sets r3 again); the adapters run the original for
  UpdateSteering when tan would take its `_except1` path, and for
  UpdateTorqueSplit in mode 2 in a forward gear (neither seen in play).
  Migration 3's rows (38 in `car-physics.md`, one new: `sub_820BAA78`
  CollisionShape::Init; `sub_820B4CF0`'s r4 corrected: clear means
  automatic shifts) and 2 in `vehicle-accessors.md`.
  Migration 4's 15 rows in `car-physics.md` and 4 in
  `vehicle-accessors.md`. For review: RemovePair copies a moved pair
  with `c_runtime::Memcpy` only outside the physical views and for
  ranges that don't overlap; otherwise RemoveObject's and RemoveGroups'
  adapters run the original removals from the start (the same result).
- `car-physics.md` (new, 129 rows), `known-data.md`'s new rows, 5 leads
  added (the collision narrow phase and shapes, and others).
- `race-overlays.md`: the matrix-math lead's rows (UpdateLightSprites,
  UpdateParticlesB, UpdateParticlesC, UpdateSmoke, Smoke::SetFrame,
  ModelMatrix, ParticlesB::Emit's f1 corrected to +6256) and 23 adapters
  removed (Native). For review: native callers of Vec4::Transform go
  through a new `debug::Vec4Transform`, which keeps its override's
  `--debug_hide` and frame-state checks; UpdateSmoke stores its size word
  quieted when it comes from the table (as the translated code, which
  keeps it as a double); its harness never uses a smoke capacity of -1
  (the original would copy into 2^32 - 1 emitters).
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

## Run 2026-10-22 10:47 (`b7-loop-22-10-26`)

- `c-runtime.md`, `ui-movies.md`, `ui-movies-game.md`, `sprites.md`: the
  adapters lead's rows (Overridden -> Native, and notes on the 4 kept).
- `race-overlays.md` (new, 154 rows; `sub_8215DEA0` moved from
  `functions.md`), `sprites.md`'s intro, `known-data.md`'s new rows;
  the first migration's 34 rows (Overridden). For review: RacerId's
  adapter (`sub_82126AE0`) sign-extends r3 where the original
  zero-extends a found id (only the upper 32 bits; callers compare 32).
  The second migration's 6 rows and the 9 names.
  The third migration's 10 rows (4 new). For review: NaN-only harness
  results depend on which operand the compiler kept in the translated
  vector adds: Smoke::Emit keeps the first operand's NaN (as the Xenon
  does), the sparks a plain `addps` matching how this build compiled
  theirs; a rebuild could bring back NaN-only differences there.
  The fourth migration's 32 rows, and `sub_820C0128`'s notes in
  `functions.md`. For review: on the native SpeedKmh path the state
  capture (`--capture_dir`) no longer hears about the vehicle (the
  game's other callers still pass it on).
  The fifth migration's 8 rows (4 Overridden, `sub_8215E078` Merged, 3
  Native). `attract_race.sh` now counts seconds logged at 31 ticks/s too
  (it reported 19 for a 22-second race).
