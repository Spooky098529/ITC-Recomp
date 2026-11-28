// The alternate HUD natively: adapters in front of src/alt_hud/
// (docs/functions/alt-hud.md), with the register steps the harness shares
// (hooks/alt_hud.h).

#include "hooks/alt_hud.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "alt_hud/alt_hud.h"
#include "base/flags.h"
#include "hooks/heap.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_alt_hud, true, "ITC",
              "Run the alternate HUD's functions (docs/functions/alt-hud.md: MenuList::Release "
              "sub_8220BBB0 and AltHud::Destroy sub_82216BA8 (no adapter: only RaceScreens::"
              "~RaceScreens's plain C++ reaches it) with --native_heap, their blocks "
              "the allocation layer's (Destroy's operator delete through its override where its "
              "plain C++ refuses); AltHud::SetUp sub_82217C70 with SetUpGauges sub_82216C40, "
              "PowerClass sub_82216B08 (Car::SetupVehicle through its override where its plain "
              "C++ refuses), SetUpExtraGauges sub_822178A0, ShowGauge sub_82217380 and ShowPanels "
              "sub_82216D78; AltHud::Tick sub_82217AC8 with MenuList::PressedX sub_8220BF00 "
              "(ActionMapper::Pressed's plain C++, or the mapper's slot through the function "
              "table), SetSpeed sub_82216E00, UpdateExtraGauges sub_82217A30 (the extra gauges' "
              "digits, never seen in play, the originals), SetGear sub_82217460, SetTach "
              "sub_822173E8, SetNode25 sub_822174E0, SetBar sub_822175A0, SetBoost sub_82217538, "
              "SetThrottle sub_82217638, SetBrake sub_82217690 and SetSteering sub_822176E8; "
              "AltHud::RunMovie sub_82216C00 (UiMovie::Run through its override); the functions "
              "under SetUp and Tick have no adapters: only their native callers reach them, and "
              "the flag switches those) natively instead of through the translated code (the "
              "same writes; checked by --run_harnesses=alt_hud)");
ITC_FLAG_DECLARE(bool, native_action_mappers);

namespace hooks {

namespace {

// The guest calls the plain C++ makes (alt_hud::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  alt_hud::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
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
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint64_t lr_;
};

}  // namespace

ITC_REGS_STEP(AltHudMenuListRelease) {
  if (NativeHeapActive()) {
    if (const auto r3 = alt_hud::MenuListRelease(ctx.r3.u32, ctx.r1.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c8220BBB0(ctx);
}

ITC_REGS_STEP(AltHudMenuListPressedX) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = alt_hud::MenuListPressedX(ctx.r3.u32, ctx.r1.u32,
                                         ITC_FLAG(native_action_mappers), guest.calls());
}

ITC_REGS_STEP(AltHudPowerClass) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = alt_hud::PowerClass(ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(AltHudSetUpGauges) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::SetUpGauges(ctx.r3.u32, ctx.r1.u32, guest.calls(), &f1);
  ctx.f1.f64 = f1;
}

ITC_REGS_STEP(AltHudSetUpExtraGauges) {
  ctx.r3.u64 = alt_hud::SetUpExtraGauges(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudShowPanels) {
  ctx.r3.u64 = alt_hud::ShowPanels(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudShowGauge) {
  ctx.r3.u64 = alt_hud::ShowGauge(ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetUp) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::SetUp(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls(), &f1);
  ctx.f1.f64 = f1;
}

ITC_REGS_STEP(AltHudSetSpeed) {
  ctx.fpscr.disableFlushMode();
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::SetSpeed(ctx.r4.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
}

ITC_REGS_STEP(AltHudUpdateExtraGauges) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = alt_hud::UpdateExtraGauges(ctx.r3.u32, ctx.f1.f64, ctx.r5.u64, ctx.r6.u64,
                                          ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(AltHudSetGear) {
  ctx.r3.u64 = alt_hud::SetGear(ctx.r4.s32, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetNode25) {
  ctx.r3.u64 = alt_hud::SetNode25(ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetTach) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetTach(ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetBar) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetBar(ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetBoost) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetBoost(ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetThrottle) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetThrottle(ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetBrake) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetBrake(ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudSetSteering) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetSteering(ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(AltHudTick) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::Tick(ctx.r3.u32, ctx.r1.u32, ITC_FLAG(native_action_mappers),
                             guest.calls(), &f1);
  ctx.f1.f64 = f1;
}

ITC_REGS_STEP(AltHudDestroy) {
  if (NativeHeapActive()) {
    GuestCalls guest(ctx);
    if (const auto r3 = alt_hud::Destroy(ctx.r3.u32, ctx.r1.u32, guest.calls())) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c82216BA8(ctx);
}

ITC_REGS_STEP(AltHudRunMovie) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = alt_hud::RunMovie(ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. None for the functions only native callers reach (their register
// steps stay for the harness): PowerClass, SetUpGauges, SetUpExtraGauges,
// ShowGauge and ShowPanels (from SetUp), MenuList::PressedX, SetSpeed,
// UpdateExtraGauges, SetGear, SetTach, SetNode25, SetBar, SetBoost,
// SetThrottle, SetBrake and SetSteering (from Tick); SetUp and Tick, from
// the race HUD machine's native handlers only in play (src/race_hud_machine/,
// by their plain C++; kind 4's paths, never seen in play, reach the
// originals; the user's call, 2026-10-31); Destroy, from RaceScreens::
// ~RaceScreens's plain C++ only (src/race_hud_object/, 2026-11-08).
// MenuList::Release's stays for its ~50 other callers.

#define ITC_ALT_HUD(address, step)                \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_alt_hud)) { \
    hooks::step(ctx);                       \
  }

ITC_ALT_HUD(8220BBB0, AltHudMenuListRelease)
ITC_ALT_HUD(82216C00, AltHudRunMovie)

// For the fallback originals (docs/missing-functions.md): they call these by
// address, so each runs its register step directly (no dispatch entry).
ITC_DIRECT_ENTRY(8220BF00) { hooks::AltHudMenuListPressedX(ctx); }
ITC_DIRECT_ENTRY(82216C40) { hooks::AltHudSetUpGauges(ctx); }
ITC_DIRECT_ENTRY(82216D78) { hooks::AltHudShowPanels(ctx); }
ITC_DIRECT_ENTRY(82216E00) { hooks::AltHudSetSpeed(ctx); }
ITC_DIRECT_ENTRY(822173E8) { hooks::AltHudSetTach(ctx); }
ITC_DIRECT_ENTRY(82217460) { hooks::AltHudSetGear(ctx); }
ITC_DIRECT_ENTRY(822174E0) { hooks::AltHudSetNode25(ctx); }
ITC_DIRECT_ENTRY(82217538) { hooks::AltHudSetBoost(ctx); }
ITC_DIRECT_ENTRY(822175A0) { hooks::AltHudSetBar(ctx); }
ITC_DIRECT_ENTRY(82217638) { hooks::AltHudSetThrottle(ctx); }
ITC_DIRECT_ENTRY(82217690) { hooks::AltHudSetBrake(ctx); }
ITC_DIRECT_ENTRY(822176E8) { hooks::AltHudSetSteering(ctx); }
ITC_DIRECT_ENTRY(822178A0) { hooks::AltHudSetUpExtraGauges(ctx); }
ITC_DIRECT_ENTRY(82217A30) { hooks::AltHudUpdateExtraGauges(ctx); }
// For the fallback originals (docs/missing-functions.md): the lift.py
// batches call these by address too (round of 2026-11-28).
ITC_DIRECT_ENTRY(82216B08) { hooks::AltHudPowerClass(ctx); }
ITC_DIRECT_ENTRY(82217C70) { hooks::AltHudSetUp(ctx); }
ITC_DIRECT_ENTRY(82216BA8) { hooks::AltHudDestroy(ctx); }
ITC_DIRECT_ENTRY(82217AC8) { hooks::AltHudTick(ctx); }
