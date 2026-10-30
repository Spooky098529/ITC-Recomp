# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-10-30 01:06 on `b7-loop-29-10-26-2` (the user's
  call, 2026-10-30: this run stays on the last run's branch), from
  92ffb30; summary `b7-loop-2026-10-30-0106.md`.
- Limit for this run: 3 subsystems; 0 taken.
- First, a cleanup lead without mapping (not counted), the last run's
  stop: `CarSound::Part840Update`'s harness (`sub_8210ECF0`,
  `car-sounds.md`), 1 of 80 cases differing once in `itc_2559`
  (intermittent). The precedent: this run's and the 2026-10-24 21:33
  run's first leads were the previous stop's harness. Migration
  checked (d5cfd5c: the mixer swaps XAudio's two source-voice stage
  lists between a case's versions; the real-voice cases compare them
  by role, as the XAudio harnesses' `LockWords`; 0 of 80 over 5 runs,
  `itc_2569`-`itc_2573`; nothing else ready in `car-sounds.md`).
  Validated (`itc_2574`, `itc_2575`): 56 of 56 harness lines, every
  harness 0 outside the expected groups; `CarSounds::StopAll` already
  on the drive list.
- Subsystem 1 of 3: the app's start-up (`sub_82106548`, the top of
  `leads.md`). Mapping agent running.

## Done

- The last stop's `CarSound::Part840Update` harness (cleanup lead, not
  counted): d5cfd5c, a harness fix only (the stage lists compared by
  role); validated (`itc_2574`, `itc_2575`).

## Rows to review

`review.md`, "Run 2026-10-30 01:06".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 3031, work left 3287 (wrapped 104, known 501,
  unknown 2682).
- After the cleanup's migration (`itc_2573`: race 38 s, 0 errors,
  car_sounds 0 differing): ours 3031, work left 3287.
- After its validation (`itc_2574`: race 7 s, 0 errors, every harness
  0 differing but the expected groups): ours 3031, work left 3287.
