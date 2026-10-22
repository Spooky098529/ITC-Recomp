# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. The first run's
record: `b7-loop-2026-10-22-0111.md` (and this file's history).

## Now

- Run started 2026-10-22 10:47 on `b7-loop-22-10-26` (made by the user,
  from 600ed23); summary `b7-loop-2026-10-22-1047.md`.
- Subsystem 2: the vehicle accessors (the leads' vehicle, wheel, body,
  contact and scene-model accessors, with `sub_825C8388`'s lead folded
  in: both block the race overlays' updates). Step: mapping (agent
  running).
- Session 2: pass 1 of 20, subsystem 1 of 3. In all: subsystem 2 of 5.
- Known: `b7_status.py` checks `sub_825A9934` (`__chkstk`) "Dropped, but
  it ran", and every function made Native or Merged this run "..., but
  it ran" (`seen_in_play.txt` predates them; a new coverage session
  clears them).

## Done

- The race overlays (`race-overlays.md`, mapped in 6baeabf; 158 rows):
  88 ours (77 Overridden, 9 Native, 2 Merged) in five migrations
  (76ddc05..8af4216), each validated (harness lines for every function);
  `--native_race_overlays`. Nothing ready and takeable left: the rest
  waits on guest vtable calls, guest allocation, memcpy's mirror
  recording (no plain C++ entry), unseen callees, and `sub_8215E150`
  (with RaceOverlays::Run).
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
  The second migration's 6 rows and the 9 names.
  The third migration's 10 rows (4 new). For review: NaN-only harness
  results depend on which operand the compiler kept in the translated
  vector adds: Smoke::Emit keeps the first operand's NaN (as the Xenon
  does), the sparks a plain `addps` matching how this build compiled
  theirs; a rebuild could bring back NaN-only differences there.
  The fourth migration's 32 rows, and `sub_820C0128`'s notes in
  `functions.md`. For review: on the native SpeedKmh path the state
  capture (`--capture_dir`) no longer hears about the vehicle (the
  game's other callers still pass it on).
  The fifth migration's 8 rows (4 Overridden, `sub_8215E078` Merged, 3
  Native). `attract_race.sh` now counts seconds logged at 31 ticks/s too
  (it reported 19 for a 22-second race).

## Expected to differ

- `sub_82129598` Trail::AddPoint (`race-overlays.md`, 8d7ad32): "NaN or
  infinite inputs (expected to differ)", 126 cases, 5 differ: only the
  NaNs' sign and payload (the translated code's Newton step as the
  compiler rewrote it); its 474 finite cases are the same. Accepted by
  the user, 2026-10-22.

## To check in a drive

Functions made native this run with no `native running (game):` line in
the validation agent's attract race; the user drives (with
`--log_level=info`) and checks the log for them when the loop ends.

- Adapters lead (itc_512.log): `sub_825A9DD8` strcpy_s, `sub_825B6378`
  ___dtold, `sub_825B8B48` $I10_OUTPUT, `sub_825B6458` _fltout2,
  `sub_825AE8E0` _cftof_l, `sub_825AE708` _cftof2_l, `sub_825B61E8`
  _fptostr, `sub_825B7A80` _putwc_nolock, `sub_8211B298`
  AnimationList::Get.
- Race overlays (itc_524.log): `sub_82129598` Trail::AddPoint (probably
  the skid marks); (itc_540.log) `sub_8212D818` Sparks::Emit (no
  scraping in attract races); (itc_563.log) `sub_82123A00`
  Sprite::ResetStyle2, `sub_8212A868` Smoke::InRegion (again not seen in
  itc_584.log).

Checked in two drives (2026-10-22, `itc_585.log` and `itc_586.log`): all
seen except `sub_825A9DD8` strcpy_s, whose copy runs inside the native
number conversion with no line of its own; its other callers don't run in
play.

## Totals

- Start (2026-10-22, after the first run): ours 335, work left 6418
  (wrapped 142, known 243, unknown 6033).
- After the adapters lead: unchanged (ours 335, work left 6418).
- After mapping the race overlays: ours 335, work left 6418 (wrapped
  142, known 396, unknown 5880).
- After the race overlays: ours 422, work left 6331 (wrapped 142, known
  313, unknown 5876).

## Stopped

(Session 1 was ended by the user after the race overlays, 2026-10-22:
not a failure. Session 2 started the same day.)
