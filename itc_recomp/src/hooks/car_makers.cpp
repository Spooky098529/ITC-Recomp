// The car objects' makers natively: adapters in front of src/car_makers/
// (docs/functions/car-makers.md), with the register steps the harness
// shares (hooks/car_makers.h). Each leaves r3 as the original does.

#include "hooks/car_makers.h"

#include "base/flags.h"
#include "car_makers/car_makers.h"
#include "hooks/front_end_car_draw.h"

ITC_FLAG_DECLARE(bool, native_scene_models);
ITC_FLAG_DECLARE(bool, native_car_specs);

ITC_FLAG_BOOL(native_car_makers, true, "ITC",
              "Run the car objects' makers (docs/functions/car-makers.md): CarModels::BlockOf "
              "sub_8215AAD0 (its case sub_8215AB04 merged), SceneModelRecord::SetPaint "
              "sub_8215AC70, SetWheels sub_8215B238, SetChoice sub_8215B1A0 and "
              "CarRecord::SetBytes sub_8215B0E0 (with CarRecord::ResetBytes sub_820D0900, no "
              "adapter: only SetBytes calls it), SceneModelRecord::FromCar sub_8215B878 (with "
              "--native_scene_models and --native_car_specs; memcpy and AeroParts::Get82 its "
              "originals where theirs decline) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=car_makers)");

namespace hooks {

namespace cm = car_makers;

void CarMakersBlockOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::BlockOf(ctx.r3.u32);
}

void CarMakersSetPaint(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::SetPaint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void CarMakersSetWheels(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::SetWheels(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarMakersSetChoice(PPCContext& ctx, uint8_t* base) {
  cm::SetChoice(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32);
}

void CarMakersSetBytes(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::SetBytes(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarMakersFromCar(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_scene_models) || !ITC_FLAG(native_car_specs)) {
    __imp__sub_8215B878(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::FromCar(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u64, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_CAR_MAKERS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_car_makers)) {   \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_CAR_MAKERS(8215AAD0, CarMakersBlockOf)
ITC_CAR_MAKERS(8215AC70, CarMakersSetPaint)
ITC_CAR_MAKERS(8215B238, CarMakersSetWheels)
ITC_CAR_MAKERS(8215B1A0, CarMakersSetChoice)
ITC_CAR_MAKERS(8215B0E0, CarMakersSetBytes)
ITC_CAR_MAKERS(8215B878, CarMakersFromCar)
// sub_8215AB04 (BlockOf's case for id 1, the jump table's base) needs no
// adapter: only the original BlockOf reaches it. sub_820D0900 has none:
// only SetBytes calls it.
