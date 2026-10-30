# Known functions: the career's progress

The player's career over the save data (`SaveData::Of(i)`,
`../known-data.md`; `82110400`-`82116D00`): what a race's result adds
to the stats record (save data +48: counts by mode and by opponent, the
win and loss streaks, the last 15 results' records, running means), the
12-byte records of the 400 items (save data +0x199F8 + 12 × id: the
halfwords +0, +2, +4 counted up to 999 by the race's outcome against
that item), the unlocks checked after a race (the conditions of the
data tables' 56-byte entries, table 0, and 40-byte entries, `DataTables::`
in `data-tables.md`; bits set in the save data, rewards granted by kind),
the stage reached (the bits of save +6092, `SaveData::FirstUnsetFlag` in
`race-world.md`), the garage's list (cars of 1,968 bytes from +6120,
`race-setup.md`'s garage walk), the day's advance (+6100, 7 a week) and
the set-up of a race outside `Race::SetUp` (`Career::SetUpEvent`
`sub_82114268`: the day's races picked from tables 1 to 7 at random,
then the race event made, `RaceEvent::Make` `sub_82144E58`, mapped
with the rest of the race event under its lead in `../loops/leads.md`).
Names are ours and mostly guesses. Read 2026-10-30 by the B7 loop's
mapping agent; the large unseen functions only skimmed.

Shared, with their rows where they are: `SaveData::Of` and
`RacerRecords::At`, `Stages::Type` (`vehicle-accessors.md`),
`DataTables::Item`, `Entry56`, `Entry40`, `IsLoading` and
`ItemRecords::Item` (`data-tables.md`), `SaveData::FirstUnsetFlag`
(`race-world.md`), `RunningMean::Add`, `RaceData::ResetGlobal`,
`Car::Init` and `Car::SetModel` (`race-setup.md`), `Params::GetCarSpec`
(`car-specs.md`), the save data's own rows (`save-data.md`, among them
`sub_821133A8` to `sub_82114128` in this range), `sub_82111AE0`,
`sub_82111B30` and `sub_821128E0` (`data-tables.md`), `PlayTime::Update`
(`save-interface.md`), `rand` (`race-overlays.md`), `memcpy`,
`memset` and `__chkstk` (`c-runtime.md`). Callers outside: the race's
end (`sub_82150E00`, from `sub_82151EF0`), the race states
(`RaceStates::OnRaceEnd` `sub_821644D8`, `sub_821634D8`,
`race-states.md`), and the front end (`sub_82180CE8`, `sub_82188CE8`,
`sub_82188D44`, the callers of `Race::SetUp`; `sub_82182E68`,
`sub_82221AC0`, `sub_82197278`, `sub_8225A520`, `sub_822280D8`,
`sub_82228138`, `sub_8222AE00`, `sub_821C0FF0`, `sub_8220F840`,
`sub_8216E150`); the new ones are in `../loops/leads.md`.

What a native port meets (read 2026-10-30): no kernel imports, kernel
objects or locks. The calls out are all ours or Dropped already:
`rand` (`sub_82293568`, `Career::PickNext` and `Career::SetUpEvent`),
`memcpy`, `memset`, `__chkstk` (Dropped; a native caller leaves it out),
the data tables' and the save data's accessors. Three functions branch
through a jump table in `.rdata` (`Condition::Test` at `0x82008130`,
ten kinds; `Rewards::Grant` at `0x82008150`, 42 kinds); the recompiler
split two of their cases into functions of their own (`sub_821129E0`,
`sub_82114C7C`), which a native port does inside its caller (Merged).
Float constants: `0x82000A34` (0.0), `0x82002A0C`, `0x820029D8`,
`0x820082A0`, `0x82001FBC`. Several functions copy a 3,008-byte item
onto their stack (`memcpy`) only to read a byte or two of it; B7 changes
no behavior (the user's call), so the copies' stack bytes are left as
the original leaves them. Blockers outside the file: the race event
(`RaceEvent::Make` `sub_82144E58` and `RaceEvent::Init` `sub_82144A50`
under `Career::SetUpEvent`, `sub_82144720` under
`Career::EventQuery`, `sub_82144740` under `Career::EventLocation`;
its lead) and `sub_821104D8` (the car object's makers' lead, under
`Garage::Add`; ready itself). Nothing here needs a call the user hasn't
made.

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82115190` | `Stats::AddRaceResult` | | r3 = a player's save data, r4 = the race data. The player's result record: `RacerRecords::At(race data + 0x59AE8, -1)` (the last of the results `RaceStates::OnRaceEnd` keeps, `race-states.md`). Only for the race data's +0xA3AF4 (probably the race mode) 2 or 3: in the stats record (save +48) the week's block's words +4900 = the record's +808, +4912 += +812, +4908 += +816; floats +4888 = max(+804) and +5976 = max(+804); `RunningMean::Merge(stats, record +796)` twice (`sub_82112148`); running means (`RunningMean::Add`, weight 1, then `RunningMean::Merge` into the save's) at save +5928 (+812), +5936 (+816), +5960 (float +820), +5968 (1.0 at `0x82002A0C` when +784 is nonzero, else 0.0); stats +5988 += +820; +5996 = max(+16); `Results::Push(save + 4996, race data)` (`sub_82111FC8`); then, when the current stage's type (`Stages::Type(race data + 0x59620, -1)`) is 0: save +5944's mean from +788 × `[0x820082A0]` (0.0 when +3920 is 3), stats +5980 and +5984 = max(+828 × `[0x820029D8]`). Then, same modes, for the four words at +8 + 972 × i (the opponents' item ids, -1 none), by the record's +3924 (probably the outcome): 1 `ItemRecords::AddWin` (and save +6048 = 1 when the item's byte +35 is 5, read from a 3,008-byte stack copy of the item), 3 `ItemRecords::AddOutcome3` (not seen in play), else `ItemRecords::AddLoss` and, when +3920 is 3, stats halfwords +18 and +5804 + 14 × mode (+8) up to 9,999. 508 lines, a 3,184-byte frame. From `RaceStates::OnRaceEnd` (`sub_821644D8`). Not read in detail (the record's fields' meanings are guesses). Blocked only by its own callees in this file. |
| `sub_82110850` | `ResultRecord::Fill` | | r3 = a 48-byte result record, r4 = the race data: from the last result (`RacerRecords::At(race data + 0x59AE8, -1)`): +0, +4 = its +812, +816; +8 = its 8 bytes at +796; +40 = word +60 of the race data's racer record (+48 + 3,936 × the result's +4); +16 = (+3924 == 1); +20 to +28 = the words at +980, +1952, +2924; +44 = halfword +968; +32 = +972; +36 = the race data's +32 (the mode). From `Results::Push`. Read in full. |
| `sub_82111FC8` | `Results::Push` | | r3 = the save's last results (save +4996, stats +4948: +0 a count, up to 15; +4 15 records of 48 bytes), r4 = the race data: the 14 records moved down one (`memcpy` of 48 bytes each, from the end), the newest at +4 by `ResultRecord::Fill`, the count up. From `Stats::AddRaceResult`. Read in full. |
| `sub_82112148` | `RunningMean::MergeTwo` | | r3 = the stats record, r4 = a packed {float mean, count} doubleword: `RunningMean::Merge(+4892, r4)` and `RunningMean::Merge(+5904, r4)`. From `Stats::AddRaceResult`. Read in full. |
| `sub_82111CB0` | `RunningMean::Merge` | Overridden | r3 = {+0 a float mean, +4 a count}, r4 = another as a doubleword (the float in the upper word, the count in the lower): unless the counts add to 0, count = the sum and mean = (mean × old count + r4's mean × its count) / sum (the counts converted to floats). Leaf. From `RunningMean::MergeTwo`, `Stats::CloseDay`, `Stats::AddRaceResult`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career` (the doubleword stored at the caller's r1 + 24 as the original; the conversions' red zone not written), the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82114B88` | `ItemRecords::AddWin` | | r3 = a player's save data, r4 = an item id: builds {id, the item's record (save +0x199F8 + 12 × id), the item (`ItemRecords::Item`)} on the stack, `Stats::AddWin(stats, that, save +6104 the mode)`, then the record's halfword +2 up by one, at most 999. From `Stats::AddRaceResult`. Read in full. |
| `sub_82114B08` | `ItemRecords::AddLoss` | | As `ItemRecords::AddWin` with `Stats::AddLoss` and the record's +0. From `Stats::AddRaceResult`. Read in full. |
| `sub_82114A88` | `ItemRecords::AddOutcome3` | | As `ItemRecords::AddWin` with `Stats::AddOutcome3` (`sub_82113B50`) and the record's +4. From `Stats::AddRaceResult` (outcome 3). Not seen in play. Read in full. |
| `sub_821139B8` | `Stats::AddWin` | | r3 = the stats record, r4 = {id, record, item}, r5 = the mode: `Counter::AddWin(stats)`; the win streak +5970 up to 9,999 and the best +5968 = max; the day block's (+24) streak +4828 up and its best +4830 = max; the loss streak +5972 = 0; `Counter::AddWin(stats +4864)` (the week's block) and its +52 up when the item's byte +35 is 1; `Opponents::AddWin(stats +24)`; `Counter::AddWin(stats +4924)`; by mode, halfwords +5796 + 14 × mode +2 and +10 up to 9,999. From `ItemRecords::AddWin`. Read in full. |
| `sub_82113AB8` | `Stats::AddLoss` | | As `Stats::AddWin` for a loss: `Counter::AddLoss(stats)`; the loss streak +5972 up, the win streaks +5970 and +4852 (the day block's) = 0; `WeekCounter::AddLoss(stats +4864)`; `Opponents::AddLoss(stats +24)`; `Counter::AddLoss(stats +4924)`; by mode +4 and +10. From `ItemRecords::AddLoss`. Read in full. |
| `sub_82113B50` | `Stats::AddOutcome3` | | As `Stats::AddLoss` for outcome 3 with `Counter::AddOutcome3`; all three streaks = 0; `Opponents::AddOutcome3`; by mode +6 and +10. From `ItemRecords::AddOutcome3`. Not seen in play. Read in full. |
| `sub_82111E10` | `Counter::AddWin` | | r3 = a 24-byte counter block (+0, +4, +8 bit sets by the item's byte +35; +12 wins, +14 losses, +16 outcome 3; +22 races; halfwords up to 9,999), r4 = {id, record, item}: +12 up; when the record's +2 is still 0 (the first win against it) and byte +35 isn't negative, bit +35 set at +0; +22 up. Leaf. From `Stats::AddWin`, `Opponents::AddWin`. Read in full. |
| `sub_82111E80` | `Counter::AddLoss` | | As `Counter::AddWin` with +14, the record's +0 and the bit set at +4. Leaf. From `WeekCounter::AddLoss`, `Opponents::AddLoss`, `Stats::AddLoss`. Read in full. |
| `sub_82111EF0` | `Counter::AddOutcome3` | | As `Counter::AddWin` with +16, the record's +4 and the bit set at +8. Leaf. From `Opponents::AddOutcome3`, `Stats::AddOutcome3`. Not seen in play. Read in full. |
| `sub_82111F60` | `WeekCounter::AddLoss` | | r3 = the week's block (stats +4864): `Counter::AddLoss`, then by the item's byte +35: 4 the halfword +56 up, 5 +58 (up to 9,999). From `Stats::AddLoss`. Read in full. |
| `sub_821134D0` | `Opponents::AddWin` | | r3 = the day block (stats +24: a 24-byte counter, then +24 a list of up to 400 item ids raced with its count at +1624, then +1628 + 8 × id three halfwords {losses, wins, outcome 3}): `Counter::AddWin`; +1630 + 8 × id up, at most 999; the id appended to the list when not in it. From `Stats::AddWin`. Read in full. |
| `sub_82113580` | `Opponents::AddLoss` | | As `Opponents::AddWin` with `Counter::AddLoss` and +1628 + 8 × id. From `Stats::AddLoss`. Read in full. |
| `sub_82113628` | `Opponents::AddOutcome3` | | As `Opponents::AddWin` with `Counter::AddOutcome3` and +1632 + 8 × id. From `Stats::AddOutcome3`. Not seen in play. Read in full. |
| `sub_821116E0` | `Career::AddRivalWin` (probably) | | r3 = a player's save data, r4 = an item id or -1 (nothing): `ItemRecords::Item` twice (answers unused), stats halfword +20 and +5808 + 14 × mode (+12) up to 9,999, the item's record +6 up to 999. From `sub_821634D8` (race states). Not seen in play. Read in full. |
| `sub_82116588` | `Progress::AfterRace` | | r3 = a player's save data; returns the stage just reached (2 to 8) or -1. The race data's (`[0x826C5B2C]`) words +0xA3AD0 to +0xA3ADC added to stats +5956, +5952, +5960 and +5964 (the last divided by 50 first). The location: save +6112 when it isn't 48; else +6108 when the race data's flags +0x59AE4 have 0x10 (a rival race, `Race::SetUp`), else 48 (the test of +6108 against 14 and 20 before it leads to the same either way). Each table-0 56-byte entry (`DataTables::Entry56(0, i)`, kind +0 not 4) not yet unlocked (bit i of save +0x1CA7C clear) whose `Entry56::Allows(entry, mode, location, Stage::Of, 1)` holds and whose three conditions at +24 (8 bytes each, -1 ends; any when +20 is 0, all when nonzero; none passes) pass `Condition::Test(c, 0, 0)`: bit i set at +0x1CA7C and {0, 0, i} appended to the 12-byte list at save +0x1C778 (count +0x1CA78). Each 40-byte entry (`DataTables::Entry40(i)`, +16 the conditions, -1 ends) not yet granted (bit i of +0x1D508) whose conditions pass `Condition::Test(c, 0, entry +0)` (any or all by +12): `Rewards::Grant(save, i, entry +4, entry +8)`. Then by `SaveData::FirstUnsetFlag` (0 to 5): the stage's test (`Items::AnyWonKind3Stage1`, `Items::AllWonKind3`, `Items::AnyWonKind3Stage2`, `Items::AllWonKind4Stage2`, `Items::AllWonKind4Stage3`, `Items::AllWonKind5`); when it holds, +6092's bit set and locations unlocked (`Locations::Unlock` bits of +0x199E0, `Locations::Unlock2` of +0x199E8; stage 0 bits 4, 19, 20 and 12, 18; 1 2, 3, 15 to 18 and 4 to 7, 16, 17; 2 5, 21, 22 and 13, 19). Last, when +6112 isn't 48, +6108 = +6112 and +6112 = 48. 778 lines. From `sub_82150E00` (the race's end) and `sub_82228068` (not seen). Read; the location's test not read in detail. |
| `sub_821162B0` | `Progress::CheckUnlocks` (probably) | | Probably `Progress::AfterRace`'s two unlock passes alone (the table-0 entries and the 40-byte rewards, with `Stage::Of`, `Condition::Test`, `Rewards::Grant`), without the race's totals or the stage. 420 lines. From `sub_82150E00`, `sub_8225A720`, `sub_8225B9B8`. Not seen in play. Not read in detail. |
| `sub_82111BA8` | `Entry56::Allows` | Overridden | r3 = a 56-byte data entry, r4 = the mode, r5 = a location, r6 = the stage (0 to 4), r7 = whether to test the location: 0 when +0 is 4; else its mask +4: with r7, the location's bit (bit 6 for 48, bits 0 to 5 for 14 to 19; other locations untested) must be set; when bits 7 to 9 are used, the mode's bit (7, 8, 9 for modes 0, 1, else; mode 3 untested); when bits 10 to 14 are used, the stage's (bit stage + 10; stage 4 tests bit 13, as 3 does). 1 only when a test ran and none failed. Leaf. From `Progress::AfterRace`, `Career::SetUpEvent`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82112950` | `Condition::Test` | | r3 = an 8-byte condition {+0 kind 0 to 9, +4 a value}, r4 = a location (-1 none, 1 to 7 a table's), r5 = a mask of item kinds (bits 9 + the item's halfword +42; tested only when one of bits 10 to 14 is set). Through the jump table at `0x82008130`: counts of items (over the data tables' count, `[0x826C5CA0]` +8; each copied to the stack, 3,008 bytes) with a nonzero record +0 and byte +35 of 2, of 3 to 5 (`sub_821129E0`) or any, at least +4; the record of item +4: its +2 nonzero, zero, or all three halfwords zero; the day of the week (save +6100 % 7) above or below +4; whether entry +4 is done or isn't (r4 1 to 7: the save's word +0x1BE78 + 4 × value, or +0x1C178 + 4 × value, is 2; r4 0: bit +4 of +0x1CA7C, unlocked; r4 -1 or above 7 fails). Returns 1 when it holds. 547 lines, a 3,168-byte frame. From `Progress::AfterRace`, `Progress::CheckUnlocks`, `Career::SetUpEvent`. Read; which kind number is which not worked out. |
| `sub_821129E0` | | | One of `Condition::Test`'s cases (the jump table's target, split off as a function by the recompiler; it uses its caller's frame and registers): the items whose byte +35 is 3, 4 or 5 counted. Only from `Condition::Test`. Read in full. A native port does it inside `Condition::Test` (Merged). |
| `sub_82114C08` | `Rewards::Grant` | | r3 = a player's save data, r4 = a 40-byte entry's index, r5 = a reward kind (0 to 41), r6 = a value: bit r4 set at +0x1D508 (granted); then by the jump table at `0x82008150` bits set in the unlock set at +0x19930 (the copy of `0x826C50A8`, `save-data.md`) at r6 plus offsets (0, 10, 20, 27, 34 and others), noting whether any was new; when one was, {kind, value, index} appended to the 12-byte list at +0x1CA84 (count +0x1D504). 911 lines. From `Progress::AfterRace`, `Progress::CheckUnlocks`. Read; the 42 kinds not read one by one. |
| `sub_82114C7C` | | | One of `Rewards::Grant`'s cases split off by the recompiler (bit r6 and r6 + 10 of +0x19930). Only from `Rewards::Grant`. Not seen in play. Read in full. A native port does it inside `Rewards::Grant` (Merged). |
| `sub_821121F8` | `Stage::Of` | Overridden | r3 = a player's save data: `SaveData::FirstUnsetFlag` mapped 0, 1 to 0; 2, 3 to 1; 4 to 2; 5 to 3; 6 to 4. From `Career::SetUpEvent`, `Progress::AfterRace`, `Progress::CheckUnlocks`, `sub_82156CC0` (`game-modes.md`), `sub_821A0228` (front end). Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82111780` | `Items::AnyWonKind3Stage1` | Overridden | No arguments: whether any of the 400 items with halfword +42 = 1, byte +36 = 0, byte +35 = 3 has a nonzero win count (player 0's record +2). From `Progress::AfterRace` (stage 0's test). Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82111818` | `Items::AnyWonKind3Stage2` | | As `Items::AnyWonKind3Stage1` with +42 = 2, +36 = 1. From `Progress::AfterRace` (stage 2). Not seen in play. Read in full. |
| `sub_821118B0` | `Items::AllWonKind3` | | No arguments: 0 when any item with +42 = 1 and +35 = 3 has no win, else 1. From `Progress::AfterRace` (stage 1). Not seen in play. Read in full. |
| `sub_82111940` | `Items::AllWonKind4Stage2` | | As `Items::AllWonKind3` with +42 = 2, +35 = 4. From `Progress::AfterRace` (stage 3). Not seen in play. Read in full. |
| `sub_821119D0` | `Items::AllWonKind4Stage3` | | As `Items::AllWonKind3` with +42 = 3, +35 = 4. From `Progress::AfterRace` (stage 4). Not seen in play. Read in full. |
| `sub_82111A60` | `Items::AllWonKind5` | | As `Items::AllWonKind3` with only +35 = 5. From `Progress::AfterRace` (stage 5). Not seen in play. Read in full. |
| `sub_82110B40` | `Items::AllWonKind5Or4` | | r3 = a player's save data: 0 when +6048 is set; else 0 when any item whose byte +35 is 5, or 4 with +42 = 4, has no win (each item copied to the stack first), else 1. From `sub_82112260`. Not seen in play. Read in full. |
| `sub_82111D20` | `Locations::Unlock` | | r3 = a player's save data, r4 = a bit: sets it in +0x199E0 (the race data's +20, `Race::SetUp`) when clear. Leaf. From `Progress::AfterRace`. Not seen in play. Read in full. |
| `sub_82111D98` | `Locations::Unlock2` | | As `Locations::Unlock` for +0x199E8. From `Progress::AfterRace`. Not seen in play. Read in full. |
| `sub_82114268` | `Career::SetUpEvent` | | r3 = a player's save data, r4 = the mode, r5 = the location. A bit set of locations 14 to 19 on the stack; save +6104 = r4, +6108 = r5, +6112 = 48; `RaceData::ResetGlobal`. Unless the day (+6100) is 1 (and at most 999): for a location 14 to 19, the table-0 unlocks as in `Progress::AfterRace`; then for tables 1 to 7 (`Entry56(t, i)`; tables 2 to 7 against the save's words at +0x1B278 and +0x1B578 + 768 × (t - 2), 2 skipping kinds 0 to 2 and 4) the entries `Entry56::Allows` and `Condition::Test` accept collected per location and kind; then for each of the 6 locations, up to 8 of each kind's entries picked at random (`rand`, without repeats; at most 2, 4, 2, 8 by kind) into the location's offers at save +0x1AD20 + 100 × location (12-byte entries {0, table, index}, count at +96). Last, `RaceEvent::Make(&r4, &r5, the stack bit set)` (`sub_82144E58`, the race event's lead). 1,158 lines, a ~120 KB frame (`__chkstk`). From `sub_82180CE8`, `sub_82188CE8`, `sub_82188D44` (the callers of `Race::SetUp`). Read; the tables' kinds and the offers' use not worked out. Blocked by the race event (`RaceEvent::Make`'s tree; b7_groups counts its functions in this group, as only this calls it: over 1,500 lines with them, so split the migration). |
| `sub_82111638` | `Career::EventQuery` | | r3 not read, r4: nonzero gives -1; else a tail call to `sub_82144720` (the race event, `[0x826C6078]`: `sub_82144300` on it, or 0 without one). From `sub_822280D8`, `sub_82228138` (front end, before `Race::SetUp`). Read in full. Blocked by the race event's lead. |
| `sub_82111650` | `Career::EventLocation` | | r3 = a player's save data: a stack word from the location +6108 (14 to 19 give 4 to 9, else 4), `sub_82144740(&it)` (the race event's entry (it + 3) × 12: `sub_82143DC8`, `sub_82143F90`). From `sub_82150E00` (the race's end). Read in full. Blocked by the race event's lead. |
| `sub_82116AF0` | `Career::NextDay` | | r3 = a player's save data: the day +6100 up by one; each garage car's word +1896 up (probably its days owned); save +0x1ACE0 = 0; `Stats::CloseDay(stats)`; `Career::PickNext(save, 0)`; `Stats::StartDay(stats, day % 7)`. From `sub_82197278`, `sub_82221AC0`, `sub_8225A520` (front end). Read in full. |
| `sub_82113898` | `Stats::CloseDay` | | r3 = the stats record: +6028 (days) up to 9,999; running means (`RunningMean::Add` weight 1, `RunningMean::Merge`) at +5936 from +5960, +5944 from +5964 and +5928 from +5952 × 100 / +5956 (the compiler's divide-by-zero traps kept). From `Career::NextDay`. Read in full. |
| `sub_82112088` | `Stats::StartDay` | | r3 = the stats record, r4 = the day of the week: on 0 the week's block (+4864: 24 bytes cleared, floats +4888, +4892 = 0.0, words +4896 to +4912 and halfwords +4916 to +4922 = 0); `DayLog::Push(+5672, the day block +24)`; the day block cleared (4,828 bytes, then its +4828 to +4836 = 0); the streaks +5970, +5972 = 0; +5952, +5960, +5964 = 0, +5956 = 1. From `Career::NextDay`. Read in full. |
| `sub_82112028` | `DayLog::Push` | | r3 = a log of 10 12-byte records (+0 a count, up to 10; records from +4), r4 = the day block: records moved up one, the newest {the block's +22 (races), float +4836, +18}. Leaf. From `Stats::StartDay`. Read in full. |
| `sub_821159B0` | `Career::PickNext` | | r3 = a player's save data, r4 = a flag. Only once the stats' races (+70) are 15 or more and the day (+6100) is 3 to 999 or above 999: a level (0 to 15) from +70 against 10, 20, 30, 50, 70, 90, 110, 140, 170, 200, 240, 280, 320, 360, 400; the 20-byte pick at save +8 copied to the stack; `sub_821115C8` on it; with r4, `sub_82115550`, then `sub_82115708`; without (or when both refuse), `sub_82112260`, else `sub_82110CA8` and `sub_82111140`; the pick's copy at +28 kept when unchanged, else its bytes +10 and +18 from `rand` (odd or even); the level into its byte +0; written back to +8. 436 lines. From `Career::NextDay`, `sub_82182E68`, `sub_82192958`, `sub_821FA230` (front end). Read; its callees mostly not (none seen in play). |
| `sub_821115C8` | `Pick::IsSpecial` (probably) | | r3 to r5 = the 20-byte pick by value: 1 when its +4 is 5 and halfword +8 is 17 to 70, or its +12 is 3 and +16 is 4 or 5. Leaf. From `Career::PickNext`. Not seen in play. Read in full. |
| `sub_82112260` | | | r3 = a player's save data, r4 = the pick, r5 = the level. Fills the pick from the recent results (the last results' and the day log's sums and tests: `sub_82110610`, `sub_821105A8`, `sub_82110678`, `sub_821106D0`, `sub_82110928`, `sub_82110970`, `sub_82110750`, `sub_82110798`), `sub_82110BE8`, `Items::AllWonKind5Or4`, `sub_82111140`. 841 lines. From `Career::PickNext`. Not seen in play. Not read in detail. |
| `sub_82110CA8` | | | 733 lines, no calls. From `Career::PickNext`, `sub_82115708`. Not seen in play. Not read. |
| `sub_82111140` | | | 766 lines, no calls. From `Career::PickNext`, `sub_82112260`. Not seen in play. Not read. |
| `sub_82115550` | | | r3 = a player's save data: a car built on the stack (`Car::Init`, `memcpy`, `sub_820CC778`, `sub_820CC6A0`, `sub_820CC480`, the car object's functions without rows) and the items read. 249 lines. From `Career::PickNext`. Not seen in play. Not read in detail. |
| `sub_82115708` | | | As `sub_82115550`, with `sub_82110CA8`. 373 lines. From `Career::PickNext`. Not seen in play. Not read in detail. |
| `sub_82110BE8` | `Progress::TablesDoneAtLeast` | | r3 = a player's save data, r4 = a percentage: whether 100 × (the entries of tables 2 to 7 whose word at +0x1B578 + 768 × (t - 2) + 4 × i is 2) / (all their entries) is at least r4 (the divide-by-zero traps kept). From `sub_82112260`. Not seen in play. Read in full. |
| `sub_821105A8` | `Results::MeanWord0` | | r3 = the last results (+0 the count, 48-byte records from +4): the mean of their word +0 (0.0 for none). Leaf. From `sub_82112260`. Not seen in play. Read in full. |
| `sub_82110610` | `Results::MeanWord4` | | As `Results::MeanWord0` for word +4. Not seen in play. Read in full. |
| `sub_82110678` | `Results::MeanFloat8` | | As `Results::MeanWord0` for the float +8 (the mean of the record's pair at +8). Not seen in play. Read in full. |
| `sub_821106D0` | `Results::WonPercent` | | 100 × (records whose +16, won, is nonzero) / count, as a float (0.0 for none). Not seen in play. Read in full. |
| `sub_82110750` | `Results::AllWord40Are` | | r4 = a value: 1 when the count is 15 and every record's +40 equals r4. Not seen in play. Read in full. |
| `sub_82110798` | `Results::AnyRepeated` | | r4 = a number: 1 when some record's +20 (the first of its three words) appears at least r4 times among the records after it. Not seen in play. Read in full. |
| `sub_82110928` | `DayLog::SumRaces` | | r3 = the day log: the sum of records 1 to 9's word +0 (the races; the newest left out) × `[0x82001FBC]`. Not seen in play. Read in full. |
| `sub_82110970` | `DayLog::SumWord8` | | As `DayLog::SumRaces` for word +8. Not seen in play. Read in full. |
| `sub_82115CC0` | `Career::PickFromCar` | Overridden | r3 = a player's save data: the pick at +8 set from the selected garage car (+0x19920-th): +12 = 4, +16 = the car's byte +1569 (sign-extended), +18 = 1, +20 = 3, +24 = its id (+12) % 4 (signed), +26 = 1, byte +8 = 0; then copied to +28. Leaf. From `sub_82182E68`, `sub_82221AC0` (front end). Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82116CE0` | `Garage::Add` | | r3 = a player's save data, r4 = a car object (1,968 bytes): the car's spec word +744 (`Car::SpecWord744`); `Garage::InsertCar(garage, car, its count)`; on the new last car, `sub_821104D8(car, its id)` (the car object's makers' lead); when the spec word was nonzero and the new car's differs, its +1900 = the old one. From `sub_82182E68` (front end). Read in full. Blocked by `sub_821104D8` (ready itself). |
| `sub_821104B0` | `Car::SpecWord744` | Overridden | r3 = a car object (r4, r5 unused): `Params::GetCarSpec(+12)`'s word +744. From `Garage::Add`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_821154E0` | `Garage::InsertCar` | | r3 = the garage (save +6120), r4 = a car, r5 = a position: `GarageList::Insert(r5)`, then the 1,968-byte car copied into the slot of the node at that position (+212 + 1,968 × slot). From `Garage::Add`. Read in full. |
| `sub_82113468` | `GarageList::Insert` | | r3 = the garage, r4 = a position: a node taken from the free list (+200) and linked in at that position (4-byte nodes {+0 next, +2 slot} from +0, the head at +204), the count +208 up. Leaf. From `Garage::InsertCar`. Read in full. |
| `sub_82113400` | `GarageList::Remove` | | r3 = the garage, r4 = a position: the node there unlinked and put back on the free list, the count down. Leaf. From `Garage::Remove`. Not seen in play. Read in full. |
| `sub_82116BE0` | `Garage::Remove` | | r3 = a player's save data, r4 = a position: `GarageList::Remove`; the selected car +0x19920 down by one when above r4. From `Garage::Sell`. Not seen in play. Read in full. |
| `sub_82116D88` | `Garage::Sell` (probably) | | r3 = a player's save data: stats +6006 up to 9,999, then a tail call to `Garage::Remove`. From `sub_82192958` (front end). Not seen in play. Read in full. |
| `sub_82112860` | `ItemFlags::Set` | Overridden | r3 = a player's save data, r4 = 0 (anything else does nothing), r5 = an item id: bit r5 set in the 400-bit set at +0x1ACE4. Leaf. From `sub_8222AE00` (front end). Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_821128A0` | `Bits24::SetAll` | | r3 = a bit set: word +0 = 0, then bits 0 to 23 set. Leaf. From `sub_82222038`. Not seen in play. Read in full. |
| `sub_82111628` | `PlayTime::Mark` | Overridden | r3 = a player's save data: +6088 = `[0x826C5B90]` (the frame counter `PlayTime::Add` measures from). Leaf. From `sub_821C0FF0`, `sub_82221AC0`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82110B18` | `Stats::AddPoints` | Overridden | r3 = the stats record, r4 = an amount: +6024 += r4, at most 9,999,999 (probably the money earned). Leaf. From `sub_8216E150`, `sub_82192958`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82110B10` | `Stats::WinStreak` | Overridden | r3 = the stats record: halfword +5970 (zero-extended). Leaf. From `sub_8220F840` (on a stack copy). Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82110580` | `DayBlock::WinStreak` | Overridden | r3 = the day block (stats +24, or a copy): halfword +4828, the day's win streak (zero-extended; `sub_8216E150` compares it with 20, 15, 10, 7). Leaf. From `sub_8216E150`, `sub_8225A520`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_82110588` | `DayBlock::BestStreak` | | As `DayBlock::WinStreak` for +4830, the day's best. From `sub_82140290`. Not seen in play. Read in full. |
| `sub_82110590` | `DayBlock::AddWord4832` | Overridden | r3 = the day block, r4: word +4832 += r4. Leaf. From `sub_8216E150`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
| `sub_821105A0` | `DayBlock::Word4832` | Overridden | Word +4832. Leaf. From `sub_8225A520`. Read in full. Native (2026-10-30): `src/career/accessors.cpp` behind `--native_career`, the adapter in `src/hooks/career.cpp`; harness `career`. Not called in an attract race. |
