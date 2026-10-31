// The race HUD object natively: adapters in front of src/race_hud_object/
// (docs/functions/race-hud-object.md), with the register steps the harness
// shares (hooks/race_hud_object.h).

#include "hooks/race_hud_object.h"

#include "base/flags.h"
#include "race_hud_object/race_hud_object.h"

ITC_FLAG_BOOL(native_race_hud_object, true, "ITC",
              "Run the race HUD object's functions (docs/functions/race-hud-object.md: "
              "HudCurve::Eval sub_82212118, PowInt sub_82213C40, HudBar::Step sub_82211FB8, "
              "RaceHud::SetGear sub_82212DB0, SetBar sub_82214698, ResetBar sub_82212668, "
              "ClearWarning sub_822122A0, SetPlayer sub_82212690, Touching sub_82215820) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=race_hud_object)");

namespace hooks {

namespace rh = race_hud_object;

void RaceHudObjectCurveEval(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rh::CurveEval(base, ctx.r3.u32, ctx.f1.f64);
}

void RaceHudObjectPowInt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = rh::PowInt(base, ctx.f1.f64, ctx.r4.s32);
}

void RaceHudObjectBarStep(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rh::BarStep(base, ctx.r3.u32, ctx.r4.s32, ctx.r1.u32);
}

void RaceHudObjectSetGear(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::SetGear(base, ctx.r3.u32, ctx.r4.s32, ctx.r1.u32);
}

void RaceHudObjectSetBar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rh::SetBar(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

void RaceHudObjectResetBar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rh::ResetBar(base, ctx.r3.u32);
}

void RaceHudObjectClearWarning(PPCContext& ctx, uint8_t* base) {
  rh::ClearWarning(base, ctx.r3.u32);
}

void RaceHudObjectSetPlayer(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::SetPlayer(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceHudObjectTouching(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::Touching(base, ctx.r4.u32, ctx.r1.u32);
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off.

#define ITC_RACE_HUD_OBJECT(address, step)    \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_race_hud_object)) {  \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

ITC_RACE_HUD_OBJECT(82212118, RaceHudObjectCurveEval)
ITC_RACE_HUD_OBJECT(82213C40, RaceHudObjectPowInt)
ITC_RACE_HUD_OBJECT(82211FB8, RaceHudObjectBarStep)
ITC_RACE_HUD_OBJECT(82212DB0, RaceHudObjectSetGear)
ITC_RACE_HUD_OBJECT(82214698, RaceHudObjectSetBar)
ITC_RACE_HUD_OBJECT(82212668, RaceHudObjectResetBar)
ITC_RACE_HUD_OBJECT(822122A0, RaceHudObjectClearWarning)
ITC_RACE_HUD_OBJECT(82212690, RaceHudObjectSetPlayer)
ITC_RACE_HUD_OBJECT(82215820, RaceHudObjectTouching)
