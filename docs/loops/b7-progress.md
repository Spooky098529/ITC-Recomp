# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: `b7-loop-2026-10-22-0111.md`, `b7-loop-2026-10-22-1047.md`,
`b7-loop-2026-10-22-1439.md`, `b7-loop-2026-10-22-2211.md`,
`b7-loop-2026-10-23-0152.md`, `b7-loop-2026-10-23-1106.md`, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-23 13:44 on `b7-loop-23-10-26-3` (the user's
  branch), from 97be4fc; summary `b7-loop-2026-10-23-1344.md`.
- Subsystem 1 of 3 (the model animation) finished: the rest waits on
  the graphics device.
- Subsystem 2 of 3 (the car specs) finished: the rest waits on the
  archive.
- Subsystem 3 of 3: the game's cameras above `CameraPass::Update` (the
  lead whose functions run every frame, nearly all ready); mapping agent
  running.

## Done

- The car specs (`car-specs.md`, already mapped; 48 rows): 47 ours in
  one migration (d87b553..114aa35), validated (47 of 47 harness lines;
  all ran in the attract race or the replay, `itc_947`, `itc_948` ok);
  `--native_car_specs`. Left: `Params::Load` (the archive);
  `Params::Free` not seen in play.

- The model animation (`model-animation.md`, mapped in c425049; 39
  rows): 36 ours in one migration (6f51fc2, 6714e72), validated (37 of
  37 harness lines with `models.md`'s `sub_8259C3A8`; 34 ran in the
  attract race or the replay, `itc_932`, `itc_933` ok; 3 to drive);
  `--native_model_animation`. Left: the primitive drawer's methods (the
  graphics device).

## Rows to review

This run's rows: `review.md` ("Run 2026-10-23 13:44").

## Expected to differ

This run's groups: `expected-differences.md` (the "13:44 run" lines).

## To check in a drive

`to-drive.md` (this run's section, and what's left from earlier runs).

## Totals

- Start: ours 1023, work left 5730 (wrapped 129, known 586, unknown 5015).
- After mapping the model animation: ours 1023, work left 5730 (wrapped
  129, known 622, unknown 4979).
- After the model animation's migration 1: ours 1060, work left 5696
  (wrapped 129, known 588, unknown 4979). Race 19 s at 30 ticks/s with
  `--run_harnesses=all`, 0 errors, every harness 0 differing outside
  the earlier "(expected to differ)" groups (none added).
- After the car specs' migration 1: ours 1107, work left 5649 (wrapped
  129, known 541, unknown 4979). Race 19 s (`itc_946`), the same checks.

## Stopped
