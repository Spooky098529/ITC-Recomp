// The car parameter tables natively: adapters in front of
// src/car_specs/car_specs.cpp (docs/functions/car-specs.md). The loads'
// callbacks are reached through Archive::Load's function pointer, so their
// adapters stay until it's native; they call the archive's TOC lookup
// (sub_8209D868, the archive's: not native) as a guest call and hand its
// answer to the plain C++. The swaps under them (CarSpec::Swap and its
// parts, the wheels' and table 83's) have no adapters: only the callbacks'
// plain C++ calls them (the flag switches the callbacks). The aero parts'
// swaps (sub_820CB6C8, sub_820CB7E8) have none either: only the getters'
// plain C++ calls them. Car::Refresh's callees (sub_820C4888, sub_820C4568,
// Tire::InchUp sub_820C46B8) have none: only its plain C++ calls them.

#include "guest.h"

#include "base/flags.h"
#include "car_specs/car_specs.h"

ITC_FLAG_BOOL(native_car_specs, true, "ITC",
              "Run the car parameter tables (docs/functions/car-specs.md: the loads' "
              "callbacks, sub_820CC248, sub_820CC0A8, sub_820C9DF0 and sub_820CBD90, and the "
              "swaps under them, CarSpec::Swap sub_820CC168 and its parts, "
              "sub_820CC1F0, sub_820CBE10, sub_820CB588 and sub_820CB658; the getters, "
              "sub_820C9E98, sub_820C9EB0, sub_820C9EE0, sub_820C9E00, sub_820C9928 and "
              "sub_820C9DD8; the aero parts' getters, sub_820CB848 to sub_820CBC90, their "
              "swaps and sub_820CBD20; Car::Refresh, sub_820C4998, with sub_820C4888, "
              "sub_820C4568 and Tire::InchUp, sub_820C46B8) natively "
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

// The getters: r3 a word (zero-extended; the originals' carry the
// arguments' upper halves, which the game's 32-bit code doesn't read).
#define ITC_CAR_SPECS_WORD(address, expr) \
  extern "C" REX_FUNC(sub_##address) {   \
    ITC_CAR_SPECS_FALLBACK(address)      \
    ctx.r3.u64 = uint32_t(expr);         \
  }

ITC_CAR_SPECS_WORD(820C9E98, GetCarSpec(base, ctx.r3.u32))
ITC_CAR_SPECS_WORD(820C9EB0, GetWheel(base, ctx.r3.s32))
ITC_CAR_SPECS_WORD(820C9EE0, FirstWheelOfModel(base, ctx.r3.s32))
ITC_CAR_SPECS_WORD(820C9928, GetTire(ctx.r3.u32, ctx.r4.s32, ctx.r5.u32, ctx.r6.s32))
ITC_CAR_SPECS_WORD(820C9DD8, Exists(base))

// Params::GetCarParts: r3 = out, left as it came.
extern "C" REX_FUNC(sub_820C9E00) {
  ITC_CAR_SPECS_FALLBACK(820C9E00)
  uint32_t parts[9];
  GetCarParts(base, ctx.r4.u32, parts);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, ctx.r3.u32 + 4 * i, parts[i]);
  }
}

// The aero part getters: r3 = out (returned as it came), r4 = the parts (the
// 9 pointers read here; the original reads the one it needs), r5 = the level;
// the original where memcpy's copy wouldn't be the host's. They turn
// flush-to-zero off, as the originals' lfs leave it.
#define ITC_CAR_SPECS_AERO(address, function)                          \
  extern "C" REX_FUNC(sub_##address) {                                 \
    ITC_CAR_SPECS_FALLBACK(address)                                    \
    uint32_t parts[9];                                                 \
    for (uint32_t i = 0; i < 9; i++) {                                 \
      parts[i] = guest::LoadU32(base, ctx.r4.u32 + 4 * i);             \
    }                                                                  \
    if (!function(base, ctx.r3.u32, parts, ctx.r5.s32)) {              \
      __imp__sub_##address(ctx, base);                                 \
      return;                                                          \
    }                                                                  \
    ctx.fpscr.disableFlushMode();                                      \
  }

ITC_CAR_SPECS_AERO(820CB848, AeroPart74)
ITC_CAR_SPECS_AERO(820CB8D0, AeroPart75)
ITC_CAR_SPECS_AERO(820CB958, AeroPart76)
ITC_CAR_SPECS_AERO(820CB9E0, AeroPart77)
ITC_CAR_SPECS_AERO(820CBA68, AeroPart78)
ITC_CAR_SPECS_AERO(820CBAF0, AeroPart79)
ITC_CAR_SPECS_AERO(820CBB78, AeroPart80)
ITC_CAR_SPECS_AERO(820CBC00, AeroPart81)
ITC_CAR_SPECS_AERO(820CBC90, AeroPart82)

// sub_820CBD20: r3 = the parts; the count. Flush-to-zero off first, as the
// original's lfs sets it before its compare.
extern "C" REX_FUNC(sub_820CBD20) {
  ITC_CAR_SPECS_FALLBACK(820CBD20)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = CountBodyKits(base, ctx.r3.u32);
}

// Car::Refresh: r3 = the car; r3 as the original leaves it (its last
// memcpy's destination). The original where one of its memcpys wouldn't be
// the host's copy. Flush-to-zero off first, as its first lfs sets it (the
// tires' float steps need it).
extern "C" REX_FUNC(sub_820C4998) {
  ITC_CAR_SPECS_FALLBACK(820C4998)
  ctx.fpscr.disableFlushMode();
  uint32_t r3;
  if (!CarRefresh(base, ctx.r3.u32, &r3)) {
    __imp__sub_820C4998(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
