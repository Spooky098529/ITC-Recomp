# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-29 22:22 on `b7-loop-29-10-26-2` (the user's
  branch), from 6fe366a; summary `b7-loop-2026-10-29-2222.md`.
- Limit for this run: 3 subsystems.
- First, a cleanup lead without mapping (not counted), the last run's
  stop: the XMA codec's two destructors' harness (`sub_8246A918`,
  `sub_8246AAD0`: their case releases the codec through the original),
  then what's ready in `xaudio-objects.md` (the resampler, the reverb's
  core). The precedent: the run of 2026-10-24 21:33 began with the
  previous stop's harness lines as such a lead. Migration 1 checked
  (3ea5812..01a849c: the destructors' cases, `SetLevels` fixed, the
  resampler, 10 adapters removed, four reverb stages; 91 of 109 ours).
  Validated (`itc_2529`, `itc_2530`): 23 of 23 harness lines; two to
  drive. Migration 2 running. Ready and left: `PcmCodec::Process`, the
  reverb core's other stages and its process, set-up and create.

## Done

## Rows to review

`review.md`, "Run 2026-10-29 22:22".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 2999, work left 3318 (wrapped 104, known 532,
  unknown 2682).
- After the cleanup's migration 1 (race 38 s, 0 errors, xaudio_objects
  0 differing): ours 3012, work left 3305 (wrapped 104, known 519,
  unknown 2682).
- After its validation (`itc_2529`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 3012, work left 3305.

## Stopped
