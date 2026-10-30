# Known functions: the location screen

A front-end screen of the career (`82227158`-`8222F258`, the lead's
"front-end menus"): the **location screen**, where the player is at one
of the six locations (the save data's +6108, 14 to 19; `career.md`,
`race-event.md`) and picks what to do there. It shows the location's
course scenery (`Scenery::`, `course-scenery.md`) through a camera
animation, the location's opponents (the race event's group for the
location, kinds 4 to 9: `RaceEvent::GroupItem`) as cars posed in the
scene (pool 7, `scene-models.md`), and two UI movies (21, the menus,
and 30, an opponent's card; `ui-movies-game.md`) with pictures loaded
from the archive as XPR textures. It is one of three screens of a
front-end mode (one of the 852-byte mode types, `game-modes.md`; which
one isn't known): `sub_8222FD68` (a lead) allocates a 28,940-byte block
at the mode's +848 with three state machines and adds this screen's to
the mode's dispatcher; this screen's is the one at +9640,
set up by `LocationScreen::InitStates` (`sub_8222F258`), with 19
transitions over 11 states: 0 create, 1 set up, 2 wait for the loads, 4
start, 5 running (message 0 the transitions and the draw, 1 the page's
input; 5, 6, 14, 15, 17, 18 and 20 from other screens), 7, 8, 9 and 10
leaving (message 4 unloads and destroys, then the state kept at the
machine's +9620: 11 after a race was set up, 13 otherwise). The screen
itself is a 912-byte object at `[0x82814E50]` with a page object
(`[0x82814E48]`: 16 pages, each an input handler run by
`LocationScreen::UpdatePage`, and 36 page transitions, each an animation
handler run by `LocationScreen::Update` until it reports done) and four
menus (`[0x82814E4C]`, `[0x82814E54]`, `[0x82814E58]`,
`[0x82814E5C]`, 68 bytes each, stepped by the front end's shared menu
helpers `sub_82186568` and `sub_821864C8`, a lead). Page 0 is the
location's menu; pages 1 and 2 the location's list (the save data's
location record) and its text; 3 to 6 the opponents, an opponent's
text, the confirm menu (race, the rival's card, back) and the card;
9 and 11 set up the race (`Race::SetUp` without or with the opponent,
`race-setup.md`) and leave; 12, 13 and 15 come after a race (its result
texts and the save data's list of unlocks to show). Names are ours and
mostly guesses ("probably" where unsure); what the location's list and
the unlocks are isn't known. Read 2026-10-30 by the B7 loop's mapping
agent: the state machine's table, the set-up, the loads, the teardown,
the small helpers and the page and transition tables in full; the pages
and transitions skimmed (their calls, constants and the page object's
writes), marked "Not read in detail". The fields are in
`../known-data.md` (`0x82814E48`-`0x82814E5C`, "location screen",
"screen picture", `0x82636518`, `0x82602EA0`, and the save data's
location records and unlock list under `0x826C5B58`).

Shared, with their rows where they are: `CourseView::Draw`
(`sub_822282E8`, `camera-modes.md`: drawn by `LocationScreen::Update`),
`StateMachine::Add` (`game-modes.md`), the camera animations
(`CameraAnims::Load`, `Find`, `Evaluate`, `ObjectMatrix`, `Clear`,
`FreeAll`, `SetGlobals`, `sub_82150B80`; `camera-modes.md`), the
viewports and passes (`Viewports::Init`, `Release`,
`CameraPasses::EnableForView`; `cameras.md`), the course scenery
(`Scenery::Create`, `Setup`, `StartLoad`, `IsLoaded`, `Unload`,
`Destroy`; `course-scenery.md`), the scene models
(`SceneModelRecord::SceneModelRecord`, `Reset`, `ScenePools::SetUpB`;
`scene-models.md`), the cars (`Car::Init`, `Car::FromBuild`,
`race-setup.md`; `Car::Refresh`, `Params::GetCarSpec`, `car-specs.md`;
`CarData::CarData`, `Vehicle::SetupFromCar`, `car-physics.md`), the
matrix math (`Matrix::Multiply`, `RotateX`, `RotateY`, `RotateZ`,
`Inverse`; `matrix-math.md`), the vehicle's accessors (`SaveData::Of`,
`RacerRecords::At`, `sub_820FFC68`, `ScenePools::ClaimB`, `GetB`,
`sub_825C8390` (returns 1: transitions 21 and 22);
`vehicle-accessors.md`), the race event (`RaceEvent::Current`,
`race-setup.md`; `RaceEvent::GroupItem`, `Items::Available`,
`race-event.md`), the career (`Career::EventQuery`,
`Progress::AfterRace`, `ItemFlags::Set`; `career.md`), `Race::SetUp`
(`race-setup.md`), the data tables (`DataTables::Item`, `Entry56`,
`Find20`, `Entry20`; `data-tables.md`), the UI movies (`UiMovies::Find`,
`Open`, `IsLoaded`, `Delete`, `UiMovie::Run`, `PlayNamed`, `Play`,
`LoopLast`, `FinishNamed`, `Finish`, `FinishAll`, `ResetPose`,
`ResetNode`, `ShowNode`, `HideNode`, `FindNode`, `SetText`,
`SetNodeText`, `SetNodeTexture`, `SetNodeColor`, `SetPictureTexture`,
`GetPictureTexture`, `RevealText`, `ScrollText`, `sub_8211D8D8`,
`sub_8211C4F0`, `sub_8211C5F8`; `ui-movies-game.md`; `UiMovie::Child`,
`ui-movies.md`), `ScreenFades::FadeIn`, `FadeOut` (`mode-tick.md`),
`XPR::XPR`, `Create` (`models.md`), `XPR::Release`
(`strings-and-font.md`), `XPR::GetTexture` and the render orchestrator
`sub_820C5DE8` (`functions.md`), `Texture::GetLevelDesc`
(`graphics-resources.md`), `Archive::AssetSize`, `Archive::Load`
(`files-and-saves.md`), `Archive::CancelQueued` (`archive.md`),
`VirtualAlloc`, `VirtualFree`, `operator new`, `operator delete`
(`heap.md`), `memcpy`, `sin`, `XMemSet` (`c-runtime.md`) and the empty
`sub_82106540` (`audio.md`: page 10, and a sound call's stand-in at the
draw's start). Not here, in leads (`../loops/leads.md`): the menus'
input helpers `sub_82186568` and `sub_821864C8`, the menu sound
`sub_821103F0`, the scene's update, constructor and teardown
(`sub_820C9608`, `sub_820C8A50`, `sub_820C5798`, `sub_820C4D88`,
`sub_820D0D50`), the scene models' load poll `sub_820DA318` and record
builder `sub_8215B878`, and the mode's set-up `sub_8222FD68` with the
mode's two other screens. The course scenery's owners' lead names this
screen's create, set-up, load poll, unload and destroy; their rows are
here. Not seen in play (`../coverage.md`): 22 of them, marked.

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
| `sub_8222F258` | `LocationScreen::InitStates` | | r3 = the screen's state machine (the mode's block + 9640, from `sub_8222FD68`, which sets +9612 = 1 and +9616 = 0): 19 transitions through `StateMachine::Add` (functor `0x82011918`, the machine as the object), as (state, message) → handler, next state: (0, 1) `Create`; (1, 1) `SetUp`, 2; (2, 1) `PollLoad`; (4, 1) `Start`, 5; (5, 1) `UpdatePage`; (5, 0) `Update`; (5, 5) `RunCardMovie`; (5, 14) `OnMessage14`; (5, 15) and (5, 17) `OnMessage15`; (5, 18) `ApplyQueuedPage`; (5, 20) `OnMessage20`; (5, 6) `OnMessage6`, 8; (8, 3) `OnMessage3`; (9, 7) `sub_822895E0`; (5, 4), (9, 4), (10, 4) `Unload`; (7, 4) `Destroy`, 6. Read in full (its table). |
| `sub_822895E0` | `ScreenMachine::Reenter` (probably) | | r3 = a screen's state machine: +9604 (the state) = 4 when +9608 (the screen's movies loaded) is set, else 1. Leaf. No direct callers: a handler of this screen's (state 9, message 7) and of the title mode's machine (`sub_822213B0`, `../loops/leads.md`). Read in full. |
| `sub_82227A50` | `LocationScreen::Create` | | r3 = the machine (state 0, message 1): +9608 = 0; allocates (`VirtualAlloc`) the page object (28 bytes, `[0x82814E48]`: +0, +4, +8, +24 = 0, +12, +20 = -1) and the screen (912 bytes, `[0x82814E50]`, reset by `LocationScreen::Reset`); +864 = the save data's location (+6108), and by it (14 to 19) +852 the location's index (0 to 5) and +804 the race event's group kind (4 to 9; others leave 0 and 4); +812 = the location record's count (save +0x1AD20 + 100 × index, its +96); +808 = the group's opponent count (`RaceEvent::Current`'s halfword at 12 × (kind + 7) for kinds 4 to 9, else at +84); four 68-byte menus (`[0x82814E4C]` 5 items, its cursor 2 when +816 is set; `[0x82814E54]` +812 items; `[0x82814E58]` +808 items; `[0x82814E5C]` 3 items); `Scenery::Create` and +552 = `[0x826C551C]`; the scene (`sub_820C8A50`); a 48-byte record on the stack (`sub_820D0D50`, words 1 and 7) to `sub_820C8DE8` (`car-models.md`) on the scene; `Viewports::Init`, `CameraPasses::EnableForView`, `CameraAnims::Clear`; +9604 = 1 when +9612 is set, else 9. Read in full. |
| `sub_822279A8` | `LocationScreen::Reset` | | r3 = the screen: +0, +552, +800, +856, +860 = 0, +864 = 14; `LocationScreen::ResetView(+560)`; the picture lists' counts (+136, +404) and default textures (+544, +548) = 0; +804 = 4; +808, +812, +816, +820, +852, +868, +872, +876, +888, +892, +900 = 0; +880, +884 = -1. Only caller `LocationScreen::Create`. Read in full. |
| `sub_82227818` | `LocationScreen::ResetView` (probably) | | r3 = the screen + 560: +0, +4, +12 = 0, +8, +16 = 1; three 16-byte vectors at +32, +48, +64 of (`[0x82000A34]` ×3, `[0x82000A30]`; probably 0, 0, 0, 1) and +80 likewise, +96 to +108 = `[0x82000A34]`; two identity matrices (64 bytes, `memcpy`) at +112 and +176. Only caller `LocationScreen::Reset`. Read in full. |
| `sub_8222C300` | `LocationScreen::SetUp` | | r3 = the machine (state 1, message 1; then 2). The location's three camera animations loaded (`CameraAnims::Load` of the words +0, +4, +8 of `0x82636518` + 16 × index); the pictures queued (`LocationScreen::LoadPicture`, each id onto the ring at +140 or +408 as it goes): for each entry of the unlock list (save +0x1C778, count +0x1CA78) and of the location record, the 56-byte entries (`DataTables::Entry56`, chained by +8) whose +16 isn't -1, as assets 22696 + id (list A) and 22741 + id (list B); for each of the group's opponents, the opponent's 20-byte entries (`DataTables::Find20` 0 to 12, `Entry20`, chained by +4) not already queued (`LocationScreen::IsPictureQueued`); the human racer's (`RacerRecords::At(-1)`) the same way from +872 (then +872, +876 = 0); the scene's +5052 = 1; for each opponent a car on its stack (`Car::Init`, `FromBuild` from the item, flags +3888 |= 2, `Car::Refresh`), a scene model record (`SceneModelRecord::SceneModelRecord`, `Reset`, `sub_8215B878`) and a pool 7 model (`ScenePools::ClaimB`, into +824 + 4i, `ScenePools::SetUpB`, its +4296 = 1); the scenery's configuration by `LocationScreen::SceneryConfig(word +12, save +6104)` from `0x82602EA0` (8 bytes an entry, into a 44-byte configuration), `Scenery::Setup`, `Scenery::StartLoad`; UI movies 21 (asset 22563) and 30 (22573) opened (`UiMovies::Open`); `ScreenFades::FadeIn([0x82001BEC])`; +896 = 0. 449 lines. Read in full. |
| `sub_8228F0A0` | `LocationScreen::SceneryConfig` (probably) | Overridden | (r3 + 2) × 3 + r4: an index into `0x82602EA0`'s 8-byte entries, from a location's word +12 and the mode (save +6104). Leaf. Only caller `LocationScreen::SetUp`. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_8222C270` | `LocationScreen::LoadPicture` | | r3 = the screen, r4 = an asset id, r5 = 0 or 1 (the list): allocates the asset's size (`Archive::AssetSize`, `VirtualAlloc`), clears it (`XMemSet`) and asks `Archive::Load(id, buffer, callback, screen)` with `LocationScreen::OnPictureLoadedA` (r5 = 0) or `B` (1); nothing for other r5. Only caller `LocationScreen::SetUp`. Read in full. |
| `sub_82228E88` | `LocationScreen::OnPictureLoadedA` | | The archive's load callback (r4 = the buffer, r6 = the screen; no direct callers): a 16-byte picture (`VirtualAlloc`; +0 loaded, +4 the id, +8 a 12-byte {texture, width, height} from `operator new`, +12 an XPR from `operator new(24)`, `XPR::XPR`, `XPR::Create` over the buffer); the texture `XPR::GetTexture(0)`, its size from `Texture::GetLevelDesc`; the id taken from ring A's front (+140, count +268 down, start +272 up); the buffer freed (`VirtualFree`); +0 = 1; appended to list A (+8, count +136). Read in full. |
| `sub_82228FC8` | `LocationScreen::OnPictureLoadedB` | | The same for list B (ring +408, list +276, count +404). Not read in detail (the same calls and size as `OnPictureLoadedA`). |
| `sub_822291E8` | `LocationScreen::IsPictureQueued` | Overridden | r3 = a picture id: 1 when ring A (+140, from +272, +268 entries of 32) holds it, else 0. Leaf. Only caller `LocationScreen::SetUp`. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_82229108` | `LocationScreen::PictureA` | Overridden | r3 = a picture id: the texture record (+8) of list A's picture with that id, else the default +544. Leaf. 8 callers (5 seen: the transitions and `ShowLocationEntry`, `ShowUnlockEntry`), each passing it to `UiMovie::SetPictureTexture` (node 6, slot 1). Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_82229158` | `LocationScreen::PictureB` | Overridden | The same over list B (+276, count +404), the default +548; its callers pass it to slot 0. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_82229238` | `LocationScreen::PollLoad` | | r3 = the machine (state 2, message 1). Returns early (to run again) until each is ready: the location's camera animation (`CameraAnims::Find`), evaluated into +592 to +623 (two vectors) and +664, +668; the scene's update (`sub_820C9608`) and `sub_820C5DE8(scene, 0)`; each opponent's model (`ScenePools::GetB(scene, 7, [+824 + 4i])`): its matrix from the third animation (`CameraAnims::ObjectMatrix` with `0x82010780`'s words), posed (`LocationScreen::PoseCar`), loaded (`sub_820DA318`); every list A picture loaded (+0); movies 21 and 30 loaded (`UiMovies::IsLoaded`) and `Scenery::IsLoaded`. Then once `ScreenFades::FadeOut(30)` (+896 = 1), +544 and +548 = movie 21's node 6 textures (`UiMovie::GetPictureTexture`), +9608 = 1 and the state 4. 214 lines. Read in full. |
| `sub_82228588` | `LocationScreen::PoseCar` | | r3 = an opponent's scene model + 4288, r4 = the opponent's index in the group: the item's car built on the stack (`Car::Init`, `FromBuild`, flags +3888 \|= 1, `Car::Refresh`) and its physics data (`CarData::CarData`, `Vehicle::SetupFromCar`) read for the body's height and the wheels' places, angles through `sin`; the model's matrix (+16, the model's +4304) moved and turned (`Matrix::Multiply`, `RotateZ`, `RotateY`), its inverse kept on the stack; the four wheels' matrices (+80 and +336 + 64k, 2 × 2) set from it (`RotateX`, `RotateY`, `RotateZ`, `Multiply`); +732 = 1. 342 lines, a 9,424-byte frame. Only caller `LocationScreen::PollLoad`. Not read in detail. |
| `sub_8222F0B0` | `LocationScreen::Start` | | r3 = the machine (state 4, message 1; then 5): +892 = 1 when the garage's selected car's spec (`Params::GetCarSpec`, save +0x19920 walked through the garage's list) has byte +771 = 1; `LocationScreen::ResetMovie`; the first page: after a race (`RacerRecords::At(-1)` and `sub_820FFC68` nonzero, +816 = 0) page 12 through transition 23 (+872, +876 = 0), else with an unlock to show (save +0x1CA78) page 13 through 25, else page 0; `ScreenFades::FadeIn(30)`. Read in full. |
| `sub_8222EC08` | `LocationScreen::UpdatePage` | | r3 = the machine, r4 = the dispatcher (state 5, message 1): unless the page (+0 of `[0x82814E48]`) is -1, the page's handler from its 16-entry table (pages 0 to 15: `Page0` to `Page15`; page 10 the empty `sub_82106540`) with r3 = the dispatcher. Then `[0x8265ECE0]` = the screen's +592 (16 bytes); when +900 is set it's cleared and the page object restarted: a pending transition (+12) begun again (the page -1), or the page put back to the one this update started with; +16 = -1. 1 indirect call. Read in full. |
| `sub_8222ED98` | `LocationScreen::Update` | | r3 = the machine (state 5, message 0): `sub_82106540(100, 30, 0, 0x82010908)`; while a transition runs (+12 of `[0x82814E48]` not -1) its handler from the 36-entry table (`Transition0` to `Transition35`; 21 and 22 `sub_825C8390`) with r3 = the page object + 24 (its step), and when it reports done the page becomes +4 (or the queued +16 starts), +24 = 0; else `UiMovie::Run(movie 21, 0)`. Then `CourseView::Draw([0x82814E50])`, the scene's +5052 = 1, `LocationScreen::UpdateView`, the scene's update (`sub_820C9608`) and `sub_820C5DE8(scene, 0)`. 1 indirect call. Read in full. |
| `sub_82227E20` | `LocationScreen::UpdateView` (probably) | | No arguments: the second camera animation's object matrix (`CameraAnims::ObjectMatrix`, the menu `[0x82814E54]`'s cursor into `0x82010780`'s words) into its frame; then copies the constants at `0x826366A0`, `0x826366B0` and `0x826366C0` into `0x8296B950`, `0x8296B980`, `0x8296B9B0`, `0x8265ED00` and fills 128 bytes at `0x8296BAE0` and `0x8296BB60` from its frame. Probably the lights of the course view. Only caller `LocationScreen::Update`. Not read in detail. |
| `sub_82227158` | `LocationScreen::OnMessage14` | Overridden | (state 5, message 14) +880 = 0, +884 = -1; when the menu `[0x82814E4C]`'s cursor is 3, +880 = 8, 4: +880 = 9, with +884 = 1 (transition 1, the fade out). Probably the answer of another screen's dialog (pages 8 and 9 leave). Leaf. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_822271B8` | `LocationScreen::OnMessage15` | | (state 5, messages 15 and 17) +880 = 0, +884 = -1 (back to page 0). Leaf. Read in full. Not seen in play. |
| `sub_822271D8` | `LocationScreen::OnMessage20` | | (state 5, message 20) +880 = 10, +884 = -1. Leaf. Read in full. Not seen in play. |
| `sub_8222C9B0` | `LocationScreen::ApplyQueuedPage` | | (state 5, message 18) `LocationScreen::HidePictures` when +880 is 0; then the page object: +8 = the page when it differs from +880, +16 = -1, and either the page = +880 (no transition, +884 = -1) or +12 = +884, +4 = +880, the page -1; +880, +884 = -1. Read in full. |
| `sub_822291A8` | `LocationScreen::RunCardMovie` | Overridden | (state 5, message 5, posted every update by `Page6`) `UiMovie::Run(movie 30, 0)`. Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`; `UiMovie::Run` through its override from a frame like the original's, with its return address); checked by `--run_harnesses=location_screen` (`Run`'s update stood in). |
| `sub_82229578` | `LocationScreen::OnMessage6` | | (state 5, message 6; then 8) `UiMovie::FinishAll` on movies 21 and 30; when the machine's +9616 is set they're deleted (`UiMovies::Delete`) and +9608 = 0. Read in full. Not seen in play. |
| `sub_82229610` | `LocationScreen::OnMessage3` | | (state 8, message 3) when the screen's +4 is 2, message 9 posted to the dispatcher's ring (r4 + 204; 50 12-byte entries, count +600, start +604); the state 9. Leaf. Read in full. Not seen in play. |
| `sub_82229678` | `LocationScreen::Unload` | | (states 5, 9, 10, message 4) state 7; movie 21 finished, its node 6 pictures put back to +544 and +548 (`UiMovie::SetPictureTexture`) and deleted; movie 30 finished and deleted; `Archive::CancelQueued(1)`; `Scenery::Unload(+552)`; message 4 posted to the dispatcher's ring (to `Destroy`). Read in full. |
| `sub_82228548` | `LocationScreen::Destroy` | | (state 7, message 4) the state = the machine's +9620 (11 or 13, set by the leaving pages); `CameraAnims::FreeAll`, `Viewports::Release`, `Scenery::Destroy`, `LocationScreen::FreePictures`. Read in full. |
| `sub_82228498` | `LocationScreen::FreePictures` | | r3 = the screen: each picture of list A, then B, taken from the back, freed (`Picture::Free`, `VirtualFree`). Only caller `LocationScreen::Destroy`. Read in full. |
| `sub_82228438` | `Picture::Free` | | r3 = a screen picture: +0 = 0; the 12-byte texture record cleared and deleted (`operator delete`); the XPR (+12) released (`XPR::Release`); +8, +12 = 0. Only caller `LocationScreen::FreePictures`. Read in full. |
| `sub_8222FCA0` | `FrontEndMode::NextMode` (probably) | | r3 = an 8-byte answer, r4 = the mode: the answer cleared; when `LocationScreen::Free` reports done, the answer = the 8 bytes at the mode's +848 (the block) + 28928. No direct callers: probably a method of the mode (`Modes::Tick` reads such an answer from the mode's delegate, `game-modes.md`; this mode's delegate is another functor, `0x8223D4B0` with vtable `0x82010FA0`, set by `sub_8222FD68`). Read in full. |
| `sub_82228220` | `LocationScreen::Free` | | No arguments: `LocationScreen::PollSceneTeardown([0x82814E50])`; once done, the page object, the four menus and the screen freed (`VirtualFree`) and their pointers (`0x82814E48`-`0x82814E5C`) cleared; returns 1, else 0. Only caller `FrontEndMode::NextMode`. Read in full. |
| `sub_82227780` | `LocationScreen::PollSceneTeardown` | | r3 = the screen: when +856 is 0, `sub_820C5798(scene)` and, when it returns nonzero, +856 = 1, +860 = 5 and `sub_820C4D88`; then each call counts +860 down, running the scene's update (`sub_820C9608`), and at 0 clears +856 and returns 1. Only caller `LocationScreen::Free`. Read in full. |
| `sub_8222CA48` | `LocationScreen::Page0` | | The location's menu (page 0; pages get r3 = the dispatcher, r4 = the machine): the menu `[0x82814E4C]` stepped (`sub_82186568`); on accept (`sub_821864C8(menu, 159)`) by the cursor: 0, page 1 through transition 4 when the location record has entries, else sound 3 (`sub_821103F0`); 1, +800 = 0 and page 3 through 9; 2, when +892 is 0, +816 = 1 and page 7 through 7, else sound 3; 3 and 4, movie 16's texts (30 or 31, then (1, 3, 2) and (1, 4, 3)) and its word +0xA000 = 1, message 10 posted to the dispatcher, page 10, sound 0 and `LocationScreen::ShowPictures`. On back (158): sound 2, the cursor 4, transition 3. When the cursor moved (+8): sound 4, transition 2. Not read in detail. |
| `sub_82228A90` | `LocationScreen::Page1` | | The location's list (menu `[0x82814E54]`): accept, sound 0 and page 2 through transition 7; back, sound 2 and page 0 through 5; moved, sound 4 and transition 6. Read in full. |
| `sub_82229798` | `LocationScreen::Page2` | | The list entry's text: on accept, sound 0 and `UiMovie::RevealText(movie 21, 4, 2)`; when it has no more, the menu `[0x82814E54]`'s +24 = 0 and page 1 through transition 8; node (4, 0) shown while `sub_8211D8D8(4, 2)` says more is left. Not read in detail. |
| `sub_82228B80` | `LocationScreen::Page3` | | The opponents (menu `[0x82814E58]`): accept, sound 0 and page 4 through transition 12; back, sound 2 and page 0 through 10; moved, sound 3 or 4 and transition 11. Not read in detail. |
| `sub_82229888` | `LocationScreen::Page4` | | The opponent's text (movie 21's node (5, 1)): on accept `UiMovie::RevealText`; when done, either `LocationScreen::NextOpponentText` finds more (+888 = 1, transition 9 with 12 queued, page 4) or, +888 = 0, by `Items::Available` of the opponent's item, page 5 or 3 through 9. Not read in detail. |
| `sub_82228CC0` | `LocationScreen::Page5` | | The confirm menu (`[0x82814E5C]`, 3 items): accept on 0, sound 0 and page 11 through transition 1 (the race); on 1, when the opponent's kind is 4 or 5 (`LocationScreen::OpponentHasCard`), page 6 through 17, else sound 3; on 2 or back, page 3 through 9; moved, sound 4 and transition 16. Read in full. |
| `sub_82229A48` | `LocationScreen::Page6` | | The opponent's card (movie 30): accept or back, sound 0 and page 5 through transition 18; every update posts message 5 (`LocationScreen::RunCardMovie`) to the dispatcher's ring. Read in full. |
| `sub_82229B38` | `LocationScreen::Page7` | | The screen's +4 = 2 and message 6 posted (`LocationScreen::OnMessage6`, then state 8, then message 3's `OnMessage3` posts 9): probably leaves for another screen and comes back (state 9). Leaf. Read in full. Not seen in play. |
| `sub_82228068` | `LocationScreen::Page8` | | The save data's count for the location (halfword +5886 + 2 × index) up by one (up to 9,999), `Progress::AfterRace(save)`, the machine's +9620 = 13 and the state 10 (leaving). Read in full. Not seen in play. |
| `sub_822280D8` | `LocationScreen::Page9` | | `Career::EventQuery(save, 0)`, `Race::SetUp(save, -1)` (a race without a picked opponent), +9620 = 11, the state 10 (leaving). Read in full. |
| `sub_82228138` | `LocationScreen::Page11` | | The picked opponent's item (`RaceEvent::GroupItem` of +800), `Career::EventQuery(save, 1)`, `Race::SetUp(save, the item's id)`, +9620 = 11, the state 10. Read in full. |
| `sub_82229CF0` | `LocationScreen::Page12` | | After a race (from `LocationScreen::Start`): on accept, `UiMovie::RevealText(movie 21, 4, 2)`; when done, `LocationScreen::NextResultText` for more, else page 8 through transition 24 (+8 = -1) or, with an unlock to show, page 13 through 24 with 25 queued. Not read in detail. |
| `sub_82229E98` | `LocationScreen::Page13` | | The unlocks: on accept, `RevealText(4, 2)`; when done, `LocationScreen::NextUnlockEntry` for the entry's next part, else `LocationScreen::HasMoreUnlocks` (page 13 through 26 with 25 queued); at the end page 8 through 26, or page 0 through 26 with 29 queued, +868 = 0 and the list's count (save +0x1CA78) = 0. Not read in detail. |
| `sub_82228E18` | `LocationScreen::Page14` | | Accept: sound 0 and page 3 through transition 9. Read in full. Not seen in play. |
| `sub_82229B98` | `LocationScreen::Page15` | | As `Page12`, with `LocationScreen::NextResultTextB`: page 13 through 24 with 25 queued. Not read in detail. Not seen in play. |
| `sub_822271F8` | `LocationScreen::NextResultText` | Overridden | Racer 0's item (`RacerRecords::At(0)` +980): its 20-byte entry k (`DataTables::Find20`; k 12, or 10 or 11 when the human racer's (`At(-1)`) +3924 is 1, by an item record's halfword at save +0x199FA + 12 × id) at +872, `Entry20`; when its +4 (the next) isn't 0, +872 += it and 1, else +872, +876 = 0 and 0. Only caller `Page12`. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`; the item copied onto its frame with memcpy's plain C++, the original where that copy can't be native); checked by `--run_harnesses=location_screen`. |
| `sub_82227310` | `LocationScreen::NextResultTextB` | | Likewise from the human racer (`RacerRecords::At(-1)`) only; `[0x82814E68]` = 1 on one path. Only caller `Page15`. Not read in detail. Not seen in play. |
| `sub_82227430` | `LocationScreen::NextOpponentText` | | The picked opponent's item (`RaceEvent::GroupItem` of +800) and `Items::Available`, then an item by its halfwords (3 and 65,535 tested) and its 20-byte entries (`Find20`, `Entry20`) stepped at +872 as `NextResultText`; 1 while more. Only caller `Page4`. Not read in detail. |
| `sub_82227618` | `LocationScreen::NextUnlockEntry` | Overridden | `DataTables::Entry56(+876, +872)`: when its +8 (the next) isn't 0, +872 += it and 1; else +872, +876 = 0 and 0. Only caller `Page13`. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_822281D8` | `LocationScreen::HasMoreUnlocks` | Overridden | 1 when +868 is below the save's unlock count (+0x1CA78), else 0. Only caller `Page13`. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_82227708` | `LocationScreen::OpponentHasCard` (probably) | Overridden | The picked opponent's item (`RaceEvent::GroupItem` of +800): 1 when its halfword +88 is 4 or 5, else 0. 4 callers (`Page5`, `ShowCard`, `Transition14`, `Transition16`). Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`; the item copied onto its frame with memcpy's plain C++, the original where that copy can't be native); checked by `--run_harnesses=location_screen`. |
| `sub_82227690` | `LocationScreen::KindText` (probably) | Overridden | r3 = a kind (1 to 7), r4 = an index: a string id, r4 plus 6718, 7108, 6722, 7268, 6887, 7017 or 7411 by the kind; 0 for others. Leaf. Only caller `Transition7`. Read in full. Native under `--native_location_screen` (`src/location_screen/helpers.cpp`); checked by `--run_harnesses=location_screen`. |
| `sub_8222A060` | `LocationScreen::ShowPictures` | Overridden | Movie 21: node (1, 4) and (1, 5) textures 0, (1, 6) and (1, 7) textures 1 (`UiMovie::SetNodeTexture`), nodes (1, 6), (1, 7) shown, (1, 8), (1, 9), (1, 13), (1, 14) hidden. 5 callers. Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`: the node setters' plain C++ called directly; the movie's id stored at its frame's +80 for `UiMovies::Find` as the original); checked by `--run_harnesses=location_screen`. |
| `sub_8222A150` | `LocationScreen::HidePictures` | Overridden | Movie 21: nodes (1, 4), (1, 5) textures 0, nodes (1, 6) to (1, 9), (1, 13), (1, 14) hidden. 13 callers (8 seen). Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`: the node setters' plain C++ called directly; the movie's id stored at its frame's +80 for `UiMovies::Find` as the original); checked by `--run_harnesses=location_screen`. |
| `sub_8222AC18` | `LocationScreen::SetCarColor` (probably) | Overridden | Movie 21's node (9, 7) color (`UiMovie::SetNodeColor`) 2 when +892 is 1, else 0. 8 callers. Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`: the node setters' plain C++ called directly; the movie's id stored at its frame's +80 for `UiMovies::Find` as the original); checked by `--run_harnesses=location_screen`. |
| `sub_8222BFE8` | `LocationScreen::SetCursor` (probably) | Overridden | r3 = a number: movie 21's node (9, 0)'s +16 = r3 as a float. 6 callers (0, 40, 80, 160). Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`: the node setters' plain C++ called directly; the movie's id stored at its frame's +80 for `UiMovies::Find` as the original); checked by `--run_harnesses=location_screen`. |
| `sub_8222C058` | `LocationScreen::ListNode` | Overridden | r3 = a number: movie 21's node (10, 0)'s +16 = r3 as a float (as `SetCursor` for node (9, 0)), the node returned. Callers `Transition5`, `Transition6`. Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`: the node setters' plain C++ called directly; the movie's id stored at its frame's +80 for `UiMovies::Find` as the original); checked by `--run_harnesses=location_screen`. |
| `sub_8222C0C8` | `LocationScreen::ShowArrows` | Overridden | Movie 21's nodes (-1, 17) and (-1, 18) shown or hidden by the opponent's place in the group (+800 against +808: one opponent hides both, the first shows 18 and hides 17, the last shows 17 and hides 18, others show both). Returns the last node. 3 callers. Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`: the node setters' plain C++ called directly; the movie's id stored at its frame's +80 for `UiMovies::Find` as the original); checked by `--run_harnesses=location_screen`. |
| `sub_8222C188` | `LocationScreen::ShowOpponentSpecs` | | The picked opponent's car spec (`Params::GetCarSpec` of the item's car) written as texts into movie 21's nodes (12, 9), (12, 10), (12, 11) (`UiMovie::SetNodeText`). Callers `Transition9`, `Transition11`. Not read in detail. |
| `sub_8222CD60` | `LocationScreen::ResetMovie` | | Movie 21: `ResetPose`, `FinishAll`, texts (-1, 35), (-1, 36) and node 9's from the save data, a named animation played and looped, `LocationScreen::ShowPictures`. 8 callers. Not read in detail. |
| `sub_8222B048` | `LocationScreen::ShowListEntry` | | The location record's entry at the menu `[0x82814E54]`'s cursor (`DataTables::Entry56`): node 10's parts shown or hidden, its pictures (`PictureA`, `PictureB`, node 6) and text. Callers `Transition4`, `Transition8`. Not read in detail. |
| `sub_8222B200` | `LocationScreen::ShowOpponentPictures` | | Node 14's textures (`UiMovie::SetNodeTexture` slots 2 and 1) from the picked opponent's item and another item (`DataTables::Item`); node 14 hidden or shown. Callers `Transition9`, `Transition28`. Not read in detail. |
| `sub_8222B438` | `LocationScreen::ShowOpponentInfo` | | The picked opponent's texts in node 12 (6 with string 478, 4, 7, 8) and 13, node 17's textures from its item, and node 12's children 14 and 15 (`UiMovie::FindNode`, `UiMovie::Child`) switched. 312 lines. Callers `Transition9`, `Transition11`. Not read in detail. |
| `sub_8222A978` | `LocationScreen::ShowUnlockEntry` | | The unlock list's entry at +868 (`DataTables::Entry56`): texts (4, 1), (4, 2), pictures (`PictureA`, `PictureB`, node 6), node (4, 0) by `sub_8211D8D8`. Only caller `Transition25`. Not read in detail. |
| `sub_8222BBD0` | `LocationScreen::ShowCard` | | Movie 30: the picked opponent's card: nodes 2 and 3, texts (3, 55 to 58; strings 402, 401, 407, 408) and (-1, 12), node 6 by `LocationScreen::OpponentHasCard`, two named animations played and looped. Only caller `Transition17`. Not read in detail. |
| `sub_8222CEB0` | `LocationScreen::Transition0` | | Page transition 0 (`LocationScreen::Update`'s table; r3 = the step word, 0 on the first call, which starts the animations and sets it to 1; returns nonzero when done): `SetCursor(80)`, `Play(-1, 0)`, then the location record's text (string 575) scrolling (`sub_8211C4F0` with 300, `ScrollText`), `SetCarColor`, `HidePictures`. Not read in detail. Not seen in play. |
| `sub_8222AD50` | `LocationScreen::Transition1` | Overridden | Transition 1 (the race's and the leaving pages' fade): `ScreenFades::FadeOut([0x82001BF0])` on the first call; movie 21 run; done when `0x826A50DC` or `0x826A50E4` is set (the fade's words). Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`; `UiMovie::Run` through its override from a frame like the original's, with its return address); checked by `--run_harnesses=location_screen` (`Run`'s update stood in). |
| `sub_8222D610` | `LocationScreen::Transition2` | | Transition 2 (page 0's cursor moved): named animations 24 and 23 finished, `SetCursor(160)`, 23, 37 and 38 played, a text by the cursor (string 588 among them), the text's scroll reset (`sub_8211C5F8`). Not read in detail. |
| `sub_8222D4F8` | `LocationScreen::Transition3` | | Transition 3 (page 0's back): animations 24 and 23 finished, `SetCursor(160)`, an animation played and looped, a text and its scroll reset. Not read in detail. Not seen in play. |
| `sub_8222D828` | `LocationScreen::Transition4` | | Transition 4 (page 0 to 1): movie 21 reset (`ResetPose`, `FinishAll`), `ShowPictures`, `Play(-1, 1)`, animation 103 played and looped, `ShowListEntry`, text 578, the camera animation evaluated (`CameraAnims::Evaluate`, a move). Not read in detail. |
| `sub_8222DA10` | `LocationScreen::Transition5` | | Transition 5 (page 1 to 0): `ResetMovie`, `HidePictures`, text 573, `Play(-1, 0)`, the camera animation back, `ListNode`, the menu `[0x82814E54]` cleared (+0 to +64), animation 103 finished, `SetCarColor`. Not read in detail. |
| `sub_8222DCE8` | `LocationScreen::Transition6` | | Transition 6 (page 1's cursor moved): animations 62 and 63, `ListNode`, the entry's pictures (`PictureA`, `PictureB`) and animation 9. Not read in detail. Not seen in play. |
| `sub_8222AE00` | `LocationScreen::Transition7` | | Transition 7 (page 1 to 2): the location record's entry at the cursor (`DataTables::Entry56`); the save's +6076 = 0 and `ItemFlags::Set(save, 0, id)` (probably marks it seen), `KindText`, `Play(-1, 8)`, texts (4, 1) and (4, 2), animation 5, `HidePictures`. Not read in detail. |
| `sub_8222DC08` | `LocationScreen::Transition8` | | Transition 8 (page 2 to 1): node (-1, 11) reset, `ShowListEntry`, `Play(-1, 10)`, an animation looped, `ShowPictures`. Not read in detail. |
| `sub_8222DEE0` | `LocationScreen::Transition9` | | Transition 9 (to the opponents, page 3, and page 4's steps): `ResetMovie`, `HidePictures`, nodes 17 and 18 hidden, `Play(-1, 2)`, `ShowOpponentInfo`, the camera animation, `ShowOpponentSpecs`, `ShowOpponentPictures`, then `ShowArrows`, `Play(-1, 13)` looped, `SetCarColor`. 183 lines. Not read in detail. |
| `sub_8222E1A0` | `LocationScreen::Transition10` | | Transition 10 (page 3 to 0): `ResetMovie`, `HidePictures`, nodes 17, 18 hidden, `Finish(-1, 13)`, text 574, `SetCursor(40)`, the camera animation back, `Play(-1, 0)`, `SetCarColor`. Not read in detail. Not seen in play. |
| `sub_8222E368` | `LocationScreen::Transition11` | | Transition 11 (page 3's cursor moved): `CameraAnims::SetGlobals`, `ShowArrows`, `ShowOpponentInfo`, `ShowOpponentSpecs`, an animation, `sub_82150B80` and `CameraAnims::Evaluate` (the camera to the next car). Not read in detail. Not seen in play. |
| `sub_8222B8A8` | `LocationScreen::Transition12` | | Transition 12 (page 3 to 4): `Finish(-1, 13)`, nodes 17, 18 hidden, `Play(-1, 9)`, the opponent's first text (by `Items::Available`, `DataTables::Find20`, `Entry20`, node (5, 1)) and pictures, `HidePictures`. 218 lines. Not read in detail. |
| `sub_8222E5E8` | `LocationScreen::Transition13` | | Transitions 13 and 15: animation 97, `Play(-1, 15)`, then `Play(-1, 13)` looped and `ShowArrows`. Not read in detail. Not seen in play. |
| `sub_8222E6E0` | `LocationScreen::Transition14` | | Transition 14 (to the confirm menu, page 5): the menu `[0x82814E5C]` reset (3 items, wrapping), node 9's text, `SetCursor(0)`, text 579, node (9, 6)'s color 2 when `OpponentHasCard`, `Play(-1, 11)`, `ShowPictures`, animation 13 looped. Not read in detail. |
| `sub_8222E8D8` | `LocationScreen::Transition16` | | Transition 16 (page 5's cursor moved): animations 23 and 24, `SetCursor(80)`, node (9, 6)'s color by `OpponentHasCard`, a text and its scroll reset. Not read in detail. |
| `sub_8222EAD0` | `LocationScreen::Transition17` | | Transition 17 (page 5 to 6): `ShowCard`, animation 52 on movie 21 and one on movie 30, both run. Not read in detail. |
| `sub_8222BE60` | `LocationScreen::Transition18` | Overridden | Transition 18 (page 6 to 5): animation 53 on movie 30, both movies run until done. Read in full. Native under `--native_location_screen` (`src/location_screen/movies.cpp`; `UiMovie::Run` through its override from a frame like the original's, with its return address); checked by `--run_harnesses=location_screen` (`Run`'s update stood in). |
| `sub_8222BF18` | `LocationScreen::Transition19` | | Transitions 19 and 20: movie 21 run until done. Read in full. Not seen in play. |
| `sub_8222A470` | `LocationScreen::Transition23` | | Transition 23 (to page 12, after a race): +820 = 1; the human racer's item's result text (`DataTables::Find20(item, 11)`, `Entry20`), texts (4, 2), (4, 1), node (4, 0) hidden, pictures (`PictureA`, `PictureB`), animation 5 looped, `ScreenFades::FadeIn(60)`, animation 95, `HidePictures`. 180 lines. Not read in detail. |
| `sub_8222D2E8` | `LocationScreen::Transition24` | | Transitions 24 and 26: `ScreenFades::FadeOut(60)`, animation 98, `ResetMovie`, `SetCarColor`. Not read in detail. |
| `sub_8222D1A8` | `LocationScreen::Transition25` | | Transition 25 (to page 13): `ShowUnlockEntry`, animation 5 looped, `FadeIn(60)`, animation 95, `HidePictures`; at the end the page 13 set directly (+12, +16 = -1). Not read in detail. |
| `sub_8222AC78` | `LocationScreen::Transition27` | | Transition 27: animation 6, the picked opponent's text (`Find20(item, 9)`, `Entry20`, node (5, 1)). Not read in detail. Not seen in play. |
| `sub_8222D458` | `LocationScreen::Transition28` | | Transition 28: `ResetMovie`, `ShowOpponentPictures`, `Play(-1, 11)`, `SetCarColor`. Not read in detail. Not seen in play. |
| `sub_8222CFF0` | `LocationScreen::Transition29` | | Transition 29 (page 13 to 0): `FadeIn(30)`, `Play(-1, 0)`, `HidePictures`, text 573 scrolling (`sub_8211C4F0` with 300, `ScrollText`), `SetCarColor`. Not read in detail. |
| `sub_8222AAB0` | `LocationScreen::Transition30` | | Transition 30: as `Transition25` from an Entry56 (texts (4, 1), (4, 2), pictures, animations 5 and 95, `HidePictures`). Not read in detail. Not seen in play. |
| `sub_8222D3B8` | `LocationScreen::Transition31` | | Transition 31: animation 98, `ResetMovie`, `SetCarColor`. Not read in detail. Not seen in play. |
| `sub_8222A218` | `LocationScreen::Transition32` | | Transition 32: as `Transition23` from the human racer only. Not read in detail. Not seen in play. |
| `sub_8222A718` | `LocationScreen::Transition33` | | Transition 33: as `Transition23` without the fade. Not read in detail. |
| `sub_8222D108` | `LocationScreen::Transition34` | | Transition 34: animation 98, `ResetMovie`. Read in full. |
| `sub_8222BF80` | `LocationScreen::Transition35` | | Transition 35: movie 21 run until done. Read in full. Not seen in play. |
