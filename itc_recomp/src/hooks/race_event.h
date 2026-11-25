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
  RaceEventGuest(lifted::Regs& ctx);
  const race_event::Guest& get() const { return guest_; }

 private:
  static uint32_t Test(void* user, uint32_t function, uint32_t condition, uint32_t level,
                       uint32_t depth);
  lifted::Regs& ctx_;
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
void RaceEventMake(lifted::Regs& ctx);
void RaceEventDestroy(lifted::Regs& ctx);
void RaceEventPlaceUnlockedCurrent(lifted::Regs& ctx);
void RaceEventSectionRivalCurrent(lifted::Regs& ctx);
void RaceEventItemsAvailable(lifted::Regs& ctx);
void RaceEventGroupItem(lifted::Regs& ctx);

// The functions only native callers reach (the harness's), with the
// original's registers.
void RaceEventConditionMet(lifted::Regs& ctx);
void RaceEventConditionState(lifted::Regs& ctx);
void RaceEventUnlockAll(lifted::Regs& ctx);
void RaceEventUnlockAny(lifted::Regs& ctx);
void RaceEventUnlockState(lifted::Regs& ctx);
void RaceEventUnlockStateKind2(lifted::Regs& ctx);
void RaceEventUnlockStateKind5(lifted::Regs& ctx);
void RaceEventHasSectionCondition(lifted::Regs& ctx);
void RaceEventCategoryMask(lifted::Regs& ctx);
void RaceEventFloatAtLeast(lifted::Regs& ctx);
void RaceEventConstruct(lifted::Regs& ctx);
void RaceEventSetMode(lifted::Regs& ctx);
void RaceEventAlloc(lifted::Regs& ctx);
void RaceEventGroupAlloc(lifted::Regs& ctx);
void RaceEventFree(lifted::Regs& ctx);
void RaceEventGroupFree(lifted::Regs& ctx);
void RaceEventPoolsAddItem(lifted::Regs& ctx);
void RaceEventItemsBeaten(lifted::Regs& ctx);
void RaceEventPoolsAddRivals(lifted::Regs& ctx);
void RaceEventPoolsAddRivalsOf(lifted::Regs& ctx);
void RaceEventPoolsTrim(lifted::Regs& ctx);
void RaceEventPoolsToGroup(lifted::Regs& ctx);
void RaceEventPoolsCopyIds(lifted::Regs& ctx);
void RaceEventPoolsToGroupNoList(lifted::Regs& ctx);
void RaceEventInit(lifted::Regs& ctx);
void RaceEventGroupPromoteFlagged(lifted::Regs& ctx);
void RaceEventGroupSortOpen(lifted::Regs& ctx);
void RaceEventGroupPromoteUnlocked(lifted::Regs& ctx);
void RaceEventRefresh(lifted::Regs& ctx);
void RaceEventRefreshCurrent(lifted::Regs& ctx);
void RaceEventUpdateLocation(lifted::Regs& ctx);
void RaceEventFindRacer(lifted::Regs& ctx);
void RaceEventSectionRival(lifted::Regs& ctx);
void RaceEventPlaceUnlocked(lifted::Regs& ctx);

}  // namespace hooks
