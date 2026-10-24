# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-23 20:22 on `b7-loop-23-10-26-5` (the user's
  branch), from cf1f2cb; summary `b7-loop-2026-10-23-2022.md`.
- `leads.md`'s top lead (VirtualAlloc and VirtualFree, `sub_82295398`,
  `sub_82295330`) dropped: both already ours (native memory).
- Subsystem 1 of 3 (the AI drivers) finished: 143 of 148 ours; the
  rest waits on `sub_820900C0` and `sub_821434E8`.
- Subsystem 2 of 3 (the racer controllers) finished: 27 of 27 ours.
- Subsystem 3 of 3: the track zones (`track-zones.md`, mapped in
  52cd53b from the "path search" lead, which turned out to be 70
  stretches of the course's route the player's car is checked against
  for the HUD; 92 rows, 12 ready groups, no outside blockers). Step:
  migration 1 (agent running).

## Done

- The racer controllers (`racer-controllers.md`, mapped in b5491fb; 27
  rows): 27 ours (15 Native) in one migration (699bbde..7b85ae8) and
  the three jump handlers' prints (94b17b0); validated (`itc_1227`,
  `itc_1228`, `itc_1229`, `itc_1230`; replays ok; 2 to drive);
  `--native_racer_controllers`. Left: none; the handlers' adapters stay
  for the race flow's functors.

- The AI drivers (`ai-drivers.md`, mapped in 1eeb084; 148 rows): 143
  ours (119 Native) in five migrations (c94d6be..91b8da2,
  e01cc35..24669a4, 1521ad2..18ea84b, fcec6fd..2ea28d9,
  5a7f90d..2ddd638), each validated (`itc_1124`/`1125`, `1145`/`1146`,
  `1169`/`1170`, `1185`/`1186`, `1203`/`1204`; replays ok; 14 to
  drive); `--native_ai_drivers`. Left: Destroy and DestroyRings
  (`sub_820900C0`), UpdateStarting, UpdateTrack and NoteFixedSections
  (`sub_821434E8`).

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 20:22").

## Expected to differ

This run's groups: `expected-differences.md` (the "20:22 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1223, work left 5534 (wrapped 123, known 559, unknown 4852).
- After mapping the AI drivers: ours 1223, work left 5534 (wrapped 123,
  known 705, unknown 4706).
- After the AI drivers' migration 1: ours 1297, work left 5460 (wrapped
  123, known 631, unknown 4706). Race 22 s (`itc_1123`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (no
  "(expected to differ)" groups added).
- After the AI drivers' migration 2: ours 1315, work left 5442 (wrapped
  123, known 613, unknown 4706). Race 22 s (`itc_1144`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (none
  added).
- After the AI drivers' migration 3: ours 1337, work left 5420 (wrapped
  123, known 591, unknown 4706). Race 21 s (`itc_1168`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (none
  added).
- After the AI drivers' migration 4: ours 1353, work left 5404 (wrapped
  123, known 575, unknown 4706). Race 30 s (`itc_1184`, now
  `attract_race.sh 40`, the user's call) with `--run_harnesses=all`, 0
  errors, every harness 0 differing (none added).
- After the AI drivers' migration 5: ours 1366, work left 5391 (wrapped
  123, known 562, unknown 4706). Race 30 s (`itc_1202`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (none
  added).
- After mapping the racer controllers: ours 1366, work left 5391
  (wrapped 123, known 585, unknown 4683).
- After the racer controllers' migration 1: ours 1393, work left 5365
  (wrapped 123, known 559, unknown 4683). Race 28 s (`itc_1226`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (none
  added).
- After the racer controllers: ours 1393, work left 5365 (wrapped 123,
  known 559, unknown 4683).
- After mapping the track zones: ours 1393, work left 5365 (wrapped
  123, known 651, unknown 4591).
