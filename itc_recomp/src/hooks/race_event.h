// The race event's register steps (src/race_event/,
// docs/functions/race-event.md), shared by the adapters
// (src/hooks/race_event.cpp, behind --native_race_event) and the harness
// (without the flag; for the functions with no adapter, only the harness).
// Each leaves r3 as the original does and turns flush-to-zero off first.
// The tests handed back to the recompiled code (kind 40) run through
// RaceEventGuest's guest call.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "race_event/race_event.h"

namespace hooks {

// The Guest the race event's plain C++ takes: its tests run through `ctx`,
// `depth` bytes below the stack pointer (Guest's depth plus Met's frame).
class RaceEventGuest {
 public:
  RaceEventGuest(lifted::Regs& ctx, uint8_t* base);
  const race_event::Guest& get() const { return guest_; }

 private:
  static uint32_t Test(void* user, uint32_t function, uint32_t condition, uint32_t level,
                       uint32_t depth);
  lifted::Regs& ctx_;
  uint8_t* base_;
  race_event::Guest guest_;
};

// RaceEvent::Make sub_82144E58 (r3 = &the mode, r4 = &the location, r5 =
// the out word): no adapter (only Career::SetUpEvent's plain C++ calls it);
// the harness's step. The functions with adapters (their recompiled
// callers): RaceEvent::Destroy sub_82144DF0, RaceEvent::PlaceUnlockedCurrent
// sub_821442D8 (r3 = &the position, r4 = &the flag), RaceEvent::
// SectionRivalCurrent sub_821434A0 (r3 = &the position), Items::Available
// sub_821435C8 (r3 = the id), RaceEvent::GroupItem sub_821431C0 (r3 = the
// event, r4 = &the kind, r5 = &the index). The original where the plain C++
// refuses (the memory not the layer's).
void RaceEventMake(PPCContext& ctx, uint8_t* base);
void RaceEventMake(lifted::Regs& ctx, uint8_t* base);
void RaceEventDestroy(PPCContext& ctx, uint8_t* base);
void RaceEventDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceEventPlaceUnlockedCurrent(PPCContext& ctx, uint8_t* base);
void RaceEventPlaceUnlockedCurrent(lifted::Regs& ctx, uint8_t* base);
void RaceEventSectionRivalCurrent(PPCContext& ctx, uint8_t* base);
void RaceEventSectionRivalCurrent(lifted::Regs& ctx, uint8_t* base);
void RaceEventItemsAvailable(PPCContext& ctx, uint8_t* base);
void RaceEventItemsAvailable(lifted::Regs& ctx, uint8_t* base);
void RaceEventGroupItem(PPCContext& ctx, uint8_t* base);
void RaceEventGroupItem(lifted::Regs& ctx, uint8_t* base);

// The functions only native callers reach (the harness's), with the
// original's registers.
void RaceEventConditionMet(PPCContext& ctx, uint8_t* base);
void RaceEventConditionMet(lifted::Regs& ctx, uint8_t* base);
void RaceEventConditionState(PPCContext& ctx, uint8_t* base);
void RaceEventConditionState(lifted::Regs& ctx, uint8_t* base);
void RaceEventUnlockAll(PPCContext& ctx, uint8_t* base);
void RaceEventUnlockAll(lifted::Regs& ctx, uint8_t* base);
void RaceEventUnlockAny(PPCContext& ctx, uint8_t* base);
void RaceEventUnlockAny(lifted::Regs& ctx, uint8_t* base);
void RaceEventUnlockState(PPCContext& ctx, uint8_t* base);
void RaceEventUnlockState(lifted::Regs& ctx, uint8_t* base);
void RaceEventUnlockStateKind2(PPCContext& ctx, uint8_t* base);
void RaceEventUnlockStateKind2(lifted::Regs& ctx, uint8_t* base);
void RaceEventUnlockStateKind5(PPCContext& ctx, uint8_t* base);
void RaceEventUnlockStateKind5(lifted::Regs& ctx, uint8_t* base);
void RaceEventHasSectionCondition(PPCContext& ctx, uint8_t* base);
void RaceEventHasSectionCondition(lifted::Regs& ctx, uint8_t* base);
void RaceEventCategoryMask(PPCContext& ctx, uint8_t* base);
void RaceEventCategoryMask(lifted::Regs& ctx, uint8_t* base);
void RaceEventFloatAtLeast(PPCContext& ctx, uint8_t* base);
void RaceEventFloatAtLeast(lifted::Regs& ctx, uint8_t* base);
void RaceEventConstruct(PPCContext& ctx, uint8_t* base);
void RaceEventConstruct(lifted::Regs& ctx, uint8_t* base);
void RaceEventSetMode(PPCContext& ctx, uint8_t* base);
void RaceEventSetMode(lifted::Regs& ctx, uint8_t* base);
void RaceEventAlloc(PPCContext& ctx, uint8_t* base);
void RaceEventAlloc(lifted::Regs& ctx, uint8_t* base);
void RaceEventGroupAlloc(PPCContext& ctx, uint8_t* base);
void RaceEventGroupAlloc(lifted::Regs& ctx, uint8_t* base);
void RaceEventFree(PPCContext& ctx, uint8_t* base);
void RaceEventFree(lifted::Regs& ctx, uint8_t* base);
void RaceEventGroupFree(PPCContext& ctx, uint8_t* base);
void RaceEventGroupFree(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsAddItem(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsAddItem(lifted::Regs& ctx, uint8_t* base);
void RaceEventItemsBeaten(PPCContext& ctx, uint8_t* base);
void RaceEventItemsBeaten(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsAddRivals(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsAddRivals(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsAddRivalsOf(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsAddRivalsOf(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsTrim(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsTrim(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsToGroup(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsToGroup(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsCopyIds(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsCopyIds(lifted::Regs& ctx, uint8_t* base);
void RaceEventPoolsToGroupNoList(PPCContext& ctx, uint8_t* base);
void RaceEventPoolsToGroupNoList(lifted::Regs& ctx, uint8_t* base);
void RaceEventInit(PPCContext& ctx, uint8_t* base);
void RaceEventInit(lifted::Regs& ctx, uint8_t* base);
void RaceEventGroupPromoteFlagged(PPCContext& ctx, uint8_t* base);
void RaceEventGroupPromoteFlagged(lifted::Regs& ctx, uint8_t* base);
void RaceEventGroupSortOpen(PPCContext& ctx, uint8_t* base);
void RaceEventGroupSortOpen(lifted::Regs& ctx, uint8_t* base);
void RaceEventGroupPromoteUnlocked(PPCContext& ctx, uint8_t* base);
void RaceEventGroupPromoteUnlocked(lifted::Regs& ctx, uint8_t* base);
void RaceEventRefresh(PPCContext& ctx, uint8_t* base);
void RaceEventRefresh(lifted::Regs& ctx, uint8_t* base);
void RaceEventRefreshCurrent(PPCContext& ctx, uint8_t* base);
void RaceEventRefreshCurrent(lifted::Regs& ctx, uint8_t* base);
void RaceEventUpdateLocation(PPCContext& ctx, uint8_t* base);
void RaceEventUpdateLocation(lifted::Regs& ctx, uint8_t* base);
void RaceEventFindRacer(PPCContext& ctx, uint8_t* base);
void RaceEventFindRacer(lifted::Regs& ctx, uint8_t* base);
void RaceEventSectionRival(PPCContext& ctx, uint8_t* base);
void RaceEventSectionRival(lifted::Regs& ctx, uint8_t* base);
void RaceEventPlaceUnlocked(PPCContext& ctx, uint8_t* base);
void RaceEventPlaceUnlocked(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
