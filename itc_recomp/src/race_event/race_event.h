// The race event natively (docs/functions/race-event.md): plain C++ over the
// guest layout of the event (160 bytes at [0x826C6078]), its pools and groups,
// the data tables' items and a player's save data, store for store as the
// recompiled functions. Adapters in src/hooks/race_event.cpp (behind
// --native_race_event), register steps shared with the harness in
// src/hooks/race_event.h.

#pragma once

#include <cstdint>
#include <optional>

namespace race_event {

constexpr uint32_t kEvent = 0x826C6078;      // the race event (a pointer)
constexpr uint32_t kModeMasks = 0x82008DF4;  // words by mode: the items' mode mask
constexpr uint32_t kCaps = 0x82008E00;       // halfwords: locations 0 to 3's caps
constexpr uint32_t kStages = 0x82008E08;     // bytes: the stage each location needs
constexpr uint32_t kLocationGroups = 0x82008D74;  // halfwords: a location's group
constexpr uint32_t kModeWords = 0x82008D88;       // words by mode (kinds 25, 26)
constexpr uint32_t kRaceData = 0x826C5B2C;        // the race data (a pointer)
constexpr uint32_t kRoute = 0x826C5B10;           // the course's route
constexpr uint32_t kManager = 0x826D82F4;         // the racers' manager (a pointer)
// The floats PlaceUnlocked and SectionRival step positions by.
constexpr uint32_t kBackStep = 0x82001FB0;
constexpr uint32_t kSliceUnit = 0x82000A30;
constexpr uint32_t kBackScale = 0x82008E38;
constexpr uint32_t kForwardScale = 0x82008E34;

// The save data's parts read here.
constexpr uint32_t kDay = 6100;
constexpr uint32_t kGarage = 6120;
constexpr uint32_t kSelectedCar = 0x19920;
constexpr uint32_t kItemRecords = 0x199F8;  // 400 records of 12 bytes

// The tests the native code hands back to the recompiled code: kind 40
// (EventCondition::Power, sub_82141668: never seen in play; its power curve's
// sub_820CE7F0 and sub_820CE600 aren't ours yet), run by the adapter's guest
// call with r3 = the condition and r4 = &the level. Returns the test's r3.
// The test reads stack words below it that it never writes, so it runs at
// the original's depth: `depth` is the bytes of the original's frames
// between the adapter's stack pointer and EventCondition::Met's caller, each
// native level adding its original frame (Down).
struct Guest {
  uint32_t (*test)(void* user, uint32_t function, uint32_t condition, uint32_t level,
                   uint32_t depth);
  void* user;
  uint32_t depth = 0;
  Guest Down(uint32_t frame) const {
    Guest g = *this;
    g.depth += frame;
    return g;
  }
};

// --- The conditions (EventCondition::, Unlock::) ---

// sub_82142820 EventCondition::Met, with its case sub_82142868 merged: r3 =
// a condition (20 bytes), `level` = &the item's level (passed to the tests).
// 1 when the kind's answer is the wanted one (+0), kind 0 and kinds outside
// 0 to 67 always, 67 never.
uint32_t ConditionMet(uint8_t* base, uint32_t condition, uint32_t level, const Guest& guest);
// sub_82142C00 EventCondition::State: 1 met; else 2 for the kinds that can
// still be met (6 to 21, 23, 24, 65), 1 for 67, else 0.
uint32_t ConditionState(uint8_t* base, uint32_t condition, uint32_t level, const Guest& guest);
// sub_82142C80 Unlock::All and sub_82142CE0 Unlock::Any: an unlock block's
// (an item's +40) two conditions.
uint32_t UnlockAll(uint8_t* base, uint32_t block, const Guest& guest);
uint32_t UnlockAny(uint8_t* base, uint32_t block, const Guest& guest);
// sub_82142EC0 Unlock::State, with sub_82142D40 Unlock::StateKind2 and
// sub_82142E18 Unlock::StateKind5: an item's unlock state (0, 1 or 2).
uint32_t UnlockState(uint8_t* base, uint32_t item, const Guest& guest);
uint32_t UnlockStateKind2(uint8_t* base, uint32_t item, const Guest& guest);
uint32_t UnlockStateKind5(uint8_t* base, uint32_t item, const Guest& guest);
// sub_82140308 Unlock::HasSectionCondition: either condition of kind 65.
uint32_t HasSectionCondition(uint8_t* base, uint32_t block);
// sub_82140340 EventCondition::CategoryMask: the 64-bit mask of the
// categories whose items all pass (the level, the location's group, the
// mode mask, won), from the values the original reads through its pointers.
uint64_t CategoryMask(uint8_t* base, uint32_t level, uint32_t location, uint32_t modes);
// sub_82140400 EventCondition::FloatAtLeast: `value` (the float its r4
// points at) at least every argument.
uint32_t FloatAtLeast(uint8_t* base, uint32_t condition, float value);
// sub_821435C8 Items::Available: 0 for an item whose +46 is 2, else its
// availability condition (+100) with its level (+42).
uint32_t ItemsAvailable(uint8_t* base, uint32_t id, const Guest& guest);

// --- The event ---

// sub_82142F20 RaceEvent::RaceEvent: the list and the groups' pointers 0.
void Construct(uint8_t* base, uint32_t event);
// sub_82142F68 RaceEvent::SetMode.
void SetMode(uint8_t* base, uint32_t event, uint32_t mode, uint32_t location);
// sub_82143620 RaceEvent::Alloc, with sub_821433A8 EventGroup::Alloc: the
// list and the groups' entries (VirtualAlloc: the allocation layer's pages).
uint32_t Alloc(uint8_t* base, uint32_t event);
uint32_t GroupAlloc(uint8_t* base, uint32_t group);
// sub_821436C8 RaceEvent::Free, with sub_82143358 EventGroup::Free.
void Free(uint8_t* base, uint32_t event);
void GroupFree(uint8_t* base, uint32_t group);
// Whether Free's pages are the allocation layer's (or none), so it frees them
// natively (with the native heap on this thread).
bool FreeNative(uint8_t* base, uint32_t event);
// sub_82144DF0 RaceEvent::Destroy. Native only where DestroyNative: no event,
// or one whose pages are the layer's (FreeNative) in a heap block.
bool DestroyNative(uint8_t* base);
void Destroy(uint8_t* base);
// Whether Make (and Init under it) runs natively: the native heap on this
// thread, and an existing event's pages the layer's.
bool MakeNative(uint8_t* base);
// sub_82144950 EventPools::AddItem (r5 not read).
void PoolsAddItem(uint8_t* base, uint32_t pools, uint32_t item, uint32_t records,
                  const Guest& guest);
// sub_82143528 Items::Beaten: r3 = the item records, `id` = &an item id.
uint32_t ItemsBeaten(uint8_t* base, uint32_t records, uint32_t id);
// sub_82144458 EventPools::AddRivals and sub_821440F8 EventPools::AddRivalsOf
// (`id` = &an item id).
void PoolsAddRivals(uint8_t* base, uint32_t pools, uint32_t item);
void PoolsAddRivalsOf(uint8_t* base, uint32_t pools, uint32_t id);
// sub_821444D0 EventPools::Trim: `cap` the cap's halfword (the original
// reads it through a pointer once).
void PoolsTrim(uint8_t* base, uint32_t pools, uint16_t cap);
// sub_82144170 EventPools::ToGroup, with sub_82143420 EventPools::CopyIds;
// `index` the running index (in and out).
void PoolsToGroup(uint8_t* base, uint32_t pools, uint32_t group, uint32_t list, uint32_t& index);
void PoolsCopyIds(uint8_t* base, uint32_t ids, uint32_t entries, uint32_t list, uint32_t& index,
                  uint32_t flag);
// sub_821441D8 EventPools::ToGroupNoList.
void PoolsToGroupNoList(uint8_t* base, uint32_t pools, uint32_t group, uint32_t& index);
// sub_82144A50 RaceEvent::Init: r3 as the original's (0 or 1); `out` gets
// RaceEvent::FirstOwned. nullopt where operator new[] answers nullopt (the
// original runs then; nothing it reads was changed).
std::optional<uint32_t> Init(uint8_t* base, uint32_t event, uint32_t mode, uint32_t location,
                             uint32_t out, const Guest& guest);
// sub_82144E58 RaceEvent::Make. nullopt, with nothing done, where operator
// new answers nullopt; Init's nullopt the same way.
std::optional<uint32_t> Make(uint8_t* base, uint32_t mode, uint32_t location, uint32_t out,
                             const Guest& guest);
// sub_82143C70 EventGroup::PromoteFlagged.
void GroupPromoteFlagged(uint8_t* base, uint32_t group, uint32_t list);
// sub_82143DC8 EventGroup::SortOpen.
void GroupSortOpen(uint8_t* base, uint32_t group);
// sub_82143F90 EventGroup::PromoteUnlocked.
void GroupPromoteUnlocked(uint8_t* base, uint32_t group, const Guest& guest);
// sub_82144300 RaceEvent::Refresh and sub_82144720 RaceEvent::RefreshCurrent
// (r3: FirstOwned, sign-extended as the original's; 0 without an event).
uint64_t Refresh(uint8_t* base, uint32_t event, const Guest& guest);
uint64_t RefreshCurrent(uint8_t* base, const Guest& guest);
// sub_82144740 RaceEvent::UpdateLocation (`location` the word it reads).
void UpdateLocation(uint8_t* base, uint32_t location, const Guest& guest);
// sub_821431C0 RaceEvent::GroupItem: `kind` and `index` the words it reads.
uint32_t GroupItem(uint8_t* base, uint32_t event, uint32_t kind, uint32_t index);
// sub_82142FC8 RaceEvent::FindRacer: 0, or 1 or 2 with `out` the manager
// entry's route position.
uint32_t FindRacer(uint8_t* base, uint32_t entry, uint32_t& out);
// sub_82143210 RaceEvent::SectionRival and sub_821434A0
// RaceEvent::SectionRivalCurrent: `position` the route position's address.
// The r3s are the originals' 64 bits (-1 sign-extended, an id word not).
uint64_t SectionRival(uint8_t* base, uint32_t event, uint32_t position, const Guest& guest);
uint64_t SectionRivalCurrent(uint8_t* base, uint32_t position, const Guest& guest);
// sub_82143740 RaceEvent::PlaceUnlocked and sub_821442D8
// RaceEvent::PlaceUnlockedCurrent: `position` and `flag` the addresses it
// reads; `sp` the caller's stack pointer (its frame, 192 bytes, holds the
// position's copy ToMainRoad and the steps use).
uint64_t PlaceUnlocked(uint8_t* base, uint32_t event, uint32_t position, uint32_t flag,
                       uint32_t sp, const Guest& guest);
uint64_t PlaceUnlockedCurrent(uint8_t* base, uint32_t position, uint32_t flag, uint32_t sp,
                              const Guest& guest);
constexpr uint32_t kPlaceUnlockedFrame = 192;

}  // namespace race_event
