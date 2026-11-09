// The garage screen natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/garage-screen.md, the globals in docs/known-data.md):
// the type 2 mode's screen machine at its block's +0x23018, UI movie 5, the
// list of the player's garage cars over the stand. Plain C++ over guest
// memory (the machine, the page object, the two menu lists, the screen's
// data and the movie are shared with the recompiled code), with the
// originals' loads and stores in their order and their frames' words. `sp`
// is the caller's stack pointer: the back chain of the original's frame and
// its locals are written below it as the original leaves them; its register
// saves aren't. Adapters: src/hooks/garage_screen.cpp (behind
// --native_garage_screen), register steps shared with the harness in
// src/hooks/garage_screen.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0, or all eight) by address, to the callees' overrides
// (the functions never seen in play, ReloadStandCar, SetScrollArrows,
// PutCarOnStand and the busy marker's three, to the originals: their
// recompiled paths kept). UiMovies::Find, UiMovies::IsLoaded,
// UiMovie::FindNode, UiMovie::Child, MenuList::Selection, SaveData::Of and
// Params::GetCarSpec are their plain C++. The varargs UiMovie::SetText and
// SetNodeText calls pass 0 for the arguments the call sites leave unset (the
// user's call for the parking area, 2026-10-30: only dead words of the
// caller's parameter area differ).

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace garage_screen {

using Calls = race_hud_object::Calls;

// The globals: the page object (28 bytes: +0 the page UpdatePage runs, -1
// while a transition runs; +4 the page after it; +8 the page before; +12
// the transition Update runs, -1 none; +16 the one queued after it; +20;
// +24 the transition's started word), the two menu lists (72 bytes each:
// the car list, actions or view options; the car list kept beside the
// actions) and the screen's data (104 bytes: +0 the header set, +4 the
// career's pick, +24 the money, +28 the day, +32 the weekday, +36 the save
// data, +44 the selected row and +48 the list's scroll, +72 the stand
// shown, +76 the stand's model, +84 the busy marker shown). The stand's
// holder ([0x826D83E0]: +0 the garage stage's holder, whose +0 is the
// stage).
constexpr uint32_t kPages = 0x826D8420;
constexpr uint32_t kList = 0x826D8424;
constexpr uint32_t kList2 = 0x826D8428;
constexpr uint32_t kData = 0x826D842C;
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kMovie = 5;
constexpr uint32_t kAsset = 22579;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kSetNumberText = 0x8211C740;
constexpr uint32_t kSetChildNameText = 0x8211D930;
constexpr uint32_t kScrollText = 0x8211C570;
constexpr uint32_t kScrollReset = 0x8211C5F8;
constexpr uint32_t kListInit = 0x8220BAC0;
constexpr uint32_t kListSetSelection = 0x8220BC30;
constexpr uint32_t kListFit = 0x8220BC40;
constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kEmpty = 0x82106540;  // the empty shared function
constexpr uint32_t kIsBusy = 0x82179BE0;        // GarageStage::IsBusy
constexpr uint32_t kStageDraw = 0x8217FE60;     // GarageStage::Draw
constexpr uint32_t kBlockOf = 0x8215AAD0;       // CarModels::BlockOf
constexpr uint32_t kReloadModel = 0x8217F880;   // GarageStage::ReloadModel
constexpr uint32_t kStageCommand = 0x82180108;  // GarageStage::Command
// The screen's own, never seen in play (their originals).
constexpr uint32_t kSetScrollArrows = 0x82193288;
constexpr uint32_t kReloadStandCar = 0x82193B10;
constexpr uint32_t kPutCarOnStand = 0x82193BA8;
constexpr uint32_t kShowBusy = 0x82193C58;
constexpr uint32_t kHideBusy = 0x82193CE0;
constexpr uint32_t kHideBusyNow = 0x82193D48;

// --- The movie 5 helpers (helpers.cpp) ---

// sub_82193180 GarageScreen::SetCarRow (r3 a row node, r4 a garage car):
// the row's child 3 +108 = the maker's logo frame by the car's id (+12);
// children 5, 6 and 7 the maker's and the car's names (SetNodeText 1934 +
// the spec's byte +769, 1646 + id, 1718 + id). The last SetNodeText's r3.
// 128-byte frame.
uint64_t SetCarRow(uint8_t* base, uint32_t node, uint32_t car, uint32_t sp, const Calls& calls);
// sub_82194CD8 GarageScreen::FillRows: rows 0 to 3 (nodes (6, 5 + i)) from
// the garage cars at the list's scroll + i while below the garage's count
// (the save's +6328). The last SetCarRow's r3, or UiMovies::Find's (the
// pair's address). 144-byte frame.
uint64_t FillRows(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821939A0 GarageScreen::SetPrompts (r3 to r8 three pairs): node (16,
// k) shown with its two children's frames when both of pair k aren't -1,
// else hidden. The last FindNode's r3. 176-byte frame.
uint32_t SetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp);
// sub_82193A90 GarageScreen::SetHelpLine (r3 a string id): movie 5's (17,
// 0) text string `id` (UiMovie::SetText, its unset varargs 0),
// ScrollText(17, 0, 1) and the scroll reset sub_8211C5F8. The reset's r3.
// 112-byte frame.
uint64_t SetHelpLine(uint8_t* base, uint64_t id, uint32_t sp, const Calls& calls);
// sub_821956C0 GarageScreen::SetUpLists: both menu lists over the garage's
// cars (MenuList::Init), the first's selection and scroll from the data's
// +44 and +48, the rows shown, the scroll arrows (SetScrollArrows, the
// original), FillRows, the current car's marker, SetPrompts(0, 0, 1, 1, -1,
// -1) and SetHelpLine(558). SetHelpLine's r3. 176-byte frame.
uint64_t SetUpLists(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The transitions (transitions.cpp; r3 the started word's address, the
// page object's +24) ---
//
// sub_82195BA0 GarageScreen::Transition0 (the opening): the first call the
// animations 131, 128, 125, 126, 45 and 49, the data's +44 and +48 = 0 and
// SetUpLists; then UiMovie::Run. 1 when Run answers 0. 112-byte frame.
uint64_t Transition0(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82192D60 GarageScreen::Transition1 (the list's highlight): the first
// call 67 + the selection and 77 played and looped; then Run. 1. 128-byte
// frame.
uint64_t Transition1(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82192E20 GarageScreen::Transition3 (the closing): the first call 67 +
// the selection, 77 and 128 finished and the fade out; then Run. 1 when the
// fades' counts (0x826A50DC, 0x826A50E4) are both 0. 112-byte frame.
uint64_t Transition3(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);

// --- The car menu's, the view's and the plate's transitions never seen in
// play (menu.cpp; r3 the started word's address; Transition11 and
// Transition15 stay the originals: their finish stores UiMovie::Run's r8
// leftover into the started word, live state) ---
//
// sub_82194F30 Transition2 (the list moved): the highlight moved, and when
// the list scrolled the rows refilled (FillRows), the scroll's animations,
// the row scrolled in (SetCarRow in row 9), the current car's marker and
// the arrows (SetScrollArrows). 1. 176-byte frame.
uint64_t Transition2(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82195D10 Transition4 (into the car's menu): 67 + the row and 77
// finished, 132 played, the data's +52 and +56 = 0, SetUpActions; once Run
// answers 0, (7, 5) and (7, 6) colored 2 or 0 by whether it's the current
// car. 1 when Run answers 0. 128-byte frame.
uint64_t Transition4(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82192EF8 Transition5 (the action's highlight): 80 + the action played
// and looped (not actions 0 and 1 on the current car), the data's +72 = 1.
// 1. 128-byte frame.
uint64_t Transition5(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82194680 Transition6 (the action moved): the highlights, 90 and 91,
// the marker (SetActionMarker, or 90 or 91 by the move), the help line. 1.
// 224-byte frame.
uint64_t Transition6(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_821951A8 Transition7 (another car): the second list's selection and
// scroll into the data's +44 and +48, the highlights, ReloadStandCar, the
// car copied (+160) and ShowCarSpecs, the current car's mark and colors,
// the help line. 1. 2,192-byte frame.
uint64_t Transition7(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82195E30 Transition8 (back to the list): the highlight finished, 133
// played, the data's +72 = 0, SetUpLists; once Run answers 0 the page
// object to transition 1 then page 0 (+24 = 0). 0. 128-byte frame.
uint64_t Transition8(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82195410 Transition9 (into the view) and sub_82195530 Transition10
// (back to it from its options): the highlight finished, the view's
// animations, SetUpView. 1 when Run answers 0. 112-byte frames.
uint64_t Transition9(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t Transition10(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_821955E0 Transition12 (into the view's options): 143 played,
// SetUpOptions; once Run answers 0 with the data's +80 set, node (2, 1)'s
// children 6 and 7 colored 2. 1 when Run answers 0. 128-byte frame.
uint64_t Transition12(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82192FD0 Transition13 (the option's highlight) and sub_821948B0
// Transition14 (the option moved; 2,160-byte frame): the highlight (26 and
// 27 for options 1 and 2 with the data's +80 set), Transition14 the marker
// and the help line too. 1. 128-byte frame.
uint64_t Transition13(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t Transition14(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_821930D8 Transition16 (to the plate editor) and sub_82196170
// Transition17 (back from it: the data's +52 = 2, SetUpActions): 146 or
// 147 played, nodes (3, 1) and (3, 2) +108 = 1 and 3 or 3 and 1. 1 when Run
// answers 0. 128-byte frames.
uint64_t Transition16(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t Transition17(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// Update's table: the transition at `target` run natively (its plain C++,
// r3 into `*r3`) when it's one of the native ones; false otherwise.
bool RunTransition(uint8_t* base, uint32_t target, uint32_t started, uint32_t sp,
                   const Calls& calls, uint64_t* r3);
// sub_821958D0 GarageScreen::SetUpActions: the first list as the five
// actions, the second the car list (its selection, scroll, Fit), the
// action's selection (+52) and marker, the actions' texts, the car copied
// and ShowCarSpecs, SetPrompts(0, 0, 1, 1, -1, -1), the current car's
// mark, the stage's +6052 = 1, the help line, nodes (-1, 0), (-1, 13) and
// (-1, 14) shown unless the garage holds one car. The last node (r3).
// 2,192-byte frame.
uint64_t SetUpActions(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82194B20 GarageScreen::SetUpOptions: the first list as the view's
// four options, the marker, the options' texts (node (2, 1)'s children 5 to
// 8), the data's +80 (the car's spec's byte +771 is 1), the help line.
// SetHelpLine's r3. 2,160-byte frame.
uint64_t SetUpOptions(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The pages seen in play (pages.cpp; r3 the mode) ---
//
// sub_82194078 GarageScreen::CarListPage (page 0): MenuList::Update on the
// first list; when it didn't move, accept (the selection and scroll into
// the data's +44 and +48, ReloadStandCar the original, transition 4 then 5
// to page 1, sound 0) or back (transition 3 to page 4, sound 2); when it
// moved, transition 2 on page 0, sound 4. MenuSound::Play's r3, or
// Cancelled's (0). 96-byte frame.
uint64_t CarListPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82192C18 GarageScreen::LeavePage (page 4): the stage's +6052 = 1,
// message 22 posted on `mode`'s ring. A leaf (r3 left).
void LeavePage(uint8_t* base, uint32_t mode);
// sub_82194EA8 GarageScreen::UpdatePage (r3 the machine, r4 the mode): the
// page from its table (112-byte frame, no bounds check) with r3 = the mode
// and r5 as given. The page's r3, or `r3` with no page (-1).
uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint64_t mode, uint64_t r5, uint32_t sp,
                    const Calls& calls);

// --- The pages never seen in play and the reply handlers (view.cpp) ---
//
// sub_821941A8 GarageScreen::ActionsPage (page 1, r3 the mode): both lists
// updated; a move of the actions (transition 6) or of the car list
// (transitions 7 then 5); an accept by the action (0 the use question,
// message 34; 1 the sale question, 36; neither for the current car, sound
// 3; 2 the plate editor, 28, transition 16; 3 the view, transition 9; 4 the
// spec sheet, 24) with SetPrompts; a cancel back to the list (transition
// 8). MenuSound::Play's r3, or Cancelled's (0), or the action (above 4).
// 144-byte frame.
uint64_t ActionsPage(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_82191DE0 GarageScreen::ViewPage (page 2) and sub_82192030
// ViewOptionsPage (page 3, r3 the mode): the player's mapper's buttons
// (slot 12 fired, slot 8 held): back (transition 11 or 15), the options
// (12 then 13) or the option's accept (the toggles, transition 15), then
// the stand's buttons (the camera's commands 4 and 5, the turns, the
// camera presets). The last call's r3 (the mode with none). 112-byte
// frames.
uint64_t ViewPage(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
uint64_t ViewOptionsPage(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_82193E38 OnSpecSheetClosed (5, 26): page 1, SetPrompts(0, 0, 1, 1,
// -1, -1) (a tail call from `sp`). Its r3.
uint64_t OnSpecSheetClosed(uint8_t* base, uint32_t sp);
// sub_821928F8 OnPlateCancelled (5, 30; r4 the mode): the data's +60 = 0,
// message 32. A leaf (r3 left).
void OnPlateCancelled(uint8_t* base, uint32_t mode);
// sub_82193E80 OnPlateDone (5, 29; r4 the mode): the data's +60 = 1,
// SetPlateQuestion, message 34, the data's +68 = 0. SetPlateQuestion's r3.
// 112-byte frame.
uint64_t OnPlateDone(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82193F18 OnPlateEditorLeft (5, 31): unless the data's +60 is set,
// transition 17 to page 1 and SetPrompts (a tail call). Its r3, or `r3`.
uint64_t OnPlateEditorLeft(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82192BA8 OnNo (5, 39 and 5, 41; r4 the mode): the data's +64 = 0;
// with +68 0, message 32. A leaf (r3 left).
void OnNo(uint8_t* base, uint32_t mode);
// sub_82193F80 OnDialogDone (5, 42): answered yes (+64), transition 8 to
// page 0; else SetPrompts and by the question (+68): 0 transition 17 to
// page 1, 1 or 2 page 1 at once. SetPrompts' r3, or `r3`. 96-byte frame.
uint64_t OnDialogDone(uint8_t* base, uint64_t r3, uint32_t sp);

// --- Update (update.cpp) ---
//
// sub_82196230 GarageScreen::Update (r3 the machine, r4 the mode; neither
// read): the empty sub_82106540(100, 30, 0, 0x8200A3C8); the running
// transition from its table in the frame (no bounds check; the originals
// for those not seen in play) or, with none, UiMovie::Run unless on page 4;
// a finished one starts the queued one or goes to the page after; then,
// while the stand is shown (the data's +72), the busy marker (ShowBusy and
// HideBusy, the originals) and the selected garage car's model reloaded,
// the car put on the stand (PutCarOnStand, the original) and preset 1, or
// the stand drawn; without it, the busy marker hidden (HideBusyNow, the
// original). The last call's r3 (or the movie). 272-byte frame.
uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The machine's handlers (handlers.cpp) ---
//
// sub_82191C58 GarageScreen::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the two menu lists (72 each) and the data (104)
// VirtualAlloc'ed into their globals with their first words; +9604 = 1 or 9
// by +9612. The last VirtualAlloc's r3. 112-byte frame.
uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82191618 GarageScreen::OpenMovie: UiMovies::Open(5, 22579) (a tail
// call, from `sp` with `lr`). Its r3.
uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82191D70 GarageScreen::WaitForLoad (r3 the machine): +9608 = 1 once
// movie 5 is loaded, then +9604 = 9 or 4 by +9612. r3 IsLoaded's when
// called, else the machine (`r3`). 96-byte frame.
uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82191BB0 GarageScreen::ReadSave: player 0's save data's career pick
// (+8, 20 bytes), money (+6116), weekday and day (+6100, at most 999) into
// the data's +4 to +32, the save at +36. SaveData::Of's r3. 96-byte frame.
uint64_t ReadSave(uint8_t* base, uint32_t sp);
// sub_82192C80 GarageScreen::SetHeader: movie 5's pose reset, the header's
// texts (the pick's name, the money, string 8 with the day; SetText's
// unset varargs 0) and the weekday's frame; the data's +72, +76 = 0, +0 =
// 1. The weekday node (r3). 112-byte frame.
uint64_t SetHeader(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82193DC0 GarageScreen::Start: ReadSave and SetHeader (their plain
// C++), the page object to page 0 after transitions 0 and 1, the fade in.
// ScreenFades::FadeIn's r3. 96-byte frame.
uint64_t Start(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821927E8 GarageScreen::Unload (r3 the machine): UiMovie::FinishAll
// (movie 5); with +9616, UiMovies::Delete(5) and +9608 = 0. The last call's
// r3. 112-byte frame.
uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82192850 GarageScreen::Close (r3 the machine, r4 the mode): +9604 =
// 7, UiMovie::FinishAll(movie 5), UiMovies::Delete(5), message 4. Delete's
// r3. 112-byte frame.
uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_821915B0 GarageScreen::Destroy: the page object, the first list and
// the data VirtualFree'd and their globals 0 (the second list kept, as the
// original). The last VirtualFree's r3. 112-byte frame.
uint64_t Destroy(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The texts and markers never seen in play (texts.cpp) ---
//
// sub_82193288 GarageScreen::SetScrollArrows (r3 the list's scroll, r4 the
// count): node (6, 3) +80 (up) = scroll != 0; node (6, 4) +80 (down) = 1 at
// scroll 0, else scroll + 4 != count. The down node (r3). 128-byte frame.
uint64_t SetScrollArrows(uint8_t* base, uint32_t scroll, uint32_t count, uint32_t sp);
// sub_821936B0 GarageScreen::SetActionMarker (r3 the action): node (7, 0)
// +16 = the action as a float (through the frame's +80). The node (r3).
// 112-byte frame.
uint64_t SetActionMarker(uint8_t* base, uint32_t action, uint32_t sp);
// sub_82193720 GarageScreen::SetOptionMarker (r3 the option): node (2, 1)'s
// child 0 +16 = the option as a float. The child (r3). 112-byte frame.
uint64_t SetOptionMarker(uint8_t* base, uint32_t option, uint32_t sp);
// sub_82193798 GarageScreen::ShowSpecTexts (r3 a record of figures):
// sub-scene 18's ten texts (as CarDealer::SetSpecTexts on movie 5). The
// last SetText's r3. 112-byte frame.
uint64_t ShowSpecTexts(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls);
// sub_82193910 GarageScreen::ShowCarNames (r3 a car id, r4 its maker):
// (-1, 19) string 1934 + maker, (-1, 20) 1646 + id, (-1, 21) 1718 + id. The
// last SetText's r3. 112-byte frame.
uint64_t ShowCarNames(uint8_t* base, uint64_t id, uint64_t maker, uint32_t sp,
                      const Calls& calls);
// sub_82193320 GarageScreen::SetPlateQuestion: movie 16's cursor word
// +40960 = 0; (-1, 0) string 96, (1, 3) 2 and (1, 4) 3 (SetNodeText, its
// unset varargs 0). The last SetNodeText's r3. 128-byte frame.
uint64_t SetPlateQuestion(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The stand's helpers never seen in play (stand.cpp) ---
//
// sub_82192770 GarageScreen::CopyGarageCar (r3 out, r4 a garage list, r5
// an index): the 1,968-byte car at the list's index (walked from its head)
// copied to `out` (memcpy). `out` (r3). 96-byte frame.
uint64_t CopyGarageCar(uint8_t* base, uint64_t out, uint32_t garage, uint32_t index,
                       uint32_t sp, const Calls& calls);
// sub_82193C58 GarageScreen::ShowBusy, sub_82193CE0 HideBusy and
// sub_82193D48 HideBusyNow: movie 5's busy marker (122 then 120 played and
// looped; 120 finished and 123 played; and 123 finished too). The last
// call's r3. 112-byte frames.
uint64_t ShowBusy(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t HideBusy(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t HideBusyNow(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82191B20 GarageScreen::ResetStand: commands 1 and 15 (the data's +88
// = 0 between), 3 when +92 is 1 and 5 when +96 is 1 (each word 0 then). The
// last Command's r3. 112-byte frame.
uint64_t ResetStand(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82194AC8 GarageScreen::SetUpView: command 58, the stage's +6052 = 0,
// SetHelpLine(553) (its plain C++). Its r3. 96-byte frame.
uint64_t SetUpView(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821918D0 GarageScreen::ToggleTurn, sub_82191920 ToggleDoors and
// sub_82191970 ToggleDoors2: the data's +88, +92 or +96 flipped (set: 0 and
// command 1, 3 or 5; else 1 and 0, 2 or 4), the command a tail call from
// `sp` with `lr`. Its r3.
uint64_t ToggleTurn(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t ToggleDoors(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t ToggleDoors2(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821919C0 GarageScreen::TurnLeft and sub_82191A18 TurnRight: the
// data's +88 = 0, command 1, then 9 or 10. The last Command's r3. 96-byte
// frames.
uint64_t TurnLeft(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t TurnRight(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The car objects never seen in play (cars.cpp) ---
//
// sub_82193B10 GarageScreen::ReloadStandCar: the save's garage list (98,612
// bytes) copied to its frame's +2048, the selected car (the data's +44 +
// +48) out of it (CopyGarageCar, its plain C++) to +80, its model
// (CarModels::BlockOf) reloaded on the stand (GarageStage::ReloadModel)
// into the data's +76. ReloadModel's r3. A 100,688-byte frame (__chkstk's
// probe dropped).
uint64_t ReloadStandCar(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82193BA8 GarageScreen::PutCarOnStand: the garage list copied, the
// selected car out of it, a car object from it at its frame's +80
// (Car::Init, 1,884 bytes, +3888 |= 1, Car::Refresh), then
// GarageStage::SetCar(holder, car, -1). SetCar's r3. A 104,592-byte frame.
uint64_t PutCarOnStand(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82194D90 GarageScreen::ShowCarSpecs (r3 a garage car): a car object
// (+3040), a car data (+1184: CarData::CarData, Vehicle::SetupFromCar) and
// its power curve (+112: PowerCurve::FromEngine, ShiftPeak) in its frame,
// the figures' record at +80, ShowSpecTexts and ShowCarNames (their plain
// C++). ShowCarNames' r3. A 6,976-byte frame.
uint64_t ShowCarSpecs(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls);
// sub_821933D0 GarageScreen::SetUseQuestion (r3 a garage car): movie 16's
// cursor 0, its question (string 97 with the maker's and the car's names),
// yes and no (SetNodeText), a car object at its frame's +96 for the maker.
// The last SetNodeText's r3. A 4,064-byte frame.
uint64_t SetUseQuestion(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls);
// sub_821934D0 GarageScreen::SetSaleQuestion (r3 a garage car): the data's
// +100 = SalePrice (its plain C++), movie 18's cursor 1 and its texts (25,
// the maker, the names, the price by SetNodeNumberText, 527, 435, yes and
// no). The last SetNodeText's r3. A 4,112-byte frame.
uint64_t SetSaleQuestion(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls);
// sub_82191810 GarageScreen::SetUpPlateEditor (r3 a garage car): a car
// object at its frame's +80; the plate editor's data ([0x826D8438]) +48 =
// 0, +44 = 1 when the car's sizes are small (record +8 <= 2000, +76 <=
// 4700, +80 <= 2000, +84 <= 1700), +24 to +43 the car's +1568 to +1587. The
// record (r3). A 4,000-byte frame.
uint64_t SetUpPlateEditor(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls);
// sub_82191628 GarageScreen::SetUpSpecSheet (r3 a garage car; its piece
// sub_82191768 merged): a car object at its frame's +1152, the spec sheet's
// vehicle ([0x826D846C] + 112: Vehicle::SetupFromCar), its curve (+80:
// FromEngine, ShiftPeak), then the sheet's words (+16 on). ShiftPeak's r3.
// A 5,104-byte frame.
uint64_t SetUpSpecSheet(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls);
// sub_82191A70 GarageScreen::CameraA and sub_82191AC8 CameraB: the data's
// +88 = 0, command 1, then GarageStage::CameraCommand(holder, 0 or 1) (its
// plain C++). Its r3. 96-byte frames.
uint64_t CameraA(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t CameraB(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The value and the price (prices.cpp) ---
//
// sub_82192378 GarageScreen::CarValue (r3 a garage car): f1, from 0.0, plus
// the car's +1900 (a count, as a float) times the weight of each of its
// part bits set (454 bits from +1904; the weights' table around
// 0x8200A260). A leaf: the count's doubleword at sp - 112.
double CarValue(uint8_t* base, uint32_t car, uint32_t sp);
// sub_82192690 GarageScreen::SalePrice (r3 a garage car): (CarValue x
// [0x82002208] + +1900 x [0x8200A348]) x the factor by the float +1892's
// threshold, truncated, at least 50. r3. 112-byte frame.
uint64_t SalePrice(uint8_t* base, uint32_t car, uint32_t sp);

// --- The machine's set-up (init.cpp) ---
//
// sub_82196520 GarageScreen::InitStates: 20 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 1424;
uint32_t InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace garage_screen
