# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md` (their drive
lists are there), and this file's history.

## Now

- Run started 2026-10-23 01:52 on `b7-loop-23-10-26` (the user's
  branch), from 21cdb6a; summary `b7-loop-2026-10-23-0152.md`.
- Subsystem 1 of 3 (the sound cues) finished: the rest waits on XACT.
- Subsystem 2 of 3 (XACT) finished: the rest waits on blockers.
- Subsystem 3 of 3 (XACT's settings and slots, a second map into
  `xact.md`) finished. The run's limit reached: stopped.

## Done

- XACT's settings and slots (mapped in 284f3a2: 37 rows in `xact.md`, 2
  in `functions.md`): 47 ours in two migrations (4e8a8fc..ebdb01f,
  validated 52 of 52, replay ok; e659e09..222ad37, validated 6 of 6);
  `--native_xact`; `xact.md` 102 of 186. Left: the critical section,
  XAudio and the voices, vtable calls into the tracks and waves, the
  clock (GetTickCount, the user's call).

- XACT (`xact.md`, mapped in 7ba4592; 149 rows): 55 ours in one
  migration (904e6ee..0c0fd63), validated (55 of 55 harness lines, all
  ran in the attract race; the migration's log `itc_870` at the same
  code); `--native_xact`. Left: the critical section, the settings'
  unmapped slots, vtable calls into recompiled code, XAudio and the
  voices, the tracks, events and waves.

- The sound cues (`sound-cues.md`, mapped in bfa9d03; 87 rows): 30
  ours in one migration (6187975..fae15f5), validated (30 of 30
  harness lines, all ran in the attract race or the replay; replay
  ok); `--native_sound_cues`. Left: everything calling XACT through its
  vtables and their callers, the bank loads (file layer, std::string),
  the message handler.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 01:52").

## Expected to differ

This run's groups: `expected-differences.md` (the "01:52 run" lines).

## To check in a drive

None: every function made native this run ran in an attract race or
the pad replay.

## Totals

- Start: ours 837, work left 5916 (wrapped 137, known 322, unknown 5457).
- After mapping the sound cues: ours 837, work left 5916 (wrapped 137,
  known 405, unknown 5374).
- After the sound cues' migration 1: ours 867, work left 5886 (wrapped
  137, known 375, unknown 5374).
- After mapping XACT: ours 867, work left 5886 (wrapped 137, known 524,
  unknown 5225).
- After XACT's migration 1: ours 922, work left 5831 (wrapped 137, known
  469, unknown 5225).
- After XACT's second map: ours 922, work left 5831 (wrapped 137, known
  508, unknown 5186).
- After XACT's migration 2: ours 968, work left 5785 (wrapped 137, known
  462, unknown 5186).
- After XACT's migration 3: ours 970, work left 5783 (wrapped 137, known
  460, unknown 5186).

## Stopped

2026-10-23, the run's limit (3 subsystems: the sound cues, XACT, XACT's
settings and slots). Not a failure: every check and validation passed.
The user reviews `review.md` ("Run 2026-10-23 01:52"; the GetTickCount
call), drives a race, and merges.
Drive (2026-10-23, `itc_895.log`, ~2 minutes at 30 ticks/s, 0 errors;
everything sounded normal to the user): all 133 functions whose Status
changed this run logged `native running (game)`.
