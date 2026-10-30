// The career's progress natively (docs/functions/career.md): plain C++ over
// the guest layout of a player's save data (offsets are the save data's
// unless said), store for store as the recompiled functions. Adapters in
// src/hooks/career.cpp (behind --native_career), register steps shared with
// the harness in src/hooks/career.h.

#pragma once

#include <cstdint>

namespace career {

// The save data's parts.
constexpr uint32_t kStats = 48;                // the stats record
constexpr uint32_t kItemRecords = 0x199F8;     // 400 records of 12 bytes
constexpr uint32_t kItemFlags = 0x1ACE4;       // a 400-bit set
constexpr uint32_t kGarage = 6120;             // the garage's list
constexpr uint32_t kSelectedCar = 0x19920;     // the selected garage car
// The play time's frame counter (PlayTime::Add measures from it).
constexpr uint32_t kPlayTimeFrames = 0x826C5B90;

// --- Small accessors ---

// sub_82111628 PlayTime::Mark: save +6088 = [0x826C5B90].
void PlayTimeMark(uint8_t* base, uint32_t save);
// sub_82110580 DayBlock::WinStreak: halfword +4828 of the day block.
uint32_t DayBlockWinStreak(uint8_t* base, uint32_t block);
// sub_82110590 DayBlock::AddWord4832: word +4832 += amount (its low 32 bits).
void DayBlockAddWord4832(uint8_t* base, uint32_t block, uint64_t amount);
// sub_821105A0 DayBlock::Word4832: word +4832.
uint32_t DayBlockWord4832(uint8_t* base, uint32_t block);
// sub_82110B10 Stats::WinStreak: halfword +5970 of the stats record.
uint32_t StatsWinStreak(uint8_t* base, uint32_t stats);
// sub_82110B18 Stats::AddPoints: +6024 += amount, at most 9,999,999 (signed).
void StatsAddPoints(uint8_t* base, uint32_t stats, uint64_t amount);
// sub_82112860 ItemFlags::Set: with `zero` 0 and a non-negative id, bit id
// of the 400-bit set at +0x1ACE4.
void ItemFlagsSet(uint8_t* base, uint32_t save, uint32_t zero, uint32_t id);
// sub_821104B0 Car::SpecWord744: Params::GetCarSpec(car +12)'s word +744.
uint32_t CarSpecWord744(uint8_t* base, uint32_t car);
// sub_82115CC0 Career::PickFromCar: the pick at +8 from the selected garage
// car, then its 20 bytes copied to +28.
void PickFromCar(uint8_t* base, uint32_t save);

// --- Running means ---

// sub_82111CB0 RunningMean::Merge: {+0 a float mean, +4 a count} merged with
// `packed` (the float's bits in the upper word, the count in the lower),
// stored first at `sp` + 24 (the caller's frame, as the original's std).
void RunningMeanMerge(uint8_t* base, uint32_t mean, uint64_t packed, uint32_t sp);

// --- The stage and the unlock tests ---

// sub_821121F8 Stage::Of: SaveData::FirstUnsetFlag's 0 to 6 as 0, 0, 1, 1,
// 2, 3, 4 (anything above 6 as it came).
uint32_t StageOf(uint8_t* base, uint32_t save);
// sub_82111BA8 Entry56::Allows: whether the 56-byte entry's mask allows the
// mode, the location (with `test_location`) and the stage: 1 only when a
// test ran and none failed.
uint32_t Entry56Allows(uint8_t* base, uint32_t entry, uint32_t mode, uint32_t location,
                       uint32_t stage, uint32_t test_location);
// sub_82111780 Items::AnyWonKind3Stage1: whether any item with halfword +42
// 1, byte +36 0 and byte +35 3 has a win in player 0's record (+2).
uint32_t AnyWonKind3Stage1(uint8_t* base);

}  // namespace career
