# Known functions: the power graph screen

The type 6 and 7 front-end modes' **power graph screen** (`type6-type7-modes.md`):
the screen machine at the type 6 block's +52696 (its member at +52692,
vtable `0x82010A68`) and the type 7 block's +42356 (member +42352, vtable
`0x82010FD0`), set up by `PowerGraphScreen6::InitMachine` `sub_82237B78`
and `PowerGraphScreen7::InitMachine` `sub_822470A8`
([`type6-type7-modes.md`](type6-type7-modes.md), which lists the
transitions). Both machines register the same handlers, mapped here: UI
movie 9 (asset 22550) over the car the player picked, its specs written
as texts, and the torque and power graph itself drawn by the car's page
of whoever opened it (`Type6Screen0::PowerGraphPage`,
`Type7Screen0::PowerGraphPage`, `PanelSetBase::PowerGraphPage` through
`CarList::DrawCarPowerGraph` or `DrawEntryPowerGraph`,
[`power-graph.md`](power-graph.md)) only while `[0x82815EC0]` is set:
this screen sets it once its open animation has finished and clears it
when the close animation starts. The record the texts come from is
filled before the open by `PowerGraph::SetEntry` (`sub_82250368`,
[`car-list.md`](car-list.md)) or `PowerGraph::SetFromCar`
(`sub_82250488`, [`type6-type7-modes.md`](type6-type7-modes.md)). The
type 2 mode's spec sheet is a different machine with handlers of its own
(`PowerGraphScreen::`, [`type2-screens.md`](type2-screens.md)), sharing
only `PowerGraphScreen::OpenMovie` `sub_82250328`.

Its blocks are globals (`../known-data.md`), `VirtualAlloc`ed by
`OnEnter` and freed by `OnLeft`, laid out as the type 2 dialog's
(`type2-dialog.md`): `[0x82815EB4]` the page object (28 bytes: +0 the
page `Draw` runs, -1 while a step runs; +4 the page after the step; +8
the page before; +12 the step `Update` runs, -1 none; +16 the step
queued after it; +20; +24 the step's started word, the steps' r4),
`[0x82815EB8]` the menu (68 bytes, a "front-end menu", +16 = 7) and
`[0x82815EBC]` the data (92 bytes: +0 1 once movie 9 is loaded, +4 the
machine's state queued by page 1 (21 none, 9), +8 to +88 the record,
`../known-data.md`).

The machine (keys (state, message); r3 = the member, its machine at +4,
so +9608 is the machine's state, +9612 its loaded word and +9616 the
mode's word the entry reads; r4 = the mode, whose message ring at +204
the vtable slots post to). Type 6's ids, type 7's in brackets: (0, 1)
`OnEnter` makes the blocks and goes to 1 or 9; (1, 1) `OpenMovie` → 2;
(2, 1) `WaitForLoad` → 9 once movie 9 is loaded; (9, 21) [(9, 18)]
`Open` (message 21 from `Type6Screen0`'s button 163 or
`Type6PanelScreen::OpenPowerGraph`; 18 from type 7's) queues step 0 and
page 0 and goes to 4 (or to 1 when the movie isn't loaded, the request
then probably lost, as the type 2 dialog's); (4, 1) `Shown` → 5; in 5,
`Update` (message 0) runs the steps (0 the open animation, 1 the close)
and `Draw` (message 1) the pages (0 the buttons, 1 closed); page 0 posts
slot 2's message (23 [20]) when a button closes the screen and page 1
slot 3's (24 [21], `Type6Screen0::PickBack` [`Type7Screen0::PickBack`])
and queues state 9; (5, 24) [(5, 21)] → 8 (the empty `sub_82106540`) and
(8, 3) [(8, 2)] `Screen::Wait` → 9; (5, 4), (9, 4), (10, 4) [3] `Leave`
→ 7 (posting slot 1's message, 4 [3]), (7, 4) [3] → 6 `OnLeft`.

Read for the B7 loop's mapping agent, 2026-11-13, from the lead "a
front-end screen that lists cars" (`sub_8224BDC0`, `sub_82250778`,
`sub_82250890`, `sub_82250AA8`, `sub_82251320`; `sub_8224BDC0` was by
then `PanelSetBase::PowerGraphPage` in `car-list.md`), with `Draw`'s
page table and `Update`'s step table (both built on their frames: 8-byte
entries, the function and a this-adjust 0) and the three vtable slots
of each member; every function read in full. The vtables' words weren't
read (no image dump at hand): the slots are given by the functions
beside each Init posting the messages the machines answer (probably).
Everything seen in play is ready (`b7_groups.py`: 15 groups of one
function each, `Step0` with `SetTexts`); `Draw`'s and `Update`'s tables
and the slot calls are indirect calls the tool doesn't count, so the
pages, steps and slots go before them. Shared, with their rows
elsewhere: `PowerGraphScreen::OpenMovie` `sub_82250328`
([`type2-screens.md`](type2-screens.md)), `Screen::Wait` `sub_822508F0`
and the two Inits ([`type6-type7-modes.md`](type6-type7-modes.md)),
`Type6PanelScreen::OpenPowerGraph` `sub_82247058` (posting 21: probably
also type 7's slot 3, the same code; [`type67-screens.md`](type67-screens.md)),
`StateMachine::Add` `sub_82106158` ([`game-modes.md`](game-modes.md)),
the empty `sub_82106540` ([`audio.md`](audio.md); the (5, 24) transition
and `Update`'s first call), `UiMovies::Find` `sub_8211B3F0`, `IsLoaded`
`sub_8211BE98`, `Delete` `sub_8211E258`, `UiMovie::FindNode`
`sub_8211A578`, `SetText` `sub_8211C1F8`, `Play` `sub_8211D978` and
`Run` `sub_8211E1B8` ([`ui-movies-game.md`](ui-movies-game.md)),
`Menu::Step` `sub_82186568` and `Menu::Pressed` `sub_821864C8`
([`menu-helpers.md`](menu-helpers.md)), `MenuSound::Play`
`sub_821103F0` ([`menu-sounds.md`](menu-sounds.md)), `VirtualAlloc`
`sub_82295398` and `VirtualFree` `sub_82295330` ([`heap.md`](heap.md)).
`SetTexts`' varargs `UiMovie::SetText` calls fall under the user's call
of 2026-10-30 (native callers pass 0 for the arguments a call site
leaves unset). In play, type 7's screen was opened and closed (its pages
and slots 1 and 2 ran); type 6's only left (its slot 1 ran, slots 2 and
3 never did).

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
| `sub_82250778` | `PowerGraphScreen67::OnEnter` | | (0, 1), both machines: r3 = the member. +9612 = 0; `VirtualAlloc(0, 28, 0x1000, 4)` into `[0x82815EB4]` (the page object: +0, +4, +8, +24 = 0, +12 and +20 = -1, +16 not written), `VirtualAlloc(0, 68, 0x1000, 4)` into `[0x82815EB8]` (the menu: +16 = 7; +0 to +12, +20 to +40, +48, +52 and +64 = 0; +44, +56 and +60 not written), `VirtualAlloc(0, 92, 0x1000, 4)` into `[0x82815EBC]` (the data: +0 = 0, +4 = 21; no check of any answer); +9608 = 1 when +9616 isn't 0, else 9; `[0x82815EC0]` = 0. A 112-byte frame (r30, r31 saved). No direct callers. Seen in play. Read in full. |
| `sub_82250D70` | `PowerGraphScreen67::WaitForLoad` | | (2, 1): while +9612 is 0, +9612 = 1 once `UiMovies::IsLoaded(9)`; once set: the data's +0 = 1 and +9608 = 9. As `PowerGraphScreen::WaitForLoad` (`type2-screens.md`) without its clear. A 96-byte frame. No direct callers. Seen in play. Read in full. |
| `sub_82250338` | `PowerGraphScreen67::Shown` | | (4, 1) → 5: `[0x82815EC0]` = 0. Leaf. No direct callers. Seen in play. Read in full. |
| `sub_82250890` | `PowerGraphScreen67::Open` | | (9, 21) for type 6, (9, 18) for type 7 (each registered twice): with +9612 set, the page object p (`[0x82815EB4]`): p +8 = p +0 when that isn't 0, p +12 = 0 (step 0, the open animation), p +4 = 0 (page 0 after it), p +16 = -1, p +0 = -1, +9608 = 4; else +9608 = 1 (the movie loaded again; the request probably lost). Then `[0x82815EC0]` = 0. Leaf. No direct callers. Seen in play. Read in full. |
| `sub_82250A20` | `PowerGraphScreen67::Draw` | | (5, 1): when the page (`[0x82815EB4]` +0) isn't -1, an indirect call through a two-entry table it builds on its frame (r1 + 80, 8 bytes an entry: the function, a this-adjust 0): page 0 `MenuPage` `sub_82250948`, 1 `ClosedPage` `sub_82250348`, with r3 = the member (+ the adjust) and r4 unchanged (the mode). A 112-byte frame. No direct callers. Seen in play. Read in full. |
| `sub_82250948` | `PowerGraphScreen67::MenuPage` | | Page 0: r3 = the member, r4 = the mode. `Menu::Step([0x82815EB8])`; by the record's tab word (the data's +88, unsigned): below 3, buttons 158 and 159; 3 to 5, 158 and 163 (the button that opened it from `Type6Screen0`); 6 and above, none: when `Menu::Pressed(menu, button)` answers for either, `MenuSound::Play(2)`, the page object p: p +8 = p +0 when that isn't 1, p +12 = 1 (step 1, the close animation), p +16 = -1, p +4 = 1 (page 1 after it), p +0 = -1; then the member's vtable slot 2 (+8) with (r3, r4): posts 23 (type 6, `sub_82237AD8`) or 20 (type 7, `sub_82247008`) (probably). A 112-byte frame (r29 to r31 saved). No direct callers. Seen in play. Read in full. In the menu helpers' lead (`../loops/leads.md`). |
| `sub_82250348` | `PowerGraphScreen67::ClosedPage` | | Page 1 (after the close animation): the data's +4 = 9 (the state `Update` applies), then a tail call to the member's vtable slot 3 (+12) with r3 and r4 unchanged: posts 24 (type 6, `sub_82237B28`) or 21 (type 7, `sub_82247058`) (probably), which `Type6Screen0::PickBack` and `Type7Screen0::PickBack` take as the screen closed. Leaf. No direct callers. Seen in play. Read in full. |
| `sub_822513B8` | `PowerGraphScreen67::Update` | | (5, 0): r3 = the member. First the empty `sub_82106540`(100, 30, 0, `0x82011208`, a string not read). m = movie 9 (`UiMovies::Find` on `0x826C5CCC`). A two-entry step table on its frame (r1 + 80, as `Draw`'s): step 0 `Step0` `sub_82251320`, 1 `Step1` `sub_82250AA8`. With the page object p: when the step (p +12) is -1, `UiMovie::Run(m, 0)`; when it is 0 or more, the step is called with r3 = the member, r4 = p + 24, and when it answers non-zero: with no step queued (p +16 = -1), p +0 = p +4 (the next page) and p +12 = -1; else p +16 = -1, p +20 = 0, p +12 = the queued step; then p +24 = 0. A negative step other than -1 is finished at once the same way without a call. Then, when the data's +4 isn't 21: +9608 = it and the data's +4 = 21. A 144-byte frame (r29 to r31 saved). No direct callers. Seen in play. Read in full. |
| `sub_82251320` | `PowerGraphScreen67::Step0` | | The open animation: r3 = the member, r4 = the started word. m = movie 9. On the first call (the word 0): `SetTexts(r3)`, `UiMovie::Play(m, -1, 0, 0, 0, 0, 0)` (animation 0), the word = 1. Then `UiMovie::Run(m, 0)`; returns 1 once it answers 0 (nothing playing), and then `[0x82815EC0]` = 1 (the graph shown); else 0. A 128-byte frame (r29 to r31 saved). No direct callers. Seen in play. Read in full. |
| `sub_82250DD8` | `PowerGraphScreen67::SetTexts` | | r3 = the member (passed on to `SetParts`). m = movie 9; `UiMovie::SetText(m, -1, node, string, arg)` on the root's nodes: 5 to 9 and 23 to 28 the labels 258 to 268; 15 to 19 strings 446, 440, 447, 447, 447; 35 to 38 437, 439, 449, 449; 42 to 44 438, 448, 448; 10 to 14 string 8 with the record's (the data's +8) +0 × `[0x82004510]`, +4 × `[0x82003F00]` and +8, +12, +16 × `[0x82004550]` (each an integer made a float, multiplied in double, rounded to float, truncated by `fctiwz`, through its frame's +80); 20 string the record's +24 + 1934; 21, 22 and 29 strings the car id (+20) + 1646, + 1790 and + 1718; 30 the spec byte +28 + 421; 31 the class +32 + 428; 32 the flag +36 + 426; 33, 39, 40, 41 string 8 with +44, +40, +48, +56; 34 string 436 with the float +52 × `[0x82003F08]` as a double in r7 (the frame's +48). Then `UiMovie::FindNode(m, -1, 3)`: its +80 (shown) = 1 for the tab word (+80, the data's +88) 2 or 5, 0 for 0, 1, 3 and 4, unchanged above 5; when it's set, `SetParts(r3, the data + 68)` (the record's +60). The string ids not looked up. A 128-byte frame (r29 to r31 and f31 saved). From `Step0` only. Seen in play. Read in full. |
| `sub_82250CD0` | `PowerGraphScreen67::SetParts` | | r3 = the member, r4 = the record's +60 (five words from the car's +1568, written by `PowerGraph::SetFromCar` only): bytes +2 to +4 and +5 to +11 sign-extended into two word arrays on its frame (r1 + 80, r1 + 96); `SetPartIcon(r3, byte +1)`, `SetPartRow2(r3, r1 + 80)`, `SetPartRow1(r3, r1 + 96)`. Probably the car's parts as icons (not looked further). A 144-byte frame. From `SetTexts` only, for tabs 2 and 5. Not seen in play. Read in full. |
| `sub_82250B48` | `PowerGraphScreen67::SetPartIcon` (probably) | | r4 = a value: `UiMovie::FindNode(movie 9, 3, 0)` +108 = r4 (probably the node's frame). A 112-byte frame. From `SetParts` only. Not seen in play. Read in full. |
| `sub_82250BA0` | `PowerGraphScreen67::SetPartRow2` (probably) | | r4 = a word array: g = `UiMovie::FindNode(movie 9, 3, 2)`; for each of g's +104 children (from index g +100, `FindNode(m, 3, g +100 + i)`): a value other than 10: +108 = it and +80 = 1; 10: +80 = 0 (hidden). A 144-byte frame (r27 to r31 saved). From `SetParts` only. Not seen in play. Read in full. |
| `sub_82250C40` | `PowerGraphScreen67::SetPartRow1` (probably) | | r4 = a word array: g = `UiMovie::FindNode(movie 9, 3, 1)`; for 7 children (`FindNode(m, 3, g +100 + i)`): a value below 37 (signed): +108 = it and +80 = 1; else +80 = 0. A 144-byte frame (r27 to r31 saved). From `SetParts` only. Not seen in play. Read in full. |
| `sub_82250AA8` | `PowerGraphScreen67::Step1` | | The close animation: r3 not read, r4 = the started word. m = movie 9. On the first call: `UiMovie::Play(m, -1, 1, 0, 0, 0, 0)` (animation 1), the word = 1, `[0x82815EC0]` = 0 (the graph hidden). Then returns 1 once `UiMovie::Run(m, 0)` answers 0, else 0. A 112-byte frame (r30, r31 saved). No direct callers. Seen in play. Read in full. |
| `sub_82250900` | `PowerGraphScreen67::Leave` | | (5, 4), (9, 4), (10, 4) for type 6; with message 3 for type 7: +9608 = 7, `UiMovies::Delete(9)`, the member's vtable slot 1 (+4) with (r3, r4) (posts 4 for type 6, `sub_82237A88`, or 3 for type 7, `sub_82246FB8`, probably: the `(7, 4)` [3] that runs `OnLeft`), +9608 = 7 again. A 112-byte frame (r29 to r31 saved). No direct callers. Seen in play. Read in full. |
| `sub_822502C0` | `PowerGraphScreen67::OnLeft` | | (7, 4) → 6 for type 6, (7, 3) → 6 for type 7: `VirtualFree(block, 0, 0x8000)` of `[0x82815EB4]`, `[0x82815EB8]` and `[0x82815EBC]` (answers not read), then the three globals = 0. A 112-byte frame (r29 to r31 saved). No direct callers. Seen in play. Read in full. |
| `sub_82237A88` | `PowerGraphScreen6::PostLeave` (probably) | | Probably slot 1 of vtable `0x82010A68` (beside the Init `sub_82237B78`; the vtable's words not read): r4 = the mode: message (4, 0, 0) posted to its ring (+204, 50 entries of 12 bytes: the count +804 incremented, the entry (the head +808 + the count - 1) mod 50). Leaf. From `Leave`. Seen in play. Read in full. |
| `sub_82237AD8` | `PowerGraphScreen6::PostClosing` (probably) | | Probably slot 2 of vtable `0x82010A68`: message (23, 0, 0) posted, as `PostLeave`. Leaf. From `MenuPage`. Its handler not looked for. Not seen in play. Read in full. |
| `sub_82237B28` | `PowerGraphScreen6::PostClosed` (probably) | | Probably slot 3 of vtable `0x82010A68`: message (24, 0, 0) posted, as `PostLeave`: `Type6Screen0::PickBack`'s (5, 24) and this machine's (5, 24) → 8. Leaf. From `ClosedPage`. Not seen in play. Read in full. |
| `sub_82246FB8` | `PowerGraphScreen7::PostLeave` (probably) | | Probably slot 1 of vtable `0x82010FD0` (beside the Init `sub_822470A8`): message (3, 0, 0) posted, as type 6's. Leaf. From `Leave`. Seen in play. Read in full. |
| `sub_82247008` | `PowerGraphScreen7::PostClosing` (probably) | | Probably slot 2 of vtable `0x82010FD0`: message (20, 0, 0) posted, as type 6's. Leaf. From `MenuPage`. Its handler not looked for (type 7's (9, 20) is `sub_822404D8`, `type6-type7-modes.md`). Seen in play. Read in full. Slot 3 is probably `sub_82247058` (posting 21, its row in `type67-screens.md` as `Type6PanelScreen::OpenPowerGraph`). |
