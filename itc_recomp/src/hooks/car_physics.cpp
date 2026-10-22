// The car physics natively: adapters in front of src/car_physics/
// (docs/functions/car-physics.md). Each passes the original's stack frame
// (its r1 after the stwu) for the temporaries, and leaves r3 (and f1 where
// the original leaves a value there) as the original does: the physics step
// relies on callees leaving r3 as it was (sub_820B6498's row).

#include "guest.h"

#include "base/flags.h"
#include "car_physics/car_physics.h"

ITC_FLAG_BOOL(native_car_physics, true, "ITC",
              "Run the car physics (docs/functions/car-physics.md: the rigid body, sub_820B6A38 "
              "to sub_820B7928) natively instead of through the translated "
              "code (the same words, bit for bit; checked by --run_harnesses=car_physics)");

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
// r4 = out (3 floats), r5 = the point; f1 = the last dot product, r3 its
// temporary (the frame's +192).
ITC_CAR_PHYSICS(820B6A38,
                ctx.f1.f64 = RigidBodyEffectiveMassAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                      ctx.r1.u32 - 272);
                ctx.r3.u64 = ctx.r1.u32 - 272 + 192)
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
