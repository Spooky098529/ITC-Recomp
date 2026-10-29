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
- Subsystem 2 of 3: XAudio's voice methods (`sub_8245A768` and ten
  more, the car sounds' blockers; the lead closest to ready, 8 of its
  11 ready). Map checked (e784bc0); its stop (the two locks) put to
  the user: host recursive locks with a readable depth, taken by every
  user (the user's call, 2026-10-29). Migration 1 (29a2e7f..fbcd34f,
  34 ours, what takes neither lock) stopped on the spin lock, taken
  inline: the user's call, 2026-10-29, the game's own spin lock until
  its users are native, the API lock a host recursive lock now. Step:
  migration 2 (agent running).

## Done

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

## Stopped

Not stopped.
