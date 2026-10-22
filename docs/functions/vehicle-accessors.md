# Known functions: vehicle accessors

The small accessors the race overlays (`race-overlays.md`) and the car
code read the cars and the race through: a vehicle by index and its
wheels' contact records and slip (`820C0140`-`820C0590`), the vehicle's
three physics objects (its rigid body at vehicle + 336, `820B6638`-`820B69D8`;
its collision object at + 340 and the contacts on it, with the
collision world at `[0x826C2D88]`, `820B7BF0`-`820B8D10`; and the object
at + 344, `820B9658`-`820B99C8`), the scene's object pools
(`820C42C8`-`820C5D70`, over the linked list `sub_82593FC0`), the race
data's stage list and racer records (`820FF808`-`820FFF10`), the players'
save data (`sub_82105620`), two constant switches (`825C8388`,
`825C8390`) and the vector library they use (`82293590`,
`82293818`-`822938F0`, `82294028`-`82294608`: 3D vectors kept in 16 bytes).
Mostly leaves with many callers in the car code; the physics itself (the
rigid body's integration, the vehicle's physics set-up `sub_820B5670` and
`sub_820B5B08`, the collision world's pairs) is not in this file (see
`docs/loops/leads.md`). The fields are in `docs/known-data.md` (the
vehicle, its wheels, the rigid body, the collision object, contacts, the
scene's pools, the race data). Shared with `race-overlays.md`, which keeps
their rows: `Model::HasPoint` (`sub_820D9DB0`) and `Model::PointPosition`
(`sub_820DA988`), both Native, the scene models' point lookups;
`Vehicle::GetSpeedKmh` (`sub_820C0128`, Overridden) stays in
`functions.md` (it isn't in `seen_in_play.txt`, as its override runs
instead).

Floats named by address are in `.rdata`: `0x82000A30` is 1.0 and
`0x82000A34` 0.0 (`known-data.md`), `0x820029E0` 3.6 (m/s to km/h, as
`Vehicle::GetSpeedKmh` uses it) and `0x820029EC` 1/30. The vector
functions keep x, y, z at +0, +4, +8 and a w at +12; which w each leaves
is noted, as callers may read it (a native version must leave the same).

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
| `sub_820C0468` | `Vehicle::Get` | | r3 = an index: `[0x826C2E34]` + 6,272 × r3, or 0 when `[0x826C2E34]` is 0 (no bounds check). Leaf. 70 callers (59 seen) across the car code, the minimap (`sub_820F5740`), `Car::TickUpdate` (`sub_8215C780`); unblocks `RaceOverlays::Bind` (`sub_8215E2E0`, with `sub_820C5D70`). |
| `sub_820C0140` | | | r3 = a vehicle: the float at +2224. Leaf. From `Car::TickUpdate` (`sub_8215C780`), `sub_8210E470`, `sub_822153A0`. What the float is isn't known. |
| `sub_820C0148` | `Vehicle::Speed` | | r3 = a vehicle: the float at +48 (speed, m/s). Leaf. From `sub_82174200` (3 callers, 1 seen). |
| `sub_820C0150` | `Vehicle::GetDistancePerFrame` | | Speed (m/s) times a hard-coded 1/30 s. r3 = a vehicle: +48 × the float at `0x820029EC`. Leaf. From `sub_82163140`, `sub_82163C50`, `sub_82173E28`. |
| `sub_820C0168` | `Vehicle::Identity` | | r3 = a vehicle: r3 + 5232, a small record (its halfword +4 indexes the table at `[0x826C5B10]` in `sub_8213D1F0`; +4 and +6 are copied by `sub_820C0590`; probably the car's model or identity). Leaf. 27 callers (26 seen), around `8213C248`-`821644D8`. |
| `sub_820C0170` | `Vehicle::WheelContact` | | r3 = a vehicle, r4 = a wheel: r3 + 5328 + 96 × r4, the wheel's 96-byte contact record (+16 the contact point, halfword +80 the surface). Leaf. Unblocks `OverlayView::WheelContact` (`sub_821268C8`) and `OverlayView::WheelSurfaceMarks` (`sub_821269B0`) in `race-overlays.md`. |
| `sub_820C0180` | `Vehicle::WheelSlip` | | r3 = a vehicle, r4 = a wheel, f1 = a threshold: 0.0 when the wheel record's byte +17 (vehicle + 433 + 368 × wheel, on the ground) is 0; else the contact's slip speed: the body's velocity at the wheel record's +64 (`sub_820B68C0` on vehicle + 336), dotted with the record's +144 and +128 (probably the forward and side axes, `sub_82294608`), minus (the record's +248 × `[vehicle + 2160 + 4 × wheel]` + 52, probably spin times radius) on the first, and the length of that (`sub_822942E0`, `sub_82294608`, `sqrtf` `sub_82293590`); 0.0 unless it's above f1 × the float at `0x82002BD8`. From `sub_820C0290` and `OverlayView::WheelSlip` (`sub_82126A70`, which it unblocks with `sub_820B68C0` and the vector functions). |
| `sub_820C0290` | `Vehicle::MaxWheelSlip` | | r3 = a vehicle, f1 = a threshold: the largest `sub_820C0180` of wheels 0-3 (starting from 0.0). Only caller `sub_8210E5C0`. |
| `sub_820C0450` | | | r3 = a vehicle, r4 = a value: when the vehicle's +344 object is set, its halfword +114 = r4's low 16 bits (tail-calls `sub_820B99C8`). Only caller `sub_82138D20`. |
| `sub_820C0490` | `Quaternion::ToMatrix3` | | r3 = out (a matrix's three rows of 16 bytes), r4 = a quaternion (x, y, z, w): the rotation matrix, scaled by the float at `0x820021E8` (probably 2.0) over the quaternion's squared length (`vmsum4fp128`); writes +0-+8, +16-+24, +32-+40 only (the rows' w left as they were). Leaf. Only caller `sub_820C0550`. |
| `sub_820C0550` | `Quaternion::ToMatrix` | | r3 = out (a 4x4 matrix), r4 = a quaternion: `sub_820C0490`, then the fourth row = (0, 0, 0, 1). Returns r3. Only caller `Car::TickUpdate` (`sub_8215C780`). |
| `sub_820C0590` | | | r3 = out, r4 = a vehicle: out +0 = speed × 3.6 (km/h, the float at `0x820029E0`), out +4 and +6 = the vehicle's halfwords +5236, +5238 (`sub_820C0168`'s +4, +6). Leaf. From `sub_821724F8` (2 callers, 1 seen). |
| `sub_820B6638` | `RigidBody::IsFixed` | | r3 = a rigid body (the vehicle's +336): its byte +0 (set: immovable, no inverse mass). Leaf. Only caller seen `sub_820BB868`. |
| `sub_820B6640` | `RigidBody::SetGravity` | | r3 = a rigid body, r4 = a vector: +480 = r4 (`sub_822940E0`), +496 = r4 × the mass (+4; `sub_82294430`), probably gravity and the weight. From the vehicle's physics set-up (`sub_820B5670`, `sub_820B5B08`) and `sub_820B77D0`. |
| `sub_820B6698` | `RigidBody::SetMass` | | r3 = a rigid body, f1 = the mass: +4 = f1, +8 = 1.0 / f1 (0.0 when +0 is set), +496 = +480 × f1. From `sub_820AF1C0`, `sub_820B5670`, `sub_820B5B08`, `sub_820B77D0`. |
| `sub_820B6708` | `RigidBody::Mass` | | r3 = a rigid body: the float at +4. Leaf. Only caller seen `sub_820BB868`. |
| `sub_820B67D8` | `RigidBody::Position` | | r3 = a rigid body: r3 + 208 (16 bytes, the body's origin, probably). Leaf. 13 callers (12 seen) in the physics and car code; unblocks `OverlayView::BodyPosition` (`sub_82126F80`). |
| `sub_820B67E0` | `RigidBody::SetCenterOfMass` | | r3 = a rigid body, r4 = an offset: +192 = r4, then the center of mass +208 + (+192 rotated by the matrix at +224, `sub_82293880`) put in that matrix's fourth row (+272, w 1.0, `sub_82293818`) and copied to +16 with w 0.0. From `sub_820AF1C0`, `sub_820AFE90`, `sub_820B2CC0`, `sub_820B5670`, `sub_820B5B08`, `sub_820B77D0`. |
| `sub_820B6880` | | | r3 = a rigid body: r3 + 80. Leaf. From `sub_820C23E8`, `sub_82153290`, `sub_82166658`. What +80 holds isn't known. |
| `sub_820B6888` | `RigidBody::Velocity` | | r3 = a rigid body: r3 + 32 (the velocity, as `sub_820B68C0` uses it). Leaf. From `sub_820B2808`, `sub_820B4420`, `sub_820BBE30`. |
| `sub_820B6890` | | | r3 = a rigid body: r3 + 144. Leaf. From `sub_820B81C8`, `sub_820BE150`. Not read in detail. |
| `sub_820B6898` | | | r3 = a rigid body: r3 + 160. Leaf. From `sub_820B13C8`, `sub_820B81C8`. Not read in detail. |
| `sub_820B68A0` | | | r3 = a rigid body: 0.0 when +0 (fixed) is set, else the float at +48. Leaf. Only caller `sub_820BBB20`. |
| `sub_820B68C0` | `RigidBody::PointVelocity` | | r3 = out, r4 = a rigid body, r5 = a point: the velocity at the point, +32 + (+96 × (point - +16)) (`sub_822942E0`, the cross product `sub_82294518`, `sub_822942A0`); w 1.0. Returns r3. 9 callers in the physics, and `sub_820C0180`. |
| `sub_820B6928` | | | r3 = a rigid body: r3 + 224 (the matrix `sub_820B67E0` reads, probably its orientation). Leaf. From `sub_820B2808`, `sub_821467F0`. |
| `sub_820B6930` | `RigidBody::Matrix` | | r3 = a rigid body: r3 + 288, the body's matrix (64 bytes). Leaf. 13 callers, among them `Car::TickUpdate`; unblocks `OverlayView::BodyMatrix` (`sub_821268B0`) and `OverlayView::BodyMatrixCopy` (`sub_82126FF0`). |
| `sub_820B6948` | `RigidBody::AngularVelocity` | | r3 = a rigid body: r3 + 96 (as `sub_820B68C0` uses it). Leaf. From `sub_820B13C8`, `sub_820B18B8`, `sub_820B2808`, `sub_820B2CC0`, `sub_821467F0`. |
| `sub_820B6950` | | | r3 = a rigid body, f1: stores f1 at +112. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B6958` | `RigidBody::AddForce` | | r3 = a rigid body, r4 = a vector: +64 += r4 (`sub_82294320`). From `sub_820B2CC0`, `sub_820B81C8`, `sub_820B92C8`. |
| `sub_820B6960` | `RigidBody::AddTorque` | | r3 = a rigid body, r4 = a vector: +128 += r4. Only caller `sub_820B92C8`. |
| `sub_820B6968` | `RigidBody::AddForceAt` | | r3 = a rigid body, r4 = a force, r5 = a point: +128 += (point - +16) × force (`sub_822942E0`, `sub_82294518`), +64 += force. Only caller `sub_820B2CC0`. |
| `sub_820B69D8` | `RigidBody::AddTorqueAt` | | As `sub_820B6968` without the force: +128 += (point - +16) × force. From `sub_820AFBD8`, `sub_820B81C8`. |
| `sub_820B7BF0` | `CollisionObject::Link` | | r3 = a collision object (the vehicle's +340), r4 = a list head: puts r3 first in the list (+228 next, +232 previous). Leaf. From `sub_820B9448`, `sub_820B9570`. |
| `sub_820B7C18` | `CollisionObject::Unlink` | | r3 = a collision object, r4 = a list head: takes r3 out of the list (`sub_820B7BF0`'s links). Leaf. From `sub_820B9448`, `sub_820B9570`. |
| `sub_820B7C90` | `CollisionObject::FirstContact` | | r3 = a collision object, r4 = a mask: the first contact in its list (+240, linked by +68) whose word +4 has a bit of r4, or 0. Leaf. From `sub_820B2808`, `sub_8210E778`, `sub_8210E938`, `sub_82215820`. The sparks (`sub_82124728`) read the list directly. |
| `sub_820B7CB8` | | | r3 = a collision object: its word +264. Leaf. Only caller `sub_820BC068`. |
| `sub_820B7CC0` | | | r3 = a collision object, r4: stores r4 at +264. Leaf. From `sub_820BBC40`, `sub_820BBD30`, `sub_820BC068`. |
| `sub_820B7CC8` | `CollisionObject::AddGroups` | | r3 = a collision object, r4 = bits: the bits not already in +248 are set there, and when any of them is in the low 24 bits, tail-calls `sub_820B94B0` (the collision world's pairs, outside this file) with them. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7CF0` | `CollisionObject::RemoveGroups` | | As `sub_820B7CC8`, clearing: the bits of r4 set in +248 are cleared, and those in the low 24 bits passed to `sub_820B8AF8` (outside this file). From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D18` | | | r3 = a collision object, r4 = bits: clears them in +252. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D28` | | | r3 = a collision object, r4 = bits: sets them in +256. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D38` | | | r3 = a collision object, r4 = bits: clears them in +256. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D48` | | | r3 = a collision object, r4 = bits: sets them in +260. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D58` | | | r3 = a collision object, r4 = bits: clears them in +260. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D68` | | | r3 = a collision object, r4 = a vector: copies it to +208 (`sub_822940E0`; probably the box's size). From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D70` | | | r3 = a collision object, f1: stores f1 at +224. Leaf. From `sub_820B5670`, `sub_820B5B08`. |
| `sub_820B7D78` | | | r3 = out, r4 = a 68-byte record: copies it (three vectors with `sub_822940E0`, then +48 to +64 word by word). Returns r3. Only caller `sub_820B7ED0`. Not read in detail. |
| `sub_820B8768` | | | r3: stores it at `0x826C2D78`. Leaf. Only caller `sub_820FF250` (the physics' set-up, probably). |
| `sub_820B8778` | | | r3: stores it at `0x826C2D7C`. Leaf. Only caller `sub_820FF250`. |
| `sub_820B8788` | `CollisionWorld::SetCallback` | | r3: stores it at the collision world's +8 (`[0x826C2D88]`). Leaf. Only caller `sub_820FF250`. |
| `sub_820B8798` | `CollisionWorld::Callback` | | Tail-calls the function at the collision world's +8 with r3 and the rest passed on (indirect; what it points to isn't read). From `sub_820AFE90`, `sub_820B4420`. |
| `sub_820B8A48` | `CollisionWorld::NewContact` | | Takes the next of the collision world's 80-byte records (+48 the array, +52 used, +44 the capacity) and counts it, or returns 0 when full. Leaf. From `sub_820B7C50`, `sub_820B81C8`. |
| `sub_820B8C90` | | | The collision world's word +60. Leaf. From `sub_820FC2E0`, `sub_820FEBD0`. |
| `sub_820B8CA0` | | | The collision world's word +64. Leaf. Only caller `sub_820B8548`. |
| `sub_820B8CB0` | | | The collision world's word +68. Leaf. Only caller `sub_820B81C8`. |
| `sub_820B8CC0` | `Contact::Object` | | r3 = a contact: its word +0 when the top byte of its flags (+4) is 0, else 0. Leaf. From `sub_820FF050`, `sub_82164228`; unblocks `CarOverlays::UpdateSparks` (`sub_82124728`) with `sub_820B8D00`, `sub_820B8D08`, `sub_822940E0` and `sub_825C8388`. |
| `sub_820B8CE0` | | | r3 = a contact: its word +0 when the top byte of its flags (+4) is nonzero, else 0 (`sub_820B8CC0`'s other case). Leaf. Only caller `sub_820FF050`. |
| `sub_820B8D00` | `Contact::Position` | | r3 = a contact: r3 + 16 (its position). Leaf. 7 callers in the physics and `CarOverlays::UpdateSparks`. |
| `sub_820B8D08` | `Contact::Strength` | | r3 = a contact: the float at +64 (probably the impact's strength). Leaf. 6 callers, among them `CarOverlays::UpdateSparks`. |
| `sub_820B8D10` | `Contact::Next` | | r3 = a contact, r4 = a mask: the next contact (+68 on) whose flags (+4) have a bit of r4, or 0. Leaf. Only caller `sub_820FF050`. |
| `sub_820B9658` | | | r3 = the vehicle's +344 object, f1: stores f1 at +20. Leaf. From the vehicle's physics set-up (`sub_820B5670`, `sub_820B5B08`: the car data's +1796). |
| `sub_820B9660` | | | r3 = the vehicle's +344 object, f1: stores f1 at +16. Leaf. From `sub_820B5670`, `sub_820B5B08` (the car data's +1792) and `sub_820CDC18`. |
| `sub_820B9668` | | | r3 = the vehicle's +344 object: the float at +28. Leaf. Only caller `sub_820B81C8`. |
| `sub_820B9670` | | | r3 = the vehicle's +344 object, f1: stores f1 at +28. Leaf. From `sub_820B5670`, `sub_820B5B08` (the car data's +1796). |
| `sub_820B9678` | | | r3 = the vehicle's +344 object, f1: stores f1 at +24. Leaf. From `sub_820B5670`, `sub_820B5B08` (the car data's +1792). |
| `sub_820B99B0` | | | r3 = an object: its halfword +112. Leaf. Only caller `sub_820FDC18`. |
| `sub_820B99B8` | | | r3 = an object, r4: stores r4's low halfword at +112. Leaf. From `sub_820C3150`, `sub_820C33B8`. |
| `sub_820B99C0` | | | r3 = an object: its halfword +114. Leaf. Only caller `sub_820FDC18`. |
| `sub_820B99C8` | | | r3 = an object, r4: stores r4's low halfword at +114. Leaf. Only caller `sub_820C0450` (on the vehicle's +344 object). |
| `sub_82593FB0` | `List::Init` | | r3 = a list: +0 (the first node) and +8 (the count) = 0. Leaf. Only caller seen `sub_820C89A8`. |
| `sub_82593FC0` | `List::At` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). r3 = a list, r4 = n: the n-th node (from the first at +0, linked by each node's +8); 0 for an empty list, and the first node when n is past the end (not 0). Leaf. 16 callers, the scene's pools (`sub_820C42C8` and the others here, `sub_820C5DE8`). |
| `sub_820C42C8` | `ScenePools::ClaimA` | | r3 = the scene (`[0x826C2E44]`), r4 = a pool: the first free object (+16 = 0) of the pool's list (scene + 56 + 12 × pool; its count at +8) is marked in use (+16 = 3) and its index returned, or -1. From seven of the scene's model set-ups (`sub_820DB818` to `sub_820E70C0`). |
| `sub_820C4420` | `ScenePools::GetA` | | r3 = the scene, r4 = a pool, r5 = an index: the object (`sub_82593FC0`, found twice) when in use (+16 = 3), else 0. From six of those set-ups. |
| `sub_820C4550` | | | r3: r3 + 1888, or 0 when r3 is 0. Leaf. 32 callers (12 seen), among them the vehicle's set-up (`sub_820C3150`, `sub_820C33B8`) and `821541xx`-`8215B878`. Not read in detail (probably a vehicle's +1888 record). |
| `sub_820C5A60` | | | r3 = the scene: for each in-use object of the second set's pool 7 (scene + 5028, as `sub_820C5D70`), `sub_820DA180(object)` (outside this file, probably a scene model's update). Only caller `sub_820C9608`. |
| `sub_820C5AB0` | `ScenePools::ClaimB` | | As `sub_820C42C8` on the second set of pools (scene + 4944 + 12 × pool). From `sub_820D9870`, `sub_820DA758`, `sub_820DBE80`, `sub_8215C0D8`, `sub_8217F880`, `sub_8217FCB0`, `sub_8222C300`. |
| `sub_820C5C20` | `ScenePools::GetB` | | As `sub_820C4420` on the second set of pools. 61 callers (24 seen), many around `82178B40`-`82179DF0`. |
| `sub_820C5D70` | `ScenePools::FindModel` | | r3 = the scene, r4 = a kind, r5 = n: the n-th in-use object (+16 = 3) of pool 7 of the second set (scene + 5028, count + 5036) whose +32 is r4, or 0. From `CarOverlays::FindModel` (`sub_821253F0`), `RaceOverlays::Bind` (`sub_8215E2E0`) and `sub_82153A50`; unblocks them with `sub_82593FC0`. |
| `sub_820FF808` | `RacerRecord::Init` | | r3 = a 3,936-byte racer record: `sub_820CD5D8(r3)` (outside this file), then +3920 = 0.0, +3912 and +3916 = -1, halfwords +3904 and +3906 = -1, +3924 = 0. 14 callers (6 seen), among them the race's set-up `sub_82115D98`. |
| `sub_820FF918` | | | r3: stored at the race data's +0x59618 (`[0x826C5B2C]`) unless 0. Leaf. Only caller seen `sub_82115D98`. |
| `sub_820FF938` | | | r3: stored at the race data's +0x5961C unless 0. Leaf. Only caller seen `sub_82115D98`. |
| `sub_820FF968` | | | r3: stored at the race data's +32. Leaf. Only caller seen `sub_82115D98`. |
| `sub_820FF9F8` | `Racers::IndexOf` | | r3 = the racer records (the race data + 48), r4 = a racer id: the index (0-92) of the first record whose +3912 is r4, or -1. Leaf. From `sub_82112DB0`, `sub_82112E2C` and `sub_82126B50` (race overlays; unblocks it with `sub_820FFCF8`). |
| `sub_820FFA30` | `Stages::HasNext` | | r3 = the stage list (the race data + 0x59620): 1 when the current stage (+804) is before the last (+800 the count, minus 1), else 0. Leaf. From `sub_82164B50`, `sub_8216E150`. |
| `sub_820FFA50` | `Stages::SetType` | | r3 = the stage list, r4 = a type, r5 = a stage or -1: nothing for type 4; with -1 a stage is added (its +0 = r4, the count +800 up), else stage r5's +0 = r4. Leaf. From `sub_82112DB0`, `sub_82112E2C`, `sub_821634D8`. |
| `sub_820FFAB0` | | | r3 = the stage list, r4: stored at +808. Leaf. Only caller seen `sub_82115D98`. |
| `sub_820FFBE8` | `Stages::Type` | | r3 = the stage list, r4 = a stage or -1 (the current, +804): its word +0 (stages are 80 bytes). Leaf. 9 callers (7 seen). |
| `sub_820FFC08` | | | As `sub_820FFBE8`, the stage's word +4. Leaf. Only caller `sub_821654F8`. |
| `sub_820FFC30` | | | As `sub_820FFBE8`: 1 when the stage's type is 2 or 3, else 0. Leaf. 9 callers (8 seen). |
| `sub_820FFC68` | | | r3 = the stage list: its word +808. Leaf. From `sub_82164B50`, `sub_8216E150`, `sub_8222F0B0`. |
| `sub_820FFC70` | | | As `sub_820FFBE8`, the stage's word +76. Leaf. From `sub_821654F8`, `sub_821713C0`, `sub_82174D28`. |
| `sub_820FFC98` | `Stages::Racer` | | r3 = the stage list, r4 = a slot (0-3), r5 = a stage or -1: the stage's word +44 + 8 × slot (a racer index, -1 for none). Leaf. From `sub_821654F8`, `sub_821713C0`. |
| `sub_820FFCC8` | | | As `sub_820FFC98`, the word +48 + 8 × slot. Leaf. Only caller `sub_821654F8`. |
| `sub_820FFCF8` | `Stages::HasRacer` | | r3 = the stage list, r4 = a racer index, r5 = a stage or -1: 1 when one of the stage's four slots (+44 + 8i, until a -1) is r4, else 0. Leaf. 8 callers, among them `sub_82126B50` (race overlays). |
| `sub_820FFD48` | | | r3 = out, r4 = the stage list, r5 = a stage or -1: copies the stage's 16 bytes at +24 to out. Leaf. Only caller `sub_821654F8`. |
| `sub_820FFD88` | `Stages::Copy` | Wrapped | The race draw step's parts and the per-car update's callees, wrapped only to name which one advances per-frame state (`--find_per_frame_state`) and to skip one (`--debug_hide`) (`ITC_FRAME_STATE_WRAP`, `src/debug/frame_state_diff.cpp`). r3 = out, r4 = the stage list, r5 = a stage or -1: copies the stage's 80 bytes to out (`memcpy`, `sub_825A7410`). Returns r3. 26 callers (22 seen), among them the minimap and `Car::TickUpdate`. |
| `sub_820FFDD8` | | | r3 = a list of 972-byte entries (the count at +0), r4, r5: adds an entry with +4 = r4, +8 = r5. Leaf. Only caller `sub_821654F8`. Not read in detail. |
| `sub_820FFE08` | `RacerRecords::Add` | | r3 = an array of 3,936-byte racer records (the count at +0x49FE0, room for 77), r4 = a record: copied to the end (`memcpy`) and counted. Only caller seen `sub_821644D8`. |
| `sub_820FFE58` | `RacerRecords::At` | | r3 = such an array, r4 = n or -1: record n (no bounds check), or the last when -1 (0 when there are none). Leaf. 29 callers (17 seen), among them `Car::TickUpdate`. |
| `sub_820FFE98` | | | r3 = out, r4: copies two halfwords (4 bytes). Leaf. From `sub_82162330`, `sub_821635D0`. |
| `sub_820FFEB0` | | | r3 = out, r4 = five pairs of halfwords: copies the pair before the first whose first halfword is 0xFFFF, or the fifth when none is (when the first already is, the 4 bytes before r4). Leaf. From `sub_82101990`, `sub_82101A10`, `sub_82162330`. |
| `sub_820FFF10` | | | r3 = out, r4 = five pairs of halfwords, r5 = i: pair i for i below 5, else (-1, -1). Leaf. From `sub_82162330`, `sub_82164090`. |
| `sub_82105620` | `SaveData::Of` | | r3 = a player: `[0x826C5B58 + 4 × r3]` (no bounds check), probably the player's save data. Leaf. 223 callers (94 seen); unblocks `sub_82126BB0`, `sub_82126C08` and `sub_82126C58` (race overlays). |
| `sub_825C8388` | | | Returns 0. Leaf. 28 callers (15 seen): the race overlays' effect updates (`CarOverlays::UpdateMarker` `sub_82123A80`, `UpdateModelSprites` `sub_82123E48`, `UpdateSparks` `sub_82124728`, `UpdateTrails` `sub_82124EE0`, `UpdateLightSprites` `sub_821251C0`, `UpdateParticlesB` `sub_821252D0`, `UpdateSmoke` `sub_82125B60`) return early when it's nonzero, and code around `82364F40`, `8237DDE8`, `8245BF80`, `82461070`-`824619B0`, `8249E378`. Probably a debug switch compiled to a constant. |
| `sub_825C8390` | | | Returns 1. Leaf. 4 callers, 1 seen (`sub_822E0840`). Probably a switch like `sub_825C8388`. |
| `sub_82293590` | `sqrtf` | | `fsqrts` f1. Leaf. 16 callers (9 seen): the physics, `sub_820C0180`, `sub_82294270`. |
| `sub_82293818` | `Matrix::SetTranslation` | | r3 = a 4x4 matrix, r4 = a vector: the fourth row (+48) = (x, y, z, 1.0). From `sub_820B6710`, `sub_820B67E0`, `sub_820B71B0`, `sub_820B77D0`. |
| `sub_82293880` | `Matrix::Rotate` | | r3 = out, r4 = a matrix, r5 = a vector: x × row 0 + y × row 1 + z × row 2 (rows of 16 bytes; `fmadds`), w 0.0. Leaf. 8 callers in the physics. |
| `sub_822938F0` | `Matrix::TransformPoint` | | As `sub_82293880` plus the fourth row (+48), w 0.0. Leaf. From `sub_820B13C8`, `sub_820B9C40`, `sub_820BACD0`, `sub_820BAE78`. |
| `sub_82294028` | `Vec3::Add` | | r3 = out, r4, r5: out = r4 + r5 (x, y, z), w 1.0. Leaf. From `sub_822942A0`, `sub_82294320`, `sub_822943C8`, `sub_820C23E8`, `sub_82170BC0`. |
| `sub_82294068` | `Vec3::Sub` | | r3 = out, r4, r5: out = r4 - r5, w 1.0. Leaf. 12 callers (11 seen). |
| `sub_822940A8` | `Vec4::ScaleTo` | | r3 = out, r4 = a vector, f1: all four components × f1. Leaf. 8 callers. |
| `sub_822940E0` | `Vec4::Copy` | | r3 = out, r4: copies the four floats (as floats: `lfs`/`stfs`). Leaf. 91 callers (76 seen), among them `CarOverlays::UpdateSparks`. |
| `sub_82294108` | `Vec3::CrossW1` | | r3 = out, r4, r5: r4 × r5 (`fmsubs`, in a different order from `sub_82294518`), w 1.0. Leaf. Only caller `sub_820C23E8`. |
| `sub_82294170` | `Vec3::Dot` | | r3, r4 (any alignment, loaded with `lvsl`/`vperm`): r3.x × r4.x + r3.y × r4.y + r3.z × r4.z by `vmsum3fp128`, in f1. Leaf. 18 callers (13 seen); `sub_82294608` is its alias. A native version must match `vmsum3fp128`'s rounding. |
| `sub_822941B0` | `Vec3::Normalize` | | r3 = out, r4: r4 / its length (`fsqrts`, then one division and three multiplications), w 1.0; returns the length in f1 (no check for 0). Leaf. 9 callers. |
| `sub_82294200` | `Vec3::Direction` | | r3 = out, r4, r5: r4 - r5 normalized as in `sub_822941B0`, w 1.0; returns the length. Leaf. From `Car::TickUpdate`, `sub_820FC900`, `sub_820FDC18` and others (6). |
| `sub_82294270` | `Vec3::Length` | | r3: `sqrtf(Dot(r3, r3))`. 11 callers (6 seen). |
| `sub_82294298` | `Vec3::NormalizeSelf` | | r3: `sub_822941B0(r3, r3)`. 9 callers (7 seen). |
| `sub_822942A0` | `Vec3::Sum` | | r3 = out, r4, r5: `sub_82294028` (w 1.0; the 0.0 it stores at out + 12 first is overwritten); returns r3. 27 callers (18 seen). |
| `sub_822942E0` | `Vec3::Difference` | | As `sub_822942A0` with `sub_82294068`. 32 callers (22 seen). |
| `sub_82294320` | `Vec3::AddTo` | | r3 += r4 (`sub_82294028`, w 1.0). 24 callers (20 seen). |
| `sub_82294340` | `Vec4::ScaleBy` | | r3 ×= f1 (`sub_822940A8`, w too). 7 callers (6 seen). |
| `sub_82294348` | `Vec4::DivideBy` | | r3 ×= 1.0 / f1 (w too). From `sub_820B7ED0` (2 callers, 1 seen). |
| `sub_82294360` | | | r3 = out, r4, r5: r4 - r5 through a temporary, w 1.0; returns r3. Only caller seen `sub_820BD578`. |
| `sub_822943C8` | | | r3 = out, r4, r5: r4 + r5 through a temporary, w 1.0; returns r3. Only caller seen `sub_820BC978`. |
| `sub_82294430` | `Vec4::Scaled` | | r3 = out, r4, f1: out + 12 = 0.0, then `sub_822940A8` (all four × f1); returns r3. So in place (r3 = r4) w comes out 0.0, else r4's w × f1. 36 callers (25 seen). |
| `sub_82294470` | | | As `sub_82294430` with the vector in r5 (r4 not read). From `sub_820AF1C0`, `sub_820B70C8`. |
| `sub_822944B0` | | | r3 = out, r4, f1: r4 × f1 through a temporary, w 1.0; returns r3. Only caller seen `sub_820BC978`. |
| `sub_82294518` | `Vec3::Cross` | | r3 = out, r4, r5: r4 × r5 (`fmsubs`), w 0.0. Leaf. 13 callers (12 seen), among them `sub_820B68C0`. |
| `sub_82294568` | `Vec3::Lerp` | | r3 = out, r5 = a, r6 = b, f1 = t (r4 not read): a - (a - b) × t, w 1.0; returns r3. From `sub_820BF320` (5 callers, 1 seen). |
| `sub_82294608` | `Vec3::DotOf` | | Tail-calls `sub_82294170` (the same dot product). 38 callers (27 seen), among them `sub_820C0180`. |
