# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-28 08:36 on `b7-loop-28-10-26` (the user's
  branch), from 4ffc2cd; summary `b7-loop-2026-10-28-0836.md`.
- Limit for this run: 3 subsystems.
- Subsystem 1 of 3: `leads.md`'s top lead, `sub_8235AAC0` (probably
  a DXT compressor, from `Vinyl::Compress` `sub_820ED090`; 1 ready
  group, 11 functions, 2,495 lines: split the migration). Mapped as
  texture compression (`texture-compression.md`, 157 rows, all new,
  24 seen in play: `XGCompressSurface` and the XGraphics/D3DX
  blitter, the vinyl sheet's A8R8G8B8 -> DXT5; f1c484c; 1 lead
  added). Map checked (0 errors, 1 ready group `sub_8235AAC0`, its 8
  indirect calls reaching 13 more functions seen in play, no
  blockers outside it, no kernel calls). Migration agent 1 running.

## Done

(nothing yet)

## Rows to review

This run's rows: `review.md` ("Run 2026-10-28 08:36").

## Expected to differ

This run's groups: `expected-differences.md` (the "08:36 run" lines).

## To check in a drive

`to-drive.md` (this run's heading, when it has any).

## Totals

- At the start: ours 2400, work left 4052 (wrapped 103, known 522,
  unknown 3427).
- After the texture compression's map: ours 2400, work left 4052
  (wrapped 103, known 546, unknown 3403).

## Stopped

(not stopped)
