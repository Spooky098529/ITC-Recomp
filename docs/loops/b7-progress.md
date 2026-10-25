# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-25 01:17 on `by-loop-25-10-26` (the user's
  branch), from db25cd9; summary `b7-loop-2026-10-25-0117.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead, the app framework, mapped
  last run (`framework.md`, 68 rows, c4b5b86). Map checked (0 errors,
  68 of 68 in play, ready groups; nothing calls the kernel). Migration
  1 done after five batches (f27748b..e080edd: getters, thunks, the
  run loop's maps and request vector, dispatchers, constructors; 50 of
  68 ours, 15 adapters removed; `--native_framework`; its races ran
  every harness but `ai_drivers`). The loop stopped, then resumed
  (the user's call, 2026-10-25, after dbc8ee0's harness fix). Step:
  validation, agent running.

## Done

(none finished: the framework has its main group left, see "Stopped")

## Rows to review

This run's rows: `review.md` ("Run 2026-10-25 01:17").

## Expected to differ

This run's groups: `expected-differences.md` (the "01:17 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1847, work left 4604 (wrapped 113, known 651, unknown 3840).
- After the framework's migration 1: ours 1897, work left 4554 (wrapped
  111, known 603, unknown 3840). Race 30 s (`itc_1559`, every harness
  but `ai_drivers`), 0 errors, those harnesses 0 differing.

## Stopped

Resumed (the user's call). Earlier: 2026-10-25, after the framework's migration 1, whose report's `stop:`
wasn't `none`: `attract_race.sh 40 --run_harnesses=all` fails on this
branch, not from the framework. The `ai_drivers` harness's
`AiDriver::UpdateIdle` (`sub_821399A8`) case 1928 makes the recompiled
original fault (guest reads near `0x7035xxxx`, millions of "Unhandled
guest access violation" lines, 0 s raced; `itc_1535`-`itc_1538`, the
same with `--native_framework=false`) whenever `--use_hardcoded_shaders`
is on, the default since the HLSL compiler's replacement; with it off
the harness passes (`itc_1539`); the last good run was `itc_1518`. The
agent's guess, not confirmed: the case reads guest memory its set-up
doesn't write, and sees different leftovers now. For the user: how to
fix or exclude that case; then the framework's validation, and its
main group (`main`, `App::Run`, ~304 lines). 1 of 3 subsystems
started. `check_function_statuses.py`: 0 errors.

Fixed afterward (dbc8ee0, the user's go-ahead): a harness bug, not the
shaders. The Update family's cases (AiPath::Update, AiDriver::Update,
DriveCar, UpdateIdle) reach `AiPath::MeasureOthers` but never set the
racer count or the driver's list at +452, so an index was a fill word and
its record a wild address, mapped or not by where the harness block
landed (`0x02191000` since the shaders changed the boot's allocations).
Now set as the Avoid cases do. `attract_race.sh 40 --run_harnesses=all`
(`itc_1568`): 27 s, 0 errors, every harness 0 differing outside
"(expected to differ)". Still to do: the framework's validation.
