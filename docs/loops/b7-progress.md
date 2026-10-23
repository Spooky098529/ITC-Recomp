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
- Subsystem 3 of 3: the rest of XACT's upper part (its unmapped vtable
  slots, `825BF5C8`-`825D0000`, with the small shared helpers), rows
  added to `xact.md` (mapped in 284f3a2: 37 rows there, 2 in
  `functions.md`; 33 ready groups); step 8, migration 2 done (4e8a8fc..ebdb01f, 46
  ours); validation agent running.
  `sub_822956F0` (probably GetTickCount, reading the kernel's
  timestamp bundle) left recompiled: the user's call.

## Done

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

(none yet)

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
