// The race HUD object natively: adapters in front of src/race_hud_object/
// (docs/functions/race-hud-object.md), with the register steps the harness
// shares (hooks/race_hud_object.h).

#include "hooks/race_hud_object.h"

#include "base/flags.h"
#include "race_hud_object/race_hud_object.h"
#include "track_zones/track_zones.h"

#include <optional>

ITC_FLAG_BOOL(native_race_hud_object, true, "ITC",
              "Run the race HUD object's functions (docs/functions/race-hud-object.md: "
              "HudCurve::Eval sub_82212118, PowInt sub_82213C40, HudBar::Step sub_82211FB8, "
              "RaceHud::SetGear sub_82212DB0, SetBar sub_82214698, ResetBar sub_82212668, "
              "ClearWarning sub_822122A0, SetPlayer sub_82212690, Touching sub_82215820, "
              "SetTach sub_82212D58, SetBoost sub_82212E18, SetSpeed sub_82214260, SetTime "
              "sub_82213750, SetDistance sub_82214A50, Reset sub_822121C0, ShowGauge "
              "sub_82212D08, SetUpExtraGauges sub_82213DB0, ApplyOptions sub_82212768, "
              "UpdateView sub_82212B68, SetUpGauges sub_82213C90, SetUpMirror sub_822135B0, "
              "TearDown sub_822141A0, SetUpSigns sub_822122B0, UpdateExtraGauges sub_82215160; "
              "memcpy where its plain C++ refuses, CameraPasses::RenderTarget, "
              "TrackZones::Create and the extra gauges' digits through their overrides "
              "or the originals; UpdateSigns sub_82214718, UpdateSign sub_82212F00, "
              "UpdatePlaceName sub_82213510, with TrackZones::Update and IsInside's plain C++; "
              "UiMovie::FinishNamed, SetText and ClearText through their overrides; Tick "
              "sub_822153A0, TickTimer sub_82214D78) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=race_hud_object)");

namespace hooks {

namespace rh = race_hud_object;

namespace {

// The guest calls the plain C++ makes (race_hud_object::Calls), through
// this context: r1, r3 to r6, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  rh::Calls calls() { return {this, &GuestCalls::Call}; }
  // The zones' slots that aren't the native ones (track_zones::OwnerCalls):
  // r4 set when asked, else left.
  track_zones::OwnerCalls zone_calls() { return {this, &GuestCalls::CallZone}; }

 private:
  static uint64_t CallZone(void* user, uint32_t function, uint32_t r3, uint32_t r4, bool set_r4,
                           uint32_t frame, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    ctx.r3.u64 = r3;
    if (set_r4) {
      ctx.r4.u64 = r4;
    }
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    return ctx.r3.u64;
  }
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = args[0];
    ctx.r4.u64 = args[1];
    ctx.r5.u64 = args[2];
    ctx.r6.u64 = args[3];
    ctx.r7.u64 = args[4];
    ctx.r8.u64 = args[5];
    ctx.r9.u64 = args[6];
    ctx.r10.u64 = args[7];
    if (f1 != nullptr) {
      ctx.f1.f64 = *f1;
    }
    ctx.lr = lr;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

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

void RaceHudObjectSetUpGauges(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::SetUpGauges(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceHudObjectSetUpMirror(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::SetUpMirror(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceHudObjectTearDown(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::TearDown(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceHudObjectSetUpSigns(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::SetUpSigns(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceHudObjectUpdateExtraGauges(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::UpdateExtraGauges(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u64, ctx.r6.u64,
                                     ctx.r1.u32, guest.calls());
}

void RaceHudObjectUpdateSigns(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::UpdateSigns(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls(),
                               guest.zone_calls());
}

void RaceHudObjectUpdateSign(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::UpdateSign(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls(),
                              guest.zone_calls());
}

void RaceHudObjectUpdatePlaceName(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      rh::UpdatePlaceName(base, ctx.r3.u32, ctx.r1.u32, guest.calls(), guest.zone_calls());
}

void RaceHudObjectTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::Tick(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls(),
                        guest.zone_calls());
}

void RaceHudObjectTickTimer(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rh::TickTimer(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
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
ITC_RACE_HUD_OBJECT(82213C90, RaceHudObjectSetUpGauges)
ITC_RACE_HUD_OBJECT(822135B0, RaceHudObjectSetUpMirror)
ITC_RACE_HUD_OBJECT(822141A0, RaceHudObjectTearDown)
ITC_RACE_HUD_OBJECT(822122B0, RaceHudObjectSetUpSigns)
ITC_RACE_HUD_OBJECT(82215160, RaceHudObjectUpdateExtraGauges)
ITC_RACE_HUD_OBJECT(82214718, RaceHudObjectUpdateSigns)
ITC_RACE_HUD_OBJECT(82212F00, RaceHudObjectUpdateSign)
ITC_RACE_HUD_OBJECT(82213510, RaceHudObjectUpdatePlaceName)
ITC_RACE_HUD_OBJECT(822153A0, RaceHudObjectTick)
ITC_RACE_HUD_OBJECT(82214D78, RaceHudObjectTickTimer)
