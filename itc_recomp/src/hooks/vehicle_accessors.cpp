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

// --- The vector library (src/vehicle_accessors/vectors.cpp) ---

#include "vehicle_accessors/vectors.h"

ITC_FLAG_BOOL(native_vector_library, true, "ITC",
              "Run the game's vector library (docs/functions/vehicle-accessors.md: sqrtf, "
              "sub_82293590; the matrix helpers, sub_82293818 to sub_822938F0; the 3D vector "
              "helpers, sub_82294028 to sub_82294608) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=vehicle_accessors)");

// Defines an adapter for a function returning nothing (r3 left as it was):
// `call` uses `ctx` and `base`.
#define ITC_VECTOR_VOID(address, call)         \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_vector_library)) {   \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    ctx.fpscr.disableFlushMode();             \
    call;                                     \
  }

// The same for a single returned in f1.
#define ITC_VECTOR_FLOAT(address, expr)        \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_vector_library)) {   \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    ctx.fpscr.disableFlushMode();             \
    ctx.f1.f64 = double(expr);                \
  }

using namespace vector_library;

ITC_VECTOR_FLOAT(82293590, Sqrtf(ctx.f1.f64))
// r3 comes back as the matrix's fourth row (the original's copy's
// destination).
ITC_VECTOR_VOID(82293818, MatrixSetTranslation(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r3.u32 + 48)
ITC_VECTOR_VOID(82293880, MatrixRotate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(822938F0, MatrixTransformPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(82294028, Vec3Add(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(82294068, Vec3Sub(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(822940A8, Vec4ScaleTo(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_VECTOR_VOID(822940E0, Vec4Copy(base, ctx.r3.u32, ctx.r4.u32))
ITC_VECTOR_VOID(82294108, Vec3CrossW1(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_FLOAT(82294170, Vec3Dot(base, ctx.r3.u32, ctx.r4.u32))
ITC_VECTOR_FLOAT(822941B0, Vec3Normalize(base, ctx.r3.u32, ctx.r4.u32))
ITC_VECTOR_FLOAT(82294200, Vec3Direction(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_FLOAT(82294270, Vec3Length(base, ctx.r3.u32))
ITC_VECTOR_FLOAT(82294298, Vec3NormalizeSelf(base, ctx.r3.u32))
ITC_VECTOR_VOID(822942A0, Vec3Sum(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(822942E0, Vec3Difference(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(82294320, Vec3AddTo(base, ctx.r3.u32, ctx.r4.u32))
ITC_VECTOR_VOID(82294340, Vec4ScaleBy(base, ctx.r3.u32, ctx.f1.f64))
ITC_VECTOR_FLOAT(82294348, Vec4DivideBy(base, ctx.r3.u32, ctx.f1.f64))
ITC_VECTOR_VOID(82294360, Vec3SubCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(822943C8, Vec3AddCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(82294430, Vec4Scaled(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_VECTOR_VOID(82294470, Vec4ScaledFrom(base, ctx.r3.u32, ctx.r5.u32, ctx.f1.f64))
ITC_VECTOR_VOID(822944B0, Vec3ScaledCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_VECTOR_VOID(82294518, Vec3Cross(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_VECTOR_VOID(82294568, Vec3Lerp(base, ctx.r3.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64))
ITC_VECTOR_FLOAT(82294608, Vec3DotOf(base, ctx.r3.u32, ctx.r4.u32))
