# Known functions: the vehicle

The **vehicle** code above the car physics (`820BF000`-`820C3C00`, with
the engine's power curve at `820CE1D8`-`820CE7F0` and a route step at
`82170F90`): the seven 6,272-byte vehicles at `[0x826C2E34]` made,
constructed and destroyed (`Vehicles::Create`, `Vehicles::Destroy`); a
vehicle set up from a car object (`Vehicle::SetUp`, `Vehicle::ResetForCar`:
the car data at vehicle +3376 filled by the car loader, the car object
kept at +5712, the physics set up, the surface grip records, the status
record at +6160); its tick (`Vehicle::Tick`, once a tick for each racer
from the race's states: the car input record applied to the steering,
throttle, brake and handbrake setters (`Vehicle::ApplyInput`), two physics
steps of 1/60 s each, the track positions updated, then the status record:
the speed in km/h, the place on the route, wrong way, the distance, the
engine's two temperatures and the brakes' fade); the vehicle placed on the
route (`Vehicle::PlaceOnRoute`); and a few queries the race and camera
code make. The car physics under it (the physics step, the rigid body,
the collision world, the car loader `Vehicle::SetupFromCar` with
`Vehicle::ApplyTuning` and `CarData::ApplySettings`) are in
`car-physics.md`; the fields are in `docs/known-data.md` (the vehicle's
physics, "vehicle status", "power curve", "track position").

Shared with other files, which keep their rows: the car physics'
functions (`car-physics.md`: `Vehicle::Vehicle`, `CarData::CarData`,
`Vehicle::SetUpPhysics`, `ResetPhysics`, `DestroyPhysics`,
`RefreshFromBody`, `PhysicsStep`, `SyncFromBody`, `EngineTorque`, the
controls' setters, `CollisionWorld::RemoveObject`, the rigid body's
setters, `CollisionObject::ResetTransform`); the accessors and the vector
library (`vehicle-accessors.md`: `RigidBody::Position`,
`RigidBody::PointVelocity`, `sub_820B6880`, `sub_820C4550`,
`sub_820B99B8`, `Vec3::` and `Vec4::`); the matrix and quaternion math
(`matrix-math.md`: `Quaternion::FromMatrix`, `Quaternion::XAxis`); the
course's route (`track-route.md`: `TrackRoute::SlicePoint`,
`TrackRoute::LanePoint`, `TrackRoute::Normalize`); the track positions
(`collision-world.md`: `TrackPosition::Init`, `TrackPosition::Update`,
`Race::RacerCount`); `Race::RacerVehicle` (`minimap.md`);
`AiPath::BranchMasksFor` (`ai-drivers.md`); `CollisionShape::Current`
(`collision.md`); `memcpy`, `memset`, `cos` and the vector destructor
iterator (`c-runtime.md`); `operator new` and `operator delete`
(`heap.md`); the empty destructor `sub_82106540` (`audio.md`).

Not here: the callers, which are other subsystems' (the race's states
and manager at `82165000`-`82175000`, the camera modes at
`82152028`-`82156CC0`, the car sounds' `sub_8210E5C0`, the race code at
`82104310`-`82104E00`; leads in `../loops/leads.md`). Callees never seen
in play (no rows): the shift and digital-control setters under
`Vehicle::ApplyInput` (`sub_820B0098`, `sub_820B00A8`, `sub_820B00B8`:
bytes +2284, +2285, +2286 = 1; `sub_820B03E0`, `sub_820B0428`: the
digital brake and throttle ramps; `sub_820B6170`: the wheel's steering,
with `sub_820B0668` and `sub_820B3E98` under it) and the route offset
`sub_820FA8F8` under `Vehicle::PlaceOnRoute` (only with a nonzero offset
across the road).

Native so far (B7 loop, 2026-10-27, `--native_vehicle`, on by default;
plain C++ in `src/vehicle/`, adapters and register steps in
`src/hooks/vehicle_native.cpp` and `.h`, harness `vehicle`): the leaves,
the route position's step, the track positions' update and links,
`Vehicle::Refresh`, and the vehicles' lifetime (`Vehicles::Create`,
`Destroy`, `Vehicle::RemoveFromWorld`, only while `--native_heap` routes
the game's heap to the allocation layer, as the car physics' lifetime).
The originals' temporaries go in their frames at the same offsets
(register saves and back chains not written); a callee that would refuse
(a shape that isn't a box, the world's callback not the game's, a block
the layer didn't give) is checked first, with nothing done, and the
original runs. The harness reuses the collision world harness's course
(`BuildTestCourse`) and the car physics harness's fake world and vehicle
(`car_objects::`), exported from their files.

Floats named by address are in `.rdata` (read from the image):
`0x82000A30` 1.0, `0x82000A34` 0.0, `0x82001BE0` -1.0, `0x82001BDC`
1000.0, `0x82001E8C` 80.0, `0x82002204` 0.05, `0x8200222C` 0.01,
`0x820027A8` 1.5, `0x82002968` 0.25, `0x8200296C` 0.3, `0x820029D8`
0.001, `0x820029E0` 3.6, `0x820029EC` 1/30, `0x82002A0C` 100.0,
`0x82002BBC` 0.1, `0x82002BC0` 0.5, `0x82002BD4` 1/32,767,
`0x82002BD8` 1/3.6, `0x82002CF8` 1/3,600, `0x82003314` 400.0,
`0x82003380` 1/7,018.7 (probably torque × rpm to horsepower),
`0x82003384` 800.0; the doubles `0x82002CD8` π/4 and `0x82002CE0` π/3.
The float at `0x82601838` is in `.data` (0.016666668, 1/60, in the image;
not checked for writes, so probably but not surely constant).

Status:

- **Native**: plain C++ in `itc_recomp/src/` that doesn't use the
  PowerPC context (`ctx`): normal arguments and host structs (it may
  still read and write guest memory through `src/guest.h` while
  recompiled code shares the data). The game reaches it without `ctx`,
  from native or overridden callers, and the recompiled original is no
  longer called (except behind a fallback flag or to check against it).
  No `REX_FUNC` override of it exists, not even an adapter kept only for
  a harness (that's Overridden until the adapter goes).
  The end state for every function.
- **Overridden**: an `extern "C" REX_FUNC(sub_XXXXXXXX)` override in
  `itc_recomp/src/` (or a macro defining one) does the work instead of
  the original: a thin adapter in front of native code, or a body that
  still uses `ctx` (calling guest code, reading registers). It may call
  the original when its flag is off, or to check itself against it.
- **Wrapped**: an override that does something of its own (skipping
  render-only passes, logging, timing) and still calls the original
  (`__imp__sub_XXXXXXXX`) for the work.
- **Dropped**: its work isn't needed on PC (hardware set-up, kernel calls
  the SDK already stubbed), so it no longer runs: an override in
  `itc_recomp/src/` replaces it with nothing (or a fixed return value), or
  its callers, made native, leave the call out (no override then; the row
  says which caller). Logs, a harness or the renderer's frame dumps have
  shown that nothing reads what it did. The original is no longer called,
  except behind a fallback flag while the drop is being checked. A
  function the game never calls in the first place isn't Dropped: its
  status stays blank and its row says it's never called.
- **Merged**: its work is done inside a native caller's plain C++ (a
  few lines only that caller needs), so it has no function or override of
  its own in `itc_recomp/src/`. The row names the caller(s) and the
  harness that covers the work. The original is no longer called, except
  behind the caller's fallback flag.
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

A function usually moves from recompiled or Wrapped to Overridden to
Native as the `ctx` edge moves up through its callers, or straight to
Dropped when its work isn't needed.
`itc_recomp/tools/check_function_statuses.py` checks this column, in
every file of the tables, against `src/` (an override marked Native, a
status with no override, an override with no row, a row in two files);
run it after changing either, on any branch.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_820C3BD0` | `Vehicles::Create` | Overridden | No arguments. Allocates guest memory: 43,904 bytes (`operator new` `sub_825A6880`); when it isn't 0, the seven vehicles in it constructed (`Vehicle::Construct`, 6,272 bytes apart, an inline loop); `[0x826C2E34]` = the block (or 0), then `memset`(the block, 0, 43,904) (`sub_825A71D0`): the constructors' work is zeroed right after (and a failed allocation would `memset` address 0). Only caller `sub_82173A90` (the race's states, a lead). Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`, only while `--native_heap` routes the heap to the allocation layer): `vehicle::VehiclesCreate` with `Vehicle::Construct`'s plain C++, the memset kept (address 0 too when the block is 0, as the original); harness `vehicle`. |
| `sub_820C30D8` | `Vehicle::Construct` | Native | r3 = a vehicle (6,272 bytes); returns r3. The whole vehicle's constructor: `Vehicle::Vehicle` (`sub_820B42C8`, the physics part, `car-physics.md`), `CarData::CarData` (`sub_820AEF80`) on +3376, then the w of +5248 to +5296 (+5260, +5276, +5292, +5308) = 1.0 (`0x82000A30`) and in the four 96-byte track positions from +5328 the w of +16, +32, +48, +64 = 1.0. Only caller `Vehicles::Create` (the zeroing after it undoes this). Read in full. Plain C++ since the B7 loop (2026-10-27): `vehicle::VehicleConstructAll`, called by `Vehicles::Create`'s; no adapter (its only caller is native; `--native_vehicle` switches that caller). `Vehicle::Vehicle` and `CarData::CarData` as plain C++ with their frames below it. |
| `sub_820C3B78` | `Vehicles::Destroy` | Overridden | No arguments. When `[0x826C2E34]` isn't 0: `Vehicles::DestroyAll` on it, then `operator delete` (`sub_825A70E8`, frees guest memory); `[0x826C2E34]` = 0 either way. Callers `sub_82171250` (seen) and `sub_82171308` (not seen), the race's states. Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`, only while `--native_heap` routes the heap to the allocation layer): `vehicle::VehiclesDestroy`, `Vehicles::DestroyAll`'s work merged in; where a vehicle's `DestroyPhysics` or the block's delete wouldn't be native the original runs (all seven checked first). Harness `vehicle`. |
| `sub_820C21E0` | `Vehicles::DestroyAll` | Merged | r3 = the vehicles' block: the seven vehicles from the last to the first (6,272 bytes apart): each one's four track positions (+5328, 96 bytes) through the vector destructor iterator (`sub_820900C0`, count 4, with the empty destructor `sub_82106540`), then `Vehicle::DestroyPhysics` (`sub_820B0E70`). Only caller `Vehicles::Destroy`. Read in full. Merged since the B7 loop (2026-10-27) into `vehicle::VehiclesDestroy` (`Vehicles::Destroy`'s plain C++; the iterator's native version with the empty destructor, `DestroyPhysics` as plain C++); harness `vehicle` (`Vehicles::Destroy`'s cases). |
| `sub_820C3150` | `Vehicle::SetUp` | | r3 = a vehicle, r4 = a car object: the vehicle set up for a race from the car. Block 2 = `sub_820C4550`(car) (car +1888); +6152 = 0, +5712 = the car, +6148 = car +3888 (the car's flags); `Vehicle::SetupFromCar` (`sub_820C27E8`, the car loader) into the car data at +3376; `VehicleStatus::Reset` on +6160; `Vehicle::SetUpPhysics` (`sub_820B5670`; byte 1, groups 7); `Vehicle::LinkTrackPositions`; byte +2664 = 1; `Vehicle::SetValue2388` (`sub_820B3C20`) with (0.0, 0) when the car's signed byte +76 is 0, else (byte × 0.1, 1); byte +2386 = (car byte +77 isn't 0); +2524 = 6 with block 2's byte +131 = 1, else the car's word +1672, and byte +2512 = 1 when that's 6 or the word is 3 or more (else 0); with block 2's byte +69 = 1: the car's engine stage (+1656) 8 or more sets car data byte +1605 = 1, byte +2512 = 1 and +2524 = at least 3, else car data byte +1605 = 0; +2372 \|= 2; byte +2249 = (car +3892 isn't 0); `Vehicle::SetUpSurfaceGrip`(vehicle, car); bytes +2380 = 1, +2384 = 0; from two tables built on the stack by the car's word +1676 (unchecked, 0 to 6): +2416 = 1.0, 1.5, 1.55, 1.6, 1.65, 1.7, 1.75 and +2420 = 0, 390, 450, 510, 570, 630, 690 (as floats; probably the nitrous' part: `Vehicle::ApplyInput` toggles byte +2424 only when +2420 is above 0.0); with a +344 box, `sub_820B99B8`(box, 2 when the car's flags have 0x4, else 1 with 0x2, else 0) (its halfword +112). Callers `sub_82171718`, `sub_821724F8`, `sub_82173A90` (seen; the race's states and set-up) and `sub_821706E8` (not seen). Read in full. Blocked only by `VehicleStatus::Reset`'s power curve (here) and its own callees here. |
| `sub_820C33B8` | `Vehicle::ResetForCar` | | r3 = a vehicle, r4 = a car object: as `Vehicle::SetUp`, but `Vehicle::ResetPhysics` (`sub_820B5B08`; groups 7) instead of `SetUpPhysics` (the rigid body kept), and with block 2's byte +69 = 1 an engine stage of 8 or more sets only car data byte +1605 = 1 (bytes +2512 and +2524 as left before). The same callers as `Vehicle::SetUp`. Read in full (compared with `Vehicle::SetUp` instruction by instruction). |
| `sub_820C1E00` | `Vehicle::SetUpSurfaceGrip` | | r3 = a vehicle, r4 = a car object. +2496 = 0 when the car's flags (+3888) have 0x2 or 0x4, else 1. For each axle (front: the car's tire type +1712 and grade +1716; rear: +1720, +1724; the grade taken as 0 when the type is 0): two factors from the tire type's pairs in the tables at `0x826014A0` and `0x82601560` (8 bytes a type) lerped by grade × 0.25 and taken × 0.04 + 0.8, then nine 24-byte records (by surface, probably; nine floats a type in the table at `0x82601580`, × the second factor for records 1 and 8, the first for the others, × the float at `0x82601488`) built on the stack from four stack tables and copied (`memcpy`, 24 bytes) into +5716 + 24 j (front) or +5932 + 24 j (rear), with pointers to them at +2668 + 4 j and +2796 + 4 j: +0, +12, +20 from the stack tables, +4 a value × 0.01, +8 the grip, +16 grip² / (grade × 0.25 + 1.0) × 0.25. The tables are in `.data` (not checked for writes). From `Vehicle::SetUp`, `Vehicle::ResetForCar`. Not read in detail (what the records' fields mean isn't known). |
| `sub_820C1D08` | `Vehicle::LinkTrackPositions` | Overridden | r3 = a vehicle: the shape = `CollisionShape::Current`(collision object +340's +32) (`sub_820BAE38`); +2984 = vehicle + 5232 (the car's track position); for each wheel i: the wheel's +20 (vehicle +436 + 368 i) = vehicle + 5328 + 96 i (its track position) and the shape's slot 20 (+80, through its vtable) with (i, that track position) (`BoxShape::SetCacheWord`, `collision.md`: the box's corner track positions). 5 callers: `Vehicle::SetUp`, `Vehicle::ResetForCar` and `sub_821046F8`, `sub_82104898`, `sub_82104D40` (the race code, a lead). Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::VehicleLinkTrackPositions`; the shape's slot 20 is the box's `SetCacheWord` as plain C++, and where the record's template or current copy isn't a box the original runs (decided first). Harness `vehicle` (real boxes). |
| `sub_820BFC48` | `VehicleStatus::Reset` | | r3 = a vehicle status record (the vehicle's +6160), r4 = the vehicle: +0, +76, +84, +88 = 0.0; +8, +12, +72, +80 = 0; halfwords +4, +6 = -1; +92, +96 = 80.0 (`0x82001E8C`; probably the engine's temperatures); +100 = 1.0; then a power curve on its stack (1,064 bytes at r1 + 80): `PowerCurve::FromEngine`(curve, vehicle + 4644, the car data's +1268), `PowerCurve::ShiftPeak`(curve, 0), and +104 = the curve's +1048 (the car data's +1420). From `Vehicle::SetUp`, `Vehicle::ResetForCar`. Read in full. |
| `sub_820CE7D8` | `PowerCurve::FromEngine` | | r3 = a power curve (1,064 bytes), r4 = the car data + 1268: the curve's +1044 = r4 + 12 (the car data's +1280), then `PowerCurve::Build`(r3, r4 + 24) (tail call; the torque curve at the car data's +1292). 12 callers, 4 seen: `VehicleStatus::Reset`, `sub_821DF788`, `sub_821E7368`, `sub_82216B08` (probably the front end's tuning screens). Read in full. |
| `sub_820CE430` | `PowerCurve::Build` | | r3 = a power curve, r4 = 33 floats (the car data's torque curve +1292, then +1420): clears +0 to +511 and +512 to +1023 (32 entries of 16 bytes each); +1048 = r4 + 128 (the car data's +1420, the torque's scale), +1040 = 800.0, +1028 = 0; for the 32 points i: rpm (i + 1) × 400.0 into entry i's +0; a point at 0.01 or more counts in +1028, its torque (the point × +1048) into +4 and its power (torque × rpm × `0x82003380`) into +8, the largest power's index into +1024; a point below 0.01 gets its torque from the two entries before it (2 × the last - the one before, at least 0.0) and the power from it. Then +1032 = trunc((800.0 - the peak entry's rpm) × 0.01), at least -20, and +1036 = trunc((+1044 - the peak's rpm) × 0.01), at most 20 (how far the peak may move, in 100 rpm steps); `PowerCurve::ShiftPeak`(r3, 0). Only caller `PowerCurve::FromEngine`. Read; the branch for points below 0.01 not in detail. |
| `sub_820CE1D8` | `PowerCurve::ShiftPeak` | | r3 = a power curve, r4 = a shift (in 100 rpm steps): +1052 = (the peak's index + 1) × 400.0 + r4 × 100.0, clamped to [+1040, +1044]; +1056, +1060 = 0.0; the rpm axis remapped on its stack (below r1, no frame: a leaf using 512 bytes under the stack pointer): points below the peak scaled to end at the new peak, those after it stretched from there to the end; then for each of the +1028 points, its torque looked up (lerped) at its rpm in the remapped axis, into the second table (+512 + 16 i: +0 the rpm, +4 the torque, +8 the power × `0x82003380`), with the largest power into +1056 and its rpm into +1060. 14 callers, 4 seen (`PowerCurve::Build`, `VehicleStatus::Reset`, `sub_821DF788`, `sub_821E7368`; probably the tuning screens' power band). Read; the remapping not in detail. A native version keeps the stack words it leaves below r1. |
| `sub_820C3020` | `Vehicle::Tick` | | r3 = a vehicle, r4 = the racer's index (its low byte), r5 = a car object or 0, r6 = a flag (both passed to `VehicleStatus::Update`). Its input = `[0x826C5B30 + 4 × index]` (the car input record); the status record's +0 = the speed (+48) × 3.6 (km/h), halfwords +4, +6 = the car's route position (+5236, +5238). Then twice: `Vehicle::ApplyInput`(vehicle, input), the input's flags lose 0x1, 0x2 and 0x10000 (the gear changes and the nitrous toggle act once), `Vehicle::PhysicsStep` (`sub_820B6498`) with f1 = the float at `0x82601838` (1/60), `Vehicle::UpdateTrackPositions`. Then `VehicleStatus::Update`(vehicle + 6160, vehicle, r5, r6). Callers `sub_82165E20`, `sub_821662E0`, `sub_821714D8`, `sub_82173A90` (the race's states; once a tick for each racer, probably). Read in full. |
| `sub_820BFD30` | `Vehicle::ApplyInput` | Overridden | r3 = a vehicle, r4 = a car input record (`../known-data.md`): dt = the float at `0x82601838` (1/60). Steering by the input's kind (byte +17): 0 (the pad): with flags 0x02000000 (digital), `Vehicle::SteerTowards` (`sub_820B5FD8`) with 0.0 and r4 = 1 at -32,767 or r5 = 1 at 32,767, else with the steering (halfword +8) × 1/32,767; 1 (a wheel): `sub_820B6170` (never seen) with the steering, dt and a rate from the settings byte +16; 2 (an AI driver): `Vehicle::SetSteering` (`sub_820B3D10`); 3 and up: none. The brake (halfword +6): with flags 0x20000000, `sub_820B03E0` (never seen; a ramp with a rate from byte +15, full at 32,767), else `Vehicle::SetBrake` (`sub_820B3E08`) with a curve x² + 2(x - x²) × (byte +15 + 10) × 0.05 of x = the input / 32,767. The throttle (halfword +4) the same way with byte +14: `sub_820B0428` (never seen) or `Vehicle::SetThrottle` (`sub_820B3E68`). The handbrake (halfword +10): with flags 0x80000000, `Vehicle::RampHandbrake` (`sub_820B04C0`, dt × 4.0, full at 32,767), else `Vehicle::SetHandbrake` (`sub_820B3E38`) with the halfword as a float. `Vehicle::RampReverseThrottle` (`sub_820B0478`) with dt and flags 0x200. With byte +2249 (set when the car's +3892 isn't 0): flags 0x2000 sets byte +2286 (`sub_820B00B8`); without it, flags 0x1 sets byte +2284 (`sub_820B0098`, shift up) or else 0x2 byte +2285 (`sub_820B00A8`, shift down) (all three never seen). Flags 0x10000 toggles byte +2424: 1 when it was 0 and +2420 is above 0.0, else 0 (probably the nitrous). Only caller `Vehicle::Tick`. Read in full. Ready: its six never-seen callees are leaves or near them (port them with it, or keep the original for those branches). Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::VehicleApplyInput`, the integers converted through its frame's +80 and +88 as the original, the setters as plain C++ (`SteerTowards`' original in its frame where that refuses); its six callees never seen in play are left to the original: where one would run (the input's kind 1, flags 0x20000000 or 0x08000000, or the shift flags) the whole original runs, decided first. Harness `vehicle` (the car physics harness's vehicle region; numbers, and NaNs and infinities anywhere). |
| `sub_820C1D78` | `Vehicle::UpdateTrackPositions` | Overridden | r3 = a vehicle: the car's track position (+5232) point (+16 to +24) = the rigid body's position (`RigidBody::Position` `sub_820B67D8` of +336), `TrackPosition::Update` (`sub_820F8470`); for each wheel i, its track position (+5328 + 96 i) point = the wheel's mount point in the world (+480 + 368 i to +488), `TrackPosition::Update`. Only caller `Vehicle::Tick` (twice a tick). Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::VehicleUpdateTrackPositions`, `TrackPosition::Update` as plain C++ with its frame below (r3 not kept: Update's plain C++ doesn't, and `Vehicle::Tick` doesn't read it); harness `vehicle` (the collision world harness's course). |
| `sub_820C23E8` | `VehicleStatus::Update` | | r3 = a vehicle status record (+6160), r4 = the vehicle, r5 = a car object or 0, r6 = a flag. The car's track position copied to the stack (`memcpy`, 96 bytes); its route position one slice on (`RoutePosition::Step`, 1); the slice's direction from the two slices' centers (`TrackRoute::SlicePoint`, point 0): +32 = it normalized, +68 = how far along it the car is (the body's position, `RigidBody::Position`; 0.0 for a zero-length slice), +48 = the point on the slice, +64 = the distance across from it (negative to one side, `Vec3::CrossW1`'s y below 0.0 gives × -1.0); +16 = the body's x axis (`Quaternion::XAxis` of `sub_820B6880`), its y 0.0, normalized; its dot with the slice's direction against cos(π/4) and cos(π/3) sets and clears +8 (probably wrong way, with hysteresis). With r6: +84 and +88 += the speed × 3.6 × 0.001 × 1/30 (km a tick: probably the trip and the total). With a car object: +92 and +96 (80.0 at the reset) eased toward targets from the rpm (+2228), the engine stage (car +1656) and part (+1664) (probably the water and oil temperatures), +100 from `Vehicle::EngineTorque` (`sub_820B00C8`) and +104. Then the vehicle's +2432 (the brakes' efficiency, read by `Vehicle::BrakeTorques`) lowered by the brake (+292) × the speed × (+3540 - +3544), at least 0.5, and +2428 lowered by +96 × 0.00001 × 0.01, at least 0.7. Callers `Vehicle::Tick`, `sub_821724F8` (seen) and `sub_821706E8` (not seen). Read; the temperatures' part not in detail. |
| `sub_82170F90` | `RoutePosition::Step` | Overridden | r3 = a route position (two halfwords: section, slice), r4 = a step in slices, r5 = a branch kind (halfword): masks = `AiPath::BranchMasksFor`(&kind) (`sub_8213D378`, the kind on the stack), the slice += r4, then `TrackRoute::Normalize`(`0x826C5B10`, r3, masks) (`sub_820F9FE8`, tail position). 6 callers, 5 seen: `VehicleStatus::Update`, `Vehicle::PlaceOnRoute`, `sub_82165E20`, `sub_82173050`, `sub_82173E28`. Read in full. In the race's area (`82170000`-`82175000`) but called from the vehicle; kept here. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::RoutePositionStep`, the kind written into its frame's +80 as the original; `AiPath::BranchMasksFor` and `TrackRoute::Normalize` as plain C++; harness `vehicle` (the track route harness's random routes, the race data's flags). |
| `sub_820C3608` | `Vehicle::PlaceOnRoute` | | r3 = a vehicle, r4 = a route position, f1 = a speed (km/h), f2 = an offset across the road (0.0 in play), r6 = a lane (-1 for none), r7 = the direction (0 reversed). Three route positions on its stack: one slice back, one on (`RoutePosition::Step` with -1 and 1, kind -1) and r4's own; the car's track position (+5232) gets r4's route position and `TrackPosition::Init` (`sub_820F8578`; with a nonzero f2 also `sub_820FA8F8`, never seen). The frame from the course: forward between the slices back and on, across between the slice's two edges (`TrackRoute::SlicePoint`, points 0, 1 and 2; with f2, `sub_820FA8F8` at f2 ± 2.5 instead), each reversed when r7 is 0, normalized and crossed into a matrix (vector instructions, `Matrix::FromRows`), turned into a quaternion (`Quaternion::FromMatrix`) for `RigidBody::SetOrientation`; `Vehicle::SyncFromBody`, `CollisionObject::ResetTransform`. Velocity = forward × f1 × 1000/3600 (`RigidBody::SetVelocity`). Position: with a lane, `TrackRoute::LanePoint` at the slice back; else the slice back's center (`[[0x826C5B10] + 12]` + 4 × section, that record + 84 × slice, its +124 to +132); 0.3 higher, `RigidBody::SetPosition`; `SyncFromBody` and `ResetTransform` again; the four wheels' track positions = the car's (`TrackPosition::Copy` from a `memcpy` of it); `RigidBody::SetAngularVelocity`(0). Callers `sub_82104898`, `sub_82165E20`, `sub_821662E0`, `sub_821724F8`, `sub_82173A90` (seen) and `sub_821706E8`. Read; the vector math not in detail. |
| `sub_820BFBB8` | `Matrix::FromRows` | Overridden | r3 = out (a 4x4 matrix), r4 to r7 = four 16-byte rows copied as words into +0, +16, +32, +48. Leaf. Callers `Vehicle::PlaceOnRoute` and `sub_825D9FC8`. Returns r3. Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::MatrixFromRows` (`src/vehicle/vehicle.cpp`), adapter in `src/hooks/vehicle_native.cpp`; harness `vehicle` (rows anywhere in the scratch area, out among them). |
| `sub_820C2150` | `TrackPosition::Copy` | Overridden | r3 = out, r4 = a track position: halfwords +0, +2, +4, +6, the four 16-byte vectors +16 to +79 (as doublewords) and halfwords +80, +82 (+8 to +15 not copied). Leaf. 6 callers seen: `Vehicle::PlaceOnRoute`, `sub_82104310`, `sub_82104380`, `sub_821043F0`, `sub_82153290`, `sub_82166658`. Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::TrackPositionCopy`, the same loads and stores in order (+8 to +15 left); harness `vehicle`. |
| `sub_820C0118` | `Vehicle::Refresh` | Overridden | r3 = a vehicle: `Vehicle::RefreshFromBody` (`sub_820B62E8`, tail call) with f1 = the float at `0x82601838` (1/60; probably for `ProbeGround`'s callees). Only caller `sub_82171560` (the race's states). Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::VehicleRefresh`, the float read from guest memory, `RefreshFromBody` as plain C++ (the original where the ground callback isn't the game's, as its adapter); harness `vehicle` (the car physics harness's world and vehicle; r3 and f1 compared). |
| `sub_820C20E0` | `Vehicle::RemoveFromWorld` | Overridden | r3 = a vehicle: +6148 = 0; with a collision object (+340), `CollisionWorld::RemoveObject` (`sub_820B9448`) and +340 = 0; with a +344 object, its vtable slot 0 (+0) with r4 = 1 (probably its deleting destructor); +344 = 0. The rigid body (+336) is kept. 6 callers seen: `sub_82170668`, `sub_82171978`, `sub_82171E50`, `sub_82172118`, `sub_821724F8`, `sub_82173A90` (the race's states). Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`, only while `--native_heap` routes the heap to the allocation layer): `vehicle::VehicleRemoveFromWorld`; the +344 object's slot 0 is a box's `Shape::DeletingDestructor` as plain C++ (as `Vehicle::DestroyPhysics`), and where the pairs' removal or the shape's deletion wouldn't be native the original runs (decided first). Harness `vehicle`. |
| `sub_820C0310` | `Vehicle::SlipWheelValue` | Overridden | r3 = a vehicle, f1 = a speed (probably km/h): once (bit 0x1 of `[0x826C2E3C]`), `[0x826C2E38]` = f1 × 1/3.6 (the threshold). For each wheel on the ground (byte +433 + 368 i): the slip = the length of (the point velocity at its mount point (`RigidBody::PointVelocity` `sub_820B68C0`) along its forward (+560) and side (+544) axes) - (the spin +664 × the radius (wheel data +52), 0); the wheel slipping most above the threshold gives f1 = its +684 (0.0 when none). Only caller `sub_8210E5C0` (probably the tires' sounds, with `Vehicle::MaxWheelSlip`, `vehicle-accessors.md`). Read in full; what wheel +268 holds isn't known. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::VehicleSlipWheelValue`, the temporaries in its frame (+80 to +159) as the original; both comparisons `bge` (a NaN slip keeps the wheel so far). Harness `vehicle`. |
| `sub_820C2248` | `Racers::NearestHumanVehicle` | Overridden | r3 = a point, r4 = an index n: a table of seven (0.0, 0) pairs on the stack; for the racers (`Race::RacerCount`) whose vehicles (`Race::RacerVehicle`) have flags 0x1 or 0x80 (+6148, probably the human cars), (the squared distance from the point to the body's position, the racer); sorted by distance (a selection sort over the racer count rather than the count found, so with fewer human cars than racers the unused (0.0, racer 0) pairs come first); returns `Race::RacerVehicle` of pair n's racer. Callers `sub_82153A50`, `sub_82153E58` (the camera modes). Read in full. A native version keeps the sort's quirk. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::RacersNearestHumanVehicle`, the table, the racers found and the differences in its frame as the original, the sort's quirk kept (`ble`: ties and NaNs keep the pair so far). Harness `vehicle` (a fake race manager and vehicles). |
| `sub_820BFCE8` | `CameraCurve::Load` | Overridden | r3 = an object, r4 = an index, r5 = a word: 90 floats from `0x82600EE8` + 360 × r4 (`.data`) into +0 to +356, +360 to +716 = 0.0, +720 = r5. Leaf. Callers `sub_82154A48`, `sub_82154D38` (the camera modes); probably theirs, though at the vehicle's addresses. The name is a guess. Read in full. Native since the B7 loop (2026-10-27, `--native_vehicle`): `vehicle::CameraCurveLoad`, the table read from guest memory (four curves: it ends where the surface tables at `0x826014A0` start); harness `vehicle` (the four curves). |
