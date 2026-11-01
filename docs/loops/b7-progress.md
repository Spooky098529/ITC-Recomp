# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-01 12:56 on `b7-loop-01-11-26`, from d8348c7b;
  summary `b7-loop-2026-11-01-1256.md`. Limit: 3 subsystems.
- Subsystem 1 of 3: the boot mode (`boot-mode.md`). Done.
- Subsystem 2 of 3: `Car::DrawUpdate` `sub_820DAC68` (the scene
  model's per-pass update; its lead from `leads.md`, ready: 78
  functions, 16,029 lines, split for the migration). Map checked
  (24c2b1c1, `car-draw.md`: 86 rows, 14 moved from `functions.md`; 0
  errors; 1 ready group; the second set's per-frame passes lead
  covered and deleted; the reading split over six subagents, one at a
  time). Step: migration (agent running).

## Done

- The boot mode: `boot-mode.md` (map 49161912, migration 49f60a69,
  `--native_boot_mode`, 12 of 12 in play with 7 in `movies.md`;
  validated `itc_3219`: race 53 s, 0 errors, 19 of 19 harness lines,
  16 ran in the race, replay `itc_3220` ok, 3 to drive).

## Rows to review

`review.md`, "Run 2026-11-01 12:56".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3962, work left 2441 (wrapped 82, known 490,
  unknown 1869). `b7_status.py`'s one check (`movies.md:117`
  `sub_82108018`: its row says never called, but it ran) was there
  before the run.
- After the boot mode's map: ours 3962, work left 2441 (wrapped 82,
  known 498, unknown 1861).
- After its migration (`itc_3218`: race 38 s, 0 errors, boot_mode 0
  differing): ours 3981, work left 2424 (wrapped 80, known 483,
  unknown 1861).
- After its validation (`itc_3219`, 120 s: race 53 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3981, work left
  2424.
- After the car draw's map: ours 3981, work left 2424 (wrapped 80,
  known 547, unknown 1797).
