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
- Subsystem 1 of 3 done: XACT's tracks and events (below). Recorded in
  the plan's table and boundary log.
- Subsystem 2 of 3 done: XAudio's engine and mixer (below). Recorded in
  the plan's table and boundary log.
- The user's calls (2026-10-29, now under "Precedent first" in
  `b7-loop.md`): XAudio's threads, events, semaphore and notification
  through their imports as the original does; QueryPerformanceCounter
  from the host clock at 50 MHz, like GetTickCount.
- The continuations (not counted) done (below). Recorded in the plan's
  table and boundary log.
- Subsystem 3 of 3: XAudio's factory objects and codecs (`leads.md`,
  with the XAPO lead). Map a611121 checked (`xaudio-objects.md`, 121
  rows, 109 in play, 86 ready groups). Step: migration agent 1 running.

## Done

- The continuations: QueryPerformanceCounter (the host clock),
  `XACTGetTimeMs`, XACT's tracks 55 of 55, waves 31 of 31 (`Fire`'s,
  `SendNotification`'s, `Notify`'s and `SetState`'s r3 matched in full),
  XAudio's engine 47 of 47; be9c0c5..12896c8. Validated (`itc_2472`,
  `itc_2473`; `itc_2481`, `itc_2482`, the replay logged the intermittent
  BaseHeap pair; `itc_2503`, `itc_2504`): 20, 16 and 16 harness lines;
  `XACTWave::Finish` to drive.
- XAudio's engine and mixer: `xaudio-engine.md`, 40 of 47 in play
  native in one migration, 11 adapters removed; `--native_xaudio_engine`;
  610125a, e9d3f21..d17ae9f. Validated (`itc_2458`, `itc_2459`): 40 of
  40 harness lines, all ran in play; nothing to drive.
- XACT's tracks and events: `xact-tracks.md`, 47 of 55 in play native
  in two migrations, 16 adapters removed; `--native_xact_tracks`;
  a3880fc, c8ad278..6fc2206, 84a6ef7..65b3944. Validated (`itc_2432`,
  `itc_2433`; `itc_2443`, `itc_2444`): 44 and 5 harness lines; one to
  drive.

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
- After XAudio's engine's migration 1 (race 37 s, 0 errors,
  xaudio_engine 0 differing): ours 2892, work left 3572 (wrapped 103,
  known 523, unknown 2946).
- After its validation (`itc_2458`: race 8 s, 0 errors, every harness
  0 differing but the expected groups): ours 2892, work left 3572.
- After QueryPerformanceCounter and XACT's clock users (`itc_2471`:
  race 38 s, 0 errors, clock, xact, xact_tracks, xact_waves 0
  differing; the user's coverage commit cf7114c took 148 stale entries
  out): ours 2909, work left 3407 (wrapped 103, known 513, unknown
  2791).
- After its validation (`itc_2472`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 2909, work left 3407.
- After XACT's waves' last two (`itc_2480`: race 38 s, 0 errors,
  xact_waves, xact_tracks, xact, xaudio 0 differing): ours 2912, work
  left 3405 (wrapped 104, known 510, unknown 2791).
- After its validation (`itc_2481`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 2912, work left 3405.
- After the r3 fix and XAudio's engine's 7 (race 38 s, 0 errors,
  xact, xact_waves, xact_tracks, xaudio_engine, xaudio 0 differing):
  ours 2919, work left 3398 (wrapped 104, known 503, unknown 2791).
- After its validation (`itc_2503`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 2919, work left 3398.
- After XAudio's objects' map: ours 2919, work left 3398 (wrapped 104,
  known 612, unknown 2682).
