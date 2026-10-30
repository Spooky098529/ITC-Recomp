# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 11:39 on `b7-loop-30-10-26`, from e320bad;
  summary `b7-loop-2026-10-30-1139.md`.
- Limit for this run: 3 subsystems; 1 taken.
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
  `career.md`). Validation running.

## Done

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
