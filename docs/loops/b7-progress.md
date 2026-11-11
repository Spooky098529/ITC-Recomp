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
  one at a time). Left: none. Step: validation agent.

## Done

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

## Stopped
