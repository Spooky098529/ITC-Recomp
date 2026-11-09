# Known functions: the type 2 mode's dialog

The type 2 front-end mode's **dialog** (its machine at the mode block's
+0x1E4C8, set up by `Type2Dialog::Init` from the mode's Init
`sub_82197278`; the mode and its other machines are in
[`type2-screens.md`](type2-screens.md),
[`car-dealer.md`](car-dealer.md), [`options-screen.md`](options-screen.md),
[`save-screen.md`](save-screen.md) and the leads in `../loops/leads.md`):
the yes/no questions and the notices the mode's screens show over UI
movies 16 to 19 (assets 22559 to 22562), as the type 6 and 7 modes'
dialogs do (`Type6Dialog::`, [`type6-type7-modes.md`](type6-type7-modes.md),
whose `Dialog::OpenMovies` and `Dialog::Leave` this one registers too).
Its blocks are globals (`../known-data.md`), each `VirtualAlloc`ed by
`OnEnter` and freed by `OnLeft`: `[0x826D83F8]` the page object (28
bytes, laid out as the car dealer's `0x826D83E8`: +0 the page `Draw`
runs, -1 while a step runs; +4 the page after the step; +8 the page
before; +12 the step `Update` runs, -1 none; +16 the step queued after
it; +20; +24 the step's started word, the steps' r3), `[0x826D83FC]` the
menu (68 bytes, a "front-end menu": items 0 to 1, wrapping; yes and no)
and `[0x826D8400]` the dialog's data (68 bytes, only +0 to +8 used: +0 1
once the movies are loaded, +4 the machine's state queued by the close
steps (21 none, 9), +8 the movie shown, 16 to 19). Two words beside
them: `[0x82636200]` the dialog showing (0 to 3, written by the pages
and the open handlers, read by `Cancel`) and `[0x82636204]` its status
(-1 loading the movies, 3 idle, 5 an open asked, 0 just shown, 4 shown
and asked by `TakesInput`, 1 taking input, 2 cancelled), as the type 6
dialog's +9756.

The machine (keys (state, message), r3 = the machine, r4 = the mode,
whose message ring at +204 the handlers post to): (0, 1) `OnEnter`
makes the blocks and goes to 1 (the mode sets +9612 = 1) or 9; (1, 1)
`Dialog::OpenMovies` → 2; (2, 1) `WaitForMovies` → 9; in 9 an open
message picks the dialog and goes to 4 (or 1 when the movies aren't
loaded yet: the request is then probably lost, as the type 6 dialog's);
(4, 1) `Shown` → 5; in 5, Update (message 0) runs the steps and Draw
(message 1) the pages; a close step queues state 9 (the data's +4), and
`Update` then posts message 42 and goes back to 9; (5, 48) `Cancel`;
(5, 5) → 8 (the empty `sub_82106540`) and (8, 3) `Wait` → 9; (5, 4) and
(9, 4) `Dialog::Leave` → 7, (7, 4) `OnLeft` → 6. The dialogs:

| Dialog | Opened by | Movie | Page | Steps (open, close, cursor) | Replies |
|---|---|---|---|---|---|
| 0, a yes/no | message 34 (the hub's quit and second questions, `type2-screens.md`; the car dealer's buy dialogs, `car-dealer.md`) | 16 | 0 | 0, 1, 8 | 38 yes, 39 no, 41 back |
| 1, a notice | message 35 (`Type2Hub::RecordPage`, the player's record) | 17 | 1 | 2, 3 | 40 |
| 2, a yes/no | message 36 (not looked for) | 18 | 2 | 4, 5, 9 | 38 yes, 39 no, 41 back |
| 3, a notice | message 37 (the car dealer: the garage full, the price above the money) | 19 | 3 | 6, 7 | 40 |

The yes/no dialogs start on the cursor the caller left in the movie's
word +40960 (0 yes, 1 no). The replies are posted (message, 0, 0) to the
mode's ring when the page starts its close step; `Cancel` (message 48,
probably from the mode's `sub_82197858`, not seen in play) closes the
page showing as its back button does (posting 41, or 40 for dialog 3).
`MenuSound::Play` cues: 0 accept (and both buttons on the notices), 2
back, 4 the cursor moved, 5 each open and close animation.

Read for the B7 loop's mapping agent, 2026-11-09, from the transitions
the Init adds (its frame's stores replayed by a script following its
registers and six-word copy loops), `Draw`'s page table and `Update`'s
step table; every function read in full. Everything seen in play is
ready (`b7_groups.py`: 21 groups of one function each); `Draw`'s and
`Update`'s tables are indirect calls the tool doesn't count, so the pages
and steps go before them. Shared, with their rows elsewhere:
`StateMachine::Add` `sub_82106158` ([`game-modes.md`](game-modes.md)),
the empty `sub_82106540` ([`audio.md`](audio.md); the (5, 5)
transition, and `Update`'s first call), `Dialog::OpenMovies`
`sub_82225810` and `Dialog::Leave` `sub_82225C70`
([`type6-type7-modes.md`](type6-type7-modes.md)), the UI movie calls
`UiMovies::Find` `sub_8211B3F0`, `IsLoaded` `sub_8211BE98`,
`UiMovie::ResetPose` `sub_8211ABA8`, `PlayNamed` `sub_8211DAC8`, `Run`
`sub_8211E1B8`, `LoopLast` `sub_8211DDE8`, `LoopNamed` `sub_8211DDE0`,
`FinishNamed` `sub_8211DF20` and `FinishAll` `sub_8211DFF8`
([`ui-movies-game.md`](ui-movies-game.md)), `Menu::Step`
`sub_82186568` and `Menu::Pressed` `sub_821864C8`
([`menu-helpers.md`](menu-helpers.md)), `MenuSound::Play`
`sub_821103F0` ([`menu-sounds.md`](menu-sounds.md)), `VirtualAlloc`
`sub_82295398` and `VirtualFree` `sub_82295330` ([`heap.md`](heap.md)).

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
| `sub_82188178` | `Type2Dialog::Init` | | r3 = the dialog's machine (the type 2 mode block's +0x1E4C8). A 1,168-byte frame (r27 to r31 and lr saved): 16 `StateMachine::Add` (`sub_82106158`) calls, each with a {`0x82011918`, the machine, handler} functor copied into the frame (six-word copy loops) and the state to go to at the copy's +24 (0 for none): (0, 1) → `OnEnter` `sub_821869B8`; (1, 1) → state 2, `Dialog::OpenMovies` `sub_82225810`; (2, 1) → `WaitForMovies` `sub_821880D0`; (4, 1) → state 5, `Shown` `sub_82186490`; (5, 1) → `Draw` `sub_82187E50`; (5, 0) → `Update` `sub_82187EC8`; (5, 48) → `Cancel` `sub_82186E40`; (5, 5) → state 8, the empty `sub_82106540`; (8, 3) → `Wait` `sub_82186AD8`; (9, 34) → `OpenConfirm` `sub_82186C30`; (9, 35) → `OpenNotice` `sub_82186AF0`; (9, 36) → `OpenConfirm2` `sub_82186D40`; (9, 37) → `OpenNotice2` `sub_82186B90`; (5, 4) and (9, 4) → `Dialog::Leave` `sub_82225C70`; (7, 4) → state 6, `OnLeft` `sub_82186428`. All keys' third and fourth words 0. No return value. From the type 2 mode's Init `sub_82197278` (`type2-screens.md`'s intro, a lead). Seen in play. Read in full (the stores replayed). |
| `sub_821869B8` | `Type2Dialog::OnEnter` | Overridden | (0, 1): the first Draw. r3 = the machine. A 128-byte frame (r28 to r31 saved). +9608 = 0 (the movies-loaded flag); `VirtualAlloc(0, 28, 0x1000, 4)` into `[0x826D83F8]`, the page object: +0, +4, +8 = 0, +20 = +12 = -1, +24 = 0 (+16 not written); `VirtualAlloc(0, 68, ..)` into `[0x826D83FC]`, the menu: +12 = 0 (first item), +16 = 1 (last), +20 = 1 (wraps), +0, +4, +8, +24 to +40, +48, +52 and +64 = 0 (+44, +56, +60 not written); `VirtualAlloc(0, 68, ..)` into `[0x826D8400]`, the data: +0 = 0, +8 = 0, +4 = 21 (no state queued). `[0x82636200]` = 0. With +9612 nonzero: state (+9604) 1 and `[0x82636204]` = -1; else state 9 and `[0x82636204]` = 3. No return value. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OnEnter` (`src/type2_dialog/handlers.cpp`), VirtualAlloc through its override from its frame; harness `type2_dialog` (the blocks made compared by their written words, then freed). A functor the machine calls: the adapter stays (`src/hooks/type2_dialog.cpp`, `--native_type2_dialog`). |
| `sub_821880D0` | `Type2Dialog::WaitForMovies` | Overridden | (2, 1): every Draw in state 2. r3 = the machine. A 96-byte frame (r31 saved). While +9608 is 0: `UiMovies::IsLoaded` 16, 17, 18, 19 in turn (stops at the first not loaded); all loaded: +9608 = 1. On the next Draw (+9608 set): the data's +0 = 1, state 9, `[0x82636204]` = 3. No return value. As `Type6Dialog::WaitForMovies`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::WaitForMovies` (`handlers.cpp`), `UiMovies::IsLoaded`'s plain C++; harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186490` | `Type2Dialog::Shown` | Overridden | (4, 1) → state 5: `[0x82636204]` = 0 (just opened; the pages and `TakesInput` read it). Leaf. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::Shown` (`handlers.cpp`); harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82187E50` | `Type2Dialog::Draw` | Overridden | (5, 1): every Draw while shown. r3 = the machine, r4 = the mode. A 112-byte frame holding the page table {`ConfirmPage` `sub_82187160`, `NoticePage` `sub_82187508`, `Confirm2Page` `sub_82187330`, `Notice2Page` `sub_82187618`}: with the page (`[0x826D83F8]` +0) not -1, calls page[+0] with r3 = the mode (r4 unchanged; no bounds check). Returns the page's r3 (ignored). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::Draw` (`src/type2_dialog/handlers.cpp`), the page called through the guest from the table it writes in its frame (r4 to r10 as the original leaves them where known); harness `type2_dialog` (the pages recompiled on both sides). A functor the machine calls: the adapter stays. |
| `sub_82187EC8` | `Type2Dialog::Update` | Overridden | (5, 0): every Update while shown. r3 = the machine, r4 = the mode. A 208-byte frame (r25 to r31 saved). First `sub_82106540(100, 30, 0, 0x82009F28)` (empty). m = the movie with id data +8 (`UiMovies::Find` on `0x826C5CCC`). The step table in the frame: {`OpenConfirmStep` `sub_82187728`, `CloseConfirmStep` `sub_821877F8`, `OpenNoticeStep` `sub_821878B0`, `CloseNoticeStep` `sub_82187948`, `OpenConfirm2Step` `sub_821879F8`, `CloseConfirm2Step` `sub_82187AC8`, `OpenNotice2Step` `sub_82187B80`, `CloseNotice2Step` `sub_82187C18`, `MoveCursorStep` `sub_82187CC8`, `MoveCursor2Step` `sub_82187D80`}. With no step (+12 = -1): `UiMovie::Run(m, 0)`. Else calls step[+12] (r3 = the page object's +24, r4 = `0x82187948`, a leftover of the table's build, not read; no bounds check); when it returns nonzero (or the step is another negative value): with no step queued (+16 = -1) the page becomes the next page (+0 = +4) and +12 = -1; else +12 = the queued step, +16 = -1, +20 = 0; then +24 = 0. Then with a state queued (data +4 not 21): for 9, message (42, 0, 0) posted straight into the mode's ring (+204 + 12 × ((+804 + 1 + +808 - 1) mod 50), the count +804 incremented first: the dialog closed); state = data +4, data +4 = 21, and `[0x82636204]` = 3 unless it's 5 (an open asked meanwhile). No return value. As `Type6Dialog::Update`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::Update` (`handlers.cpp`), the step called through the guest from its frame's table (r3 the started word, r4 to r10 the original's leftovers), UiMovie::Run through its override; the empty `sub_82106540` call left out (it returns at once); harness `type2_dialog` (the steps recompiled on both sides, the steps' saves of Update's registers below its frame not compared). A functor: the adapter stays. |
| `sub_82186E40` | `Type2Dialog::Cancel` | Overridden | (5, 48): r3 = the machine, r4 = the mode. A 128-byte frame (r28 to r31 saved). For each dialog k from 0 to 3 in turn: on page k, +8 = k (if not already), +12 = its close step (1, 3, 5, 7), +16 = -1, +4 = k, +0 = -1 and its message posted; else with `[0x82636200]` = k (its step running) the message posted alone. The message is (41, 0, 0) for dialogs 0 to 2 and (40, 0, 0) for 3, straight into the mode's ring. Then `[0x82636204]` = 2. No return value. Message 48 probably from the mode's `sub_82197858` (not seen in play, no row; beside the mode's Init), which posts it when `TakesInput` answers 1. Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::Cancel` (`handlers.cpp`); harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186AD8` | `Type2Dialog::Wait` | Overridden | (8, 3): state 9, `[0x82636204]` = 3. r3 = the machine. Leaf. State 8 comes from (5, 5) (who posts 5 not looked for). As `Type6Dialog::Wait`. Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::Wait` (`handlers.cpp`); harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186C30` | `Type2Dialog::OpenConfirm` | Overridden | (9, 34): r3 = the machine. A 112-byte frame (r30, r31 saved). Data +8 = 16; m = movie 16 (`UiMovies::Find`), `UiMovie::ResetPose(m)`; the menu's +4 (previous) = its cursor +0, the cursor = m's +40960 (the default choice the caller set: 0 yes, 1 no); page object +4, +0, +8 = 0, +20 = +12 = -1, +24 = 0; `[0x82636200]` = 0. With the movies loaded (+9608): +8 kept (it is 0), step 0 (`OpenConfirmStep`), +16 = -1, next page 0, page -1, state 4; else state 1. Then `[0x82636204]` = 5. No return value. As `Type6Dialog::OpenConfirm`. Message 34 from the hub (`Type2Hub::`, `type2-screens.md`) and the car dealer (`car-dealer.md`). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OpenConfirm` (`handlers.cpp`), `UiMovie::ResetPose` through its override, `UiMovies::Find` its plain C++; harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186AF0` | `Type2Dialog::OpenNotice` | Overridden | (9, 35): r3 = the machine. Data +8 = 17 (no `Find`); page object +24 = 0, +4, +0, +8 = 1, +20 = +12 = -1; `[0x82636200]` = 1. With the movies loaded: +8 kept (it is 1), +16 = -1, next page 1, page -1, step 2 (`OpenNoticeStep`), state 4; else state 1. Then `[0x82636204]` = 5. Leaf. Message 35 from `Type2Hub::RecordPage` (`sub_821900B8`, `type2-screens.md`). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OpenNotice` (`handlers.cpp`, shared with `OpenNotice2`); harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186D40` | `Type2Dialog::OpenConfirm2` | Overridden | (9, 36): as `OpenConfirm` for movie 18 without `ResetPose`: data +8 = 18; m = movie 18; the menu's previous = its cursor, the cursor = m's +40960; page object +4, +0, +8 = 2, +20 = +12 = -1, +24 = 0; `[0x82636200]` = 2; loaded: +16 = -1, next page 2, page -1, step 4 (`OpenConfirm2Step`), state 4; else state 1. Then `[0x82636204]` = 5. A 112-byte frame (r31 saved). Who posts 36 not looked for. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OpenConfirm2` (`handlers.cpp`; r3 the movie table pair's address `Find` leaves); harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186B90` | `Type2Dialog::OpenNotice2` | Overridden | (9, 37): as `OpenNotice` for movie 19: data +8 = 19, page object +24 = 0, +4, +0, +8 = 3, +20 = +12 = -1; `[0x82636200]` = 3; loaded: +16 = -1, next page 3, page -1, step 6 (`OpenNotice2Step`), state 4; else state 1. Then `[0x82636204]` = 5. Leaf. Message 37 from the car dealer (the garage full or the price above the money, `car-dealer.md`). Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::OpenNotice` (`handlers.cpp`); harness `type2_dialog`. A functor: the adapter stays. |
| `sub_82186428` | `Type2Dialog::OnLeft` | Overridden | (7, 4) → state 6: `VirtualFree(p, 0, 0x8000)` for `[0x826D83F8]`, `[0x826D83FC]` and `[0x826D8400]`, then the three set to 0. r3 not read. A 112-byte frame (r29 to r31 saved). No return value. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OnLeft` (`handlers.cpp`), VirtualFree through its override; harness `type2_dialog` (the layer's pages the case made, whether each was freed). A functor: the adapter stays. |
| `sub_821864A0` | `Type2Dialog::TakesInput` (probably) | Overridden | No arguments. With `[0x82636204]` = 0 (just shown) sets it to 4 (the page then skips its next input frame); returns 1 when `[0x82636204]` is 1 (a page taking input), else 0. From `sub_82197858` (the type 2 mode's, not seen in play, no row; not read in detail: on one of its paths, with `[0x82636204]` not 3, a 1 here makes it post message 48 to its r3's +208 ring, and it answers 1 when `[0x82636204]` is 3). Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::TakesInput` (`handlers.cpp`); harness `type2_dialog`. Its caller `sub_82197858` is recompiled: the adapter stays. |
| `sub_82187160` | `Type2Dialog::ConfirmPage` | Overridden | Page 0 (from `Draw`): r3 = the mode. A 144-byte frame (r25 to r31 saved). `[0x82636200]` = 0. With `[0x82636204]` = 4: sets it 1, nothing else (one frame without input). Else `[0x82636204]` = 1; `Menu::Step([0x826D83FC])`; `Menu::Pressed(menu, 159)` (accept): `MenuSound::Play(0)`, +8 = the page (if not 0), step 1 (`CloseConfirmStep`), +16 = -1, next page 0, page -1, message (38, 0, 0) for the cursor 0 (yes) or (39, 0, 0) otherwise (no); then `Pressed(158)` (back): `MenuSound::Play(2)`, the same close and (41, 0, 0); then with the cursor moved (menu +8): `MenuSound::Play(4)`, +8 kept, step 8 (`MoveCursorStep`), +16 = -1, next page 0, page -1. Three checks in turn, not exclusive (`Menu::Pressed` answers only when the step didn't move). Posts straight into the mode's ring. No return value. As `Type6Dialog::ConfirmPage`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::ConfirmPage` (`src/type2_dialog/pages.cpp`), Menu::Step, Menu::Pressed and MenuSound::Play through their overrides, the replies posted inline; harness `type2_dialog`. The adapter stays: `Draw` calls it through its table. |
| `sub_82187508` | `Type2Dialog::NoticePage` | Overridden | Page 1 (from `Draw`): r3 = the mode. A 112-byte frame (r30, r31 saved). `[0x82636200]` = 1; `[0x82636204]` = 1 (after reading it); with it 4 before: nothing else. Else `Menu::Step`; on 159 (accept) or 158 (back): `MenuSound::Play(0)` for both, +8 = the page (if not 1), +16 = -1, page -1, step 3 (`CloseNoticeStep`), next page 1, message (40, 0, 0) posted. No return value. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::NoticePage` (`pages.cpp`, shared with `Notice2Page`); harness `type2_dialog`. The adapter stays (`Draw`'s table). |
| `sub_82187330` | `Type2Dialog::Confirm2Page` | Overridden | Page 2 (from `Draw`): r3 = the mode. A 112-byte frame (r29 to r31 saved). `[0x82636200]` = 2; `[0x82636204]` = 1, with it 4 before nothing else. Else `Menu::Step`; with the cursor moved (menu +8): `MenuSound::Play(4)`, +8 = the page (if not 2), next page 2, step 9 (`MoveCursor2Step`), +16 = -1, page -1; else on 159: `MenuSound::Play(0)`, the close step 5 (`CloseConfirm2Step`) to page 2, (38, 0, 0) for the cursor 0 or (39, 0, 0); else on 158: `MenuSound::Play(2)`, the same close and (41, 0, 0). One of the three (unlike `ConfirmPage`). No return value. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::Confirm2Page` (`pages.cpp`); harness `type2_dialog`. The adapter stays (`Draw`'s table). |
| `sub_82187618` | `Type2Dialog::Notice2Page` | Overridden | Page 3 (from `Draw`): as `NoticePage` with `[0x82636200]` = 3, step 7 (`CloseNotice2Step`), next page 3, message (40, 0, 0). A 112-byte frame (r30, r31 saved). Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::NoticePage` (`pages.cpp`); harness `type2_dialog`. The adapter stays (`Draw`'s table). |
| `sub_82187728` | `Type2Dialog::OpenConfirmStep` | Overridden | Step 0 (from `Update`'s table): r3 = the page object's +24 (the started word). A 128-byte frame (r29 to r31 saved). m = movie 16 (`UiMovies::Find`). The first call (+24 0): `MenuSound::Play(5)`, `UiMovie::PlayNamed(m, 5, 0, 0, 0, 0)` (the opening), +24 = 1. Then `UiMovie::Run(m, 0)`; when it returns 0 (done): `PlayNamed(m, {1, 2}[the menu's cursor], 0, 0, 0, 0)` (the highlight of yes or no, from a table in its frame) and `UiMovie::LoopLast(m)`. Returns 1 when done, else 0. As `Dialog::OpenConfirmStep`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OpenConfirmStep` (`src/type2_dialog/steps.cpp`, shared with `OpenConfirm2Step`), MenuSound::Play and the UI movie calls through their overrides from its frame with the original's return addresses, `UiMovies::Find` its plain C++; adapter `src/hooks/type2_dialog.cpp` behind `--native_type2_dialog`; harness `type2_dialog`. The adapter stays: `Update` calls it through its table. |
| `sub_821877F8` | `Type2Dialog::CloseConfirmStep` | Overridden | Step 1: as `OpenConfirmStep` on movie 16 with animation 6 (the closing), no highlight; when `Run(m, 0)` returns 0: `UiMovie::FinishAll(m)`, then the data's +4 = 9 (`Update` then goes to state 9 and posts 42). Returns 1 when done. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::CloseConfirmStep` (`steps.cpp`), as `OpenConfirmStep`'s; harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_821878B0` | `Type2Dialog::OpenNoticeStep` | Overridden | Step 2: movie 17; first call `MenuSound::Play(5)` and `PlayNamed(m, 2, 0, 0, 0, 0)`, +24 = 1; returns 1 when `UiMovie::Run(m, 0)` returns 0. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::NoticeStep` (`steps.cpp`, the four notice steps); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_82187948` | `Type2Dialog::CloseNoticeStep` | Overridden | Step 3: as `OpenNoticeStep` on movie 17 with animation 3; when done, the data's +4 = 9 (no `FinishAll`). A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::NoticeStep` (`steps.cpp`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_821879F8` | `Type2Dialog::OpenConfirm2Step` | Overridden | Step 4: as `OpenConfirmStep` on movie 18 (animation 5, then the highlight {1, 2}[cursor] and `LoopLast`). A 128-byte frame (r29 to r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::OpenConfirmStep` (`steps.cpp`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_82187AC8` | `Type2Dialog::CloseConfirm2Step` | Overridden | Step 5: as `CloseConfirmStep` on movie 18 (animation 6), the data's +4 = 9 written before `FinishAll(m)` (the other order from step 1). A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::CloseConfirmStep` (`steps.cpp`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_82187B80` | `Type2Dialog::OpenNotice2Step` | Overridden | Step 6: as `OpenNoticeStep` on movie 19 (animation 2). A 112-byte frame (r30, r31 saved). Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::NoticeStep` (`steps.cpp`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_82187C18` | `Type2Dialog::CloseNotice2Step` | Overridden | Step 7: as `CloseNoticeStep` on movie 19 (animation 3, the data's +4 = 9). A 112-byte frame (r30, r31 saved). Not seen in play. Read in full. Native (B7 loop, 2026-11-09; ported though not seen in play): `type2_dialog::NoticeStep` (`steps.cpp`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_82187CC8` | `Type2Dialog::MoveCursorStep` | Overridden | Step 8: m = movie 16; first call (+24 0): `UiMovie::FinishNamed(m, {1, 2}[the menu's previous cursor +4])`, `PlayNamed(m, {1, 2}[cursor +0], 0, 0, 0, 0)`, `LoopLast(m)`, +24 = 1; then `UiMovie::Run(m, 0)`. Always returns 1 (one call). A 128-byte frame (r28 to r31 saved). As `Dialog::MoveCursorStep`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::MoveCursorStep` (`steps.cpp`, shared with `MoveCursor2Step`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
| `sub_82187D80` | `Type2Dialog::MoveCursor2Step` | Overridden | Step 9: as `MoveCursorStep` on movie 18, with `UiMovie::LoopNamed(m, {1, 2}[cursor])` instead of `LoopLast`. A 128-byte frame (r28 to r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `type2_dialog::MoveCursorStep` (`steps.cpp`); harness `type2_dialog`. The adapter stays (`Update`'s table). |
