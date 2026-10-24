# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, `b7-loop-2026-10-23-1722.md`,
`b7-loop-2026-10-23-2022.md`, `b7-loop-2026-10-24-0325.md`,
`b7-loop-2026-10-24-1601.md`, and this file's history. What's left to
see in a drive, from every run: `to-drive.md`.

## Now

- Run started 2026-10-24 17:46 on `b7-loop-24-10-26-3` (the user's
  branch), from 206854d; summary `b7-loop-2026-10-24-1746.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead (`sub_8212EC90`,
  `sub_8212ED78`, `sub_8212EA98`, `sub_8212EDE0`: at the end of the race
  overlays' address range but not overlays; all four ready). Mapped as the post effects
  (`post-effects.md`, 57 rows: 3 moved from `functions.md`, 54 new;
  f3643e1; 32 ready groups; 3 leads added); map checked (0 errors, 57 of
  57 in play). Migration 1 done (ece3c32..66b8f81: 43 functions,
  44 of 57 ours, nothing ready left; 9 adapters removed;
  `--native_post_effects`), validated (`itc_1395`, `itc_1396`; all
  ran in play or the replay). Finished.

## Done

- The post effects (`post-effects.md`, mapped in f3643e1; 57 rows): 44
  ours in one migration (ece3c32..66b8f81), 9 adapters removed;
  validated (`itc_1395`, `itc_1396`); `--native_post_effects`. Left:
  `RenderTextures::Acquire` and the recorders on `sub_8235A9F8`, the
  models' shader pairs, the vertex declaration, `sub_8209AE48`.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-24 17:46").

## Expected to differ

This run's groups: `expected-differences.md` (the "17:46 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1714, work left 5046 (wrapped 122, known 587, unknown 4337).
- After mapping the post effects: ours 1714, work left 5046 (wrapped
  122, known 641, unknown 4283).
- After the post effects' migration 1: ours 1757, work left 5003
  (wrapped 122, known 598, unknown 4283). Race 25 s (`itc_1394`) with
  `--run_harnesses=all`, 0 errors, every harness 0 differing.

## Stopped

(not stopped)
