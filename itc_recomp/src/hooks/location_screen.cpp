// The career's location screen natively: adapters in front of
// src/location_screen/ (docs/functions/location-screen.md), with the
// register steps the harness shares (hooks/location_screen.h).

#include "hooks/location_screen.h"

#include "base/flags.h"
#include "location_screen/location_screen.h"

ITC_FLAG_BOOL(native_location_screen, true, "ITC",
              "Run the career's location screen (docs/functions/location-screen.md: "
              "LocationScreen::SceneryConfig sub_8228F0A0, KindText sub_82227690, "
              "IsPictureQueued sub_822291E8, PictureA sub_82229108, PictureB sub_82229158, "
              "OnMessage14 sub_82227158, HasMoreUnlocks sub_822281D8, NextUnlockEntry "
              "sub_82227618, OpponentHasCard sub_82227708, NextResultText sub_822271F8) "
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
