# Known functions: the car scene

The car scene: the 5,536-byte object at `[0x826C2E44]` that holds the
cars' models wherever cars are shown (the race, the front end's garage
and shops, the career's parking area), with the first set of object
pools at `[0x826C2E40]` beside it (`../known-data.md`). Named
`CarScene::` here, as `Scene::Update` (`sub_820988C8`, `functions.md`)
is the app's scene object, another class. `CarScene::Create` makes it
(`VirtualAlloc`), with the first set (`ScenePools::MakeA`), its lighting
block, the scene's shared assets reset, the paint palette and the
accessories' set; `sub_820C8DE8` (`car-models.md`) then copies a 48-byte
set-up record into it (+0 = 3), makes the pools and starts the shared
assets' loads (`CarScene::LoadAssets`): XPR2 texture bundles, part
models and TGA images, finished by `CarScene::OnFileLoaded`, the loads'
callback. Its update (`CarScene::Update`, 9 callers: the race's draw
step and teardown, the parking area, the front end's car draw and
teardown) runs the scene models' updates and the first set's deferred
texture releases; once a teardown is started (`CarScene::StartTeardown`:
+0 = 4 with a count of 2 updates, the first set the same) the next
updates count it down, then destroy the second set's objects, release
the shared assets and free the scene, and the first set and the
accessories' set after it.

The scene's layout: +0 the state (0 made, 3 set up, 4 tearing down); +4
the set-up record (48 bytes, copied by `sub_820C8DE8`: +4 the model set,
0 a or 1 b, +8 to +36 the pools' counts, +40 and +44 the scene's own
load buffer and its size, 0 for the archive's shared one); +48 the
teardown's count; +64 the shared assets (4,880 bytes, below); +4944 + 12
× pool the second set's eight lists (+0 the first node, +8 the count;
pool 7 the scene models); +5040 the first set (`[0x826C2E40]`); +5044
`sub_820C8DE8`'s r5; +5048, +5052 cleared when made (+5052 = 1 from
`LocationScreen::Update`); +5056, +5060 the load buffer and its size;
+5168 to +5247 five float4s whose w is 1.0 when made; +5248 to +5520 the lighting block
(`CarScene::InitLighting`; +5500 read by `sub_820C9170`,
`functions.md`). The shared assets (offsets from scene + 64): 24-byte
load slots (`LoadSlot::`, `car-models.md`; +20 the state, 2 loading, 3
done), each XPR2 slot with an XPR object (24 bytes) and most with a
texture provider (20 bytes, `models.md`) given the bundle as slot 0:
+0 (asset 141, not loaded in set b unless +5044 is set; XPR +24), +48
(586) and +72 (not loaded by `CarScene::LoadAssets`; XPRs +96, +120,
providers +144, +164), +184 (888; +208, +232), +252 (144; +276, +300),
+320 (145; +344, +368); a part model at +416 (464 bytes, `PartModel::`)
from +388 (142, its part file) and +880 (143, its textures); +904 (146)
a TGA image (header +928, pixels +948); +952 (106; +976, +1000), +1020
(107; +1044, +1068); four part models at +1184 + 464i from +1088 + 24i
(129 to 132 in set a, 133 to 136 in set b) and +3040 + 24i (137 to
140); 19 TGA images from +3136 + 24i (110 to 128; pixels +3592 + 4i,
headers +3668 + 20i, probably the license plate's, `CarModel::MakePlate`
in `car-models.md`); a part model at +4096 from +4048 and +4072 (108,
109; set a only); +4560 (105; XPR +4584); four bundles from +4608 + 24i
(97 to 100 in set a, 101 to 104 in set b; XPRs +4704 + 24i, providers
+4800 + 20i).

Read by the B7 loop's mapping agent (2026-11-01) from three leads in
`../loops/leads.md` (the scene's update; its maker and teardown helpers;
its set-up and teardown objects), all in full except
`CarScene::InitLighting` (its stores and calls only). Shared, with rows
elsewhere: `ScenePools::MakeA`, `CreateA`, `NameMatches` and the first
set's objects' destructors in `scene-loaders.md`; `sub_820C8DE8`,
`LoadSlot::Load`, `LoadShared`, `Reset`, `ReleaseBuffer`,
`TGA::CreateImage`, `TgaHeader::Clear`, `Texture::Free` and
`DeferRelease` (`sub_820C3F50`, which starts the releases
`ScenePools::UpdateA` counts down) in `car-models.md`; `List::Init`,
`List::At`, `ScenePools::SetUp` and `sub_820C5A60` (the scene models'
updates) in `vehicle-accessors.md`; `XPR::XPR`, `~XPR`, `Create`,
`Release`, `TextureProvider::TextureProvider`, `~TextureProvider`,
`PartModel::PartModel`, `~PartModel`, `Model::Load` and `Model::Release`
in `models.md`; `TextureProvider::ClearAll` and `Set` (vtable slots 2
and 4) in `scenery-lists.md`; `PartModel::ScaleBlended` and
`PaintPalette::Init` in `material-passes.md`; `Accessories::Create`,
`SetScale`, `ReleaseAll` and `Destroy` in `accessories.md`, with the
set's getter `sub_820F0E50` in `scene-models.md`; `SceneModel::Update`
and `SceneModel::Build` in `scene-models.md`; `cos` in `c-runtime.md`;
`VirtualAlloc`, `VirtualFree` and `operator delete` in `heap.md`; the
empty `sub_82106540` in `audio.md`. Not here: the scene's draw
(`sub_820C5DE8`, with `sub_820C4EB8` and `sub_820C4FB8`, rows in
`car-scene-draw.md` since 2026-11-04), the second set's objects' makers under `sub_820C8DE8`
and the point records they use (`second-set.md` since 2026-11-01), the
parts' finishers
`CarScene::LoadsDone` calls (`load-poll.md` since 2026-11-01), and
the front end's scene owners (`sub_82180B00`, `sub_82179AE8`): leads in
`../loops/leads.md`. The callers in the race and the parking area are
in `race-scene.md`, `location-screen.md` and the course scenery's
owners' lead.

The float at `0x82002FE8` (`CarScene::Create`'s accessory scale) wasn't
read from the image; `0x82000A30` is 1.0.

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
| `sub_820C8A50` | `CarScene::Create` | Overridden | No arguments: `[0x826C2E44]` = `VirtualAlloc(0, 5536, MEM_COMMIT, PAGE_READWRITE)` and, when made, `CarScene::CarScene` on it (the scene's +0 = 0 is stored without checking the allocation); `[0x82601854]` = 167 and `[0x82601858]` = 270 (read by the scene's draw `sub_820C5DE8`, copied to `0x8296B90C` and `0x8296B900`); `ScenePools::MakeA` (`scene-loaders.md`); +5040 = `[0x826C2E40]`; `CarScene::InitLighting(+5248)`; `SceneAssets::Reset(+64)`; +5044, +5048, +5052 = 0; `PaintPalette::Init` (`material-passes.md`); `Accessories::Create`; `Accessories::SetScale(the set, {x, x, x, 1.0})` with x the float at `0x82002FE8` (`accessories.md`). A 128-byte frame. From `RaceScene::SetUpCars` (`race-scene.md`), `LocationScreen::Create` (`location-screen.md`) and the front end's `sub_82180B00` (a lead). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, over the allocation layer with the models' lifetime (the scene's pages `memory::AllocPages`); `ScenePools::MakeA`, `PaintPalette::Init`, `Accessories::Create` and `SetScale` through their plain C++, the scale built in its frame (harness `scene`; a frame dump against `--native_scene=false` the same). |
| `sub_820C89A8` | `CarScene::CarScene` | Native | r3 = the scene: `SceneSetUp::SceneSetUp(+4)`, `SceneAssets::SceneAssets(+64)`, `List::Init` on the eight lists at +4944 + 12i (`vehicle-accessors.md`), then the float at `0x82000A30` (1.0) into +5180, +5196, +5212, +5228, +5244, +5260 + 16i (i 0 to 7) and +5388 + 16i (i 0 to 8), the w of the float4s there. Returns r3. Only caller `CarScene::Create`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native (`--native_models`, the layer, the host lock) and the providers' storage can be given natively (else the original; harness `scene`). Adapter removed (2026-11-01): its native callers (`CarScene::Create`) call the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820D0D50` | `SceneSetUp::SceneSetUp` (probably) | Overridden | r3 = a 48-byte scene set-up record: +36 and +40 (the own load buffer and its size) = 0; r11 = 0. Leaf. From `CarScene::CarScene` (the scene's +4), `RaceScene::SetUpCars` (`race-scene.md`), `LocationScreen::Create` (`location-screen.md`) and `sub_82180B00` (the last three on their stacks, then given to `sub_820C8DE8` and destroyed with the empty `sub_82106540`). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene` (harness `scene`). |
| `sub_820C8528` | `CarScene::InitLighting` (probably) | Native | r3 = the scene + 5248: fills +0 to +271 with float4s, some of constants (the floats at `0x82002674`, `0x820021F8`, `0x8200296C`, `0x82002208`, `0x820021F4`, `0x820021E8`, `0x82002968`, `0x82000A30`, `0x82000A34` and others) and some from 12 calls to `cos` (`c-runtime.md`) on doubles at `0x82002FE0` and `0x82002FD8`, then a float at +272 (scene +5520). Probably the scene's lights (`sub_820C9170` reads scene +5500, a light cone's angle, `functions.md`). A 288-byte frame with f27 to f31 saved. Only caller `CarScene::Create`. Read for its stores and calls only, not in detail. Read in full for its migration (2026-11-01): three cones built alike (+176, +192 to +232, +240 to +264) from cos of the double at `0x82002FE0` and of those at `0x82002FD8`, `0x82002FC8`, `0x82002FB8` (1.0 / (cos a - cos b)); the float at `0x8200256C` into +272. Its r3 is left the doubleword at its frame's +96. Native in `src/scene/lighting.cpp` behind `--native_scene` (strict floating point; the frame's words as the original leaves them; harness `scene`). Adapter removed (2026-11-01): its native callers (`CarScene::Create`) call the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820C5008` | `SceneAssets::SceneAssets` | Native | r3 = the shared assets (scene + 64, 4,880 bytes): `XPR::XPR` on +24, +96, +120, +208, +276, +344, +976, +1044, +4584 and +4704 + 24i (four); `TextureProvider::TextureProvider(-1)` on +144, +164, +232, +300, +368, +1000, +1068 and +4800 + 20i (four); `PartModel::PartModel` on +416, +1184 + 464i (four) and +4096 (`models.md`); `TgaHeader::Clear` on +928 and +3668 + 20i (19; `car-models.md`). The load slots aren't touched (`SceneAssets::Reset` clears them). Returns r3. Only caller `CarScene::CarScene`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native (`--native_models`, the layer, the host lock) and the providers' storage can be given natively (else the original; harness `scene`). Adapter removed (2026-11-01): its native caller `CarScene::CarScene` calls the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820C4A70` | `SceneAssets::Reset` | Native | r3 = the shared assets: `LoadSlot::Reset` (`car-models.md`) on every slot (+0, +48, +72, +184, +252, +320, +388, +880, +904, +952, +1020, +1088 + 24i, +3040 + 24i, +3136 + 24i, +4048, +4072, +4560, +4608 + 24i), `XPR::Release` on every XPR, `TextureProvider::ClearAll` (vtable slot 2, `scenery-lists.md`) on every provider, `Model::Release` on the five part models (`models.md`), and +948 and +3592 + 4i (19, the TGA pixels' words) = 0, in the layout's order (each XPR slot, its XPR, its provider). A 128-byte frame. From `CarScene::Create` and `SceneAssets::Release`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native, every provider's slot 2 is `ClearAll` (called directly) and the pending frees have room for every block (else the original; harness `scene`, on the game's list). Adapter removed (2026-11-01): its native callers (`CarScene::Create` and `SceneAssets::Release`) call the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820C8B40` | `CarScene::LoadAssets` | Overridden | r3 = the scene: starts the shared assets' loads with the scene as the context and `CarScene::OnFileLoaded` (`0x820C52A0`) as the callback, `LoadSlot::LoadShared` (into the load buffer) unless said: assets +48 asset 586; +0 141 unless the model set (+4) is b and +5044 is 0; +184 888, +252 144, +320 145; +388 142 (`LoadSlot::Load`, its own pages); +880 143, +904 146, +952 106, +1020 107; set a only: +4048 108 (`Load`) and +4072 109; +4560 105; then four pairs, +1088 + 24i (`Load`; 129 + i in set a, 133 + i in set b) and +3040 + 24i (137 + i); +3136 + 24i 110 + i (19); +4608 + 24i 97 + i (set a) or 101 + i (set b). Slot +72 isn't loaded. Only caller `sub_820C8DE8` (`car-models.md`). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, over the allocation layer with the archive's queue native (`LoadSlot::LoadShared`'s and `Load`'s plain C++, `Archive::Load` the native queue's; harness `scene`, the loads recorded). |
| `sub_820C52A0` | `CarScene::OnFileLoaded` | Overridden | The shared assets' load callback (no direct callers; given by `CarScene::LoadAssets`): r3 = the archive request, r4 = the data, r6 = the scene. Finds the slot whose request (+4) is r3, in the order of the assets' layout, and finishes it: an XPR2 slot `XPR::Create(its XPR, data)` (`models.md`) and, when it has one, its provider's slot 4 (`TextureProvider::Set(provider, 0, the XPR)`, `scenery-lists.md`); a part file slot (+388, +1088 + 24i, +4048) only its state; a texture slot (+880, +3040 + 24i, +4072) `Model::Load(its part model, the part file slot's buffer (+16), data)` and both slots released, and for +880 `PartModel::ScaleBlended(+416, shape i, 1.0)` for each of the model's +412 shapes (`material-passes.md`); a TGA slot (+904, +3136 + 24i) `TGA::CreateImage(header, the slot's size (+8), &data, the pixels' word)` (`car-models.md`). Then `LoadSlot::ReleaseBuffer` on it and its state (+20) = 3. Nothing when no slot matches. A 128-byte frame with f31 saved (the 1.0). 7 indirect calls (the providers' slot 4). Runs at the scene's set-up (seen in play). Read in full. Read in full for its migration (2026-11-01): the slot +0's and +4560's XPR2s have no provider; the data's address is stored at the caller's frame's +28 (its r1 + 156: `TGA::CreateImage`'s buffer word). Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native and the matched branch's slots are freed natively (else the original): `XPR::Create`, `Model::Load` and `TGA::CreateImage` through their plain C++ (their originals as guest calls where it declines), the providers' slot +16 `TextureProvider::Set`'s plain C++ when it's that function, else a guest call; the adapter stays (reached through the archive's callback). Harness `scene` (the XPR2, part file, TGA and no-match branches; the texture branches' `Model::Load` on part files only in play); a frame dump against `--native_scene=false` the same. |
| `sub_820C9608` | `CarScene::Update` | | No arguments. When the scene (`[0x826C2E44]`) exists: in state 4 (+0, a teardown started) with +48 not 0, +48 counts down by one; at 0 `ScenePools::DestroyB(scene)`, +5056 = 0 when the scene has no load buffer of its own (+40 is 0), `SceneAssets::Release(+64)`, `CarScene::~CarScene(scene, 0)`, `VirtualFree(scene, 0, MEM_RELEASE)` and `[0x826C2E44]` = 0. In other states, the scene models' updates (`sub_820C5A60`, `vehicle-accessors.md`) and `[0x8260185C]` = 3 (counted down by one a tick by `Modes::Tick`, `game-modes.md`; no other reader found). Then always `ScenePools::UpdateA`, and when the scene is gone and the accessories' set (`sub_820F0E50`) exists, `Accessories::Destroy` (`accessories.md`). 9 callers seen: `Race::DrawStep` (`functions.md`, every draw pass in a race), `RaceScene::Teardown` (50 times, `race-scene.md`), `LocationScreen::PollLoad`, `Update` and `PollSceneTeardown` (`location-screen.md`), `sub_821516F0` (the course scenery's owners' lead), `sub_8217F880` (the scene models' owners' lead), the front end's car draw `sub_8217FE60` and its teardown `sub_82179AE8` (leads). Read in full. |
| `sub_820C3DA0` | `ScenePools::UpdateA` | Overridden | No arguments. When the first set (`[0x826C2E40]`) exists: in state 4 (+4, a teardown started) with +0 not 0, +0 counts down by one; at 0 `ScenePools::DestroyA(set)`, then each of the 4 × (+40 + +12) deferred release records at `[set + 52]` (20 bytes) whose +16 is above 0: `Texture::Free(+0, +4)` and `Texture::Free(+8, +12)` (`car-models.md`) and +0 to +12 = 0; then `FreePagesIn(set + 52)`, `VirtualFree(set, 0, MEM_RELEASE)` and `[0x826C2E40]` = 0. In other states, each record whose +16 is above 0 counts it down by one, and at 0 frees its two textures the same way and clears +0 to +12 (the releases `DeferRelease`, `sub_820C3F50`, starts with +16 = 2: textures freed two updates after their release). Only caller `CarScene::Update`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, over the allocation layer (a teardown due needs `DestroyA`'s objects, the records' block and the set to be the layer's pages, else the original); `Texture::Free`'s plain C++ where its adapter would run it, else its adapter as a guest call (the destructors `DestroyA` calls may put off releases first; harness `scene`). |
| `sub_820C3CF8` | `ScenePools::DestroyA` | Native | r3 = the first set: for each of its seven pools (+56 + 12 × pool), while `List::At(list, 0)` finds an object: its vtable slot 0 with r4 = 0 (the deleting destructors with flags 0, `scene-loaders.md`, which unlink it), then `VirtualFree(object, 0, MEM_RELEASE)`. The pool's index is compared against 0 to 7 by branches that all take the same path. Only caller `ScenePools::UpdateA`. 1 indirect call. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, over the allocation layer (every object the layer's pages, else the original); the destructors are guest calls through their vtables from the original's 128-byte frame (the lead's call: the pool objects' destructors through `ctx`; harness `scene`). Adapter removed (2026-11-01): its native caller `ScenePools::UpdateA` (its destructors' guest calls through the caller's callback) calls the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820D09A0` | `FreePagesIn` (probably) | Overridden | r3 = where a block's address is: `VirtualFree([r3], 0, MEM_RELEASE)`, a tail call (the word not cleared). As `FreePagesAt` (`sub_820C3CE0`, `car-models.md`) with the address in r3. 6 callers seen: `ScenePools::UpdateA` (the release records), `SceneAssets::Release` (the TGA pixels) and `sub_820E7598`, `sub_820E79C8`, `sub_820E7CA0`, `sub_820E8028` (the second set's objects, not read). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, over the allocation layer (`--native_heap`; the original for a word that isn't the layer's pages, 0 too; harness `scene`). |
| `sub_820C4D88` | `CarScene::StartTeardown` | Overridden | No arguments: when the scene exists, +0 = 4 and +48 = 2 (its teardown, run by the next two `CarScene::Update` calls), `ScenePools::StartTeardownA`, then `Accessories::ReleaseAll(sub_820F0E50())` (`accessories.md`). From `RaceScene::Teardown` (`race-scene.md`), `LocationScreen::PollSceneTeardown` (`location-screen.md`) and the front end's `sub_82179AE8` (a lead). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, with `Accessories::ReleaseAll`'s plain C++ where its adapter would run it (`--native_accessories`, the layer, the models' lifetime, `SetReleasable`; else the original; harness `scene`). |
| `sub_820C3C80` | `ScenePools::StartTeardownA` | Native | No arguments: the first set's (`[0x826C2E40]`, not checked) +4 = 4 and +0 = 2. Leaf. Only caller `CarScene::StartTeardown`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene` (harness `scene`). Adapter removed (2026-11-01): its native caller `CarScene::StartTeardown` calls the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820C5798` | `CarScene::LoadsDone` (probably) | Overridden | r3 = the scene: 0 while any of the shared assets' slots +0, +48, +72, +184, +388, +880, +4072 and +4680 (their +20; scene +84 to +4764) is loading (2); then, for each of the second set's eight pools and each object in it (`List::At`) that's in use (+16 = 3), 0 unless it's finished: pool 0 `sub_820D4048(object +24)` nonzero and the car model's decals (+4088) not loading (2); pool 1 `sub_820E2E68`, 2 `sub_820DBFF8`, 3 `sub_820E4140`, 4 `sub_820DDA60`, 5 `sub_820E7328`, 6 `sub_820E5A68` (each with the object, nonzero; the parts' finishers in `load-poll.md`); pool 7 the scene model's deferred build (+5040) 0. 1 when all are. A jump table on the pool. From `ScenePools::DestroyB` (its answer not read), `LocationScreen::PollSceneTeardown` (`location-screen.md`), `sub_821516F0` and `sub_82179AE8` (leads). Read in full. Overridden: plain C++ `scene::LoadsDone` (`src/scene/scene.cpp`) behind `--native_scene`, `List::At`'s plain C++, the finishers (`CarModel::IsReady` and the six `::Finish`) as guest calls through their overrides (their diagnostics' wraps) with r4 the object's index as `List::At` leaves it, adapter in `src/hooks/scene.cpp` (its callers are recompiled); harness `scene` (the finishers' finished words set, the 0 answers from the asset slots, pool 0's car models and pool 7; B7 loop's build screen run, 2026-11-06). Not called in a 40-second attract race. |
| `sub_820C59B0` | `ScenePools::DestroyB` | | r3 = the scene: `CarScene::LoadsDone(scene)` (its answer not read: the parts' finishers run once more), then for each of the second set's eight pools (+4944 + 12 × pool), while `List::At(list, 0)` finds an object: its vtable slot 0 with r4 = 0 (the pool objects' destructors, not mapped), then `VirtualFree(object, 0, MEM_RELEASE)`. A jump table on the pool whose eight cases all take that path. Only caller `CarScene::Update`. 1 indirect call. Read in full. |
| `sub_820C4C80` | `SceneAssets::Release` | Overridden | r3 = the shared assets: `LoadSlot::ReleaseBuffer` (`car-models.md`) on the slots +0, +48, +72, +184, +252, +320, +388, +880, +904, +952, +1020, +4048, +4072 and +4560, `FreePagesIn(+948)` (the TGA's pixels), `Model::Release(+4096)`, then +1088 + 24i and +3040 + 24i (four pairs), +3136 + 24i with `FreePagesIn(+3592 + 4i)` (19), +4608 + 24i (four), in that order; then `SceneAssets::Reset`. A 128-byte frame. Only caller `CarScene::Update`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native, every slot's buffer and pixels' pages are freed natively (each its own) and the pending frees have room; a pixels' word that isn't the layer's pages goes to `FreePagesIn`'s original as a guest call (harness `scene`, on the game's list). |
| `sub_820C9110` | `CarScene::~CarScene` | Overridden | r3 = the scene, r4 = flags: `List::~List` on the eight lists, last first (+5028 down to +4944; empty by then, `ScenePools::DestroyB`), `SceneAssets::~SceneAssets(+64)`, the empty `sub_82106540` on +4 (the set-up record's destructor, `audio.md`), then `operator delete(scene)` (`heap.md`) when r4's bit 0 is set. Returns r3. Only caller `CarScene::Update` (r4 = 0: the pages freed after). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native and `~SceneAssets` would run natively (and, with r4's bit 0, the scene a heap block); the lists' destructors through `~List`'s plain C++, the set-up record's empty destructor left out (harness `scene`, on the game's list). |
| `sub_820C5168` | `SceneAssets::~SceneAssets` | Native | r3 = the shared assets: what `SceneAssets::SceneAssets` made, last first: `~TextureProvider` on +4800 + 20i (four, last first), `XPR::~XPR` on +4704 + 24i (four) and +4584, `PartModel::~PartModel(+4096)`, the empty `sub_82106540` on the 19 TGA headers (+3668 + 20i), `~PartModel` on +1184 + 464i (four), then +1068, +1044, +1000, +976, +928 (the header), +416, +368, +344, +300, +276, +232, +208, +164, +144, +120, +96, +24 (`models.md`). Only caller `CarScene::~CarScene`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`, where the models' lifetime is native and every member's plain C++ would run (the providers' storage the layer's heap blocks, the part models' members, room in the pending frees; else the original); the TGA headers' empty destructor left out (harness `scene`, on the game's list). Adapter removed (2026-11-01): its native caller `CarScene::~CarScene` calls the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_82594188` | `List::~List` (probably) | Native | A jump to `List::DeleteAll`. Only caller `CarScene::~CarScene`. Read in full. Native in `src/scene/scene.cpp` behind `--native_scene` (`ListDestruct`, calling `DeleteAll`'s plain C++; harness `scene`). Adapter removed (2026-11-01): its native caller `CarScene::~CarScene` calls the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_82593FF8` | `List::DeleteAll` (probably) | Native | r3 = a list: while its first node (+0) isn't 0, that node's vtable slot 0 with r4 = 1 (its deleting destructor, which unlinks it). Only caller `List::~List`. 1 indirect call (probably not taken in play: the scene's lists are empty by then). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene`; the nodes' deleting destructors are guest calls through their vtables from the original's 96-byte frame (harness `scene`). Adapter removed (2026-11-01): its native caller `List::~List` (its destructors' guest calls through the caller's callback) calls the plain C++, and `--native_scene` now switches only the callers; the harness keeps its register step. |
| `sub_820C8950` | `CarScene::SetInteriorWord` (probably) | Overridden | r3 = the scene, r4 = a word: for each scene model in pool 7 (+5028, its count +5036; `List::At`) whose interior is ready (+5072), the interior's (+5068) +11616 = r4 (read by `sub_820E53F0` for the accessory, `accessories.md`). Only caller the front end's car draw `sub_8217FE60` (a lead). Read in full. Native in `src/scene/scene.cpp` behind `--native_scene` (harness `scene`). |
| `sub_820C4E68` | `CarScene::GetTga` (probably) | | r3 = the scene, r4 = a 20-byte header (out), r5 = a word (out): memcpy of the scene's +992 (the TGA header at assets +928) into r4 and `[r5]` = +1012 (its pixels, assets +948). From `CarModel::OnFileLoaded`, `OnFileLoadedB` and `ReloadDecals` (`sub_820D47A8`, `sub_820D4F80`, `sub_820D5D20`, `car-models.md`): the car model's +3724 = 1 image branch, which those hand back to their originals. Not seen in play (the coverage list), though it ran in an attract race from `OnFileLoadedB` at `0x820D5694` (`itc_2848`, the car models' migration agent, 2026-10-31). A 112-byte frame. Read in full. |
