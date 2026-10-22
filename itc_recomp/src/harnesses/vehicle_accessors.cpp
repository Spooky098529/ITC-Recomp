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
      {"RigidBody::Field80 (sub_820B6880)", __imp__sub_820B6880, NativeRigidBodyField80},
      {"RigidBody::Velocity (sub_820B6888)", __imp__sub_820B6888, NativeRigidBodyVelocity},
      {"RigidBody::Field144 (sub_820B6890)", __imp__sub_820B6890, NativeRigidBodyField144},
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
  memory->SystemHeapFree(block);
}

}  // namespace harness
