# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-13 on `b7-loop-13-11-26` (made by the lead at
  the user's call, from `rexglue-removal-project`), from 59e05585;
  summary `b7-loop-2026-11-13-1231.md`. Limit: 10 subsystems (the
  user's call for this run).
- First: two leads without mapping (not counted toward the limit):
  `RaceStates::OnRaceEnd` `sub_821644D8` and the adapter fix of
  `CarList::DrawEntryPowerGraph` `sub_82250670`; their lines deleted
  from `leads.md`. Migrated 98f4fe26..52b87de7: `DrawEntryPowerGraph`'s
  r31 sign-extended (8 new cases, differing before the fix);
  `OnRaceEnd` native under `--native_race_states` with its 4
  never-seen callees and theirs ported (the course record, `Time::Add`;
  by the user's 2026-11-12 call to minimize recompiled code), 5
  adapters removed (`RacerRecords::Add`, `Stats::AddRaceResult`,
  `Achievements::Check`, `Write`, `Replay::Keep`); every harness 0
  differing, race 37 s at 40, 0 errors; no subagents. Validation agent
  running. Subsystems so far: 0 of 10.

## Done

## Rows to review

The run's section of `review.md` ("Run 2026-11-13 12:31").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5989, work left 718 (wrapped 51, known 361,
  unknown 306).

## Stopped
