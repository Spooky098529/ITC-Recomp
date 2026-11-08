// The front end's options screen natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/options-screen.md, the blocks in
// docs/known-data.md): the screen's handlers, its pages and steps, and the
// settings read from and written back to the save data in memory. Plain C++
// over guest memory (the settings and the page blocks are shared with the
// recompiled code), with the originals' loads and stores in their order.
// Where an original builds values below its stack pointer or in its frame,
// the same words are written there (`sp` is the caller's stack pointer: the
// back chain of the original's frame is written as the original leaves it;
// its register saves aren't). Adapters: src/hooks/options_screen.cpp
// (behind --native_options_screen), register steps shared with the harness
// in src/hooks/options_screen.h.

#pragma once

#include <cstdint>

namespace options_screen {

// The screen's blocks (docs/known-data.md): the page block (28 bytes), the
// front-end menu (68), the settings `o` (716).
constexpr uint32_t kPage = 0x826D8470;
constexpr uint32_t kMenu = 0x826D8474;
constexpr uint32_t kSettings = 0x826D8478;

// --- The controls' tables and the settings' leaves (controls.cpp) ---

// sub_821BA950 Controls::ButtonIndex: a button code as the save keeps it
// (0 to 30, unsigned) to its index in the options' button lists (0 to 16),
// -1 for the others (its -1 cases the split piece sub_821BA984, merged).
uint32_t ButtonIndex(uint32_t code);
// sub_821BAA18 Controls::ButtonCode: an index (-1 to 18, by r3 + 1 unsigned)
// to the code the save keeps; 0 outside (its -1 case the split piece
// sub_821BAA50, merged).
uint32_t ButtonCode(uint32_t index);
// sub_821BAAE0 Controls::AnalogIndex: a code to its index in the analog
// lists, -1 for most (its case 7 the split piece sub_821BAB14, merged).
uint32_t AnalogIndex(uint32_t code);
// sub_821BAD08 Controls::AnalogPicture: an analog index (-1 to 18, by r3 +
// 1 unsigned) to the picture shown, else -1 (its -1 cases the split piece
// sub_821BAD40, merged).
uint32_t AnalogPicture(uint32_t index);

// sub_821BA7E0 Options::IsButtonTaken and sub_821BAB58 Options::IsAnalogTaken:
// the twelve button edits (or six analog edits) in menu order copied below
// `sp` (sp - 48 or sp - 32, as the leaves leave them); 1 when another item
// holds the same value as item `item` (read from those words as the
// original does, any item), else 0.
uint32_t IsButtonTaken(uint8_t* base, uint32_t item, uint32_t sp);
uint32_t IsAnalogTaken(uint8_t* base, uint32_t item, uint32_t sp);

// sub_821BA550 Options::StepButtons and sub_821BABD8 Options::StepAnalog:
// the settings' side word (+668 or +676) = the menu's side (+36) when it's
// -1 or 1, else 0; with a side, 1 on the items before the last (12 or 6),
// and on the last the OK/Default word +660 toggled (left on 1, right on 0)
// and 1, else 0.
uint32_t StepButtons(uint8_t* base);
uint32_t StepAnalog(uint8_t* base);

// sub_821B9DB8 Settings::Reset: r3 = the settings (716 bytes): +0 = 0, the
// three 192-byte blocks (+44, +236, +428) and +12 (32 bytes) cleared
// (memset), then the words its row lists. Its 112-byte frame's back chain
// at sp - 112. Returns r3 as the original leaves it (memset's last
// destination, `settings` + 12).
uint32_t SettingsReset(uint8_t* base, uint32_t settings, uint32_t sp);

}  // namespace options_screen
