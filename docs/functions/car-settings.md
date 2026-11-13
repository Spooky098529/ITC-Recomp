# Known functions: the car settings screen

The **car settings screen** (probably; `CarSettings::`, our name): the
machine the type 2 front-end mode sets up at its block's +0x173D0 with
`CarSettings::Init` `sub_821B6FC0` (from the mode's Init `sub_82197278`,
a lead) and opens with message 18 (the hub's item 2, string 111,
[`type2-screens.md`](type2-screens.md)) or 19; UI movie 3, asset 22554.
The type 5 front-end mode (the parking area's, `location-screen.md`)
sets the same machine up at its +848 block's +0 with its own Init
`CarSettings::InitType5` `sub_821B7878` (its other replies' keys, opened
by message 9). The type 2 mode's other machines are in
[`type2-screens.md`](type2-screens.md), [`type2-dialog.md`](type2-dialog.md),
[`garage-screen.md`](garage-screen.md), [`race-menu.md`](race-menu.md),
[`tuning-machine.md`](tuning-machine.md), [`car-dealer.md`](car-dealer.md),
[`options-screen.md`](options-screen.md) and
[`save-screen.md`](save-screen.md). The screen adjusts the selected
garage car's settings, the car object's setting bytes +72 to +94 and its
gear ratios +36 to +60 (`../known-data.md`'s "car object"), each between
fixed bounds, page by page, and in the type 2 mode offers a test run on
one of four courses (routes 8 to 10, `CarSettings::SetUpTestRun`), with
the best times of those courses (`CourseRecords::Of`) shown. The pages
of settings it offers depend on the car's parts (the tuning's +32, +40,
+56 and +136) and its drive type (spec block 2's byte +130; 4 lets a
page pick either of a pair and opens page 21).

It has the shape the type 2 mode's other machines have
(`type2-screens.md`'s intro; keys (state, message, 0, 0), member function
functors over vtable `0x82011918`, r3 the machine and r4 the mode, whose
message ring at +204 (50 entries of 12 bytes, the count +600 and the
start +604) the handlers post to): (0, 1) `Create` → 1 or 9; (1, 1) → 2
`OpenMovie`; (2, 1) `WaitForMovie` → 4 or 9; (9, 18) `Reenter` and
(9, 19) `ReenterToCourses` → 1 or 4; (4, 1) → 5 `Start`; in 5, Draw
(message 1) runs the page's input `Draw` and Update (message 0) the
transitions `Update` (`sub_821B6818`, a lead with the transitions); the
mode's yes/no dialog's replies (5, 38) `OnYes`, (5, 39) `OnNo`, (5, 41)
`OnBack`, (5, 42) `OnDialogClosed`; (5, 17) → 8 `Unload` (posted by page
6), (8, 3) `Leave`; (5, 4), (9, 4) and (10, 4) `Close`; (7, 4) → 6
`Destroy`. In the type 5 mode the replies are (5, 14), (5, 15), (5, 17)
and (5, 18), `Unload` is (5, 8), and (9, 9) `Reenter` opens it (no
`ReenterToCourses`). The handlers that post a message pick it by the
current mode's type `[0x826032DC]` (2 or 5; nothing for other types). Its
blocks are globals, each `VirtualAlloc`ed (`MEM_COMMIT`,
`PAGE_READWRITE`) by `Create` and freed by `Destroy`: `[0x826D8458]` the
page object (28 bytes, laid out as the race menu's `0x826D8404`: +0 the
page `Draw` runs, -1 while a transition runs; +4 the page after; +8 the
page before; +12 the transition `Update` runs, -1 none; +16 the one
queued after it; +20; +24 the transition's set-up done), `[0x826D845C]`
a front-end menu (68 bytes; `Menu::Step` runs on it every page, `Menu::Pressed`
reads buttons 159 (accept) and 158 (back), +0 the cursor, +8 set when
the last step moved it, +36 the side asked, -1 or 1) and `[0x826D8460]`
the screen's data (764 bytes; `../known-data.md`'s "car settings"). Pages
(`Draw`'s table on the page object's +0, 0 to 21; r3 the mode): 0 the
top menu (`TopPage`: five items, 0 to pages 1, 1 to 2, 2 to 3, 3 to 4,
4 to 5 the test run, each only when the car allows it; back to page 6),
1 to 4 the groups (`GroupPage1` to `GroupPage4`), 5 the test run's
courses (`CoursePage`), 6 leaving (`LeavePage`), 7 starting the test run
(`StartTestRunPage`), 8 the gear ratios (`GearPage`), 9 to 21 one
setting each (`SettingPage9` to `SettingPage21`). A page that moves on
sets the page object's +8 = +0 (unless already the target), +0 = -1, +4
= the page after, +12 = the transition, +16 = the queued one, and plays
a menu sound (`MenuSound::Play`: 0 accept, 2 back, 3 refused, 4 moved).
Each setting page edits a working copy of its values (+0 to +112 of the
data) between bounds; its last item applies the copy, takes the car's
values back or takes the stock car's (+732, 0 to 2), and back takes the
car's values back. The transitions (`Update`'s table on the page
object's +12) are the lead's.

In play (`../coverage.md`) the machine was only set up, created, its
movie opened and waited on, closed and destroyed (`Init`, `InitType5`,
`Create`, `OpenMovie`, `WaitForMovie`, `Close`, `Destroy`: 7 of the 91
rows): it was never started, and its pages and their helpers (84 rows)
never ran ("Not seen in play"). Names are ours (guesses from what the
code does). Read 2026-11-11 by the B7 loop's mapping agent: the two
Inits through `itc_recomp/tools/init_step_tables.py`, the handlers, the
pages, `Start` and its set-up, the test run's set-up and the small
helpers in full; the adjusters, the bars and the gear ratio helpers in
outline (their calls, the fields they read and write, their bounds),
the clones compared with the first of their family. `Update`
`sub_821B6818` (a 6,592-byte frame holding a table of its
transitions' functions, numbers up to 96 used by the pages) and the 114 functions only it reaches
(`821AB518`-`821AC2A8`, `821AC338`-`821ACF28`, `821AD2E0`-`821AD5D8`,
`821ADAF0`, `821ADF28`, `821AE198`, `821AE640`, `821AE6D8`, `821AEB48`,
`821AEC18`, `821B1678`-`821B49F8`, `821B4CE8`-`821B6708`) are left to a lead (the ~150-row
limit). The page functions and the bar helpers below are shared with
those transitions (named in the rows' callers).
Ready (`b7_groups.py`): 7 groups, each one of the functions seen in play
(the two Inits, `Create`, `OpenMovie`, `WaitForMovie`, `Close`,
`Destroy`); the others never ran, so the tool counts none of them, and
nothing outside the subsystem blocks it. A migration of `Init` takes the
functors of functions never seen in play (`Start`, `Draw`, the replies,
`Unload`, `Leave`, the re-entries) as they are: functors, with `Update`
recompiled until the lead's map.
Migrated by the B7 loop (2026-11-11, `--native_car_settings`,
`src/car_settings/`, adapters `src/hooks/car_settings.cpp`, harness
`car_settings`): the functions seen in play (the two Inits, `Create`,
`OpenMovie`, `WaitForMovie`, `Close`, `Destroy`). The functions never seen in play stay
recompiled (the Inits add them as functors, as the original does).

Shared, with their rows elsewhere: `StateMachine::Add` `sub_82106158`
([`game-modes.md`](game-modes.md)), `Menu::Step` `sub_82186568` and
`Menu::Pressed` `sub_821864C8` ([`menu-helpers.md`](menu-helpers.md)),
`MenuSound::Play` `sub_821103F0` ([`menu-sounds.md`](menu-sounds.md)),
`SaveData::Of` `sub_82105620` and `sub_820C4550` (block 2's address)
([`vehicle-accessors.md`](vehicle-accessors.md)), `CourseRecords::Of`
`sub_82105BF8` ([`save-data.md`](save-data.md)), `Car::Init`
`sub_820CD5D8`, `Car::SetModel` `sub_820CD8A8`, `Car::SetupVehicle`
`sub_820CD5C8`, `RaceData::ResetGlobal` `sub_821021A8`, `Stages::Clear`
`sub_82101900`, `Race::SetRoute` `sub_82101D38`, `Stages::SetRoute`
`sub_82101E90`, `Stages::AddRacer` `sub_820FFAB8`, `Race::AddRacer`
`sub_821017C0`, `Race::AddStage` `sub_82101828`, `Race::PlaceRacers`
`sub_82102048` ([`race-setup.md`](race-setup.md)), `sub_820FF918`,
`sub_820FF968`, `sub_820FFAB0` ([`vehicle-accessors.md`](vehicle-accessors.md)),
`Car::Refresh` `sub_820C4998`, `Params::GetCarParts` `sub_820C9E00`,
`AeroParts::Get82` `sub_820CBC90` ([`car-specs.md`](car-specs.md)),
`CarData::CarData` `sub_820AEF80` ([`car-physics.md`](car-physics.md)),
`GarageStage::FitCar` `sub_82178C40` ([`car-makers.md`](car-makers.md)),
`memcpy` `sub_825A7410` ([`c-runtime.md`](c-runtime.md)),
`VirtualAlloc` `sub_82295398` and `VirtualFree` `sub_82295330`
([`heap.md`](heap.md)), the UI movie calls `UiMovies::Open`
`sub_8211E808`, `IsLoaded` `sub_8211BE98`, `Find` `sub_8211B3F0`,
`Delete` `sub_8211E258`, `UiMovie::ResetPose` `sub_8211ABA8`,
`FindNode` `sub_8211A578`, `SetText` `sub_8211C1F8`, `SetNumberText`
`sub_8211C740`, `SetNodeText` `sub_8211C7E8`, `SetChildNameText`
`sub_8211D930`, `FinishAll` `sub_8211DFF8`
([`ui-movies-game.md`](ui-movies-game.md)) and `UiMovie::Child`
`sub_8259D658` ([`ui-movies.md`](ui-movies.md)).

Notes for the migration: `CoursePage`'s three `UiMovie::SetText` calls
on movie 16 leave r7 to r10 unset, and `Start`'s piece `Fill` sets r7
only for the day's (register leftovers: native callers pass 0, the
user's call of 2026-10-30, "Precedent first" in `../loops/b7-loop.md`).
`Close` and `Unload` are this machine's own (`Close` deletes movie 3,
unlike `TuningMachine::Close` `sub_821D6A78`). `ReenterToCourses` is
only in the type 2 mode's Init. Nothing here was seen past `Create`, so
the migration's harness builds the data block's world itself (the
selected garage car, the save data's course records).

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
| `sub_821B6FC0` | `CarSettings::Init` | Overridden | r3 = the car settings machine (the type 2 mode block's +0x173D0): 18 transitions through `StateMachine::Add` (`sub_82106158`), keys (state, message, 0, 0), each a member function functor over vtable `0x82011918` (the machine, the function) copied into the frame (six-word copy loops), the next state at the copy's +24 (0 for none): (0, 1) `Create` `sub_821AA2F8`; (1, 1) → 2 `OpenMovie` `sub_821A8DD0`; (2, 1) `WaitForMovie` `sub_821AA490`; (4, 1) → 5 `Start` `sub_821AFBD0`; (5, 1) `Draw` `sub_821B4BA0`; (5, 0) `Update` `sub_821B6818` (a lead); (5, 38) `OnYes` `sub_821A8DE0`; (5, 39) `OnNo` `sub_821A8E28`; (5, 41) `OnBack` `sub_821A8E70`; (5, 42) `OnDialogClosed` `sub_821A8E88`; (10, 4) `Close` `sub_821AAF70`; (5, 17) → 8 `Unload` `sub_821AAF08`; (8, 3) `Leave` `sub_821AFC88`; (9, 18) `Reenter` `sub_821AA500`; (9, 19) `ReenterToCourses` `sub_821AA530`; (5, 4), (9, 4) `Close`; (7, 4) → 6 `Destroy` `sub_821AA408`. A 1,296-byte frame (r27 to r31 saved); decodes as the other Inits' step tables (`itc_recomp/tools/init_step_tables.py`, the functors' words the original never writes copied as the frame holds them). Only caller the type 2 mode's Init `sub_82197278`. Seen in play. Read in full (through the tool). Native (B7 loop, 2026-11-11): `car_settings::Init` (`src/car_settings/init.cpp`) over its step table (`init_tables.h`, from `itc_recomp/tools/init_step_tables.py`), `StateMachine::Add`'s plain C++; the functors of the handlers never seen in play added as they are (those handlers stay recompiled); harness `car_settings`. Its caller, the type 2 mode's Init, is recompiled: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821B7878` | `CarSettings::InitType5` | Overridden | The same machine's Init for the type 5 mode (the parking area's; r3 its +848 block's +0, set up by `ModeType5::Init` `sub_8222FD68`, [`type5-mode.md`](type5-mode.md)): 17 transitions, the same handlers as `Init` but for the replies (5, 14) `OnYes`, (5, 15) `OnNo`, (5, 17) `OnBack`, (5, 18) `OnDialogClosed`, `Unload` at (5, 8) → 8 and `Reenter` at (9, 9); no `ReenterToCourses`. A 1,232-byte frame (r27 to r31 saved); decodes through `itc_recomp/tools/init_step_tables.py`. Only caller `sub_8222FD68`. Seen in play. Read in full (through the tool). Native (B7 loop, 2026-11-11): `car_settings::InitType5` (`src/car_settings/init.cpp`) over its step table (`init_tables.h`), as `Init`; harness `car_settings`. Its caller, the type 5 mode's set-up `sub_8222FD68`, is recompiled: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821AA2F8` | `CarSettings::Create` | Overridden | (0, 1): r3 = the machine. +9608 (the movie loaded) = 0; `VirtualAlloc(0, 28, 0x1000, 4)` (`sub_82295398`) into `[0x826D8458]` (the page object: +0, +4, +8, +24 = 0, +12, +20 = -1; +16 not written), `VirtualAlloc(0, 68, …)` into `[0x826D845C]` (the menu: +0, +4, +8, +12, +20, +24 to +40, +48, +52, +64 = 0, +16 = 4), `VirtualAlloc(0, 764, …)` into `[0x826D8460]` (the data: +748 = 0, +760 = 21; the rest as the allocation leaves it, zero); +9604 = 1 when +9612 (open on its message) is set, else 9. A 112-byte frame (r30, r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-11): `car_settings::Create` (`src/car_settings/handlers.cpp`), `VirtualAlloc` through its override; harness `car_settings` (the blocks made compared by content, then freed). A functor the machine calls: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821A8DD0` | `CarSettings::OpenMovie` | Overridden | (1, 1) → 2: `UiMovies::Open(3, 22554)` (`sub_8211E808`, tail call). Seen in play. Read in full. Native (B7 loop, 2026-11-11): `car_settings::OpenMovie` (`src/car_settings/handlers.cpp`), `UiMovies::Open` through its override with the adapter's own r1 and lr (the tail call); harness `car_settings` (the call recorded). A functor the machine calls: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821AA490` | `CarSettings::WaitForMovie` | Overridden | (2, 1): while +9608 is 0, +9608 = 1 once `UiMovies::IsLoaded(3)` (`sub_8211BE98`); once set, +9604 = 9 when +9612 is set, else 4. A 96-byte frame (r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-11): `car_settings::WaitForMovie` (`src/car_settings/handlers.cpp`), `UiMovies::IsLoaded`'s plain C++; harness `car_settings`. A functor the machine calls: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821AA500` | `CarSettings::Reenter` | | (9, 18) (type 2), (9, 9) (type 5): the data's +580 = 0 (start at the top menu); +9604 = 4 when +9608 (the movie loaded) is set, else 1. Leaf. Not seen in play. Read in full. |
| `sub_821AA530` | `CarSettings::ReenterToCourses` | | (9, 19), only in the type 2 mode's `Init`: the data's +580 = 1 (start at the test run's courses, `Start`; probably back from a test run); +9604 = 4 when +9608 is set, else 1. Leaf. Not seen in play. Read in full. |
| `sub_821AFBD0` | `CarSettings::Start` | | (4, 1) → 5: r3 = the machine. `SetUp` (`sub_821AF0A8`), `Fill` (`sub_821AB2C0`); then on the page object: with the data's +580 set, +8 = +0 (unless 5), +12 = 26, +16 = 23, +4 = 5, +0 = -1 and the data's +744 = 0; else +8 = +0 (unless 0), +12 = 1, +16 = 2, +4 = 0, +0 = -1 and +744 = 1. A 96-byte frame. Not seen in play. Read in full. |
| `sub_821AF0A8` | `CarSettings::SetUp` | | Fills the data block `[0x826D8460]` from player 0's save data (`SaveData::Of(0)`) and the selected garage car (the save's +0x19920 counted through the garage's halfword links from +6120 +204; the car at +6120 + 212 + 1,968 × slot, its tuning at +1648): a frame car (`Car::Init`, the garage car's 1,884 bytes, `Car::Refresh`), a stock car of the same id (`Car::Init`, `Car::SetModel(car +12)`, `Car::Refresh`) and a car data (`CarData::CarData`, `Car::SetupVehicle`). +528 to +547 the save's +8 to +27 (the name), +548 its +6116 (the money), +552 the day (+6100) mod 7, +556 the day (at most 999); +584, +588, +592 = the tuning's +32, +56, +40 nonzero (which setting groups the car allows: the gears, pages 9 and 10, pages 11 to 16); +596 = with the tuning's +136 0 the frame car's +3744 bit 0, else `AeroParts::Get82(Params::GetCarParts(car id), +136)` +24 bit 0x4 (page 17); +688 = block 2's byte +130 (probably the drive type; 4 both ends), +692 = the tuning's +52, +696 and +700 = the car data's floats +388 and +1280, +600 = block 2's +100. The best times: for routes 8, 9 and 10 (`CourseRecords::Of(save + 0x1D52C, {route, 1})`), five 12-byte rows each at +348, +408, +468 ({the entry's +20, its +0, 1}, or +8 = 0 past the record's count, halfword +48; entries by `CourseRecord::Entry`). The car's settings into +116 to +228 (the applied copy: floats +116 to +136 the gear ratios car +40 to +60, +140 the final drive car +36; words from the car's signed bytes +72 to +94, +212 = byte +93 × 5) and the stock car's into +232 to +344 the same way; the car's values replaced by the stock's where +584, +588, +592 or +596 is 0. In the type 2 mode +560 = 0; in the type 5 mode +560 = 1 and +752 = the save's +6108 (the location, 14 to 19) − 14, else 3. A 9,936-byte frame (`__savegprlr_24`). Only caller `Start`. Not seen in play. Read in full (its stores in outline). |
| `sub_821AAE90` | `CourseRecord::Entry` | | r3 = out (28 bytes), r4 = a course record, r5 = a rank: follows the record's halfword links from halfword +44 r5 times and copies the 28-byte entry +52 + 28 × the link's halfword +2 into r3; returns r3. A 96-byte frame (r31 saved). Only caller `SetUp`. Not seen in play. Read in full. |
| `sub_821AB2C0` | `CarSettings::Fill` | | Movie 3 (`UiMovies::Find`): `UiMovie::ResetPose`; `SetChildNameText(m, 8, 6, data +528)` (the name), `SetNumberText(m, 8, 9, +548)` (the money), `SetText(m, 8, 13, 8, +556)` (string 8 with the day); nodes found with `UiMovie::FindNode` (scene, child): (8, 11) +108 = the weekday (+552), (10, 4), (10, 5) +108 = 0, (10, 6), (10, 7) +108 = 1, (10, 8), (10, 9) +80 = 0; `SetNodeText(m, node, r5)` on (4, 3) with 4, (4, 4) with 6, (4, 5) with 7. With +560 0: +564 = 5 and node (-1, 17) +80 = 0; with +560 1: +564 = 4, node (-1, 17) +80 = 1 and `SetText(m, -1, 29, 369 + +752)` (the location's name). Then the word stack at +708 (its count +728) reset to one 0, +756 = 0, +740 = 0, +748 = 1, +576 = 0. A 128-byte frame (r28 to r31 saved). Only caller `Start`. Not seen in play. Read in full. |
| `sub_821B4BA0` | `CarSettings::Draw` | | (5, 1): r4 = the mode. Builds a 22-entry table of the pages' functions in its frame and, unless the page object's +0 is -1, calls entry +0 with r3 = the mode: 0 `TopPage`, 1 to 4 `GroupPage1` to `GroupPage4`, 5 `CoursePage`, 6 `LeavePage`, 7 `StartTestRunPage`, 8 `GearPage`, 9 to 21 `SettingPage9` to `SettingPage21`. A 304-byte frame (r16 to r31 saved). Not seen in play. Read in full. |
| `sub_821A8DE0` | `CarSettings::OnYes` | | (5, 38) (type 2), (5, 14) (type 5): the data's +572 = 0; player 0's save data's +0x1F870 = 1. A 96-byte frame. Not seen in play. Read in full. |
| `sub_821A8E28` | `CarSettings::OnNo` | | (5, 39), (5, 15): +572 = 1; the save's +0x1F870 = 0. A 96-byte frame. Not seen in play. Read in full. |
| `sub_821A8E70` | `CarSettings::OnBack` | | (5, 41), (5, 17): +572 = 2. Leaf. Not seen in play. Read in full. |
| `sub_821A8E88` | `CarSettings::OnDialogClosed` | | (5, 42), (5, 18): with +572 2 (back): the page object's +8 = +0 (unless 5), +0 = 5 (`CoursePage` again), +12 = -1; else +8 = +0 (unless 7), +12 = 5, +0 = -1, +4 = 7 (`StartTestRunPage` after transition 5); +16 = -1. Leaf. Not seen in play. Read in full. |
| `sub_821AAF08` | `CarSettings::Unload` | | (5, 17) → 8 (type 2), (5, 8) → 8 (type 5): `UiMovie::FinishAll` on movie 3; with +9616 set, `UiMovies::Delete(3)` and +9608 = 0. A 112-byte frame (r31 saved). Not seen in play. Read in full. |
| `sub_821AFC88` | `CarSettings::Leave` | | (8, 3): r3 = the machine, r4 = the mode: posts message 6 (type 2, the hub) or 7 (type 5) to the mode's ring (+204), `Apply` (`sub_821AFA08`), +9604 = 9. A 96-byte frame (r31 saved). Not seen in play. Read in full. |
| `sub_821AAF70` | `CarSettings::Close` | Overridden | (5, 4), (9, 4), (10, 4): r3 = the machine, r4 = the mode: +9604 = 7; movie 3 (`UiMovies::Find`): when there, `UiMovie::FinishAll` and `UiMovies::Delete(3)`; message 4 posted to the mode's ring for mode types 2 and 5. A 112-byte frame (r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-11): `car_settings::Close` (`src/car_settings/handlers.cpp`), `UiMovies::Find`'s plain C++, `UiMovie::FinishAll` and `UiMovies::Delete` through their overrides; returns Delete's r3, or 0 when movie 3 isn't there; harness `car_settings` (both branches, the mode types). A functor the machine calls: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821AA408` | `CarSettings::Destroy` | Overridden | (7, 4) → 6: +9604 = 11 when the data's +576 is 1 (a test run set up), else 6; `VirtualFree(p, 0, 0x8000)` (`sub_82295330`) on `[0x826D8458]`, `[0x826D845C]` and `[0x826D8460]`, and the three globals = 0. A 128-byte frame (r28 to r31 saved). Seen in play. Read in full. Native (B7 loop, 2026-11-11): `car_settings::Destroy` (`src/car_settings/handlers.cpp`), `VirtualFree` through its override; harness `car_settings` (the blocks made as the layer's pages, whether each was freed compared). A functor the machine calls: the adapter stays (`src/hooks/car_settings.cpp`, `--native_car_settings`). |
| `sub_821AFA08` | `CarSettings::Apply` | | Copies the applied settings into the selected garage car (player 0's save, found as `SetUp` does): floats +116 to +136 into the car's +40 to +60, +140 into +36; the words +144 to +228 into its bytes +72 to +94 (+144, +148 → +72, +73; +152, +156 → +74, +75; +160, +164 → +80, +81; +168, +172 → +78, +79; +176 to +204 → +82 to +89; +208 → +94; +212 / 5 → +93; +216, +220 → +90, +91; +224, +228 → +76, +77). A 96-byte frame. Callers `Leave`, `StartTestRunPage`. Not seen in play. Read in full. |
| `sub_821AED18` | `CarSettings::SetUpTestRun` | | r3 = the course picked (0 to 3, the data's +568): the route 8 (0, 1), 9 (2) or 10 (3) and the stage kind 4 (0; none added) or 3. `RaceData::ResetGlobal`, `Stages::Clear(race data +0x59620)`, the race data's (`[0x826C5B2C]`) +0xA3AF0 = 1 and +0xA3AF4 = 2, `Race::SetRoute(route, 48)`, `sub_820FF918(0)`; player 0's selected garage car's +1964 and the frame car's +3892 = 1 when the dialog's answer (+572) was 0 (yes), else 0; the frame car (`Car::Init`, the garage car's 1,884 bytes, `Car::Refresh`) added with `Race::AddRacer(car, 0, the garage car's float +1892, 0)`; for kinds other than 4 `Stages::SetRoute(stages, route, 48, -1)`, `Stages::AddRacer(stages, 0, 2, -1)` and +0xA3AF4 = 0, else +0xA3AF4 = 1; `Race::AddStage(kind)`, `sub_820FF968(0)`, +0x59AE4 \|= 8, `sub_820FFAB0(stages, 1)`, `Race::PlaceRacers`. A 4,064-byte frame (`__savegprlr_24`). Only caller `StartTestRunPage`. Not seen in play. Read in full. |
| `sub_821AEF48` | `CarSettings::RefitStandCar` | | r3 = which pair (0 camber, page 11; 1 page 12; 2 page 16): unless +560 is 1 (the type 5 mode, no stand), copies the selected garage car (1,968 bytes) into the frame, puts the working pair over the applied bytes (0: +44/+48 into the car's +80/+81, 1: +52/+56 into +78/+79, 2: +84/+88 into +88/+89; the others from +160, +168, +200), builds a car object from it (`Car::Init`, 1,884 bytes, flags +3888 \|= 1, `Car::Refresh`) and fits it on the stand (`GarageStage::FitCar([[0x826D83E0]], car)`). A 6,016-byte frame. Callers `SettingPage11`, `SettingPage12`, `SettingPage16`. Not seen in play. Read in full. |
| `sub_821AA560` | `CarSettings::TopPage` | | Page 0 (r3 = the mode): `Menu::Step` on `[0x826D845C]`; moved (+8): transition 4, page 0 again, sound 4; accept (159) by the cursor: 0 (with +584 or +588) → page 1 (transitions 6, 7), 1 (+592) → page 2 (10, 11), 2 (+592) → page 3 (14, 15), 3 → page 4 (18, 19), 4 (with +560 0) → page 5 (22, 23), sound 0, or sound 3 when the car doesn't allow it; back (158): transition 5, page 6, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821AA7B8` | `CarSettings::GroupPage1` | | Page 1, as `TopPage`: moved → transition 8, page 1; accept: 0 (+584) → page 8, the gears (27, 28); 1 (+588) → page 9 (32, 33); 2 (+588) → page 10 (37, 38); else sound 3; back → transition 9, page 0. A 96-byte frame. Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821AA970` | `CarSettings::GroupPage2` | | Page 2: moved → 12; accept: 0 → page 11 (42, 43), 1 → page 12 (47, 48); back → 13, page 0. A 96-byte frame. Only caller `Draw`. Not seen in play. Read in outline (its branches as `GroupPage1`'s). |
| `sub_821AAAB8` | `CarSettings::GroupPage3` | | Page 3: moved → 16; accept: 0 to 3 → pages 13 to 16 (52, 53; 57, 58; 62, 63; 67, 68); back → 17, page 0. A 96-byte frame. Only caller `Draw`. Not seen in play. Read in outline. |
| `sub_821AAC60` | `CarSettings::GroupPage4` | | Page 4: moved → 20; accept: 0 (with +596) → page 17 (72, 73), 1 to 3 → pages 18 to 20 (77, 78; 82, 83; 87, 88), 4 (with +688 4) → page 21 (92, 93), else sound 3; back → 21, page 0. A 96-byte frame. Only caller `Draw`. Not seen in play. Read in outline. |
| `sub_821AB038` | `CarSettings::CoursePage` | | Page 5, the test run's course: moved → transition 24, page 5, sound 4; accept: sound 0, +568 = the cursor (the course), movie 16 (the mode's dialog): `SetText(m, -1, 0, 10)`, `SetText(m, 1, 3, 0)`, `SetText(m, 1, 4, 1)`, its +0xA000 (the default answer) = 0 when player 0's save +0x1F870 is set, else 1; the page object all -1 (+0, +12, +16; +8 = +0 unless -1); message 34 (type 2) or 10 (type 5) posted to the mode's ring (the dialog); back: transition 25, page -1, sound 2. A 112-byte frame (r30, r31 saved). Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821AB250` | `CarSettings::LeavePage` | | Page 6: posts message 17 (type 2) or 8 (type 5) to the mode's ring (`Unload`). Leaf. Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821AFD28` | `CarSettings::StartTestRunPage` | | Page 7: `Apply`, `SetUpTestRun(+568)`; +576 = 1 (`Destroy` then leaves +9604 at 11), +760 = 10. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821AFD78` | `CarSettings::GearPage` | | Page 8, the gear ratios (items 0 to 5 the gears, 6 the final drive, 7 apply): moved → transition 29, sound 4; `GearAdjust` changed one → transition 30, sound 4; accept on items 0 to 6 moves the cursor to item 7 (+4 = the item); on item 7 by +732: 0 the working floats (+0 to +24) into the applied (+116), `GearBarsApplied`, back to page 1; 1 the applied into the working, 2 the stock (+232) into the working, then `GearBars`; back: the applied into the working (`memcpy`, 28 bytes), `GearBars`, transition 0 then 31, page 1, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821A8EF8` | `CarSettings::GearAdjust` | | For `GearPage`: with the menu's side (+36) -1 or 1 changes the gear under the cursor by a step (the float at `0x8200222C`) within its bounds (+604 + 12 × item: {min, max, …}, seven entries to +687) and pushes the following gears so they stay in order (each at most the one before); item 6 (the final drive) alone; on item 7 cycles +732 (0 to 2, +736 the one before). Returns 1 when a value changed. Registers saved through `__savegprlr`. Not seen in play. Read in outline. |
| `sub_821AD840` | `CarSettings::GearBars` | | Movie 3: for the seven ratios (working, +0 to +24) and nodes (20, i): child 21's text with string 442 (`SetNodeText`), a bar, and a speed worked out from the ratio, the car data's +696 and +700 floats (at most 999; string 8 into child 23); child 5 a bar. Floats f27 to f31 saved. Callers `GearPage` and the transitions `sub_821B1678`, `sub_821B25C8`, `sub_821B4E10` (the lead). Not seen in play. Read in outline. |
| `sub_821ADA08` | `CarSettings::GearBarsApplied` | | As `GearBars` for the applied ratios (+116 to +140): child 6's marker on nodes (20, i). Callers `GearPage`, `sub_821B1678`, `sub_821B4E10` (the lead). Not seen in play. Read in outline. |
| `sub_821ACFB0` | `CarSettings::SetBar` | | r3 = a node, r4 = a value, r5 = the minimum, r6 = the maximum: child 7's text = the value (`SetNodeText(movie 3, child 7, 8, value)`); child 5's +12 = (value − min) / (max − min) × the float at `0x8200A518`, truncated (the bar's length). A frame from `__savegprlr_27` (f31 saved). Callers the working bars below (12). Not seen in play. Read in full. |
| `sub_821AD090` | `CarSettings::SetMark` | | r3 = a node, r4, r5, r6 as `SetBar`: child 6's +12 = the same scaled value (the applied value's mark); child 9's +28 = the float at `0x8200A51C`. Callers the applied bars below (9). Not seen in play. Read in full. |
| `sub_821AD148` | `CarSettings::SetMarkSingle` | | As `SetMark` for the single-value pages (children 6 and 8). Callers `SettingBars17`, `18`, `19`, `21`'s applied ones (`sub_821AE7A8`, `sub_821AE868`, `sub_821AE928`, `sub_821AEAE8`). Not seen in play. Read in outline. |
| `sub_821AD200` | `CarSettings::SetBarPercent` | | As `SetBar` with the value's text through string 443 (a percentage, probably) on child 7. Only caller `sub_821AEA88` (page 21). Not seen in play. Read in outline. |
| `sub_821AFF30` | `CarSettings::SettingPage9` | | Page 9, one setting (items 0 and 1 the pair, 2 apply; the working +28 and +32, applied +144 and +148, stock +260 and +264; the car's bytes +72, +73): `Menu::Step`; moved → transition 34, page 9, sound 4; `SettingAdjust9` changed it → transition 35, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks9`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars9`); back: the applied into the working, `SettingBars9`; leaving: transition 0 then 36, page 1, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in full. |
| `sub_821A9248` | `CarSettings::SettingAdjust9` | | For `SettingPage9`: with the menu's side (+36) -1 or 1, the value under the cursor's index by the drive type (+688: 0 the first, 1 to 3 the second, 4 the cursor's) (+28 and +32) by 1 within 0 to 10 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in full. |
| `sub_821ADC48` | `CarSettings::SettingBars9` | | Movie 3: the working +28 and +32 on nodes (21, 8) and (21, 9) through `SetBar` (0 to 10) (the value picked by the drive type, as `SettingAdjust9`). Callers `SettingPage9` and the transitions `sub_821B1678`, `sub_821B2870` (the lead). Not seen in play. Read in full. |
| `sub_821ADD00` | `CarSettings::SettingMarks9` | | Movie 3: the marks (the applied values, after an apply) at the working +28 and +32 on nodes (21, 8) and (21, 9) through `SetMark` (0 to 10). Callers `SettingPage9` and `sub_821B1678` (the lead). Not seen in play. Read in outline. |
| `sub_821B0100` | `CarSettings::SettingPage10` | | Page 10, one setting (items 0 and 1 the pair, 2 apply; the working +36 and +40, applied +152 and +156, stock +268 and +272; the car's bytes +74, +75): `Menu::Step`; moved → transition 39, page 10, sound 4; `SettingAdjust10` changed it → transition 40, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks10`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars10`); back: the applied into the working, `SettingBars10`; leaving: transition 0 then 41, page 1, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A93B0` | `CarSettings::SettingAdjust10` | | For `SettingPage10`: with the menu's side (+36) -1 or 1, the value under the cursor's index by the drive type (+688: 0 the first, 1 to 3 the second, 4 the cursor's) (+36 and +40) by 1 within 0 to 10 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821ADDB8` | `CarSettings::SettingBars10` | | Movie 3: the working +36 and +40 on nodes (21, 8) and (21, 9) through `SetBar` (0 to 10) (the value picked by the drive type, as `SettingAdjust10`). Callers `SettingPage10` and the transitions `sub_821B1678`, `sub_821B29D0` (the lead). Not seen in play. Read in outline. |
| `sub_821ADE70` | `CarSettings::SettingMarks10` | | Movie 3: the marks (the applied values, after an apply) at the working +36 and +40 on nodes (21, 8) and (21, 9) through `SetMark` (0 to 10). Callers `SettingPage10` and `sub_821B1678` (the lead). Not seen in play. Read in outline. |
| `sub_821B02D0` | `CarSettings::SettingPage11` | | Page 11, one setting (items 0 and 1 the pair, 2 apply; the working +44 and +48, applied +160 and +164, stock +276 and +280; the car's bytes +80, +81 (probably the camber, front and rear)): `Menu::Step`; moved → transition 44, page 11, sound 4; `SettingAdjust11` changed it → transition 45, sound 4, `RefitStandCar(0)` when a value changed and on back; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks11`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars11`); back: the applied into the working, `SettingBars11`; leaving: transition 0 then 46, page 2, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9518` | `CarSettings::SettingAdjust11` | | For `SettingPage11`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+44 and +48) by 1 within -10 to 0 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821ADF98` | `CarSettings::SettingBars11` | | Movie 3: the working +44 and +48 on nodes (22, 7) and (22, 8) through `SetBar` (-10 to 0). Callers `SettingPage11` and the transitions `sub_821B1960`, `sub_821B2C78`, `sub_821B4FD8` (the lead). Not seen in play. Read in outline. |
| `sub_821AE018` | `CarSettings::SettingMarks11` | | Movie 3: the marks (the applied values, after an apply) at the working +44 and +48 on nodes (22, 7) and (22, 8) through `SetMark` (-10 to 0). Callers `SettingPage11` and `sub_821B1960`, `sub_821B4FD8` (the lead). Not seen in play. Read in outline. |
| `sub_821B04C8` | `CarSettings::SettingPage12` | | Page 12, one setting (items 0 and 1 the pair, 2 apply; the working +52 and +56, applied +168 and +172, stock +284 and +288; the car's bytes +78, +79): `Menu::Step`; moved → transition 49, page 12, sound 4; `SettingAdjust12` changed it → transition 50, sound 4, `RefitStandCar(1)` when a value changed and on back; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks12`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars12`); back: the applied into the working, `SettingBars12`; leaving: transition 0 then 51, page 2, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9658` | `CarSettings::SettingAdjust12` | | For `SettingPage12`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+52 and +56) by 1 within -5 to 5 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE098` | `CarSettings::SettingBars12` | | Movie 3: the working +52 and +56 on nodes (22, 7) and (22, 8) through `SetBar` (-5 to 5). Callers `SettingPage12` and the transitions `sub_821B1960`, `sub_821B2F20` (the lead). Not seen in play. Read in outline. |
| `sub_821AE118` | `CarSettings::SettingMarks12` | | Movie 3: the marks (the applied values, after an apply) at the working +52 and +56 on nodes (22, 7) and (22, 8) through `SetMark` (-5 to 5). Callers `SettingPage12` and `sub_821B1960` (the lead). Not seen in play. Read in outline. |
| `sub_821B06C0` | `CarSettings::SettingPage13` | | Page 13, one setting (items 0 and 1 the pair, 2 apply; the working +60 and +64, applied +176 and +180, stock +292 and +296; the car's bytes +82, +83): `Menu::Step`; moved → transition 54, page 13, sound 4; `SettingAdjust13` changed it → transition 55, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks13`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars13`); back: the applied into the working, `SettingBars13`; leaving: transition 0 then 56, page 3, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9798` | `CarSettings::SettingAdjust13` | | For `SettingPage13`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+60 and +64) by 1 within -5 to 5 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE240` | `CarSettings::SettingBars13` | | Movie 3: the working +60 and +64 on nodes (23, 7) and (23, 8) through `SetBar` (-5 to 5). Callers `SettingPage13` and the transitions `sub_821B1B40`, `sub_821B3080`, `sub_821B51C0` (the lead). Not seen in play. Read in outline. |
| `sub_821AE2C0` | `CarSettings::SettingMarks13` | | Movie 3: the marks (the applied values, after an apply) at the working +60 and +64 on nodes (23, 7) and (23, 8) through `SetMark` (-5 to 5). Callers `SettingPage13` and `sub_821B1B40`, `sub_821B51C0` (the lead). Not seen in play. Read in outline. |
| `sub_821B0890` | `CarSettings::SettingPage14` | | Page 14, one setting (items 0 and 1 the pair, 2 apply; the working +68 and +72, applied +184 and +188, stock +300 and +304; the car's bytes +84, +85): `Menu::Step`; moved → transition 59, page 14, sound 4; `SettingAdjust14` changed it → transition 60, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks14`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars14`); back: the applied into the working, `SettingBars14`; leaving: transition 0 then 61, page 3, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A98D8` | `CarSettings::SettingAdjust14` | | For `SettingPage14`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+68 and +72) by 1 within -5 to 5 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE340` | `CarSettings::SettingBars14` | | Movie 3: the working +68 and +72 on nodes (23, 7) and (23, 8) through `SetBar` (-5 to 5). Callers `SettingPage14` and the transitions `sub_821B1B40`, `sub_821B31E0` (the lead). Not seen in play. Read in outline. |
| `sub_821AE3C0` | `CarSettings::SettingMarks14` | | Movie 3: the marks (the applied values, after an apply) at the working +68 and +72 on nodes (23, 7) and (23, 8) through `SetMark` (-5 to 5). Callers `SettingPage14` and `sub_821B1B40` (the lead). Not seen in play. Read in outline. |
| `sub_821B0A60` | `CarSettings::SettingPage15` | | Page 15, one setting (items 0 and 1 the pair, 2 apply; the working +76 and +80, applied +192 and +196, stock +308 and +312; the car's bytes +86, +87): `Menu::Step`; moved → transition 64, page 15, sound 4; `SettingAdjust15` changed it → transition 65, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks15`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars15`); back: the applied into the working, `SettingBars15`; leaving: transition 0 then 66, page 3, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9A18` | `CarSettings::SettingAdjust15` | | For `SettingPage15`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+76 and +80) by 1 within -5 to 5 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE440` | `CarSettings::SettingBars15` | | Movie 3: the working +76 and +80 on nodes (23, 7) and (23, 8) through `SetBar` (-5 to 5). Callers `SettingPage15` and the transitions `sub_821B1B40`, `sub_821B3340` (the lead). Not seen in play. Read in outline. |
| `sub_821AE4C0` | `CarSettings::SettingMarks15` | | Movie 3: the marks (the applied values, after an apply) at the working +76 and +80 on nodes (23, 7) and (23, 8) through `SetMark` (-5 to 5). Callers `SettingPage15` and `sub_821B1B40` (the lead). Not seen in play. Read in outline. |
| `sub_821B0C30` | `CarSettings::SettingPage16` | | Page 16, one setting (items 0 and 1 the pair, 2 apply; the working +84 and +88, applied +200 and +204, stock +316 and +320; the car's bytes +88, +89): `Menu::Step`; moved → transition 69, page 16, sound 4; `SettingAdjust16` changed it → transition 70, sound 4, `RefitStandCar(2)` when a value changed and on back; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks16`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars16`); back: the applied into the working, `SettingBars16`; leaving: transition 0 then 71, page 3, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9B58` | `CarSettings::SettingAdjust16` | | For `SettingPage16`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+84 and +88) by 1 within -5 to 5 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE540` | `CarSettings::SettingBars16` | | Movie 3: the working +84 and +88 on nodes (22, 7) and (22, 8) through `SetBar` (-5 to 5). Callers `SettingPage16` and the transitions `sub_821B1B40`, `sub_821B35E0` (the lead). Not seen in play. Read in outline. |
| `sub_821AE5C0` | `CarSettings::SettingMarks16` | | Movie 3: the marks (the applied values, after an apply) at the working +84 and +88 on nodes (22, 7) and (22, 8) through `SetMark` (-5 to 5). Callers `SettingPage16` and `sub_821B1B40` (the lead). Not seen in play. Read in outline. |
| `sub_821B0E28` | `CarSettings::SettingPage17` | | Page 17, one setting (item 0 the value, 1 apply; the working +92, applied +208, stock +324; the car's bytes +94): `Menu::Step`; moved → transition 74, page 17, sound 4; `SettingAdjust17` changed it → transition 75, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks17`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars17`); back: the applied into the working, `SettingBars17`; leaving: transition 0 then 76, page 4, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9C98` | `CarSettings::SettingAdjust17` | | For `SettingPage17`: with the menu's side (+36) -1 or 1, the value (item 0) +92 by 1 within 0 to 10 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE748` | `CarSettings::SettingBars17` | | Movie 3: the working +92 on node (24, 4) through `SetBar` (0 to 10). Callers `SettingPage17` and the transitions `sub_821B1DA0`, `sub_821B3740`, `sub_821B53A8` (the lead). Not seen in play. Read in outline. |
| `sub_821AE7A8` | `CarSettings::SettingMarks17` | | Movie 3: the marks (the applied values, after an apply) at the working +92 on node (24, 4) through `SetMarkSingle` (0 to 10). Callers `SettingPage17` and `sub_821B1DA0`, `sub_821B53A8` (the lead). Not seen in play. Read in outline. |
| `sub_821B0FC8` | `CarSettings::SettingPage18` | | Page 18, one setting (item 0 the value, 1 apply; the working +112, applied +228, stock +344; the car's bytes +77): `Menu::Step`; moved → transition 79, page 18, sound 4; `SettingAdjust18` changed it → transition 80, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks18`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars18`); back: the applied into the working, `SettingBars18`; leaving: transition 0 then 81, page 4, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9DE0` | `CarSettings::SettingAdjust18` | | For `SettingPage18`: with the menu's side (+36) -1 or 1, the value (item 0) +112 by 1 within 0 to 10 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE808` | `CarSettings::SettingBars18` | | Movie 3: the working +112 on node (24, 4) through `SetBar` (0 to 10). Callers `SettingPage18` and the transitions `sub_821B1DA0`, `sub_821B39A8` (the lead). Not seen in play. Read in outline. |
| `sub_821AE868` | `CarSettings::SettingMarks18` | | Movie 3: the marks (the applied values, after an apply) at the working +112 on node (24, 4) through `SetMarkSingle` (0 to 10). Callers `SettingPage18` and `sub_821B1DA0` (the lead). Not seen in play. Read in outline. |
| `sub_821B1168` | `CarSettings::SettingPage19` | | Page 19, one setting (item 0 the value, 1 apply; the working +108, applied +224, stock +340; the car's bytes +76): `Menu::Step`; moved → transition 84, page 19, sound 4; `SettingAdjust19` changed it → transition 85, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks19`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars19`); back: the applied into the working, `SettingBars19`; leaving: transition 0 then 86, page 4, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821A9F28` | `CarSettings::SettingAdjust19` | | For `SettingPage19`: with the menu's side (+36) -1 or 1, the value (item 0) +108 by 1 within 0 to 10 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE8C8` | `CarSettings::SettingBars19` | | Movie 3: the working +108 on node (24, 4) through `SetBar` (0 to 10). Callers `SettingPage19` and the transitions `sub_821B1DA0`, `sub_821B3B08` (the lead). Not seen in play. Read in outline. |
| `sub_821AE928` | `CarSettings::SettingMarks19` | | Movie 3: the marks (the applied values, after an apply) at the working +108 on node (24, 4) through `SetMarkSingle` (0 to 10). Callers `SettingPage19` and `sub_821B1DA0` (the lead). Not seen in play. Read in outline. |
| `sub_821B1308` | `CarSettings::SettingPage20` | | Page 20, one setting (items 0 and 1 the pair, 2 apply; the working +100 and +104, applied +216 and +220, stock +332 and +336; the car's bytes +90, +91): `Menu::Step`; moved → transition 89, page 20, sound 4; `SettingAdjust20` changed it → transition 90, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks20`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars20`); back: the applied into the working, `SettingBars20`; leaving: transition 0 then 91, page 4, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821AA070` | `CarSettings::SettingAdjust20` | | For `SettingPage20`: with the menu's side (+36) -1 or 1, the value under the cursor (item 0 or 1) (+100 and +104) by 1 within -10 to 0 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AE988` | `CarSettings::SettingBars20` | | Movie 3: the working +100 and +104 on nodes (23, 7) and (23, 8) through `SetBar` (-10 to 0). Callers `SettingPage20` and the transitions `sub_821B1DA0`, `sub_821B3D70` (the lead). Not seen in play. Read in outline. |
| `sub_821AEA08` | `CarSettings::SettingMarks20` | | Movie 3: the marks (the applied values, after an apply) at the working +100 and +104 on nodes (23, 7) and (23, 8) through `SetMark` (-10 to 0). Callers `SettingPage20` and `sub_821B1DA0` (the lead). Not seen in play. Read in outline. |
| `sub_821B14D8` | `CarSettings::SettingPage21` | | Page 21, one setting (item 0 the value, 1 apply; the working +96, applied +212, stock +328; the car's bytes +93 (the applied value / 5; probably the rear's torque share in percent)): `Menu::Step`; moved → transition 94, page 21, sound 4; `SettingAdjust21` changed it → transition 95, sound 4; accept on a value moves the menu's cursor to apply (its +4 the item before; the moved transition); on apply by +732: 0 the working into the applied (`SettingMarks21`) and leaving, 1 the applied into the working, 2 the stock into the working (`SettingBars21`); back: the applied into the working, `SettingBars21`; leaving: transition 0 then 96, page 4, sound 2. A 96-byte frame (r31 saved). Only caller `Draw`. Not seen in play. Read in outline (compared with `SettingPage9`). |
| `sub_821AA1B0` | `CarSettings::SettingAdjust21` | | For `SettingPage21`: with the menu's side (+36) -1 or 1, the value (item 0) +96 by 1 within 0 to 100 (clamped); on the apply item cycles +732 (0 to 2; +736 the one before). Returns 1 when the value changed. Leaf. Not seen in play. Read in outline (its fields and bounds; compared with `SettingAdjust9`). |
| `sub_821AEA88` | `CarSettings::SettingBars21` | | Movie 3: the working +96 on node (25, 5) through `SetBarPercent` (0 to 100). Callers `SettingPage21` and the transitions `sub_821B1DA0`, `sub_821B3FD8` (the lead). Not seen in play. Read in outline. |
| `sub_821AEAE8` | `CarSettings::SettingMarks21` | | Movie 3: the marks (the applied values, after an apply) at the working +96 on node (25, 5) through `SetMarkSingle` (0 to 100). Callers `SettingPage21` and `sub_821B1DA0` (the lead). Not seen in play. Read in outline. |
