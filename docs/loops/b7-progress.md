# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-25 01:17 on `by-loop-25-10-26` (the user's
  branch), from db25cd9; summary `b7-loop-2026-10-25-0117.md`.
- Limit for this run: 3 subsystems; since 2026-10-25, the framework only
  (the user's call: end the run after it).
- Subsystem 1 of 3: `leads.md`'s top lead, the app framework, mapped
  last run (`framework.md`, 68 rows, c4b5b86). Map checked (0 errors,
  68 of 68 in play, ready groups; nothing calls the kernel). Migration
  1 done after five batches (f27748b..e080edd: getters, thunks, the
  run loop's maps and request vector, dispatchers, constructors; 50 of
  68 ours, 15 adapters removed; `--native_framework`; its races ran
  every harness but `ai_drivers`). The loop stopped, then resumed
  (the user's call, 2026-10-25, after dbc8ee0's harness fix). Validated
  (`itc_1569`: 50 of 50 harness lines, all 50 ran in play; 0 errors).
  Migration 2 done (eb25d68, ad01ea7: the main group, 7 adapters
  removed; 56 of 68 ours, nothing ready left), validated (`itc_1587`:
  8 of 8 harness lines, all ran in play). Finished.

## Done

- The app framework (`framework.md`, mapped in c4b5b86; 68 rows): 56
  ours in two migrations (f27748b..e080edd, eb25d68..ad01ea7), 22
  adapters removed; validated (`itc_1569`, `itc_1587`);
  `--native_framework`. Left: `Framework::Init` and the services'
  set-ups, `sub_820908D8`, `Users::Create`, `SaveDevices::Create`,
  `sub_82090840`/`890`/`8C0`, `FrameStart::OnTick`.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-25 01:17").

## Expected to differ

This run's groups: `expected-differences.md` (the "01:17 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1847, work left 4604 (wrapped 113, known 651, unknown 3840).
- At the end (after migration 2 and its validation): ours 1903, work
  left 4548 (wrapped 111, known 597, unknown 3840). Race 27 s
  (`itc_1587`), 0 errors, every harness 0 differing.
- After the framework's validation: race 27 s (`itc_1569`), 0 errors,
  every harness 0 differing.
- After the framework's migration 1: ours 1897, work left 4554 (wrapped
  111, known 603, unknown 3840). Race 30 s (`itc_1559`, every harness
  but `ai_drivers`), 0 errors, those harnesses 0 differing.

## Stopped

2026-10-25, after the framework, by the user's call (end the run after
this subsystem). 1 subsystem migrated. `check_function_statuses.py`: 0
errors. Earlier in the run: stopped after migration 1 on the
`ai_drivers` harness's fault (UpdateIdle's case 1928 reading a wild
address: the Update family's cases never set the racer count and the
list at +452, left to the fill, mapped or not by where the harness
block landed; exposed when the hardcoded shaders changed the boot's
allocations, not caused by them), fixed in dbc8ee0, then resumed.
