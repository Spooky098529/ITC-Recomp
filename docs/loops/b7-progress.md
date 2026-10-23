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
- Subsystem 2 of 3: XACT's body (lead `sub_825C2BB0`, the user's call
  to translate it); step 2, the mapping agent running.

## Done

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
