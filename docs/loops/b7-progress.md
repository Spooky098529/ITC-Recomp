# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-12 on `b7-loop-11-11-26` (the previous run's
  branch, kept by the user's call for this run), from 575b2b9a;
  summary `b7-loop-2026-11-12-0826.md`. Limit: 5 subsystems (the
  user's call for this run), raised to 10 by the user on 2026-11-13.
- First: the replay theater's check (the previous run's port
  49287852, unchecked: its harness, a race, `--native_type10_mode` on,
  the adapterless rows set to Native), a lead without mapping (it
  doesn't count toward the limit). Migration 50318046..93b074ab: a
  harness fault fixed first (by the precedent: `MoveCursor`'s case 86
  wrote guest 0x10 in a loop, a replay record's +28 given wild values
  where play gives 0-99), then 46 of 46 harnesses at 0 differing,
  `--native_type10_mode` on by default, 8 adapterless rows Native,
  `Replay::SetUpRace` left recompiled (never ran); no adapters
  removable (all reached through tables); race 38 s at 40, 0 errors;
  no subagents. Validated: `itc_4951` (race 57 s at 180, 0 errors,
  every harness 0 differing outside the expected groups), 46 of 46
  harness lines, no prints added; none ran in the race or the replay
  `itc_4952` (ok): 46 to drive. Recorded.
- Subsystem 1 of 5: the type 9 front-end mode (`sub_8225D6D0` its
  Init, its slots 6, 8, 10, 11, 13, +32 slot 0; ready groups under it;
  the previous run's "Next"; the top lead `sub_82562FF0` is inside the
  movie library's range, skipped as before); its line deleted from
  `leads.md`. Mapped `todays-result.md` (0752af3f: 57 rows, 31 in
  play: the career's end of the day after a career race, two machines,
  today's result (UI movie 27, block +0, `Career::NextDay`) and today's
  event (UI movie 0, block +9640, the unlocks); leaves for the boot
  mode when a movie is queued, else the garage; 26 ready groups; one
  lead added, the type 12 mode; no subagents); map checked (0 errors,
  file listed, 26 ready groups). Migration b7718efe..ba41ed1b (57 of
  57 ours: 38 Overridden, 19 Native, `Kind`'s adapter removed;
  `--native_todays_result`; never-seen paths ported; 57 harnesses 0
  differing; race 37 s at 40, 0 errors; harness faults fixed on the
  way; three of the map's addresses corrected; no subagents). Left:
  none. Validated: `itc_4967` (race 57 s at 180, 0 errors, every
  harness 0 differing outside the expected groups), 57 of 57 harness
  lines, no prints added; none in the race, 31 in the replay
  `itc_4968` (ok; today's result's path), 26 to drive (today's event).
  Recorded.
- Subsystem 2 of 5: the type 12 front-end mode (its Init
  `sub_82278058`, ready, 25 functions and 8,521 lines under it, with
  `sub_8225F330` and the handlers after the type 9 mode's code; the
  lead the type 9 map added; the title's main menu item that leaves
  15); its line deleted from `leads.md`. Mapped `network-mode.md`
  (89120eb9: 127 rows, 108 in play: the Xbox Live mode, "NETWORK
  BATTLE" and "NETWORK TIME ATTACK", its mode select, 14 InitStates, 8
  base Inits; 97 ready groups, `NextMode` and `Teardown` blocked by
  `sub_82292710` outside it; 5 leads added; no subagents); map checked
  (0 errors, file listed). Its `stop:`: an Xbox Live mode the player
  can open. The user's call (2026-11-12, asked): port its screens
  natively, its Live calls left recompiled (no behavior change);
  then, the same day, changed by the user to minimize the code left
  recompiled: the Live calls answered by plugging in the data the
  SDK's answers give today; in "Precedent first", sent to the running
  migration agent. Migration 1b7594a1..21c8cb3b (five batches, one
  subagent each, one at a time): 130 of 130 rows ours (95 Overridden,
  32 Native, 2 Merged, `CheckMultiplayerPrivilege` Dropped with its
  answer plugged in; 23 adapters removed; `XUserCheckPrivilege`
  Dropped in `system-services.md`; `Interrupt` `sub_822785E0` left,
  blocked by unread helpers); the Live mask `[0x826C795C]` read as the
  data B5's stub writes, `XShowSigninUI` left out (answer unread);
  `Session::Make`/`Free` turned out to be the game's own memory,
  ported; `--native_network_mode`; harnesses 0 differing outside two
  new expected groups; race 36 s at 40, 0 errors. Left: none (the
  other screens are leads). Validated: `itc_5001` (race 54 s at 180,
  0 errors, every harness 0 differing outside the expected groups;
  harnesses ~124 s), 127 of 127 harness lines, no prints added; none
  in the race or the replay `itc_5002` (ok): 127 to drive. Recorded.
- Subsystem 3 of 5: the race's end on the career's side
  (`sub_82150E00`, ready, seen at every career race's end; it blocks
  `RaceMode::NextMode`; the network mode's other screens never ran in
  play, so a lead run in play was taken first); its line deleted from
  `leads.md`. Mapped `race-end.md` (c8f1147b: 1 row,
  `RaceMode::ModeAfterRace`, read in full: the mode a race goes to
  after it ends, by race kind and flags; its callees and caller have
  rows elsewhere; ready with `RaceMode::NextMode` `sub_82151EF0`, 2
  functions, 377 lines; no leads; no subagents); map checked (0
  errors, file listed, 1 ready group). Migration ba317c7e..fc2138de
  (`RaceMode::NextMode` and `ModeAfterRace` native under
  `--native_race_end`; `Progress::CheckUnlocks` ported too, never seen,
  by the user's call to minimize what's left recompiled; 6 adapters
  removed: `ModeAfterRace`, `CheckUnlocks`, `AfterRace`,
  `EventLocation`, `Session::Free`, `CheckSystemRequest`; harnesses 0
  differing; race 33 s at 40, 0 errors; no subagents). Left: none;
  `race-loading.md`'s `WaitCourse` and `EndTick` still wait on
  `Scenery::IsLoaded` `sub_820F1F68` (ready itself). Validated:
  `itc_5006` (race 54 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~124 s), 7 of 7 harness
  lines, no prints added; 2 ran in the race, 2 more in the replay
  `itc_5007` (ok), 3 to drive. Recorded.
- Next: `Scenery::IsLoaded` `sub_820F1F68` (`course-scenery.md`,
  ready) with what it unblocks in `race-loading.md` (`WaitCourse`
  `sub_821514A0`, `EndTick` `sub_821516F0`), a lead without mapping
  (not counted). Migration ce238773..880882d4 (`IsLoaded` under
  `--native_course_scenery`, `WaitCourse` and `EndTick` under
  `--native_race_loading`; 12 `course-scenery.md` adapters removed;
  harnesses 0 differing; race 38 s at 40, 0 errors; no subagents).
  Left: `LocationScreen::PollLoad` `sub_82229238` newly ready
  (`location-screen.md`), and 9 groups already ready in
  `course-scenery.md` (`Scenery::Unload`, `CourseSet::Load`,
  `RecordHeadlightProjectors`, `Scenery::Draw` and smaller). Validated:
  `itc_5016` (race 49 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~124 s), 15 of 15 harness
  lines, no prints added; 14 ran in the race, `EndTick` in the replay
  `itc_5017` (ok), none to drive. Recorded.
- Next: the course scenery's ready groups (`Scenery::Unload`,
  `UnloadPools`, `Destroy`, `CourseSet::Load`, `OnLoaded`,
  `AreaLights::Load`, `AreaModel::Load`, `RecordHeadlightProjectors`,
  `Scenery::Draw`, all seen in play) with `LocationScreen::PollLoad`
  `sub_82229238`, newly ready; a lead without mapping (not counted).
  Migration 8516e7c6..95d287ef (five batches, the last by one
  subagent: `Unload`, `UnloadPools`, `Destroy`, `CourseSet::Load`
  with the area lights and models, `OnLoaded`, `PollLoad`,
  `AnimationSet::FindByTexture` (moved from `functions.md`),
  `RecordHeadlightProjectors` (frame dump: noise only); 15 adapters
  removed, in `course-scenery.md`, `camera-modes.md`,
  `location-screen.md`, `ui-movies-game.md`, `vehicle-accessors.md`;
  harnesses 0 differing; race 38 s at 40, 0 errors). Left: the rest
  of `Scenery::Draw`'s split (`DrawArea`, `Minimap::Draw`, the
  billboards, the light groups, `Scenery::Draw`), all ready.
  Validation failed: `itc_5046` (race 54 s at 180, 0 errors), 12
  `front_end_car_draw` and `tuning_screen` groups differing (all at 0
  before), all in dead stack bytes below the caller's frame: removing
  `ScenePools::GetB`'s adapter (3c822092) lets recompiled callers'
  calls run the recompiled `GetB`, which writes its frame there, while
  the native side writes nothing; and 24 of 25 harness lines,
  `UiMovie::GetPictureTexture` `sub_8211AB28` with no print (its work
  inlined in `PollLoad`'s plain C++, its harness passing). By the
  precedents (harness faults: `RaceMenu::Transition3`,
  `GarageScreen::Transition6`; no own print: `Texture::GetLevelDesc`),
  a fix agent fixes the harnesses and makes `GetPictureTexture`'s row
  Merged; then the validation again. 14 ran in the race, 5 in the
  replay `itc_5047` (ok), 6 not in game. The fix (1fdeb678): logging
  confirmed `GetB`'s back chain and saves at each caller's r1 in all
  12 groups (no native difference); the harnesses leave out exactly
  those words (`GetBSaves`); `GetPictureTexture` Merged into
  `PollLoad`. Validated again by the fix agent's run `itc_5052` (race
  54 s at 180, 0 errors, every harness 0 differing outside the
  expected groups, checked by the lead), 24 of 24 harness lines; 5 to
  drive. Recorded.
- Next: the rest of `Scenery::Draw`'s split (`DrawArea` `sub_820F3B70`,
  `Minimap::Draw` `sub_820F5740`, the billboards `sub_8209C1A0`,
  `sub_8209CC38`, `sub_8209CDB0`, the light groups `sub_82148658`,
  `Scenery::Draw` `sub_820F6068`), a lead without mapping (not
  counted). Migration f1e47061..ea8acc8f (four batches: the billboards'
  record and draw, `LightGroups::Draw`, `DrawArea`, `Minimap::Draw`
  inside its blink-clock wrap; 12 adapters removed (`Billboards::Draw`,
  `Model::DrawShapeDepth`, nine under the minimap); harnesses 0
  differing, the callers' files' too after each removal; frame dumps
  at 30 fps without interpolation pixel-identical against each
  fallback flag; race 35 s at 40, 0 errors; no subagents). Left:
  `Scenery::Draw` itself (ready). Validated: `itc_5101` (race 54 s at
  180, 0 errors, every harness 0 differing outside the expected
  groups; harnesses ~124 s), 16 of 16 harness lines, no prints added;
  13 ran in the race, 1 in the replay `itc_5102` (ok), 2 to drive.
  Recorded.
- Next: `Scenery::Draw` `sub_820F6068` (ready, 678 lines, its
  frame-state wrap kept), the group's last, no mapping (not counted).
  Migration 7bf375c1..bd6bfc88 (`Scenery::Draw` native inside its
  frame-state wrap, `TextureAnimations::Advance` and `Minimap::Draw`
  through their per-frame wraps; 11 adapters removed; harnesses 0
  differing, the callers' files' too; frame 600 at 30 fps without
  interpolation pixel-identical against the fallback; race 33 s at
  40, 0 errors; no subagents). Left: none ready in
  `course-scenery.md`. Validated: `itc_5112` (race 54 s at 180, 0
  errors, every harness 0 differing outside the expected groups;
  harnesses ~124 s), 12 of 12 harness lines, one print added
  (`Lights::FindNearest`, 8aa9f2af); all 12 ran in the race, none to
  drive. Recorded.
- Subsystem 4 of 10: the options screen's save side (pages 9 and 10,
  categories 5 and 6: `sub_821C6468`, `sub_821BBC10`, `sub_821BC7B0`,
  ..., all seen in play) with what's left of the save screens' lead
  (`821BBD18`-`821C2620`, `82217E20`-`8221E908` outside
  `race-screens.md`), the two leads taken together as the first says;
  both lines deleted from `leads.md`. Mapped `options-load-save.md`
  (fde73e96: 16 rows, 14 in play: category 5, page 9, is the load,
  category 6, page 10, the save; 6 ready groups; one lead added (the
  controller pages' callees), the in-race screens' lead extended with
  `8221BE90` on (47 never seen); no subagents); map checked (0 errors,
  file listed, 6 ready groups). Migration d2d04519..2111429c (16 of 16
  ours, `--native_options_load_save`, the two never-seen ported; the
  `SetDialog`s' adapters removed; harness cases seeded per case
  (`CaseRandom`) after shifted streams left accept paths unreached;
  harnesses 0 differing, `options_screen`'s too; race 35 s at 40, 0
  errors; no subagents). Left: none. Validated: `itc_5125` (race 53 s
  at 180, 0 errors, every harness 0 differing outside the expected
  groups; harnesses ~125 s), 16 of 16 harness lines, no prints added;
  none in the race, the save page's 7 in the replay `itc_5126` (ok), 9
  to drive. Recorded.
- Subsystem 5 of 10: `Type6Screen0`'s pages and steps (`sub_82234BF0`
  and 8 more, all seen in play, ready; called through its tables at
  `0x826367B8`); its line deleted from `leads.md`. Mapped into
  `type6-type7-modes.md` (4ed5e21e: 13 new rows, 4 pages, 4 steps, 3
  helpers, `PowerGraph::SetFromCar` and its split-off case; the menu
  page `sub_82234958` taken from the menu helpers' lead; 9 ready
  groups; one lead added (probably the type 5 mode's delegates); no
  subagents); map checked (0 errors, 9 ready groups). Migration
  d642e99d..3a90ef05 (the 13 rows ours, with type 7's 5 unported steps
  and pages, `PowerGraph::SetFromCar` (type 7's menu page and the car
  list now call it), `CarList::DrawCarPowerGraph`; 10 adapters
  removed; harnesses 0 differing; race 38 s at 40, 0 errors; no
  subagents). Left: none ready (42 never-seen rows). Validated:
  `itc_5131` (race 53 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~125 s), 24 of 24 harness
  lines, no prints added; none in the race or the replay `itc_5132`
  (ok): 24 to drive. Recorded.
- Subsystem 6 of 10: probably the type 5 front-end mode: its delegates
  (`sub_82230050`, `sub_82230328`, `sub_82230458` with `sub_8222FD08`,
  ready, seen in play) with the mode set-up beside them
  (`sub_8222FD68`, `sub_82226A10`, `sub_821B7878`, `sub_8222FC50`), two
  leads taken together; both lines deleted from `leads.md`. Mapped
  `type5-mode.md` (4a90a16a: 33 rows, 17 in play: the type 5 mode, the
  parking area's (vtable `0x82006C24`, 15 slots), its three machines,
  the car settings at +0, the parking area at +9640, and a new dialog
  at +19280 (`Type5Dialog`, "TXR_PaCmnWindow", the type 2 dialog's
  design); 15 ready groups (the pages and steps before `Draw` and
  `Update`, reached through tables); no leads; no subagents); map
  checked (0 errors, 15 ready groups). Migration 33bdeaf2..d30920e0
  (one batch: 33 of 33 ours, `--native_type5_mode`, the 16 never seen
  ported with their callers; `FindShown` and `Type5Dialog::Init`'s
  adapters removed; the quirks kept; a harness fault fixed before the
  commit (`LoadingScreen::Finish` read past the three movies with the
  world's "none" index); 33 harnesses 0 differing; race 38 s at 40, 0
  errors; no subagents). Left: none. Validated: `itc_5139` (race 52 s
  at 180, 0 errors, every harness 0 differing outside the expected
  groups; harnesses ~126 s), 33 of 33 harness lines, no prints added;
  none in the race or the replay `itc_5140` (ok): 33 to drive.
  Recorded.
- Subsystem 7 of 10: the front-end screens that show a car on its
  stand (`sub_821807D8`, `sub_82184B80`, ... `sub_8224CB28`, seen in
  play; ready roots `sub_82201B40`, `sub_82202928`); its line deleted
  from `leads.md`. Mapped (c2578bab): the lead was mostly stale (6
  of its 8 already had rows); 2 new rows in `tuning-pages.md`, the
  vinyl editor's state 34 page `TuningScreen::InputVinylView`
  `sub_82202928` and `LeaveVinylView` `sub_82201B40` (ready). So the
  vinyl editor's input pages lead (states 23 to 31, `sub_82204B70` and
  more, the first five seen in play, mostly ready) is taken into the
  same subsystem, mapped next, and both are migrated together; its
  line deleted from `leads.md`. Mapped (b54839a0): 27 new rows in
  `tuning-pages.md` (14 in play: the vinyl editor's pages for states
  23 to 31, their movie-side updates, the decal images' list window and
  archive callback); 7 ready groups, the rest ready bottom up inside
  the file (`InputVinylImages`' group over 1,500 lines: split); no
  leads; no subagents); map checked (0 errors). Migration
  e989acb8..5b6f12d3 (five batches: the decal images' loads, states
  34, 23, 29 to 31 (two never seen, ported) and 24's pages with their
  movie sides; 18 rows ours, 7 Native without adapters; harnesses 0
  differing, `car_makers`' too (its tuning stand world given a mapper
  block and a recording sound manager); race 35 s at 40, 0 errors; no
  subagents). Left: `InputVinylLayers` and `InputVinylImages` (ready;
  the second split), the never-seen layer actions and order pages.
  Validated: `itc_5150` (race 53 s at 180, 0 errors, every harness 0
  differing outside the expected groups; harnesses ~126 s), 18 of 18
  harness lines, no prints added; none in the race or the replay
  `itc_5151` (ok): 18 to drive. Recorded. Second migration
  07d8cdb7..a0dc392a (two subagents, one at a time: states 24 and 28
  with their movie sides, `InputLayerActions` (never seen) with its
  helpers, `HighlightItem` and `HighlightSelected` (`tuning-screen.md`);
  7 decal image adapters removed; two harness faults fixed before the
  commits; harnesses 0 differing; race 30 s at 40, 0 errors). Left:
  `InputLayerOrder` and the sticker helpers, never seen. Validated:
  `itc_5166` (race 52 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~126 s), 16 of 16 harness
  lines, no prints added; `RestoreStickerTexture` ran in the replay
  `itc_5167` (ok), the other 15 to drive (11 new to the list, 4
  already there). Recorded. Third migration 82a29244 (state 27's
  `InputLayerOrder` with its four callees, the sticker helpers
  `SwapLayerRecords`, `SetGroupImage`, `SetImageGroup`, all never seen;
  a harness hang fixed before the commit (the state history laid with
  a 24); harnesses 0 differing; race 30 s at 40, 0 errors; no
  subagents). Left: nothing in the vinyl editor; `HandleInput`
  `sub_821E6950` and `Command21` `sub_822087C8` now ready. Validated:
  `itc_5173` (race 52 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~126 s), 8 of 8 harness
  lines, no prints added; none in the race or the replay `itc_5174`
  (ok): 8 to drive. Recorded; subsystem 7 done.
- Next: `TuningScreen::HandleInput` `sub_821E6950` (ready, seen in
  play, 424 lines; 23 never-seen callees) and `Command21`
  `sub_822087C8` (ready), a lead without mapping (not counted).
  Migration 2787b4aa..658969d6 (four batches: `HandleInput` (Input0
  Merged) and `Command21`, 29 adapters removed (the pages HandleInput
  alone called, and four helpers), the sticker set-up under
  `InputItems` (never seen) ported; two harness faults fixed (a
  `Command21` case with wild layer positions; a signaling NaN the
  recompiled `lfs`/`stfs` quiet, by the `kVinyls` precedent);
  harnesses 0 differing; race 29 s at 40, 0 errors; no subagents).
  Left: 20 never-seen pages `HandleInput` calls through the guest
  (state 7's option rows next, the paint, the light kits, the
  interior; the sticker editor's own lead). Validated: `itc_5181`
  (race 51 s at 180, 0 errors, every harness 0 differing outside the
  expected groups; harnesses ~127 s), 34 of 34 harness lines, no
  prints added; 9 ran in the replay `itc_5182` (ok), 25 not in game
  (the new ones added to the drive list). Recorded.
- Subsystem 8 of 10: a front-end screen that lists cars (`sub_8224BDC0`
  and `sub_82250778`, `sub_82250890`, `sub_82250AA8`, `sub_82251320`,
  seen in play, ready); its line deleted from `leads.md`. Mapped
  `power-graph-screen.md` (4ed4704c: 22 rows, 16 in play: the type 6
  and 7 modes' power graph screen machine, its handlers, pages, steps,
  texts and the members' slots (inferred, "probably"); 15 ready groups;
  pages, steps and slots before their table callers; no leads; no
  subagents); map checked (0 errors, 15 ready groups). Migration
  acc7bc1a..2f90c6eb (three batches: 22 of 22 ours,
  `--native_power_graph_screen`, the never-seen helpers ported; no
  adapter removable (all reached through tables and vtables); harness
  fixes on the way (movie 9 given 21 text records after a mutation
  test went unseen; sub-scene 3 kept a leaf); harnesses 0 differing;
  race 38 s at 40, 0 errors; no subagents). Left: none. Validated:
  `itc_5190` (race 48 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~130 s), 22 of 22 harness
  lines, no prints added; none in the race or the replay `itc_5191`
  (ok): 22 to drive. Recorded.
- Subsystem 9 of 10: after a race, which achievements were earned
  (`sub_82290788` with `sub_82290698`, seen in play, ready; its writer
  `sub_82290A30`); its two lead lines (the same functions) deleted
  from `leads.md`. An Xbox feature: its Xbox calls get plugged-in data
  by the user's call of 2026-11-12. Mapped `achievements.md`
  (d06dc44a: 5 rows, 2 in play: `Achievements::Check`, `AllBeaten`,
  `BeatsCourseTime`, `Time::Less`, `Write`; 1 ready group; no leads; no
  subagents); map checked (0 errors). Its note: skipping
  `XUserWriteAchievements` would stop the SDK's achievements file. The
  user's call (2026-11-13, asked): keep the SDK call (through the
  guest); added to "Precedent first". Migration 7f7fce7e..848036fc (5
  of 5 ours, `--native_achievements`; `AllBeaten` and
  `BeatsCourseTime`'s adapters removed; `Write` calls
  `XUserWriteAchievements` through the guest; harnesses 0 differing;
  race 38 s at 40, 0 errors; no subagents). Left: none;
  `RaceStates::OnRaceEnd` `sub_821644D8` now ready (a cleanup lead).
  Validated: `itc_5198` (race 50 s at 180, 0 errors, every harness 0
  differing outside the expected groups; harnesses ~128 s), 5 of 5
  harness lines, no prints added; `Check` and `AllBeaten` ran in the
  replay `itc_5199` (ok), 3 to drive. Recorded.
- Eight stale leads deleted from `leads.md` (every function in them
  mapped since: the `XMemCpy` callers, the network mode's handler, the
  career screens, the race event's destroyer, the garage car resets,
  the stand's car makers' callers, the menu helpers' screens, the
  `0x826364EC` readers).
- Subsystem 10 of 10: the framework's scene constructor (`sub_82098A98`
  and its callees, seen in play) with the renderer's constructor
  (`sub_82092358`, `sub_820922B8`), two leads taken together; both
  lines deleted from `leads.md`. Mapped `renderer.md`, renamed
  `frame-driver.md` by the user's call (2026-11-13: "renderer" was
  confusing beside our Vulkan renderer) (6d9bc131: 41
  rows, 9 moved from `functions.md`: the renderer at framework + 76
  and the app's scene at + 80, their constructors, `Thread::Thread` and
  `Begin` with the kernel thread imports, the renderer's frame and
  listener calls, `Scene::Update` and `Draw` (Wrapped); 11 ready
  groups; `FrameDriver::Init`, `WaitForFrame` and `StartFrame` blocked by
  the core's `Event::` functions (recompiled by the user's call,
  2026-10-24); no leads; no subagents); map checked (0 errors).
  Migration cac0f347..e721e59a (six batches with the rename 2a098b95:
  `FrameDriver::` interface, requests, frame; `Scene::Update`, `Draw`
  and its constructor; `Thread::Thread`, `Begin` with the thread
  imports; `VideoOutput::UnbindAll`; `--native_frame_driver`; frame 600
  pixel-identical against the fallback and a second run; harnesses 0
  differing, `app_states`' and `framework`'s too; race 38 s at 40, 0
  errors; no subagents). Found and fixed: the `set_register` helper in
  `hooks/app_states.cpp` and `framework.cpp` put register n into
  r(n+1) (c96869cc). Left: `FrameDriver::Present` (its harness would
  run `Device::Swap`'s override), `Thread::Start`. Validated:
  `itc_5227` (race 50 s at 180, 0 errors, every harness 0 differing
  outside the expected groups; harnesses ~128 s), 22 of 22 harness
  lines, no prints added; all 22 ran in the race, none to drive.
  Recorded; subsystem 10 done: the limit.

## Done

- The frame driver and the app's scene: `frame-driver.md`,
  `--native_frame_driver`, cac0f347..e721e59a; validated.

- The achievements: `achievements.md` 5 of 5 ours,
  `--native_achievements`, 7f7fce7e..848036fc; validated.

- The power graph screen: `power-graph-screen.md` 22 of 22 ours,
  `--native_power_graph_screen`, acc7bc1a..2f90c6eb; validated.

- `HandleInput` and `Command21` (no mapping): 29 adapters removed, the
  sticker set-up ported, 2787b4aa..658969d6; validated.

- The vinyl editor (with the stand screens' lead's state 34): 29 new
  rows in `tuning-pages.md` and 2 in `tuning-screen.md` ours, with the
  sticker helpers, under `--native_tuning_pages`, e989acb8..82a29244;
  validated.

- The type 5 mode (the parking area's, with its dialog):
  `type5-mode.md` 33 of 33 ours, `--native_type5_mode`,
  33bdeaf2..d30920e0; validated.

- `Type6Screen0`: 13 rows in `type6-type7-modes.md` ours, with type
  7's steps and the power graph's set-up, d642e99d..3a90ef05; validated.

- The options' load and save pages: `options-load-save.md` 16 of 16
  ours, `--native_options_load_save`, d2d04519..2111429c; validated.

- `Scenery::Draw` (no mapping): 11 adapters removed,
  7bf375c1..bd6bfc88; validated. The course scenery has nothing ready
  left.

- `Scenery::Draw`'s rest, part one (no mapping): the billboards, light
  groups, `DrawArea`, `Minimap::Draw`, 12 adapters removed,
  f1e47061..ea8acc8f; validated.

- The course scenery's ready groups (no mapping): 10 made native, 15
  adapters removed, 8516e7c6..95d287ef, harness fix 1fdeb678;
  validated.

- `Scenery::IsLoaded` (no mapping): with `WaitCourse` and `EndTick`, 12
  adapters removed, ce238773..880882d4; validated.

- The race's end: `race-end.md` 1 of 1 ours, with `RaceMode::NextMode`
  and `Progress::CheckUnlocks`, `--native_race_end`,
  ba317c7e..fc2138de; validated.

- The type 12 mode (the Xbox Live network mode): `network-mode.md` 130
  of 130 ours, `--native_network_mode`, 1b7594a1..21c8cb3b; Live calls
  answered with plugged-in data; validated.

- The type 9 mode (today's result and today's event):
  `todays-result.md` 57 of 57 ours, `--native_todays_result`,
  b7718efe..ba41ed1b; validated.

- The replay theater's check: `type10-mode.md` 46 of 47 ours,
  `--native_type10_mode` on, 50318046..93b074ab; validated.

## Rows to review

The run's section of `review.md` ("Run 2026-11-12 08:26").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5612, work left 984 (wrapped 55, known 392,
  unknown 537).
- After the theater's check: ours 5620, work left 980 (wrapped 55,
  known 388, unknown 537).

- After the type 9 mode's map: ours 5620, work left 980 (wrapped 55,
  known 419, unknown 506).

- After its migration: ours 5677, work left 949 (wrapped 55, known
  388, unknown 506).

- After the type 12 mode's map: ours 5677, work left 949 (wrapped 55,
  known 496, unknown 398).

- After its migration: ours 5808, work left 840 (wrapped 55, known
  388, unknown 397).

- After the race end's map: ours 5808, work left 840 (wrapped 55,
  known 389, unknown 396).

- After the race end's migration: ours 5811, work left 838 (wrapped
  55, known 387, unknown 396).

- After `Scenery::IsLoaded`: ours 5814, work left 835 (wrapped 55,
  known 384, unknown 396).

- After the course scenery's ready groups: ours 5824, work left 825
  (wrapped 55, known 374, unknown 396).

- After `Scenery::Draw`'s rest, part one: ours 5830, work left 819
  (wrapped 53, known 370, unknown 396).

- After the options' load and save map: ours 5831, work left 818
  (wrapped 52, known 386, unknown 380).

- After the options' load and save: ours 5847, work left 804 (wrapped
  52, known 370, unknown 382).

- After `Type6Screen0`: ours 5866, work left 792 (wrapped 52, known
  367, unknown 373).

- After the type 5 mode's map: ours 5866, work left 792 (wrapped 52,
  known 384, unknown 356).

- After the type 5 mode: ours 5899, work left 775 (wrapped 52, known
  367, unknown 356).

- After the vinyl editor's map: ours 5899, work left 775 (wrapped 52,
  known 381, unknown 342).

- After the vinyl editor: ours 5935, work left 758 (wrapped 52, known
  366, unknown 340).

- After `HandleInput`: ours 5941, work left 757 (wrapped 52, known
  365, unknown 340).

- After the power graph screen's map: ours 5941, work left 757
  (wrapped 52, known 381, unknown 324).

- After the power graph screen: ours 5963, work left 741 (wrapped 52,
  known 365, unknown 324).

- After the achievements' map: ours 5963, work left 741 (wrapped 52,
  known 367, unknown 322).

- After the achievements: ours 5968, work left 739 (wrapped 52, known
  365, unknown 322).

- After the renderer's map: ours 5968, work left 739 (wrapped 52,
  known 381, unknown 306).

- After the frame driver: ours 5989, work left 718 (wrapped 51, known
  361, unknown 306).

## Stopped

2026-11-13, at the run's limit of 10 subsystems (raised from 5 by the
user), after the frame driver's validation. All ten validated, with
the replay theater's check, the course scenery and `HandleInput` as
leads without mapping. Nothing uncommitted. Totals: ours 5612 -> 5989,
work left 984 -> 718. The summary, `review.md`, `to-drive.md`, the
plan's status line and CLAUDE.md are up to date.
