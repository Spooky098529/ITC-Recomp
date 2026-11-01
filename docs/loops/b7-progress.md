# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-31 23:57 on `b7-loop-31-10-26-2`, from
  2eb33a3f; summary `b7-loop-2026-10-31-2357.md`.
- Subsystem 1 of 5: the device's shader setters and the render queue
  (`device-shaders.md`). Done (map 112775ae, migration
  657efc7b..e727bc39, validated `itc_3054`: 120 s, race 55 s, 0 errors,
  7 of 7 harness lines, all 7 ran in the race, replay not needed).
- Cleanup lead (no mapping, not counted): `RaceHudMachine::CreateScreens`
  `sub_8216CA28`, `ClosePause` `sub_8216C658`, `ResultsDraw`
  `sub_8216D6D0`, `AfterRaceDraw` `sub_8216C6D8` with `sub_82211D10`,
  and `sub_82211D08` (`race-hud-machine.md`, `race-hud-object.md`),
  all ready; then `--removable` on the three files. Migration checked
  (61f7966b: the six; 8c5e2c76..7476d237: 6 adapters removed in
  `race-screens.md` and `race-hud-object.md`; the three files'
  harnesses 0 differing, race 25 s with them, 0 errors; unseen callees
  left on the recompiled path behind the flags; no subagents). Left:
  `PauseDraw` `sub_8216CF08` and `sub_8216CFE0`, ready now (a new
  cleanup lead). Validated (`itc_3072`, 120 s: race 54 s, 0 errors;
  replay `itc_3073` ok): 12 of 12 harness lines, 2 ran in the race, 5
  in the replay, 5 to drive. Done.
- Cleanup lead (no mapping, not counted): `RaceHudMachine::PauseDraw`
  `sub_8216CF08` and `sub_8216CFE0` (`race-hud-machine.md`), then
  `sub_82211D08`'s adapter. Migration checked (acac516d: both, kinds 0
  and 4 handing back to the original; 0a96f03b..0c9db58c: the
  adapters of `DrawPauseMenu`, `RaceHud::DrawWarning`, `RaceHud::Draw`
  removed, a counter at 0 in an attract race and the replay; three
  harnesses 0 differing, races 24-25 s with them, 0 errors; no
  subagents). Validated (`itc_3080`, 120 s: race 52 s, 0 errors;
  replay `itc_3081` ok): 5 of 5 harness lines, all 5 ran in the
  replay, none to drive. Done.
- Cleanup lead (no mapping, not counted): the jump-table pieces
  `sub_8220DA38` and `sub_8221A56C` (`race-screens.md`). Nothing
  migrated (`itc_3082`, race_screens 0 differing, 0 errors): each is
  its recompiled MainPage's case 0, popping MainPage's frame, so it
  goes native or Merged with `PauseMenu::MainPage` and
  `AfterRaceMenu::MainPage`, which wait on `sub_8211CBD8`,
  `RaceDialog::SetText` and `RaceDialog::Open`; no adapter removable.
  No validation needed.
- Limit for this run: 5 subsystems (the user's call, 2026-11-01; 3
  before); 3 taken.
- Subsystem 2 of 5: the render queue's methods (`sub_8258EDC8`,
  `sub_8258E5F8`, `sub_8258E618`, `sub_8258E5E0`, `sub_8258ED78`),
  from `leads.md` (all ready, every pass). Map checked (ee891a3a: 24
  rows added to `device-shaders.md`, the queue (the game's `OT`, an
  ordering table) with its vtable `0x8206CB8C`'s slots, `PassList::Add`,
  the replay `sub_82591238` with its sort and chunk walker; 2 rows
  moved from `functions.md`; 9 ready groups; the indirect calls reach
  `PrimitiveDrawer::Init` and the render command tables; 1 lead added;
  no subagents). Migration checked (77d89868..f1c2a45a, 6e542f59: the 9
  groups, 19 rows, `PassList::Clear`'s adapter removed; 25 of 25 in
  play ours; `--native_render_queue`; the `--find_menu_selection`
  wrappers moved into the new adapters, the renderer's command hooks
  untouched; frame dump pixel-identical against the flag off; harness
  render_queue and device_shaders 0 differing, race 38 s, 0 errors; no
  subagents). Nothing ready left. Validated (`itc_3092`, 120 s: race
  54 s, 0 errors; replay `itc_3093` ok): 19 of 19 harness lines, 17
  ran in the race, 2 to drive. Done.

- Cleanup lead (no mapping, not counted): `XPR::GetTexture`
  `sub_82597118` (Wrapped by `frame_state_diff`), then the parking
  area's `OnPictureLoadedA` and `B` and what else it unblocks.
  Migration checked (b409aa64..b82b56b5: GetTexture under
  `--native_models` inside the diagnostics' wrap; the two picture
  callbacks; 15 in `scene-loaders.md`; 8 adapters removed; every
  harness 0 differing, races 30-38 s, 0 errors; no subagents). Left
  ready in `scene-loaders.md`: `Cockpit::BindTextures`,
  `TrafficCar::BindTextures`, `ModelPoint::Find`,
  `Cockpit::OnFileLoaded`, `TrafficCar::OnFileLoaded` (a second
  migration after the validation). Validated (`itc_3105`, 120 s: race
  54 s, 0 errors; replay `itc_3106` ok; `OnPictureLoadedB`'s print,
  shared with A's in one call site, split, 06f85ec9): 21 of 21 harness
  lines, 12 ran in the race, 7 in the replay, 2 to drive. Second
  migration checked (cc03a279..1ff4d670: the five, all ran in the
  game; `scene-loaders.md` 78 of 78 ours; `XPR::FindByName`'s adapter
  kept for `CarModel::OnFileLoaded`'s hand-back that runs in play;
  harness scene_loaders 0 differing, frame dumps alike, race 38 s, 0
  errors; no subagents). Nothing ready left. Validated (`itc_3127`,
  120 s: race 54 s, 0 errors): 5 of 5 harness lines, all 5 ran in the
  race, replay not needed. Done.

- Subsystem 3 of 5: the scene's update (`sub_820C9608`, `sub_820C3DA0`,
  `sub_820C59B0`, `sub_820C4C80`, `sub_820F1078`), from `leads.md` (14
  ready groups under it since the scene loaders are ours; the scene at
  `[0x826C2E44]` lead beside it, `sub_820C8A50`, given to the mapping
  agent to fold in if it fits). Map checked (e8dcd16c: `scene.md`,
  23 rows, 22 in play: `CarScene::` at `[0x826C2E44]`, its making,
  loading, update and teardown; three leads folded in and deleted, 3
  added; 11 ready groups; blocked by the scene models' updates
  `sub_820C5A60` and the parts' finishers (Wrapped in `functions.md`);
  no subagents). Migration checked (9a461ba8..613dd3db: the 11 groups,
  19 of 22 ours, 9 adapters removed, `--native_scene`; harness scene 19
  groups 0 differing; frame dumps alike; race 38 s, 0 errors; the
  teardown path not seen in an attract race; no subagents). Left
  blocked: `CarScene::Update`, `ScenePools::DestroyB`,
  `CarScene::LoadsDone`. Step: validation agent running.

## Done

- `XPR::GetTexture` and the rest of the scene loaders (cleanup lead):
  `functions.md`, `location-screen.md` (the picture callbacks),
  `scene-loaders.md` 78 of 78; 8 adapters removed; b409aa64..1ff4d670.
  Validated (`itc_3105`, `itc_3127`).

- The render queue's methods (subsystem 2): `device-shaders.md`, 25 of
  25 in play with subsystem 1; `--native_render_queue`;
  ee891a3a..f1c2a45a. Validated (`itc_3092`).

- The race HUD machine's `PauseDraw` and `RacePauseDraw` (cleanup
  lead): with 3 adapters removed; acac516d..0c9db58c. Validated
  (`itc_3080`).

- The race HUD machine's `CreateScreens`, `ClosePause`, `ResultsDraw`,
  `AfterRaceDraw` (cleanup lead): with `sub_82211D08`, `sub_82211D10`
  and 6 adapters removed; 61f7966b..7476d237. Validated (`itc_3072`).

- The device's shader setters and the render queue (subsystem 1):
  `device-shaders.md`, 7 of 7 in play, one migration;
  `--native_device_shaders`; 112775ae..e727bc39. Validated
  (`itc_3054`).

## Rows to review

`review.md`, "Run 2026-10-31 23:57".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3705, work left 2694 (wrapped 101, known 513,
  unknown 2080). `b7_status.py`'s one check (`movies.md:117`
  `sub_82108018`: its row says never called, but it ran) was there
  before the run.
- After the device shaders' map: ours 3705, work left 2694 (wrapped
  101, known 519, unknown 2074).
- After its migration (`itc_3053`: race 38 s, 0 errors, device_shaders
  0 differing): ours 3712, work left 2687 (wrapped 100, known 513,
  unknown 2074).
- After its validation (`itc_3054`, 120 s: race 55 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3712, work left
  2687.
- After the race HUD machine cleanup's migration (`itc_3071`: race 25 s,
  0 errors, three harnesses 0 differing): ours 3718, work left 2681
  (wrapped 100, known 507, unknown 2074).
- After its validation (`itc_3072`, 120 s: race 54 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3718, work left
  2681.
- After the PauseDraw lead's migration (`itc_3079`: race 25 s, 0
  errors, three harnesses 0 differing): ours 3720, work left 2679
  (wrapped 100, known 505, unknown 2074).
- After its validation (`itc_3080`, 120 s: race 52 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3720, work left
  2679.

- After the render queue's map: ours 3720, work left 2679 (wrapped
  100, known 521, unknown 2058).

- After its migration (`itc_3091`: race 38 s, 0 errors, render_queue
  0 differing): ours 3738, work left 2661 (wrapped 98, known 505,
  unknown 2058).

- After its validation (`itc_3092`, 120 s: race 54 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3738, work left
  2661.

- After the GetTexture lead's migration (`itc_3103`: race 38 s, 0
  errors, four files' harnesses 0 differing): ours 3753, work left 2646
  (wrapped 97, known 491, unknown 2058).

- After the scene loaders' second migration (`itc_3124`: race 38 s, 0
  errors, scene_loaders 0 differing): ours 3758, work left 2641
  (wrapped 97, known 486, unknown 2058).

- After its validation (`itc_3127`, 120 s: race 54 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3758, work left
  2641.

- After the car scene's map: ours 3758, work left 2641 (wrapped 97,
  known 508, unknown 2036).

- After the car scene's migration (`itc_3139`: race 38 s, 0 errors,
  scene 0 differing): ours 3777, work left 2622 (wrapped 97, known 489,
  unknown 2036).

## Stopped
