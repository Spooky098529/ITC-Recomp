# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-04 19:51 on `b7-loop-03-11-26` (the user's call,
  2026-11-04: keep going on this branch, no new one), from a65d2472;
  summary `b7-loop-2026-11-04-1951.md`. Limit: 3 subsystems.
- Subsystem 1 of 3: the cars put back on the road (the top lead, the
  race data's record copies `sub_82102B98`, `sub_82102AA8`, with the
  lead that calls them, the race code at `82104310`-`82104E00`:
  `sub_821044D8` and the rest; both lines deleted from `leads.md`).
  Map checked (dcfccb78: `replay-snapshots.md`, 30 rows, all new, 26
  in play; 0 errors; 3 ready groups, `sub_821044D8`, `sub_82104AB8`,
  `sub_82104F68`; not resets: the replay's start and end snapshots;
  the input recordings' lead taken with it). Migration
  e9389918..1a5be4a1 (`--native_replay_snapshots`, 26 of 26 in play
  ours, 2 adapters removed; RaceReplay's four in the `race_world`
  harness; `itc_3484`/`itc_3486`: race 28 s (the harnesses' time), 0
  errors, every harness 0 differing; none runs in an attract race; no
  subagents). Validated `itc_3487` (race 47 s, 0 errors, 26 of 26
  harness lines, every harness 0 differing but the expected groups);
  none ran in the race or the replay `itc_3488` (ok): 26 to drive.
  Done.
- Subsystem 2 of 3: the race's sounds (lead picked: the race's sound
  handlers `sub_82175908` and the rest, 8 of 8 ready, all in play,
  with the readers of the scene models' sound slots `sub_82175F20`,
  `sub_82176188` beside them, 2 of 2 ready; both lines deleted from
  `leads.md`). Map checked (53aa7a89: `race-sounds.md`, 31 rows, all
  new, 29 in play; 0 errors; 15 ready groups; the race mode's machine
  at +138284; `RaceSounds::Init` 1,680 lines, split the migration).
  `rand` `sub_825A9BB8` (per-thread seed over `_getptd`, left for the
  core 2026-10-25) blocks `PlayStartMusic` `sub_82175DB0`, `Begin`
  `sub_82176618` and `IdleTick` `sub_821767A8`: raised with the user
  (the call's own condition). Step: migration agent running (the
  ready groups).

## Done

- The replay's snapshots: `replay-snapshots.md` (map dcfccb78,
  migration e9389918..1a5be4a1, `--native_replay_snapshots`, 26 of 26
  in play; validated `itc_3487`: race 47 s, 0 errors, 26 of 26 harness
  lines, replay `itc_3488` ok, 26 to drive).
## Rows to review

`review.md`, "Run 2026-11-04 19:51".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4099, work left 2309 (wrapped 62, known 480,
  unknown 1767).
- After the replay snapshots' map: ours 4099, work left 2309 (wrapped
  62, known 506, unknown 1741).
- After its migration: ours 4125, work left 2283 (wrapped 62, known
  480, unknown 1741).
- After its validation (`itc_3487`): ours 4125, work left 2283.
- After the race's sounds' map: ours 4125, work left 2283 (wrapped
  62, known 509, unknown 1712).

## Stopped
