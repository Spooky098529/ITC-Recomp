// The car parameter tables natively: adapters in front of
// src/car_specs/car_specs.cpp (docs/functions/car-specs.md). The loads'
// callbacks are reached through Archive::Load's function pointer, so their
// adapters stay until it's native; they call the archive's TOC lookup
// (sub_8209D868, the archive's: not native) as a guest call and hand its
// answer to the plain C++. The swaps under them (CarSpec::Swap and its
// parts, the wheels' and table 83's) have no adapters: only the callbacks'
// plain C++ calls them (the flag switches the callbacks).

#include "guest.h"

#include "base/flags.h"
#include "car_specs/car_specs.h"

ITC_FLAG_BOOL(native_car_specs, true, "ITC",
              "Run the car parameter tables (docs/functions/car-specs.md: the loads' "
              "callbacks, sub_820CC248, sub_820CC0A8, sub_820C9DF0 and sub_820CBD90, and the "
              "swaps under them, CarSpec::Swap sub_820CC168 and its parts, "
              "sub_820CC1F0, sub_820CBE10, sub_820CB588 and sub_820CB658) natively "
              "instead of through the translated code (same results; checked by "
              "--run_harnesses=car_specs)");

using namespace car_specs;

#define ITC_CAR_SPECS_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                  \
  if (!ITC_FLAG(native_car_specs)) {    \
    __imp__sub_##address(ctx, base);    \
    return;                             \
  }

namespace {

// The archive's TOC for an asset (r3 = the id): the guest function.
uint32_t Toc(PPCContext& ctx, uint8_t* base, uint32_t id) {
  ctx.r3.u64 = id;
  sub_8209D868(ctx, base);
  return ctx.r3.u32;
}

}  // namespace

// The swapping callbacks turn flush-to-zero off, as the originals leave it
// (their lfs set it); f0 and f13 (volatile, read by no caller) aren't set.
extern "C" REX_FUNC(sub_820CC248) {
  ITC_CAR_SPECS_FALLBACK(820CC248)
  const uint32_t object = ctx.r6.u32;
  const uint32_t toc = Toc(ctx, base, 72);
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = OnCarSpecsLoaded(base, object, toc);
}

extern "C" REX_FUNC(sub_820CC0A8) {
  ITC_CAR_SPECS_FALLBACK(820CC0A8)
  const uint32_t object = ctx.r6.u32;
  const uint32_t toc = Toc(ctx, base, 73);
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = OnWheelsLoaded(base, object, toc);
}

extern "C" REX_FUNC(sub_820C9DF0) {
  ITC_CAR_SPECS_FALLBACK(820C9DF0)
  OnLoaded(base, ctx.r6.u32);
}

extern "C" REX_FUNC(sub_820CBD90) {
  ITC_CAR_SPECS_FALLBACK(820CBD90)
  const uint32_t object = ctx.r6.u32;
  const uint32_t toc = Toc(ctx, base, 83);
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = OnTable83Loaded(base, object, toc);
}
