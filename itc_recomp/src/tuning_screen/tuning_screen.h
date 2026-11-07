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

}  // namespace tuning_screen
