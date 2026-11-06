# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-06 01:02 on `b7-loop-06-11-26`, from 8f0dd3af;
  summary `b7-loop-2026-11-06-0102.md`. Limit: 3 subsystems.
- Two leads deleted as covered: the device lock's callers (all six
  mapped: `frame-passes.md`, `app-start-up.md`, `movies.md`,
  `screen-objects.md`, `post-effects.md`) and the other `ScopedLock`
  users (five in `shader-objects.md` and `models.md`, ours;
  `sub_82593E78` is `MovieWrapper::Open` in `movies.md`).
- Subsystem 1 of 3: the graphics library's command buffer (lead
  picked: `sub_8229E308`, `sub_8229EB10`, `sub_8229E5D0`,
  `sub_8229E768`, `sub_8229DE48`, `sub_8229EDA8`, `sub_8229DC28`,
  `sub_822AF978`; its line deleted from `leads.md`). Map checked
  (aa4b202c: `command-buffer.md`, 36 rows, 16 moved from
  `functions.md`, 24 in play; 0 errors; 9 ready groups; 2 leads
  added; no subagents). Migration e041aa4a..abca2df1
  (`--native_command_buffer`: the segment tail, save, chain close,
  callback packets, next space, data space, the default draw
  registers' marks without their packets; `sub_8229DC28`,
  `sub_8229E5D0`, `sub_8229DE48`, `sub_8229DDD8`, `sub_822AC470`
  Dropped; `itc_3604`: race 38 s, 0 errors, every harness 0 differing
  but `sub_822AF978`'s expected group; no subagents); left:
  `sub_8229EB10` (ready), then the segment flow's cycle
  (`sub_8229EF48`, `EDA8`, `EC00`, `E908`). Validated `itc_3607`
  (race 46 s, 0 errors, 7 of 7 harness lines, 4 ran in the race,
  `SaveSegment`, `CommandChain::Close`, `WriteCallback` in the replay
  `itc_3608` (ok); none to drive). Second migration
  8d43620c..632f2b7d (`CommandChain::NewBlock` with `Init`, the
  segment flow's cycle as one group with `EndSegment`'s counter wrap
  kept, `ReserveWords` with `InsertCallback`, `WaitForIdle` and
  `sub_8229F0E0` without their packets; 5 adapters removed;
  `itc_3622`: race 36 s, 0 errors, every harness 0 differing but the
  expected groups; no subagents); nothing ready left (the rest's
  callers are recompiled packet writers and the tiling worker). Step:
  validation agent running.

## Done

## Rows to review

`review.md`, "Run 2026-11-06 01:02".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4197, work left 2213 (wrapped 60, known 469,
  unknown 1684).
- After the command buffer's map: ours 4197, work left 2213 (wrapped
  60, known 481, unknown 1672).
- After its migration: ours 4209, work left 2201 (wrapped 60, known
  469, unknown 1672).
- After its validation (`itc_3607`): ours 4209, work left 2201.
- After its second migration: ours 4219, work left 2191 (wrapped 59,
  known 460, unknown 1672).

## Stopped
