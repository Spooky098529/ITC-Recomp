// The car physics (src/car_physics/) against the recompiled functions: a
// rigid body (and the vectors passed with it) in a block of the scratch
// area, filled anew for each case with float words, and the whole block,
// r3 and (where the original leaves a value there) f1 compared. The
// originals run with the native flags of their callees off (the vector
// library, the matrix math, the vehicle accessors, the C runtime's math and
// the car physics), so the whole chain is the recompiled code's. Three
// groups, as the matrix math's harness: numbers only (below 10^6 in size,
// denormals and signed zeros among them, so no infinities and from them no
// NaNs are made), those numbers with one NaN word put in the block or f1,
// and anything anywhere, NaNs and infinities too (expected to differ: where
// two NaNs meet in one operation, which comes out depends on the order the
// compiler gave the operands, in the translated code as in ours, and the
// translated code, built without -ffp-model=strict, may also flip a NaN's
// sign where LLVM rewrites a subtraction; the game's bodies hold no NaNs).

#include "harness.h"

#include "base/flags.h"
#include "car_physics/car_physics.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <cstring>
#include <iterator>
#include <string>

ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_vector_library);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_math);

namespace harness {

namespace {

namespace cp = car_physics;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }
#define ITC_NATIVE_STEP_VECTOR(name, body) ITC_NATIVE_STEP(name, body; ctx.fpscr.enableFlushMode())

ITC_NATIVE_STEP(NativeUpdateOrigin, cp::RigidBodyUpdateOrigin(base, ctx.r3.u32, ctx.r1.u32 - 176);
                ctx.r3.u64 = ctx.r3.u32 + 336)
ITC_NATIVE_STEP(NativeUpdateVelocity,
                ctx.r3.u64 = cp::RigidBodyUpdateVelocity(base, ctx.r3.u32, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeUpdateAngularVelocity,
                ctx.f1.f64 = cp::RigidBodyUpdateAngularVelocity(base, ctx.r3.u32,
                                                                ctx.r1.u32 - 144);
                ctx.r3.u64 = ctx.r3.u32 + 96)
ITC_NATIVE_STEP_VECTOR(NativeUpdateOrientation,
                       cp::RigidBodyUpdateOrientation(base, ctx.r3.u32, ctx.r1.u32 - 320);
                       ctx.r3.u64 = ctx.r3.u32 + 416)
ITC_NATIVE_STEP(NativeSetOrientation,
                cp::RigidBodySetOrientation(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96);
                ctx.r3.u64 = ctx.r3.u32 + 336)
ITC_NATIVE_STEP(NativeSetCenterOfMassPosition,
                cp::RigidBodySetCenterOfMassPosition(base, ctx.r3.u32, ctx.r4.u32,
                                                     ctx.r1.u32 - 96);
                ctx.r3.u64 = ctx.r3.u32 + 336)
ITC_NATIVE_STEP(NativeSetPosition,
                cp::RigidBodySetPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192);
                ctx.r3.u64 = ctx.r3.u32 + 16)
ITC_NATIVE_STEP(NativeSetVelocity,
                ctx.r3.u64 = cp::RigidBodySetVelocity(base, ctx.r3.u32, ctx.r4.u32,
                                                      ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeSetAngularVelocity,
                ctx.f1.f64 = cp::RigidBodySetAngularVelocity(base, ctx.r3.u32, ctx.r4.u32,
                                                             ctx.r1.u32 - 160);
                ctx.r3.u64 = ctx.r3.u32 + 96)
ITC_NATIVE_STEP_VECTOR(NativeSetInertia,
                       cp::RigidBodySetInertia(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 320);
                       ctx.r3.u64 = ctx.r3.u32 + 416)
ITC_NATIVE_STEP(NativeApplyImpulses,
                ctx.r3.u64 = cp::RigidBodyApplyImpulses(base, ctx.r3.u32, ctx.f1.f64,
                                                        ctx.r1.u32 - 144))
ITC_NATIVE_STEP(NativeIntegrate,
                ctx.r3.u64 = cp::RigidBodyIntegrate(base, ctx.r3.u32, ctx.f1.f64,
                                                    ctx.r1.u32 - 208))
ITC_NATIVE_STEP(NativeReset, cp::RigidBodyReset(base, ctx.r3.u32, ctx.r1.u32 - 176);
                ctx.r3.u64 = ctx.r3.u32 + 96)
ITC_NATIVE_STEP(NativeConstruct, cp::RigidBodyConstruct(base, ctx.r3.u32, ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeEffectiveMassAt,
                ctx.f1.f64 = cp::RigidBodyEffectiveMassAt(base, ctx.r3.u32, ctx.r4.u32,
                                                          ctx.r5.u32, ctx.r1.u32 - 272);
                ctx.r3.u64 = ctx.r1.u32 - 272 + 192)
ITC_NATIVE_STEP(NativeContactImpulse, uint32_t r3 = 0;
                ctx.f1.f64 = cp::RigidBodyContactImpulse(base, ctx.r3.u32, ctx.r4.u32,
                                                         ctx.r5.u32, ctx.r1.u32 - 208, &r3);
                ctx.r3.u64 = r3)
ITC_NATIVE_STEP(NativeFrictionImpulse,
                cp::RigidBodyFrictionImpulse(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                             ctx.f1.f64, ctx.f2.f64, ctx.r1.u32 - 192))
// With a fixed body, the original (as the adapter).
void NativeContactImpulseWith(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  uint32_t r3 = 0;
  if (!cp::RigidBodyContactImpulseWith(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                       ctx.r1.u32 - 272, &f1, &r3)) {
    __imp__sub_820B72C8(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
}

// The vehicle's physics step's helpers.
ITC_NATIVE_STEP(NativeBeginStep,
                ctx.r3.u64 = cp::VehicleBeginStep(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeWheelLoads, cp::VehicleWheelLoads(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceValue,
                ctx.f1.f64 = cp::VehicleSurfaceValue(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSurfaceValue20,
                ctx.f1.f64 = cp::VehicleSurfaceValue20(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSurfaceValue12,
                ctx.f1.f64 = cp::VehicleSurfaceValue12(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSurfaceValue16,
                ctx.f1.f64 = cp::VehicleSurfaceValue16(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeAccumulateTireForces,
                ctx.r3.u64 = cp::VehicleAccumulateTireForces(base, ctx.r3.u32,
                                                             ctx.r1.u32 - 192))
ITC_NATIVE_STEP(NativeRollingResistance, cp::VehicleRollingResistance(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeTurnWheels, cp::VehicleTurnWheels(base, ctx.r3.u32, ctx.r1.u32))
ITC_NATIVE_STEP(NativeBrakeTorques, cp::VehicleBrakeTorques(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeAntiLock, cp::VehicleAntiLock(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSuspensionTorques,
                ctx.r3.u64 = cp::VehicleSuspensionTorques(base, ctx.r3.u32, ctx.r1.u32 - 256))

ITC_NATIVE_STEP(NativeApplyForces,
                ctx.r3.u64 = cp::VehicleApplyForces(base, ctx.r3.u32, ctx.r1.u32 - 352))
ITC_NATIVE_STEP(NativeEngineTorque,
                ctx.f1.f64 = cp::VehicleEngineTorque(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32))
ITC_NATIVE_STEP(NativeMagicFormula,
                ctx.f1.f64 = cp::TireMagicFormula(base, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSetGearValues, cp::VehicleSetGearValues(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeAxleReactionTorque,
                ctx.f1.f64 = cp::VehicleAxleReactionTorque(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeUpdateFreeAxle, cp::VehicleUpdateFreeAxle(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSetBrake,
                ctx.f1.f64 = cp::VehicleSetBrake(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSetHandbrake,
                ctx.f1.f64 = cp::VehicleSetHandbrake(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSetThrottle,
                ctx.f1.f64 = cp::VehicleSetThrottle(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeRampReverseThrottle,
                cp::VehicleRampReverseThrottle(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeRampHandbrake,
                cp::VehicleRampHandbrake(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSetValue2388,
                cp::VehicleSetValue2388(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))

// The physics step's updates.
ITC_NATIVE_STEP(NativeCopyRows, cp::MatrixCopyRows(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP_VECTOR(NativeMultiplyTranslation,
                       cp::MatrixMultiplyTranslation(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r1.u32 - 192))
ITC_NATIVE_STEP(NativeTractionControl, cp::VehicleTractionControl(base, ctx.r3.u32, ctx.r1.u32))
ITC_NATIVE_STEP(NativeUpdateTireGrip, cp::VehicleUpdateTireGrip(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateEngineTorque,
                cp::VehicleUpdateEngineTorque(base, ctx.r3.u32, ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeUpdateBrakeHeat, cp::VehicleUpdateBrakeHeat(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateBackfire, cp::VehicleUpdateBackfire(base, ctx.r3.u32, ctx.r1.u32))
ITC_NATIVE_STEP(NativeUpdateTireWear,
                cp::VehicleUpdateTireWear(base, ctx.r3.u32, ctx.r1.u32 - 112))
// Where tan would fail, or mode 2 in a forward gear, the original (as the
// adapters).
ITC_NATIVE_STEP(NativeUpdateSteering,
                if (!cp::VehicleUpdateSteering(base, ctx.r3.u32, ctx.r1.u32 - 160))
                    __imp__sub_820B18B8(ctx, base))
ITC_NATIVE_STEP(NativeUpdateTorqueSplit,
                if (!cp::VehicleUpdateTorqueSplit(base, ctx.r3.u32))
                    __imp__sub_820B2100(ctx, base))
ITC_NATIVE_STEP(NativeUpdateTurbo, cp::VehicleUpdateTurbo(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateTireForces,
                ctx.r3.u64 = cp::VehicleUpdateTireForces(base, ctx.r3.u32, ctx.r1.u32 - 272))
ITC_NATIVE_STEP(NativeUpdateAtRest,
                ctx.r3.u64 = cp::VehicleUpdateAtRest(base, ctx.r3.u32, ctx.r1.u32 - 192))
ITC_NATIVE_STEP(NativeUpdateAero,
                ctx.r3.u64 = cp::VehicleUpdateAero(base, ctx.r3.u32, ctx.r1.u32 - 240))
ITC_NATIVE_STEP(NativeUpdateWheelAxes,
                ctx.r3.u64 = cp::VehicleUpdateWheelAxes(base, ctx.r3.u32, ctx.r1.u32 - 256))

// The vehicle's state from its body, its copies.
ITC_NATIVE_STEP(NativeSyncFromBody,
                ctx.f1.f64 = cp::VehicleSyncFromBody(base, ctx.r3.u32, ctx.r1.u32 - 368);
                ctx.r3.u64 = ctx.r3.u32 + 2016)
ITC_NATIVE_STEP(NativeWheelCopy, cp::WheelCopy(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeWheelRecord2Copy, cp::WheelRecord2Copy(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeVehicleReset, double f1 = 0.0;
                cp::VehicleReset(base, ctx.r3.u32, ctx.r1.u32 - 240, &f1); ctx.f1.f64 = f1;
                ctx.r3.u64 = ctx.r3.u32 + 3312)
ITC_NATIVE_STEP(NativeSetUpGearing, cp::VehicleSetUpGearing(base, ctx.r3.u32, ctx.r1.u32 - 176))
ITC_NATIVE_STEP(NativeSetSteering,
                ctx.f1.f64 = cp::VehicleSetSteering(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSteerRecordFill,
                cp::SteerRecordFill(base, ctx.r3.u32, ctx.r7.u32, ctx.f1.f64, ctx.f2.f64,
                                    ctx.f3.f64))
void NativeUpdateSuspension(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  ctx.r3.u64 = cp::VehicleUpdateSuspension(base, ctx.r3.u32, ctx.r1.u32 - 144, &f1);
  ctx.f1.f64 = f1;
}
ITC_NATIVE_STEP(NativeUpdateWheelSuspension,
                ctx.f1.f64 = cp::VehicleUpdateWheelSuspension(base, ctx.r3.u32, ctx.r4.u32,
                                                              ctx.r1.u32 - 176))
ITC_NATIVE_STEP(NativeSuspensionSpring,
                ctx.f1.f64 = cp::VehicleSuspensionSpring(base, ctx.r3.u32, ctx.r4.u32,
                                                         ctx.f1.f64, ctx.r6.u32))
ITC_NATIVE_STEP(NativeUpdateDrivetrain,
                cp::VehicleUpdateDrivetrain(base, ctx.r3.u32, ctx.r1.u32 - 176))
ITC_NATIVE_STEP(NativeUpdateDrivenAxle,
                cp::VehicleUpdateDrivenAxle(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64,
                                            ctx.r1.u32))
ITC_NATIVE_STEP(NativeUpdateGearbox,
                cp::VehicleUpdateGearbox(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeShiftGears, cp::VehicleShiftGears(base, ctx.r3.u32, ctx.r1.u32 - 96))
// The collision world.
ITC_NATIVE_STEP(NativeSurfaceA, ctx.f1.f64 = cp::CollisionSurfaceA(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceB, ctx.f1.f64 = cp::CollisionSurfaceB(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceC, ctx.f1.f64 = cp::CollisionSurfaceC(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceD, ctx.f1.f64 = cp::CollisionSurfaceD(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceOfA, ctx.f1.f64 = cp::CollisionSurfaceOfA(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceOfB, ctx.f1.f64 = cp::CollisionSurfaceOfB(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceOfC, ctx.f1.f64 = cp::CollisionSurfaceOfC(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceOfD, ctx.f1.f64 = cp::CollisionSurfaceOfD(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSurfaceSetA, cp::CollisionSurfaceSetA(base, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSurfaceSetB, cp::CollisionSurfaceSetB(base, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSurfaceSetC, cp::CollisionSurfaceSetC(base, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSurfaceSetD, cp::CollisionSurfaceSetD(base, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSurfacesReset, cp::CollisionSurfacesReset(base))
ITC_NATIVE_STEP(NativeResetSurfaces, cp::CollisionWorldResetSurfaces(base);
                ctx.r3.u64 = 0x826C2E30)
ITC_NATIVE_STEP(NativeShapeInit, cp::CollisionShapeInit(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeObjectConstruct, cp::CollisionObjectConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeAccumulateForce,
                ctx.r3.u64 = cp::CollisionObjectAccumulateForce(base, ctx.r3.u32, ctx.r4.u32,
                                                                ctx.r5.u32, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeAddContact, ctx.r3.u64 = cp::CollisionObjectAddContact(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeRecordsMerge,
                ctx.r3.u64 = cp::CollisionRecordsMerge(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                       ctx.r6.u32, ctx.r1.u32 - 304))
// The car data and the vehicle's constructor.
ITC_NATIVE_STEP(NativeCarDataSetDefaults,
                ctx.r3.u64 = cp::CarDataSetDefaults(base, ctx.r3.u32, ctx.r1.u32 - 192))
ITC_NATIVE_STEP(NativeCarDataConstruct, cp::CarDataConstruct(base, ctx.r3.u32, ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeCarDataFillAxle,
                cp::CarDataFillAxle(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeCarDataConvertUnits,
                ctx.f1.f64 = cp::CarDataConvertUnits(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeVehicleConstruct, double f1 = 0.0;
                cp::VehicleConstruct(base, ctx.r3.u32, ctx.r1.u32 - 112, &f1); ctx.f1.f64 = f1)
// The world's pairs and its reset.
ITC_NATIVE_STEP(NativeHasPair,
                ctx.r3.u64 = cp::CollisionWorldHasPair(base, ctx.r3.u32, ctx.r4.u32) ? 1 : 0)
ITC_NATIVE_STEP(NativeLinkPair, ctx.r3.u64 = cp::CollisionWorldLinkPair(base, ctx.r3.u32,
                                                                        ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeAddPairs,
                ctx.r3.u64 = cp::CollisionWorldAddPairs(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeUnlinkPair, ctx.r3.u64 = cp::CollisionWorldUnlinkPair(base, ctx.r3.u32))
// r3 comes back as the slot (memcpy's).
ITC_NATIVE_STEP(NativeMovePair, cp::CollisionWorldMovePair(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r4.u32)
// Where the move's memcpy can't be native, the original (as the adapter).
void NativeRemovePair(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (!cp::CollisionWorldRemovePair(base, ctx.r3.u32, &r3)) {
    __imp__sub_820B8A88(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
ITC_NATIVE_STEP(NativeWorldReset, ctx.r3.u64 = cp::CollisionWorldReset(base))
// Where a pair's removal can't be native, the original from the start (as
// the adapters).
void NativeRemovePairsOf(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (!cp::CollisionWorldRemovePairsOf(base, ctx.r3.u32, &r3)) {
    __imp__sub_820B8BA8(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeRemovePairsByGroups(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (!cp::CollisionWorldRemovePairsByGroups(base, ctx.r3.u32, ctx.r4.u32, &r3)) {
    __imp__sub_820B8AF8(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeRemoveObject(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  uint32_t r3 = 0;
  if (!cp::CollisionWorldRemoveObject(base, object, &r3)) {
    ctx.r3.u64 = object;
    ctx.r1.u64 = ctx.r1.u32 - 112;
    __imp__sub_820B8BA8(ctx, base);
    ctx.r1.u64 = ctx.r1.u32 + 112;
    return;
  }
  ctx.r3.u64 = r3;
}
ITC_NATIVE_STEP(NativeAddGroups,
                ctx.r3.u64 = cp::CollisionObjectAddGroups(base, ctx.r3.u32, ctx.r4.u32))
void NativeRemoveGroups(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  uint32_t groups = 0, r3 = 0;
  if (!cp::CollisionObjectRemoveGroups(base, object, ctx.r4.u32, &groups, &r3)) {
    ctx.r3.u64 = object;
    ctx.r4.u64 = groups;
    __imp__sub_820B8AF8(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
// Where tan would fail, the original (as the adapters).
void NativeSteerRecordCurve(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  if (!cp::SteerRecordCurve(base, ctx.r3.u32, ctx.f1.f64, &f1)) {
    __imp__sub_820B3E98(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
}
void NativeSteerTowards(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  if (!cp::VehicleSteerTowards(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.r4.u32,
                               ctx.r5.u32, &f1)) {
    __imp__sub_820B5FD8(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
}

// Runs the recompiled version with the native flags off, so the functions
// it calls that are ours run the originals too.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  bool* flags[] = {&ITC_FLAG(native_car_physics), &ITC_FLAG(native_matrix_math),
                   &ITC_FLAG(native_vector_library), &ITC_FLAG(native_vehicle_accessors),
                   &ITC_FLAG(native_math)};
  bool saved[std::size(flags)];
  for (size_t i = 0; i < std::size(flags); i++) {
    saved[i] = *flags[i];
    *flags[i] = false;
  }
  recompiled(ctx, base);
  for (size_t i = 0; i < std::size(flags); i++) {
    *flags[i] = saved[i];
  }
}

// NaNs of both kinds and signs, infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

// Float words: odd floats, random bits, tiny values (below 2^-60, whose
// products are denormal: the vector unit's flush-to-zero), and ordinary
// values (-64 to 64) so the arithmetic is mostly on numbers.
uint32_t FloatWord(Random& r) {
  switch (r.Below(7)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    case 2:
      return std::bit_cast<uint32_t>(std::ldexp(float(int32_t(r.Below(1 << 20)) - (1 << 19)), -80));
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}

// Numbers below 10^6 in size (denormals and signed zeros among them), so
// no infinity is made from them, and from infinities NaNs: two NaNs made so
// would meet (as in the third group).
uint32_t NumberWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (!(std::fabs(std::bit_cast<float>(word)) < 1e6f));
  return word;
}

Random Seeded(uint64_t seed) {
  Random r;
  for (uint64_t i = 0; i < seed; i++) {
    r.Next();
  }
  return r;
}

// The block: the body at +0 (528 bytes), the vector arguments at +0x300
// (a point or vector) and +0x340 (a normal), an output at +0x380, a second
// body at +0x400.
constexpr uint32_t kBlockBytes = 0x700;
constexpr uint32_t kArgument = 0x300, kNormal = 0x340, kOut = 0x380, kBodyB = 0x400;

// The registers each function takes.
enum Shape {
  kBodyR4,        // r3 = the body, r4 = a vector, f1
  kMassAt,        // r3 = the body, r4 = out, r5 = the point
  kImpulse,       // r3 = the body, r4 = the point, r5 = the normal
  kFriction,      // r3 = out, r4 = the body, r5 = the point, r6 = the normal, f1, f2
  kImpulseWith,   // r3 = body a, r4 = the point, r5 = the normal, r6 = body b
  kMatrix,        // r3 = out (+0x380), r4 = a matrix (+0), r5 = a vector
};

}  // namespace

void CarPhysicsVehicle(Context& h);
void CarPhysicsCollision(Context& h);
void CarPhysicsPairs(Context& h);
void CarPhysicsCarDataDerive(Context& h);
void CarPhysicsCarDataApplySettings(Context& h);
void CarPhysicsVehicleApplyTuning(Context& h);

void CarPhysics(Context& h) {
  // The block 16-byte aligned, as the game's bodies are.
  const uint32_t block = (h.data + 15) & ~15u;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    bool compare_f1;
    Shape shape = kBodyR4;
    // The impulses: f1 = +512 x vn, vn the negated dot product. One NaN put
    // in can reach both factors, once negated: where it meets itself so,
    // which comes out depends on the operand order (expected to differ).
    bool impulse = false;
  } functions[] = {
      {"RigidBody::UpdateOrigin (sub_820B6E70)", Original<__imp__sub_820B6E70>,
       NativeUpdateOrigin, false},
      {"RigidBody::UpdateVelocity (sub_820B70C8)", Original<__imp__sub_820B70C8>,
       NativeUpdateVelocity, false},
      {"RigidBody::UpdateAngularVelocity (sub_820B7478)", Original<__imp__sub_820B7478>,
       NativeUpdateAngularVelocity, true},
      {"RigidBody::UpdateOrientation (sub_820B6F38)", Original<__imp__sub_820B6F38>,
       NativeUpdateOrientation, false},
      {"RigidBody::SetOrientation (sub_820B71E8)", Original<__imp__sub_820B71E8>,
       NativeSetOrientation, false},
      {"RigidBody::SetCenterOfMassPosition (sub_820B71B0)", Original<__imp__sub_820B71B0>,
       NativeSetCenterOfMassPosition, false},
      {"RigidBody::SetPosition (sub_820B6710)", Original<__imp__sub_820B6710>, NativeSetPosition,
       false},
      {"RigidBody::SetVelocity (sub_820B7238)", Original<__imp__sub_820B7238>, NativeSetVelocity,
       false},
      {"RigidBody::SetAngularVelocity (sub_820B7500)", Original<__imp__sub_820B7500>,
       NativeSetAngularVelocity, true},
      {"RigidBody::SetInertia (sub_820B7280)", Original<__imp__sub_820B7280>, NativeSetInertia,
       false},
      {"RigidBody::ApplyImpulses (sub_820B75B8)", Original<__imp__sub_820B75B8>,
       NativeApplyImpulses, false},
      {"RigidBody::Integrate (sub_820B7658)", Original<__imp__sub_820B7658>, NativeIntegrate,
       false},
      {"RigidBody::Reset (sub_820B77D0)", Original<__imp__sub_820B77D0>, NativeReset, false},
      {"RigidBody::RigidBody (sub_820B7928)", Original<__imp__sub_820B7928>, NativeConstruct,
       false},
      {"RigidBody::EffectiveMassAt (sub_820B6A38)", Original<__imp__sub_820B6A38>,
       NativeEffectiveMassAt, true, kMassAt},
      {"RigidBody::ContactImpulse (sub_820B6C78)", Original<__imp__sub_820B6C78>,
       NativeContactImpulse, true, kImpulse, true},
      {"RigidBody::FrictionImpulse (sub_820B6D48)", Original<__imp__sub_820B6D48>,
       NativeFrictionImpulse, false, kFriction},
      {"RigidBody::ContactImpulseWith (sub_820B72C8)", Original<__imp__sub_820B72C8>,
       NativeContactImpulseWith, true, kImpulseWith, true},
      {"Matrix::CopyRows (sub_820AFB78)", Original<__imp__sub_820AFB78>, NativeCopyRows, false,
       kMatrix},
      {"Matrix::MultiplyTranslation (sub_820B0D68)", Original<__imp__sub_820B0D68>,
       NativeMultiplyTranslation, false, kMatrix},
  };
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN", ", NaNs and infinities anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      Results negated(std::string(f.name) +
                      ", one NaN meeting its own negation (expected to differ)");
      for (uint64_t seed = 0; seed < 200; seed++) {
        Random fill = Seeded(seed * 11 + 5 * uint64_t(group) + 1);
        auto word = [group](Random& r) {
          return group == kNaNs ? FloatWord(r) : NumberWord(r);
        };
        const double f1 = double(std::bit_cast<float>(word(fill)));
        const double f2 = double(std::bit_cast<float>(word(fill)));
        // The one NaN: in the block, or (one case in ten) in f1.
        const uint32_t nan_word = kNaNWords[fill.Below(std::size(kNaNWords))];
        const uint32_t nan_at = fill.Below(kBlockBytes / 4 + kBlockBytes / 40);
        // Shapes the body's fields take in the game, some of the time: not
        // fixed (byte +0 clear), still (the momentum +176 zero, the angular
        // momentum +400 zero), the caps (+48, +112) positive.
        const bool movable = fill.Below(4) != 0;
        const bool still = fill.Below(5) == 0;
        const bool not_spinning = fill.Below(5) == 0;
        const bool positive_caps = fill.Below(2) == 0;
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random words = Seeded(seed * 13 + 7 * uint64_t(group) + 3);
          for (uint32_t i = 0; i < kBlockBytes; i += 4) {
            guest::StoreU32(h.base, block + i, word(words));
          }
          if (movable) {
            guest::StoreU32(h.base, block, 0);
            guest::StoreU32(h.base, block + kBodyB, 0);
          }
          for (uint32_t i = 0; still && i < 16; i += 4) {
            guest::StoreU32(h.base, block + 176 + i, 0);
          }
          for (uint32_t i = 0; not_spinning && i < 16; i += 4) {
            guest::StoreU32(h.base, block + 400 + i, 0);
          }
          if (positive_caps) {
            for (uint32_t at : {48u, 112u}) {
              guest::StoreU32(h.base, block + at, guest::LoadU32(h.base, block + at) & 0x7FFFFFFF);
            }
          }
          if (group == kOneNaN && nan_at < kBlockBytes / 4) {
            guest::StoreU32(h.base, block + 4 * nan_at, nan_word);
          }
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          switch (f.shape) {
            case kBodyR4:
              t.r3.u64 = block;
              t.r4.u64 = block + kArgument;
              break;
            case kMassAt:
              t.r3.u64 = block;
              t.r4.u64 = block + kOut;
              t.r5.u64 = block + kArgument;
              break;
            case kImpulse:
              t.r3.u64 = block;
              t.r4.u64 = block + kArgument;
              t.r5.u64 = block + kNormal;
              break;
            case kFriction:
              t.r3.u64 = block + kOut;
              t.r4.u64 = block;
              t.r5.u64 = block + kArgument;
              t.r6.u64 = block + kNormal;
              break;
            case kMatrix:
              t.r3.u64 = block + kOut;
              t.r4.u64 = block;
              t.r5.u64 = block + kArgument;
              break;
            case kImpulseWith:
              t.r3.u64 = block;
              t.r4.u64 = block + kArgument;
              t.r5.u64 = block + kNormal;
              t.r6.u64 = block + kBodyB;
              break;
          }
          t.f1.f64 = group == kOneNaN && nan_at >= kBlockBytes / 4
                         ? double(std::bit_cast<float>(nan_word))
                         : f1;
          t.f2.f64 = f2;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v] = Read(h.base, block, kBlockBytes);
          for (int i = 0; i < 4; i++) {
            result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
          }
          for (int i = 0; f.compare_f1 && i < 8; i++) {
            result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
          }
        }
        // The impulse's two factors both NaN (+512 and f1, the
        // recompiled run's).
        auto nan_at_word = [&](size_t at) {
          uint32_t w = 0;
          for (int i = 0; i < 4; i++) {
            w = (w << 8) | result[0][at + i];
          }
          return std::isnan(std::bit_cast<float>(w));
        };
        const bool meets_negation =
            f.impulse && group == kOneNaN && nan_at_word(512) &&
            std::isnan(std::bit_cast<double>([&] {
              uint64_t w = 0;
              for (int i = 0; i < 8; i++) {
                w = (w << 8) | result[0][kBlockBytes + 4 + i];
              }
              return w;
            }()));
        (meets_negation ? negated : results).Check(result[0], result[1],
                      fmt::format("seed {} f1 {:016X}", seed, std::bit_cast<uint64_t>(f1)));
      }
      results.Report();
      if (f.impulse && group == kOneNaN) {
        negated.Report();
      }
    }
  }
  CarPhysicsVehicle(h);
  CarPhysicsCollision(h);
  CarPhysicsPairs(h);
  CarPhysicsCarDataDerive(h);
  CarPhysicsCarDataApplySettings(h);
  CarPhysicsVehicleApplyTuning(h);
}

namespace {

// A vehicle (6,272 bytes) is larger than the scratch area: its cases use
// guest stack further down (as the sprites' harness), with what it points
// to: the car data, the surface records and the rigid body.
constexpr uint32_t kVehicleArea = 0x10000;  // below the scratch area
constexpr uint32_t kVehicle = 0, kCarData = 0x2000, kSurfaces = 0x2800, kBody = 0x2900;
// The collision object (vehicle +340) and two contacts in its list.
constexpr uint32_t kObject = 0x2B80, kContacts = 0x2D00;
constexpr uint32_t kVehicleAreaBytes = 0x2E00;

// What r4 holds: a wheel (0-3), an axle's first wheel (0 or 2), a byte
// (any word), or the tire coefficients (four floats in the surface
// records' area). The copies: r3 = a wheel's record and r4 another's (the
// next wheel's, by the case's wheel), or r3 = the rear's wheel record 2
// (+2016) and r4 the front's (+1888).
// The steering record: r3 = the vehicle's +2988, r7 = the front axle data
// (car data +336).
enum R4Kind {
  kWheelR4,
  kAxleR4,
  kByteR4,
  kCoefficientsR4,
  kWheelCopyR4,
  kRecord2CopyR4,
  kSteerRecordR4,
  kCarDataR3,     // r3 = the car data
  kAxleSourceR4,  // r3 = the car data, r4 = a source record (vehicle +4000), r5 a byte
};

// Fills the area for one case: float words (by the group's generator),
// then the pointers and the bytes the functions branch on.
template <typename Word>
void FillVehicle(uint8_t* base, uint32_t area, Random& r, Word word) {
  for (uint32_t i = 0; i < kVehicleAreaBytes; i += 4) {
    guest::StoreU32(base, area + i, word(r));
  }
  const uint32_t v = area + kVehicle;
  guest::StoreU32(base, v + 272, area + kCarData);
  guest::StoreU32(base, v + 336, area + kBody);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = v + 416 + 368 * w;
    guest::StoreU32(base, v + 2144 + 4 * w, v + (w < 2 ? 1888 : 2016));
    guest::StoreU32(base, v + 2160 + 4 * w, area + kCarData + (w < 2 ? 336 : 608));
    guest::StoreU8(base, wheel + 17, uint8_t(r.Below(4) != 0));
    guest::StoreU32(base, wheel + 176, r.Below(8));
    guest::StoreU32(base, wheel + 204, r.Below(100));
  }
  for (uint32_t k = 0; k < 8; k++) {
    for (uint32_t table : {2668u, 2796u}) {
      guest::StoreU32(base, v + table + 4 * k,
                      r.Below(4) == 0 ? 0 : area + kSurfaces + 32 * r.Below(8));
    }
  }
  guest::StoreU32(base, v + 348, r.Below(5));
  // The collision object and its contacts (flags +4, the next +68).
  guest::StoreU32(base, v + 340, r.Below(5) == 0 ? 0 : area + kObject);
  guest::StoreU32(base, area + kObject + 240, r.Below(4) == 0 ? 0 : area + kContacts);
  for (uint32_t c = 0; c < 2; c++) {
    const uint32_t contact = area + kContacts + 128 * c;
    constexpr uint32_t kFlags[] = {0x01000000, 0x02000000, 0x04000000, 0x00000001};
    guest::StoreU32(base, contact + 4, kFlags[r.Below(4)]);
    guest::StoreU32(base, contact + 68, c == 0 && r.Below(2) == 0 ? contact + 128 : 0);
  }
  guest::StoreU8(base, v + 2384, uint8_t(r.Below(4) == 0));
  // The engine's shake: its phase near 1.0, its counter 0 to 5.
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2340, std::bit_cast<uint32_t>(float(r.Below(1200)) / 1000.0f));
  }
  guest::StoreU32(base, v + 2344, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(6));
  guest::StoreU8(base, v + 2664, uint8_t(r.Below(3) != 0));
  guest::StoreU8(base, v + 2386, uint8_t(r.Below(3) != 0));
  guest::StoreU32(base, v + 3156, r.Below(1000));
  guest::StoreU32(base, v + 2252, uint32_t(int32_t(r.Below(8)) - 1));  // the gear, -1 to 6
  // The physics step's updates: the traction control's counter, the next
  // gear, nitrous, the handbrake and the clutch, some of the time as the
  // game has them.
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2392, r.Below(40));
  }
  guest::StoreU32(base, v + 2256, uint32_t(int32_t(r.Below(8)) - 1));
  guest::StoreU8(base, v + 2424, uint8_t(r.Below(2)));
  // The backfire's switch and latch, the shift byte, the tire wear's mode
  // and switches.
  guest::StoreU8(base, v + 2512, uint8_t(r.Below(4) != 0));
  guest::StoreU8(base, v + 3240, uint8_t(r.Below(2)));
  guest::StoreU8(base, v + 2281, uint8_t(r.Below(2)));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2524, r.Below(4000));
  }
  guest::StoreU32(base, v + 2496, r.Below(4));
  guest::StoreU8(base, v + 2625, uint8_t(r.Below(2)));
  guest::StoreU8(base, v + 2383, uint8_t(r.Below(2)));
  // The steering's switch, the drive's mode, the turbo and its valve (car
  // data +1604, +1605).
  guest::StoreU8(base, v + 276, uint8_t(r.Below(2)));
  guest::StoreU32(base, v + 2376, r.Below(7));
  guest::StoreU8(base, area + kCarData + 1604, uint8_t(r.Below(3) != 0));
  guest::StoreU8(base, area + kCarData + 1605, uint8_t(r.Below(2)));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 2268, 0);
  }
  if (r.Below(2) == 0) {
    // The rpm, normalized (the engine's torque curve).
    guest::StoreU32(base, v + 2228, std::bit_cast<uint32_t>(float(r.Below(1100)) / 1000.0f));
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, v + 296, 0);
  }
  switch (r.Below(3)) {
    case 0:
      guest::StoreU32(base, v + 2264, 0x3F800000);
      break;
    case 1:
      guest::StoreU32(base, v + 2264, 0);
      break;
  }
  // The gear count (car data +1268), 1 to 7: SetUpGearing loops over it and
  // stores by it.
  guest::StoreU32(base, area + kCarData + 1268, 1 + r.Below(7));
  // The substeps (+2368), 1 to 10 (0 some of the time): the suspension
  // loops over them.
  guest::StoreU32(base, v + 2368, r.Below(8) == 0 ? 0 : 1 + r.Below(10));
  // The gearbox's bytes (the shift's direction +2276, +2277, the requests
  // +2284 to +2286, +2249) and the pedals at 0.0 some of the time.
  for (uint32_t at : {2276u, 2277u, 2284u, 2285u, 2286u, 2249u}) {
    guest::StoreU8(base, v + at, uint8_t(r.Below(3) == 0));
  }
  for (uint32_t at : {284u, 288u, 292u}) {
    if (r.Below(3) == 0) {
      guest::StoreU32(base, v + at, 0);
    }
  }
  // Vehicle::ApplyForces's bytes: in the air (+3132), tipped (+3241), at
  // rest (+2928).
  for (uint32_t at : {3132u, 3241u, 2928u}) {
    guest::StoreU8(base, v + at, uint8_t(r.Below(2)));
  }
}

}  // namespace

void CarPhysicsVehicle(Context& h) {
  const uint32_t area = (h.data - kVehicleArea) & ~0xFu;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    bool compare_f1;
    R4Kind r4 = kWheelR4;
    // f1 a value from 0.0 to 1.1 (the engine's normalized rpm).
    bool unit_f1 = false;
    // r3 compared (UpdateSteering's is what tan's _ctrlfp leaves, which the
    // game doesn't read: the physics step sets r3 again).
    bool compare_r3 = true;
    // One NaN put in can meet its own negation (a "... meeting its own
    // negation (expected to differ)" group, as RigidBody::ContactImpulse's).
    bool negation = false;
  } functions[] = {
      {"Vehicle::BeginStep (sub_820AFAE8)", Original<__imp__sub_820AFAE8>, NativeBeginStep,
       true},
      {"Vehicle::WheelLoads (sub_820AFD30)", Original<__imp__sub_820AFD30>, NativeWheelLoads,
       false},
      {"Vehicle::SurfaceValue (sub_820B0508)", Original<__imp__sub_820B0508>,
       NativeSurfaceValue, true},
      {"Vehicle::SurfaceValue20 (sub_820B0560)", Original<__imp__sub_820B0560>,
       NativeSurfaceValue20, true},
      {"Vehicle::SurfaceValue12 (sub_820B05B8)", Original<__imp__sub_820B05B8>,
       NativeSurfaceValue12, true},
      {"Vehicle::SurfaceValue16 (sub_820B0610)", Original<__imp__sub_820B0610>,
       NativeSurfaceValue16, true},
      {"Vehicle::AccumulateTireForces (sub_820B0908)", Original<__imp__sub_820B0908>,
       NativeAccumulateTireForces, false},
      {"Vehicle::RollingResistance (sub_820B2748)", Original<__imp__sub_820B2748>,
       NativeRollingResistance, false},
      {"Vehicle::TurnWheels (sub_820AFDA8)", Original<__imp__sub_820AFDA8>, NativeTurnWheels,
       false},
      {"Vehicle::BrakeTorques (sub_820B0838)", Original<__imp__sub_820B0838>,
       NativeBrakeTorques, false},
      {"Vehicle::AntiLock (sub_820B0718)", Original<__imp__sub_820B0718>, NativeAntiLock,
       false},
      {"Vehicle::SuspensionTorques (sub_820AFBD8)", Original<__imp__sub_820AFBD8>,
       NativeSuspensionTorques, false},
      {"Vehicle::EngineTorque (sub_820B00C8)", Original<__imp__sub_820B00C8>,
       NativeEngineTorque, true, kWheelR4, true},
      {"Tire::MagicFormula (sub_820B09C8)", Original<__imp__sub_820B09C8>, NativeMagicFormula,
       true, kCoefficientsR4},
      {"Vehicle::SetGearValues (sub_820B3C90)", Original<__imp__sub_820B3C90>,
       NativeSetGearValues, false},
      {"Vehicle::AxleReactionTorque (sub_820B3130)", Original<__imp__sub_820B3130>,
       NativeAxleReactionTorque, true, kAxleR4},
      {"Vehicle::UpdateFreeAxle (sub_820B3010)", Original<__imp__sub_820B3010>,
       NativeUpdateFreeAxle, false, kAxleR4},
      {"Vehicle::SetBrake (sub_820B3E08)", Original<__imp__sub_820B3E08>, NativeSetBrake, true},
      {"Vehicle::SetHandbrake (sub_820B3E38)", Original<__imp__sub_820B3E38>,
       NativeSetHandbrake, true},
      {"Vehicle::SetThrottle (sub_820B3E68)", Original<__imp__sub_820B3E68>, NativeSetThrottle,
       true},
      {"Vehicle::RampReverseThrottle (sub_820B0478)", Original<__imp__sub_820B0478>,
       NativeRampReverseThrottle, false, kByteR4},
      {"Vehicle::RampHandbrake (sub_820B04C0)", Original<__imp__sub_820B04C0>,
       NativeRampHandbrake, false, kByteR4},
      {"Vehicle::SetValue2388 (sub_820B3C20)", Original<__imp__sub_820B3C20>,
       NativeSetValue2388, true, kByteR4},
      {"Vehicle::TractionControl (sub_820B1790)", Original<__imp__sub_820B1790>,
       NativeTractionControl, false},
      {"Vehicle::UpdateTireGrip (sub_820B1FB8)", Original<__imp__sub_820B1FB8>,
       NativeUpdateTireGrip, false},
      {"Vehicle::UpdateEngineTorque (sub_820B22D8)", Original<__imp__sub_820B22D8>,
       NativeUpdateEngineTorque, false},
      {"Vehicle::UpdateBrakeHeat (sub_820B35C8)", Original<__imp__sub_820B35C8>,
       NativeUpdateBrakeHeat, false},
      {"Vehicle::UpdateBackfire (sub_820B3788)", Original<__imp__sub_820B3788>,
       NativeUpdateBackfire, false},
      {"Vehicle::UpdateTireWear (sub_820B39C8)", Original<__imp__sub_820B39C8>,
       NativeUpdateTireWear, false},
      {"Vehicle::UpdateSteering (sub_820B18B8)", Original<__imp__sub_820B18B8>,
       NativeUpdateSteering, false, kWheelR4, false, false},
      {"Vehicle::UpdateTorqueSplit (sub_820B2100)", Original<__imp__sub_820B2100>,
       NativeUpdateTorqueSplit, false},
      {"Vehicle::UpdateTurbo (sub_820B24A8)", Original<__imp__sub_820B24A8>, NativeUpdateTurbo,
       false},
      {"Vehicle::UpdateTireForces (sub_820B3F80)", Original<__imp__sub_820B3F80>,
       NativeUpdateTireForces, false},
      {"Vehicle::UpdateAtRest (sub_820B2808)", Original<__imp__sub_820B2808>, NativeUpdateAtRest,
       false},
      {"Vehicle::UpdateAero (sub_820B2A48)", Original<__imp__sub_820B2A48>, NativeUpdateAero,
       false},
      {"Vehicle::ApplyForces (sub_820B2CC0)", Original<__imp__sub_820B2CC0>, NativeApplyForces,
       false},
      {"Vehicle::UpdateWheelAxes (sub_820B1B00)", Original<__imp__sub_820B1B00>,
       NativeUpdateWheelAxes, false, kWheelR4, false, true, true},
      // A NaN in the body's position: translated by -p and then by p, the
      // two meet in the matrix +3312.
      {"Vehicle::SyncFromBody (sub_820B13C8)", Original<__imp__sub_820B13C8>,
       NativeSyncFromBody, true, kWheelR4, false, true, true},
      {"Wheel::Copy (sub_820B0A50)", Original<__imp__sub_820B0A50>, NativeWheelCopy, false,
       kWheelCopyR4},
      {"WheelRecord2::Copy (sub_820B0CC0)", Original<__imp__sub_820B0CC0>,
       NativeWheelRecord2Copy, false, kRecord2CopyR4},
      {"Vehicle::Reset (sub_820B0EE8)", Original<__imp__sub_820B0EE8>, NativeVehicleReset, true},
      {"Vehicle::SetUpGearing (sub_820B4AA8)", Original<__imp__sub_820B4AA8>,
       NativeSetUpGearing, false},
      {"Vehicle::SetSteering (sub_820B3D10)", Original<__imp__sub_820B3D10>, NativeSetSteering,
       true},
      {"SteerRecord::Fill (sub_820B0668)", Original<__imp__sub_820B0668>, NativeSteerRecordFill,
       false, kSteerRecordR4},
      // r3 is what tan's _ctrlfp leaves (as UpdateSteering's).
      {"SteerRecord::Curve (sub_820B3E98)", Original<__imp__sub_820B3E98>,
       NativeSteerRecordCurve, true, kSteerRecordR4, false, false},
      {"Vehicle::SteerTowards (sub_820B5FD8)", Original<__imp__sub_820B5FD8>, NativeSteerTowards,
       true, kByteR4, false, false},
      {"Vehicle::UpdateSuspension (sub_820B6328)", Original<__imp__sub_820B6328>,
       NativeUpdateSuspension, true},
      {"Vehicle::UpdateWheelSuspension (sub_820B5588)", Original<__imp__sub_820B5588>,
       NativeUpdateWheelSuspension, true},
      // r6 = a velocity in the vehicle (+5000).
      {"Vehicle::SuspensionSpring (sub_820B0198)", Original<__imp__sub_820B0198>,
       NativeSuspensionSpring, true},
      {"Vehicle::UpdateDrivetrain (sub_820B4778)", Original<__imp__sub_820B4778>,
       NativeUpdateDrivetrain, false},
      {"Vehicle::UpdateDrivenAxle (sub_820B31D0)", Original<__imp__sub_820B31D0>,
       NativeUpdateDrivenAxle, false, kAxleR4},
      {"Vehicle::UpdateGearbox (sub_820B4CF0)", Original<__imp__sub_820B4CF0>,
       NativeUpdateGearbox, false, kByteR4},
      {"Vehicle::ShiftGears (sub_820B6430)", Original<__imp__sub_820B6430>, NativeShiftGears,
       false},
      {"CarData::SetDefaults (sub_820AECB8)", Original<__imp__sub_820AECB8>,
       NativeCarDataSetDefaults, false, kCarDataR3},
      {"CarData::CarData (sub_820AEF80)", Original<__imp__sub_820AEF80>, NativeCarDataConstruct,
       false, kCarDataR3},
      {"CarData::FillAxle (sub_820AEE98)", Original<__imp__sub_820AEE98>, NativeCarDataFillAxle,
       false, kAxleSourceR4},
      {"CarData::ConvertUnits (sub_820AF018)", Original<__imp__sub_820AF018>,
       NativeCarDataConvertUnits, true, kCarDataR3},
      {"Vehicle::Vehicle (sub_820B42C8)", Original<__imp__sub_820B42C8>, NativeVehicleConstruct,
       true},
  };
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN",
                                     ", NaNs and infinities anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      Results negated(std::string(f.name) +
                      ", one NaN meeting its own negation (expected to differ)");
      for (uint64_t seed = 0; seed < 100; seed++) {
        Random pick = Seeded(seed * 17 + 3 * uint64_t(group) + 2);
        auto word = [group](Random& r) { return group == kNaNs ? FloatWord(r) : NumberWord(r); };
        double f1 = double(std::bit_cast<float>(word(pick)));
        const uint32_t wheel = pick.Below(4);
        const uint32_t byte_word = pick.Below(3) == 0 ? uint32_t(pick.Next()) : pick.Below(2);
        if (f.unit_f1 && pick.Below(4) != 0) {
          f1 = double(float(pick.Below(1100)) / 1000.0f);
        }
        uint32_t r3 = area + kVehicle, r4 = wheel;
        switch (f.r4) {
          case kWheelCopyR4:
            r3 = area + kVehicle + 416 + 368 * ((wheel + 1) & 3);
            r4 = area + kVehicle + 416 + 368 * wheel;
            break;
          case kRecord2CopyR4:
            r3 = area + kVehicle + 2016;
            r4 = area + kVehicle + 1888;
            break;
          case kSteerRecordR4:
            r3 = area + kVehicle + 2988;
            break;
          case kWheelR4:
            break;
          case kAxleR4:
            r4 = wheel & 2;
            break;
          case kByteR4:
            r4 = byte_word;
            break;
          case kCoefficientsR4:
            r4 = area + kSurfaces + 16 * wheel;
            break;
          case kCarDataR3:
            r3 = area + kCarData;
            break;
          case kAxleSourceR4:
            r3 = area + kCarData;
            r4 = area + kVehicle + 4000;
            break;
        }
        const uint32_t nan_word = kNaNWords[pick.Below(std::size(kNaNWords))];
        const uint32_t nan_at = pick.Below(kVehicleAreaBytes / 4);
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random fill = Seeded(seed * 19 + 7 * uint64_t(group) + 5);
          FillVehicle(h.base, area, fill, word);
          if (group == kOneNaN) {
            // Over a float word only: not a pointer (above 10^6 as a float)
            // or a small integer (a denormal or zero: the counts, indices
            // and bytes the functions branch on).
            const uint32_t at = area + 4 * nan_at;
            const uint32_t old = guest::LoadU32(h.base, at);
            if ((old & 0x7F800000) != 0 && std::fabs(std::bit_cast<float>(old)) < 1e6f) {
              guest::StoreU32(h.base, at, nan_word);
            }
          }
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = r3;
          t.r4.u64 = r4;
          // The other registers some take (SteerTowards' f2, f3 and r5,
          // SteerRecord::Fill's r7), from their own generator.
          Random more = Seeded(seed * 23 + 11 * uint64_t(group) + 7);
          t.f2.f64 = double(std::bit_cast<float>(word(more)));
          t.f3.f64 = double(std::bit_cast<float>(word(more)));
          t.r5.u64 = more.Below(3) == 0 ? uint32_t(more.Next()) : more.Below(2);
          t.r7.u64 = area + kCarData + 336;
          t.r6.u64 = area + kVehicle + 5000;
          t.f1.f64 = f1;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v] = Read(h.base, area, kVehicleAreaBytes);
          for (int i = 0; f.compare_r3 && i < 4; i++) {
            result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
          }
          for (int i = 0; f.compare_f1 && i < 8; i++) {
            result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
          }
        }
        // One NaN that spreads into two values, one of them negated, which
        // then meet: the results differ only in NaNs' signs (UpdateWheelAxes:
        // a NaN angle's sine, negated, and cosine summed).
        bool only_signs = f.negation && group == kOneNaN && result[0] != result[1] &&
                          result[0].size() == result[1].size();
        for (size_t i = 0; only_signs && i + 4 <= result[0].size(); i += 4) {
          uint32_t a = 0, b = 0;
          for (int k = 0; k < 4; k++) {
            a = (a << 8) | result[0][i + k];
            b = (b << 8) | result[1][i + k];
          }
          only_signs = a == b || ((a ^ b) == 0x80000000 && std::isnan(std::bit_cast<float>(a)));
        }
        (only_signs ? negated : results).Check(result[0], result[1],
                      fmt::format("seed {} r4 {:08X} f1 {:016X}", seed, r4,
                                  std::bit_cast<uint64_t>(f1)));
      }
      results.Report();
      if (f.negation && group == kOneNaN) {
        negated.Report();
      }
    }
  }
}

namespace {

// The collision world's cases: a region of guest stack further down (below
// the vehicle's), the surface settings (0x826C2DF0, 64 bytes) and the
// collision world's pointer (0x826C2D88) set for each case and put back
// after. Compared: the region, the surface settings, r3 and f1.
constexpr uint32_t kCollisionArea = 0x14000;
constexpr uint32_t kCollisionAreaBytes = 0x1000;
constexpr uint32_t kSurfaceGlobals = 0x826C2DF0, kSurfaceBytes = 64;
constexpr uint32_t kWorldPointer = 0x826C2D88;
// In the region: the object (320 bytes) or the records (+0x000), a point
// (+0x700), a force (+0x740), the body (+0x800), a fake world (+0xA00) and
// its contacts (+0xB00); the merge's summary (+0x300) and hits (+0x400).
constexpr uint32_t kPoint = 0x700, kForce = 0x740, kBodyAt = 0x800, kWorld = 0xA00,
                   kContactsAt = 0xB00, kSummary = 0x300, kHits = 0x400;

enum CollisionShape {
  kFlagsR3,     // r3 = flags (0x01000000, 0x02000000, 0x04000000 bits)
  kBitsR4,      // r4 = bits, f1 = a value
  kNoArgs,      // the surfaces' reset
  kObjectR3,    // r3 = the object (or shape) at the region
  kForceArgs,   // r3 = the object, r4 = a point, r5 = a force
  kContactR3,   // r3 = the object, a fake world
  kMergeArgs,   // r3 = out, r4 = the summary, r5 = the hits, r6 = a count
};

}  // namespace

void CarPhysicsCollision(Context& h) {
  const uint32_t area = (h.data - kCollisionArea) & ~0xFu;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    CollisionShape shape;
    bool compare_f1;
  } functions[] = {
      {"CollisionSurface::A (sub_820BC1F8)", Original<__imp__sub_820BC1F8>, NativeSurfaceA,
       kFlagsR3, true},
      {"CollisionSurface::B (sub_820BC240)", Original<__imp__sub_820BC240>, NativeSurfaceB,
       kFlagsR3, true},
      {"CollisionSurface::C (sub_820BC288)", Original<__imp__sub_820BC288>, NativeSurfaceC,
       kFlagsR3, true},
      {"CollisionSurface::D (sub_820BC2D0)", Original<__imp__sub_820BC2D0>, NativeSurfaceD,
       kFlagsR3, true},
      {"CollisionSurface::OfA (sub_820BC540)", Original<__imp__sub_820BC540>, NativeSurfaceOfA,
       kFlagsR3, true},
      {"CollisionSurface::OfB (sub_820BC548)", Original<__imp__sub_820BC548>, NativeSurfaceOfB,
       kFlagsR3, true},
      {"CollisionSurface::OfC (sub_820BC530)", Original<__imp__sub_820BC530>, NativeSurfaceOfC,
       kFlagsR3, true},
      {"CollisionSurface::OfD (sub_820BC538)", Original<__imp__sub_820BC538>, NativeSurfaceOfD,
       kFlagsR3, true},
      {"CollisionSurface::SetA (sub_820BC318)", Original<__imp__sub_820BC318>,
       NativeSurfaceSetA, kBitsR4, false},
      {"CollisionSurface::SetB (sub_820BC360)", Original<__imp__sub_820BC360>,
       NativeSurfaceSetB, kBitsR4, false},
      {"CollisionSurface::SetC (sub_820BC3A8)", Original<__imp__sub_820BC3A8>,
       NativeSurfaceSetC, kBitsR4, false},
      {"CollisionSurface::SetD (sub_820BC3F0)", Original<__imp__sub_820BC3F0>,
       NativeSurfaceSetD, kBitsR4, false},
      {"CollisionSurfaces::Reset (sub_820BC438)", Original<__imp__sub_820BC438>,
       NativeSurfacesReset, kNoArgs, false},
      {"CollisionWorld::ResetSurfaces (sub_820BC550)", Original<__imp__sub_820BC550>,
       NativeResetSurfaces, kNoArgs, false},
      {"CollisionShape::Init (sub_820BAA78)", Original<__imp__sub_820BAA78>, NativeShapeInit,
       kObjectR3, false},
      {"CollisionObject::CollisionObject (sub_820B7AE8)", Original<__imp__sub_820B7AE8>,
       NativeObjectConstruct, kObjectR3, false},
      {"CollisionObject::AccumulateForce (sub_820B7DF8)", Original<__imp__sub_820B7DF8>,
       NativeAccumulateForce, kForceArgs, false},
      {"CollisionObject::AddContact (sub_820B7C50)", Original<__imp__sub_820B7C50>,
       NativeAddContact, kContactR3, false},
      {"CollisionRecords::Merge (sub_820B7ED0)", Original<__imp__sub_820B7ED0>,
       NativeRecordsMerge, kMergeArgs, false},
  };
  uint8_t saved_surfaces[kSurfaceBytes];
  std::memcpy(saved_surfaces, h.base + kSurfaceGlobals, kSurfaceBytes);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN",
                                     ", NaNs and infinities anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  constexpr uint32_t kFlagBits[] = {0x01000000, 0x02000000, 0x04000000};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      for (uint64_t seed = 0; seed < 100; seed++) {
        Random pick = Seeded(seed * 29 + 3 * uint64_t(group) + 4);
        auto word = [group](Random& r) { return group == kNaNs ? FloatWord(r) : NumberWord(r); };
        const double f1 = double(std::bit_cast<float>(word(pick)));
        uint32_t bits = 0;
        for (uint32_t bit : kFlagBits) {
          bits |= pick.Below(2) == 0 ? bit : 0;
        }
        bits |= uint32_t(pick.Next()) & 0x00FFFFFF;
        const uint32_t count = pick.Below(9);
        const uint32_t nan_word = kNaNWords[pick.Below(std::size(kNaNWords))];
        const uint32_t nan_at = pick.Below((kCollisionAreaBytes + kSurfaceBytes) / 4);
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random fill = Seeded(seed * 31 + 7 * uint64_t(group) + 9);
          for (uint32_t i = 0; i < kCollisionAreaBytes; i += 4) {
            guest::StoreU32(h.base, area + i, word(fill));
          }
          for (uint32_t i = 0; i < kSurfaceBytes; i += 4) {
            guest::StoreU32(h.base, kSurfaceGlobals + i, word(fill));
          }
          // The object's body (+16), contacts' head (+240); the records'
          // ids (+60, 0 to 3 or -1) and flags (+56); the fake world.
          guest::StoreU32(h.base, area + 16, area + kBodyAt);
          guest::StoreU32(h.base, area + 240, fill.Below(3) == 0 ? 0 : area + kContactsAt);
          for (uint32_t r = 0; r < 8; r++) {
            const uint32_t hit = area + kHits + 80 * r;
            guest::StoreU32(h.base, hit + 60,
                            fill.Below(5) == 0 ? uint32_t(-1) : fill.Below(4));
            guest::StoreU32(h.base, hit + 56,
                            fill.Below(3) == 0 ? 0 : kFlagBits[fill.Below(3)]);
          }
          guest::StoreU32(h.base, area + kWorld + 44, fill.Below(5));
          guest::StoreU32(h.base, area + kWorld + 52, fill.Below(6));
          guest::StoreU32(h.base, area + kWorld + 48, area + kContactsAt);
          guest::StoreU32(h.base, kWorldPointer, area + kWorld);
          if (group == kOneNaN) {
            // Over a float word only (not a pointer, id, count or flags).
            const uint32_t at = nan_at < kCollisionAreaBytes / 4
                                    ? area + 4 * nan_at
                                    : kSurfaceGlobals + 4 * (nan_at - kCollisionAreaBytes / 4);
            const uint32_t old = guest::LoadU32(h.base, at);
            if ((old & 0x7F800000) != 0 && std::fabs(std::bit_cast<float>(old)) < 1e6f) {
              guest::StoreU32(h.base, at, nan_word);
            }
          }
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.f1.f64 = f1;
          switch (f.shape) {
            case kFlagsR3:
              t.r3.u64 = bits;
              break;
            case kBitsR4:
              t.r3.u64 = area;
              t.r4.u64 = bits;
              break;
            case kNoArgs:
            case kObjectR3:
            case kContactR3:
              t.r3.u64 = area;
              break;
            case kForceArgs:
              t.r3.u64 = area;
              t.r4.u64 = area + kPoint;
              t.r5.u64 = area + kForce;
              break;
            case kMergeArgs:
              t.r3.u64 = area;
              t.r4.u64 = area + kSummary;
              t.r5.u64 = area + kHits;
              t.r6.u64 = count;
              break;
          }
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v] = Read(h.base, area, kCollisionAreaBytes);
          const std::vector<uint8_t> surfaces = Read(h.base, kSurfaceGlobals, kSurfaceBytes);
          result[v].insert(result[v].end(), surfaces.begin(), surfaces.end());
          for (int i = 0; i < 4; i++) {
            result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
          }
          for (int i = 0; f.compare_f1 && i < 8; i++) {
            result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
          }
        }
        results.Check(result[0], result[1],
                      fmt::format("seed {} bits {:08X} f1 {:016X}", seed, bits,
                                  std::bit_cast<uint64_t>(f1)));
      }
      results.Report();
    }
  }
  std::memcpy(h.base + kSurfaceGlobals, saved_surfaces, kSurfaceBytes);
  guest::StoreU32(h.base, kWorldPointer, saved_world);
}


namespace {

// The collision world's pairs: a fake world in a region of guest stack
// further down (below the collision cases'), with 2 to 8 objects (some
// active, in a random order), pairs made by the recompiled AddPairs first
// (the same in both runs), then the function. The world's pointer and the
// surface settings are put back after. Compared: the region, the surface
// settings and r3. Integers only (the one float, +28, is 0.0 copied).
constexpr uint32_t kPairsArea = 0x18000;
constexpr uint32_t kPairsAreaBytes = 0x1000;
// The world at +0, the objects (320 bytes each) at +0x100, the pairs (32
// bytes each, up to 28) at +0xB00.
constexpr uint32_t kPairsObjects = 0x100, kPairs = 0xB00;

enum PairsShape {
  kHasPairArgs,     // r3, r4 = two objects
  kLinkPairArgs,    // r3 = the next pair, r4, r5 = two objects
  kAddPairsArgs,    // r3 = an object, r4 = groups
  kUnlinkPairArgs,  // r3 = a pair
  kMovePairArgs,    // r3 = the last pair, r4 = a slot unlinked first
  kRemovePairArgs,  // r3 = a pair's index
  kResetArgs,       // nothing
  kObjectGroups,    // r3 = an object, r4 = groups (or bits, some in the top byte)
};

}  // namespace

void CarPhysicsPairs(Context& h) {
  const uint32_t area = (h.data - kPairsArea) & ~0xFu;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    PairsShape shape;
  } functions[] = {
      {"CollisionWorld::HasPair (sub_820B8C20)", Original<__imp__sub_820B8C20>, NativeHasPair,
       kHasPairArgs},
      {"CollisionWorld::LinkPair (sub_820BC068)", Original<__imp__sub_820BC068>, NativeLinkPair,
       kLinkPairArgs},
      {"CollisionWorld::AddPairs (sub_820B94B0)", Original<__imp__sub_820B94B0>, NativeAddPairs,
       kAddPairsArgs},
      {"CollisionWorld::UnlinkPair (sub_820BBC40)", Original<__imp__sub_820BBC40>,
       NativeUnlinkPair, kUnlinkPairArgs},
      {"CollisionWorld::MovePair (sub_820BBD30)", Original<__imp__sub_820BBD30>, NativeMovePair,
       kMovePairArgs},
      {"CollisionWorld::RemovePair (sub_820B8A88)", Original<__imp__sub_820B8A88>,
       NativeRemovePair, kRemovePairArgs},
      {"CollisionWorld::Reset (sub_820B85E8)", Original<__imp__sub_820B85E8>, NativeWorldReset,
       kResetArgs},
      {"CollisionWorld::RemovePairsOf (sub_820B8BA8)", Original<__imp__sub_820B8BA8>,
       NativeRemovePairsOf, kObjectGroups},
      {"CollisionWorld::RemovePairsByGroups (sub_820B8AF8)", Original<__imp__sub_820B8AF8>,
       NativeRemovePairsByGroups, kObjectGroups},
      {"CollisionWorld::RemoveObject (sub_820B9448)", Original<__imp__sub_820B9448>,
       NativeRemoveObject, kObjectGroups},
      {"CollisionObject::AddGroups (sub_820B7CC8)", Original<__imp__sub_820B7CC8>,
       NativeAddGroups, kObjectGroups},
      {"CollisionObject::RemoveGroups (sub_820B7CF0)", Original<__imp__sub_820B7CF0>,
       NativeRemoveGroups, kObjectGroups},
  };
  uint8_t saved_surfaces[kSurfaceBytes];
  std::memcpy(saved_surfaces, h.base + kSurfaceGlobals, kSurfaceBytes);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  const uint32_t world = area;
  auto object_at = [area](uint32_t i) { return area + kPairsObjects + 320 * i; };
  auto pair_count = [&h, world] { return guest::LoadU32(h.base, world + 40); };
  for (const auto& f : functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 200; seed++) {
      std::vector<uint8_t> result[2];
      std::string what;
      for (int v = 0; v < 2; v++) {
        Random fill = Seeded(seed * 37 + 11);
        for (uint32_t i = 0; i < kPairsAreaBytes; i += 4) {
          guest::StoreU32(h.base, area + i, uint32_t(fill.Next()));
        }
        for (uint32_t i = 0; i < kSurfaceBytes; i += 4) {
          guest::StoreU32(h.base, kSurfaceGlobals + i, NumberWord(fill));
        }
        const uint32_t objects = 2 + fill.Below(7);
        guest::StoreU32(h.base, world + 12, objects);
        guest::StoreU32(h.base, world + 16, object_at(0));
        guest::StoreU32(h.base, world + 36, area + kPairs);
        guest::StoreU32(h.base, world + 40, 0);
        guest::StoreU32(h.base, kWorldPointer, world);
        // The active list: the objects in a random order, some left out.
        uint32_t order[8];
        for (uint32_t i = 0; i < objects; i++) {
          order[i] = i;
        }
        for (uint32_t i = objects - 1; i > 0; i--) {
          std::swap(order[i], order[fill.Below(i + 1)]);
        }
        // The active list (+20) and the free list (+24), linked by +228
        // (next) and +232 (previous); the groups the object is in (+244)
        // and those it collides with (+248), some in the top byte.
        uint32_t heads[2] = {0, 0};
        for (uint32_t i = 0; i < objects; i++) {
          const uint32_t object = object_at(order[i]);
          guest::StoreU32(h.base, object + 244, fill.Below(16));
          guest::StoreU32(h.base, object + 248,
                          fill.Below(16) | (fill.Below(2) == 0 ? 0x01000000 : 0));
          guest::StoreU32(h.base, object + 268, fill.Below(3));
          guest::StoreU32(h.base, object + 264, 0);
          uint32_t& list = heads[fill.Below(5) == 0 ? 1 : 0];
          guest::StoreU32(h.base, object + 228, list);
          guest::StoreU32(h.base, object + 232, 0);
          if (list != 0) {
            guest::StoreU32(h.base, list + 232, object);
          }
          list = object;
        }
        guest::StoreU32(h.base, world + 20, heads[0]);
        guest::StoreU32(h.base, world + 24, heads[1]);
        guest::StoreU32(h.base, world + 28, fill.Below(9));
        // The pairs made by the recompiled AddPairs.
        const uint32_t adds = fill.Below(5);
        for (uint32_t i = 0; i < adds; i++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = object_at(fill.Below(objects));
          t.r4.u64 = fill.Below(16);
          Original<__imp__sub_820B94B0>(t, h.base);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        const uint32_t a = object_at(fill.Below(objects));
        const uint32_t b = object_at(fill.Below(objects));
        const uint32_t count = pair_count();
        const uint32_t index = count == 0 ? 0 : fill.Below(count);
        bool skip = false;
        switch (f.shape) {
          case kHasPairArgs:
            t.r3.u64 = a;
            t.r4.u64 = b;
            break;
          case kLinkPairArgs:
            if (a == b) {
              skip = true;
            }
            t.r3.u64 = area + kPairs + 32 * count;
            t.r4.u64 = a;
            t.r5.u64 = b;
            break;
          case kAddPairsArgs:
            t.r3.u64 = a;
            t.r4.u64 = fill.Below(16);
            break;
          case kUnlinkPairArgs:
            t.r3.u64 = area + kPairs + 32 * index;
            skip = count == 0;
            break;
          case kMovePairArgs: {
            // Unlinked by the original first, as RemovePair does.
            skip = count < 2 || index == count - 1;
            if (!skip) {
              PPCContext u = CallContext(h);
              u.r3.u64 = area + kPairs + 32 * index;
              Original<__imp__sub_820BBC40>(u, h.base);
            }
            t.r3.u64 = area + kPairs + 32 * (count - 1);
            t.r4.u64 = area + kPairs + 32 * index;
            break;
          }
          case kRemovePairArgs:
            t.r3.u64 = index;
            skip = count == 0;
            break;
          case kResetArgs:
            break;
          case kObjectGroups:
            t.r3.u64 = a;
            t.r4.u64 = fill.Below(16) | (fill.Below(3) == 0 ? 0x01000000 : 0);
            break;
        }
        what = fmt::format("seed {} objects {} pairs {} a {:08X} b {:08X} index {}", seed,
                           objects, count, a, b, index);
        if (skip) {
          result[v].clear();
          continue;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        result[v] = Read(h.base, area, kPairsAreaBytes);
        const std::vector<uint8_t> surfaces = Read(h.base, kSurfaceGlobals, kSurfaceBytes);
        result[v].insert(result[v].end(), surfaces.begin(), surfaces.end());
        for (int i = 0; i < 4; i++) {
          result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
        }
      }
      if (!result[0].empty() || !result[1].empty()) {
        results.Check(result[0], result[1], what);
      }
    }
    results.Report();
  }
  std::memcpy(h.base + kSurfaceGlobals, saved_surfaces, kSurfaceBytes);
  guest::StoreU32(h.base, kWorldPointer, saved_world);
}


namespace {

// CarData::Derive's cases: a car data record (1,820 bytes) further down
// (below the pairs' region) filled with float words, the call's frame (the
// original's 1008 bytes, with its callees' below it) filled with random
// words. Three groups, as the others here: numbers only, one NaN word in
// the car data, and anything anywhere (NaNs and infinities too). Shapes the
// branches take, some of the time: an axle's radius (+388, +660) 0.0, the
// aero factors (+1800, +1804) 0.0, the cambers (+540, +812) 0.0, and the
// axles' x equal with no offset (+80's x 0.0), so the distances from the
// center and the wheelbase are 0.0. Compared (harness::Outputs): the car
// data as floats (it only writes floats with stfs, and the quaternions'
// bits; what it doesn't write is the same on both sides), the frame from
// +4 to the register saves (+888) as floats except the temporary body's
// integer words (+352: +0, +12, +516, exact), the caller's frame from r1
// to the scratch area, r3, r1 and f1.
constexpr uint32_t kDeriveArea = 0x1C000;
constexpr uint32_t kDeriveBytes = 0x720;
constexpr uint32_t kDeriveFrame = 1008, kDeriveSaves = 888;

ITC_NATIVE_STEP(NativeCarDataDerive,
                ctx.f1.f64 = cp::CarDataDerive(base, ctx.r3.u32, ctx.r1.u32 - 1008);
                ctx.r3.u64 = ctx.r1.u32 - 1008 + 352)

uint32_t SmallWord(Random& r) {
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 12)) - (1 << 11)) / 64.0f);
}

}  // namespace

void CarPhysicsCarDataDerive(Context& h) {
  const uint32_t data = (h.data - kDeriveArea) & ~0xFu;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t frame = r1 - kDeriveFrame;
  constexpr uint32_t kBelow = 1024;  // the callees' frames
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN", ", NaNs and infinities anywhere"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  for (Group group : {kNumbers, kOneNaN, kNaNs}) {
    Results results(std::string("CarData::Derive (sub_820AF1C0)") + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 200; seed++) {
      Random pick = Seeded(seed * 29 + 13 * uint64_t(group) + 9);
      auto word = [group](Random& r) { return group == kNaNs ? FloatWord(r) : NumberWord(r); };
      const bool zero_front_radius = pick.Below(4) == 0;
      const bool zero_rear_radius = pick.Below(4) == 0;
      const bool zero_aero = pick.Below(3) == 0;
      const bool zero_length = pick.Below(3) == 0;
      const bool zero_cambers = pick.Below(3) == 0;
      const bool same_x = pick.Below(4) == 0;
      const uint32_t x = SmallWord(pick);
      const uint32_t nan_word = kNaNWords[pick.Below(std::size(kNaNWords))];
      const uint32_t nan_at = pick.Below(kDeriveBytes / 4);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random fill = Seeded(seed * 31 + 17 * uint64_t(group) + 11);
        for (uint32_t i = 0; i < kDeriveBytes; i += 4) {
          guest::StoreU32(h.base, data + i, word(fill));
        }
        for (uint32_t at = frame - kBelow; at < h.data + kScratchBytes; at += 4) {
          guest::StoreU32(h.base, at, uint32_t(fill.Next()));
        }
        if (zero_front_radius) {
          guest::StoreU32(h.base, data + 388, 0);
        }
        if (zero_rear_radius) {
          guest::StoreU32(h.base, data + 660, 0);
        }
        if (zero_aero) {
          guest::StoreU32(h.base, data + 1800, 0);
        }
        if (zero_length) {
          guest::StoreU32(h.base, data + 1804, 0);
        }
        if (zero_cambers) {
          guest::StoreU32(h.base, data + 540, 0);
          guest::StoreU32(h.base, data + 812, 0);
        }
        if (same_x) {
          guest::StoreU32(h.base, data + 336, x);
          guest::StoreU32(h.base, data + 608, x);
          guest::StoreU32(h.base, data + 80, 0);
        }
        if (group == kOneNaN) {
          guest::StoreU32(h.base, data + 4 * nan_at, nan_word);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = data;
        (v == 0 ? GuestFunction(Original<__imp__sub_820AF1C0>) : NativeCarDataDerive)(t, h.base);
        out[v].Floats(h.base, data, kDeriveBytes / 4);
        out[v].Bytes(Read(h.base, frame + 4, 76));
        out[v].Floats(h.base, frame + 80, (352 - 80) / 4);
        const uint32_t body = frame + 352;
        out[v].Word(guest::LoadU32(h.base, body + 0));
        out[v].Floats(h.base, body + 4, 2);
        out[v].Word(guest::LoadU32(h.base, body + 12));
        out[v].Floats(h.base, body + 16, (516 - 16) / 4);
        out[v].Word(guest::LoadU32(h.base, body + 516));
        out[v].Floats(h.base, body + 520, 2);
        out[v].Bytes(Read(h.base, frame + 880, kDeriveSaves - 880));
        out[v].Bytes(Read(h.base, r1, h.data - r1));
        out[v].Word(t.r3.u32);
        out[v].Word(t.r1.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

namespace {

// sub_820C05C8's (CarData::ApplySettings, the name ours) cases: a car data
// record (1,820 bytes) where Derive's goes, a car object (1,792 bytes) and
// its spec's block 2 (320 bytes) above it, filled with float words; the
// call's frame (the original's 192 bytes, its callees' below it) filled with
// random words. Three groups, as Derive's: numbers only, one NaN word in the
// car data, and anything anywhere (NaNs and infinities too). The car's
// setting bytes (+72 to +94) random signed bytes, each at an extreme (-128,
// -1, 0, 1, 127) some of the time; the tuning +1756 above 0 or not, +1700
// 0, 1 or another, +1704 0, negative, or 1 to 5 (the table's index: 4 and 5
// read the frame's leftovers at +96 and +100, the same on both sides; past
// them the original's register saves); the car data's +172 0, 1 or another,
// +176 1 or not, setting byte 76 0 or not; a radius (+388, +660) 0.0 some of
// the time (asin of an infinity). Compared (harness::Outputs): the car data
// as floats (it only writes floats with stfs), the car object and the spec
// exactly (not written), the frame from +4 to the register saves (+104)
// exactly (pow's stfd of x and y at +16 and +24, the setting bytes' std and
// the table at +80), the caller's frame from r1 to the scratch area, r1 and
// f1. Not r3: the original leaves pow's there, which its only caller
// doesn't read, and the adapter leaves r3 as it was.
constexpr uint32_t kSettingsFrame = 192, kSettingsSaves = 104;
constexpr uint32_t kSettingsCar = 0x800, kSettingsCarBytes = 0x700;
constexpr uint32_t kSettingsSpec = 0x1000, kSettingsSpecBytes = 0x140;

ITC_NATIVE_STEP(NativeCarDataApplySettings,
                ctx.f1.f64 = cp::CarDataApplySettings(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                      ctx.r1.u32 - 192))

}  // namespace

void CarPhysicsCarDataApplySettings(Context& h) {
  const uint32_t data = (h.data - kDeriveArea) & ~0xFu;
  const uint32_t car = data + kSettingsCar, spec = data + kSettingsSpec;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t frame = r1 - kSettingsFrame;
  constexpr uint32_t kBelow = 1024;  // the callees' frames
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN", ", NaNs and infinities anywhere"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  constexpr int8_t kExtremes[] = {-128, -1, 0, 1, 127};
  for (Group group : {kNumbers, kOneNaN, kNaNs}) {
    Results results(std::string("CarData::ApplySettings (sub_820C05C8)") + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 300; seed++) {
      Random pick = Seeded(seed * 37 + 19 * uint64_t(group) + 5);
      auto word = [group](Random& r) { return group == kNaNs ? FloatWord(r) : NumberWord(r); };
      uint8_t settings[23];
      for (uint8_t& b : settings) {
        b = pick.Below(3) == 0 ? uint8_t(kExtremes[pick.Below(std::size(kExtremes))])
                               : uint8_t(pick.Next());
      }
      const uint32_t all_at = pick.Below(4);  // 1: every byte -128, 2: every byte 127
      const uint32_t tuning_1756 = pick.Below(2) ? 1 + pick.Below(4) : -pick.Below(3);
      const uint32_t picks[] = {0, 1, 2, uint32_t(pick.Next())};
      const uint32_t tuning_1700 = picks[pick.Below(4)];
      const uint32_t indices[] = {0, uint32_t(-1 - int32_t(pick.Below(4))), 1, 2, 3, 4, 5};
      const uint32_t tuning_1704 = indices[pick.Below(std::size(indices))];
      const uint32_t kinds[] = {0, 1, 2, uint32_t(pick.Next())};
      const uint32_t kind = kinds[pick.Below(4)];
      const uint32_t v176 = pick.Below(2) ? 1 : uint32_t(pick.Next());
      const bool byte76_zero = pick.Below(4) == 0;
      const bool zero_front_radius = pick.Below(6) == 0;
      const bool zero_rear_radius = pick.Below(6) == 0;
      const uint32_t nan_word = kNaNWords[pick.Below(std::size(kNaNWords))];
      const uint32_t nan_at = pick.Below(kDeriveBytes / 4);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random fill = Seeded(seed * 41 + 23 * uint64_t(group) + 7);
        for (uint32_t i = 0; i < kDeriveBytes; i += 4) {
          guest::StoreU32(h.base, data + i, word(fill));
        }
        for (uint32_t i = 0; i < kSettingsCarBytes; i += 4) {
          guest::StoreU32(h.base, car + i, word(fill));
        }
        for (uint32_t i = 0; i < kSettingsSpecBytes; i += 4) {
          guest::StoreU32(h.base, spec + i, word(fill));
        }
        for (uint32_t at = frame - kBelow; at < h.data + kScratchBytes; at += 4) {
          guest::StoreU32(h.base, at, uint32_t(fill.Next()));
        }
        for (uint32_t i = 0; i < std::size(settings); i++) {
          const uint8_t b = all_at == 1 ? 0x80 : all_at == 2 ? 0x7F : settings[i];
          guest::StoreU8(h.base, car + 72 + i, b);
        }
        if (byte76_zero) {
          guest::StoreU8(h.base, car + 92, 0);
        }
        guest::StoreU32(h.base, car + 1756, tuning_1756);
        guest::StoreU32(h.base, car + 1700, tuning_1700);
        guest::StoreU32(h.base, car + 1704, tuning_1704);
        guest::StoreU32(h.base, data + 172, kind);
        guest::StoreU32(h.base, data + 176, v176);
        if (zero_front_radius) {
          guest::StoreU32(h.base, data + 388, 0);
        }
        if (zero_rear_radius) {
          guest::StoreU32(h.base, data + 660, 0);
        }
        if (group == kOneNaN) {
          guest::StoreU32(h.base, data + 4 * nan_at, nan_word);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = data;
        t.r4.u64 = car;
        t.r5.u64 = spec;
        (v == 0 ? GuestFunction(Original<__imp__sub_820C05C8>) : NativeCarDataApplySettings)(
            t, h.base);
        out[v].Floats(h.base, data, kDeriveBytes / 4);
        out[v].Bytes(Read(h.base, car, kSettingsCarBytes));
        out[v].Bytes(Read(h.base, spec, kSettingsSpecBytes));
        out[v].Bytes(Read(h.base, frame + 4, kSettingsSaves - 4));
        out[v].Bytes(Read(h.base, r1, h.data - r1));
        out[v].Word(t.r1.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

namespace {

// sub_820C0D28's (Vehicle::ApplyTuning) cases: a car data record (1,856
// bytes compared, to +1832's word) where Derive's goes and a car object
// (4,096 bytes) above it, filled with float words; the call's frame (the
// original's 480 bytes, its callees' below it) filled with random words.
// Three groups, as Derive's: numbers only, one NaN word in the car data, and
// anything anywhere. The tuning levels (car +1648 on) 0, negative or
// positive, each up to the largest level whose table read stays below the
// original's register saves (frame +256: the levels reading the table at
// +208 up to 11, +1672 (its largest table +160) up to 23, +1676 (read
// unchecked at +96) from -23 to 39; past them the reads land on the saves,
// which the native version doesn't make), so out-of-range levels read the
// frame's leftovers as the original does; block 2's +68 (the cylinder
// code) 0 to 9, +69 (turbo) 0 or not, +131 (not a road car) 0 or not; the
// wheel numbers +1728, +1736 0, negative or positive, the inch-ups +1732,
// +1740 small; +1756 -1 some of the time, +1784 and +1800 0 or not; the
// copied aero record's flags (car +3864) every pair of bits; the torque
// curve (+1292) with a 1.0 and falling points some of the time. Compared
// (harness::Outputs): the car data as floats (it only writes floats with
// stfs), the car exactly (not written), the frame from +4 to the register
// saves (+256) exactly (the tables are the game's constants, the levels'
// std at +80, the words at +96 to +112, the copied record), the caller's
// frame from r1 to the scratch area, r1, r3 and f1.
constexpr uint32_t kTuningFrame = 480, kTuningSaves = 256;
constexpr uint32_t kTuningCar = 0x800, kTuningCarBytes = 0x1000;
constexpr uint32_t kTuningDataBytes = 0x740;

void NativeVehicleApplyTuning(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  ctx.r3.u64 = cp::VehicleApplyTuning(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 480, &f1);
  ctx.f1.f64 = f1;
}

}  // namespace

void CarPhysicsVehicleApplyTuning(Context& h) {
  const uint32_t data = (h.data - kDeriveArea) & ~0xFu;
  const uint32_t car = data + kTuningCar;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t frame = r1 - kTuningFrame;
  constexpr uint32_t kBelow = 1024;  // the callees' frames
  if (!cp::VehicleApplyTuningCopyFits(car, frame)) {
    Results skipped("Vehicle::ApplyTuning (sub_820C0D28): the copy doesn't fit");
    skipped.Check(std::vector<uint8_t>{0}, std::vector<uint8_t>{1}, "copy");
    skipped.Report();
    return;
  }
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN", ", NaNs and infinities anywhere"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  // The levels read as table indices (their offsets from car + 1648) and
  // the largest each may be.
  struct Level {
    uint32_t offset;
    int32_t lowest, highest;
  };
  constexpr Level kLevels[] = {{8, -3, 11},  {16, -3, 11}, {24, -3, 23}, {28, -23, 39},
                               {32, -3, 11}, {36, -3, 11}, {40, -3, 11}, {48, -3, 11},
                               {60, -3, 11}};
  for (Group group : {kNumbers, kOneNaN, kNaNs}) {
    Results results(std::string("Vehicle::ApplyTuning (sub_820C0D28)") + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 400; seed++) {
      Random pick = Seeded(seed * 43 + 29 * uint64_t(group) + 3);
      auto word = [group](Random& r) { return group == kNaNs ? FloatWord(r) : NumberWord(r); };
      int32_t levels[std::size(kLevels)];
      for (size_t i = 0; i < std::size(kLevels); i++) {
        const Level& l = kLevels[i];
        switch (pick.Below(4)) {
          case 0: levels[i] = 0; break;
          case 1: levels[i] = l.lowest + int32_t(pick.Below(uint32_t(-l.lowest))); break;
          case 2: levels[i] = 1 + int32_t(pick.Below(6)); break;  // in range
          default: levels[i] = 1 + int32_t(pick.Below(uint32_t(l.highest))); break;
        }
      }
      const uint32_t cylinders = pick.Below(10);
      const uint32_t turbo = pick.Below(2) ? 0 : 1 + pick.Below(255);
      const uint32_t not_road = pick.Below(3) == 0 ? 1 + pick.Below(255) : 0;
      auto signed_word = [&pick]() {
        switch (pick.Below(3)) {
          case 0: return 0u;
          case 1: return uint32_t(-1 - int32_t(pick.Below(4)));
          default: return 1 + pick.Below(8);
        }
      };
      const uint32_t front_wheel = signed_word(), rear_wheel = signed_word();
      const uint32_t front_inch = uint32_t(int32_t(pick.Below(9)) - 2);
      const uint32_t rear_inch = uint32_t(int32_t(pick.Below(9)) - 2);
      const uint32_t aero_77 = pick.Below(3) == 0 ? 0xFFFFFFFFu : pick.Below(4);
      const uint32_t flagged = pick.Below(2) ? 0 : 1 + pick.Below(4);
      const uint32_t level_152 = pick.Below(2) ? 0 : 1 + pick.Below(4);
      const uint32_t record_flags = pick.Below(4) | (pick.Below(4) == 0 ? (pick.Next() & ~3u) : 0);
      const uint32_t curve_mode = pick.Below(3);  // 0 random, 1 a peak and falling, 2 no 1.0
      const uint32_t peak = pick.Below(32);
      const uint32_t nan_word = kNaNWords[pick.Below(std::size(kNaNWords))];
      const uint32_t nan_at = pick.Below(kTuningDataBytes / 4);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random fill = Seeded(seed * 47 + 31 * uint64_t(group) + 13);
        for (uint32_t i = 0; i < kTuningDataBytes; i += 4) {
          guest::StoreU32(h.base, data + i, word(fill));
        }
        for (uint32_t i = 0; i < kTuningCarBytes; i += 4) {
          guest::StoreU32(h.base, car + i, word(fill));
        }
        for (uint32_t at = frame - kBelow; at < h.data + kScratchBytes; at += 4) {
          guest::StoreU32(h.base, at, uint32_t(fill.Next()));
        }
        if (curve_mode != 0) {
          // Points from 1.0 falling by random steps (0.9 to 1.0 apart, some
          // below 0.975 x the one before), with or without the 1.0.
          float point = curve_mode == 1 ? 1.0f : 0.99f;
          for (uint32_t i = 0; i < 32; i++) {
            const float value = i == peak && curve_mode == 1 ? 1.0f
                                : i < peak                   ? 0.5f + 0.01f * float(i)
                                                             : point;
            guest::StoreU32(h.base, data + 1292 + 4 * i, std::bit_cast<uint32_t>(value));
            if (i >= peak) {
              point *= 0.9f + 0.1f * float(fill.Below(100)) / 100.0f;
            }
          }
        }
        for (size_t i = 0; i < std::size(kLevels); i++) {
          guest::StoreU32(h.base, car + 1648 + kLevels[i].offset, uint32_t(levels[i]));
        }
        guest::StoreU8(h.base, car + 1888 + 68, uint8_t(cylinders));
        guest::StoreU8(h.base, car + 1888 + 69, uint8_t(turbo));
        guest::StoreU8(h.base, car + 1888 + 131, uint8_t(not_road));
        guest::StoreU32(h.base, car + 1728, front_wheel);
        guest::StoreU32(h.base, car + 1732, front_inch);
        guest::StoreU32(h.base, car + 1736, rear_wheel);
        guest::StoreU32(h.base, car + 1740, rear_inch);
        guest::StoreU32(h.base, car + 1756, aero_77);
        guest::StoreU32(h.base, car + 1784, flagged);
        guest::StoreU32(h.base, car + 1800, level_152);
        guest::StoreU32(h.base, car + 3636 + 204 + 24, record_flags);
        if (group == kOneNaN) {
          guest::StoreU32(h.base, data + 4 * nan_at, nan_word);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = data;
        t.r4.u64 = car;
        (v == 0 ? GuestFunction(Original<__imp__sub_820C0D28>) : NativeVehicleApplyTuning)(
            t, h.base);
        out[v].Floats(h.base, data, kTuningDataBytes / 4);
        out[v].Bytes(Read(h.base, car, kTuningCarBytes));
        out[v].Bytes(Read(h.base, frame + 4, kTuningSaves - 4));
        out[v].Bytes(Read(h.base, r1, h.data - r1));
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

}  // namespace harness
