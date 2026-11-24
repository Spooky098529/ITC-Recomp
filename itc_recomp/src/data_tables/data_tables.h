// The data tables at [0x826C5CA0] as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/data-tables.md):
// twelve assets loaded once at boot into one block, their entries
// byte-swapped in place by the loads' callbacks, then read by index all
// through the game. Since E5 step 5 the object is in host memory and the
// block is a blob (the files' bytes in PC memory, read by address as guest
// memory is). Adapters: src/hooks/data_tables.cpp.

#pragma once

#include "archive/archive.h"

#include <cstdint>
#include <optional>

namespace data_tables {

// The table object's pointer (schema::DataTables, schema/data_tables.h; the
// constant kept for other areas' code and the harnesses).
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

// --- The set-up and the loads (from BootData::CreateObjects, once) ---

// sub_82116DB0 DataTables::Init: the 64-byte object (in host memory since
// E5 step 5, schema::DataTables::New) into 0x826C5CA0, its +12, +4 and +8 =
// 0; then two passes over the twelve assets (94 at +16, 84 at +60, the eight at +20 to +48, 95 at +52,
// 96 at +56), each table pointer the block (+12) plus the sizes so far (0
// while there's no block) and +4 counted up from 0; the sizes from the TOC
// (Archive::Toc(id) + 16 x id + 24, or + 20 when that's 0;
// Archive::AssetSize for the eight); between the passes the block (the
// original's operator new[] of the total; a blob since E5 step 5,
// schema::DataTablesBlock). Returns r3 as the original leaves it (the last
// Archive::Toc's); no longer nullopt (kept for the adapter).
std::optional<uint32_t> Init(uint8_t* base);

using LoadCall = archive::LoadCall;
// sub_82117240 DataTables::Load: the twelve loads into the tables Init set
// (94 into +16 with OnItemsLoaded and the object; 84 into +60 with OnLoaded84
// and the object; the eight into +20 to +48 with On56Loaded and the table;
// 95 into +52 with On20Loaded and 96 into +56 with On40Loaded, the table),
// each serial stored at the object's +0. Returns the last serial (r3).
int64_t Load(uint8_t* base, const LoadCall& load);

}  // namespace data_tables
