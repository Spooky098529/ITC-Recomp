// The car physics natively: adapters in front of src/car_physics/
// (docs/functions/car-physics.md). Each passes the original's stack frame
// (its r1 after the stwu) for the temporaries, and leaves r3 (and f1 where
// the original leaves a value there) as the original does: the physics step
// relies on callees leaving r3 as it was (sub_820B6498's row).

#include "guest.h"

#include "base/flags.h"
#include "car_physics/car_physics.h"

ITC_FLAG_BOOL(native_car_physics, true, "ITC",
              "Run the car physics (docs/functions/car-physics.md: the rigid body's updates and "
              "setters, sub_820B6710 to sub_820B75B8) natively instead of through the translated "
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

// r3 comes back as the body matrix's fourth row (the last copy's).
ITC_CAR_PHYSICS(820B6E70, RigidBodyUpdateOrigin(base, ctx.r3.u32, ctx.r1.u32 - 176);
                ctx.r3.u64 = ctx.r3.u32 + 336)
ITC_CAR_PHYSICS(820B70C8, ctx.r3.u64 = RigidBodyUpdateVelocity(base, ctx.r3.u32, ctx.r1.u32 - 160))
// r3 comes back as +96.
ITC_CAR_PHYSICS(820B7478,
                ctx.f1.f64 = RigidBodyUpdateAngularVelocity(base, ctx.r3.u32, ctx.r1.u32 - 144);
                ctx.r3.u64 = ctx.r3.u32 + 96)
// r3 comes back as +416 (Matrix::MultiplyReversed's out).
ITC_CAR_PHYSICS_VECTOR(820B6F38, RigidBodyUpdateOrientation(base, ctx.r3.u32, ctx.r1.u32 - 320);
                       ctx.r3.u64 = ctx.r3.u32 + 416)
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
