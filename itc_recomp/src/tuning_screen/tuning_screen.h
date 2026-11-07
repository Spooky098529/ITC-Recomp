// The tuning screen natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/tuning-screen.md, the fields in docs/known-data.md's
// "tuning screen"): the front end's tuning shop, its part tables, its debug
// state handlers and its page set-ups. Plain C++ over guest memory (the
// screen is shared with the recompiled code), with the originals' loads and
// stores in their order; `sp` is the caller's stack pointer, and the
// original's frame (its back chain and the words it stores there) is
// written as the original leaves it, its register saves aren't. Adapters:
// src/hooks/tuning_screen.cpp (behind --native_tuning_screen), register
// steps shared with the harness in src/hooks/tuning_screen.h.

#pragma once

#include <cstdint>

namespace tuning_screen {

// The screen's fields.
constexpr uint32_t kState = 9652;      // the state machine (3 to 58)
constexpr uint32_t kCategory = 9656;   // m: 0 power to 6 interior
constexpr uint32_t kItem = 9660;       // p: a byte, the item in the category
constexpr uint32_t kLevel = 9661;      // a byte: the level pick
constexpr uint32_t kType = 9662;       // a byte: the type pick
constexpr uint32_t kType2 = 9663;      // a byte
constexpr uint32_t kOption = 9664;     // a byte: the option pick
constexpr uint32_t kPrice = 25120;     // GetPrice's answer kept
constexpr uint32_t kPartSet = 33900;   // the power and power train tables' row
constexpr uint32_t kSaveData = 18904;
constexpr uint32_t kMovieId = 2;       // the screen's UI movie (13 and 14 the editors')  // the save data (its +6120 the garage's list)

// --- The part tables (tables.cpp): leaves with no frame ---

// sub_821CC6C0 TuningScreen::CategoryItemCount: the items in category +9656
// (unsigned): 4, 5, 1, 3, 10, 8, 4, else 0.
uint32_t CategoryItemCount(uint8_t* base, uint32_t screen);
// sub_821CDB18 TuningScreen::IsItemAvailable: the screen's word for item
// `item` of the current list, 1 where there's none (r3, zero-extended).
uint32_t IsItemAvailable(uint8_t* base, uint32_t screen, uint64_t item);
// sub_821CDED8 TuningScreen::WheelNumber (its case 0 the split piece
// sub_821CDF0C, merged): `model` + the brand's first number (all 64 bits),
// 0 above brand 16.
uint64_t WheelNumber(uint64_t brand, uint64_t model);
// sub_821CD160 TuningScreen::GetPrice (its case 0 the split piece
// sub_821CD308, merged): +25120 = 0, then the current choice's word of the
// price tables where there's one; r3 = +25120.
uint32_t GetPrice(uint8_t* base, uint32_t screen);
// sub_821CD868 TuningScreen::GetCurrentChoice: the current item's word, 0
// where there's none.
uint32_t GetCurrentChoice(uint8_t* base, uint32_t screen);
// sub_821CDC38 TuningScreen::GetChoiceFlag: the current item's word for
// choice `choice`, 0 where there's none.
uint32_t GetChoiceFlag(uint8_t* base, uint32_t screen, uint64_t choice);

// --- The selected garage car (garage_car.cpp) ---

// The selected garage car's record g (the save data's list at +6120 walked
// [+0x19920] steps from the head halfword +204, cars of 1,968 bytes from
// it); the fields the screen writes are at g + 212 + k ("c+k").
uint32_t SelectedGarageCar(uint8_t* base, uint32_t screen);
// sub_821DCC28 TuningScreen::StoreToGarageCar: r3 = the screen (all 64
// bits). The screen's current choices into the car. No frame: its tables
// and conversion temporaries below `sp` as the original leaves them (its
// register saves, sp - 64 to sp, aren't written). Returns r3 as the
// original leaves it: the sticker loop's last tens x 10 (64 bits), else the
// screen.
uint64_t StoreToGarageCar(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821DF448 TuningScreen::EquipChoice: the choice just made into the car,
// by the state (5 to 8). A leaf; r3 left as it is.
void EquipChoice(uint8_t* base, uint32_t screen);

// sub_821DE2D8 TuningScreen::AeroPartValue (its case 0 the split piece
// sub_821DF24C, merged; aero.cpp): r4 = the type - 1, r5 = the aero item,
// both unchecked. Its frame's constant tables written below `sp` as the
// original leaves them, then the selected car's table[type][item] (r3
// zero-extended), all 64 bits set for the ids with none.
uint64_t AeroPartValue(uint8_t* base, uint32_t screen, uint64_t type, uint64_t item,
                       uint32_t sp);

// --- The part list's marks (marks.cpp) ---

// Movie 2: its id at `frame` + 80, then UiMovies::Find's movie.
uint32_t Movie(uint8_t* base, uint32_t frame);
// sub_821EE6D0 TuningScreen::HidePartMarks: the six rows' (sub-scene 1's
// nodes 14 to 19) children 3 to 5 hidden. Its 128-byte frame below `sp`.
// r3 the last child.
uint32_t HidePartMarks(uint8_t* base, uint32_t sp);
// sub_821EE760 TuningScreen::ShowPartMarks: r3 = the screen (all 64 bits),
// `row` a row node, `group` and `index` a wheel (WheelNumber's). By +25196
// (0 both, 1 front, 2 rear): child 5 shown, with 4 for the front wheel's
// match (+27836) and 3 for the rear's (+27840). Its 128-byte frame. r3 as
// the original leaves it: the last child shown, else WheelNumber's, else
// the screen.
uint64_t ShowPartMarks(uint8_t* base, uint64_t r3, uint32_t row, uint64_t group, uint64_t index,
                       uint32_t sp);

// --- Movie 2's node setters (nodes.cpp) ---
// Each with the original's frame below `sp` (its back chain and the movie
// id at +80) and r3 as the original leaves it.

// sub_821DA3F8 TuningScreen::ShowInteriorPicture: `type` 0 hides node
// (-1, 0); else it's shown and its child 0's +108 = `index` (+0, +4, +8,
// +11 for the byte +9662 1 to 4; untouched otherwise). r3 the node or the
// child.
uint32_t ShowInteriorPicture(uint8_t* base, uint32_t screen, uint32_t type, uint32_t index,
                             uint32_t sp);
// sub_821DFB20 TuningScreen::HideTireMarks: nodes (30, 0) and (33, 0)'s
// children 11, 10 and 1 hidden. r3 the last child.
uint32_t HideTireMarks(uint8_t* base, uint32_t sp);
// sub_821DFBE8 TuningScreen::HideNode19: node (-1, 19) hidden by the state
// and picks (its row). r3 the node hidden, else the movie.
uint64_t HideNode19(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821E22E8 TuningScreen::ShowTireMarks: the tire's level and option
// marks on nodes (30, 0) and (33, 0) (its row), HideTireMarks from its
// frame outside the tire states. r3 the last child, or GetCurrentChoice's.
uint32_t ShowTireMarks(uint8_t* base, uint32_t screen, uint32_t sp);

// --- The debug state handlers (debug.cpp) ---
// Each only prints a debug menu through the empty sub_82106540, left out:
// its frame's back chain and string tables written as the original leaves
// them; r3 the last print's first argument. sub_821CBE08
// TuningScreen::DebugMainMenu (state 3), sub_821CBEE8 DebugLevelSelect (5),
// sub_821CBFC0 DebugTypeSelect (6), sub_821CC248 DebugOptionSelect (8; 500
// or 520 by the category and item), sub_821CC3A8 DebugConfirmBuy (9),
// sub_821CC488 DebugConfirmEquip (11), sub_821CFCE8 DebugCategoryMenu (4;
// r3 CategoryItemCount's last answer), sub_821E8068 DebugWheelPartSelect
// (15), sub_821E8128 DebugWheelKindSelect (16), sub_821E8298
// DebugWheelInchSelect (17), sub_821FD710 DebugDecalTopMenu (23, 38),
// sub_821FD880 DebugLayerSelect (24, 39), sub_821FDCB8 DebugLayerEdit (26,
// 41), sub_821FE080 DebugLayerSet (28, 43), sub_821FE100 DebugEditPosition
// (29, 44; no frame), sub_821FE178 DebugEditView (34, 49; no frame),
// sub_821FE190 DebugPaintConfirm (35, 50).
uint32_t DebugMainMenu(uint8_t* base, uint32_t sp);
uint32_t DebugLevelSelect(uint8_t* base, uint32_t sp);
uint32_t DebugTypeSelect(uint8_t* base, uint32_t sp);
uint32_t DebugOptionSelect(uint8_t* base, uint32_t screen, uint32_t sp);
uint32_t DebugConfirmBuy(uint8_t* base, uint32_t sp);
uint32_t DebugConfirmEquip(uint8_t* base, uint32_t sp);
uint32_t DebugCategoryMenu(uint8_t* base, uint32_t screen, uint32_t sp);
uint32_t DebugWheelPartSelect(uint8_t* base, uint32_t sp);
uint32_t DebugWheelKindSelect(uint8_t* base, uint32_t sp);
uint32_t DebugWheelInchSelect(uint8_t* base, uint32_t sp);
uint32_t DebugDecalTopMenu(uint8_t* base, uint32_t screen, uint32_t sp);
uint32_t DebugLayerSelect(uint8_t* base, uint32_t screen, uint32_t sp);
uint32_t DebugLayerEdit(uint8_t* base, uint32_t sp);
uint32_t DebugLayerSet(uint8_t* base, uint32_t sp);
uint32_t DebugEditPosition();
uint32_t DebugEditView();
uint32_t DebugPaintConfirm(uint8_t* base, uint32_t sp);

}  // namespace tuning_screen
