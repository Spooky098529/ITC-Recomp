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
             bool compare_f1 = true) {
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
    for (int i = 0; i < 4; i++) {
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
  memory->SystemHeapFree(block);
}

}  // namespace harness
