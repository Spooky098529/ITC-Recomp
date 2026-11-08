// The type 2 mode's screens natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type2-screens.md, the globals in
// docs/known-data.md): four of the screen machines in the type 2 mode's
// block (the hub at +0, the spec sheet at +56624, the build screen's machine
// at +0x1BF20, the license plate editor at +0x20A70). Plain C++ over guest
// memory (the machines, their page objects, menus and data, and the movies
// are shared with the recompiled code), with the originals' loads and
// stores in their order and their frames' words. `sp` is the caller's stack
// pointer: the back chain of the original's frame and its locals are
// written below it as the original leaves them; its register saves aren't.
// Adapters: src/hooks/type2_screens.cpp (behind --native_type2_screens),
// register steps shared with the harness in src/hooks/type2_screens.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0) by address, to the callees' overrides. UiMovies::Find,
// UiMovies::IsLoaded, UiMovie::FindNode, MenuList::Selection and the game's
// rand are their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace type2_screens {

using Calls = race_hud_object::Calls;

// The machines' words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The license plate editor's globals: the page object (28 bytes), the menu
// list (72) and the data (76).
constexpr uint32_t kPlatePages = 0x826D8430;
constexpr uint32_t kPlateList = 0x826D8434;
constexpr uint32_t kPlateData = 0x826D8438;
constexpr uint32_t kPlateMovie = 26;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kGetLocalTime = 0x822968B8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kClearText = 0x8211C470;
constexpr uint32_t kListInit = 0x8220BAC0;
constexpr uint32_t kListSetSelection = 0x8220BC30;
constexpr uint32_t kListFit = 0x8220BC40;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kListPressed152 = 0x8220BE70;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kEmpty = 0x82106540;  // the empty shared function
constexpr uint32_t kMemset = 0x825A71D0;
constexpr uint32_t kMenuListRelease = 0x8220BBB0;

// The spec sheet's globals: the page object (28 bytes), the menu (68) and
// the data (1,968).
constexpr uint32_t kSheetPages = 0x826D8464;
constexpr uint32_t kSheetMenu = 0x826D8468;
constexpr uint32_t kSheetData = 0x826D846C;
constexpr uint32_t kSheetMovie = 9;
// The build screen's globals (build_screen.h's): the transition machine
// (28 bytes), the two menu lists (72 each) and the screen data (0x3266C).
constexpr uint32_t kBuildPages = 0x826D843C;
constexpr uint32_t kBuildList = 0x826D8440;
constexpr uint32_t kBuildGroupList = 0x826D8444;
constexpr uint32_t kBuildScreen = 0x826D8448;
constexpr uint32_t kBuildScreenBytes = 0x3266C;
constexpr uint32_t kBuildMovie = 6;

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id);

// --- Shared by four type 2 machines (plate_editor.cpp) ---

// sub_821D6A20 Screen::LeaveToHub (r3 the machine, r4 the mode): message 6
// posted, +9604 = 9. A leaf: r3 the machine.
void LeaveToHub(uint8_t* base, uint32_t machine, uint32_t mode);

// --- The license plate editor's set-up and teardown (plate_editor.cpp) ---

// sub_82198210 PlateEditor::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the menu list (72) and the data (76) VirtualAlloc'ed
// into their globals with their first words; +9604 = 1 or 9 by +9612. The
// last VirtualAlloc's r3. 112-byte frame.
uint64_t PlateCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82198180 PlateEditor::OpenMovie: UiMovies::Open(26, 22556) (a tail
// call). Its r3.
uint64_t PlateOpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821982F0 PlateEditor::WaitForLoad (r3 the machine): +9608 = 1 once
// movie 26 is loaded, then +9604 = 9 or 4 by +9612. r3 IsLoaded's when
// called, else the machine (`r3`). 96-byte frame.
uint64_t PlateWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82198498 PlateEditor::Close (r3 the machine, r4 the mode): +9604 = 7,
// UiMovie::FinishAll(movie 26), UiMovies::Delete(26), message 4. Delete's
// r3. 112-byte frame.
uint64_t PlateClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls);
// sub_82198118 PlateEditor::Destroy: the three blocks VirtualFree'd, the
// globals 0. The last VirtualFree's r3. 112-byte frame.
uint64_t PlateDestroy(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82198190 PlateEditor::OnMessage32: the page object to page 4 after
// transition 4. A leaf.
void PlateOnMessage32(uint8_t* base);
// sub_82198988 PlateEditor::LeavePage (r3 the mode): the data's +72 = 9,
// message 31. A leaf.
void PlateLeavePage(uint8_t* base, uint32_t mode);
// sub_821981C8 PlateEditor::CharLineKind (r3 seven words): 0 when the
// data's +56 is 0; 1 when the word before +56 isn't 10; else 2 when +56 is
// 3, 0 otherwise. A leaf.
uint32_t PlateCharLineKind(uint8_t* base, uint32_t chars);
// sub_82198360 PlateEditor::RandomPlate (r3 the class, not read; r4 the
// plate): bytes +1 to +11 from rand() x the local time's milliseconds
// (GetLocalTime into the frame's +80, through its override). r3 the plate.
// 128-byte frame.
uint64_t PlateRandom(uint8_t* base, uint32_t plate, uint32_t sp, const Calls& calls);
// sub_82198420 PlateEditor::LoadPlate: with the data's +48, RandomPlate(+44,
// data + 24) (its plain C++); then the 20 bytes at +24 copied to +4. r3 as
// the original leaves it (`r3`, or RandomPlate's). 96-byte frame.
uint64_t PlateLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821989E8 PlateEditor::ResetMovie: the data's +64 = 5, +68 = 4;
// UiMovie::ResetPose(movie 26) (through its override). Its r3. 96-byte
// frame.
uint64_t PlateResetMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82199BB8 PlateEditor::Start: LoadPlate and ResetMovie (their plain
// C++), the page object to page 0 after transitions 0 and 1. ResetMovie's
// r3. 96-byte frame.
uint64_t PlateStart(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The license plate editor's movie helpers (plate_editor.cpp) ---

// sub_82199898 PlateEditor::ShowRegion: node (2, 0) +108 = `region`. The
// node (r3). 112-byte frame.
uint32_t PlateShowRegion(uint8_t* base, uint32_t region, uint32_t sp);
// sub_821998F0 PlateEditor::ShowDigits (r3 three words): node (2, 2)'s
// children: +108 the digit and shown, hidden for 10. r3 the last child, or
// node (2, 2) with none. 144-byte frame.
uint32_t PlateShowDigits(uint8_t* base, uint32_t digits, uint32_t sp);
// sub_82199990 PlateEditor::ShowChars (r3 seven words): node (2, 1)'s
// children 0 to 6: +108 the character and shown below 37, else hidden. r3
// the last child. 144-byte frame.
uint32_t PlateShowChars(uint8_t* base, uint32_t chars, uint32_t sp);
// sub_82199AA8 PlateEditor::MarkField: node (1, 0)'s children shown only
// for `field`. r3 the last child, or node (1, 0) with none. 128-byte frame.
uint32_t PlateMarkField(uint8_t* base, uint32_t field, uint32_t sp);
// sub_821992D8 PlateEditor::FinishRegionAnim: named 2 + the selection x 3
// + the column (+52) finished (UiMovie::FinishNamed through its override).
// Its r3. 112-byte frame.
uint64_t PlateFinishRegionAnim(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82199410 PlateEditor::FinishDigitAnim: named 15 + the selection x 4 +
// the column finished. Its r3. 112-byte frame.
uint64_t PlateFinishDigitAnim(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The license plate editor's cells and menus (plate_cells.cpp) ---
//
// The text setters (UiMovie::SetText, SetNodeText, ClearText) and the menu
// list's calls (MenuList::Init, SetSelection, Fit) through their overrides;
// SetText's and SetNodeText's unset varargs 0 (the user's call for the
// parking area, 2026-10-30).

// sub_821990D0 PlateEditor::ShowRegionCells: nodes (3, 0)'s and (3, 2)'s
// children for the rows from the list's scroll (+4): shown with string
// 5411 + 3 x the scroll + i (SetNodeText on the first), or hidden from 14
// on; the arrows (-1, 8) and (-1, 9) by the scroll and the data's +64. r3
// the down arrow. 176-byte frame.
uint32_t PlateShowRegionCells(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82199350 PlateEditor::ShowDigitCells: node (4, 0)'s children:
// SetText(8, i) for the first ten, ClearText for the rest; the arrows
// hidden. r3 the down arrow. 128-byte frame.
uint32_t PlateShowDigitCells(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82199488 PlateEditor::ShowCharCells: as ShowRegionCells on nodes
// (5, 0) and (5, 2), ten a row, strings 5425 +, hidden from 38 on; the
// arrows by the scroll and the data's +68. r3 the down arrow. 176-byte
// frame.
uint32_t PlateShowCharCells(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82199688 PlateEditor::ShowCharLine (r3 0, 1 or 2): node (4, 2)'s
// children colored (sub_8211A630's plain C++): 0, child 0 color 2 and the
// rest 0; 1, child 10 color 2; 2, children 0 and 10. r3 the movie after a
// color, else node (4, 2). 128-byte frame.
uint32_t PlateShowCharLine(uint8_t* base, uint32_t kind, uint32_t sp);
// sub_82199A20 PlateEditor::ShowPlate (r3 a plate): its bytes as words in
// the frame (+80 the digits, +96 the characters), ShowRegion, ShowDigits
// and ShowChars (their plain C++). ShowChars' r3. 144-byte frame.
uint32_t PlateShowPlate(uint8_t* base, uint32_t plate, uint32_t sp);
// sub_82199C10 PlateEditor::SetUpRegionMenu, sub_82199CC8
// SetUpNumberMenu and sub_82199D88 SetUpCharsMenu: MenuList::Init over the
// rows (the data's +64, 3 or the data's +68; 3 shown; the actions 154,
// 153, 155, 156, 159, 158), the field's row selected (SetSelection, Fit 1)
// and its column at the data's +52, MarkField and the cells (their plain
// C++); the region's HelpLine::Set 568 (car_dealer's plain C++). The last
// call's r3. 128-byte frames.
uint64_t PlateSetUpRegionMenu(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t PlateSetUpNumberMenu(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t PlateSetUpCharsMenu(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The license plate editor's transitions (plate_transitions.cpp) ---
//
// Each takes `step`, the address of the page object's +24 (r3): its set-up
// on the first call (the word 0, then 1), then UiMovie::Run(movie 26, 0);
// the named animations (UiMovie::PlayNamed with r5 to r8 0, LoopNamed,
// FinishNamed, FinishAll) and Run through their overrides; the plate
// editor's helpers and HelpLine::Set their plain C++. r3 1, or 1 when Run
// answers 0 where said.

// sub_8219A3A8 PlateEditor::Transition0: named 66, SetUpRegionMenu,
// ShowPlate(the data's +4). Run's answer. 112-byte frame.
uint64_t PlateTransition0(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82198A40 PlateEditor::Transition1: named 2 + the selection x 3 + the
// column, 0 and 68 played and looped; node (3, 1) +68 = the float at
// 0x82000A30. 128-byte frame.
uint64_t PlateTransition1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82198C20 PlateEditor::Transition4: the region's cell, 0 and 68
// finished, FinishAll, named 67 played. Run's answer. 112-byte frame.
uint64_t PlateTransition4(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8219A5E8 PlateEditor::Transition5: FinishRegionAnim, named 69, the
// data's +56 = 0, SetUpNumberMenu, the help line 569, ShowDigits(the three
// digits as words at the frame's +88). Run's answer. 144-byte frame.
uint64_t PlateTransition5(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82198D00 PlateEditor::Transition6: named 15 + the selection x 4 + the
// column played and looped; node (4, 1) +68 = the float at 0x82000A30.
// 128-byte frame.
uint64_t PlateTransition6(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8219A820 PlateEditor::Transition8: the digit's animation finished,
// SetUpNumberMenu, ShowDigits (the frame's +88). 144-byte frame.
uint64_t PlateTransition8(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8219AA00 PlateEditor::Transition10: FinishDigitAnim, the data's +56 =
// 0, SetUpCharsMenu, named 71, the help line 571; when Run answers 0,
// ShowCharLine(the data's +60). Run's answer. 128-byte frame.
uint64_t PlateTransition10(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82198DC8 PlateEditor::Transition11: named 27 + the selection x 10 +
// the column played and looped. 128-byte frame.
uint64_t PlateTransition11(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_8219AC88 PlateEditor::Transition13: the character's animation
// finished, SetUpCharsMenu, the seven characters as words at the frame's
// +96, the data's +60 = CharLineKind, ShowChars, ShowCharLine(+60).
// 160-byte frame.
uint64_t PlateTransition13(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
// sub_82198FB8 PlateEditor::Transition19: FinishAll, the root's nodes 7, 6,
// 5, 3, 4, 8, 9 hidden. 160-byte frame.
uint64_t PlateTransition19(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The license plate editor's pages and update (plate_pages.cpp) ---
//
// The pages (r3 the mode): MenuList::Update, Accepted, Cancelled and
// Pressed152 and MenuSound::Play through their overrides; the page object's
// moves and the messages inline. The last call's r3 (Pressed152's when
// nothing is pressed).

// sub_82199ED0 PlateEditor::RegionPage. 112-byte frame.
uint64_t PlateRegionPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8219A138 PlateEditor::NumberPage. 112-byte frame.
uint64_t PlateNumberPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82198540 PlateEditor::CharsPage. 112-byte frame.
uint64_t PlateCharsPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8219B050 PlateEditor::UpdatePage (r3 the machine, r4 the mode): the
// five page handlers' addresses at the frame's +80; unless the page is -1,
// the page's handler called (r3 the mode, through the guest context); then,
// when the data's +72 isn't 21, the machine's +9604 = it and +72 = 21. The
// handler's r3, else `r3`. 128-byte frame.
uint64_t PlateUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls);
// sub_8219B108 PlateEditor::Update: the empty sub_82106540 with its
// arguments; the twenty transitions' addresses at the frame's +96; the
// running transition (the page object's +12) called with the page object's
// +24's address (through the guest context), or none (a negative one counts
// as done), the queued one started on a nonzero answer; with none (-1),
// UiMovie::Run(movie 26, 0). The last call's r3 (the movie for a negative
// one). 304-byte frame.
uint64_t PlateUpdate(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The spec sheet's and the build screen's handlers (sheet_build.cpp) ---

// sub_821B8120 PowerGraphScreen::Create (r3 the machine): +9608 = 0; the
// page object, the menu and the data VirtualAlloc'ed into their globals
// with their first words; +9604 = 1 or 9 by +9612. The last VirtualAlloc's
// r3. 112-byte frame.
uint64_t SheetCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82250328 PowerGraphScreen::OpenMovie: UiMovies::Open(9, 22550) (a
// tail call). Its r3.
uint64_t SheetOpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821B8900 PowerGraphScreen::WaitForLoad (r3 the machine): while +9608
// is 0, +9608 = 1 once movie 9 is loaded; once set, the data's +0 = 1, +4 =
// 0, +16 on cleared (memset, 1,952 bytes, through its override), +9604 =
// 9. r3 IsLoaded's or memset's, else `r3`. 96-byte frame.
uint64_t SheetWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821B8320 PowerGraphScreen::Close (r3 the machine, r4 the mode): +9604
// = 7, UiMovies::Delete(9), message 4. Delete's r3. 96-byte frame.
uint64_t SheetClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls);
// sub_821B80B8 PowerGraphScreen::Destroy: the three blocks VirtualFree'd,
// the globals 0. The last VirtualFree's r3. 112-byte frame.
uint64_t SheetDestroy(uint8_t* base, uint32_t sp, const Calls& calls);

// sub_8219BD80 BuildScreen::Create (r3 the machine): +9608 = 0; the
// transition machine, the two menu lists and the screen data
// VirtualAlloc'ed into their globals with their first words; +9604 = 1 or
// 9 by +9612. The last VirtualAlloc's r3. 112-byte frame.
uint64_t BuildCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8219B950 BuildScreen::OpenMovie: UiMovies::Open(6, 22557) (a tail
// call). Its r3.
uint64_t BuildOpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_8219BE98 BuildScreen::WaitForLoad (r3 the machine): +9608 = 1 once
// movie 6 is loaded, then +9604 = 9 or 4 by +9612. r3 IsLoaded's when
// called, else `r3`. 96-byte frame.
uint64_t BuildWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8219C298 BuildScreen::Unload (r3 the machine): UiMovie::FinishAll(movie
// 6); with +9616, UiMovies::Delete(6) and +9608 = 0. The last call's r3.
// 112-byte frame.
uint64_t BuildUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8219C300 BuildScreen::Close (r3 the machine, r4 the mode): +9604 = 7,
// FinishAll(movie 6), Delete(6), message 4. Delete's r3. 112-byte frame.
uint64_t BuildClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls);
// sub_8219B8C8 BuildScreen::Destroy: MenuList::Release(the list), the four
// blocks VirtualFree'd, the globals 0 but the group list's (left as it was).
// The last VirtualFree's r3. 112-byte frame.
uint64_t BuildDestroy(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219C100 BuildScreen::SaveBuildStates: player 0's save + 0x19A00 + 12i
// = the build's +36 (the screen's + 0x2C7A8 + 52i) for the 400 builds. r3
// the save data. 96-byte frame.
uint32_t BuildSaveBuildStates(uint8_t* base, uint32_t sp);
// sub_8219C3A8 BuildScreen::LeavePage (r3 the mode): GarageStage::SetDark(the
// stage, 0) (build_screen's plain C++), SaveBuildStates, message 20. r3 the
// save data. 96-byte frame.
uint32_t BuildLeavePage(uint8_t* base, uint32_t mode, uint32_t sp);
// sub_8219B960 BuildScreen::DetailsPage: MenuList::Update on the list; when
// it isn't moving, accepted (sound 0) or cancelled (sound 2) back with
// transition 20; while it moves, transition 19 and sound 4 (MenuList:: and
// MenuSound::Play through their overrides). The last call's r3. 96-byte
// frame.
uint64_t BuildDetailsPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219FA80 BuildScreen::UpdatePage (r4 the mode): the five page
// handlers' addresses at the frame's +80; unless the level is -1, its
// handler called (r3 the mode, through the guest context). Its r3, else
// `r3`. 112-byte frame.
uint64_t BuildUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls);

// --- The hub's handlers and helpers (hub.cpp) ---
//
// The hub's globals: the page object (28 bytes), the menu (68), the data
// (asked for 68 bytes, used up to +447: VirtualAlloc gives a page) and the
// load counter; its movie 0.
constexpr uint32_t kHubPages = 0x826D8410;
constexpr uint32_t kHubMenu = 0x826D8414;
constexpr uint32_t kHubData = 0x826D8418;
constexpr uint32_t kHubCounter = 0x826D841C;
constexpr uint32_t kHubMovie = 0;
constexpr uint32_t kSetChildNameText = 0x8211D930;
constexpr uint32_t kSetNumberText = 0x8211C740;

// sub_8218D6A8 Type2Hub::OnYes and sub_8218D6C0 OnNo: the data's +28 = 0
// or 1. Leaves.
void HubOnYes(uint8_t* base);
void HubOnNo(uint8_t* base);
// sub_8218E2D8 Type2Hub::ImagesLoadedA and sub_8218E320 ImagesLoadedB (r3
// the data): 1 when every record of the first list (+32, count +216) or
// the second (+220, count +404) has its +0 set, else 0. Leaves.
uint32_t HubImagesLoadedA(uint8_t* base, uint32_t data);
uint32_t HubImagesLoadedB(uint8_t* base, uint32_t data);
// sub_8218E420 Type2Hub::WaitForLoad (r3 the machine): while +9608 is 0,
// +9608 = 1 once movies 0, 9 and 10 and both image lists are loaded; the
// counter at 0x826D841C (the empty sub_82106540 called while its / 15 is
// even) counted up; once loaded and the counter at least 1 (the patched
// comparison: 60 in the original, the PATCH: in generated/), the counter 0
// and +9604 = 9 or 4 by +9612. The last call's r3, else `r3`. 112-byte
// frame.
uint64_t HubWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_8218D650 Type2Hub::FreeImage (r3 an image record): +0 = 0, its +8 and
// +12 VirtualFree'd. The last VirtualFree's r3. 96-byte frame.
uint64_t HubFreeImage(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls);
// sub_8218E5B0 Type2Hub::OpenSelected (r3 the machine, r4 the mode): the
// message by the menu's cursor (8, 12, 18, 10, 23, 21, 14, else 51)
// posted, +9604 = 9. A leaf.
void HubOpenSelected(uint8_t* base, uint32_t machine, uint32_t mode);
// sub_8218D9B8 Type2Hub::Reenter (r3 the machine): with +9608, the page
// object to page 0 after transition 4 (the data's +0 set) or 0, then 1, and
// +9604 = 4; else +9604 = 1. A leaf.
void HubReenter(uint8_t* base, uint32_t machine);
// sub_8218E7A4 Type2Hub::PostMessage5 (r3 the mode): message 5. A leaf.
void HubPostMessage5(uint8_t* base, uint32_t mode);
// sub_8218E768 Type2Hub::Choose (r3 the mode, r4 the machine): by the menu's
// cursor (more than 7 nothing): 0 to 6 PostMessage5, 7 the machine's +9604
// = 10 and the data's +4 = 1. A leaf.
void HubChoose(uint8_t* base, uint32_t mode, uint32_t machine);
// sub_8218E530 Type2Hub::Unload (r3 the machine): the data's +20 = 0;
// FinishAll(movie 0), its object's (+28) +36 and +32 put back from the
// data's +440 and +444; with +9616, UiMovies::Delete(0) and +9608 = 0. The
// last call's r3. 128-byte frame.
uint64_t HubUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218E688 Type2Hub::Close (r3 the machine, r4 the mode): +9604 = 7;
// FinishAll(movie 0) when found, its object's words put back, Delete(0),
// message 4. Delete's r3. 112-byte frame.
uint64_t HubClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                  const Calls& calls);
// sub_8218F530 Type2Hub::SetPrompts (r3 to r8 three pairs): movie 0's
// sub-scene 10 nodes 0 to 2 shown (+80) with their two children's frames
// (+108) the pair when neither is -1, else hidden. The last FindNode's r3.
// 192-byte frame.
uint32_t HubSetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp);
// sub_8218F698 Type2Hub::Back: the page object to page 0 (no transition),
// then SetPrompts(0, 0, 6, 2, 7, 3) (a tail call, on `sp`). Its r3.
uint32_t HubBack(uint8_t* base, uint32_t sp);
// sub_8218E810 Type2Hub::SetUpStatus: movie 0's name (8, 6), money (8, 9)
// and day (8, 13, string 8, at most 999) from player 0's save data
// (SetChildNameText, SetNumberText and SetText through their overrides),
// node (8, 11)'s frame the day % 7; GarageStage::SetPlates(the stage, 1);
// the data's +0 = 1, +4 and +8 = 0. r3 the stage. 128-byte frame.
uint64_t HubSetUpStatus(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218DBD0 Type2Hub::NextNotice: the chained entry (0), the first
// unread entry of the first notice list copied to the data's +428 (1), or
// none, the list's count 0 (2). 96-byte frame.
uint32_t HubNextNotice(uint8_t* base, uint32_t sp);
// sub_8218DD78 Type2Hub::NextNotice2: the first unread entry of the second
// list copied to the data's +416 (1), or none, its count 0 (0). 96-byte
// frame.
uint32_t HubNextNotice2(uint8_t* base, uint32_t sp);
// sub_8218F6E0 Type2Hub::Start: movie 0's object's +36 and +32 kept in the
// data's +440 and +444; SetUpStatus; NextNotice: a notice, page 4 after
// transitions 5 and 1; none, NextNotice2: page 5 or 0 after 0 and 1; the
// data's +20 = 0. NextNotice's or NextNotice2's r3. 112-byte frame.
uint32_t HubStart(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The hub's set-up, teardown and notice images (hub_images.cpp) ---
//
// VirtualAlloc, VirtualFree, XMemSet, memcpy, Archive::Load (the load
// queued, the callback ImageLoaded), GarageStage::MakeModel, UiMovies::Open,
// XPR::Create, XPR::GetTexture and Texture::GetLevelDesc through their
// overrides; Archive::AssetSize, CarModels::BlockOf, SaveData::Of and
// DataTables::Entry56 their plain C++.
constexpr uint32_t kXMemSet = 0x822962B0;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kArchiveLoad = 0x8209DE48;
constexpr uint32_t kStageMakeModel = 0x8217FCB0;
constexpr uint32_t kXprCreate = 0x82596F68;
constexpr uint32_t kXprGetTexture = 0x82597118;
constexpr uint32_t kGetLevelDesc = 0x822A5B78;
constexpr uint32_t kHubImageLoaded = 0x8218D8E8;

// sub_8218DCC0 Type2Hub::MarkNoticeImages: for each entry of player 0's
// first notice list, its data table chain (Entry56, by the entries' +8):
// bit +16 of each (when not negative) set in the data's +408 bit set; the
// entry's +0 at the frame's +80. The last Entry56's r3, else the save data.
// 160-byte frame.
uint32_t HubMarkNoticeImages(uint8_t* base, uint32_t sp);
// sub_8218E078 Type2Hub::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the menu (68) and the data (68 asked for: a page, used
// up to +447) VirtualAlloc'ed into their globals with their first words;
// MarkNoticeImages; +9604 = 1 or 9 by +9612. MarkNoticeImages' r3. 128-byte
// frame.
uint32_t HubCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218DE88 Type2Hub::FreeImages (r3 the data): both image lists emptied
// from the last, each record FreeImage'd (its plain C++) and VirtualFree'd.
// The last VirtualFree's r3, else the data. 112-byte frame.
uint64_t HubFreeImages(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls);
// sub_8218E1A8 Type2Hub::Destroy (r3 the machine): the data's +20 = 0; with
// its +4, +9604 = 14; FreeImages; the three blocks VirtualFree'd, the
// globals 0. The last VirtualFree's r3. 128-byte frame.
uint64_t HubDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218DF38 Type2Hub::LoadNoticeImage (r3 the data, r4 i): for assets
// 22696 + i and 22741 + i, a buffer of the asset's size (VirtualAlloc,
// XMemSet 0), a 16-byte record (+4 = i) appended to the first or second
// list, Archive::Load(asset, buffer, ImageLoaded, record, 0, 0). The last
// Load's r3. 144-byte frame.
uint64_t HubLoadNoticeImage(uint8_t* base, uint32_t data, uint32_t index, uint32_t sp,
                            const Calls& calls);
// sub_8218E240 Type2Hub::LoadNoticeImages: LoadNoticeImage (its plain C++)
// for each i below 46 set in the data's +408. r3 the data. 112-byte frame.
uint32_t HubLoadNoticeImages(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218E368 Type2Hub::OpenMovie: with a car in player 0's garage, the
// selected one (by the slot links) copied to the frame's +80 (memcpy, 1,968
// bytes) and its model block (CarModels::BlockOf(its +12)), else 0;
// GarageStage::MakeModel(the stage, the block, 1); UiMovies::Open(0,
// 22545); LoadNoticeImages. r3 the data. 2,064-byte frame.
uint32_t HubOpenMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218D8E8 Type2Hub::ImageLoaded (the archive's callback: r4 the buffer,
// r6 the record): +12 = XPR::Create on a 24-byte VirtualAlloc, +8 = a
// 12-byte texture record (VirtualAlloc: +0 XPR::GetTexture(0), +4 and +8
// the level's width and height from Texture::GetLevelDesc into the frame's
// +80); the buffer VirtualFree'd; +0 = 1. VirtualFree's r3. 144-byte frame.
uint64_t HubImageLoaded(uint8_t* base, uint32_t buffer, uint32_t record, uint32_t sp,
                        const Calls& calls);

// --- The hub's page dispatch, movie helpers and transitions (hub_movie.cpp) ---
//
// UiMovie::Play (sub-scene, child), PlayNamed, Loop, LoopNamed, LoopLast,
// Finish, FinishNamed, SetNodeColor, SetText, ScrollText, ScrollReset,
// HasMoreText, Run, the screen fades and Params::GetCarSpec through their
// overrides (Play's r6 to r9, PlayNamed's r5 to r8 as the call sites set
// them; SetText's unset varargs 0); UiMovies::Find and UiMovie::FindNode
// their plain C++.
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kSetNodeColor = 0x8211A5B8;
constexpr uint32_t kScrollText = 0x8211C570;
constexpr uint32_t kScrollReset = 0x8211C5F8;
constexpr uint32_t kHasMoreText = 0x8211D8D8;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kGetCarSpec = 0x820C9E98;

// sub_82190498 Type2Hub::UpdatePage (r3 the machine, r4 the mode): the seven
// page handlers' addresses at the frame's +80; unless the page is -1, the
// page's handler called (r3 the mode, r4 the machine, through the guest
// context). Its r3, else `r3`. 128-byte frame.
uint64_t HubUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                       const Calls& calls);
// sub_8218F4D8 Type2Hub::SetRecordText: movie 17's (-1, 1) text = string
// 7527 + the data's +424. SetText's r3. 96-byte frame.
uint64_t HubSetRecordText(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821900B8 Type2Hub::RecordPage (r3 the mode): SetRecordText; the page
// object to none; message 35; the data's +24 = 2. SetText's r3. 96-byte
// frame.
uint64_t HubRecordPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8218EA90 Type2Hub::ShowBusy and sub_8218EB10 HideBusy: movie 0's
// named 56 and 54 played and the last looped, or 57 played and 54 finished.
// The last call's r3. 112-byte frames.
uint64_t HubShowBusy(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t HubHideBusy(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218F3F8 Type2Hub::ShowNoticeImage (r3 an image index): movie 0's
// object's +36 and +32 the texture records (+8) of the two lists' records
// for it, 0 when none. r3 UiMovies::Find's. 112-byte frame.
uint32_t HubShowNoticeImage(uint8_t* base, uint32_t image, uint32_t sp);
// sub_821903D0 Type2Hub::ShowNotice: the notice's entry (Entry56): texts
// (0, 1) string 4650 + its +12 and (0, 2) 6683 + the data's +436;
// ShowNoticeImage(its +16); node (0, 0) shown by HasMoreText(0, 2). r3 the
// node. 128-byte frame.
uint32_t HubShowNotice(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218FB20 Type2Hub::FillMenu: the eight items' texts, the description
// (ScrollText, ScrollReset, string 536 + the cursor or 587, 588), the
// prompts, the data's +12 = the selected garage car's spec's byte +771 is 1.
// GetCarSpec's r3. 176-byte frame.
uint64_t HubFillMenu(uint8_t* base, uint32_t sp, const Calls& calls);
// The transitions (r3 the started word's address: the set-up while it's 0,
// then 1), each running movie 0 (Run(movie, 0)):
// sub_821901C0 Transition0 (Run's answer 0), sub_8218EC78 Transition1 (1),
// sub_8218EE48 Transition2 (1), sub_8218ED98 Transition3 (the fades done),
// sub_82190548 Transition5 (Run's answer 0), sub_8218F098 Transition6 (the
// fades done) and sub_821902C8 Transition7 (the fades done). Frames 112,
// 128, 144, 112, 112, 112, 112.
uint64_t HubTransition0(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HubTransition1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HubTransition2(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HubTransition3(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HubTransition5(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HubTransition6(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);
uint64_t HubTransition7(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The hub's pages and dialog replies (hub_pages.cpp) ---
//
// Menu::Step, Menu::Pressed, MenuSound::Play, UiMovie::RevealText,
// HasMoreText and SetText, memcpy, PlayTime::Add and Update and the spec
// sheet's car data sub_8218D708 (never seen in play) through their
// overrides (or the original); the hub's helpers their plain C++.
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kRevealText = 0x8211D880;
constexpr uint32_t kPlayTimeAdd = 0x821109B8;
constexpr uint32_t kPlayTimeUpdate = 0x82112838;
constexpr uint32_t kSheetCarData = 0x8218D708;
// The player's record's screen (sub_8220B408's: +8 the record).
constexpr uint32_t kRecordScreen = 0x826D84CC;

// sub_8218DA48 Type2Hub::FillPlayerRecord: into [0x826D84CC] + 8 from a copy
// of player 0's save +48 (6,044 bytes, memcpy into the frame's +96): three
// halfwords, the ratio (single precision: +12 / +22 x [0x82002A0C], or
// [0x82000A34] with +22 0), more fields (one truncated as fctiwz, as the
// recompiled code: NaN 0x80000000, from INT_MAX saturated), the play time
// (PlayTime::Add into the frame's +80), the garage's count and save +
// 0x1991C. 6,176-byte frame.
void HubFillPlayerRecord(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218E8F8 Type2Hub::NoticePage: Menu::Pressed(159) (sound 0): while
// movie 0's text (0, 2) reveals more, node (0, 0) shown by HasMoreText;
// done, NextNotice: page 4 after 11 and 10 or 9 and 8; none, NextNotice2:
// page 5 or 0 after 6 and 7. The last call's r3. 112-byte frame.
uint64_t HubNoticePage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8218F800 Type2Hub::OnDialogDone (r4 the mode): by the data's +24 (the
// dialog's step) and +28 (the answer): the second quit question (movie 16,
// message 34), page 0 with the prompts, page 6 and message 43, page 3
// after 3 with PlayTime::Update, NextNotice2's page 5. The last call's r3,
// else `r3`. 128-byte frame.
uint64_t HubOnDialogDone(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls);
// sub_8218FCC8 Type2Hub::MenuPage (r3 the mode): Menu::Step (moved: sound
// 4, transition 2); else by Menu::Pressed: 159 by the item (page 3 after
// transition 3, sound 3 for items 1 and 2 with a second-list car, or the
// quit question), 158 transition 12, 163 the spec sheet (the car copied to
// the frame's +96, sub_8218D708, message 24), 164 the record
// (FillPlayerRecord, message 27). The last call's r3. 2,112-byte frame.
uint64_t HubMenuPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);

// --- The build screen's entry (build_enter.cpp) ---
//
// DataTables::Item, SaveData::Of and UiMovie::FindNode their plain C++;
// memcpy, BuildList::Insert, Stage::Of, UiMovie::ResetPose, the text
// setters and ScreenFades::FadeIn through their overrides.
constexpr uint32_t kBuildListInsert = 0x8219C230;
constexpr uint32_t kStageOf = 0x821121F8;

// sub_8219C160 BuildList::RemoveAt (r3 a build list, r4 a position): the
// node at the position unlinked (by the halfword links from +1604) and put
// at the head of the free list (+1600), the count (+1608) down by one. A
// leaf: r3 the list.
void BuildListRemoveAt(uint8_t* base, uint32_t list, uint32_t position);
// sub_821A0228 BuildScreen::Fill: the screen data from player 0's save data
// and the data tables: the name, money, weekday and day (at most 999); the
// 400 builds (52-byte records built in the frame's +96 and memcpy'd to
// +0x2C784 + 52i); the 56 build lists (+76 + 3252j) made empty, each build
// inserted into its group's (BuildList::Insert from the frame's +80); the
// count of state-2 builds as a percentage (+0x32654, single precision and
// fctiwz as the recompiled code); each list's summary (+36 + 3252j);
// the builds above the player's stage (Stage::Of + 1) removed
// (RemoveAt's plain C++); +0x32658 whether a kind-3 list has builds. The
// last call's r3. 240-byte frame.
uint64_t BuildFill(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219C438 BuildScreen::SetUpStatus: movie 6's ResetPose; the name (0,
// 6), money (0, 9) and day (0, 13, string 8) from the screen data's +4 to
// +32, node (0, 11)'s frame the weekday; the data's +0x3265C and +0x32660
// = 0, +0 = 1. r3 the node. 112-byte frame.
uint32_t BuildSetUpStatus(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821A0860 BuildScreen::Enter: Fill and SetUpStatus (their plain C++);
// the transition machine started (page 0 after transitions 0 and 1);
// ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0). FadeIn's r3. 96-byte frame.
uint64_t BuildEnter(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The build screen's level pages and their jumps (build_pages.cpp) ---
//
// The jumps (the shoulder buttons over the list [0x826D8440]): the player's
// action mapper (0x826BAB80 + 8 x [0x826BABA4], none when its +4 is 1)
// asked through its slot 12 (+48, through `calls`) for the back action,
// then the forward one; the cursor (the list's scroll + selection) moved to
// the previous (from the cursor down, wrapping from the end) or next
// (wrapping to the start) item the jump takes; then UiMovie::FinishNamed on
// movie 6 (29 + the selection for the groups, 61 + it for the builds, 93
// for the stand), MenuList::SetSelection(the item less the scroll) and
// MenuList::Fit(0) through `calls`. 1 when it jumped, else 0. 160-byte
// frames (the movie id at +80).
// sub_8219DBC8 JumpGroupA: 161 and 162 over the shown groups (the screen's
// +0x318C4, count +0x319A4) marked (+32 set).
uint32_t BuildJumpGroupA(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219DDB8 JumpGroupB: 163 and 164 over the groups whose +28 is 0.
uint32_t BuildJumpGroupB(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219DFA8 JumpBuildA: 161 and 162 over the screen's build list
// (+0x319A8, its halfword count +0x31FF0) to builds whose +32 isn't their
// +36.
uint32_t BuildJumpBuildA(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219E348 JumpBuildB: 163 and 164 to builds whose kind (+8) isn't 2.
uint32_t BuildJumpBuildB(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219E600 JumpStandA and sub_8219E9A0 JumpStandB: as JumpBuildA and B
// with the named animation 93.
uint32_t BuildJumpStandA(uint8_t* base, uint32_t sp, const Calls& calls);
uint32_t BuildJumpStandB(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219EEC0 BuildScreen::GroupsPage (level 0): MenuList::Update on the
// list; still, JumpGroupA and B (their plain C++); moving or jumped,
// transition 3 and sound 4; else accepted (sound 0, transitions 6 then 7
// to level 1), cancelled (sound 2, transition 5 to level 4) or the list's
// +28 set (sound 4, transition 4). The last call's r3. 96-byte frame.
uint64_t BuildGroupsPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219F048 BuildScreen::BuildsPage (level 1): both lists updated; the
// list moving or a jump (JumpBuildA, B), transition 8; the group list
// moving (+24), transition 9 (sound 4 for both); accepted, transitions 11
// then 12 to level 2 (sound 0); cancelled, transition 10 back (sound 2).
// The last call's r3. 112-byte frame.
uint64_t BuildBuildsPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8219F1C8 BuildScreen::BuildPage (level 2): as BuildsPage with
// JumpStandA, B and transitions 14, 15, 17 then 18 to level 3 (the selected
// build's +36 = its +32), 16 back.
uint64_t BuildBuildPage(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The machines' set-ups (init_states.cpp) ---
//
// The transitions added to `machine` (StateMachine::Add's plain C++) from
// functors built in a frame of their own (`frame`, the original's stores in
// order). Each returns the last Add's r3 (an index when `replaced`, else
// the new transition).

// sub_82190B18 Type2Hub::InitStates: 22 transitions, a 1,552-byte frame.
constexpr uint32_t kHubInitFrame = 1552;
uint32_t HubInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
// sub_821B90D8 PowerGraphScreen2::InitStates: 12, 912 bytes.
constexpr uint32_t kSheetInitFrame = 912;
uint32_t SheetInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
// sub_821A16C0 BuildScreen::InitStates: 12, 912 bytes.
constexpr uint32_t kBuildInitFrame = 912;
uint32_t BuildInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
// sub_8219B2E8 PlateEditor::InitStates: 12, 912 bytes.
constexpr uint32_t kPlateInitFrame = 912;
uint32_t PlateInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace type2_screens
