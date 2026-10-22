// The vehicle accessors natively: adapters in front of
// src/vehicle_accessors/vehicle_accessors.cpp
// (docs/functions/vehicle-accessors.md).

#include "guest.h"

#include "base/flags.h"
#include "vehicle_accessors/vehicle_accessors.h"

ITC_FLAG_BOOL(native_vehicle_accessors, true, "ITC",
              "Run the vehicle accessors (docs/functions/vehicle-accessors.md: Vehicle::Get, "
              "sub_820C0468, and the wheels' contact records, sub_820C0170; the rigid body's "
              "position and matrix, sub_820B67D8 and sub_820B6930; the contacts' object, "
              "position and strength, sub_820B8CC0 to sub_820B8D08; Racers::IndexOf and "
              "Stages::HasRacer, sub_820FF9F8 and sub_820FFCF8; SaveData::Of, sub_82105620; "
              "the switch sub_825C8388) natively instead of through the translated code (same "
              "results; checked by --run_harnesses=vehicle_accessors)");

// Defines an adapter returning a word in r3: `address` the function's
// (sub_<address>), `expr` the result (using `ctx` and `base`).
#define ITC_ACCESSOR_WORD(address, expr)          \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    if (!ITC_FLAG(native_vehicle_accessors)) {   \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    ctx.r3.u64 = uint32_t(expr);                 \
  }

// The same for a signed word (-1 sign-extended, as the original's li).
#define ITC_ACCESSOR_SIGNED(address, expr)        \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    if (!ITC_FLAG(native_vehicle_accessors)) {   \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    ctx.r3.s64 = int32_t(expr);                  \
  }

// The same for a float returned in f1 (the original's lfs, a single widened
// to double).
#define ITC_ACCESSOR_FLOAT(address, expr)         \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    if (!ITC_FLAG(native_vehicle_accessors)) {   \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    ctx.fpscr.disableFlushMode();                \
    ctx.f1.f64 = double(expr);                   \
  }

using namespace vehicle_accessors;

ITC_ACCESSOR_WORD(820C0468, VehicleGet(base, ctx.r3.u32))
ITC_ACCESSOR_WORD(820C0170, VehicleWheelContact(ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_WORD(820B67D8, RigidBodyPosition(ctx.r3.u32))
ITC_ACCESSOR_WORD(820B6930, RigidBodyMatrix(ctx.r3.u32))
ITC_ACCESSOR_WORD(820B8CC0, ContactObject(base, ctx.r3.u32))
ITC_ACCESSOR_WORD(820B8D00, ContactPosition(ctx.r3.u32))
ITC_ACCESSOR_FLOAT(820B8D08, ContactStrength(base, ctx.r3.u32))
ITC_ACCESSOR_SIGNED(820FF9F8, RacersIndexOf(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_WORD(820FFCF8, StagesHasRacer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_ACCESSOR_WORD(82105620, SaveDataOf(base, ctx.r3.u32))
ITC_ACCESSOR_WORD(825C8388, DebugSwitchOff())
