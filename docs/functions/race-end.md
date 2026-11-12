# Known functions: the race's end (the mode after a race)

The **race's end**: the one function that picks the game mode to go to
once a race is over (`RaceMode::ModeAfterRace` `sub_82150E00`), called
by the race mode's next-mode query (`RaceMode::NextMode` `sub_82151EF0`,
[`race-loading.md`](race-loading.md)) every tick once the race mode's
+848 machine is unloaded (state 35); `NextMode` copies its 8-byte answer
(byte +0 1 when a mode is asked for, word +4 its type, made by
`Mode::Create`, [`game-modes.md`](game-modes.md)) into its own. It reads
the race data `[0x826C5B2C]` (`../known-data.md`): what kind of race
it was (+0xA3AF0) and the flags (+0x59AE4: 0x8 an event race and 0x10 a
rival race, set by `Race::SetUp`, [`race-setup.md`](race-setup.md);
0x8000 and 0x10000 the race mode's +32 slot 0's hold), and for a career
race (kind 1) asks the career's progress what the race unlocked
(`Progress::AfterRace` `sub_82116588`, [`career.md`](career.md)): the
career goes on to today's result (type 9,
[`todays-result.md`](todays-result.md)), the parking area (type 5,
[`location-screen.md`](location-screen.md)) or the career's garage
(type 2), or, at the two stages that end with movies (6 and 8), to the
boot mode (type 3, [`boot-mode.md`](boot-mode.md)) with two movies
queued in its table and type 9 after them. Other kinds go to their own
front-end modes (6, 7, 8, 10, 12) or the title (4). Its callees are all
in other files: `SaveData::Of` (`vehicle-accessors.md`),
`Progress::AfterRace` and `Progress::CheckUnlocks` `sub_821162B0` and
`Career::EventLocation` (`career.md`), `ScreenFades::FadeIn`
(`mode-tick.md`) and `Session::Free` (`network-mode.md`). Mapped by the
B7 loop's mapping agent, 2026-11-12; ready by `b7_groups.py` with its
caller (`RaceMode::NextMode`, 2 functions, 377 lines; one callee,
`Progress::CheckUnlocks`, never seen in play).

Status:

- **Native**: plain C++ in `itc_recomp/src/` that doesn't use the
  PowerPC context (`ctx`): normal arguments and host structs (it may
  still read and write guest memory through `src/guest.h` while
  recompiled code shares the data). The game reaches it without `ctx`,
  from native or overridden callers, and the recompiled original is no
  longer called (except behind a fallback flag or to check against it).
  No `REX_FUNC` override of it exists, not even an adapter kept only for
  a harness (that's Overridden until the adapter goes).
  The end state for every function.
- **Overridden**: an `extern "C" REX_FUNC(sub_XXXXXXXX)` override in
  `itc_recomp/src/` (or a macro defining one) does the work instead of
  the original: a thin adapter in front of native code, or a body that
  still uses `ctx` (calling guest code, reading registers). It may call
  the original when its flag is off, or to check itself against it.
- **Wrapped**: an override that does something of its own (skipping
  render-only passes, logging, timing) and still calls the original
  (`__imp__sub_XXXXXXXX`) for the work.
- **Dropped**: its work isn't needed on PC (hardware set-up, kernel calls
  the SDK already stubbed), so it no longer runs: an override in
  `itc_recomp/src/` replaces it with nothing (or a fixed return value), or
  its callers, made native, leave the call out (no override then; the row
  says which caller). Logs, a harness or the renderer's frame dumps have
  shown that nothing reads what it did. The original is no longer called,
  except behind a fallback flag while the drop is being checked. A
  function the game never calls in the first place isn't Dropped: its
  status stays blank and its row says it's never called.
- **Merged**: its work is done inside a native caller's plain C++ (a
  few lines only that caller needs), so it has no function or override of
  its own in `itc_recomp/src/`. The row names the caller(s) and the
  harness that covers the work. The original is no longer called, except
  behind the caller's fallback flag.
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

A function usually moves from recompiled or Wrapped to Overridden to
Native as the `ctx` edge moves up through its callers, or straight to
Dropped when its work isn't needed.
`itc_recomp/tools/check_function_statuses.py` checks this column, in
every file of the tables, against `src/` (an override marked Native, a
status with no override, an override with no row, a row in two files);
run it after changing either, on any branch.


| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82150E00` | `RaceMode::ModeAfterRace` | Native | r3 = the 8-byte answer (a temporary on `RaceMode::NextMode`'s frame, +80), r4 = the race mode (not read). A 144-byte frame (`__savegprlr_26`). Returns r3. The answer cleared (byte +0, word +4); every answer below is built in the frame's +80 to +87 (byte +80 = 1, word +84 the type; bytes +81 to +83 never written, the stack's) and stored with one doubleword: the precedent of `BootMode::NextMode` (`boot-mode.md`) for its native copy. By the race data `[0x826C5B2C]` (`R`): flags `R` +0x59AE4 with 0x8000 → 4 (the title), `[0x826364EC]` = 1; else with 0x10000 → `Session::Free` (`sub_82292710`), 12. Else by the kind `R` +0xA3AF0: 0 → `ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0)` (black, layer 0), then 4 with `[0x826364EC]` = 1 as above; 2 → 6; 3 → 7; 4 → 8 (type 8 never made in play); 5 → 12 when `R` +0xA3AF4 is 7 and the session `[0x82816E3C]` isn't 0, else 10; 6 → 12. Any other kind (1, the career): `save = SaveData::Of(0)`; `R` +0xA3AF4 0 or 1 → 2. Else `stage = Progress::AfterRace(save)`: **6**: with unlocks to show (save +0x1CA78 not 0): flag 0x8 → 9 (the flag cleared), else flag 0x10 → the flag cleared, `Career::EventLocation(save)`, 5, else the answer stays clear; then save +0x1D524 = 6. Without unlocks: the boot movies' table `0x826C5BAC` = {6, 7, 11, 11, 11}, its +20 (`0x826C5BC0`, the type after them) = 9, +24 = 1, +28 = 0; 3; `ScreenFades::FadeIn` as above; `Progress::CheckUnlocks(save)`. **8**: as 6 without unlocks, the table {8, 9, 11, 11, 11}. **Another stage** (2 to 5, 7): save +0x1D524 = the stage; with unlocks: flag 0x8 → 9 (cleared), else flag 0x10 → cleared, `Career::EventLocation`, 5, else clear; without: 9. **-1** (nothing reached): flag 0x8 → cleared; then when the kind is 1 and `SaveData::Of(0)` +6100 (the event) is 1 → 2, else 9; flag 0x8 clear: flag 0x10 → cleared, `Career::EventLocation`, 5; else 9. Posts no messages and touches no machines. 524 lines (286 by `b7_groups.py`'s count). Read in full. Seen in play (every career race's end, `../loops/b7-progress.md`). Ready with its caller `RaceMode::NextMode` (`race-loading.md`, which it unblocks); its callees are ours (`SaveData::Of`, `Progress::AfterRace`, `Career::EventLocation`, `ScreenFades::FadeIn`, `Session::Free` Overridden) but `Progress::CheckUnlocks` (`career.md`, never seen in play, recompiled: a guest call). Also (read again for the port): in the -1 case the day test is `+6100` above 999 → 9, then 1 → 2. Overridden (B7 loop, 2026-11-12): plain C++ `race_end::ModeAfterRace` in `itc_recomp/src/race_end/race_end.cpp` behind an adapter in `src/hooks/race_end.cpp` (`--native_race_end`); every callee's plain C++ called directly (`SaveData::Of`, `Progress::AfterRace` and `Progress::CheckUnlocks` where `Condition::Test`'s copies fit, else their guest calls, `Career::EventLocation` with the race event's tests through the guest, `ScreenFades::FadeIn`, `Session::Free`); the answer built in the original's frame and copied (`BootMode::NextMode`'s precedent, bytes +81 to +83 the stack's); `Progress::CheckUnlocks` ported with it (the user's call, 2026-11-12: as little left recompiled as possible). Checked by the `race_end` harness (the race data, save data, data tables, race event and session built by the harness; every kind, flag and stage). Not run in an attract race (no race ends). Native (B7 loop, 2026-11-12): its adapter removed in the same run, its only caller `RaceMode::NextMode` calling its plain C++; `--native_race_end` now switches only `NextMode`; harness `race_end` through the register step. |
