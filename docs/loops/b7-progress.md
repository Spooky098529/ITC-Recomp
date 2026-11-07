# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-07 10:56 on `b7-loop-07-11-26` (made by the user
  for this run), from 48e2b333; summary `b7-loop-2026-11-07-1056.md`.
  Limit: 3 subsystems (`b7-loop.md`'s setting; the previous run's 8 was
  the user's call for that run).
- Subsystem 1 of 3: the tuning screen's other vtable slots and pages
  (lead picked: `sub_821D6AD0` and the 17 others, the most ready groups
  in `leads.md`, 45, beside the tuning screen just finished; the top two
  leads skipped: `sub_82562FF0` inside the movie library's range, the
  worker pools blocked only by the core's event and critical section
  wrappers; its line deleted from `leads.md`). Map checked (92fea740:
  `tuning-pages.md`, 106 rows, all new, 67 in play: `TuningScreen::HandleInput`
  `sub_821E6950` and its pages for states 0, 3-8, 15-22, 53-62; 0
  errors; 48 ready groups; 4 leads added (the vinyl and sticker editors'
  pages, the screen's other helper callers, `sub_8224B248`), the menu
  helpers' lead trimmed; three subagents, one at a time). Migration
  94da1ea6..5fd92bc3 (`--native_tuning_pages`, harness `tuning_pages`:
  the counts, owned flags, wheel and inch list ends, the decal images'
  textures and movie restores, `EnterEditor`, `StoreVinyls`,
  `LoadVinyls`, `ShowPricePanel`, `SetBrandLogo`, `Color::HsvToRgb`; 22
  Overridden, 6 Merged, 6 adapters removed; race 38 s, 0 errors, all
  its harnesses 0 differing; no subagents); left: 27 ready groups
  (`DecalImages::UnloadAll`/`Unload` first; the pages; `sub_821D74A0`
  with `sub_821D32C8` over 1,500 lines, to split). Validated
  `itc_4251` (race 92 s at 180, the harnesses ~86 s; 0 errors, every
  harness 0 differing, 28 of 28 harness lines; none in the race, 10 in
  the replay `itc_4252` (ok), 18 to drive). Second migration agent
  running.

## Done

## Rows to review

`review.md`, "Run 2026-11-07 10:56".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 4476, work left 1937 (wrapped 57, known 418,
  unknown 1462).
- After the tuning pages' map: ours 4476, work left 1937 (wrapped 57,
  known 485, unknown 1395).
- After its migration: ours 4510, work left 1904 (wrapped 57, known
  452, unknown 1395).

## Stopped
