// The career's location screen natively: adapters in front of
// src/location_screen/ (docs/functions/location-screen.md), with the
// register steps the harness shares (hooks/location_screen.h).

#include "hooks/location_screen.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "location_screen/location_screen.h"

ITC_FLAG_BOOL(native_location_screen, true, "ITC",
              "Run the career's location screen (docs/functions/location-screen.md: "
              "LocationScreen::SceneryConfig sub_8228F0A0, KindText sub_82227690, "
              "IsPictureQueued sub_822291E8, PictureA sub_82229108, PictureB sub_82229158, "
              "OnMessage14 sub_82227158, HasMoreUnlocks sub_822281D8, NextUnlockEntry "
              "sub_82227618, OpponentHasCard sub_82227708, NextResultText sub_822271F8, "
              "ShowPictures sub_8222A060, HidePictures sub_8222A150, SetCarColor sub_8222AC18, "
              "SetCursor sub_8222BFE8, ListNode sub_8222C058, ShowArrows sub_8222C0C8, "
              "RunCardMovie sub_822291A8, Transition1 sub_8222AD50, Transition18 sub_8222BE60; "
              "UiMovie::Run through its override; ScreenMachine::Reenter sub_822895E0, Reset "
              "sub_822279A8 with ResetView sub_82227818 (no adapter: only Reset's plain C++ calls it), "
              "ApplyQueuedPage sub_8222C9B0, "
              "UpdatePage sub_8222EC08, its page's handler through the guest context; InitStates "
              "sub_8222F258 over StateMachine::Add's plain C++) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=location_screen)");

namespace hooks {

namespace ls = location_screen;

void LocationScreenSceneryConfig(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = ls::SceneryConfig(ctx.r3.u64, ctx.r4.u64);
}

void LocationScreenKindText(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = ls::KindText(ctx.r3.u32, ctx.r4.u64);
}

void LocationScreenIsPictureQueued(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::IsPictureQueued(base, ctx.r3.u32);
}

void LocationScreenPictureA(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::PictureA(base, ctx.r3.u32);
}

void LocationScreenPictureB(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::PictureB(base, ctx.r3.u32);
}

void LocationScreenOnMessage14(PPCContext&, uint8_t* base) { ls::OnMessage14(base); }

void LocationScreenHasMoreUnlocks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::HasMoreUnlocks(base);
}

void LocationScreenNextUnlockEntry(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::NextUnlockEntry(base);
}

void LocationScreenOpponentHasCard(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = ls::OpponentHasCard(base, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_82227708(ctx, base);
}

void LocationScreenNextResultText(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = ls::NextResultText(base, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_822271F8(ctx, base);
}

void LocationScreenShowPictures(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::ShowPictures(base, ctx.r1.u32);
}

void LocationScreenHidePictures(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::HidePictures(base, ctx.r1.u32);
}

void LocationScreenSetCarColor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::SetCarColor(base, ctx.r1.u32);
}

void LocationScreenSetCursor(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ls::SetCursor(base, ctx.r1.u32, ctx.r3.u32);
}

void LocationScreenListNode(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ls::ListNode(base, ctx.r1.u32, ctx.r3.u32);
}

void LocationScreenShowArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::ShowArrows(base, ctx.r1.u32);
}

namespace {

struct GuestRun {
  PPCContext& ctx;
  uint8_t* base;
};

uint64_t RunThrough(void* user, uint32_t movie, uint32_t draw_only, uint32_t frame_bytes,
                    uint32_t lr) {
  auto& guest = *static_cast<GuestRun*>(user);
  PPCContext& ctx = guest.ctx;
  guest_call::InFrame(ctx, guest.base, frame_bytes, lr, [&](uint32_t) {
    ctx.r3.u64 = movie;
    ctx.r4.u64 = draw_only;
    sub_8211E1B8(ctx, guest.base);
  });
  return ctx.r3.u64;
}

// Runs `step(run)` with UiMovie::Run through this context; lr put back.
template <typename Step>
uint64_t WithRun(PPCContext& ctx, uint8_t* base, Step step) {
  const uint64_t lr = ctx.lr;
  GuestRun guest{ctx, base};
  const ls::RunCall run{&guest, RunThrough};
  const uint64_t r3 = step(run);
  ctx.lr = lr;
  return r3;
}

}  // namespace

void LocationScreenRunCardMovie(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = WithRun(ctx, base, [&](const ls::RunCall& run) {
    return ls::RunCardMovie(base, sp, run);
  });
}

void LocationScreenTransition1(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, step = ctx.r3.u32;
  ctx.r3.u64 = WithRun(ctx, base, [&](const ls::RunCall& run) -> uint64_t {
    return ls::Transition1(base, sp, step, run);
  });
}

void LocationScreenTransition18(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, step = ctx.r3.u32;
  ctx.r3.u64 = WithRun(ctx, base, [&](const ls::RunCall& run) -> uint64_t {
    return ls::Transition18(base, sp, step, run);
  });
}

void LocationScreenReenter(PPCContext& ctx, uint8_t* base) { ls::Reenter(base, ctx.r3.u32); }

void LocationScreenReset(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32;
  if (!ls::ResetNative(screen)) {
    __imp__sub_822279A8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ls::Reset(base, screen);
  ctx.r3.u64 = screen + 560 + 176;
}

void LocationScreenResetView(PPCContext& ctx, uint8_t* base) {
  const uint32_t view = ctx.r3.u32;
  if (!ls::ResetNative(view - 560)) {
    __imp__sub_82227818(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ls::ResetView(base, view);
  ctx.r3.u64 = view + 176;
}

void LocationScreenApplyQueuedPage(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = ls::ApplyQueuedPage(base, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
  }
}

namespace {

void PageThrough(void* user, uint32_t handler, uint32_t dispatcher, uint32_t machine,
                 uint32_t frame_bytes, uint32_t lr) {
  auto& guest = *static_cast<GuestRun*>(user);
  PPCContext& ctx = guest.ctx;
  uint8_t* base = guest.base;
  guest_call::InFrame(ctx, base, frame_bytes, lr, [&](uint32_t) {
    ctx.r3.u64 = dispatcher;
    ctx.r4.u64 = machine;
    ctx.ctr.u64 = handler;
    REX_CALL_INDIRECT_FUNC(handler);
  });
}

}  // namespace

void LocationScreenUpdatePage(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  GuestRun guest{ctx, base};
  const ls::PageCall call{&guest, PageThrough};
  ls::UpdatePage(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, call);
  ctx.lr = lr;
}

void LocationScreenInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 =
      ls::InitStates(base, machine, ctx.r1.u32 - ls::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define LOCATION_SCREEN_ADAPTER(address, step)  \
  extern "C" REX_FUNC(sub_##address) {          \
    REX_FUNC_PROLOGUE();                        \
    if (!ITC_FLAG(native_location_screen)) {    \
      __imp__sub_##address(ctx, base);          \
      return;                                   \
    }                                           \
    hooks::step(ctx, base);                     \
  }

LOCATION_SCREEN_ADAPTER(8228F0A0, LocationScreenSceneryConfig)
LOCATION_SCREEN_ADAPTER(82227690, LocationScreenKindText)
LOCATION_SCREEN_ADAPTER(822291E8, LocationScreenIsPictureQueued)
LOCATION_SCREEN_ADAPTER(82229108, LocationScreenPictureA)
LOCATION_SCREEN_ADAPTER(82229158, LocationScreenPictureB)
LOCATION_SCREEN_ADAPTER(82227158, LocationScreenOnMessage14)
LOCATION_SCREEN_ADAPTER(822281D8, LocationScreenHasMoreUnlocks)
LOCATION_SCREEN_ADAPTER(82227618, LocationScreenNextUnlockEntry)
LOCATION_SCREEN_ADAPTER(82227708, LocationScreenOpponentHasCard)
LOCATION_SCREEN_ADAPTER(822271F8, LocationScreenNextResultText)
LOCATION_SCREEN_ADAPTER(8222A060, LocationScreenShowPictures)
LOCATION_SCREEN_ADAPTER(8222A150, LocationScreenHidePictures)
LOCATION_SCREEN_ADAPTER(8222AC18, LocationScreenSetCarColor)
LOCATION_SCREEN_ADAPTER(8222BFE8, LocationScreenSetCursor)
LOCATION_SCREEN_ADAPTER(8222C058, LocationScreenListNode)
LOCATION_SCREEN_ADAPTER(8222C0C8, LocationScreenShowArrows)
LOCATION_SCREEN_ADAPTER(822291A8, LocationScreenRunCardMovie)
LOCATION_SCREEN_ADAPTER(8222AD50, LocationScreenTransition1)
LOCATION_SCREEN_ADAPTER(8222BE60, LocationScreenTransition18)
LOCATION_SCREEN_ADAPTER(822895E0, LocationScreenReenter)
LOCATION_SCREEN_ADAPTER(822279A8, LocationScreenReset)
LOCATION_SCREEN_ADAPTER(8222C9B0, LocationScreenApplyQueuedPage)
LOCATION_SCREEN_ADAPTER(8222EC08, LocationScreenUpdatePage)
LOCATION_SCREEN_ADAPTER(8222F258, LocationScreenInitStates)
