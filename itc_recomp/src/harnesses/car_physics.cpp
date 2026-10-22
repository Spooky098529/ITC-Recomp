// The car physics (src/car_physics/) against the recompiled functions: a
// rigid body (and the vectors passed with it) in a block of the scratch
// area, filled anew for each case with float words, and the whole block,
// r3 and (where the original leaves a value there) f1 compared. The
// originals run with the native flags of their callees off (the vector
// library, the matrix math, the vehicle accessors, the C runtime's math and
// the car physics), so the whole chain is the recompiled code's. Three
// groups, as the matrix math's harness: numbers only (infinities, denormals
// and signed zeros among them), finite numbers with one NaN word put in the
// block or f1, and NaNs anywhere (expected to differ: where two NaNs meet
// in one operation, which comes out depends on the order the compiler gave
// the operands, in the translated code as in ours, and the translated
// code, built without -ffp-model=strict, may also flip a NaN's sign where
// LLVM rewrites a subtraction; the game's bodies hold no NaNs).

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

uint32_t NumberWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (std::isnan(std::bit_cast<float>(word)));
  return word;
}

// Finite numbers only, so no NaN is made from them (inf - inf, 0 x inf)
// to meet the one NaN put in.
uint32_t FiniteWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (!std::isfinite(std::bit_cast<float>(word)));
  return word;
}

Random Seeded(uint64_t seed) {
  Random r;
  for (uint64_t i = 0; i < seed; i++) {
    r.Next();
  }
  return r;
}

// The block: the body at +0 (528 bytes), the vector argument at +0x300.
constexpr uint32_t kBlockBytes = 0x400;
constexpr uint32_t kArgument = 0x300;

}  // namespace

void CarPhysics(Context& h) {
  // The block 16-byte aligned, as the game's bodies are.
  const uint32_t block = (h.data + 15) & ~15u;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    bool compare_f1;
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
  };
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN", ", NaNs anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      for (uint64_t seed = 0; seed < 200; seed++) {
        Random fill = Seeded(seed * 11 + 5 * uint64_t(group) + 1);
        auto word = [group](Random& r) {
          return group == kNaNs ? FloatWord(r) : group == kOneNaN ? FiniteWord(r) : NumberWord(r);
        };
        const double f1 = double(std::bit_cast<float>(word(fill)));
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
          t.r3.u64 = block;
          t.r4.u64 = block + kArgument;
          t.f1.f64 = group == kOneNaN && nan_at >= kBlockBytes / 4
                         ? double(std::bit_cast<float>(nan_word))
                         : f1;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v] = Read(h.base, block, kBlockBytes);
          for (int i = 0; i < 4; i++) {
            result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
          }
          for (int i = 0; f.compare_f1 && i < 8; i++) {
            result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
          }
        }
        results.Check(result[0], result[1],
                      fmt::format("seed {} f1 {:016X}", seed, std::bit_cast<uint64_t>(f1)));
      }
      results.Report();
    }
  }
}

}  // namespace harness
