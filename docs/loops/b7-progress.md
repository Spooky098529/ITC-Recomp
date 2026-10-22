# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. The first run's
record: `b7-loop-2026-10-22-0111.md` (and this file's history).

## Now

- Run started 2026-10-22 10:47 on `b7-loop-22-10-26` (made by the user,
  from 600ed23); summary `b7-loop-2026-10-22-1047.md`.
- Subsystem: the race overlays (`race-overlays.md`, mapped in 6baeabf).
  First migration agent: 34 functions Overridden in 5 groups
  (76ddc05..3fc85b8), `--native_race_overlays`; 32 ready groups left.
  Validated (itc_512.log): 34 of 34 harness lines, all ran in play.
- Adapters lead validated: prints added (b88246d), 29 of 31 harness
  lines; `sub_825B6458` (_fltout2) and `sub_8211B298`
  (AnimationList::Get) have no function of their own (their work is
  inside their native callers): set to the new Merged status (the
  user's, 2026-10-22), not a halt.
- Next: the race overlays' second migration agent (32 ready groups).
- Session: pass 4 of 20, subsystem 1 of 5. In all: subsystem 1 of 5.
- Known at the start: `b7_status.py` checks `c-runtime.md:221`
  `sub_825A9934` (`__chkstk`) "Dropped, but it ran" (from the first run:
  its override does nothing, so it still shows as seen).

## Done

- Adapters lead (no mapping, not counted): 31 adapters removed, now
  Native (`c-runtime.md` 23, `ui-movies.md` 5, `ui-movies-game.md` 3);
  4 recorders' adapters stay (their work calls the renderer through
  `ctx`).
- `b7_status.py` now also checks the 31 removed adapters' functions
  "Native, but it ran": `seen_in_play.txt` predates them (their
  recompiled bodies ran then); a new coverage session (the user plays,
  `docs/coverage.md`) clears them. Harnesses moved to their own register steps. No flags.
  3403e03..2cf3c9c. Race 20 s, 0 errors, all harnesses 0 differing.

## Rows to review

- `c-runtime.md`, `ui-movies.md`, `ui-movies-game.md`, `sprites.md`: the
  adapters lead's rows (Overridden -> Native, and notes on the 4 kept).
- `race-overlays.md` (new, 154 rows; `sub_8215DEA0` moved from
  `functions.md`), `sprites.md`'s intro, `known-data.md`'s new rows;
  the first migration's 34 rows (Overridden). For review: RacerId's
  adapter (`sub_82126AE0`) sign-extends r3 where the original
  zero-extends a found id (only the upper 32 bits; callers compare 32).

## To check in a drive

Functions made native this run with no `native running (game):` line in
the validation agent's attract race; the user drives (with
`--log_level=info`) and checks the log for them when the loop ends.

- Adapters lead (itc_512.log): `sub_825A9DD8` strcpy_s, `sub_825B6378`
  ___dtold, `sub_825B8B48` $I10_OUTPUT, `sub_825B6458` _fltout2,
  `sub_825AE8E0` _cftof_l, `sub_825AE708` _cftof2_l, `sub_825B61E8`
  _fptostr, `sub_825B7A80` _putwc_nolock, `sub_8211B298`
  AnimationList::Get.

## Totals

- Start (2026-10-22, after the first run): ours 335, work left 6418
  (wrapped 142, known 243, unknown 6033).
- After the adapters lead: unchanged (ours 335, work left 6418).
- After mapping the race overlays: ours 335, work left 6418 (wrapped
  142, known 396, unknown 5880).

## Stopped

(not stopped)
