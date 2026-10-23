# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`,
`b7-loop-2026-10-23-1344.md`, and this file's history. What's left to
see in a drive, from every run: `to-drive.md`.

## Now

- Run started 2026-10-23 17:22 on `b7-loop-23-10-26-4` (the user's
  branch), from 305cc52; summary `b7-loop-2026-10-23-1722.md`.
- Subsystem 1 of 3 (the string table and font) finished: the rest
  waits on `DataFile::GetEntry`, the archive, the XPR, the shader
  compiler and the device's state save.
- Subsystem 2 of 3: `std::string` (`leads.md`'s top lead), mapped in
  7aeebb5 (16 rows, 3 ready groups); step: migration 1 (a migration
  agent running).

## Done

- The string table and font (`strings-and-font.md`, mapped in 0974397;
  36 rows): 25 ours in one migration (16458d3..29aea2e), validated (27
  of 27 harness lines; `itc_1043`, `itc_1044`, replay ok; 2 to drive);
  `--native_strings_and_font`. Left: `Strings::Get` and what calls it,
  the loads, `Font::Font` and the shaders.

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 17:22").

## Expected to differ

This run's groups: `expected-differences.md` (the "17:22 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1139, work left 5617 (wrapped 123, known 551, unknown 4943).
- After mapping the string table and font: ours 1139, work left 5617
  (wrapped 123, known 586, unknown 4908).
- After the string table and font's migration 1: ours 1167, work left
  5590 (wrapped 123, known 559, unknown 4908). Race 19 s (`itc_1042`)
  with `--run_harnesses=all`, 0 errors, every harness 0 differing (no
  "(expected to differ)" groups added).
- After mapping `std::string`: ours 1167, work left 5590 (wrapped 123,
  known 575, unknown 4892).

## Stopped
