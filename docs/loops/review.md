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

## Run 2026-10-30 23:03 (`b7-loop-30-10-26-2`)

- The material passes' map (2da185e): `functions/material-passes.md`,
  23 new rows (22 seen in play): `PartModel::SetPaint`, `SetPaintBody`
  (three HSV colors in r4-r9, through `Color::HsvToRgb`; the byte
  conversion `Color::HsvToRgb8`'s results overwritten but for a
  negative hue, kept as is), `ApplyPalette` over the palette at
  `0x826C5110` (`known-data.md`; for pairs the third color from entry j,
  not 11+j, probably a slip in the original), `PaintPalette::Init`
  `sub_820D2330` (taken in from the scene's lead), the small passes,
  `Projector::Matrices` with `Matrix::LookAt` (what it's for not known),
  four table lookups; `scene-loaders.md`'s seven walks now name the
  passes. Two leads added.
- `car-models.md` (already mapped; the map checked by the lead, not
  remapped). Migration 1's rows (9f71b49..233c175): `LoadSlot::LoadShared`
  and `Load`, `LoadSetA` and `LoadSetB` (now Native; return `int64_t`,
  as `Archive::Load`'s serial is sign-extended), `TeamEmblem`,
  `LoadDecals`, `LoadExterior`, `SetVinylTextures`, `Wheels::SetUp` with
  the rims and brakes (Native), `CarInterior::LoadSetCBody` and
  `LoadParts`. For review: `GetLastError` (`sub_82296EC0` Native,
  `sub_82295F18`, `functions.md`) ported as `last_error::Get` with no
  flag, as B6's SetLastError; `LoadSetCBody`'s sprintf (only when the
  node file is empty) follows the parking area's varargs call (r6-r10
  spilled as 0); `LoadSetA` and the rims not seen in the attract race
  (harness only); no frame dump (`LoadExterior` only makes the sheet and
  queues loads: the sheet's bytes compared in the harness). The
  validation counted `sub_82295F18` (an adapter with no plain C++ of its
  own, calling `last_error::Get`) through `sub_82296EC0`'s print.
  Migration 2's rows (27d7e64..9260d11): `CarInterior::Load` (its
  accessories' two vtable calls through the guest), `FreePagesAt`
  `sub_820C3CE0` (new row), `CarModel::MakePlate` `sub_820ED6D0` (the
  name a guess: a 300 x 150 texture from five images) with
  `sub_820C4DD8` and `sub_820ED630` (new rows), the file-loaded
  callbacks `sub_820D47A8` and `sub_820D4F80`, `ReloadDecals`
  `sub_820D5D20`, `Skeleton::LocalMatrices` `sub_82599C58`
  (`models.md`); 5 adapters removed. For review: the +3724 = 1 image
  branch (`sub_820C4E68`, no row) runs in an attract race though the
  coverage list lacks it: the callbacks and `ReloadDecals` hand it back
  to their originals, `Vinyl::DrawDecal`'s adapter stays, and it is a
  lead (dd86385's message still says "never seen in play", corrected in
  9260d11's rows); `LoadExterior`'s and `ReloadDecals`' cases no longer
  compare `Texture::Create`'s frame words (the callees' originals save
  registers there; `TextureCreates` still compares it, as
  `Vinyl::Compress`); r3 not compared for the callbacks and
  `LocalMatrices` (no caller reads it, as `ResetPose`); no frame dump
  for the plate (both textures' bytes compared in the harness).

## Run 2026-10-30 11:39 (`b7-loop-30-10-26`)

- The race event's map (84e5334): `functions/race-event.md`, 102 new
  rows (37 seen in play, 65 not), among them `EventCondition::Met`
  (`sub_82142820`, a 68-kind jump table) and its 61 kind tests (only
  kinds 21 and 31 ran in play), its case 1 split off by the recompiler
  (`sub_82142868`); `known-data.md`'s row for `0x826C6078` rewritten as
  the event's layout, with rows for the event group, the pools and the
  20-byte condition; stale "not mapped" notes in `career.md` and
  `ai-drivers.md` pointed to it; the lead for `sub_821442D8`,
  `sub_821434A0`, `sub_82140308` folded in.
- The race event's migration (6cf1882, `--native_race_event`): adapters
  only on the six entry points recompiled code calls; `Make`, `Init`,
  `Destroy` native only when the native heap holds the event's pages.
  Choices for review: `EventCondition::Power` (kind 40, `sub_82141668`)
  left recompiled and called from native `Met` through a guest call run
  at the original's stack depth (it reads stack words below it it never
  writes; native code reaching `generated/`, the user's call of
  2026-10-25 to look at later); the kind tests' stores to their own
  frames left out; r3 not compared for the functions returning nothing
  (as the race world's harnesses); the 58 kind tests never seen in play
  ported; `Met`'s row corrected (kind 64 tests bit 0x20, not 0x10).
  `career.md`: `Career::EventQuery`, `EventLocation` Overridden,
  `DayBlock::BestStreak` Native.
- The cleanup after it (3f8fa4b..8267f2d, `--native_career`):
  `Career::SetUpEvent` native over its 121,200-byte frame kept in guest
  memory where the original has it, calling `race_event::Make`'s plain
  C++ (the original `Make` from the frame where native refuses);
  falls back to the original where `Condition::Test`'s or
  `RaceData::ResetGlobal`'s copies wouldn't be native from that frame;
  its harness cases in the race_event harness. `Garage::Add` and
  `sub_821104D8` (new row, `Car::SetModelAndClass`, probably): reads a
  stack word the original never writes when spec byte +708 > 7, kept
  as the original. Adapters removed (Native): `Entry56::Allows`,
  `Condition::Test`, `RaceEvent::Make`, `Car::SpecWord744`,
  `Garage::InsertCar`. Harness notes: the career harness puts rand's
  seed back after each case; case seeds of a shared generator shifted
  repeat (about half of the first 48 cases duplicates), so
  SetUpEvent's group mixes its own per case (other harnesses probably
  have the same, not changed); table 0's walk can read past the table
  as the original does, so the harness keeps done bits inside it.
- The camera modes' map (9ac322c): `functions/camera-modes.md`, 100
  rows (2 moved from `functions.md`: `sub_82152CD8`, `sub_821594B8`;
  98 new; 90 seen in play): the race mode's camera director at +10512
  (`CameraDirector::InitStates` `sub_82159950`, `SetMode`, `Update`),
  its six views' camera objects (names "probably": chase, two in-car,
  the start's look at the racers, the TV camera of replays and attract
  races with its path files and the attract filter's `rand() % 4`, the
  scripted cutscene `sub_82156CC0` with the rival logo), the camera
  animations table `0x826C79B8`, two front-end cameras; vtables read
  from the image. `known-data.md` rows added; pointers in `cameras.md`,
  `game-modes.md`, `track-zones.md` and two leads updated. Large
  functions skimmed, small ones read in full.
- The camera modes' migration 1 (5a1babc..4c274ed,
  `--native_camera_modes`): map corrections (the animations' count at
  `0x826D81B8`, not `0x826D01B8`; `ToView4` sets +9636 = 1 and +77408 =
  0; `Viewport::Offset` moves only the target; `Duration` reads the
  first block's +16; the intro camera's flag word is race data
  +0x59AE4). Choices: `ViewCamera1/2::Update` leave out the original's
  unused `NearestHumanVehicle` body read; `CameraPath::Evaluate`'s cases
  avoid mid-curve keys of kind 3+, where `Curve::Evaluate` returns a
  stack word the original's `SplinePoint` may have left (not taken in
  1,006 evaluations in a race; `Curve::Evaluate`'s own harness covers
  it); `RivalLogo::Play` is `PlayNamed`'s first native caller (runs the
  original under `--log_pause_movie`). 10 adapters removed; 25
  functions native with no adapter from the start.
- The user's call (2026-10-30): all harnesses together outgrew
  `attract_race.sh 40` (the camera modes' validation, `itc_2730`, cut
  off in `camera_modes`); the validation agent now runs
  `attract_race.sh 60 --run_harnesses=all`, migration agents stay at
  40 (`b7-loop.md`, "Precedent first"; `CLAUDE.md`).
- The camera modes' migration 2 (96a281f..27ada48): `UiMovies::Open`
  and `Delete` with `UiMovie::Unload` taken into `ui-movies-game.md`'s
  code (`--native_ui_movies_game`; callees native where their own
  checks say so, else their originals from frames like the
  originals'). Row corrections: the attract filter's `rand() % 4` runs
  on every TV camera update while +43520 is 0 (probably once a camera
  path), not once; the TV camera's range table holds at most 40, from
  `0x82618D30`; `RivalLogo::Open`'s record +8 id, +12 name, +16 asset;
  `Cutscene::Update` sets bit 0 of race data +0x59AE4, not +0x5E6E4.
  Native `Cutscene::Step` draws the logo through `sub_82211A28`'s
  override, so `--redraw_rival_logo` is unchanged; a harness-only
  stand-in (`race_hud_hooks::g_rival_logo_draw_stand_in`, null outside
  harness runs) is checked in that override. Batch 3's four adapter
  removals were checked by the absence of a game line instead of a
  temporary counter (their functions don't run in attract races);
  batch 4's had the counter (`itc_2746`).
- The camera modes' migration 3 (81d51e9..d71eff5):
  `CameraDirector::InitStates` replays its 34 transitions' stores and
  copies in the original's frame from a table extracted from the
  generated code (as `RaceWorld::Init`); `Cutscene::Cutscene`'s 30
  scripts as tables read from the generated code by a script
  (`src/camera_modes/cutscene_scripts.cpp`), falling back to the
  original when `ChaseReady` refuses or the stack isn't aligned; the
  constructor can't choose scripts 3-5, and script 5's body is never
  checked by a harness. `SetMode` calls the six view constructors
  through their adapters' steps from the original's frame; its harness
  answers them with a stand-in (`hooks::SetCameraConstructStandIn`),
  so those six adapters stay though only `SetMode` calls them.
  `CameraDirector::Update` stays Wrapped: `--redraw_rival_logo`'s
  wrapper runs the native form on logic passes
  (`hooks::CameraModesUpdate`), noted in `frame-interpolation.md`; its
  harness keeps view modes 0-5 (past 5 the original calls a null
  camera). `Resume` falls back if the color filter's slots aren't the
  native ones. `Viewport::LookAt`'s adapter removed (`cameras.md`); the
  `GarageCamera::Update` harness then compares the viewport as floats
  but the view mode (+1860), as the cameras harness does (24 cases had
  differed by NaN sign only).
- The location screen's map (3bf92d8): `functions/location-screen.md`,
  99 new rows (77 seen in play): the lead's "front-end menus" are one
  screen, the parking area (the user's correction, 2026-10-30: the
  player loads into it, picks one of up to 6 opponent cars and races,
  the other way to start a race besides the highway; the file's
  intro updated by the migration agent) (save data +6108, locations
  14-19): the location's
  scenery through camera animations, its opponents posed as cars, UI
  movies 21 and 30, XPR pictures; its state machine at +9640 of a
  28,940-byte block at a front-end mode's +848 (which mode type isn't
  known), 19 transitions over 11 states; a page object at `[0x82814E48]`
  with 16 pages and 36 transitions (pages 9 and 11 call `Race::SetUp`).
  `known-data.md` rows added (the page object, four menus, the
  912-byte screen at `0x82814E50`, three save-data fields). Folded in:
  the course scenery's front-end owners and `sub_822280D8`,
  `sub_82228138` (their lead lines kept with a note); 3 leads added.
- The parking area's migration 1 (f0ee4da..a4cfca2,
  `--native_location_screen`): the file retitled "the parking area"
  with the user's description (574e514; the file name, flag, `src/`
  directory and `LocationScreen::` names kept). `UiMovie::SetText`
  taken under `--native_ui_movies_game`, with `hooks::FormatGuestCalls`
  added for `Strings::FormatV`'s plain C++; its harness uses its own
  string table (on the game's, one case never returned from
  `Strings::Get` for string 0x502 on both sides, not looked into).
  `UpdatePage`'s harness covers pages -1, 7 and 10 only (the others run
  the menus' input). `InitStates` replays the original's frame stores
  from a step table generated from the translated code. Row
  corrections: `ShowArrows` hides nodes 17 and 18 with one opponent;
  `ListNode` sets the node's +16 to r3 as a float. The replay doesn't
  reach the parking area: no frame dump. Slip: two temporary logging
  edits reverted with `git checkout` rather than committed first (logs
  only, nothing committed lost). The user's call: the 22 `SetText`
  callers pass 0 for the r7-r10 they don't set.
- The parking area's migration 2 (1af8d5b..df29d57): a
  `location_screen::Calls` interface carries the guest calls left (the
  heap's, memcpy's and the archive's adapters where their plain C++
  refuses; `UiMovie::Finish`, `FinishNamed`, `FinishAll`, `Run`
  through their overrides; `Career::EventQuery` and `Race::SetUp`
  through their register steps; the race event's and `FormatV`'s).
  The `SetText` callers run natively only while the UI movies' text
  setters do (`hooks::LocationScreenTextsNative`), else the originals;
  a setter refusing after that check is a logged abort. Outside the
  subsystem: `hooks::StartUpRecorded` checks added at the start of
  `hooks::RaceSetupSetUp` and `hooks::CareerEventQuery`, for the
  harness to record those calls. The harness caught a porting bug,
  fixed (`Transition17` writes the confirm menu `0x82814E5C`); row
  correction: `UpdateView` reads the opponent menu's cursor. The 7
  adapter removals' counter (`itc_2794`) ran in an attract race,
  where the screen doesn't run at all.
- The parking area's migration 3 (38cc3dc..74c7bf3): `PoseCar` calls
  the cars' and vehicle's plain C++, its multiplies noted for
  interpolation with the original call addresses, the original run if
  the frame isn't 16-byte aligned; its harness inputs are made up (the
  game's Params and items aren't loaded when harnesses run), shaped
  like the SetupFromCar and Race::SetUp cases, and its cases force
  flush-to-zero off at the start (an earlier case's native adapter had
  left it on), as car_physics does. `UiMovie::SetNodeText` taken under
  `--native_ui_movies_game` (Wrapped -> Overridden), run from its
  logging override in `pause_movie_log.cpp` when `--log_pause_movie`
  is off; its unset varargs passed as 0 (the user's call on
  `SetText`). `Transition8`'s cases keep sub-scenes' child ranges
  inside the root's (random trees fault in the original too).

## Run 2026-10-30 01:06 (`b7-loop-29-10-26-2`)

- Car sounds, cleanup migration (d5cfd5c, `car-sounds.md`, the last
  run's stop): `CarSound::Part840Update`'s 1 of 80 was the game's mixer
  swapping XAudio's two source-voice stage lists (engine +92 and +104,
  `[engine+120]` the next one) between a case's recompiled and native
  versions, so the made voice joined a different list; shown by logging
  `[engine+120]` and by forcing the swap (42 of 80 differed without the
  fix). For review: the real-voice cases now write those two list
  pointers as tokens (the call's next list, the other one) instead of
  addresses, following the XAudio harnesses' `LockWords` (fa8dcc1). No
  code change; the row's note says so.
- The app's start-up, map (d5647db, `app-start-up.md`, 30 rows;
  `sub_822A8980` moved from `functions.md`; `known-data.md`: the app's
  interface at `0x8266D088`, `0x826BABA0` and `0x8266D084`, before
  "probably the scene's render manager", the display's tiling block,
  the billboards' vertex declaration). For review: `Voice::Init`'s row
  in `system-services.md` says "per remote talker (none)", but 8 remote
  talkers are set up at boot (left as it was, noted in `sub_8257F388`'s
  row); `ActionMappers::Init` is called with r3 = app + 16, which
  doesn't match its row ("the input manager"), not checked. The call
  asked: the voice engine (XHV, `Voice::Init` `sub_8257B7A8` under the network manager's `sub_8214F6E8`): not made; its users seen in play (`sub_8214FA78` every frame, the UI's calls through the engine's +52, the release `sub_8214F690`) skipped natively with what they'd read with no headset (no talkers, no data), Status Dropped (the user's call, 2026-10-30, "Map Xbox features, don't port").
- The app's start-up, migration 1 (c9aed90..6292712): the voice engine
  not made (`sub_8214F6E8` Dropped, keeping its two words +456 and
  +444; the engine pointer `0x826C74A0` stays 0; the voice thread's
  override removed). For review: no reader of the engine needed a skip
  of its own (`sub_8214FA78` and `sub_8214F810` already out by B5's
  `Network::Update`; the UI's +52 headset checks `sub_8226BCB8`,
  `sub_8226EEC8` run only when `sub_82260908` answers 1, a session
  state only system link and Live set; the release checks for null);
  no harness for the Dropped set-up (the original would make an engine
  with a thread). The `notify` harness now empties its listener's
  queue before its cases (the SDK's start-up notifications go to the
  first system listener, which was voice's). `sub_82459C10` Merged.
- The app's start-up, migration 2 (427db89..a63717c; rows also in
  `strings-and-font.md`, `movies.md`, `sound-cues.md`, `xaudio.md`).
  For review: `App::StartUp`'s harness adds a recorder hook,
  `hooks::g_start_up_recorder` (null outside the harness), asked first
  by the twelve callees' adapters it reaches (in `archive_streaming.cpp`,
  `action_mappers.cpp`, `cameras.cpp`, `mode_tick.cpp`,
  `car_sounds.cpp`, `save_interface.cpp`, `save_data.cpp`,
  `archive.cpp`; the `save_interface` and `save_data` adapters written
  out instead of by their macros); the font's vertex shader compile
  given r9 = 0, as the original leaves it after `CreateVertexDeclaration`'s
  memcpy in play (an r9 of 3 made the stored-shader override compile
  live); `Text::Init`, `SceneTarget::Create`, `Billboards::Init` and
  `Network::SetUp` keep their adapters (reached through guest calls).
  The stop on `App::App`'s movie wrapper lock answered by the host
  mutex precedent (the lead's reading, not asked).
- The app's start-up, migration 3 (6e8e860..e4104fe; rows also in
  `movies.md`, `video-output.md`, `sound-cues.md`, `cameras.md`,
  `framework.md`). For review: the movie wrapper's critical section
  (+4, users only the wrapper's slots 0 and 2-4 through `ScopedLock`;
  none runs with the FFmpeg movies) is a host `std::recursive_mutex` by
  its address, as the memory blocks' lock (`memory_blocks.h`), its 28
  bytes left as operator new leaves them; `AudioManager::AudioManager`'s
  +1604 copies a frame word the original never writes (frames placed as
  the original's); the framework harness's `DispatchObjects` keeps its
  random words above the allocation layer's range (`App::Run` case 171
  hit a heap block once the boot allocations moved); seven adapters
  `--removable` lists kept so the start-up recorder sees their calls
  (as `Text::Init`'s); the camera class and sound manager cases compare
  heap bytes nothing writes (they match as both versions get the same
  block); the getter `sub_82379350` runs only in its harness; two
  leads deleted (the second camera class, taken; `sub_82593D78` and
  `sub_82593D88`, the wrapper's slots).
- The data load at boot, map (562dc47, `data-load.md`, 25 rows, 6 not
  seen in play; `known-data.md`). For review: `Defaults::Init`
  `sub_820CCDC0` fills the save defaults' bit sets that `save-data.md`'s
  `Defaults::Table*` rows said had no known filler (`0x826C3044`,
  `0x826C3080`, `0x826C3C24`, `0x826C50A8`, `0x826C50E4`,
  `0x826C4064`); `[0x826C5B4C]`'s block is 0x2225C0 bytes;
  `[0x826C5B50]` probably the replay kept after a race (static copies at
  `0x826D8520`, `0x82816E50`); `sub_82291328` loads a second set of
  three UI movies whose users never ran in play. One lead added (the
  front end's state machine `sub_822213B0`, `sub_82220C88`).
- The data load at boot, migration 1 (b810489..f4f201f; rows also in
  `data-tables.md`, `car-specs.md`, `models.md`, `strings-and-font.md`,
  `files-and-saves.md`, `ui-movies-game.md`). For review: native loads
  go through a new `archive::LoadCall`, and `hooks::NativeArchiveLoad`
  hands each load to a harness's recorder when one is set (a harness
  had queued a real `Strings::Load` that later wrote into freed
  memory); `hooks::StartUpRecorded` now asked first by the adapters of
  `Archive::Load`, `SaveData::Create`, `RaceData::ResetGlobal`,
  `Params::Init` and the new ones; 12 adapters `--removable` lists kept
  (the native callers go through them, as `App::StartUp`'s); kept as
  the original: `Defaults::Init`'s r3 sign-extended,
  `Shaders::LoadAll`'s r3 = `BootData::Load`'s frame + 80, failed
  allocations' stores near 0.
- The career, map (d91b1bb, `career.md`, 76 rows, 35 not seen in
  play; `known-data.md`: the item record and stats record rows). For
  review: the race event (`82140340`-`82144E58`) kept as its own lead
  rather than mapped here (it would take the file to ~170 rows, and
  its `RaceEvent::` rows live in `race-setup.md` and `race-world.md`);
  two jump-table cases (`sub_821129E0`, `sub_82114C7C`) to be Merged
  into their callers; the 3,008-byte stack copies of an item kept as
  the original leaves them. Three leads added (the race's end on the
  career's side `sub_82150E00`; front-end screens over the career; a
  probable menu sound `sub_821103F0`).
- The career, migration 1 (fd83bca..a3d7ec3). For review: no career
  function runs in an attract race (front end and the career race's
  end), so no group was counted in a race; `Career::PickNext`
  `sub_821159B0` native only up to its gate (15 races and day 3, the
  path seen in play), the original past it; the stack copies (the
  3,008-byte items, the running means, the conversions) written where
  the originals leave them, the last of each loop only; the harness
  skips the frame words the originals' later callees overwrite with
  back chains and saved registers (r1 - 272 and r1 - 256 in
  `AfterRace`, `CloseDay`'s +80 pair under `NextDay`), as native code
  saves no registers on the guest stack, with no "(expected to
  differ)" group; `Stats::CloseDay`'s division by 0 gives 0 as the
  translated code's trap does, with a warning logged; the outcome-3
  group (not seen in play) ported for `Stats::AddRaceResult`; the
  career files build with the strict float flags. Two mutation tests
  (reverted) showed the harness catches changes.

## Run 2026-10-29 22:22 (`b7-loop-29-10-26-2`)

- XAudio's objects, cleanup migration 1 (3ea5812..01a849c,
  `xaudio-objects.md`): the XMA codec's destructors given harness cases
  of their own (the last run's stop); `XAudioMatrix::SetLevels`
  `sub_82466188` changed to copy its levels as words (a signaling NaN
  kept bit for bit, as the recompiled lfs/stfs pair and `SetMatrix` do;
  a real difference the new cases' inputs exposed); the resampler and
  its kernels native (its constants addressed from `0x82050000`,
  `../native-audio.md`); the resampler's and stages' harness cases sync
  the host's flush-to-zero into the copied context; the copy kernels'
  prefetch and cache-line bookkeeping left out (no memory effect). For
  review: 10 adapters removed from `b7_groups.py --removable`'s list
  include vtable slots (`RetirePacket` slot +92, `SubmitPacket` and
  others), uncalled in an attract race by a counter; a recompiled path
  through the vtable (menus) would run the original.
- XAudio's objects, cleanup migration 2 (187ea5d..ea486dc): the reverb
  core (`src/xaudio/reverb_core.cpp`; the float helpers shared in
  `src/xaudio/ppc_float.h`), `Reverb::SetParameters` and
  `PcmCodec::Process` taking the voice-list spin lock inline (the
  2026-10-29 call), `PcmCodec::Process` calling the native targets of
  its vtable calls directly (others and the loop-end callback through
  guest calls), `I3dl2::ToNative` over `c_runtime::Log10`. For review:
  the native core doesn't write its 296-byte register-save area or the
  `stwu` back-chain word below the stack pointer, and the native math
  doesn't write `log`'s scratch words (as with `FilterCoefficient`'s
  `pow` and `cos`); the harness doesn't compare them.
- XAudio's objects, cleanup migration 3 (1328597..0522cc6; 110 of 110
  ours): `ReverbCore::Apply` and `Init` (`src/xaudio/reverb_setup.cpp`,
  converted from `generated/` by a throwaway script the agent checked,
  not committed), the reverb's construction and pass, `ReverbCore::SetGain`
  Merged; `ReverbCore::Apply`'s overlapping memcpy left to the original
  through a guest call (as `Reverb::SetParameters`). Rows corrected:
  `I3dl2::SetParameter` (past index 12 nothing is stored, but the native
  parameters are still made and applied), `ReverbCore::Init` (runs on
  the reverb's +208 or an I3DL2 side's +52). For review: one run's
  `XAudioMatrix::MixRamp` case differed only on the live spin lock's
  depth while the game's audio thread held it (not seen again); this
  file's harness now compares the depth only when its thread owns the
  lock (fa8dcc1); `xaudio_engine.cpp` and `xaudio.cpp`'s harnesses read
  it the same way and weren't changed. The pass harness no longer
  compares `ReverbCore::Apply`'s 296-byte register-save area (shown
  through by the recompiled callers once the adapters went; 0522cc6).

## Run 2026-10-29 15:44 (`b7-loop-29-10-26`)

- XAudio's factory objects (map a611121): `docs/functions/xaudio-objects.md`,
  121 rows (all new; 12 not seen in play), with the XAPO lead: the
  class tables (XAudio's default at `0x8204A7B0`, ids 0-3; the game's 7
  at `0x820713E0`: the source codec (PCM for format types 0-3, XMA for
  4), the output matrix, the render driver, the reverb (probably
  I3DL2, 309,936 bytes), the volume meter, two effects never seen), the
  codecs' packet queue, the PCM and XMA codec slots, the resampler and
  its 6 kernels that ran (the other 94 get no rows), the matrix's set
  and mix, the reverb (its signal code in outline), the volume meter and
  the shared Release `sub_8246CE70`. The class table's entries are size
  at +4 and create at +8 (the "first" and "second" function in
  `xaudio-engine.md`'s factory rows). XMA's hardware-facing functions
  keep their rows in `audio.md` (shared); none of the new file's XMA
  rows touch the XMA object or registers. Kernel use: the voice lists'
  spin lock only (the 2026-10-29 call); the reverb times its passes with
  QueryPerformanceCounter (ours). `known-data.md`: 10 layouts.
- XAudio's objects' migration 1 (32c197d..4c32e78, `--native_xaudio_objects`):
  78 of 109 in play ours (20 Native from the start: only native callers
  reach them), with `XAudioRegisterProcessingCallback` `sub_8245A5E8` and
  `XAudio::Commit` `sub_8245A6E8` in `xaudio.md` (94 of 96; its last two,
  `XAudioShutDown` and the static initializer `sub_825DA0F0`, left
  recompiled: they release or initialize the live engine and API lock
  and can't be harnessed). Corrections to the map: `OnSubmit`
  `sub_82469AF8` releases one lock level per failed check;
  `SubmitPacket` `sub_82469EF8` takes the first free entry; bit 5 of
  `RetirePacket`'s flags probably "a free entry available". A fix
  outside the subsystem: `XAudioBuffer::Clear` and `ClearThunk`'s
  adapters now sign-extend their error r3 as the original's
  `lis`/`ori`. Matching notes (in `native-audio.md`): the translated
  code's `lfs`/`stfs` pairs fold to a bit copy (signaling NaNs kept);
  its `vmaddfp` is a rounded product then a sum, flush to zero on.
  Adapters kept though `--removable` listed them: six vtable slots,
  and `sub_824656B8` (an indirect caller not ruled out). Seen with the
  xaudio or xaudio_engine harnesses: 4-7 silence fills in the first
  10 s, also with `--native_xaudio_objects=false` (the harnesses holding
  the mixer's lock). Left: the resampler and the reverb's core (ready).
  Its validation added a print per address in the reverb's delay line
  fills (259d68e) and one on `XmaCodec::SubmitRegion`'s E_NOTIMPL return
  (e3708da), and stopped the run: the XMA codec's destructors
  `sub_8246A918` and `sub_8246AAD0` have no harness line (the
  `XAudioCodec::Create` XMA cases release through `__imp__sub_8246A758`).
- The user's calls (2026-10-29), asked by the lead: XAudio's own
  threads, events, semaphore and title terminate notification are used
  by native code through their imports exactly as the original does
  (host objects later, once every user is native); QueryPerformanceCounter
  (`sub_825BA0C8`) is answered from the host steady clock at the 50 MHz
  timebase, no flag, one clock for recompiled and native callers, as
  GetTickCount. Both now under "Precedent first" in `b7-loop.md`.
- QueryPerformanceCounter and what it unblocked (be9c0c5..b32d7d2, by the
  call above): `helpers::PerformanceCounter()` (`src/helpers/clock.cpp`,
  50 MHz host steady clock from process start, the original's second read
  when the low word is 0 kept; no flag), `XACTGetTimeMs` (`--native_xact`),
  the tracks' and events' play, schedule, fire, done and audio updates
  (`xact-tracks.md` 55 of 55), `XACTEngine::SendNotification` (row in
  `xact.md`, code in `src/xact/waves.cpp` beside `Notify`), `XACTWave::Notify`,
  `SetState`, `Release`, the destructors and `InMemoryWave::Stop`
  (`xact-waves.md` 28 of 30); 7 more adapters removed. Choices to look
  at: `helpers::SetClocksForHarness` gives the harness's two versions
  the same clock readings (thread-local); XACT's clock flag
  `[0x8266CDF0]` and frequency `[0x82971480]` are left as the game's
  (XAudio's thread reads them), so the trap paths aren't in the cases
  and go through the guest call; `XACTPlayWaveEvent::Fire` left r3 the
  time's low word zero-extended where the original sign-extends it
  (to be matched in the next migration, by the no-behavior-change
  precedent); paths never seen in play (the random spread, RemoveHelper,
  HasReleaseRpc, SwitchInstance, the RPC steps, the registration
  matcher) go through the guest call.
- XACT's waves' last two (f1e1ee9..dcdb43e): `XACTInMemoryWave::Prepare`
  `sub_825D3B10` and `XACTWave::AudioUpdate` `sub_825D2560` (with
  `XACTWave::Finish` `sub_825D1FD8`, not seen in play, ported inside it);
  `xact-waves.md` 31 of 31; 12 adapters removed (5 in `xact.md`, 1 in
  `xact-tracks.md`, 6 in `xact-waves.md`). `XACTPlayWaveEvent::Fire`'s r3
  now the full 64-bit time as the original leaves it (the harness
  compares r3 in full, with times past 2^31 ms). Choices to look at:
  `xaudio::LastCallR3()` (the last guest call's full r3, per thread) and
  `xaudio::Regs` widened to r3-r10; `GetStartPosition`'s adapter
  sign-extends r3 as the original's `li`/`lis`; `CreateVoice`
  `sub_825C31D8` now Wrapped by a harness-only stand-in (records r3-r10
  and hands back the harness's voice on the harness's thread, else the
  original; the stand-in rule in "Checking with a harness"); Prepare's
  reverb level copied as raw bits (the original keeps a signaling NaN
  through `lfs`/`stfs`); AudioUpdate's cases give a next wave only with
  an owner (the original reads the owner unchecked there) and skip the
  first 12 bytes of SendNotification's copy when none is sent (the
  original's saved registers from XAudio's GetState).
- The r3 fix and XAudio's engine's last 7 (320b782..12896c8):
  `SendNotification`, `Notify` and `SetState` leave r3 in full as the
  original does (a revert made 34, 11 and 43 cases differ; the old
  Notify/SetState cases had both sides on SendNotification's native
  adapter, now switched with the version). `XAudio::CreateEngine` with
  `XAudioEngine::Create` and `Init`, the mixer callback with `Mix`,
  `MasterThread` and `WorkerThread` native (`xaudio-engine.md` 47 of 47;
  9 more Native): the kernel's imports called at their thunks through
  the guest call with the original's registers (the user's call); the
  threads' entry points stay `0x8245BE08` and `0x8245BED0` (now the
  adapters); the mixer's lock `0x8264D4F8` entered through
  `RtlEnterCriticalSection` as the original (its only other user, the
  teardown `sub_8245B940`, never runs; our output thread doesn't take
  it). Harness approach to look at: while the output thread waits on the
  mixer's lock, each version runs over fresh kernel object headers the
  harness writes at the fixed addresses, then their states are read,
  handles closed and the live bytes put back; Init's cases use
  stand-ins on the harness's thread (`xaudio::SetMakingForHarness`), so
  `SetUpProcessors` and `MasteringCreate` keep their adapters as the
  stand-in point; thread creation isn't in the cases (checked at boot:
  the live engine's 372 bytes and the objects' headers matched the
  fallback's but for the handles and +300); Create's path where Init
  fails (deleting through the real teardown) has no case. Audio: 5-7
  silence fills in the first 10 s while the harness holds the mixer's
  lock, then none.
- XAudio's engine and mixer (map 610125a): `docs/functions/xaudio-engine.md`,
  70 rows (all new; 23 not seen in play: the teardown, the factory's
  id-128 classes, the sizing allocator, the stats callback, the callback
  unregistration). The mix runs on XAudio's own guest threads
  (`XAudio::MasterThread` `sub_8245BE08`, `XAudio::WorkerThread`
  `sub_8245BED0`, made suspended by `XAudioEngine::Init` with
  `ExCreateThread`, priority 15), woken through two synchronization
  events (`0x82954EF8` mix now, `0x82954EE8` mix done), a notification
  event (`0x82954EC4`, title terminating, waited on with mix done by
  `KeWaitForMultipleObjects`), a kernel semaphore (`0x82954ED4`, count
  0, limit 6) and a title terminate notification (`0x82954EB4`). None
  has a precedent (the archive's semaphores are the nearest for the
  semaphore): for the user's call before the groups using them are
  migrated. The voice lists' spin lock is taken inline as the original
  does (2026-10-29); the mixer's own lock `0x8264D4F8` is taken only by
  the callback and the teardown. `known-data.md`: the engine, stage,
  factory, mastering voice, render driver, allocator, kernel objects
  and stats copies.
- XAudio's engine's migration 1 (e9d3f21..d17ae9f, `--native_xaudio_engine`):
  40 of 47 in play ours (11 adapters removed): the leaves, the engine's
  and factory's making, the mastering voice, the stages, callbacks and
  commit. Choices to look at: where the original releases the voice
  lists' spin lock with depth, owner and IRQL kept in registers, ours
  reads the lock's words (the same values while it's held; the
  `RunStage` row); `KeGetCurrentProcessType` in `SetUpProcessors` is
  called through its import thunk, as the original does; the engine
  constructor's and `TakeStats`' harness cases run holding the mixer's
  lock `0x8264D4F8` (our output thread waits on it) and put the live
  `[0x82954F08]` back before leaving it. Left for the user's call (the
  kernel objects above): `XAudio::CreateEngine` `sub_82459D60` with
  `XAudioEngine::Create` and `Init`, the mixer callback `sub_8245FBE0`
  with `XAudioEngine::Mix` `sub_8245BF80`, `XAudio::MasterThread` and
  `WorkerThread`.
- XACT's tracks and events (map a3880fc): `docs/functions/xact-tracks.md`,
  143 rows (all new; 88 not seen in play): the tracks, track instances
  and events of `825D0000`-`825D9000`. The lead's "owner's forwarders"
  at `825D1CC8`-`825D1DD0` are the wave's methods (`XACTWave::SetVolume`,
  `SetPitch` and the rest, each one slot of the wave's voice at +44).
  In play only event type 1 (play wave) runs: every sound the game plays
  is simple, with no RPCs, so the RPC, transition and variation paths
  never ran. `sub_82483208` (the play-wave event's SetWave) lies in the
  movie library's range but only XACT reaches it, so it has a row here.
  Only `XACTTrackInstance::CancelEvents` (`sub_825D5520`) takes XACT's
  lock (the engine's +144, inline): taken as the original does (the
  user's call, 2026-10-29). `known-data.md`: the track, track instance,
  events, event record and two tables. Two leads added (the parameter
  events, none in play; a static matrix set-up `sub_825D8F88`).
- XACT's tracks' migration 1 (c8ad278..6fc2206, `--native_xact_tracks`):
  44 of 55 in play ours (13 adapters removed). Corrections to the map:
  `sub_825D0AA0` was called twice in an attract race, from
  `XACTComplexSound::Prepare`, so not every sound is simple (the intro
  says so now); `XACTEvent::Create` and the play-wave constructor pass
  the word at record + 12 for types 2 and 5, not its address. Harness
  coverage to look at: both `PrepareSimple`s are tested only on their
  failure paths (a successful Prepare needs a real wave) and weren't
  called in the attract races; the helper list is kept empty in the
  cancel and destructor cases, as in play. Paths never seen in play go
  through the guest call or decline to the original (the RPC entries,
  the transition event `sub_825D0B08`, `SetVariation` `sub_825D1478`,
  `sub_825CA6F0`, `sub_825C1548`, `Create`'s other event types), said in
  the rows. `tracks.cpp` is built with strict floating point, as
  `waves.cpp`. Left: `XACTTrackInstance::Prepare` `sub_825D5A48` (ready)
  and its callers; the rest waits on `XACTGetTimeMs` over
  QueryPerformanceCounter.
  Its validation: three functions (`XACTTrack::SetVolume` `sub_825D0BF8`,
  `UpdateVolume` `sub_825D0428`, `XACTEvent::Create` `sub_825D0F00`)
  print on each native return path instead of as their first statement,
  since they hand some inputs back to the recompiled code.
- XACT's tracks' migration 2 (84a6ef7..65b3944, same flag): `XACTTrack::Prepare`
  `sub_825D0AA0`, `CreateInstance` `sub_825D02F8` and
  `XACTTrackInstance::Prepare` `sub_825D5A48` (the last two and
  `XACTEvent::Create` now Native), and `XACTSound::ScheduleEvent`
  `sub_825CB0C8` (row in `xact.md`, code beside the events; its adapter
  stays for `XACTEvent::Schedule`, recompiled). Harness coverage to look
  at: every new play-wave event's Prepare fails in the harness (its fake
  bank can't make a real wave), so the success path keeping a play-wave
  event is checked only by the attract race (two calls from
  `825CCB98`). 47 of 55 ours; the other 8 wait on `XACTGetTimeMs` over
  QueryPerformanceCounter.

## Run 2026-10-28 22:22 (`b7-loop-28-10-26-3`)

- The car sounds and X3DAudio (map 26adc92): `docs/functions/car-sounds.md`,
  66 rows (1 moved from `functions.md`, 65 new; 4 not seen in play):
  the seven 1,264-byte per-racer car sound objects at `[0x826C5C3C]`
  (eleven parts each: engine, tires, hits, wind, road noise, pass-by),
  played straight through XAudio voices into a submix, not through
  XACT; the game's in-memory wave bank reader (`WaveBankView::`); and
  X3DAudio (13 rows; the range holds 14 functions, not ~40 as the lead
  guessed), translated as the game's code by the XACT precedent
  (2026-10-23). X3DAudioInitialize keeps its row in `sound-cues.md`.
  `sub_82110080` in the lead was a return address inside
  `CarSound::Update` (`sub_82110048`). `known-data.md`: the car sounds'
  globals `0x826C5C3C`-`0x826C5C90` and fields. One lead added
  (XAudio's voice methods, the subsystem's blockers).
- The car sounds' migration 1 (9a0dcc7..3e6c24e, `--native_car_sounds`):
  24 ours (12 adapters removed: `EngineSound::EngineSound` and the eleven
  X3DAudio functions), `CarSound::Update3D` with all of X3DAudio's math
  (`src/car_sounds/`; `x3daudio.cpp` and `car_sound_3d.cpp` built with
  strict floating point). For review: `X3DAudioCalculate` runs the
  original whole for EmitterAngle (0x40) or Delay (0x2), never asked for
  in play; `X3DAudio::Pan` calls the game's `tan` when the rounding mode
  isn't nearest; `SoundGlobals::Init` is native only while the memory
  blocks are native, and calls `Music::Play` through its register step;
  `CarSounds::Destroy` frees through operator delete's plain C++, the
  game's when it refuses; harness gaps: Pan's only 2- and 6-speaker
  handles (others read guest 0 and fault, natively too), atan2's two link
  stores into Matrix's frame not written, Curves' f registers left
  differently (unread), Cone's dead reads skipped. Row corrections:
  `SetBanks`' table read, `TireSound::Amount` steps only +76,
  `Update3D`'s listener and emitter, +200 the reverb level and +204 the
  Doppler factor (`known-data.md` still says unknown: left for the
  user's OK). The rest waits on XAudio's voice methods.
- XAudio's voices (map e784bc0): `docs/functions/xaudio.md`, 108 new
  rows (15 not seen in play): the API entry points `82459DB0`-`8245A7C8`
  and what's below them seen in play; the API lock is the critical
  section at `0x8264D518` (+4 of a static object with vtable
  `0x8204A868` at `0x8264D514`), the mixer's own lock `0x8264D4F8`, the
  voice lists' recursive spin lock `0x82954F10` (+0 the kernel spin
  lock, +4 depth, +8 owner, +12 saved IRQL). Shared rows stay in
  `sound-cues.md`, `c-runtime.md`, `audio.md`, `vehicle-accessors.md`,
  `xact.md`, `heap.md`. Two leads added (the engine and mixer; the
  factory's codec, matrix and effect objects).
- The user's call (2026-10-29), asked because `XAudio::WaitForVoice`
  (`sub_8245AE60`) reads the API lock's recursion count, leaves it that
  many times and takes it back: XAudio's API lock and voice-list spin
  lock become host recursive locks whose depth can be read, taken by
  every user, native and recompiled (added to "Precedent first" in
  `b7-loop.md`).
- XAudio's migration 1 (29a2e7f..fbcd34f, `--native_xaudio`): 34 ours,
  only what takes neither lock (5 adapters removed). For review: calls
  into the recompiled objects (codecs, matrix objects, effects,
  allocators, the voices' vtables) go through a guest call built over
  `ctx` in a frame of the original's size, setting only the registers
  the original sets (SetFormat's r5, Reformat's r6/r7 left by memcpy's
  plain C++); paths left to the original: the trapping rates (0 or above
  48,000) in `XAudioBuffer::Size`, `Clear` and the constructor,
  memcpy's refusals in `Reformat`, `GetAllocator`'s size-0 allocator;
  `Alloc`, `DefaultAllocator` and `GetAllocator` native only with
  `--native_heap`; `SetFormat` keeps its adapter (slot +88 of the source
  voice's vtable). Harness gaps: `BaseSize`'s effect path and the
  factory thunks' real targets. Map correction: `XAudioBuffer::Size`
  reads +0 type, +5 channels, +8 rate. Sound checked by the output's
  counters (frames played, silence fills) against a
  `--native_xaudio=false` run: the same.
- The user's call (2026-10-29), asked because the voice lists' spin
  lock `0x82954F10` is taken inline in 57 functions (about 30 in play
  outside the subsystem: the engine, mixer, codecs): native code takes
  the game's own spin lock exactly as the original does until its users
  are native, then it becomes a host recursive lock; the API lock
  `0x8264D518` becomes the host recursive lock now (its users are all in
  `xaudio.md`, with `sub_8245A410` in `sound-cues.md`).
- XAudio's migration 2 (19a919f..24cc6ea, `--native_xaudio`): 58 more
  (92 of 96 in play ours, 37 Native): the voices with outputs, the voice
  making and passes, the methods under the spin lock, the source voices'
  start, stop and pass, the constructors and destructors, and the API
  entry points but `XAudioShutDown`, the callback's and the commit. The
  locks by the user's calls: the spin lock the game's, taken with the
  original's inline reads and writes and the same kernel imports
  (through the guest call at the imports' thunk addresses); the API lock
  still the game's critical section, not switched: 21 of its 29 users
  are ours, the 8 left (`sub_8245A410`, `XAudioShutDown`, `sub_8245A5E8`,
  `sub_8245A668`, `sub_8245A6E8`, `sub_8245AFA0`, `sub_8245ED60`,
  `sub_825DA0F0`) wait on the engine lead or have no harness possible.
  For review: `XAudioShutDown` left recompiled (its harness would release
  the live engine); nested plain C++ in frames of the original's size
  below the caller's, and callers setting the registers the original
  sets before a plain C++ callee (`xaudio.md`'s intro); harnesses
  against the live engine (list inserts under the spin lock, voices made
  with the real allocators and freed; one small native-XMA handle per
  XMA codec made in `SourceInit`/`CreateSource` cases isn't freed;
  `WaitForVoice`'s wait loop not harnessed); adapters kept though listed
  removable (`sub_82460BF0`, `sub_82461D50`, `sub_82460CB8`,
  `sub_824619B0`); `ProcessEffect` reads an unwritten stack word as the
  original does. Sound: the same counters as `--native_xaudio=false`.
- The car sounds' migration 2 (be59a3f..edce57c, `--native_car_sounds`):
  29 more (53 of 62 in play ours; 19 adapters removed, with XAudio's
  `SubmitPacket`): the voice holders, the voices made and started, stop
  and release, `CarSounds::Init`, the hits, wind, pass-by and three other
  parts' updates (part A of `CarSound::Update`'s subtree, by a subagent).
  For review: the plain C++ calls XAudio's entry points' plain C++
  directly in frames of the original's size (`xaudio::FrameBelow`; the
  prologue's saved registers not written, as XAudio's adapters);
  harnesses with real voices made by the engine (the mixer held out by
  its lock, voices stopped, unlinked and freed after); `sub_8210EEE0`
  sets slot +52's volume without checking for a voice, as the original.
  Row corrections: `SetPitch` clamps to a minimum; `SetPitchScaled` uses
  r9 and f3; the two hit parts were swapped (`sub_8210E778` the impact,
  `sub_8210E938` the scrape); `WindSound` reads the vehicle's half-words
  at +5410 + 96i. Pause, Resume, StopAll and the releases not reached in
  the attract race. Sound: the same counters as
  `--native_car_sounds=false`.
- The car sounds' migration 3 (fd88bdf..cfd838d, `--native_car_sounds`,
  by four subagents one at a time): `CarSound::Update` and the engine,
  tire, roll, horn and turbo updates; 22 adapters removed; 62 of 62 in
  play ours. For review: `CarSound::Update`'s adapter stays (its caller
  `sub_82175F20` is recompiled); XAudio's `SetOutputVoices`,
  `MixToOutputs`, `StopOrTail` keep theirs; before plain C++ callees,
  `xaudio::Regs` sets r3 upward and f1 only, so f2 (before
  `EngineSound::Update`, `SetPitch`, `SetPitchScaled`) and r7 (before
  `TireLayer::Update`) aren't left as the original leaves them (the
  callees take them as arguments; the harnesses match); `CarSound::Update`'s
  tan through the guest's; harness gaps where nested callees' register
  saves land on callers' locals. Ported as the game has them:
  `UpdatePitched`'s pow branch (above 12,000 with the speed capped at
  8,000) and layer 0's f2 always 1.0. Row corrections: `CarSound::Update`'s
  r4 is 1 when the vehicle's +6148 & 0x81 is set; `EngineLayer::Update`
  reads two eight-point curves (volume, pitch); `TireLayer::Update` drives
  one voice; `sub_8210F118` a looping horn (wave 11 + the car's +1796
  byte, input bit 0x20000); `sub_8210F4E8` the turbo whistle from the
  boost at +2944. Sound: the same counters as `--native_car_sounds=false`.
- XACT's waves and voices (map a950696): `docs/functions/xact-waves.md`,
  91 new rows (30 seen in play): XACT's voice class (`825C9AF8`-`825CA680`,
  vtable `0x82075B98`), the reverb submix wrapper (vtable `0x82075BD4`)
  and its maker `sub_825C3398` (never ran), the waves `825D1C10`-`825D53D8`
  (base, in-memory and streaming, the stream entries, the variation and
  transition tables) and the tracks' volume and pitch getters
  `sub_825D0268`, `sub_825D02B0`. XACT's lock (the engine's critical
  section, +144, taken inline in 62 functions, entered again on purpose
  by `sub_825D1C78`) follows the 2026-10-29 call: the game's, taken as
  the original does, until every user is native. `known-data.md`: the
  voice, reverb submix, wave, variation table; probable cue and sound
  fields. The tracks lead narrowed (the tracks go with the events).
- XACT's waves' migration 1 (bfb4cbc..4c7a053, `--native_xact_waves`,
  `src/xact/`): 22 of 30 in play ours (16 adapters removed, 7 of them
  XAudio's entry points and 5 in `xact.md`). For review: XACT's voice
  methods call XAudio's plain C++ directly (the car sounds' convention),
  with or without `--native_xaudio`; the waves call the voice's Start,
  Stop and FlushPackets through their plain C++ while its slot holds the
  native function, through the guest call otherwise (the wave bank's
  AddListener and `GetWaveBank` alike, with the native heap on); Create,
  Init and the voice's destructor run the original with the native heap
  off; XACT's lock taken only by `XACTEngine::RequestCommit`, through the
  guest call at the imports' thunks (the 2026-10-29 call); harness gaps:
  frame words compared only for GetState and SubmitPacket, Play and
  PrepareNext always given a voice and a new wave, deferred starts only
  on playing voices, Create without a reverb and its failure paths;
  `sub_82379D50`'s adapter stays (called from XAudioVoice::Pass's
  recompiled code). Row correction: `GetStartPosition` uses the count
  byte while +25's bit 0x20 is clear. Sound: the same counters as
  `--native_xact_waves=false`.

## Run 2026-10-28 18:45 (`b7-loop-28-10-26-2`)

- The archive (map a8a12e3): `docs/functions/archive.md`, 27 new rows
  (the archive's B4 rows stay in `files-and-saves.md`, named shared:
  `Archive::Init`, `Shutdown`, `Load`, `AssetSize` (`sub_820A0678`,
  named and read in full), the queueing, slot lookup, picker, set-up,
  finish, next request, loose read, loader thread, Ucl). Corrections in
  `files-and-saves.md`: the slot lookup, picker, queueing and
  `Archive::Shutdown` rows read in full (the slot table starts at
  `0x826A5168`, not `0x826A5170`). `known-data.md`: the archive's
  globals `0x826A5100`-`0x826A512C`, the TOC, the 92-byte request
  slots, the queue at `0x826BAB08`, the four semaphores, the loader
  thread's parameters at `0x82600B50`. Two leads taken into the map
  (`sub_8209D7B0`, `sub_82294BA8`), two added.
- The user's call (2026-10-28): the archive loader's four semaphores
  (queue lock `0x826BAAE0`, work `0x826BAAB8`, request-done
  `0x826BAA68`, idle `0x826BAA90`) become host counting semaphores with
  a readable count (two users read +32), taken by every user, native
  and recompiled (the loose read `sub_8209DFA8` signals request-done);
  this replaces the 2026-10-24 note that the Semaphore class stays
  recompiled for the archive loader.
- The archive's migration 1 (47d1ab1..eac2d83, `--native_archive`): 14
  functions ours (rows in `archive.md` and `files-and-saves.md`). For
  review: `hooks::VirtualFree` exported from `src/hooks/heap.cpp` for
  the request finish's decommit (does nothing in the allocation layer);
  the request finish's callback and `LoaderFunctor::Invoke`'s tail call
  stay guest calls through `ctx` from their adapters; `Ucl::Unpack`'s
  stored-block copy goes through `native::Buffers_Memcpy`; the harness
  takes the game's queue lock (translated `Semaphore::Wait`/`Signal`)
  around cases touching the live queue; Ucl cases use an NRV2B encoder
  in the harness and 16 assets from BUILD.DAT.
- The archive's migration 2 (f4b6d09..db11bea, `--native_archive`
  extended): `Archive::Load`, the queueing (named `Archive::Queue`),
  WaitIdle, QueueLength, CancelQueued, `Archive::Cancel`, `PickNext`,
  `SetUp`, `NextRequest`, the loader thread's and `Archive::Init`'s
  waits; 8 adapters removed. The four semaphores are host counting
  semaphores by the user's call: `Semaphore::Wait` `sub_82592670` and
  `Signal` `sub_825926E8` (`video-output.md`) are now Overridden,
  routing the four addresses to the host ones and every other
  semaphore to the game's code; the choice is made at start-up (with
  `--native_archive=false` the four stay the game's, raced 38 s, 0
  differ). Rows in `archive.md`, `files-and-saves.md`,
  `video-output.md`, `known-data.md`.
- The file layer (map f053e6c): `docs/functions/file-layer.md`, 21 new
  rows (11 in play): the file object `GameFile` (vtable `0x82002270`)
  over `AsyncFile` (`0x8206CC0C`), `File::` (`0x8206C05C`) and a base
  stream (`0x8206BFF8`); the path object `Path::` (`0x8206CC3C`,
  `Path::Build` `sub_82593CA0`, only kind 0 in play); `GamePath::`
  (`0x820022A0`, only `Movie::Open`, no longer called). Shared rows
  stay in `files-and-saves.md`, `sound-cues.md`, `movies.md`,
  `c-runtime.md`. `known-data.md`: the file and path objects; the
  global strings' addresses corrected (`0x8265EE7C`/`98`, not
  `0x8266EE7C`/`98`). One lead added (`sub_82593D78`, `sub_82593D88`).
- The file layer's migration 1 (47a7c1d..edea212,
  `--native_file_layer`): the file and path objects' constructors and
  destructors, `Path::Build`, `GamePath::SetGame`, the open by name; 7
  adapters removed. sprintf `sub_825A93D0`, `_vsnprintf`
  `sub_825AB010`, `_vsnprintf_l`, `_output_l` (`c-runtime.md`, under
  `--native_c_runtime`; `src/c_runtime/narrow_format.cpp`, harness
  `c_runtime_format`), following `_woutput_l`'s precedent: a dry run
  first, and the original runs for what the native one doesn't do
  (`%n`, wide text, floats other than `%f`, the heap's buffer, a
  forced point, no room, a lead byte ending the format, output over
  its own format or arguments); the native path ran in ~3,500 of 4,870
  sprintf cases. Format strings in play: `"%s%s"`, `"%s%s%d"`,
  `"data\audio\%s.xgs"`, `"game:\%s"`. The archive's loose read
  `sub_8209DFA8` under `--native_archive`; it and the open by name stay
  adapters calling B4's `File::` adapters and the read-error report
  through `ctx`.
- The file layer's validation added `_vsnprintf`'s missing print
  (a2eadab). In `Sprintf`, `Vsnprintf`, `_output_l` and `Path::Build`
  the print comes after the checks that hand the call back to the
  original (it logs only when the native path runs); `Path::Build`
  has it at two places.
- The save system's interface (map 3aa9f33):
  `docs/functions/save-interface.md`, 62 rows (`sub_821109B8` moved
  from `functions.md`, 61 new; 41 in play): `[0x826BDC90]` is app +
  16, the app's interface (vtable `0x82000E38`, stored by
  `SaveInterface::Init` `sub_8209FFB8`, which sets the content root
  "TXR360"); its slot +12 (`sub_820943B0`) the save devices, +8
  (`sub_820943C8`, shared in `system-services.md`) the users; the
  wrappers over the save slots' getters and request setters (+968,
  +972, +976); the front end's save layer (`8228F6B0`-`82290640`):
  the save buffers at `0x828161F0`, `GameData::Save` (SystemData.bin,
  129,400 bytes), `GameData::Load`, the replays (rep_* files,
  1,231,088 bytes), `PlayTime::Update`. No Xbox service called (B4's
  `SaveSlot` steps answer the requests). `known-data.md`:
  `0x826BDC90`, `0x828161F0`, the replay record, the save slot's
  request words. Two leads added.
- The save interface's migration 1 (63d02d8..53423d6,
  `--native_save_interface`): all 41 rows in play ours, 17 adapters
  removed; also taken under this flag: `sub_82295D48`, `sub_82295C18`
  (`files-and-saves.md`) and the users' vtable targets `sub_820943C8`,
  `sub_82493700` (`system-services.md`). For review: the plain C++
  checks each vtable target is ours first and hands back to `__imp__`
  if not (a "vtable targets" harness group checks the game's vtables);
  SaveBuffers, BuildList and Sort run natively only with
  `--native_heap` (and `--native_memcpy`); the harness points the live
  globals `0x826BDC90`, `0x828161F0` and `0x826C5B58` at harness copies
  while each case runs, then puts them back (the harnesses run before
  the race; no live saves touched); two harness spots don't compare
  stack words that hold the original's register saves or back chains
  (`PlayTime`'s out inside `GameData::Save`, `Sort`'s copy inside
  `BuildList`), and `GameData::Save`'s compare leaves out
  `GetLocalTime`'s seconds (ce1a3bd). `sub_82295C18`'s adapter stays
  for `hooks/saves.cpp`.

## Run 2026-10-28 08:36 (`b7-loop-28-10-26`)

- Texture compression (map f1c484c): `docs/functions/texture-compression.md`,
  157 rows (all new; 24 seen in play, the rest "Not seen in play"):
  XGraphics' `XGCompressSurface` (probably, `sub_8235AAC0`) over D3DX's
  image blitter (the per-format codecs from `ImageCodec::Create`
  `sub_823F6C60`, ten blitters and filters), used in play only by
  `Vinyl::Compress` (A8R8G8B8 -> DXT5, premultiplied). Map correction:
  the lead had `sub_82368170` as a caller; it's a callee
  (`XGCopySurface`, probably; never ran). Not part of the HLSL
  compiler. `known-data.md`: `0x82954660`, `0x8263B7E8`, `0x8263C768`,
  `0x82001030`, the codec vtables and layout; notes in `heap.md` and
  `car-models.md`; one lead added (`XGCopySurface`'s 24 per-format
  routines, none seen in play).
- Texture compression, migration 1 (cdf8494..af2c1ee): 29 rows in
  `texture-compression.md` (24 ours, 18 Native; 5 with notes only);
  map correction in `XGCompressSurface`'s row (the YUV formats
  `0x1A20004B`/`4C` go with flags 4). Code in
  `src/texture_compression/` (strict-float options), adapters in
  `src/hooks/texture_compression.cpp`; allocation through
  `src/memory/heap.h`, the native path only while the native heap is
  active. Handed to the originals (never in play): DXN/CTX1
  destinations, sources copied or swapped first, the YUV formats, the
  eight filters, `CopyBlocks`, the color key's slot 3, sRGB, DXT block
  pointers other than DXT4/5's. For review: (1) stack leftovers:
  `XGCompressSurface` keeps its two descriptions and blit object on
  the guest stack at the original's offsets, and the color encoder's
  adapter writes the two words the original stores in its caller's
  frame; but a few words the original's callees leave below the stack
  pointer after returning (the color encoder's r6/r3 slots, the
  premultiplied pixel copy, `EncodeRow`'s 0,0,0,1 padding table) aren't
  written natively (no caller reads them); (2) `FitEndpoints`' negative
  index path near a multiple of -2^28 would read into `EncodeBlock`'s
  frame in the original (pixels hundreds of millions of steps out of
  range; in the row); (3) `EncodeRow` on a box cut on the right reads
  leftover heap memory in the original (the harness primes the heap; in
  play the box is the whole surface). The vinyl's detail couldn't be
  judged from the dumps (cars too far); the `XGCompressSurface` harness
  compresses a full 1024x1024 sheet byte for byte.
- `Vinyl::Compress` without mapping (689b0c8, 53ea064): rows in
  `car-models.md` for `Vinyl::Compress` `sub_820ED090`,
  `Texture::Create` `sub_820ECF08`, `Texture::Free` `sub_820ECFC8` (a
  new row), plain C++ in `src/car_models/vinyl.{h,cpp}` under
  `--native_car_models` (only with the native heap; `Texture::Free` and
  `Vinyl::Compress` only with an old header 0 or in the allocation
  layer's range, VirtualFree's adapter's condition);
  `XGCompressSurface` Native (`texture-compression.md`'s row and
  intro), `--native_texture_compression` now switched by
  `--native_car_models`. Kept as the original: `Resource::Release`
  `sub_822A67B0` a guest call from a 272-byte frame with the original's
  return addresses; the frame's words written as the original writes
  them, except the saved registers (as `XGCompressSurface`). Harness:
  allocation addresses compared as which allocation plus offset; the
  comparison of `Texture::Create`'s frame inside `Vinyl::Compress`'s
  cases removed (the original `XGCompressSurface` saves registers over
  it, its plain C++ doesn't; `Texture::Create`'s own cases check it).
- The vinyl sheet's decal path without mapping (de91953..c9f00d1): 21
  rows in `car-models.md` (two new: `sub_820EED58`, `sub_820EED38`;
  `sub_820EE698` Merged into `Vinyl::DrawDecal`, the recompiler's split
  of its switch; `sub_820D4078`, `sub_820D3C80` Native with no adapter;
  `Vinyl::Compress` Native). Plain C++ in `src/car_models/tga.*`,
  `raster.*`, `decals.*` (strict-float; float steps in the translated
  code's operand order), under `--native_car_models`. Handed to the
  originals: TGAs other than type 2 with 24 or 32 bits (RLE never seen),
  files or a sheet outside the allocation layer's pages, region numbers
  above 9. Adapters kept: `sub_820D5750` (`Archive::Load`'s callback),
  `sub_820EE638`, `sub_820ED020`, `sub_820EEBC0` (recompiled callers
  outside the lead). One group expected to differ
  (`expected-differences.md`, "Unwritten header bytes in the frame").
- The run stopped at the decal path's validation (`ClearCorner` with
  no harness line); the lead fixed the harness by the user's call
  (9dc3c72: compress cases with no slot loading, and the cleanup's
  double free of the new sheet header they exposed) and resumed the
  run.
- The race world (map 5b6d76c): `docs/functions/race-world.md`, 54 rows
  (all new; 45 in play, 9 not seen): the race mode's state machine at
  +116812 (`RaceWorld::`, 24 transitions; states 1 load, 2 loading, 3
  start, 7 racing, 24 paused, 37 end), the racers' manager at
  `[0x826D82F4]` (80 entries of 28 bytes: 77 opponents and 3 traffic;
  at most 4 opponents and the allowed traffic get one of the 7
  vehicles, streamed by distance from the car's speed), the race
  event's start placements (`RaceEvent::Place*`), `RaceStart::Place`
  and `PlaceRolling`, the save's distance on unload. `known-data.md`:
  `0x826D82F4` extended, two new rows; `leads.md`: lead 31 deleted (all
  mapped here), `sub_82174D28` dropped from lead 50, one lead added (the
  race's other states); `ai-drivers.md`: three notes repointed.
- The race world, migration 1 (4824fa1..1472716): 25 rows in
  `race-world.md` (24 ours, 2 Native, `sub_821708B4` Merged) and
  `TrackCollision::Tick` `sub_820FF050` in `collision-world.md` (under
  `--native_collision_world`). Code in `src/race_world/`,
  `src/track_collision/track_world.cpp`. Map corrections:
  `RaceManager::BranchesMeet` returns 1 when r4 is 0 or 29 or the same
  branch as r3 (its case table in the row); `ResetVehicles` calls
  `Vehicle::SetUp` for slot -1 and `ResetForCar` otherwise. Refactors
  with no behavior change: the guest-call lambdas of the vehicle and
  car physics adapters moved into shared functions
  (`hooks::VehicleTickCalls`, `VehicleSetUpOriginals`,
  `CollisionWorldUpdateCalls`, new `src/hooks/car_physics.h`); the
  vehicle and car physics harnesses export set-up helpers.
- The race world, migration 2 (1be0160..b3013b9): 37 rows in
  `race-world.md` (25 Native; `sub_82144874`, `sub_8217437C` Merged).
  Map corrections: `PlaceGroupFixed`'s groups 0 and 1 swapped;
  `PlaceStarts` places three groups by `PlaceGroupFixed` and one by
  `Split` or `PlaceGroup`; `RaceStart::Place` ticks 300 times in mode 6
  with the stage's +0 not 1, else 30; `PlaceRolling`'s "all" is save
  +6100 equal to 1; `OnLoaded`'s lane is the racer's place mod 2;
  `MoveUnseen` has no stepping loop (section ranges corrected);
  `Stream`'s kinds 2 and 3 go to `StreamCruise`, 5 to
  `StreamTwoPlayers`, the rest to `StreamNear` (attract races stream
  through `StreamNear`). For review: `StreamTraffic`'s neighbor search
  left out as dead code (its 0x800 count never goes below 0; a comment
  and the row say so); `GiveVehicle` `sub_821706E8` and `sub_821743A8`
  (neither seen in play) got overrides running the original or a
  harness stand-in while one is set (the time harness's precedent), so
  their rows are Wrapped; `hooks::AiUpdateAll` with a harness stand-in
  in `src/hooks/ai_drivers.*`; the vehicle harness's SetUp cases
  factored into exported helpers (case counts unchanged). One group
  expected to differ (`AddRacers`, random traffic numbers, 5 of 200:
  stack leftovers). The harnesses now take ~29 s of the 40-second race
  (`race_world`'s 8.3 s), leaving ~9 s of attract race.
- The race world, migration 3 (981658f): `RaceManager::Refresh`
  `sub_82171978` (r3 not kept: no caller reads it; stops at the first
  `GiveVehicle` without a free slot, as the original); a shared
  `OpponentsCall` helper with `StreamOpponents`' adapter.
- The race's states (map 009568e): `docs/functions/race-states.md`, 78
  rows (all new; 61 in play): `RaceStates::Init` `sub_82169920`'s machine
  at +107168 (87 transitions in the intro: stage set-up, loading, rolling
  and standing starts, countdown, racing, pause, quit, end, replay, the
  menu after the race, between stages and the next stage (not seen),
  free run, a demo, abort), the race's progress at `[0x826D82E8]`
  (`RaceProgress::`, 7,248 bytes; 972-byte results entries: times, laps,
  distance, hits, places, SP, probably spirit points), the per-tick
  checks by stage type, its messages to `RaceWorld`. Not read in detail
  (rows from their calls): `sub_821635D0`, `sub_82162330`,
  `sub_82166658`, `sub_821634D8`, `sub_821653E8` and the unseen
  handlers. Kind 6's System Link branch mentioned once, not mapped.
  `known-data.md`: `0x826D82E8` expanded, "race states" and the loading
  screen `0x82815E60` rows; four leads added, three leads' notes.
- The race's states, migration 1 (edb5487..2b3fe33): 35 rows in
  `race-states.md` (6 Native; `Time::Sub` `sub_82162230` Native with
  no adapter, never seen). Code in `src/race_states/`. The System Link
  case of `StandingStartTick` runs the original whole (the row says
  so). Float compares follow the translated code's `bge` as `!lt` and
  `ble` as `!gt` (a NaN takes them; `src/race_states/steps.h`). Map
  corrections: `DrainSp`'s side test uses flags 0x1, 0x2 and 0x80 (not
  0x40); `CheckLap` compares with the next lap's distance
  (`+7160 + 4 * (R+60 + 1)`); `TickFraction` reads r3 (the slot's
  entry); `MapToSection`'s walk described. Exports added:
  `race_setup::CopyCourseRoute`, `RaceWorldOpponentsCall`,
  `RaceWorldStartCalls`, `RaceWorldListRacersOriginal`.
- The race's states, migration 2 (66b8f50..0af672e): 19 rows in
  `race-states.md` (14 more Native; `sub_82163000` and `RouteDistance`
  Native with no adapter). `race_setup::RouteCopiesNative` and
  `CopyCourseRoute` take an optional `to` offset (default 96; callers
  unchanged). For review: `RaceTick`'s and `OnGo`'s harness cases give
  each racer 1 to 3 sections (with an empty list `FollowRoute` writes
  over the slot's manager entry, in the original too; in play
  `StartRoute` always lists the start first); `StartRoute`'s harness
  routes are built, not the game's (the real routes loop on random
  links); `RaceTick`, `OnGo` and `StartRoute` run the original whole
  where the course route's memcpy copies aren't native, and `RaceTick`
  also with +7220's 0x8 (`sub_821653E8`, never seen).

## Run 2026-10-27 22:31 (`b7-loop-27-10-26`)

- The app's update and its state stack (map ac92c3b):
  `docs/functions/app-states.md`, 56 rows (2 moved from `functions.md`:
  `App::Update` `sub_82093DC0` and the Wrapped `sub_82092968`; 54 new),
  with the containers it uses (`PtrList::Erase` `sub_82098E80`, mapped
  here instead of `input.md`; `OpList::IncSize` `sub_820AD768`, from the
  containers lead); `game-modes.md`'s `Modes::Tick` row says what calls
  it; `known-data.md`'s app fields and two new rows (app state stack,
  state layer); `leads.md`'s app start-up and containers lines brought
  up to date, one lead added (`820AC998`-`820ACC08`). For review: the
  map's reading of `App::Update`'s player 1 Start toggle as probably a
  debug pause (it picks handler map +28 or +40; both reach
  `Modes::Tick`).
- The app states, migration 1 (7bb4dfd..b29697d): 31 rows in
  `app-states.md`. `App::RunHandler` `sub_82092968` was Wrapped by
  `src/debug/menu_selection_scan.cpp` (`--find_menu_selection`): the
  wrap moved into its adapter in `src/hooks/app_states.cpp`, so the
  row is Overridden. Map correction: `OpList::OpList` links the head to
  itself at +0 and +4. Kept as the originals: traps logged with a
  warning and execution goes on (as `main_loop.cpp`); each walk's
  functor-adjustment word keeps the frame's stack word in its low half
  (dead stack); `App::Update` calls the button tests `sub_8258CEB0` and
  `sub_8258CEE0` as guest calls. Handed to the originals (never in
  play): odd `ApplyRequest` types, `length_error`, `bad_alloc`.
  `std_list::Erase` added in `src/std_list/ptr_list.{h,cpp}`.
  Validation 1 noted that in `sub_82093BE0`, `sub_820944E0`,
  `sub_820938C0` and a few others the print comes after an early check
  that hands the call back to the recompiled code (some print at more
  than one site), not as the first statement; left as written.
- The app states, migration 2 (4baecb1..30892a7): 28 rows in
  `app-states.md`, now 56 of 56 ours; new code in
  `src/app_states/layers.{h,cpp}`; `AppCalls` got a `native_heap`
  field. Map correction: `StateLayer::Update` `sub_820ADF00` and
  `UpdateToggled` `sub_820AE010` are reached through slot thunks and
  delegates, not called directly. Left on the original (never in play):
  `StateLayer::ApplyOp`'s pop and replace (types 1 and 3),
  `AppStates::OnMessage`'s messages 1, 5 and 6 (the step and the
  teardowns), and every failed `operator new`. Adapters kept: those
  reached through vtables, delegates or function pointers, and
  `AppStates::Create` (called from the app's constructor
  `sub_82094590`). One commit (be3d047) left three rows' Status blank;
  fixed in the next (a7c59b8).
- The scene's loading objects (map 2616cc8):
  `docs/functions/scene-loaders.md`, 78 new rows: the first scene pool
  set's (`[0x826C2E40]`) seven loading classes (car model, parts,
  cockpit body, interior, pool 4's model, wheels, traffic body), their
  embedded loads, `ScenePools::MakeA`, `CreateA`, `ReleaseA`, the
  `PoolNode` base, the load callbacks (`sub_820D6528` the cockpit's,
  `sub_820D8430` the traffic body's, `sub_820D7578` the interior's),
  `XPR::FindByName` and `ModelPoint::Find` `sub_820D02D0`.
  `known-data.md`: the first set's fields, rows for the pool node,
  cockpit body, pool 4's object, traffic body and model point; the car
  model, interior, parts and wheels rows extended. 3 leads added. Noted
  in the rows: the deleting destructors call `operator delete` on
  objects `ScenePools::CreateA` takes from `VirtualAlloc` (nothing calls
  them directly); `sub_820D2270` walks every material and uses nothing.
- The scene loaders, migration 1 (4b47c12, dfbc5a0): 20 rows in
  `scene-loaders.md`. `CarParts::SlotOf` computes in 64 bits as the
  original's register adds; `CarParts::Part` returns only 32 bits (its
  one caller loads r5 and r6 with `lwz` and stores r3 with `stw`).
  `CarModelLoads`' destructor leaves r3 = loads + 32 (as
  `Skeleton::~Skeleton`'s adapter). `CarModelLoads`' constructor and
  destructor run natively only while `hooks::ModelsNativeLifetime()`
  holds; the destructor checks its members first and runs the original
  if a check fails. `Pool4Object::SetRecord` hands overlapping copies
  and physical-view destinations to the original (as
  `CarModel::SetRecord`). `MakeA` and `sub_820D0960` run natively only
  over the allocation layer.
- The scene loaders' blockers (no mapping; e9e8e77..effc3f0): rows in
  `car-models.md` (`sub_820D0730`, `sub_820D0758`, `sub_820C3F50`,
  `sub_820D4508`, and a line in its intro), `models.md` (`sub_820CE928`,
  `sub_820CE990`, `sub_820CE9C8`) and 19 in `scene-loaders.md`; new
  flag `--native_car_models` (`src/car_models/`); the part model under
  `--native_models`; `models/model.h` exposes `ModelReleaseNodes` and
  `ModelMembersDestructible`. Choices: the provider's slot 2 is called
  natively only when it is `TextureProvider::ClearAll` `sub_82594328`,
  otherwise the whole function runs the original; matrices the
  originals build on their stack and copy with memcpy are written
  straight into place (their frames not written), and a car model at or
  above the physical views (`0xA0000000`) runs the original so memcpy's
  override still records the copies for the renderer; a function whose
  slots would free the same pages twice runs the original. The
  harnesses' recompiled side runs with all three flags off. Frame dumps
  after batch 5: 2,414 draws against 2,412 with the flags off, the off
  run's frame captured a moment later (the same after batch 2).
- The scene loaders' blockers, migration 2 (ecdb767..016410c): 36 rows
  in `scene-loaders.md`, 3 in `models.md` (`sub_82599AF0` named
  `Skeleton::ResetPose`, probably; `sub_820CE9F8`; `sub_825896C0` named
  `SetSharedShaders`, probably). Choices: the six deleting destructors
  run natively only with bit 0 of their flags clear (the first set's
  teardown `sub_820C3CF8` passes r4 = 0); with it set (`operator delete`
  on `VirtualAlloc` pages, never seen) the original runs.
  `ScenePools::CreateA` and `Interior::OnPartLoaded` make guest calls to
  the originals (the constructors; `Model::Load`) from a frame like the
  original's when a callee's plain C++ declines, as the scenery lists
  do; no harness case reaches `CreateA`'s fallback. For review:
  `Skeleton::ResetPose`'s harness doesn't compare r3 (177 cases differed
  in r3 only): it ends as `ApplyAnimation` leaves it (its adapter leaves
  the skeleton, its original a loop register; its own cases don't
  compare r3 either), and all five callers set r3 right after. Open,
  not investigated: with `--native_scene_loaders=false`, three 256x256
  textures sit at other physical addresses (draws otherwise the same);
  not caused by this migration's adapters, so from the earlier ones.
- The players' save data (map 8cbdba1, 1fedb13):
  `docs/functions/save-data.md`, 23 rows (1 moved from `functions.md`,
  22 new): `SaveData::Create` and `Delete`, the progress, stats, course
  records, options and control maps, five default tables' getters,
  `TitleScreen::ResetPlayer` `sub_82221D00`, `MenuList::Init`
  `sub_8220BAC0` (about 75 front-end screens call it; its neighbors in a
  new lead) and `Players::ResetControllers` `sub_8209F5E0`; 4 not seen
  in play (`CourseRecords::Of` `sub_82105BF8`, `sub_82105740`,
  `sub_82105688`, `Progress::Restart` `sub_82114128`). The mapping agent
  first left those 4 without rows, following `b7-loop.md`'s "Only
  functions seen in play get rows"; the lead had them added by the
  user's later call (2026-10-23, CLAUDE.md). `b7-loop.md`'s line may
  want updating. Kept as the originals: `Stats::Stats`'s writes are
  wiped at once by `Progress::Clear`; `SaveData::Create` runs the
  set-ups even when the allocation fails. `known-data.md`'s
  `0x826C5B58` row and a new `[0x82814E30]` row; `mode-tick.md`'s
  `OnModeChange` row.
- The save data, migration 1 (0fb0eb9..d585a41): 19 rows in
  `save-data.md` and its intro; code in `src/save_data/`. Adapters kept
  for recompiled callers: `Progress::Clear` and `SetDefaults`
  (`sub_82221AC0`), the `Table3044` getter (`sub_82110448`),
  `SaveData::Create` (`sub_82105868`, `sub_8209E7E8`), `Delete`
  (`sub_8209E7E8`), `ResetPlayer` (`sub_82222B60`), `MenuList::Init`
  (about 75 screens), `Players::ResetControllers` (`sub_82106548`).
  Kept as the originals: `Stats::Stats`'s writes; the 0.0 float through
  float, double, float (`lfs`, `stfs`); the getters' r3 and r11 sign
  extended; each r3 as the original leaves it. Not written natively
  (following earlier ports' scratch below r1): `SetDefaults`' stack copy
  of `Table3044`, `ControlMap`'s stack table, the two stack words
  `ResetPlayer` passes to `MenuList::Init`. The allocating ones run
  natively only under `--native_heap`; `SetDefaults`' copies go through
  `native::Buffers_Memcpy` (the original when `--native_memcpy` is off
  or the ranges may overlap); `MenuList::Init` runs the original when
  its old block isn't the allocation layer's; on `operator new`'s
  nullopt (never seen) `Create`'s adapter runs the original.

## Run 2026-10-25 23:42 (`b7-loop-25-10-26-4`)

- The race's set-up (map 7d2acaa): `docs/functions/race-setup.md`, 52
  new rows (none moved); shared rows left in `vehicle-accessors.md`
  (`RacerRecord::Init`), `car-physics.md` and others; `known-data.md`'s
  race data, stage list, racer records (the race data's 77 records at
  +0x59AE8), `0x826C6078`, save data, item and car object rows extended,
  four rows added (course route, route, course record, 972-byte entry);
  3 leads added, 6 extended. Never seen in play: `sub_82105BF8`,
  `sub_820FFA88`, `sub_820CC450`.
- The race's set-up, migration 1 (2e9d8eb..357f247): 31 rows changed in
  `race-setup.md`, one note in `c-runtime.md` (the array helper calls
  `sub_82101D20`'s plain C++). Corrections to the map:
  `RaceEvent::FirstOwned` `sub_821430A0` returns the first item whose
  save record sums to 0 or below (the map had it reversed; the name may
  be wrong); `CourseRoutes::Get` loads a fifth float at `0x82001FD4`.
  `CourseRoutes::Get` replays the original's 1,175 constant stores into
  its frame (`r1 - 4480`) on every call, with the index and out words.
  Not reproduced (following earlier ports): register saves and scratch
  below the stack pointer (`CarTuning::Reset`'s red zone, the
  `fctiwz`/`fcfid` conversions, `sub_82101EB0`'s running mean); copied:
  `Car::SetModel`'s +72 twice and +73 left.
- The race's set-up, migration 2 (9b93272..482c90c): 25 rows in
  `race-setup.md` and `RacerRecord::Init` `sub_820FF808` in
  `vehicle-accessors.md` (now under `--native_race_setup`). Each native
  callee runs in the frame the original's call gives it, the frame
  leftovers (route copies, stack-built records) written as the original
  leaves them; register saves not written (as migration 1). Paths left
  on the original from the same frame through a guest-call callback: a
  memcpy its override wouldn't make natively, `Car::Refresh` refusing,
  the never-seen branches (`sub_82105BF8`, `Race::PlaceRacers`'
  +0xA3AF4 = 0 or 4; `sub_820FFA88`, `Race::SetUpRival`'s kind 1). f1
  not handed back after `Car::FromBuild` (as its adapter). The harness
  builds made-up specs, items, save data and an event (the game's
  parameters aren't loaded when harnesses run). For a decision: one new
  expected group, `Race::SetUp`'s traffic records' 42 unwritten bytes a
  record, copied into the race data from register saves the native
  callees don't write (`expected-differences.md`, "Unwritten stack bytes
  copied"; whether the game reads them isn't known).
- The race's set-up, migration 3 (62e7b9a..2f54d5a, four parts by
  subagents): `car-physics.md`'s `sub_820AF1C0`, `sub_820C05C8` (named
  `CarData::ApplySettings`, our name), `sub_820C0D28`, `sub_820C27E8`
  rewritten from a full read; `sub_820AEE98`, `sub_820AF018`,
  `sub_820B6A38` set to Native; `race-setup.md`'s `sub_820CD5C8`
  Overridden. `SetupFromCar`'s and `SetupVehicle`'s adapters stay
  (recompiled callers in play).
- The car physics' ready groups (no mapping; f8a45a7..b488eb2): rows in
  `car-physics.md` (52, one new: `sub_8259D5A8`), `vehicle-accessors.md`
  (28), `matrix-math.md` (2), `collision.md` (2); 74 adapters removed
  after a first-call probe showed none called in an attract race.
  Learned: the translated `ble` is "not greater", so a NaN keeps the
  value so far (`ProbeGround`'s speed clamp, the step's travel minimum;
  in `car-physics.md`'s intro). `PhysicsStep`'s callees that refuse on
  paths never seen (`UpdateSteering`'s tan error, `UpdateTorqueSplit`'s
  mode 2) run their original in the step's frame through a
  `std::function`, as the mode tick's draws. The allocating functions
  run natively only under `--native_heap`. Not ported: `Create`'s and
  `SetQueryCapacity`'s passed-in buffer branches (refuse, run the
  original); `RemoveObject`'s branch in `DestroyPhysics` ported.
  Removable adapters outside the area left: `sub_820FF938`,
  `sub_820FF9F8`, `sub_820FFAB0`.
- The car physics, migration 2 (3a5e3bd, bd3e362): `CollisionWorld::Update`
  Overridden, its four callees Native with no adapter, 10 more Native;
  `sub_82294330` `Vec3::SubFrom` a new Merged row in
  `vehicle-accessors.md` (not seen in play, done inside `Respond`);
  `collision-world.md`'s `sub_820FF1B0`, `sub_820FEBD0` notes. The two
  queries in play (`TrackCollision::WallHits`, `GroundHits`) called
  natively whatever `--native_collision_world` says (as the track
  position calls the route's functions whatever `--native_track_route`
  says); other query targets and refusing callees run as guest calls.
  `Respond` wasn't reached in the attract race (harness only).
- The vehicle (map ebba856): `docs/functions/vehicle.md`, 25 new rows
  (21 in `820BF000`-`820C3C00`, and the engine's power curve
  `sub_820CE7D8`, `sub_820CE430`, `sub_820CE1D8` and
  `RoutePosition::Step` `sub_82170F90`); `known-data.md`'s `0x826C2E34`
  extended, four rows added; 2 leads. Corrected since: `Vehicle::Tick`
  runs `Vehicle::PhysicsStep` twice a tick with f1 = 1/60, where
  `car-physics.md` said "probably once a tick" (its intro and row
  fixed before the merge). Never seen in play: the
  six under `Vehicle::ApplyInput` (`sub_820B0098`, `sub_820B00A8`,
  `sub_820B00B8`, `sub_820B03E0`, `sub_820B0428`, `sub_820B6170`) and
  `sub_820FA8F8`.
- The vehicle, migration 1 (3aa0018..b46632a): 24 rows in `vehicle.md`
  and a "Native so far" paragraph in its intro. `ApplyInput` runs the
  whole original when one of its six never-seen callees would run
  (decided before anything is written); refusing callees
  (`SetupFromCar`, `Set/ResetPhysics`, `LinkTrackPositions`,
  `SteerTowards`, the physics step, the status copy) run their
  original in the caller's frame; `Create`, `Destroy`,
  `RemoveFromWorld` and the physics set-up run natively only under
  `--native_heap` (as the car physics). The mapping notes kept
  (`Vehicles::Create`'s memset, the sort's quirk, `ShiftPeak`'s stores
  below r1, `TrackPosition::Copy`'s +8 to +15, the 1/60 and tables read
  from guest memory). Harness: 207 of 300 `SetUp` and 198 of 300
  `ResetForCar` cases left out and counted (random car data sends
  `EngineTorque`'s curve index outside guest memory, crashing the
  original too); helpers shared from `collision_world.cpp` and
  `car_physics_objects.cpp`.
- The vehicle, migration 2 (0133cf6): `Vehicle::PlaceOnRoute`
  `sub_820C3608` Overridden; a nonzero offset across the road (NaN
  included) runs the whole original, for the never-seen
  `sub_820FA8F8` (as `ApplyInput`); refusing `ResetTransform` and
  memcpy run their originals in its frame; the orientation's vector
  math copied op for op with flush-to-zero.

## Run 2026-10-25 16:38 (`b7-loop-25-10-26-3`)

- For later (the user's call, 2026-10-25: debug when the code is
  translated): one pad replay after the shader objects' migration 3
  (`itc_1847`, killed partway) ran the menus unpaced from boot
  (150-188 ticks/s, later 1,300-2,267): with `--skip_intro` (a testing
  flag, off for players), menus skip the 30 Hz tick wait until frame
  pacing sees the first race (`TitleScreen_SkipTickWait`,
  `renderer_hooks.cpp`), and that run apparently never saw the race the
  title screen runs at boot. Not reproduced (`itc_1851` with
  `--log_race_changes=true`, `itc_1849`, `itc_1850`, `itc_1848`: all at
  30). The user's suggestion: default to paced menus and flip on the
  first race.
- For later (the user's call, 2026-10-25: dropped for now): native code
  reaches `generated/` three ways: the adapters' `--native_X=false`
  fallbacks, ports that hand uncovered cases to `__imp__` from the start
  (some for cases that can occur in play: a full command list, another
  list's chunk, more than 4 textures; nothing logs when one fires), and
  guest calls from native code into still-recompiled callees (vtables,
  callbacks). To investigate later.
- The user's call: `_mtinit` `sub_825AA708` (the C runtime's
  per-thread data over KeTlsAlloc/KeTlsSetValue) left for the core;
  raised again only when it blocks a subsystem's ready work.
- `scene-models.md` (new, 9a490b3): 48 rows, all new: the cars as the
  scene holds them (pool 7 of the second pool set at `[0x826C2E44]`):
  `SceneModel::SetUp` (`sub_820DB458`, from a 4,244-byte record),
  `SceneModel::Build` (`sub_820D9870`, claiming parts in the other
  pools), `SceneModel::Update` (`sub_820DA180`), `ScenePools::SetUpB`
  (`sub_820C5B28`), the parts' links, the records' defaults and
  copies, the first set's loaders `car-models.md` lacked, the changed
  record's re-apply (`sub_820DA758`) and the decals' reload. Shared
  rows stay in `vehicle-accessors.md`, `car-models.md` and
  `race-overlays.md`. `known-data.md`: the car parts object, the scene
  model's fields, the 4,244-byte record, `0x826C5510` (probably the
  accessories' set), the sound slots at `0x826D8334`-`0x826D83AC` and
  their asset table `0x82616318` (probably the engine sound banks).
  `leads.md`: 6 leads (the load poll, `ScenePools::ReleaseB`'s ~80
  release cases, the scene models' owners, the sound slots' readers,
  the scene's update and teardown, the archive queue's append).
- `scene-models.md` migration 1 (8c076d0..88bdea2; `src/scene_models/`,
  `src/hooks/scene_models.cpp`, harness `src/harnesses/scene_models.cpp`):
  23 of 48 ours, `--native_scene_models`. For review:
  `sub_820C3CA0`'s VirtualAlloc through `memory::AllocPages` (the
  2026-10-22 call; native only under `--native_heap`); the originals'
  stack temporaries not written (the post effects' and track route's
  precedent); copies into the physical views, overlapping records or
  anything `c_runtime::Memcpy` refuses fall back to the original (the
  minimap's `List80::Copy` precedent); `sub_820D0A20` Native with no
  adapter (only `CarRecord::Reset` calls it).
- The "no mapping" lead, `UiMovie::DrawTexts` and what it unblocks
  (c4141e4..b8b7f8a; `src/ui_movies/texts.cpp`, `update.cpp`,
  `src/hooks/strings_and_font.h`): `DrawTexts`, `ScrollOffset`
  (Native, no adapter), `UiMovie::Update`, `Draw`, `Run` under the
  existing `--native_ui_movies_game` and `--native_ui_movies`;
  `_init_pointers` and the empty destructor `sub_82106540` under
  `--native_c_runtime`; 9 adapters removed (the text accessors,
  `Font::TextWidth`, `_init_pointers`' five setters). For review: two
  harness stand-in hooks (`hooks::g_text_draw_stand_in`,
  `ui_movies_hooks::g_update_callee_stand_in`) checked by the
  overrides; four removable adapters kept (`DrawTexts`, `sub_8259FA00`,
  `sub_8259DAC0`, `Update`: the `--log_pause_movie` watch and the frame
  pacing's replay and track handling live there); `_mtinit`
  (`sub_825AA708`) left, as it calls KeTlsAlloc/KeTlsSetValue (no
  precedent for TLS). A replay (`itc_1719`) logged the SDK's
  "BaseHeap::Release failed" twice at transitions, also in `itc_1528`
  (before this run), not in a re-run (`itc_1721`).
- The DrawTexts lead's migration 2 (81a1ede, dc97a73):
  `UiMovie::Finish`, `FinishNamed` (inside their `--log_pause_movie`
  overrides in `src/debug/pause_movie_log.cpp`; with the flag on the
  originals run, as `Loop` and `Play`) and `FinishAll`
  (`--native_ui_movies_game`); `race-hud.md` (new, 3 rows):
  `RaceHud::RunMovie` `sub_822141E8` (probably; Overridden),
  `sub_82212908` Merged into it (its `RaceTime::FromTicks` call left
  out: a pure leaf whose result nothing reads), the rival logo's draw
  `sub_82211A28` (moved from `functions.md`, Overridden; its
  `--redraw_rival_logo` override now calls the native draw;
  `frame-interpolation.md` noted); `--native_race_hud`. State 3's
  `sub_82212958` never ran: the original runs there, no row. Four
  removable adapters kept (the pause movie watch's three, called
  through `ctx`; `UiMovie::Draw`'s, all guest calls).
- The UI movies' ready groups (no mapping; 7d83fb5..ad1dbcb;
  `src/ui_movies/objects.cpp`, `effect.cpp`, harness
  `src/harnesses/ui_movie_objects.cpp`): 26 rows in `ui-movies-game.md`
  (the vectors, `UiMovie::UiMovie`, `~UiMovie`, the texts freed,
  `GetText`, the named animations' set-up, `sub_8211BEE8` named
  `RaceHud::DrawMirror` (probably), the six text setters over
  `GetText`) and a new intro paragraph; under `--native_ui_movies_game`
  (the allocating ones also need `--native_heap` and
  `--native_c_runtime_heap`). For review: each function hands a case it
  couldn't do natively to the original first, with nothing done (the
  destructor and texts' free check every block, none freed twice);
  `sub_8211E378`'s `std::string` calls abort with a log if one refuses
  after the checks (as `std_string`'s `Require`); `sub_8259EBE8`'s
  override in `src/hooks/ui_movies.cpp` also checks the harness stand-in
  hook.
- `scenery-lists.md` (new, a373941): 27 rows (3 moved from
  `functions.md`: `sub_825951E0`, `Recorder::RecordObject`
  `sub_825953D0`, `sub_82146B80`; 24 new): `ModelList::Load`,
  `LoadPoints` (the +3180 list's 20 point-group pointers at
  `0x82617750` + 256 × i), `Release`; the draw decisions
  (`sub_82147850` by the race data's +24 flags, `sub_82147958` by the
  course layout, `sub_82147B18` by race mode and course,
  `Race::CourseLayout` `sub_8228F408`); `PointList::Draw`,
  `SceneryModel::DrawShape` and `RecordShape`; the shared model draws
  (`Model::IsShapeVisible`, `Recorder::RecordSceneLights`,
  `Model::RecordShapeDepth`, `DrawShape`, `DrawShapeDepth`) and the
  texture provider's slots 2 and 4. `sub_82147A20` and `sub_8228F484`
  are pieces the recompiler split from their parents. Pointers in
  `course-scenery.md` and `sprites.md`; `known-data.md`: the draws'
  constants `0x8265ECA0`-`0x8265ED3F`, the vertex declaration table
  `0x8296B8B8`, the sort keys `0x8296B8FC`-`0x8296B90C`, the scene light
  constants `0x8296B950`-`0x8296BCDF`. Not read: two switch tables, and
  `SceneryModel::RecordShape` past its outline. 2 leads added.
- `scenery-lists.md` migration 1 (e3295c1..c96cd1b;
  `src/scenery_lists/`, `src/hooks/scenery_lists.cpp`): 24 rows in
  `scenery-lists.md` and 5 in `models.md` (`--native_scenery_lists`,
  `--native_models`). For review: `Recorder::RecordObject` records
  natively inside its interpolation wrapper (`blend_state.cpp`); native
  callers call `BlendState_BeforeRecordObject`/`AfterRecordObject`
  themselves with the wrapper's lr and r24-r31 through a new
  `BlendState_SetRecordContext` (frame dumps and `--log_blend_state`
  counts matched the fallback); the draws' adapters set flush-to-zero
  off at entry and after each guest call, as the originals do before
  their scalar float work (59d655c; the harness runs cases with it on);
  each recorder runs the original from the start when the command list
  would refuse a slot, another list's chunk is open or a command list
  diagnostic flag is on; left on the recompiled path (never ran): slot
  4's reserve, debug prints and resize, `sub_82589728`'s message path,
  `RecordShape`'s material animations and more than 4 textures;
  `RecordShape`'s adapter kept (guest calls to the model's and
  provider's slots).
- `scenery-lists.md` migration 2 (343035d): `Model::DrawShape`
  `sub_82596B18` (`--native_scenery_lists`) and `Model::RecordShape`
  `sub_82595550` (`models.md`, `--native_models`) in
  `src/scenery_lists/model_draws.cpp`, reusing the recorders' helpers.
  For review: the same declines as migration 1 (material animations,
  more than 4 textures, a vertex declaration it leaves to the original,
  diagnostics, another list's chunk, no room) run the original from the
  start; its interpolation record context (`0x82596BA0`, the queue
  calls' return addresses) as the original's; the harness turns
  `--native_models` off on the recompiled side, compares
  `IsShapeVisible`'s product only where no recompiled `RecordShape` ran
  (its register saves below the stack pointer aren't copied), and sets
  the fake model's +364 to 0; both adapters kept (guest calls through
  `ctx`).
- `shader-objects.md` (new, 4047496): 7 rows: `VertexShader::Create`
  `sub_822AA018`, `PixelShader::Create` `sub_822A9EE8`,
  `VertexShader::Init` `sub_822A8740` (up to 32 12-byte entries,
  probably the vertex fetches), `PixelShader::Init` `sub_822A85B8`, the
  memory blocks' `Memory::AllocVirtual` `sub_8258DFF0` (kind 0),
  `AllocPhysicalEx` `sub_8258E098` (kind 1) and `PendingFrees::FreeDue`
  `sub_8258E1D8`; `c-runtime.md`: `XMemCpy` `sub_82295890` (probably)
  and its unaligned 128-byte line copy `sub_82296FE0`. `known-data.md`:
  `[0x82638308]` (read only by `XMemCpy`'s byte loop, never stored,
  probably 1: to confirm by a log, as `XMemSet`'s step was), the memory
  block's kinds and makers. `models.md`: `sub_822A9FD8`, `sub_822A9EA8`
  read in full. `leads.md`: the ScopedLock users' lead trimmed to
  `sub_82593E78`; 1 lead added (`XMemCpy`'s other callers).
- `shader-objects.md` migration 1 (7bc02b9..840ecff;
  `--native_shader_objects`, with `XMemCpy` under `--native_c_runtime`
  and `models.md`'s chain up to `Model::Load` under `--native_models`).
  For review: a new `native::Buffers_Memcpy` split out of memcpy's
  override in `src/native/buffers.cpp`, so native callers' copies into
  physical memory (microcode, vertex data, XPR data) are recorded for
  the renderer as the game's memcpy's are; where a callee's plain C++
  declines (never seen in play), `Model::Load`, `Mesh::LoadShaders` and
  `VertexShader::Create` call that callee's original as a guest call
  (as `scenery_lists`' `ReleaseCalls`); the game's debug prints in these
  functions are warnings in our log (`Mesh::Parse`'s without its "Version
  Error" text, which the harness counted as an error line); correction
  to the map: `Resource::Relocate`'s type 6 does nothing;
  `0x82638308` logged as 1 (`XMemCpy` does one host copy then, else
  replays the byte loop).
- The shader objects' migration 2 (800b20c..49bad70): `DataFile::GetEntry`
  (`files-and-saves.md`, new flag `--native_data_files`,
  `src/game_data/data_file.cpp`); `ModelList::Load`, `LoadPoints`
  (`scenery-lists.md`, Native); 12 rows in `course-scenery.md` (the
  course set's parse, the areas' lights and models parsed and their
  callbacks, `Scenery::MakePoolObject` with the constructors, the
  resets, `AreaModel::~Model`); `Strings::Get` and a slot's
  `XPR::Release` (`strings-and-font.md`). For review: everything that
  makes or frees models runs natively only where `Model::Load`'s adapter
  does (`hooks::ModelsNativeLifetime()` and `hooks::ModelLoadGuest`
  exported from `src/hooks/models.h`); the attract race has two
  variants (2,412 or 2,414 draws in a dump) by timing, so dumps compare
  only within one.
- The shader objects' migration 3 (220ea7f..a5141f5): `course-scenery.md`:
  `Scenery::CollectAreas`, `CollectModels`, `DrawSetLists` (new
  `src/course_scenery/collect.cpp`, strict floats; `DrawSetLists`'
  harness in `src/harnesses/scenery_lists.cpp`); `strings-and-font.md`:
  `Strings::FormatV` (its 0xFFD2 case's `sub_82108A70`, the font's
  number, never seen, left a guest call) and `Strings::Format`; 8
  adapters removed (`SceneryList::Add`, `PoolObject::PoolObject`,
  `ModelList::Init`, `Strings::Get`, `FormatGrouped`, `PageCount`,
  `ModelList::IsBEntryOn`, `IsCEntryOn` now Native). For review: new
  exported helpers (`hooks::SceneryDrawGuest`, `SceneryListsDrawCalls`,
  `SceneryListsSlotDiagnostics`, `CourseSceneryDrawSetLists`,
  `StringsFormatVSteps`, `StringsFormatSteps`).

## Run 2026-10-25 03:59 (`b7-loop-25-10-26-2`)

- `game-modes.md` (new, 7871fe6): 31 rows (3 moved from
  `functions.md`, 28 new): the game's modes (`Mode::Create`,
  `Modes::Tick`), the race mode (type 1, 167,264 bytes; constructor
  `sub_821073B8`, destructor `sub_82107518`), its message dispatcher and
  state machines (`StateMachine::Handle`, `Find`, `IndexOf`), the race
  overlays' state machine set-up `sub_8215E6C8` (33 transitions listed)
  and its four handlers, and the race-only `Buffer28::` functions in the
  camera modes' range. `known-data.md`: the mode, race mode, state
  machine and transition layouts, `0x826032DC`/`0x826032E0`,
  `0x826C5B68`; the race flow's row now says it's the race mode's
  +97548. `leads.md`: 2 leads added, notes on the camera modes and
  framework objects leads.
- `game-modes.md` migration 1 (21ff7d5..c504e59; `src/game_modes/`,
  `src/hooks/game_modes.cpp`, harness `src/harnesses/game_modes.cpp`):
  21 of 31 ours, `--native_game_modes`. For review:
  - `RaceOverlaysMachine::Init` (`sub_8215E6C8`) replays the original's
    377 stores and copies in the same stack frame (`r1 - 2272`, a table
    read off the generated code), so the transitions get the same three
    unwritten functor storage words (stack leftovers; nothing reads
    them; as track zones did).
  - `StateMachine::Find`'s default is returned as
    `0xFFFFFFFF826C5B68` in r3 (sign-extended, as the original).
  - `Transition::Make` (`sub_82105FE0`) and `Transition::Assign`
    (`sub_82106100`) are Merged into `StateMachine::Add`; their own
    harness cases went with them (Add's cover them).
  - New names: `RaceOverlaysMachine::SetFlags`, `ClearFlags`,
    `SetFlag820`, `ResetOverlays`, `Member67760::Member67760`,
    `Member13360::Member13360`, `Buffers28::~Buffers28`.
- `models.md` (f28c6f7): 16 new rows for the model class
  (`Model::Model`, `~Model`, `Model::Release` read), the texture
  provider and its `std::vector` helpers, `XPR::~XPR`, `XPR::Release`,
  the put-off frees (`Memory::FreeLater`, `FreePhysicalLater`, the
  pending frees list at `0x8296BD54`, freed by `sub_8258E1D8` each
  pass); `memmove_s` in `c-runtime.md`. `known-data.md`: the texture
  provider, the `XLMM` block header, `0x8296BD38`, `0x8296BD54`.
  `leads.md`: one lead (`sub_820C5008`/`sub_820C5168`), notes on three.
  The map stopped on the memory blocks' lock; the user's call
  (2026-10-25): port it too (a host `std::mutex`).
- `models.md` migration 1 (6148c1e..b35547a; `src/models/`,
  `src/hooks/models.cpp`, harness `src/harnesses/models.cpp`): all 17
  rows ours, `--native_models` (`memmove_s` under `--native_c_runtime`).
  For review:
  - The memory blocks' lock is a `std::recursive_mutex`, not a plain
    `std::mutex`: the critical section it replaces is recursive, and the
    harness holds it around its cases on the game's own pending-frees
    list while the functions take it again. No user in play takes it
    twice.
  - Every user of `0x8296BD38` takes it: `FreeLater` and
    `FreePhysicalLater` directly; the recompiled ones (`sub_8258DFF0`,
    `sub_8258E098`, `sub_8258E138`, `sub_8258E1D8`) through `ScopedLock`
    (`sub_8258E580`, `sub_8258E5B8`, `video-output.md`), overridden to
    take the host lock for this critical section only (every other one
    goes to `__imp__`). Host lock or guest critical section is chosen
    once, from `--native_models` at the first call.
  - New read-only helpers in the model animation code for
    `Model::~Model`'s checks: `TrackSetReleasable`,
    `SkeletonDestructible`.
  - An `UninitializedCopy` harness case with a null destination was
    fixed in the case (the original checks null per slot and writes
    from address 4), noted in the row.
- `game-modes.md` cleanup (no mapping; ce8d477..4ff0b54): 38 of 42
  ours. For review:
  - 11 new rows read on the way: `Mode::Create`'s mode constructors
    (types 2-7, 9, 10, 12) and `MachineMember::MachineMember`
    (`sub_82108160`); `sub_821077EC`, a piece of `Mode::Create`'s switch
    in its stack frame, Merged into it; type 8 (`sub_82107670`, never
    seen in play) still runs the original.
  - `sub_82106A50`, `sub_82107168`, `sub_82108160` and the mode
    constructors are Native with no adapter (only native code calls
    them).
  - `race-overlays.md`'s `RaceOverlays::Clear` (`sub_8215DE80`) adapter
    removed (RaceMode::RaceMode its only caller in play).
  - The pending-frees harness helpers moved to
    `src/harnesses/pending_frees.h`, shared by the models and game modes
    harnesses.
  - The first batch's two groups in one commit (ce8d477); one counter
    race for the six removals, each then built and committed alone.
- `mode-tick.md` (new, 2a11eec): 23 rows, what `Modes::Tick` calls
  besides the mode's slots: the per-mode settings (`Modes::SetChecks`,
  `SetSceneTargetMode`, `Presence::SetForMode`, presence probably
  Dropped at `Modes::Tick` by B5's call), the sign-in watch
  (`SignInWatch::Update`, `CheckIdle`, `OnModeChange`; its nine prompt
  handlers never ran, no rows), `Billboards::FlipRing`, the screen fades
  (two layers at `0x826A50AC`), `Music::Play` (`BGM_SYSTEM`,
  `bgm_tmp_01`-`17`). `game-modes.md`: `Modes::Modes` (`sub_821067F8`)
  added. `known-data.md`: their fields and globals. `leads.md`: the save
  data's creation and deletion. The lead's "loading screen" guess was
  wrong. Also: `models.md`'s `TextureProvider::Clear` row reworded ("it
  never reaches `memmove_s`") so `b7_status.py` doesn't read it as never
  called.
- `mode-tick.md` migration 1 (4975324..1897994; `src/mode_tick/`,
  `src/hooks/mode_tick.cpp`, harness `src/harnesses/mode_tick.cpp`):
  21 of 23 ours, `--native_mode_tick`; `Modes::Modes` under
  `--native_game_modes`. For review:
  - `Presence::SetForMode` Dropped (B5's presence call): no presence
    set; only what's left once its wrapper `sub_82294F70` is dropped (the
    last error cleared, r3 = 0) is kept, as the new mode's slot 0 after
    it isn't read. `check_function_statuses.py` lists it under "to
    check" for its fallback, as device-state's.
  - `lstrcmpA` (`sub_82296130`) and `CompareStringA` (`sub_82297100`),
    XAPI's, still recompiled with no rows: made plain C++ in
    `src/helpers/compare_string.{h,cpp}` by B6's call for the pure
    string helpers (as `MultiByteToWideChar`, and XAPI's memset in B7);
    the agent asks the user to confirm that call reaches them. 2,000 and
    1,000 harness cases, 0 differing.
  - The sign-in watch's prompt states 1-9 (never seen in play) and the
    trap paths run the original.
  - Stack copies (the stage copy, the player's name, the color
    rectangle, the music table) written into the original's frames; the
    scene getters and render-queue submits as guest calls from frames
    like the original's, as the cameras do.
  - Map corrections: `ColorRect::Record` has one caller
    (`ScreenFades::Update`); `CompareStringA` read in full.
- `minimap.md` (new, f86ea54): 15 rows (`Minimap::Draw` moved from
  `functions.md`, 14 new): the map's models, the point markers, the car
  markers (an arrow in five colors by type: the player, a human or
  rival-less racer, three rivals by player 0's save), the five stack
  pools and the blink phase (`0x826C5AA8`, stepping every five calls);
  `Minimap::Draw` stays Wrapped by `--per_tick_minimap_blink`;
  `sub_820F2E68` is case 1 of `Minimap::RacerMarkerType`'s switch (no
  row, ported with it). Also: `matrix-math.md`'s `sub_8209B038` (the
  minimap forms its address for the marker pools), `course-scenery.md`'s
  intro, a note on the course set's lists lead, `known-data.md` (the
  blink clock, the colors, the marker triangles, `0x826C7824`, the race
  manager's 28-byte entries, the course set's +1064 and +3180 lists).
- `minimap.md` migration 1 (3d8e197, 20d9037; `src/minimap/`,
  `src/hooks/minimap.cpp`, harness `src/harnesses/minimap.cpp`): 12 of
  15 ours, `--native_minimap`. For review:
  - Interpolation kept: `Primitive::RecordTransform` makes its product
    where the original's call puts it and calls `BlendState_OnMultiply`
    with return address 0x8217AEDC; `List80::Insert` calls
    `BlendState_OnCopy` with 0x820F529C; `Minimap::Draw` (still
    recompiled) keeps its `--per_tick_minimap_blink` wrap.
  - To the original: with the command list's diagnostics on, an open
    chunk or a full list; `sub_820F2CA8`'s branch (never seen in play;
    its harness cases left out: on made-up data its chained records
    recursed until the stack overflowed); `Triangle::Record`'s kind 1
    (`sub_8217C4C0`, never seen in play).
  - `Triangle::RecordFlat`'s render-queue submit a guest call from a
    128-byte frame, as `ColorRect::RecordFlat`'s.
  - `List80::Link` and `Triangle::RecordFlat` Native with no adapter.
- The `atexit` step (no mapping, counted as subsystem 5 by the user's
  call; 19f9deb..1244dd3): `atexit` and `_onexit` native in
  `src/c_runtime/exit.cpp` under `--native_c_runtime`, then its callers
  under their own flags. For review:
  - The exit lock (the C runtime's lock 8, critical section
    `0x82971248`) is one host `std::recursive_mutex`: `_onexit` takes it
    directly; `_lock`/`_unlock` (`sub_825B3430`, `sub_825B32D0`) are
    overridden to take it for lock 8 only (r3 left as the originals
    leave it), every other number the game's; host or guest chosen once
    at boot. Recursive because `doexit` holds it while the registered
    functions run, which may call `atexit`. Nothing else touches entry 8
    (made at boot; the lazy creator and the teardown don't).
  - New rows in `c-runtime.md` for the lock wrappers and the exit path
    (`sub_825AA218`, `sub_825AA318`, `sub_825AA338`, never seen in
    play).
  - Native callers register through `c_runtime::AtexitFrom`; the game's
    `atexit` through `ctx` only where the native one can't run (never in
    play).
  - `std-string.md` corrected: the global strings' static set-ups'
    addresses were misread (`0x8265EE7C`/`0x8265EE98`, clean-ups
    `0x825DB328`/`0x825DB380`); the harness caught it.
  - `UiMovie::TrackProperty`'s two stack-leftover bytes read from the
    original's frame.
  - The minimap's marker draws: the frame dump draws the same car marker
    as the fallback, not pixel-identical only because the two dumps were
    taken at different moments (the map shifted).
  - Kept: `_lock`/`_unlock`'s adapters (they keep the exit lock's users
    together) and `Triangle::Record`'s (a guest call through `ctx`).
- The `atexit` step's migration 2 (8a0dd9e..9e046a9): for review:
  - `UiMovie::ApplyTrack` (`sub_825A0548`, Wrapped -> Overridden):
    plain C++ in `src/ui_movies/keys.cpp`; frame pacing's override in
    `per_frame_logic.cpp` keeps its per-tick skips and calls it; the two
    cubic key functions where the native ones give `nullopt`, and type
    2's cubic and eased keys (never seen in play), go to the originals
    through `ctx` from a frame like the original's; the stand-in first
    key written in the original's frame.
  - `UiMovie::ApplyAnimation` (`sub_825A08E0`): plain C++ in the new
    `src/ui_movies/animation.cpp` (strict float settings); the
    original's key search kept, quirk included (the next key compared
    against the animation's time, not the track's local time);
    sub-animations recursed 224 bytes lower a level; each track through
    ApplyTrack's override, so the per-tick skips still apply. Its
    harness runs the recompiled side with `--native_ui_movies` off.
  - `__onexitinit` (probably; `sub_825A6F00`): new row in
    `c-runtime.md`, over the native calloc.
  - 11 adapters removed (wcscmp, TrackProperty, the key functions, the
    UI movies' accessors).

## Run 2026-10-25 01:17 (`by-loop-25-10-26`)

- `framework.md` (mapped in c4b5b86, last run; its rows are in that
  run's section below): map checked this run (0 errors, 68 of 68 in
  play). Note: `b7_status.py` flags `movies.md:108` `sub_82108018`, whose
  row says never called but which ran (the user's movie work, not this
  run's).
- `framework.md` migration 1 (f27748b..e080edd; `src/framework/`,
  `src/hooks/framework.cpp`, harness `src/harnesses/framework.cpp`):
  50 of 68 ours, 15 adapters removed (Native), `--native_framework`.
  For review:
  - The map's rotations were swapped: `sub_82092B10` is
    `HandlerMap::RotateLeft`, `sub_82092A00` `RotateRight`; corrected.
  - Paths handed to the original (never seen in play): InsertN's
    copy_backward path (316 Insert/InsertN cases go there, logged) and
    `ApplyRequest`'s removal types 1 and 3.
  - `TickHandler::Invoke` (`sub_82099A98`) and `sub_82096340`, Wrapped
    before: their existing overrides (`game_messages.cpp`,
    `frame_state_diff.cpp`) now call the native path (Overridden).
  - `DebugText::DebugText` stores an uninitialized word of its own stack
    frame as the job's state; the adapter reads and stores the same word.
  - `sub_82094350`'s adapter stays: `sub_820A8508` calls it through a
    vtable.
  - Not compared in the harnesses: InsertN's r3, r4-r11 after
    `Framework::Framework` and `Create`, a map head's key and delegate,
    the nodes' padding bytes +22/+23 (nothing sets or reads them).
- `framework.md` migration 2 (eb25d68, ad01ea7; `src/framework/main_loop.cpp`):
  the main group native (`main`, `sub_82090378`, `sub_8209ABE8`,
  `App::Run`, `ForEachTickHandler`, `ForEachRequest`), 7 adapters removed;
  56 of 68 ours, nothing ready left. For review:
  - Every virtual call and functor stays a guest call, from frames like
    the originals' (their return addresses and stack words); tick
    handlers still go through `TickHandler::Invoke`'s override.
  - The native loop sets r26 to r31 as the recompiled `App::Run` keeps
    them, and puts them back on return: a recompiled device setter reads
    r27's high word (`Device::SetDefaultState`'s harness differed without).
  - The originals' traps (tw 22) log a warning and go on, as the
    recompiled code does; none seen in play.
  - `main`'s harness makes the read-only page of its framework vtable
    (`0x82000B90`) writable through the SDK's `rex::memory::Protect` for
    its test slots, then puts the access back.
  - `FromFvf`'s and `SetFVF`'s pad byte groups now differ (400, 100):
    `expected-differences.md`.

## Run 2026-10-24 21:33 (`b7-loop-24-10-26-4`)

- Cleanup lead `sub_8209F430` ActionMappers::Init (`action-mappers.md`;
  3ac7be7, 9e36924): plain C++ `InitCreate`
  (`src/action_mappers/action_mappers.cpp`), under
  `--native_action_mappers` (needs `--native_heap`); 24 cases in the
  `action_mappers_binding` harness. 7 adapters removed (Native:
  `sub_8259C868`, `sub_8259C898`, `sub_8259C818`, `sub_8209F228`,
  `sub_8210AEB0`, `sub_8210AF00`, `sub_8210C180`). For review:
  - Init takes its five blocks from operator new first, in the
    original's order; if one isn't given, it frees them all and runs the
    original.
  - Its adapter runs BindAll's adapter inside the original's 112-byte
    frame; BindAll stays Overridden (a guest call to the owner's slot
    +36: `sub_820944C8` -> `sub_82094350` -> probably `sub_82090788`,
    the app's, unmapped).
  - The harness cases call Init's adapter (it makes guest calls), and
    give both versions the same operator new blocks with the same
    random leftovers.
  - Kept though `--removable` lists them: `sub_8210C370`,
    `sub_8258CF10`, `sub_825924E0` (the fallback's `--log_pause_menu`
    and `--log_input_reads` diagnostics), `sub_820991E0` and
    `sub_8210AF08` (guest calls through `ctx`).
- Three cleanup leads (2e5ce7c..b6888ac; `race-overlays.md`,
  `ai-drivers.md`, `c-runtime.md`):
  - `sub_8212BFE8` ParticlesB::ParticlesB and the deleting destructors
    `sub_8212B358`, `sub_8212C070` Overridden; `sub_8212B288`,
    `sub_8212BF88` Native from the start (only the native deleting
    destructors call them). The deleting destructors run natively only
    with `--native_heap`, and fall back to the original (bit 0 set) for
    a block the allocation layer didn't give. They're called from the
    recompiled CarOverlays::DeleteChildren at a race's end, so attract
    races don't reach them.
  - `sub_8213BDF0`, `sub_8213BE80` (JoinCornersByExit, ByEntry; never
    seen in play, no rows) native inside AiPath::JoinCornerPair;
    PathCalls' join callback gone. Like the other native joins, they
    don't write the original's fcfid scratch below the stack pointer.
    What's left of PathCalls is the +5532 normalizer's other targets
    (never in the game), so UpdateIdle's (`sub_821399A8`, listed
    removable) and DriveCar's adapters stay.
  - Flush-to-zero: the problem was real. ThinLTO had moved
    Smoke::InRegion's second `dpps` past the control word's restore: a
    new "tiny values" group had 135 of 400 differing. Fixed as in
    ee810a9 (a `Flushing()` helper, `vector_library::PinInMode`
    barriers); 0 differing since. The barriers changed Sparks::Emit's
    `mulps` operand order (14 NaN-only differences), so it now picks
    the kept NaN explicitly (`MulKeepingFirstNaN`); Sparks::UpdateStreak
    still relies on a plain `addps` that happens to match.
  - A latent harness input: with the shared random stream shifted,
    AiDriver::DriveCar's case 540 made the recompiled side fault without
    end (guest `0x7028C0CC`; `itc_1497`, `itc_1498`). The new groups
    were put last so earlier inputs stay as before, but DriveCar's case
    generation can make an input the original faults on.
  - `sub_82138D00` SetParams, `sub_82139DA0` StartRace,
    `sub_82139580` ResetControl: adapters removed (Native).
- `screen-objects.md` (new, mapped in 78f986b): 42 rows (1 moved from
  `functions.md`: `sub_82130978`, now `ScreenObject::Record`, Wrapped),
  the race's screen object (128,000 bytes, one a view: a 1024-wide
  A8R8G8B8 texture, a 720-triangle fan with `rand`-jittered UVs and 20
  rings of 36 points; strength = speed km/h / 300 at +76; probably a
  screen blur while the nitrous runs, started and stopped by the race's
  update from the vehicle's byte +2424). `Start` and `Stop`
  (`sub_8212EDF8`, `sub_8212EED0`) never ran, so `Record`'s 1,038-line
  body never ran either. For review:
  - Names on guesses: the `ScreenObject::` functions, `UvMap::` and
    `UvColumn::` (the two map types), the nitrous trigger, +84 as the
    sort key (599/601).
  - Correction: the lead said `std::string`-keyed trees; the keys are
    doubles (the string is only the `length_error` message).
  - `sub_82131680` (`UvColumn::Insert`) and `sub_82131FB0`
    (`UvMap::Insert`) mapped here, not under the containers' lead (only
    the screen objects use these two map types).
  - `post-effects.md`'s intro and `sub_8215B690`, `sub_8212EDE0` rows
    now point here; `known-data.md` gets the screen object and the race
    object's +31152/+31156/+31160.
  - Left open: the objects at +88 and +92 (set through vtable calls in
    `Record`; who makes them isn't known).
  Migration 1 (472d5ac..4e3513e): 38 of 42 native under
  `--native_screen_objects` (`src/screen_objects/`, adapters in
  `src/hooks/screen_objects.cpp`, harness `screen_objects`; the tree
  functions also need `--native_heap`, `~ScreenObject`
  `--native_video_output`), 8 adapters removed. For review:
  - `sub_8212EEF8` (UvColumn's iterator decrement, never seen in play,
    no row) ported so the native InsertUnique has no recompiled callee;
    its own harness group.
  - Handed to the original from the start: the trees' `bad_alloc` and
    `length_error`; erasing part of a tree (`sub_8212F030`,
    `sub_8212EFA8`, `sub_82132318`, `sub_821318B8`, never ran); a
    checked iterator's trap; SetSpeed while +24 is set (calls
    `sub_8212F778`, never ran); BuildRings over a map that isn't empty.
  - Inside a native caller, an operator new or delete refusal after the
    checks, or a broken tree, is logged and aborts (as
    `std_list/ptr_list.cpp`).
  - AddRingPoint's r3 set to what the original leaves (no caller reads
    it), compared only by kind for the freed head; its temporaries live
    in the original's 272-byte guest stack frame.
  - Two corrected notes: both AllocateNodes rows said 0 returns 0 (it
    calls operator new(0)).
  - Left: `rand` (`sub_825A9BB8`, the core's thread data) blocks
    BuildFan, Init and the constructor; `sub_822A8980` and the shader
    pairs not taken (they unblock nothing yet); `Record`'s body never
    ran, so no frame dump was compared.
- `framework.md` (new, mapped in c4b5b86): 68 rows (5 moved from
  `functions.md`: `main` `sub_82090108`, `App::Run` `sub_8209A760`,
  `ForEachTickHandler` `sub_82099D50`, `TickHandler::Invoke`
  `sub_82099A98`, `sub_82096340`), the app's framework (its services
  made through factories at boot, the run loop and its tick handlers);
  `known-data.md`'s framework, run loop, app (partial), `0x8296B910`,
  `0x8296B914`, `0x8266D080`, `0x8296BCE0`; 5 leads added. For review:
  - BindAll's slot +36 worked out: `sub_820944C8` -> `sub_82094350` ->
    `sub_82090830` -> `sub_82090778`, the input manager at framework+88
    (natively `[[owner + 8] + 52]`), not `sub_82090788` as the lead
    guessed (that's framework+124, the save devices); corrected in
    `action-mappers.md` and `input.md`.
  - Names on guesses: `Framework::`, `RunLoop::`, `HandlerMap::`
    (probably `std::map<unsigned, Delegate*>`), `RequestVector::`
    (probably `std::vector`), `FrameStart`, `DebugText`,
    `DebugFont::SetPosition`, `PoolMaker::PoolMaker`, the factories
    (`Users::`, `SaveDevices::`, `WorkerPool::`, `Renderer::`,
    `Scene::`, `App::Create`).
  - Corrections: `App::Run`'s "quit flag at this+12" is the tick
    handler map's size; `TickHandler::Invoke`'s r3 is the run loop;
    `system-services.md`'s "app" (`App::ReadUsers`, `App::PollSignIn`)
    is the users and sign-in object at framework+128 (noted in
    `framework.md` only, not renamed).
  - Taken from other leads: `sub_82093400`, `sub_82093740` (the
    containers'), `sub_820944C8`, `sub_82094350` (the app's start-up);
    both lines updated.

## Run 2026-10-24 17:46 (`b7-loop-24-10-26-3`) (reviewed 2026-10-24)

- `post-effects.md` (new, mapped in f3643e1): 57 rows (3 moved from
  `functions.md`: `Glow::Record`, `PostEffects::RecordAll`
  (`sub_82133960`, unnamed before), `sub_821363F0`), the post effects
  (up to 15 screen-space effects a view, objects at `0x826C5D80` made
  from the table at `0x826174A8`: the glow, probably a color filter, the
  motion blur, a two-pass filter over render target 3 or 4, two kinds
  never run); `known-data.md`'s "Values from the image" (the three
  `.data` tables, float constants); 3 leads. For review:
  - `sub_821363F0` renamed from `View::UploadPreviousViewProjection` to
    `MotionBlur::Record` (it records the whole motion blur), and the
    mention in `matrix-math.md`'s `Matrix::Copy` row changed to match.
  - `sub_8212EDE0` belongs to the race's screen objects (made by
    `sub_82133000`, now a lead), kept here beside its caller
    `sub_8215B690` (probably the pause toggle).
  - The glow's tables at `0x82617280` and `0x826171D8` probably the
    DirectX SDK HDR sample's glare library (`GLAREDEF`/`STARDEF`).
  - `RenderTextures::Acquire` (`sub_82133190`) returns an uninitialized
    stack word when an id is in use with another key (split screen's
    second player, probably; not checked whether it runs).
  Migration 1 (ece3c32..66b8f81): 43 functions native under
  `--native_post_effects` (`src/post_effects/`, adapters in
  `src/hooks/post_effects.cpp`, harness `post_effects`), 9 adapters
  removed. For review:
  - Stack leftovers below the stack pointer not reproduced
    (RecordTexelSize's c32, StarDef::Init's colors, AddQuad's integer
    conversions); nothing reads them.
  - The recording helpers check for room first; with the list full the
    whole original runs (its message and write through address 0); the
    command list's diagnostics (`--log_slot_callers`) skipped, as in
    earlier native recorders.
  - RecordBeginStates and RecordEndStates allocate one slot more than
    they write (24 and 13), as the original.
  - Left on the recompiled code behind fallbacks: the release paths and
    deleting destructors when a block isn't the allocation layer's or
    `--native_c_runtime_heap`/`--native_heap` is off; ReleaseSurface
    without `--native_video_output`; TargetFilter::TargetFilter where
    memcpy's copy can't be native or without
    `--native_graphics_resources`/`--native_heap`.
  - Frame interpolation's overrides untouched (`Glow::Record`, the
    motion blur's copy at 8213653C, `PostEffects::RecordAll`).
  - `RenderTextures::Acquire` not ported (blocked by `sub_8235A9F8`):
    its uninitialized-word path is still the original's.
  - The destructors, release paths and DestroyRace run only when a race
    ends: attract races don't reach them (harness only).
  - New helpers in `src/hooks/post_effects.h`: `PostEffectsReleaseTexture`,
    `PostEffectsReleaseSurface`, `PostEffectsDestroyCalls`.
- Cleanup leads (no mapping; 1197f81..cdaa66d): `sub_820BAAA0`
  CollisionShape::~CollisionShape's adapter removed (`collision.md`);
  `AiDrivers::Destroy` and `DestroyRings` native under
  `--native_ai_drivers` (`ai-drivers.md`; fall back while
  `--native_heap` is off or the table isn't the layer's; run only when a
  race ends: harness only); the UI movie walk native under
  `--native_ui_movies` (`ui-movies.md`, a new Merged row `sub_8259D730`
  the clip intersection, never in play, ported into ApplyTransform;
  `src/ui_movies/draw.cpp`, strict fp). For review:
  - `UiMovie::Draw` stays Overridden (its work uses `ctx`, inside the
    `--log_pause_movie` watch override); DrawPicture's and
    DrawPrimitive's adapters stay (a guest call to `sub_8259EBE8`)
    though `--removable` lists them; DrawNode reaches them through a
    callback from the original's 272-byte frames.
  - Frame interpolation kept: every Matrix::Multiply and Copy in the
    walk noted through `BlendState_OnMultiply`/`OnCopy` with the
    original's return address; Vec4::Transform through
    `debug::Vec4Transform`. Frame dumps match `--native_ui_movies=false`
    (2,414 draws).
  - Left on the recompiled code: the text list's grow `sub_8259EC58`
    (never in play: a full list runs the original append or QueueText);
    a picture without a texture runs the original DrawPicture.
  - Float words copied bit for bit where the original pairs lfs/stfs
    (a signaling NaN keeps its bits).
  - The DrawPicture, DrawPrimitive and tree harnesses compare bytes
    exactly instead of with `harness::Outputs`.
  - Found along the way: `sub_8210A758` Text::Draw's harness compares a
    slot word as bytes outside the vertex ranges it marks as floats
    (a NaN sign showed as 1 difference when its random inputs changed);
    the new cases now use their own `Random`, Text::Draw back to 0. A
    gap in that harness worth a later look.
- `input.md` (new, mapped in 167d407): 29 rows, the input manager and
  input processors (B1's 13 input rows moved from `functions.md`, kept
  with "B1's native code" or "Read (2026-10-24)" added; four unnamed
  ones named `AnyPressed`, `PressedOrRepeat`, `TestPressed`,
  `TestRepeat`, the getters `LogicalPad::Values` and
  `PhysicalPad::Values`); 1 lead (the app framework). For review:
  - The manager's vtable `0x82000F48` read from the image: slots 1
    `GetProcessor`, 2 `GetLogicalPad`, 3 `AddListener`, 5
    `GetPhysicalPad` in play.
  - Four `std::list` of pointers helpers mapped here as `PtrList::`
    (`sub_820AD058`, `sub_82099C08`, `sub_82095958`, `sub_820958B8`;
    the last taken from the C++ containers lead, which says so); shared
    with other subsystems (the sound manager).
  - Correction to the action mappers' map: the object
    `ActionMappers::Init` is given is probably the app object from
    `sub_82106548` (its slot +36 gives the input manager), not the input
    manager (`known-data.md`'s `0x826BAB78` row, `input.md`'s intro;
    `action-mappers.md`'s rows unchanged, its pointers moved to
    `input.md`).
  - The processor's constructor sets the repeat period to 4; the
    mappers' repeat objects use 3 (both as read).
  Migration 1 (57373be..4dd7a11): all 29 native under `--native_input`
  (`src/input/input_manager.*`, `src/std_list/ptr_list.*` for other
  subsystems to call, adapters in `src/hooks/input_manager.cpp`), with
  `ActionMapper::ActionMapper`, `ActionMapper::Bind` and
  `InputClientList::BindAll` (`action-mappers.md`, under
  `--native_action_mappers`); 16 adapters removed. For review:
  - A harness depends on the heap's layout: with the binding cases in
    the `action_mappers` harness, the later `strings_and_font` harness
    failed (`Font::Draw` differed, then its swprintf cases crashed on a
    read of guest 0x5E44, or a call to 0x00200020), and the race didn't
    start; one `memory::Alloc(0x4400)` and its `Free` before it is
    enough. Probably the same as the eleventh run's swprintf crash with
    other random inputs (above). Not investigated; worked around by
    putting the binding cases in their own harness,
    `action_mappers_binding`, run last.
    Looked into (the user's call, 2026-10-24): not the heap's layout
    (an Alloc and Free moved nothing) but the harnesses' one shared
    random generator: the binding cases drew from it, and later
    harnesses got other inputs. The crash was `swprintf`'s format
    generator (a bare "I" then '%' starts a new conversion; the original
    and the native agree), the differences NaN signs in vertices the
    drawing cases compared as bytes (clipped glyphs'). Fixed in the
    harnesses (`strings-and-font.md`'s intro): each harness starts its
    own generator, seeded from its name; `action_mappers_binding` runs
    after `action_mappers` again; 12 other seeds 0 differing.
  - Kept as guest calls (the functions stay Overridden): the manager's
    constructor's two handler registrations through the dispatcher's
    slots +8 and +4 (from the original's 192-byte frame); `BindAll`'s
    app slot +36 (logged: `sub_820944C8`, through `sub_82094350`,
    probably to `sub_82090788`; the input manager each time, players
    0-3).
  - Left on the recompiled code: `AddListener` on a list with a listener
    already (its reset `sub_82592400` never runs); `AllocateNodes`'
    `bad_alloc` and `IncSize`'s `length_error` paths (the latter doesn't
    throw in the original: a case past the limit crashed in it, so the
    cases stop at the limit); the constructor and Bind with vtable slots
    other than those seen in play; anything allocating with
    `--native_heap` off.
  - The `--log_input_reads` wrappers moved from
    `src/frame_pacing/game_messages.cpp` into the new adapters (same
    counts).

## Run 2026-10-24 16:01 (`b7-loop-24-10-26-2`)

- `c-runtime.md`: 2 new rows (mapped in 3a15cda), `sub_820911A8` and
  `sub_820900C0`, probably the compiler's `vector constructor iterator`
  and `vector destructor iterator` (no exception handling, so not the
  library's `__ehvec_*`), added to the C runtime file instead of a new
  one, with an intro paragraph; `known-data.md` (the race data's
  +0x59AEC records, `0x826D82D4`); 2 leads (the race object at
  `[0x826D82D4]`, `sub_8217C708` with `sub_821DF350`) and the race data
  lead extended with `sub_82101D20`.
  Migration 1 (f624a82, 93e739f): both helpers native under
  `--native_c_runtime` (`src/c_runtime/arrays.cpp`). For review: the
  native targets (`SetWsToOne`, `Particle::Particle`) called as plain
  C++, the empty destructor `sub_82106540` skipped, any other target
  (`sub_82101D20` in play) a guest call through an `ElementCall`
  callback from a 128-byte frame like the original's; r3 left as it
  came with the empty destructor (volatile, no caller reads it); a count
  of 0x80000000 wraps to 2^31 calls in both, left out of the harness.
  Adapters removed: wcslen, mbtowc, the wide write_char (Native);
  memcpy_s's (`sub_825A7270`) kept, its work a guest call to memcpy's
  override. To look at later: the new cases first drew from the shared
  `h.random`, which shifted later harnesses' inputs, and the
  strings-and-font swprintf harness (`sub_8210A490`) then crashed (guest
  writes at 0x70000000, probably an endless loop; `itc_1337.log`): a
  swprintf case, or the native swprintf, fails on other random inputs.
  The new cases use their own generator.
- `course-scenery.md` (new, 71 rows; 3 moved from `functions.md`:
  `Lights::FindNearest`, `TextureAnimations::Advance`, `sub_820F6068`
  now `Scenery::Draw`; 68 new; mapped in 043a0d2): the course's areas,
  their models and lights collected around each viewport and streamed
  through `Archive::Load` into five pools of physical memory (manager at
  `[0x826C5514]`/`[0x826C551C]`), the scenery's draw and the cars'
  headlight projectors (`sub_820F34C8`). Four rows read only in outline
  (`sub_820F6068`, `sub_820F3B70`, `sub_820F34C8`, `sub_820F52A8`);
  `known-data.md` 7 rows; 5 leads.
  Migration 1 (551a868..e7309a0): 37 of 73 ours under
  `--native_course_scenery` (`src/course_scenery/`; `sub_82146B20`,
  `sub_82146AC0` moved from `functions.md`, Native), six adapters
  removed. For review: the native key and reset slots called directly,
  other targets and every load slot through a `Calls` callback from a
  frame laid out like the original's (the slot adapters stay for the
  vtables); `Scenery::LimitLargeModels` (`sub_820F2B80`) reads past its
  5-entry stack table for an area above 4, matched by running
  `__savegprlr_27` first and keeping the table in guest stack memory;
  `ObjectPools::Init` and `Shutdown` native only with the native heap
  on; `TextureAnimations::Advance`'s render-only skip in
  `per_frame_logic.cpp` kept where it was; frame dumps with the flag on
  and off had the same 2,414 draws. Left: blocked on the critical
  section wrappers, `MmQueryAllocationSize`'s, memset,
  `DataFile::GetEntry`, the XPR, the archive's queue and the texture
  provider.
- `c-runtime.md`: `sub_822962B0` memset (probably XAPI's `XMemSet`; new
  row, a212f92: vector stores and `dcbzl` for whole 128-byte lines, the
  same bytes as a plain memset; its 16-byte loops step by the word at
  `[0x8263830C]`); `heap.md`: `sub_822965C0` (probably `XPhysicalSize`,
  over MmQueryAllocationSize) and `sub_822965D8` (probably
  `XQueryMemoryProtect`, over MmQueryAddressProtect), kept for the core;
  1 lead (`sub_82562FF0`). `atexit` left out of this subsystem (its
  critical sections).
  Migration 1 (671f5d0..b549e6c): XMemSet native under
  `--native_c_runtime`, with what it unblocked: `Scenery::Init`
  (`--native_course_scenery`), `CameraPasses::Init` and `InitPasses`
  (`--native_cameras`); `InitPasses`' and `SetMode`'s adapters removed.
  For review: `[0x8263830C]` logged as 16 (nothing in `generated/`
  stores there), so XMemSet is one host memset at stride 16 and follows
  the translated steps for any other stride (harness cases for 0, 8, 20,
  32, 48); the scratch below r1 not written; `CameraPass::Init`'s adapter
  has a thread-local test hook (`hooks::camera_pass_init_calls`, null
  outside the harness) so the harness can record its calls;
  `Scenery::Init` leaves r3 as the original does. `Viewports::Init` and
  `Viewport::Init` still wait on `atexit`.

## Run 2026-10-24 03:25 (`b7-loop-24-10-26`)

- `device-state.md` (new, 131 rows; 11 moved from `functions.md`, 120
  new; mapped in 8b1b86b), `known-data.md` (the setter tables at
  `0x826388F8` and `0x82638D78`, the device's fields); the blocker notes
  in `graphics-resources.md` and `video-output.md` point to it; 1 lead
  (the command buffer, `8229DC28`-`8229F228`). For review: the
  shadow, dirty masks, fetch constants, bound objects, scissor and the
  predicated tiling bit (+10808 bit 1) Kept (the renderer reads them);
  the packets written straight into the command buffer (the screen
  scissor, binning and WAIT_UNTIL, the GPU register split and its dummy
  draws, gamma) Dropped by the graphics rule (nothing reads them: the
  lead checked `gpu_link.cpp` only counts them for its log); the fence
  writes in SetStreamSource, SetIndices and SetTexture kept (recompiled
  code reads fences); the WRAP8-15 setters' dirty bit quirk (bit 45,
  0x2183's) kept.
  Migration 1 (7b1c218..c421c0b): 121 of 131 ours (the 105 state
  setters, the bound objects, constants, SetFVF, the scissor, viewport
  and surface info; `sub_822A6A88` VertexDeclaration::FromFvf Native;
  `sub_822A0F80` SetScreenScissor Dropped). For review: zero-difference
  "(expected to differ)" groups for an uninitialized stack word in the
  originals (the fence list entry's bit 31 in SetStreamSource,
  SetIndices, SetTexture; FromFvf's end-marker pad byte), written 0 by
  ours; a fence on a full fence list leaves the whole call to the
  original, and GetViewport's copy to a physical destination to
  memcpy's override (neither in play); native float copies keep a
  signaling NaN's bits where the originals quiet it (compared as
  floats); the constant copies as raw 16-byte copies (as lvlx/lvrx);
  the adapters written one per line for `check_function_statuses.py`;
  the setters' adapters stay (the device's tables point at them).
  Migration 2 (fd70fa2..fd026da): 131 of 131 ours; SetRenderTarget,
  SetRenderTarget_External, SetDepthStencilSurface, and the packet
  writers without their packets (SetDefaultState, DirtyAllState,
  `sub_8229B7D8`, SetShaderGprAllocation with `sub_8229F680`'s dummy
  draws and their reservations left out, SetGammaRamp); `sub_822B0C38`
  Dropped (no override: SetGammaRamp's C++ leaves the call out; its
  other caller `sub_822A3AA8` isn't seen in play); `functions.md`'s
  Device::SetUpDisplay, Init and SetDisplayMode rows (they call the plain
  C++; their guest calls in the new `src/hooks/device_state_calls.h`).
  For review: guest calls kept through `DeviceCalls` (the draw's Wrapped
  flush helpers `sub_822ADC08` and `sub_822ADE30`, so `sub_8229F680`'s
  mask flush and renderer marks match; non-native table setters, none in
  play; SetTexture's original on a full fence list); SetDefaultState
  calls each setter natively by its table address (the empty
  `sub_82106540` as nothing); the command buffer allocator's bookkeeping
  (+13532, +44, +48, +13516, +10809 bit 6) is read only by the command
  buffer code, not `src/native`; SetGammaRamp writes +13608 itself
  instead of calling memcpy (the original runs if the device were ever
  in the physical views); temporary logs showed no REG_TO_MEM in play
  and the gamma's COND_WRITE (display register 0x1922) the only one.

- `video-output.md` (new, 30 rows; 6 moved from `functions.md`:
  VideoOutput::SetUpDisplay, WaitForVblank, ResolveFrontBuffer,
  Event::Create, Wait, Signal; 24 new; mapped in 2fdfea4),
  `known-data.md` (the video output, renderer +384, device +10760, the
  semaphore's layout, `0x8266D088`, the vblank counts); 3 leads (the
  device lock's other callers, `sub_822A8A60` probably
  SetShaderGPRAllocation, the other ScopedLock users). Corrections to
  earlier maps: "state saved and restored" is "device locked and
  unlocked" in `cameras.md` (`sub_8209FBF0`, `sub_8209FC7C`,
  `sub_820A2F40`, `sub_820A2F94`, the intro) and `strings-and-font.md`
  (Font::Font, the intro).
  Migration 1 (c234559..bafb51c): the video output's constructor,
  LockDevice, UnlockDevice, Device::Acquire/ReleaseThreadOwnership and
  `sub_825B9A60` CurrentThreadId (`functions.md`; through the SDK's
  `XThread::GetCurrentThreadId`) native, with `cameras.md`'s
  MakeRenderTarget, SetUpSurfaces and Init (their helpers Merged); the
  sync wrappers' rows say they stay recompiled (the user's call). For
  review: an unlock without the lock held is logged and skipped (a
  binary semaphore can't count to 2 as the game's could; never seen);
  device +10760 written as the game does; SetUpSurfaces' leak of its
  level 0 surface, once a pass at boot, kept; the cube face's surface
  `sub_822A5C50` (never in play) left a guest call; the lock's guest-call
  return addresses in SetUpSurfaces per type, its other guest calls
  type 3's (nothing reads them); MakeRenderTarget's and SetUpSurfaces'
  adapters stay (guest calls through `ctx`). The user's call (2026-10-24): (a): the device lock alone over a host primitive, a
`std::binary_semaphore` (a semaphore may be released by another thread
than the one that took it; a mutex may not), through LockDevice,
UnlockDevice and the video output's constructor; the Semaphore class
stays recompiled for the archive loader.

- The graphics resources' first migration (e4730b8..e6b9811): 25 of 45
  ours (7 Native), the new row `sub_822B5EA8` Edram::Free (Native, not
  seen in play), `sub_822A6268` named `Resource::DestroySurface`, the
  file's intro. For review: native callers call `refcount::AddRef`
  without the lock whatever `--refcount_global_lock` says (the flag now
  switches only recompiled callers); `Texture::BaseSize` and
  `TextureLayout::LevelSize` read their in/out words once, not again
  after each store (matters only if an out pointed into the header: no
  caller does); `LevelSize` doesn't keep r3's upper 32 bits;
  `Resource::DestroySurface`'s adapter pops Destroy's 112-byte frame
  (the recompiler split it off); the buffers' and CreateSurface's
  failed-allocation paths have no cases (the allocation layer aborts
  when out of memory); a frame dump against the fallback flag showed the
  same targets and draws, the resolve addresses offset by a constant
  (probably the heap's layout; not confirmed with two fallback runs).
  Validated (`itc_1267`, `itc_1269`): `sub_822A5B78`
  Texture::GetLevelDesc has no print of its own (the original only
  jumps to LevelDesc, whose plain C++ its adapter calls; its harness
  and LevelDesc's game line cover it); AddRef's print added (2783ad0).
- The graphics resources' second migration (062da1d..4be0508): 46 of
  46 ours; new row `sub_822A4A40` TextureLayout::MipTailOffset (Native,
  not seen in play). `functions.md` rows changed: `sub_8229E908` (also a
  fence wait), `sub_822AC470` (the data cache flush), `sub_825B9A60` (the
  current thread's id), and new `sub_8229DDD8` (widens the device's
  pending flush range; Dropped from Lock); their Status blank (other
  recompiled callers). For review: paths kept on the originals, never
  taken in play (TextureHeader::Set for unknown types, which read an
  uninitialized stack word; levels from 2^31 in LevelOffset, LockLevel,
  LockRect, LockRectLevel; Destroy's type 9 with `sub_822AD9D8`;
  XGSetTextureHeader with its header out in the physical views or across
  two views); GetSurfaceLevel leaves out its LevelOffset call (it writes
  only its own stack); XGSetTextureHeaderEx stores the 52-byte header
  directly instead of calling memcpy; the lock count by compare-and-swap
  without the recompiler's global lock (as `refcount.cpp`); frame dumps
  matched the fallback flag's.

- `graphics-resources.md` (new, 44 rows; 8 moved from `functions.md`,
  36 new; mapped in 0e67c00), `known-data.md` (the texture, surface
  and buffer headers, the level description, the GPU format table at
  `0x82013468`, the EDRAM allocator at `0x82953E20`, the hi-z flag at
  `0x829537C4`, the device's target and stream fields, the device
  pointer at `0x820007B0`); 3 leads (the device's state setters at
  `822A3030`-`822A4340` and `sub_822A6008`, the boot movie player's
  graphics side at `8248xxxx`, the resources' other makers and users).
  Shared rows left where they were: `sub_822A5B68`, `sub_8235A9F8`,
  `sub_8235A990` (`models.md`), XMemAlloc and XMemFree (`heap.md`).
  The user's calls (2026-10-24): (1) `Edram::Alloc` (`sub_822B5EF8`) made native with a `std::mutex`
in place of the kernel's critical section (its bases kept: the
renderer keys targets by them); (2) `Resource::Lock`'s and
`Resource::Destroy`'s fence waits, `Lock`'s cache flush packet and
`Unlock`'s data cache flush (`sub_822AC470`) Dropped, the lock count,
the returned address and the freeing kept (a harness group for the
command buffer's words expected to differ).

## Run 2026-10-23 20:22 (`b7-loop-23-10-26-5`)

- `ai-drivers.md` (new, 148 rows; 2 moved from `functions.md`, 146
  new; mapped in 1eeb084), `known-data.md` (the driver, path, corner,
  other-racer, skill and car input record layouts; the route file's
  corners table and per-slice corner list; globals `0x826C6074`,
  `0x826C6078`, `0x826C5B30`, `0x826D82E8`; `.rdata` tables at
  `0x82008970`, `0x82008A08`), `track-route.md`'s intro; 2 leads (the
  race states' route users, the drivers' callers). For review: the two
  moved rows corrected (`sub_8213F530` is `AiPath::Update`,
  `sub_8213D438` `AiPath::Reset`, not the speed trail); unsure names:
  the other-racer record states 0-3, driver states 2-6, the corner
  points at data +48 to +54, `AiDriver::KeepsGapFrom`,
  `AiPath::RaceAllowsSides`, `AiDriver::IsFree`,
  `AiPath::LeaderBranchKind`, `AiPath::FollowLeaderBranch`, the skill
  block as a profile plus triggers, vehicle flags 0x81 as the player's
  car, input flag 0x200 as reverse when stuck; ~30 rows "Not read in
  detail" (the corner planning and avoidance math); `AiDrivers::Create`
  zeroes the table after `AiDriver::InitPath` set its floats (the
  game's own, kept).
  Migration 1's rows (c94d6be..91b8da2; `--native_ai_drivers`; 74 ours,
  24 Native): notes corrected for `sub_821390D0`, `sub_8213D438`,
  `sub_82139B40`, `sub_82139820`, `sub_8213C5C8`, `sub_8213C630`,
  `sub_82146408`, `sub_82139CB0`, `sub_8213ADF8`, `sub_821452A0`,
  `sub_821454C8`, `sub_8213C010`, `sub_8213C3A8`, `sub_8213C420`,
  `sub_8213AC70`, `sub_8213D020`. Code in `src/ai_drivers/` (strict
  float flags), adapters `src/hooks/ai_drivers.cpp`, harness
  `src/harnesses/ai_drivers.cpp`; the course's route builder moved,
  unchanged, from the track_route harness into
  `src/harnesses/test_route.h` (shared). Left on the recompiled code:
  UpdateSteerInput's kinds 2 and 3 (`sub_8213AB40`, `sub_82139A08`,
  never seen in play, tail-called); AllSettled and AdvanceCorners hand
  the call back to the original when memcpy's copy can't be native (a
  physical-view destination or a range crossing a view's end; none
  seen in the harness). Kept: stack-passed words written to the
  originals' frame offsets; r3 left as each original leaves it;
  single-word float copies keep a signaling NaN's bits; StartRace and
  LimitThrottle read the vehicle without a null check, as the
  originals. The AI harness refills its block every fourth case (about
  4 s to under 1 s).
  Migration 2's rows (e01cc35..24669a4; 18 more ours, 22 adapters
  removed: 92 ours, 46 Native): `sub_821462A0` (UpdateState's state 6,
  never seen in play, no row) ported with UpdateState and its own
  harness cases, said in UpdateState's row and the intro; JoinCornerPair's
  equal-direction steps (`sub_8213BDF0`, `sub_8213BE80`, never seen in
  play) left on the recompiled code, called by AddCornersAt's adapter
  from the original's stack frames; SteerToTarget's row corrected (+152
  is sin × +384 + cos × +392, not cos × +384 + sin × +392).
  Migration 3's rows (1521ad2..18ea84b; 22 more ours, 25 adapters
  removed: 114 ours, 71 Native; 23 new rows). Never seen in play,
  ported from reading only with their own harness cases (no rows; the
  file's "Not seen in play" paragraph), names guessed: UpdateSteerInput's
  kinds 2 and 3 (`sub_8213AB40` SteerIntoCorner, `sub_82139A08`
  SteerOutOfCorner, `sub_8213A200` SteerAtCornerPoint, `sub_82138FA8`
  SteerByRoom, `sub_821392D8` LimitSlide), `sub_8213D898` AvoidByLane
  (with AvoidOthers), `sub_8213F7E0` EntryWord and `sub_8213F868`
  EntryValue (with AiSkill::TriggerStarts), and `sub_8213F9A4`,
  `sub_8213FB98` (jump targets made functions, inside TriggerStarts and
  TriggerEnds). Corrections: the steering steps' f1 now compared (the
  original leaves the x difference to the target in f1 without atan2);
  MeasureOthers skips a negative racer slot (not the list's end), and
  its running sum and ring entry carry over between racers from entry 2
  (kept); DropOffRouteCorners changes the last kept record only when
  its +37 is set and more than one is kept. Kept, probably a game bug:
  SpreadOffset (`sub_8213B890`) adds the leftover after a clamp to the
  same entry several times, not the ones after it.
  Migration 4's rows (fcec6fd..2ea28d9; 16 more ours, 13 adapters
  removed: 130 ours, 84 Native). `AiDrivers::UpdateAll` keeps its
  adapter: each driver's update goes through the pointer at +6148,
  still recompiled targets, so the plain C++ takes a callback and the
  adapter makes the guest call from a frame like the original's.
  Corrections: `LeaderBranchKind` and `FollowLeaderBranch` also give
  kind 5 (branches 1 and 5, through `sub_8213D2B8` and `sub_8213EC8C`,
  jump targets made functions, never seen in play, ported with their
  callers); `BranchMasksFor`'s kinds 1, 6 and 7 fall through to the
  +32 and +48 lists' checks. r3 kept 64-bit where the original
  sign-extends (BranchMasksFor's list address, FindSideSection's -1).
  Harness limits: FindSideSection and CompareWith always given a
  vehicle (the originals read it without a null check), CompareWith
  and UpdateAll cases use finite floats in the first seven drivers.
  Migration 5's rows (5a7f90d..2ddd638; 13 more ours, 35 adapters
  removed: 143 ours, 119 Native). Fallbacks: when AdvanceCorners can't
  copy natively (corner records in the physical views, never with heap
  drivers), DropPassed, AiPath::Update, AiDriver::Update, DriveCar and
  UpdateIdle return false first and the top adapter runs the original
  whole. For review: `Extend`'s calls it can't make natively (the
  +5532 normalizer when not NormalizeByLane or NormalizeByBranch,
  AddCornersAt's join step; neither in play) go through a `PathCalls`
  struct the top adapter (DriveCar's or UpdateIdle's) hands down, as
  UpdateAll's +6148 calls; with that, `Extend`'s adapter was removed:
  does it fit "its work is plain C++"? Kept, the game's: `Extend`
  appends to the section list with no bound check (past 8 sections it
  writes into the corner count +5162; harness routes cross at most two
  a call). Corrections: StartRanked moves vehicles with flags 0x81 to
  the front, not the back, and sorts only with three or more;
  BranchMasks uses +5528's kind when +5524 is -1; UpdateIdle's reset
  bit is 0x2. Harness: DriveCar's cases also compare the vehicle's
  +2249 and +2424 (an "extras" mechanism); BranchMasks' cases have
  their own 0xA3B00-byte race-data block. Still recompiled: Destroy
  and DestroyRings (wait on `sub_820900C0`), UpdateStarting,
  UpdateTrack and NoteFixedSections (wait on `sub_821434E8`); 22
  adapters stay for recompiled callers in play, 2 (NormalizeByLane,
  NormalizeByBranch) for the +5532 pointer, and UpdateAll's for the
  +6148 targets.
- `racer-controllers.md` (new, 26 rows; 3 moved from `functions.md`,
  23 new; mapped in b5491fb): the per-tick code filling each car's
  input record from the pad or the AI driver (`CarInputs::Create` and
  `Destroy`, `PlayerControl::ReadPad`, the input recording and replay,
  the race flow's per-state handlers, `AiDriver::Setup`);
  `known-data.md` (the car input record is 20 bytes, +17 0 pad / 1
  wheel / 2 AI; `0x826C5B30`, the race data's +0x59AE4 flags, the race
  flow object, the control map, `0x826C5B4C`, `0x826C7960` and
  `0x826C7964`); 3 leads. For review: the handlers are reached through
  functors `sub_82160AE0` registers (a lead), so their adapters stay
  until that table is native; the state numbers from simulating those
  registrations, their meanings guessed (7 the race start, 23 the race,
  22 after the finish; messages 29, 30, 34 "probably" pause and
  replay); `sub_8209F340`, `sub_8209F3B8` named `Pad::IsConnected`,
  `Pad::IsWheel` (`sub_82591C08` moved too); `ai-drivers.md`'s input
  record size (18 bytes) and DriveCar's callers to correct;
  `RacerControl::DriveByAiWithPad` (`sub_8215F7E8`) would write the
  steering on its scale's overflow, which a clamped value can't reach
  (kept).
  Migration 1's rows (699bbde..7b85ae8; `--native_racer_controllers`;
  27 of 27 ours, 15 Native; `sub_821024D0` and `sub_82591C08` Merged;
  `sub_8215F5F0`, `sub_82160880`, `sub_82102A28`, never seen in play,
  ported with harness cases, no rows; `ai-drivers.md`'s DriveCar and
  UpdateIdle rows corrected). The agent ran `git checkout` on
  uncommitted work once (batch 4, after a mutation test), against the
  rules; it redid the same edits and committed them (a71062d), nothing
  lost. New row: `sub_8215FC38` (RaceFlow::OnState34) drives every car
  in attract races but isn't in `seen_in_play.txt` (found with a
  temporary log): the coverage list may miss other attract-race paths;
  it leaves r3 at the last racer count read (kept). Correction:
  `RaceFlow::AllByAi` passes hold flag 1 to both callees (the cars are
  held). For review: the input record's bytes +18 and +19 are written
  by nothing: `PlayerControl::Update` builds the record in its stack
  frame, so they're the frame's leftovers; native code keeps the same
  frame offsets (the harness matches), but the leftovers under native
  callers can differ in play from the recompiled ones (they look like
  padding nothing reads); `DriveByAiWithPad` likewise reads its control
  words from the frame when a control of 0 or above 30 writes nothing.
  DriveCar's and UpdateIdle's adapter work moved into
  `hooks::AiDriveCar` and `AiUpdateIdle` (`src/hooks/ai_drivers.h`),
  with stand-in pointers only the racer_controllers harness sets.
  ReadPad calls `Vehicle_GetSpeedKmh` directly (no `--capture_dir`
  note); `PlayerInput::Read` reads mapper slots 8, 10, 12 and 14
  through the action mappers' plain C++ when `--native_action_mappers`
  is on (slot 5, `sub_8210BA68`, still through the vtable). Adapters
  kept: the 8 handlers reached through the race flow's functors, and
  `sub_82102968`, `sub_82170FF8` for recompiled callers.
- `track-zones.md` (new, 92 rows, all new; mapped in 52cd53b): not the
  AI's path search the lead guessed but 70 hardcoded stretches of the
  course's route (zones, their set-ups the slot 0 of 70 vtables at
  `0x82011AF0`), each of up to 16 route ranges (start and end (section,
  slice), two 64-slot section lists), checked each tick against the
  player's car; the owner at `[0x828161EC]` (`TrackZones::`); fixed-size
  lists for 16, 64 and 128 slots; the HUD reads the flags
  (`sub_82212F00`, `sub_82213510`, a lead). `known-data.md`:
  `0x828161EC`, the zone and route range layouts, `0x82603274`,
  `0x82011AF0`, `0x8200D838`. For review: `sub_820C9718`
  (`Racers::Vehicle`) has its row here, though the race manager's (to
  move when that's mapped); `List16::Link` and `List64::Insert` shared
  with the camera modes and the model list; unsure: what the zones mean
  (probably expressway route names or signs), `RouteRange::ToNeighbor`,
  `TrackZones::Create`'s player choice (probably the second player);
  zone 55's only range is the point (0, 0)-(0, 0), probably a
  placeholder; `RouteRange::WithinEnds`'s "slice above -1" test as the
  code has it.
  Migration 1's rows (31bc1e8..a99b454; `--native_track_zones`; 92 of
  92 ours, 9 Native): the 70 set-ups as one plain C++ function each
  over a table of their ranges read from `generated/` (`kSetupRanges`,
  checked against the rows), each range built at the original's stack
  offset; `RouteRange::Init`'s row corrected (the end from r4 + 4, r5
  not read). The coverage list misses three camera-mode functions that
  run in attract races (`sub_82155288`, `sub_821562F8`, `sub_820FE830`,
  seen by temporary logs, no rows: the camera modes' lead): Init,
  AddBranch, Update and Test keep their adapters for them. For review:
  Create's harness zeroes the owner's lists' unwritten slots before
  comparing (copied from the set-ups' stack; in the original, its
  callees' frame leftovers, which native Create doesn't write; nothing
  reads them, the walks stop at the counts; the set-ups' own harness
  compares them byte for byte); the range tests' harnesses leave out
  cases whose walk might never end on the random route (the original
  wouldn't return either: OnPath 29 of 1000, Test 70, Update 51,
  Zone::Update 123 of 300, logged).

## Run 2026-10-23 17:22 (`b7-loop-23-10-26-4`)

- `strings-and-font.md` (new, 35 rows; 0 moved, 35 new; mapped in
  0974397), `known-data.md` (`0x826C5C34` and `0x826C5C00` split into
  their own rows; new rows: the font page, the text draw parameters, the
  draw counts at `0x826C5BCC`, the pixel shader sources at `0x826C5C04`);
  1 lead (the app's framework beside the string table). For review:
  `ui-movies-game.md` corrected (a text's +20 is the page shown, +24 the
  lines to skip; `sub_8210A5C0` counts page breaks, so `sub_8211C930`
  moves to the next page; `UiMovie::RevealText`'s name kept, probably
  "next page"); `files-and-saves.md`'s `sub_8210A8D0` named
  `Strings::Load`; unsure: `Font::SetParams`'s +8 and +12,
  `Font::DrawPass`'s slot layout (command 36 not in `gpu-functions.md`),
  `sub_82108860` probably `XPR::Release`, `sub_8210A490` probably
  `swprintf` (its ready group is the C runtime's wide printf).
  Migration 1's rows (16458d3..29aea2e; `--native_strings_and_font`;
  25 of 36 ours, 14 Native; `c-runtime.md`'s `sub_825A9A80` Merged,
  `sub_825A9980` and `sub_825B13D0` Native; `sub_82108810` a new row,
  not seen in play). For review: the agent ran `git checkout` on
  `src/strings_and_font/font.cpp` with uncommitted work in it (against
  the "commit before reverting" rule), then re-applied the same edits,
  checked with all harnesses (`itc_1029`) and a frame dump, before
  16b310a; kept on the recompiled code (never in play): rotated text
  (`sub_821097F0`, `sub_82108F68`), the second pass (`sub_82109D80`),
  DrawPass with a chunk open, a list possibly too full or the command
  list's diagnostics on, Strings::Init and Release without
  `--native_heap`, swprintf's `%n`, `%Z`, float types but `%f` and
  `%f` needing the heap or a forced point, output holding 0xFFFF,
  FormatGrouped's negative values holding 0xFFFF or over 240 units;
  swprintf's `_woutput_l` is a step-by-step translation
  (`src/c_runtime/wide_format.cpp`) run once dry to decide whether it
  can run natively, its buffer where the original's frame would be
  (2,542 of 3,441 harness cases took the native path); DrawPass's and
  Font::Draw's adapters stay (the render queue's call through its
  vtable); new helpers `src/strings_and_font/steps.h`,
  `c_runtime::Vswprintf` in `format.h`.
- `std-string.md` (new, 16 rows; 0 moved, 16 new; mapped in 7aeebb5:
  Dinkumware's `std::basic_string<char>`, the library's wildcard search,
  find and replace-all, two global strings' static set-ups),
  `known-data.md` (the 28-byte string layout, the global strings at
  `0x8266EE7C` and `0x8266EE98`); 1 lead (the containers'
  instantiations). For review, not edited: `ui-movies-game.md`'s
  `sub_8211E378` separator from `0x82001DE8` called "." without the
  image's string read (maybe "/"); `files-and-saves.md`'s `sub_8209DFA8`
  leaves out the path's copy into a string and its wildcard replace
  (`sub_82593088`, never matched in play); `sub_82593040` is the
  substring copy constructor, not "making an empty one"; unsure:
  `String::ReplaceAll`, `String::Find`, `WildcardSearch` (probably; the
  `*` backtracking not read in detail), the global strings' texts.
  Migration 1's rows (2028ce7..5fdcfc2; `--native_std_string`; 14 of
  16 ours, 5 Native). For review: paths left to the original, checked
  before anything changes (never in play): memcpy_s's and free's
  errors, operator new's new handler, `_Xlen`, `_Xran`, erase on a
  self-assign, `append` from the string's own text (`sub_820A6E60`),
  `replace` (`sub_82592E18`) when the wildcard replace matches, copies
  not below 0xA0000000 in one view (the mirrors' memcpy recording
  kept); the constructors write the empty start before their assign's
  checks (the original writes the same bytes again on a fallback);
  `_Tidy`'s r3 kept as the original leaves it; allocating paths native
  only with `--native_heap`; the harness's ReplaceAll patterns can't
  match except the game's (`/` to `\`: a matching-anything pattern
  makes the original loop forever), and `_Copy`, `_Grow` cases use
  capacities 15 and up (the `_Copy` quirk in the file's intro); new
  helpers in `src/std_string/std_string.cpp`.
- `track-route.md` (new, 40 rows; 0 moved, 40 new; mapped in e211173:
  route positions normalized and moved along linked sections, points,
  lanes and distances, `TrackRoute::Follow`), `known-data.md`
  (`0x826C5B10`'s full layout, the default branch mask at `0x82008A00`,
  the track position's +4 and +6 as a route position, the vehicle's
  +5232 as the car's track position); 1 lead (the race code's users of
  the route). For review: `collision-world.md`'s `TrackPosition::Init`
  (`sub_820F8578`) corrected (its point comes from its route position,
  not the other way round); `cameras.md`'s intro points to
  `track-route.md` for `sub_820AB408`, `sub_820AB498`;
  `vehicle-accessors.md`'s `sub_820C0168` `Vehicle::Identity` probably
  deserves a new name (left); `sub_820F985C` is a jump table's case 0
  split off by the recompiler; unsure: `TrackRoute::SliceTest`,
  `SpecialSection` (fixed section numbers, probably one course's),
  links 6 and 8 as the left and right neighbors, the two point tables
  as the road's edges, `LinkDistance`, `PathDistance`, `ToMainRoad`'s
  end not read in detail.
  Migration 1's rows (9c3cbaa..7785364; `--native_track_route`; 33
  rows changed, 17 Native, `sub_820F985C` Merged). For review: the map
  corrected for `sub_820F9448` (every linked section's lanes added up;
  kinds 1, 3 and 4 only not taken), `sub_820F9600` (taken when it's the
  only match), `sub_820FB030` (the shoulders start at half the width
  plus 0.5); kept on the recompiled code: SlicePoint's points 5 to 16
  (`sub_820F8FA0`), the unseen callers `sub_820F9DD0` and `sub_820FB2B0`;
  stack temporaries mostly not written (nothing reads them after the
  return), but Point and AlongSlice still write their position copy at
  the original's frame + 80; Distance's r3 kept as the original leaves
  it; new helpers inside `src/track_route/track_route.cpp`, built with
  `-ffp-model=strict -ffp-contract=off`. Attract races with all
  harnesses now reach 18 s at 30 ticks/s inside `attract_race.sh 25`
  (19 before): the harnesses take ~4.9 s before the race; without them
  23 s as before (`itc_1084`, checked by the lead). Since (the user's
  call, 2026-10-23), harness runs use `attract_race.sh 30`.
  Migration 2's rows (0b863a1..1b58c97; all 40 of `track-route.md`
  ours; `collision-world.md`'s `TrackPosition::Update` (`sub_820F8470`)
  and `Init` (`sub_820F8578`) native under `--native_collision_world`
  in `src/track_collision/track_cell.cpp`, calling the route's native
  Follow and Point whatever `--native_track_route` says; 7 adapters
  removed). For review: Follow's full search (`sub_820FB2B0`, never in
  play) ported as `TrackRoute::FullSearch`, with its own harness group
  but no row of its own (described in Follow's row; CLAUDE.md gives
  unseen functions rows marked "Not seen in play": a row may be
  wanted), keeping two quirks of the original (the next link from the
  last linked section in bounds; the whole kind byte tested `>= 0x30`);
  Follow, PathDistance, Update and Init don't keep the original's r3
  (no caller reads it); positions the callees read by address stay in
  the original's frames; PathDistance's harness uses routes of 8-slice
  sections (the original's loops can run forever on random links);
  18 adapters stay for the race and vehicle code's recompiled callers.

## Run 2026-10-23 13:44 (`b7-loop-23-10-26-3`)

- `model-animation.md` (new, 36 rows; 0 moved, 36 new; mapped in
  c425049), `known-data.md` (the model's +304 to +384 spelled out; new
  rows: the track set, the animation section with its 20-byte keys,
  the skeleton); 5 leads. For review: `models.md`'s `sub_82599A20`
  corrected (the per-node float4s' x, y, z set to 0.0, not 1.0) and
  `sub_8259A9E0`'s row given its r6 instance count (+160) and the
  per-instance cursors at +128; the primitive drawer (`sub_825A1438`,
  `sub_825A16B0`, `sub_825A17A0`) and `sub_825A18D8` aren't animation
  but have rows, as the lead named them; `sub_825A5778` to
  `sub_825A6460` are probably XAudio's effects (XAPO), not animation (a
  lead); only some track sets ran in play (the +364 material animation
  and +384 sets' helpers and `sub_825A20E8` never did); Model::Animate
  advances times by 1.0 a call from `sub_820F6068`, probably on
  render-only passes too (not checked).
  Migration 1's 36 rows (6f51fc2, 6714e72; `--native_model_animation`;
  16 Native, written without adapters as only native callers reach
  them; 3 new rows, Native and not seen in play: `sub_825A0FE0`,
  `sub_825A10A8`, `sub_825A20E8`; `models.md`'s `sub_8259C3A8` Native,
  named Skeleton::UpdateNodeMatrices; left: the primitive drawer's
  vtable methods, blocked by the graphics device). For review: the
  releases, set-ups, frees, destructors and the drawer's constructor
  run natively only under `--native_heap`, handing the call to the
  original when free or operator new would refuse it (the collision
  precedent), and abort if `OperatorNewArray` fails after those checks;
  `TrackSet::Setup208` is native only for sections with no tracks (all
  in play) and `sub_8259C3A8` hands node types 8 and 16 to the
  original, so `sub_825A6608`, `sub_825A1938`, `sub_8259B748` and
  `sub_8259BE40` stay recompiled (both checks assume the data isn't
  written during the call); `sub_8259C3A8` notes its multiplies with
  `BlendState_OnMultiply` under the original's return addresses;
  `Curve::Sample` corrected (before the second key it interpolates the
  first two, not clamps); the harnesses never build node types 8 or 16
  or Setup208 sections with tracks; the new code reuses
  `car_physics/float_ops.h` and `xact::WaveEntryConstruct`.
- `car-specs.md` (already mapped from the game data check; the map
  checked by the lead, not remapped). Migration 1's 46 rows (d87b553,
  04c3ebd, f1727ea, 80867d4, 114aa35; `--native_car_specs`; 9 adapters
  removed and 25 Native with no adapter; 12 unnamed rows named, among
  them `Car::CopySpec` `sub_820C4888`, `Car::CopyAeroParts`
  `sub_820C4568`, `CountBodyKits` `sub_820CBD20`; left: `Params::Load`,
  blocked by the archive, and `Params::Free`, not seen in play). For
  review: the swaps byte-swap integers where the original round-trips
  through lfs/stfs (folded in `generated/`; the harness checks
  signaling NaNs and denormals); the archive's TOC lookup `sub_8209D868`
  (no row anywhere) stays a guest call from the adapters; the four load
  callbacks keep their adapters (reached through a function pointer);
  `Car::Refresh` runs the original if a copy wouldn't be the host's
  memcpy or the car's span overlaps a source (never taken), its aero
  getters write into the car directly and the 36-byte parts block is a
  host array; the float adapters turn flush-to-zero off unconditionally
  (the data tables' precedent); `sub_820CB0C0`'s leftover r3 reproduced;
  the Refresh, CopySpec, CopyAeroParts and InchUp harnesses use made-up
  tables (the game's load after the harnesses), and some cases point
  the parameter global at a test object for the call (the data tables'
  precedent: the load callbacks run when the main thread lets them);
  `Params::Init` native only under `--native_heap`.
- `cameras.md` (new, 42 rows; 6 moved from `functions.md`, still
  Wrapped with their old text, 36 new; mapped in 8fb7201): the library's
  `Camera` class, the 21 camera passes at `0x826BAC00` and their render
  targets, the two 1,872-byte viewports at `[0x826C2CE8]`; 4 leads (the
  camera modes at `82152028`-`82156CC0`, a second camera class, the
  graphics device's resource calls, memset and atexit), and
  `sub_820AB408`, `sub_820AB498` added to the route lead. For review:
  `known-data.md`'s `0x826C2CE8` row corrected (1,872-byte viewports of
  twenty 80-byte entries, not 80-byte records) with new rows for
  `0x826BABC0`-`0x826BABFC`, the viewport and its entry; a reversed
  depth range from `Camera::BuildProjection` (+384 near, +388 far);
  "probably" for mode 1's passes 4-9 as a cube map's faces, the entries'
  +68 as a focal length in pixels; `CameraPass::RecordBegin`,
  `RecordEnd` and `Viewport::UpdateEntries` not read in detail.
  Migration 1's 20 rows (81d1c0c..1736a63; `--native_cameras`; 1
  Merged, `sub_8209F7DC`; `CameraPass::SetZoom`'s adapter removed). For
  review: interpolation's hooks kept: `CameraPass::Update` builds its
  view at the original's stack address (r1 + 128, the copy note's key)
  and notes the copy and multiply under the original's return addresses
  (0x820A3D18, 0x820A3D90), through a new
  `interpolation::BlendState_OnCopy` beside `BlendState_OnMultiply`
  (`--log_blend_state`: the same blend counts with the flag on and off);
  the `ITC_FRAME_STATE_WRAP` wrappers of RenderTarget, Disable and
  EnableForView written out with the native work inside, and the
  `--log_camera_pass_records` wrapper calling `hooks::CameraPassUpdate`
  (new `src/hooks/cameras.h`); vtable slots go to the plain C++ only
  when they hold the library's function (always in play); BuildProjection,
  SetViewportSize and Update run the original when the stack pointer
  isn't 16-byte aligned; RecordViewport and RecordClear keep guest calls
  (the scene's getters, the render queue) and run the original with the
  command list's diagnostics on, and where a chunk wouldn't fit (never in
  play) call the scene's getters once more than the original;
  float-register words copied as bits (signaling NaNs kept, as the
  translation does); `Viewports::Release` native only under
  `--native_heap`; the harness changes live game globals (the mode, the
  passes' enabled words, pass 0's near plane, pass 4's field of view,
  the viewports and scene pointers, the open-chunk count) and puts them
  back after each run; the SetZoom adapter removal (7b4c8b1) was built
  and raced only with the next commit; two functions named
  `RecordClear` (`sub_8209FA38` here, `sub_825A18D8` in
  `model-animation.md`).
  Migration 2's 13 rows (14a4cc8..11837c9; still `--native_cameras`;
  2 Merged, `sub_820A0D38`, `sub_820A1CA8`; the adapters of
  `Camera::BuildProjection` and `Viewport::UpdateEntries` removed). For
  review: Camera::Update leaves out the original's dead `tan` and `atan`
  (results never read; memory and r3 the same, only f1 on return
  differs, which the caller doesn't read); it keeps interpolation's
  notes (its view at the original's r1+112, the copies at 0x825936C4
  and 0x825936D4, the multiply at 0x82593798); the record passes keep
  their `frame_state_diff` wrapper (`--find_per_frame_state`,
  `--debug_hide`) and RecordEnd calls RenderTarget through its override
  with the original's return addresses; with the command list's
  diagnostics on, or a full list or another chunk open, the original
  runs and the scene's getters are called once more (never in play);
  RecordBegin, RecordEnd and RecordViewport keep their adapters (guest
  calls); the record-pass harness checks only the last submission to
  its made-up queue; map corrected: `sub_825D9310` (marked never run)
  is the 21 passes' static constructor, run at boot; +372 is height ×
  0.5 × tan(fov/2).

## Run 2026-10-23 11:06 (`b7-loop-23-10-26-2`)

- `action-mappers.md` (new, 40 rows; 15 moved from `functions.md`, 25
  new), 4 `known-data.md` rows (the globals `0x826BAB78`/`7C`/`80`, the
  client list at `0x82970C14`, the settings table at
  `0x82600B84`/`B88`, the action mapper's layout); 2 leads (the input
  processor's constructors, the app's start-up). For review: the read
  slots never seen in play (`sub_8210BA68`, `sub_8210BED8`,
  `sub_8210BFA8`) stay in `functions.md`; of the button tests only
  `sub_8258CF10` and `sub_825924E0` (called only by the mappers) moved;
  `sub_8210C370`'s old note (a copy of MenuList::Update's) kept, with
  the real description after it; the config slots
  (`sub_8210B698`, `sub_8210BDF0`, `sub_8210BE30`) matched to their
  slots from `sub_8209F228`'s arguments and the constants' values,
  both "probably" (the vtables and tables weren't read from the image).
  Migration 1's 36 rows (`--native_action_mappers`; 10 Native, adapters
  removed; 4 rows left: `sub_8210AF08` BindAll and `sub_8210B270` Bind,
  whose 7 calls into the input manager's vtables aren't identified, and
  `sub_8210C180`, `sub_8209F430`, blocked by `sub_8258CE60`). No behavior
  change (the user's call, 2026-10-23: migrate off the recompiled code
  only): every harness matches exactly, the registers the originals
  leave behind too; the per-tick override of `sub_8210B018` still skips
  render-only passes first. For review: bits 30-31 of the held,
  pressed and released words come from an uninitialized stack word in
  `sub_8210BBA0` (r25's high half as saved by `sub_8210B7D0`), and the
  native code is handed the same value (`docs/pause-menu-fix.md`); the
  update, Configure and the set-up run the original unless every repeat
  and curve object's slot is the function seen in play; their slot 0
  (`sub_8243DC88`, `sub_82106540`, shared with other classes) done in
  place, no rows; the `--log_pause_menu` and `--log_input_reads`
  diagnostics moved into `src/hooks/action_mappers.cpp` over new helpers
  in `src/debug/pause_menu_log` and `src/frame_pacing/game_messages`,
  and the adapters of `sub_8210C370`, `sub_8258CF10`, `sub_825924E0`
  kept for them on the fallback path; c67265f has a batch and four
  adapter removals together; new names `InputProcessor::AllHeld`,
  `TestHeld`, `ActionMapper::Held`, `Pressed`, `Fired`, `AnalogRaw`,
  `SetThreshold`, `SetRepeat`, `SetCurve`.
- `data-tables.md` (new, 18 rows, none moved): `[0x826C5CA0]`'s set-up
  (`sub_82116DB0`), twelve asset loads (94: 400 items of 3,008 bytes,
  "probably" the game's items, keyed by the save data's 12-byte records
  at +0x199F8; 84 with no reader but the load; eight 56-byte tables from
  the ids at `0x82616C64`; 95's 20-byte and 96's 40-byte entries), their
  load callbacks byte-swapping the 56-, 20- and 40-byte tables in place
  (the items aren't), and the accessors; 4 `known-data.md` rows; 4 leads
  (the boot data load, the save data's items, `82140340`-`82144E58`, the
  front-end callers at `822271F8`-`8222F0B0`). For review: the eight
  asset ids at `0x82616C64` not recorded (they need an image dump; one
  outside the repo wasn't used); `SwapWord` (`sub_820C97D8`) swaps a
  word through a float load and store, which in the recompiled code
  probably quiets a signaling NaN's bits (the 360 wouldn't): the native
  code keeps the recompiled result (no behavior change, the user's
  call; see migration 1 below: there's no quieting).
  Migration 1's 16 rows (`--native_data_tables`; 4 Native: SwapWord and
  the three entry swaps; left: DataTables::Init and Load, blocked by
  the archive's `sub_8209DE48`, `sub_8209D868`, `sub_820A0678`). For
  review: SwapWord's float round trip is folded away by the compiler in
  the release build (a plain byte swap; signaling NaNs come through
  unchanged), and the native code writes the same steps under the same
  floating-point settings, so it folds the same (256 signaling-NaN
  patterns compared exactly; the mapping's "probably quieted" corrected
  in the row); the adapter turns flush-to-zero off as the original
  does, and doesn't set f0 (no caller reads it); the accessors return
  r3 zero-extended where the originals keep the argument's upper 32
  bits (as the vehicle accessors do: the game reads the low word only);
  Entry56, Find20, Entry20, Entry40 and ItemRecords::Item weren't seen
  in the attract race (the front end, the save data); the five load
  callbacks keep their adapters (reached through Archive::Load's
  function pointer), the seven accessors too (recompiled callers).
  After the run's validation: `sub_82116DA8`'s adapter calls a new
  one-line `data_tables::RecordsItem` (over `Item`) with its own print,
  as its harness step does (the user's call).

## Run 2026-10-23 01:52 (`b7-loop-23-10-26`)

- `sound-cues.md` (new, 87 rows; 4 moved from `functions.md`:
  `sub_8259D1A8`, `sub_821184E8`, `sub_82118FB0`, `sub_82118C18`), 8 new
  `known-data.md` rows (the sound manager, the audio engine, the bank
  pair and holders, the sound, the emitter, the listener, the XACT and
  XAudio engines); 4 leads added (XACT's body, X3DAudio and the engine
  sounds, the data tables, a file-layer helper). For review: XACT's
  vtable slots named only "probably <method>"; the intro records the
  user's call that XACT will probably be translated (it replaces "keep
  the game's XACT" in `native-audio.md`'s decisions, edited there 2026-10-23
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
  XACT's second map (37 rows in `xact.md`: the global settings'
  accessors, the renderer's voice slots, the sound classes' and
  listener slots, the wave entry's getters; 2 in `functions.md`:
  `sub_822956F0` probably GetTickCount, `sub_8220BC28` probably
  MenuList::GetPreviousSelection; `known-data.md`'s sound fields +96,
  +104, +120, +128; 1 lead, the code above the tracks). For review: the
  settings' slots +16, +20, +32, +36, +44, +48, +52, +56 "probably",
  from what callers do (no image dump read); for your call:
  GetTickCount made native means reading the kernel's timestamp bundle
  the SDK fills, or a host clock (left recompiled).
  XACT's migration 2 (46 rows ours: 39 Overridden, the rest Native, 7
  of them by adapter removal; the intro's settings vtable +0 to +68,
  read from the loaded image). For review: paths seen in play only,
  the rest handed to the original (`sub_825CC9C8` no optional sections,
  `sub_825CB470` flags without 0x2, `sub_825C27C0` no presets and no
  effect or both, `sub_825C2910` +100 is 0, `sub_825C6A20` +240 is 0,
  `sub_825C7798` no listener cues); `sub_825CC380` reads the bank's
  entry through the accessors instead of a stack copy (same words, all
  read before the stores); the folded getters `sub_82099BA0`,
  `sub_820A8958`, `sub_82379D50` overridden (same results for the game
  code folded with them); the harness's `Case` compares f1, a
  `Relocate` helper. Waiting on GetTickCount (your call): XACT's clock
  `sub_825BFE28` and everything above it.
  XACT's migration 3: `sub_825C8440` GetWaveRegion (builds its entry
  accessor in the original's stack slot, as the collision and matrix
  adapters do) and the folded getter `sub_8220BC28` (`functions.md`,
  one load of +20, the menus' callers get the same result) under
  `--native_xact`; 4 adapters removed.

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
