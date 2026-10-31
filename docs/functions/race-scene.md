# Known functions: the race scene

The race scene: one of the race mode's state machines (race mode +
87936, set up by `RaceSceneMachine::Init` from `RaceMode::Init`,
`game-modes.md`; names here are ours) and the 31,168-byte object it
keeps at `[0x826D82D4]` (`RaceScene::`, `../known-data.md`), made anew
each race. The object holds the race cars' seven scene model slots
(+16: the count, 7, then a 4,288-byte slot a vehicle at +32: the pool 7
scene model, its record, the pool index), a set of 11 lights (+30064,
three of kind 1 and eight of kind 2), and the race's screen objects
(+31152, or +31156 and +31160 in split screen; `screen-objects.md`).
Its machine's handlers (functors through `StateMachine::Add`) run the
race's scene: state 1 makes the course scenery and starts its load
(`sub_8215B790`), message 3 in state 2 sets the scene up
(`RaceScene::SetUp`: the cars' scene models, the lights, the post
effects, `post-effects.md`, the screen objects, and the glows' and color
filters' parameters from a table), then in state 38 every Update
(message 0) runs the race's update (`sub_8215D2B0`) and every Draw
(message 1) the race's draw step (`Race::DrawStep`, `functions.md`);
message 29 (`0x1D`, sent on pause and on resume, `../pause-menu-fix.md`)
and 58 toggle the machine's +9608 and the race's effects
(`Race::ToggleEffects`, `post-effects.md`), probably the pause; message
44 goes to state 32 (message 3 there comes back to 38, clearing the
toggle; never seen in play), 49 to state 37, whose message 50 tears the
scene down (`RaceScene::Teardown`: the course scenery unloaded and
destroyed, the scene's teardown run 50 times, the post effects and
screen objects destroyed) and frees the object (`RaceScene::Destroy`),
then state 35.

Read by the B7 loop's mapping agent (2026-10-31) from two leads in
`../loops/leads.md` (the race's post effect users; the race object at
`[0x826D82D4]`), all in full. Shared, with rows elsewhere:
`StateMachine::Add` (`sub_82106158`) and `RaceMode::Init` in
`game-modes.md`; `PostEffects::Create`, `PostEffects::Get`,
`PostEffects::DestroyRace`, `Race::ToggleEffects`, the glows' setters
(`Glow::Set8436` to `Set8476`, vtable slots 2 to 12) and
`ColorFilter::Set8120` (slot 8) in `post-effects.md`;
`ScreenObject::ScreenObject` and `~ScreenObject` in
`screen-objects.md`; `SceneModelRecord::SceneModelRecord` in
`scene-models.md`; `sub_820C8DE8` in `car-models.md`; `Scenery::Unload`
and `Scenery::Destroy` in `course-scenery.md`; `RecordClear` in
`functions.md`; `Race::RacerCount` in `collision-world.md`; `cos` and
the array destructor iterator (`sub_820900C0`) in `c-runtime.md`;
`operator new` and `operator delete` in `heap.md`; the empty
`sub_82106540` in `audio.md`; `Race::DrawStep` in `functions.md`. Not
here, in leads (`../loops/leads.md`): the race's course scenery owners
(`sub_8215B790`, `sub_8215D2B0`, the race's update), a car's scene
model set-up (`sub_8215C0D8`), the scene's constructor and teardown
(`sub_820C8A50`, `sub_820C4D88`) and update (`sub_820C9608`).
`CameraDirector::Resume` (`sub_821597F0`, named in the post effect
users' lead) is the camera director's, in `camera-modes.md`.

The float constants named by address (`0x82000A30`, `0x82001070`,
`0x82001BE4`, `0x82002578`, `0x8200222C`, `0x82001FC0`, `0x82009798`,
`0x82002204`, and the doubles at `0x82009790` and `0x82002FE0`) weren't
read from the image; `0x82000A34` is 0.0 (`career.md`).

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
| `sub_8215D808` | `RaceSceneMachine::Init` | Overridden | r3 = the state machine (race mode + 87936). `RaceScene::Create`, +9604 (the state) = 1, then 11 transitions through `StateMachine::Add` ((state, message) → next state, 0 = stay; functors of vtable `0x82011918`, a member function on the machine, or `0x820069BC`, a plain state change): (1, 0) → 2 `RaceSceneMachine::OnStart`; (2, 3) → 38 `RaceSceneMachine::OnSetUp`; (38, 0) `OnUpdate`; (38, 1) `OnDraw`; (38, 29) and (38, 58) `TogglePause`; (38, 44) → 32 (no functor); (32, 3) → 38 `Resume`; (38, 49) → 37 (no functor); (37, 0) the empty `sub_82106540`; (37, 50) → 35 `OnEnd`. From `RaceMode::Init` (`game-modes.md`), once a race. Read in full. Native: `race_scene::MachineInit` (`src/race_scene/race_scene_init.cpp`; the 11 transitions' stores and six-word copies replayed in the original's 864-byte frame from a table decoded from the listing by a script that followed its registers, so the functors' unwritten storage words keep the stack's words as the original's; `StateMachine::Add`'s plain C++), adapter in `src/hooks/race_scene.cpp` behind `--native_race_scene` (B7 loop, 2026-10-31); only while `--native_heap` routes the game's heap to the allocation layer (operator new's plain C++), the original otherwise or where operator new refuses; r3 as the last `StateMachine::Add` leaves it. Harness `race_scene` (8 cases: empty and filled machines, planted keys, the frame, the scene's block). Runs once a race (seen in an attract race). |
| `sub_8215D738` | `RaceScene::Create` | Native | No arguments: `[0x826D82D4]` = 0, then `operator new(31168)` and, when made, `RaceScene::RaceScene` on it into `[0x826D82D4]` (else 0). From `RaceSceneMachine::Init`. Read in full. Native: `race_scene::SceneCreate` (`src/race_scene/race_scene.cpp`, operator new's plain C++), no adapter: its only caller, `RaceSceneMachine::Init`, is native, and `--native_race_scene` switches it (B7 loop, 2026-10-31). Covered by `RaceSceneMachine::Init`'s harness cases (the pointer and the block). |
| `sub_8215D6C8` | `RaceScene::RaceScene` | Native | r3 = the object: `SceneModelRecord::SceneModelRecord` (`scene-models.md`) on the seven car slots' records (+48 + 4,288 × i), then the 11 light entries' (+30080 + 96 × i) +28, +44, +60, +76 and +92 (the w of their five vectors at +16 to +80) = the float at `0x82000A30`. Returns r3. From `RaceScene::Create`. Read in full. Native: `race_scene::SceneConstruct` (`src/race_scene/race_scene.cpp`; `SceneModelRecord::SceneModelRecord`'s plain C++), no adapter: only `RaceScene::Create`'s plain C++ calls it, and `--native_race_scene` switches its caller (B7 loop, 2026-10-31). Covered by `RaceSceneMachine::Init`'s harness cases (the scene's 31,168 bytes). |
| `sub_8215D098` | `RaceSceneMachine::OnStart` | | Handler (no direct callers; state 1, message 0): r3 = the machine: `sub_8215B790([0x826D82D4])` (the course scenery made, set up and its load started; a lead), then the machine's +9608 = 0. Read in full. |
| `sub_8215D528` | `RaceSceneMachine::OnSetUp` | | Handler (no direct callers; state 2, message 3): `RaceScene::SetUp([0x826D82D4])` (a tail call). Read in full. |
| `sub_8215D100` | `RaceScene::SetUp` | | r3 = the object. `RaceScene::SetUpCars(+16)`, `RaceLights::SetUp(+30064)`, the screen objects' pointers +31152, +31156, +31160 = 0; then by the race data's (`[0x826C5B2C]`) word +0xA3AF0: 4 (split screen, probably): `PostEffects::Create(0, n)` for 5, 9, 6, 7, 11, 10 (`post-effects.md`), and two screen objects (`operator new(128000)`, `ScreenObject::ScreenObject(object, 1024, 288, 0, 0, pass 1, 599)` into +31156 and `(1024, 288, 0, 288, pass 2, 601)` into +31160; 0 when not made); else effects 1, 2, 3 and one (1024 × 576 at 0, 0, pass 0, key 599) into +31152. Then `RaceScene::SetEffectParams(1, 0)` when the race data's word +4 is 8, 9 or 10, else `(0, race data +32)`. From `RaceSceneMachine::OnSetUp`, once a race. Read in full. |
| `sub_8215C248` | `RaceScene::SetUpCars` | | r3 = the object + 16 (the car slots). The scene made (`sub_820C8A50`, a lead; `[0x826C2E44]`); +0 = 7; a 48-byte record on the stack (`sub_820D0D50` clears its +36 and +40; +0 = 1, +4 to +32 = 7, +36, +40 = 0) given to `sub_820C8DE8(scene, record, 0)` (`car-models.md`); each of the seven slots (+16 + 4,288 × i) +0 = 0 (the model) and +4272 = -1 (the pool index); then for each racer (`Race::RacerCount`; the racers' manager `[0x826D82F4]` + 2308 + 4i): `sub_8215C0D8(r3, scene, the slot of its vehicle index (racer +20), the race data's racer record +48 + 3,936 × racer +24, -1)` (a car's scene model set-up, a lead); then the empty `sub_82106540` on the record. From `RaceScene::SetUp`. Read in full. |
| `sub_8215CF78` | `RaceLights::SetUp` (probably) | Overridden | r3 = the object + 30064 (the light set): +0, +4, +8 and +1072 = 0; for each of the 11 entries (+16 + 96 × i), `RaceLights::InitEntry(entry, i)`, then by its kind (+0) 1: +4 += 1, 2: +8 += 1, and either: +0 += 1 (so 11, 3 and 8). From `RaceScene::SetUp`. What reads the set isn't known (probably the race's draw). Read in full. Native: `race_scene::LightsSetUp` (`src/race_scene/race_scene.cpp`), adapter in `src/hooks/race_scene.cpp` behind `--native_race_scene` (B7 loop, 2026-10-31); r3 (the last entry, r3 + 976) and f1 (the last `cos`) as the original leaves them. Harness `race_scene` (4 cases: random sets, the counts, the entries' floats, r3, f1). Runs once a race (seen in an attract race). |
| `sub_8215C368` | `RaceLights::InitEntry` (probably) | Native | r3 = a 96-byte entry, r4 = its index (11 or more: nothing). +0 = 0; with c = `[0x82000A30]` and z = 0.0 (`0x82000A34`): index 0 to 2: kind (+0) 1, +32 = (a, a, a, c) with a = `[0x82001070]`, +48 = (c, c, c, c); 3 to 10: kind 2, +32 = (`[0x82001BE4]`, 0.0, 0.0, c), +48 = (c, c, c, c), +64 = (`[0x82002578]`, `[0x8200222C]`, c, c), +80 = (cos(the double at `0x82002FE0`), cos(the double at `0x82009790`), 0.0, c) (probably a spot light's cone, by its cosines). Then +16 (probably the color) with p = `[0x82001FC0]`, q = `[0x82009798]`: index 1 (p, p, q), 2 (q, p, p), 3 (p, p, p), 4 (0.0, p, q), 5 (q, p, 0.0), 6 (p, p, 0.0), 0 and 7 to 10 (q, p, q); w = c. From `RaceLights::SetUp`. Read in full. The `cos` results are rounded to single before they're stored; all other words are the image's floats through `lfs` and `stfs`; the frame's temporaries (+80 to +124) hold every word stored, so nothing from the stack reaches the entry. Native: `race_scene::LightsInitEntry` (`src/race_scene/race_scene.cpp`; `cos`'s plain C++, `c-runtime.md`), no adapter: its only caller, `RaceLights::SetUp`, is native, and `--native_race_scene` switches it (B7 loop, 2026-10-31). Harness `race_scene` (32 cases: indices -2 to 13 on random entries). |
| `sub_8228F5C0` | `RaceScene::SetEffectParams` (probably) | | r3 = a kind, r4 = an index: the 44-byte entry (11 floats) at `0x82637CC0` + 44 × (3 × r3 + r4) when r3 is below 8, else the one at `0x82637C94` (just before the table); `Glows::SetParams(entry)`; then the color filters' slot 8 (`ColorFilter::Set8120`) with the float at `0x82002204` (0.05, probably, `post-effects.md`) on effects 6 and 10 when `[0x826C2CE8]` + 8 is set (split screen), else on effect 2 (`PostEffects::Get`; no null checks). From `RaceScene::SetUp` only. Read in full. |
| `sub_8228F0B0` | `Glows::SetParams` (probably) | | r3 = the 11-float entry: when `[0x826C2CE8]` + 8 is set (split screen), effects 5 and 9, else effect 1 (the glows, `PostEffects::Get`; no null checks): vtable slots 2 to 12 (`Glow::Set8436`, `Set8440`, `Set8444`, `Set8448`, `SetStrength`, `Set8456`, `SetHistoryWeight`, `Set8464`, `Set8468`, `Set8472`, `Set8476`, `post-effects.md`) with the entry's floats +0 to +40 in order (32 indirect calls in split screen, 11 else). From `RaceScene::SetEffectParams` only. Read in full. |
| `sub_8215D538` | `RaceSceneMachine::OnUpdate` | | Handler (no direct callers; state 38, message 0, every Update): `sub_8215D2B0([0x826D82D4])` (the race's update, a lead; a tail call). Always there in a race (`../pause-menu-fix.md`). Read in full. |
| `sub_8215C770` | `RaceSceneMachine::OnDraw` | | Handler (no direct callers; state 38, message 1, every Draw, render-only passes too): `Race::DrawStep([0x826D82D4])` (`sub_8215B4F8`, `functions.md`, a tail call; r31 is left as the caller had it, `../pause-menu-fix.md`). Read in full. |
| `sub_8215C750` | `RaceSceneMachine::TogglePause` (probably) | | Handler (no direct callers; state 38, messages 29 and 58): r3 = the machine: +9608 = (+9608 == 0), then `Race::ToggleEffects([0x826D82D4])` (a tail call; `post-effects.md`). Message 29 (`0x1D`) is sent on pause and on resume (`../pause-menu-fix.md`). Read in full. |
| `sub_8215D0D8` | `RaceSceneMachine::Resume` (probably) | | Handler (no direct callers; state 32, message 3): r3 = the machine: when +9608 is set, +9608 = 0 and `Race::ToggleEffects([0x826D82D4])` (a tail call). Not seen in play. Read in full. |
| `sub_8215D7D8` | `RaceSceneMachine::OnEnd` | | Handler (no direct callers; state 37, message 50): `RaceScene::Teardown([0x826D82D4])`, then `RaceScene::Destroy`. Once a race, at its end. Read in full. |
| `sub_8215B720` | `RaceScene::Teardown` | | r3 = the object: `RaceScene::UnloadScenery`, the scene's teardown started (`sub_820C4D88`, a lead) and the scene's update (`sub_820C9608`, a lead) run 50 times, `PostEffects::DestroyRace`, then for each of the screen objects +31152, +31156, +31160 that exists, `ScreenObject::~ScreenObject` and `operator delete`, and the pointer cleared. From `RaceSceneMachine::OnEnd`. Read in full. |
| `sub_8215AA88` | `RaceScene::UnloadScenery` | | No arguments read: `Scenery::Unload([0x826C551C])` and `Scenery::Destroy` (`course-scenery.md`), then `RecordClear` with r4 = 0 (the sort key), r5 = 49 (flags), r6 = 0 (the color), r7 = 0 (the depth) (`functions.md`). From `RaceScene::Teardown` only. Named in the course scenery owners' lead (`../loops/leads.md`), mapped here. Read in full. |
| `sub_8215D790` | `RaceScene::Destroy` | | No arguments: when `[0x826D82D4]` is set, `RaceScene::DestroyCars(+16)` and `operator delete` of it; the pointer isn't cleared (`RaceScene::Create` clears it next race). From `RaceSceneMachine::OnEnd`. Read in full. |
| `sub_8215D548` | `RaceScene::DestroyCars` | | r3 = the object + 16: the compiler's destructor of the seven car slots, from the last to the first: on each slot's record (slot + 16, the slot at r3 + 16 + 4,288 × i) its arrays through the array destructor iterator (`sub_820900C0`, `c-runtime.md`) with the empty `sub_82106540`: three 16-byte elements at record +32, +80, +128; three 64-byte ones at +192 and +384; six 64-byte ones at +576 to +3264 (384 apart, eight arrays). Nothing else (the destructor does nothing). From `RaceScene::Destroy`. Read in full. |
