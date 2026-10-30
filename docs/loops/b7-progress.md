# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 11:39 on `b7-loop-30-10-26`, from e320bad;
  summary `b7-loop-2026-10-30-1139.md`.
- Limit for this run: 3 subsystems; 2 taken.
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
  play ours). Validation running. Left: `CameraDirector::InitStates`,
  `TvCamera::TvCamera`, `Update` and `Step`, `GarageCamera::Update`;
  then, past `UiMovies::Open` (ready), the cutscene and the director's
  `SetMode`, `Update` (`--redraw_rival_logo`'s split kept).

## Done

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
