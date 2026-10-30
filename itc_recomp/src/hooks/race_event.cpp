// The race event natively: adapters in front of src/race_event/
// (docs/functions/race-event.md), with the register steps the harness
// shares (hooks/race_event.h).

#include "hooks/race_event.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "race_event/race_event.h"

ITC_FLAG_BOOL(native_race_event, true, "ITC",
              "Run the race event (docs/functions/race-event.md: RaceEvent::Make sub_82144E58 "
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

RaceEventGuest::RaceEventGuest(PPCContext& ctx, uint8_t* base)
    : ctx_(ctx), base_(base), guest_{&RaceEventGuest::Test, this} {}

uint32_t RaceEventGuest::Test(void* user, uint32_t function, uint32_t condition,
                              uint32_t level, uint32_t depth) {
  auto& self = *static_cast<RaceEventGuest*>(user);
  PPCContext& ctx = self.ctx_;
  if (function != 0x82141668) {
    ITC_LOG_ERROR("RaceEventGuest: no test {:08X}", function);
    return 0;
  }
  uint32_t r3 = 0;
  // From the original's depth (its callers' frames and Met's), Met's call's
  // return.
  guest_call::InFrame(ctx, self.base_, depth, 0x82142A58, [&](uint32_t) {
    ctx.r3.u64 = condition;
    ctx.r4.u64 = level;
    sub_82141668(ctx, self.base_);  // EventCondition::Power
    r3 = ctx.r3.u32;
  });
  ctx.fpscr.disableFlushMode();
  return r3;
}

void RaceEventMake(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64;
  if (re::MakeNative(base)) {
    RaceEventGuest guest(ctx, base);
    const auto made = re::Make(base, guest::LoadU32(base, uint32_t(r3)),
                               guest::LoadU32(base, uint32_t(r4)), uint32_t(r5), guest.get());
    if (made) {
      ctx.r3.u64 = *made;
      return;
    }
  }
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.r5.u64 = r5;
  __imp__sub_82144E58(ctx, base);
}

void RaceEventDestroy(PPCContext& ctx, uint8_t* base) {
  if (!re::DestroyNative(base)) {
    __imp__sub_82144DF0(ctx, base);
    return;
  }
  re::Destroy(base);
}

void RaceEventPlaceUnlockedCurrent(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t position = ctx.r3.u32, flag = ctx.r4.u32, sp = ctx.r1.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::PlaceUnlockedCurrent(base, position, flag, sp, guest.get());
}

void RaceEventSectionRivalCurrent(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t position = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::SectionRivalCurrent(base, position, guest.get());
}

void RaceEventItemsAvailable(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t id = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::ItemsAvailable(base, id, guest.get());
}

void RaceEventGroupItem(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = re::GroupItem(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                             guest::LoadU32(base, ctx.r5.u32));
}

// --- No adapters: the harness's steps ---

void RaceEventConditionMet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t condition = ctx.r3.u32, level = ctx.r4.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::ConditionMet(base, condition, level, guest.get());
}

void RaceEventConditionState(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t condition = ctx.r3.u32, level = ctx.r4.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::ConditionState(base, condition, level, guest.get());
}

void RaceEventUnlockAll(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t block = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::UnlockAll(base, block, guest.get());
}

void RaceEventUnlockAny(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t block = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::UnlockAny(base, block, guest.get());
}

void RaceEventUnlockState(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t item = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::UnlockState(base, item, guest.get());
}

void RaceEventUnlockStateKind2(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t item = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::UnlockStateKind2(base, item, guest.get());
}

void RaceEventUnlockStateKind5(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t item = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::UnlockStateKind5(base, item, guest.get());
}

void RaceEventHasSectionCondition(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = re::HasSectionCondition(base, ctx.r3.u32);
}

void RaceEventCategoryMask(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = re::CategoryMask(base, guest::LoadU16(base, ctx.r3.u32),
                                guest::LoadU32(base, ctx.r4.u32), guest::LoadU32(base, ctx.r5.u32));
}

void RaceEventFloatAtLeast(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = re::FloatAtLeast(base, ctx.r3.u32, guest::LoadFloat(base, ctx.r4.u32));
}

void RaceEventConstruct(PPCContext& ctx, uint8_t* base) { re::Construct(base, ctx.r3.u32); }

void RaceEventSetMode(PPCContext& ctx, uint8_t* base) {
  re::SetMode(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
              guest::LoadU32(base, ctx.r5.u32));
}

void RaceEventAlloc(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = re::Alloc(base, ctx.r3.u32); }

void RaceEventGroupAlloc(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = re::GroupAlloc(base, ctx.r3.u32);
}

void RaceEventFree(PPCContext& ctx, uint8_t* base) { re::Free(base, ctx.r3.u32); }

void RaceEventGroupFree(PPCContext& ctx, uint8_t* base) { re::GroupFree(base, ctx.r3.u32); }

void RaceEventPoolsAddItem(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t pools = ctx.r3.u32, item = ctx.r4.u32, records = ctx.r6.u32;
  RaceEventGuest guest(ctx, base);
  re::PoolsAddItem(base, pools, item, records, guest.get());
}

void RaceEventItemsBeaten(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = re::ItemsBeaten(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceEventPoolsAddRivals(PPCContext& ctx, uint8_t* base) {
  re::PoolsAddRivals(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceEventPoolsAddRivalsOf(PPCContext& ctx, uint8_t* base) {
  re::PoolsAddRivalsOf(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceEventPoolsTrim(PPCContext& ctx, uint8_t* base) {
  re::PoolsTrim(base, ctx.r3.u32, guest::LoadU16(base, ctx.r4.u32));
}

void RaceEventPoolsToGroup(PPCContext& ctx, uint8_t* base) {
  uint32_t index = guest::LoadU32(base, ctx.r6.u32);
  re::PoolsToGroup(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, index);
  guest::StoreU32(base, ctx.r6.u32, index);
}

void RaceEventPoolsCopyIds(PPCContext& ctx, uint8_t* base) {
  uint32_t index = guest::LoadU32(base, ctx.r6.u32);
  re::PoolsCopyIds(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, index,
                   guest::LoadU32(base, ctx.r7.u32));
  guest::StoreU32(base, ctx.r6.u32, index);
}

void RaceEventPoolsToGroupNoList(PPCContext& ctx, uint8_t* base) {
  uint32_t index = guest::LoadU32(base, ctx.r5.u32);
  re::PoolsToGroupNoList(base, ctx.r3.u32, ctx.r4.u32, index);
  guest::StoreU32(base, ctx.r5.u32, index);
}

void RaceEventInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64;
  RaceEventGuest guest(ctx, base);
  const auto made = re::Init(base, uint32_t(r3), guest::LoadU32(base, uint32_t(r4)),
                             guest::LoadU32(base, uint32_t(r5)), uint32_t(r6), guest.get());
  if (made) {
    ctx.r3.u64 = *made;
    return;
  }
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.r5.u64 = r5;
  ctx.r6.u64 = r6;
  __imp__sub_82144A50(ctx, base);
}

void RaceEventGroupPromoteFlagged(PPCContext& ctx, uint8_t* base) {
  re::GroupPromoteFlagged(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceEventGroupSortOpen(PPCContext& ctx, uint8_t* base) {
  re::GroupSortOpen(base, ctx.r3.u32);
}

void RaceEventGroupPromoteUnlocked(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t group = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  re::GroupPromoteUnlocked(base, group, guest.get());
}

void RaceEventRefresh(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::Refresh(base, event, guest.get());
}

void RaceEventRefreshCurrent(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::RefreshCurrent(base, guest.get());
}

void RaceEventUpdateLocation(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t location = guest::LoadU32(base, ctx.r3.u32);
  RaceEventGuest guest(ctx, base);
  re::UpdateLocation(base, location, guest.get());
}

void RaceEventFindRacer(PPCContext& ctx, uint8_t* base) {
  uint32_t out = 0;
  const uint32_t r = re::FindRacer(base, ctx.r3.u32, out);
  if (r != 0) {
    guest::StoreU32(base, ctx.r4.u32, out);
  }
  ctx.r3.u64 = r;
}

void RaceEventSectionRival(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32, position = ctx.r4.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::SectionRival(base, event, position, guest.get());
}

void RaceEventPlaceUnlocked(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t event = ctx.r3.u32, position = ctx.r4.u32, flag = ctx.r5.u32, sp = ctx.r1.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = re::PlaceUnlocked(base, event, position, flag, sp, guest.get());
}

}  // namespace hooks

#define RACE_EVENT_ADAPTER(address, step)  \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_race_event)) {    \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

RACE_EVENT_ADAPTER(82144E58, RaceEventMake)
RACE_EVENT_ADAPTER(82144DF0, RaceEventDestroy)
RACE_EVENT_ADAPTER(821442D8, RaceEventPlaceUnlockedCurrent)
RACE_EVENT_ADAPTER(821434A0, RaceEventSectionRivalCurrent)
RACE_EVENT_ADAPTER(821435C8, RaceEventItemsAvailable)
RACE_EVENT_ADAPTER(821431C0, RaceEventGroupItem)
