// The car models natively: adapters in front of src/car_models/
// (docs/functions/car-models.md), with the register steps the harness
// shares (hooks/car_models.h).

#include "hooks/car_models.h"

#include "base/flags.h"
#include "car_models/car_model.h"
#include "car_models/load_slots.h"
#include "hooks/heap.h"
#include "hooks/models.h"

ITC_FLAG_BOOL(native_car_models, true, "ITC",
              "Run the car models' pieces (docs/functions/car-models.md: LoadSlot::Reset "
              "sub_820D0730, with --native_heap LoadSlot::ReleaseBuffer sub_820D0758, and the "
              "deferred release sub_820C3F50; with --native_heap and the memory blocks' host lock "
              "CarModel::Release sub_820D4508) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=car_models)");

namespace hooks {

namespace cm = car_models;

void CarModelsLoadSlotReset(PPCContext& ctx, uint8_t* base) {
  cm::LoadSlotReset(base, ctx.r3.u32);
}

void CarModelsLoadSlotReleaseBuffer(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = cm::LoadSlotReleaseBuffer(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D0758(ctx, base);
}

void CarModelsDeferRelease(PPCContext& ctx, uint8_t* base) {
  cm::DeferRelease(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

void CarModelsCarModelRelease(PPCContext& ctx, uint8_t* base) {
  if (hooks::ModelsNativeLifetime()) {
    ctx.fpscr.disableFlushMode();  // as CarModel::Reset
    if (const auto r3 = cm::CarModelRelease(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D4508(ctx, base);
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off.

#define ITC_CAR_MODELS(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_car_models)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

ITC_CAR_MODELS(820D0730, CarModelsLoadSlotReset)
ITC_CAR_MODELS(820D0758, CarModelsLoadSlotReleaseBuffer)
ITC_CAR_MODELS(820C3F50, CarModelsDeferRelease)
ITC_CAR_MODELS(820D4508, CarModelsCarModelRelease)
