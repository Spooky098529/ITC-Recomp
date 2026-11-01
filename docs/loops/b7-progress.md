# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-31 23:57 on `b7-loop-31-10-26-2`, from
  2eb33a3f; summary `b7-loop-2026-10-31-2357.md`.
- Limit for this run: 3 subsystems; 1 taken.
- Subsystem 1 of 3: the graphics device's side under the primitive
  drawer (`sub_822AA018`, `sub_822A9EE8`, `sub_822A8958`,
  `sub_822A8618`, `sub_822A8960`, `sub_825915E0`), the top of
  `leads.md` (the first two already in `shader-objects.md`). Map
  checked (112775ae: `device-shaders.md`, 9 rows, 7 in play: the
  device's SetVertexShader with its entry, SetPixelShader,
  SetVertexDeclaration, the render queue's constructor and pass lists;
  `sub_822A8808`'s row moved from `functions.md`, Wrapped by
  `src/native/refcount.cpp`; 4 ready groups, no outside blockers; 1
  lead added; no subagents). Step: migration agent running.

## Done

## Rows to review

`review.md`, "Run 2026-10-31 23:57".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3705, work left 2694 (wrapped 101, known 513,
  unknown 2080). `b7_status.py`'s one check (`movies.md:117`
  `sub_82108018`: its row says never called, but it ran) was there
  before the run.
- After the device shaders' map: ours 3705, work left 2694 (wrapped
  101, known 519, unknown 2074).

## Stopped
