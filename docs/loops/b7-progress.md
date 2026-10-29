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
- Subsystem 1 of 3: XACT's tracks and events (the top of `leads.md`,
  with the next lead, as it says they read best mapped together).
  Map a3880fc checked (`xact-tracks.md`, 143 rows, 55 in play, 34
  ready groups). Migration 1 checked (44 ours,
  c8ad278..6fc2206). Validated (`itc_2432`,
  `itc_2433`: 44 of 44 harness lines; one to drive). Step: migration
  agent 2 running (`XACTTrackInstance::Prepare` and above).

## Done

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
