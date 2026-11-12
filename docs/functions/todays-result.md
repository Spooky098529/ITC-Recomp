# Known functions: the type 9 mode (today's result)

The type 9 front-end mode (`ModeType9::`, the name ours): the career's
**end of the day**, after a career race. Its two screens' updates pass
the strings "TXR_MenuTodaysResult" and "TXR_TodaysEvent" to an empty
function (a debug print stripped from the release, probably), which name
them: **today's result** (`TodaysResult::`, the day's and the career's
counts, the day number and the money, UI movie 27) and **today's
event** (`TodaysEvent::`, the unlocks just made, one page each with
their images, UI movie 0). The race's end on the career's side
(`sub_82150E00`, a lead in `../loops/leads.md`, from
`RaceMode::NextMode`, [`race-loading.md`](race-loading.md)) answers type
9 on several of its paths. The mode is 852 bytes like types 2, 4 to 7
and 10 (made by `ModeType9::ModeType9` in [`game-modes.md`](game-modes.md)),
released by `ModeType9::Release` `sub_8225D590`
([`front-end-screen-helpers.md`](front-end-screen-helpers.md)). When it
ends it asks for the type 2 mode (the career's garage) or, with a
movie queued in the boot movies' list (`0x826C5BAC`,
[`boot-mode.md`](boot-mode.md)), the boot mode (type 3), which plays it
and then goes on to type 2, or back to type 9 for movie 6.

Its vtable `0x82006CAC` has 14 slots: 0 `Init` `sub_8225D6D0`, 1
`Release`, 2 to 5 and 9 `Mode::NoNextMode` (`sub_8222FC90`), 6 `NextMode`
`sub_8225DC38`, 7 `Mode::DeletingDestructor` (`sub_82106E38`; both in
`boot-mode.md`), 8 `WaitForLoads` `sub_8225D918`, 10 `WaitForShown`
`sub_8225DA68`, 11 `LeaveScreen` `sub_8225DB70`, 12 `NextModeAnswer`
`sub_8225D638` and 13 `Leaving` `sub_8225DE78` (reached through the
slot thunks `Mode::SlotNThunk` in [`type2-mode.md`](type2-mode.md) and
`Mode::NextModeThunk` `sub_82235C08` in `boot-mode.md`). Its +32 vtable
`0x82006CA0` has three: `IsLoaded` `sub_8225D6A8`, `ForceExit`
`sub_8225D828` and the empty `sub_82106540` ([`audio.md`](audio.md)).
`NextModeAnswer` and `ForceExit` are shared with the type 10 mode and
keep their rows in [`type10-mode.md`](type10-mode.md); both read or
write the block's +19292, the same offset in both modes' blocks.

`Init` allocates the mode's block (19,300 bytes at +848, `VirtualAlloc`)
with two state machines and adds both to the mode's dispatcher: +0
**today's result** (`TodaysResult::InitStates` `sub_8225B358`) and +9640
**today's event** (`TodaysEvent::InitStates` `sub_8225CF30`); both open
on their message (+9612 = 1, +9616 = 0, so neither deletes its movie on
unload). The block's +19280 is the shown machine's index, as the
mode's helpers number them: 27 today's result (+0), 0 today's event
(+9640) (`ModeType9::FindShown`, which looks at indexes 0 to 46 and
knows only these two). `Init` shows today's event first when
`TodaysEvent::Kind` isn't 0 (unlocks were made, `SaveData::Of(0)`
+0x1CA78 > 0, and the save's +0x1D524 holds a movie 2 to 6), else
today's result alone. The flow, as the type 10 mode's: `WaitForLoads`
runs the loading screen until both machines' movies are loaded, posts
the shown machine's reenter message (6 for today's result, 8 for
today's event: their (9, 6) and (9, 8) `ScreenMachine::Reenter`
`sub_822895E0`, [`location-screen.md`](location-screen.md)), then
`NextMode` runs each tick (Draw and Update queued, the shown machine
watched): its state 8 goes through `LeaveScreen` (message 3) and
`WaitForShown` (until its movie is loaded again) back to `NextMode`; 10
queues message 4 (both machines' `Leave`, then `Destroy`); 13 answers
mode 2 (the garage) and 20 mode 3 (the boot mode, a movie queued) at
once.

Both machines have the type 2 mode's shape (`type2-screens.md`'s intro:
keys (state, message, 0, 0), member function functors over vtable
`0x82011918`, r3 the machine and r4 the mode's dispatcher (+36), whose
message ring at +204, the mode's +240, the handlers post to), 13
transitions each:

- **Today's result** (UI movie 27, asset 22578): (0, 1) `Create` → 1
  or 9, (1, 1) → 2 `OpenMovie`, (2, 1) `WaitForMovie` → 4 or 9, (9, 6)
  `ScreenMachine::Reenter` → 4, (4, 1) → 5 `Start`, in 5 Draw (message
  1) runs a page and Update (message 0) a transition, (5, 5) → 8
  `Unload`, (8, 3) `Unloaded` → 10, (5, 4), (9, 4) and (10, 4) `Leave`
  → 7, (7, 4) → 6 `Destroy` (13 or 20). Its blocks are globals
  (`../known-data.md`): `[0x828160C8]` the page object (28 bytes, laid
  out as the title screen's and the replay theater's), `[0x828160CC]` a
  menu (68 bytes, `Menu::Pressed`'s, [`menu-helpers.md`](menu-helpers.md))
  and `[0x828160D0]` the screen's data (68 bytes asked; the copies
  reach +87, inside the page `VirtualAlloc` gives). `Start` reads the
  day (`ReadDay`, which also moves the career to the next day,
  `Career::NextDay`, [`career.md`](career.md)) and sets the movie's
  texts. Pages: 0 `WaitPage` (the accept button), 1 `LeavePage` (queues
  a movie from the save's +0x1D524 when one is set, frees the race
  event, posts message 5). Transitions: 0 `Intro`, 1 `ShowLoop`, 2
  `FadeOut`.
- **Today's event** (UI movie 0, asset 22545): the same keys, with
  (5, 7) → 8 `Unload`, (8, 3) `Unloaded` (stops the screen's sound, cue
  2 of bank 1, posts message 6: today's result's reenter, then state 9)
  and (9, 8) `ScreenMachine::Reenter`. Its blocks: `[0x828160D4]` the
  page object, `[0x828160D8]` a menu (68 bytes) and `[0x828160DC]` the
  data (68 bytes asked, fields up to +411 used, inside the page). It
  walks the save's unlocks just made (+0x1C778, 12-byte entries, count
  +0x1CA78; [`../known-data.md`](../known-data.md)'s save data): each
  entry names a 56-byte data table entry (`DataTables::Entry56`,
  [`data-tables.md`](data-tables.md)) whose +12 picks its title string
  (4650 + it), +16 its image (assets 22696 + i and 22741 + i, loaded at
  `OpenMovie` through `Archive::Load` with `Type2Hub::ImageLoaded`,
  [`type2-screens.md`](type2-screens.md)) and +8 the offset to the
  entry's next page (0 the last), the body string 6683 + the entry
  index. Pages: 0 `EventPage` (accept: the text revealed first, then
  the next page or unlock), 1 `EndPage`. Transitions: 0 `Intro`, 1
  `FadeOut`, 2 `NextUnlock`, 3 `Type2Hub::Transition9` and 5
  `Type2Hub::Transition11` (shared, `type2-screens.md`), 4 `NextPage`.
  With movie 6 queued its `EndPage` queues movies 6 and 7 with the type
  9 mode after them and leaves for the boot mode (state 10, then
  `Destroy`'s 20) instead of handing over to today's result.

In play (`../coverage.md`) the mode's Init, its load wait, `NextMode`,
`LeaveScreen` and `WaitForShown`, today's result through its pages and
transitions and the way to the garage, and today's event's set-up with
nothing to show (`Kind` 0), its `Leave` and `Destroy` were seen. Today's
event's pages, the movie queued after the day and `Leaving` weren't.

Names are ours. Read 2026-11-12 by the B7 loop's mapping agent: the
mode's functions, both machines' set-ups, handlers, pages and
transitions in full (the Inits' transition tables extracted from their
functor stores); `ReadDay`'s stats fields in outline (their meanings
from `../known-data.md`'s stats record). Fields and globals:
`../known-data.md`'s "type 9 mode block" and `[0x828160C8]` to
`[0x828160DC]`. Shared, with rows elsewhere: `ModeType9::NextModeAnswer`
and `ForceExit` (`type10-mode.md`), `ModeType9::Release`
(`front-end-screen-helpers.md`), `sub_8211AD00` (a screen's start),
`UiMovies::Open`, `Find`, `IsLoaded`, `Delete`, `UiMovie::Run`, `Play`,
`PlayNamed`, `Loop`, `LoopNamed`, `LoopLast`, `Finish`, `FinishNamed`,
`FinishAll`, `FindNode`, `SetText`, `SetNumberText`, `SetChildNameText`,
`RevealText`, `HasMoreText` (`sub_8211D8D8`)
([`ui-movies-game.md`](ui-movies-game.md)), `Strings::Load`
([`files-and-saves.md`](files-and-saves.md)), `VirtualAlloc`,
`VirtualFree` ([`heap.md`](heap.md)), `XMemSet`
([`c-runtime.md`](c-runtime.md)), `memcpy` (`c-runtime.md`),
`ScreenFades::FadeIn` and `FadeOut` ([`mode-tick.md`](mode-tick.md)),
`StateMachine::Add` and `GameMessages::Dispatch` (`game-modes.md`),
`LoadingScreen::Show`, `Run` and `Finish` (`race-loading.md`),
`SaveData::Of` ([`vehicle-accessors.md`](vehicle-accessors.md)),
`DayBlock::WinStreak`, `DayBlock::Word4832`, `Career::NextDay` and
`Progress::CheckUnlocks` (`career.md`), `DataTables::Entry56`
(`data-tables.md`), `Archive::Load`, `Archive::AssetSize`
(`files-and-saves.md`), `Type2Hub::ImageLoaded`, `FreeImage`,
`Transition9`, `Transition11` (`type2-screens.md`), `Menu::Pressed`
(`menu-helpers.md`), `MenuSound::Play` and `Sound::Play`
([`menu-sounds.md`](menu-sounds.md)), the sound manager's
`IsCuePlaying` and `StopCue` ([`sound-cues.md`](sound-cues.md)),
`RaceEvent::Destroy` ([`race-event.md`](race-event.md)),
`ScreenMachine::Reenter` (`location-screen.md`), the empty
`sub_82106540` (`audio.md`) and the slot thunks (`type2-mode.md`,
`boot-mode.md`). Nothing in `itc_recomp/src` hooks this file's functions
(checked 2026-11-12).

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
| `sub_8225D6D0` | `ModeType9::Init` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; both machines' `InitStates` its plain C++, the other callees through their overrides); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225D918` | `ModeType9::WaitForLoads` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225DC38` | `ModeType9::NextMode` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225DB70` | `ModeType9::LeaveScreen` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225DA68` | `ModeType9::WaitForShown` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225DE78` | `ModeType9::Leaving` | Overridden |  Ported too, though never seen in play (the mode's other slots are ours, the garage screen's precedent, 2026-11-09): plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225D6A8` | `ModeType9::IsLoaded` (probably) | Overridden |  Ported too, though never seen in play (the mode's other slots are ours): plain C++ in `itc_recomp/src/todays_result/mode.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; reached only through the vtable, so the adapter stays); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225D658` | `ModeType9::FindShown` | Native |  Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/mode.cpp`, no adapter; its callers `NextMode`, `LeaveScreen` and `WaitForShown` are ours and call it directly (the original only behind `--native_todays_result` off); checked by the `todays_result` harness. |
| `sub_8225BA48` | `TodaysEvent::Kind` (probably) | Overridden |  Plain C++ in `itc_recomp/src/todays_result/mode.cpp` (`SaveData::Of` its plain C++) behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default), kept while today's event's `Create` and `EndPage` run recompiled; `ModeType9::Init` calls the plain C++ directly; checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225B358` | `TodaysResult::InitStates` | Native |  Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/init.cpp` (a step table from `itc_recomp/tools/init_step_tables.py`, StateMachine::Add's plain C++), no adapter; its only caller `ModeType9::Init` is ours and calls it directly (the original only behind `--native_todays_result` off); checked by the `todays_result` harness. |
| `sub_8225A7A0` | `TodaysResult::Create` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; VirtualAlloc through its override, the data's 68 bytes asked from it as the original does); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A510` | `TodaysResult::OpenMovie` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A930` | `TodaysResult::WaitForMovie` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; `UiMovies::IsLoaded` its plain C++); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225B300` | `TodaysResult::Start` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A520` | `TodaysResult::ReadDay` | Native |  Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/result.cpp`, no adapter; its only caller `Start` is ours and calls it directly (the original only behind `--native_todays_result` off); memcpy, `DayBlock::WinStreak`, `Word4832` and `Career::NextDay` through their overrides; the percentages converted in doubles, each step rounded to a float, as the original; checked by the `todays_result` harness. |
| `sub_8225B258` | `TodaysResult::SetUpMovie` | Native |  Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/result.cpp`, no adapter; its only caller `Start` is ours and calls it directly (the original only behind `--native_todays_result` off); `UiMovie::FindNode` its plain C++; checked by the `todays_result` harness. |
| `sub_8225ADE0` | `TodaysResult::SetTexts` | Native |  Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/result.cpp`, no adapter; its only caller `SetUpMovie` is ours and calls it directly (the original only behind `--native_todays_result` off); the setters through their overrides, the varargs `SetText` calls' unset arguments 0 by the user's call of 2026-10-30, the double's bits in r7 and at the frame's +48 as the original; checked by the `todays_result` harness. |
| `sub_8225B0C8` | `TodaysResult::Draw` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; the page through the guest from its table); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225B128` | `TodaysResult::Update` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; the transition through the guest from its table; the empty `sub_82106540` left out); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A9B0` | `TodaysResult::WaitPage` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; `Menu::Pressed` and `MenuSound::Play` through their overrides); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225AB30` | `TodaysResult::LeavePage` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; `RaceEvent::Destroy` through its override); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A720` | `TodaysResult::QueueMovie` | Native |  Ported too, though never seen in play (its caller `LeavePage` is ours, the garage screen's precedent, 2026-11-09). Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/result.cpp`, no adapter; its only caller `LeavePage` is ours and calls it directly (the original only behind `--native_todays_result` off); `Progress::CheckUnlocks`, recompiled and never seen in play, through the guest; checked by the `todays_result` harness. |
| `sub_8225ABD8` | `TodaysResult::Intro` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225AC88` | `TodaysResult::ShowLoop` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225AD20` | `TodaysResult::FadeOut` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225AA20` | `TodaysResult::Unload` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A9A0` | `TodaysResult::Unloaded` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225AA88` | `TodaysResult::Leave` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225A8A8` | `TodaysResult::Destroy` | Overridden |  Plain C++ in `itc_recomp/src/todays_result/result.cpp` behind an adapter in `itc_recomp/src/hooks/todays_result.cpp` (`--native_todays_result`, on by default; VirtualFree through its override); checked by the `todays_result` harness (B7 loop, 2026-11-12). |
| `sub_8225CF30` | `TodaysEvent::InitStates` | Native |  Native (B7 loop, 2026-11-12): plain C++ in `itc_recomp/src/todays_result/init.cpp` (a step table from `itc_recomp/tools/init_step_tables.py`, StateMachine::Add's plain C++), no adapter; its only caller `ModeType9::Init` is ours and calls it directly (the original only behind `--native_todays_result` off); checked by the `todays_result` harness. |
| `sub_8225BE50` | `TodaysEvent::Create` | | (0, 1): r3 = the machine. +9608 = 0; `[0x828160D4]` = a 28-byte page object (as today's result's), `[0x828160D8]` = a 68-byte menu (the same words), `[0x828160DC]` = 68 bytes asked (+0, +188, +376, +380, +384 = 0, +396 = -1). `Kind()` not 0 → `MarkImages`, +9604 = 1 when +9612 is set, else 9; 0 → +9608 = 1 (nothing to load) and +9604 = 9. A 112-byte frame (`__savegprlr_29`). Seen in play (`Kind` 0). Read in full. Blocked only by `Kind` (in this file). |
| `sub_8225BAB8` | `TodaysEvent::MarkImages` | | No arguments: for each of the save's unlocks just made (+0x1C778, 12-byte entries, count +0x1CA78), each page of its chain (`DataTables::Entry56(table +4, entry +8)`, then + the record's +8 while not 0): the bit of the record's +16 (when not negative) set in `[0x828160DC]` +380 (46 bits, two words). A 160-byte frame (`__savegprlr_26`). Only caller `Create`. Not seen in play. Read in full. |
| `sub_8225C130` | `TodaysEvent::OpenMovie` | | (1, 1): `UiMovies::Open(0, 22545)`; `LoadImages`. A 96-byte frame. Not seen in play. Read in full. |
| `sub_8225C008` | `TodaysEvent::LoadImages` | | No arguments: for i = 0 to 45 with bit i set in `[0x828160DC]` +380, `LoadImagePair(data, i)`. A 112-byte frame. Only caller `OpenMovie`. Not seen in play. Read in full. |
| `sub_8225BD10` | `TodaysEvent::LoadImagePair` | | r3 = the data, r4 = an image index i: for assets 22696 + i and 22741 + i, a buffer of `Archive::AssetSize` bytes (`VirtualAlloc`, cleared with `XMemSet`) and a 16-byte record {+0 0 (loaded), +4 i, +8 0, +12 0} added to the list at +4 (count +188) or +192 (count +376), then `Archive::Load(asset, buffer, Type2Hub::ImageLoaded 0x8218D8E8, 0, the record)` (`type2-screens.md`: +8 the texture record, +12 the XPR, +0 = 1 when loaded). A 144-byte frame (`__savegprlr_26`). Only caller `LoadImages`. Not seen in play. Read in full. |
| `sub_8225C160` | `TodaysEvent::WaitForMovie` | | (2, 1): with +9608 0, +9608 = 1 once `UiMovies::IsLoaded(0)` and both image lists are loaded (`ImagesLoadedA`, `ImagesLoadedB`); once set, +9604 = 9 when +9612 is set, else 4. A 96-byte frame. Not seen in play. Read in full. |
| `sub_8225C0A0` | `TodaysEvent::ImagesLoadedA` | | r3 = the data: 1 when every record in the list at +4 (count +188) has +0 set, else 0. Leaf. Only caller `WaitForMovie`. Not seen in play. Read in full. |
| `sub_8225C0E8` | `TodaysEvent::ImagesLoadedB` | | The same for the list at +192 (count +376). Leaf. Only caller `WaitForMovie`. Not seen in play. Read in full. |
| `sub_8225C990` | `TodaysEvent::Start` | | (4, 1): `Sound::Play(bank 1, cue 2)` (`menu-sounds.md`; stopped by `Unloaded`); movie 0's +28 object's +36 and +32 saved to the data's +400 and +404; `SetUpTexts`; `NextUnlockEntry` not 2 → the page object: +8 = +0 when +0 isn't 0, +12 = 0 (`Intro`), +4 = 0 (then `EventPage`), +16 = -1, +0 = -1. A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225C648` | `TodaysEvent::SetUpTexts` | | No arguments: on movie 0 with `SaveData::Of(0)`: node (8, 6) `SetChildNameText` with the save's +8 record; (8, 9) `SetNumberText` with +6116 (the money); (8, 13) string 8 with the day (+6100, at most 999); node (10, 11) +108 = the day % 7; nodes (10, 6), (10, 8), (10, 7), (10, 9) +80 = 0; the data's +0 = 1. A 128-byte frame (`__savegprlr_28`). Only caller `Start`. Not seen in play. Read in full. |
| `sub_8225BB70` | `TodaysEvent::NextUnlockEntry` | | No arguments, on `[0x828160DC]`: with an entry shown (+396 not -1) whose record (`DataTables::Entry56(+392, +396)`) has +8 nonzero, +396 += it and answers 0 (the entry's next page); else the save's first unlock just made with +0 = 0 is marked 1 and copied to the data's +388 to +396, answers 1; none left: the save's +0x1CA78 = 0, answers 2. A 96-byte frame. From `Start`, `EventPage`. Not seen in play. Read in full. |
| `sub_8225C928` | `TodaysEvent::Draw` | | (5, 1): the page in `[0x828160D4]` +0 (-1 none) from {`EventPage` `0x8225C428`, `EndPage` `0x8225C590`}, called with r3 = the dispatcher, r4 = the machine. A 96-byte frame. Not seen in play. Read in full. |
| `sub_8225CDE8` | `TodaysEvent::Update` | | (5, 0): `sub_82106540(100, 30, 0, "TXR_TodaysEvent")`; movie 0; as `TodaysResult::Update` over `[0x828160D4]` with {`Intro` `0x8225CB18`, `FadeOut` `0x8225C770`, `NextUnlock` `0x8225CC80`, `Type2Hub::Transition9` `0x8218F150`, `NextPage` `0x8225CD50`, `Type2Hub::Transition11` `0x8218F220`}, and with no transition `UiMovie::Run(movie, 0)` whatever the page. A 144-byte frame. Not seen in play. Read in full. |
| `sub_8225C428` | `TodaysEvent::EventPage` | | Page 0: on `Menu::Pressed([0x828160D8], 159)`: `MenuSound::Play(0)`; `UiMovie::RevealText(movie 0, 0, 2)` answering 0 → `NextUnlockEntry`: 0 → +12 = 5 (`Transition11`), +16 = 4 (`NextPage`), +4 = 0; 1 → +12 = 3 (`Transition9`), +16 = 2 (`NextUnlock`), +4 = 0; 2 → +12 = 1 (`FadeOut`), +16 = -1, +4 = 1 (then `EndPage`); each with +8 = +0 (when not 0, or not 1 for 2) and +0 = -1. Answering nonzero → node (0, 0) +80 = `HasMoreText(movie, 0, 2)` (1 or 0). A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225C590` | `TodaysEvent::EndPage` | | Page 1: r3 = the dispatcher, r4 = the machine: `Kind()` 1 → message 7 queued (`Unload`), the data's +408 = 0; else the machine's +9604 = 10, +408 = 1, `QueueEventMovies`. A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225B9B8` | `TodaysEvent::QueueEventMovies` | | As `TodaysResult::QueueMovie`, with the movie after it 7 when it is 6, and the mode after = 9 (this mode again). Only caller `EndPage`. Not seen in play. Read in full. |
| `sub_8225CB18` | `TodaysEvent::Intro` | | Transition 0: on its first call `UiMovie::Play(movie 0, -1, 2)`, `Play(8, 0)`, `LoopLast`, `Play(15, 0)`, `LoopLast`, `PlayNamed(2)`, `LoopNamed(2)`, `ScreenFades::FadeIn([0x82001BEC], …)`, `ShowUnlock`, the word = 1; `UiMovie::Run`; when it answers 0, `Play(12, 1)` and `Loop(12, 1)`, answers 1. A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225C770` | `TodaysEvent::FadeOut` | | Transition 1: on its first call `UiMovie::Finish(movie 0, 8, 0)`, `Finish(15, 0)`, `FinishNamed(2)`, `ScreenFades::FadeOut(60, 0, 0, 0, 0)`, the word = 1; `Run`; answers 1 when both fades' words are 0. A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225CC80` | `TodaysEvent::NextUnlock` | | Transition 2: on its first call `ScreenFades::FadeIn(60, …)`, `Play(movie 0, -1, 4)`, `ShowUnlock`, the word = 1; `Run`; answers 1 when both fades' words are 0. A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225CD50` | `TodaysEvent::NextPage` | | Transition 4: on its first call `Play(movie 0, -1, 4)`, `ShowUnlock`, the word = 1; `Run`; answers 1 when it answers 0. A 112-byte frame. Not seen in play. Read in full. |
| `sub_8225CA50` | `TodaysEvent::ShowUnlock` | | No arguments: the record `DataTables::Entry56(data +392, +396)`: node (0, 1) string 4650 + its +12, node (0, 2) string 6683 + the entry index (+396) (`UiMovie::SetText`); `ShowImages(its +16)`; node (0, 0) +80 = `HasMoreText(movie, 0, 2)`. A 128-byte frame (`__savegprlr_28`). From `Intro`, `NextUnlock`, `NextPage`. Not seen in play. Read in full. |
| `sub_8225C848` | `TodaysEvent::ShowImages` | | r3 = an image index: movie 0's +28 object's +36 = the texture record (+8) of the first record in the list at +4 whose +4 is the index (0 when none), its +32 the same from the list at +192. A 112-byte frame. Only caller `ShowUnlock`. Not seen in play. Read in full. |
| `sub_8225C348` | `TodaysEvent::Leave` | | (5, 4), (9, 4), (10, 4): r3 = the machine, r4 = the dispatcher: +9604 = 7; movie 0 found → `FinishAll`, its +28 object's +36 and +32 put back from the data's +400 and +404; `UiMovies::Delete(0)`; message 4 queued. A 112-byte frame. Seen in play. Read in full. Ready (66 lines). |
| `sub_8225C1F8` | `TodaysEvent::Unload` | | (5, 7) → 8: movie 0 found → `FinishAll` and the +28 object's words put back as `Leave`'s; with +9616 set, `UiMovies::Delete(0)` and +9608 = 0. A 128-byte frame (`__savegprlr_29`). Not seen in play. Read in full. |
| `sub_8225C278` | `TodaysEvent::Unloaded` | | (8, 3): r3 = the machine, r4 = the dispatcher: the sound manager (`[0x826C5C80]`) slot 14 (`IsCuePlaying`) for bank 1, cue 2, and if so slot 13 (`StopCue`); message 6 queued (today's result's reenter); +9604 = 9. A 144-byte frame (`__savegprlr_27`). Not seen in play. Read in full. |
| `sub_8225BF78` | `TodaysEvent::Destroy` | | (7, 4) → 6: +9604 = 20 when the data's +408 (movies queued) is set; `FreeImages(data)`; `VirtualFree` of `[0x828160D4]`, `[0x828160D8]`, `[0x828160DC]`, the three = 0. A 128-byte frame (`__savegprlr_28`). Seen in play. Read in full. Ready with `FreeImages` (108 lines). |
| `sub_8225BC60` | `TodaysEvent::FreeImages` | | r3 = the data: each list (+4 count +188, +192 count +376) emptied from its end: `Type2Hub::FreeImage(record)` (`type2-screens.md`) and `VirtualFree(record)`. A 112-byte frame (`__savegprlr_29`). Only caller `Destroy`. Seen in play (empty lists). Read in full. |
