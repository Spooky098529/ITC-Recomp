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
  `leads.md`. Mapped `type2-mode.md` (21 rows, 16 in play: the Init,
  the vtables `0x82006B9C` and `0x82006B90`, `Interrupt`
  `sub_82197858`, `MachineOf`, `AllLoaded`, `FindShown`) and
  `user-info.md` (14 rows, all in play: the player's record, UI movie
  10, at block +0x102D8), with two hub rows in `type2-screens.md`
  (ebde5050; 10 and 11 ready groups; no leads, no subagents); map
  checked. Migration fd78a046 (the user info screen, 14 of 14,
  `--native_user_info`) and 5c818857 (the type 2 mode, 20 of 21,
  `--native_type2_mode`; `Resume` a jump into the hub's recompiled
  `sub_8218DE18`); race 37 s at 40, 0 errors, their harnesses and
  `type67_modes`' 0 differing outside its recorded group; no
  subagents. Left: none. Validated: `itc_4825` (race 64 s at 180, the
  harnesses ~114.5 s, 0 errors, every harness 0 differing outside the
  expected groups), 33 of 33 harness lines, no prints added; 4 ran in
  the race, 16 more in the replay `itc_4826` (ok), 13 to drive.
  Recorded.
- Subsystem 3 of 3: the tuning screen's refresh `sub_821E3AD0` (3,838
  lines, seen in play, ready; it blocks `TuningMachine::Start`) and its
  callees; its line deleted from `leads.md`. Mapped into
  `tuning-screen.md` (cbff27b9: 8 rows, 1 in play, the refresh; its
  blocks by line range in the intro's "The refresh"; its 7 callees not
  seen in play; no leads, no subagents); map checked. Step: migration
  agent running (with `TuningMachine::Start` `sub_821E7368` once the
  refresh is ours).

## Done

- The type 2 mode and its user info screen: `type2-mode.md` 20 of 21,
  `user-info.md` 14 of 14, `--native_type2_mode`, `--native_user_info`,
  fd78a046, 5c818857; validated.

- The car settings screen: `car-settings.md`, 7 of 7 in play (all
  Overridden), `--native_car_settings`, 42211c94..73886334; its
  validation halted on another subsystem's harness, fixed (f691a91e)
  and validated again.

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
- After the type 2 mode's map: ours 5409, work left 1095 (wrapped 56,
  known 420, unknown 619).
- After its migration: ours 5443, work left 1065 (wrapped 56, known
  390, unknown 619).
- After the refresh's map: ours 5443, work left 1065 (wrapped 56,
  known 391, unknown 618).

## Stopped

(Halted 2026-11-11 at the car settings' validation on
`GarageScreen::Transition6`'s harness; fixed and resumed by the user's
call, see "Now".)
