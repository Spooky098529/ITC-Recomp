# B7 loop progress

Kept by the loop's lead (`b7-loop.md`). Short: details are in the
commits, the function files and the subsystems' docs. Earlier runs'
records: the `b7-loop-2026-*.md` summaries beside this file, and this
file's history. What's left to see in a drive, from every run:
`to-drive.md`.

## Now

- Run started 2026-11-14 on `b7-loop-13-11-26` (the previous run's
  branch, the user's call), from c6de612c; summary
  `b7-loop-2026-11-14-0501.md`. Limit: 10 subsystems (the user's call
  for this run).
- Dropped from `leads.md`: `sub_82562FF0` (inside the movie library's
  range `82481A70`-`82575B07`, reached only through a pointer, no
  direct caller in the game's own code: not mapped, "Rules that still
  hold").
- The user's calls (2026-11-14, asked by the lead): the worker pools
  follow XAudio's call (the kernel imports called as the original
  does, host objects once every user is native); `Device::Clear`'s
  worker by the graphics rule (native, packets Dropped, device state
  kept). Added to "Precedent first".
- Subsystem 1 of 10: the four in-race screens never seen in play
  (`sub_82211390` and the rest, the lead's line deleted). Mapped
  d71b917e: `race-screens-unseen.md`, 99 rows (all new, none seen in
  play), 11 ready groups; `SplitResults::Draw` and `Kind6Menu::Update`
  blocked (the network battle's sends, the network manager, the race
  dialog's never-seen save calls); 1 lead added; check passed. Kind 6
  is the Xbox Live time attack's race (the 2026-11-12 Live call
  followed). Migrated fdd8b61e..55b1f75c (4 batches, forked
  subagents one at a time): PauseMenu4, RaceResults, SplitResults
  (Draw blocked by the network battle's sends), Kind6Menu with the
  network manager's `Leaderboard::Rank`, `CarLogo`,
  `Network::SetLiveState`, and the blockers `RaceDialog::StartSave`,
  `ShowSaving`, `SetNote`, `UiMovie::SetNodeCharText`; 2 adapters
  removed; `--native_race_screens_unseen`; every harness 0 differing,
  races 30/29/26/24 s at 40, 0 errors. Two questions for the user
  (the sends, the leaderboard read). Step: validation.

## Done

## Rows to review

`review.md`, "Run 2026-11-14 05:01".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 6537, work left 676 (wrapped 51, known 354,
  unknown 271).

## Stopped
