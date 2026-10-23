# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, and this file's history. What's left to
see in a drive, from every run: `to-drive.md`.

## Now

- Run started 2026-10-23 17:22 on `b7-loop-23-10-26-4` (the user's
  branch), from 305cc52; summary `b7-loop-2026-10-23-1722.md`.
- Subsystem 1 of 3 (the string table and font) finished: the rest
  waits on `DataFile::GetEntry`, the archive, the XPR, the shader
  compiler and the device's state save.
- Subsystem 2 of 3 (`std::string`) finished: the two static set-ups
  wait on `atexit`.
- Subsystem 3 of 3 (the course's route) finished: all 40 ours, with
  `TrackPosition::Init` and `Update`; 18 adapters stay for recompiled
  callers. The loop stopped at the run's limit (below).

## Done

- The course's route (`track-route.md`, mapped in e211173; 40 rows): 40
  ours in two migrations (9c3cbaa..7785364, 0b863a1..1b58c97), with
  `collision-world.md`'s `TrackPosition::Init` and `Update`; validated
  (33 of 33 and 15 of 15 harness lines; `itc_1085`, `itc_1086`,
  `itc_1099`, `itc_1100`, replays ok; 4 to drive);
  `--native_track_route`. Left: none ready; the race and vehicle code's
  recompiled callers keep 18 adapters.

- `std::string` (`std-string.md`, mapped in 7aeebb5; 16 rows): 14 ours
  in one migration (2028ce7..5fdcfc2), validated (14 of 14 harness
  lines, all ran in the attract race; `itc_1064`, `itc_1065`, replay
  ok; none to drive); `--native_std_string`. Left: the static set-ups
  (`atexit`).

- The string table and font (`strings-and-font.md`, mapped in 0974397;
  36 rows): 25 ours in one migration (16458d3..29aea2e), validated (27
  of 27 harness lines; `itc_1043`, `itc_1044`, replay ok; 2 to drive);
  `--native_strings_and_font`. Left: `Strings::Get` and what calls it,
  the loads, `Font::Font` and the shaders.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 17:22").

## Expected to differ

This run's groups: `expected-differences.md` (the "17:22 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1139, work left 5617 (wrapped 123, known 551, unknown 4943).
- After mapping the string table and font: ours 1139, work left 5617
  (wrapped 123, known 586, unknown 4908).
- After the string table and font's migration 1: ours 1167, work left
  5590 (wrapped 123, known 559, unknown 4908). Race 19 s (`itc_1042`)
  with `--run_harnesses=all`, 0 errors, every harness 0 differing (no
  "(expected to differ)" groups added).
- After mapping `std::string`: ours 1167, work left 5590 (wrapped 123,
  known 575, unknown 4892).
- After `std::string`'s migration 1: ours 1181, work left 5576 (wrapped
  123, known 561, unknown 4892). Race 19 s (`itc_1063`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing (no
  "(expected to differ)" groups added).
- After mapping the course's route: ours 1181, work left 5576 (wrapped
  123, known 601, unknown 4852).
- After the course's route's migration 1: ours 1215, work left 5542
  (wrapped 123, known 567, unknown 4852).
  Race 18 s (`itc_1083`) with `--run_harnesses=all`, 0 errors, every
  harness 0 differing (none added); 18 s is the harnesses' ~4.9 s inside
  the 25 s timeout: without them 23 s (`itc_1084`), accepted by the
  lead.
- After the course's route's migration 2: ours 1223, work left 5534
  (wrapped 123, known 559, unknown 4852). Race 23 s (`itc_1098`, with
  `attract_race.sh 30`) with `--run_harnesses=all`, 0 errors, every
  harness 0 differing (none added).

## Stopped

2026-10-23, after the course's route: the run's limit (3 subsystems)
reached. Everything passed: harnesses 0 differing outside the earlier
"(expected to differ)" groups (none added), attract races with all
harnesses 18-19 s under `attract_race.sh 25` (accepted: the harnesses'
~5 s; 23 s without them, `itc_1084`), then 23 s under
`attract_race.sh 30` (the user's call), no errors, replays ok. Next:
the user reviews `review.md` ("Run 2026-10-23 17:22"), drives a race
for `to-drive.md`, and merges.
Concluded (the user's call, 2026-10-23): merged into
`rexglue-removal-project` before a drive; `to-drive.md`'s functions
for this run are still to see.
