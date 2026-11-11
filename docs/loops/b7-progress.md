# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-11 on `b7-loop-11-11-26` (made by the user for
  this run), from ef7711e6; summary `b7-loop-2026-11-11-0838.md`.
  Limit: 5 subsystems (the user's call for this run).
- Subsystem 1 of 5: the tuning screen's update `sub_821E6C50` (with
  `sub_821E6CD4`, `sub_821E6F90`; 1,601 lines, ready; first in the last
  run's "Next"; the top lead, `sub_82562FF0`, is inside the movie
  library's range, skipped as before); its line deleted from
  `leads.md`. Mapped into `tuning-screen.md` (eba99e25: 64 new rows,
  3 in play: the update and its two pieces, read in full, six blocks by
  line range in the intro's "The update"; 61 callees not seen in play,
  23 debug state handlers and 38 command runners, in outline; the
  sticker editor's commands wait on its lead, `Command21` on the vinyl
  editor's; no leads added, `sub_821FA460` added to the sticker lead;
  no subagents); map checked (0 errors, 1 ready group). Migration
  1f7f4e8e..360b0cbc (the update Overridden, its pieces Merged, 47
  adapters removed, 40 of the 61 never-seen callees ported; 21 left
  recompiled, blocked by the sticker and vinyl editors' leads and three
  never-seen `tuning-pages.md` setters; under `--native_tuning_screen`;
  race 31 s at 40, 0 errors, its harnesses 0 differing; 3 subagents,
  one at a time). Left: none. Validation failed: `itc_4919`, the
  `TuningScreen::Update` harness stuck at its `DebugRemoveConfirm`
  (`sub_821CC620`) group reading guest 0x700D0000 over and over (9.4M
  error lines, the race never started), 69 of 88 harness lines (19
  debug handlers' groups never ran); every harness that finished at 0
  differing; no prints added; the replay `itc_4920` ok (20 ran in it).
  By the user's call (2026-11-11, asked: "find, fix, continue"), a fix
  agent finds the wild read with logging and fixes the harness; then
  the validation again. The fix (505b2a31): case 11's restore path
  copied a random engine stage into the garage car, and `Car::Refresh`
  read the spec's engine records past their end (a wild torque curve
  count running off the stack top 0x700D0000; the address-dependent
  case passed in the migration's shorter run); the stages and tire
  types now laid in range from their own generator. Validated again by
  the fix agent's run `itc_4926` (race 60 s at 180, 0 errors, every
  harness 0 differing outside the expected groups, 88 of 88 harness
  lines, checked by the lead). 20 ran in the replay `itc_4920` (ok), 68
  to drive. Recorded.
- Subsystem 2 of 5: the front end's title menu (`sub_82222B60`,
  `sub_82221AC0`: ready, seen in play at every boot) with the
  front-end machine's set-up beside it (`sub_822213B0`, `sub_82220C88`,
  probably the boot or title screen); both leads' lines deleted from
  `leads.md`. Mapped `title-screen.md` (de062dec: 87 rows, 2 moved
  from `functions.md`, 55 in play: the type 4 mode, its Init
  `sub_82224C70` and Release, the title load machine at +28920
  (`TitleLoad::Init` `sub_822213B0`, 12 transitions) and the title
  screen's at +38560 (`TitleScreen::Init` `sub_82224270`, 17
  transitions, its pages and transition table); `TitleScreen::Update`
  `sub_822221A0` Wrapped (the attract hook); 47 ready groups; no leads,
  no subagents); map checked. Migration 05cef504..216e9843 (86 of 87
  ours, `--native_title_screen`, `src/title_screen/`; `TitleScreen::Update`
  Overridden inside the `--attract_after_s` wrap; the two machine Inits'
  adapters removed; `SetUpAttractRace` `sub_82222038` left recompiled,
  its harness world not built before the boot's data load; three
  existing adapters (`BootData::Load`, `Career::NextDay`,
  `CarDealer::StartFirstRace`) now ask the harness recorder first;
  race 33 s at 40, 0 errors, its harnesses and the touched files' 0
  differing; no subagents). Left: none. Step: validation agent.

## Done

- The tuning screen's update: `tuning-screen.md` 126 of 126 ours (the
  update Overridden, 47 adapters removed, 40 never-seen callees
  ported, 21 left recompiled), under `--native_tuning_screen`,
  1f7f4e8e..360b0cbc, harness fix 505b2a31; validated.

## Rows to review

The run's section of `review.md` ("Run 2026-11-11 08:38").

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 5445, work left 1063 (wrapped 56, known 389,
  unknown 618).
- After the update's map: ours 5445, work left 1063 (wrapped 56,
  known 392, unknown 615).
- After its migration: ours 5488, work left 1060 (wrapped 56, known
  389, unknown 615).
- After the title screen's map: ours 5488, work left 1060 (wrapped 56,
  known 442, unknown 562).
- After its migration: ours 5574, work left 1005 (wrapped 55, known
  388, unknown 562).

## Stopped
