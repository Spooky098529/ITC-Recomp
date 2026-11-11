// The car settings screen natively: adapters in front of src/car_settings/
// (docs/functions/car-settings.md), with the register steps the harness
// shares (hooks/car_settings.h).

#include "hooks/car_settings.h"

#include "base/flags.h"
#include "car_settings/car_settings.h"

ITC_FLAG_BOOL(native_car_settings, true, "ITC",
              "Run the car settings screen's machine (docs/functions/car-settings.md: the "
              "type 2 mode's +0x173D0 machine and the type 5 mode's +848 one, UI movie 3) "
              "natively instead of through the translated code, as far as it was seen in play: "
              "its handlers Create sub_821AA2F8 and Destroy sub_821AA408 (VirtualAlloc and "
              "VirtualFree through their overrides), OpenMovie sub_821A8DD0, WaitForMovie "
              "sub_821AA490 (UiMovies::IsLoaded's plain C++) and Close sub_821AAF70 "
              "(UiMovie::FinishAll and UiMovies::Delete through their overrides) (the same "
              "writes; checked by --run_harnesses=car_settings)");

namespace hooks {

namespace {

namespace cs = car_settings;

// The guest calls the plain C++ makes (car_settings::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  cs::Calls calls() { return {this, &GuestCalls::Call}; }

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

void CarSettingsCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cs::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// A tail call: UiMovies::Open runs with the adapter's own sp and lr.
void CarSettingsOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cs::OpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void CarSettingsWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cs::WaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void CarSettingsClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cs::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void CarSettingsDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cs::Destroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_CAR_SETTINGS(address, step)  \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_car_settings)) { \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// The machine's handlers (functors, from the recompiled state machine).
ITC_CAR_SETTINGS(821AA2F8, CarSettingsCreate)
ITC_CAR_SETTINGS(821A8DD0, CarSettingsOpenMovie)
ITC_CAR_SETTINGS(821AA490, CarSettingsWaitForMovie)
ITC_CAR_SETTINGS(821AAF70, CarSettingsClose)
ITC_CAR_SETTINGS(821AA408, CarSettingsDestroy)
