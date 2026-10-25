// The vehicle accessors (src/hooks/vehicle_accessors.cpp) against the
// recompiled functions. Vehicles, bodies, contacts and the race data are live
// guest objects: the cases build their own in a block taken from the system
// heap for the run, so nothing the game holds is written, except the vehicle
// array's pointer (0x826C2E34), which Vehicle::Get's cases point at the
// block and put back.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "vehicle_accessors/vectors.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <functional>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_vector_library);

namespace harness {

namespace {

namespace va = vehicle_accessors;

constexpr uint32_t kVehicles = 0x826C2E34;

// The register steps: the cases run the plain C++ through these (not the
// adapters, so they keep working once an adapter goes), against the
// recompiled versions.
#define ITC_NATIVE_WORD(name, expr)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.r3.u64 = uint32_t(expr);              \
  }
#define ITC_NATIVE_SIGNED(name, expr)         \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.r3.s64 = int32_t(expr);               \
  }
#define ITC_NATIVE_FLOAT(name, expr)          \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    ctx.f1.f64 = double(expr);                \
  }

ITC_NATIVE_WORD(NativeVehicleGet, va::VehicleGet(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleWheelContact, va::VehicleWheelContact(ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeRigidBodyPosition, va::RigidBodyPosition(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyMatrix, va::RigidBodyMatrix(ctx.r3.u32))
ITC_NATIVE_WORD(NativeContactObject, va::ContactObject(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeContactPosition, va::ContactPosition(ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeContactStrength, va::ContactStrength(base, ctx.r3.u32))
ITC_NATIVE_SIGNED(NativeRacersIndexOf, va::RacersIndexOf(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeStagesHasRacer,
                va::StagesHasRacer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_WORD(NativeSaveDataOf, va::SaveDataOf(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeDebugSwitchOff, va::DebugSwitchOff())

// The rigid body's and the wheels' (r3 as the adapters leave it).
ITC_NATIVE_WORD(NativeRigidBodyIsFixed, va::RigidBodyIsFixed(base, ctx.r3.u32))
void NativeRigidBodySetGravity(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodySetGravity(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u32 + 496;
}
void NativeRigidBodySetMass(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodySetMass(base, ctx.r3.u32, ctx.f1.f64);
  ctx.r3.u64 = ctx.r3.u32 + 496;
}
ITC_NATIVE_FLOAT(NativeRigidBodyMass, va::RigidBodyMass(base, ctx.r3.u32))
void NativeRigidBodySetCenterOfMass(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodySetCenterOfMass(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u32 + 16;
}
ITC_NATIVE_WORD(NativeRigidBodyField80, va::RigidBodyField80(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyVelocity, va::RigidBodyVelocity(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyField144, va::RigidBodyField144(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyForce, va::RigidBodyForce(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyTorque, va::RigidBodyTorque(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyField160, va::RigidBodyField160(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyOrientation, va::RigidBodyOrientation(ctx.r3.u32))
ITC_NATIVE_WORD(NativeRigidBodyAngularVelocity, va::RigidBodyAngularVelocity(ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeRigidBodyFloat48, va::RigidBodyFloat48(base, ctx.r3.u32))
void NativeRigidBodyPointVelocity(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodyPointVelocity(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
void NativeRigidBodySetFloat112(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodySetFloat112(base, ctx.r3.u32, ctx.f1.f64);
}
void NativeRigidBodyAddForce(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodyAddForce(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u32 + 64;
}
void NativeRigidBodyAddTorque(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodyAddTorque(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u32 + 128;
}
void NativeRigidBodyAddForceAt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodyAddForceAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = ctx.r3.u32 + 64;
}
void NativeRigidBodyAddTorqueAt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::RigidBodyAddTorqueAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = ctx.r3.u32 + 128;
}
ITC_NATIVE_FLOAT(NativeVehicleWheelSlip,
                 va::VehicleWheelSlip(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_FLOAT(NativeVehicleMaxWheelSlip,
                 va::VehicleMaxWheelSlip(base, ctx.r3.u32, ctx.f1.f64))

ITC_NATIVE_FLOAT(NativeVehicleFloat2224, va::VehicleFloat2224(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVehicleSpeed, va::VehicleSpeed(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVehicleDistancePerFrame, va::VehicleDistancePerFrame(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleIdentity, va::VehicleIdentity(ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleSetObjectHalf114,
                va::VehicleSetObjectHalf114(base, ctx.r3.u32, ctx.r4.u32))

#define ITC_NATIVE_SET(name, call)            \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    call;                                     \
  }

ITC_NATIVE_SET(NativeQuaternionToMatrix3, va::QuaternionToMatrix3(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeQuaternionToMatrix, va::QuaternionToMatrix(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeVehicleSpeedRecord, va::VehicleSpeedRecord(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectLink, va::CollisionObjectLink(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectUnlink,
               va::CollisionObjectUnlink(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeCollisionObjectFirstContact,
                va::CollisionObjectFirstContact(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeCollisionObjectWord264, va::CollisionObjectWord264(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeCollisionObjectSetWord264,
               va::CollisionObjectSetWord264(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectClearBits252,
               va::CollisionObjectClearBits252(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectSetBits256,
               va::CollisionObjectSetBits256(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectClearBits256,
               va::CollisionObjectClearBits256(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectSetBits260,
               va::CollisionObjectSetBits260(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionObjectClearBits260,
               va::CollisionObjectClearBits260(base, ctx.r3.u32, ctx.r4.u32))
void NativeCollisionObjectSetSize(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  va::CollisionObjectSetSize(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u32 + 208;
}
ITC_NATIVE_SET(NativeCollisionObjectSetFloat224,
               va::CollisionObjectSetFloat224(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_SET(NativeCollisionRecordCopy, va::CollisionRecordCopy(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeCollisionSetGlobal78, va::CollisionSetGlobal78(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeCollisionSetGlobal7C, va::CollisionSetGlobal7C(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeCollisionWorldSetCallback, va::CollisionWorldSetCallback(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeCollisionWorldNewContact, va::CollisionWorldNewContact(base))
ITC_NATIVE_WORD(NativeCollisionWorldWord60, va::CollisionWorldWord60(base))
ITC_NATIVE_WORD(NativeCollisionWorldWord64, va::CollisionWorldWord64(base))
ITC_NATIVE_WORD(NativeCollisionWorldWord68, va::CollisionWorldWord68(base))
ITC_NATIVE_WORD(NativeContactOtherObject, va::ContactOtherObject(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeContactNext, va::ContactNext(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativeObject344SetFloat20, va::Object344SetFloat20(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_SET(NativeObject344SetFloat16, va::Object344SetFloat16(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_FLOAT(NativeObject344Float28, va::Object344Float28(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeObject344SetFloat28, va::Object344SetFloat28(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_SET(NativeObject344SetFloat24, va::Object344SetFloat24(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_WORD(NativeObjectHalf112, va::ObjectHalf112(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeObjectSetHalf112, va::ObjectSetHalf112(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeObjectHalf114, va::ObjectHalf114(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeObjectSetHalf114, va::ObjectSetHalf114(base, ctx.r3.u32, ctx.r4.u32))

ITC_NATIVE_SET(NativeRaceDataSetWord59618, va::RaceDataSetWord59618(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeRaceDataSetWord5961C, va::RaceDataSetWord5961C(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeRaceDataSetWord32, va::RaceDataSetWord32(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeStagesHasNext, va::StagesHasNext(base, ctx.r3.u32))
ITC_NATIVE_SET(NativeStagesSetType, va::StagesSetType(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SET(NativeStagesSetWord808, va::StagesSetWord808(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeStagesWord808, va::StagesWord808(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeStagesType, va::StagesType(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeStagesWord4, va::StagesWord4(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeStagesIsType2Or3, va::StagesIsType2Or3(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeStagesWord76, va::StagesWord76(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeStagesRacer, va::StagesRacer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_WORD(NativeStagesWord48, va::StagesWord48(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SET(NativeStagesCopy24, va::StagesCopy24(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// Where the native version declines, the original runs (as the adapter).
void NativeStagesCopy(PPCContext& ctx, uint8_t* base) {
  if (!va::StagesCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_820FFD88(ctx, base);
  }
}
ITC_NATIVE_SET(NativeEntriesAdd, va::EntriesAdd(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
void NativeRacerRecordsAdd(PPCContext& ctx, uint8_t* base) {
  uint32_t added = 0;
  if (!va::RacerRecordsAdd(base, ctx.r3.u32, ctx.r4.u32, &added)) {
    __imp__sub_820FFE08(ctx, base);
    return;
  }
  ctx.r3.u64 = added;
}
ITC_NATIVE_WORD(NativeRacerRecordsAt, va::RacerRecordsAt(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativePairCopy, va::PairCopy(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativePairsLast, va::PairsLast(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SET(NativePairsAt, va::PairsAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_WORD(NativeDebugSwitchOn, va::DebugSwitchOn())
ITC_NATIVE_SET(NativeListInit, va::ListInit(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeListAt, va::ListAt(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SIGNED(NativeScenePoolsClaimA, va::ScenePoolsClaimA(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SIGNED(NativeScenePoolsClaimB, va::ScenePoolsClaimB(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeScenePoolsGetA,
                va::ScenePoolsGetA(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_WORD(NativeScenePoolsGetB,
                va::ScenePoolsGetB(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_WORD(NativeScenePoolsFindModel,
                va::ScenePoolsFindModel(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_WORD(NativeObjectRecord1888, va::ObjectRecord1888(ctx.r3.u32))

namespace vl = vector_library;

#define ITC_NATIVE_VOID(name, call)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    call;                                     \
  }

ITC_NATIVE_FLOAT(NativeSqrtf, vl::Sqrtf(ctx.f1.f64))
void NativeMatrixSetTranslation(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vl::MatrixSetTranslation(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u32 + 48;
}
ITC_NATIVE_VOID(NativeMatrixRotate, vl::MatrixRotate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeMatrixTransformPoint,
                vl::MatrixTransformPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec3Add, vl::Vec3Add(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec3Sub, vl::Vec3Sub(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec4ScaleTo, vl::Vec4ScaleTo(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_VOID(NativeVec4Copy, vl::Vec4Copy(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_VOID(NativeVec3CrossW1, vl::Vec3CrossW1(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_FLOAT(NativeVec3Dot, vl::Vec3Dot(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeVec3Normalize, vl::Vec3Normalize(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeVec3Direction,
                 vl::Vec3Direction(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_FLOAT(NativeVec3Length, vl::Vec3Length(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVec3NormalizeSelf, vl::Vec3NormalizeSelf(base, ctx.r3.u32))
ITC_NATIVE_VOID(NativeVec3Sum, vl::Vec3Sum(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec3Difference,
                vl::Vec3Difference(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec3AddTo, vl::Vec3AddTo(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_VOID(NativeVec4ScaleBy, vl::Vec4ScaleBy(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_FLOAT(NativeVec4DivideBy, vl::Vec4DivideBy(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_VOID(NativeVec3SubCopy, vl::Vec3SubCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec3AddCopy, vl::Vec3AddCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec4Scaled, vl::Vec4Scaled(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_VOID(NativeVec4ScaledFrom,
                vl::Vec4ScaledFrom(base, ctx.r3.u32, ctx.r5.u32, ctx.f1.f64))
ITC_NATIVE_VOID(NativeVec3ScaledCopy,
                vl::Vec3ScaledCopy(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_VOID(NativeVec3Cross, vl::Vec3Cross(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VOID(NativeVec3Lerp,
                vl::Vec3Lerp(base, ctx.r3.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64))
ITC_NATIVE_FLOAT(NativeVec3DotOf, vl::Vec3DotOf(base, ctx.r3.u32, ctx.r4.u32))

// Runs the recompiled version with the flags off, so the functions it calls
// that are ours run the originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_vehicle_accessors);
  const bool vectors = ITC_FLAG(native_vector_library);
  ITC_FLAG(native_vehicle_accessors) = false;
  ITC_FLAG(native_vector_library) = false;
  recompiled(t, base);
  ITC_FLAG(native_vehicle_accessors) = native;
  ITC_FLAG(native_vector_library) = vectors;
}

// The odd float words mixed into fields: NaNs of both kinds and signs,
// infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t RandomWord(Random& r) {
  return r.Below(4) == 0 ? kOddFloats[r.Below(std::size(kOddFloats))] : uint32_t(r.Next());
}

// Random words (odd floats among them) over `bytes` at `at`.
void FillRandom(uint8_t* base, Random& r, uint32_t at, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, at + i, RandomWord(r));
  }
}

// A generator `seed` steps along, so a case's setup writes the same words
// for both versions.
Random Seeded(uint64_t seed) {
  Random r;
  for (uint64_t i = 0; i < seed; i++) {
    r.Next();
  }
  return r;
}

using Regions = std::vector<std::pair<uint32_t, uint32_t>>;

// One case through both versions: `setup()` writes the inputs (the same
// each time), `args(t)` sets the registers, the function runs; the regions'
// bytes, r3 and f1 are compared.
template <typename Setup, typename Args>
void Compare(Context& h, Results& results, const Regions& regions, Setup setup, Args args,
             GuestFunction recompiled, GuestFunction native, const std::string& what,
             bool compare_f1 = true, bool compare_r3 = true) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    setup();
    PPCContext t = CallContext(h);
    // Flush-to-zero off, in the context's copy of the control word too: the
    // harness's context may carry a stale "on" that would keep the
    // translated code's enableFlushMode from setting it (the game's own
    // contexts keep the two in step).
    t.fpscr.disableFlushModeUnconditional();
    args(t);
    if (v == 0) {
      RunRecompiled(recompiled, t, h.base);
    } else {
      native(t, h.base);
    }
    for (const auto& [at, bytes] : regions) {
      const std::vector<uint8_t> part = Read(h.base, at, bytes);
      result[v].insert(result[v].end(), part.begin(), part.end());
    }
    for (int i = 0; compare_r3 && i < 4; i++) {
      result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
    }
    for (int i = 0; compare_f1 && i < 8; i++) {
      result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
    }
  }
  results.Check(result[0], result[1], what);
}

// Words to try as addresses or indices: small ones, edges and random ones.
std::vector<uint32_t> Words(Random& r, int random_count) {
  std::vector<uint32_t> words = {0, 1, 2, 3, 4, 5, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF,
                                 0x40000000, 0x82000000};
  for (int i = 0; i < random_count; i++) {
    words.push_back(uint32_t(r.Next()));
  }
  return words;
}

constexpr uint32_t kBlockBytes = 0x60000;  // 93 racer records fit (366,048 bytes)
constexpr uint32_t kRecordBytesHarness = 3936;

// The getters that only add to their argument: random arguments, upper
// halves included.
void AddressCases(Context& h) {
  Random& r = h.random;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } functions[] = {
      {"RigidBody::Position (sub_820B67D8)", __imp__sub_820B67D8, NativeRigidBodyPosition},
      {"RigidBody::Matrix (sub_820B6930)", __imp__sub_820B6930, NativeRigidBodyMatrix},
      {"Contact::Position (sub_820B8D00)", __imp__sub_820B8D00, NativeContactPosition},
      {"DebugSwitchOff (sub_825C8388)", __imp__sub_825C8388, NativeDebugSwitchOff},
      {"DebugSwitchOn (sub_825C8390)", __imp__sub_825C8390, NativeDebugSwitchOn},
      {"Object::Record1888 (sub_820C4550)", __imp__sub_820C4550, NativeObjectRecord1888},
      {"Vehicle::Identity (sub_820C0168)", __imp__sub_820C0168, NativeVehicleIdentity},
      {"RigidBody::Field80 (sub_820B6880)", __imp__sub_820B6880, NativeRigidBodyField80},
      {"RigidBody::Velocity (sub_820B6888)", __imp__sub_820B6888, NativeRigidBodyVelocity},
      {"RigidBody::Field144 (sub_820B6890)", __imp__sub_820B6890, NativeRigidBodyField144},
      {"RigidBody::Force (sub_820988C0)", __imp__sub_820988C0, NativeRigidBodyForce},
      {"RigidBody::Torque (sub_82379328)", __imp__sub_82379328, NativeRigidBodyTorque},
      {"RigidBody::Field160 (sub_820B6898)", __imp__sub_820B6898, NativeRigidBodyField160},
      {"RigidBody::Orientation (sub_820B6928)", __imp__sub_820B6928,
       NativeRigidBodyOrientation},
      {"RigidBody::AngularVelocity (sub_820B6948)", __imp__sub_820B6948,
       NativeRigidBodyAngularVelocity},
  };
  for (const auto& f : functions) {
    Results results(f.name);
    for (uint32_t word : Words(r, 30)) {
      const uint64_t upper = r.Next() << 32;
      Compare(h, results, {}, [] {}, [&](PPCContext& t) { t.r3.u64 = upper | word; },
              f.recompiled, f.native, fmt::format("r3 {:X}", word));
    }
    results.Report();
  }

  Results results("Vehicle::WheelContact (sub_820C0170)");
  for (uint32_t vehicle : Words(r, 6)) {
    for (uint32_t wheel : Words(r, 3)) {
      Compare(h, results, {}, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = vehicle;
                t.r4.u64 = wheel;
              },
              __imp__sub_820C0170, NativeVehicleWheelContact,
              fmt::format("vehicle {:X} wheel {:X}", vehicle, wheel));
    }
  }
  results.Report();
}

void VehicleGetCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t saved = guest::LoadU32(h.base, kVehicles);
  Results results("Vehicle::Get (sub_820C0468)");
  for (uint32_t array : {0u, block, saved}) {
    for (uint32_t index : Words(r, 10)) {
      Compare(h, results, {}, [&] { guest::StoreU32(h.base, kVehicles, array); },
              [&](PPCContext& t) { t.r3.u64 = index; }, __imp__sub_820C0468, NativeVehicleGet,
              fmt::format("array {:X} index {:X}", array, index));
    }
  }
  guest::StoreU32(h.base, kVehicles, saved);
  results.Report();
}

void ContactCases(Context& h, uint32_t block) {
  {
    Results results("Contact::Object (sub_820B8CC0)");
    for (uint64_t seed = 0; seed < 40; seed++) {
      Compare(h, results, {},
              [&] {
                Random fill = Seeded(seed);
                FillRandom(h.base, fill, block, 128);
                if (seed % 2) {
                  // The top byte of the flags clear: the object is returned.
                  guest::StoreU32(h.base, block + 4, uint32_t(fill.Next()) & 0x00FFFFFF);
                }
              },
              [&](PPCContext& t) { t.r3.u64 = block; }, __imp__sub_820B8CC0,
              NativeContactObject, fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("Contact::Strength (sub_820B8D08)");
    std::vector<uint32_t> values(std::begin(kOddFloats), std::end(kOddFloats));
    for (int i = 0; i < 30; i++) {
      values.push_back(uint32_t(h.random.Next()));
    }
    for (uint32_t value : values) {
      Compare(h, results, {}, [&] { guest::StoreU32(h.base, block + 64, value); },
              [&](PPCContext& t) { t.r3.u64 = block; }, __imp__sub_820B8D08,
              NativeContactStrength, fmt::format("value {:08X}", value));
    }
    results.Report();
  }
}

void RaceDataCases(Context& h, uint32_t block) {
  Random& r = h.random;
  {
    // Ids 0-11 in the records, so some searches find one and some don't.
    Results results("Racers::IndexOf (sub_820FF9F8)");
    for (uint64_t seed = 0; seed < 4; seed++) {
      for (uint32_t id : {0u, 1u, 3u, 7u, 11u, 12u, 0xFFFFFFFFu}) {
        Compare(h, results, {},
                [&] {
                  Random fill = Seeded(seed);
                  for (uint32_t i = 0; i < 93; i++) {
                    guest::StoreU32(h.base, block + 3912 + 3936 * i,
                                    seed == 3 ? 0xFFFFFFFF : fill.Below(4 * (seed + 1) + 50));
                  }
                },
                [&](PPCContext& t) {
                  t.r3.u64 = block;
                  t.r4.u64 = id;
                },
                __imp__sub_820FF9F8, NativeRacersIndexOf,
                fmt::format("seed {} id {}", seed, int32_t(id)));
      }
    }
    results.Report();
  }
  {
    // Ten stages; slots from -1 to 5.
    Results results("Stages::HasRacer (sub_820FFCF8)");
    for (uint64_t seed = 0; seed < 12; seed++) {
      for (uint32_t stage : {0xFFFFFFFFu, 0u, 1u, 4u, 9u}) {
        for (uint32_t racer : {0u, 2u, 5u, 0xFFFFFFFFu}) {
          Compare(h, results, {},
                  [&] {
                    Random fill = Seeded(seed);
                    FillRandom(h.base, fill, block, 808 + 4);
                    for (uint32_t s = 0; s < 10; s++) {
                      for (uint32_t i = 0; i < 4; i++) {
                        guest::StoreU32(h.base, block + 80 * s + 44 + 8 * i,
                                        uint32_t(int32_t(fill.Below(7)) - 1));
                      }
                    }
                    guest::StoreU32(h.base, block + 804, fill.Below(10));
                  },
                  [&](PPCContext& t) {
                    t.r3.u64 = block;
                    t.r4.u64 = racer;
                    t.r5.u64 = stage;
                  },
                  __imp__sub_820FFCF8, NativeStagesHasRacer,
                  fmt::format("seed {} stage {} racer {}", seed, int32_t(stage), int32_t(racer)));
        }
      }
    }
    results.Report();
  }
  {
    Results results("SaveData::Of (sub_82105620)");
    for (uint32_t player = 0; player < 16; player++) {
      Compare(h, results, {}, [] {}, [&](PPCContext& t) { t.r3.u64 = player; },
              __imp__sub_82105620, NativeSaveDataOf, fmt::format("player {}", player));
    }
    Compare(h, results, {}, [] {}, [&](PPCContext& t) { t.r3.u64 = uint64_t(-1); },
            __imp__sub_82105620, NativeSaveDataOf, "player -1");
    results.Report();
  }
  (void)r;
}

// Float words for vectors: odd floats, random bits, and ordinary values
// (-64 to 64) so the arithmetic is mostly on numbers.
uint32_t FloatWord(Random& r) {
  switch (r.Below(4)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}

// Tiny float words (below 2^-60, either sign): their products are
// denormal, so the vector unit's flush-to-zero shows.
uint32_t TinyWord(Random& r) {
  return std::bit_cast<uint32_t>(std::ldexp(float(int32_t(r.Below(1 << 20)) - (1 << 19)), -80));
}

// A scalar argument for f1 (a single widened, as callers pass).
double FloatArgument(Random& r) {
  return double(std::bit_cast<float>(FloatWord(r)));
}

// The vectors' cases: a, b, the output and a matrix in the block, filled
// anew for each case; the output also put on a or b (the original's loads
// and stores in its order make that matter), and a on b. All 0x400 bytes
// compared.
constexpr uint32_t kA = 0x100, kB = 0x200, kOut = 0x300, kMatrix = 0x340;

void VectorCases(Context& h, uint32_t block) {
  Random& r = h.random;
  enum Shape { kR3, kR3R4, kR3R4R5, kR3R4F1, kR3R5F1, kR3F1, kR3R5R6F1, kF1, kR3MatR5, kR3R4Mat };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Shape shape;
    bool compare_f1;
  } functions[] = {
      {"sqrtf (sub_82293590)", __imp__sub_82293590, NativeSqrtf, kF1, true},
      {"Matrix::SetTranslation (sub_82293818)", __imp__sub_82293818, NativeMatrixSetTranslation,
       kR3R4Mat, true},
      {"Matrix::Rotate (sub_82293880)", __imp__sub_82293880, NativeMatrixRotate, kR3MatR5, true},
      // f1 is a scratch register there (left the matrix's +48); callers
      // don't read it, so it isn't compared.
      {"Matrix::TransformPoint (sub_822938F0)", __imp__sub_822938F0, NativeMatrixTransformPoint,
       kR3MatR5, false},
      {"Vec3::Add (sub_82294028)", __imp__sub_82294028, NativeVec3Add, kR3R4R5, true},
      {"Vec3::Sub (sub_82294068)", __imp__sub_82294068, NativeVec3Sub, kR3R4R5, true},
      {"Vec4::ScaleTo (sub_822940A8)", __imp__sub_822940A8, NativeVec4ScaleTo, kR3R4F1, true},
      {"Vec4::Copy (sub_822940E0)", __imp__sub_822940E0, NativeVec4Copy, kR3R4R5, true},
      {"Vec3::CrossW1 (sub_82294108)", __imp__sub_82294108, NativeVec3CrossW1, kR3R4R5, true},
      {"Vec3::Dot (sub_82294170)", __imp__sub_82294170, NativeVec3Dot, kR3R4, true},
      {"Vec3::Normalize (sub_822941B0)", __imp__sub_822941B0, NativeVec3Normalize, kR3R4R5, true},
      {"Vec3::Direction (sub_82294200)", __imp__sub_82294200, NativeVec3Direction, kR3R4R5, true},
      {"Vec3::Length (sub_82294270)", __imp__sub_82294270, NativeVec3Length, kR3, true},
      {"Vec3::NormalizeSelf (sub_82294298)", __imp__sub_82294298, NativeVec3NormalizeSelf, kR3,
       true},
      {"Vec3::Sum (sub_822942A0)", __imp__sub_822942A0, NativeVec3Sum, kR3R4R5, true},
      {"Vec3::Difference (sub_822942E0)", __imp__sub_822942E0, NativeVec3Difference, kR3R4R5,
       true},
      {"Vec3::AddTo (sub_82294320)", __imp__sub_82294320, NativeVec3AddTo, kR3R4, true},
      {"Vec4::ScaleBy (sub_82294340)", __imp__sub_82294340, NativeVec4ScaleBy, kR3F1, true},
      {"Vec4::DivideBy (sub_82294348)", __imp__sub_82294348, NativeVec4DivideBy, kR3F1, true},
      {"Vec3::SubCopy (sub_82294360)", __imp__sub_82294360, NativeVec3SubCopy, kR3R4R5, true},
      {"Vec3::AddCopy (sub_822943C8)", __imp__sub_822943C8, NativeVec3AddCopy, kR3R4R5, true},
      {"Vec4::Scaled (sub_82294430)", __imp__sub_82294430, NativeVec4Scaled, kR3R4F1, true},
      {"Vec4::ScaledFrom (sub_82294470)", __imp__sub_82294470, NativeVec4ScaledFrom, kR3R5F1,
       true},
      {"Vec3::ScaledCopy (sub_822944B0)", __imp__sub_822944B0, NativeVec3ScaledCopy, kR3R4F1,
       true},
      {"Vec3::Cross (sub_82294518)", __imp__sub_82294518, NativeVec3Cross, kR3R4R5, true},
      {"Vec3::Lerp (sub_82294568)", __imp__sub_82294568, NativeVec3Lerp, kR3R5R6F1, true},
      {"Vec3::DotOf (sub_82294608)", __imp__sub_82294608, NativeVec3DotOf, kR3R4, true},
  };
  // Where the output and inputs go: apart, the output on a, on b, a on b,
  // and (for the dot product's unaligned loads) everything 4 bytes on.
  const struct {
    uint32_t out, a, b;
    const char* name;
  } layouts[] = {
      {kOut, kA, kB, "apart"},          {kA, kA, kB, "out on a"},
      {kB, kA, kB, "out on b"},         {kOut, kA, kA, "a on b"},
      {kOut + 4, kA + 4, kB + 8, "unaligned"},
  };
  // Whether the case's inputs (the vectors and matrix the shape reads, and
  // f1 where it's read) hold two or more NaNs: then which NaN comes out
  // depends on the order the compiler gave each operation's operands (the
  // Xenon keeps the first; x86 keeps the first of its own order, and the
  // compiler may swap the operands of an addition or choose an FMA form),
  // in the translated code as in ours, so those cases go in their own group
  // (expected to differ).
  auto two_nans = [&](Shape shape, uint32_t out, uint32_t a, uint32_t b, double f1) {
    std::vector<uint32_t> inputs;
    auto add = [&](uint32_t at, uint32_t bytes) {
      for (uint32_t i = 0; i < bytes; i += 4) {
        if (std::find(inputs.begin(), inputs.end(), at + i) == inputs.end()) {
          inputs.push_back(at + i);
        }
      }
    };
    bool uses_f1 = false;
    switch (shape) {
      case kR3:
        add(a, 16);
        break;
      case kR3R4:
        add(out, 16);
        add(a, 16);
        break;
      case kR3R4R5:
        add(a, 16);
        add(b, 16);
        break;
      case kR3R4F1:
      case kR3R5F1:
        add(a, 16);
        uses_f1 = true;
        break;
      case kR3F1:
        add(out, 16);
        uses_f1 = true;
        break;
      case kR3R5R6F1:
        add(a, 16);
        add(b, 16);
        uses_f1 = true;
        break;
      case kF1:
        uses_f1 = true;
        break;
      case kR3MatR5:
        // The matrix's words read: each row's x, y, z.
        for (uint32_t row = 0; row < 64; row += 16) {
          add(block + kMatrix + row, 12);
        }
        add(a, 16);
        break;
      case kR3R4Mat:
        add(a, 16);
        break;
    }
    int nans = uses_f1 && std::isnan(f1) ? 1 : 0;
    for (uint32_t at : inputs) {
      nans += std::isnan(guest::LoadFloat(h.base, at)) ? 1 : 0;
    }
    return nans >= 2;
  };
  for (const auto& f : functions) {
    Results results(f.name);
    Results nan_results(std::string(f.name) + ", two or more NaN inputs (expected to differ)");
    for (const auto& layout : layouts) {
      for (uint64_t seed = 0; seed < 40; seed++) {
        const double f1 = FloatArgument(r);
        Random peek = Seeded(seed * 7 + 3);
        for (uint32_t i = 0; i < 0x400; i += 4) {
          guest::StoreU32(h.base, block + i, FloatWord(peek));
        }
        const bool nans = two_nans(f.shape, block + layout.out, block + layout.a,
                                   block + layout.b, f1);
        Compare(
            h, nans ? nan_results : results, {{block, 0x400}},
            [&] {
              Random fill = Seeded(seed * 7 + 3);
              for (uint32_t i = 0; i < 0x400; i += 4) {
                guest::StoreU32(h.base, block + i, FloatWord(fill));
              }
            },
            [&](PPCContext& t) {
              const uint32_t out = block + layout.out, a = block + layout.a,
                             b = block + layout.b, matrix = block + kMatrix;
              t.f1.f64 = f1;
              switch (f.shape) {
                case kR3:
                  t.r3.u64 = a;
                  break;
                case kR3R4:
                case kR3R4R5:
                case kR3R4F1:
                  t.r3.u64 = out;
                  t.r4.u64 = a;
                  t.r5.u64 = b;
                  break;
                case kR3R5F1:
                  t.r3.u64 = out;
                  t.r5.u64 = a;
                  break;
                case kR3F1:
                  t.r3.u64 = out;
                  break;
                case kR3R5R6F1:
                  t.r3.u64 = out;
                  t.r5.u64 = a;
                  t.r6.u64 = b;
                  break;
                case kF1:
                  break;
                case kR3MatR5:
                  t.r3.u64 = out;
                  t.r4.u64 = matrix;
                  t.r5.u64 = a;
                  break;
                case kR3R4Mat:
                  t.r3.u64 = matrix;
                  t.r4.u64 = a;
                  break;
              }
            },
            f.recompiled, f.native,
            fmt::format("{} seed {} f1 {:016X}", layout.name, seed, std::bit_cast<uint64_t>(f1)),
            f.compare_f1);
      }
    }
    results.Report();
    nan_results.Report();
    // Tiny vectors (and f1): the block filled with tiny words only.
    Results tiny_results(std::string(f.name) + ", tiny values");
    for (const auto& layout : layouts) {
      for (uint64_t seed = 0; seed < 20; seed++) {
        Random pick = Seeded(seed * 5 + 1);
        const double f1 = double(std::bit_cast<float>(TinyWord(pick)));
        Compare(
            h, tiny_results, {{block, 0x400}},
            [&] {
              Random fill = Seeded(seed * 7 + 3);
              for (uint32_t i = 0; i < 0x400; i += 4) {
                guest::StoreU32(h.base, block + i, TinyWord(fill));
              }
            },
            [&](PPCContext& t) {
              const uint32_t out = block + layout.out, a = block + layout.a,
                             b = block + layout.b, matrix = block + kMatrix;
              t.f1.f64 = f1;
              switch (f.shape) {
                case kR3:
                  t.r3.u64 = a;
                  break;
                case kR3R4:
                case kR3R4R5:
                case kR3R4F1:
                  t.r3.u64 = out;
                  t.r4.u64 = a;
                  t.r5.u64 = b;
                  break;
                case kR3R5F1:
                  t.r3.u64 = out;
                  t.r5.u64 = a;
                  break;
                case kR3F1:
                  t.r3.u64 = out;
                  break;
                case kR3R5R6F1:
                  t.r3.u64 = out;
                  t.r5.u64 = a;
                  t.r6.u64 = b;
                  break;
                case kF1:
                  break;
                case kR3MatR5:
                  t.r3.u64 = out;
                  t.r4.u64 = matrix;
                  t.r5.u64 = a;
                  break;
                case kR3R4Mat:
                  t.r3.u64 = matrix;
                  t.r4.u64 = a;
                  break;
              }
            },
            f.recompiled, f.native,
            fmt::format("{} seed {} f1 {:016X}", layout.name, seed, std::bit_cast<uint64_t>(f1)),
            f.compare_f1);
      }
    }
    tiny_results.Report();
  }
}

// Float words without NaNs (infinities, denormals and signed zeros kept):
// where no NaN is an input, the NaNs that come out are the default one, the
// same in both versions.
uint32_t NumberWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (std::isnan(std::bit_cast<float>(word)));
  return word;
}

// The rigid body's cases: a body (0x200 bytes) and three vectors in the
// block, filled anew for each case, numbers only in one group and NaNs
// allowed in another (expected to differ: where two NaNs meet, which comes
// out follows the compiler's operand order); the vectors also put inside
// the body (the originals' order of loads and stores makes that matter).
constexpr uint32_t kBody = 0x1000, kVector = 0x1400, kPoint = 0x1440, kBodyOut = 0x1480;

void RigidBodyCases(Context& h, uint32_t block) {
  Random& r = h.random;
  enum Shape { kBodyOnly, kBodyF1, kBodyVector, kBodyVectorPoint, kOutBodyPoint };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Shape shape;
    bool compare_f1;
    // Whether it does arithmetic on two inputs (where two NaNs can meet).
    bool arithmetic = true;
  } functions[] = {
      {"RigidBody::IsFixed (sub_820B6638)", __imp__sub_820B6638, NativeRigidBodyIsFixed,
       kBodyOnly, true, false},
      // f1 is left the mass there (a scratch register callers don't read).
      {"RigidBody::SetGravity (sub_820B6640)", __imp__sub_820B6640, NativeRigidBodySetGravity,
       kBodyVector, false},
      {"RigidBody::SetMass (sub_820B6698)", __imp__sub_820B6698, NativeRigidBodySetMass, kBodyF1,
       true},
      {"RigidBody::Mass (sub_820B6708)", __imp__sub_820B6708, NativeRigidBodyMass, kBodyOnly,
       true, false},
      {"RigidBody::SetCenterOfMass (sub_820B67E0)", __imp__sub_820B67E0,
       NativeRigidBodySetCenterOfMass, kBodyVector, true},
      {"RigidBody::Float48 (sub_820B68A0)", __imp__sub_820B68A0, NativeRigidBodyFloat48,
       kBodyOnly, true, false},
      {"RigidBody::PointVelocity (sub_820B68C0)", __imp__sub_820B68C0,
       NativeRigidBodyPointVelocity, kOutBodyPoint, true},
      {"RigidBody::SetFloat112 (sub_820B6950)", __imp__sub_820B6950, NativeRigidBodySetFloat112,
       kBodyF1, true, false},
      {"RigidBody::AddForce (sub_820B6958)", __imp__sub_820B6958, NativeRigidBodyAddForce,
       kBodyVector, true},
      {"RigidBody::AddTorque (sub_820B6960)", __imp__sub_820B6960, NativeRigidBodyAddTorque,
       kBodyVector, true},
      {"RigidBody::AddForceAt (sub_820B6968)", __imp__sub_820B6968, NativeRigidBodyAddForceAt,
       kBodyVectorPoint, true},
      {"RigidBody::AddTorqueAt (sub_820B69D8)", __imp__sub_820B69D8, NativeRigidBodyAddTorqueAt,
       kBodyVectorPoint, true},
  };
  // Where the vector, the point and the output go: apart, or inside the
  // body (on fields the functions read or write).
  const struct {
    uint32_t vector, point, out;
    const char* name;
  } layouts[] = {
      {kVector, kPoint, kBodyOut, "apart"},
      {kBody + 64, kBody + 16, kBody + 32, "inside the body"},
      {kBody + 128, kBody + 208, kBody + 96, "inside the body, elsewhere"},
      {kBody + 480, kBody + 192, kBody + 16, "on the gravity and center"},
  };
  for (const auto& f : functions) {
    Results results(f.name);
    Results nan_results(std::string(f.name) + ", NaN inputs (expected to differ)");
    for (const auto& layout : layouts) {
      for (uint64_t seed = 0; seed < 40; seed++) {
        const bool nans = seed % 4 == 3;
        const double f1 = double(std::bit_cast<float>(nans ? FloatWord(r) : NumberWord(r)));
        Compare(
            h, nans && f.arithmetic ? nan_results : results, {{block + kBody, 0x500}},
            [&] {
              Random fill = Seeded(seed * 5 + 1);
              for (uint32_t i = 0; i < 0x500; i += 4) {
                guest::StoreU32(h.base, block + kBody + i,
                                nans ? FloatWord(fill) : NumberWord(fill));
              }
              // Fixed or not.
              guest::StoreU8(h.base, block + kBody, seed % 3 == 0 ? 0 : uint8_t(fill.Next()));
            },
            [&](PPCContext& t) {
              t.f1.f64 = f1;
              switch (f.shape) {
                case kBodyOnly:
                case kBodyF1:
                  t.r3.u64 = block + kBody;
                  break;
                case kBodyVector:
                  t.r3.u64 = block + kBody;
                  t.r4.u64 = block + layout.vector;
                  break;
                case kBodyVectorPoint:
                  t.r3.u64 = block + kBody;
                  t.r4.u64 = block + layout.vector;
                  t.r5.u64 = block + layout.point;
                  break;
                case kOutBodyPoint:
                  t.r3.u64 = block + layout.out;
                  t.r4.u64 = block + kBody;
                  t.r5.u64 = block + layout.point;
                  break;
              }
            },
            f.recompiled, f.native,
            fmt::format("{} seed {} f1 {:016X}", layout.name, seed, std::bit_cast<uint64_t>(f1)),
            f.compare_f1);
      }
    }
    results.Report();
    if (f.arithmetic) {
      nan_results.Report();
    }
  }
}

// The wheels' slip: a vehicle, its body (+336) and its wheels' data
// (+2160 + 4 x wheel) in the block; wheels 0-4, on the ground or not. r3
// isn't compared: the originals leave it a stack address where the wheel is
// on the ground (callers read only f1).
constexpr uint32_t kSlipVehicle = 0x2000, kSlipBody = 0x4000, kSlipWheels = 0x4400;

void WheelSlipCases(Context& h, uint32_t block) {
  Random& r = h.random;
  auto setup = [&](uint64_t seed, bool nans) {
    return [&h, block, seed, nans] {
      Random fill = Seeded(seed * 3 + 2);
      for (uint32_t i = 0; i < 0x2600; i += 4) {
        guest::StoreU32(h.base, block + kSlipVehicle + i,
                        nans ? FloatWord(fill) : NumberWord(fill));
      }
      guest::StoreU32(h.base, block + kSlipVehicle + 336, block + kSlipBody);
      for (uint32_t wheel = 0; wheel < 5; wheel++) {
        guest::StoreU32(h.base, block + kSlipVehicle + 2160 + 4 * wheel,
                        block + kSlipWheels + 64 * wheel);
        guest::StoreU8(h.base, block + kSlipVehicle + 433 + 368 * wheel,
                       fill.Below(4) == 0 ? 0 : 1);
      }
    };
  };
  {
    Results results("Vehicle::WheelSlip (sub_820C0180)");
    Results nan_results("Vehicle::WheelSlip (sub_820C0180), NaN inputs (expected to differ)");
    for (uint64_t seed = 0; seed < 60; seed++) {
      const bool nans = seed % 4 == 3;
      for (uint32_t wheel = 0; wheel < 5; wheel++) {
        // Thresholds from 0 up, some small enough that slips count.
        const double f1 = seed % 2 ? double(std::bit_cast<float>(NumberWord(r)))
                                   : double(float(r.Below(100)) / 100.0f);
        Compare(h, nans ? nan_results : results, {}, setup(seed, nans),
                [&](PPCContext& t) {
                  t.r3.u64 = block + kSlipVehicle;
                  t.r4.u64 = wheel;
                  t.f1.f64 = f1;
                },
                __imp__sub_820C0180, NativeVehicleWheelSlip,
                fmt::format("seed {} wheel {} f1 {:016X}", seed, wheel,
                            std::bit_cast<uint64_t>(f1)),
                true, false);
      }
    }
    results.Report();
    nan_results.Report();
  }
  {
    Results results("Vehicle::MaxWheelSlip (sub_820C0290)");
    Results nan_results("Vehicle::MaxWheelSlip (sub_820C0290), NaN inputs (expected to differ)");
    for (uint64_t seed = 0; seed < 60; seed++) {
      const bool nans = seed % 4 == 3;
      const double f1 = seed % 2 ? double(std::bit_cast<float>(NumberWord(r)))
                                 : double(float(r.Below(100)) / 100.0f);
      Compare(h, nans ? nan_results : results, {}, setup(seed, nans),
              [&](PPCContext& t) {
                t.r3.u64 = block + kSlipVehicle;
                t.f1.f64 = f1;
              },
              __imp__sub_820C0290, NativeVehicleMaxWheelSlip,
              fmt::format("seed {} f1 {:016X}", seed, std::bit_cast<uint64_t>(f1)), true,
              false);
    }
    results.Report();
    nan_results.Report();
  }
}

// The other objects' cases: a region of the block (0x1800 bytes) filled
// anew for each case, the object at its start; `prepare(fill, region)`
// then sets fields (pointers, lists), `args(t, pick, region)` the
// registers from a generator seeded the same for both versions. The
// region, the collision globals (0x826C2D78, 0x826C2D7C) and r3 and f1 are
// compared; the globals and the collision world's pointer are put back.
constexpr uint32_t kRegion = 0x8000, kRegionBytes = 0x1800;
constexpr uint32_t kCollisionGlobals = 0x826C2D78, kCollisionWorld = 0x826C2D88;

struct ObjectFunction {
  const char* name;
  GuestFunction recompiled, native;
  std::function<void(Random&, uint32_t)> prepare;
  std::function<void(PPCContext&, Random&, uint32_t)> args;
  bool compare_f1 = true;
  // Whether it does arithmetic on two float inputs (NaN cases a group of
  // their own, expected to differ).
  bool arithmetic = false;
};

void ObjectCases(Context& h, uint32_t block, const ObjectFunction& f, int count) {
  const uint32_t region = block + kRegion;
  const uint32_t saved_globals[2] = {guest::LoadU32(h.base, kCollisionGlobals),
                                     guest::LoadU32(h.base, kCollisionGlobals + 4)};
  const uint32_t saved_world = guest::LoadU32(h.base, kCollisionWorld);
  Results results(f.name);
  Results nan_results(std::string(f.name) + ", NaN inputs (expected to differ)");
  for (int seed = 0; seed < count; seed++) {
    const bool nans = seed % 4 == 3;
    Compare(
        h, nans && f.arithmetic ? nan_results : results,
        {{region, kRegionBytes}, {kCollisionGlobals, 8}},
        [&] {
          Random fill = Seeded(uint64_t(seed) * 11 + 5);
          for (uint32_t i = 0; i < kRegionBytes; i += 4) {
            guest::StoreU32(h.base, region + i, nans ? FloatWord(fill) : NumberWord(fill));
          }
          guest::StoreU32(h.base, kCollisionGlobals, saved_globals[0]);
          guest::StoreU32(h.base, kCollisionGlobals + 4, saved_globals[1]);
          guest::StoreU32(h.base, kCollisionWorld, saved_world);
          if (f.prepare) {
            f.prepare(fill, region);
          }
        },
        [&](PPCContext& t) {
          Random pick = Seeded(uint64_t(seed) * 13 + 7);
          t.r3.u64 = region;
          t.r4.u64 = uint32_t(pick.Next());
          t.f1.f64 = double(std::bit_cast<float>(nans ? FloatWord(pick) : NumberWord(pick)));
          if (f.args) {
            f.args(t, pick, region);
          }
        },
        f.recompiled, f.native, fmt::format("seed {}", seed), f.compare_f1);
  }
  guest::StoreU32(h.base, kCollisionGlobals, saved_globals[0]);
  guest::StoreU32(h.base, kCollisionGlobals + 4, saved_globals[1]);
  guest::StoreU32(h.base, kCollisionWorld, saved_world);
  results.Report();
  if (f.arithmetic) {
    nan_results.Report();
  }
}

void OtherObjectCases(Context& h, uint32_t block) {
  // Puts the collision world in the region (+0x400), with a small capacity
  // and count.
  auto world = [&h](Random& fill, uint32_t region) {
    guest::StoreU32(h.base, kCollisionWorld, region + 0x400);
    guest::StoreU32(h.base, region + 0x400 + 44, fill.Below(5));
    guest::StoreU32(h.base, region + 0x400 + 52, fill.Below(6));
  };
  // A chain of contacts (80 bytes each from +0x800, linked by +68, ending
  // in 0 after up to six), its first at the object's +240 and the start
  // contact's +68.
  auto contacts = [&h](Random& fill, uint32_t region) {
    const uint32_t count = fill.Below(7);
    uint32_t next = 0;
    for (uint32_t i = count; i-- > 0;) {
      const uint32_t contact = region + 0x800 + 80 * i;
      guest::StoreU32(h.base, contact + 68, next);
      guest::StoreU32(h.base, contact + 4, uint32_t(fill.Next()) & (fill.Below(2) ? 0xFF : ~0u));
      next = contact;
    }
    guest::StoreU32(h.base, region + 240, next);
    guest::StoreU32(h.base, region + 68, next);
  };
  // A list for Link and Unlink: the head at +0x10, the object (the region)
  // and two others (+0x400, +0x600) linked in some way (+228 next, +232
  // previous).
  auto list = [&h](Random& fill, uint32_t region) {
    const uint32_t others[] = {0, region + 0x400, region + 0x600};
    guest::StoreU32(h.base, region + 0x10, fill.Below(3) == 0 ? 0 : others[1 + fill.Below(2)]);
    guest::StoreU32(h.base, region + 232, others[fill.Below(2)]);
    guest::StoreU32(h.base, region + 228, others[2 * fill.Below(2)]);
    for (uint32_t other : {others[1], others[2]}) {
      guest::StoreU32(h.base, other + 228, others[fill.Below(3)]);
      guest::StoreU32(h.base, other + 232, others[fill.Below(3)]);
    }
  };
  auto head = [](PPCContext& t, Random&, uint32_t region) { t.r4.u64 = region + 0x10; };
  // A vector or record elsewhere in the region, or overlapping the object.
  auto source = [](PPCContext& t, Random& pick, uint32_t region) {
    const uint32_t at[] = {0x200, 0x204, 0x210, 0xD0, 0x0, 0x8};
    t.r4.u64 = region + at[pick.Below(6)];
  };
  auto out_and_source = [](PPCContext& t, Random& pick, uint32_t region) {
    const uint32_t at[] = {0x200, 0x204, 0x210, 0x100, 0x0, 0x8};
    t.r3.u64 = region + 0x100;
    t.r4.u64 = region + at[pick.Below(6)];
  };
  auto mask = [](PPCContext& t, Random& pick, uint32_t) {
    t.r4.u64 = pick.Below(2) ? uint32_t(1) << pick.Below(32) : uint32_t(pick.Next());
  };
  const ObjectFunction functions[] = {
      {"Vehicle::Float2224 (sub_820C0140)", __imp__sub_820C0140, NativeVehicleFloat2224},
      {"Vehicle::Speed (sub_820C0148)", __imp__sub_820C0148, NativeVehicleSpeed},
      {"Vehicle::GetDistancePerFrame (sub_820C0150)", __imp__sub_820C0150,
       NativeVehicleDistancePerFrame},
      {"Vehicle::SetObjectHalf114 (sub_820C0450)", __imp__sub_820C0450,
       NativeVehicleSetObjectHalf114,
       [&h](Random& fill, uint32_t region) {
         guest::StoreU32(h.base, region + 344, fill.Below(3) == 0 ? 0 : region + 0x400);
       }},
      {"Quaternion::ToMatrix3 (sub_820C0490)", __imp__sub_820C0490, NativeQuaternionToMatrix3,
       nullptr, out_and_source, true, true},
      {"Quaternion::ToMatrix (sub_820C0550)", __imp__sub_820C0550, NativeQuaternionToMatrix,
       nullptr, out_and_source, true, true},
      {"Vehicle::SpeedRecord (sub_820C0590)", __imp__sub_820C0590, NativeVehicleSpeedRecord,
       nullptr,
       [](PPCContext& t, Random& pick, uint32_t region) {
         t.r3.u64 = region + 0x100 + 4 * pick.Below(4);
         t.r4.u64 = region;
       }},
      {"CollisionObject::Link (sub_820B7BF0)", __imp__sub_820B7BF0, NativeCollisionObjectLink,
       list, head},
      {"CollisionObject::Unlink (sub_820B7C18)", __imp__sub_820B7C18,
       NativeCollisionObjectUnlink, list, head},
      {"CollisionObject::FirstContact (sub_820B7C90)", __imp__sub_820B7C90,
       NativeCollisionObjectFirstContact, contacts, mask},
      {"CollisionObject::Word264 (sub_820B7CB8)", __imp__sub_820B7CB8,
       NativeCollisionObjectWord264},
      {"CollisionObject::SetWord264 (sub_820B7CC0)", __imp__sub_820B7CC0,
       NativeCollisionObjectSetWord264},
      {"CollisionObject::ClearBits252 (sub_820B7D18)", __imp__sub_820B7D18,
       NativeCollisionObjectClearBits252},
      {"CollisionObject::SetBits256 (sub_820B7D28)", __imp__sub_820B7D28,
       NativeCollisionObjectSetBits256},
      {"CollisionObject::ClearBits256 (sub_820B7D38)", __imp__sub_820B7D38,
       NativeCollisionObjectClearBits256},
      {"CollisionObject::SetBits260 (sub_820B7D48)", __imp__sub_820B7D48,
       NativeCollisionObjectSetBits260},
      {"CollisionObject::ClearBits260 (sub_820B7D58)", __imp__sub_820B7D58,
       NativeCollisionObjectClearBits260},
      {"CollisionObject::SetSize (sub_820B7D68)", __imp__sub_820B7D68,
       NativeCollisionObjectSetSize, nullptr, source},
      {"CollisionObject::SetFloat224 (sub_820B7D70)", __imp__sub_820B7D70,
       NativeCollisionObjectSetFloat224},
      {"CollisionRecord::Copy (sub_820B7D78)", __imp__sub_820B7D78, NativeCollisionRecordCopy,
       nullptr, out_and_source},
      {"Collision::SetGlobal78 (sub_820B8768)", __imp__sub_820B8768,
       NativeCollisionSetGlobal78},
      {"Collision::SetGlobal7C (sub_820B8778)", __imp__sub_820B8778,
       NativeCollisionSetGlobal7C},
      {"CollisionWorld::SetCallback (sub_820B8788)", __imp__sub_820B8788,
       NativeCollisionWorldSetCallback, world},
      {"CollisionWorld::NewContact (sub_820B8A48)", __imp__sub_820B8A48,
       NativeCollisionWorldNewContact, world},
      {"CollisionWorld::Word60 (sub_820B8C90)", __imp__sub_820B8C90, NativeCollisionWorldWord60,
       world},
      {"CollisionWorld::Word64 (sub_820B8CA0)", __imp__sub_820B8CA0, NativeCollisionWorldWord64,
       world},
      {"CollisionWorld::Word68 (sub_820B8CB0)", __imp__sub_820B8CB0, NativeCollisionWorldWord68,
       world},
      {"Contact::OtherObject (sub_820B8CE0)", __imp__sub_820B8CE0, NativeContactOtherObject,
       [&h](Random& fill, uint32_t region) {
         if (fill.Below(2)) {
           guest::StoreU32(h.base, region + 4, uint32_t(fill.Next()) & 0x00FFFFFF);
         }
       }},
      {"Contact::Next (sub_820B8D10)", __imp__sub_820B8D10, NativeContactNext, contacts, mask},
      {"Object344::SetFloat20 (sub_820B9658)", __imp__sub_820B9658, NativeObject344SetFloat20},
      {"Object344::SetFloat16 (sub_820B9660)", __imp__sub_820B9660, NativeObject344SetFloat16},
      {"Object344::Float28 (sub_820B9668)", __imp__sub_820B9668, NativeObject344Float28},
      {"Object344::SetFloat28 (sub_820B9670)", __imp__sub_820B9670, NativeObject344SetFloat28},
      {"Object344::SetFloat24 (sub_820B9678)", __imp__sub_820B9678, NativeObject344SetFloat24},
      {"Object::Half112 (sub_820B99B0)", __imp__sub_820B99B0, NativeObjectHalf112},
      {"Object::SetHalf112 (sub_820B99B8)", __imp__sub_820B99B8, NativeObjectSetHalf112},
      {"Object::Half114 (sub_820B99C0)", __imp__sub_820B99C0, NativeObjectHalf114},
      {"Object::SetHalf114 (sub_820B99C8)", __imp__sub_820B99C8, NativeObjectSetHalf114},
  };
  for (const ObjectFunction& f : functions) {
    ObjectCases(h, block, f, 60);
  }
}

// The race data's cases: the race data pointer (0x826C5B2C) pointed at a
// block of its own for the setters (put back after), and a stage list (10
// stages of 80 bytes, the count +800, the current +804) and racer records
// (a few of 3,936 bytes, the count at +0x49FE0) in the block.
constexpr uint32_t kRaceDataPointer = 0x826C5B2C;
constexpr uint32_t kStages = 0x10000;

void RaceDataMoreCases(Context& h, uint32_t block, uint32_t records_block) {
  Random& r = h.random;
  const uint32_t saved = guest::LoadU32(h.base, kRaceDataPointer);
  // The setters: r3 = a value (0 among them), into the block taken as the
  // race data (0x5A000 bytes, room for +0x5961C).
  {
    const struct {
      const char* name;
      GuestFunction recompiled, native;
    } setters[] = {
        {"RaceData::SetWord59618 (sub_820FF918)", __imp__sub_820FF918,
         NativeRaceDataSetWord59618},
        {"RaceData::SetWord5961C (sub_820FF938)", __imp__sub_820FF938,
         NativeRaceDataSetWord5961C},
        {"RaceData::SetWord32 (sub_820FF968)", __imp__sub_820FF968, NativeRaceDataSetWord32},
    };
    for (const auto& f : setters) {
      Results results(f.name);
      for (uint32_t value : Words(r, 10)) {
        Compare(h, results, {{records_block + 0x59610, 0x10}, {records_block, 0x40}},
                [&] {
                  guest::StoreU32(h.base, kRaceDataPointer, records_block);
                  Fill(h.base, records_block, 0x40);
                  Fill(h.base, records_block + 0x59610, 0x10);
                },
                [&](PPCContext& t) { t.r3.u64 = value; }, f.recompiled, f.native,
                fmt::format("value {:X}", value));
      }
      results.Report();
    }
    guest::StoreU32(h.base, kRaceDataPointer, saved);
  }

  // The stage list: types 0-5 and words, the count 0-9, the current 0-9.
  auto stages = [&h, block](uint64_t seed) {
    return [&h, block, seed] {
      Random fill = Seeded(seed * 17 + 9);
      FillRandom(h.base, fill, block + kStages, 812);
      for (uint32_t s = 0; s < 10; s++) {
        guest::StoreU32(h.base, block + kStages + 80 * s, fill.Below(6));
      }
      guest::StoreU32(h.base, block + kStages + 800, fill.Below(10));
      guest::StoreU32(h.base, block + kStages + 804, fill.Below(10));
    };
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    bool slot, out;
  } stage_functions[] = {
      {"Stages::HasNext (sub_820FFA30)", __imp__sub_820FFA30, NativeStagesHasNext},
      {"Stages::SetType (sub_820FFA50)", __imp__sub_820FFA50, NativeStagesSetType},
      {"Stages::SetWord808 (sub_820FFAB0)", __imp__sub_820FFAB0, NativeStagesSetWord808},
      {"Stages::Word808 (sub_820FFC68)", __imp__sub_820FFC68, NativeStagesWord808},
      {"Stages::Type (sub_820FFBE8)", __imp__sub_820FFBE8, NativeStagesType},
      {"Stages::Word4 (sub_820FFC08)", __imp__sub_820FFC08, NativeStagesWord4},
      {"Stages::IsType2Or3 (sub_820FFC30)", __imp__sub_820FFC30, NativeStagesIsType2Or3},
      {"Stages::Word76 (sub_820FFC70)", __imp__sub_820FFC70, NativeStagesWord76},
      {"Stages::Racer (sub_820FFC98)", __imp__sub_820FFC98, NativeStagesRacer, true},
      {"Stages::Word48 (sub_820FFCC8)", __imp__sub_820FFCC8, NativeStagesWord48, true},
      {"Stages::Copy24 (sub_820FFD48)", __imp__sub_820FFD48, NativeStagesCopy24, false, true},
      {"Stages::Copy (sub_820FFD88)", __imp__sub_820FFD88, NativeStagesCopy, false, true},
  };
  for (const auto& f : stage_functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 8; seed++) {
      for (uint32_t stage : {0xFFFFFFFFu, 0u, 3u, 9u}) {
        for (uint32_t value : {2u, 4u, 0xFFFFFFFFu, 3u}) {
          Compare(h, results, {{block + kStages, 0x400}}, stages(seed),
                  [&](PPCContext& t) {
                    if (f.out) {
                      t.r3.u64 = block + kStages + 0x340 + 4 * (seed % 3);
                      t.r4.u64 = block + kStages;
                      t.r5.u64 = stage;
                    } else {
                      t.r3.u64 = block + kStages;
                      t.r4.u64 = f.slot ? value & 3 : value;
                      t.r5.u64 = stage;
                      if (!f.slot) {
                        // r4 the stage for the getters, the type for SetType.
                        if (std::string(f.name).find("SetType") == std::string::npos &&
                            std::string(f.name).find("SetWord") == std::string::npos) {
                          t.r4.u64 = stage;
                        }
                      }
                    }
                  },
                  f.recompiled, f.native,
                  fmt::format("seed {} stage {} value {}", seed, int32_t(stage), int32_t(value)));
        }
      }
    }
    results.Report();
  }

  // Lists of 972-byte entries: the count 0-3.
  {
    Results results("Entries::Add (sub_820FFDD8)");
    for (uint64_t seed = 0; seed < 20; seed++) {
      Compare(h, results, {{block + kStages, 0x1000}},
              [&h, block, seed] {
                Random fill = Seeded(seed);
                FillRandom(h.base, fill, block + kStages, 0x1000);
                guest::StoreU32(h.base, block + kStages, fill.Below(4));
              },
              [&](PPCContext& t) {
                Random pick = Seeded(seed + 100);
                t.r3.u64 = block + kStages;
                t.r4.u64 = uint32_t(pick.Next());
                t.r5.u64 = uint32_t(pick.Next());
              },
              __imp__sub_820FFDD8, NativeEntriesAdd, fmt::format("seed {}", seed));
    }
    results.Report();
  }

  // Racer records: the count 0-3 (or -1, or 0) in the records block, a
  // record to add from the block.
  {
    auto records = [&h, records_block, block](uint64_t seed) {
      return [&h, records_block, block, seed] {
        Random fill = Seeded(seed * 3);
        FillRandom(h.base, fill, records_block, 4 * kRecordBytesHarness);
        FillRandom(h.base, fill, block + kStages, kRecordBytesHarness);
        const uint32_t counts[] = {0, 1, 2, 3, 0xFFFFFFFF};
        guest::StoreU32(h.base, records_block + 0x49FE0, counts[seed % 5]);
      };
    };
    Results add("RacerRecords::Add (sub_820FFE08)");
    Results at("RacerRecords::At (sub_820FFE58)");
    for (uint64_t seed = 0; seed < 10; seed++) {
      if (seed % 5 != 4) {
        Compare(h, add,
                {{records_block, 4 * kRecordBytesHarness}, {records_block + 0x49FE0, 4}},
                records(seed),
                [&](PPCContext& t) {
                  t.r3.u64 = records_block;
                  t.r4.u64 = block + kStages;
                },
                __imp__sub_820FFE08, NativeRacerRecordsAdd, fmt::format("seed {}", seed));
      }
      for (uint32_t n : {0xFFFFFFFFu, 0u, 1u, 76u, 0x80000000u}) {
        Compare(h, at, {}, records(seed),
                [&](PPCContext& t) {
                  t.r3.u64 = records_block;
                  t.r4.u64 = n;
                },
                __imp__sub_820FFE58, NativeRacerRecordsAt,
                fmt::format("seed {} n {}", seed, int32_t(n)));
      }
    }
    add.Report();
    at.Report();
  }

  // Pairs of halfwords: five pairs, some 0xFFFF firsts.
  {
    auto pairs = [&h, block](uint64_t seed) {
      return [&h, block, seed] {
        Random fill = Seeded(seed * 7);
        FillRandom(h.base, fill, block + kStages, 64);
        for (uint32_t i = 0; i < 5; i++) {
          if (fill.Below(3) == 0) {
            guest::StoreU16(h.base, block + kStages + 8 + 4 * i, 0xFFFF);
          }
        }
      };
    };
    const struct {
      const char* name;
      GuestFunction recompiled, native;
    } functions[] = {
        {"Pair::Copy (sub_820FFE98)", __imp__sub_820FFE98, NativePairCopy},
        {"Pairs::Last (sub_820FFEB0)", __imp__sub_820FFEB0, NativePairsLast},
        {"Pairs::At (sub_820FFF10)", __imp__sub_820FFF10, NativePairsAt},
    };
    for (const auto& f : functions) {
      Results results(f.name);
      for (uint64_t seed = 0; seed < 30; seed++) {
        for (uint32_t i : {0u, 2u, 4u, 5u, 0xFFFFFFFFu, 0x7FFFFFFFu}) {
          if (int32_t(i) < 0 && f.native == NativePairsAt) {
            continue;  // pair -1 would be read before the block
          }
          Compare(h, results, {{block + kStages, 64}}, pairs(seed),
                  [&](PPCContext& t) {
                    t.r3.u64 = block + kStages + 32 + 4 * (seed % 3);
                    t.r4.u64 = block + kStages + 8;
                    t.r5.u64 = i;
                  },
                  f.recompiled, f.native, fmt::format("seed {} i {}", seed, int32_t(i)));
        }
      }
      results.Report();
    }
  }
}

// The scene's pools: a scene in the block with lists of 0-5 objects (+16
// 0, 3 or another, +32 a kind 0-2) for pools 0-2 of the first set (+56 +
// 12 x pool) and the second (+4944 + 12 x pool), and pool 7 of the second
// (+5028); objects 48 bytes apart from +0x2000, each list's nodes linked by
// +8.
constexpr uint32_t kScene = 0x20000;

void ScenePoolCases(Context& h, uint32_t block) {
  const uint32_t scene = block + kScene;
  auto setup = [&h, scene](uint64_t seed) {
    return [&h, scene, seed] {
      Random fill = Seeded(seed * 19 + 4);
      FillRandom(h.base, fill, scene, 0x1400);
      uint32_t next_object = scene + 0x2000;
      auto make_list = [&](uint32_t list) {
        const uint32_t count = fill.Below(6);
        uint32_t first = 0, previous = 0;
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t object = next_object;
          next_object += 48;
          FillRandom(h.base, fill, object, 48);
          const uint32_t states[] = {0, 3, 3, 1};
          guest::StoreU32(h.base, object + 16, states[fill.Below(4)]);
          guest::StoreU32(h.base, object + 32, fill.Below(3));
          guest::StoreU32(h.base, object + 8, 0);
          if (previous) {
            guest::StoreU32(h.base, previous + 8, object);
          } else {
            first = object;
          }
          previous = object;
        }
        guest::StoreU32(h.base, list, first);
        // The count as given, or off by one (the claims trust it).
        guest::StoreU32(h.base, list + 8, count == 0 ? 0 : count - fill.Below(2));
      };
      for (uint32_t pool = 0; pool < 3; pool++) {
        make_list(scene + 56 + 12 * pool);
        make_list(scene + 4944 + 12 * pool);
      }
      make_list(scene + 5028);
    };
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    int kind;  // 0 list, 1 claim, 2 get, 3 find
    uint32_t list_offset;
  } functions[] = {
      {"List::Init (sub_82593FB0)", __imp__sub_82593FB0, NativeListInit, 0, 56},
      {"List::At (sub_82593FC0)", __imp__sub_82593FC0, NativeListAt, 0, 4944},
      {"ScenePools::ClaimA (sub_820C42C8)", __imp__sub_820C42C8, NativeScenePoolsClaimA, 1, 0},
      {"ScenePools::ClaimB (sub_820C5AB0)", __imp__sub_820C5AB0, NativeScenePoolsClaimB, 1, 0},
      {"ScenePools::GetA (sub_820C4420)", __imp__sub_820C4420, NativeScenePoolsGetA, 2, 0},
      {"ScenePools::GetB (sub_820C5C20)", __imp__sub_820C5C20, NativeScenePoolsGetB, 2, 0},
      {"ScenePools::FindModel (sub_820C5D70)", __imp__sub_820C5D70, NativeScenePoolsFindModel,
       3, 0},
  };
  for (const auto& f : functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 30; seed++) {
      for (uint32_t pool = 0; pool < 3; pool++) {
        for (uint32_t index : {0u, 1u, 2u, 4u, 7u, 0xFFFFFFFFu}) {
          if (f.kind == 2 || f.kind == 0) {
            // GetA and GetB read the object's +16 even for an empty list
            // (address 16): only lists with objects.
            const uint32_t list =
                f.kind == 0 ? scene + f.list_offset + 12 * pool
                : f.native == NativeScenePoolsGetA ? scene + 56 + 12 * pool
                                                   : scene + 4944 + 12 * pool;
            setup(seed)();
            if (f.kind == 2 && guest::LoadU32(h.base, list) == 0) {
              continue;
            }
          }
          if (f.kind == 1 || f.kind == 3) {
            if (index > 2 && index != 0xFFFFFFFFu) {
              continue;
            }
          }
          Compare(h, results, {{scene, 0x1400}, {scene + 0x2000, 48 * 40}}, setup(seed),
                  [&](PPCContext& t) {
                    switch (f.kind) {
                      case 0:
                        t.r3.u64 = scene + f.list_offset + 12 * pool;
                        t.r4.u64 = index;
                        break;
                      case 1:
                        t.r3.u64 = scene;
                        t.r4.u64 = pool;
                        break;
                      case 2:
                        t.r3.u64 = scene;
                        t.r4.u64 = pool;
                        t.r5.u64 = index;
                        break;
                      case 3:
                        t.r3.u64 = scene;
                        t.r4.u64 = pool;
                        t.r5.u64 = index;
                        break;
                    }
                  },
                  f.recompiled, f.native,
                  fmt::format("seed {} pool {} index {}", seed, pool, int32_t(index)));
        }
      }
    }
    results.Report();
  }
}

}  // namespace

void VehicleAccessors(Context& h) {
  auto* memory = rex::system::kernel_state()->memory();
  const uint32_t block = memory->SystemHeapAlloc(kBlockBytes);
  if (!block) {
    ITC_LOG_ERROR("harness: vehicle_accessors: no block from the system heap");
    return;
  }
  AddressCases(h);
  VehicleGetCases(h, block);
  ContactCases(h, block);
  RaceDataCases(h, block);
  VectorCases(h, block);
  RigidBodyCases(h, block);
  WheelSlipCases(h, block);
  OtherObjectCases(h, block);
  ScenePoolCases(h, block);
  // The racer records and the race data's setters need a block of their
  // own (the race data reaches +0x5961C).
  const uint32_t records = memory->SystemHeapAlloc(0x5A000);
  if (records) {
    RaceDataMoreCases(h, block, records);
    memory->SystemHeapFree(records);
  } else {
    ITC_LOG_ERROR("harness: vehicle_accessors: no second block from the system heap");
  }
  memory->SystemHeapFree(block);
}

}  // namespace harness
