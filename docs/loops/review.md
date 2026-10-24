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
