# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-10 on `b7-loop-10-11-26-2` (made by the user for
  this run), from aadaff66; summary `b7-loop-2026-11-10-2352.md`.
  Limit: 3 subsystems.
- Subsystem 1 of 3: the type 2 mode's machine at +0x173D0
  (`sub_821B6FC0` and its transitions, UI movie 3; first in the last
  run's "Next"; the top lead, `sub_82562FF0`, is inside the movie
  library's range; its line deleted from `leads.md`). Mapped
  `car-settings.md` (98f0843b: 91 rows, 7 in play: the car settings
  screen, UI movie 3, the garage car's gear ratios and settings, with
  the type 5 mode's Init `sub_821B7878`; 7 ready groups; `Update`
  `sub_821B6818` and the 114 functions only it reaches left as a lead;
  no subagents); map checked. Migration 42211c94..73886334 (the five
  handlers, then both Inits by step tables; 7 of 7 in play ours, all
  Overridden: functors and recompiled callers; `--native_car_settings`;
  races 38 s at 40, 0 errors, its 7 harnesses 0 differing; no
  subagents). Left: none (the 84 never seen in play stay recompiled).
  Validation halted: `itc_4810` (race 65 s at 180, the harnesses
  ~113 s, 0 errors), 7 of 7 harness lines, every `car_settings` case
  matched, but `GarageScreen::Transition6`'s harness (the forty-first
  run's `garage_screen`) differed in 2 of 100 cases. No prints added.
  The replay `itc_4811` was ok: 6 ran in it, `InitType5` to drive.
- After the halt, by the user's call (2026-11-11, "find what the
  problem is and fix it, then continue"): `Transition6` and
  `Transition7` index the actions' 5-entry help line tables with the
  action list's selection unchecked, and the harness's menu world gave
  it -2 to count + 1, so `SetText` read text from random string ids
  (a temporary log, `itc_4812`); the harness keeps it to the 5 actions,
  as `SetUpActions` inits the list in play (f691a91e). Validated again
  `itc_4813` (race 65 s at 180, 0 errors, every harness 0 differing
  outside the expected groups, 7 of 7 harness lines). Loop resumed.
- Subsystem 2 of 3: the type 2 mode's Init `sub_82197278` with the
  machine Init it still holds, `sub_8220B408` (a front-end screen's
  transitions, with `sub_8220B070` to `sub_8220B2C0`); ready since the
  car settings' Inits (809 lines); both leads' lines deleted from
  `leads.md`. Step: mapping agent running.

## Done

- The car settings screen: `car-settings.md`, 7 of 7 in play (all
  Overridden), `--native_car_settings`, 42211c94..73886334; its
  validation halted on another subsystem's harness.

## Rows to review

The run's section of `review.md` ("Run 2026-11-10 23:52").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5402, work left 1102 (wrapped 56, known 390,
  unknown 656).

- After the car settings' map: ours 5402, work left 1102 (wrapped 56,
  known 397, unknown 649).

- After its migration: ours 5409, work left 1095 (wrapped 56, known
  390, unknown 649).

## Stopped

(Halted 2026-11-11 at the car settings' validation on
`GarageScreen::Transition6`'s harness; fixed and resumed by the user's
call, see "Now".)
