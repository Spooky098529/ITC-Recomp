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
  differing, race 37 s at 40, 0 errors; no subagents. Validated:
  `itc_5235` (race 50 s at 180, 0 errors, every harness 0 differing
  outside the expected groups), 15 of 15 harness lines, no prints
  added; none ran in the race or the replay `itc_5236` (ok): 15 to
  drive. Recorded.
- Subsystem 1 of 10: the morph jobs (`sub_820ACC08` and the
  `820AC998`-`820ACC08` vtable slots: the 224-byte job the second
  worker pool runs for `Model::MorphShape`, the wheels' per-draw
  morphs; ready, run every frame); its line deleted from `leads.md`.
  Mapped `morph-jobs.md` (3b635fc5: 21 rows, 14 in play, with the
  shared 36-byte work item class at vtable `0x820066DC`; 13 ready
  groups; the pool's post `sub_82095A28` a guest call by the
  `DebugText`/`FramePasses` precedent; no leads added). Check passed.
  Migrated 752011b3..63bc5ef5: all 21 rows ours (13 ready groups and 7
  never-seen functions in one batch), `--native_morph_jobs`;
  `Model::MorphShape` calls the maker's plain C++; `Morph::Blend`'s
  adapter removed; 19 adapters kept (vtable slots); two harness faults
  fixed on the way (r3's high word); every harness 0 differing, race
  33 s at 40, 0 errors; no subagents. Validated: `itc_5244` (race
  50 s at 180, 0 errors, every harness 0 differing outside the
  expected groups), 22 of 22 harness lines, 3 prints added (3123f921);
  the work item's slots and `Morph::Blend` ran in the race, none in
  the replay `itc_5245` (ok): 15 to drive. Recorded; subsystem 1
  done.
- Subsystem 2 of 10: the save devices' constructor (`sub_820A6160`,
  `sub_820A4560`, run at boot, ready); its line deleted from
  `leads.md`. Mapping agent running.

## Done

- The morph jobs: `morph-jobs.md` 21 of 21 ours,
  `--native_morph_jobs`, 752011b3..3123f921; validated.

- Cleanup leads (no mapping): `RaceStates::OnRaceEnd` with the course
  record (`race-states.md`), `DrawEntryPowerGraph`'s r31
  (`power-graph.md`), 5 adapters removed, 98f4fe26..52b87de7;
  validated.

## Rows to review

The run's section of `review.md` ("Run 2026-11-13 12:31").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5989, work left 718 (wrapped 51, known 361,
  unknown 306).
- After the cleanup leads: ours 5998, work left 717 (wrapped 51,
  known 360, unknown 306).
- After the morph jobs' map: ours 5998, work left 717 (wrapped 51,
  known 374, unknown 292).
- After the morph jobs: ours 6019, work left 703 (wrapped 51, known
  360, unknown 292).

## Stopped
