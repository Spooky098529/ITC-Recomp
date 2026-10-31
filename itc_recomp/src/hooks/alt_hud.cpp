// The alternate HUD natively: adapters in front of src/alt_hud/
// (docs/functions/alt-hud.md), with the register steps the harness shares
// (hooks/alt_hud.h).

#include "hooks/alt_hud.h"

#include "alt_hud/alt_hud.h"
#include "base/flags.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_alt_hud, true, "ITC",
              "Run the alternate HUD's functions (docs/functions/alt-hud.md: MenuList::Release "
              "sub_8220BBB0 with --native_heap, its block the allocation layer's; AltHud::SetUp sub_82217C70 with SetUpGauges sub_82216C40, "
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
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  alt_hud::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
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

void AltHudMenuListRelease(PPCContext& ctx, uint8_t* base) {
  if (NativeHeapActive()) {
    if (const auto r3 = alt_hud::MenuListRelease(base, ctx.r3.u32, ctx.r1.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_8220BBB0(ctx, base);
}

void AltHudMenuListPressedX(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = alt_hud::MenuListPressedX(base, ctx.r3.u32, ctx.r1.u32,
                                         ITC_FLAG(native_action_mappers), guest.calls());
}

void AltHudPowerClass(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = alt_hud::PowerClass(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void AltHudSetUpGauges(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::SetUpGauges(base, ctx.r3.u32, ctx.r1.u32, guest.calls(), &f1);
  ctx.f1.f64 = f1;
}

void AltHudSetUpExtraGauges(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = alt_hud::SetUpExtraGauges(base, ctx.r3.u32, ctx.r1.u32);
}

void AltHudShowPanels(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = alt_hud::ShowPanels(base, ctx.r3.u32, ctx.r1.u32);
}

void AltHudShowGauge(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = alt_hud::ShowGauge(base, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void AltHudSetUp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::SetUp(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls(), &f1);
  ctx.f1.f64 = f1;
}

void AltHudSetSpeed(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::SetSpeed(base, ctx.r4.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
}

void AltHudUpdateExtraGauges(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = alt_hud::UpdateExtraGauges(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u64, ctx.r6.u64,
                                          ctx.r1.u32, guest.calls());
}

void AltHudSetGear(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = alt_hud::SetGear(base, ctx.r4.s32, ctx.r1.u32);
}

void AltHudSetNode25(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = alt_hud::SetNode25(base, ctx.r4.u32, ctx.r1.u32);
}

void AltHudSetTach(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetTach(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

void AltHudSetBar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetBar(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

void AltHudSetBoost(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetBoost(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}

void AltHudSetThrottle(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetThrottle(base, ctx.f1.f64, ctx.r1.u32);
}

void AltHudSetBrake(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetBrake(base, ctx.f1.f64, ctx.r1.u32);
}

void AltHudSetSteering(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = alt_hud::SetSteering(base, ctx.f1.f64, ctx.r1.u32);
}

void AltHudTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = alt_hud::Tick(base, ctx.r3.u32, ctx.r1.u32, ITC_FLAG(native_action_mappers),
                             guest.calls(), &f1);
  ctx.f1.f64 = f1;
}

void AltHudDestroy(PPCContext& ctx, uint8_t* base) {
  if (NativeHeapActive()) {
    GuestCalls guest(ctx, base);
    if (const auto r3 = alt_hud::Destroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls())) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_82216BA8(ctx, base);
}

void AltHudRunMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = alt_hud::RunMovie(base, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. None for the functions only native callers reach (their register
// steps stay for the harness): PowerClass, SetUpGauges, SetUpExtraGauges,
// ShowGauge and ShowPanels (from SetUp), MenuList::PressedX, SetSpeed,
// UpdateExtraGauges, SetGear, SetTach, SetNode25, SetBar, SetBoost,
// SetThrottle, SetBrake and SetSteering (from Tick). MenuList::Release's
// stays for its ~50 other callers.

#define ITC_ALT_HUD(address, step)        \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_alt_hud)) {      \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_ALT_HUD(8220BBB0, AltHudMenuListRelease)
ITC_ALT_HUD(82217C70, AltHudSetUp)
ITC_ALT_HUD(82217AC8, AltHudTick)
ITC_ALT_HUD(82216C00, AltHudRunMovie)
