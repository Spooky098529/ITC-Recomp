// The car physics natively: adapters in front of src/car_physics/
// (docs/functions/car-physics.md). Each passes the original's stack frame
// (its r1 after the stwu) for the temporaries, and leaves r3 (and f1 where
// the original leaves a value there) as the original does: the physics step
// relies on callees leaving r3 as it was (sub_820B6498's row).

#include "guest.h"

#include "base/flags.h"
#include "car_physics/car_physics.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_car_physics, true, "ITC",
              "Run the car physics (docs/functions/car-physics.md: the rigid body, sub_820B6A38 "
              "to sub_820B7928; the vehicle's physics step's helpers, updates and controls, "
              "sub_820AFAE8 to sub_820B6430; the vehicle's state from its body and its "
              "copies, sub_820B13C8, sub_820B0A50, sub_820B0CC0; its reset, gearing and "
              "steering, sub_820B0EE8, sub_820B4AA8, sub_820B3D10, sub_820B5FD8 with "
              "sub_820B0668 and sub_820B3E98; the suspension, sub_820B6328, "
              "sub_820B5588, sub_820B0198; the drivetrain, sub_820B4778, sub_820B31D0; the gearbox, "
              "sub_820B6430, sub_820B4CF0; the collision world's objects, contacts and "
              "surfaces, sub_820B7AE8 to sub_820B7ED0, sub_820BAA78, sub_820BC1F8 to "
              "sub_820BC550; its pairs and reset, sub_820B94B0, sub_820B8A88, sub_820B85E8 with "
              "sub_820B8C20, sub_820BC068, sub_820BBC40, sub_820BBD30, sub_820B9448 with "
              "sub_820B8BA8, sub_820B8AF8, and the collision objects' groups, sub_820B7CC8, "
              "sub_820B7CF0, and their translation, sub_820B7B60; the vehicle's forces applied, sub_820B2CC0; the car data and the "
              "vehicle's constructor, sub_820AEF80 with sub_820AECB8, sub_820AEE98, "
              "sub_820AF018, sub_820AF1C0, sub_820B42C8; the car's settings and tuning, sub_820C05C8, sub_820C0D28; the car loader, sub_820C27E8; the vehicle's physics objects, sub_8259D5A8, sub_820B8798 (no adapter; the ground callback only), sub_820B7998, sub_820B9570 with sub_820B7A00, sub_820B0E70 (these two only while --native_heap routes the game's heap to the allocation layer), the ground probe sub_820B4420, sub_820AFE90, sub_820B62E8) natively instead of through the translated code "
              "(the same words, bit for bit; checked by --run_harnesses=car_physics)");

// Defines an adapter: `body` uses `ctx` and `base`. Flush-to-zero off for
// the work; VECTOR leaves it on after (the original's last step is the
// vector unit's).
#define ITC_CAR_PHYSICS(address, body)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_car_physics)) { \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    ctx.fpscr.disableFlushMode();        \
    body;                                \
  }
#define ITC_CAR_PHYSICS_VECTOR(address, body) \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_car_physics)) {     \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    ctx.fpscr.disableFlushMode();            \
    body;                                    \
    ctx.fpscr.enableFlushMode();             \
  }

using namespace car_physics;

// --- The rigid body: r3 = the body ---

// RigidBody::UpdateOrigin, UpdateVelocity, UpdateAngularVelocity and
// UpdateOrientation (sub_820B6E70, sub_820B70C8, sub_820B7478,
// sub_820B6F38) have no adapters: their callers in play are native.

// r4 = the quaternion; r3 comes back as +336 (UpdateOrigin's).
ITC_CAR_PHYSICS(820B71E8,
                RigidBodySetOrientation(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96);
                ctx.r3.u64 = ctx.r3.u32 + 336)
// r4 = the point; r3 comes back as +336.
ITC_CAR_PHYSICS(820B71B0,
                RigidBodySetCenterOfMassPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96);
                ctx.r3.u64 = ctx.r3.u32 + 336)
// r4 = the position; r3 comes back as +16.
ITC_CAR_PHYSICS(820B6710, RigidBodySetPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192);
                ctx.r3.u64 = ctx.r3.u32 + 16)
// r4 = the velocity.
ITC_CAR_PHYSICS(820B7238, ctx.r3.u64 = RigidBodySetVelocity(base, ctx.r3.u32, ctx.r4.u32,
                                                            ctx.r1.u32 - 112))
// r4 = the angular velocity; r3 comes back as +96.
ITC_CAR_PHYSICS(820B7500,
                ctx.f1.f64 = RigidBodySetAngularVelocity(base, ctx.r3.u32, ctx.r4.u32,
                                                         ctx.r1.u32 - 160);
                ctx.r3.u64 = ctx.r3.u32 + 96)
// r4 = the inertia; UpdateOrientation is a tail call (its frame from this
// r1); r3 comes back as +416.
ITC_CAR_PHYSICS_VECTOR(820B7280,
                       RigidBodySetInertia(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 320);
                       ctx.r3.u64 = ctx.r3.u32 + 416)
// f1 = dt. r3 comes back as the body when it's fixed, else as
// UpdateVelocity leaves it.
ITC_CAR_PHYSICS(820B75B8, ctx.r3.u64 = RigidBodyApplyImpulses(base, ctx.r3.u32, ctx.f1.f64,
                                                              ctx.r1.u32 - 144))
// f1 = dt; r3 comes back as the body when it's fixed, else +336.
ITC_CAR_PHYSICS(820B7658, ctx.r3.u64 = RigidBodyIntegrate(base, ctx.r3.u32, ctx.f1.f64,
                                                          ctx.r1.u32 - 208))
// r3 comes back as +96 (SetAngularVelocity's).
ITC_CAR_PHYSICS(820B77D0, RigidBodyReset(base, ctx.r3.u32, ctx.r1.u32 - 176);
                ctx.r3.u64 = ctx.r3.u32 + 96)
// r3 comes back as the body.
ITC_CAR_PHYSICS(820B7928, RigidBodyConstruct(base, ctx.r3.u32, ctx.r1.u32 - 96))
// RigidBody::EffectiveMassAt (sub_820B6A38) has no adapter: its only
// caller, CarData::Derive, is native.
// r4 = the point, r5 = the normal; f1 = the impulse.
extern "C" REX_FUNC(sub_820B6C78) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B6C78(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  ctx.f1.f64 = RigidBodyContactImpulse(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                       ctx.r1.u32 - 208, &r3);
  ctx.r3.u64 = r3;
}
// r3 = out, r4 = the body, r5 = the point, r6 = the normal, f1 x f2 = the
// limit; r3 comes back as out.
ITC_CAR_PHYSICS(820B6D48, RigidBodyFrictionImpulse(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32, ctx.f1.f64, ctx.f2.f64,
                                                   ctx.r1.u32 - 192))
// r3 = body a, r4 = the point, r5 = the normal, r6 = body b; f1 = the
// impulse. With a fixed body (whose term the original reads from its
// stack's leftovers), the original.
extern "C" REX_FUNC(sub_820B72C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B72C8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  uint32_t r3 = 0;
  if (!RigidBodyContactImpulseWith(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                   ctx.r1.u32 - 272, &f1, &r3)) {
    __imp__sub_820B72C8(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
}

// --- The vehicle's physics step: r3 = the vehicle ---

// f1 = dt (left as it is).
ITC_CAR_PHYSICS(820AFAE8, ctx.r3.u64 = VehicleBeginStep(base, ctx.r3.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820AFD30, VehicleWheelLoads(base, ctx.r3.u32))
// Vehicle::SurfaceValue, SurfaceValue20, SurfaceValue12 and SurfaceValue16
// (sub_820B0508, sub_820B0560, sub_820B05B8, sub_820B0610) have no
// adapters: their callers in play are native.
ITC_CAR_PHYSICS(820B0908,
                ctx.r3.u64 = VehicleAccumulateTireForces(base, ctx.r3.u32, ctx.r1.u32 - 192))
ITC_CAR_PHYSICS(820B2748, VehicleRollingResistance(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820AFDA8, VehicleTurnWheels(base, ctx.r3.u32, ctx.r1.u32))
ITC_CAR_PHYSICS(820B0838, VehicleBrakeTorques(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820B0718, VehicleAntiLock(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820AFBD8,
                ctx.r3.u64 = VehicleSuspensionTorques(base, ctx.r3.u32, ctx.r1.u32 - 256))
// f1 = x; f1 = the torque. The original passes the curve's index through
// its r1 - 16.
ITC_CAR_PHYSICS(820B00C8,
                ctx.f1.f64 = VehicleEngineTorque(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32))
// Tire::MagicFormula (sub_820B09C8) has no adapter: its callers in play
// are native.
// Vehicle::SetGearValues, AxleReactionTorque and UpdateFreeAxle
// (sub_820B3C90, sub_820B3130, sub_820B3010) have no adapters: their
// callers in play are native.
// f1 = the value; f1 comes back clamped.
ITC_CAR_PHYSICS(820B3E08, ctx.f1.f64 = VehicleSetBrake(base, ctx.r3.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820B3E38, ctx.f1.f64 = VehicleSetHandbrake(base, ctx.r3.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820B3E68, ctx.f1.f64 = VehicleSetThrottle(base, ctx.r3.u32, ctx.f1.f64))
// r4 = the byte, f1 = time.
ITC_CAR_PHYSICS(820B0478,
                VehicleRampReverseThrottle(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820B04C0, VehicleRampHandbrake(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
// f1 = the value, r4 = the byte.
ITC_CAR_PHYSICS(820B3C20, VehicleSetValue2388(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))

// --- The physics step's updates ---

// r3 = out, r4 = the matrix; r3 comes back as out.
ITC_CAR_PHYSICS(820AFB78, MatrixCopyRows(base, ctx.r3.u32, ctx.r4.u32))
// r3 = out, r4 = the matrix, r5 = the vector; Matrix::Multiply leaves
// flush-to-zero on.
ITC_CAR_PHYSICS_VECTOR(820B0D68, MatrixMultiplyTranslation(base, ctx.r3.u32, ctx.r4.u32,
                                                           ctx.r5.u32, ctx.r1.u32 - 192))
// r3 = the vehicle, left as it is (the physics step relies on it).
ITC_CAR_PHYSICS(820B1790, VehicleTractionControl(base, ctx.r3.u32, ctx.r1.u32))
ITC_CAR_PHYSICS(820B1FB8, VehicleUpdateTireGrip(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820B22D8, VehicleUpdateEngineTorque(base, ctx.r3.u32, ctx.r1.u32 - 96))
ITC_CAR_PHYSICS(820B35C8, VehicleUpdateBrakeHeat(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820B3788, VehicleUpdateBackfire(base, ctx.r3.u32, ctx.r1.u32))
ITC_CAR_PHYSICS(820B39C8, VehicleUpdateTireWear(base, ctx.r3.u32, ctx.r1.u32 - 112))
// Where tan would take its _except1 path (an infinite or NaN steering), the
// original. r3 isn't kept: the step sets it again after this call.
extern "C" REX_FUNC(sub_820B18B8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B18B8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!VehicleUpdateSteering(base, ctx.r3.u32, ctx.r1.u32 - 160)) {
    __imp__sub_820B18B8(ctx, base);
  }
}
// Mode 2 in a forward gear (sub_820B0340, never in play): the original.
extern "C" REX_FUNC(sub_820B2100) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B2100(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!VehicleUpdateTorqueSplit(base, ctx.r3.u32)) {
    __imp__sub_820B2100(ctx, base);
  }
}
ITC_CAR_PHYSICS(820B24A8, VehicleUpdateTurbo(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820B3F80,
                ctx.r3.u64 = VehicleUpdateTireForces(base, ctx.r3.u32, ctx.r1.u32 - 272))
ITC_CAR_PHYSICS(820B2808, ctx.r3.u64 = VehicleUpdateAtRest(base, ctx.r3.u32, ctx.r1.u32 - 192))
ITC_CAR_PHYSICS(820B2A48, ctx.r3.u64 = VehicleUpdateAero(base, ctx.r3.u32, ctx.r1.u32 - 240))
ITC_CAR_PHYSICS(820B2CC0, ctx.r3.u64 = VehicleApplyForces(base, ctx.r3.u32, ctx.r1.u32 - 352))
ITC_CAR_PHYSICS(820B1B00,
                ctx.r3.u64 = VehicleUpdateWheelAxes(base, ctx.r3.u32, ctx.r1.u32 - 256))

// --- The vehicle's state from its body, its copies ---

// f1 comes back as 0.5 (the midpoints' scale), r3 as +2016.
ITC_CAR_PHYSICS(820B13C8,
                ctx.f1.f64 = VehicleSyncFromBody(base, ctx.r3.u32, ctx.r1.u32 - 368);
                ctx.r3.u64 = ctx.r3.u32 + 2016)
// r3 = out, r4 = the record; r3 comes back as out.
ITC_CAR_PHYSICS(820B0A50, WheelCopy(base, ctx.r3.u32, ctx.r4.u32))
ITC_CAR_PHYSICS(820B0CC0, WheelRecord2Copy(base, ctx.r3.u32, ctx.r4.u32))

// f1 comes back as the last Quaternion::FromAxisAngle's, r3 as +3312
// (memcpy's).
extern "C" REX_FUNC(sub_820B0EE8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B0EE8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  VehicleReset(base, ctx.r3.u32, ctx.r1.u32 - 240, &f1);
  ctx.f1.f64 = f1;
  ctx.r3.u64 = ctx.r3.u32 + 3312;
}
ITC_CAR_PHYSICS(820B4AA8, VehicleSetUpGearing(base, ctx.r3.u32, ctx.r1.u32 - 176))
// f1 = the input; f1 comes back as the second sin's.
ITC_CAR_PHYSICS(820B3D10, ctx.f1.f64 = VehicleSetSteering(base, ctx.r3.u32, ctx.f1.f64))
// SteerRecord::Fill and SteerRecord::Curve (sub_820B0668, sub_820B3E98) have
// no adapters: their caller in play, SteerTowards, is native.
// f1 = the target, f2 = dt, f3, r4 and r5 = the bytes; f1 comes back as the
// curve. Where tan would fail, the original. r3 isn't kept (tan's _ctrlfp
// leaves it; its caller sets it again).
extern "C" REX_FUNC(sub_820B5FD8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B5FD8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  if (!VehicleSteerTowards(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.r4.u32,
                           ctx.r5.u32, &f1)) {
    __imp__sub_820B5FD8(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
}

// --- The suspension ---

// r3 comes back as +2592, f1 as wheel 3's +8.
extern "C" REX_FUNC(sub_820B6328) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B6328(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  ctx.r3.u64 = VehicleUpdateSuspension(base, ctx.r3.u32, ctx.r1.u32 - 144, &f1);
  ctx.f1.f64 = f1;
}
// Vehicle::UpdateWheelSuspension and SuspensionSpring (sub_820B5588,
// sub_820B0198) have no adapters: their callers in play are native.

// --- The drivetrain ---

// r3 left as it is (the physics step relies on it).
ITC_CAR_PHYSICS(820B4778, VehicleUpdateDrivetrain(base, ctx.r3.u32, ctx.r1.u32 - 176))
// Vehicle::UpdateDrivenAxle (sub_820B31D0) has no adapter: its caller in
// play is native.

// --- The gearbox ---

// Vehicle::UpdateGearbox (sub_820B4CF0) has no adapter: its caller in play
// is native. r3 left as it is (the physics step relies on it).
extern "C" REX_FUNC(sub_820B6430) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B6430(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  VehicleShiftGears(base, ctx.r3.u32, ctx.r1.u32 - 96);
}

// --- The collision world ---

// r3 = the flags; f1 = the value.
// The getters CollisionSurface::A to D (sub_820BC1F8, sub_820BC240,
// sub_820BC288, sub_820BC2D0) have no adapters: their callers in play, the
// wrappers below, are native.
ITC_CAR_PHYSICS(820BC540, ctx.f1.f64 = CollisionSurfaceOfA(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820BC548, ctx.f1.f64 = CollisionSurfaceOfB(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820BC530, ctx.f1.f64 = CollisionSurfaceOfC(base, ctx.r3.u32))
ITC_CAR_PHYSICS(820BC538, ctx.f1.f64 = CollisionSurfaceOfD(base, ctx.r3.u32))
// r4 = the bits, f1 = the value.
ITC_CAR_PHYSICS(820BC318, CollisionSurfaceSetA(base, ctx.r4.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820BC360, CollisionSurfaceSetB(base, ctx.r4.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820BC3A8, CollisionSurfaceSetC(base, ctx.r4.u32, ctx.f1.f64))
ITC_CAR_PHYSICS(820BC3F0, CollisionSurfaceSetD(base, ctx.r4.u32, ctx.f1.f64))
// CollisionSurfaces::Reset (sub_820BC438) has no adapter: its caller in
// play, ResetSurfaces, is native.
// r3 comes back as 0x826C2E30.
ITC_CAR_PHYSICS(820BC550, CollisionWorldResetSurfaces(base); ctx.r3.u64 = 0x826C2E30)
// CollisionShape::Init (sub_820BAA78) has no adapter: its caller in play,
// the collision object's constructor, is native.
// r3 = the object (left as it is).
ITC_CAR_PHYSICS(820B7AE8, CollisionObjectConstruct(base, ctx.r3.u32))
// r4 = the point, r5 = the force; r3 comes back as +176.
ITC_CAR_PHYSICS(820B7DF8, ctx.r3.u64 = CollisionObjectAccumulateForce(base, ctx.r3.u32,
                                                                      ctx.r4.u32, ctx.r5.u32,
                                                                      ctx.r1.u32 - 160))
// r4 = the offset; r3 comes back as +304. A shape record whose current
// copy isn't a box: the original.
extern "C" REX_FUNC(sub_820B7B60) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B7B60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!CollisionObjectTranslate(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128)) {
    __imp__sub_820B7B60(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 304;
}
// r3 comes back as the contact, or 0.
ITC_CAR_PHYSICS(820B7C50, ctx.r3.u64 = CollisionObjectAddContact(base, ctx.r3.u32))
// r3 = out, r4 = the summary, r5 = the hits, r6 = the count; r3 comes back
// as the count left.
ITC_CAR_PHYSICS(820B7ED0, ctx.r3.u64 = CollisionRecordsMerge(base, ctx.r3.u32, ctx.r4.u32,
                                                             ctx.r5.u32, ctx.r6.u32,
                                                             ctx.r1.u32 - 304))
// CollisionWorld::HasPair, LinkPair, UnlinkPair and MovePair (sub_820B8C20,
// sub_820BC068, sub_820BBC40, sub_820BBD30) have no adapters: their only
// callers, AddPairs and RemovePair, are native.
// r3 = the object, r4 = the groups.
ITC_CAR_PHYSICS(820B94B0, ctx.r3.u64 = CollisionWorldAddPairs(base, ctx.r3.u32, ctx.r4.u32))
// CollisionWorld::RemovePair (sub_820B8A88) has no adapter: its callers in
// play, RemovePairsOf and RemovePairsByGroups, are native.
// r3 comes back as 0x826C2E30 (ResetSurfaces').
ITC_CAR_PHYSICS(820B85E8, ctx.r3.u64 = CollisionWorldReset(base))
// CollisionWorld::RemovePairsOf and RemovePairsByGroups (sub_820B8BA8,
// sub_820B8AF8) have no adapters: their only callers, RemoveObject and
// CollisionObject::RemoveGroups, are native. Where a pair's removal can't be
// native, the rest of the removals by the original (from the start: the
// same as it would have done).
// r3 = the object.
extern "C" REX_FUNC(sub_820B9448) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B9448(ctx, base);
    return;
  }
  const uint32_t object = ctx.r3.u32;
  uint32_t r3 = 0;
  if (!CollisionWorldRemoveObject(base, object, &r3)) {
    ctx.r3.u64 = object;
    ctx.r1.u64 = ctx.r1.u32 - 112;
    __imp__sub_820B8BA8(ctx, base);
    ctx.r1.u64 = ctx.r1.u32 + 112;
    return;
  }
  ctx.r3.u64 = r3;
}
// r3 = the object, r4 = the bits.
ITC_CAR_PHYSICS(820B7CC8, ctx.r3.u64 = CollisionObjectAddGroups(base, ctx.r3.u32, ctx.r4.u32))
extern "C" REX_FUNC(sub_820B7CF0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B7CF0(ctx, base);
    return;
  }
  const uint32_t object = ctx.r3.u32;
  uint32_t groups = 0, r3 = 0;
  if (!CollisionObjectRemoveGroups(base, object, ctx.r4.u32, &groups, &r3)) {
    // RemovePairsByGroups is the original's tail call (its frame here).
    ctx.r3.u64 = object;
    ctx.r4.u64 = groups;
    __imp__sub_820B8AF8(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

// --- The car data and the vehicle's constructor ---

// CarData::SetDefaults (sub_820AECB8) has no adapter: its caller in play,
// CarData::CarData, is native.
// r3 = the car data (left as it is).
ITC_CAR_PHYSICS(820AEF80, CarDataConstruct(base, ctx.r3.u32, ctx.r1.u32 - 96))
// CarData::FillAxle, ConvertUnits and Derive, sub_820C05C8
// (CarData::ApplySettings) and Vehicle::ApplyTuning (sub_820AEE98,
// sub_820AF018, sub_820AF1C0, sub_820C05C8, sub_820C0D28) have no adapters:
// their only caller, Vehicle::SetupFromCar, is native.
// r3 = the car data, r4 = the car object; f1 comes back as CarData::Derive's,
// r3 as its temporary body (r1 - 368 - 1008 + 352). Where ApplyTuning's
// memcpy wouldn't be copied natively, the original.
extern "C" REX_FUNC(sub_820C27E8) {
  REX_FUNC_PROLOGUE();
  const uint32_t frame = ctx.r1.u32 - 368;
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820C27E8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  if (!VehicleSetupFromCar(base, ctx.r3.u32, ctx.r4.u32, frame, &f1)) {
    __imp__sub_820C27E8(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
  ctx.r3.u64 = frame - 1008 + 352;
}
// r3 = the vehicle (left as it is); f1 comes back as Vehicle::Reset's.
extern "C" REX_FUNC(sub_820B42C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B42C8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  VehicleConstruct(base, ctx.r3.u32, ctx.r1.u32 - 112, &f1);
  ctx.f1.f64 = f1;
}

// --- The vehicle's physics objects (src/car_physics/vehicle_physics.cpp) ---

namespace {

// The ones allocating or freeing guest memory: only while the game's heap
// is the allocation layer's.
bool NativeLifetime() {
  return ITC_FLAG(native_car_physics) && hooks::NativeHeapActive();
}

}  // namespace

// r3 = the body, r4 = the float (r3 left as it is).
ITC_CAR_PHYSICS(8259D5A8, RigidBodySetFloat48(base, ctx.r3.u32, ctx.r4.u32))
// CollisionWorld::Callback (sub_820B8798) has no adapter: its callers in
// play, Vehicle::ProbeGround and Unflip, are native.
// r3 = the collision object; r3 comes back as its shape record (+32).
extern "C" REX_FUNC(sub_820B7998) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820B7998(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t object = ctx.r3.u32;
  if (!CollisionObjectResetTransform(base, object, ctx.r1.u32 - 112)) {
    __imp__sub_820B7998(ctx, base);
    return;
  }
  ctx.r3.u64 = object + 32;
}
// CollisionObject::Init (sub_820B7A00) has no adapter: its only caller,
// CollisionWorld::AddObject, is native.
// r3 = the body, r4 = the shape, r5 = the groups; r3 comes back as the
// object.
extern "C" REX_FUNC(sub_820B9570) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime()) {
    __imp__sub_820B9570(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t object = 0;
  if (!CollisionWorldAddObject(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 128,
                               &object)) {
    __imp__sub_820B9570(ctx, base);
    return;
  }
  ctx.r3.u64 = object;
}
// r3 = the vehicle.
extern "C" REX_FUNC(sub_820B0E70) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime()) {
    __imp__sub_820B0E70(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!VehicleDestroyPhysics(base, ctx.r3.u32, &r3)) {
    __imp__sub_820B0E70(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
// r3 = the vehicle; f1 comes back as the last dot product (+88), r3 as
// r1 - 304 + 160. A callback other than the ground's: the original.
extern "C" REX_FUNC(sub_820B4420) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics) || !CollisionWorldCallbackKnown(base)) {
    __imp__sub_820B4420(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  ctx.r3.u64 = VehicleProbeGround(base, ctx.r3.u32, ctx.r1.u32 - 304, &f1);
  ctx.f1.f64 = f1;
}
// r3 = the vehicle; r3 comes back as the original leaves it.
extern "C" REX_FUNC(sub_820AFE90) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics)) {
    __imp__sub_820AFE90(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!VehicleUnflip(base, ctx.r3.u32, ctx.r1.u32 - 384, &r3)) {
    __imp__sub_820AFE90(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
// r3 = the vehicle; r3 comes back as UpdateWheelAxes'.
extern "C" REX_FUNC(sub_820B62E8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_physics) || !CollisionWorldCallbackKnown(base)) {
    __imp__sub_820B62E8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = VehicleRefreshFromBody(base, ctx.r3.u32, ctx.r1.u32 - 96);
}
