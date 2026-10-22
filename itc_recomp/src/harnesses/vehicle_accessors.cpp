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

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <iterator>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_vehicle_accessors);

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

// Runs the recompiled version with the flags off, so the functions it calls
// that are ours run the originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_vehicle_accessors);
  ITC_FLAG(native_vehicle_accessors) = false;
  recompiled(t, base);
  ITC_FLAG(native_vehicle_accessors) = native;
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
             GuestFunction recompiled, GuestFunction native, const std::string& what) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    setup();
    PPCContext t = CallContext(h);
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
    for (int i = 0; i < 8; i++) {
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
  memory->SystemHeapFree(block);
}

}  // namespace harness
