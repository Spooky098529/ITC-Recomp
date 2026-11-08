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

#include "race_hud_object/race_hud_object.h"

#include <cstdint>

namespace options_screen {

// The guest calls the plain C++ makes: through the function table from the
// original's frame with its return address (the adapter's job).
using Calls = race_hud_object::Calls;

// The mode type (0x826032DC) the context and the posts follow, and the save
// data's options (player 0's save data + 0x1F85C).
constexpr uint32_t kModeType = 0x826032DC;
constexpr uint32_t kSaveOptions = 0x1F85C;
// The guest functions called through `Calls`: memcpy where its plain C++
// declines, SoundOptions::SetEffectsVolume and SetMusicVolume and
// UiMovies::Open through their overrides.
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kSetEffectsVolume = 0x821B9C20;
constexpr uint32_t kSetMusicVolume = 0x821B9CF0;
constexpr uint32_t kMoviesOpen = 0x8211E808;

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

// --- The settings' read and write (settings.cpp) ---

// sub_821BBF00 Settings::Read: the settings from player 0's save data (its
// row): the context by the mode type, the current settings, the controls'
// indexes (Controls::ButtonIndex and AnalogIndex over the four 60-byte
// records copied to its frame), the defaults (a 13-word table on its
// frame), type 2's career info, then SoundOptions::SetEffectsVolume and
// SetMusicVolume (through `calls`). Its 272-byte frame below `sp` (+80 the
// globals' base 0x826E0000, +84 the table, +144 the last record). r3
// SetMusicVolume's.
uint64_t SettingsRead(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821BAE10 Settings::Write: the settings back into player 0's save
// data (its row; the analog slot 4's word 8 kept as the original writes
// it). Its 176-byte frame below `sp` (+80 the record). r3 the last
// memcpy's (the last record's address).
uint64_t SettingsWrite(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The machine's handlers (handlers.cpp) ---

// sub_821BB9A8 Options::OnEnter: r3 = the machine: +9608 = 0, the seven
// blocks allocated (the allocation layer's pages: the adapter runs it only
// over the layer) and set up, Settings::Reset on the settings, +9604 = 1 or
// 9 by +9612. Its 128-byte frame below `sp`. r3 the last block.
uint32_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp);
// sub_821BBBA0 Options::WaitForMovie: r3 = the machine: while +9608 is 0,
// it = 1 once UiMovies::IsLoaded(7); then +9604 = 9 or 4 by +9612. Its
// 96-byte frame. r3 IsLoaded's when asked, else the machine (`r3`).
uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821B9F88 Options::OpenMovie: UiMovies::Open(7, 22553), a tail call
// (`sp` and `lr` the caller's). Its r3.
uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821BCE38 Options::OnClosed: r3 = the machine, r4 = the dispatcher:
// Settings::Write (from its 112-byte frame), message 6 (types 2, 6, 8), 5
// (type 7) or 8 (type 4) queued in the dispatcher's ring (+204), +9604 = 9.
// r3 Settings::Write's.
uint64_t OnClosed(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls);

}  // namespace options_screen
