# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md` (their drive lists are there), and this
file's history.

## Now

- Run started 2026-10-23 11:06 on `b7-loop-23-10-26-2` (the user's
  branch), from 5b4cecd; summary `b7-loop-2026-10-23-1106.md`.
- Subsystem 1 of 3 (the action mappers) finished: the rest waits on
  the input manager's vtable targets and `sub_8258CE60`.
- Subsystem 2 of 3 (the data tables) finished: the rest waits on the
  archive. The loop stopped at its validation (below).

## Done

- The data tables (`data-tables.md`, mapped in d8f876f; 18 rows): 16
  ours in one migration (db90dc4..4344730), validated 15 of 16 harness
  lines (16 of 16 after `RecordsItem`, `itc_923`) (`itc_921`; replay `itc_922` ok); `--native_data_tables`. Left:
  DataTables::Init and Load (the archive).

- The action mappers (`action-mappers.md`, mapped in 422b250; 40 rows):
  36 ours in one migration (006e5e4..c67265f), validated (35 of 35
  harness lines, one Merged; all ran in the attract race or the replay,
  replay ok); `--native_action_mappers`. Left: BindAll and Bind (the
  input manager's vtable targets), the mapper's constructor and Init
  (`sub_8258CE60`).

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 11:06").

## Expected to differ

This run's groups: `expected-differences.md` (the "11:06 run" lines).

## To check in a drive

The data tables' `sub_821171B0` Find20, `sub_82117210` Entry20,
`sub_82117228` Entry40 (in neither the attract race nor the replay; the
replay was ok), `sub_82116DA8` ItemRecords::Item ran in a second replay after its
print was added (`itc_924`, 127 s, 0 errors); the other three didn't. The action mappers' functions all ran.

## Totals

- Start: ours 971, work left 5782 (wrapped 137, known 459, unknown 5186).
- After mapping the action mappers: ours 971, work left 5782 (wrapped
  137, known 484, unknown 5161).
- After the action mappers' migration 1: ours 1007, work left 5746
  (wrapped 129, known 456, unknown 5161). Its races ran 19 s at 30
  ticks/s with `--run_harnesses=all` (36 new harnesses at boot); 23 s
  without, with `--native_action_mappers` on and off (`itc_913`,
  `itc_914`).
- After mapping the data tables: ours 1007, work left 5746 (wrapped 129,
  known 474, unknown 5143).
- After the data tables' migration 1: ours 1023, work left 5730 (wrapped
  129, known 458, unknown 5143).

## Stopped

2026-10-23, at the data tables' validation (settled after, below): `sub_82116DA8`
ItemRecords::Item has no harness line (`missing:`). It's a thunk whose
adapter (`src/hooks/data_tables.cpp`) and harness step both call
`data_tables::Item` directly, which prints as `sub_82117178`; its 300
harness cases match. Giving it a print means a plain C++ function of
its own and a harness change (more than the validation agent may do),
or the user may call it Merged. Everything else passed. The user
reviews `review.md` ("Run 2026-10-23 11:06"), drives a race, and merges.
Settled (the user's call, 2026-10-23): the adapter stays (two recompiled
callers in play, `sub_820F2D28`, `sub_82114B88`), and calls a one-line
`data_tables::RecordsItem` with its own print, as does its harness step:
`native running (harness)` logged, 300 cases 0 differing (`itc_923`, 19
s, 0 errors). It's still on the drive list (its callers are the save
data's).
Concluded (the user's call, 2026-10-23): the run ends at 2 subsystems,
not resumed for a third. Next: the user reviews `review.md`, drives a
race for the drive list above, and merges; a new run starts afresh.
