# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-28 22:22 on `b7-loop-28-10-26-3` (the user's
  branch), from 26a5119; summary `b7-loop-2026-10-28-2222.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3 done: the car sounds and X3DAudio (below).
  Recorded in the plan's table and boundary log.
- Subsystem 2 of 3 done: XAudio's voices (below). Recorded in the
  plan's table and boundary log.
- Subsystem 1's continuation (not counted): the car sounds' migration
  2, their groups under XAudio's voice methods now ready. Migration 2
  done (be59a3f..edce57c, 29 more, 53 of 62 ours; stopped at five
  batches, `CarSound::Update`'s other parts ready). Validated
  (`itc_2382`, `itc_2383`): 37 of 37 harness lines; one to drive.
  Migration 3 done (fd88bdf..cfd838d, 62 of 62 in play ours, nothing
  ready left). Step: validation (agent running).

## Done

- XAudio's voices: `xaudio.md`, 92 of 96 in play native in two
  migrations, 37 Native; both locks the game's, taken as the original
  does (the user's calls, 2026-10-29); `--native_xaudio`; e784bc0,
  29a2e7f..fbcd34f, 19a919f..24cc6ea, print ceb220f. Validated
  (`itc_2356`, `itc_2357`): 92 of 92 harness lines; nine to drive.
- The car sounds and X3DAudio: `car-sounds.md`, 24 of 62 in play
  native in one migration, 12 adapters removed; `--native_car_sounds`;
  26adc92, 9a0dcc7..3e6c24e. Validated (`itc_2328`, `itc_2329`): 24 of
  24 harness lines, all ran in play or the replay; nothing to drive.

## Rows to review

`review.md`, "Run 2026-10-28 22:22".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 2628, work left 3833 (wrapped 103, known 498,
  unknown 3232).
- After the car sounds' map: ours 2628, work left 3833 (wrapped 103,
  known 559, unknown 3171).
- After the car sounds' migration 1 (`itc_2327`: race 38 s, 0 errors,
  car_sounds and x3daudio 0 differing): ours 2652, work left 3809
  (wrapped 103, known 535, unknown 3171).
- After its validation (`itc_2328`: race 8 s, the harnesses ~30 s, 0
  errors, every harness 0 differing but the expected groups): ours
  2652, work left 3809.
- After XAudio's map: ours 2652, work left 3809 (wrapped 103, known
  628, unknown 3078).
- After XAudio's migration 1 (`itc_2337`: race 38 s, 0 errors, xaudio
  0 differing): ours 2686, work left 3775 (wrapped 103, known 594,
  unknown 3078).
- After XAudio's migration 2 (race 38 s, 0 errors, xaudio 0
  differing): ours 2744, work left 3720 (wrapped 103, known 539,
  unknown 3078).
- After its validation (`itc_2356`: race 8 s, the harnesses ~30 s, 0
  errors, every harness 0 differing but the expected groups): ours
  2744, work left 3720.
- After the car sounds' migration 2 (race 38 s, 0 errors, car_sounds,
  x3daudio and xaudio 0 differing): ours 2773, work left 3691 (wrapped
  103, known 510, unknown 3078).
- After its validation (`itc_2382`: race 8 s, 0 errors, every harness
  0 differing but the expected groups): ours 2773, work left 3691.
- After the car sounds' migration 3 (`itc_2397`: race 38 s, 0 errors,
  car_sounds, x3daudio, xaudio 0 differing): ours 2782, work left 3682
  (wrapped 103, known 501, unknown 3078).

## Stopped

Not stopped.
