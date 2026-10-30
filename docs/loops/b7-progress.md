# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 11:39 on `b7-loop-30-10-26`, from e320bad;
  summary `b7-loop-2026-10-30-1139.md`.
- Limit for this run: 3 subsystems; 3 taken.
- Subsystem 1 of 3: the race event (`sub_82140340`...`sub_82144E58`,
  the top of `leads.md`). Mapped (84e5334, `race-event.md`, 102 rows,
  37 in play, 12 ready groups, no stop; nothing outside blocks it).
  Migration 1 checked (6cf1882: every ready group, 93 Native and 6
  Overridden of 102, `EventCondition::Met` with 60 of its 61 tests,
  kind 40 `sub_82141668` left recompiled through a guest call at the
  original's depth; with the career's `EventQuery`, `EventLocation`,
  `DayBlock::BestStreak`; `--native_race_event`; nothing ready left,
  `Career::SetUpEvent` now ready in `career.md`). Validated
  (`itc_2673`, replay `itc_2676` ok): 103 of 103 harness lines; 26 ran
  in the replay, 77 to drive. Subsystem 1 finished.
- Next, a cleanup lead without mapping (not counted): what the race
  event unblocked in `career.md` (`Career::SetUpEvent` `sub_82114268`).
  Migration checked (3f8fa4b..8267f2d: `Career::SetUpEvent` over its
  121,200-byte frame in guest memory, `Garage::Add` and
  `Car::SetModelAndClass` `sub_821104D8` (ready, from the car objects'
  makers' lead); 5 adapters removed; no new flag; nothing ready left in
  `career.md`). Validated (`itc_2692`, replay `itc_2693`, broke on the
  intermittent SDK heap error, ran to the end): 8 of 8 harness lines; 4
  ran in the replay, 4 to drive.
- Subsystem 2 of 3: the game's camera modes (`sub_82152338`...
  `sub_822282E8`; picked over the front-end menus, as ready (44 ready
  groups each) and run every frame in a race). Mapped (9ac322c,
  `camera-modes.md`, 100 rows, 90 in play, 38 ready groups, no stop;
  blocked outside by `UiMovies::Open`, `Delete` and the scenery's
  update and draw). Migration 1 checked (5a1babc..4c274ed, 5 batches,
  4 by subagents one at a time: the director's view handlers, the
  camera animations and paths, the rival logo's pieces, the chase,
  intro and in-car views' cameras, the TV camera's curves; 10 adapters
  removed; `--native_camera_modes`; frame dumps the same; 74 of 90 in
  play ours). Validation 1 (`itc_2730`, replay `itc_2731` ok): cut off,
  all harnesses now take over 40 s, 11 functions without a harness
  line; asked the user: the validation runs `attract_race.sh 60` from
  now on (the user's call, 2026-10-30, in `b7-loop.md`). Validated
  again (`itc_2732`, 60 s: race 14 s, 0 errors): 74 of 74 harness
  lines; 38 ran in play, 12 in the replay, 24 to drive. Migration 2
  checked (96a281f..27ada48, 5 batches, 3 by subagents one at a time:
  `UiMovies::Open` and `Delete` under `--native_ui_movies_game`, the
  rival logo's open and close, the cutscene's step and update (the
  logo's draw still through `--redraw_rival_logo`'s override), the TV
  camera, `GarageCamera::Update`; 9 adapters removed; 82 of 90 in play
  ours). Validated (`itc_2750` at 60 s, replay `itc_2751` ok): 17 of 17
  harness lines; 10 ran in play, 1 in the replay, 4 more to drive.
  Migration 3 checked (81d51e9..d71eff5: `CameraDirector::InitStates`,
  `Cutscene::Cutscene` split over three subagents one at a time,
  `SetMode`, `OnRaceStart`, `Resume`, `Update` (run on logic passes by
  `--redraw_rival_logo`'s wrapper, its split kept); 11 adapters
  removed; 87 of 90 in play ours; nothing ready left, `CourseView::Draw`
  waits on the scenery). Validated (`itc_2766` at 60 s, replay
  `itc_2767` ok): 17 of 17 harness lines; 2 more to drive. Subsystem 2
  finished.
- Subsystem 3 of 3: the front-end menus (`sub_822271F8`...
  `sub_8222C300`, the readiness ranking's other top lead, 44 ready
  groups). Mapped (3bf92d8, `location-screen.md`: the lead was one
  screen, the parking area (the user, 2026-10-30: the player loads
  into it, picks one of up to 6 opponent cars and races; the other way
  to start a race besides driving on the highway); 99 rows, 77 in
  play, 32 ready groups, no stop; blocked outside by the UI movies'
  text setters, the menu sound and input, the scene and its models,
  the scenery's set-up and teardown, `XPR::GetTexture`). Migration 1
  running. Left: `CameraDirector::InitStates`,
  `TvCamera::TvCamera`, `Update` and `Step`, `GarageCamera::Update`;
  then, past `UiMovies::Open` (ready), the cutscene and the director's
  `SetMode`, `Update` (`--redraw_rival_logo`'s split kept).

## Done

- The camera modes (subsystem 2): `camera-modes.md`, 87 of 90 in play,
  three migrations, with `UiMovies::Open` and `Delete`;
  `--native_camera_modes`; 5a1babc..d71eff5. Validated (`itc_2732`,
  `itc_2750`, `itc_2766`).
- `Career::SetUpEvent` (cleanup lead, not counted): with `Garage::Add`
  and `sub_821104D8`, 5 adapters removed; `--native_career`;
  3f8fa4b..8267f2d. Validated (`itc_2692`, `itc_2693`).
- The race event (subsystem 1): `race-event.md`, 100 of 102, one
  migration; `--native_race_event`; 6cf1882. Validated (`itc_2673`,
  `itc_2676`).
## Rows to review

`review.md`, "Run 2026-10-30 11:39".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3167, work left 3165 (wrapped 104, known 474,
  unknown 2587).
- After the race event's map: ours 3167, work left 3165 (wrapped 104,
  known 511, unknown 2550).
- After the race event's migration 1 (`itc_2672`: race 37 s, 0 errors,
  race_event and career 0 differing): ours 3271, work left 3126
  (wrapped 104, known 472, unknown 2550).
- After its validation (`itc_2673`: race 37 s, 0 errors, every harness
  0 differing but the expected groups): ours 3271, work left 3126.
- After the cleanup's migration (`itc_2691`: race 37 s, 0 errors,
  career and race_event 0 differing): ours 3274, work left 3123
  (wrapped 104, known 470, unknown 2549).
- After its validation (`itc_2692`: race 5 s after the harnesses, 0
  errors, every harness 0 differing but the expected groups): ours
  3274, work left 3123.
- After the camera modes' map: ours 3274, work left 3123 (wrapped 104,
  known 558, unknown 2461).
- After the camera modes' migration 1 (`itc_2729`: race 26 s, 0
  errors, camera_modes 0 differing): ours 3348, work left 3049
  (wrapped 104, known 484, unknown 2461).
- After its validation (`itc_2732`, 60 s: race 14 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3348, work left
  3049.
- After the camera modes' migration 2 (`itc_2749`: race 26 s, 0
  errors, touched harnesses 0 differing): ours 3359, work left 3038
  (wrapped 104, known 473, unknown 2461).
- After its validation (`itc_2750`, 60 s: race 13 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3359, work left
  3038.
- After the camera modes' migration 3 (`itc_2765`: race 26 s, 0
  errors, camera_modes and cameras 0 differing): ours 3364, work left
  3033 (wrapped 104, known 468, unknown 2461).
- After its validation (`itc_2766`, 60 s: race 13 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3364, work left
  3033.
- After the location screen's map: ours 3364, work left 3033 (wrapped
  104, known 545, unknown 2384).
- After the parking area's migration 1 (`itc_2782`: race 36 s, 0
  errors, touched harnesses 0 differing): ours 3390, work left 3007
  (wrapped 104, known 519, unknown 2384).
- After its validation (`itc_2783`, 60 s: race 12 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3390, work left
  3007.
- After the parking area's migration 2 (`itc_2795`: race 31 s, 0
  errors, touched harnesses 0 differing but the expected group):
  ours 3413, work left 2984 (wrapped 104, known 496, unknown 2384).
- After its validation (`itc_2796`, 60 s: race 8 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3413, work left
  2984.
