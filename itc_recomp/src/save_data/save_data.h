// A player's save data in memory, natively (docs/functions/save-data.md):
// the progress's clear and the stats record's set-ups, the unlock bit set
// and the default tables' getters. Plain C++ over the guest layout, store
// for store as the recompiled functions; adapters in src/hooks/save_data.cpp
// (behind --native_save_data), register steps shared with the harness in
// src/hooks/save_data.h. Offsets are the save data's unless said.

#pragma once

#include <cstdint>
#include <optional>

namespace save_data {

// The save data's parts.
constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kStats = 48;            // the stats record, 6,044 bytes
constexpr uint32_t kStatsBytes = 6044;
constexpr uint32_t kProgressBytes = 0x1D52C;  // what Progress::Clear clears
constexpr uint32_t kUnlocks = 0x1ACE4;     // BitSet400
// The play time's frame counter (Progress::SetDefaults copies it to +6088).
constexpr uint32_t kPlayTimeFrames = 0x826C5B90;
// The float the set-ups store (0.0, at 0x82000A34 in the xex).
constexpr uint32_t kZeroFloat = 0x82000A34;

// The default tables (static, in the xex's data).
constexpr uint32_t kTable50A8 = 0x826C50A8;
constexpr uint32_t kTable3044 = 0x826C3044;
constexpr uint32_t kTable3E34 = 0x826C3E34;
constexpr uint32_t kTable3C24 = 0x826C3C24;
constexpr uint32_t kTable50E4 = 0x826C50E4;

// sub_821056C0 Stats::Stats: r3 = the stats record (save data + 48). The
// float at kZeroFloat to +4892, 0 to +4896; 15 pairs {float, 0} at +4960 +
// 48i; nine pairs at +5880 + 8i.
void StatsConstruct(uint8_t* base, uint32_t stats);
// sub_821136D8 Stats::ResetRecords: 15 records of 48 bytes from `records` +
// 4 ({0, 0, 0.0, 0, 0, -1, -1, -1, 0, 0, (+40 left), 0}), then +0 = 0.
void StatsResetRecords(uint8_t* base, uint32_t records);
// sub_82113730 Stats::Reset: the record cleared and given its starting
// values (with Stats::ResetRecords at +4948). Returns r3 as the original
// leaves it: the record + 4948.
uint32_t StatsReset(uint8_t* base, uint32_t stats);
// sub_82113BD8 Progress::Clear: 0x1D52C bytes cleared, then the progress's
// starting values (with Stats::Reset at +48). Returns r3 as the original
// leaves it: Stats::Reset's.
uint32_t ProgressClear(uint8_t* base, uint32_t save);
// sub_821133A8 BitSet400::SetAll: 13 words cleared, then bits 0 to 399 set.
void BitSet400SetAll(uint8_t* base, uint32_t set);

// sub_82113E60 Progress::SetDefaults: a new game's progress (Progress::Clear,
// the default tables copied, the unlock bits, the play time). The copies
// from the tables go through memcpy's plain C++ (native::Buffers_Memcpy);
// SetDefaultsNative says whether they can (else the adapter runs the
// original from the start). Returns r3 as the original leaves it: the
// unlock set (+0x1ACE4).
bool SetDefaultsNative(uint32_t save);
uint32_t ProgressSetDefaults(uint8_t* base, uint32_t save);

// sub_821058A8 ControlMap::SetDefault: the 15 default control numbers.
void ControlMapSetDefault(uint8_t* base, uint32_t map);
// sub_82105940 Options::SetDefaults: the settings, then the four control
// maps (+44, +104, +164, +224). Returns r3 as the original leaves it: the
// last map (+224).
uint32_t OptionsSetDefaults(uint8_t* base, uint32_t options);
// sub_82105D08 CourseRecords::Reset: the header, then 22 course records of
// 408 bytes from +20 (CourseRecord::Reset's plain C++). Returns r3 as the
// original leaves it: the last CourseRecord::Reset's.
uint32_t CourseRecordsReset(uint8_t* base, uint32_t records);

// The players' save data slots (SaveData::Of reads them).
constexpr uint32_t kSlots = 0x826C5B58;
constexpr uint32_t Slot(uint32_t player) { return kSlots + (player << 2); }
// sub_821055D8 SaveData::Delete: the slot's block freed (operator delete's
// plain C++, nothing for 0), the slot 0. Returns r3 as the original leaves
// it with operator delete's adapter native (the block). nullopt, with
// nothing done, where the original must run: a block the layer didn't give.
std::optional<uint32_t> SaveDataDelete(uint8_t* base, uint32_t player);

// The default tables' getters sub_820CCCC0, sub_820CCCD0, sub_820CCCE0,
// sub_820CCCF0, sub_820CCD00.
uint32_t DefaultsTable50A8();
uint32_t DefaultsTable3044();
uint32_t DefaultsTable3E34();
uint32_t DefaultsTable3C24();
uint32_t DefaultsTable50E4();

}  // namespace save_data
