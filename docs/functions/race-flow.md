# Known functions: the race flow

The **race flow** (`RaceFlow::`): one of the race mode's state machines
(race mode + 97548, set up by `RaceFlow::Init` `sub_82160AE0` from
`RaceMode::Init`, `game-modes.md`; the layout is `StateMachine::`'s,
`game-modes.md`: 200 transitions of 48 bytes, the count at +9600, the
state at +9604). It picks how each racer's input record is filled
through a race: its handlers are the racer controllers'
(`racer-controllers.md`), which fill the cars' input records from the
pad, the input recordings or the AI drivers, held or let go, by the
state. Its own words: +9604 the state (1 at set-up), +9608 flags (0 at
set-up; bit 0x1 set by `RaceFlow::OnSetUp`), +9612 and +9616 the human
cars' players (0 and 1; both 0 when the race data's +0xA3AF0, the race
kind, is 6). Names are ours; which state is which is guessed from the
handlers and the messages the other machines post and take
(`race-states.md`, `race-hud-machine.md`).

The transitions `RaceFlow::Init` adds ((state, message) → next state,
handler; 0 = stay; 53 in all, keys' two other words 0; handlers are
functors of vtable `0x82011918`, a member function on the race flow, or
`0x820069BC`, a state change only; worked out by replaying the
function's stores and copies): (1, 0) → 3 `OnSetUp`; (3, 26) → 22; (3,
5) → 5; (3, 17) → 16; (3, 46) → 34; (34, 0) `OnState34`; (34, 49) →
37; (5, 0) `OnHold`; (5, 7) → 7; (5, 17) → 16; (7, 0) `OnState7`; (7,
29) → 24; (7, 17) → 16; (7, 8) → 8; (7, 49) → 37; (7, 19) → 20; (24,
29) → 7; (11, 0) `OnHold`; (11, 27) → 16; (16, 0) `OnState16`; (16, 26)
→ 22; (16, 19) → 20; (16, 15) → 11; (8, 0) `OnHold`; (8, 49) → 37;
(17, 0) `OnHold`; (17, 26) → 22; (22, 0) `OnState22`; (22, 28) → 23;
(22, 19) → 20; (20, 0) `OnHold`; (20, 7) → 7; (23, 0) `OnState23`;
(23, 29) → 25; (23, 34) and (23, 36) → 30; (23, 22) → 14; (23, 49) →
37; (23, 58) → 25; (25, 29) and (25, 58) → 23; (25, 44) → 32; (14, 0)
`OnState14` (`sub_8215FC30`); (14, 17) → 16; (30, 0) `OnHold`; (30,
17) → 16; (30, 7) → 7; (30, 42) → 17; (30, 49) → 37; (30, 44) → 32;
(32, 0) `OnAbort` (`sub_8215F7B0`, which sets the state to 3 itself);
(37, 0) nothing (`sub_82106540`); (37, 50) → 35 `OnEnd`. So, probably
(by the messages `race-hud-machine.md` reads the same way): 1 the start
(the input records made), 3 waiting for the stage (17 a stage set up →
16; 26 the countdown → 22; 5 a free run's intro → 5; 46 → 34, every
car by its AI driver, probably an attract race, which stays in 34 until
the end, 49); 5 a free run's intro (held, until 7); 7 the race after the
intro (the players and the held computer racers; 29 the pause → 24 and
back); 16 a stage set up (26 the countdown → 22, 15 → 11, held until 27);
22 the countdown and the race (`OnState22`: every car by the AI, or the
stage's way; 28 → 23); 23 the race (the players and the AI; the replay
ran out, 34, or 36 → 30; 29 the pause or 58 a view → 25 and back; 22 →
14); 30 after the race or the replay (held; 42 a replay's start → 17, 7
→ 7); 20 held (19 the race flags cleared, until 7); 8 held (until the
end); 25 paused (44 an abort → 32: every input record cleared, back to
3); 37 the end (49 from 7, 8, 23, 30 or 34), then 35 (50: the input
records freed). Messages 29 (the pause), 44, 49 and 50 are those the
other machines take too.

Read by the B7 loop's mapping agent (2026-11-04) from the race flow's
set-up lead in `../loops/leads.md`, all in full, alone (no subagents).
Shared, with rows elsewhere: `StateMachine::Add` (`sub_82106158`),
`Transition::Make`, `Transition::Assign`, `StateMachine::Handle` and
`RaceMode::Init` in `game-modes.md`; the empty `sub_82106540` in
`audio.md`; the handlers `RaceFlow::OnSetUp` (`sub_8215F728`),
`OnState34` (`sub_8215FC38`), `OnHold` (`sub_8215FC28`), `OnState7`
(`sub_82160940`), `OnState16` (`sub_8215FB90`), `OnState22`
(`sub_82160948`), `OnState23` (`sub_821609E0`) and `OnEnd`
(`sub_8215F5E8`), with `RacerControl::DriveByAi` (`sub_8215F968`), in
`racer-controllers.md`; `CarInput::Clear` (`sub_820C96E8`) in
`ai-drivers.md`; `Race::RacerCount` (`sub_821705F8`) in
`collision-world.md`; `Vehicle::Get` (`sub_820C0468`) in
`vehicle-accessors.md`; `Delegate::Invoke` (`sub_8223A3F0`, the
member functors' call) in `functions.md`. The handlers' adapters stay
while `StateMachine::Handle` calls them through their guest addresses
(`src/hooks/racer_controllers.cpp`). Fields: `../known-data.md` ("race
flow", "transition", `0x826C5B30`).

When `RaceFlow::Init` is made native, the precedent is
`RaceStates::Init` and `RaceHudMachine::Init` (the user's call that B7
changes no behavior: stack leftovers are copied as the original leaves
them): each functor is six words copied from a stack slot of the
function's 3,600-byte frame where only the vtable (and for member
functors the object and the function) were written, so the table keeps
five (or three) of the stack's words a transition; replay the stores
and copies in the original's frame.

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
| `sub_82160AE0` | `RaceFlow::Init` | | r3 = the race flow (race mode + 97548): +9604 = 1, +9608 = 0; +9612 = 0 and +9616 = 1, or both 0 when the race data's (`[0x826C5B2C]`) +0xA3AF0 is 6; then the 53 transitions in the intro, each through `StateMachine::Add` (a key of (state, message, 0, 0) at the frame's +80, r5 = a frame slot pointing at the functor's six words copied from another slot, the next state at r5 + 28). 1,661 lines, a 3,600-byte frame; no other calls. Returns `StateMachine::Add`'s last r3. From `RaceMode::Init` only (once a race set-up). Seen in play. Read in full (2026-11-04). |
| `sub_8215F7B0` | `RaceFlow::OnAbort` | | r3 = the race flow: `CarInputs::ClearAll`, then +9604 (the state) = 3. Leaf otherwise. No direct callers: registered by `RaceFlow::Init` as state 32's handler (event 0), reached by message 44 from states 25 and 30 (probably an abort or a restart from the pause, back to waiting for a stage). Not seen in play. Read in full. |
| `sub_821022B0` | `CarInputs::ClearAll` | | The seven car input records (`[0x826C5B30 + 4i]`, i 0 to 6) cleared, each by `CarInput::Clear`. From `RaceFlow::OnAbort` only. Not seen in play. Read in full. |
| `sub_8215FC30` | `RaceFlow::OnState14` | | A jump to `RaceFlow::AiUnlessFlagged`. No direct callers: registered by `RaceFlow::Init` as state 14's handler (event 0), reached by message 22 from state 23. Not seen in play. Read in full. |
| `sub_8215FB08` | `RaceFlow::AiUnlessFlagged` | | r3, r4 not read: each racer (`[0x826D82F4]` + 2308 + 4i, its +20 the index; `Race::RacerCount` read again every pass) by its AI driver (`RacerControl::DriveByAi`), `Vehicle::Get` called on the index first (its answer unused), held (1) unless the racer's word +0 has 0x10 and lacks 0x4000 (then 0). Returns 1. From `RaceFlow::OnState14` only. Not seen in play (it reaches `DriveByAi`'s adapter, `src/hooks/racer_controllers.cpp`). Read in full. |
