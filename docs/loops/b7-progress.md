# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-12 on `b7-loop-11-11-26` (the previous run's
  branch, kept by the user's call for this run), from 575b2b9a;
  summary `b7-loop-2026-11-12-0826.md`. Limit: 5 subsystems (the
  user's call for this run).
- First: the replay theater's check (the previous run's port
  49287852, unchecked: its harness, a race, `--native_type10_mode` on,
  the adapterless rows set to Native), a lead without mapping (it
  doesn't count toward the limit). Migration 50318046..93b074ab: a
  harness fault fixed first (by the precedent: `MoveCursor`'s case 86
  wrote guest 0x10 in a loop, a replay record's +28 given wild values
  where play gives 0-99), then 46 of 46 harnesses at 0 differing,
  `--native_type10_mode` on by default, 8 adapterless rows Native,
  `Replay::SetUpRace` left recompiled (never ran); no adapters
  removable (all reached through tables); race 38 s at 40, 0 errors;
  no subagents. Validated: `itc_4951` (race 57 s at 180, 0 errors,
  every harness 0 differing outside the expected groups), 46 of 46
  harness lines, no prints added; none ran in the race or the replay
  `itc_4952` (ok): 46 to drive. Recorded.
- Subsystem 1 of 5: the type 9 front-end mode (`sub_8225D6D0` its
  Init, its slots 6, 8, 10, 11, 13, +32 slot 0; ready groups under it;
  the previous run's "Next"; the top lead `sub_82562FF0` is inside the
  movie library's range, skipped as before); its line deleted from
  `leads.md`. Mapped `todays-result.md` (0752af3f: 57 rows, 31 in
  play: the career's end of the day after a career race, two machines,
  today's result (UI movie 27, block +0, `Career::NextDay`) and today's
  event (UI movie 0, block +9640, the unlocks); leaves for the boot
  mode when a movie is queued, else the garage; 26 ready groups; one
  lead added, the type 12 mode; no subagents); map checked (0 errors,
  file listed, 26 ready groups). Migration b7718efe..ba41ed1b (57 of
  57 ours: 38 Overridden, 19 Native, `Kind`'s adapter removed;
  `--native_todays_result`; never-seen paths ported; 57 harnesses 0
  differing; race 37 s at 40, 0 errors; harness faults fixed on the
  way; three of the map's addresses corrected; no subagents). Left:
  none. Validation agent running.

## Done

- The replay theater's check: `type10-mode.md` 46 of 47 ours,
  `--native_type10_mode` on, 50318046..93b074ab; validated.

## Rows to review

The run's section of `review.md` ("Run 2026-11-12 08:26").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5612, work left 984 (wrapped 55, known 392,
  unknown 537).
- After the theater's check: ours 5620, work left 980 (wrapped 55,
  known 388, unknown 537).

- After the type 9 mode's map: ours 5620, work left 980 (wrapped 55,
  known 419, unknown 506).

- After its migration: ours 5677, work left 949 (wrapped 55, known
  388, unknown 506).

## Stopped
