// The career's progress natively (docs/functions/career.md): plain C++ over
// the guest layout of a player's save data (offsets are the save data's
// unless said), store for store as the recompiled functions. Adapters in
// src/hooks/career.cpp (behind --native_career), register steps shared with
// the harness in src/hooks/career.h.

#pragma once

#include <cstdint>
#include <optional>

#include "race_event/race_event.h"

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
// sub_82110588 DayBlock::BestStreak: halfword +4830 of the day block (no
// adapter: only EventCondition::SectionRival's plain C++ calls it).
uint32_t DayBlockBestStreak(uint8_t* base, uint32_t block);
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

// sub_821104D8 Car::SetModelAndClass (probably): r3 = a car object, r4 = a
// car id: Car::SetModel (its old +1884 kept first), +1648 = the spec's byte
// +708 as 0 to 3 (0 to 2: 0, 3 and 4: 2, 5 and 6: 1, 7: 3; above 7 the word
// at its frame's +80, which it never writes, read from `sp` - 128 + 80),
// +1884 = 3 if it was 3, else spec +748; +1900 = the spec of the car's +12's
// word +744. Returns that spec (r3 as the original leaves it).
constexpr uint32_t kSetModelAndClassFrame = 128;
uint32_t CarSetModelAndClass(uint8_t* base, uint32_t car, uint32_t id, uint32_t sp);
// sub_82116CE0 Garage::Add: r3 = a player's save data, r4 = a car object:
// Car::SpecWord744, Garage::InsertCar at the count (memcpy through `copy`
// where its plain C++ can't copy), then on the new last car (the count less
// one steps from the head) Car::SetModelAndClass(it, the car's +12) from its
// 128-byte frame below `sp`; with a nonzero spec word, the new car's +1900 =
// it when the new car's own differs. Returns r3 as the original leaves it.
constexpr uint32_t kGarageAddFrame = 128;
using CopyCall = void (*)(void* user, uint32_t destination, uint32_t car);
uint32_t GarageAdd(uint8_t* base, uint32_t save, uint32_t car, uint32_t sp, CopyCall copy,
                   void* user);

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
// Whether Condition::Test's copies from `sp` are memcpy's plain C++ (then it
// never answers nullopt).
bool ConditionTestFits(uint8_t* base, uint32_t sp);
// sub_82114C08 Rewards::Grant (with its jump table's case sub_82114C7C
// merged): bit `index` of +0x1D508 set; then by `kind` (0 to 41) bits at
// `value` plus offsets set in the unlock set (+0x19930; 38 the set at
// +0x19924, 40 +0x199A8, 41 +0x199C4), the first tested; when it was clear
// (or negative), and always for 39 (38 unless already set), {0, 8, index}
// appended to the list at +0x1CA84 (count +0x1D504). Returns the save data
// (r3 as the original leaves it).
uint32_t RewardsGrant(uint8_t* base, uint32_t save, uint32_t index, uint32_t kind,
                      uint32_t value);

// --- A race's result ---

// sub_82112148 RunningMean::MergeTwo: RunningMean::Merge(stats +4892) and
// (+5904) from its 112-byte frame below `sp`. Returns stats + 5904.
constexpr uint32_t kMergeTwoFrame = 112;
uint32_t RunningMeanMergeTwo(uint8_t* base, uint32_t stats, uint64_t packed, uint32_t sp);
// sub_82115190 Stats::AddRaceResult: the player's last result (the race
// data's RacerRecords::At(+0x59AE8, -1)) into the stats record for the race
// modes 2 and 3 (+0xA3AF4): the week's sums, the bests, the running means
// (on its 3,184-byte frame below `sp`, +80, with the conversions at +88, as
// the original), Results::Push, the stage's means; then each opponent's item
// record by the outcome (ItemRecords::AddWin, the item copied to frame +96
// and save +6048 = 1 for its byte +35 of 5; AddOutcome3; AddLoss, with the
// stats' +18 and the mode's +8 for +3920 of 3). AddRaceResultNative: whether
// its copies are memcpy's plain C++ (else the original runs; nothing
// written). Returns r3 as the original leaves it.
constexpr uint32_t kAddRaceResultFrame = 3184;
bool AddRaceResultNative(uint8_t* base, uint32_t save, uint32_t race, uint32_t sp);
uint32_t StatsAddRaceResult(uint8_t* base, uint32_t save, uint32_t race, uint32_t sp);

// sub_821159B0 Career::PickNext's gate: true when it does nothing (the stats'
// races +70 below 15, or the day +6100 below 3); past it (never seen in
// play) the original runs.
bool PickNextSkips(uint8_t* base, uint32_t save);

// --- The progress after a race, the day ---

// The stage tests (no arguments read: player 0's records): sub_82111818
// Items::AnyWonKind3Stage2 (+42 2, +36 1, +35 3, any won); sub_821118B0
// Items::AllWonKind3 (+42 1, +35 3), sub_82111940 Items::AllWonKind4Stage2
// (+42 2, +35 4), sub_821119D0 Items::AllWonKind4Stage3 (+42 3, +35 4),
// sub_82111A60 Items::AllWonKind5 (+35 5): 0 when such an item has no win,
// else 1.
uint32_t AnyWonKind3Stage2(uint8_t* base);
uint32_t AllWonKind3(uint8_t* base);
uint32_t AllWonKind4Stage2(uint8_t* base);
uint32_t AllWonKind4Stage3(uint8_t* base);
uint32_t AllWonKind5(uint8_t* base);
// sub_82111D20 Locations::Unlock, sub_82111D98 Locations::Unlock2: bit `bit`
// (when not negative) set in +0x199E0 or +0x199E8.
void LocationsUnlock(uint8_t* base, uint32_t save, uint32_t bit);
void LocationsUnlock2(uint8_t* base, uint32_t save, uint32_t bit);
// sub_82116588 Progress::AfterRace: the race data's totals into the stats,
// the table-0 unlocks and the 40-byte rewards (Condition::Test from its
// 160-byte frame below `sp`), the stage reached and its locations, the
// location put back. Returns the stage reached (2 to 8) or -1.
// AfterRaceNative: whether Condition::Test's copies are native (else the
// original runs; nothing written).
constexpr uint32_t kAfterRaceFrame = 160;
bool AfterRaceNative(uint8_t* base, uint32_t sp);
uint32_t ProgressAfterRace(uint8_t* base, uint32_t save, uint32_t sp);
// sub_82113898 Stats::CloseDay: +6028 (days) up; running means from its
// 128-byte frame below `sp` (+80, the conversions at +88, as the original):
// +5936 from +5960, +5944 from +5964, +5928 from +5952 x 100 / +5956 (a
// division by 0, a trap the translated code logs and runs on, gives 0).
// Returns stats + 5928.
constexpr uint32_t kCloseDayFrame = 128;
uint32_t StatsCloseDay(uint8_t* base, uint32_t stats, uint32_t sp);
// sub_82116AF0 Career::NextDay: the day up, each garage car's +1896 up, save
// +0x1ACE0 = 0, Stats::CloseDay, Career::PickNext(save, 0) (its gate
// natively, past it `pick_next(user, save)`, the original), Stats::StartDay
// (the day % 7). Returns Stats::StartDay's.
constexpr uint32_t kNextDayFrame = 112;
using PickNextCall = void (*)(void* user, uint32_t save);
uint32_t CareerNextDay(uint8_t* base, uint32_t save, uint32_t sp, PickNextCall pick_next,
                       void* user);

// --- The race event's refreshes (docs/functions/race-event.md) ---

// sub_82111638 Career::EventQuery: -1 with `flag` set, else
// RaceEvent::RefreshCurrent (r3 as the original's, 64 bits).
uint64_t EventQuery(uint8_t* base, uint32_t flag, const race_event::Guest& guest);
// sub_82111650 Career::EventLocation: the location (+6108; 14 to 19 give 4 to
// 9, else 4) to RaceEvent::UpdateLocation (from a word on its frame in the
// original).
void EventLocation(uint8_t* base, uint32_t save, const race_event::Guest& guest);

// --- The day's offers and the race event (src/career/event_setup.cpp) ---

// sub_82114268 Career::SetUpEvent: r3 = the save data, the mode, the
// location, from its 121,200-byte frame below `sp` (the original's: the
// locations' bit set at +80, the caps at +96, the counts at +160, the picks
// at +480, the entries at +1280; mode and location stored at sp + 28 and
// sp + 36, the caller's frame). The mode, the location and 48 into the save
// data, RaceData::ResetGlobal; unless the day is 1, table 0's unlocks (for a
// location 14 to 19), tables 1 to 7's entries collected by kind and
// location, up to 8 picked a location into the offers (+0x1AD20), then
// RaceEvent::Make (its plain C++; `make(user, frame)` runs the original from
// the frame where the plain C++ refuses). Returns r3 as the original leaves
// it. SetUpEventNative: whether Condition::Test's copies and
// RaceData::ResetGlobal's are native from the frame (else the original runs;
// nothing written).
constexpr uint32_t kSetUpEventFrame = 121200;
using MakeCall = uint64_t (*)(void* user, uint32_t frame);
bool SetUpEventNative(uint8_t* base, uint32_t sp);
uint64_t SetUpEvent(uint8_t* base, uint32_t save, uint32_t mode, uint32_t location, uint32_t sp,
                    const race_event::Guest& guest, MakeCall make, void* user);

}  // namespace career
