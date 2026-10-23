// The data tables at [0x826C5CA0] as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/data-tables.md):
// twelve assets loaded once at boot into one block, their entries
// byte-swapped in place by the loads' callbacks, then read by index all
// through the game. The tables are guest memory shared with the recompiled
// code, so these work on guest memory. Adapters: src/hooks/data_tables.cpp.

#pragma once

#include <cstdint>

namespace data_tables {

// The table object's pointer (a 64-byte object: +4 the loads pending, +8 the
// item count, +12 the block, +16 to +60 the tables).
constexpr uint32_t kTables = 0x826C5CA0;

// SwapWord (sub_820C97D8): reverses a guest word's four bytes in place, with
// the translated code's steps (a single widened to double and narrowed back,
// then the bytes stored reversed).
void SwapWord(uint8_t* base, uint32_t address);

// The entries' swaps (sub_82111AE0, sub_82111B30, sub_821128E0); each returns
// the address of the last word it swapped (what the original leaves in r3).
uint32_t SwapEntry20(uint8_t* base, uint32_t entry);
uint32_t SwapEntry56(uint8_t* base, uint32_t entry);
uint32_t SwapEntry40(uint8_t* base, uint32_t entry);

// Archive::Load's callbacks (r6 = the context). OnItemsLoaded (sub_821170D8)
// and OnLoaded84 (sub_821170C0) leave r3 as it was; the others return what
// the original leaves in r3 (the last entry's last word swapped).
void OnItemsLoaded(uint8_t* base, uint32_t object);
void OnLoaded84(uint8_t* base);
uint32_t On56Loaded(uint8_t* base, uint32_t table);
uint32_t On20Loaded(uint8_t* base, uint32_t table);
uint32_t On40Loaded(uint8_t* base, uint32_t table);

}  // namespace data_tables
