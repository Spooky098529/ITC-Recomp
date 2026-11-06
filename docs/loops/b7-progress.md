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
  `sub_822AF978`; its line deleted from `leads.md`). Step: mapping
  agent running.

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

## Stopped
