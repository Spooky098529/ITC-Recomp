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

}  // namespace garage_screen
