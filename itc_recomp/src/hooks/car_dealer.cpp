// The car dealer natively: adapters in front of src/car_dealer/
// (docs/functions/car-dealer.md), with the register steps the harness shares
// (hooks/car_dealer.h).

#include "hooks/car_dealer.h"

#include "base/flags.h"
#include "car_dealer/car_dealer.h"

ITC_FLAG_BOOL(native_car_dealer, true, "ITC",
              "Run the car dealer (docs/functions/car-dealer.md: the type 2 mode's screen "
              "machine where the player buys a car) natively instead of through the translated "
              "code: CarDealer::InitStates sub_82185920 over StateMachine::Add's plain C++; the "
              "handlers Create sub_82180D68 (VirtualAlloc through its override), OpenMovie "
              "sub_82180C08 (UiMovies::Open through its override), WaitForLoad sub_82180F20, "
              "AfterDetail sub_82180C48 and Page4 sub_82181B98; the helpers CheckLimits "
              "sub_821812B8, ReloadStandModel sub_82181330 (CarModels::BlockOf and "
              "GarageStage::ReloadModel through their overrides), SetKindBar sub_82182158, "
              "SetColorBar sub_82182770, SetScrollArrows sub_82182DD0, SetHeader sub_82182C28, "
              "SetPrompts sub_82182B30 and HelpLine::Set sub_82199B38 (UiMovie::SetText, its "
              "unset varargs 0, ScrollText and the scroll reset through their overrides) "
              "(the same writes; checked by --run_harnesses=car_dealer)");

namespace hooks {

namespace {

namespace cd = car_dealer;

// The guest calls the plain C++ makes (car_dealer::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  cd::Calls calls() { return {this, &GuestCalls::Call}; }

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

void CarDealerInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = cd::InitStates(base, machine, ctx.r1.u32 - cd::kInitStatesFrame, &replaced);
  // StateMachine::Add's adapter's r3: a new transition's address from the
  // machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
void CarDealerCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerOpenMovie(PPCContext& ctx, uint8_t* base) {
  // A tail call: UiMovies::Open runs on the caller's frame with its lr.
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void CarDealerWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void CarDealerAfterDetail(PPCContext&, uint8_t* base) { cd::AfterDetail(base); }
void CarDealerPage4(PPCContext& ctx, uint8_t* base) { cd::Page4(base, ctx.r3.u32); }
void CarDealerCheckLimits(PPCContext&, uint8_t* base) { cd::CheckLimits(base); }
void CarDealerReloadStandModel(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::ReloadStandModel(base, ctx.r1.u32, guest.calls());
}
void CarDealerSetKindBar(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetKindBar(base, ctx.r3.u64, ctx.r1.u32);
}
void CarDealerSetColorBar(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetColorBar(base, ctx.r3.u64, ctx.r1.u32);
}
void CarDealerSetScrollArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetScrollArrows(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32);
}
void CarDealerSetHeader(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetHeader(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void CarDealerSetPrompts(PPCContext& ctx, uint8_t* base) {
  const uint32_t args[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                            ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = cd::SetPrompts(base, args, ctx.r1.u32);
}
void HelpLineSet(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::HelpLineSet(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_CAR_DEALER(address, step)       \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_car_dealer)) {     \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The machine's set-up (from the type 2 mode's Init sub_82197278,
// recompiled).
ITC_CAR_DEALER(82185920, CarDealerInitStates)
// The handlers: member function functors the machine calls (no direct
// callers).
ITC_CAR_DEALER(82180D68, CarDealerCreate)
ITC_CAR_DEALER(82180C08, CarDealerOpenMovie)
ITC_CAR_DEALER(82180F20, CarDealerWaitForLoad)
ITC_CAR_DEALER(82180C48, CarDealerAfterDetail)
// The page handlers (from UpdatePage's table on its stack).
ITC_CAR_DEALER(82181B98, CarDealerPage4)
// The helpers (from the recompiled pages, transitions and set-ups).
ITC_CAR_DEALER(821812B8, CarDealerCheckLimits)
ITC_CAR_DEALER(82181330, CarDealerReloadStandModel)
ITC_CAR_DEALER(82182158, CarDealerSetKindBar)
ITC_CAR_DEALER(82182770, CarDealerSetColorBar)
ITC_CAR_DEALER(82182DD0, CarDealerSetScrollArrows)
ITC_CAR_DEALER(82182C28, CarDealerSetHeader)
ITC_CAR_DEALER(82182B30, CarDealerSetPrompts)
ITC_CAR_DEALER(82199B38, HelpLineSet)
