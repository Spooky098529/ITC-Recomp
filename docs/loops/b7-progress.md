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
  (the sends, the leaderboard read). Validated (`itc_5457`, race 37 s
  at 180, 0 errors, every harness 0 differing outside the expected
  groups; replay `itc_5458` ok): 101 of 101 harness lines, no prints
  added; none ran in the race or the replay: 101 to drive. The
  user's calls (2026-11-14): the network battle's sends `sub_8214AB48`
  and `sub_8214AA70` Dropped (sends on the system link socket B5 no
  longer makes, results unread: the 2026-11-12 Live call); the
  leaderboard read `sub_82292CC0` answered natively as no Live (failed,
  +180 = 4, no buffer; today's stub leaves a garbage-size buffer).
  Step: second migration (`SplitResults::Draw`, the read).
- The user's call (2026-11-14): no multiplayer. Selecting Xbox Live
  or split screen on the main menu does nothing; those modes (types
  12 and 8) and the race's kind 4 and kind 6 screens are bypassed, not
  ported. The second migration (Live work), asked to stop, had
  finished: ea049be0..589a55a1 (`SplitResults::Draw` with the sends
  Dropped, `Leaderboard::Read` answering no Live with `SetUpSpec`,
  `Kind6Menu::SaveReplay` with `RaceDialog::AskSlot`, `StoreReplay`,
  `Replay::CopyFrom`; 3 adapters removed); every harness 0 differing
  but the read's expected group (20 of 20); races 21-28 s at 40, 0
  errors; nothing uncommitted. Its validation skipped (the user's
  call, 2026-11-14: straight to the bypass; its 10 functions are
  Live or split screen work the bypass makes unreachable). The bypass
  (a lead without mapping), b77b4b39..4499727c: `TitleScreen::MainMenu`
  items 3 (Xbox Live) and 4 (split screen) play the refusal cue and
  stop, behind `--multiplayer_menu_items` (off by default);
  `Mode::Create` hands type 12 to the original without the flag;
  harnesses 0 differing outside the two bypass groups; race 34 s at
  40, 0 errors; a synthetic pad replay checked both items, flag off
  and on. Static map: reachable 11,617 -> 11,011, unported 3,703 ->
  3,232 (never seen 3,059 -> 2,588). The user's answers: the items
  silent (c66d07b1), not greyed; `Mode::Create`'s guard kept; the race
  HUD's kinds 4 and 6 under the flag (23d07ac8). The static map's six
  gaps in reading our C++ fixed (18fa8e23): type 8 no longer
  reachable. Subsystem 1 finished and recorded.
- Subsystem 2 of 10: the C runtime's per-thread data and critical
  section wrappers (`825A`-`825B`, the static map's top blockers and
  frontier clusters, unmapped; ported since B7's goal). Mapped
  552e90f8: `c-runtime-core.md`, 118 rows (1 moved, 117 new; 21 seen
  in play), 56 ready groups; only `_cinit` `sub_822979E0` blocked (the
  static initializers, a lead); 2 leads added; check passed. The
  never-seen paths ending in `KeBugCheck` and `_CxxThrowException`'s
  `RtlRaiseException` are imports called as the original does (B7's
  goal). Migrated 67da37a3..4708b65b (5 batches, 2 removal commits):
  43 rows ours (`_mtinit`, `_getptd_noexit`, the locks' set-up,
  `errno`, the thread notify routines, `_ioinit`, `_initstdio`,
  `CloseHandle` and the rest), 7 adapters removed,
  `--native_c_runtime_core`; every harness 0 differing, races 38 s at
  40, 0 errors. Left: functions a harness can't run on its thread
  (the `KeBugCheck` stub `sub_825AA1A0`, `_CxxThrowException`,
  `RaiseException`, `XapiThreadStartup`, `_invalid_parameter`'s trap;
  they block `_getptd` and `_amsg_exit`). The user's call: ported and
  checked by reading, no harness (in "Precedent first"). Second
  migration b66014dd..cade1fbb: 27 rows ours (`_getptd`, `_amsg_exit`,
  `XapiThreadStartup`, the raisers, the file handle locks, the
  funclets, `exp`, `_fassign_l`), `CloseHandle`'s adapter removed;
  every harness 0 differing, races 38 s, 0 errors; 6 checked by
  reading (no harness). Ready groups left (`_close`, `_commit`,
  `_lseeki64_nolock`, `_fileno`, `terminate` and the rest). Step:
  validation of both migrations, then a third. Validation `itc_5492`:
  65 of 65 harness lines, replay ok, but `SaveSlot::WaitClose` (4 of
  32) and `LoadStep` (2 of 96) differed: `CloseHandle`'s dead saves
  below sp (its back chain and lr), now native without a guest frame;
  a harness fault by the 2026-11-10 precedent, masked 9bfdc4b1 (logged
  first). Validated again `itc_5495` (race 35 s at 180, 0 errors,
  every harness 0 differing outside the expected groups, 65 of 65).
  Third migration eb65b4c4..26c6db34: 22 rows ours (`_close`,
  `_commit`, `_lseeki64`, `_write`, `_flush`, `_fileno`, `terminate`
  by reading, the funclets, `SetFilePointer` and `WriteFile` Dropped
  -> Native), `_close_nolock`'s adapter removed; every harness 0
  differing, race 38 s, 0 errors. Left: fclose, `_fcloseall`, the
  stream buffer helpers, `bsearch`, and `_fflush_nolock`/flsall's pair
  with what's above it. Validated `itc_5504` (race 36 s at 180, 0
  errors, every harness 0 differing outside the expected groups), 21
  of 21; replay `itc_5505` ok; none ran in game: 21 to drive. Step:
  fourth migration.
- The static map (`static_map.py`, 620fd294, 2eeab044; ours' edges
  from our C++ in `src/`): reachable 11,620, ours 6,310, unported
  3,711 (644 seen in play, 3,067 never seen), frontier 1,917. Leads
  are picked from it once this subsystem is done.

## Done

- The in-race screens never seen (`race-screens-unseen.md`): 99 rows
  ours, `--native_race_screens_unseen`, d71b917e..23d07ac8.

## Rows to review

`review.md`, "Run 2026-11-14 05:01".

## Expected to differ

`expected-differences.md`.

## To check in a drive

`to-drive.md`.

## Totals

- At the start: ours 6537, work left 676 (wrapped 51, known 354,
  unknown 271).
- After the in-race screens: ours 6651, work left 676; static map
  unported 3,227 (644 seen, 2,583 never seen).

## Stopped
