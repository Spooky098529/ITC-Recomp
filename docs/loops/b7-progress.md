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
- Subsystem 1 of 3: the device's shader setters and the render queue
  (`device-shaders.md`). Done (map 112775ae, migration
  657efc7b..e727bc39, validated `itc_3054`: 120 s, race 55 s, 0 errors,
  7 of 7 harness lines, all 7 ran in the race, replay not needed).
- Cleanup lead (no mapping, not counted): `RaceHudMachine::CreateScreens`
  `sub_8216CA28`, `ClosePause` `sub_8216C658`, `ResultsDraw`
  `sub_8216D6D0`, `AfterRaceDraw` `sub_8216C6D8` with `sub_82211D10`,
  and `sub_82211D08` (`race-hud-machine.md`, `race-hud-object.md`),
  all ready; then `--removable` on the three files. Migration checked
  (61f7966b: the six; 8c5e2c76..7476d237: 6 adapters removed in
  `race-screens.md` and `race-hud-object.md`; the three files'
  harnesses 0 differing, race 25 s with them, 0 errors; unseen callees
  left on the recompiled path behind the flags; no subagents). Left:
  `PauseDraw` `sub_8216CF08` and `sub_8216CFE0`, ready now (a new
  cleanup lead). Validated (`itc_3072`, 120 s: race 54 s, 0 errors;
  replay `itc_3073` ok): 12 of 12 harness lines, 2 ran in the race, 5
  in the replay, 5 to drive. Done.
- Cleanup lead (no mapping, not counted): `RaceHudMachine::PauseDraw`
  `sub_8216CF08` and `sub_8216CFE0` (`race-hud-machine.md`), then
  `sub_82211D08`'s adapter. Step: migration agent running.

## Done

- The race HUD machine's `CreateScreens`, `ClosePause`, `ResultsDraw`,
  `AfterRaceDraw` (cleanup lead): with `sub_82211D08`, `sub_82211D10`
  and 6 adapters removed; 61f7966b..7476d237. Validated (`itc_3072`).

- The device's shader setters and the render queue (subsystem 1):
  `device-shaders.md`, 7 of 7 in play, one migration;
  `--native_device_shaders`; 112775ae..e727bc39. Validated
  (`itc_3054`).

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
- After its migration (`itc_3053`: race 38 s, 0 errors, device_shaders
  0 differing): ours 3712, work left 2687 (wrapped 100, known 513,
  unknown 2074).
- After its validation (`itc_3054`, 120 s: race 55 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3712, work left
  2687.
- After the race HUD machine cleanup's migration (`itc_3071`: race 25 s,
  0 errors, three harnesses 0 differing): ours 3718, work left 2681
  (wrapped 100, known 507, unknown 2074).
- After its validation (`itc_3072`, 120 s: race 54 s, 0 errors, every
  harness 0 differing but the expected groups): ours 3718, work left
  2681.

## Stopped
