# Known functions: the race's update

The race's scene object (`[0x826D82D4]`, `RaceScene::`, the race scene's
state machine in `race-scene.md`) on its own side of the course scenery
and the cars: in state 1 its machine makes the course scenery, sets it up
from a configuration for the race and starts its load
(`RaceScene::LoadScenery`), and in state 38, every Update (logic passes,
30 a second), it runs the race's update (`RaceScene::Update`): the
directional lights' constants put where the scene's draws read them
(`RaceLights::Update`), the course scenery's streaming
(`Scenery::Update`, `course-scenery.md`), the cars' tick update
(`Car::TickUpdate`: each racer's scene model remade when its car changed,
its draw block filled from the vehicle: the lights' flags, the gauges,
the body's and the four wheels' matrices; the scene models of slots no
racer uses released), the post effects marked updated, and the race's
screen objects (`screen-objects.md`) and glows driven by the followed
cars' speed and nitrous. The polls of the scenery's load in the race's
loading states (`CourseSetup::WaitCourse` `sub_821514A0` and
`CourseSetup::EndTick` `sub_821516F0`) have their rows in
`race-loading.md`; the scenery's unload (`RaceScene::UnloadScenery`
`sub_8215AA88`) in `race-scene.md`.

Read by the B7 loop's mapping agent (2026-11-06) from the course
scenery's race-side owners' lead (`../loops/leads.md`), all in full.
`Car::TickUpdate` moved here from `functions.md` with what's new; the
car's scene model remake (`sub_8215C0D8`) taken from the scene models'
owners' lead. Names are ours. Shared, with rows elsewhere: the course
scenery's `Scenery::Create`, `Setup`, `StartLoad`, `Update`,
`IsLoaded` and what they call (`course-scenery.md`); the race scene's
machine, `RaceScene::SetUpCars` and `RaceLights::SetUp`
(`race-scene.md`); `TimesThreePlus` (`sub_8228F090`) and
`Race::RacerCount` in `collision-world.md`; `PostEffects::MarkUpdated`,
`PostEffects::Get` and `Glow::SetFromSpeed` (`sub_82133878`, vtable
slot 24) in `post-effects.md`; `ScreenObject::SetSpeed` in
`screen-objects.md`; `Racers::Vehicle` in `track-zones.md`;
`Vehicle::Get`, `Vehicle::GetSpeedKmh` (`functions.md`), `sub_820C0140`,
`Vec3::Direction`, `RigidBody::Matrix`, `Quaternion::ToMatrix`,
`Stages::Copy`, `RacerRecords::At` and `ScenePools::ClaimB` in
`vehicle-accessors.md`; `Matrix::RotateZ`, `RotateY` and `Multiply` in
`matrix-math.md`; `Matrix::MultiplyTranslation` in `car-physics.md`;
`memcpy` in `c-runtime.md`; `SceneModel::PollLoad` in `load-poll.md`;
`ScenePools::ReleaseB` in `functions.md`; `ScenePools::SetUpB` (and
the scene models' set-ups under it) in `scene-models.md`;
`SceneModelRecord::FromCar` in `car-makers.md`. `ScreenObject::Start`
and `Stop` (`sub_8212EDF8`, `sub_8212EED0`), in the screen objects'
range but without rows there (never seen in play), have theirs here.
Fields: `../known-data.md` (`0x826D82D4`, `0x82602EA0`, "scene model",
"scene model block").

The float constants named by address (`0x820021E8`, `0x8200979C`,
`0x820097A0`) weren't read from the image; `0x82000A30` is 1.0 and
`0x82000A34` 0.0 (`race-scene.md`, `location-screen.md`), `0x820086EC`
1/300 (`screen-objects.md`).

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
| `sub_8215B790` | `RaceScene::LoadScenery` | | r3 = the race scene (`[0x826D82D4]`, from `RaceSceneMachine::OnStart`, state 1, once a race). `Scenery::Create`, then the scene's +0 = the manager (`[0x826C551C]`); the configuration's index by `TimesThreePlus`: (1, 0), so 3, when the race data's (`[0x826C5B2C]`) +4 (probably the course) is 8, 9 or 10, else (0, the race data's +32, the mode), so 0 to 2; a 44-byte configuration on the stack (frame +80): +0 = 1 (the streaming mode), +4 = 1 when the race data's +0xA3AF0 is 4 (split screen: the view count − 1), else 0, +8 to +20 = 0, +24 and +28 the entry's two words (`0x82602EA0` + 8 × index), +36 and +40 = 0; +32 isn't written (the frame's word as the stack leaves it; read by `Scenery::SetupPools` only in mode 2). `Scenery::Setup(manager, configuration)` and `Scenery::StartLoad([0x826C551C])`. 160-byte frame. 71 lines. Read in full. |
| `sub_8215D2B0` | `RaceScene::Update` | | r3 = the race scene (from `RaceSceneMachine::OnUpdate`, state 38, every Update: logic passes, once a tick in a race). `RaceLights::Update(+30064)`, `Scenery::Update(+0)`, `Car::TickUpdate(+16)` (the car slots), `PostEffects::MarkUpdated`. Then the followed cars: the first's racer index is 0, or with the race data's +0xA3AF0 6 and its +0x59614 2 (two human cars) 1 when `[0x826C7824]` is 0, else 0; a = `Racers::Vehicle(that)`, b = `Racers::Vehicle(1)`. With +0xA3AF0 4 (split screen): for a with screen object +31156 and post effect 5, then b with +31160 and effect 9; else a with +31152 and effect 1. For each (when the vehicle and the screen object exist): the effect's (`PostEffects::Get`) vtable slot 24 (`Glow::SetFromSpeed`) with the speed (`Vehicle::GetSpeedKmh`; the effect isn't checked for 0); the vehicle's byte +2424 (probably the nitrous) set and the object's +24 (running) clear: `ScreenObject::Start`; +2424 clear and +24 set: `ScreenObject::Stop`; then `ScreenObject::SetSpeed(object, speed × [0x820086EC])` (1/300). 144-byte frame (f30, f31 saved). 217 lines; one indirect call (slot 24). Read in full. |
| `sub_8215D008` | `RaceLights::Update` (probably) | Overridden | r3 = the light set (race scene + 30064). `0x8265ED00` = (0.0, 0.0, 0.0, 1.0) (`[0x82000A34]`, `[0x82000A30]`; a 16-byte copy through the frame's +80, read by `SceneLights::RecordProjectors` as x, y, z, `scenery-lists.md`); then for each of the 11 entries (+16 + 96 × i) of kind 1 (+0), `RaceLights::UploadDirectional(entry, n)` with n counting them (0 to 2 as set up). 112-byte frame. Only caller `RaceScene::Update`. Read in full. Native (B7 loop, 2026-11-06): `race_update::LightsUpdate` in `src/race_update/race_update.cpp` (the (0, 0, 0, 1) left at its frame's +80 as the original; its back chain and register saves not written), adapter in `src/hooks/race_update.cpp` (`--native_race_update`; r3 left at the set + 1072 and r4 the count, as the original; its caller `RaceScene::Update` is recompiled). Harness `race_update` (300 cases with `UploadDirectional`: the game's three kind-1 entries, many, none or any kinds; direction lanes ordinary, zero, denormal, huge, infinite or NaN). Ran in an attract race; a frame dump the same as one with the flag off. |
| `sub_8215C698` | `RaceLights::UploadDirectional` (probably) | Native | r3 = a light entry (kind 1), r4 = n (its slot): `0x8296B950` + 16n = the entry's +16 normalized (`vmsum3fp128` for x² + y² + z² in every lane, `vrsqrtefp` with one Newton step, all four lanes multiplied; a zero length gives the zero vector through the select), `0x8296B980` + 16n = +32 and `0x8296B9B0` + 16n = +48 (copied as doublewords). Also stores `[0x82000A30]` (1.0) at r1 − 4, below the stack pointer, never read. Leaf. The arrays are the scene lights' (`SceneLights::RecordDirections` normalizes `0x8296B950` again, `RecordProjectors` copies the other two, `scenery-lists.md`; the garage stage fills them as `GarageStage::SetLights`, `front-end-car-draw.md`), so +16 is the light's direction, not its color as `RaceLights::InitEntry` had it (probably). Only caller `RaceLights::Update`. Read in full. Native (B7 loop, 2026-11-06): `race_update::UploadDirectional` in `src/race_update/race_update.cpp` (the normalize as the translated code, `car_scene_draw::ops::Normalized` under flush-to-zero; the normalized vector left at its r1 − 16 as the original, which covers the 1.0 at r1 − 4), no adapter: its only caller, `RaceLights::Update`, is native, and `--native_race_update` switches it. Harness `race_update` (with `RaceLights::Update`; the arrays and the stack vector compared as floats). Ran in an attract race. |
| `sub_8215C780` | `Car::TickUpdate` | | The tick's car record update (logic passes; r31 = the car record, r26 = the vehicle): copies the car's world matrix into car + 16 and stores the gauge values at car + 696 (`sub_820C0128`, the speed in km/h), + 700 (`sub_820C0140`, the revs), + 704 (`[r26 + 2944]`, the boost) and + 712 (`[r26 + 6260]`). Read in full (B7 loop, 2026-11-06): r3 = the race scene's car slots (scene + 16: +0 the count, slot v at +16 + 4,288 × v: +0 its scene model, +16 its record, +4272 its pool index), from `RaceScene::Update` only. The "car record" is the slot's scene model's draw block (scene model + 4288, the block `Car::DrawUpdate` reads, `car-draw.md`). First `Vec3::Direction` of pass 0's (`0x826BAC00`) +416 and +400 (w 1.0) into the frame's +96, never read. Each slot's seen flag (frame +592 + 4v) = 0, and its model's +4296 and +4300 (block +8, +12) = 0. Then for each racer j (`Race::RacerCount`; `[[0x826D82F4] + 2308 + 4j]`): v = its +20, the vehicle `Vehicle::Get(v)`, seen = 1; when its flags +0 have 0x200 and v isn't 0: the car = the race data's racer record (`[0x826C5B2C]` + 48 + 3,936 × racer +24); with flag 0x2 `RaceScene::VaryCar(car, j, v, racer +16)` and the paint pick (racer +16 + v + j) / 2 (rounded toward zero), else the pick = the car's halfword +0; `RaceScene::RemakeCarModel(slots, [0x826C2E44], slot, car, pick)`, and when it answers 1 the flag 0x200 cleared. Then with a model (block = model + 4288): unless split screen (`[0x826C2CE8]` +8 set) or the vehicle isn't a player's (+6148 without 0x1 or 0x80), the view decides: with the race data's +0xA3AF0 6 and the stage's (`Stages::Copy(frame +688, race data + 0x59620, -1)`) +0 not 1, the car must have 0x80 when `[0x826C7824]` is 0, else 0x1 (or it's drawn as any other), and viewport 0's (`[[0x826C2CE8]]`) view mode (+1860) is read; otherwise viewport 0's for a 0x1 car, else viewport 1's (+4); mode 2 hides the body, mode 1 sets +12 (block, cockpit only); with +0xA3AF0 4, the last racer record's (`RacerRecords::At(race data + 0x59AE8, -1)`) +3924 1 for a car without 0x1, or 2 for one without 0x80, clears both. Hidden: block +8 = 0, +16 = the body's matrix (`RigidBody::Matrix(vehicle +336)`, `memcpy` 64), the doubleword +0 = 1, or 65 when the car's input record (`[0x826C5B30 + 4 × (v & 0xFF)]`) flags have 0x100; nothing more for that car. Otherwise: +8 = 1, +0 = 1, or 9 with input flag 0x8, then \|= 16 when the gear (vehicle +2252) is -1 and \|= 64 with input flag 0x100 (0x1 and 0x40 are the low and high beams `CockpitPoints::DrawHeadlights` reads; 0x8 and 0x10 probably the brake and reverse lights); +696 the speed, +700 `sub_820C0140`, +704 vehicle +2944, +712 +6260, +716 +6256, +720 +6252; +16 the body's matrix; then the four wheels k (vehicle + 444 + 368k, its data `[vehicle +272]` + 96k): +596 + 4k = 2 when \|wheel +220\| > `[0x8200979C]`, 1 when > `[0x820097A0]`, else 0; +612 + 4k = t² for t = wheel +192 ÷ `[vehicle +272]` +164 × `[0x820021E8]` − 1.0, or 0.0 when t < 0 (or NaN); +336 + 64k = identity × `Matrix::RotateZ` by wheel +0; +80 + 64k = `Matrix::RotateY` (by wheel +8, plus the vehicle's +324 for wheels 0 and 1, probably the steering) × `Quaternion::ToMatrix(wheel +20)`, then `Matrix::MultiplyTranslation` by the position (data +880, +884 plus wheel −28, +888); +732 = 1. Last, each slot not seen whose model is 0 or done loading (`SceneModel::PollLoad` nonzero), with a pool index: `ScenePools::ReleaseB([0x826C2E44], 7, index)`, index = -1. The identity matrices are built in the frame from `vupkd3d128` and copied. 976-byte frame (v127 saved). 738 lines. |
| `sub_8215C0D8` | `RaceScene::RemakeCarModel` | | r3 = the car slots (not read), r4 = the scene (`[0x826C2E44]`), r5 = a car slot, r6 = a car (a racer record), r7 = the paint pick. Returns 0 at once while the slot's model (+0) is loading (`SceneModel::PollLoad` 0). Else the old one released (`ScenePools::ReleaseB(scene, 7, +4272)`, +4272 = -1, when it isn't -1), +4272 = `ScenePools::ClaimB(scene, 7, 0)`, the slot's record (+16) from the car (`SceneModelRecord::FromCar(+16, car, pick)`), +0 = `ScenePools::SetUpB(scene, 7, +4272, +16)`; on its block (model + 4288): +8 = 1, +16 and +336 = identity (`memcpy` 64 each from the frame); with the record's kind (+16) 3 (traffic) and its +16 (slot +32) 0 or 7, +592 = 1. Returns 1. 208-byte frame (v127 saved). From `RaceScene::SetUpCars` (`race-scene.md`; pick -1) and `Car::TickUpdate`. The scene models' owners' lead had it as "remakes a race car's scene model". Read in full. |
| `sub_8215B330` | `RaceScene::VaryCar` (probably) | Overridden | r3 = a car (a racer record), r4 = the racer index, r5 = the vehicle index, r6 = the racer's +16. A seed s from the three by r6 % 6 (signed): 0: (r4 × 10 + r5) × 10 + r6 % 10; 1: (r4 × 10 + r6) × 10 + r5 % 10; 2: (r5 × 10 + r4) × 10 + r6 % 10; 3: (r5 × 10 + r6) × 10 + r4 % 10; 4: (r6 × 10 + r5) × 10 + r4 % 10; 5: (r6 × 10 + r4) × 10 + r5 % 10; negative: 0. Then into the car's +1568 bytes (the 20 bytes `Car::SetModel` sets partly at random, `race-setup.md`), each added to s after it's taken: with the car's word +12 64, +1580 = s % 2; +1569 = s % 14; +1573 = s % 10, or 10 for 0; +1574 and +1575 = s % 10 when the byte before is 10, else s % 9 + 1, each 10 for 0; +1576 = s % 9 + 1. All signed. Returns r3. Leaf. Only caller `Car::TickUpdate`, for racers with flag 0x2 (the rivals and the event's opponents). Read in full. Native (B7 loop, 2026-11-06): `race_update::VaryCar` in `src/race_update/race_update.cpp` (the original's r31 save below the stack pointer, a register save, not written), adapter in `src/hooks/race_update.cpp` (`--native_race_update`; its caller `Car::TickUpdate` is recompiled). Harness `race_update` (600 cases: the car's word +12 64 or random, the three numbers small, negative, at the edges or any word). Ran in an attract race. |
| `sub_8212EDF8` | `ScreenObject::Start` | | r3 = a screen object: nothing when its camera pass (`0x826BAC00` + 592 × +80) isn't enabled (+588 0) or it's already running (+24); else +24 = 1; with an object at +88 (probably its glow), its vtable slots 13, 21 and 22 into +28, +32 and +36 (floats, probably the settings it saves); with one at +92, its slot 9 with (+52, +56, +60). 96-byte frame. From `RaceScene::Update` when the car's +2424 is set. Not seen in play (`screen-objects.md`). Read in full. |
| `sub_8212EED0` | `ScreenObject::Stop` | | r3 = a screen object: when +26 is clear and +24 (running) set, +26 = 1 (probably stopping; `sub_8212F778`, not seen, then restores). Leaf. From `RaceScene::Update` when the car's +2424 is clear. Not seen in play (`screen-objects.md`). Read in full. |
