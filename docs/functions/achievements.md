# Known functions: achievements

The game's **achievements**: after a race, which of its 27 achievements
the race earned, and the write of those to the system
(`XUserWriteAchievements`). An Xbox feature: mapped here, and by the
user's call (2026-11-12, the Xbox Live mode's precedent, "Precedent
first" in `../loops/b7-loop.md`) its Xbox call is to be answered with
plugged-in data or skipped where its result isn't read (Dropped), never
ported as a stand-in. The check (`Achievements::Check` `sub_82290788`)
runs from two callers: `RaceStates::OnRaceEnd` (`sub_821644D8`,
[`race-states.md`](race-states.md)), with kind 0 after a career race
(the race data's +0xA3AF4 2 and 3, after `Stats::AddRaceResult`) and
kind 1 after the race kinds 0, 4 and 7 (the time attacks, probably),
and the type 12 mode's network battle screen (`sub_82264B90`, the
handler (4, 1) of `Type12Screen9::InitBase`,
[`network-mode.md`](network-mode.md); its rows a lead in
`../loops/leads.md`), with kind 2 after a network battle won
(probably: its +9672 below +9668, `[0x826C6E14]` 0). Both call
`Achievements::Write` (`sub_82290A30`) with the check's 27 flags when
it answers 1. In play the check ran (`seen_in_play.txt`) but never
answered 1: the write, the course times' look-up and the Xbox call
never ran.

The 27 achievements are a table in `.rdata` at `0x82011FB8`
(`../known-data.md`): 27 pairs {kind, id}, ids 2 to 28 in order:

- Kind 0, ids 2 to 16, by items beaten (an item's wins, the halfword at
  `SaveData::Of(0)` + 0x199FA + 12 × item, nonzero; the items probably
  the career's rivals, `career.md`, `race-event.md`): id 2 item 376, 3
  item 377, 4 item 378, 5 item 381, 6 item 379, 7 item 380, 8 item 382,
  9 all of items 383 to 391, 10 item 391, 11 item 396, 12 item 392, 13
  all of 393 to 395, 14 item 397, 15 item 398, 16 item 399.
- Kind 1, ids 17 to 24, by course time: id 17 + n for the route n (0
  to 7) when the race's route (the progress's +6076, `[0x826D82E8]`) is
  n and the player's time (the progress's +2204, the results record's
  first entry's time) is below the route's target (the table at
  `0x82012090`: routes 0 and 1 5:00.000, 2 and 3 6:00.000, 4 3:30.000,
  5 6:00.000, 6 and 7 16:30.000).
- Kind 2, ids 25 to 28, by a count of the player's: the word at
  `0x826C7670` + 364 + 4 × the player's signed-in slot
  (`Players::SignedInSlot(0)`, `mode-tick.md`), plus 1, at least 1, 10,
  50 and 100 (the table at `0x820120D0`). The user table at
  `0x826C7670` is `Network::Update`'s (`system-services.md`); who writes
  its +364 words wasn't found (probably the network battles' wins, from
  the profile or Live). Not read further.

Shared with other files: `XUserWriteAchievements` (`sub_82294E58`, the
XAM wrapper, in [`system-services.md`](system-services.md)),
`SaveData::Of` (`sub_82105620`, [`vehicle-accessors.md`](vehicle-accessors.md)),
`Players::SignedInSlot` (`sub_8209F658`, [`mode-tick.md`](mode-tick.md)).
`sub_820FF708` (`Time::Less`) has its row here, also called by
`Time::Clamp` (`sub_820FF790`, [`race-states.md`](race-states.md)) and
`sub_82105EA0` (a course record, from `RaceStates::OnRaceEnd`; no row,
never ran). Not here: `sub_82290AB8`, after `Achievements::Write`, a
state check of the other UI movies' word `[0x82816600]`
([`data-load.md`](data-load.md)) and `[0x82638128]`, called only by
three of the sign-in watch's handlers (`sub_8209E610`, `sub_8209E698`,
`sub_8209ECC8`, never run, without rows: `mode-tick.md`); never ran.
Mapped 2026-11-13 by the B7 loop's mapping agent from the lead
"probably which achievements were earned"; every row below was read in
full in `generated/` unless it says otherwise.

Made native 2026-11-13 by the B7 loop (`src/achievements/`, adapters
`src/hooks/achievements.cpp`, `--native_achievements`, on by default;
harness `achievements`): all five rows, the three never seen in play too
(the user's call, 2026-11-12). By the user's later call (2026-11-13),
`Achievements::Write` keeps calling `XUserWriteAchievements` through the
guest as the original does (not skipped), so the SDK goes on recording
unlocks; that replaces the "skipped where its result isn't read" above for
this call. The harness hands the wrapper's calls to a recorder (its Wrapped
override), so a case never unlocks anything. `Check`'s adapter stays (its
callers are recompiled), as does `Time::Less`'s (`Time::Clamp`,
`sub_82105EA0`).

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
| `sub_82290788` | `Achievements::Check` | Overridden | r3 = the kind (0, 1 or 2), r4 = 27 words out (the caller's frame: `RaceStates::OnRaceEnd`'s +112, `sub_82264B90`'s +160). For each of the 27 pairs at `0x82011FB8` (see above), its word out = 0, then, when the pair's kind is r3, by its id (a jump table over id − 2 at `0x820120F8`, 0 to 26): ids 2 to 8, 10 to 12, 14 to 16 one item (a second jump table at `0x820120E8` picks it: 376, 377, 378, 381, 379, 380, 382, 391, 396, 392, 397, 398, 399, stored in its frame's +80), id 9 the nine items 383 to 391 (frame +112 to +144), id 13 the three items 393 to 395 (frame +88 to +96): the word = `Achievements::AllBeaten`(the list, its count); ids 17 to 24 (a third table at `0x820120E0` gives n = id − 17): when the progress's (`[0x826D82E8]`) +6076 is n, the word = `Achievements::BeatsCourseTime`(progress + 6076, progress + 2204), else stays 0; ids 25 to 28: `Players::SignedInSlot(0)` (`sub_8209F658`, `mode-tick.md`), and when it's not negative, the word = 1 when `[0x826C77DC + 4 × slot]` + 1 is at least the id's threshold (1, 10, 50, 100: the word at `0x82012074` + 4 × the pair's index, the table at `0x820120D0` for indexes 23 to 26), else stays 0. Returns 1 when any word is 1, else 0. Its frame's +80 is loaded into r29 at the start (a stack leftover), overwritten before every use. 362 lines with `AllBeaten` (`b7_groups.py`). From `RaceStates::OnRaceEnd` (`sub_821644D8`, `race-states.md`: kind 0 after a career race, 1 after the race kinds 0, 4 and 7) and `sub_82264B90` (the type 12 mode's network battle screen, kind 2; a lead); both call `Achievements::Write` with r4's words when it answers 1. Seen in play (it never answered 1: `Write` never ran). No Xbox call: game data only. Ready (`b7_groups.py`, with `AllBeaten`; `BeatsCourseTime` never ran). Overridden since 2026-11-13 (`src/achievements/achievements.cpp`, adapter `src/hooks/achievements.cpp`, `--native_achievements`; harness `achievements`). The item lists stored in its frame as the original does; `AllBeaten`, `BeatsCourseTime`, `Players::SignedInSlot` and `SaveData::Of` called as plain C++. The adapter stays: both callers are recompiled. |
| `sub_82290698` | `Achievements::AllBeaten` | Overridden | r3 = a list of item ids (words), r4 = its count: 1 when every item's wins (the halfword at `SaveData::Of(0)` + 0x199FA + 12 × id: item record +2, `career.md`) are nonzero (or the count is 0), else 0 (stops at the first zero). Calls `SaveData::Of(0)` (`sub_82105620`, `vehicle-accessors.md`) first, always. Leaf otherwise. Only caller `Achievements::Check`. Seen in play. Overridden since 2026-11-13 (`src/achievements/achievements.cpp`, adapter `src/hooks/achievements.cpp`, `--native_achievements`; harness `achievements`). Its back chain stored; native `Check` calls its plain C++. |
| `sub_82290710` | `Achievements::BeatsCourseTime` | Overridden | r3 = &a route number (the progress's +6076), r4 = &a packed time (the progress's +2204: the results record's first entry's time, the player's): looks the route up in the 8 pairs {route, packed time} at `0x82012090` (routes 0 to 7: 5:00.000, 5:00.000, 6:00.000, 6:00.000, 3:30.000, 6:00.000, 16:30.000, 16:30.000); found, 1 when `Time::Less`(r4, &the pair's time) (the time below the target), else 0; not found 0. Only caller `Achievements::Check` (ids 17 to 24). Not seen in play. Overridden since 2026-11-13 (`src/achievements/achievements.cpp`, adapter `src/hooks/achievements.cpp`, `--native_achievements`; harness `achievements`). Ported though never seen in play (the user's call, 2026-11-12); `Time::Less` as plain C++. |
| `sub_820FF708` | `Time::Less` (probably) | Overridden | r3, r4 = &two packed times (minutes in bits 16-30, seconds 10-15, thousandths 0-9, bit 31 the sign: negative when set): 1 when the first, in signed thousandths ((minutes × 60 + seconds) × 1000 + thousandths, negated by the sign), is below the second, else 0 (a byte). Leaf. From `Achievements::BeatsCourseTime`, `Time::Clamp` (`sub_820FF790`, `race-states.md`) and `sub_82105EA0` (a course record, `RaceStates::OnRaceEnd`'s, no row). Not seen in play. Overridden since 2026-11-13 (`src/achievements/achievements.cpp`, adapter `src/hooks/achievements.cpp`, `--native_achievements`; harness `achievements`). Ported though never seen in play (the user's call, 2026-11-12). The adapter stays for `Time::Clamp` and `sub_82105EA0` (recompiled). |
| `sub_82290A30` | `Achievements::Write` | Overridden | r3 = the 27 words `Achievements::Check` filled. For each index i whose word is nonzero: the 8-byte entry at `0x828164F0` + 8i gets +0 = `[0x826BABA4]` (player 0's controller's user, `Players::`) and +4 = the pair's id (`0x82011FBC` + 8i, 2 to 28), then `XUserWriteAchievements`(1, the entry, the overlapped at `0x828161F8` + 28i) (`sub_82294E58`, `system-services.md`: `XMsgStartIORequest(0xFB, 0xB0008, overlapped, {count, list} on its frame at +80, 8)`, and with an overlapped answers 997, or 1627 when the request fails). Returns nothing; reads nothing back (the wrapper's answer is dropped, and nothing in `generated/` reads the overlappeds or the entries again: their addresses are formed only here). From `RaceStates::OnRaceEnd` and `sub_82264B90`, each when `Check` answers 1. Not seen in play. The Xbox call: what the SDK does today (`rexglue-sdk`'s `xam_msg.cpp` and `xgi_app.cpp`): `XMsgStartIORequest` dispatches the message to the XGI app, which for `0xB0008` logs and, for each entry, calls `KernelState::UnlockAchievement`(the id at +4) (the SDK's achievement manager: the achievements registered from the XEX's XDBF, an unknown id ignored with a warning; an unlock is saved to `achievements/<title id>.toml` under the user data root and its unlock and notification callbacks called; the app's notification dialog isn't made, `itc_app.h` has it commented out), answers 0; the overlapped is completed at once with 0 (`CompleteOverlappedImmediate`) and the call answers 997 (`ERROR_IO_PENDING`). For the migration: the entries' writes kept (guest memory, as the original leaves it), the wrapper call skipped (Dropped by the user's call: its answer and the overlapped aren't read, and nothing the game shows or does changes); left: the SDK's own unlock record (the `.toml` above), which the game never reads, would no longer be written (a host side effect, for the user's note). Overridden since 2026-11-13 (`src/achievements/achievements.cpp`, adapter `src/hooks/achievements.cpp`, `--native_achievements`; harness `achievements`). Ported though never seen in play (the user's call, 2026-11-12). By the user's call (2026-11-13, "Precedent first"), the entries are written as the original does and `XUserWriteAchievements` is still called through the guest (the function table, r1 its 144-byte frame, lr `0x82290A98`), not skipped, so the SDK keeps recording unlocks: the migration note above (the call Dropped) is superseded. r3 left as the original leaves it (the last call's answer, else the words' address). In the harness the wrapper's calls go to a recorder (its Wrapped override, `system-services.md`), so no case unlocks anything. |
