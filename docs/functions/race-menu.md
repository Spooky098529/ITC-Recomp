# Known functions: the type 2 mode's race menu

The type 2 front-end mode's **race menu** (probably): the machine at the
mode block's +9640, set up by `RaceMenu::Init` `sub_8218CE10` from the
mode's Init `sub_82197278` and opened by message 8 (the hub's item 0;
UI movie 1, asset 22546). The mode and its other machines are in
[`type2-screens.md`](type2-screens.md), [`type2-dialog.md`](type2-dialog.md),
[`garage-screen.md`](garage-screen.md),
[`tuning-machine.md`](tuning-machine.md), [`car-dealer.md`](car-dealer.md),
[`options-screen.md`](options-screen.md),
[`save-screen.md`](save-screen.md) and the leads in `../loops/leads.md`.
It is where the career's races start: page 3 picks one of three race
modes (`+9820`, 0 to 2, strings 128 to 130 with a description 545 +
mode), page 4 one of up to 15 events (their names strings 360 to 373,
their kinds strings 395 +, a badge 401/402, 407 or 408; which are open
depends on the progress bits, player 0's save +0x199E0), page 5 one of
an event's two variants (the events below a count only), then the mode's
yes/no dialog asks a question (string 10; its answer written to player 0's
save +0x1F870, probably an option the player's car takes:
`../known-data.md` has it as the car object's +3892), and on its close
the screen fades out and `RaceMenu::StartEvent` sets the race up:
`Career::SetUpEvent(save, mode, location)` with the location the event
gives (0 to 19), then for locations 0 to 13 `Race::SetUp(save, -1)` (a
race at once; +9792 = 1), for 14 to 19 nothing more (the parking area's
locations, `location-screen.md`, where an opponent is picked; +9792 = 0).
`Destroy` then leaves the machine's +9604 at 11 (a race) or 12 (a
location), or on day 2 sets the boot movies' list `0x826C5BAC` (movie 1,
then the mode after it 1 or 5, as `CarDealer::StartFirstRace` does) and
+9604 = 20. Page 3's back fades out and returns to the hub (message 6).

It has the shape the type 2 mode's other machines have
(`type2-screens.md`'s intro; keys (state, message, 0, 0), member function
functors over vtable `0x82011918`, r3 the machine and r4 the mode, whose
message ring at +204 the handlers post to): (0, 1) `Create` → 1 or 9;
(1, 1) → 2 `OpenMovie`; (2, 1) `WaitForMovie` → 4 or 9; (9, 8)
`ScreenMachine::Reenter` → 4; (4, 1) → 5 `Start`; in 5, Draw (message 1)
runs the page's input `Draw` and Update (message 0) the transitions
`Update`; the dialog's replies (5, 38) `OnYes`, (5, 39) `OnNo`, (5, 41)
`OnBack`, (5, 42) `OnDialogClosed`; (5, 7) → 8 `Unload` (posted by page
9), (8, 3) `LeaveToHub`; (5, 4), (9, 4) and (10, 4) `TuningMachine::Close`
`sub_821D6A78` (shared, [`tuning-machine.md`](tuning-machine.md): +9604 =
7, message 4 posted) and (7, 4) → 6 `Destroy`. Its blocks are globals,
each `VirtualAlloc`ed (28, 28 and 68 bytes, `MEM_COMMIT`,
`PAGE_READWRITE`) by `Create` and freed by `Destroy`: `[0x826D8404]` the
page object (laid out as the hub's `0x826D8410`: +0 the page `Draw` runs,
-1 while a transition runs; +4 the page after; +8 the page before; +12
the transition `Update` runs, -1 none; +16 the one queued after it; +20;
+24 the transition's set-up done), `[0x826D8408]` a second page object
(-1 in every word but +24 after `Create`, written again only by `Back`;
`Draw` runs `Back` when its +0 is 7) and `[0x826D840C]` a front-end menu
(`Menu::Step` runs on it every page, its first and last item 0: the pages
move their own cursors). The machine's own words are in
`../known-data.md` ("race menu"). Pages (`Draw`'s jump table on the page
object's +0, 0 to 10): 3 `KindPage`, 4 `EventPage`, 5 `VariantPage`, 7
`Back`, 9 `LeavePage`, 10 `StartPage`; 0 to 2, 6 and 8 nothing (6 and 8
while the question shows). Transitions (`Update`'s jump table on +12, 0
to 12; each set up once while +24 is 0, then `UiMovie::Run(movie 1, 0)`
each call until it answers 0, then the queued one or the page after): 0
`Transition0`, 1 none (ends at once), 3 `Transition3`, 5 `Transition5`, 6
`AnimateVariantPage`, 11 `AskConfirm`, 12 `Transition12`; 2, 4, 7 to 10
nothing but the movie's run. Pages 3, 4 and 5 also print debug text
through the empty `sub_82106540` (`audio.md`) from `Update`.

In play (`../coverage.md`) the screen was opened, a mode and an event
(with a variant) picked, the question answered yes, and a race started;
backing out (pages 7 and 9, `Back`, `LeavePage`, `Unload`, `LeaveToHub`),
the dialog's no and back (`OnNo`, `OnBack`, `Transition12`) never ran (8
of the 38 rows not seen in play).

Names are ours (guesses from what the code does). Read 2026-11-10 by the
B7 loop's mapping agent: the Init through
`itc_recomp/tools/init_step_tables.py` (17 transitions, the lead's list),
the handlers, pages, transitions and small helpers in full, the
refresh (`Refresh`), the two tables' set-up (`SetUpEvents`) and the
pages' animation functions (`AnimateKindPage`, `AnimateEventPage`,
`AnimateVariantPage`, `Transition3`) in outline (their calls, the fields
they read and write, their branches). Ready (`b7_groups.py`): 21 groups,
their roots the Init, `Create`, `OpenMovie`, `WaitForMovie`, `OnYes`,
`OnDialogClosed`, `Destroy`, `Refresh`, `SetUpEvents`,
`SetEventRange`, the page 3 piece `sub_8218C5DC` (with `KindPage` and
`AnimateKindPage`), `EventPage` (with `AnimateEventPage`; 973 lines),
`AnimateVariantPage`, `StartPage` (with `StartEvent` and its piece),
`Transition0`'s piece `sub_8218C8A8` (with `Transition0`),
`Transition3`, `Transition5`, `AskConfirm` and the three debug pages.
`Start`, `Draw` (2,002 lines with its group: split the migration) and
`Update` wait only on groups here. Nothing outside the subsystem blocks
it. The indirect calls are the action mapper's slots 10 and 12
(`ActionMapper::Pressed`, `Fired`, [`action-mappers.md`](action-mappers.md),
through player 0's mapper entry `0x826BAB80` + 8 × `[0x826BABA4]`, as
`Menu::Step` reads it) and the sound manager's (`[0x826C5C80]`) slots 13
and 14 (`AudioManager::StopCue`, `IsCuePlaying`,
[`sound-cues.md`](sound-cues.md)).

Shared, with their rows elsewhere: `StateMachine::Add` `sub_82106158`
([`game-modes.md`](game-modes.md)), `ScreenMachine::Reenter`
`sub_822895E0` ([`location-screen.md`](location-screen.md)),
`TuningMachine::Close` `sub_821D6A78`
([`tuning-machine.md`](tuning-machine.md)), `Menu::Step` `sub_82186568`
([`menu-helpers.md`](menu-helpers.md)), `MenuSound::Play` `sub_821103F0`
and `Sound::Play` `sub_82175850` ([`menu-sounds.md`](menu-sounds.md)),
`ScreenFades::FadeIn` `sub_8209D038` and `FadeOut` `sub_8209D0C8`
([`mode-tick.md`](mode-tick.md)), `SaveData::Of` `sub_82105620`
([`vehicle-accessors.md`](vehicle-accessors.md)), `Career::SetUpEvent`
`sub_82114268` ([`career.md`](career.md)), `Race::SetUp` `sub_82115D98`
([`race-setup.md`](race-setup.md)), the empty `sub_82106540`
([`audio.md`](audio.md)), `memcpy` `sub_825A7830`
([`c-runtime.md`](c-runtime.md)), `VirtualAlloc` `sub_82295398` and
`VirtualFree` `sub_82295330` ([`heap.md`](heap.md)), the UI movie calls
`UiMovies::Open` `sub_8211E808`, `IsLoaded` `sub_8211BE98`, `Find`
`sub_8211B3F0`, `Delete` `sub_8211E258`, `UiMovie::ResetPose`
`sub_8211ABA8`, `FindNode` `sub_8211A578`, `ShowNode` `sub_8211AC00`,
`HideNode` `sub_8211AC48`, `SetNodeTexture` `sub_8211A708`, `SetText`
`sub_8211C1F8`, `SetNumberText` `sub_8211C740`, `SetChildNameText`
`sub_8211D930`, `ScrollText` `sub_8211C570`, `sub_8211C5F8` (the text's
scroll reset), `AnimationTime` `sub_8211BA00`, `Play` `sub_8211D978`,
`Loop` `sub_8211DDD8`, `Finish` `sub_8211DE38`, `PlayNamed`
`sub_8211DAC8`, `FinishNamed` `sub_8211DF20`, `FinishAll` `sub_8211DFF8`,
`Run` `sub_8211E1B8` ([`ui-movies-game.md`](ui-movies-game.md)) and
`UiMovie::Child` `sub_8259D658` ([`ui-movies.md`](ui-movies.md)).

Notes for the migration: `AskConfirm`'s and `EventPage`'s three
`UiMovie::SetText` calls on movie 16 leave r7 to r10 unset, and
`Refresh`'s calls but the day's (string 8, r7 the day) too (register
leftovers: native callers pass 0, the user's call of 2026-10-30,
"Precedent first" in `../loops/b7-loop.md`). `AnimateKindPage` stores
`Pressed(158)`'s answer into its frame's +80 and never reads it. `Create`
writes 0 to `[0x826D8404]` and `[0x826D840C]` before allocating them
(not `[0x826D8408]`). `SetUpEvents` builds its two tables in its frame
with overlapping stores between two `memcpy`s: its frame words are part
of what it leaves. `Draw`'s case 3, `StartEvent`'s case 0 and
`Update`'s transition 0 are pieces the recompiler split off as
functions (`sub_8218C5DC`, `sub_82188D44`, `sub_8218C8A8`): their work
goes inside their parents' plain C++ (Merged).

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_8218CE10` | `RaceMenu::Init` | Overridden | r3 = the race menu's machine (the type 2 mode block's +9640): 17 transitions through `StateMachine::Add` (`sub_82106158`), keys (state, message, 0, 0), each a member function functor over vtable `0x82011918` (the machine, the function) copied into the frame (six-word copy loops), the next state at the copy's +24 (0 for none): (0, 1) `Create` `sub_82188E20`; (1, 1) → 2 `OpenMovie` `sub_821889E0`; (2, 1) `WaitForMovie` `sub_821890D8`; (4, 1) → 5 `Start` `sub_8218C9C8`; (5, 1) `Draw` `sub_8218C578`; (5, 0) `Update` `sub_8218C678`; (5, 38) `OnYes` `sub_821895F0`; (5, 39) `OnNo` `sub_82189648`; (5, 41) `OnBack` `sub_82188938`; (5, 42) `OnDialogClosed` `sub_821896A0`; (5, 7) → 8 `Unload` `sub_8218AF08`; (8, 3) `LeaveToHub` `sub_8218AF80`; (9, 8) `ScreenMachine::Reenter` `sub_822895E0`; (5, 4), (9, 4) and (10, 4) `TuningMachine::Close` `sub_821D6A78`; (7, 4) → 6 `Destroy` `sub_82188FE0`. A 1,232-byte frame (r27 to r31 saved). It decodes as the other Inits' step tables (`itc_recomp/tools/init_step_tables.py`: 221 stores, the functors' words the original never writes copied as the frame holds them). Only caller the type 2 mode's Init `sub_82197278` (a lead). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::Init` (`src/race_menu/init.cpp`) over its step table (`init_tables.h`, from `itc_recomp/tools/init_step_tables.py`), `StateMachine::Add`'s plain C++; harness `race_menu`. Its caller, the type 2 mode's Init, is recompiled: the adapter stays (`src/hooks/race_menu.cpp`, `--native_race_menu`). |
| `sub_82188E20` | `RaceMenu::Create` | Overridden | (0, 1): r3 = the machine. The page +9640 = 0; +9608 (the movie loaded) = 0; +9648 = 0, +9652 = 1; +9716, +9724, +9728 = 0; byte +9776 (the page history's depth) = 0; +9780 (refresh) = 1, +9784 = 1; +9788, +9792, +9800, +9804 = 0; the history +9732 to +9772 (11 words) = 0; +9656 to +9712 (15 words) = 15, then +9656 = 7, +9660 = 8 and their count +9720 = 2 (a debug list, `DebugEventPage`). `[0x826D8404]` and `[0x826D840C]` = 0, then `VirtualAlloc(0, 28, 0x1000, 4)` (`sub_82295398`) into `[0x826D8404]` (the page object: +0, +4, +8 = 3, +12, +20 = -1, +24 = 0; +16 not written), again into `[0x826D8408]` (+0, +4, +8, +12, +20 = -1, +24 = 0) and `VirtualAlloc(0, 68, ...)` into `[0x826D840C]` (the menu: +0 to +64 = 0 but +44, +56, +60). Then +9604 = 1 when +9612 is set (the mode sets it: open on the message), else 9. A 144-byte frame (`__savegprlr_26`). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::Create` (`src/race_menu/handlers.cpp`), `VirtualAlloc` through its override; harness `race_menu` (the blocks made compared by content, then freed). A functor the machine calls: the adapter stays (`src/hooks/race_menu.cpp`, `--native_race_menu`). |
| `sub_821889E0` | `RaceMenu::OpenMovie` | Overridden | (1, 1) → 2: `UiMovies::Open(1, 22546)` (`sub_8211E808`); +9608 = 0. A 96-byte frame (r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::OpenMovie` (`src/race_menu/handlers.cpp`), `UiMovies::Open` through its override; harness `race_menu`. A functor: the adapter stays. |
| `sub_821890D8` | `RaceMenu::WaitForMovie` | Overridden | (2, 1): while +9608 is 0, +9608 = 1 once `UiMovies::IsLoaded(1)`; once set, +9780 = 1 and +9604 = 9 when +9612 is set, else 4. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::WaitForMovie` (`src/race_menu/handlers.cpp`), `UiMovies::IsLoaded`'s plain C++; harness `race_menu`. A functor: the adapter stays. |
| `sub_8218C9C8` | `RaceMenu::Start` | Overridden | (4, 1) → 5: r3 = the machine. +9788, +9796, +9812, halfword +9816, +9820 to +9836, +9972, +9976, +10100 to +10108, +9648 = 0; +9644 = 3, +9640 = 3 (page 3), +9780 = 1, +9784 = 1. Player 0's save data (`SaveData::Of(0)`) into +10144; its +0x199E0 (the progress bits) into +10148; its +8 to +27 (the name) into +10112 to +10131; +6116 (the money) into +10132; +6100 (the day, at most 999) into +10136 and the day mod 7 (the weekday) into +10140. `SetUpEvents`, `SetEventRange`. Movie 1 (`UiMovies::Find`): `UiMovie::ResetPose`; `Refresh`; floats set on nodes found by `UiMovie::FindNode` (scene, child): (19, 0), (20, 0), (8, 0) +16 = `[0x82000A34]`; (11, 2), (11, 3), (11, 4), (11, 0) +12 = `[0x82002228]`, +16 = `[0x8200A134]`; (11, 1) +12 = `[0x82000C34]`, +16 = `[0x82000A34]`; nodes (11, 20) to (11, 23) hidden, then shown by the progress bits: 0x20 all four; else 0x4 or 0x8 21 to 23; else 0x10 21 and 22; else 22; (10, 0) +12, +16 = `[0x82000A34]`. The page object: +0, +4, +8 = the page (3), +12, +20 = -1, +24 = 0; the menu: +8, +12, +16, +20, +24, +28, +36, +44, +56, +64 = 0, +60 = 8, +0, +4 = +9648; then the page object again (+8 the old page unless the same, +0 = -1, +4 = 3, +12 = 0 (transition 0), +16 = -1). Sounds through the sound manager `[0x826C5C80]`: when cue 2 of bank 1 plays (slot 14, `IsCuePlaying`) it's stopped (slot 13, `StopCue`), and unless cue 0 of bank 1 plays, `Sound::Play(1, 0)` (`sub_82175850`; probably the menu's music, the hub's being cue 2). `ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0)`. A 176-byte frame (r28 to r31, f29 to f31). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::Start` (`src/race_menu/start.cpp`), `SaveData::Of`, `SetUpEvents`, `SetEventRange`, `Refresh` and `UiMovie::FindNode` as plain C++, `ResetPose`, the shows and hides, the sound manager's slots 14 and 13, `Sound::Play` and `FadeIn` through their overrides; the nodes' floats (`lfs`/`stfs` of constants) copied as words; harness `race_menu` (the progress bits' shapes, the day 0 to 999, around 999, above it and any, the money, the name record; a sound manager whose slots 14 and 10 are the getter `sub_82090770` and 13 the recorder `sub_820A8420`; its frame and `Refresh`'s compared, `SetUpEvents`' register saves left there not). Read again for the port: node (11, 0) gets +16 = `[0x82000A34]` then +12 = `[0x82000C34]` (as (11, 1)), not `[0x82002228]`/`[0x8200A134]` (only (11, 2) to (11, 4) do). Its caller, the machine's transition (a functor), is recompiled: the adapter stays (`src/hooks/race_menu.cpp`, `--native_race_menu`). |
| `sub_821891C8` | `RaceMenu::SetUpEvents` | Native | r3 = the machine. Builds two 15-entry tables of 8-byte pairs by the progress bits (+10148: 0x20; else 0x4 or 0x8; else 0x10; else none): `memcpy(+9980, frame +80, 120)` (`sub_825A7830`; each pair's +0 the event's kind, its strings 395 + kind, and +4 a second kind or -1, the variant `VariantPage` offers), then the frame changed and `memcpy(+9852, frame +80, 120)` (each pair's +0 the event's type 0 to 14, `StartEvent`'s jump table, and +4 its badge 0 to 2, `Refresh`'s strings 401/402, 407, 408; entries past the open events -1). Writes its frame's words +80 to +199 with overlapping stores between the copies. A 288-byte frame (`__savegprlr_22`). Only caller `Start`. Seen in play. Read in outline (the stores, not every pair's value). Native (B7 loop, 2026-11-10): `race_menu::SetUpEvents` (`src/race_menu/events.cpp`), its frame's words +80 to +199 stored in the original's order (the overlapping stores kept) and both copies through the second memcpy's plain C++ (`c_runtime::CopyForward`, under `--native_c_runtime`, else the guest call); harness `race_menu` (the progress bits' shapes, its frame compared). Its only caller `Start` is native (B7 loop, 2026-11-10) and calls its plain C++: the adapter removed (an attract race showed it uncalled), the fallback flag `--native_race_menu` now switches only `Start`; the harness runs its register step. |
| `sub_82189158` | `RaceMenu::SetEventRange` | Native | r3 = the machine. By the progress bits (+10148): 0x20: +9844 (the last event) = 14, +9848 (the events with a variant below it) = 9; else 0x4 or 0x8: 12, 8; else 0x10: 7, 5; else 5, 4; +9840 (the first) = 0. Leaf. Only caller `Start`. Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::SetEventRange` (`src/race_menu/handlers.cpp`); harness `race_menu`. Its only caller `Start` is native (B7 loop, 2026-11-10) and calls its plain C++: the adapter removed (an attract race showed it uncalled), the fallback flag `--native_race_menu` now switches only `Start`; the harness runs its register step. |
| `sub_8218BC18` | `RaceMenu::Refresh` | Overridden | r3 = the machine: movie 1's texts for the page; returns 1, or 0 for pages other than 3 to 6 (`Update` keeps +9780 set while it answers 0). Always: `SetChildNameText(m, 0, 6, +10112)` (the name), `SetNumberText(m, 0, 9, +10132)` (the money), `SetText(m, 0, 13, 8, the day)`, `SetNodeTexture(m, 0, 11, the weekday)`, nodes (2, 2) and (2, 10) hidden, `SetNodeTexture` (2, 6) and (2, 7) = 1, nodes (10, 1) to (10, 15) hidden. Page 3: textures (15, 3) to (15, 5) = the mode (+9820), `SetText` (19, 5) to (19, 7) = 128 to 130, (1, 0) = 545 + mode, `ScrollText(m, 1, 0, 1)`. Page 4: the same textures; node (-1, 17) hidden at the list's top (+9828 - +9972 = +9840) else shown, (-1, 18) hidden at its bottom; the list's three rows (20, 5) to (20, 7) = the event names (strings 360 to 373, a 15-entry list by the progress bits) from the visible row; with the scroll flag +9836 1 or 2 the row scrolled in (20, 8) or (20, 15) set and +9836 = 0; (8, 5) = 395 + the kind (+9980 pair), (8, 6) = 395 + the variant's kind or hidden when -1; node (10, type + 1) shown and the children of nodes (10, 1) to (10, 15) set by type (a jump table: children 2, 3, 6 to 9's +80); (8, 0) hidden; badges (11, 20) to (11, 23) by the pair's +4: 0 → 20 (with bit 0x20) and 21 shown, texts 402, 401; 1 → 22, 407; 2 → 23, 408; (1, 0) = 548. Page 5: node (10, type + 1) shown, its children by type and the variant (+9724; a jump table), the badges as page 4's, (1, 0) = 549. Page 6: nothing (returns 1). A 224-byte frame (`__savegprlr_26`). From `Start` and `Update`. Seen in play. Read in outline (every call and branch, not each jump table's case). Native (B7 loop, 2026-11-10): `race_menu::Refresh` (`src/race_menu/refresh.cpp`), `FindNode` and `Child` as plain C++, the setters through their overrides; `SetText`'s r7 to r10 left unset by the call sites passed 0 (the day's r7 kept), the user's call of 2026-10-30; harness `race_menu` (pages 0 to 10, the cursor, scroll flag and tables shaped; the parameter area +48 to +79 left out). With bit 0x20 page 4's list is 15 names, 360 to 374; with fewer (13, 8 or 6) the rows past them read the frame's words as the caller left them, which the native code reads at the same addresses. Its caller `Start` is native and calls its plain C++ (2026-11-10); `Update` is recompiled: the adapter stays until it's ours. |
| `sub_8218C578` | `RaceMenu::Draw` | | (5, 1), every Draw: r3 = the machine, r4 = the mode. With +9784 set: the page object's +0 (0 to 10, a jump table) picks: 3 `KindPage` (through the piece `sub_8218C5DC`), 4 `EventPage`, 5 `VariantPage`, 7 `Back`, 9 `LeavePage`, 10 `StartPage` (r3, r4 passed on); the others nothing. Then `Back` when the second page object's +0 (`[0x826D8408]`) is 7 (not 6). A 112-byte frame (r30, r31 saved). Seen in play. Read in full. |
| `sub_8218C5DC` | | | Not a function: `Draw`'s case 3 (page 3) that the recompiler split off: `KindPage(r31, r30)`, then `Draw`'s tail (the second page object's check) and epilogue. Its work goes inside `Draw`'s plain C++. Seen in play. Read in full. |
| `sub_8218AFD8` | `RaceMenu::KindPage` | | Page 3, the race mode: r3 = the machine. `Menu::Step([0x826D840C])`; then player 0's action mapper (`0x826BAB80` + 8 × `[0x826BABA4]`; nothing read when its +4 is 1): fired (slot 12) 153 or 167 (up): `MenuSound::Play(4)`, +9780 = 1, +9824 = the mode, the mode (+9820) - 1 (below 0 → 2), +9648 = it; 154 or 168 (down): the same, + 1 (above 2 → 0); pressed (slot 10) 159 (A): `MenuSound::Play(0)`; +9972, +9976, +9828, +9832, +9812, halfword +9816, +9716 = 0; +9780 = 1; +9824 = +9648 = the mode; +9644 = the page (3), +9640 = 4; history[+9776] (+9732 + 4n) = 3 and the depth + 1; the page object to page 4 through transition 3 (+0 = -1, +8 the old page unless the same, +12 = 3, +16 = -1); 158 (B): `MenuSound::Play(2)`; +9824 = +9648 = the mode; +9640 = 9; the page object to page 9 at once (+0, +4, +8 = 9, +12, +20 = -1, +24 = 0, then +0 = 9, +12 = +16 = -1); `ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0)`. Last `AnimateKindPage`. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. |
| `sub_821898C0` | `RaceMenu::AnimateKindPage` | | r3 = the machine: movie 1's animations for page 3's input (it reads the mapper again, as `KindPage`): A: `sub_8211C5F8(m, 1, 0)` (the text's scroll reset), `Finish(m, 19, mode + 2)`, `Finish(m, -1, 0)`; B: `Pressed(158)` stored to its frame and not used; up or down: the scroll reset, animations 12 or 13 of scene 19 finished or played by the step (from mode 0 to 2 or 2 to 0 both, with node (19, 0)'s +16 = (float)(mode × 40)), then `Finish`, `Play(m, 15, 1, 1)`, `Finish(m, 19, old mode + 2)`, `Play` and `Loop(m, 19, mode + 2)`. A 128-byte frame (`__savegprlr_29`). Only caller `KindPage`. Seen in play. Read in outline. |
| `sub_8218B2C8` | `RaceMenu::EventPage` | | Page 4, the event: r3 = the machine, r4 = the mode. `Menu::Step([0x826D840C])`; then the mapper: up (153, 167): `MenuSound::Play(4)`; the visible row +9972 - 1 (below 0: 0, scroll flag +9836 = 1), +9976 the old row; the cursor +9828 - 1, +9832 the old; below +9840 the cursor = +9844 and the row = 2 (wraps); +9780 = 1; +9716 = the cursor. Down (154, 168): the same upward (the row above 2: 2, +9836 = 2; above +9844: the first, row 0). A (159): `MenuSound::Play(0)`; +9836 = 0, +9644 = the page, +9716 = the cursor; when +9848 is above +9716 (read before it's written; the steps keep it the cursor): page 5 through transition 5 (+9640 = 5, halfword +9816, +9724, +9812 = 0, +9780 = 1); else page 6 (+9640 = 6; halfword +9816, +9728, +9784 = 0, +9780 = 1) and the question asked at once as `AskConfirm` does (movie 16's three texts, its +40960 by save +0x1F870, page object +0 = 8 and message 34 posted on the mode's ring); either way history[+9776] = 4 and the depth + 1. B (158): `MenuSound::Play(2)`; +9716 = the cursor, +9780 = 1, +9836 = 0, +9644 = the page, +9640 = 3, halfword +9816 = 0; the page object to page 3 through transition 0 (written twice). Last `AnimateEventPage`. A 160-byte frame (`__savegprlr_25`). Seen in play. Read in full. |
| `sub_82189FE8` | `RaceMenu::AnimateEventPage` | | r3 = the machine: movie 1's animations for page 4's input (reads the mapper again): A: the scroll reset and `Finish` of (20, row + 2), (-1, 1), (11, 12) and (-1, 6) or (10, 0) by +9848 against +9716; B: the scroll reset, a run of `Finish`es and `Play(m, ...)`/`Loop` of the mode page's animations; up and down: the row's animations (scene 20, the old row +9976 and the new +9972, + 2), the list's ends (+9840, +9844), the badges (scene 11, 7 and others by the old and new pair's +4) and node 10's; then with +9836 1 or 2, `FinishNamed` and `PlayNamed(m, 148 or 149, 1)` (the list's scroll). A 128-byte frame (`__savegprlr_29`). Only caller `EventPage`. Seen in play. Read in outline. |
| `sub_8218B750` | `RaceMenu::VariantPage` | | Page 5, the event's variant: r3 = the machine. `Menu::Step([0x826D840C])`; then the mapper: up (153, 167): +9780 = 1, +10104 = the old choice +10100; when it was set, `MenuSound::Play(4)`; +10100 = 0; down (154, 168): +9780 = 1, +10104 = +10100; when the event has a variant (the +9980 pair's +4 not -1) and the choice wasn't 1, `MenuSound::Play(4)`, +10100 = 1; +9724 = +10100. A (159): `MenuSound::Play(0)`; +9728, halfword +9816 = 0; +9724 = the choice, +9780, +9800 = 1, +9644 = the page, +9640 = 6; the page object (+8 the old page unless -1, +12 = 11 `AskConfirm`, +4 = +16 = +0 = -1). B (158): `MenuSound::Play(2)`; +9724 = the choice, +9780 = 1, halfword +9816 = 0, +9644 = the page, +9640 = 4; the page object to page 4 through transition 3. Last `AnimateVariantPage`. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. |
| `sub_8218A928` | `RaceMenu::AnimateVariantPage` | | r3 = the machine: movie 1's animations for page 5's input (reads the mapper again); also `Update`'s transition 6. A: `Finish` of (8, 2), (8, 3), (-1, 2), (11, 12), (10, 0); B: the scroll reset, `Finish` of (8, 2), (8, 3), (10, 0), (-1, 2), (11, 12), (-1, 6), (-1, 4), `Play(m, -1, 4, 1)`; with +10100 1, `Finish`, `Play(m, 8, 12, 1)` and +10100 = +10104 = 0; then `Finish` and `Play(m, 20, row + 28, 1)`; down (with a variant and +10104 0): (8, 2) finished, `Play` and `Loop(m, 8, choice + 2)`, (8, 13) finished and played; up (with +10104 1): (8, 3) finished, the same with (8, 12). A 128-byte frame (`__savegprlr_29`). From `VariantPage` and `Update`. Seen in play. Read in full. |
| `sub_82188A20` | `RaceMenu::Back` (probably) | Overridden | Page 7, and when the second page object's +0 is 7: r3 = the machine. The history's depth (byte +9776) - 1; the page +9640 = history[depth] (+9732 + 4n); the page object: +0, +4, +8 = it, +12, +20 = -1, +24 = 0; the second page object: +0, +4, +8, +12, +20 = -1, +24 = 0; +9780 = 1. Leaf. From `Draw` and its piece `sub_8218C5DC` (the tail's check). Not seen in play. Read in full. Native (B7 loop, 2026-11-10, never seen in play): `race_menu::Back` (`src/race_menu/handlers.cpp`); harness `race_menu`. Its callers `Draw` and the piece `sub_8218C5DC` are recompiled: the adapter stays until they're ours. |
| `sub_8218B9F0` | `RaceMenu::LeavePage` | | Page 9, after page 3's back: r3 = the machine, r4 = the mode. Waits for the fades (`[0x826A50E4]` and `[0x826A50DC]` both 0); then on the sound manager: cue 0 of bank 1 stopped when it plays, cue 2 of bank 1 played (`Sound::Play(1, 2)`) unless it plays (the hub's music back); message 7 posted on the mode's ring; +9808 = 6 (the message `LeaveToHub` posts). A 176-byte frame (`__savegprlr_26`). Not seen in play. Read in full. |
| `sub_8218BB18` | `RaceMenu::StartPage` | | Page 10, after the question's close: r3 = the machine. Once both fades' counts (`[0x826A50E4]`, `[0x826A50DC]`) are 0: `StartEvent`, +9604 = 10 (the machine closes on message 4). A 96-byte frame (r31 saved). Seen in play. Read in full. Was in the lead of `Race::SetUp`'s callers' neighbors (`../loops/leads.md`). |
| `sub_82188CE8` | `RaceMenu::StartEvent` | | r3 = the machine. The location by the event's type (the +9852 pair of the cursor +9828; a jump table, 0 to 14, through the piece `sub_82188D44` for 0): 0 the variant +9724; 1 +9724 + 2; 2 +9724 + 4; 3 6; 4 7; 5 +9724 + 8; 6 +9724 + 10; 7 12; 8 13; 9 to 14 → 14 to 19; above 14 → 0. `Career::SetUpEvent(save +10144, the mode +9820, location)` (`sub_82114268`); location 14 to 19: +9792 = 0; else +9792 = 1 and `Race::SetUp(save, -1)` (`sub_82115D98`). A 112-byte frame (r30, r31 saved). Only caller `StartPage`. Seen in play. Read in full. Was in the lead of `Race::SetUp`'s callers (`../loops/leads.md`). |
| `sub_82188D44` | | | Not a function: `StartEvent`'s case 0 that the recompiler split off: the location = +9724, then `StartEvent`'s tail (the set-up calls) and epilogue. Its work goes inside `StartEvent`'s plain C++. Seen in play. Read in full. Was in the lead of `Race::SetUp`'s callers (`../loops/leads.md`). |
| `sub_8218C678` | `RaceMenu::Update` | | (5, 0), every Update: r3 = the machine, r4 = the mode. A debug print through the empty `sub_82106540(160, 80, 0, 0x8200A174)`. Movie 1: when `AnimationTime(m, -1, 1)` is `[0x82001FB0]` or `AnimationTime(m, -1, 4)` is `[0x82001FD8]`, animation (-1, 6) finished, played and looped; when `AnimationTime(m, -1, 2)` is `[0x82001FD8]`, or +9796 is 1 on page 5: node (8, 0) shown and animation (8, +10100 + 2) finished, played and looped. With +9780 1: `Refresh`, +9780 = 1 when it answers 0, else 0. By the page +9640: 3 `DebugKindPage`, 4 `DebugEventPage`, 5 `DebugVariantPage`. Then the page object's transition +12 (a jump table, 0 to 12): -1 `UiMovie::Run(movie 1, 0)` only; above 12 `Run` and the transition ended; else with +24 0 the set-up (+24 = 1): 0 `Transition0` (through the piece `sub_8218C8A8`), 1 ended at once (the queued one or the page after), 3 `Transition3`, 5 `Transition5`, 6 `AnimateVariantPage`, 11 `AskConfirm` (r4 the mode), 12 `Transition12`, others nothing; then `Run(movie 1, 0)`; when it answers 0 the queued transition (+16) runs next (+16 = -1, +20 = 0) or, with none, +0 = +4 (the page); +24 = 0. A 176-byte frame (`__savegprlr_23`, f31). Seen in play. Read in full. |
| `sub_8218C8A8` | | | Not a function: `Update`'s transition 0 that the recompiler split off: `Transition0(r30)`, then `Update`'s `Run` and the transition's end, and its epilogue. Its work goes inside `Update`'s plain C++. Seen in play. Read in full. |
| `sub_82189740` | `RaceMenu::Transition0` | | Transition 0 (to page 3): r3 = the machine. Movie 1; the first time (+9812 0): `Finish` and `Play(m, -1, 0, 1)`, (0, 0) finished, played and looped, (-1, 5) and (9, 1) the same, +9812 = 1; then animation (19, mode + 2) finished, played and looped. A 128-byte frame (`__savegprlr_29`). Only caller the piece `sub_8218C8A8`. Seen in play. Read in full. |
| `sub_82189C28` | `RaceMenu::Transition3` | | Transition 3 (to page 4): r3 = the machine. Movie 1; the first time (+9812 0): node floats as `Start` sets them ((20, 0), (11, 2) to (11, 4), (11, 0), (11, 1), (10, 0); (10, 0)'s +80 = 1), animation (11, 5, 4, 3 or 2 by the progress bits) finished and played, (-1, 1) and (-1, 5) the same, +9812 = 1; then (10, 0) and (20, row + 2) finished, played and looped; nodes (10, 1) to (10, 9)'s children 6 to 9 shown by a jump table on the index; with +9796 set the transition ended at once (the queued one or the page after) and +9796 = 0. A 176-byte frame (`__savegprlr_25`, f29 to f31). Only caller `Update`. Seen in play. Read in outline. |
| `sub_8218A7F0` | `RaceMenu::Transition5` | | Transition 5 (to page 5): r3 = the machine. Movie 1; the first time (+9812 0): (20, row + 14) finished and played, (-1, 2) the same, +9812 = 1; then (11, 12) finished, played and looped; with +9796 set the transition ended at once and +9796 = 0. A 128-byte frame (`__savegprlr_29`). Only caller `Update`. Seen in play. Read in full. |
| `sub_8218ACE8` | `RaceMenu::AskConfirm` | | Transition 11 (the question): r3 = the machine, r4 = the mode. +9784 = 0; movie 16 (the mode's yes/no dialog, `type2-dialog.md`): `SetText(m, -1, 0, 10)` (the question), `SetText(m, 1, 3, 0)`, `SetText(m, 1, 4, 1)`; m's +40960 (the dialog's first cursor) = 0 (yes) when player 0's save +0x1F870 is set, else 1 (no). The page object: +8 the old page unless 8, +0 = 8, +12 = +16 = -1; message 34 (open the yes/no dialog) posted on the mode's ring (+204, its count +600, start +604); then the transition ended (the queued one, none, so +0 = +4) and +24 = 0. A 128-byte frame (`__savegprlr_29`). Only caller `Update`. Seen in play. Read in full. |
| `sub_8218AE48` | `RaceMenu::Transition12` | | Transition 12 (back from the question, `OnBack`): movie 1's animation (10, 0) finished, played and looped; the transition ended at once (the queued one: 5 or 3). A 112-byte frame (r31 saved). Only caller `Update`. Not seen in play. Read in full. |
| `sub_8218BB68` | `RaceMenu::DebugKindPage` | | Page 3's debug text: the empty `sub_82106540` (`audio.md`) at (170, 100) (`0x8200A158`), a line at the menu's cursor (`0x8200A044`, y = (+0 + 8) × 15) and three strings (`0x8200A144`, `0x8200A14C`, `0x8200A138`, format `0x8200A040`). Nothing else. A 128-byte frame (`__savegprlr_29`). Only caller `Update`. Seen in play. Read in full. |
| `sub_82188A98` | `RaceMenu::DebugEventPage` | | Page 4's debug text: the empty `sub_82106540` with a line at the menu's cursor and the +9720 entries of +9656 as indexes into 15 strings (`0x8200A048` to `0x8200A100`; the title `0x8200A0E4`, the cursor's line `0x8200A044`). Nothing else. A 240-byte frame (`__savegprlr_21`). Only caller `Update`. Seen in play. Read in full. |
| `sub_82188C00` | `RaceMenu::DebugVariantPage` | | Page 5's debug text through the empty `sub_82106540` (the title `0x8200A11C`, the strings `0x8200A10C` and `0x8200A114`, the cursor's line `0x8200A044`); then when the menu's +8 (`[0x826D840C]`, the last step's direction) isn't 0: the page object to page 5 through transition 10 (+8 the old page unless 5, +16 = -1, +12 = 10, +4 = 5, +0 = -1). A 128-byte frame (`__savegprlr_28`). Only caller `Update`. Seen in play. Read in full. |
| `sub_821895F0` | `RaceMenu::OnYes` | Overridden | (5, 38): r3 = the machine. +9788 (answered) = 1; player 0's save +0x1F870 = 1; +10108 = 0. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::OnAnswer` (`src/race_menu/handlers.cpp`), `SaveData::Of`'s plain C++; harness `race_menu`. A functor: the adapter stays. |
| `sub_82189648` | `RaceMenu::OnNo` | Overridden | (5, 39): as `OnYes` with save +0x1F870 = 0 and +10108 = 1. Not seen in play. Read in full. Native (B7 loop, 2026-11-10, never seen in play): `race_menu::OnAnswer` (`src/race_menu/handlers.cpp`); harness `race_menu`. A functor: the adapter stays. |
| `sub_82188938` | `RaceMenu::OnBack` | Overridden | (5, 41): r3 = the machine. +9796 = 1, +10108 = 0; by +9644 (the page before the question): 5 → +9640 = 5, +9780 = 1, +10104 = +10100, the page object (+8 the old page unless 5, +16 = 5, +0 = -1); 4 → +10100 = +10104 = 0, +9640 = 4, +9780 = 1, the page object (+8 likewise, +16 = 3, +0 = -1); then +4 = that page and +12 = 12 (`Transition12`, then 5 or 3); other pages nothing. Leaf. Not seen in play. Read in full. Native (B7 loop, 2026-11-10, never seen in play): `race_menu::OnBack` (`src/race_menu/handlers.cpp`); harness `race_menu`. A functor: the adapter stays. |
| `sub_821896A0` | `RaceMenu::OnDialogClosed` | Overridden | (5, 42): r3 = the machine. Movie 1; when +9788 (answered) is set: `ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0)`, `UiMovie::Run(m, 0)`, +9640 = 10, the page object (+8 the old page unless 10, +0 = 10, +12 = +16 = -1). +9784 = 1. A 128-byte frame (`__savegprlr_29`). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::OnDialogClosed` (`src/race_menu/handlers.cpp`), `UiMovies::Find`'s plain C++, the fade and `UiMovie::Run` through their overrides; harness `race_menu`. A functor: the adapter stays. |
| `sub_8218AF08` | `RaceMenu::Unload` | Overridden | (5, 7) → 8: r3 = the machine. `UiMovie::FinishAll(movie 1)`; when +9616 is set, `UiMovies::Delete(1)` and `Delete(16)` and +9608 = 0. A 128-byte frame (`__savegprlr_29`). Not seen in play. Read in full. Native (B7 loop, 2026-11-10, never seen in play): `race_menu::Unload` (`src/race_menu/handlers.cpp`), `FinishAll` and `UiMovies::Delete` through their overrides; harness `race_menu` (Delete's calls recorded). A functor: the adapter stays. |
| `sub_8218AF80` | `RaceMenu::LeaveToHub` | Overridden | (8, 3): r3 = the machine, r4 = the mode: message (+9808, 0, 0) (6, the hub, from `LeavePage`) posted on the mode's ring; +9604 = 9. Leaf. Not seen in play. Read in full. Native (B7 loop, 2026-11-10, never seen in play): `race_menu::LeaveToHub` (`src/race_menu/handlers.cpp`); harness `race_menu`. A functor: the adapter stays. |
| `sub_82188FE0` | `RaceMenu::Destroy` | Overridden | (7, 4) → 6: r3 = the machine. `VirtualFree(..., 0, MEM_RELEASE)` (`sub_82295330`) on `[0x826D8404]`, `[0x826D8408]`, `[0x826D840C]` (the globals left as they are). With the day (+10136) 2: the boot movies' list (`0x826C5BAC`, `boot-mode.md`): +0 = 1 (movie 1), +4 to +16 = 11 (none), +20 (the mode after them) = 1 when +9792 is 1, else 5, +24 = 1, +28 = 0; +9604 = 20. Else +9604 = 11 when +9792 is 1 (a race set up), else 12 (a parking area location). A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-10): `race_menu::Destroy` (`src/race_menu/handlers.cpp`), `VirtualFree` through its override; harness `race_menu` (whether each block was freed compared). A functor: the adapter stays. |
