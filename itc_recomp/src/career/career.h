// The career's progress natively (docs/functions/career.md): plain C++ over
// the guest layout of a player's save data (offsets are the save data's
// unless said), store for store as the recompiled functions. Adapters in
// src/hooks/career.cpp (behind --native_career), register steps shared with
// the harness in src/hooks/career.h.

#pragma once

#include <cstdint>
#include <optional>

namespace career {

// The save data's parts.
constexpr uint32_t kStats = 48;                // the stats record
constexpr uint32_t kItemRecords = 0x199F8;     // 400 records of 12 bytes
constexpr uint32_t kItemFlags = 0x1ACE4;       // a 400-bit set
constexpr uint32_t kGarage = 6120;             // the garage's list
constexpr uint32_t kSelectedCar = 0x19920;     // the selected garage car
constexpr uint32_t kUnlockSet = 0x19930;       // the unlocks the rewards set
// The play time's frame counter (PlayTime::Add measures from it).
constexpr uint32_t kPlayTimeFrames = 0x826C5B90;
// The float the set-ups store (0.0, at 0x82000A34 in the xex).
constexpr uint32_t kZeroFloat = 0x82000A34;

// Whether `size` bytes copy natively (memcpy's plain C++): below the
// physical views (memcpy's override records those for the renderer's
// mirrors) and not sharing memory with the source.
bool CanCopy(uint32_t destination, uint32_t source, uint32_t size);

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

// --- A race's outcome against an item (an opponent) ---

// What ItemRecords::Add* build on their stack ({id, the item's record, the
// item}) and hand their callees by pointer: here by value.
struct Opponent {
  uint32_t id;
  uint32_t record;
  uint32_t item;
};
enum class Outcome { kLoss = 0, kWin = 1, kOutcome3 = 2 };

// sub_82111E10 Counter::AddWin, sub_82111E80 Counter::AddLoss, sub_82111EF0
// Counter::AddOutcome3: a 24-byte counter block's count (+12, +14, +16) up;
// on the first such outcome against the item (its record's +2, +0, +4 still
// 0) and its byte +35 not negative, bit +35 of the set at +0, +4, +8; +22
// (races) up. Returns the counter (r3 as the originals leave it).
uint32_t CounterAddWin(uint8_t* base, uint32_t counter, const Opponent& o);
uint32_t CounterAddLoss(uint8_t* base, uint32_t counter, const Opponent& o);
uint32_t CounterAddOutcome3(uint8_t* base, uint32_t counter, const Opponent& o);
// sub_82111F60 WeekCounter::AddLoss: Counter::AddLoss, then by the item's
// byte +35 (signed): 4 +56 up, 5 +58 up. Returns the block.
uint32_t WeekCounterAddLoss(uint8_t* base, uint32_t week, const Opponent& o);
// sub_821134D0 Opponents::AddWin, sub_82113580 Opponents::AddLoss,
// sub_82113628 Opponents::AddOutcome3: the day block's counter; its
// halfword +1628 + 8 x id + 2, + 0, + 4 up (at most 999); the id appended to
// the list at +24 (count +1624) when not in it. Returns the block.
uint32_t OpponentsAddWin(uint8_t* base, uint32_t block, const Opponent& o);
uint32_t OpponentsAddLoss(uint8_t* base, uint32_t block, const Opponent& o);
uint32_t OpponentsAddOutcome3(uint8_t* base, uint32_t block, const Opponent& o);
// sub_821139B8 Stats::AddWin, sub_82113AB8 Stats::AddLoss, sub_82113B50
// Stats::AddOutcome3: the stats record's counters, streaks, the week's and
// the day's blocks and the counts by mode. Return stats + 4924 (r3 as the
// originals leave it).
uint32_t StatsAddWin(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode);
uint32_t StatsAddLoss(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode);
uint32_t StatsAddOutcome3(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode);
// sub_82114B88 ItemRecords::AddWin, sub_82114B08 ItemRecords::AddLoss,
// sub_82114A88 ItemRecords::AddOutcome3: the Opponent built in their
// 144-byte frame (below `sp`, at +80, as the originals), Stats::Add* (mode
// save +6104), then the item's record +2, +0, +4 up (at most 999). Return
// Stats::Add*'s r3.
constexpr uint32_t kItemRecordsFrame = 144;
uint32_t ItemRecordsAddWin(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp);
uint32_t ItemRecordsAddLoss(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp);
uint32_t ItemRecordsAddOutcome3(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp);

// --- The last results and the day log ---

// sub_82110850 ResultRecord::Fill: a 48-byte record from the race data's
// last result (RacerRecords::At(race + 0x59AE8, -1)). Returns that result.
uint32_t ResultRecordFill(uint8_t* base, uint32_t out, uint32_t race);
// sub_82111FC8 Results::Push: the 14 records moved down (memcpy's copy, from
// the end), the newest filled at +4, the count (+0) up to 15. Returns
// ResultRecord::Fill's. ResultsPushNative: whether the copies are native
// (else the original runs; nothing written).
bool ResultsPushNative(uint32_t list);
uint32_t ResultsPush(uint8_t* base, uint32_t list, uint32_t race);
// sub_82112028 DayLog::Push: 10 records of 12 bytes moved up, the newest
// {the day block's +22, float +4836, +18}, the count up to 10. Returns the
// log.
uint32_t DayLogPush(uint8_t* base, uint32_t log, uint32_t day);
// sub_82112088 Stats::StartDay: on weekday 0 the week's block cleared;
// DayLog::Push(+5672, the day block); the day block cleared; the streaks and
// the day's sums reset. Returns the day block (memset's r3).
uint32_t StatsStartDay(uint8_t* base, uint32_t stats, uint32_t weekday);

// --- The garage's list ---

// sub_82113468 GarageList::Insert: a node from the free list (+200) linked
// in after `position` - 1 steps from the head (+204; at the head for 0),
// the count (+208) up. Returns the list.
uint32_t GarageListInsert(uint8_t* base, uint32_t list, uint32_t position);
// sub_821154E0 Garage::InsertCar: GarageList::Insert, then the 1,968-byte
// car copied into the slot of the node at `position` (+212 + 1,968 x slot).
// `destination` is that slot (memcpy's r3); false where memcpy's plain C++
// can't copy it (the caller then runs memcpy).
bool GarageInsertCar(uint8_t* base, uint32_t garage, uint32_t car, uint32_t position,
                     uint32_t* destination);

// --- The unlocks' conditions and the rewards ---

// sub_82112950 Condition::Test (with its jump table's case sub_821129E0
// merged): whether an 8-byte condition {kind 0 to 9, value} holds for
// player 0's save data, `location` and `mask` (the item kinds tested when
// one of bits 10 to 14 is set). The item counts (7, 8, 9) copy each item
// onto its 3,168-byte frame below `sp` (at +80; the last copy left there).
// nullopt, with nothing written, where those copies can't be memcpy's plain
// C++ (the caller then runs the original). Returns 0 or 1.
constexpr uint32_t kConditionTestFrame = 3168;
std::optional<uint32_t> ConditionTest(uint8_t* base, uint32_t condition, uint32_t location,
                                      uint32_t mask, uint32_t sp);
// sub_82114C08 Rewards::Grant (with its jump table's case sub_82114C7C
// merged): bit `index` of +0x1D508 set; then by `kind` (0 to 41) bits at
// `value` plus offsets set in the unlock set (+0x19930; 38 the set at
// +0x19924, 40 +0x199A8, 41 +0x199C4), the first tested; when it was clear
// (or negative), and always for 39 (38 unless already set), {0, 8, index}
// appended to the list at +0x1CA84 (count +0x1D504). Returns the save data
// (r3 as the original leaves it).
uint32_t RewardsGrant(uint8_t* base, uint32_t save, uint32_t index, uint32_t kind,
                      uint32_t value);

}  // namespace career
