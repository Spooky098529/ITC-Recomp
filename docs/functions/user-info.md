# Known functions: the user info screen

The type 2 front-end mode's **user info screen** (`UserInfo::`, named
after the string `TXR_MenuUserInfo` its `Update` passes to an empty
function, a stripped debug print probably): the player's record, its
machine at the mode block's +0x102D8, set up by `UserInfo::InitStates`
`sub_8220B408` from the mode's Init (`ModeType2::Init`,
[`type2-mode.md`](type2-mode.md)) and opened by message 27, which the
hub posts on button 164 after `Type2Hub::FillPlayerRecord` has filled
the screen's data ([`type2-screens.md`](type2-screens.md)). UI movie 10
(asset 22552). It shows the record from player 0's save data (the stats
record at save +48): eleven labels (strings 284 to 294), three counts
(stats +12, +14, +16 as words, string 8), a ratio of +12 to +22 times
100 (string 453; probably the win rate), stats +5968, two distances
(stats +5976 and +5904, floats) and a third (+5992, truncated) shown
times 0.62138 (kilometers in miles, strings 452 and 8), a number (stats
+6024), the play time (`PlayTime::Add`: hours up to 9999, minutes,
seconds; string 529, refreshed every Draw) and two more words (the
garage's count and save +0x1991C, string 444), with four fixed texts
(441 twice, 446, 435). Button 158 or 164 leaves it back to the hub.

It has the shape the type 2 mode's other machines have
(`type2-screens.md`'s intro; keys (state, message, 0, 0), member
function functors over vtable `0x82011918`, r3 the machine and r4 the
dispatcher at the mode's +36, which the other files call the mode, whose
message ring at +204 (50 entries of 12 bytes, the count +600 and the
start +604) the handlers post to): (0, 1) `Create` → 1 or 9; (1, 1) → 2
`OpenMovie`; (2, 1) `WaitForLoad` → 9; (9, 27) `Reenter` → 4 or 1; (4,
1) → 5 the empty `sub_82106540`; in 5, Draw (message 1) runs
`UpdatePage`'s page and Update (message 0) `Update`'s transition; (5, 5)
→ 8 the empty function and (8, 3) `ScreenMachine::Wait` → 9; (5, 4) and
(9, 4) `Close` → 7, (7, 4) `Destroy` → 6. Its pages: 0 `MenuPage`, 1
the empty function (while closing); its transitions: 0 `Opening` (the
texts set, animation 0), 1 `Closing` (animation 1, then state 9).

Its blocks are globals (`../known-data.md`), each `VirtualAlloc`ed by
`Create` and freed by `Destroy` (the globals set to 0): `[0x826D84C4]`
the page object (28 bytes, laid out as the other machines': +0 the page
`UpdatePage` runs, -1 while a transition runs; +4 the page after it; +8
the page before; +12 the running transition, -1 none; +16 the queued
one; +20; +24 the transition's started word), `[0x826D84C8]` a menu (68
bytes, a "front-end menu": items 0 to 7, no wrap, stepped every Draw
but its cursor not read) and `[0x826D84CC]` the data (60 bytes: +0 1
once loaded, +4 the state `Update` moves the machine to (21 none, 9 from
`Closing`), +8 to +56 the record `Type2Hub::FillPlayerRecord` writes).

Names are ours ("user info" from the string; what the record's fields
count isn't known). Read 2026-11-11 by the B7 loop's mapping agent:
every function in full (`InitStates` through
`itc_recomp/tools/init_step_tables.py`). Everything seen in play; every
group ready (`b7_groups.py`; `UpdatePage`'s and `Update`'s tables are
indirect calls the tool doesn't count, so the pages and transitions go
before them). The `UiMovie::SetText` calls (`sub_8211C1F8`, varargs)
leave the argument registers they don't set as the caller's leftovers
(r8 to r10, or r10 only): by the user's call for the parking area
(2026-10-30), native callers pass 0 for those. Shared, with rows
elsewhere: `StateMachine::Add` `sub_82106158`
([`game-modes.md`](game-modes.md)), the empty `sub_82106540`
([`audio.md`](audio.md)), `ScreenMachine::Wait` `sub_82277068`
([`type6-type7-modes.md`](type6-type7-modes.md)), `VirtualAlloc`
`sub_82295398` and `VirtualFree` `sub_82295330` ([`heap.md`](heap.md)),
`Menu::Step` `sub_82186568` and `Menu::Pressed` `sub_821864C8`
([`menu-helpers.md`](menu-helpers.md)), `MenuSound::Play` `sub_821103F0`
([`menu-sounds.md`](menu-sounds.md)), `SaveData::Of` `sub_82105620`
([`vehicle-accessors.md`](vehicle-accessors.md)), `PlayTime::Add`
`sub_821109B8` ([`save-interface.md`](save-interface.md)) and the UI
movie calls `UiMovies::Open` `sub_8211E808`, `Find` `sub_8211B3F0`,
`IsLoaded` `sub_8211BE98`, `Delete` `sub_8211E258`, `UiMovie::Play`
`sub_8211D978`, `Run` `sub_8211E1B8`, `FinishAll` `sub_8211DFF8`,
`SetText` `sub_8211C1F8` and `SetNumberText` `sub_8211C740`
([`ui-movies-game.md`](ui-movies-game.md)).

Made native 2026-11-11 by the B7 loop (`--native_user_info`,
`itc_recomp/src/user_info/`, adapters in `src/hooks/user_info.cpp`,
the `user_info` harness): all 14, the page and transitions reached
through the tables `UpdatePage` and `Update` build on their frames (as
the plate editor's), the empty debug call `Update` makes first left out
(it returns at once), and `SetText`'s unset varargs passed as 0. The
attract race doesn't reach the screen (the career's hub, button 164).

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
| `sub_8220B408` | `UserInfo::InitStates` | Overridden | r3 = the user info machine (the type 2 mode block's +0x102D8): 12 transitions through `StateMachine::Add` (`sub_82106158`), keys (state, message, 0, 0), each a member function functor over vtable `0x82011918` (the machine, the function) copied into the frame (six-word copy loops), the next state at the copy's +24 (0 for none): (0, 1) `Create` `sub_8220AA50`; (1, 1) → 2 `OpenMovie` `sub_8220AA40`; (2, 1) `WaitForLoad` `sub_8220B070`; (4, 1) → 5 the empty `sub_82106540`; (5, 1) `UpdatePage` `sub_8220B260`; (5, 0) `Update` `sub_8220B2C0`; (5, 5) → 8 the empty `sub_82106540`; (8, 3) `ScreenMachine::Wait` `sub_82277068`; (9, 27) `Reenter` `sub_8220AB60`; (5, 4) and (9, 4) `Close` `sub_8220ABB8`; (7, 4) → 6 `Destroy` `sub_8220A9D8`. A 912-byte frame (r27 to r31 and lr saved). It decodes as the other Inits' step tables (`itc_recomp/tools/init_step_tables.py`, the functors' words the original never writes copied as the frame holds them). Returns the last `Add`'s r3. Only caller `ModeType2::Init` `sub_82197278` ([`type2-mode.md`](type2-mode.md)), whose group it is in. Seen in play. Read in full (through the tool). Plain C++ in `itc_recomp/src/user_info/init.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), from the type 2 mode's Init; its step table (`init_step_tables.py`) over `StateMachine::Add`'s plain C++; checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220AA50` | `UserInfo::Create` | Overridden | (0, 1): r3 = the machine. A 112-byte frame (r30, r31 and lr saved). +9608 = 0; `VirtualAlloc(0, n, 0x1000, 4)` three times: the page object (28 bytes, `[0x826D84C4]`: +0, +4, +8, +24 = 0, +12, +20 = -1, +16 left), the menu (68 bytes, `[0x826D84C8]`: +16 = 7 (the last item), +0 to +12, +20 to +40, +48, +52 and +64 = 0, +44, +56 and +60 left) and the data (60 bytes, `[0x826D84CC]`: +0 = 0, +4 = 21); no check for 0. +9604 = 1 when +9612 (set by the mode's Init) is nonzero, else 9. Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (`VirtualAlloc` through its override); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220AA40` | `UserInfo::OpenMovie` | Overridden | (1, 1) → 2: a jump to `UiMovies::Open(10, 22552)` (UI movie 10, its asset). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (`UiMovies::Open` through its override); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220B070` | `UserInfo::WaitForLoad` | Overridden | (2, 1): r3 = the machine. A 96-byte frame. With +9608 0: when `UiMovies::IsLoaded(10)`, +9608 = 1 (the state stays 2); with +9608 set: the data's +0 = 1 and +9604 = 9 (suspended until message 27). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (`UiMovies::IsLoaded`'s plain C++); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220AB60` | `UserInfo::Reenter` | Overridden | (9, 27), opened by the hub: r3 = the machine. With +9608 set: the page object's +8 = its +0 when that's nonzero, +12 = 0 (transition 0, `Opening`), +16 = -1, +4 = 0, +0 = -1; +9604 = 4. Else +9604 = 1 (open the movie first). Leaf. Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls; checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220B260` | `UserInfo::UpdatePage` | Overridden | (5, 1), every Draw: r3 = the machine, r4 = the dispatcher. A 96-byte frame. Unless the page (`[0x826D84C4]` +0) is -1, the page's handler from a two-entry table on the frame (+80 page 0 `MenuPage` `sub_8220B0D8`, +84 page 1 the empty `sub_82106540`), called with r3 = the dispatcher. Seen in play. Read in full. Ready (its table an indirect call). Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (the page through the guest from its table); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220B2C0` | `UserInfo::Update` | Overridden | (5, 0), every Update: r3 = the machine. A 112-byte frame (r30, r31 and lr saved). The empty `sub_82106540(100, 30, 0, "TXR_MenuUserInfo" (0x8200CB08))` (a debug print left in, probably); movie 10 (`UiMovies::Find`); with a running transition (the page object's +12, not -1): from 0 up, the transition from a two-entry table on the frame (+80 `Opening` `sub_8220B1C8`, +84 `Closing` `sub_8220AC60`) with r3 = the page object + 24 (its started word); a nonzero answer finishes it as `ScreenPages::Finish` does (the queued one (+16) runs next with +16 = -1 and +20 = 0, or with none queued +12 = -1 and the page +0 = +4; +24 = 0); a negative transition other than -1 is finished at once the same way without a call; with none running, `UiMovie::Run(movie, 0)`. Then when the data's +4 isn't 21, +9604 = it and +4 = 21. Seen in play. Read in full. Ready (its table an indirect call). Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (the transitions through the guest from its table, `UiMovie::Run` through its override; the empty debug call left out); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220B0D8` | `UserInfo::MenuPage` | Overridden | Page 0, r3 = the dispatcher. A 112-byte frame (r30, r31 and lr saved). `Menu::Step([0x826D84C8])` (its answer not read); `UserInfo::SetPlayTimeText`; then on `Menu::Pressed(menu, 158)` or `Menu::Pressed(menu, 164)` (back, or the button that opened it): the page object's +8 = its +0 when nonzero, +4 = 0, +16 = -1, +0 = -1, +12 = 1 (transition 1, `Closing`); message 6 queued in the dispatcher's ring (+204; the hub's, back to the hub); `MenuSound::Play(2)` (back). Seen in play. Read in full. Ready (with `SetPlayTimeText`). Plain C++ in `itc_recomp/src/user_info/pages.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), reached through `UpdatePage`'s table (`Menu::Step`, `Menu::Pressed` and `MenuSound::Play` through their overrides, `SetPlayTimeText`'s plain C++); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220AFC0` | `UserInfo::SetPlayTimeText` | Native | No arguments read. A 112-byte frame (r31 and lr saved). Movie 10 (`UiMovies::Find`); `PlayTime::Add(frame + 80, SaveData::Of(0) + 48)`: its 8 bytes into the data's +44 and +48 (the hours; the minutes and seconds bytes at +48 and +49); `UiMovie::SetText(movie, -1, 23, 529, hours (at most 9999), the byte +48, the byte +49)`, r10 left as the hours (a register leftover: native callers pass 0, the user's call of 2026-10-30). Every Draw from `MenuPage`. Seen in play. Read in full. In `MenuPage`'s group. Plain C++ in `itc_recomp/src/user_info/pages.cpp` with no adapter: only `MenuPage` (native) calls it, the flag switches that (`PlayTime::Add` and `UiMovie::SetText` through their overrides, `SaveData::Of`'s plain C++, r10 passed as 0); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220B1C8` | `UserInfo::Opening` | Overridden | Transition 0, r3 = the page object + 24 (its started word). A 112-byte frame (r30, r31 and lr saved). Movie 10; on the first call (+24 0): `UserInfo::SetTexts`, `UiMovie::Play(movie, -1, 0, 0, 0, 0, 0)` (animation 0), +24 = 1; every call `UiMovie::Run(movie, 0)`; returns 1 when `Run` answers 0 (the animation done), else 0. Seen in play. Read in full. Ready (with `SetTexts`). Plain C++ in `itc_recomp/src/user_info/pages.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), reached through `Update`'s table (`UiMovie::Play` and `Run` through their overrides, `SetTexts`'s plain C++); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220AD08` | `UserInfo::SetTexts` | Native | No arguments read. A 192-byte frame (r29 to r31, f31 and lr saved). Movie 10's texts (`UiMovie::SetText(movie, -1, node, string, ...)`): nodes 3 to 7 and 13 to 18 (a table on the frame) strings 284 to 294; nodes 25 and 26 string 441, 27 446, 28 435; from the data (`[0x826D84CC]`): nodes 8, 9, 10 string 8 with +8, +12, +16; node 11 string 453 with +20 (a float, as a double in r7); node 12 string 8 with +24; nodes 19 and 20 string 452 with +28 and +32 (floats) × `[0x8200A510]` (0.62138, a double: kilometers to miles), rounded to single; node 21 string 8 with +36 (a word, through a double, × 0.62138, rounded to single, truncated: fctiwz, NaN 0x80000000, from INT_MAX saturated, stored at the frame's +80 and read back); node 22 `UiMovie::SetNumberText(movie, -1, 22, +40)`; node 23 string 529 with +44 (at most 9999), the bytes +48 and +49 (as `SetPlayTimeText`); node 24 string 444 with +52 and +56. The registers a call doesn't set (r8 to r10 after one argument, r10 after three) keep what the last call left (leftovers: native callers pass 0, the user's call of 2026-10-30). Only caller `Opening`. Seen in play. Read in full. Plain C++ in `itc_recomp/src/user_info/pages.cpp` with no adapter: only `Opening` (native) calls it, the flag switches that (`UiMovie::SetText` and `SetNumberText` through their overrides, the unset varargs passed as 0); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220AC60` | `UserInfo::Closing` | Overridden | Transition 1, r3 = the page object + 24. A 112-byte frame (r30, r31 and lr saved). Movie 10; on the first call: `UiMovie::Play(movie, -1, 1, 0, 0, 0, 0)` (animation 1), +24 = 1; every call `UiMovie::Run(movie, 0)`; when it answers 0 (done) the data's +4 = 9 (`Update` then moves the machine to 9) and returns 1, else 0. Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/pages.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), reached through `Update`'s table; checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220ABB8` | `UserInfo::Close` | Overridden | (5, 4) and (9, 4): r3 = the machine, r4 = the dispatcher. A 112-byte frame (r31 and lr saved). +9604 = 7; `UiMovie::FinishAll` on movie 10 (`UiMovies::Find`), `UiMovies::Delete(10)`; message 4 queued in the dispatcher's ring (+204) for (7, 4). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (`UiMovie::FinishAll` and `UiMovies::Delete` through their overrides); checked by the `user_info` harness (B7 loop, 2026-11-11). |
| `sub_8220A9D8` | `UserInfo::Destroy` | Overridden | (7, 4) → 6: `VirtualFree(.., 0, MEM_RELEASE)` on `[0x826D84C4]`, `[0x826D84C8]` and `[0x826D84CC]`, then the three globals = 0. A 112-byte frame (r29 to r31 and lr saved). Seen in play. Read in full. Ready. Plain C++ in `itc_recomp/src/user_info/handlers.cpp` behind an adapter in `src/hooks/user_info.cpp` (`--native_user_info`), a functor the machine calls (`VirtualFree` through its override); checked by the `user_info` harness (B7 loop, 2026-11-11). |
