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

}  // namespace type2_screens
