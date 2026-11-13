# Known functions: the type 5 mode (the parking area's mode)

The type 5 front-end mode (`ModeType5::`, the name ours): the mode of
the career's **parking area** ([`location-screen.md`](location-screen.md),
the screen where the player picks one of up to 6 opponents to race). It
is made by `ModeType5::ModeType5` `sub_82106F08`
([`game-modes.md`](game-modes.md)), 852 bytes like types 2, 4, 6, 7, 9,
10 and 12, entered from the type 2 mode's `NextMode` (the shown
machine in state 12, [`type2-mode.md`](type2-mode.md)), and released
by `ModeType5::Release` `sub_8222FB90`
([`front-end-screen-helpers.md`](front-end-screen-helpers.md)). Its
dialog's `Update` passes the string "TXR_PaCmnWindow" ("Pa" the parking
area, probably; the type 2 dialog's is "TXR_MenuCmnWindow") to the empty
`sub_82106540`, a debug print stripped from the release, probably. No
other debug strings near the code.

Its vtable `0x82006C24` has 15 slots (read from `default.xex`, decrypted
and decompressed into the agent's scratchpad, nothing of it in the
repo): 0 `Init` `sub_8222FD68`, 1 `Release`, 2 to 5, 9 and 10
`Mode::NoNextMode` (`sub_8222FC90`, [`boot-mode.md`](boot-mode.md)), 6
`NextMode` `sub_82230458`, 7 `Mode::DeletingDestructor` (`sub_82106E38`,
`boot-mode.md`), 8 `WaitForLoads` `sub_82230050`, 11 `WaitForShown`
`sub_82230168`, 12 `LeaveScreen` `sub_82230260`, 13 `NextModeAnswer`
`sub_8222FCA0` (its row in `location-screen.md`, as
`FrontEndMode::NextMode`) and 14 `Leaving` `sub_82230328`, reached
through the slot thunks `Mode::Slot8Thunk` and `Slot11Thunk` to
`Slot13Thunk` ([`type2-mode.md`](type2-mode.md)), `Mode::NextModeThunk`
`sub_82235C08` (`boot-mode.md`) and `Mode::Slot14Thunk` `sub_8222FC50`
(here: only this mode's vtable has a slot 14). Its +32 vtable
`0x82006C18` (the three words before the main one) has three:
`Interrupt` `sub_8222FED0`, `ForceExit` `sub_8222FF60` and `Resume`
`sub_8222FC38` (none seen in play).

`Init` allocates the mode's block (28,940 bytes at +848, `VirtualAlloc`)
with three state machines, each set up by its own Init and added to the
mode's dispatcher (+36) in the order +9640, +0, +19280: +0 the **car
settings** (`CarSettings::InitType5` `sub_821B7878`,
[`car-settings.md`](car-settings.md); its id 3), +9640 the **parking
area** (`LocationScreen::InitStates` `sub_8222F258`,
`location-screen.md`; id 21) and +19280 the mode's **dialog**
(`Type5Dialog::Init` `sub_82226A10`, here; id 16), the yes/no questions
and notices over UI movies 16 to 19 that the other two open, as the type
2 mode's dialog ([`type2-dialog.md`](type2-dialog.md)) and the type 6
and 7 modes' ([`type6-type7-modes.md`](type6-type7-modes.md)). All three
open on their message (+9612 = 1, +9616 = 0 at the machine's +9612). The
block's +28920 is the shown machine's id (3, 16 or 21;
`ModeType5::FindShown`, 21 at Init), +28924 = 21 at Init (readers not
looked for) and +28928 the next mode's 8-byte answer (`NextMode` and
`ForceExit` write it, `NextModeAnswer` hands it to `Modes::Tick` once
`LocationScreen::Free` reports done). See `../known-data.md`'s "type 5
mode block".

The flow (the delegate at the mode's +8, a functor {`0x82010FA0`, the
mode, the slot thunk, 0}): `Init` shows the loading screen and sets the
delegate to `Mode::Slot8Thunk` (`WaitForLoads`), which runs the loading
screen and posts Draw (message 1) until the three machines' movies are
loaded (block +9608, +19248, +28888), then plays bank 1's cue 3,
finishes the loading screen and hands over to `Mode::NextModeThunk`
(`NextMode`). `NextMode` runs every tick: Draw and Update (messages 1
and 0) queued and dispatched; the shown machine in state 8 →
`LeaveScreen` (message 3 alone) → `WaitForShown` (until the movies are
loaded again) → `NextMode`; the parking area in state 10 → message 4
(leave); in state 11 (a race set up) → the answer {1, mode 1, the race},
in 13 → {1, mode 9, today's result ([`todays-result.md`](todays-result.md))},
and `Leaving` (slot 14), which posts message 4 and hands over to
`NextModeAnswer` (slot 13). `Leaving`'s check of the machines' states
looks at ids 0 to 2 only, none of which is a machine, so it always
hands over at once (the type 2 mode's looks at 0 to 46). `ForceExit`
(the system's sign-out or the like, from the +32 interface) writes the
answer {1, mode 4, the title} (reasons 0 and 1, with `[0x826364EC]` =
1) or {1, mode 12} (2 and 3) and goes to `Leaving`; `Interrupt` answers
1 only when everything is loaded and the dialog is idle (state 9), else
it queues message 18 (the dialog's `OnInterrupt`) and answers 0;
`Resume` sets the parking area's `[0x82814E50]` +900 (its page
restarted, `LocationScreen::UpdatePage`).

The dialog's machine (keys (state, message), r3 = the machine, r4 = the
dispatcher, the mode's +36, whose ring at +204 is the mode's +240): (0,
1) `OnEnter` makes its blocks and goes to 1 (load the movies) or 9;
(1, 1) `Dialog::OpenMovies` → 2; (2, 1) `WaitForMovies` → 9; in 9 an
open message picks the dialog and goes to 4 (or 1 when the movies
aren't loaded yet); (4, 1) → 5 (the empty `sub_82106540`); in 5, Update
(message 0) runs the steps and Draw (message 1) the pages; a close step
queues state 9 (the data's +4), and `Update` then posts message 18
(closed) and goes back to 9; (5, 18) `OnInterrupt` asks the page
showing to close with message 20; (8, 3) `ScreenMachine::Wait`
`sub_82277068` (`type6-type7-modes.md`; no transition enters 8); (5, 4)
and (9, 4) `Dialog::Leave` → 7, (7, 4) `OnLeft` → 6. Its blocks are
globals (`../known-data.md`), each `VirtualAlloc`ed by `OnEnter` and
freed by `OnLeft`, laid out as the type 2 dialog's: `[0x82814E3C]` the
page object (28 bytes: +0 the page `Draw` runs, -1 while a step runs;
+4 the page after the step; +8 the page before; +12 the step `Update`
runs, -1 none; +16 the step queued after it; +20; +24 the step's started
word, the steps' r3), `[0x82814E40]` the menu (68 bytes, items 0 to 1,
wrapping: yes and no) and `[0x82814E44]` the data (68 bytes, +0 to +12
used: +0 1 once the movies are loaded, +4 the state queued by the close
steps (21 none, 9), +8 the movie shown (16 to 19), +12 set by (5, 18)).
No status word (unlike the type 2 dialog's `[0x82636204]`).

| Dialog | Opened by | Movie | Page | Steps (open, close, cursor) | Replies |
|---|---|---|---|---|---|
| 0, a yes/no | message 10 (`LocationScreen::Page0`'s items 3 and 4, the cursor on no; the car settings' course page, `car-settings.md`) | 16 | 0 | 0, 1, 8 | 14 yes, 15 no, 17 back, 20 interrupted |
| 1, a notice | message 11 (not looked for) | 17 | 1 | 2, 3 | 16, 20 interrupted |
| 2, a yes/no | message 12 (not looked for) | 18 | 2 (the same page function as 0) | 4, 1, 8 | 14 yes, 15 no, 17 back, 20 interrupted |
| 3, a notice | message 13 (not looked for) | 19 | 3 | 6, 7 | 16, 20 interrupted |

Dialog 2 shares dialog 0's page function, which closes through step 1
and moves the cursor through step 8 (movie 16's) and sets the next page
to 0 whichever page it runs for; steps 5 (movie 18's close) and 9 (sound
5 only) are in the table but no page or open sets them. The replies go
to the car settings' (5, 14) `OnYes`, (5, 15) `OnNo`, (5, 17) `OnBack`
and (5, 18) `OnDialogClosed` (`car-settings.md`) and the parking area's
(5, 14), (5, 15) and (5, 20) (`LocationScreen::OnMessage14`, `15`, `20`).
`MenuSound::Play` cues: 0 accept (and both buttons on the notices), 2
back, 4 the cursor moved, 5 each open and close animation.

In play (`../coverage.md`) the mode's Init, `WaitForLoads`, `NextMode`,
`FindShown`, `Leaving` and the slot 14 thunk, and the dialog's Init,
`OnEnter`, `WaitForMovies`, `Draw`, `Update`, `OpenConfirm`, `OnLeft`,
the confirm page and the steps 0, 1 and 8 were seen; `WaitForShown`,
`LeaveScreen`, the +32 interface, the notices, the second yes/no and
`OnInterrupt` weren't.

Names are ours. Read 2026-11-13 by the B7 loop's mapping agent (no
subagents): every function in full, the dialog's transitions extracted
from its Init's functor stores (a script following its registers and
six-word copy loops), `Draw`'s page table and `Update`'s step table.
Everything is ready (`b7_groups.py`); `Draw`'s and `Update`'s tables are
indirect calls the tool doesn't count, so the pages and steps go before
them. Shared, with their rows elsewhere: `ModeType5::ModeType5`,
`StateMachine::Add` `sub_82106158` and `GameMessages::Dispatch`
`sub_821062E8` (`game-modes.md`), `ModeType5::Release`
(`front-end-screen-helpers.md`), `NextModeAnswer` `sub_8222FCA0`,
`LocationScreen::InitStates` and `LocationScreen::Free`
(`location-screen.md`), `CarSettings::InitType5` (`car-settings.md`),
`Mode::NoNextMode`, `Mode::DeletingDestructor` and
`Mode::NextModeThunk` (`boot-mode.md`), the slot thunks
`Mode::Slot8Thunk` `sub_8223D4B0`, `Slot11Thunk` to `Slot13Thunk`
(`type2-mode.md`), `sub_8211AD00` (a screen's start), `UiMovies::Find`
`sub_8211B3F0`, `IsLoaded` `sub_8211BE98`, `UiMovie::ResetPose`
`sub_8211ABA8`, `PlayNamed` `sub_8211DAC8`, `Run` `sub_8211E1B8`,
`LoopLast` `sub_8211DDE8`, `FinishNamed` `sub_8211DF20` and `FinishAll`
`sub_8211DFF8` ([`ui-movies-game.md`](ui-movies-game.md)),
`Strings::Load` ([`files-and-saves.md`](files-and-saves.md)),
`Archive::WaitIdle` ([`archive.md`](archive.md)), `VirtualAlloc`
`sub_82295398` and `VirtualFree` `sub_82295330` ([`heap.md`](heap.md)),
`ScreenFades::FadeIn` ([`mode-tick.md`](mode-tick.md)),
`LoadingScreen::Show`, `Run` and `Finish` ([`race-loading.md`](race-loading.md)),
`Sound::Play` and `MenuSound::Play` `sub_821103F0`
([`menu-sounds.md`](menu-sounds.md)), `Menu::Step` `sub_82186568` and
`Menu::Pressed` `sub_821864C8` ([`menu-helpers.md`](menu-helpers.md)),
`Dialog::OpenMovies` `sub_82225810`, `Dialog::Leave` `sub_82225C70` and
`ScreenMachine::Wait` `sub_82277068` (`type6-type7-modes.md`), the type 2
dialog's `OpenNoticeStep` `sub_821878B0` and `OpenNotice2Step`
`sub_82187B80` (`type2-dialog.md`, steps 2 and 6 of this dialog's table
too) and the empty `sub_82106540` ([`audio.md`](audio.md)). Nothing in
`itc_recomp/src` hooks this file's functions (checked 2026-11-13; a
comment in `src/hooks/car_settings.cpp` names `sub_8222FD68`).

Native (B7 loop, 2026-11-13): every function here under
`--native_type5_mode` (`src/type5_mode/`, adapters
`src/hooks/type5_mode.cpp`, harness `type5_mode`), those never seen in
play ported with them. `Leaving`'s check over ids 0 to 2 and `NextMode`'s
answer bytes built over its frame's doubleword are kept as the original
has them. The rows' `PlayNamed` of the highlights pass 0 in r5 (read
again for the harness; the map had 1). The harness's world keeps
`LoadingScreen::Run` and `Finish` on the loading screen's world with its
current movie 0 to 2: with -1 they use the current movie's index, and 3
(no screen) would read past the three movies' words.

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
| `sub_8222FD68` | `ModeType5::Init` | Overridden | Slot 0 of vtable `0x82006C24`, from `Modes::Tick` once the mode is made: r3 = the mode. A 128-byte frame (`__savegprlr_29`). `sub_8211AD00` (a screen's start); `Strings::Load([0x826C5C34], 0x7FFB)`; `Archive::WaitIdle`; mode +848 = `VirtualAlloc(0, 28940, 0x1000, 4)` (no check for 0); `LocationScreen::InitStates(block +9640)`, `CarSettings::InitType5(block +0)`, `Type5Dialog::Init(block +19280)`; the three added to the dispatcher (+36: the list from +36, count +236) in the order +9640, +0, +19280; block +28924 = +28920 = 21 (the parking area shown); +19252 = 1, +19256 = 0 (the parking area's +9612, +9616), +9612 = 1, +9616 = 0 (the car settings'), +28892 = 1, +28896 = 0 (the dialog's); `ScreenFades::FadeIn([0x82001BEC])`; `LoadingScreen::Show(1, 0, -1)`; the delegate (+8, with +4 = +8) = {`0x82010FA0`, the mode, `Mode::Slot8Thunk` `0x8223D4B0` (`WaitForLoads`), 0} (the functor built at the frame's +80). No return value. No direct callers. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::ModeInit` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the mode's vtables: the adapter stays; harness `type5_mode`. |
| `sub_82230050` | `ModeType5::WaitForLoads` | Overridden | Slot 8 (through `Mode::Slot8Thunk`, set by `Init`): r3 = the 8-byte answer, r4 = the mode. A 128-byte frame (`__savegprlr_29`). The answer cleared (byte +0, word +4); `LoadingScreen::Run(-1)`. When the parking area's (block +19248), the car settings' (+9608) and the dialog's (+28888) movies are loaded (each 1): `Sound::Play(&1, &3)` (bank 1's cue 3), `LoadingScreen::Finish(-1)`, the delegate = {`0x82010FA0`, the mode, `Mode::NextModeThunk` `0x82235C08`, 0}. Then message 1 (Draw) queued (the ring at +240: count +840, start +844, 50 entries of 12 bytes) and `GameMessages::Dispatch(mode + 36)`. Returns r3. No direct callers. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::WaitForLoads` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the slot thunks and `Mode::NextModeThunk`: the adapter stays; harness `type5_mode`. |
| `sub_82230458` | `ModeType5::NextMode` | Overridden | Slot 6, from `Mode::NextModeThunk` every tick once loaded: r3 = the answer, r4 = the mode. A 160-byte frame (`__savegprlr_25`). The answer cleared; `FindShown(block)`; the shown machine (block +28920: 3 the car settings at +0, 16 the dialog at +19280, 21 the parking area at +9640, else none, read as address 0) in state 8 → the delegate's function = `0x8222FC70` (`Mode::Slot12Thunk`: `LeaveScreen`; the functor word at the frame's +80); the parking area's state (block +19244) 10 → message 4 queued (leave). Then messages 1 (Draw) and 0 (Update) queued (no system UI check, unlike the type 2 mode's) and `GameMessages::Dispatch(mode + 36)`. After it, the parking area's state 11 (a race set up) → block +28928 = a doubleword from the frame's +80 with byte 1 and the word 1 (the race; the three bytes after byte 1 the frame's: the functor word's when the state 8 branch ran this call, else what the stack held), 13 → the same with 9 (today's result, `todays-result.md`); both then set the delegate's function to `0x8222FC50` (`Mode::Slot14Thunk`: `Leaving`). Returns r3. No direct callers. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::NextMode` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the slot thunks and `Mode::NextModeThunk`: the adapter stays; harness `type5_mode`. |
| `sub_82230260` | `ModeType5::LeaveScreen` (probably) | Overridden | Slot 12, the delegate's function for one tick after `NextMode` saw the shown machine in state 8: r3 = the answer, r4 = the mode. A 128-byte frame (`__savegprlr_29`). The answer cleared; `FindShown(block)`; message 3 queued alone (the machines' (8, 3) `Wait`s), `GameMessages::Dispatch(mode + 36)`; the delegate's function = `0x8222FC60` (`Mode::Slot11Thunk`: `WaitForShown`; the original's null check can't fail). Returns r3. As `ModeType10::LeaveScreen`. No direct callers. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::LeaveScreen` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the slot thunks and `Mode::NextModeThunk`: the adapter stays; harness `type5_mode`. |
| `sub_82230168` | `ModeType5::WaitForShown` (probably) | Overridden | Slot 11, after `LeaveScreen`: r3 = the answer, r4 = the mode. A 112-byte frame (r31 saved). The answer cleared; `FindShown(block)` (r4 kept: the callee doesn't touch it); when the three machines' movies are loaded (block +19248, +9608, +28888 each 1), the delegate's function = `Mode::NextModeThunk` again; message 1 (Draw) queued and `GameMessages::Dispatch(mode + 36)`. No loading screen. Returns r3. No direct callers. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::WaitForShown` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the slot thunks and `Mode::NextModeThunk`: the adapter stays; harness `type5_mode`. |
| `sub_82230328` | `ModeType5::Leaving` (probably) | Overridden | Slot 14, through `Mode::Slot14Thunk` (set by `NextMode` and `ForceExit`): r3 = the answer, r4 = the mode. A 128-byte frame (`__savegprlr_29`). The answer cleared; message 4 queued (leave) and `GameMessages::Dispatch(mode + 36)`; then a check of the machines for the ids 0 to 2 (state 6, 11, 14, 12 or 20 each, as `ModeType2::Leaving`'s), but no machine has those ids (3, 16 and 21 do), so the check always passes: the delegate's function = `0x8222FC80` (`Mode::Slot13Thunk`: `NextModeAnswer` `sub_8222FCA0`, `location-screen.md`). Returns r3. No direct callers. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::Leaving` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the slot thunks and `Mode::NextModeThunk`: the adapter stays; harness `type5_mode`. |
| `sub_8222FD08` | `ModeType5::FindShown` (probably) | Native | r3 = the block: for r10 0 to 46, the machine for the id (3 the block, 16 +19280, 21 +9640, others none): when it's in state 5 or 4 (running or starting), block +28920 = the id (the last such wins). Leaf. From `NextMode`, `LeaveScreen` and `WaitForShown`. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::FindShown` (`src/type5_mode/mode.cpp`); no adapter (removed, 2026-11-13: only the native delegate functions `NextMode`, `LeaveScreen` and `WaitForShown` call it directly; `--native_type5_mode` now switches only them; register steps in `src/hooks/type5_mode.cpp` for the harness); harness `type5_mode`. |
| `sub_8222FC50` | `Mode::Slot14Thunk` | Overridden | r3 = the answer, r4 = an object: tail-calls the object's vtable slot 14 (+56) with them, as `Mode::Slot8Thunk` and the others (`type2-mode.md`). The type 5 mode's `Leaving` (set by `NextMode` and `ForceExit`); only this mode's vtable has a slot 14. Leaf. No direct callers. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::Slot14Thunk` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, the delegate's function: the adapter stays; harness `type5_mode`. |
| `sub_8222FED0` | `ModeType5::Interrupt` (probably) | Overridden | The +32 vtable's (`0x82006C18`) slot 0: r3 = the mode + 32 (its +816 the block). Leaf. 0 until the three machines' movies are loaded (block +19248, +9608, +28888 each 1); then 1 when the dialog's state (block +28884) is 9 (idle); else message 18 queued (the dialog's `OnInterrupt`; the ring at r3 + 208 = mode + 240; no dispatch) and 0. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::Interrupt` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the mode's vtables: the adapter stays; harness `type5_mode`. |
| `sub_8222FF60` | `ModeType5::ForceExit` (probably) | Overridden | The +32 slot 1: r3 = the mode + 32, r4 = a number. r4 0 or 1: the delegate's function (at r3 - 32 + 8, built at r1 - 16, below the stack pointer) = `0x8222FC50` (`Mode::Slot14Thunk`: `Leaving`), block (r3 + 816) +28928 = {byte 1, the low three bytes of `0x8222FC50` (the functor word stored there first), the word 4} (the title), `[0x826364EC]` = 1; r4 2 or 3: the same with 12 and `[0x826364EC]` left; other values nothing. Leaf. As `ModeType9::ForceExit` (`type10-mode.md`) with this mode's slot 14 and answer offset. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::ForceExit` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the mode's vtables: the adapter stays; harness `type5_mode`. |
| `sub_8222FC38` | `ModeType5::Resume` (probably) | Overridden | The +32 slot 2: `[0x82814E50]` (the parking area) +900 = 1 (its page restarted on the next `LocationScreen::UpdatePage`). r3 not read. Leaf. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::Resume` (`src/type5_mode/mode.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, reached through the mode's vtables: the adapter stays; harness `type5_mode`. |
| `sub_82226A10` | `Type5Dialog::Init` | Native | r3 = the dialog's machine (the type 5 block's +19280, from `ModeType5::Init`). A 1,104-byte frame (`__savegprlr_27`): 15 `StateMachine::Add` (`sub_82106158`) calls, each with a {`0x82011918`, the machine, handler} functor copied into the frame (six-word copy loops) and the state to go to at the copy's +24 (0 for none): (0, 1) → `OnEnter` `sub_82225878`; (1, 1) → state 2, `Dialog::OpenMovies` `sub_82225810`; (2, 1) → `WaitForMovies` `sub_82226978`; (4, 1) → state 5, the empty `sub_82106540`; (5, 1) → `Draw` `sub_822266F8`; (5, 0) → `Update` `sub_82226788`; (5, 18) → `OnInterrupt` `sub_82225860`; (8, 3) → `ScreenMachine::Wait` `sub_82277068`; (9, 10) → `OpenConfirm` `sub_82225A90`; (9, 11) → `OpenNotice` `sub_82225980`; (9, 12) → `OpenConfirm2` `sub_82225B88`; (9, 13) → `OpenNotice2` `sub_82225A08`; (5, 4) and (9, 4) → `Dialog::Leave` `sub_82225C70`; (7, 4) → state 6, `OnLeft` `sub_822257A8`. As `Type2Dialog::Init` (`type2-dialog.md`) with its own messages and no (5, 5). Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::DialogInit` (`src/type5_mode/dialog_init.cpp`); no adapter (removed, 2026-11-13: only the native `ModeInit` calls it directly; `--native_type5_mode` now switches only them; register steps in `src/hooks/type5_mode.cpp` for the harness); harness `type5_mode`. |
| `sub_82225878` | `Type5Dialog::OnEnter` | Overridden | (0, 1): the first Draw. r3 = the machine. A 112-byte frame (`__savegprlr_29`). +9608 = 0 (the movies-loaded flag); `VirtualAlloc(0, 28, 0x1000, 4)` into `[0x82814E3C]`, the page object: +0, +4, +8, +24 = 0, +20 = +12 = -1 (+16 not written); `VirtualAlloc(0, 68, ..)` into `[0x82814E40]`, the menu: +12 = 0 (first item), +16 = 1 (last), +20 = 1 (wraps), +0, +4, +8, +24 to +40, +48, +52 and +64 = 0 (+44, +56, +60 not written); `VirtualAlloc(0, 68, ..)` into `[0x82814E44]`, the data: +0, +8, +12 = 0, +4 = 21 (no state queued). State (+9604) 1 with +9612 nonzero (1 from the mode's Init), else 9. No return value. As `Type2Dialog::OnEnter` without its status words. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::OnEnter` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82226978` | `Type5Dialog::WaitForMovies` | Overridden | (2, 1): every Draw in state 2. r3 = the machine. A 96-byte frame (r31 saved). While +9608 is 0: `UiMovies::IsLoaded` 16, 17, 18, 19 in turn (stops at the first not loaded); all loaded: +9608 = 1. On the next Draw (+9608 set): the data's +0 = 1, state 9. No return value. As `Type2Dialog::WaitForMovies` without the status word. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::WaitForMovies` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_822266F8` | `Type5Dialog::Draw` | Overridden | (5, 1): every Draw while shown. r3 = the machine, r4 = the dispatcher. A 112-byte frame holding the page table {`ConfirmPage` `sub_82225D10`, `NoticePage` `sub_82225F30`, `ConfirmPage` again, `Notice2Page` `sub_82226088`} (and a 0 at its +96): with the page (`[0x82814E3C]` +0) not -1, calls page[+0] with r3 = the dispatcher, r4 = the machine (no bounds check). Returns the page's r3 (ignored). 1 indirect call. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::Draw` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82226788` | `Type5Dialog::Update` | Overridden | (5, 0): every Update while shown. r3 = the machine, r4 = the dispatcher. A 208-byte frame (`__savegprlr_25`). First `sub_82106540(100, 30, 0, "TXR_PaCmnWindow")` (`0x82010760`; empty). m = the movie with id data +8 (`UiMovies::Find` on `0x826C5CCC`). The step table in the frame: {`OpenConfirmStep` `sub_822261E0`, `CloseConfirmStep` `sub_822262B8`, the type 2 dialog's `OpenNoticeStep` `sub_821878B0`, `CloseNoticeStep` `sub_82226370`, `OpenConfirm2Step` `sub_82226420`, `CloseConfirm2Step` `sub_822264B8`, the type 2 dialog's `OpenNotice2Step` `sub_82187B80`, `CloseNotice2Step` `sub_82226568`, `MoveCursorStep` `sub_82226618`, `Step9` `sub_822266D0`}. With no step (+12 = -1): `UiMovie::Run(m, 0)`. Else, for a step 0 or more, calls step[+12] (r3 = the page object's +24, r4 = `0x82226370`, a leftover of the table's build, not read; no bounds check); when it returns nonzero (or the step is another negative value): with no step queued (+16 = -1), the page = the next page (+0 = +4) and +12 = -1; else +12 = +16, +16 = -1, +20 = 0; then +24 = 0. Then with the data's +4 not 21: the state (+9604) = it, and when it is 9, the data's +12 = 0 and message (18, 0, 0) posted to the dispatcher's ring (+204) (the dialog closed); the data's +4 = 21. No return value. 1 indirect call. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::Update` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82225860` | `Type5Dialog::OnInterrupt` | Overridden | (5, 18): message 18 while shown (from `ModeType5::Interrupt`): the data's (`[0x82814E44]`) +12 = 1, which the page showing reads to close with message 20. r3 not read. Leaf. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::OnInterrupt` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82225A90` | `Type5Dialog::OpenConfirm` | Overridden | (9, 10): r3 = the machine. A 112-byte frame (r30, r31 saved). Data +8 = 16; m = movie 16 (`UiMovies::Find`), `UiMovie::ResetPose(m)`; the menu's +4 (previous) = its cursor +0, the cursor = m's +40960 (the default choice the caller set: 0 yes, 1 no); page object +4, +0, +8 = 0, +20 = +12 = -1, +24 = 0. With the movies loaded (+9608): +8 kept (it is 0), +12 = 0 (`OpenConfirmStep`), +16 = -1, next page 0, page -1, state 4; else state 1 (the request then lost, as the type 2 dialog's). No return value. As `Type2Dialog::OpenConfirm` without the status words. Message 10 from `LocationScreen::Page0` and `CarSettings::CoursePage` (type 5). Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::OpenConfirm` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82225980` | `Type5Dialog::OpenNotice` | Overridden | (9, 11): r3 = the machine. Data +8 = 17 (no `Find`); page object +4, +0, +8 = 1, +20 = +12 = -1, +24 = 0. With the movies loaded: +8 kept (it is 1), +16 = -1, next page 1, page -1, step 2 (the type 2 dialog's `OpenNoticeStep`), state 4; else state 1. Leaf. Who posts 11 not looked for. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::OpenNotice` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82225B88` | `Type5Dialog::OpenConfirm2` | Overridden | (9, 12): as `OpenConfirm` for movie 18 without `ResetPose`: data +8 = 18; m = movie 18; the menu's previous = its cursor, the cursor = m's +40960; page object +4, +0, +8 = 2, +20 = +12 = -1, +24 = 0; loaded: +16 = -1, next page 2, page -1, step 4 (`OpenConfirm2Step`), state 4; else state 1. A 112-byte frame (r31 saved). Who posts 12 not looked for. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::OpenConfirm2` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82225A08` | `Type5Dialog::OpenNotice2` | Overridden | (9, 13): as `OpenNotice` for movie 19: data +8 = 19, page object +4, +0, +8 = 3, +20 = +12 = -1, +24 = 0; loaded: +16 = -1, next page 3, page -1, step 6 (the type 2 dialog's `OpenNotice2Step`), state 4; else state 1. Leaf. Who posts 13 not looked for. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::OpenNotice` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_822257A8` | `Type5Dialog::OnLeft` | Overridden | (7, 4) → state 6: `VirtualFree(p, 0, 0x8000)` for `[0x82814E3C]`, `[0x82814E40]` and `[0x82814E44]`, then the three set to 0. r3 not read. A 112-byte frame (`__savegprlr_29`). No return value. As `Type2Dialog::OnLeft`. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::OnLeft` (`src/type5_mode/dialog_handlers.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, a functor the machine calls: the adapter stays; harness `type5_mode`. |
| `sub_82225D10` | `Type5Dialog::ConfirmPage` | Overridden | Pages 0 and 2 (from `Draw`): r3 = the dispatcher. A 144-byte frame (`__savegprlr_25`). `Menu::Step([0x82814E40])`; `Menu::Pressed(menu, 159)` (accept): `MenuSound::Play(0)`, the page object's +8 = its page (when not 0), step 1 (`CloseConfirmStep`), +16 = -1, next page 0, page -1, and (14, 0, 0) posted for the cursor 0 (yes) or (15, 0, 0) for 1 (no). Then `Pressed(158)` (back): `MenuSound::Play(2)`, the same close and (17, 0, 0). Then with the cursor moved (the menu's +8): `MenuSound::Play(4)`, +8 = the page (when not 0), step 8 (`MoveCursorStep`), +16 = -1, next page 0, page -1. Then with the data's +12 set (`OnInterrupt`): the same close and (20, 0, 0). Posts straight into the ring (+204). Each check runs after the one before, so two can fire in one Draw. No return value. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::ConfirmPage` (`src/type5_mode/dialog_pages.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Draw`'s table: the adapter stays; harness `type5_mode`. |
| `sub_82225F30` | `Type5Dialog::NoticePage` | Overridden | Page 1 (from `Draw`): r3 = the dispatcher. A 144-byte frame (`__savegprlr_25`). `Menu::Step([0x82814E40])`; on 159 (accept) or 158 (back): `MenuSound::Play(0)`, the page object's +8 = its page (when not 1), step 3 (`CloseNoticeStep`), +16 = -1, next page 1, page -1, (16, 0, 0) posted. Then with the data's +12 set: the same close and (20, 0, 0). No return value. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::NoticePage` (`src/type5_mode/dialog_pages.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Draw`'s table: the adapter stays; harness `type5_mode`. |
| `sub_82226088` | `Type5Dialog::Notice2Page` | Overridden | Page 3: as `NoticePage` with page 3 and step 7 (`CloseNotice2Step`); the same messages 16 and 20. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::NoticePage` (`src/type5_mode/dialog_pages.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Draw`'s table: the adapter stays; harness `type5_mode`. |
| `sub_822261E0` | `Type5Dialog::OpenConfirmStep` | Overridden | Step 0: r3 = the page object's +24 (the started word). A 128-byte frame (`__savegprlr_29`). m = movie 16 (`UiMovies::Find`). The first frame (+24 0): `MenuSound::Play(5)`, `UiMovie::PlayNamed(m, 5, 0, 0, 0, 0)` (the opening), +24 = 1. Then `UiMovie::Run(m, 0)`; when it returns 0 (done): `UiMovie::ResetPose(m)`, `PlayNamed(m, {1, 2}[cursor `[0x82814E40]` +0], 0, 0, 0, 0)` (the highlight of yes or no, from a table in its frame) and `UiMovie::LoopLast(m)`. Returns 1 when done, else 0. As the type 2 dialog's with the `ResetPose` added. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::OpenConfirmStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_822262B8` | `Type5Dialog::CloseConfirmStep` | Overridden | Step 1: r3 = the started word. A 112-byte frame (r30, r31 saved). m = movie 16; first frame: `MenuSound::Play(5)`, `PlayNamed(m, 6, 0, 0, 0, 0)` (the closing), +24 = 1; when `Run(m, 0)` returns 0: `UiMovie::FinishAll(m)` and the data's +4 = 9 (`Update` then goes to state 9 and posts 18). Returns 1 when done. As `Type2Dialog::CloseConfirmStep`. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::CloseConfirmStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_82226370` | `Type5Dialog::CloseNoticeStep` | Overridden | Step 3: as `CloseConfirmStep` on movie 17 with animation 3 and no `FinishAll`. As the type 2 dialog's. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::SimpleStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_82226420` | `Type5Dialog::OpenConfirm2Step` | Overridden | Step 4: movie 18; the first frame `MenuSound::Play(5)`, `PlayNamed(m, 5, 0, 0, 0, 0)`, +24 = 1; returns 1 when `Run(m, 0)` returns 0. No highlight (the type 2 dialog's plays one). A 112-byte frame (r30, r31 saved). Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::SimpleStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_822264B8` | `Type5Dialog::CloseConfirm2Step` | Overridden | Step 5: as the type 2 dialog's `CloseConfirm2Step` (movie 18, animation 6, the data's +4 = 9 when done). No page or open sets step 5 (page 2 closes through step 1), so probably never run. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::SimpleStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_82226568` | `Type5Dialog::CloseNotice2Step` | Overridden | Step 7: as `CloseNoticeStep` on movie 19 (animation 3, the data's +4 = 9 when done). As the type 2 dialog's. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::SimpleStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_82226618` | `Type5Dialog::MoveCursorStep` | Overridden | Step 8: m = movie 16; first frame (+24 0): `UiMovie::FinishNamed(m, {1, 2}[previous cursor, the menu's +4])`, `PlayNamed(m, {1, 2}[cursor +0], 0, 0, 0, 0)`, `LoopLast(m)`, +24 = 1; then `UiMovie::Run(m, 0)`. Always returns 1 (one frame). A 128-byte frame (`__savegprlr_28`). As `Dialog::MoveCursorStep` (`type6-type7-modes.md`) over this dialog's menu. Seen in play. Read in full. Native (B7 loop, 2026-11-13): `type5_mode::MoveCursorStep` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
| `sub_822266D0` | `Type5Dialog::Step9` (probably) | Overridden | Step 9: `MenuSound::Play(5)`, returns 1. r3 not read. A 96-byte frame. No page or open sets step 9, so probably never run. Not seen in play. Read in full. Native (B7 loop, 2026-11-13; ported though not seen in play): `type5_mode::Step9` (`src/type5_mode/dialog_steps.cpp`); adapter `src/hooks/type5_mode.cpp` behind `--native_type5_mode`, from `Update`'s table: the adapter stays; harness `type5_mode`. |
