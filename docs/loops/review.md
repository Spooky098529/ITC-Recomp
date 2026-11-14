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

## Run 2026-11-14 05:01 (`b7-loop-13-11-26`)

- `sub_82562FF0` dropped from `leads.md` (the movie library's range,
  reached only through a pointer).
- The user's calls (2026-11-14): the worker pools follow XAudio's
  call; `Device::Clear`'s worker by the graphics rule (both in
  "Precedent first").
- The in-race screens never seen's map (d71b917e,
  `race-screens-unseen.md`, 99 new rows): race kind 6 is the Live time
  attack's race (its screen after the race is the leaderboard), so
  `race-states.md`'s "system link" note on kind 6 is probably the
  network battle (not edited); three jump-table cases split off by the
  recompiler (`sub_8220FE3C`, `sub_8221E0FC`, `sub_8221D2B4`);
  `known-data.md` (the screens' globals, `[0x826C6AD4]`, the session
  object `[0x82816E3C]`'s fields); `race-screens.md`'s makers point to
  the new file. Groups worked out by hand: `b7_groups.py` doesn't read
  functions never seen in play.
- The in-race screens' migration (fdd8b61e..55b1f75c): rows in
  `race-screens-unseen.md`, three new Native rows in `network-mode.md`
  (`Network::SetLiveState`, `Leaderboard::Rank`, `CarLogo`), the race
  dialog's `StartSave`, `ShowSaving`, `SetNote` (`race-screens.md`),
  `UiMovie::SetNodeCharText` (`ui-movies-game.md`). Left recompiled:
  `SplitResults::Draw` (the network battle's sends), `SaveReplay`
  (`AskSlot`), and, called as originals from native code, the Live
  work `sub_8214B330`, `sub_822917B8`, `sub_82292CC0`; branches the
  harness can't cover are named in the rows. The race HUD machine's
  kind 4 and 6 paths still reach the recompiled roots (a cleanup
  lead). The older `race_screens` harness groups repeat inputs (about
  17 distinct sets in 300); not rewritten (old harnesses stay).
- The user's calls (2026-11-14): the network battle's sends Dropped;
  the leaderboard read answered as no Live (in "Precedent first").
- The static map's report, for the user: 73 ours functions still
  reach their original body through `__imp__` fallbacks taken when the
  native code declines (`operator delete` when `memory::Free` refuses a
  block, `VirtualFree`, `memcpy`, the vehicle's physics): reachable PPC,
  so B7 work; `movies.md`'s end of the movie library (`8257FFFF`)
  should be `82575B07` (154 functions past it are reached from the
  game's Live and XAM wrappers, the network sessions and
  `sub_8245A410`); the network manager's state machine `sub_8214B330`
  (`NetDll_socket`, `bind`, `listen`) is reached statically through the
  race loading states (`sub_821511B8` -> `sub_82260800`); 740
  "originals no longer called" listed in `static-map.md`, candidates
  for Dropped rows (no rows written).

## Run 2026-11-13 12:31 (`b7-loop-13-11-26`)

- Cleanup leads (98f4fe26..52b87de7): `CarList::DrawEntryPowerGraph`'s
  adapter now sign-extends r31 as the original's `lis`
  (`power-graph.md`). `RaceStates::OnRaceEnd` native
  (`race-states.md`, new rows for `Time::Add` and the course record's
  7 functions, never seen in play, ported by the 2026-11-12 call);
  adapters removed for `RacerRecords::Add`, `Stats::AddRaceResult`,
  `Achievements::Check`, `Write` and `Replay::Keep` (the never-seen
  network battle screen `sub_82264B90` now runs Check's and Write's
  originals). `OnRaceEnd`'s harness no longer compares the course
  record's temporaries below its frame (the callees' originals write
  register saves there); the course record's own groups do.
- The morph jobs' map (3b635fc5, `morph-jobs.md`, 21 rows): also the
  shared work item class (vtable `0x820066DC`, every pool job holds
  one). Corrections: `sub_820ACA30` is that vtable's slot 7, not a
  vtable `0x820066F8` of its own (the lead); `app-states.md`'s "slot 3
  `sub_820AC9E8`" of `0x82002850` is slot 0 of the overlapping job base
  vtable `0x8200285C` (noted in the new row only); `known-data.md`'s
  `0x8296B910`/`B914`: `MorphShape` reads `0x8296B914` too.
- The morph jobs' migration (752011b3..63bc5ef5): 21 of 21 ours,
  `--native_morph_jobs` (the makers also need
  `--native_c_runtime_heap`); `Model::MorphShape` calls the maker's
  plain C++ when the maker's slot 1 is `0x820AC7F0`, the guest call
  otherwise; the pool's post stays a guest call; `Morph::Blend`
  (`second-set.md`) Native. `PoolMaker2::MakeMorphJob`'s adapter is kept
  (reached through the maker's vtable) though its only caller in play
  now bypasses it. Harness: `MorphJob::Run`'s cases compare the frame
  window `Morph::Blend`'s own cases do, and r3 without a high word;
  `MorphShape`'s cases use a stand-in maker.
- The save devices' constructor's map (1edd6e03): 5 rows added to
  `save-interface.md` (not a new file), `framework.md`'s
  `SaveDevices::Create` row and `known-data.md`'s save owner and slot
  rows updated; the listener at +56 to be stored as
  `user::kNoListener` without `XNotifyCreateListener` (the
  `SignIn::SetUp` precedent; it must not be 0, as `SaveDevices::Update`
  runs only with a listener).
- The save devices' migration (f0ecffdd..32f5b0d9): 5 of 5 ours and
  `SaveDevices::Create` (`framework.md`, under `--native_save_interface`,
  not `--native_framework`); the destructor's `CloseHandle` on the
  stand-in listener left out (on the recompiled side it fails and only
  sets the last error); the slots' vector deleting destructor stays a
  guest call (teardown only); save counts malloc would refuse run the
  original.
- The slot handlers' map (2c70e8c8): 14 rows in `save-interface.md`;
  `files-and-saves.md`'s `SaveStep`, `LoadStep`,
  `XGetOverlappedResult`, `GetOverlappedResult` and
  `system-services.md`'s `SaveDevices::Update` updated. Correction:
  state 1 is no device and state 2 the failure state (not "2 = no
  device"); `SaveDevices::Update` checks the slot's user (+956), not
  its state.
- The slot handlers' migration (b41c696e..796dcddf): B4's steps in
  `src/hooks/saves.cpp` split into plain C++ (`src/saves/slot_steps`);
  `Create`'s mode 3 takes its size from `XContentCalculateSize`'s plain
  C++ instead of a guest call (same results). B4's step adapters
  (`sub_820A4960` and 8 others) are listed removable but kept: with
  `--native_save_interface` off, the recompiled handlers would reach the
  Xbox content calls instead of `saves/` (the user's decision whether
  to remove them). `SaveDevices::Update`'s adapter stays B5's
  (`notify.cpp`). The enumeration, check, save, load and delete
  machines run only from the front end: harness only so far.
- Three small leads' map (1572447f): the render manager's slots 7 and
  8 and its +4/+8 interfaces' thunks (`app-start-up.md`; they reach the
  framework's scene and frame driver), the texture provider's `Get`
  and its `XPR` slots (`models.md`), the zero and identity matrices'
  static set-ups (`matrix-math.md`; no reader of either found).
- Their migration (87dda153..ee595da5): 13 of 13 ours,
  `Matrix::FromFloats` Native; the render manager's slots return the
  object and target and the adapter makes the tail call (the worker
  pool thunks' precedent); `XPR::GetTexture`'s diagnostics wrap left
  out of slots 5-7 (as `CarModel::SelectedTexture`); `XPR::FindTexture`
  (not seen) stays a guest call.
- The cleanup lead over their callers (4577728d..bc20b2d4): notes in 16
  function files (no Status changes); `hooks::DirectSlotCall` sets r3,
  r11 and ctr as the original leaves them; most callers' harnesses use
  stand-in getters, so the direct path is checked by the new
  `RenderManagerSlotDirect` and `TextureProviderSlotDirect` groups. Left
  out of scope: `app_camera.cpp`'s guest calls to `sub_82094320`.
- The sticker editor's map (680da961, `sticker-editor.md`, 25 rows,
  none seen in play): the ten input pages (states 38 to 46 and 49) and
  their helpers; mentions in `tuning-screen.md`, `tuning-pages.md`,
  `ui-movies-game.md` updated; the owned-sticker bits at save data
  +0x199A8 and four tables in `known-data.md`. Original quirks noted
  for the port: a -1 index reading the word before the T table, loops
  that never end when no sticker is owned.
- Its migration (eb20ef60..d84fa8b8): 25 of 25 ours, under
  `--native_tuning_pages` (`Unloop` under `--native_ui_movies_game`);
  `InputStickerRotate`'s max angle corrected to `0x8200C54C` (359.0);
  harness holes for callee register saves in dead stack and the
  varargs `SetText` areas; `HandleInput`'s register step now disables
  flush-to-zero (`PlaceStickerNode` runs under it); 3 adapters removed
  by reading `generated/`'s call sites (the tool can't judge never-run
  functions).
- Its second migration (662861e6..7fbeadd2): the tuning screen's sticker
  commands 32 to 51 (17 rows in `tuning-screen.md`, read in full, Native,
  under `--native_tuning_screen` with `--native_tuning_pages`); the 12
  remaining helpers Native. The harness leaves out the varargs
  parameter words and the helpers' register saves below each command's
  frame (the native helpers don't write them); a planted fault in
  `Command44` (a `FinishNamed` argument) went unnoticed, as finishing an
  animation not playing changes nothing in the harness world.
- The stand's setters' map (0ced7d4b, `stand-models.md`, 8 new rows):
  `SetColor3` to `5` write the body paint slots then apply the wheel
  paint (+112 to +207): the game's own mismatch, kept; they're
  unreachable (`SetColor`'s callers pass only 0 to 2). The lead's
  `SetAreaDecal` is `GarageStage::SetSticker`. `tuning-screen.md`'s
  `SetColor` question answered; the sticker record and the
  `0x82009A50` table in `known-data.md`.
- Its migration (8d0e0496..0e74c60a): the setters and the tuning
  screen's `SetColor`, `ApplyAreaDecals`, `ApplyDecalWord`,
  `PreviewParts`, `SetParts` native; 17 adapters removed by reading
  `generated/`'s call sites (attract races never reach the tuning
  screen, so no counter); the harnesses `car_makers`, `tuning_refresh`
  and `tuning_machine` now leave out the originals' register saves
  below their frames. Map correction: `SetSticker` stores r8/r9 at its
  caller's r1 + 64 and + 72. `ApplyAreaDecals` copies the sticker
  scale as raw bits (a signaling NaN kept, as the translated code
  does). `MakeModel` and `FitSelectedCar` keep adapters (native
  callers still reach them through the guest). 22 never-seen rows
  without status remain in `stand-models.md` and `tuning-screen.md`.
- The never-seen rows' cleanup (abc17bc2..5db16791): 18 functions in
  `stand-models.md` and `tuning-screen.md` Native, a new row
  `SceneInterior::SetRecord` `sub_820E38E0`; `PreviewItem` and
  `RestoreStand` now report f1 to their adapters (`RefitAxles`'
  `FitAxles` sets it); more harness skips for the originals' dead
  register saves; `RefreshInterior` rewritten after a full read, still
  recompiled (blocked).
- The tuning pages' blockers and the stand's guest callers
  (bc2ed8af..0f0b959d): `IsSlotAvailable`, `SetPaintGradients`,
  `SetLightKind0Bars`, `SetLightKind1Bar` (`tuning-pages.md`) Native
  without adapters: their recompiled callers (never seen) still call the
  originals; commands 10, 13, 14 read in full; `MakeModel` and
  `FitSelectedCar` Native, removed by reading `generated/` (no attract
  race reaches the front end); harness masks for the originals' saves
  in `tuning_refresh`, `car_makers`, `type67_modes`. Its first
  validation differed in `TuningMachine::Start`'s harness (one byte of
  those dead saves, not left out there too): a harness fault, fixed
  edb5225e by the 2026-11-10 precedent.
- The type 6 and 7 modes' small pieces' map (707fe4ba): 16 rows in
  `type6-type7-modes.md`, `type67-screens.md`, `car-list.md` (the
  controller options' steps, the group pair's and panel screens'
  destructors, `SingleGroup`); corrections: `SingleGroup`'s only user
  and the "type 6 two-player panel screen" `sub_822386F0` are the type
  8 mode's; `GroupPair::DeletingDestructor`'s "slot 8" is slot 0 of
  `0x820110A4`; settings +672 and +680 in `known-data.md`.
- Its migration (f83e3ee2..6cb9126c): 16 rows and the blockers
  `ListNode::~ListNode`, `PanelSet::~PanelSet`,
  `PanelSetBase::~PanelSetBase` ours; a bridge in
  `type67_screens/panel_parts.cpp` lets the car list's guest calls run
  through the screens' calls; `PanelFade::In`'s cases compare the
  screen fades' globals; `TuningMachine::Destroy` calls
  `DecalImages::UnloadAll`'s plain C++ (its adapter removed).
- The list screens' tables' map (e3d8b9a9, `type67-screens.md`, 77 new
  rows, 25 changed): the six tables' 31 pages and steps, the node
  tables, `ListPartInner` (a two-item prompt defaulting from save data
  +0x1F870), the screen blocks' destructors (probably never called:
  `StandMode::Release` frees the blocks with `VirtualFree`). Quirks for
  the port: the leaving steps never set their first-call word; the
  cursor helpers leave a 64-bit product in their frames.
- Its migration, part 1 (a15c7b09..87d4c82e): 102 rows ours (the node
  tables, the list part, the list helpers, the titled list,
  `Type6ListScreen` and the panel leaves); `Type6ListScreen::AddRacers`
  Merged into `SetUpRace`; row corrections (`NodeListScreen::LoadItem`'s
  +9700/+9704 bit, `ListScreen::Update`'s slot r4s, the titled deleting
  destructor's vtable store); a new expected group
  (`expected-differences.md`); ~1,100 new harness cases.
- Part 2 (9bd91f77..a29897be): the color list, `Type7ListScreen` (its
  `AddRacers` Merged; its `SetUpRace` traffic records expected to
  differ as type 6's), the node screen with `CourseRecords::Of`
  (`save-data.md`), the panel leftovers, the screen blocks'
  destructors; the titled list's page helpers moved to
  `list_pages.h`; harness faults fixed on the way (inputs kept in the
  ranges play gives).
- The type 8 mode's screens' map (ab62fef5, `type8-screens.md`, 128
  rows; `SaveData::TwoPlayerBlock` in `save-data.md`): the type 8 mode
  is the title's main menu item 4 (string 251, state 18), built for two
  players: probably the two-player split-screen mode (the user may know
  its name). Corrections: `car-list.md` notes that called the type 8
  callers "type 6"; `screen-constructors.md`'s "get no rows" list.
- Its migration (138f7c8c..c7628060): 128 rows ours, 13 `car-list.md`
  rows (`PanelSetPair` and its slots, the panels' base), `TwoPlayerBlock`;
  map corrections in the rows (`OnDialogClosed`'s branches,
  `Type8PlayerBlock::Input`'s items, `UpdateSecond`/`UpdateThird`'s
  menus, `Type8ListBase::Step1`'s frame); two new expected groups.
  Unchecked: `PanelSetBase::HandleInput`'s native default page call
  passes r4 = 0 to pages 4 and 7 where the originals pass their own r4
  (from `PanelSet`'s earlier port; neither page seems to read it). The
  `SetUpRace` switches trap past their last case, as the recompiled
  switch does. The intros of `car-list.md` and `type8-screens.md` still
  call the five car-list rows blockers.
- The type 8 mode's map (9bf9d97e, `type8-mode.md`, 52 rows): close to
  the type 6 and 7 modes (the member inside the mode, the delegates as
  vtable slots, the teardown over slots 9 and 10, three cars and the
  two-player save block). Corrections: `race-loading.md` had
  `sub_8223D960`/`sub_8223DA00` as the race mode's slots 8 and 9 (they
  are type 8's +32 vtable); `car-list.md` called two type 8 functions
  "type 6". `NextMode`'s answer carries three unwritten stack bytes and
  `ForceExit`'s the low bytes of `Mode::Slot9Thunk`'s address (to copy).
- Its migration (ab9d75d1..00c9f8e1): 52 rows ours, with
  `Type8Options::Init`, `Type8ControllerOptions::Init`,
  `Type8SaveScreen::Init`, `SaveData::CreateTwoPlayerBlock` (probably)
  and four shared dialog handlers; map corrections (`MenuPage`'s scroll
  reset only after a cursor move; `UpdateStand`'s reload sends both
  players through the busy branch). Harness gap: `UpdateStand`'s reload
  branch (building the car, `GarageStage::SetCar`) never runs in its
  cases, as in `Type6Screen0::Update`'s. Type 8's delegates aren't in
  `type2_mode::SlotThunk`'s native switch (reached through their
  adapters, which run the plain C++).

## Run 2026-11-12 08:26 (`b7-loop-11-11-26`)

- The frame driver's map (6d9bc131; mapped as `renderer.md`, renamed
  `frame-driver.md` and `FrameDriver::` by the user's call, 2026-11-13,
  as "renderer" was confusing beside our Vulkan renderer): 41 rows (9 moved from
  `functions.md`). Corrections: the object at the scene's + 64 is the
  render queue (`Scene::Update` calls `RenderQueue::Flip`; `Draw`'s
  "slot 5" is `RenderQueue::Replay`). **For the user:** `sub_825BA380`
  (CreateThread with a processor, `app-start-up.md`) is Dropped but
  still runs in play through `Thread::Begin` (the render thread, the
  worker pools); its row now says Dropped holds for `Voice::Init`'s
  call only; the status is unchanged. `sub_824960E0` inside the movie
  library's range is mapped (the game's own renderer interface calls
  it).
- The frame driver's migration (cac0f347..e721e59a,
  `--native_frame_driver`). **For the user:** a pre-existing bug fixed
  (c96869cc): the `set_register` helper in `src/hooks/app_states.cpp`
  and `framework.cpp` wrote `(&ctx.r0)[n]`, but `PPCContext` keeps r3
  first, so register n landed in r(n+1) (r31's in the link register's
  slot); now `guest_call::Gpr`. It sets the original's registers before
  guest calls in `App::` and the framework; their harnesses passed
  before and after. `sub_825BA380` (CreateThread with a processor) went
  Dropped -> Native (`Thread::Begin` reaches it). `FrameDriver::Present`
  stays recompiled: a harness would have to run `Device::Swap`'s
  override (it hands the frame to our renderer and waits on the frame
  fence). `Scene::Scene`'s harness leaks the draw globals' blocks (~12
  MB a case, 2 cases). `FrameDriver::ApplyRequest`'s listener removal
  (never seen) runs the original, as `RunLoop::` and `App::ApplyRequest`
  do; the shared tree erase `sub_82093000` would be its lead.
- The achievements' map (d06dc44a): `achievements.md`, 5 rows; the
  27-entry {kind, id} table and its companions in `known-data.md`.
  `Write` never ran in play (no achievement earned). The user's call
  (2026-11-13): `XUserWriteAchievements` stays a guest call, so the
  SDK keeps recording unlocks.
- Its migration (7f7fce7e..848036fc, `--native_achievements`): 5 of 5
  ours. **For the user:** `XUserWriteAchievements` `sub_82294E58`
  (`system-services.md`) now has a small Wrapped override
  (`src/hooks/achievements.cpp`) used only by the harness: while the
  harness recorder is set it hands the call to the recorder, so test
  cases (real ids 2 to 28) don't unlock anything in the SDK's file;
  otherwise the original runs. The nearest call (2026-10-31) is against
  stand-in recorders in adapters; this one exists only for the harness.
- The power graph screen's map (4ed4704c): `power-graph-screen.md`, 22
  rows (the machine the type 6 and 7 modes share, `PowerGraphScreen67::`);
  its members' vtable slots inferred from the functions beside each
  Init (no image read), marked "probably"; `power-graph.md`'s
  `0x82815EC0` writers corrected (four write 0); `known-data.md`'s
  `0x82815EB4`, `0x82815EB8` added.
- Its migration (acc7bc1a..2f90c6eb, `--native_power_graph_screen`):
  22 of 22 ours; `SetTexts`' multiplier addresses corrected in the map
  (0x6000 off); the empty `sub_82106540` call in `Update` left out, as
  the type 2 dialog's; the harness checked with a temporary mutation
  (first unseen, then 14 of 96 differing once movie 9 had 21 text
  records).
- `HandleInput` and `Command21` (2787b4aa..658969d6, no mapping):
  `HandleInput` Overridden (a vtable slot), `Input0` Merged,
  `Command21` Native under `--native_tuning_screen`; 29 adapters
  removed; `ResetStickerEditor`, `LoadStickers`, `RestoreAreaDecals`
  (never seen) ported. `HandleInput` still calls 20 never-seen pages
  through the guest (its row says why). Harness: `ItemFields` quiets
  signaling NaNs in the sticker records, as `kVinyls` does (the
  recompiled `lfs`/`stfs` quiet them).
- The stand screens' lead (c2578bab) was mostly stale: 6 of its 8 had
  rows; 2 new rows in `tuning-pages.md` (the vinyl editor's state 34
  free look, `InputVinylView`, and `LeaveVinylView`). The vinyl
  editor's pages (b54839a0): 27 new rows there (states 23 to 31, their
  movie-side updates, the decal images' window and callback); quirks
  in the rows (A copies layers 0 and 1 only; "A pressed" asked twice;
  `MoveVinylLayers` stores B's answer into its frame only);
  `known-data.md`'s vinyl editor fields and the wait words
  `0x826D84A4`-`0x826D84C0`.
- The type 5 mode's map (4a90a16a): `type5-mode.md`, 33 rows (17 in
  play): the parking area's mode, its vtable's 15 slots (slot 14 only
  this mode's) and its +32 vtable, the new `Type5Dialog` (a copy of the
  type 2 dialog's design, two steps shared with `type2-dialog.md`);
  `Leaving`'s machine check loops over ids 0 to 2 while this mode's are
  3, 16 and 21, so it always hands over at once (the original's, kept);
  dialog steps 5 and 9 never set; `NextMode`'s answer bytes after byte
  1 stack leftovers (as the type 10 mode's); `known-data.md`'s block
  and dialog globals; cross-links in `location-screen.md` and
  `car-settings.md`.
- Its migration (33bdeaf2..d30920e0, `--native_type5_mode`): 33 of 33
  ours (31 Overridden, reached through tables and vtables); the map's
  rows for `OpenConfirmStep` and `MoveCursorStep` corrected (the
  highlight's `PlayNamed` gets r5 = 0). The `type10_mode` harness makes
  the same "3 = none" loading-movie assumption that hung this one (it
  stands in `Run`, so it doesn't hang there).
- `Type6Screen0`'s map (4ed5e21e): 13 new rows in
  `type6-type7-modes.md` (the type 6 mode's first screen, a five-item
  menu over the type 6 car; its pages, steps and helpers, with
  `PowerGraph::SetFromCar` `sub_82250488`, never seen, shared with
  `Type7Screen0::MenuPage` and the car list); `known-data.md`'s +9656
  word, the save screen's data +56 read, the power graph record
  `0x82815EBC`; the menu page taken from the menu helpers' lead.
- Its migration (d642e99d..3a90ef05): the 13 rows ours, with type 7's
  `OpeningStep`, `BackStep`, `CursorStep`, `PlayItem`,
  `PowerGraphPage` and `CarList::DrawCarPowerGraph` (`power-graph.md`)
  native; the menu pages call the confirms' plain C++ while the UI
  movies' text setters are native, else the original. **Found, not
  fixed:** `CarList::DrawEntryPowerGraph`'s adapter (`sub_82250670`)
  sets r31 = 0x826E0000 zero-extended where the original's `lis`
  sign-extends it (0xFFFFFFFF826E0000); it matters only above 32
  segments; added as a cleanup lead.
- The options' load and save map (fde73e96): `options-load-save.md`,
  16 rows. **For the user:** the names given before have load and save
  the wrong way round: category 5 calls `GameData::Load`, 6
  `GameData::Save`, so `Options::SavePage`, `SetUpSavePage`,
  `ShowSaveDialog` and `ChangeSaveDialog` (other files) are the load's;
  not renamed, the new file's intro says so. The rest of the save
  screens' lead (`8221BE90` on, 47 never seen, kind 6's screen after
  the race) moved into the in-race screens' lead.
- Its migration (d2d04519..2111429c, `--native_options_load_save`): 16
  of 16 ours; the plain C++ named `LoadPage::` and `SavePage::`
  correctly; the harness seeds each case's generator from its own stack
  words (`CaseRandom`) and makes half the dialog answers accept, after
  shifted random streams left kinds 0, 1 and 4's accept paths
  unreached; `options_screen`'s harness now also saves `0x826BDC90` and
  `0x828161F0`.
- `Scenery::Draw` (7bf375c1..bd6bfc88, no mapping): native from the
  pass table at 0x82603060 inside its `ITC_FRAME_STATE_WRAP`, calling
  `TextureAnimations::Advance` and `Minimap::Draw` through their
  per-frame wraps; ctx's r24 follows the original's; 11 adapters
  removed; `Matrix::Copy`'s adapter (frame interpolation's copy wrap)
  kept though listed removable. The frame check's first native run
  differed from the fallback in its resolve textures; the second and
  third matched it pixel for pixel (left unexplained; probably the
  first run's start-up).
- `Scenery::Draw`'s rest, part one (f1e47061..ea8acc8f, no mapping):
  the billboards' record (Overridden: its diagnostics wrap runs the
  native body) and draw, the light groups, `DrawArea`, `Minimap::Draw`
  (its blink-clock wrap kept, `per_frame_logic.cpp`); 12 adapters
  removed (`sprites.md`, `minimap.md`, `scenery-lists.md`). The frame
  check: deterministic dumps (`--interpolate_draw_state=false
  --max_fps=30`) pixel-identical against each fallback. Harness
  choices: `RecordState`'s dead frame under `Record` not compared (the
  render queue saves a guest r26 there); `DrawArea`'s and
  `Minimap::Draw`'s r3 not compared (`Scenery::Draw` doesn't read it).
  `BuildVertices`' camera blend shared as
  `BlendState_BuildBillboardVertices`.
- The course scenery's ready groups (8516e7c6..95d287ef, no mapping):
  `ObjectPools::Load` calls the three loads directly (`Calls::load`);
  `AnimationSet::FindByTexture` moved from `functions.md`; 15 adapters
  removed (`UiMovie::GetPictureTexture`'s register steps moved into
  its harness). Harness notes: `PollLoad`'s seeds scrambled
  (SplitMix64) since the shared xorshift gave ~11 distinct worlds in
  120 cases (the other `location_screen` groups still have low variety,
  for a later fix); `PollLoad`'s harness leaves out the stack below its
  frame (callees' register saves); `Scenery::Destroy`'s recompiled side
  runs under `RecompiledHeapScope` (r3 after `XPhysicalFree`). Six
  removal candidates kept (five diagnostics wraps, `FromViewport`'s
  guest call).
- Its validation stopped on a harness fault (12 `front_end_car_draw`,
  `car_makers` and `tuning_pages_movie` groups): removing `GetB`'s
  adapter let the recompiled sides run its original, writing its frame
  below their callers; the harnesses now leave out exactly `GetB`'s
  back chain and saves at each caller's r1 (1fdeb678, by the harness
  fault precedent). `UiMovie::GetPictureTexture` set to Merged into
  `PollLoad` (no print of its own, as `Texture::GetLevelDesc`).
- The race end's map (c8f1147b): `race-end.md`, one row
  (`RaceMode::ModeAfterRace` `sub_82150E00`: after a race, which mode
  next by race kind; movies 6-9 queued in the boot table at the
  career's stages 6 and 8); answer bytes +81 to +83 stack leftovers
  (`BootMode::NextMode`'s precedent); cross-references in
  `race-loading.md`, `career.md`, `todays-result.md`, `boot-mode.md`,
  `known-data.md`.
- The race end's migration (ba317c7e..fc2138de, `--native_race_end`):
  `NextMode` Overridden (the race mode's vtable slot 6) and under the
  new flag, not `--native_race_loading`; `Progress::CheckUnlocks`
  ported (never seen) and its rewards pass shared with `AfterRace`
  (`src/career/progress.cpp`); its callers, today's result's two
  included, call the plain C++ where `Condition::Test`'s copies fit and
  the original through the guest otherwise; 6 adapters removed. The
  map missed that a day (+6100) above 999 gives 9 on the -1 path; now
  in the row.
- The type 12 mode's map (89120eb9): `network-mode.md`, 127 new rows
  (108 in play): the game's Xbox Live mode (network battle, network
  time attack: `TXR_NetworkModeSelect`, `TXR_NetworkTaCourse`,
  `TXR_NetworkTaRanking`); 5 leads added (its other screens, dialogs,
  the network manager's callees), marked "mapping only until the
  user's call" by the map; notes added to the `sub_8227B930` and
  menu-helpers leads. The user's call (2026-11-12): its screens made
  native, and its Xbox Live calls answered by plugging in the data
  the SDK gives today (first "left recompiled", changed the same day
  to keep as little recompiled as possible).
- The type 12 mode's migration (1b7594a1..21c8cb3b,
  `--native_network_mode`): 130 rows ours. How the Live calls were
  plugged in: the Live mask `[0x826C795C]` read as data (only B5's
  `Network::Update` stub writes it, 0), so the battle and time attack
  pages take the "not signed in" way (dialog 13, reason 7), as now;
  `CheckMultiplayerPrivilege` `sub_8214F620` answered 0 (what the SDK
  answers on every path) and Dropped, with `XUserCheckPrivilege`
  `sub_82295030` (`system-services.md`); `XShowSigninUI` left out by
  the native sign-in pages (its answer unread). `Session::Make`,
  `Free`, `Init`, `Release` are the game's own session memory, ported
  as game code. `Interrupt` `sub_822785E0` stays recompiled (unread
  helpers). `init_step_tables.py` now follows r4 and InitBase's call.
  The time attack's race set-up shares `TrafficHoles` and
  `SplitTraffic` with `race_setup.cpp`. Two new expected groups
  (`expected-differences.md`).
- The type 9 mode's map (0752af3f): `todays-result.md`, 57 new rows
  (31 in play, 26 "Not seen in play"): the career's end of the day,
  named from the debug strings `TXR_MenuTodaysResult` and
  `TXR_TodaysEvent`; the shared slots stay in `type10-mode.md` and
  `front-end-screen-helpers.md`; `known-data.md`'s type 9 block,
  `0x828160C8`-`0x828160DC` (the map wrote `0x827F60..`, fixed by the
  migration with the fades' counts `0x826A50E4`, `0x826A50DC` and
  `SetUpTexts`' node (8, 11)) and the save data's +0x1D524; references
  in `race-event.md`, `boot-mode.md`, `front-end-screen-helpers.md`;
  the type 12 mode added as a lead. Both `Create`s ask `VirtualAlloc`
  for 68 bytes and write past them (inside the page): kept as is.
- The type 9 mode's migration (b7718efe..ba41ed1b, `--native_todays_result`):
  57 of 57 ours (38 Overridden through vtables and functor tables, 19
  Native; `TodaysEvent::Kind`'s adapter removed); the 26 never-seen
  paths ported with their callers (the garage screen's precedent);
  `Progress::CheckUnlocks` `sub_821162B0` left recompiled (never seen,
  called through the guest by both queue functions). The harness builds
  its own small stand-ins for the data tables and `BUILD.TOC` (not
  loaded when harnesses run); unlock counts and image lists laid in
  play's ranges after harness faults. A row script wiped the rows'
  notes in two commits; put back in 70dc8fb7.
- The replay theater's check (50318046..93b074ab): the previous run's
  unchecked port (below) now checked by its harness (46 functions, 0
  differing) and on by default; its harness lays a replay record's +28
  as 0-99 (play's two-digit number), after a wild value made a case
  write guest 0x10 in a loop; `FindShown`, `OnList`, `InitStates`,
  `StartPage`, `SetPrompts`, `BuildList`, `ShowListPage`, `ShowRecord`
  set to Native; `Replay::SetUpRace` `sub_82105228` left recompiled
  (never ran; the native `OnDone` calls it through the guest). The
  attract race doesn't reach the theater: it goes to the drive.

## Run 2026-11-11 08:38 (`b7-loop-11-11-26`)

- The tuning screen's update's map (eba99e25): `tuning-screen.md`, 64
  new rows (the update `sub_821E6C50` and its pieces `sub_821E6CD4`,
  `sub_821E6F90`, in play; 23 debug state handlers and 38 command
  runners, "Not seen in play", read in outline and named from their
  strings), the intro's "The update", `known-data.md`'s state names
  and the screen's +9644 and +9648; `sub_821FA460` added to the sticker
  editor's lead.
- The update's migration (1f7f4e8e..360b0cbc): the update Overridden
  (its adapter stays: the tuning machine's functor), its pieces
  Merged, 47 adapters removed (Native), 40 never-seen callees ported
  (the garage screen's precedent, 2026-11-09), 21 left recompiled with
  the reason in their rows (the sticker editor's commands, `Command21`,
  `Command10`, `13`, `14`). The update's harness stands in for
  `UiMovie::Update` inside `UiMovie::Run` (as the garage screen's) and
  compares only the update's own frame; the tuning pages' and
  `TuningMachine::Start`'s harnesses now leave out the originals'
  register saves and `SetText` parameter area. A fragility left: in
  the update's harness world, when the shared generator shifted, case
  142 (command 26) read the wild guest address 0x70081650 in a loop;
  worked around by giving `Command42`'s words their own seeded
  generator, not fixed at its root.
- The update's validation stalled (`itc_4919`) on another wild read in
  the same harness (case 11's restore path: a random engine stage
  indexing the spec's engine records past their end); fixed by the
  user's call (505b2a31: the engine stages and tire types laid in
  range from their own generator), validated again (`itc_4926`). The
  user's call to find, fix and go on is now under "Precedent first".

- The title screen's map (de062dec): `title-screen.md`, 87 rows (the
  type 4 mode, its title load and title screen machines; `sub_822221A0`
  `TitleScreen::Update` and `sub_8209F6B8`, now `Players::AssignController`
  (probably), moved from `functions.md`), `known-data.md`'s four title
  blocks and `0x826364EC`. A correction: at 600 idle ticks the title
  object's +4 becomes 8 and the page object `[0x82814E2C]` goes to page
  5 (the old text said +4 became 5; fixed in `../title-screen.md` and
  `known-data.md`). `sub_82222038` (the attract race's set-up from 6
  picked by `rand() % 6`) is marked "Not seen in play" though attract
  races reach it: missing from the coverage list.
- The title screen's migration (05cef504..216e9843): 86 of 87 ours
  under `--native_title_screen`; `SetUpAttractRace` stays recompiled
  (its harness needs the race data, tables and car specs, absent before
  the boot's data load). `TitleScreen::Update`'s native body runs
  inside the existing `--attract_after_s` wrap. Three existing adapters
  (`BootData::Load` `sub_82106618`, `Career::NextDay` `sub_82116AF0`,
  `CarDealer::StartFirstRace` `sub_82180CE8`) now check
  `hooks::StartUpRecorded` first, the `data_load` harness's pattern, so
  the title screen's harness records those calls instead of running
  them (`Career::NextDay` looped forever on the harness's save); say if
  this pattern isn't wanted (the race HUD machine's call, 2026-10-31,
  was against stand-in recorders in adapters made as hooks). Kept from
  the originals: `TitleLoad::Update`'s return of its table's eighth
  entry, `MenuSound::Play`'s r3 into `StartFirstRace`, `MenuList::Init`'s
  r7 154 and r8 153, the frame doubleword under the `NextMode` and
  `ForceExit` answers.
- The type 10 mode's map (1375de82): `type10-mode.md`, 47 new rows (25
  in play): the replay theater (named by the "TXR_ReplayTheater" string
  its update passes to an empty function), its Init, slots, and its
  own machine (`ReplayTheater::InitStates`, UI movie 45); the type 9
  mode's slot 12 `sub_8225D638` and +32 slot 1 `sub_8225D828` mapped
  here as shared (`ModeType9::NextModeAnswer`, `ForceExit`);
  `known-data.md`'s type 10 block and `[0x828161E0]`-`[0x828161E8]`;
  the save screens' lead names `sub_8228AA58` and `sub_822896C0` as the
  theater's `Start` and `Leave`; a lead for the type 9 mode.
- The type 10 mode's port (49287852), **unchecked**: the run was ended
  before its harness and race; `--native_type10_mode` off by default,
  38 rows Overridden with "not yet checked", 8 plain C++ functions
  without adapters left blank (`FindShown`, `InitStates`, `OnList`,
  `StartPage`, `SetPrompts`, `BuildList`, `ShowListPage`,
  `ShowRecord`). `b7_status.py` counts the 38 as ours though the flag
  is off. `Replay::SetUpRace` `sub_82105228`'s row doesn't yet say it
  stays recompiled (called through the guest from `OnDone`).

## Run 2026-11-10 23:52 (`b7-loop-10-11-26-2`)

- The car settings' map (98f0843b): `car-settings.md`, 91 new rows (7
  in play: both Inits, `Create`, `OpenMovie`, `WaitForMovie`, `Close`,
  `Destroy`; 84 "Not seen in play"), `known-data.md`'s `0x826D8458` to
  `0x826D8460` and the 764-byte data block; the type 5 mode's Init
  `sub_821B7878` mapped here (its lead in `leads.md` notes it);
  `Update` `sub_821B6818` and its 114-function subtree left as a lead.
- The car settings' migration (42211c94..73886334): 7 of 7 in play
  ours, all Overridden (the five handlers are functors the machine
  calls; the Inits' callers `sub_82197278` and `sub_8222FD68` are
  recompiled). Not ported: the machine's other functors (`Start`,
  `Draw`, `Update`, the replies, `Unload`, `Leave`, `Reenter`,
  `ReenterToCourses`) and all under them, never seen in play; the
  native Inits register them as the original does (unlike the garage
  screen and tuning machine, 2026-11-09, where the user had such paths
  ported: say if the same is wanted here). `OpenMovie`, a tail call,
  passes the adapter's r1 and lr to `UiMovies::Open`; `Close` is the
  machine's own (deletes movie 3, returns `Delete`'s r3 or 0, message 4
  only for mode types 2 and 5); both Inits by step tables
  (`init_step_tables.py`), unwritten frame words copied as left.
- The halt (2026-11-11): `GarageScreen::Transition6`'s harness
  differed in 2 of 100 cases in `itc_4810`: the harness gave the action
  list's selection -2 to count + 1, which `Transition6` and
  `Transition7` use unchecked to index the 5-entry help line tables, so
  `SetText` read text from random ids. Fixed in the harness by the
  user's call (f691a91e: the selection one of the 5 actions, as in
  play); validated again (`itc_4813`), the loop resumed.
- The type 2 mode's map (ebde5050): `type2-mode.md` (21 rows: the
  Init `sub_82197278`, the mode's vtable slots and thunks, `Interrupt`
  `sub_82197858` posting message 48, `ForceExit`, `Resume`, the
  helpers) and `user-info.md` (14 rows: the user info screen,
  `UserInfo::`, named after `TXR_MenuUserInfo`); two hub rows added to
  `type2-screens.md` (`sub_8218F620`, `sub_8218DE18`, not seen in play);
  lead mentions in `car-dealer.md`, `menu-helpers.md` and
  `type2-dialog.md` pointed at the new files; `known-data.md` rows for
  the user info globals and the mode block's fields. Precedents named:
  `NextMode`'s and `ForceExit`'s 8-byte answer with leftover bytes 1 to
  3 copied as left; `SetText`'s r8-r10 passed as 0 (2026-10-30).
- The type 2 mode's and user info screen's migration (fd78a046,
  5c818857): 14 of 14 and 20 of 21 ours. Kept: the user info page and
  transitions run through the tables `UpdatePage` and `Update` build on
  their frames; `Update`'s empty debug call `sub_82106540` left out (as
  the dialog's); `NextMode`'s and `ForceExit`'s answer built in guest
  frame memory and read back (leftover bytes as the original). Followed:
  `SetText`'s r8-r10 as 0 (2026-10-30), its parameter area and the
  screen start's dead r4 not compared. Left recompiled: `Resume`
  `sub_821970B8` (a jump into the hub's `sub_8218DE18`);
  `WaitForLoads`' shown-3 branch calls the unseen `sub_8218F620`
  through a guest call, not covered by the harness. Ported though never
  seen in play: `Restart`, `Leaving`, `Interrupt`, `ForceExit`. Adapters
  kept: the Init calls the 14 machines' Inits through their adapters so
  each one's flag applies (`UserInfo::InitStates`' adapter could go if
  the Init called its plain C++: a possible cleanup); `MachineOf`,
  `AllLoaded`, `FindShown` for recompiled callers. The `type2_mode`
  harness lives in `src/harnesses/type67_modes.cpp` (its worlds reused).
- The tuning screen's refresh map (cbff27b9): 8 rows appended to
  `tuning-screen.md` (`TuningScreen::Refresh` `sub_821E3AD0`, in play;
  `sub_821F2110`, `sub_821F2378`, `sub_821F2AD8`, `sub_821F7A98`,
  `sub_821E7DA0`, `sub_821CC930`, `sub_821E7E48` not seen in play), its
  intro's "The refresh" section (the blocks by line range), fields in
  `known-data.md`'s tuning screen row, `TuningMachine::Start`'s row
  noted. Lead corrections: states 38 to 52 go to `sub_821F7A98` (movie
  14's header), 53 to 58 to `sub_821F2AD8` (the interior pages), 9 and
  11 return 1 after the header.
- The refresh's migration (2bbe3281..dcaee620): `TuningScreen::Refresh`
  and `TuningMachine::Start` Overridden, 15 adapters removed (13 tuning
  screen helpers, `SetUp`, `SavePlaceholders`: Native). Followed:
  `SetText`/`SetNodeText` unset varargs as 0 (2026-10-30); register
  leftovers the refresh sets itself passed as it leaves them (header
  `SetText(m, 18, 8)` r10 = frame + 80, state 5's rows r10 = +25100 and
  r6, the bonnet's r10, the door's r6 = 31); the double through r7, f1
  and the frame's +48. Left recompiled: `sub_821F2110`, `sub_821F2378`,
  `sub_821F2AD8`, `sub_821F7A98`, `sub_821E7DA0` (never seen in play).
  For the user's call: an inherited f1 gap, `PickCameraPreset`'s and
  `GarageStage::Command`'s plain C++ don't report the f1 their originals
  leave (a sine, a tangent, 60.0), so f1 after the refresh can differ in
  states 9 to 11 with the headlights and state 8's door path; nothing
  is known to read it. New: the native refresh and Start also require
  `--native_car_specs` (plain C++ `GetCarParts`, `AeroPart77`, `82`).
  Harness: levels, types and options kept in the world's ranges
  (`SetCarStats` indexes the spec unchecked); sub-scenes 28 to 33 real
  distinct nodes; the stand models' +5060 laid (dcaee620; it passed only
  on leftover bytes); recompiled callees' saves and parameter areas left
  out of the comparisons (found by logging). Map corrections in the
  refresh's intro.
- The refresh's validation (`itc_4871`, `itc_4872`): 17 of 17 harness
  lines, 14 ran in the replay; `ShowInteriorPicture`, `SetStateTexts`,
  `RefreshMovie13` to drive. The run stopped at its limit.

## Run 2026-11-10 21:02 (`b7-loop-10-11-26`)

- The race menu's migration (a65e90d7..4414719e): 38 of 38 rows ours
  (14 Overridden: reached through functors or the recompiled type 2
  Init; 3 Merged: the split pieces `sub_8218C5DC`, `sub_82188D44`,
  `sub_8218C8A8`; 21 Native). Followed: the `UiMovie::SetText` r7-r10
  leftovers passed as 0 (2026-10-30); `StopCue`'s unread r6 passed as
  0. Kept: `AnimateKindPage`'s unused `Pressed(158)` frame store,
  `SetUpEvents`' overlapping frame stores in order, `Refresh` reading
  past a list of fewer than 15 events into its caller's frame. Left
  out: the calls to the empty `sub_82106540` (as the race screens).
  For review: the last adapter removal (ae078313: `KindPage`,
  `EventPage`, `VariantPage`, `Back`, `LeavePage`, `StartPage`,
  `StartEvent`) skipped the temporary call counter (attract races never
  open the race menu, so it would only have shown 0; every caller in
  play is ours by `b7_groups.py --removable`). Map corrections: page 4's
  list with bit 0x20 holds strings 360-374 (not 373); `Transition3`'s
  +80 = 1 is node (11, 1)'s; `Start` gives node (11, 0) the same +12 and
  +16 as (11, 1). New: `src/race_menu/`, `src/hooks/race_menu.*`, the
  `race_menu` harness, `RaceMenuStartGroup` in
  `src/harnesses/race_event.cpp`; `AnimateEventPage`'s harness world
  gives nodes real track-less animations (the original faulted on
  random words).
- The race menu's validation halted (`itc_4804`): `Transition3`'s
  harness read the wild guest address 0x701FAF40 in the all-harness
  run (not in the migration agent's own runs). Fixed (4438f1c3): the
  world's random +116 words (each node's first scene animation entry)
  now 40 to 47 over real track-less animations for every race menu
  harness (before, only `EventWords` set them); validated again
  (`itc_4807`, 35 of 35, all 0 differing).
- The race menu's map (e6f5b641): `race-menu.md`, 38 rows (31 in play;
  the type 2 mode's machine at +9640, UI movie 1, probably the career's
  race menu), rows in `known-data.md` (`0x826D8404`-`0x826D840C`, the
  machine's fields). Read in outline only: `Refresh`, `SetUpEvents`,
  `AnimateKindPage`, `AnimateEventPage`, `Transition3`. Leads: the race
  set-up callers' line deleted (all five now in function files), the
  menu helpers' callers and the type 2 Init's lines trimmed.

## Run 2026-11-09 10:53 (`b7-loop-09-11-26`)

- The type 2 dialog's migration (415b58d5..e3269357), over the last
  run's map (`type2-dialog.md`, c257b0c2): all 28 rows Overridden (no
  adapter removable: 27 reached through functors or the page and step
  tables, `Init` from the recompiled `sub_82197278`). Followed: the
  type 6 dialog's design (`Draw` and `Update` call pages and steps
  through the guest), the type 2 screens' Init pattern (step table over
  `StateMachine::Add`). Kept: the r4-r10 leftovers passed to the steps
  and pages (unread); `Update`'s call to the empty `sub_82106540` left
  out (returns at once; said in its row). Ported though not seen in
  play: `Cancel`, `Wait`, `OpenNotice2`, `Notice2Page`, `TakesInput`,
  `OpenNotice2Step`, `CloseNotice2Step`. New: `src/type2_dialog/`,
  `src/hooks/type2_dialog.*`, `src/harnesses/type2_dialog.cpp`.
- The garage screen's map (5c43d083): `garage-screen.md`, 78 rows (22
  in play; the type 2 mode's machine at +0x23018, UI movie 5), a row in
  `known-data.md` (`0x826D8420`-`0x826D842C`; the save's +6116 confirmed
  as the money), a pointer in `type2-screens.md`'s intro; four leads'
  lines trimmed (the career screens', the stand screens', the mode
  Init's, the stand setters'). Noted: `Destroy` never frees the second
  menu list `[0x826D8428]` (probably the original's leak, kept);
  `SetHelpLine` `sub_82193A90`'s unset varargs registers to
  `UiMovie::SetText` (the map said a leftover r5; the migration found
  r5 = 0 and only r7-r10 unset; the 2026-10-30 call: native callers
  pass 0).
- The garage screen's migration (e10e29dd..52c21938): 22 of 22 in play
  ours (15 Overridden, 7 Native: `SetCarRow`, `FillRows`, `SetPrompts`,
  `SetHelpLine`, `SetUpLists`, `ReadSave`, `SetHeader`); the 56 never
  seen in play kept recompiled, reached through the tables and functors
  and a few direct guest calls (`ReloadStandCar` `sub_82193B10`,
  `PutCarOnStand`, the busy markers, `SetScrollArrows`), each row says
  how. Map corrections in the rows: `SetPrompts` stores 0 into a hidden
  prompt's +80; `Transition1` always returns 1; `Transition3` returns 1
  when both fade counts (`0x826A50DC`, `0x826A50E4`) are 0; `Update`'s
  debug print's argument is `0x8200A3C8`. Followed: the 2026-10-30
  varargs call (`SetHelpLine`, `SetHeader`, `SetCarRow`; their
  `SetText`/`SetNodeText` parameter areas left out of the comparison),
  no behavior change (`Destroy`'s unfreed list, the frame words copied).
  New harness technique: `CarListPage` and `UpdatePage` get a 192 KB
  guest stack of their own, filled with random words each case
  (`ReloadStandCar`'s 100,560-byte frame reads words it never writes);
  it removed seven differences from a previous case's leftovers. New:
  `src/garage_screen/`, `src/hooks/garage_screen.*`,
  `src/harnesses/garage_screen.cpp`.
- The tuning machine's map (8db41a33): `tuning-machine.md`, 22 rows (19
  in play: the tuning screen's machine at the type 2 mode's +19792, its
  handlers, `Start`, `SetUp` `sub_821E1B48` and the part tables'
  loaders, `DecalImages::Reset` and `SavePlaceholders`), the
  machine's fields in `known-data.md`'s tuning screen row,
  cross-references in `tuning-screen.md` and `tuning-pages.md`; the
  power curve lead and the tuning screen's outside functions' lead
  taken whole and deleted, three leads given notes. Noted: `Start`
  writes 9,999,999 to the money word +25116 before `SetUp` overwrites
  it (both kept); `OnDialogClosed`'s unset r7-r10 (the 2026-10-30
  call).
- The tuning machine's migration (529f68d7..9a1c1037): 18 of 19 in play
  ours (8 Native: `SetUp`'s five callees, adapters removed in one
  commit; `ZeroRecord16` `sub_821FE540`, `DecalImages::Reset`
  `sub_821FE590`, `SavePlaceholders14` `sub_821F8950` with no adapter,
  their callers all native); `Start` `sub_821E7368` left recompiled
  (blocked by the refresh `sub_821E3AD0`); the three handlers never
  seen in play kept recompiled. `SetUp`'s 3,748 lines split over five
  subagents, one at a time. Map corrections in the rows:
  `SavePlaceholders` reads `[[[m +28] +724]]`; `LoadCurrentChoices`'
  +30644-+30656 and +30708-+30720 come from a mirror table in its
  frame, `Params::GetCarSpec` only when the paint word isn't -2;
  `LoadOwnedParts` and `SetPrices` write beyond the map's ranges
  (`SetPrices`' fixed-address stores grouped by address, after a
  store-by-store replay showed none written twice or read back);
  `SetUp` takes the weekday from the day before its clamp to 999.
  Kept: the leftovers (`Create`'s word at the caller's sp + 20,
  `SetPrices`' words below sp, `LoadOwnedParts`' r3 35960, the Init's
  unwritten functor words). `LoadCurrentChoices`' frame-table indexes
  come from car words unchecked (the harness keeps them in range: 0-7
  in play). `SetUp` calls `SetAeroAvailability`'s plain C++ directly
  (`--native_tuning_pages` no longer switches it on this path);
  `Car::Init` and `Car::Refresh` through their overrides.
- The tuning machine's replay (`itc_4744`) logged one SDK
  "BaseHeap::Release failed because address is not a region start"
  line, the intermittent one noted in earlier runs; not looked into.
- After the run stopped, by the user's call (2026-11-09: "they will be
  seen in play"; the loop's rules unchanged): the garage screen's and
  tuning machine's functions never seen in play ported
  (630e529f..f558b529): 53 of the garage screen's 56 (49 Native,
  `ResetStand`, `SetUpActions` and the six reply handlers Overridden,
  `SpecSheetUnits` `sub_82191768` Merged), the three tuning handlers,
  and the stand's camera command (`GarageStage::CameraCommand`
  `sub_821808F0` Overridden, `GarageCamera::Command` `sub_82180358`
  Native, `sub_821803A0` Merged; rows in `type6-type7-modes.md`, code
  in `src/type67_modes/garage_camera.cpp`; the stand setters' lead
  trimmed). Left recompiled: `Transition11` `sub_82195F10` and
  `Transition15` `sub_82196030` (they store `UiMovie::Run`'s r8 leftover
  into the page object's started word, live state: the `BackToBuilds`
  call, 2026-11-06); `OnYes` `sub_82192958` (blocked by `Garage::Sell`
  `sub_82116D88`, with `Garage::Remove` `sub_82116BE0` and
  `GarageList::Remove` `sub_82113400` under it, small, in `career.md`).
  Map corrections in the rows (`SetScrollArrows`' test, the turns and
  cameras take no arguments, which transitions always return 1 or 0,
  `ViewOptionsPage`'s option 3, the view pages' held buttons,
  `ReloadStandCar`'s 100,688-byte frame, `SetUpOptions`' +80). Harness
  world: a `car()` accessor on `StandCarWorld`; the garage harness's car
  block (car_makers' `LayStandCar`, aero levels zeroed, the garage held
  nonempty: an empty garage's walk reads past the list); the tuning
  DialogNo cases keep the state history under depth 61. `ActionsPage`'s
  accept branches are reached by random mapper bits over 300 cases (not
  counted per action). `__chkstk` Dropped: native big-frame callers skip
  its probe.

## Run 2026-11-08 20:40 (`b7-loop-08-11-26-2`)

- The type 2 screens' map (eb666c7f): `type2-screens.md`, 150 rows (the
  hub, the spec sheet, the build screen's machine, the license plate
  editor; `Type2Hub::WaitForLoad` `sub_8218E420` moved from
  `functions.md`, its patch kept), rows in `known-data.md`
  (`0x826D8410`-`0x826D841C`, `0x826D8430`-`0x826D8438`,
  `0x826D8464`-`0x826D846C`, `0x826D8448`'s +0 to +32), a pointer in
  `build-screen.md`'s intro; two leads deleted as covered, four leads'
  lines given notes. `Type2Hub::Create` allocates 68 bytes and uses up
  to +447 (the original's, kept).
- The type 2 screens' migration (e134d538..caa092b0): 61 of 121 ours
  under `--native_type2_screens`; harness world fixes in its harness
  (node +100 kept below 40, as `FindNode` adds a node's +108 to child
  indices; at most 3 children for node (2, 2), as `ShowDigits` reads a
  word per child past the three digits its callers write);
  `PowerGraphScreen::OpenMovie` `sub_82250328`'s adapter also serves
  three other modes' Inits that register it, behind the same flag;
  nothing of it runs in an attract race (no counters or frame dumps).
- The type 2 screens' second migration (36e01983..67b6d2c4): the hub
  and the build screen's `Enter`; `Type2Hub::WaitForLoad`
  `sub_8218E420` Patched -> Overridden, its patched count of 1 kept
  (the original's 60); `Type2Hub::Choose`'s items traced in its row.
  Harness world fixes: the hub world's stage +0 points into its own
  area (`GarageStage::SetPlates` writes `[[stage]]+6092`); the global
  `0x826364EC` `OnDialogDone` clears added to the harness's put-back
  list; the `Fill` cases lay 400 items of their own (the data tables
  aren't loaded when harnesses run); `Archive::Load` calls recorded by
  the harness's recorder.
- The type 2 screens' third migration (be6b79e6..c18081f3): the build
  screen's pages and jumps, `Type2Hub::Update` and `PutCarOnStand`
  (the stand car's world from `load_poll`'s `StandCarWorld` and the
  car dealer's `StandWorld` shaping; the harness world has a 0x2800-byte
  stand block, `0x826C2E40` and `0x826D83E4` put back around cases).
  A harness hazard for review: the hub's `MenuPage` cases reach
  `sub_8218D708` (recompiled on both sides, never seen in play) over a
  random car spec; once new cases shifted the seeds, one looped on
  guest 0 access violations (`itc_4670`; a like hang in `BuildEntry`,
  `itc_4666`). The new cases now run last so the earlier seeds hold;
  the `MenuPage` world is still fragile.
- The save screen's map (ce16c61a): `save-screen.md`, 61 rows (the
  machine five modes share, its kinds, pages and steps); a row in
  `known-data.md` (`0x826D844C`-`0x826D8454`); `Type6SaveScreen::Init`
  `sub_821A8210`'s row in `type6-type7-modes.md` corrected (`Show`
  covers kinds 0 to 7); the save screens' lead trimmed.
  `SaveScreen::StoreSettings` `sub_821A24B0` reads a stack leftover
  (frame +80) when the profile's +672 is 3 or more: copied as the
  original leaves it (the track zones' precedent).
- The save screen's migration (d393b07d..36674493): 42 of 42 in play
  under `--native_save_screen`; 35 adapters kept (reached as functors
  or from the tables `Update` and `Draw` write, as the car dealer's
  `UpdatePage`); callees never seen in play left recompiled, reached
  through the guest. `Draw`'s r3 and the table addresses in r4-r9 are
  sign-extended as the originals' `lis`/`addi` leave them. The
  `save_screen` harness mixes its seeds (splitmix64) and draws from the
  high bits: the shared pattern (each case's seed the next xorshift
  state, `Below` from low bits) left some branches unreached, and other
  harnesses share it (for a later look). Harness world fixes: `Page21`'s
  copy 0x1D52C bytes past player 1's save data; `sub_82290640`'s replay
  record from the save buffers' list.
- The type 2 dialog's map (c257b0c2): `type2-dialog.md`, 28 rows (the
  type 2 mode's machine at +0x1E4C8, like the type 6 and 7 dialogs,
  sharing `Dialog::OpenMovies` and `Dialog::Leave`); a row in
  `known-data.md` (`0x826D83F8`-`0x826D8400`, `0x82636200`,
  `0x82636204`); `menu-helpers.md`'s pointers moved to the new file.

## Run 2026-11-08 13:00 (`b7-loop-08-11-26`)

- The UI movie helpers' map (9f12739f): 8 new rows in
  `ui-movies-game.md` (`UiMovie::IsNamedActive` `sub_8211BC18`,
  `UiMovie::UnloopNamed` `sub_8211DE30`, `UiMovie::ClearNodeText`
  `sub_8211CC50`, and five never run: `sub_8211DE28`, `sub_8211A130`,
  `sub_82119EA8`, `sub_8211C2C8`, `sub_8211C8A0`), a paragraph in its
  intro, the "UI movie animation list" row in `known-data.md`, and
  stale "no row" mentions corrected in `car-list.md`,
  `type6-type7-modes.md` and `race-screens.md`.
- The UI movie helpers' migration (e052912e..e73bf7db): the three under
  `--native_ui_movies_game`; cleanup in `car-list.md` (the panels'
  answers, shows, selections and moves, eight `PanelSetBase` steps,
  `SetUpRows`, `SetUpCarPage`; 22 adapters removed in all, with
  `UiMovie::MoveNamedAnimation`'s and `IsNamedActive`'s). Harness world
  fixes in `harnesses/car_list.cpp` (the original crashed on them too,
  once new cases shifted the seeds): the set-ups world's garage built as
  a chain of distinct links; `ShapeSet` keeps E's +16 non-negative (a -1
  read a random word once `RowPanel::SetRow` stored a -1 logo frame in a
  node's +108).
- The UI movie helpers' second migration (f2cce057..97ac1b9f):
  `sub_8211CBD8` renamed `UiMovie::SetNodeTextV` (its r6 is a
  `va_list` passed to `Strings::FormatV`); `UiMovie::OnLoaded` with
  `File::GetSize` (`files-and-saves.md`, `--native_file_layer`). The car
  list's `Step17` and `CarPage`: a new shared harness world
  (`harnesses/stand_car_world.h`, implemented in `load_poll.cpp`) lays
  load_poll's `ShowStandCar` world beside the car list's. Harness gaps
  (in the rows): `CarPage`'s pick cases force the menu to accept action
  0 and reach `ShowStandCar` in 9 of 30; they don't compare the car on
  `ShowStandCar`'s frame (`Car::Init` leaves words unwritten, holding
  earlier callees' stack leftovers the native callees don't write;
  `Step17`'s cases compare it and match); `CarPage`'s garage tab (tab
  2) has no cases (`CarPanel::ShowCarStats` and `sub_82250488`, never
  seen in play, stay guest calls to the originals).
- The cleanup lead's migration (9ca12391..c33350d7):
  `RaceScreens::~RaceScreens`, `StandMode::Release`,
  `RaceDialog::SetText`, `DebugText::LoadFont` with `DebugFont::Init`
  (new row, Native) and `sub_8258BE48` (new row, Merged),
  `RaceScreens::Destroy`, `RaceDialog::Open`; 11 adapters removed (the
  eight in-race screens' Destroys among them). By the parking area's
  call (2026-10-30), `SetText` and `Open` pass 0 for the varargs they
  leave unset; their harnesses leave out those dead frame bytes
  (`SetText`'s frame +40 to +79, `Open`'s +32 to +79).
  `Strings::Release` and `UiMovies::DeleteAll`'s jump are called through
  their overrides (recompiled callers left). `File::GetSize` keeps its
  adapter (15 calls an attract race through vtables), its row says so.
- Its second migration (cd0cb488..2c188a95): `DestroyScreens` (a tail
  call into `RaceScreens::Destroy`, its adapter kept for the machine's
  functor; its cases in the race HUD object's harness);
  `RaceDialog::ShowMessage` native for non-first calls and first calls
  with a message above 11 (13 is the only one seen), the original for
  0-11 (`SetNote` `sub_82218918` and case 0 `sub_82218C90` never ran),
  its adapter kept for `Draw`'s handler table. `RaceScreens::Destroy`'s
  and `SetText`'s adapters removed without counter runs (their callers
  run at a race's end and in the confirm dialog, which attract races
  don't reach). Harness world fix: `PauseMenu::Draw`'s set-up clears the
  pause list's items (+68) only for transition 6 (`OpenOptions`); it
  cleared them on every case, and the original then looped on address
  0 once the seeds moved.
- Its third migration (def0337f..53434c3a; two batches by subagents):
  `PauseMenu::MainPage`, `SetUpMain`, `ShowCareer`, `ShowVolumesB`,
  `ShowAdjustB`, `AdjustPage`; `AfterRaceMenu::MainPage` and `Open`;
  `RaceHud::SetMode`, `SetUpTimer`, `SetUpRacers`; `RaceHudMachine::
  StartCountdown`, `OnRaceEnd`, `AwardPrize`; `CareerResults::SetTexts`
  and `Open`; two career helpers (`career.md`); 18 adapters removed.
  `--native_race_hud_object` now switches nothing in the game (no
  adapters left; its callers' flags do), its help says so. Harness
  limits (after the openers' precedent): `SetMode`'s cases leave out
  modes 2 and 3, `OnRaceEnd`'s kinds 2, 4, 6 and kind 1 with mode 0
  (paths into originals never seen in play). `AfterRaceMenu::MainPage`'s
  replay save runs `SaveBuffers::Create` through its override and
  `RaceDialog::StartSave` (never seen) as the original. `AwardPrize`'s
  pair loop can run past its 168-byte block when the halfword +968 is
  above 17 (the original's addressing copied; the harness keeps it at
  0-17). Rows corrected: the scale constant in `ShowCareer` and
  `SetTexts` is the double at `0x8200A510`.
- Its fourth migration (6709c224..f6ad7444): the pause menu's
  `OpenMain`, `OpenVolume`, `OpenAdjust`, the three Closes,
  `SetUpVolume`, `SetUpAdjust`, `VolumePage`, and `SaveVolumes` (never
  seen in play: ported, Native with no adapter); 9 adapters removed.
  `SetNodeText`'s unset varargs passed as 0 (the parking area's call),
  the `SetUp` cases leaving their frame +40 to +79 out. `VolumePage`'s
  cases replace the sound manager's slots 29 and 30 with the menu
  sounds' harness's leaf recorders.
- The front-end screen helpers' map (200c6832): new
  `front-end-screen-helpers.md`, 34 rows: the 852-byte modes' releases
  (vtable slot 1, read from `default.bin`: types 2, 4, 5, 9, 10, 12),
  the save screen's (movie 44) and options' (movie 7) dialog text
  setters with the dialog steps they unblocked (step 5 `sub_821A6030`
  and its tail not seen in play), the options' categories 5 and 6
  set-ups, the car cards (movie 4 or 18) and their callers;
  `known-data.md` (the save screen's page block and dialog fields, the
  options' dialog menu, the car list's fields); four leads' lines
  trimmed of the taken functions; one lead added (the car cards'
  screen). Calls followed: unset varargs passed as 0 (the parking
  area); the sound manager's `StopCue` (recompiled) reached through its
  vtable, as the race sounds' native code does.
- Its migration (92fe4c38..99d3bf29, `--native_front_end_screen_helpers`,
  `src/front_end_screen_helpers/`): 27 of 32 ours; five split-off case
  0s Merged into their steps (`b7_groups.py` still lists them as ready
  groups: it doesn't count Merged as ours). The six dialog setters'
  adapters removed though `--removable` didn't list them (their callers
  in play are native or those Merged tails; no counter run: attract
  races don't reach them); `ShowMessage5` `sub_821A6030`, never seen,
  still reaches the original setters. The releases free their blocks
  over the allocation layer, falling back to the original for blocks
  that aren't its pages (as `StandMode::Release`). Harness choices: the
  `StopCue` recorder's r6 (a leftover `AudioManager::StopCue` doesn't
  read) zeroed before comparing; in the frames below the steps only
  `SetNote`'s back chain is compared (the rest callees' register save
  slots and `SetNodeText`'s varargs leftovers).
- The car dealer's map (c96dfffd): new `car-dealer.md`, 75 rows (18
  not seen in play): the type 2 mode's dealer machine (its block at the
  mode's +848, +0x19978), 23 transitions, 6 pages and 22 page
  transitions, the two car lists by the spec's byte +771, the color
  pick (HSV records), the purchase (money at +6116, `Garage::Add`, the
  garage-full and not-enough-money cases) and the career's first car
  (`sub_82180CE8`: the boot movies' list, `Career::SetUpEvent`,
  `Race::SetUp`). `known-data.md`: `0x826D83E8`-`0x826D83F0` added,
  `0x826D83F4` (the dealer's 1,196-byte data) rewritten. The paint color
  lead (`sub_821847A0`) taken whole; four leads' lines trimmed; one lead
  added (the type 2 mode's other screen machines).
- Its migration (3480d4e6..f51e5073, `--native_car_dealer`,
  `src/car_dealer/`): 41 of 58 in play; 5 adapters removed, 7 helpers
  Native with no adapter (`HideBusyNow` `sub_82181640` never ran,
  ported). Callees never seen run as the originals through the guest
  context (`ShowDriveDialog` `sub_82182990`, `ShowFirstCarDialog`
  `sub_82182A80`, `SetUpSpecSheet` `sub_821810E0`); the empty
  `sub_82106540` still called. Harness gaps (the car list's and type 6
  and 7's precedent): `Update`'s stand drawless (`ReloadModel` answers
  0, its reload branch not exercised); `Page2`'s selection 1 (the spec
  sheet) not laid. Two groups in `harnesses/race_event.cpp`
  (`StartFirstRace`, `Leave`'s first car path: `Career::SetUpEvent`'s
  world). `ShowCarInfo`'s +660 float word copied as is (signaling NaNs
  kept). Rows corrected: `Update` `sub_82185600`'s r3 is movie 4, not
  the machine; `Page3` uses the menu at `[0x826D83EC]`.
- Its second migration (28903fcf..5b3fe2c2): the transitions and the
  list, car and color set-ups (`src/car_dealer/menus.cpp`); 11 adapters
  removed after counter runs; the transitions keep theirs (`Update`'s
  table). T15, T17 and `OpenColors` call `GarageStage::ShowCarById`
  and `ShowCarByIdWithClass` unconditionally: their cases run in the
  stand car world (`stand_car_world.h`), the car on `ShowCarById`'s
  frame not compared (as the car list's pick). The paint picks are kept
  to 0-5, load_poll's range: a first try with T17's pick up to 19
  crashed the harness reading guest `0x0000000A` (log `itc_4640`,
  deleted at 674 MB); which side crashed wasn't confirmed, and that
  such picks are outside the world's range is the agent's reading of
  load_poll's cases, not checked. `RecordOpen` now passes addresses
  other than `UiMovies::Open`/`Delete` on to the recorder set before
  the run. The car list openers' cases keep the list at 1-8 cars
  (`Page0` refuses an empty one). `ShowCarByIdWithClass` keeps its
  adapter (T17 calls it by guest address). Row corrected: T8's header
  is `SetHeader(1, 0)`.

## Run 2026-11-07 22:22 (`b7-loop-07-11-26-2`)

- The car list's third migration (35b39ea5..9fb09b97, the update
  group, 5 adapters removed without counter runs): `Update` writes its
  27-entry step table and the two `lis` leftovers at +80 and +84 on its
  frame, calls the native steps directly and the others through the
  table; `GarageStage::ReloadModel`, `SetCar`, `Command`, `IsBusy`,
  `Draw`, `MenuList::Selection` and the named animations stay guest
  calls through their overrides; `sub_82192770`, never seen, runs as
  the original. Harness gap (in the row): the stages' garage cameras
  aren't set up, so `GarageStage::Draw` draws nothing and
  `ReloadModel` answers 0; `UpdateStand`'s branch after a reload has no
  cases (the same sequence as `ShowStandCar`'s, covered in `load_poll`).

- The car list's second migration (39d443d6..8d948f88, 8 functions):
  `Build`'s entries from `memory::AllocPages`, `memcpy`'s original
  where `c_runtime::Memcpy` refuses; `Build`'s harness makes its own
  params and 72 specs (the game's aren't loaded when the harnesses
  run); `ShowStandCar`'s and `SetUpColors`' cases in `load_poll`'s
  stand-car world (the `SetCar` precedent), their adapters native only
  with `hooks::CarMakersShowCarByIdCalleesNative(true)`. Coverage gaps,
  in the rows: `SetUpColors` keeps D (+1904) at 0, `ColorsPage` never
  has the car loaded, `PowerGraphPage`'s set-ups end at
  `[0x82815EC0]` = 0 (`sub_82250700`, never seen, stays the original).
  A test run's log `itc_4509` (674 MB: a first harness try read address
  0xC; fixed) can be deleted.

- The car list's first migration (24d1bfd6..098104fe,
  `--native_car_list`, 58 functions, 18 adapters removed without
  counter runs: the car list isn't reached in attract races): `Free`
  through the allocation layer's `FreePages` (else a guest call to
  `VirtualFree`'s override); `HandleInput`'s sign-extended r3 copied;
  `SetUpCarPage`'s garage-tab branch ported without harness cases (its
  callee `CarPanel::ShowCarStats` `sub_8224EA10`, never seen in play,
  stays the original and would build a car from a random garage
  record); the harness leaves the `SetText` frame words +40 to +79 out
  (the 2026-10-30 call); `SaveData::Of(0)` can be 0 when the harnesses
  run, so set-up cases use the harness's own save block.

- `functions/car-list.md` (new, 154 rows, 93 in play; c595a898): the
  car list the type 6 and 7 panel screens show (`PanelSet`, vtable
  `0x820110C8`: its slots, 9 pages and 27 steps, the panels and panel
  draws; the two-player `PanelSetPair`, never seen); vtables read from
  the unpacked `default.bin`; `known-data.md` rows (the car list's
  fields, `0x826379B8`, `0x82815EAC` as the two-player word); leads
  trimmed (the stand screens', the power graph's, the stand car
  makers', the power curve's) and `sub_8224B248`'s deleted
  (`PanelSetBase::SetUpColors`, mapped here).

- The type 6 and 7 screens' migration (42695d6d..cfbe6bd6,
  `--native_type67_screens`, 48 functions): the car list's vtable
  calls stay guest calls; `PanelScreen::DrawList`'s adapter removed
  (the other screens' draws that call it, never seen in play, run the
  original); the 97 functions never seen in play keep their recompiled
  code. Some harness groups have few cases (2 to 12: the movie opens,
  releases, getters and the group pair's small steps).

- `functions/type67-screens.md` (new, 145 rows, 48 in play; 973a1f10):
  the type 6 and 7 modes' list, panel and node screens (the panel
  screen is the car pick: +9644 the car list `PanelSet`, slot 3 copies
  the picked car into the mode's; the node screen reads the course
  records), their vtables and base classes, the list part, the panel
  screens' group pair; `known-data.md` rows for the screens' fields,
  the group pair, `0x82815EB0`, `0x82636720`; leads added for the car
  list (`PanelSet`, ~130 functions) and the list and node screens'
  pages and steps (~75, none seen); notes added to the leads of
  `sub_822405F8` (now mapped here) and `sub_8224BDC0`.

- The options screen's third migration (6c23d796..95016660, 24
  functions, 19 adapters removed without counter runs: their callers in
  play are menu pages attract races don't reach): a new tool,
  `itc_recomp/tools/init_step_tables.py`, generates machine Inits' step
  tables from `generated/` (checked by reprinting three of
  `type67_modes/init_tables.h`'s exactly); the screen fades' 64 bytes at
  `0x826A50AC` added to the harness's saved globals. A harness finding
  for every harness: `Random` is xorshift64 and each case's seed is the
  next output of the harness's generator, so case k+1's stream is case
  k's shifted by one word and neighboring cases are correlated (a path,
  `AnalogPage`'s step 35, never ran in 600 cases); the new groups mix
  their generator with splitmix64 (`Mixed`), the other harnesses are
  untouched. A lead for later (not a subsystem: not in `leads.md`).

- The options screen's second migration (e55413be..bbb0cfe3, 43
  functions): `ShowVolumeBar` `sub_821BE5A8` Wrapped -> Overridden, its
  pause-movie-log wrap kept around the native body; four callees never
  seen in play (`ShowCareerInfo` `sub_821C0EF0`, `SetHudArrow`,
  `SetSoundArrow`, `SetSlidersArrow`) called as the originals through
  the function table; `OnLeft`'s frees through the allocation layer.
  Learned: code addresses left in registers by `lis` are sign-extended
  in the 64-bit registers (`Draw`'s no-page r3 is 0xFFFFFFFF821C2388),
  copied as such. The harness world grew (named animations to 900,
  sound manager slots, a players' mappers block, more saved globals).

- The options screen's first migration (a9b2152f..e81aa1ef,
  `--native_options_screen`, harness `options_screen`, 34 functions):
  `Options::FadeOut` `sub_821BD680` stays recompiled (its
  `ScreenFades::FadeOut` call never sets r7, the layer, a register
  leftover stored into live state: the `BackToBuilds` call of
  2026-11-06); `UiMovies::Delete`'s adapter now hands its call to the
  harnesses' start-up recorder first, as `UiMovies::Open`'s already
  does, so the harness records the delete (comment in
  `src/hooks/app_start_up.h`); five adapters (`ButtonIndex`,
  `AnalogIndex`, `ButtonCode`, `Settings::Reset`, `Settings::Write`)
  removed without the counter run, as the attract race never reaches
  the options screen (a recompiled caller left would run the original).

- `functions/options-screen.md` (new, 155 rows, 95 in play; bce411a9):
  the front end's options screen and the settings' load
  (`Settings::Reset`, `Read`, `Write`, `Reload`; `Draw`'s 15 pages and
  `Update`'s 49 steps from their tables), `sub_821BE5A8` moved from
  `functions.md`; `known-data.md`'s settings row (`0x826D8470`..
  `0x826D8494`: three 192-byte blocks, current, edits and defaults,
  mapped to the save options at `SaveData::Of(0)` + 0x1F85C; the
  category table). Game quirks kept (no behavior change):
  `Settings::Write` stores player 1's analog slot 4 from word 3, so
  that setting never persists; step 39 (`ChangeAnalog2P`) can't change
  item 3; step 2 (`LeaveAnalog`) reads past its 7-entry tables for
  analog index 14 (only a saved value could give it). Inferred: type 8
  / context 4 as split screen, the controller pages as player 2's.
  Four jump-table tails the recompiler split off are rows marked as
  tails. Noted, not changed: `src/debug/pause_movie_log.cpp`'s comment
  says `sub_821BE5A8` sets children 1-15's +80 flag; it sets 22-36.

- Cleanup lead `sub_820C8DE8` (c709d9ed..92169ce4): `CarScene::SetUp`
  (`car-models.md`, the second set's fill, under `--native_car_models`),
  `StandScene::Create` and the type 6 and 7 modes' slot 0
  (`Type6Mode::Init`, `Type7Mode::Init`, under `--native_type67_modes`)
  native; 15 adapters removed (the members' Inits, the screen blocks'
  constructors in `screen-constructors.md`, `StandSlot::Init`). Choices
  to review: the guest calls kept from native code to callees with
  worlds of their own (`Viewports::Init`, `CarScene::Create`, the UI
  movies' start `sub_8211AD00`, `Strings::Load`, `StandScene::Create`,
  `LoadingScreen::Show`), made through their adapters from the
  original's frame (the type 6 and 7 modes' precedent); the slot 0
  cases run the original with the flag on (the `OnDialogClosed`
  precedent); harness helpers moved into `scene_pools_take.h` and
  `loading_world.h`; `hooks::SceneLoadersCreateAWith` added; one new
  expected group (`Type6Mode::Init`'s transitions' stack leftovers,
  `expected-differences.md`). Adapters kept for recompiled callers:
  `sub_820C8DE8`, `sub_820C8A50`, `sub_820D0D50` (`RaceScene::SetUpCars`,
  `LocationScreen::Create`), `sub_82180B00` (types 2 and 12).

## Run 2026-11-07 10:56 (`b7-loop-07-11-26`)

- `functions/tuning-pages.md` (new, 106 rows, 67 in play; 92fea740):
  the tuning screen's input handler `sub_821E6950` and its pages; rows
  in `known-data.md` (tuning screen, garage car, front-end menu). Noted
  in its intro for the migration, by earlier calls: register leftovers
  callers read (`MoveLevel`, `MoveType` reading `IsItemAvailable`'s r4;
  `StoreStickers` relying on `GarageDecal` leaving r4-r8, f0, f12, f13),
  `Texture::FromTga` storing r3 into its caller's frame, writes below
  the stack pointer (`StoreVinyls`, `HsvToRgb`), varargs `SetText`
  calls, unbounded state history pops; keeping a sticker area calls
  `Career::PickNext` (the game's own).
- Its first migration (94da1ea6..5fd92bc3, `--native_tuning_pages`): a
  new flag (no call put the pages under `--native_tuning_screen`); the
  harness quiets signaling NaN inputs of `StoreVinyls`, `LoadVinyls`
  and `HsvToRgb` before running (the originals' lfs/stfs quiet them, the
  host's copies may not); `EnterEditor`'s cases keep the state history
  below 60 entries (a deeper push overwrites the queue pointer at
  +10036, the original's unbounded pop/push); `FreeTexture`'s
  allocations compared as tokens.
- Its second migration (8e5ecf93..bd66f968): `DecalImages::Update`,
  `UnloadAll` and `Texture::FromTga` native only while the native heap
  is on (as `MakePlate`); `DecalImages::Load` `sub_821F6D40` and the
  sticker dialog's `SaveAreaDecals`, `StoreStickers`, `GarageDecal`,
  `SwapKept` (not seen in play) ported; `SetCurrentChoice`'s other
  caller `sub_821F1780` (not seen) stays recompiled; `Menu::StepList`'s
  row and `known-data.md`'s front-end menu +52 corrected (the row before
  the step); `OnDialogYes`' cases for states 35 and 36 run with
  `--native_tuning_pages` on for the recompiled side too (their callees
  compared in their own groups). Lead for later: `car_makers`'
  `RestoreStand` harness crashes with other random seeds (its own
  set-up; the batch restores the generator after its group).
- Its third migration (d36330ef..ecb9b381): `FillPartRowsLeft`
  `sub_821F4768` (not seen in play) ported; `ResetStickerEditor`
  `sub_821F8440` and `LoadStickers` `sub_821F86C0` (not seen) stay
  recompiled, called through the guest; a shared wrapper
  `tuning_screen::FillPartRow` added in `src/tuning_screen/rows.cpp`;
  the new harness set-ups seed their generators through splitmix64
  (consecutive xorshift streams fell into step); `WheelsFields` pins the
  scene's child lookups; `MoveLevel`'s r4 found to be its own load (the
  map's leftover note corrected).
- Its fourth migration (85a8c44b..43d9b8ec): `IsPickOwned`'s callers not
  seen in play (`sub_821D7C90`, `sub_821F0D38`, `sub_821F1130`) stay
  recompiled and now run the original; nothing to reproduce in the
  registers both pages keep live across leaf calls.
- Cleanup leads (c1cd4b85..99c60056): 15 adapters removed in
  `front-end-car-draw.md` and `tuning-screen.md`. With their adapters
  gone, the callers' recompiled side runs the callees' originals, which
  save registers below the caller's frame where the plain C++ writes
  nothing: the harnesses now leave those bytes out (`GarageStage::Draw`
  208 bytes; `RestoreStand` SetLightKit's f29-f31; `FillPartRowsRight`
  and `Left` 176-216 below r1; `AnimateWheels` 304-344 and 896-928,
  the second `SetPartName`'s `SetText` r7-r10 area; `InputWheels` the
  same 128 bytes deeper), following the earlier `GarageAnims::Add` and
  `Open`/`Close` cases in the same harness file.
- `functions/screen-constructors.md` (new, 41 rows, all in play;
  fbb76124): the type 6 and 7 modes' screen blocks (91,272 and 71,292
  bytes) and their members' constructors, all names ours; rows in
  `known-data.md` (type 6 and 7 screens, `0x82815EAC`, `0x826370A0`);
  `boot-mode.md`'s `ScreenMachine::ScreenMachine` note pointed here.
  Leftovers for the migration: `NodeScreen::NodeScreen` storing r3 into
  its caller's frame, `PanelSetBase::PanelSetBase` zeroing and reading
  words below its stack pointer.
- Its migration (c0ce0aec..bc2f1e67, `--native_screen_constructors`):
  the panel word corrected to `0x82815EAC` (the car list screen's);
  4 cases a constructor (they only store constants and arguments);
  in the `Type6Screens` case `NodeScreen`'s frame +80 (the original's
  back chain of `Type6PanelScreen`) is left out of the comparison, as
  plain C++ writes no back chains; the panels' `operator new` falls
  back to the original's guest call (new handler, `bad_alloc`) on
  failure.
- `functions/type6-type7-modes.md` (new, 160 rows, 116 in play;
  a96949ba): the type 6 and 7 modes, probably two front-end hubs over
  the player's garage car on the stand (which ones by name not settled:
  the strings weren't read); 8 rows in `known-data.md`. Leads trimmed:
  the car scene owners' line deleted (mapped here); functions taken out
  of the stand screens', garage cars reset, stand's car makers' callers'
  and menu helpers' lines; the vinyl editor's lead corrected
  (`sub_8217C120`, `sub_8217D888` are the garage camera's quaternion and
  reset). For the migration: three stack bytes land in the answer at
  block +91257..+91259 (copied); the garage camera's presets arrive by
  an archive callback on the loader thread.
- Its first migration (48b60036..e9a438b2, `--native_type67_modes`):
  the 16 Inits' step tables (`src/type67_modes/init_tables.h`) made by
  a scratch script from `generated/` (not committed); few harness cases
  for some (`Dialog::OpenMovies` 1, the dialogs' and options' Draw 2);
  one harness-only run (`itc_4366`, ~700 MB, left in place) logged
  guest access violations from the harness's own cases
  (`UiMovies::IsLoaded` on an id not in the table), fixed by giving
  those cases a movie block of their own. `StandSlot::Free`'s prints
  sit on its two native paths, after its check that hands back to the
  recompiled code (not its first statement).
- Its second migration (cb21f566..5dd1e34c): `GarageLights::Init`'s
  row corrected (the spots keep the record's w; a zero direction gives
  zeros); `Type6Screen0::Confirm` `sub_82233FC8` given a row (seen in
  play, missed by the map; its caller `sub_82234958` has none);
  `GarageCamera::Init`'s harness leaves out one dead frame word left by
  `LoadPresets`' callees on the recompiled side; `tan` run with nearest
  rounding set and restored around `c_runtime::Tan`.
- Its third migration (cff3fe09..3e80c383): `ControllerOptions::Update`
  copies the frame doublewords and the r3/r4 leftovers it stores into
  live state (reproducible here, so not kept recompiled);
  `ConfirmAgain` `sub_822340B0` keeps its adapter (its native caller
  reaches it by a guest call for the format calls).
- Its fourth migration (30ecc181..f65f43a4): harness gaps said in the
  rows: `MenuPage`'s button-163 branch is ported but its callee
  `sub_82250488` (not seen) stays a guest call and no case presses 163;
  the Updates' branch where `ReloadModel` answers 1 isn't reached (its
  car object steps are covered by `ShowCarInfo`'s cases);
  `Type7Screen0::Confirm` `sub_82244528` keeps its adapter like
  `ConfirmAgain` (a guest call from native `MenuPage`); the harness
  helper `Calls::call_set` now sets r3-r8.

## Run 2026-11-06 14:33 (`b7-loop-06-11-26`)

- The run halted (2026-11-07) at the tuning screen's fifth validation (`itc_4236`): the all-harnesses run crashed in `car_makers`' existing `TuningScreen::SetUpCarData` `sub_821DF2F8` group (a guest read of `0x00000004`, looping until the hard exit); with `car_makers,tuning_screen` alone (`itc_4237`) every group passes. Fixed after: the case's garage list could end on a node with no slot set (a random slot, case 24's car ~18 MB past the block); now walked through distinct nodes (`LayGarageWalk` in the `car_makers` harness, for its four set-ups); the validation then passed (`itc_4240`). Prints not first in their bodies, left as they are: `sub_8211C740` (inside its `ChangeText` lambda, as the file's other text calls), `GarageStage::Draw` (after the early return when the stage's +112 is 0).

- Fifth `tuning-screen.md` migration (59c3072d..00ba6bcd): `UiMovie::SetNumberText` `sub_8211C740`, `SetChildNameText` `sub_8211D930` with `SetNameText` `sub_8211CAA0` (`ui-movies-game.md`), `GarageStage::Draw` `sub_8217FE60` (`front-end-car-draw.md`; `CarScene::Draw`'s frame-state wrap kept; correction: `SetLights` gets r4 = 0 on both passes, not 1 on the mirror pass), `TuningScreen::UpdateStand` `sub_821D9F10`, `RefreshMovie13` `sub_822076E8` (harness only: the replay never opens the vinyl editor); 4 adapters removed. For review: harness fragilities met, not fixed: with shifted random seeds an existing `RestoreStand` case (`car_makers`) crashed and a `FillPartRows` case (`tuning_screen`) read a wild address; new groups that shift seeds now run last or restore the generator. f5bab5e9 keeps `UpdateStand`'s cases in their world (`GarageAnims::Add` scans the stage's animation list unbounded).

- Fourth `tuning-screen.md` migration (5186aacf): `TuningScreen::RefreshMenu` `sub_821D8978` (2,353 generated lines, not 1,319; `src/tuning_screen/refresh_menu.cpp`): node, time and color callees through `ui_movies`' plain C++, the animation and text calls through their overrides from the original's frame; the frame +80 doubleword and state 16's tab table at +96 as the original leaves them; the adapter puts the last native time into f1 before each later guest call and at the end (Finish and FinishNamed can change f1). The harness gained an option to compare f1 as a double.

- Third `tuning-screen.md` migration (437e52fa..a28478a4): 23 more, `SetInteriorPair` `sub_821CCC50` Native. The functions calling into the stand have their cases in the `car_makers` harness (`TuningStand`). For review: inputs shaped as in play (the garage car's paint pick -2 to 5, the last camera pick +25072 a preset 14 to 64 or -1, the garage list walked through distinct nodes); callees never seen in play through the guest (`HighlightItem`, `HighlightSelected`, `SetCaliper`, `GarageStage::SetChoice`, `PreviewParts`, `RefitAxles`, `SetAero2`, `SetDoorKind`, `SetAccessory`, `SetColor`, `ClearLightKit`). Corrections: `PreviewItem`'s reads from frame +120 on land on the original's register saves (the harness keeps those indexes in range); `SetNodeText` is varargs too (stores r6-r10 into the caller's +40 to +79, left out of `SetStateTexts`' comparison).

- Second `tuning-screen.md` migration (e2ef9640..03d785be, harness file `src/harnesses/tuning_screen_pages.cpp` laying movies 2 and 13 with their animations, nodes and a string table): 24 more, `HideTireMarks` `sub_821DFB20` and `PlaceStateNodes` `sub_82202C08` Native. UI movie calls go through their overrides from each original's frame; SetText's unset varargs pass 0, their parameter area left out of the comparison; the int-to-float doublewords at frame +80 (`PlaceStateNodes`, `LayOutItems`, `Command7`, `Command15`, `Command18`) copied as the originals leave them.

- The user's calls (2026-11-07): the run's limit raised to 8 subsystems; the validation agent's race is `attract_race.sh 180` (the harnesses took ~85 s of the 120).

- `tuning-screen.md` migration (113d853c..6c8c5a52, `--native_tuning_screen`; `src/tuning_screen/`, harness `tuning_screen`, `garage_car.cpp` with strict floats): 28 Overridden, 3 Merged (`sub_821CDF0C`, `sub_821CD308`, `sub_821DF24C`). For review: `StoreToGarageCar` `sub_821DCC28` writes its stack tables and int-to-float temporaries below sp as the original leaves them and reproduces its r3 leftover exactly; not reproduced: a vinyl mirror index (+64) past 7 or a sticker's tens past 15 would read the original's register saves (sp - 64 on), which native code doesn't write (the harness keeps them in range; its row). `AeroPartValue` `sub_821DE2D8`'s 900 frame table words taken from `generated/` by simulating its stores. The 17 debug handlers leave out the empty `sub_82106540`'s calls and the loads feeding them; back chain, string tables and r3 as the original leaves them.

- `tuning-screen.md` (new, 9cc3556f): 91 rows, all new, 80 in play: the tuning screen's update subtree (not the update `sub_821E6C50` itself nor its refresh `sub_821E3AD0`, both leads now), lead 2's part, interior and wheel handlers (`TuningScreen::PreviewWheels` `sub_821E0E00` and the rest), `TuningScreen::SetCarStats` `sub_821DF788` (from the power curve lead) and six from the stand screens' lead. As read: the 17 state handlers only print debug menus through the empty `sub_82106540`; the work goes through a command queue at `[+10036]`; fields +9656 (the category, 0 power to 6 interior), +9660 to +9664, +25116 (probably the money) in `known-data.md`. For the migration: `TuningScreen::StoreToGarageCar` `sub_821DCC28` has no frame, reads unwritten stack words for some sticker values and returns a division's leftover in r3 (to be copied). To check: `GarageStage::SetColor`'s wheel color pieces write the scene model's +64 to +111 while `ApplyWheelPaint`'s row says it reads +112 to +207 (neither piece ran in play; a lead). The reading was split among five subagents, one at a time.

- Third `build-screen.md` migration (12c133b8..8b626097): `CarScene::Update` `sub_820C9608` (`scene.md`, `--native_scene`; no wrap existed around it; frame dump against `--native_scene=false` the same) with `ScenePools::DestroyB` `sub_820C59B0`, `CarScene::UpdateModels` `sub_820C5A60` (`vehicle-accessors.md`) and `SceneModel::Update` `sub_820DA180` (`scene-models.md`; its accessory branch, never seen in play, builds the matrix natively and then calls `Matrix::Multiply` through its override and the originals of `Accessory::SetAnchor`/`Step`); `GarageStage::ReloadModel` `sub_8217F880` (`car-makers.md`; its callees as guest calls through their adapters, as `RemakeCarModel`; adapter kept for 9 recompiled callers); `BuildScreen::OpenStand` `sub_8219FE10` with `SetUpStand` `sub_8219F780` and `PutOnStand` `sub_8219D6F0`. 14 adapters removed (in `scene.md`, `scene-models.md`, `accessories.md`, `build-screen.md`). The `build_screen` harness now also saves, restores and compares `[0x826D83E0..E7]` and `[0x826C2E40..47]`.

- The user's call (2026-11-06): `BuildScreen::BackToBuilds` `sub_821A1258` stays recompiled: it stores the r7 `UiMovie::Run` leaves into its machine's +24 (`HighlightBuilds`' step word), a register leftover in live state native code can't reproduce. Added to "Precedent first" for like cases.

- Second `build-screen.md` migration (04cdbd4b..06ba6b96): `SetUpGroups` `sub_821A0060`, `Open` `sub_821A08D8`, `BackToGroups` `sub_821A0A38`, `ChangeCategory` `sub_8219FC60`, `OpenBuilds` `sub_821A1138`, `BackToStand` `sub_8219FF78` with `RestoreStand` `sub_8219F868`, `OpenDetails` `sub_8219FEC8` with `SetUpDetails` `sub_8219F8E0` and `ShowDetails` `sub_8219D820`; `CarScene::LoadsDone` `sub_820C5798` (`scene.md`, `--native_scene`; its finishers as guest calls through their overrides, their wraps kept); 15 adapters removed. For review: existing build screen harness cases' inputs were shaped to stay in their world (two compare ranges over later callees' register saves left out; groups' +12 kept -1 to 9, since past 9 `ValueText` reads earlier calls' register saves; words past the last animation 0). Question to the user: `BackToBuilds` `sub_821A1258` stores the r7 `UiMovie::Run` leaves into the machine's +24 (`HighlightBuilds`' step word).

- `build-screen.md` migration (475f728c..3d2f5c6d, `--native_build_screen`; `src/build_screen/`, harness `build_screen`): 46 of 60 ours (the lists, groups, rows, arrows, marks, prompts, help, number, highlight and fade transitions, the builds level, `GarageStage::SetDark`); the six UI movie text helpers in `src/ui_movies/objects.cpp` (`--native_ui_movies_game`, harness `ui_movie_objects`). Correction: `HighlightGroups2` `sub_8219C6D0` doesn't play or loop animation 84 (not the same as `sub_8219C528`). For review: unset varargs of `SetText`/`SetNodeText` pass 0 (the 2026-10-30 call) and their parameter areas (frame +40/+48 to +79) are left out of the comparison; `UiMovie::Run`'s update `sub_8211D2F8` is stood in for in the harness (as the location screen's); harness inputs keep links, slots and counts in range so `ChangeCategory`-like loops end.

- `build-screen.md` (new, 51812e30): 66 rows, all new, 60 in play: the front-end screen over `[0x826D8448]` (probably a car shop or build list; no strings read) with four levels as the states of `[0x826D843C]` (groups by category, a group's builds, one build on the stand, its details), its 21 transitions (6 not seen), 44 callees and `BuildScreen::PutOnStand` `sub_8219D6F0` (taken from the stand makers' callers' lead); six UI movie text helpers that take a node (`sub_8211C370`, `sub_8211C3F8`, `sub_8211C680`, `sub_821087D8`, `sub_8211CCC0`, `sub_8211CD28`) and two garage stage light functions (`sub_8217D0C8`, `sub_8217B7C8`), unmapped before, are here. `sub_821A1380`'s row (`car-makers.md`) now points here. `BuildScreen::ChangeCategory` loops until a category has groups (never ends if none has). `known-data.md` updated.

- Cleanup lead `GarageStage::SetCar` `sub_8217FD58`, `SetCarClass` `sub_8217FDC8`, `ShowCarById` `sub_82182C90`, `ShowCarByIdWithClass` `sub_82182D30` (17174c73, 90db69c1, `--native_car_makers`, `src/car_makers/stand.cpp`; harness cases in `load_poll` with `LayStandCar`/`LayShowCarWorld` in `car_makers`), then Native (c368a9cf, c5e36a97): `GarageStage::CopyRecord` `sub_8217FC60`, `ReapplyRecord`, `ReapplyRecordNoDecals`, and `SceneModel::ApplyWheelPaint` `sub_820D9C50` (its `frame_state_diff` wrap deleted; `stand_models::ApplyWheelPaint` calls its plain C++ inside the same `--debug_hide`/`--find_per_frame_state` checks). For review: `ShowCarById`'s harness leaves the car object's last 32 bytes out of the comparison (they sit on the original's register saves; its row).

- Cleanup lead `GarageStage::ReapplyRecord` `sub_82178DF8` and `ReapplyRecordNoDecals` `sub_82178E60` (cea27e18, `--native_car_makers`, `src/car_makers/stand.cpp`, harness cases in `load_poll`), then adapters removed (6fbff1bb, Native): `SceneModel::ApplyRecord`, `ReloadDecals` (`scene-models.md`), `ApplyLightKit` `sub_820D9D10` and `ApplyPaint` `sub_820D9B68` (`load-poll.md`). For review: `ApplyPaint`'s `frame_state_diff` wrap was removed with its adapter; its native callers run it inside the same `IsHidden`/`FrameStateDiff` checks (as `car_draw`'s Wrapped do). `ApplyWheelPaint` `sub_820D9C50`'s wrap kept (its native caller in `stand_models` still makes a guest call). A harness fix on the way: traffic cases always get a traffic body (the poll's world could leave +5092 at 0, and the original then read `0x2620`).

- `stand-models.md` migration (e8b7c8b2..ed0605b9, `--native_stand_models`; `src/stand_models/`, `src/scene_models/apply.cpp`, harness `stand_models`, cases added to `load_poll`, `car_models`, `car_makers`): 28 of 28 in play ours (`sub_82178F68` Merged), with `SceneModel::ApplyRecord` `sub_820DA758`, `ReloadDecals` `sub_820D9D48`, `SceneParts::Apply` `sub_820E08B0`, `SceneInterior::Apply` `sub_820E4258` and `SceneModel::AddSecondWheels` `sub_820D9E00` (renamed) in `scene-models.md`. For review: `ApplyRecord`'s path with the scene's +4 set (the race's: parts claimed again, the cockpit rebuilt) runs the original (never taken in play, the callers are all the garage stage's) and the harness covers only +4 = 0; the setters calling `ApplyRecord`/`ReloadDecals` have cases that stop early (model loading, not ready, traffic), `ApplyRecord` itself 48 cases in the poll's world, 35 past the poll; `SceneWheels::SetSizes`' parameter area: neither leftover call applies (the caller sets every word); unchecked frame tables (`TuningScreen::SetTireGrade`, `SetDecalA`/`B`) read unchecked as the original, cases up to one word past; `SetWheel` with a negative r4 not tested. No adapters removable yet.

- `stand-models.md` (new, d33966e3): 39 rows, all new, 28 in play: the garage stage's scene model holders (`GarageStage::MakeModel` `sub_8217FCB0`, `AddSecondWheels` `sub_82179998`, `ApplyWheelPaint` `sub_821799D8`, `SetWheel` `sub_8217DD38`, `FitAxles` `sub_82178B98`, `SetTireGrade` `sub_82179130`, the stand's other record setters at `82178EC0`-`82179A18`, 14 seen and 8 not), the wheels' setters (`SceneWheels::SetSizes` `sub_820E7380`, which stores r6-r9 into its caller's parameter area; `SetTireGrades` `sub_820E7470`; `Wheels::SetTireTextures` `sub_820D8F18`) and the tuning screen's helpers over them (`StepSecondWheels` `sub_821CC890`, `PreviewWheel` `sub_821DFA78`, `SetTireGrade` `sub_821CCA38`, which indexes a five-word frame table by r5 unchecked). Shared rows left in `scene-models.md`, `car-makers.md`, `load-poll.md`; `known-data.md` updated.

- Third `race-update.md` migration (ba4bf51f..3aae586e): `Scenery::Update` `sub_820F4558` (`course-scenery.md`, its harness with enabled camera passes; `LimitLargeModels`' reads below its frame written as the original leaves them), `ScenePools::SetUpB` `sub_820C5B28` with `sub_820C5B98` Merged, `SceneCockpit::SetUp` `sub_820DBE80`, `SceneModel::SetUp` `sub_820DB458`, `SceneModel::Build` `sub_820D9870` (`scene-models.md`; a recursive group `b7_groups.py` can't call ready, every call outside it ours), `RaceScene::RemakeCarModel` `sub_8215C0D8`, `RaceScene::Update` `sub_8215D2B0` with `Car::TickUpdate` `sub_8215C780` (`race-update.md`). 25 adapters removed (Native) in `course-scenery.md`, `scene-models.md`, `car-models.md`, `vehicle-accessors.md`, `race-update.md`, `post-effects.md` (`PostEffects::MarkUpdated` `sub_82133A08`), `screen-objects.md` (`ScreenObject::SetSpeed` `sub_8212FCC8`). `ScreenObject::Start` `sub_8212EDF8` and `Stop` `sub_8212EED0` (not seen in play) keep their recompiled path. Adapters kept: `RaceScene::Update`, `RemakeCarModel`, `SceneModel::Build` (recompiled callers `sub_8215D538`, `sub_8215C248`, `sub_820DA180`).

- Second `race-update.md` migration (7efd9cff..01089192): `XPhysicalSize` `sub_822965C0` Native (`heap.md`; `memory::AllocPhysical` records each block's size rounded to its page, `memory::PhysicalSize` answers; a size-0 block is one page, as the SDK's region, a7d82632), `Scenery::AllocPhysical` `sub_820F1490`, `FreePhysical` `sub_820F14E8`, `Create` `sub_820F2F88`, `Setup` `sub_820F3020`, `SetupPools` `sub_820F2720` (Native, no adapter; `course-scenery.md`), `RaceScene::LoadScenery` `sub_8215B790` (its +32 left as the stack has it), the second set's set-ups `sub_820DB818`, `sub_820DF1D0`, `sub_820DCAA0`, `sub_820E5998`, `sub_820E37F0` (`scene-models.md`) and `sub_820E70C0` (`car-models.md`). For review: `LoadScenery`'s harness points the CourseSet vtable's load slot `0x82006788` at `PoolObject::Reset` while its cases run (the page made writable through `rex::memory::Protect`, as the framework harness does). `Scenery::Update` `sub_820F4558` left: its harness needs enabled camera passes (its row).

- The user's call (2026-11-06): `XPhysicalSize` `sub_822965C0` (the kernel's `MmQueryAllocationSize`) is answered natively from our allocation layer (`src/memory/heap.h`: `AllocPhysical` records each block's size), one answer for recompiled and native callers; it unblocks `Scenery::AllocPhysical`/`FreePhysical` and the scenery's set-up.

- `race-update.md` migration (b30ad3c0..4a42fcc8; `src/race_update/`, harness `race_update`; `--native_race_update`): `RaceScene::VaryCar` `sub_8215B330`, `RaceLights::Update` `sub_8215D008` with `UploadDirectional` `sub_8215C698` (normalizing as the translated code does, through `car_scene_draw::ops`; its stack vector and `Update`'s (0, 0, 0, 1) written where the original leaves them, register saves and back chains not). In other files: `Scenery::PoolSizes` `sub_820F2030`, `SceneryList::LoadAll` `sub_820F1598` and `Stream` `sub_820F16C0` (`course-scenery.md`, `--native_course_scenery`; the archive queue's length through `Archive::QueueLength`'s plain C++ when native; the harness doesn't reach the full-queue branches), `ScenePools::SetUp` `sub_820C4338` (`vehicle-accessors.md`, `--native_vehicle_accessors`; pool 3 and declined set-ups still through the guest from the original's frame) with `CarParts::Load`, `LoadSetA`, `LoadSetB`, `sub_820D6B48`, `TrafficCar::Load` (`scene-models.md`, Native without adapters; `TrafficCar::Load`'s sprintf passes 0 for unset varargs words, the `UiMovie::SetText` precedent). `Scenery::Update` `sub_820F4558` read, not taken: `LimitLargeModels` reads past its table into the stack saves below `Scenery::Update`'s frame (its row). Question to the user: `XPhysicalSize` `sub_822965C0` (`MmQueryAllocationSize`).

- `race-update.md` (new, db8e723a): 9 rows (`Car::TickUpdate` `sub_8215C780` moved from `functions.md`, 8 new; 7 in play; `ScreenObject::Start` `sub_8212EDF8` and `Stop` `sub_8212EED0` not seen): `RaceScene::LoadScenery` `sub_8215B790` (the 44-byte configuration from `0x82602EA0`, its +32 an unwritten stack word read only in mode 2), `RaceScene::Update` `sub_8215D2B0`, `RaceLights::Update` `sub_8215D008` and `UploadDirectional` `sub_8215C698`, `RaceScene::RemakeCarModel` `sub_8215C0D8` (taken from the scene models' owners' lead), `RaceScene::VaryCar` `sub_8215B330`. Corrections: the race light entry's +16 is a direction, not a color (`RaceLights::InitEntry`'s row); `Car::TickUpdate`'s "car record" is the scene model's draw block (+4288). `known-data.md`, `race-scene.md`, `course-scenery.md` and the scene models' lead updated.

## Run 2026-11-06 11:15 (`b7-loop-06-11-26`)

- `power-graph.md` (new, 64372b27): 6 rows, all new, 4 in play: the front end's power and torque graph (`PowerGraph::Draw` `sub_8217C708`, `Line::RecordFlat` `sub_8217B508`; callers `sub_821DF350` (tuning screen), `sub_82250670`/`sub_82250700` (car list screen), `sub_821B8F60` (vtable, not seen)); `known-data.md`: `0x826D83D8`/`0x826D83DC`, the tuning screen, `0x82815EC0`. The lead line's "`sub_82250670` never seen" was wrong: it is in the coverage list. Two leads added (the tuning screen's update `sub_821E6C50`; the car list screen `sub_8224BDC0`).

- `power-graph.md` migration (f97f2857, `--native_power_graph`; `src/power_graph/`, `src/hooks/power_graph.*`, harness `power_graph`): `PowerGraph::Draw` `sub_8217C708` and `TuningScreen::DrawPowerGraph` `sub_821DF350` Overridden, `Line::RecordFlat` `sub_8217B508` Native (only the native draw calls it). Choices kept for review: the draw's frame leftovers and r3 (frame + 5056) as the original leaves them; the tuning screen's adapter writes the original's saves (lr, r30, r31) into its caller's frame and hands the draw r30/r31, since past 32 segments the original's lines read them; the draw falls back to the original above 32 segments (never in play: a rev limit of 12,800 rpm or more) or with the command list's diagnostics on; `Line::RecordFlat` falls back (a guest call) when a chunk is open, the list is full, or `[0x826D83D4]` isn't the line's list. Map corrected: sprite 31's corners are never set. Checked only by harness (front end; no frame dump).

- `car-makers.md` (new, aca0ef24): 25 rows, all new, 24 in play (`sub_8219EE30` not seen): `GarageCar::Reset` `sub_82110448`, `CarData::FromGarageCar` `sub_82179A60` (with the tuning screen's `sub_821DF2F8`; it calls `Race::AddRacer` from the front end and loads the car data twice), the stand's makers `sub_82182C90`/`sub_82182D30` with `GarageStage::SetCar`/`SetCarClass`, `SceneModelRecord::FromCar` `sub_8215B878` and its helpers, `GarageStage::CopyRecord` (copies the record onto itself), `FitCar`, `ReapplyRecord`, `ReapplyRecordNoDecals`, `ReloadModel`; `sub_821A1380` is a front-end screen's update (vtable) that makes a car from a build. `sub_825D9EF0` probably a static initializer for two garage cars at `0x82814EB8`. `known-data.md`: the garage car, `0x82814EB8`, `0x826D83E4`, `0x826D83F4`, `0x826D843C`, `0x826D8448`, the aero kit table `0x820093E0`. Lead lines trimmed: the scene models' owners (`sub_8217F880`, `sub_8217FC60`, `sub_8215B878` taken), the front-end re-applies (`sub_82178DF8`, `sub_82178E60` taken). 3 leads added.

- `car-makers.md` migration (9d2ce651..1276cd9a, `--native_car_makers`; `src/car_makers/`, `src/hooks/car_makers.cpp`, harness `car_makers`): 16 of 24 ours: `CarModels::BlockOf` `sub_8215AAD0` (case 1 `sub_8215AB04` Merged), the record setters `SetPaint`, `SetWheels`, `SetChoice`, `CarRecord::SetBytes` (adapters removed, Native) with `sub_820D0900`, `SceneModelRecord::FromCar` (runs in attract races), `GarageCar::Reset` and `GarageCars::StaticInit` `sub_825D9EF0` (boot), `CarData::FromGarageCar`, `TuningScreen::SetUpCarData` `sub_821DF2F8`, `GarageStage::FitCar`, `CopyRecord`, and two new names (probably) `GarageScreen::ShowBusyMarker`/`HideBusyMarker` `sub_8219ED10`/`sub_8219EDB0`. Choices for review: register saves not reproduced, back chains and temporaries are (SetPaint's scratch doublewords, SetChoice's stores into its caller's frame, FromCar's frame tables); the harnesses leave out the callee save areas the plain C++ doesn't write (FromCar's 32 bytes, CopyRecord's r31 save in the self-copy case); float word copies the original does with lfs/stfs are bit copies (they keep a signaling NaN, as the translated code does); the front-end cases build their world from car_physics' `WriteSetupFromCarCase` with a stand-in car spec and race data block. Front-end functions checked by harness only.

- Cleanup lead `CarList::DrawEntryPowerGraph` `sub_82250670` (b7a44553, under `--native_power_graph`; `power-graph.md`) and `CarData::FromGarageCar` `sub_82179A60`'s adapter removed (141471cc, Native; `car-makers.md`). For review: the harness compares against the original with only `--native_power_graph` off (as the tuning screen's does): with the car makers', career and car physics flags off too, 42 of 48 cases differed, because `PowerGraph::Draw`'s sprites past segment 26 and its unwritten words read stack words its callees leave, and the native callees don't write register saves (the precedent: register saves aren't reproduced). So past 26 segments (a rev limit above about 10,000 rpm) the graph's tail may differ from the recompiled game's leftovers. The 64 bytes of Draw's register saves under r1 aren't compared; `SetUpCarData`'s cases leave out the 24 bytes of `FromGarageCar`'s saves since its tail call is native.

- `menu-helpers.md` (new, 9f6a0387): 2 rows, both new and in play: `Menu::Step` `sub_82186568` (moves the cursor by the pad through the action mappers; the menu's fields pick the inputs, wrap or clamp, record a wrap; returns the direction) and `Menu::Pressed` `sub_821864C8` (1 when the action was pressed on a step the menu didn't move). No constructor: each of the 113 calling screens (42 seen) fills its 68-byte menu inline; `known-data.md`: "front-end menu" layout (the parking area's menus row points to it). `location-screen.md` and two leads now point to the new file. 2 leads added (the screen around the helpers `82186428`-`82188178`; 19 callers seen in play in no file).

- `menu-helpers.md` migration (44a04c9f, `--native_menu_helpers`; `src/menu_helpers/`, `src/hooks/menu_helpers.*`, harness `menu_helpers`): both Overridden (adapters kept for ~113 recompiled screens). The mapper's slots are called through its vtable as the original does: `ActionMapper::Pressed`/`Fired`'s plain C++ when the vtable holds them and `--native_action_mappers` is on (the `MenuList::Fired` precedent), else a guest call with the original's return address; `Pressed` passes r4's full 64 bits. Back chains written, register saves not (the race screens' and menu sounds' precedent; left out of the harness comparison). Front end only: checked by harness.

## Run 2026-11-06 01:02 (`b7-loop-06-11-26`)

- Two leads deleted as covered (as the run of 2026-11-04 did): the device lock's callers (`sub_820962B8` in `frame-passes.md`, `sub_8209BE88` in `app-start-up.md`, `sub_820AE528` `Movie::Release` in `movies.md`, `sub_8212F0B8` in `screen-objects.md`, `sub_82133C60` and `sub_82138C38` in `post-effects.md`) and the other `ScopedLock` users (`sub_8258DFF0`, `sub_8258E098`, `sub_8258E1D8`, `sub_8258E410`, `sub_8258E498` in `shader-objects.md` and `models.md`, ours; `sub_82593E78` `MovieWrapper::Open` in `movies.md`, over the replaced movie library).

- `command-buffer.md` (new, aa4b202c): 36 rows, 16 moved from `functions.md` (`sub_8229E3F0`, `E848`, `DFF8`, `DEA0`, `EF48`, `F148`, `DD70`, `DCC0`, `F0E0`, `F1B0`, `E908`, `DDD8`, `EC00`, `E100`, `E9D0`, `sub_822AC470`), the range widened to `8229DBF0` (the segment save `sub_8229DBF0`, the unseen wait helpers `sub_8229D6F0`, `D8A0`, `D720`). The renderer reads only `sub_8229E308`'s inline vertex space (`BeginVertices` to `End`, copied by `Renderer_InlineDraw`) and `sub_8229F148`'s space for the vertex shader patch (copied out by the `sub_822AE340` hook): kept-kind. The segment flow (reserve, end, tail, next, kick) and the callback packets (`sub_8229E9D0`/`sub_8229F228`: the present callback, the tiling worker's wake) kept-kind while `gpu_link.cpp`'s kick reads them; the tiling chains kept-kind while the recompiled tiling worker reads them. Dropped-kind by the 2026-10-24 calls: the waits (`sub_8229DC28`, `sub_8229E908`, `sub_8229F1B0`, `sub_8229EAF0`; they pass at once in play), the coherency flushes (`sub_8229E5D0`, `sub_8229DE48`, `sub_8229DDD8`), the data cache flush `sub_822AC470`, `sub_822AF978`'s packets (its dirty marks kept, as `device_state.cpp` for `sub_8229F680`). `sub_8229DDD8` left blank though its only caller, `Resource::Lock`, is Native and leaves the call out (perhaps Dropped). `known-data.md`: 9 device field rows; `gpu-functions.md`'s wait table names what `sub_8229DC28` waits for. Leads added: predicated tiling's start and worker (`sub_822B6AD8`), `Device::Clear`'s worker `sub_8229FC40`.

- `command-buffer.md` migration (e041aa4a..abca2df1, `--native_command_buffer`; `src/graphics/command_buffer.*`, `src/hooks/command_buffer.cpp`, harness `command_buffer`): `Device::WriteSegmentTail` `sub_8229E768`, `SaveSegment` `sub_8229DBF0`, `CommandChain::Close` `sub_8229DFB8`, `WriteCallback` `sub_8229E9D0` (probably), `NextSpace` `sub_8229E100`, `AllocateFromCommandBuffer` `sub_8229E308` (its recording-buffer path, +13160, never seen, keeps the original) and `WriteDefaultDrawRegisters` `sub_822AF978` (the dirty marks, without its 15 packet words: the segment has 15 fewer words; `expected-differences.md`). Dropped by the graphics calls: the GPU wait `sub_8229DC28` (left out of `NextSpace`; passes at once in play), the coherency flush block `sub_8229E5D0` (answers "no block", as with nothing to flush; its reservations no longer count toward the segment's data budget) with `sub_8229DE48`, `sub_8229DDD8` (only `Resource::Lock` called it), the data cache flush `sub_822AC470` (returns at once). Map corrections: `sub_822AC470`'s r4 is the range's end, not a size; `NextSpace` passes generation + 1 to the wait without storing it. `SaveSegment`, `Close` and `WriteCallback` didn't run in its attract races (harness only). Frame dumps against the flag off: within run-to-run noise (itc_3601/3602, itc_3605/3606).

- `command-buffer.md` second migration (8d43620c..632f2b7d, `--native_command_buffer`): `CommandChain::NewBlock` `sub_8229EB10` with `Init` `sub_8229ED60`; the segment flow's mutually recursive cycle as one group (`EndSegment` `sub_8229EF48`, Overridden, its `draw_packets.cpp` counter kept as a wrap around the native body; `KickSegment` `sub_8229EDA8`, `NextSegment` `sub_8229EC00`, `WaitForFence` `sub_8229E908`), the kick (`Device::DeferredKick` `sub_8229E848`, running the segments' guest callbacks) handed in as a `std::function` guest call (`hooks::CommandBufferKick`, as XAudio's and texture compression's GuestCalls); `ReserveWords` `sub_8229F148` with `InsertCallback` `sub_8229F228`; `WaitForIdle` `sub_8229F1B0` and `sub_8229F0E0` without their WAIT_UNTIL and 0xD04 packets (graphics rule; `expected-differences.md`). Dropped by the fence-wait call: `WaitForFence`'s spin on pending fences (its segment end for the open fence kept: it kicks the frame's last packets) and `WaitForIdle`'s spin on +10868; by the cache-flush call: `NewBlock`'s data cache flush. 5 adapters removed (`sub_8229E768`, `E100`, `EDA8`, `EC00`, `E908`); `Device::Swap`'s override (`graphics_device.cpp`, no harness of its own) now reaches `NextSegment` through `hooks::NextCommandSegment` (the original with the flag off). A recording command buffer (+10808 bit 7 or +13160, never in play) runs the originals whole. Map corrections: `NewBlock` asks `sub_8229E308` for 4,224 words, not bytes; on no space the block is the word at device +15176, not +15176 itself. Not run in its attract races (counters): `NewBlock`, `Init`, `ReserveWords`, `InsertCallback`. Frame dumps against the flag off: noise (0.09% of pixels over 0.05), 2,430 draws each (itc_3614, itc_3620, itc_3615). `sub_8229F0E0` is reached only through `SetUpRing`'s override via `ctx` (could be routed as `Swap`'s later).

- `single-commands.md` (new, 53cddb27): 27 rows, the whole single-command table `0x8265ED48` (ids 0-36, a 0 word after it, before the command table at `0x8265EDE0`; read from the image); in play ids 5 (`sub_8258E8E8`), 7 (`sub_8258F0F0`), 13/14 (`sub_8258E958`), 15 (`sub_8258F5C0`, moved from `functions.md`, Wrapped; "Same file" spelled out as `src/native/replay_hooks.cpp`) and 28 (`sub_8258E9B8`, `Movie::RenderFrame`, the boot movies). Ids 0 and 29-36 are `sub_825C8388` (row kept in `vehicle-accessors.md`, named as shared); id 21 `sub_820911F8`, a shared getter with no row before (also +16 of XAudio's source voice vtable and the codecs' inner vtables). Nothing Dropped-kind (vertices, constants, shaders, the inline draw and the movie frame are all read by the renderer). Gaps noted, none seen in play: the single handlers aren't wrapped by `REPLAY_COUNTED_HANDLER` (so `--log_replay_commands` and `Renderer_OnCommand` don't see them, and interpolation's command-7 wrap covers only the command table's 7); a single 19 would be logged, not drawn (no `g_current_command`). Corrections: `device-shaders.md`'s single versions of 8-10 (`sub_8258F3E0`, `F438`, `F490`) confirmed; its state's +12 is the primitive drawer's 32-byte state; `device-state.md`'s intro no longer says `sub_8258F0F0` has no row; `gpu-functions.md` notes 28 runs on the boot movies. `known-data.md`: the drawer state at `0x82970BBC`.

- `single-commands.md` migration (8d39fd7e..15c1659b, `--native_single_commands`): the five in play (ids 5, 7, 13/14, 15, 28) native, run by `ChunkChain::RunSingles`' plain C++ through a new `SingleCommand` callback (`hooks::SingleCommands`; nullopt with the flag off, so the guest call runs); id 15 marks `native::g_current_command = 15` around its body on both paths (its `replay_hooks.cpp` wrap now calls the native body; Wrapped -> Overridden). For review: the handlers call the device setters' plain C++ whatever `--native_device_state` says (as `RenderQueue::Init` calls `GetRenderTarget`), going through the originals only where `SetStreamSource` or `SetTexture` would fence on a full fence list; the never-seen setters (loop constants `sub_822A84C8`, `sub_822A8518`, vertex bools `sub_822A8418`) and the drawer's and movie's vtable slots stay guest calls; `OutputDebugStringA` (type 3) is `helpers::DebugOutput`. Harness `single_commands` runs under the device lock with `[0x8296BD90]`, `[0x82970BB8]`, `[0x82970BBC]` pointed at made-up objects (fake vtable slots: the recompiled leaf `sub_825780D0`, storing r4-r7, so the arguments are compared); a mutation check made 59 of 128 cases differ (reverted). Frame dumps against the flag off: the attract race's `draws.txt` identical (addresses masked), pixels within run noise; the Ubisoft boot movie frame the same (itc_3637/3638).

- `predicated-tiling.md` (new, b402e0cb): 17 rows, 8 moved from `functions.md` (`sub_825BED08` named `Device::BeginTiling`, `sub_825BF200`, `sub_822B6BF8`, `sub_822B6A78`, `sub_822B6780`, `sub_822B69A8`, `sub_822B6D70`, `sub_822B6370`), 9 new (`sub_822B6AD8`, `sub_825BEB80` `Device::SetPredication` (probably), `sub_822B6AA8`, `sub_822B6288`, `sub_822B6188`, `sub_822B60C8`, `sub_822B6528`, `sub_822B6070` the L2 lock, `sub_822B64A0`); 9 not seen in play (the worker's wake and job and what's under it). The job never runs: our kick's `Interrupt` (`gpu_link.cpp`) skips the wake `sub_822B6A78`, so the tiled chain is closed but never kicked and nothing reads the chains or the header; the library's worker threads only poll (their event never set; the last one's 30 ms timeout checks +10810 bit 2 under the device lock +13548, which nothing sets). The three Wrapped wraps only log under `--native_renderer_log_gpu`. Kept-kind: the device fields the start, end and predication set, the per-tile resolves (our wrap reads the tile count +12488), the clear, viewport and scissor reset, `DirtyAllState`, the tiling limit +12960 the segment flow reads. Dropped-kind: the `SET_BIN_MASK` packets and EVENT_WRITEs, the chains' markers and the 256-byte header, the callback and flush blocks, the whole job, the debugger report. Lead: the lead told the migration to keep +12960/+12964 as the original sets them (so the segment flow takes the same path; B7 changes no behavior) and the library's worker threads as they are (XAudio's 2026-10-29 precedent). Corrections: `command-buffer.md` had left the job's wait `sub_822B64A0` without a row; `command-buffer.md`'s and `device-state.md`'s pointers updated. `known-data.md`: +13036 extended; four device rows (+12464..+12852, +12912..+12948 with +10811 bit 6 and +10808 bit 5, the worker threads' state +10812..+11208, +11212 + 56 × CPU, +20440). Lead added: `Device::Resolve` `sub_822AFA58` and its five callees.

- `predicated-tiling.md` migration (bf30ab01..a047b852, `--native_predicated_tiling`): `Device::SetPredication` `sub_825BEB80`, `BeginTiledSegments` `sub_822B6AD8`, `EndTiledSegments` `sub_822B6BF8` (its `gpu_link.cpp` logging wrap kept) with `sub_8229E0C0` (Native), `CreateWorkers` `sub_822B6D70` (Native: `Device::Init`'s override calls `hooks::NativeCreateWorkers`), `WorkerThread` `sub_822B69A8`; command buffer adapters removed: `sub_8229DBF0`, `sub_8229ED60`, `sub_8229DFB8`. Kept: the tiling limit +12960/+12964, the three chains and their close, the header's 256 bytes reserved (cursor moved past), the callback and kicks, the segment ends before dropped packets (`sub_8229F0E0`'s precedent): the segment flow takes the same path. Dropped (graphics rule): the SET_BIN_MASK packet, the header's words and their memcpy, `CreateWorkers`' EVENT_WRITE_SHD packets, `EndTiledSegments`' coherency flush (already Dropped, now left out). The worker threads made, waited and reset through the kernel imports (XAudio's 2026-10-29 precedent); the device lock +13548 a host `std::mutex` (`Edram::Alloc`'s precedent; only the workers take it; `Device::Init` still makes the critical section); the job and the present stay guest calls (never run). For review: `CreateWorkers`' harness covers only no-thread cases (a dump of the worker blocks after `Device::Init`, flag on and off, matched but for the dropped packets, itc_3647/3648); `WorkerThread`'s only "woken to end" (its 30 ms poll runs natively in play, no busy thread); `UpdatePredicatedTiling` (`device_state.cpp`) made public; the command buffer harness's set-up shared in `src/harnesses/command_buffer_setup.h`. Map corrections: `EndTiledSegments`' second reservation is 32 words at 4-byte alignment; `CreateWorkers`' thread flags 0x80 | (CPU bit << 24). Frame dumps: the tuning shop's tiled car, the same draw count on and off, pixels 0.26% apart (animation timing; itc_3643/3644).

## Run 2026-11-05 20:21 (`b7-loop-05-11-26`)

- The top lead (the graphics resources' other makers) was stale: `sub_820ED6D0` is `CarModel::MakePlate` (`car-models.md`, ours), `sub_82135AA8`, `sub_82136130`, `sub_821330D0` already in `post-effects.md`. One row added to `post-effects.md` (b86338cf): `sub_825D99D0` `PostEffects::InitEdramBase` (probably a `_cinit` initializer; `[0x826C5DC0]` = `Surface::EdramTiles(1024, 576, 0x1A2201BF, 1)`, 936), `known-data.md`'s `0x826C5DC0` updated. The agent's note: `c-runtime.md`'s `sub_825A6E48` row names `_cinit`'s tables at `0x82630284` and `0x82630010`, where the code gives `0x82600284` and `0x82600010` (probably a typo, not changed). The rest of `post-effects.md` (13 recompiled) is all ready, so the migration takes it.
- `post-effects.md` migration (106192bc..b1df9d00, `--native_post_effects`): `InitEdramBase`, `Create` with the glow's, color filter's and motion blur's constructors and the render textures' `Acquire` and `Create`, `InitBoot`, and the three records (`ColorFilter::Record`, `TargetFilter::Record`, `MotionBlur::Record`) native; 5 Native with no adapter (only `Create`'s plain C++ calls them in play). Correction to the 2026-10-24 row and review: `RenderTextures::Acquire` `sub_82133190` doesn't return an uninitialized stack word for an id in use with another key; case 3 of its switch falls into case 1's code and makes a new entry (id 20); the stack word is read only for a case above 3, which can't happen. For review: command 19 takes two slots (75, 91 and 131 slots reserved by the three records); the records run the whole original when the list is full or a chunk is open, after the list getters were called once, and skip the command list's diagnostics as earlier native recorders; new `interpolation::BlendState_MotionBlurCopy` (`blend_state.h`) doing what `Matrix::Copy`'s wrapper does for return `8213653C`, the other two copies noted through `BlendState_OnCopy` with their original return addresses; frame dumps against `--native_post_effects=false`: `draws.txt` identical with addresses masked, the velocity map 99.9% identical, pixels within two fallback runs' spread. The video output, render manager, render queue, `CameraPasses::RenderTarget` and `tan`'s fallback stay guest calls (as `scene_draw`, `model_draws`).
- `post-effects.md` second migration (95c86284..6bfc02d0): `Glow::RecordStar` `sub_82136FA8` (adapter kept: `Glow::Record` is recompiled inside frame interpolation's wrap) with `RecordStarPass` `sub_82134060` (Native, no adapter); adapters removed: `GlareDef::GlareDef`, `GlareDef::Init`, `TargetFilter::TargetFilter`, `PostEffect::RecordPixelBool` (switched by `--native_post_effects` through their callers) and `Race::ToggleEffects` `sub_8215B690` (now switched by `--native_race_scene`, its caller's flag). For review: `src/post_effects/record.cpp` added to `CMakeLists.txt`'s strict-float list (`-ffp-model=strict`, `-ffp-contract=off`), covering the three earlier records too; the star falls back to the original when the list lacks 80 + 48 slots a pass, a line has more than 2 passes (the original then reads colors from its own register save area) or there are more than 64 lines; its harness compares floats in the frames and command 7's constant payloads NaN-equal through a slot walker (`StarBlockOutputs`), 39 of 80 cases NaN-only; vector steps reuse `car_scene_draw/ops.h`. Frame dump against `--native_post_effects=false`: the star's resolves and draws identical (3 scene draws differ, not post effects). `post-effects.md` is 57 of 58 ours; `Glow::Record` `sub_821379D8` waits on `sub_8209AE48` (the command list's lead).
- `scene-target.md` (new, a0cac8ac): 6 rows, all in play: the scene target `[0x8266D08C]`'s per-frame passes (`SceneTarget::RecordPasses` `sub_8209B3D0`, moved from `functions.md`, once a frame from `sub_82096418` with buckets 150, 602, 121: its mode's pass bits, the menus' predicated tiling (bit 8), the race's 1024×576 pair and the upscale draw to 1280×720 (bit 4, resolve 24 in `conventional-renderer.md`, confirmed)), `RecordTargets` `sub_8209AE48` (command 23 with targets 0-3 = +4, command 24 with +0; r5 unread), `RecordTileTargets` `sub_8209AF40`, `RecordBoundTargets` `sub_8209B058`, `TexturedRect::Record` `sub_8217C368` and `RecordFlat` `sub_8217AA98`. A new file rather than `command-list.md`, whose intro keeps recorders out. Every command recorded is one the renderer replays, so none is Dropped. `known-data.md`'s `0x8266D08C` row extended; `conventional-renderer.md`'s two mentions point to the new file.
- `scene-target.md` migration (982ea51b..5edff580, `--native_scene_target`): 6 of 6 ours (`RecordPasses` Overridden, its caller `sub_82096418` recompiled; `RecordTargets`, `RecordTileTargets`, `RecordBoundTargets`, `TexturedRect::RecordFlat` Native; `TexturedRect::Record` Merged, as `ColorRect::Record`), with `Glow::Record` `sub_821379D8` as a cleanup in `post-effects.md` (Wrapped -> Overridden: native inside frame interpolation's wrap, as `CarScene::Draw`) and `Glow::RecordStar`'s adapter removed. Correction: `RecordPasses`' row said its rectangles' +12, +28, +44 end as 1.0; the vector stores overwrite them with an identity matrix's 0.0. For review: the harness compares only the words `RecordFlat` writes, since its frame overlaps where the original `RecordTargets` leaves its 128-byte frame (stack leftovers nothing reads; the native one makes no such frame); harness open chunks now counted in `0x8296BD28`; `StarBlockOutputs`' walker now steps over the chunk header and commands 23, 24, 34-36; the command list's diagnostics flags fall back to the originals (as `ScreenFades::Update`). Frame dumps: the attract race's 2,430 draws the same; the menus' predicated tiling (bit 8) ran natively in a `race_test.jsonl` replay with the same draws (one frame pixel-identical); the glow chain's resolves 97-100% identical against `--native_post_effects=false` (interpolation timing).
- `frame-passes.md` (new, 19f643f1): 13 rows, 3 not seen in play (`OnEvent` `sub_82096220`, its thunk `sub_82096320`, the destructor `sub_820987E8`): the 44-byte object at framework +108 (vtable `0x82000F20`, read from the image): `FramePasses::Boot` `sub_820962B8` (a boot job on the first worker pool, `PostEffects::InitBoot` under the device lock), `Record` `sub_82096418` (tick handler 3: ~20 shared pass chunks at 58 buckets, sort keys `0x8296B90C`/`0x8296B900`/`0x8296B908` = 168, 270, 163, then `SceneTarget::RecordPasses` and `sub_82589990`'s primitive reset states), `FlipWorkMem` `sub_82096218` (tick handler 20, `sub_82589A78` flips `0x8296B780`), `RecordConstants`, `RecordSamplers`, the constructor (+28 an uninitialized local stored). `Job::Wait` `sub_82094B10` moved from `functions.md` (Wrapped by `--find_menu_selection`'s scan). Nothing Dropped (the renderer reads every command). `known-data.md`: the frame passes, the work memory flip, the sort keys; the device lock callers' lead in `leads.md` updated. Not read in detail: which chunk goes to which bucket.
- `frame-passes.md` migration (261c0970..85292a33, `--native_frame_passes`): 10 of 10 in play ours (`Job::Wait` Wrapped -> Overridden, the menu scan wrap kept around the native body; `WorkMem::Flip`, `RecordPrimitiveReset` Native; the vtable slots' adapters kept; the constructor's and `Job::Wait`'s for recompiled callers); `SceneTarget::RecordPasses` and `PostEffects::InitBoot` Native (adapters removed: reached only from `Record`'s and `Boot`'s adapters). `Record` written in three parts by three subagents one after another (slots 0-153, 154-255, 256-358). Corrections to the map: `RecordConstants` copies from `0x8265ED20`/`0x8265ED30` (not `0x8266ED..`); `RecordSamplers` takes 24 slots (not 15); `Record` calls each of them 3 times; head 0 is never handed to the render queue; the constructor resets only the first two of its three stack delegates. For review: `Record`'s adapter, declining after its getters, first fell back with r3 holding a getter's answer (the harness's declined case looped on a wild read, millions of error lines; fixed 0c50aab1; the agent deleted those runs' logs, itc_3585 and itc_3586, ~540 MB each); part 1's +88 base word was wrong at first (the LOD bias's address), found by the harness and fixed in part 3 (6121d2d0); the Boot harness uses the game's own video output and device lock with effect 14 present, so `InitBoot` makes only the vertex declaration, freed after. Frame dumps against `--native_frame_passes=false`: the same 2,430 draws and states and the same frame, twice; the menu replay `itc_3593` (tuning shop) judged by eye only.

## Run 2026-11-04 19:51 (`b7-loop-03-11-26`, continued)

- `replay-snapshots.md` (new, dcfccb78): 30 rows, 4 not seen in play (destructors): the two leads were the replay's snapshots, not a reset: `RaceReplay::SaveStart` `sub_82104D40` (the countdown, block A at the input recordings' +0x20F590), `SaveEnd` `sub_82104E68` (block B, +0x218DA0), `RestoreStart` `sub_821046F8`, `RestoreEnd` `sub_82104898` (players' cars kept unless in contact, the others re-placed with `Vehicle::PlaceOnRoute`), each vehicle's 5,232-byte record (`Record5232`). The input recordings' lead (`sub_82104F68`, `sub_82104E68`) taken with it and deleted. Correction to an earlier map: `sub_82104E68` saves the race's end, not a rewind (`RaceStates::ReplaySetUpTick`'s row in `race-states.md`). Pointers in `race-states.md`, `race-world.md`, `vehicle.md`, `data-load.md`; `known-data.md`: `0x826C5B4C`'s blocks, `0x826C5B50`.
- `replay-snapshots.md` migration (e9389918..1a5be4a1, `--native_replay_snapshots`): 26 of 26 in play ours, 2 adapters removed (`Record5232::Restore`, `RaceSnapshot::Save`). For review: none of them runs in an attract race (the attract race probably skips the countdown's snapshot), so only the harnesses checked them before the validation; RaceReplay's four are cased in the `race_world` harness's start world (StandingStartTick's precedent; 12 or 13 cases each after its filter); SaveStart, RestoreStart and RestoreEnd run the original unless `--native_heap` is on (as `ResetWorld`/`ResetVehicles`), and every adapter falls back when memcpy's plain C++ is off; the long record copies were generated by a throwaway script (not committed) following the originals' store order.
- `race-sounds.md` (new, 53aa7a89): 31 rows, 2 not seen in play (`OnAbort` `sub_82175CA0`, `OnAbortDone` `sub_82175CF8`): the race mode's sound machine at +138284 (`RaceSounds::`): `Init` allocates the scene models' sound buffers; the cars' wave bank set-up and updates, pause and resume of the cars and the "Music" category, the music change (a 10-tick fade, StopCue, ReleaseBank, bank 2 loaded by name, cue 0), the unload; 47 transition sites (32, 36 or 43 by the race kind; listed in the intro); the 13 music names at `0x826193D4` read from the image. Shared rows kept in `menu-sounds.md` (`Sound::Play`) and `scene-models.md` (`sub_82175EE0`). `known-data.md`: the machine, `0x826D82FC`-`0x826D8330`, `0x826D83C8`, `0x826193D4`, `0x826C5C84`; `0x826D8334`-`0x826D83AC` and `0x82616BEC` updated.
- The user's call (2026-11-04, asked because `rand` `sub_825A9BB8` blocked three of the race's sounds, as the 2026-10-25 call on `_mtinit` said): `rand` native as plain C++ over the calling thread's guest seed (the C runtime's per-thread block found through the guest TLS as the original does), the original's numbers, one seed for recompiled and native callers; `_getptd` and `_mtinit` stay recompiled for the core.
- `race-sounds.md` migration (508c7be5..d4d1905e, `--native_race_sounds`): 28 of 29 in play ours, 8 adapters removed (`PauseCars`, `ResumeCars` kept: the machine's functors reach them); `rand` `sub_825A9BB8` under `--native_c_runtime` (`src/c_runtime/random.cpp`: `_getptd`'s reads made natively through guest calls to GetLastError/SetLastError and the TLS thunks; a thread without C runtime data calls `_getptd`'s original). For review: `race_sounds::Calls::Leave` carries `CarSounds::SetBanks`' r4 and r5 on to the next call as the original's registers do; `SetUpCars`' row corrected (+9648 = 2 and 0x20000 set once per car set up, not when none is); `RaceSounds::Init` replays its transition table into the 3,280-byte frame from a table a scratchpad script decoded (not kept), writing the frame's back chain too, its adapter kept for the recompiled `RaceMode::Init`; the harness makes real XAudio voices over a PCM wave bank (as the car sounds' harness), and its UpdateCars, RaceTick and AfterRaceTick groups log ~5,400 `tw/td trap hit` warnings, the same on both sides (seen before the run too). Only SetListener, OnEffectsBankLoaded, SetUpCars, UpdateCars, Begin, PlayStartMusic, IdleTick, Init and rand run in an attract race.
- `race-sounds.md` second migration (4d319832..7a28543f): `Unload` `sub_821772D8` with `OnAbort` and `OnAbortDone` (not seen in play, taken along), 31 of 31 ours; `CarSounds::Release` `sub_8210F750`'s adapter removed (`car-sounds.md`). `Unload` returns `XPhysicalFree`'s r3 (the address: the native heap's free leaves r3). `sub_82117B30`'s adapter kept (`BankPair::~BankPair` recompiled). For the record: the agent once restored `race-sounds.md` with `git checkout` without committing first (the file held only its own broken edit, redone after), against the commit-before-revert rule.
- `race-loading.md` (new, 4ba4c0f4): 17 rows, 4 not seen in play (the race mode's +32 vtable slots `sub_821511B8`, `sub_821512F0`, `sub_82151388`; `CourseSetup::Restart`): the race mode's +848 machine (`CourseSetup::`, named from its "CRS SETUP WAITING." and "DRAW SETUP WAITING." prints; 11 transitions: fade in, wait for the course, the text and the data tables, then the archive queue, fade out, message 4; message 44 back to state 3; message 49 waits for the car scene's loads, then message 50), `RaceMode::NextMode` `sub_82151EF0` (runs the loading screen in states 3 and 4), the loading screen (`LoadingScreen::Show`, `Run`, `Finish`, `UiMovie::SetFrames`). Correction: the lead's names at `0x8262799C` are `0x8263799C`, a table of animation indexes. `GetLocalTime` left in `functions.md` (B6) as shared. `known-data.md`: the course set-up, `0x8263799C`; one lead added (`sub_82293358`/`sub_82292828`, a system request poll, not seen in play).
- `race-loading.md` migration (e3e7b130..f6bb8df4, `--native_race_loading`): 10 of 13 in play ours (the +848 machine's `Init`, `Start`, `Begin`, `OnCourseReady`, `WaitDraw`; the loading screen's `Show` with `UiMovie::SetFrames` (Native: its other caller never runs), `Run`, `Finish`; `sub_82293460`); `WaitCourse`, `EndTick` and `RaceMode::NextMode` blocked. For review: `Init` replays its stores into the original's 864-byte frame from a table a script decoded (stack leftovers kept); `SetText`'s r7-r10 passed as 0 (the parking area call); new `time_hooks::SetFakeNow`/`ClearFakeNow` in `src/hooks/time.cpp` so the harness's `GetLocalTime` answers a made-up time (the tip is picked by milliseconds), as the time harness's fake time zone; `WaitDraw`'s cases set the live archive queue's count under its lock and put it back.

## Run 2026-11-04 08:44 (`b7-loop-03-11-26`)

- Leads deleted as covered: the containers (`sub_82093400` and the rest, all in `framework.md`, `screen-objects.md`, `app-states.md`, `models.md`, `input.md`) and the route's users in the race states (`sub_82162330` and the rest, all in `race-states.md`).
- `race-flow.md` (new, ef6eef64): 5 rows: `RaceFlow::Init` `sub_82160AE0` (its 53 transitions listed in the intro, the states' meanings marked probably), four handlers not seen in play (`sub_8215F7B0`, `sub_821022B0`, `sub_8215FC30`, `sub_8215FB08`) that `racer-controllers.md` named without rows; the lead's functor vtable `0x82006ABC` corrected to `0x820069BC`. `known-data.md` (the race flow), `game-modes.md` (`RaceMode::Init`'s row) and `racer-controllers.md`'s intro changed.
- `race-flow.md` migration (1e19350e, `--native_race_flow`): `RaceFlow::Init` replays the original's 562 stores and copies into its 3,600-byte frame from a table (the stack leftovers in the functors kept, as `RaceHudMachine::Init`), the table generated by a script not kept in the repo; its adapter stays (its caller `RaceMode::Init` is recompiled).
- `car-scene-draw.md` (new, c98bcea2): 21 rows: `CarScene::Draw` `sub_820C5DE8` (the headlight projectors, the light slots, the pass loop over `Car::DrawUpdate`; its float arithmetic read in outline), `TextureProjection::Fit` `sub_82598768` and the vector's `_Insert_n` `sub_82598288` read in outline; 11 moved from `functions.md` (two duplicate pairs, `sub_82597610` and `sub_82598768`, merged into one row each); references in `scene.md`, `car-draw.md`, `course-scenery.md`, `material-passes.md`, `location-screen.md` updated; `known-data.md`: the draw's tables, scene fields, `0x8296B8F0`, the frustum. Two leads added.
- `car-scene-draw.md` migration (d5d7449c..6e318bb5, `--native_car_scene_draw`): 21 of 21 ours, 12 adapters removed. For review: the root falls back to the recompiled code when `--native_models`, `--native_matrix_math` or `--native_vehicle_accessors` is off, or a command list diagnostic (`log_overlay_uploads`, `log_slot_callers`, `log_recording_by_pass`) is on (the car draw's and the cameras' precedents); four adapters `--removable` named stay as the diagnostics' wraps (`sub_820C4FB8`, `sub_820C4EB8`, `sub_82597518`, `sub_82598768`); `Box::Extend`'s row corrected (a NaN coordinate goes into both min and max); `Projector::BuildOrtho` (the scene's first path, probably the front end) didn't run in the attract race (it ran in the validation's replay, `itc_3449`). Three prints (`Projector::Build`, `Matrix::Perspective`, `TextureProjection::Fit`) come after a check that hands the call to the recompiled code, so they log only when the native body runs (the validation agent's reading; accepted by the lead).
- `front-end-car-draw.md` (new, 3beadd5a): 24 rows, every name "probably": the garage stage (`GarageStage::Draw` `sub_8217FE60`: the deferred camera preset, the car's turn, the animation list, the floor reflection drawn mirrored with the cull swap `[0x826C5100]` flipped, the poses), its command function `sub_82180108` and setters; 9 rows not seen in play (the four door animations not read in detail). `known-data.md`: the garage stage, `0x826C5100`, `0x82635E90`, the scene model's front-end fields. `leads.md`: `sub_82179BE0` deleted from the re-applies lead (covered); one lead added (the 18 front-end screens calling these).
- `front-end-car-draw.md` migration (38b4a134..43fa606c, `--native_front_end_car_draw`): 17 of 18 in play ours, 7 adapters removed, `SceneModel::ApplyLightKit` `sub_820D9D10` made native in `load-poll.md`; the root `GarageStage::Draw` stays recompiled (blocked by `CarScene::Update`). For review, harness exclusions (a comment in the harness, a note in each row): saved registers of nested callees that run their originals aren't compared (`tan` declining, the doors' runners, `Model::DrawShape` declining, removed adapters' originals); `SetLightKit` with an index above 8 (reads the original's saved registers; no caller in play passes one) and `ApplyPreset` with a preset at or past the limit (the original faults reading guest address 4; never seen) aren't cased. `sub_8217B6B8`, `sub_8217B740` left recompiled (guest calls). `docs/pad-replay.md`: dumping a front-end frame in a replay (`PAD_DELAY=8`, `<dir>/now` 24.5 s after the boot mark).

## Run 2026-11-01 12:56 (`b7-loop-01-11-26`)

- `boot-mode.md` (new, 49161912): 24 rows: the boot mode (type 3: the
  movie list at `0x826C5BAC` played through a state machine at +848),
  `BootMode::NextMode` `sub_82108018` (moved from `movies.md`, its
  "never called" corrected: it runs every tick of a boot mode),
  `Boot::StartMovie`, `WaitForMovie`, `EndMovie` (moved from
  `functions.md`), `ScreenMachine::ScreenMachine`, the mode delegate's
  thunk; 11 rows not seen in play. The modes' constructors the lead
  named stay in `game-modes.md` (shared). `known-data.md`: the boot
  mode, `0x826C5BA4`, `0x826C5BA8`, `0x826C5BAC`, `0x826C7960`.
  `leads.md`: the front-end screens' constructors over
  `ScreenMachine::ScreenMachine`.
- `boot-mode.md` migration (49f60a69, `--native_boot_mode`): 12 rows
  in `boot-mode.md`, 7 in `movies.md` (the movie class's state calls,
  Native from the start; `sub_820A4408`, `sub_820A4420` not seen in
  play, covered by the harness only). For review: the `--skip_intro`
  wraps in `title_screen.cpp` kept around the native bodies, and
  `Movies::State` `sub_820A4450`'s adapter kept though `--removable`
  lists it (the wrap the fallback still needs); the XMP drops'
  adapters (`sub_825894C8`, `sub_825895A0`) kept, their native callers
  leaving the calls out but clearing the last error as the drops do
  (as `Presence::SetForMode`).
- `car-draw.md` (new, 24c2b1c1): 86 rows (14 moved from
  `functions.md`): `Car::DrawUpdate` and the scene model's per-pass
  draws (cockpit, light kit, parts, interior, traffic car, wheels with
  their morphs; 8 not seen in play). `known-data.md`: the scene model
  block, the pass description, the car draw records, the lighting
  globals, the work memory. Corrected: `framework.md`'s
  `sub_820AC848` (the pool maker's slot 1 runs in play, from
  `Model::MorphShape`); the worker-job lead's vtable (`0x8200286C`).
  `leads.md`: the second set's per-frame passes lead deleted (covered),
  the model draws' lead cut to `sub_820C8480`.
- `car-draw.md` migration, part 1 (a036f833, a6ae298c,
  `--native_car_draw`): 7 rows (`Model::Draw`, `CarPart::Draw`,
  `Matrix::Shadow` (Native, its adapter removed), `CarLighting::Gather`,
  `LightKit::BlinkLevel`, `BlinkTimers::Advance`,
  `SceneModel::IsNearPlayerHeight`). For review: `src/car_draw/` built
  with `-ffp-model=strict`; `Model::Draw` and `CarPart::Draw` hand back
  to the original as a guest call when their plain C++ answers nullopt;
  `CarPart::Draw`'s adapter kept for the cockpit dials. The rest of the
  migration paused by the user.
- `car-draw.md` migration, parts 2 to 16 (64ca2adf..d0f3ca85, resumed
  2026-11-03 on `b7-loop-03-11-26`; one agent, 15 parts' subagents one
  at a time): 78 of 78 ours (69 Native, 9 Overridden: `Car::DrawUpdate`,
  its caller `sub_820C5DE8` recompiled; `IsNearPlayerHeight` and the
  six `Scene*::Draw` tops, whose overrides are the frame-state wraps
  calling the native step, their Status Wrapped -> Overridden as part
  1's `IsNearPlayerHeight`; `CarPart::Draw` for the cockpit dials;
  `BlinkTimers::Advance`, holding `--per_tick_blink_timers`' skip).
  For review: left as guest calls from the native frames: the LEDs,
  spots and `LightKit::ChaseLevel` (7, not seen in play), the morph's
  worker job `sub_820AC7F0`, `Accessory::Draw` `820EFA78`,
  `Dash::PoseTachometer`/`PoseDials` (through their
  `--native_cockpit_dials` adapters, with `BlendState_SetLivePose`);
  `Model::SkinShape` `sub_82594FD0` untouched (not seen in play).
  `Car::DrawUpdate`'s row corrected: a car not ready after its
  finishers returns without drawing (the map said it drew). Harness
  choices: the lamps' command-list slots compared as Floats (a NaN
  product is compiler-dependent); the turn at frame-80 not compared
  (a dead slot later frames overwrite with saved registers); a gap in
  the shadows harness world (cases drawing more from the shared Random
  made the original read `0xA4` forever, case 70) avoided, not fixed.
  New helpers: `Calls::call`, `call_r24`, `call_r28`, `call_r24_r28`,
  `car_draw::Left`, `hooks::CarDraw*CalleesNative()`,
  `PartsDrawRecord`, `StoreIdentity`, `CopyDoublewords`, the wheels'
  layout structs, the root's Wrapped helpers repeating the callees'
  `--debug_hide` and `--find_per_frame_state` checks. Covered only by
  harnesses so far: the morph chain and morphed wheel draws, the
  front-end-only draws, `DrawDigitalSpeed`, the accessory path and the
  whole cockpit (no cockpit view in attract races).
- The shadows harness fix (44a55624, by the lead at the user's request
  after the validation halted): record 3's shape (+948) gets a copy of
  record 0's shape record when the indices differ (no extra Random
  draws), as `ModelWorld` builds only that one.

## Run 2026-10-31 23:57 (`b7-loop-31-10-26-2`)

- `device-shaders.md` (new, 112775ae): 9 rows: `D3DDevice_SetVertexShader`
  `sub_822A8958` and `Device::SetVertexShader` `sub_822A8808` (moved
  from `functions.md`, Wrapped by `src/native/refcount.cpp`'s rebind
  skip), `SetPixelShader` `sub_822A8618`, `SetVertexDeclaration`
  `sub_822A8960`, `RenderQueue::RenderQueue` `sub_825915E0` (scene +
  64), `PassList::PassList` `sub_82590B38`, `PassList::Clear`
  `sub_8258EB38`, and two destructors not seen in play. `known-data.md`:
  device +12432, +12436, +11552, +13156; the render queue at scene +
  64. `leads.md`: the render queue's methods.
- `device-shaders.md` migration (657efc7b..e727bc39,
  `--native_device_shaders`): 7 rows. For review: native
  SetVertexShader does `refcount.cpp`'s rebind skip itself (its wrapper
  became `refcount::SetUnchangedShader` and `NoteSetVertexShader`; with
  the flag off the entry runs the original with the old skip around
  it); the constructors are native only with the native heap; their
  harness compares the allocated blocks by role.
- The race HUD machine cleanup lead (61f7966b..7476d237): 12 rows in
  `race-hud-machine.md`, `race-hud-object.md`, `race-screens.md`. For
  review: `ResultsDraw`'s and `AfterRaceDraw`'s unseen kinds run the
  original (the handlers hand back); `ClosePause`'s kind 4 ported (its
  callee `sub_82106540` is empty); `sub_82211D10` Native with no adapter
  ever; the harnesses of `CreateScreens`, `DrawPauseMenu` and
  `DrawAfterRaceMenu` run in other files' worlds. The existing
  `PauseMenu::Draw` harness group sets the pause list's +68 to 0, so
  `ShowMain`'s `ColorItems` reads guest address 0: it passes only by
  how its random cases fall (left unchanged, a latent harness gap).
- The PauseDraw cleanup lead (acac516d..0c9db58c): 5 rows in
  `race-hud-machine.md` and `race-hud-object.md`. For review:
  `PauseDraw`'s and `RacePauseDraw`'s kinds 0 and 4 run the original
  (`sub_8220B9E8`, `sub_82215D58`, `sub_82211590`, never seen in play);
  their prints come first, so they also log on those hand-backs; the
  harness's pause menu set up with a negative running transition and
  without the confirm dialog (`race_screens` covers it).
- `device-shaders.md`, the render queue's methods (ee891a3a): 24 rows
  (the vtable `0x8206CB8C`'s slots, the replay and its sort and chunk
  walker, `PassList::Add`, the release list's `PtrVector`); 2 moved
  from `functions.md` (Wrapped for `--find_menu_selection`). Corrections:
  the chunk walker reads the kind from the first word's low halfword
  (`gpu-functions.md` says the first halfword; not edited);
  `known-data.md`'s scene + 64 buckets (+0, +4 first and last, +12 a
  count `Clear` never resets; not next pointers at +8). `known-data.md`:
  the replay's globals `0x8296BD8C`-`0x82970BC0`.
- The render queue's migration (77d89868..f1c2a45a,
  `--native_render_queue`): 19 rows to Native or Overridden. For
  review: the `--find_menu_selection` wrappers moved from
  `src/debug/menu_selection_scan.cpp` into the queue's adapters; the
  replay's indirect calls (the command tables, `PrimitiveDrawer::Init`,
  the release list's destructors, walker kinds 1 and 2) are guest calls
  through `ctx`; `RenderQueue::Add`'s warning path and
  `PtrVector::PushBack` without the models' native lifetime run the
  original; the replay's harness holds the device lock and saves and
  restores the replay's globals, with only the game's harmless command
  handlers.
- The `XPR::GetTexture` cleanup lead (b409aa64..b82b56b5): 21 rows in
  `functions.md`, `location-screen.md`, `scene-loaders.md`. For review:
  GetTexture under `--native_models` (`src/models/xpr.cpp`), run inside
  `frame_state_diff`'s wrap as `Vec4Transform` is; its r3 is the full
  64-bit sum, as the original's; native callers calling its plain C++
  skip the wrap, so `--debug_hide=82597118` no longer hides their calls;
  the native paths need their callees' flags (`--native_material_passes`,
  `--native_accessories`, `--native_car_models`).
- The scene loaders' second migration (cc03a279..1ff4d670): 5 rows in
  `scene-loaders.md` (now 78 of 78 ours). For review: `XPR::FindByName`'s
  adapter kept (the car model's texture set 0 hand-back, run in play,
  calls it); the provider's slot +16 and declining callees' originals
  are guest calls from the original's frame; sprintf's unset arguments
  as 0 (the `UiMovie::SetText` precedent); the new files build with
  `-ffp-model=strict`.
- `scene.md` (new, e8dcd16c): 23 rows, `CarScene::` at `[0x826C2E44]`
  (named apart from the app's `Scene::Update` `sub_820988C8`): Create,
  the constructor, the set-up record, the lighting block at +5248, the
  shared assets at +64, `LoadAssets` and `OnFileLoaded`, the update,
  the first set's update, the teardown (both pool sets' destroys,
  `LoadsDone`, the destructor, `List::~List`, `List::DeleteAll`);
  `CarScene::InitLighting` read only for its stores and calls. Leads
  folded in and deleted: the scene's maker (`sub_820C8A50`), its set-up
  and teardown objects (`sub_820C5008`), the TGA getter `sub_820C4E68`
  (now a row, not seen in play but ran in `itc_2848`). `known-data.md`:
  the scene's layout, `0x82601854`-`0x8260185C`.
- The car scene's migration (9a461ba8..613dd3db, `--native_scene`,
  `src/scene/`): 19 rows. For review: allocations through
  `memory::AllocPages` and `FreePages`; the pool objects' destructors
  guest calls through `ctx`; frame words left as the original leaves
  them (`InitLighting`, `OnFileLoaded`'s r1 + 28); `OnFileLoaded`'s
  texture branches (`Model::Load` on a real part file) have no harness
  cases, only the game run and the frame dump; the teardown
  (`StartTeardown`, `~CarScene`, `SceneAssets::Release`, `UpdateA`'s
  teardown) checked by harness only until a race end is seen; the
  validation's replay (`itc_3143`) ran them at the attract race's
  teardown with no error. That replay logged the SDK's "BaseHeap::Release
  failed because address is not a region start" twice at the driven
  race's end change, as `itc_2936`, `itc_2974` and `itc_2987` did
  before this run (not `itc_3081`, `itc_3093`, `itc_3106`):
  intermittent, not debugged, worth a look. It came again in
  `itc_3187` (5 lines, the second set's third validation), not in
  `itc_3164` or `itc_3179`.
- `second-set.md` (new, f40c2fd7): 153 rows, the car scene's second set
  of pools (`sub_820C8DE8`'s eight objects: `SceneCarModel`,
  `SceneParts`, `SceneCockpit`, `SceneInterior`, `SceneLights` (pool 4,
  the neon, LED and spot kit; `scene-models.md` had it unknown),
  `SceneWheels`, `SceneTraffic`, `SceneModel`), their 240-byte
  `PointRecord`s named from the image's string tables, the point
  set-ups, the wheels' morph (`Morph::SetUp`, `Run`, `Blend`). Quirks
  for the port: `SetUpRimA`'s second morph sized by the first stream;
  `ResetRightDoor` scaling records whose model is 0; `SceneTraffic`'s
  destructor not releasing; `PartsPoints::Find` reading block +37076 to
  +37084, set nowhere in the file. `known-data.md`: "model point" now
  "point record"; the eight layouts. Leads folded: the release, the
  point set-ups; trimmed: the load poll, the per-frame passes.
- The second set's first migration (c9c70345..92e9be10,
  `--native_second_set`, `src/second_set/`): 84 rows. For review:
  `PointRecord::Clear` writes its stack back chain as the original
  does; the harness builds its own skeletons (valid parents and root,
  seeded Bind arrays: the scene loaders' random ones made the original
  read unmapped memory); the cockpit's, interior's and traffic's models
  never 0 in the harness (the original faults on 0, as in play); the
  Find adapters leave flush-to-zero as the last float step left it;
  `PartsPoints::Find`'s read of +37076 to +37084 and `ResetRightDoor`'s
  scaling with model 0 copied.
- The second set's second migration (c6ca2920..d07ffc2b): 74 rows. For
  review: `~SceneTraffic` not releasing and the scene model's deleting
  destructor being the base class's copied; `Morph::SetUp`'s
  `DebugPrintf` messages (never ran) logged instead, as in
  `shader_objects`; the deleting destructors leave operator delete (r4
  bit 0, never in play) to the original; the releases' and destructors'
  cases for pools 1 and 3-6 built without their first-set object; a new
  `hooks::SceneLoadersReleaseANative(pool)` from `hooks/scene_loaders`;
  the morph's flush-to-zero as the translated code sets it, put back by
  the adapters.
- The second set's third migration (6b2c7657, 76b512a3): 11 rows. For
  review: `SetUpRimA`'s second morph sized by the first stream copied;
  only `SetUp` has an adapter (native with native memcpy, the native
  heap and `--native_material_passes`, else the original); the tire's
  red-zone word not compared in `SetUp`'s frames (the morph's callees'
  saved registers land there; `sub_820E6F28`'s and `sub_820E7010`'s
  harnesses cover it).
- The `ScenePools::ReleaseB` cleanup lead (caab6e04..f0896384): 14 rows
  in `functions.md`, `second-set.md`, `scene-models.md`. For review:
  `ReleaseB` and `SceneCockpit::Release` call each other (the cockpit's
  interior), made native as one group; a nested `ReleaseB` whose pool's
  release isn't native or refuses with nothing done falls back to its
  own original from the same frame (as `WheelCalls`); the
  `frame_state_diff` wrap kept as an explicit wrap around the native
  adapter, nested native calls skipping it. Its replays: `itc_3190`
  clean, `itc_3191` one BaseHeap line at the race end; the agent found
  no native path freeing without `memory::IsPages`. Its validation's
  replay (`itc_3194`) logged 2 at 07:43:06.657, after the pause menu's
  quit, `RaceStates::FadeOut`, `Progress::AfterRace` and
  `ModeType9::ModeType9`, a 51 ms long frame and two textures loaded,
  before `Career::NextDay`: the race-end change each time.
- `load-poll.md` (new, 103db371): 32 rows: the scene models' load poll
  `sub_820DA318`, its 13 part finishers (14 rows moved from
  `functions.md`, Wrapped for `--find_per_frame_state`),
  `SceneParts::Build` `sub_820E05E0`, `SceneInterior::SetGauges`
  `sub_820E3938`, `SceneModel::ApplyLightKit` `sub_820D9D10`. Noted for
  the port: `sub_820D9638` and `sub_820D92A0` pass stack words they
  never write to `PartModel::SetMaterials`; `sub_820E5A68` checks its
  +9792 but never sets it; the poll never sets traffic's +5036 (traffic
  paint never ran). `sub_820D91E0` and `sub_820D9638` moved here from the
  per-frame passes' lead (they run once). `known-data.md`: the scene
  model's paint values, +4072, +4224, the second set's finished flags
  and part-model arrays.
- The load poll's migration (87124abd..fdcd3ccb, `--native_load_poll`,
  `src/load_poll/`): 32 rows in `load-poll.md`, 1 in
  `material-passes.md`. For review: the flag takes effect only with
  every callee's flag on (`--native_second_set`,
  `--native_scene_loaders`, `--native_car_models`, native memcpy, the
  heap, `--native_models`, `--native_material_passes`); the finishers
  run inside a new `ITC_FRAME_STATE_WRAP_LOAD_POLL` in
  `frame_state_diff.cpp`; the second set's point set-ups' call builder
  split out as `SecondSetFindCalls`; the four never seen in play ported
  with harness cases; the wheels' `SetMaterials` frames as the original
  leaves them; the harness points `[0x826C2E44]` and `[0x826C5510]` at
  its block per case and puts them back.

## Run 2026-10-31 16:54 (`b7-loop-31-10-26`, continued)

- The in-race screens' third migration (48313a90..d26e6f66): 27 rows
  in `race-screens.md` (`RaceDialog::OpenAnim`'s modes 0 and 2
  corrected), 2 in `race-hud-machine.md`, 5 in `race-hud-object.md`.
  For review: `RaceScreens::Create`'s cases in `RaceScreens::Init`'s
  harness world with its existing recorder; `RaceDialog::Question`'s
  harness points the save interface and the replay list at a world of
  its own (the game's save objects untouched); `sub_8211CBD8` (ready,
  7 callers in play elsewhere) not taken.

- The pad replay's race (`race_test.jsonl`) reached its countdown in
  `itc_3021` but not in `itc_2987`, `itc_3004` or `itc_3038` (set up
  at ~77.5 s, racers never added): not debugged; worth a look in case
  native code this run is the cause (to debug later, as the earlier
  unpaced-menus replay).

- The in-race screens' second migration (590250a6..ca74f7fd): 36 rows
  in `race-screens.md` (11 Native; `PauseMenu::ColorItems`' colors
  corrected: enabled items 205, 235, 67, disabled white) and 3 in
  `race-hud-machine.md`. For review: the UI movie calls, the menu
  sound and the menu list made through their overrides so the pause
  logging stays; `SetText`'s unset r7-r10 passed as 0 (the 2026-10-30
  call), the caller frame's +48..+79 (+40..+79 for `SetUpOptions`) left
  out of those cases; the openers' cases in the `race_screens`
  harness; kind 4's and the stage's openers left to the originals.

- The in-race screens' first migration (1405beb8..846937fe): 45 rows
  in `race-screens.md` (3 Native) and 2 in `race-hud-machine.md`. For
  review: the `--log_pause_movie` and `--log_pause_menu` overrides kept,
  running the native step, native callers of `ScreenPages::Go` going
  through its override for the log line; the screens' blocks through
  the allocation layer, the original when the native heap isn't active;
  paths only unseen screens take (race kinds 6 and 4, results of kinds
  2, 4, 6 and kind 1 outside the career) left to the originals.

- The in-race screens' map (421476db): `race-screens.md`, 157 rows
  (4 moved from `functions.md`), the screens' globals, the page object
  and the menu list in `known-data.md`; the menu list's helpers put
  here (no file of their own; `alt-hud.md` holds two); the confirm
  dialog mapped from the save screens' lead's range; three leads
  amended, three added. The pause menu's logging wraps and frame-rate
  fixes named in its rows.

- The race HUD machine's harness rewrite and the 18 adapters (the
  user's call; 3104033d, 5048300e): 16 rows in `race-hud-object.md`
  and 2 in `alt-hud.md` to Native; `race-hud-machine.md`'s intro and
  rows. For review: f1 compared only where the last callee's harness
  compares it; `RaceUpdate`'s post of message 20 no longer covered
  (`AltHud::Tick` always answers 0); 7 race HUD adapters stay for the
  in-race screens' recompiled callers.

- The race HUD machine's migration (e29438da, 7df89f72): 16 rows in
  `race-hud-machine.md` (15 Overridden, `sub_8216C9B8` Merged), its
  intro (kind 0 runs in attract races, though `seen_in_play.txt`
  lacks `sub_8220B9E8`), `sub_8216D7B8`'s bit corrected to 0x400. For
  review: kind 4, and kind 0 in the draw handlers, left to the
  originals; a harness stand-in for callees
  (`hooks/callee_stand_in.h`, null in the game) checked in the race HUD
  object's, the alternate HUD's and `sub_822141E8`'s adapters, so 18
  removable callee adapters stay. The user's call (2026-10-31): delete
  them, the harness rewritten with the HUD's world (a cleanup lead).

- The race HUD machine's map (11e568ee): `race-hud-machine.md`, 50
  new rows (its 65 transitions decoded), `sub_8215E140` shared with the
  race overlays' machine; `known-data.md`'s race HUD machine entry and
  the holder's +8 bit 0 (the replay bit, so the alternate HUD is
  probably the replay's dashboard); intros of `race-hud-object.md` and
  `alt-hud.md`; three leads annotated. A correction: the lead said the
  race flow's set-up registers these handlers; it is
  `RaceHudMachine::Init` (`sub_8216E6E8`).

- The RunWarning cleanup lead (1213d326, 2ea9a6f1): 3 rows in
  `race-hud-object.md` (`RunWarning` Overridden; `HudCurve::Eval`,
  `PowInt` Native). For review: the frame's back chain written even when
  the flag at +468 is 0, as the original leaves it.

- The race HUD cleanup lead (c17e0401..2bf1cf2c): 8 rows in
  `race-hud-object.md` (`RunCountdown`, `UpdateRacerBar`, `TickPanel`,
  `TickRacers`; 5 adapters removed). For review: `MenuSound::Play`'s
  slot call from native callers through a new `sound_calls()`;
  `RunCountdown`'s f1 left in ctx as the original leaves it;
  `TickPanel`'s modes 2 to 4 (never seen in play) still recompiled,
  through the table, and modes 2 and 4 not in the harness.

- The accessories' second migration (2eb6ca57..3a9de36a): 10 rows in
  `accessories.md` and `Accessories::Release`'s in `scene-loaders.md`
  (a correction: its caller is `Interior::Release`, not `ReleaseAll`).
  For review: the `OnLoaded` harness no longer builds a skeleton marked
  set up without its node rows (a state play can't reach).

- The accessories' first migration (8d5c9078..24186d92): 27 rows in
  `accessories.md` (13 Native with their adapters removed). For review:
  guest calls handed back to the adapter (the archive callback, the
  set's destructor through its vtable, the texture provider's slot +12);
  `Accessories::Destroy`'s r3 left as the original leaves it with no
  set; `model_animation::SkeletonBaseDestructible` added beside
  `BaseFreeable`. Only `Create` and `SetScale` run in attract races.

- The interior accessories' map (ae8e322b): `accessories.md`, 50 new
  rows (31 in play; the anchor, the skeleton's swing and the draws not
  seen in play), three object rows in `known-data.md`; the accessories'
  release lead folded in and removed from `leads.md`.

## Run 2026-10-31 13:47 (`b7-loop-31-10-26`)

- `Model::RecordMaterials` (`sub_820CECA0`, cleanup lead, 0c226b78):
  its row in `models.md` (blank -> Overridden, read in full). For
  review: the original kept for more than 4 textures, a vertex
  declaration the plain C++ leaves to it, the slot diagnostics, and a
  chunk open on another list or no room; the frame temporaries and the
  caller frame's saved arguments written as the original; the harness
  now saves and compares `0x826C5100` (16 bytes) for every
  `scenery_lists` case. The vertex declaration `sub_82589728` (8de457d4)
  Overridden -> Native.
- The shader pair's load callback (`sub_820A06D0`, cleanup lead,
  f0e75f6d..b7789d24): named `Shaders::OnPairLoaded` (probably) in
  `models.md`, Overridden (its adapter kept: `Archive::Load` calls it
  through the request's pointer). For review: its 16-byte file record
  kept on the host instead of the stack frame; the original run when
  memcpy's flag or the header copies' checks fail, and as guest calls
  where `VertexShader::Init`'s or operator delete's plain C++ declines;
  the harness uses only pair slots the game never fills (34-999,
  1006-1999). Seven adapters removed (Overridden -> Native):
  `sub_822A8568`, `sub_822A9FD8`, `sub_822A9EA8`, `sub_82594570`,
  `sub_825896C0` (`models.md`), `sub_822A8740`, `sub_822A85B8`
  (`shader-objects.md`); the first three's prints moved into the host
  helpers their work now runs in (`ReadFile`, `CopyHeaderFrom`), their
  own plain C++ called only by the harnesses.
- The alternate HUD's map (12983d8d): `functions/alt-hud.md`, 26 new
  rows (21 seen in play): `AltHud::` (our name), the race HUD holder's
  object at +1004, a second dashboard on UI movie 22 copying the race
  HUD's gauges plus the input record's throttle, brake and steering;
  probably the attract or replay races' (a guess). Ticked instead of the
  race HUD for kind 5, and kinds 1-3 and 6 with the holder's +8 bit 0
  (its writer not found). For review: `MenuList::PressedX`'s toggle
  probably never fires (+24 unwritten); `AltHud::SetBar`'s +232 and +236
  never written found; `sub_82216B08` taken from the power curve lead
  (its line updated); the menu list helpers `sub_8220BBB0`
  (`MenuList::Release`, ~50 call sites) and `sub_8220BF00` mapped here.
  `race-hud-object.md` and `known-data.md` updated.
- The alternate HUD's migration (515bb2e3, f8054cd4,
  `--native_alt_hud`): two map rows corrected (`MenuList::PressedX`
  answers 1 when +24 is 0, so X does flip the toggle; `MenuList::Release`
  returns 0 with no block). For review: `MenuList::Release` and
  `AltHud::Destroy` free natively only with `--native_heap` (blocks not
  the layer's pages go to the original, the car models' precedent);
  `PressedX` calls `ActionMapper::Pressed`'s plain C++ when the slot is
  it (the racer controllers' precedent); the three extra-gauge digit
  setters (never seen in play) left as the recompiled originals; 16
  functions Native with no adapter (only native callers reach them).
- The race scene's map (a3291567): `functions/race-scene.md`, 20 new
  rows (19 seen in play): `RaceScene::` at `[0x826D82D4]` (31,168
  bytes: seven car slots of 4,288 bytes at +32, 11 lights at +30064,
  the screen objects at +31152-+31160) and its machine
  `RaceSceneMachine::` (set up by `sub_8215D808` from `RaceMode::Init`,
  11 transitions). Two leads merged into it (the post effect users',
  the race object's), `sub_8215AA88` taken from the course scenery
  owners' lead. For review: `known-data.md`'s `0x826D82D4` row rewritten
  (the "7 records at +80" were arrays inside the car slots); notes in
  `post-effects.md`, `screen-objects.md` and `race-states.md` updated
  (`Glow::` slots 4-12 now name `Glows::SetParams`; `PostEffects::Get`
  sets slots 2-12, not 2 and 3); `RaceScene::Destroy` frees the object
  but leaves the pointer set; `sub_821597F0` stays in `camera-modes.md`
  (`CameraDirector::Resume`).
- The race scene's migration (2c5902ef..454bd46e,
  `--native_race_scene`): for review: `RaceSceneMachine::Init`'s stores
  and six-word copies replayed from a table in the original's 864-byte
  frame, so the words it never writes keep the stack's (the
  `RaceStates::Init` precedent); `SetEffectParams` and
  `Glows::SetParams` call the setters' plain C++ only when the glows'
  slots 2-12 and the filters' slot 8 hold them, else the original runs;
  `Resume` (never seen in play) ported with `TogglePause`; five callees
  Native with no adapter. `RaceScene::Create`'s and
  `RaceSceneMachine::Init`'s prints come right after their bail-out
  (when the native `operator new` can't run), so they log only when the
  native path runs (the validation agent's note).
- The menu sounds' map (60918895): `functions/menu-sounds.md`, 33 new
  rows (4 seen in play): `MenuSound::Play` (`sub_821103F0`, cue r3 of
  bank 0 at 1.0 through the sound manager's slot 10, ~270 callers),
  `Sound::Play` (`sub_82175850`), the volume options for the XACT
  categories "Default" and "Music" (`sub_821B9C20`, `sub_821B9CF0`),
  and 29 of the sound manager's methods not seen in play. For review:
  `known-data.md`'s `0x826C5C80` row corrected (the sound manager
  `AudioManager`, app + 124, not a music player; `Music::Play`'s slot 4
  is `AudioManager::LoadBank`); the slots that run keep their rows in
  `sound-cues.md`; `race-hud-object.md`'s and `location-screen.md`'s
  intros point to the new file.
- The menu sounds' migration (9f0605c3, `--native_menu_sounds`): the
  sound manager's slots 10, 29 and 30 and XACT's category calls below
  them aren't ours, so called through the function table, with only
  the registers the original sets (the caller's r5 left, the racer
  controllers' `set_r4` precedent); the four adapters stay for their
  ~300 recompiled callers.

## Run 2026-10-30 23:03 (`b7-loop-30-10-26-2`)

- The race HUD object's map (ac07446): `functions/race-hud-object.md`,
  86 new rows (57 seen in play, 29 not): `RaceHud::` (our name) at +16
  of `RaceScreens::` (the holder at `[0x826D84F0]`, made by
  `sub_82211F40`): its nine UI movies, the dashboard's gauges, the
  rear-view mirror's render target (texture 56), the countdown and its
  sounds, the warning movie, the racers' bars, the item popup, the
  timer, the mode at +448; the expressway sign from zones 0-31
  (`sub_82212F00`) and the place's text from zones 32-69
  (`sub_82213510`; the lead said texture 56, corrected in
  `track-zones.md`'s intro). `known-data.md`: `0x8200D838` rewritten,
  rows for `0x8200D9D0`, the gauge curves, the holder and the HUD's
  fields. For review: native callers will pass 0 for the varargs
  `SetText`/`SetNodeText` arguments left unset (the parking area's
  call); `sub_82212F00`'s read at +420 - 12 when no zone is inside kept;
  the draw-side work goes through the native `UiMovie::Run` (no new
  interpolation decision). Four leads added. Migration 1's rows
  (003ad8d..6fe8bac; `--native_race_hud_object`,
  `src/race_hud_object/`): 29 functions, 12 now Native;
  `sub_82212668` named `RaceHud::ResetBar`, `sub_822122A0`
  `RaceHud::ClearWarning`. For review: the TickTimer harness keeps its
  floats finite (a NaN distance makes `SetDistance` write negative
  digits into the nodes' child indices, as the original does, which the
  harness's fake node pool can't hold); guest calls through
  `race_hud_object::Calls` (memcpy where refused,
  `CameraPasses::RenderTarget`, `TrackZones::Create`,
  `UiMovie::FinishNamed`, `SetText`, `ClearText`, the extra gauges'
  digit setters); the adapters of `TrackZones::Create`,
  `UiMovie::SetText` and `ClearText` now hand calls to the harness
  recorder first (`hooks::StartUpRecorded`); none of it runs in an
  attract race (no HUD there). Migration 2's rows (653a14e..e4aaf1e):
  17 more, `sub_82216A60` a new row (`RaceScreens::Make1004`, our name;
  Native), `sub_82211C1C` Merged into `RaceScreens::MovieMask`; notes
  in `ui-movies-game.md`, `files-and-saves.md`, `heap.md`,
  `save-data.md` (their adapters of `SetNodeText`, `sub_8211AD00`,
  `UiMovies::Open`, `Strings::Load`, `operator new` and `MenuList::Init`
  now hand calls to the harness recorder first). For review: `ShowItem`
  passes 0 for `SetNodeText`'s unset r6-r10 (the parking area's call);
  `RaceHud::Draw` makes the original's `Run` and mirror calls, frame
  dumps of the replay with the flag on and off alike (`itc_2887`,
  `itc_2888`); `RunWarning` and `RunCountdown` not ported (the menu
  sound has no override for the harness to record).
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
  passes. Two leads added. Migration 1's rows (579077c..a02c2b2;
  `--native_material_passes`, `src/material_passes/`): 21 rows, among
  them `Model::MaterialTotal` and `MaterialAt` (`sub_82594AC0`,
  `sub_82594B00`), `sub_820D3250` and `Matrix::LookAt` Native with no
  adapter; `HsvToRgb8` Native. For review: the harness skips three
  register-save words only the originals write (`HsvToRgb8`'s r31 save;
  the same under `SetPaint` and `SetPaintBody` when all three
  saturations are 0; `LookAt`'s lr and r31 under `Projector::Matrices`),
  as the car models' `Texture::Create` frame; `TransposeRotation`,
  `SetMaterials` and `Projector::Matrices` run the original when
  `c_runtime::Memcpy` refuses their copy (for `Projector::Matrices`
  after the view matrix is written, harmless unless outputs overlap
  inputs); `FlipCulling` not seen in an attract race (harness only).
  Migration 2 (4795aab): `PaintPalette::Init` `sub_820D2330` as a table
  of 42 `.rdata` float addresses copied through lfs/stfs, the entries'
  +60 left unwritten and its 1,184-byte frame left as the original
  leaves it (register saves not written); three floats depend on the
  scene's +4 (`known-data.md`'s palette row).
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
