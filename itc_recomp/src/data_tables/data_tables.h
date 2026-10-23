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

// The accessors, on the tables the global points at (no bounds checks, as
// the originals'):
// DataTables::IsLoading (sub_82117158): 1 while loads are pending, else 0.
uint32_t IsLoading(uint8_t* base);
// DataTables::Item (sub_82117178): item `id`'s 3,008-byte entry (asset 94).
uint32_t Item(uint8_t* base, uint32_t id);
// ItemRecords::Item (sub_82116DA8, probably): Item(id); r3 (the records) not read.
uint32_t RecordsItem(uint8_t* base, uint32_t id);
// DataTables::Entry56 (sub_82117190): entry `entry` of 56-byte table `table`.
uint32_t Entry56(uint8_t* base, uint32_t table, uint32_t entry);
// DataTables::Find20 (sub_821171B0): the index of the first 20-byte entry
// (asset 95) whose +8 is `key8` and +0 is `key0`, or -1.
int32_t Find20(uint8_t* base, uint32_t key8, uint32_t key0);
// DataTables::Entry20 (sub_82117210) and Entry40 (sub_82117228): an entry
// of asset 95's and 96's tables.
uint32_t Entry20(uint8_t* base, uint32_t index);
uint32_t Entry40(uint8_t* base, uint32_t index);

}  // namespace data_tables
