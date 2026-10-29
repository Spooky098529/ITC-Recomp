# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-29 15:44 on `b7-loop-29-10-26` (the user's
  branch), from e282f62; summary `b7-loop-2026-10-29-1544.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3 done: XACT's tracks and events (the top of `leads.md`,
  with the next lead, as it says they read best mapped together).
  Map a3880fc checked (`xact-tracks.md`, 143 rows, 55 in play, 34
  ready groups). Migration 1 checked (44 ours,
  c8ad278..6fc2206). Validated (`itc_2432`,
  `itc_2433`: 44 of 44 harness lines; one to drive). Done (below); recorded in the plan's table and boundary log.
- Subsystem 2 of 3: XAudio's engine and mixer (`leads.md`). Map
  610125a checked (`xaudio-engine.md`, 70 rows, 47 in play, 24 ready
  groups). Step: migration agent 1 running (groups without XAudio's
  threads, events and semaphore, which have no precedent).

## Done

- XACT's tracks and events: `xact-tracks.md`, 47 of 55 in play native
  in two migrations, 16 adapters removed; `--native_xact_tracks`;
  a3880fc, c8ad278..6fc2206, 84a6ef7..65b3944. Validated (`itc_2432`,
  `itc_2433`; `itc_2443`, `itc_2444`): 44 and 5 harness lines; one to
  drive. The rest waits on `XACTGetTimeMs` over QueryPerformanceCounter.

## Rows to review

`review.md`, "Run 2026-10-29 15:44".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 2804, work left 3660 (wrapped 103, known 509,
  unknown 3048).
- After XACT's tracks' map: ours 2804, work left 3660 (wrapped 103,
  known 564, unknown 2993).
- After XACT's tracks' migration 1 (`itc_2431`: race 38 s, 0 errors,
  xact_tracks 0 differing): ours 2848, work left 3616 (wrapped 103,
  known 520, unknown 2993).
- After its validation (`itc_2432`: race 7 s, the harnesses ~33 s, 0
  errors, every harness 0 differing but the expected groups): ours
  2848, work left 3616.
- After XACT's tracks' migration 2 (`itc_2442`: race 38 s, 0 errors,
  xact_tracks and xact 0 differing): ours 2852, work left 3612 (wrapped
  103, known 516, unknown 2993).
- After its validation (`itc_2443`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 2852, work left 3612.
- After XAudio's engine's map: ours 2852, work left 3612 (wrapped 103,
  known 563, unknown 2946).
