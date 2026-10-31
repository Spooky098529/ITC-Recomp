# Known functions: the race HUD machine

The **race HUD machine**: one of the race mode's state machines (race
mode + 128664, `RaceHudMachine::`, set up by `RaceHudMachine::Init`
`sub_8216E6E8` from `RaceMode::Init`, `game-modes.md`; names here are
ours, and which state is which is guessed from what the handlers do). It
drives the in-race screens' holder (`RaceScreens::` at `[0x826D84F0]`,
`race-hud-object.md`) through a race: it makes the holder at the start
and destroys it at the end, sets the HUD up once the race world has
loaded, and every Update (message 0) and Draw (message 1, render-only
passes too) runs the HUD's tick and movies, by the race kind, for the
state the race is in: before the start, a free run's intro movie, the
countdown, the race, the pause menus, the results and the menu after the
race. Which screen it runs depends on the kind (the machine's +9612, the
race data's +0xA3AF0 copied at the start: 1 to 3 and 6 the race HUD at
the holder's +16, 4 the HUD-like object at +656, probably the split
screen's, 5 the alternate HUD at +1004, `alt-hud.md`; 0 the holder's
+12, the attract race's: an attract race stays in state 2 with kind 0,
seen 2026-10-31), and on the holder's +8 bit 0, set when the
menu after the race picks the replay (message 42; the alternate HUD is
then drawn in place of the race HUD, so probably the replay's dashboard
with the pad's inputs). After a career race it works out the prize money
and adds it to player 0's save data (`RaceHudMachine::AwardPrize`).

The machine's words: +9604 the state (0 at set-up), +9608 the holder
(`[0x826D84F0]`, copied by `RaceHudMachine::CreateScreens`), +9612 the
race kind, +9616 set by message 30 and cleared by the set-up and the
pause menu's close (read only by kind 4's pause menu). The handlers get
r3 = the machine and r4 = the race mode's dispatcher (+36); they post
messages to every machine by appending to the dispatcher's ring (+204:
50 entries of 12 bytes, +0 the message, +4 and +8 0; +804 the count,
+808 the start; written inline, "posts N" below), as `race-states.md`'s
do. The holder's other screens are its +1012 (`[0x826D84D0]`, the pause
menu: `sub_8220FA88` opens it at a page, `sub_8220F638` runs it and
answers the item picked, `sub_8220FB10` draws it), +1016
(`[0x826D84E0]`, kind 4's pause menu, probably), +1020 (`[0x826D8510]`,
the menu after the race), +1024 (`[0x826D8500]`, the career's results),
+1028 (`[0x82814E18]`), +1032 (`[0x82814E1C]`) and +1036
(`[0x82814E10]`, kind 6's); their functions are the other in-race
screens' lead in `../loops/leads.md`, not mapped here.

The transitions `RaceHudMachine::Init` adds ((state, message) → next
state, handler; 0 = stay; 65 in all; handlers are functors of vtable
`0x82011918`, a member function on the machine, or `0x820069BC`, a state
change only): (0, 0) → 1 `CreateScreens`; (1, 0) nothing
(`sub_82106540`); (1, 4) → 2 `SetUp`; (2, 0) `PreRaceUpdate`; (2, 1)
`PreRaceDraw`; (2, 17) and (2, 43) → 4; (2, 5) and (2, 6) → 3
`StartIntro`; (2, 29) → 9 `OpenPause`; (2, 30) `OnMessage30`; (2, 49)
→ 24; (2, 19) → 6 `StartMovie2`; (3, 0) `IntroUpdate`; (3, 1)
`IntroDraw`; (3, 7) → 2 `EndIntro`; (4, 26) → 5 `StartCountdown`; (4,
19) → 6 `StartMovie2`; (6, 0) `Movie2Update`; (6, 1) `Movie2Draw`; (6,
7) → 2 `EndMovie2`; (5, 0) `CountdownUpdate`; (5, 1) `CountdownDraw`;
(5, 29) → 11 `OpenRacePause`; (5, 30) `OnMessage30`; (5, 38) → 21
`EndInCountdown`; (5, 19) → 6 `StopCountdown`; (7, 0) `RaceUpdate`; (7,
1) `RaceDraw`; (7, 29) → 10 `OpenRacePause`; (7, 30) `OnMessage30`; (7,
58) → 8 `ToggleView`; (7, 38) → 21 `OnRaceEnd`; (7, 42) and (7, 24) →
4; (9, 0) `PauseUpdate`; (9, 1) `PauseDraw`; (9, 29) → 2 and (9, 49) →
24 `ClosePause`; (9, 44) → 23; (8, 1) `ViewDraw`; (8, 58) → 7
`ToggleView`; (10, 0) `PauseUpdate`; (10, 1) `RacePauseDraw`; (10, 29)
→ 7 and (10, 49) → 24 `ClosePause`; (10, 44) → 23; (11, 0)
`PauseUpdate`; (11, 1) `CountdownPauseDraw`; (11, 29) → 5 and (11, 49)
→ 24 `ClosePause`; (11, 44) → 23; (21, 0) `ResultsUpdate`; (21, 1)
`ResultsDraw`; (21, 49) → 24 `CloseResults`; (22, 0) `AfterRaceUpdate`;
(22, 1) `AfterRaceDraw`; (22, 7) → 2, (22, 17) and (22, 43) → 4, (22,
44) → 23 and (22, 49) → 24 `CloseResults`; (23, 0) `Restart`
(`sub_8215E140`: back to state 1); (24, 0) nothing (`sub_82106540`);
(24, 50) → 25 `DestroyScreens`. So, probably (by the messages
`race-states.md` posts and takes): 0 the start; 1 waiting for the race
world's load (message 4); 2 before the start (17 a stage set up, 43 a
replay's start, 19 the race flags cleared, 5 and 6 a free run); 3 a
free run's intro (the HUD's movie +628, until message 7); 4 a stage set
up, until the countdown (26); 6 the HUD's movie +620 (after 19, until
7); 5 the countdown (its movie +612), then 7 the race; 8 a view of the
alternate HUD (58 in the race; only with the replay bit or kind 5); 9 to
11 paused (29 from 2, 7 and 5; 29 again goes back, 44 aborts to 23, 49
ends); 21 the results (38 at the race's end), 22 the menu after the
race; 23 an abort, back to 1; 24 the end (49), then 25 (50, the holder
destroyed). Messages 29 (the pause and the resume, as in
`race-scene.md`), 49 and 50 are those the other machines take too.

Read by the B7 loop's mapping agent (2026-10-31) from the race flow's
HUD handlers' lead in `../loops/leads.md` (the lead had them as the race
flow's, `sub_82160AE0`; they are this machine's), all in full, alone
(no subagents). Shared, with rows elsewhere: `StateMachine::Add` and
`RaceMode::Init` in `game-modes.md`; the empty `sub_82106540` in
`audio.md`; `RaceScreens::Create`, `RaceScreens::Destroy`, the holder's
small tail calls (`sub_82211D00`, `sub_82211D08`, `sub_82211D10`) and
the HUD's functions (`RaceHud::SetPlayer`, `Reset`, `ClearWarning`,
`SetUpGauges`, `ResetBar`, `SetUpExtraGauges`, `SetUpMirror`,
`ApplyOptions`, `SetUpSigns`, `Tick`, `UpdateView`, `UpdateWarning`,
`UpdateItem`, `TickPanel`, `SetMode`, `StartCountdown`, `StopCountdown`,
`Draw`, `RunWarning`, `RunItem`, `RunCountdown`, `DrawWarning`,
`StartMovie1`, `FinishMovie1`, `RunMovie1` and the unnamed
`sub_822126C8`, `sub_82212868`, `sub_82212A38`, `sub_82212AE8`) in
`race-hud-object.md`; `RaceHud::RunMovie` in `race-hud.md`;
`AltHud::SetUp`, `Tick`, `RunMovie` and `ToggleView` in `alt-hud.md`;
`Stages::Type`, `Stages::HasNext`, `sub_820FFC68`, `RacerRecords::At`,
`SaveData::Of` and `Vehicle::Get` in `vehicle-accessors.md`;
`DataTables::Item` in `data-tables.md`; `DayBlock::WinStreak`,
`DayBlock::AddWord4832` and `Stats::AddPoints` in `career.md`; `memset`
in `c-runtime.md`. Not here, in leads (`../loops/leads.md`): the other
in-race screens' functions the handlers call (their lead), and the
holder's +656 object's (`sub_82215870`, `sub_822158E8`, `sub_822157E8`,
`sub_82216850`, `sub_82216A40`, with `sub_82215D08`, `sub_82215D58`,
`sub_82215DA8`, `sub_82216810`, `sub_82216830`, `sub_822159F0`; its
lead, never seen in play).

Made native by the B7 loop's migration (2026-10-31, `--native_race_hud_machine`,
`src/race_hud_machine/`, adapters in `src/hooks/race_hud_machine.cpp`,
harness `race_hud_machine`): the handlers call their callees (the race
HUD's and the alternate HUD's functions, `race-hud-object.md`,
`alt-hud.md`, `race-hud.md`) by address through the adapters'
implementation, which runs their plain C++ directly. The harness runs
the callees for real on both sides (rewritten 2026-10-31, the user's
call: no stand-in in the callees' adapters): the recompiled handlers
reach the callees' originals (their adapters' flags off), the native
ones their plain C++, in a world built as the race HUD object's and the
alternate HUD's harnesses build theirs (the HUD at the holder's +16, the
alternate HUD's holder at +1004, the movies, the vehicles, the race
data's car objects, the track zones, the sound manager, the action
mappers), the guest calls those harnesses record recorded the same way
(`hooks::g_start_up_recorder` and the UI movies' stand-in, which also
takes `UiMovie::Draw`'s callees as the race HUD's harness does), so the
cases compare everything the handlers and their callees write; f1 only
where the last callee's own harness compares it. `RaceUpdate`'s post
(when `AltHud::Tick` answers non-zero) isn't reached: `AltHud::Tick`
always answers 0.
Kind 4 (the +656 object) and kind 0's draws (the holder's +12,
`sub_8220B9E8`) aren't ported: the adapters run the originals. Kind 0 is
the attract race's: a log of the fallbacks (2026-10-31) showed an attract
race in state 2 with kind 0 throughout, so its draws run `sub_8220B9E8`
in play although `../../itc_recomp/coverage/seen_in_play.txt` lacks it;
it belongs to the other in-race screens' lead.

"The last result" below is the last of the race data's racer records
(`RacerRecords::At(race data + 0x59AE8, -1)`, `../known-data.md`): +0
the count of its 972-byte entries (from +4), +3916, +3920 and +3924
words (+3924 1 probably a win, 2 a loss, 3; +3920 3 tested). "The
replay bit" is the holder's +8 bit 0. The float constants named by
address (`0x82001FD8`, 10.0 as other files read it;
`0x820021E8`, probably 2.0; `0x82002CF0`, `0x820027A8`, `0x820029F0`,
`0x820029D4`, `0x82002CB0`, `0x82001BE0`, probably -1.0, and
`0x82000A30`, probably 1.0) weren't read from the image; `0x82000A34` is
0.0 (`career.md`).

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
| `sub_8216E6E8` | `RaceHudMachine::Init` | Overridden | r3 = the state machine (race mode + 128664): +9604 (the state) = 0, then the 65 transitions above through `StateMachine::Add` (`game-modes.md`), each a 16-byte key (state, message, 0, 0) and a functor's six words copied in the 4,400-byte frame (storage +0 the vtable, +4 the machine, +8 the member function; the copy's other words are the frame's) with the next state at +28. From `RaceMode::Init` (`game-modes.md`), once a race. 2,149 generated lines. Read in full. Native: `race_hud_machine::MachineInit` (`src/race_hud_machine/race_hud_machine_init.cpp`; the 65 transitions' stores and six-word copies replayed in the original's 4,400-byte frame from a table decoded from the generated code by a script that followed its registers, as `RaceSceneMachine::Init`'s, `race-scene.md`, so the storage words it never writes are the stack's as before; `StateMachine::Add`'s plain C++), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); r3 as `StateMachine::Add`'s adapter leaves it; the frame's back chain and register saves aren't written. Harness `race_hud_machine` (8 cases: empty and random machines with the planted keys, the frame random; the machine, the frame, r3). Seen natively in an attract race. |
| `sub_8216CA28` | `RaceHudMachine::CreateScreens` | | (0, 0) → 1, r3 = the machine: +9612 = the race data's (`[0x826C5B2C]`) +0xA3AF0 (the kind); then `RaceScreens::Create` (`race-hud-object.md`) with the holder's kind by the kind and the current stage's type (`Stages::Type`(race data + 0x59620, -1)): kind 0 → 8; 1: type 0, 2 or 4 → 0, 1 or 3 → 2 when the race data's +0xA3AF4 is 0, else 1, other types none; 2 → 2; 3 → 3; 4: type 0 or 2 → 4, 1 or 3 → 5, else none; 5 → 9; 6: type 0 or 2 → 7, 1 or 3 → 6, else none; 7 and above → 10 (no holder made where none). Then +9608 = `[0x826D84F0]` (the holder, or what was there). Once a race. Read in full. |
| `sub_8216D450` | `RaceHudMachine::SetUp` | Overridden | (1, 4) → 2, r3 = the machine (probably when the race world has loaded, message 4). By the kind (+9612): 1 and 2, `AltHud::SetUp(holder + 1004, 0)` (`alt-hud.md`), then as 3; 3, `RaceHud::SetPlayer(holder + 16, 0)`; 6, `SetPlayer` with 1 when bit 0x400 (`rlwinm` 22, 31, 31; 0x200 here before, corrected 2026-10-31) of the player's racer record's word +3888 (the racers' manager `[0x826D82F4]` +2308, its entry's +24 the record, race data + 3,936 × (record + 1)) is set, else 0, and `AltHud::SetUp` with the same player; then for 1 to 3 and 6, on the HUD (holder + 16): `RaceHud::Reset`, `ClearWarning`, `SetUpGauges`, `ResetBar`, `SetUpExtraGauges`, `SetUpMirror`, `ApplyOptions`, `SetUpSigns` (`race-hud-object.md`). 4: for players 0 and 1, `sub_82215870(holder + 656)`, `sub_822158E8(holder + 656, player, race data + 48 + 3,936 × player)` and `sub_822157E8(holder + 656, player, the vehicle's (`Vehicle::Get`) float +2420 truncated)` (the +656 object's lead), then `AltHud::SetUp(holder + 1004, 0)`. 5: `AltHud::SetUp(holder + 1004, 0)` only. Then +9616 = 0. Once a race. Read in full. Native: `race_hud_machine::SetUp` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kind 4 (the +656 object's loop) runs the original. Harness `race_hud_machine` (100 cases: the kinds, kind 6's record word with and without the bit, the alternate HUD's SetUp's car object shaped). Seen natively in an attract race (kind 0: +9616 = 0 only). |
| `sub_8216C210` | `RaceHudMachine::PreRaceUpdate` | Overridden | (2, 0), every Update before the start: kinds 1 to 3 and 6, on the HUD: `RaceHud::Tick(0)`, `UpdateView`, `UpdateWarning`, `UpdateItem`; kind 4: `sub_82216850(holder + 656)`; others nothing. Read in full. Native: `race_hud_machine::PreRaceUpdate` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kind 4 runs the original. Harness `race_hud_machine` (80 cases: the kinds, the guest calls each side makes, what the callees write, the frame, r3, f1 where compared). Seen natively in an attract race (kind 0: nothing). |
| `sub_8216C2A0` | `RaceHudMachine::PreRaceDraw` | Overridden | (2, 1), every Draw before the start: kinds 1 to 3 and 6: `RaceHud::Draw`, `RunWarning`, `RunItem`; 4: `sub_82215D58(holder + 656, 0)`; 0: `sub_8220B9E8(holder + 12)` (another in-race screen's, not read); 5 nothing. Read in full. Native: `race_hud_machine::PreRaceDraw` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kinds 4 and 0 run the original. Kind 0 is the attract race's (seen 2026-10-31: the attract race stays in state 2 with kind 0, so `sub_8220B9E8` runs in play, though `seen_in_play.txt` lacks it); its native path waits on the other in-race screens' lead (`sub_8220B9E8`, the holder's +12: movie 18 found and run). Harness `race_hud_machine` (80 cases). |
| `sub_8216C510` | `RaceHudMachine::StartIntro` | Overridden | (2, 5) and (2, 6) → 3: `RaceHud::StartMovie1(holder + 16)` (tail call; the movie at +628). Read in full. Native: `race_hud_machine::StartIntro` (`src/race_hud_machine/handlers.cpp`; the tail call from the caller's r1 with its lr), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); the harness runs `StartMovie1` and `FinishMovie1` for real on both sides. Harness `race_hud_machine` (30 cases). |
| `sub_8216C530` | `RaceHudMachine::IntroUpdate` | Overridden | (3, 0): kinds 1 to 3 and 6: `RaceHud::Tick(0)`, `UpdateView`, `UpdateWarning`; 4: `sub_82216850(holder + 656)`. Read in full. Native: `race_hud_machine::IntroUpdate` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kind 4 runs the original. Harness `race_hud_machine` (80 cases). |
| `sub_8216C5B8` | `RaceHudMachine::IntroDraw` | Overridden | (3, 1): kinds 1 to 3 and 6: `RaceHud::RunMovie1`, `Draw`, `RunWarning`; 4: `sub_82215D58(holder + 656, 0)`; 0: `sub_8220B9E8(holder + 12)`; 5 nothing. Ready since `RaceHud::RunWarning` became ours (2026-10-31). Read in full. Native: `race_hud_machine::IntroDraw` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kinds 4 and 0 run the original (kind 0's `sub_8220B9E8` waits on the other in-race screens' lead). Harness `race_hud_machine` (80 cases). |
| `sub_8216C520` | `RaceHudMachine::EndIntro` | Overridden | (3, 7) → 2: `RaceHud::FinishMovie1(holder + 16)` (tail call). Read in full. Native: `race_hud_machine::EndIntro` (`src/race_hud_machine/handlers.cpp`; the tail call from the caller's r1 with its lr), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); the harness runs `StartMovie1` and `FinishMovie1` for real on both sides. Harness `race_hud_machine` (30 cases). |
| `sub_8216C470` | `RaceHudMachine::StartMovie2` | | (2, 19) and (4, 19) → 6: `sub_822126C8(holder + 16)` (tail call; the HUD's movie +620 played and looped, `race-hud-object.md`). Not seen in play. Read in full. |
| `sub_8216C4C0` | `RaceHudMachine::Movie2Update` | | (6, 0): `RaceHud::Tick(holder + 16, 0)` and `UpdateView`, for every kind. Not seen in play. Read in full. |
| `sub_8216C480` | `RaceHudMachine::Movie2Draw` | | (6, 1): `RaceHud::Draw(holder + 16)` and `sub_82212AE8` (the movie +620 run), for every kind. Not seen in play. Read in full. |
| `sub_8216C500` | `RaceHudMachine::EndMovie2` | | (6, 7) → 2: `sub_82212868(holder + 16)` (tail call; the movie +620 finished). Not seen in play. Read in full. |
| `sub_8216C648` | `RaceHudMachine::OnMessage30` | | (2, 30), (5, 30) and (7, 30): +9616 = 1. Leaf. Not seen in play. Read in full. |
| `sub_8216CC60` | `RaceHudMachine::StartCountdown` | | (4, 26) → 5, r3 = the machine (26 the countdown, as `race-states.md`'s `OnCountdown`). The HUD's mode by the kind and the current stage's type (`Stages::Type`): kind 1: types 1 and 3 → 2 when the race data's +0xA3AF4 is 0, else 1; kind 2 → 2; kind 4: types 1 and 3 → 1; kind 6: types 1 and 3 → 4, 0 and 2 → 3; every other case 0. Then, without the replay bit: kinds 1 to 3 and 6: the HUD's +444 = `[0x826D82E8]` + 2184 (the race's progress, `race-states.md`), `RaceHud::SetMode(mode)`, `RaceHud::StartCountdown`; kind 4: the +656 object's +344 (holder +1000) = the same, `sub_82216810(holder + 656, mode)` and `sub_822159F0(holder + 656)`. Read in full. |
| `sub_8216CB98` | `RaceHudMachine::CountdownUpdate` | Overridden | (5, 0), every Update in the countdown: kinds 1 to 3 and 6: `RaceHud::Tick(1)`, `UpdateWarning`, `TickPanel`, `UpdateView`, then +9604 = 7 (the race) once the HUD's +436 (the countdown running) is 0; kind 5: +9604 = 7 at once; kind 4: `sub_82216850` and `sub_82216A40` on holder + 656, then 7 once its +336 is 0. Read in full. Native: `race_hud_machine::CountdownUpdate` (`src/race_hud_machine/handlers.cpp`; the countdown's word read as the holder's +452), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kind 4 runs the original. Harness `race_hud_machine` (80 cases). |
| `sub_8216C330` | `RaceHudMachine::CountdownDraw` | Overridden | (5, 1): kinds 1 to 3 and 6: `RaceHud::Draw`, `RaceHud::RunMovie(0)` (`race-hud.md`), `RunCountdown`, `RunWarning`; 5: `AltHud::RunMovie`; 4: `sub_82215D58(holder + 656, 0)` and `sub_82215DA8(holder + 656, 0)`; 0: `sub_8220B9E8(holder + 12)`. Ready since `RaceHud::RunWarning` became ours (2026-10-31). Read in full. Native: `race_hud_machine::CountdownDraw` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides), `RaceHud::RunMovie`'s as its adapter makes them (`race_hud::RunMovie`'s choice, the movie calls through their overrides); kinds 4 and 0 run the original (kind 0's `sub_8220B9E8` waits on the other in-race screens' lead). Harness `race_hud_machine` (80 cases). |
| `sub_8216C3E8` | `RaceHudMachine::StopCountdown` | | (5, 19) → 6: `sub_822126C8(holder + 16)` (the movie +620), then kinds 1 to 3 and 6 `RaceHud::StopCountdown`, kind 4 `sub_82215D08(holder + 656)`. Not seen in play. Read in full. |
| `sub_8216E668` | `RaceHudMachine::EndInCountdown` | | (5, 38) → 21, r3 = the machine, r4 = the dispatcher: kinds 1 to 3 and 6 `RaceHud::StopCountdown`, kind 4 `sub_82215D08(holder + 656)`; then `RaceHudMachine::OnRaceEnd(r3, r4)`. Not seen in play. Read in full. |
| `sub_8216D918` | `RaceHudMachine::RaceUpdate` | Overridden | (7, 0), every Update in the race, r4 = the dispatcher: kinds 1 to 3 and 6 without the replay bit: `RaceHud::Tick(1)`, `UpdateView`, `UpdateWarning`, `TickPanel`; with it, and kind 5: `AltHud::Tick(holder + 1004)`, and when that answers non-zero (it never does: it returns 0), posts 20; kind 4 without the bit: `sub_82216850` and `sub_82216A40` on holder + 656 (with it, as kind 5). Read in full. Native: `race_hud_machine::RaceUpdate` (`src/race_hud_machine/handlers.cpp`; the post as `race_states::steps::Post`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kind 4 runs the original. Harness `race_hud_machine` (100 cases: the ring's count and start random; the post isn't reached, `AltHud::Tick` answering 0). |
| `sub_8216CE00` | `RaceHudMachine::RaceDraw` | Overridden | (7, 1), every Draw in the race: kinds 1 to 3 and 6 without the replay bit: `RaceHud::Draw`, `RunWarning`, `RaceHud::RunMovie(0)`; with it, and kind 5: `AltHud::RunMovie`; kind 4 without the bit: `sub_82215D58(holder + 656, 0)` (with it, `AltHud::RunMovie`); 0: `sub_8220B9E8(holder + 12)`. Ready since `RaceHud::RunWarning` became ours (2026-10-31). Read in full. Native: `race_hud_machine::RaceDraw` (`src/race_hud_machine/handlers.cpp`), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callees the race HUD's and the alternate HUD's plain C++ by address with the original's r3, r4, frame and return addresses (`hooks/race_hud_machine.cpp`; the harness runs them for real on both sides); kinds 4 and 0 run the original (kind 0's `sub_8220B9E8` waits on the other in-race screens' lead). Harness `race_hud_machine` (80 cases). |
| `sub_8216CEB8` | `RaceHudMachine::ToggleView` | | (7, 58) → 8 and (8, 58) → 7: `AltHud::ToggleView(holder + 1004)` (tail call) for kind 5, and for kinds 1 to 4 and 6 with the replay bit; else nothing. Not seen in play. Read in full. |
| `sub_8216C460` | `RaceHudMachine::ViewDraw` | | (8, 1): `AltHud::RunMovie` (tail call; r3 = holder + 1004, not read by it). Not seen in play. Read in full. |
| `sub_8216D600` | `RaceHudMachine::OpenPause` | | (2, 29) → 9 (the pause before the start): by the kind, the pause menu `sub_8220FA88(holder + 1012, page, 0)` (a tail call): 1 page 0 (8 with the replay bit), 2 page 3, 3 page 4, 5 page 8, 6 `RaceHudMachine::OpenPauseByStage`; 4 `sub_82211390(holder + 1016, +9616 != 0, the replay bit)`. Read in full. |
| `sub_8216DA40` | `RaceHudMachine::OpenRacePause` | | (5, 29) → 11 and (7, 29) → 10 (the pause in the countdown or the race): as `OpenPause` but kind 1 opens page 8 with the replay bit, else page 2 when the race data's +0xA3AF4 is 0, else `RaceHudMachine::OpenCareerPause`; kind 2 page 3 (8 with the bit); kind 6 page 8 with the bit, else `OpenPauseByStage`. Read in full. |
| `sub_8216C798` | `RaceHudMachine::OpenPauseByStage` | | r3 = the machine: the pause menu (`sub_8220FA88(holder + 1012, page, 0)`) at page 6 for the current stage's type (`Stages::Type`) 1 or 3, page 7 for 0 or 2, else 0. From `OpenPause` and `OpenRacePause` (kind 6; tail calls). Not seen in play. Read in full. |
| `sub_8216D8C8` | `RaceHudMachine::OpenCareerPause` | | r3 = the machine: the pause menu at page 1 with r5 = `RaceHudMachine::IsFirstDay` (`sub_8220FA88(holder + 1012, 1, it)`). Only caller `OpenRacePause` (kind 1 with +0xA3AF4 set). Read in full. |
| `sub_8216D3F0` | `RaceHudMachine::IsFirstDay` (probably) | Overridden | r3 not read: 1 when the race data's kind (+0xA3AF0) is 1 and player 0's save data (`SaveData::Of(0)`) has +6100 (the career's day, `career.md`) 1 (a test for at most 999 first), else 0. Leaf but for `SaveData::Of`. From `OpenCareerPause`, `ResultsDraw`, `ResultsUpdate` (twice) and `OnRaceEnd`. Read in full. Native: `race_hud_machine::IsFirstDay` (`src/race_hud_machine/handlers.cpp`; `SaveData::Of`'s plain C++), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its callers are recompiled (`OpenCareerPause`, `ResultsDraw`, `ResultsUpdate`, `OnRaceEnd`). Harness `race_hud_machine` (40 cases: the kind and the day's edges). |
| `sub_8216DB70` | `RaceHudMachine::PauseUpdate` | | (9, 0), (10, 0) and (11, 0), every Update while paused, r4 = the dispatcher. Kind 6 with the race data's +0xA3AF4 8 first ticks the HUD (`RaceHud::Tick(1)`, `UpdateWarning`, `TickPanel`). The answer: kind 4 `sub_82211458([0x826D84F0] + 1016)`, kinds 1 to 3, 5 and 6 the pause menu's update (`sub_8220F638([0x826D84F0] + 1012)`), others 0. By it (a jump table): 1 posts 29 (the resume), 2, 6 and 7 post 48, 3 posts 45 and `sub_82211D00` (the countdown stopped), 4 posts 20, 5 posts 21 (the two quits, as `race-states.md`'s (25, 20) and (25, 21)), 8 `RaceHud::ApplyOptions([0x826D84F0] + 16)` (options changed), 9 posts 48 and, when the race data's +0xA3AF4 is 7, `[0x82816E3C]` +240 = 1; others nothing. Reads the holder from `[0x826D84F0]` (+9608 only for `sub_82211D00`). Read in full. |
| `sub_8216CF08` | `RaceHudMachine::PauseDraw` | | (9, 1): kinds 1 to 3 and 6 without the replay bit: `RaceHud::Draw`, `DrawWarning`, then the pause menu's draw (`sub_82211D08(holder)`); with it, and kind 5: the pause menu's draw and `AltHud::RunMovie`; kind 4: without the bit `sub_82215D58(holder + 656, 1)` and `sub_82211590(holder + 1016)`, with it `sub_82211590` and `AltHud::RunMovie`; 0: `sub_8220B9E8(holder + 12)`. Read in full. |
| `sub_8216CFE0` | `RaceHudMachine::RacePauseDraw` | | (10, 1): as `PauseDraw` with `RaceHud::RunMovie(1)` (draw only) between `Draw` and `DrawWarning`. Read in full. |
| `sub_8216D0C8` | `RaceHudMachine::CountdownPauseDraw` | | (11, 1): kinds 1 to 3 and 6 without the replay bit: `RaceHud::Draw`, `RaceHud::RunMovie(1)`, `sub_82212A38` (the countdown drawn only), `DrawWarning`, the pause menu's draw (`sub_82211D08`); with the bit, and kind 5, nothing; kind 4: `sub_82215D58(holder + 656, 1)`, `sub_82215DA8(holder + 656, 1)`, `sub_82211590(holder + 1016)`; 0: `sub_8220B9E8(holder + 12)`. Not seen in play. Read in full. |
| `sub_8216C658` | `RaceHudMachine::ClosePause` | | (9, 29) → 2, (10, 29) → 7, (11, 29) → 5 (the resume) and (9, 49), (10, 49), (11, 49) → 24: kinds 1 to 3 and 6: `sub_8220D890(holder + 1012)` (probably the pause menu closed) and `RaceHud::ApplyOptions(holder + 16)`; kind 4: the empty `sub_82106540(holder + 1016)`; then +9616 = 0. Read in full. |
| `sub_8216E5B0` | `RaceHudMachine::OnRaceEnd` | | (7, 38) → 21 (and from `EndInCountdown`), r3 = the machine: by the kind: 1 with the race data's +0xA3AF4 0 `ShowResults(1)`, else unless `IsFirstDay`, `RaceHudMachine::AwardPrize`; 2 `ShowResults(0)`; 4 `ShowSplitResults`; 6 `ShowKind6Results` (called with r3 still the machine); then the holder's +440 (the HUD's +424, the signs on) = 0. Read in full. |
| `sub_8216D270` | `RaceHudMachine::ShowResults` (probably) | | r3 = the machine, r4 = a flag: the last result; without the replay bit, a 20-byte block on the stack (+0 = 1, +4 the result's +20, +8 its +3892, +12 the race data's +60, +16 `[0x826D82E8]` +2196) to `sub_8221F990(holder + 1028, block)`; then `sub_8221BA68(holder + 1020, 4 when r4 is set, else 5, result +3920 == 3)` (probably the menu after the race set up). From `OnRaceEnd` (kind 1 with +0xA3AF4 0: r4 = 1; kind 2: 0). Not seen in play. Read in full. |
| `sub_8216D328` | `RaceHudMachine::ShowSplitResults` (probably) | | r3 = the machine (kind 4): the last result; without the replay bit: `sub_82216830(holder + 656)`; by the result's +3924: 1 → `AltHud::SetUp(holder + 1004, 0)` and n = 0, 2 → `SetUp(.., 1)` and n = 1, 3 → `SetUp(.., 0)` and n = 2, else n = 0 without `SetUp`; `sub_82220050(holder + 1032, n, 0)`. Then `sub_8221BA68(holder + 1020, 6, result +3920 == 3)`. Only caller `OnRaceEnd`. Not seen in play. Read in full. |
| `sub_8216D7B8` | `RaceHudMachine::ShowKind6Results` (probably) | | r3 = the machine (kind 6): the last result, w = its +3916. Without the replay bit: w 0 or 2: `sub_82220050(holder + 1032, a, 1)` with a by the result's +3924: 1 → whether the player's racer record's +3888 has 0x400 (as `SetUp`; not checked against this function's own `rlwinm`), 2 → whether it hasn't, 3 → 2, else 0; w 1 or 3: `sub_82220050(holder + 1032, 0, 2)`; else nothing. Then for w 1 or 3, a block on the stack (+4 the result's +20, +12 its +4; +0 and +8 not written: the frame's leftovers) to `sub_8221EC48(holder + 1036, block)`. Only caller `OnRaceEnd`. Not seen in play. Read in full. |
| `sub_8216E150` | `RaceHudMachine::AwardPrize` (probably) | | r3 = the machine (kind 1, a career race, from `OnRaceEnd`): the last result; for kind 1, player 0's save data (`SaveData::Of(0)`). Without the replay bit: a 168-byte block on the stack (`memset` 0): +0 by the result's +3920 1 and +3924 (1 → 0, 2 → 2, 3 → 1), or 2 for +3920 3; when +3924 is 1 (a win, probably): +8 = (int)(the result's float +828 × `[0x82001FD8]`) / 1000, and for each entry after the first (+0 the count; entry +4 + 972 × i, its +4 an item id) +4 += `RaceHudMachine::OpponentPrize(item)` and +20 += `RaceHudMachine::ItemBonus(item)`; +12 a factor by the day's win streak (`DayBlock::WinStreak(save + 72)`: 20 and more `[0x820021E8]`, 15 `[0x82002CF0]`, 10 `[0x820027A8]`, 7 `[0x820029F0]`, 5 `[0x820029D4]`, 3 `[0x82002CB0]`, else `[0x82000A30]`; 0.0 without save data); +16 = (int)((+4 + +8) × it); +24 the result's float +844 (`[0x82000A30]` when it's 0.0, 0.0 when the halfword +968 is 0), +28 = (int)(+24 × `[0x82001BE0]`); for each k from 1 below the halfword +968, the float at +852 + 8 (k - 1) (`[0x82000A30]` for 0.0) and it × `[0x82001BE0]` truncated as a pair at +32 + 8n, n (+160) counted up; +164 = save +6116 (the money). `sub_8221A1F0(holder + 1024, block)` (the career's results screen, probably). Then the total (+16 + +28 + +20 + the pairs' ints) goes to `DayBlock::AddWord4832(save + 72)`, save +6116 (at most 9,999,999) and `Stats::AddPoints(save + 48)` (`career.md`). Always after: `sub_8221BA68(holder + 1020, m, result +3920 == 3)` with m = 4 when the race data's +0xA3AF4 is 0, else 3 when `Stages::HasNext` and +3924 is 1, else 1 when the stage list's word +808 (`sub_820FFC68`) is set, else 0. Read in full. |
| `sub_8216C820` | `RaceHudMachine::OpponentPrize` (probably) | Overridden | r3 not read, r4 = an item id (-1: 0): the item (`DataTables::Item`) and player 0's save data's item record (+0x199F8 + 12 × id): m = 1 unless its halfword +2 (the count) is 1, then 2 when its +0 or +4 halfword is set, else 3; b by the item's byte +35: 0 → 10, 1 → 20, 2 → 15, 3 to 5 → 30, else 0; returns (b / 10) × m × (the item's byte +37, signed, + 1) × 10,000 / 1,000. From `AwardPrize`, per opponent. Read in full. Native: `race_hud_machine::OpponentPrize` (`src/race_hud_machine/handlers.cpp`; `DataTables::Item`'s and `SaveData::Of`'s plain C++), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its caller `AwardPrize` is recompiled. Harness `race_hud_machine` (80 cases: ids -1 and 370 to 409, the item bytes and the records' halfwords). |
| `sub_8216C938` | `RaceHudMachine::ItemBonus` (probably) | Overridden | r3 not read, r4 = an item id (-1: 0): when player 0's save data's item record (+0x199FA + 12 × id, a halfword) is 1, by the id (a jump table): 376 to 378 → 1,000, 379 to 381 → 1,500, 382 → 2,000, 383 to 390 → 3,000, 391 → 5,000, 392 to 398 → 10,000, 399 → 100,000, others 0 (each a constant over 1,000); else 0. From `AwardPrize`, per opponent. Read in full. Native: `race_hud_machine::ItemBonus` (`src/race_hud_machine/handlers.cpp`, with `sub_8216C9B8` merged), adapter in `src/hooks/race_hud_machine.cpp` behind `--native_race_hud_machine` (B7 loop, 2026-10-31); its caller `AwardPrize` is recompiled. Harness `race_hud_machine` (80 cases, the jump table's ids 376 to 399 among them). |
| `sub_8216C9B8` | | Merged | A piece of `ItemBonus` (`sub_8216C938`, ids 376 to 378: 1,000,000 / 1,000), split off as its own function by the recompiler (reached from its jump table as a tail call). Not seen in play (the coverage counts it apart). Read in full. Merged into `race_hud_machine::ItemBonus` (`src/race_hud_machine/handlers.cpp`, behind `--native_race_hud_machine`); harness `race_hud_machine` (`ItemBonus`'s cases). |
| `sub_8216DFD0` | `RaceHudMachine::ResultsUpdate` | | (21, 0), every Update on the results, r4 = the dispatcher: kinds 0 to 3 and 6 `RaceHud::Tick(0)` and `UpdateWarning`, kind 4 `sub_82216850(holder + 656)`. Then an answer by the race data's kind: 4 and 6 `sub_8221FFF0(holder + 1032)`, 2 (and 1 with +0xA3AF4 0) `sub_8221F930(holder + 1028)`, 1 otherwise, unless `IsFirstDay`, `sub_8221A090(holder + 1024)`. Then with `IsFirstDay` posts 48; else when the answer isn't 0 or the replay bit is set, +9604 = 22 (the menu after the race). Read in full. |
| `sub_8216D6D0` | `RaceHudMachine::ResultsDraw` | | (21, 1): by the race data's kind: 0 `sub_8220B9E8(holder + 12)`; 1 with +0xA3AF4 0, and 2: `sub_8221F7D0(holder + 1028)`; 1 otherwise, unless `IsFirstDay`, `sub_8221A2F8(holder + 1024)`; 4 and 6 `sub_822200E0(holder + 1032)`; 3 and 5 nothing. Then kind (+9612) 4 `sub_82215D58(holder + 656, 0)`, every other `RaceHud::Draw(holder + 16)`. Read in full. |
| `sub_8216D198` | `RaceHudMachine::CloseResults` | | (21, 49) → 24 and (22, 7) → 2, (22, 17), (22, 43) → 4, (22, 44) → 23, (22, 49) → 24: by the race data's kind the empty `sub_82106540` on a screen (1: holder +1028 with +0xA3AF4 0, else +1024; 2: +1028; 4: +1032; 6: +1032, then `sub_8221C168(holder + 1036)`); then `sub_8221A498(holder + 1020)` (probably the menu after the race closed) and the holder's +440 (the HUD's signs on) = 1. Read in full. |
| `sub_8216DD88` | `RaceHudMachine::AfterRaceUpdate` | | (22, 0), every Update on the menu after the race, r4 = the dispatcher: the last result's +3916 kept (w); kinds 1 to 3 and 6 `RaceHud::Tick(0)` and `UpdateWarning`, 4 `sub_82216850(holder + 656)`, 0 `sub_8220B9E8(holder + 12)`. Then kinds 1 to 4: the menu's answer (`sub_8221BD20(holder + 1020)`, a jump table): 1 and 8 post 40, 2 posts 42 (the replay) and sets the replay bit, 4, 5, 7 and 9 post 48, 6 posts 45, 3 nothing; every post but 42's clears the bit. Kind 6: w 0 or 2 posts 40 (the bit kept); w 1 or 3: `sub_8221F1E8(holder + 1036)`: 1 posts 45, 2 posts 42 and sets the bit, 3 posts 48 (both clear it). The bit is the only writer of the holder's +8 found. Read in full. |
| `sub_8216C6D8` | `RaceHudMachine::AfterRaceDraw` | | (22, 1): kinds 1 to 3 `RaceHud::Draw(holder + 16)` and the menu's draw (`sub_82211D10(holder)`, `sub_8221BB48`); 4 `sub_82215D58(holder + 656, 0)` and the same; 6 `sub_8221EF60(holder + 1036)` when the last result's +3916 is 1 or 3; 0 `sub_8220B9E8(holder + 12)`; 5 nothing. Read in full. |
| `sub_8215E140` | `RaceHudMachine::Restart` | | (23, 0): +9604 = 1 (r3 the machine; back to waiting for the race world). Leaf. Shared: `RaceOverlaysMachine::Init` (`game-modes.md`) registers it too (the overlays' rows are in `race-overlays.md`). Not seen in play. Read in full. |
| `sub_8216C208` | `RaceHudMachine::DestroyScreens` | | (24, 50) → 25: `RaceScreens::Destroy` (tail call, `race-hud-object.md`). Once a race, at its end. Read in full. |
| `sub_8216D6CC` | | | Empty (`blr`) between `RaceHudMachine::OpenPause` and `ResultsDraw`; no callers or references found. Not seen in play. Read in full. |
| `sub_8216DB68` | | | Empty (`blr`) between `RaceHudMachine::OpenRacePause` and `PauseUpdate`; no callers or references found. Not seen in play. Read in full. |
