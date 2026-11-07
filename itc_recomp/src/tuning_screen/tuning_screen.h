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
