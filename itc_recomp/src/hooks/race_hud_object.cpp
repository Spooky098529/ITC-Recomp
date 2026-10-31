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
              "ClearWarning sub_822122A0, SetPlayer sub_82212690, Touching sub_82215820, "
              "SetTach sub_82212D58, SetBoost sub_82212E18, SetSpeed sub_82214260, SetTime "
              "sub_82213750, SetDistance sub_82214A50, Reset sub_822121C0, ShowGauge "
              "sub_82212D08, SetUpExtraGauges sub_82213DB0, ApplyOptions sub_82212768, "
              "UpdateView sub_82212B68) "
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

void RaceHudObjectSetTach(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rh::SetTach(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

void RaceHudObjectSetBoost(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rh::SetBoost(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

void RaceHudObjectSetSpeed(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = rh::SetSpeed(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
}

void RaceHudObjectSetTime(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::SetTime(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceHudObjectSetDistance(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = rh::SetDistance(base, ctx.r3.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
}

void RaceHudObjectReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rh::Reset(base, ctx.r3.u32, ctx.r1.u32);
}

void RaceHudObjectShowGauge(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::ShowGauge(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void RaceHudObjectSetUpExtraGauges(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::SetUpExtraGauges(base, ctx.r3.u32, ctx.r1.u32);
}

void RaceHudObjectApplyOptions(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::ApplyOptions(base, ctx.r3.u32, ctx.r1.u32);
}

void RaceHudObjectUpdateView(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rh::UpdateView(base, ctx.r3.u32, ctx.r1.u32);
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
ITC_RACE_HUD_OBJECT(82212D58, RaceHudObjectSetTach)
ITC_RACE_HUD_OBJECT(82212E18, RaceHudObjectSetBoost)
ITC_RACE_HUD_OBJECT(82214260, RaceHudObjectSetSpeed)
ITC_RACE_HUD_OBJECT(82213750, RaceHudObjectSetTime)
ITC_RACE_HUD_OBJECT(82214A50, RaceHudObjectSetDistance)
ITC_RACE_HUD_OBJECT(822121C0, RaceHudObjectReset)
ITC_RACE_HUD_OBJECT(82212D08, RaceHudObjectShowGauge)
ITC_RACE_HUD_OBJECT(82213DB0, RaceHudObjectSetUpExtraGauges)
ITC_RACE_HUD_OBJECT(82212768, RaceHudObjectApplyOptions)
ITC_RACE_HUD_OBJECT(82212B68, RaceHudObjectUpdateView)
