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
}

namespace {

// A vehicle (6,272 bytes) is larger than the scratch area: its cases use
// guest stack further down (as the sprites' harness), with what it points
// to: the car data, the surface records and the rigid body.
constexpr uint32_t kVehicleArea = 0x10000;  // below the scratch area
constexpr uint32_t kVehicle = 0, kCarData = 0x2000, kSurfaces = 0x2800, kBody = 0x2900;
constexpr uint32_t kVehicleAreaBytes = 0x2C00;

// What r4 holds: a wheel (0-3), an axle's first wheel (0 or 2), a byte
// (any word), or the tire coefficients (four floats in the surface
// records' area).
enum R4Kind { kWheelR4, kAxleR4, kByteR4, kCoefficientsR4 };

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
  };
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN",
                                     ", NaNs and infinities anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      for (uint64_t seed = 0; seed < 100; seed++) {
        Random pick = Seeded(seed * 17 + 3 * uint64_t(group) + 2);
        auto word = [group](Random& r) { return group == kNaNs ? FloatWord(r) : NumberWord(r); };
        double f1 = double(std::bit_cast<float>(word(pick)));
        const uint32_t wheel = pick.Below(4);
        const uint32_t byte_word = pick.Below(3) == 0 ? uint32_t(pick.Next()) : pick.Below(2);
        if (f.unit_f1 && pick.Below(4) != 0) {
          f1 = double(float(pick.Below(1100)) / 1000.0f);
        }
        uint32_t r4 = wheel;
        switch (f.r4) {
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
          t.r3.u64 = area + kVehicle;
          t.r4.u64 = r4;
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
        results.Check(result[0], result[1],
                      fmt::format("seed {} r4 {:08X} f1 {:016X}", seed, r4,
                                  std::bit_cast<uint64_t>(f1)));
      }
      results.Report();
    }
  }
}

}  // namespace harness
