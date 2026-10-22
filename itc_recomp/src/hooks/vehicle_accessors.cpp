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
              "the switch sub_825C8388; the rigid body's accessors and setters, sub_820B6638 "
              "to sub_820B69D8; the wheels' slip, sub_820C0180 and sub_820C0290; the vehicle's "
              "other accessors, sub_820C0140 to sub_820C0590; the collision object's and the "
              "collision world's, sub_820B7BF0 to sub_820B8D10; the +344 object's, "
              "sub_820B9658 to sub_820B99C8) natively instead of through the translated code (same "
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

// The same for a function returning nothing (`call` uses `ctx` and `base`,
// and sets r3 where the original leaves it something callers could see).
#define ITC_ACCESSOR_VOID(address, call)          \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    if (!ITC_FLAG(native_vehicle_accessors)) {   \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    ctx.fpscr.disableFlushMode();                \
    call;                                        \
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

// The rigid body (r3). The setters leave r3 at their last copy's or
// addition's destination, as the originals do.
ITC_ACCESSOR_WORD(820B6638, RigidBodyIsFixed(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B6640, RigidBodySetGravity(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 496)
ITC_ACCESSOR_VOID(820B6698, RigidBodySetMass(base, ctx.r3.u32, ctx.f1.f64);
                  ctx.r3.u64 = ctx.r3.u32 + 496)
ITC_ACCESSOR_FLOAT(820B6708, RigidBodyMass(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B67E0, RigidBodySetCenterOfMass(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 16)
ITC_ACCESSOR_WORD(820B6880, RigidBodyField80(ctx.r3.u32))
ITC_ACCESSOR_WORD(820B6888, RigidBodyVelocity(ctx.r3.u32))
ITC_ACCESSOR_WORD(820B6890, RigidBodyField144(ctx.r3.u32))
ITC_ACCESSOR_WORD(820B6898, RigidBodyField160(ctx.r3.u32))
ITC_ACCESSOR_FLOAT(820B68A0, RigidBodyFloat48(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B68C0, RigidBodyPointVelocity(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_ACCESSOR_WORD(820B6928, RigidBodyOrientation(ctx.r3.u32))
ITC_ACCESSOR_WORD(820B6948, RigidBodyAngularVelocity(ctx.r3.u32))
ITC_ACCESSOR_VOID(820B6950, RigidBodySetFloat112(base, ctx.r3.u32, ctx.f1.f64))
ITC_ACCESSOR_VOID(820B6958, RigidBodyAddForce(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 64)
ITC_ACCESSOR_VOID(820B6960, RigidBodyAddTorque(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 128)
ITC_ACCESSOR_VOID(820B6968, RigidBodyAddForceAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 64)
ITC_ACCESSOR_VOID(820B69D8, RigidBodyAddTorqueAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 128)

// The wheels' slip: r3 = the vehicle, r4 = a wheel, f1 = a threshold.
ITC_ACCESSOR_FLOAT(820C0180, VehicleWheelSlip(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_ACCESSOR_FLOAT(820C0290, VehicleMaxWheelSlip(base, ctx.r3.u32, ctx.f1.f64))

// The vehicle (r3).
ITC_ACCESSOR_FLOAT(820C0140, VehicleFloat2224(base, ctx.r3.u32))
ITC_ACCESSOR_FLOAT(820C0148, VehicleSpeed(base, ctx.r3.u32))
ITC_ACCESSOR_FLOAT(820C0150, VehicleDistancePerFrame(base, ctx.r3.u32))
ITC_ACCESSOR_WORD(820C0168, VehicleIdentity(ctx.r3.u32))
ITC_ACCESSOR_WORD(820C0450, VehicleSetObjectHalf114(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820C0490, QuaternionToMatrix3(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820C0550, QuaternionToMatrix(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820C0590, VehicleSpeedRecord(base, ctx.r3.u32, ctx.r4.u32))

// The collision object (r3) and the collision world.
ITC_ACCESSOR_VOID(820B7BF0, CollisionObjectLink(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B7C18, CollisionObjectUnlink(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_WORD(820B7C90, CollisionObjectFirstContact(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_WORD(820B7CB8, CollisionObjectWord264(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B7CC0, CollisionObjectSetWord264(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B7D18, CollisionObjectClearBits252(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B7D28, CollisionObjectSetBits256(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B7D38, CollisionObjectClearBits256(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B7D48, CollisionObjectSetBits260(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B7D58, CollisionObjectClearBits260(base, ctx.r3.u32, ctx.r4.u32))
// r3 comes back as object + 208 (the copy's destination).
ITC_ACCESSOR_VOID(820B7D68, CollisionObjectSetSize(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 208)
ITC_ACCESSOR_VOID(820B7D70, CollisionObjectSetFloat224(base, ctx.r3.u32, ctx.f1.f64))
ITC_ACCESSOR_VOID(820B7D78, CollisionRecordCopy(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_VOID(820B8768, CollisionSetGlobal78(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B8778, CollisionSetGlobal7C(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B8788, CollisionWorldSetCallback(base, ctx.r3.u32))
ITC_ACCESSOR_WORD(820B8A48, CollisionWorldNewContact(base))
ITC_ACCESSOR_WORD(820B8C90, CollisionWorldWord60(base))
ITC_ACCESSOR_WORD(820B8CA0, CollisionWorldWord64(base))
ITC_ACCESSOR_WORD(820B8CB0, CollisionWorldWord68(base))
ITC_ACCESSOR_WORD(820B8CE0, ContactOtherObject(base, ctx.r3.u32))
ITC_ACCESSOR_WORD(820B8D10, ContactNext(base, ctx.r3.u32, ctx.r4.u32))

// The vehicle's +344 object (r3).
ITC_ACCESSOR_VOID(820B9658, Object344SetFloat20(base, ctx.r3.u32, ctx.f1.f64))
ITC_ACCESSOR_VOID(820B9660, Object344SetFloat16(base, ctx.r3.u32, ctx.f1.f64))
ITC_ACCESSOR_FLOAT(820B9668, Object344Float28(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B9670, Object344SetFloat28(base, ctx.r3.u32, ctx.f1.f64))
ITC_ACCESSOR_VOID(820B9678, Object344SetFloat24(base, ctx.r3.u32, ctx.f1.f64))
ITC_ACCESSOR_WORD(820B99B0, ObjectHalf112(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B99B8, ObjectSetHalf112(base, ctx.r3.u32, ctx.r4.u32))
ITC_ACCESSOR_WORD(820B99C0, ObjectHalf114(base, ctx.r3.u32))
ITC_ACCESSOR_VOID(820B99C8, ObjectSetHalf114(base, ctx.r3.u32, ctx.r4.u32))

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
