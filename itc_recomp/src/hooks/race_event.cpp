// The race event natively: adapters in front of src/race_event/
// (docs/functions/race-event.md), with the register steps the harness
// shares (hooks/race_event.h).

#include "hooks/race_event.h"
#include "lifted/functions.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "race_event/race_event.h"

ITC_FLAG_BOOL(native_race_event, true, "ITC",
              "Run the race event (docs/functions/race-event.md: RaceEvent::Make sub_82144E58 (no "
              "adapter: only --native_career's Career::SetUpEvent calls it, its plain C++) "
              "with RaceEvent::RaceEvent sub_82142F20, Init sub_82144A50, SetMode sub_82142F68, "
              "Alloc sub_82143620, Free sub_821436C8, EventGroup::Alloc and Free sub_821433A8 "
              "and sub_82143358, the pools' AddItem sub_82144950, AddRivals sub_82144458, "
              "AddRivalsOf sub_821440F8, Trim sub_821444D0, ToGroup sub_82144170, CopyIds "
              "sub_82143420, ToGroupNoList sub_821441D8, Items::Beaten sub_82143528 and "
              "Unlock::HasSectionCondition sub_82140308; RaceEvent::Destroy sub_82144DF0; "
              "RaceEvent::PlaceUnlockedCurrent sub_821442D8 with PlaceUnlocked sub_82143740 and "
              "FindRacer sub_82142FC8; RaceEvent::SectionRivalCurrent sub_821434A0 with "
              "SectionRival sub_82143210; Items::Available sub_821435C8 with "
              "EventCondition::Met sub_82142820 (its case sub_82142868 merged) and its tests "
              "(kind 40, EventCondition::Power sub_82141668, left to the translated code), "
              "EventCondition::State sub_82142C00, Unlock::All, Any, State, StateKind2 and "
              "StateKind5 sub_82142C80, sub_82142CE0, sub_82142EC0, sub_82142D40, sub_82142E18; "
              "RaceEvent::GroupItem sub_821431C0; no adapters for the callees: only their "
              "callers' plain C++ calls them; RaceEvent::RefreshCurrent sub_82144720 with "
              "Refresh sub_82144300, EventGroup::PromoteFlagged sub_82143C70 and SortOpen "
              "sub_82143DC8, and RaceEvent::UpdateLocation sub_82144740 with "
              "EventGroup::PromoteUnlocked sub_82143F90, from --native_career's "
              "Career::EventQuery and EventLocation) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=race_event)");

namespace hooks {

namespace re = race_event;

RaceEventGuest::RaceEventGuest(lifted::Regs& ctx)
    : ctx_(ctx), guest_{&RaceEventGuest::Test, this} {}

uint32_t RaceEventGuest::Test(void* user, uint32_t function, uint32_t condition,
                              uint32_t level, uint32_t depth) {
  auto& self = *static_cast<RaceEventGuest*>(user);
  lifted::Regs& ctx = self.ctx_;
  if (function != 0x82141668) {
    ITC_LOG_ERROR("RaceEventGuest: no test {:08X}", function);
    return 0;
  }
  uint32_t r3 = 0;
  // From the original's depth (its callers' frames and Met's), Met's call's
  // return.
  guest_call::InFrame(ctx, depth, 0x82142A58, [&](uint32_t) {
    ctx.r3.u64 = condition;
    ctx.r4.u64 = level;
    lifted::Enter(ctx, lifted::e82141668);  // EventCondition::Power
    r3 = ctx.r3.u32;
  });
  ctx.fpscr.disableFlushMode();
  return r3;
}

ITC_REGS_STEP(RaceEventMake) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64;
  if (re::MakeNative()) {
    RaceEventGuest guest(ctx);
    const auto made = re::Make(guest::LoadU32(uint32_t(r3)),
                               guest::LoadU32(uint32_t(r4)), uint32_t(r5), guest.get());
    if (made) {
      ctx.r3.u64 = *made;
      return;
    }
  }
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.r5.u64 = r5;
  lifted::original::c82144E58(ctx);
}

ITC_REGS_STEP(RaceEventDestroy) {
  if (!re::DestroyNative()) {
    lifted::original::c82144DF0(ctx);
    return;
  }
  re::Destroy();
}

ITC_REGS_STEP(RaceEventPlaceUnlockedCurrent) {
  ctx.fpscr.disableFlushMode();
  const uint32_t position = ctx.r3.u32, flag = ctx.r4.u32, sp = ctx.r1.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::PlaceUnlockedCurrent(position, flag, sp, guest.get());
}

ITC_REGS_STEP(RaceEventSectionRivalCurrent) {
  ctx.fpscr.disableFlushMode();
  const uint32_t position = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::SectionRivalCurrent(position, guest.get());
}

ITC_REGS_STEP(RaceEventItemsAvailable) {
  ctx.fpscr.disableFlushMode();
  const uint32_t id = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::ItemsAvailable(id, guest.get());
}

ITC_REGS_STEP(RaceEventGroupItem) {
  ctx.r3.u64 = re::GroupItem(ctx.r3.u32, guest::LoadU32(ctx.r4.u32),
                             guest::LoadU32(ctx.r5.u32));
}

// --- No adapters: the harness's steps ---

ITC_REGS_STEP(RaceEventConditionMet) {
  ctx.fpscr.disableFlushMode();
  const uint32_t condition = ctx.r3.u32, level = ctx.r4.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::ConditionMet(condition, level, guest.get());
}

ITC_REGS_STEP(RaceEventConditionState) {
  ctx.fpscr.disableFlushMode();
  const uint32_t condition = ctx.r3.u32, level = ctx.r4.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::ConditionState(condition, level, guest.get());
}

ITC_REGS_STEP(RaceEventUnlockAll) {
  ctx.fpscr.disableFlushMode();
  const uint32_t block = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::UnlockAll(block, guest.get());
}

ITC_REGS_STEP(RaceEventUnlockAny) {
  ctx.fpscr.disableFlushMode();
  const uint32_t block = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::UnlockAny(block, guest.get());
}

ITC_REGS_STEP(RaceEventUnlockState) {
  ctx.fpscr.disableFlushMode();
  const uint32_t item = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::UnlockState(item, guest.get());
}

ITC_REGS_STEP(RaceEventUnlockStateKind2) {
  ctx.fpscr.disableFlushMode();
  const uint32_t item = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::UnlockStateKind2(item, guest.get());
}

ITC_REGS_STEP(RaceEventUnlockStateKind5) {
  ctx.fpscr.disableFlushMode();
  const uint32_t item = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::UnlockStateKind5(item, guest.get());
}

ITC_REGS_STEP(RaceEventHasSectionCondition) {
  ctx.r3.u64 = re::HasSectionCondition(ctx.r3.u32);
}

ITC_REGS_STEP(RaceEventCategoryMask) {
  ctx.r3.u64 = re::CategoryMask(guest::LoadU16(ctx.r3.u32),
                                guest::LoadU32(ctx.r4.u32), guest::LoadU32(ctx.r5.u32));
}

ITC_REGS_STEP(RaceEventFloatAtLeast) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = re::FloatAtLeast(ctx.r3.u32, guest::LoadFloat(ctx.r4.u32));
}

ITC_REGS_STEP(RaceEventConstruct) { re::Construct(ctx.r3.u32); }

ITC_REGS_STEP(RaceEventSetMode) {
  re::SetMode(ctx.r3.u32, guest::LoadU32(ctx.r4.u32),
              guest::LoadU32(ctx.r5.u32));
}

ITC_REGS_STEP(RaceEventAlloc) { ctx.r3.u64 = re::Alloc(ctx.r3.u32); }

ITC_REGS_STEP(RaceEventGroupAlloc) {
  ctx.r3.u64 = re::GroupAlloc(ctx.r3.u32);
}

ITC_REGS_STEP(RaceEventFree) { re::Free(ctx.r3.u32); }

ITC_REGS_STEP(RaceEventGroupFree) { re::GroupFree(ctx.r3.u32); }

ITC_REGS_STEP(RaceEventPoolsAddItem) {
  ctx.fpscr.disableFlushMode();
  const uint32_t pools = ctx.r3.u32, item = ctx.r4.u32, records = ctx.r6.u32;
  RaceEventGuest guest(ctx);
  re::PoolsAddItem(pools, item, records, guest.get());
}

ITC_REGS_STEP(RaceEventItemsBeaten) {
  ctx.r3.u64 = re::ItemsBeaten(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceEventPoolsAddRivals) {
  re::PoolsAddRivals(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceEventPoolsAddRivalsOf) {
  re::PoolsAddRivalsOf(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceEventPoolsTrim) {
  re::PoolsTrim(ctx.r3.u32, guest::LoadU16(ctx.r4.u32));
}

ITC_REGS_STEP(RaceEventPoolsToGroup) {
  uint32_t index = guest::LoadU32(ctx.r6.u32);
  re::PoolsToGroup(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, index);
  guest::StoreU32(ctx.r6.u32, index);
}

ITC_REGS_STEP(RaceEventPoolsCopyIds) {
  uint32_t index = guest::LoadU32(ctx.r6.u32);
  re::PoolsCopyIds(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, index,
                   guest::LoadU32(ctx.r7.u32));
  guest::StoreU32(ctx.r6.u32, index);
}

ITC_REGS_STEP(RaceEventPoolsToGroupNoList) {
  uint32_t index = guest::LoadU32(ctx.r5.u32);
  re::PoolsToGroupNoList(ctx.r3.u32, ctx.r4.u32, index);
  guest::StoreU32(ctx.r5.u32, index);
}

ITC_REGS_STEP(RaceEventInit) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64;
  RaceEventGuest guest(ctx);
  const auto made = re::Init(uint32_t(r3), guest::LoadU32(uint32_t(r4)),
                             guest::LoadU32(uint32_t(r5)), uint32_t(r6), guest.get());
  if (made) {
    ctx.r3.u64 = *made;
    return;
  }
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.r5.u64 = r5;
  ctx.r6.u64 = r6;
  lifted::original::c82144A50(ctx);
}

ITC_REGS_STEP(RaceEventGroupPromoteFlagged) {
  re::GroupPromoteFlagged(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceEventGroupSortOpen) {
  re::GroupSortOpen(ctx.r3.u32);
}

ITC_REGS_STEP(RaceEventGroupPromoteUnlocked) {
  ctx.fpscr.disableFlushMode();
  const uint32_t group = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  re::GroupPromoteUnlocked(group, guest.get());
}

ITC_REGS_STEP(RaceEventRefresh) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::Refresh(event, guest.get());
}

ITC_REGS_STEP(RaceEventRefreshCurrent) {
  ctx.fpscr.disableFlushMode();
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::RefreshCurrent(guest.get());
}

ITC_REGS_STEP(RaceEventUpdateLocation) {
  ctx.fpscr.disableFlushMode();
  const uint32_t location = guest::LoadU32(ctx.r3.u32);
  RaceEventGuest guest(ctx);
  re::UpdateLocation(location, guest.get());
}

ITC_REGS_STEP(RaceEventFindRacer) {
  uint32_t out = 0;
  const uint32_t r = re::FindRacer(ctx.r3.u32, out);
  if (r != 0) {
    guest::StoreU32(ctx.r4.u32, out);
  }
  ctx.r3.u64 = r;
}

ITC_REGS_STEP(RaceEventSectionRival) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32, position = ctx.r4.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::SectionRival(event, position, guest.get());
}

ITC_REGS_STEP(RaceEventPlaceUnlocked) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32, position = ctx.r4.u32, flag = ctx.r5.u32, sp = ctx.r1.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = re::PlaceUnlocked(event, position, flag, sp, guest.get());
}

}  // namespace hooks

#define RACE_EVENT_ADAPTER(address, step)  \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_event)) { \
    hooks::step(ctx);                \
  }

RACE_EVENT_ADAPTER(82144DF0, RaceEventDestroy)
RACE_EVENT_ADAPTER(821442D8, RaceEventPlaceUnlockedCurrent)
RACE_EVENT_ADAPTER(821434A0, RaceEventSectionRivalCurrent)
RACE_EVENT_ADAPTER(821435C8, RaceEventItemsAvailable)
RACE_EVENT_ADAPTER(821431C0, RaceEventGroupItem)

// For the fallback originals (docs/missing-functions.md)
ITC_DIRECT_ENTRY(82142EC0) { hooks::RaceEventUnlockState(ctx); }
ITC_DIRECT_ENTRY(82143358) { hooks::RaceEventGroupFree(ctx); }
ITC_DIRECT_ENTRY(821433A8) { hooks::RaceEventGroupAlloc(ctx); }
ITC_DIRECT_ENTRY(82143420) { hooks::RaceEventPoolsCopyIds(ctx); }
ITC_DIRECT_ENTRY(82143528) { hooks::RaceEventItemsBeaten(ctx); }
ITC_DIRECT_ENTRY(82144458) { hooks::RaceEventPoolsAddRivals(ctx); }
ITC_DIRECT_ENTRY(82140400) { hooks::RaceEventFloatAtLeast(ctx); }
ITC_DIRECT_ENTRY(82140308) { hooks::RaceEventHasSectionCondition(ctx); }
ITC_DIRECT_ENTRY(82142F20) { hooks::RaceEventConstruct(ctx); }
ITC_DIRECT_ENTRY(82142F68) { hooks::RaceEventSetMode(ctx); }
ITC_DIRECT_ENTRY(82143620) { hooks::RaceEventAlloc(ctx); }
ITC_DIRECT_ENTRY(821436C8) { hooks::RaceEventFree(ctx); }
ITC_DIRECT_ENTRY(82144170) { hooks::RaceEventPoolsToGroup(ctx); }
ITC_DIRECT_ENTRY(821441D8) { hooks::RaceEventPoolsToGroupNoList(ctx); }
ITC_DIRECT_ENTRY(821444D0) { hooks::RaceEventPoolsTrim(ctx); }
ITC_DIRECT_ENTRY(82144950) { hooks::RaceEventPoolsAddItem(ctx); }
ITC_DIRECT_ENTRY(82144A50) { hooks::RaceEventInit(ctx); }
ITC_DIRECT_ENTRY(82144E58) { hooks::RaceEventMake(ctx); }
