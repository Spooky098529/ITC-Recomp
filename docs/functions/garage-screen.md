# Known functions: the garage screen

The type 2 front-end mode's **garage screen** (its machine at the mode
block's +0x23018, set up by `GarageScreen::InitStates` from the mode's
Init `sub_82197278`; opened by message 23, the hub's item 4; the mode
and its other machines are in [`type2-screens.md`](type2-screens.md),
[`type2-dialog.md`](type2-dialog.md), [`car-dealer.md`](car-dealer.md),
[`options-screen.md`](options-screen.md),
[`save-screen.md`](save-screen.md) and the leads in
`../loops/leads.md`): UI movie 5 (asset 22579), the list of the player's
garage cars (player 0's save data: the garage list at +6120, its count
the halfword +6328, the current car's index +0x19920) over the stand
(`GarageStage::`, [`front-end-car-draw.md`](front-end-car-draw.md),
[`car-makers.md`](car-makers.md)), with the header the hub shows too
(the career's pick at the save's +8, the money +6116, the day +6100
and its weekday). Picking a car opens its menu of five actions: make it
the current car, sell it (`Garage::Sell`, [`career.md`](career.md); the
money raised by a price worked out from the car), change its license
plate (the plate editor, `PlateEditor::` in `type2-screens.md`), look
at it on the stand (turned with buttons 173 and 174, two camera
settings with 171 and 172, and a menu of the stand's animations: the
turn and the doors), and its spec sheet (the power graph screen,
`PowerGraphScreen::` in `type2-screens.md`). The questions go through
the mode's dialog (messages 34 and 36, replies 38, 39, 41 and 42).

It has the shape the type 2 mode's other machines have
(`type2-screens.md`'s intro; keys (state, message, 0, 0), r3 the
machine and r4 the mode, whose message ring at +204 the handlers post
to): (0, 1) `Create` makes the blocks and goes to 1 (the mode set +9612
= 1) or 9; (1, 1) `OpenMovie` → 2; (2, 1) `WaitForLoad` → 9 (or 4); (9,
23) `ScreenMachine::Reenter` → 4; (4, 1) `Start` → 5; in 5, Draw
(message 1) runs `UpdatePage`'s page and Update (message 0) `Update`'s
step (the transitions) and the stand; page 4 posts 22, and (5, 22) →
8 `Unload`, (8, 3) `Screen::LeaveToHub` (message 6 to the hub) → 9; (5,
4) and (9, 4) `Close` → 7, (7, 4) `Destroy` → 6. The replies: (5, 26)
the spec sheet closed (`PowerGraphScreen::Page1`); (5, 29) and (5, 30)
the plate editor done or cancelled, (5, 31) the plate editor left
(`PlateEditor::LeavePage`); (5, 38), (5, 39), (5, 41) and (5, 42) the
dialog's yes, no, back and closed. It posts 24 (the spec sheet), 28
(the plate editor), 32 (the plate editor's close,
`PlateEditor::OnMessage32`), 34 and 36 (the dialog's questions 0 and 2,
`Type2Dialog::OpenConfirm` and `OpenConfirm2`).

Its blocks are globals (`../known-data.md`), each `VirtualAlloc`ed by
`Create`: `[0x826D8420]` the page object (28 bytes, laid out as the
other machines': +0 the page `UpdatePage` runs, -1 while a transition
runs; +4 the page after it; +8 the page before; +12 the running
transition, -1 none; +16 the queued one; +20; +24 the transition's
started word), `[0x826D8424]` and `[0x826D8428]` two menu lists (72
bytes each, `../known-data.md`'s "menu list"; the first is the car list
on page 0, the five actions on page 1 and the stand's options on page 3,
the second the car list kept beside the actions, moved by actions 161
and 162 on page 1) and `[0x826D842C]` the screen's data (104 bytes).
`Destroy` frees the page object, the first list and the data, not the
second list (nor either list's flags block). The pages (`UpdatePage`'s
table): 0 `CarListPage`, 1 `ActionsPage`, 2 `ViewPage`, 3
`ViewOptionsPage`, 4 `LeavePage`; the transitions (`Update`'s table): 0
the opening, 1 the list's highlight, 2 the list moved, 3 the closing, 4
into the car's menu, 5 the action's highlight, 6 the action moved, 7
another car (the second list moved), 8 back to the list, 9 into the
view, 10 back to the view, 11 the view back to the menu, 12 into the
view's options, 13 the option's highlight, 14 the option moved, 15 the
options back to the menu, 16 to the plate editor, 17 back from it.

In play (`../coverage.md`) the screen opened, showed its list, and
closed without a car picked: its machine, `Start`, `CarListPage`,
`LeavePage`, `Update`, transitions 0, 1 and 3 and the list's set-up
ran; the car's menu, the sale, the plate, the view and the spec sheet
never did (56 of the 78 rows not seen in play).

Names are ours ("garage screen", the pages' and actions' names are
guesses from what the code does: no text was read). Read 2026-11-09 by
the B7 loop's mapping agent: the Init through
`itc_recomp/tools/init_step_tables.py` (20 transitions), the handlers,
the pages in play, `Update` and the helpers in play in full, the rest
read for their calls, their block writes and their branches ("Read in
outline"). Everything seen in play is ready (`b7_groups.py`: 15 groups,
their roots the Init, the 9 handlers seen, `UpdatePage`, `Update`,
`CarListPage`, `LeavePage` and transitions 0, 1 and 3); `UpdatePage`'s
and `Update`'s tables are indirect calls the tool doesn't count, so
the pages and transitions go before them. Shared, with their rows
elsewhere: `StateMachine::Add` `sub_82106158`
([`game-modes.md`](game-modes.md)), `Screen::LeaveToHub` `sub_821D6A20`
([`type2-screens.md`](type2-screens.md)), `ScreenMachine::Reenter`
`sub_822895E0` ([`location-screen.md`](location-screen.md)), the empty
`sub_82106540` ([`audio.md`](audio.md)), the UI movie calls
(`UiMovies::Open`, `Find`, `IsLoaded`, `Delete`, `UiMovie::ResetPose`,
`PlayNamed`, `LoopNamed`, `FinishNamed`, `FinishAll`, `Run`, `FindNode`,
`SetText`, `SetNodeText`, `SetNumberText`, `SetNodeNumberText`,
`SetChildNameText`, `SetNodeColor`, `ScrollText`, `sub_8211C5F8`,
`sub_8211A630`; [`ui-movies-game.md`](ui-movies-game.md)) and
`UiMovie::Child` `sub_8259D658` ([`ui-movies.md`](ui-movies.md)), the
menu lists' `MenuList::Init` ([`save-data.md`](save-data.md)) and
`Update`, `Accepted`, `Cancelled`, `Selection`, `GetPreviousSelection`,
`SetSelection`, `Fit` ([`race-screens.md`](race-screens.md)),
`MenuSound::Play` ([`menu-sounds.md`](menu-sounds.md)),
`ScreenFades::FadeIn` and `FadeOut` ([`mode-tick.md`](mode-tick.md)),
`SaveData::Of` and `sub_820C4550`
([`vehicle-accessors.md`](vehicle-accessors.md)), `Garage::Sell`,
`Stats::AddPoints` and `Career::PickNext` ([`career.md`](career.md)),
`Car::Init` ([`race-setup.md`](race-setup.md)), `Car::Refresh` and
`Params::GetCarSpec` ([`car-specs.md`](car-specs.md)),
`Vehicle::SetupFromCar` and `CarData::CarData`
([`car-physics.md`](car-physics.md)), `PowerCurve::FromEngine` and
`ShiftPeak` ([`vehicle.md`](vehicle.md)), `GarageStage::IsBusy`,
`Draw` and `Command` ([`front-end-car-draw.md`](front-end-car-draw.md)),
`CarModels::BlockOf`, `GarageStage::ReloadModel` and `SetCar`
([`car-makers.md`](car-makers.md)), `memcpy` and `__chkstk`
([`c-runtime.md`](c-runtime.md)), `VirtualAlloc` and `VirtualFree`
([`heap.md`](heap.md)). `sub_821808F0` and `GarageCamera::Command`
`sub_82180358` (the stand's setters' lead in `../loops/leads.md`, with
`sub_821803A0`) are called by the view's pages only (not seen in play
from here).

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
| `sub_82196520` | `GarageScreen::InitStates` | | r3 = the garage screen's machine (the type 2 mode block's +0x23018): 20 transitions through `StateMachine::Add` (`sub_82106158`), keys (state, message, 0, 0), each a member function functor over vtable `0x82011918` (the machine, the function) copied into the frame (six-word copy loops), the next state at the copy's +24 (0 for none): (0, 1) `Create` `sub_82191C58`; (1, 1) → 2 `OpenMovie` `sub_82191618`; (2, 1) `WaitForLoad` `sub_82191D70`; (4, 1) → 5 `Start` `sub_82193DC0`; (5, 26) `OnSpecSheetClosed` `sub_82193E38`; (5, 30) `OnPlateCancelled` `sub_821928F8`; (5, 29) `OnPlateDone` `sub_82193E80`; (5, 31) `OnPlateEditorLeft` `sub_82193F18`; (5, 38) `OnYes` `sub_82192958`; (5, 39) and (5, 41) `OnNo` `sub_82192BA8`; (5, 42) `OnDialogDone` `sub_82193F80`; (5, 1) `UpdatePage` `sub_82194EA8`; (5, 0) `Update` `sub_82196230`; (5, 22) → 8 `Unload` `sub_821927E8`; (8, 3) `Screen::LeaveToHub` `sub_821D6A20`; (9, 23) `ScreenMachine::Reenter` `sub_822895E0`; (5, 4) and (9, 4) `Close` `sub_82192850`; (7, 4) → 6 `Destroy` `sub_821915B0`. A 1,424-byte frame (r27 to r31 saved). It decodes as the other Inits' step tables (`itc_recomp/tools/init_step_tables.py`, the functors' words the original never writes copied as the frame holds them). No return value. Only caller the type 2 mode's Init `sub_82197278`. Seen in play. Read in full (through the tool). |
| `sub_82191C58` | `GarageScreen::Create` | | (0, 1). r3 = the machine: +9608 = 0; `VirtualAlloc(0, size, 0x1000, 4)` for the page object (28 bytes, `[0x826D8420]`: +0, +4, +8, +24 = 0, +12 and +20 = -1; +16 not written), the two menu lists (72 bytes each, `[0x826D8424]` and `[0x826D8428]`: +60 and +68 = 0 when the allocation isn't 0) and the data (104 bytes, `[0x826D842C]`: +0, +84, +88, +92, +96 = 0, written without a check); then +9604 = 1 when +9612 is set, else 9. A 112-byte frame (r30, r31 saved). No return value. Seen in play. Read in full. |
| `sub_82191618` | `GarageScreen::OpenMovie` | | (1, 1) → 2: `UiMovies::Open(5, 22579)` (tail call). Seen in play. Read in full. |
| `sub_82191D70` | `GarageScreen::WaitForLoad` | | (2, 1): every Draw in state 2. r3 = the machine. While +9608 is 0: `UiMovies::IsLoaded(5)`, and +9608 = 1 once it answers. With +9608 set: +9604 = 9 when +9612 is set (wait for message 23), else 4. A 96-byte frame (r31 saved). No return value. Seen in play. Read in full. |
| `sub_82193DC0` | `GarageScreen::Start` | | (4, 1) → 5. `ReadSave`, `SetHeader`; the page object: +8 = the page when it isn't 0, +12 = 0 (transition 0), +16 = 1 (transition 1 queued), +4 = 0 (page 0 after), +0 = -1; `ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0)` (tail position). A 96-byte frame. No return value. Seen in play. Read in full. |
| `sub_82191BB0` | `GarageScreen::ReadSave` (probably) | | No arguments. s = player 0's save data (`SaveData::Of(0)`): the data's +4 to +23 = s +8 to +27 (the career's 20-byte pick, `Career::PickNext`); +24 = s +6116 (the money); +32 = the day s +6100 mod 7 (the weekday, signed division); +28 = the day, at most 999; +36 = `SaveData::Of(0)` again (the save data the screen reads). A 96-byte frame (r31 saved). From `Start` only. Seen in play. Read in full. |
| `sub_82192C80` | `GarageScreen::SetHeader` (probably) | | No arguments. m = movie 5 (`UiMovies::Find`); `UiMovie::ResetPose(m)`; `SetChildNameText(m, 15, 6, data + 4)` (the pick's name record); `SetNumberText(m, 15, 9, data +24)` (the money); `SetText(m, 15, 13, 8, data +28)` (string 8 with the day); node (15, 11)'s +108 = data +32 (the weekday's frame, `FindNode`); the data's +72 = 0, +76 = 0, +0 = 1. A 112-byte frame (r30, r31 saved). As the hub's header (`Type2Hub::SetUpStatus`). From `Start` only. Seen in play. Read in full. |
| `sub_82194EA8` | `GarageScreen::UpdatePage` | Overridden | (5, 1): every Draw while running. r3 = the machine, r4 = the mode. A 112-byte frame holding the page table {`CarListPage` `sub_82194078`, `ActionsPage` `sub_821941A8`, `ViewPage` `sub_82191DE0`, `ViewOptionsPage` `sub_82192030`, `LeavePage` `sub_82192C18`}: with the page (`[0x826D8420]` +0) not -1, calls page[+0] with r3 = r4 = the mode, r5 as given, r6 to r10 the table (no bounds check). Returns the page's r3 (ignored), or r3 as given with no page. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::UpdatePage` (`pages.cpp`), the page called through the guest from its table (`CarListPage` and `LeavePage` through their adapters, the others the originals); harness `garage_screen` (pages -1, 0 and 4). A functor the machine calls: the adapter stays. |
| `sub_82196230` | `GarageScreen::Update` | Overridden | (5, 0): every Update while running. r3 = the machine, r4 = the mode. A 272-byte frame (`__savegprlr_20`). First the empty `sub_82106540(100, 30, 0, 0x8200A3C8)`. m = movie 5 (`UiMovies::Find`). The transition table in its frame (18 entries: `Transition0` `sub_82195BA0` to `Transition17` `sub_82196170`, in this file's order). With no transition (+12 = -1): `UiMovie::Run(m, 0)` unless the page is 4. Else calls table[+12] (r3 = the page object's +24, the started word; no bounds check); when it returns nonzero (or the index is another negative value): with nothing queued (+16 = -1) the page becomes +4 and +12 = -1; else +12 = the queued one, +16 = -1, +20 = 0; then +24 = 0. Then the stand, while the data's +72 (the stand shown) is set: the busy sign (`GarageStage::IsBusy` on `[0x826D83E0]`'s holder: busy and +84 0, `ShowBusy` and +84 = 1; not busy and +84 set, `HideBusy` and +84 = 0); then with +76 0, the selected car's model (the garage car at +44 + +48, its +12 the id: `CarModels::BlockOf`, `GarageStage::ReloadModel`) into +76, and when that's nonzero `PutCarOnStand` and `GarageStage::Command(holder, 15)` (preset 1); with +76 set, `GarageStage::Draw(holder, 0, 0)`. Without +72: +84 set, `HideBusyNow` and +84 = 0. No return value. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::Update` (`src/garage_screen/update.cpp`): the transition called through the guest from its table (transitions 0, 1 and 3 through their adapters, the others the originals, never seen in play), the stand's calls through their overrides, `PutCarOnStand`, `ShowBusy`, `HideBusy` and `HideBusyNow` (never seen in play) the originals; returns the last call's r3 (or the movie); harness `garage_screen` (the drawless stand). A functor the machine calls: the adapter stays. |
| `sub_82194078` | `GarageScreen::CarListPage` | Overridden | Page 0, r3 = the mode. A 96-byte frame (r31 saved). `MenuList::Update([0x826D8424])`; when the list didn't move (+24 0 and +4 = +8): on accept (`MenuList::Accepted`): the data's +44 = the selection (`MenuList::Selection`), +48 = the list's scroll +4, `ReloadStandCar`, the page object's +8 = the page unless 1 (a quirk: it is 0 here), +12 = 4 (into the car's menu), +16 = 5, +0 = -1, +4 = 1, `MenuSound::Play(0)`; else on cancel (`MenuList::Cancelled`): +8 = the page unless 4, +12 = 3 (the closing), +16 = -1, +0 = -1, +4 = 4, `MenuSound::Play(2)`. When it moved: +8 = the page unless 0, +12 = 2, +16 = -1, +0 = -1, +4 = 0, `MenuSound::Play(4)`. No return value. Seen in play (the accept path never: `ReloadStandCar` didn't run). Read in full. Native (B7 loop, 2026-11-09): `garage_screen::CarListPage` (`src/garage_screen/pages.cpp`), `MenuList::Update`, `Accepted`, `Cancelled` and `MenuSound::Play` through their overrides, `MenuList::Selection`'s plain C++, `ReloadStandCar` (never seen in play) the original; returns `MenuSound::Play`'s r3 (or `Cancelled`'s 0); harness `garage_screen` (over a stack of its own: `ReloadStandCar`'s 100,560-byte frame). From `UpdatePage`'s table: the adapter stays until the remove pass confirms. |
| `sub_821941A8` | `GarageScreen::ActionsPage` | | Page 1, r3 = the mode. A 144-byte frame. `MenuList::Update` on both lists. When the first (the actions) moved: transition 6, page 1, `MenuSound::Play(4)`. When the second (the car list) moved: transitions 7 then 5, page 1, sound 4. Else on accept, by the action (the first list's selection): 0, make it the current car: when the car (+44 + +48) is the current one (save +0x19920) only `MenuSound::Play(3)`; else `SetUseQuestion(the garage car)`, message 34 (the dialog's yes/no), the data's +68 = 1, `SetPrompts(0, 0, -1, -1, -1, -1)`, sound 0; 1, sell it: the same check, else `SetSaleQuestion`, message 36, +68 = 2, `SetPrompts(0, 0, 1, 1, -1, -1)`, sound 0; 2, the plate: `SetUpPlateEditor(the garage car)`, message 28 (the plate editor), transition 16, page -1 after, `SetPrompts(0, 0, 1, 1, -1, -1)`, sound 0; 3, look at it: transition 9, page 2, sound 0; 4, the spec sheet: `SetUpSpecSheet(the garage car)`, message 24, page -1, `SetPrompts(0, 1, -1, -1, -1, -1)`, sound 0. On cancel: transition 8, page 0, sound 2. The page object's +8 = the page unless it's already the target. Messages posted straight into the mode's ring. No return value. Not seen in play. Read in full. |
| `sub_82191DE0` | `GarageScreen::ViewPage` | | Page 2, r3 = the mode. A 112-byte frame. Reads its controller's action mapper (`0x826BAB80` + 8 × `[0x826BABA4]`, unless its +4 is 1) through its vtable slot 12 (+48, probably a press check) for actions in turn: 158 (back) transition 11, page 1, `MenuSound::Play(2)`; 157: transitions 12 then 13, page 3 (the options), sound 0; then without a branch between them: 4 and 5 through `sub_821808F0(holder, 4 or 5)`, 173 `TurnLeft`, 174 `TurnRight`, 171 `CameraA`, 172 `CameraB` (each by its own press). No return value. Not seen in play. Read in outline. |
| `sub_82192030` | `GarageScreen::ViewOptionsPage` | | Page 3, r3 = the mode. A 112-byte frame. `MenuList::Update([0x826D8424])` (the options, four items); when it moved: transition 14, page 3, sound 4. Then the mapper's presses: 159 (accept) by the option: 0 `ToggleTurn`, 1 `ToggleDoors`, 2 `ToggleDoors2`, 3 back to the view (transition 10, page 2), each with `MenuSound::Play(0)`; 157 back to the view (transition 10, page 2, sound 0); then as `ViewPage` 4, 5, 173, 174, 171, 172. No return value. Not seen in play. Read in outline. |
| `sub_82192C18` | `GarageScreen::LeavePage` | Overridden | Page 4, r3 = the mode: the stage's (`[[0x826D83E0]]`) +6052 = 1; message (22, 0, 0) posted straight into the mode's ring (the machine goes to 8, `Unload`). Leaf (r3 left as given). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::LeavePage` (`pages.cpp`); harness `garage_screen`. From `UpdatePage`'s table. |
| `sub_82193E38` | `GarageScreen::OnSpecSheetClosed` | | (5, 26) (`PowerGraphScreen::Page1` posts 26): the page object to page 1 (+8 = the page unless 1, +12 = -1, +16 = -1, +0 = 1), then `SetPrompts(0, 0, 1, 1, -1, -1)` (tail call). Not seen in play. Read in full. |
| `sub_821928F8` | `GarageScreen::OnPlateCancelled` | | (5, 30) (the plate editor cancelled): the data's +60 = 0; message (32, 0, 0) to the mode's ring (the plate editor's close). r4 = the mode. Leaf. Not seen in play. Read in full. |
| `sub_82193E80` | `GarageScreen::OnPlateDone` | | (5, 29) (the plate editor done): the data's +60 = 1 (a question pending), `SetPlateQuestion`, message (34, 0, 0) (the dialog's yes/no), the data's +68 = 0 (the question is the plate's). A 112-byte frame. Not seen in play. Read in full. |
| `sub_82193F18` | `GarageScreen::OnPlateEditorLeft` | | (5, 31) (`PlateEditor::LeavePage`): unless the data's +60 is set (a plate question pending): the page object's +8 = the page unless 1, +4 = 1, +16 = -1, +12 = 17 (back from the plate editor), +0 = -1, and `SetPrompts(0, 0, 1, 1, -1, -1)` (tail call). Not seen in play. Read in full. |
| `sub_82192958` | `GarageScreen::OnYes` | | (5, 38) (the dialog's yes), r4 = the mode. A 128-byte frame. The data's +64 = 1. By the question (+68): 0, the plate: message (32, 0, 0) (the plate editor's close), then the plate editor's plate (`[0x826D8438]` +4, 20 bytes) copied into the garage car's +1780 (the car +44 + +48 through the garage list); 1, the current car: save +0x19920 = the car's index, `Career::PickNext(SaveData::Of(0), 1)`, the data's +4 to +23 = the save's +8 to +27 again and `SetChildNameText(m, 15, 6, data + 4)`; 2, the sale: `Garage::Sell(the save, the index)`, the money (save +6116) += the price (+100), at most 9,999,999, `Stats::AddPoints(save + 48, the price)`, the data's +24 = the money and `SetNumberText(m, 15, 9, it)`; when the sold car was the last (the count before less 1 is the index), +44 -= 1. Other values nothing. No return value. Not seen in play. Read in full. Was in the career screens' lead (`Garage::Sell`). |
| `sub_82192BA8` | `GarageScreen::OnNo` | | (5, 39) and (5, 41) (the dialog's no and back), r4 = the mode: the data's +64 = 0; with +68 0 (the plate's question) message (32, 0, 0) (the plate editor's close). Leaf. Not seen in play. Read in full. |
| `sub_82193F80` | `GarageScreen::OnDialogDone` | | (5, 42) (the dialog closed). A 96-byte frame (r31 saved). With the data's +64 set (answered yes): transition 8 (back to the list, made again), page 0, +8 = the page unless 0. Else `SetPrompts(0, 0, 1, 1, -1, -1)` and by +68: 0, transition 17, page 1; 1 or 2, page 1 at once (+12 and +16 = -1, +0 = 1); others nothing. No return value. Not seen in play. Read in full. |
| `sub_821927E8` | `GarageScreen::Unload` | | (5, 22) → 8. r3 = the machine. A 112-byte frame (r31 saved). `UiMovie::FinishAll(movie 5)`; with +9616 set, `UiMovies::Delete(5)` and +9608 = 0. No return value. Seen in play. Read in full. |
| `sub_82192850` | `GarageScreen::Close` | | (5, 4) and (9, 4). r3 = the machine, r4 = the mode. +9604 = 7; `UiMovie::FinishAll(movie 5)`, `UiMovies::Delete(5)`; message (4, 0, 0) to the mode's ring. A 112-byte frame (r31 saved). No return value. Seen in play. Read in full. |
| `sub_821915B0` | `GarageScreen::Destroy` | | (7, 4) → 6. `VirtualFree(p, 0, 0x8000)` for `[0x826D8420]`, `[0x826D8424]` and `[0x826D842C]`, then the three set to 0; the second list `[0x826D8428]` isn't freed (nor the lists' flags blocks): probably a leak, made again by each `Create`. A 112-byte frame (r29 to r31 saved). No return value. Seen in play. Read in full. |
| `sub_82195BA0` | `GarageScreen::Transition0` | Overridden | The opening (from `Start`). r3 = the started word. A 112-byte frame (r30, r31 saved). m = movie 5. The first call: `PlayNamed(m, 131)`, `PlayNamed` and `LoopNamed` for 128, 125, 126, 45 and 49 (each `PlayNamed(m, n, 0, 0, 0, 0)`), the data's +44 = +48 = 0, `SetUpLists`, the word = 1. Then `UiMovie::Run(m, 0)`; returns 1 when it returns 0. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::Transition0` (`src/garage_screen/transitions.cpp`), the movie's calls through their overrides, `SetUpLists`' plain C++; harness `garage_screen`. Called from `Update`'s table: the adapter stays until `Update` is native. |
| `sub_82192D60` | `GarageScreen::Transition1` | Overridden | The list's highlight. A 128-byte frame. The first call: `PlayNamed` and `LoopNamed` for 67 + the first list's selection and for 77, the word = 1. Then `Run(m, 0)`; returns 1 (Run's answer unused). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::Transition1` (`transitions.cpp`), `MenuList::Selection`'s plain C++; harness `garage_screen`. From `Update`'s table: the adapter stays. |
| `sub_82194F30` | `GarageScreen::Transition2` | | The list moved. A 176-byte frame. The first call: the old row's highlight (67 + the previous selection) finished and the new one played and looped; with the scroll changed, `FillRows`, the scroll's animations (72, 73, 75, 76 finished, then 73 or 72 with 75 played by the direction, each row redrawn with `SetCarRow`), node (6, 0)'s +16 (the current car's marker) moved, and `SetScrollArrows`; the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82192E20` | `GarageScreen::Transition3` | Overridden | The closing (the list's cancel). A 112-byte frame. The first call: `FinishNamed` 67 + the selection, 77 and 128; `ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0)`; the word = 1. Then `Run(m, 0)` (its answer unused); returns 1 when the fades' counts `[0x826A50DC]` and `[0x826A50E4]` are both 0 (then page 4). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::Transition3` (`transitions.cpp`); harness `garage_screen`. From `Update`'s table: the adapter stays. |
| `sub_82195D10` | `GarageScreen::Transition4` | | Into the car's menu (the list's accept). A 128-byte frame. The first call: `FinishNamed` 67 + the selection and 77, `PlayNamed(m, 132)`, the data's +52 = 0 and +56 = 0, `SetUpActions`, the word = 1. When `Run(m, 0)` returns 0: node (7, 5)'s and (7, 6)'s colors (`SetNodeColor` 2 or 0 by whether the car is the current one). Returns 1 then. Not seen in play. Read in outline. |
| `sub_82192EF8` | `GarageScreen::Transition5` | | The action's highlight. A 128-byte frame. The first call: `PlayNamed` and `LoopNamed` 80 + the action (skipped for actions 0 and 1 when the car is the current one), the data's +72 = 1 (the stand shown), the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82194680` | `GarageScreen::Transition6` | | The action moved. A 224-byte frame. The first call: the old highlight (80 + the previous action) finished, the new one played (none for actions 0 and 1 on the current car; 104 and 105 played and finished around it), 90 and 91 finished and `SetActionMarker`; the help line by the action from two five-entry tables in its frame (585, 586, 561, 553, 563 for the current car, else 559, 560, 561, 553, 563) through `SetHelpLine`; the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_821951A8` | `GarageScreen::Transition7` | | Another car (the second list moved on page 1). A 2,192-byte frame. The first call: the data's +44 = the second list's selection, +48 = its scroll; the highlight and 104, 105 played and finished; `ReloadStandCar`; the car copied (1,968 bytes, `memcpy`) and `ShowCarSpecs` on it; node (-1, 5) shown when it's the current car; node (7, 5) and (7, 6) colors; the help line as `Transition6`'s; the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82195E30` | `GarageScreen::Transition8` | | Back to the list. A 128-byte frame. The first call: the action's highlight finished, `PlayNamed(m, 133)`, the data's +72 = 0, `SetUpLists`, the word = 1. When `Run(m, 0)` returns 0: transition 1 queued as the running one (+12 = 1, +4 = 0, +16 = -1, +0 = -1, +24 = 0). Returns 1 then. Not seen in play. Read in outline. |
| `sub_82195410` | `GarageScreen::Transition9` | | Into the view. A 112-byte frame. The first call: the action's highlight finished, `PlayNamed` 134, 135, 136 (with 1), 137, 141 (with 1), `SetUpView`, the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82195530` | `GarageScreen::Transition10` | | Back to the view from its options. A 112-byte frame. The first call: the option's highlight (2 + the selection) finished, `PlayNamed(m, 144)`, `SetUpView`, the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82195F10` | `GarageScreen::Transition11` | | The view back to the car's menu. A 128-byte frame. The first call: `PlayNamed` 142, 138, 139, 140 (the last three with 1), the data's +52 = 3, `ResetStand`, `SetUpActions`, the word = 1. When `Run(m, 0)` returns 0: transition 5 then page 1 (+12 = 5, +4 = 1, +16 = -1, +0 = -1, +24 = 0). Not seen in play. Read in outline. |
| `sub_821955E0` | `GarageScreen::Transition12` | | Into the view's options. A 128-byte frame. The first call: `PlayNamed(m, 143)`, `SetUpOptions`, the word = 1. When `Run(m, 0)` returns 0: node (2, 1)'s children 6 and 7 colored 2 (`sub_8211A630`). Not seen in play. Read in outline. |
| `sub_82192FD0` | `GarageScreen::Transition13` | | The option's highlight. A 128-byte frame. The first call: 26 played and finished, 27 played (1, 1) and finished, then `PlayNamed` and `LoopNamed` 2 + the option; the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_821948B0` | `GarageScreen::Transition14` | | The option moved. A 2,160-byte frame. The first call: the old highlight finished, 26 and 27 as `Transition13`, the new one played and looped, 12 and 13 finished, `SetOptionMarker`, `PlayNamed(m, 12)`; the help line by the option from two four-entry tables (554, 555, 556, 557, or 554, 590, 589, 557 by the car's spec: the car copied with `CopyGarageCar`, `Params::GetCarSpec` on its +12); the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82196030` | `GarageScreen::Transition15` | | The options back to the car's menu. A 128-byte frame. As `Transition11` with 145 first (the option's highlight finished): 138, 139, 140, the data's +52 = 3, `ResetStand`, `SetUpActions`; when done, transition 5 then page 1. Not seen in play. Read in outline. |
| `sub_821930D8` | `GarageScreen::Transition16` | | To the plate editor. A 128-byte frame. The first call: `PlayNamed(m, 146)`, nodes (3, 1) +108 = 1 and (3, 2) +108 = 3, the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_82196170` | `GarageScreen::Transition17` | | Back from the plate editor. A 128-byte frame. The first call: `PlayNamed(m, 147)`, nodes (3, 1) +108 = 3 and (3, 2) +108 = 1, the data's +52 = 2, `SetUpActions`, the word = 1. Returns 1 when `Run(m, 0)` returns 0. Not seen in play. Read in outline. |
| `sub_821956C0` | `GarageScreen::SetUpLists` | Overridden | No arguments. A 176-byte frame (`__savegprlr_25`). m = movie 5; n = the garage's count (save +6328). `MenuList::Init` on the first list (n items, min(n, 4) shown, wrapping, actions 154 down, 153 up, 155, 156, 159 accept, 158 back) and the second (the same, actions 162, 161, 153, 154); the first list's selection = the data's +44 (`SetSelection`), its scroll +4 = +48, `MenuList::Fit(list, 1)`. Rows 0 to 3 (nodes (6, 5) to (6, 8)) shown (+80) when below n; node (6, 1) shown when n is above 4, with `SetScrollArrows(scroll, n)`; `FillRows`; node (6, 0)'s +16 (y) = (the current car's index save +0x19920 − the scroll) × 84 + 232 as a float (the current car's marker); `SetPrompts(0, 0, 1, 1, -1, -1)`; `SetHelpLine(558)`. No return value. From `Transition0` and `Transition8`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::SetUpLists` (`helpers.cpp`): `MenuList::Init`, `SetSelection` and `Fit` through their overrides, `SetScrollArrows` (never seen in play) the original, `FillRows`, `SetPrompts` and `SetHelpLine` their plain C++; the marker's 64-bit integer left at its frame's +96 as the original; harness `garage_screen` (the lists' flag blocks compared and freed). `Transition8` (never seen in play) calls it recompiled: the adapter stays. |
| `sub_82194CD8` | `GarageScreen::FillRows` | Overridden | No arguments. A 144-byte frame. m = movie 5, n = the garage's count. For row i from 0 to 3, while i < n: node (6, 5 + i) and the garage car at the list's scroll + i (through the garage list at save +6120: its head index the halfword +204, each node's +0 the next and +2 the slot, the car at +212 + 1,968 × slot), `SetCarRow(node, the car)`. No return value. From `SetUpLists` and `Transition2`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::FillRows` (`helpers.cpp`), `SetCarRow`'s plain C++; harness `garage_screen` (the rows walked among the linked cars, the count negative now and then). `Transition2` (never seen in play) calls it recompiled: the adapter stays. |
| `sub_82193180` | `GarageScreen::SetCarRow` | Overridden | r3 = a row node, r4 = a garage car. A 128-byte frame. id = the car's +12. Child 3's +108 = the maker's logo frame: id when id < 35 (35 for 2), 35 for 43, id − 11 for 47 to 56, 44 for 71, else -1; child 5's text = string 1934 + the spec's byte +769 (`Params::GetCarSpec(id)`; probably the maker's name), child 6's = 1646 + id, child 7's = 1718 + id (`SetNodeText`; probably the car's names). No return value. From `FillRows` and `Transition2`. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::SetCarRow` (`src/garage_screen/helpers.cpp`), `UiMovie::Child` and `Params::GetCarSpec` their plain C++, `SetNodeText` through its override (its unset varargs, r6 on, passed 0: the user's 2026-10-30 call); harness `garage_screen`. Its callers native too (`FillRows`), but `Transition2` and `Transition7`'s (never seen in play) recompiled: the adapter stays. |
| `sub_821939A0` | `GarageScreen::SetPrompts` | Overridden | r3 to r8 = three pairs (a0, b0, a1, b1, a2, b2), stored in its frame: for prompt k from 0 to 2, node (16, k) n and its children (16, n +100) and (16, n +100 + 1): with a_k and b_k both not -1, n +80 = 1 (shown) and the children's +108 = a_k and b_k (probably the button's and the label's frames); else n +80 = 0 (the children untouched). Returns the last child found (r3). A 176-byte frame. No return value. As `CarDealer::SetPrompts`. 6 callers (`SetUpLists`, the reply handlers, `ActionsPage`, `SetUpActions`). Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::SetPrompts` (`helpers.cpp`), `UiMovie::FindNode`'s plain C++; harness `garage_screen`. Callers never seen in play (the reply handlers, `ActionsPage`, `SetUpActions`) call it recompiled: the adapter stays. |
| `sub_82193A90` | `GarageScreen::SetHelpLine` | Overridden | r3 = a string id: `SetText(m, 17, 0, r3)` (its varargs r7 to r10 not set by the call: register leftovers), then `UiMovie::ScrollText(m, 17, 0, 1)` and `sub_8211C5F8(m, 17, 0)`; returns its r3. A 112-byte frame (r31 saved). As `HelpLine::Set` (`car-dealer.md`). 7 callers. Seen in play. Read in full. Native (B7 loop, 2026-11-09): `garage_screen::SetHelpLine` (`helpers.cpp`), the three calls through their overrides, `SetText`'s unset varargs passed 0 (the user's 2026-10-30 call, as `HelpLine::Set`'s); harness `garage_screen` (SetText's parameter area left out). Callers never seen in play call it recompiled: the adapter stays. |
| `sub_82193288` | `GarageScreen::SetScrollArrows` | | r3 = the list's scroll, r4 = the count: node (6, 3) +80 (up) = scroll > 0, node (6, 4) +80 (down) = scroll + 4 < count (each 0 or 1). A 128-byte frame. From `SetUpLists` and `Transition2`. Not seen in play. Read in outline. |
| `sub_82193B10` | `GarageScreen::ReloadStandCar` | | No arguments. A 100,560-byte frame (`__chkstk`): the garage list (save +6120, 66,868 bytes) copied to its frame, the selected car (+44 + +48) copied out of it (`CopyGarageCar`), and the stand's model reloaded for its id (`CarModels::BlockOf`, `GarageStage::ReloadModel([0x826D83E0]'s holder, block)`) into the data's +76. No return value. From `CarListPage` and `Transition7`. Not seen in play. Read in outline. |
| `sub_82192770` | `GarageScreen::CopyGarageCar` | | r3 = out (1,968 bytes), r4 = a garage list (save +6120 or a copy), r5 = an index: the list walked from its head (halfword +204) r5 times through each node's halfword +0, then the slot (node +2) and `memcpy(r3, r4 + 212 + 1,968 × slot, 1,968)`. Returns r3. A 96-byte frame. 5 callers: `ReloadStandCar`, `PutCarOnStand`, `Transition14`, `SetUpOptions` and the car list's `PanelSetBase::UpdateStand` (`car-list.md`, native; its garage tab's path, which calls this original). Not seen in play. Read in full. |
| `sub_82193BA8` | `GarageScreen::PutCarOnStand` | | No arguments (from `Update`, once the model is loaded). A large frame (`__chkstk`): the garage list copied, the selected car copied out (`CopyGarageCar`), a car object in its frame (`Car::Init`, the garage car's first 1,884 bytes copied over it, `Car::Refresh`), then `GarageStage::SetCar([0x826D83E0]'s holder, the car, -1)`. No return value. Not seen in play. Read in outline. |
| `sub_82193C58` | `GarageScreen::ShowBusy` | | `PlayNamed(m, 122)`, `PlayNamed(m, 120)` and `LoopNamed(m, 120)` (movie 5). A 112-byte frame. From `Update`. Not seen in play. Read in full. |
| `sub_82193CE0` | `GarageScreen::HideBusy` | | `FinishNamed(m, 120)`, `PlayNamed(m, 123)`. A 112-byte frame. From `Update`. Not seen in play. Read in full. |
| `sub_82193D48` | `GarageScreen::HideBusyNow` | | `FinishNamed(m, 120)`, `PlayNamed(m, 123)`, `FinishNamed(m, 123)`. A 112-byte frame. From `Update` (the stand not shown). Not seen in play. Read in full. |
| `sub_821958D0` | `GarageScreen::SetUpActions` | | No arguments. A 2,192-byte frame. `MenuList::Init` on the first list as the five actions (5 items, wrapping, 154, 153 and the others, 159, 158); the second list's selection = the data's +44, its scroll = +48, `Fit`; the first list's selection = +52 (`SetSelection`); `SetActionMarker`; the actions' texts (`SetText(m, 7, 5 + i, ..)` from a table in its frame: 134, 135, 136, 137, 112); the car copied (1,968 bytes) and `ShowCarSpecs`; `SetPrompts(0, 0, 1, 1, -1, -1)`; node (-1, 5) shown when it's the current car; the stage's +6052 = 1; the help line as `Transition6`'s; nodes (-1, 0), (-1, 13), (-1, 14) shown or hidden by a word of its own. No return value. From transitions 4, 11, 15 and 17. Not seen in play. Read in outline. |
| `sub_821936B0` | `GarageScreen::SetActionMarker` | | r3 = the action: node (7, 0)'s +16 (a float) set from it (converted through its frame). A 112-byte frame. From `Transition6` and `SetUpActions`. Not seen in play. Read in outline. |
| `sub_82194D90` | `GarageScreen::ShowCarSpecs` | | r3 = a garage car (a copy in the caller's frame). A 6,976-byte frame. A car object in its frame (`Car::Init`, the garage car's first 1,884 bytes copied over it, `Car::Refresh`, `sub_820C4550`), a car data (`CarData::CarData`, `Vehicle::SetupFromCar`), its power curve (`PowerCurve::FromEngine`, `ShiftPeak(.., 0)`); then `ShowSpecTexts` with the figures in its frame and `ShowCarNames(id)`. No return value. From `Transition7` and `SetUpActions`. Not seen in play. Read in outline. |
| `sub_82193798` | `GarageScreen::ShowSpecTexts` | | r3 = a record in its caller's frame (the figures): ten `SetText` calls into movie 5's text nodes. A 112-byte frame. From `ShowCarSpecs`. Not seen in play. Not read in detail. |
| `sub_82193910` | `GarageScreen::ShowCarNames` | | r3 = a car id: `SetText(m, -1, 19, 1934 + the spec's byte +769)`, `SetText(m, -1, 20, 1646 + id)`, `SetText(m, -1, 21, 1718 + id)` (as `SetCarRow`'s). A 112-byte frame. From `ShowCarSpecs`. Not seen in play. Read in outline. |
| `sub_82191B20` | `GarageScreen::ResetStand` | | `GarageStage::Command(holder, 1)` (cancel the turns), the data's +88 = 0, `Command(holder, 15)` (preset 1); with +92 set, +92 = 0 and `Command(holder, 3)` (close the doors); with +96 set, +96 = 0 and `Command(holder, 5)`. A 112-byte frame. From transitions 11 and 15. Not seen in play. Read in outline. |
| `sub_82194AC8` | `GarageScreen::SetUpView` | | `GarageStage::Command(holder, 58)` (preset 44), the stage's +6052 = 0, `SetHelpLine(553)`. A 96-byte frame. From transitions 9 and 10. Not seen in play. Read in full. |
| `sub_82194B20` | `GarageScreen::SetUpOptions` | | No arguments. A 2,160-byte frame. `MenuList::Init` on the first list as the view's four options (4 items, wrapping, 154, 153, 159, 158); `SetOptionMarker`; the options' texts (node (2, 1)'s children 5 to 8: 131, 132, 133, 115); the help line by the option (as `Transition14`'s tables, the car copied and its spec read); `SetText(m, 18, 5, 421 + ..)` and `SetText(m, 18, 6, 426 + ..)` (probably the options' states); the data's +80 = 0. No return value. From `Transition12`. Not seen in play. Read in outline. |
| `sub_82193720` | `GarageScreen::SetOptionMarker` | | r3 = the option: node (2, 1)'s child 0 moved by it (a float through its frame). A 112-byte frame. From `Transition14` and `SetUpOptions`. Not seen in play. Read in outline. |
| `sub_821918D0` | `GarageScreen::ToggleTurn` | | The data's +88 (turning): set, then 0 and `GarageStage::Command(holder, 1)` (cancel the turns); else 1 and `Command(holder, 0)` (turn). From `ViewOptionsPage`. Not seen in play. Read in full. |
| `sub_82191920` | `GarageScreen::ToggleDoors` | | The data's +92: set, then 0 and `Command(holder, 3)`; else 1 and `Command(holder, 2)` (the doors open and close). From `ViewOptionsPage`. Not seen in play. Read in full. |
| `sub_82191970` | `GarageScreen::ToggleDoors2` | | The data's +96: set, then 0 and `Command(holder, 5)`; else 1 and `Command(holder, 4)` (the stand's second pair of door animations; what they open not looked for). From `ViewOptionsPage`. Not seen in play. Read in full. |
| `sub_821919C0` | `GarageScreen::TurnLeft` (probably) | | r3 = a mapper entry, r4 = 173: on its press, the data's +88 = 0, `Command(holder, 1)`, `Command(holder, 9)` (`GarageStage::StepAngles`). A 96-byte frame. From the view's pages. Not seen in play. Read in outline. |
| `sub_82191A18` | `GarageScreen::TurnRight` (probably) | | As `TurnLeft` with 174 and `Command(holder, 10)`. From the view's pages. Not seen in play. Read in outline. |
| `sub_82191A70` | `GarageScreen::CameraA` (probably) | | As `TurnLeft` with 171: the data's +88 = 0, `Command(holder, 1)`, `sub_821808F0(holder, 0)` (the garage camera, a lead). From the view's pages. Not seen in play. Read in outline. |
| `sub_82191AC8` | `GarageScreen::CameraB` (probably) | | As `CameraA` with 172 and `sub_821808F0(holder, 1)`. From the view's pages. Not seen in play. Read in outline. |
| `sub_82193320` | `GarageScreen::SetPlateQuestion` | | Movie 16's (the dialog's) cursor word +40960 = 0 (yes first); `SetNodeText` on node (-1, 0) string 96, (1, 3) string 2 and (1, 4) string 3 (the question, yes and no). A 128-byte frame. From `OnPlateDone`. Not seen in play. Read in full. |
| `sub_821933D0` | `GarageScreen::SetUseQuestion` | | r3 = a garage car. A 4,064-byte frame. Movie 16's cursor word set; a car object from the garage car (`Car::Init`, 1,884 bytes copied, `Car::Refresh`, `sub_820C4550`); `SetNodeText` on node (-1, 0) string 97 with the car's maker (1934 + the spec's byte +769) and name (1646 + id), (1, 3) 2 and (1, 4) 3. From `ActionsPage` (action 0). Not seen in play. Read in outline. |
| `sub_821934D0` | `GarageScreen::SetSaleQuestion` | | r3 = a garage car. A 4,112-byte frame. The data's +100 = `SalePrice(car)`; movie 18's cursor word set; a car object as `SetUseQuestion`'s; movie 18's texts: string 25, the maker, the car's two names, the price (`SetNodeNumberText`), 527, 435, yes (2) and no (3). From `ActionsPage` (action 1). Not seen in play. Read in outline. |
| `sub_82192690` | `GarageScreen::SalePrice` (probably) | | r3 = a garage car: (`CarValue(car)` × `[0x82002208]` + the car's word +1900 as a float × `[0x8200A348]`) × a factor, truncated to an integer, at least 50. The factor: the first of nine thresholds (floats at `0x8200A37C`) at or above the car's float +1892 picks the factor beside it (`0x8200A39C` + 4i); past all nine, `[0x8200A3BC]` (the ninth's again). `[0x82000A34]`, loaded first, is never used. A 112-byte frame (r31 saved). From `SetSaleQuestion`. Not seen in play. Read in outline. |
| `sub_82192378` | `GarageScreen::CarValue` (probably) | | r3 = a garage car: a float (f1) from its parts (476 lines, leaf). From `SalePrice`. Not seen in play. Not read in detail. |
| `sub_82191810` | `GarageScreen::SetUpPlateEditor` | | r3 = a garage car. A 4,000-byte frame. A car object from it (`Car::Init`, 1,884 bytes copied, `Car::Refresh`, `sub_820C4550`); the plate editor's data (`[0x826D8438]`): +48 = 0, +44 = 1, and words from the car object's +1568 on into +24. From `ActionsPage` (action 2). Not seen in play. Read in outline. |
| `sub_82191628` | `GarageScreen::SetUpSpecSheet` | | r3 = a garage car. A 5,104-byte frame. A car object from it (as above), `Vehicle::SetupFromCar` into the spec sheet's data (`[0x826D846C]` +112), its power curve (`PowerCurve::FromEngine`, `ShiftPeak(.., 0)`), then the spec sheet's words +16 to +56 (+96 = 2, the car's id and the curve's figures) with `SpecSheetUnits` twice. As `CarDealer::SetUpSpecSheet`. From `ActionsPage` (action 4). Not seen in play. Read in outline. |
| `sub_82191768` | `GarageScreen::SpecSheetUnits` (probably) | | Writes the spec sheet's +48 (6, or a value by its arguments), +52 and +56 (a word of its r28's +8). Leaf. From `SetUpSpecSheet` (twice). Not seen in play. Not read in detail. |
