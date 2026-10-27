// The race's set-up (src/race_setup/, docs/functions/race-setup.md) against
// the recompiled functions. Each case fills the scratch area and a block
// (a racer record is 3,936 bytes, more than the scratch area holds) with
// random words, writes its inputs, runs the original (with
// --native_race_setup off, so its callees are the originals too) or the
// plain C++ through the register steps (hooks/race_setup.h), and compares
// the block, the scratch area and r3 (all 64 bits): exactly, except the
// floats a function copies from its inputs (harness::Outputs' floats).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_specs/car_specs.h"
#include "data_tables/data_tables.h"
#include "hooks/race_setup.h"
#include "race_setup/race_setup.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_setup);

namespace harness {

// Vehicle::SetupFromCar's cases' helpers (src/harnesses/car_physics.cpp).
bool SetupFromCarFits(uint8_t* base, uint32_t car, uint32_t r1);
void WriteSetupFromCarCase(uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                           uint32_t stack_end, uint64_t seed, int group);
void RecordSetupFromCar(Outputs& out, uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                        uint32_t stack_end, const PPCContext& t);
void RunSetupFromCarOriginal(GuestFunction recompiled, PPCContext& ctx, uint8_t* base);

namespace {

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t words, Random& random) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, uint32_t(random.Next()));
  }
}

// A float's bits: now and then NaN (any sign, payload), an infinity, a zero
// of either sign, a denormal; else a value of either sign.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(16)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x807FFFFFu);
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    case 3:
      return (uint32_t(r.Next()) & 0x807FFFFFu) | 1u;
    default: {
      const float value = float(int32_t(r.Below(20001)) - 10000) / 1000.0f;
      return std::bit_cast<uint32_t>(value);
    }
  }
}

// Float fields (offsets from the block) compared as floats; the rest exactly.
void Record(Outputs& out, uint8_t* base, const Block& block, const std::vector<uint32_t>& floats) {
  uint32_t at = 0;
  for (uint32_t offset : floats) {
    if (offset > at) {
      out.Bytes(Read(base, block.address + at, offset - at));
    }
    out.Float(guest::LoadU32(base, block.address + offset));
    at = offset + 4;
  }
  out.Bytes(Read(base, block.address + at, block.bytes - at));
}

// One case through both versions: `setup(random)` writes the inputs (handed
// a copy of the same generator state both times), `args(t)` sets the
// registers; `floats` the block's float fields.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, const Block& block, Setup setup, Args args,
          GuestFunction recompiled, GuestFunction native, const std::vector<uint32_t>& floats,
          const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, h.data, kScratchBytes / 4, random);
    WriteRandom(h.base, block.address, block.bytes / 4, random);
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_race_setup) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_setup) = true;
    Record(out[v], h.base, block, floats);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A function of one pointer (r3) at a random place in the block.
void OnePointer(Context& h, const Block& block, const char* name, GuestFunction recompiled,
                GuestFunction native, uint32_t bytes) {
  Results results(name);
  for (int c = 0; c < 64; c++) {
    const uint32_t at = block.address + 4 * h.random.Below((block.bytes - bytes) / 4);
    Case(
        h, results, block, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; }, recompiled,
        native, {}, fmt::format("case {}: +{:X}", c, at - block.address));
  }
  results.Report();
}

void Parts(Context& h, const Block& block) {
  OnePointer(h, block, "Car::ResetDecals40 (sub_820CC4A0)", __imp__sub_820CC4A0,
             hooks::RaceSetupResetDecals40, 1280);
  OnePointer(h, block, "Car::ResetDecals6 (sub_820CCBF0)", __imp__sub_820CCBF0,
             hooks::RaceSetupResetDecals6, 192);
  OnePointer(h, block, "Car::ResetSettings (sub_820CD760)", __imp__sub_820CD760,
             hooks::RaceSetupResetSettings, 80);
  OnePointer(h, block, "sub_820CCC70", __imp__sub_820CCC70, hooks::RaceSetupReset1568, 20);
  OnePointer(h, block, "Car::Clear (sub_820CD7F0)", __imp__sub_820CD7F0,
             hooks::RaceSetupCarClear, 1888);
  OnePointer(h, block, "Car::Init (sub_820CD5D8)", __imp__sub_820CD5D8, hooks::RaceSetupCarInit,
             3896);

  Results tuning("CarTuning::Reset (sub_820CD618)");
  for (int c = 0; c < 64; c++) {
    const uint32_t at = block.address + 4 * h.random.Below((block.bytes - 240) / 4);
    const uint32_t flag = c % 3 == 0 ? 0 : c % 3 == 1 ? 1 : uint32_t(h.random.Next());
    const uint32_t high = uint32_t(h.random.Next());
    Case(
        h, tuning, block, [](Random&) {},
        [&](PPCContext& t) {
          t.r3.u64 = at;
          t.r4.u64 = uint64_t(high) << 32 | flag;
        },
        __imp__sub_820CD618, hooks::RaceSetupTuningReset, {},
        fmt::format("case {}: +{:X}, flag {:X}", c, at - block.address, flag));
  }
  tuning.Report();
}

// CarDecal::FromBuild: the decal at +0x100, the build's entry at +0x200 with
// random floats (+0 below 0.0 half the time, -0.0, NaNs and infinities among
// them).
void Decals(Context& h, const Block& block) {
  constexpr uint32_t kDecal = 0x100, kBuild = 0x200;
  Results results("CarDecal::FromBuild (sub_820CDC18)");
  for (int c = 0; c < 200; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          for (uint32_t offset : {0u, 4u, 8u, 12u, 16u}) {
            guest::StoreU32(h.base, block.address + kBuild + offset, RandomFloat(r));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address + kDecal;
          t.r4.u64 = block.address + kBuild;
        },
        __imp__sub_820CDC18, hooks::RaceSetupDecalFromBuild,
        {kDecal + 0, kDecal + 4, kDecal + 8, kDecal + 12, kDecal + 16},
        fmt::format("case {}", c));
  }
  results.Report();
}

// --- Car::SetModel and Car::FromBuild ---

// The block's layout for them: the car object, the build, the spec and the
// parameter object (its +12 the specs' table, set so the case's id finds the
// spec here; +64 + 4m the wheel models' first records).
constexpr uint32_t kCar = 0, kBuild = 0x800, kSpec = 0x1400, kParams = 0x1800;
constexpr uint32_t kSpecBytes = 8432;

uint32_t RandomId(Random& r) {
  static constexpr uint32_t kIds[] = {0, 11, 55, 56, 63, 64, 66, 68, 69, 70, 71};
  switch (r.Below(4)) {
    case 0:
      return kIds[r.Below(11)];
    case 1:
      return uint32_t(r.Next()) & 0xFFFF;
    default:
      return r.Below(72);
  }
}

// A word near a threshold, or random.
uint32_t Near(Random& r, uint32_t threshold) {
  return r.Below(4) == 0 ? uint32_t(r.Next()) : threshold - 2 + r.Below(5);
}

// The spec: random words, its floats random floats, the drive type 0 to 3
// or random, the four limits near theirs.
void WriteSpec(uint8_t* base, uint32_t spec, Random& r) {
  WriteRandom(base, spec, 0x400 / 4, r);
  for (uint32_t offset = 372; offset <= 416; offset += 4) {
    guest::StoreU32(base, spec + offset, RandomFloat(r));
  }
  guest::StoreU32(base, spec + 364, RandomFloat(r));
  guest::StoreU32(base, spec + 356, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(4));
  guest::StoreU8(base, spec + 709, r.Below(2) ? 0 : uint8_t(r.Next()));
  guest::StoreU32(base, spec + 648, Near(r, 2000));
  guest::StoreU32(base, spec + 716, Near(r, 4700));
  guest::StoreU32(base, spec + 720, Near(r, 2000));
  guest::StoreU32(base, spec + 724, Near(r, 1700));
}

// The car object's float fields: the drive and gears (+36 to +68), the 46
// decals' first five words.
std::vector<uint32_t> CarFloats() {
  std::vector<uint32_t> floats;
  for (uint32_t offset = 36; offset <= 68; offset += 4) {
    floats.push_back(kCar + offset);
  }
  for (uint32_t i = 0; i < 46; i++) {
    for (uint32_t k = 0; k < 5; k++) {
      floats.push_back(kCar + 96 + 32 * i + 4 * k);
    }
  }
  return floats;
}

// One case through both versions with the parameter global and the rand
// seed set alike on both sides (and compared after), put back afterwards.
template <typename Setup, typename Args>
void CarCase(Context& h, Results& results, const Block& block, Setup setup, Args args,
             GuestFunction recompiled, GuestFunction native, const std::string& what) {
  static const std::vector<uint32_t> floats = CarFloats();
  const uint32_t params = guest::LoadU32(h.base, car_specs::kParams);
  const uint32_t seed = guest::LoadU32(h.base, race_setup::kRandSeed);
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, h.data, kScratchBytes / 4, random);
    WriteRandom(h.base, block.address, block.bytes / 4, random);
    guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(random.Next()));
    guest::StoreU32(h.base, car_specs::kParams, block.address + kParams);
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_race_setup) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_setup) = true;
    Record(out[v], h.base, block, floats);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(guest::LoadU32(h.base, race_setup::kRandSeed));
    out[v].Word(guest::LoadU32(h.base, car_specs::kParams));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    guest::StoreU32(h.base, car_specs::kParams, params);
    guest::StoreU32(h.base, race_setup::kRandSeed, seed);
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// Points the parameter object's specs' table so that `id` finds the spec.
void PointSpec(uint8_t* base, const Block& block, uint32_t id) {
  guest::StoreU32(base, block.address + kParams + 12, block.address + kSpec - kSpecBytes * id);
}

void SetModels(Context& h, const Block& block) {
  Results results("Car::SetModel (sub_820CD8A8)");
  for (int c = 0; c < 300; c++) {
    uint32_t id = 0;
    CarCase(
        h, results, block,
        [&](Random& r) {
          id = RandomId(r);
          WriteSpec(h.base, block.address + kSpec, r);
          PointSpec(h.base, block, id);
          // Without the parameters now and then (Params::Exists 0).
          if (r.Below(10) == 0) {
            guest::StoreU32(h.base, car_specs::kParams, 0);
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address + kCar;
          t.r4.u64 = id;
        },
        __imp__sub_820CD8A8, hooks::RaceSetupSetModel, fmt::format("case {}: car {}", c, id));
  }
  results.Report();
}

void FromBuilds(Context& h, const Block& block) {
  Results results("Car::FromBuild (sub_820CDCE0)");
  for (int c = 0; c < 300; c++) {
    uint32_t id = 0;
    CarCase(
        h, results, block,
        [&](Random& r) {
          const uint32_t build = block.address + kBuild;
          id = RandomId(r);
          WriteSpec(h.base, block.address + kSpec, r);
          PointSpec(h.base, block, id);
          guest::StoreU32(h.base, build + 2788, id);
          // The gear factors: 0.0 of either sign now and then.
          for (uint32_t i = 0; i < 7; i++) {
            guest::StoreU32(h.base, build + 4 * i,
                            r.Below(4) == 0 ? (r.Below(2) ? 0u : 0x80000000u) : RandomFloat(r));
          }
          // The decals' entries' floats.
          for (uint32_t from : {392u, 500u}) {
            for (uint32_t k = 0; k < 3; k++) {
              for (uint32_t w = 0; w < 5; w++) {
                guest::StoreU32(h.base, build + from + 36 * k + 4 * w, RandomFloat(r));
              }
            }
          }
          for (uint32_t from : {608u, 968u, 1328u, 1688u, 1868u, 2048u, 2408u, 2588u}) {
            for (uint32_t k = 0; k < 5; k++) {
              for (uint32_t w = 0; w < 5; w++) {
                guest::StoreU32(h.base, build + from + 36 * k + 4 * w, RandomFloat(r));
              }
            }
          }
          // The wheel numbers: models -1 to 30 (the parameter object holds
          // their first records), the number's last digit any.
          for (uint32_t offset : {232u, 240u}) {
            guest::StoreU32(h.base, build + offset, uint32_t(int32_t(r.Below(320)) - 15));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address + kCar;
          t.r4.u64 = block.address + kBuild;
        },
        __imp__sub_820CDCE0, hooks::RaceSetupFromBuild, fmt::format("case {}: car {}", c, id));
  }
  results.Report();
}

// --- The race data's small pieces ---

constexpr uint32_t kSaveDataPointer = 0x826C5B58;  // SaveData::Of's table (player 0)

// One case through both versions with the globals `globals` (words) kept,
// compared after each side and put back; the block, the scratch area and r3
// as Case's (`floats` the block's float fields).
template <typename Setup, typename Args>
void GlobalsCase(Context& h, Results& results, const Block& block,
                 const std::vector<uint32_t>& globals, Setup setup, Args args,
                 GuestFunction recompiled, GuestFunction native,
                 const std::vector<uint32_t>& floats, const std::string& what) {
  std::vector<uint32_t> saved;
  for (uint32_t global : globals) {
    saved.push_back(guest::LoadU32(h.base, global));
  }
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, h.data, kScratchBytes / 4, random);
    WriteRandom(h.base, block.address, block.bytes / 4, random);
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_race_setup) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_setup) = true;
    Record(out[v], h.base, block, floats);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    for (size_t i = 0; i < globals.size(); i++) {
      out[v].Word(guest::LoadU32(h.base, globals[i]));
      guest::StoreU32(h.base, globals[i], saved[i]);
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Double(t.f1.u64);
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

void Leaves(Context& h, const Block& block) {
  {
    Results results("sub_82101630");
    for (int c = 0; c < 64; c++) {
      const int32_t location = int32_t(h.random.Below(28)) - 2;
      const uint64_t value = c % 4 == 0 ? h.random.Next() : h.random.Below(10);
      GlobalsCase(
          h, results, block, {}, [](Random&) {},
          [&](PPCContext& t) {
            t.r3.u64 = uint64_t(int64_t(location)) ^ (uint64_t(c % 3) << 40);
            t.r4.u64 = value;
          },
          __imp__sub_82101630, hooks::RaceSetupRivalLocation, {},
          fmt::format("case {}: location {}", c, location));
    }
    results.Report();
  }
  {
    // The race data's global pointed so its stage list (+0x59620) is the
    // block's start.
    Results results("Race::AddStage (sub_82101828)");
    for (int c = 0; c < 64; c++) {
      const uint32_t type = c % 5 == 0 ? uint32_t(h.random.Next()) : h.random.Below(6);
      GlobalsCase(
          h, results, block, {race_setup::kRaceData},
          [&](Random& r) {
            guest::StoreU32(h.base, race_setup::kRaceData, block.address - race_setup::kStageList);
            guest::StoreU32(h.base, block.address + 800, r.Below(11));
          },
          [&](PPCContext& t) { t.r3.u64 = type; }, __imp__sub_82101828,
          hooks::RaceSetupAddStage, {}, fmt::format("case {}: type {:X}", c, type));
    }
    results.Report();
  }
  {
    Results results("Stages::AddRacer (sub_820FFAB8)");
    for (int c = 0; c < 128; c++) {
      const int32_t stage = c % 2 == 0 ? -1 : int32_t(h.random.Below(10));
      const uint32_t racer = h.random.Below(100);
      const uint32_t word = c % 3 == 0 ? uint32_t(h.random.Next()) : 2;
      GlobalsCase(
          h, results, block, {},
          [&](Random& r) {
            guest::StoreU32(h.base, block.address + 800, r.Below(10));
            for (uint32_t i = 0; i < 10; i++) {
              guest::StoreU32(h.base, block.address + 80 * i + 76, r.Below(5));
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = block.address;
            t.r4.u64 = racer;
            t.r5.u64 = word;
            t.r6.s64 = stage;
          },
          __imp__sub_820FFAB8, hooks::RaceSetupStagesAddRacer, {},
          fmt::format("case {}: stage {}", c, stage));
    }
    results.Report();
  }
  {
    Results results("sub_820FF558");
    for (int c = 0; c < 32; c++) {
      const uint32_t at = block.address + 4 * h.random.Below(256);
      GlobalsCase(
          h, results, block, {}, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; },
          __imp__sub_820FF558, hooks::RaceSetupLapRecordReset, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results entries("sub_82101EB0");
    Results courses("CourseRecord::Reset (sub_82105C90)");
    for (int c = 0; c < 32; c++) {
      const uint32_t at = block.address + 4 * h.random.Below(256);
      GlobalsCase(
          h, entries, block, {}, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; },
          __imp__sub_82101EB0, hooks::RaceSetupEntryReset, {}, fmt::format("case {}", c));
      GlobalsCase(
          h, courses, block, {}, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; },
          __imp__sub_82105C90, hooks::RaceSetupCourseRecordReset, {}, fmt::format("case {}", c));
    }
    entries.Report();
    courses.Report();
  }
  {
    Results results("sub_82101D20");
    for (int c = 0; c < 32; c++) {
      const uint32_t at = block.address + 4 * h.random.Below(256);
      GlobalsCase(
          h, results, block, {}, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; },
          __imp__sub_82101D20, hooks::RaceSetupRaceDataEntryConstruct, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The mean (+0x100: a float, then the count) with random floats, NaNs
    // and infinities among them; the weight 0, 1, -1, the count's negative
    // (a new count of 0) or random; f1 a random float's value.
    Results results("RunningMean::Add (sub_82101698)");
    for (int c = 0; c < 200; c++) {
      uint32_t weight = 0;
      uint32_t value = 0;
      GlobalsCase(
          h, results, block, {},
          [&](Random& r) {
            guest::StoreU32(h.base, block.address + 0x100, RandomFloat(r));
            const uint32_t count = r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(100);
            guest::StoreU32(h.base, block.address + 0x104, count);
            static constexpr uint32_t kWeights[] = {0, 1, 0xFFFFFFFF};
            switch (r.Below(4)) {
              case 0:
                weight = uint32_t(r.Next());
                break;
              case 1:
                weight = 0u - count;
                break;
              default:
                weight = kWeights[r.Below(3)];
                break;
            }
            value = RandomFloat(r);
          },
          [&](PPCContext& t) {
            t.r3.u64 = block.address + 0x100;
            t.r5.u64 = weight;
            t.f1.f64 = double(std::bit_cast<float>(value));
          },
          __imp__sub_82101698, hooks::RaceSetupRunningMeanAdd, {0x100},
          fmt::format("case {}: weight {:X}", c, weight));
    }
    results.Report();
  }
  {
    Results results("Time::Pack (sub_82101700)");
    for (int c = 0; c < 200; c++) {
      uint64_t ms = 0;
      switch (c % 5) {
        case 0:
          ms = h.random.Next();
          break;
        case 1:
          ms = uint64_t(int64_t(int32_t(h.random.Below(20000000)) - 10000000));
          break;
        case 2:
          ms = uint64_t(int32_t(0x80000000u + h.random.Below(3))) ^ (h.random.Next() << 32);
          break;
        default:
          ms = h.random.Below(600000);
          break;
      }
      GlobalsCase(
          h, results, block, {}, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = ms; },
          __imp__sub_82101700, hooks::RaceSetupTimePack, {},
          fmt::format("case {}: {:X}", c, ms));
    }
    results.Report();
  }
  {
    // The save data's +48 at the block's start; modes 0 to 15, the counts
    // near 9999, +6008 the mode half the time.
    Results results("sub_82110AC0");
    for (int c = 0; c < 128; c++) {
      const uint32_t mode = h.random.Below(16);
      GlobalsCase(
          h, results, block, {},
          [&](Random& r) {
            const uint32_t count = block.address + (mode + 414) * 14;
            guest::StoreU16(h.base, count, uint16_t(r.Below(3) == 0 ? r.Next() : 9997 + r.Below(4)));
            guest::StoreU32(h.base, block.address + 6008, r.Below(2) ? mode : uint32_t(r.Next()));
            guest::StoreU16(h.base, block.address + 6012,
                            uint16_t(r.Below(3) == 0 ? r.Next() : 9997 + r.Below(4)));
          },
          [&](PPCContext& t) {
            t.r3.u64 = block.address;
            t.r4.u64 = mode;
          },
          __imp__sub_82110AC0, hooks::RaceSetupCountRace, {},
          fmt::format("case {}: mode {}", c, mode));
    }
    results.Report();
  }
  {
    // The event at +0 (its list at +0x100), the items at +0x1000 + 3008 x id
    // (ids 0 to 3: the tables' object at +0x300), the save data's records
    // at +0x400 (+0x199F8 from the save data).
    Results first("RaceEvent::FirstOwned (sub_821430A0)");
    Results current("RaceEvent::Current (sub_821434E8)");
    Results count("RaceEvent::Count (sub_821434F8)");
    Results current_first("RaceEvent::CurrentFirstOwned (sub_821434C8)");
    const std::vector<uint32_t> globals = {race_setup::kEvent, data_tables::kTables,
                                           kSaveDataPointer};
    for (int c = 0; c < 200; c++) {
      auto setup = [&](Random& r) {
        const uint32_t event = block.address;
        guest::StoreU32(h.base, race_setup::kEvent, event);
        guest::StoreU16(h.base, event + 20, uint16_t(int16_t(int32_t(r.Below(9)) - 1)));
        guest::StoreU32(h.base, event + 156, block.address + 0x100);
        for (uint32_t i = 0; i < 8; i++) {
          const uint32_t entry = block.address + 0x100 + 12 * i;
          guest::StoreU32(h.base, entry + 0, r.Below(4));
          guest::StoreU32(h.base, entry + 8, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
        }
        guest::StoreU32(h.base, data_tables::kTables, block.address + 0x300);
        guest::StoreU32(h.base, block.address + 0x300 + 16, block.address + 0x1000);
        for (uint32_t id = 0; id < 4; id++) {
          const uint32_t item = block.address + 0x1000 + 3008 * id;
          guest::StoreU16(h.base, item, uint16_t(int16_t(int32_t(r.Below(9)) - 1)));
          guest::StoreU8(h.base, item + 91, r.Below(3) == 0 ? uint8_t(r.Next()) : 1);
        }
        guest::StoreU32(h.base, kSaveDataPointer, block.address + 0x400 - 0x199F8);
        for (uint32_t i = 0; i < 12; i++) {
          const uint32_t record = block.address + 0x400 - 12 + 12 * i;
          for (uint32_t k = 0; k < 3; k++) {
            guest::StoreU16(h.base, record + 2 * k,
                            uint16_t(r.Below(2) == 0 ? r.Next() : int32_t(r.Below(5)) - 2));
          }
        }
      };
      GlobalsCase(
          h, first, block, globals, setup, [&](PPCContext& t) { t.r3.u64 = block.address; },
          __imp__sub_821430A0, hooks::RaceSetupEventFirstOwned, {}, fmt::format("case {}", c));
      GlobalsCase(
          h, current_first, block, globals,
          [&](Random& r) {
            setup(r);
            if (r.Below(8) == 0) {
              guest::StoreU32(h.base, race_setup::kEvent, 0);
            }
          },
          [](PPCContext&) {}, __imp__sub_821434C8, hooks::RaceSetupEventCurrentFirstOwned, {},
          fmt::format("case {}", c));
      if (c < 16) {
        GlobalsCase(
            h, current, block, globals, setup, [](PPCContext&) {}, __imp__sub_821434E8,
            hooks::RaceSetupEventCurrent, {}, fmt::format("case {}", c));
        GlobalsCase(
            h, count, block, globals, setup, [](PPCContext&) {}, __imp__sub_821434F8,
            hooks::RaceSetupEventCount, {}, fmt::format("case {}", c));
      }
    }
    first.Report();
    current.Report();
    count.Report();
    current_first.Report();
  }
}

// RaceData::RaceData on a block as big as the race data (a few cases: the
// whole of it compared).
void RaceDataConstructs(Context& h) {
  const Block data(0xA4000);
  if (!data.address) {
    ITC_LOG_ERROR("harness: race_setup: no block for the race data");
    return;
  }
  Results results("RaceData::RaceData (sub_821021B8)");
  for (int c = 0; c < 3; c++) {
    GlobalsCase(
        h, results, data, {}, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = data.address; },
        __imp__sub_821021B8, hooks::RaceSetupRaceDataConstruct, {}, fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

// CourseRoutes::Get: out at a random place in the block (r3's high word
// random in some cases), every index 0 to 22 and three below 0 whose entry
// lies below the frame (filled the same on both sides; -1 would read the
// back chain, 23 and above the register saves, which the native side doesn't
// write). The frame (the call's r1 - 4480) up to the scratch area filled
// with random words; compared: the block, the frame from +4 to the register
// saves (+4328: r14 to r31 and lr at r1 - 152 to r1 - 4), the caller's frame
// from r1 (the index word at r1 + 28) to the scratch area, the scratch area,
// and r3.
void CourseRoutes(Context& h, const Block& block) {
  Results results("CourseRoutes::Get (sub_820FFF50)");
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t frame = r1 - race_setup::kRoutesFrameBytes;
  constexpr uint32_t kBelow = 1024;  // filled below the frame
  constexpr uint32_t kSaves = race_setup::kRoutesFrameBytes - 152;
  std::vector<uint32_t> indexes;
  for (uint32_t i = 0; i < 23; i++) {
    indexes.push_back(i);
    indexes.push_back(i);
  }
  for (uint32_t i : {0xFFFFFFFEu, 0xFFFFFFFDu, 0xFFFFFFFBu}) {
    indexes.push_back(i);
  }
  int c = 0;
  for (uint32_t index : indexes) {
    const uint32_t at =
        block.address + 4 * h.random.Below((block.bytes - race_setup::kRouteBytes) / 4);
    const uint64_t high = c % 3 == 2 ? (h.random.Next() << 32) : 0;
    const uint64_t index_high = h.random.Next() << 32;
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, frame - kBelow, (h.data + kScratchBytes - (frame - kBelow)) / 4, random);
      WriteRandom(h.base, block.address, block.bytes / 4, random);
      after = random;
      PPCContext t = CallContext(h);
      t.r3.u64 = high | at;
      t.r4.u64 = index_high | index;
      ITC_FLAG(native_race_setup) = v == 1;
      (v == 0 ? GuestFunction(__imp__sub_820FFF50) : hooks::RaceSetupCourseRoutesGet)(t, h.base);
      ITC_FLAG(native_race_setup) = true;
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, frame + 4, kSaves - 4));
      out[v].Bytes(Read(h.base, r1, h.data - r1));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r1.u32);
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}: index {}, out +{:X}", c, int32_t(index),
                                              at - block.address));
    c++;
  }
  results.Report();
}

// (`compare_routes` false: CourseRoutes::Get's frame not compared, for
// callers whose callees' frames, register saves and scratch not written
// natively, lie over it.)
// A function copying a course route through its own frame (Route::Set,
// Route::Reset, Race::StartPosition and their callers): the frames below the
// call's r1 (`frames`, outermost first: each one's bytes and the bytes of
// register saves at its top, and CourseRoutes::Get's under the last) filled
// with random words as CourseRoutes' cases do; compared: the block,
// CourseRoutes::Get's frame from +4 to its register saves (+4328), each
// frame from +4 to its saves, the caller's frame from r1 to the scratch
// area, the scratch area, the `globals` (set by `setup`, put back after), r3
// and r1. The back chains aren't compared (not written natively).
struct Frame {
  uint32_t bytes, saves;
};

template <typename Setup, typename Args>
void FramesCase(Context& h, Results& results, const Block& block, const std::vector<Frame>& frames,
                const std::vector<uint32_t>& globals, Setup setup, Args args,
                GuestFunction recompiled, GuestFunction native,
                const std::vector<uint32_t>& floats, const std::string& what,
                bool compare_routes = true, bool compare_f1 = true,
                const std::vector<const Block*>& extra = {},
                const std::function<void(int)>& after_run = nullptr) {
  const uint32_t r1 = CallContext(h).r1.u32;
  uint32_t routes = r1;
  for (const Frame& frame : frames) {
    routes -= frame.bytes;
  }
  routes -= race_setup::kRoutesFrameBytes;
  constexpr uint32_t kBelow = 1024;
  constexpr uint32_t kSaves = race_setup::kRoutesFrameBytes - 152;
  std::vector<uint32_t> saved;
  for (uint32_t global : globals) {
    saved.push_back(guest::LoadU32(h.base, global));
  }
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, routes - kBelow, (h.data + kScratchBytes - (routes - kBelow)) / 4,
                random);
    WriteRandom(h.base, block.address, block.bytes / 4, random);
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_race_setup) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_setup) = true;
    if (after_run) {
      after_run(v);
    }
    Record(out[v], h.base, block, floats);
    if (compare_routes) {
      out[v].Bytes(Read(h.base, routes + 4, kSaves - 4));
    }
    uint32_t top = r1;
    for (const Frame& frame : frames) {
      out[v].Bytes(Read(h.base, top - frame.bytes + 4, frame.bytes - 4 - frame.saves));
      top -= frame.bytes;
    }
    out[v].Bytes(Read(h.base, r1, h.data - r1));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    for (const Block* more : extra) {
      out[v].Bytes(Read(h.base, more->address, more->bytes));
    }
    for (size_t i = 0; i < globals.size(); i++) {
      out[v].Word(guest::LoadU32(h.base, globals[i]));
      guest::StoreU32(h.base, globals[i], saved[i]);
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Word(t.r1.u32);
    if (compare_f1) {
      out[v].Double(t.f1.u64);
    }
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

template <typename Args>
void RouteCase(Context& h, Results& results, const Block& block, uint32_t frame_bytes,
               uint32_t saves, Args args, GuestFunction recompiled, GuestFunction native,
               const std::string& what) {
  FramesCase(
      h, results, block, {{frame_bytes, saves}}, {}, [](Random&) {}, args, recompiled, native,
      {}, what);
}

// Route::Set (every course route twice, location 48 now and then, r4's high
// word random), Route::Reset and Race::StartPosition (every location from -2
// to 50, some random, r3's high word random now and then).
void Routes(Context& h, const Block& block) {
  {
    Results results("Route::Set (sub_82101A10)");
    for (int c = 0; c < 46; c++) {
      const uint32_t index = uint32_t(c / 2);
      const uint32_t at = block.address + 4 * h.random.Below((block.bytes - 16) / 4);
      const uint32_t location = c % 3 == 0 ? 48u
                                : c % 3 == 1 ? h.random.Below(50)
                                             : uint32_t(h.random.Next());
      const uint64_t high = c % 4 == 3 ? (h.random.Next() << 32) : 0;
      RouteCase(
          h, results, block, race_setup::kRouteFrameBytes, 16,
          [&](PPCContext& t) {
            t.r3.u64 = at;
            t.r4.u64 = high | index;
            t.r5.u64 = high | location;
          },
          __imp__sub_82101A10, hooks::RaceSetupRouteSet,
          fmt::format("case {}: route {}, location {}, at +{:X}", c, index, int32_t(location),
                      at - block.address));
    }
    results.Report();
  }
  {
    Results results("Route::Reset (sub_82101990)");
    for (int c = 0; c < 16; c++) {
      const uint32_t at = block.address + 4 * h.random.Below((block.bytes - 16) / 4);
      RouteCase(
          h, results, block, race_setup::kRouteFrameBytes, 16,
          [&](PPCContext& t) { t.r3.u64 = at; }, __imp__sub_82101990,
          hooks::RaceSetupRouteReset, fmt::format("case {}: at +{:X}", c, at - block.address));
    }
    results.Report();
  }
  {
    Results results("Race::StartPosition (sub_82101A98)");
    for (int c = 0; c < 61; c++) {
      const uint32_t location =
          c < 53 ? uint32_t(c - 2) : uint32_t(h.random.Next());
      const uint64_t high = c % 5 == 4 ? (h.random.Next() << 32) : 0;
      RouteCase(
          h, results, block, race_setup::kStartFrameBytes, 8,
          [&](PPCContext& t) { t.r3.u64 = high | location; }, __imp__sub_82101A98,
          hooks::RaceSetupStartPosition,
          fmt::format("case {}: location {}", c, int32_t(location)));
    }
    results.Report();
  }
}

// Race::SetRoute (the race data's global pointed so its +4 is in the block),
// Stages::SetRoute (a stage list at the block's start, the stage -1 or 0 to
// 9) and sub_82101F68 (a record in `records`; Route::Reset's frame under its
// own: the top 24 bytes of Route::Reset's are register saves, Route::Reset's
// own and sub_82101EB0's r30 left there by the original's calls).
void RouteCallers(Context& h, const Block& block, const Block& records) {
  {
    Results results("Race::SetRoute (sub_82101D38)");
    for (int c = 0; c < 46; c++) {
      const uint32_t index = uint32_t(c / 2);
      const uint32_t at = block.address + 4 * h.random.Below((block.bytes - 16) / 4);
      const uint32_t location = c % 2 == 0 ? 48u : h.random.Below(50);
      FramesCase(
          h, results, block, {{race_setup::kRouteFrameBytes, 16}}, {race_setup::kRaceData},
          [&](Random&) { guest::StoreU32(h.base, race_setup::kRaceData, at - 4); },
          [&](PPCContext& t) {
            t.r3.u64 = index;
            t.r4.u64 = location;
          },
          __imp__sub_82101D38, hooks::RaceSetupRaceSetRoute, {},
          fmt::format("case {}: route {}, location {}", c, index, location));
    }
    results.Report();
  }
  {
    Results results("Stages::SetRoute (sub_82101E90)");
    for (int c = 0; c < 46; c++) {
      const uint32_t index = uint32_t(c / 2);
      const uint32_t location = c % 3 == 0 ? 48u : h.random.Below(50);
      const uint32_t stage = c % 2 == 0 ? 0xFFFFFFFFu : h.random.Below(10);
      FramesCase(
          h, results, block, {{race_setup::kRouteFrameBytes, 16}}, {},
          [&](Random& r) { guest::StoreU32(h.base, block.address + 804, r.Below(10)); },
          [&](PPCContext& t) {
            t.r3.u64 = block.address;
            t.r4.u64 = index;
            t.r5.u64 = location;
            t.r6.u64 = stage;
          },
          __imp__sub_82101E90, hooks::RaceSetupStagesSetRoute, {},
          fmt::format("case {}: route {}, location {}, stage {}", c, index, location,
                      int32_t(stage)));
    }
    results.Report();
  }
  {
    Results results("sub_82101F68");
    for (int c = 0; c < 16; c++) {
      const uint32_t at = records.address + 4 * h.random.Below((records.bytes - 3936) / 4);
      FramesCase(
          h, results, records, {{128, 48}, {race_setup::kRouteFrameBytes, 24}}, {},
          [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; }, __imp__sub_82101F68,
          hooks::RaceSetupRecordReset, {},
          fmt::format("case {}: at +{:X}", c, at - records.address));
    }
    results.Report();
  }
}

// Racers::Reset, Racers::Add and Race::AddRacer on a block as big as the
// racer records (93 records and the counts): the count 0 to 92, the car
// object another record (its id 0 to 71, or -1, 72 or random now and then),
// f1 a random float's value (its +3920 compared as a float).
void Racers(Context& h) {
  const Block records(0x59600);
  if (!records.address) {
    ITC_LOG_ERROR("harness: race_setup: no block for the racer records");
    return;
  }
  {
    Results results("Racers::Reset (sub_820FF978)");
    for (int c = 0; c < 3; c++) {
      GlobalsCase(
          h, results, records, {}, [](Random&) {},
          [&](PPCContext& t) { t.r3.u64 = records.address; }, __imp__sub_820FF978,
          hooks::RaceSetupRacersReset, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  Results add("Racers::Add (sub_820FF860)");
  Results add_racer("Race::AddRacer (sub_821017C0)");
  for (int c = 0; c < 64; c++) {
    const uint32_t count = h.random.Below(93);
    const uint32_t other = (count + 1 + h.random.Below(92)) % 93;
    const uint32_t car = records.address + race_setup::kRacerRecordBytes * other;
    const uint32_t id = c % 8 == 0   ? 0xFFFFFFFFu
                        : c % 8 == 1 ? 72u
                        : c % 16 == 2 ? uint32_t(h.random.Next())
                                      : h.random.Below(72);
    const uint32_t value = RandomFloat(h.random);
    const uint64_t word = h.random.Next();
    const uint32_t added = c % 4 == 3 ? uint32_t(h.random.Next()) : h.random.Below(3);
    const uint64_t flag = c % 3 == 0 ? 0 : c % 3 == 1 ? 1 : h.random.Next();
    const std::vector<uint32_t> floats = {race_setup::kRacerRecordBytes * count + 3920};
    auto setup = [&](Random&) {
      guest::StoreU32(h.base, records.address + race_setup::kRacersCount, count);
      guest::StoreU32(h.base, records.address + race_setup::kRacersCount + 4, added);
      guest::StoreU32(h.base, car + 12, id);
    };
    GlobalsCase(
        h, add, records, {}, setup,
        [&](PPCContext& t) {
          t.r3.u64 = records.address;
          t.r4.u64 = car;
          t.f1.f64 = double(std::bit_cast<float>(value));
          t.r6.u64 = word;
        },
        __imp__sub_820FF860, hooks::RaceSetupRacersAdd, floats,
        fmt::format("case {}: count {}, car {}, id {}", c, count, other, int32_t(id)));
    GlobalsCase(
        h, add_racer, records, {race_setup::kRaceData},
        [&](Random& r) {
          setup(r);
          guest::StoreU32(h.base, race_setup::kRaceData, records.address - 48);
        },
        [&](PPCContext& t) {
          t.r3.u64 = car;
          t.r5.u64 = flag;
          t.f1.f64 = double(std::bit_cast<float>(value));
          t.r6.u64 = word;
        },
        __imp__sub_821017C0, hooks::RaceSetupAddRacer, floats,
        fmt::format("case {}: count {}, car {}, id {}, added {}", c, count, other, int32_t(id),
                    int32_t(added)));
  }
  add.Report();
  add_racer.Report();
}

// Made-up parameters for the traffic's cars (ids 55 to 70): 16 specs of
// random words, the floats Car::SetModel reads (+364, +372 to +416) and the
// tires' (front and rear, 21 each: +44, +48, +64, +80, +84) normal values,
// the tire types (+888, +896) 0 to 4 and grades (+884, +892) 1 to 5 as
// Car::Refresh's tables expect; the wheels' and aero parts' tables pointed at
// the specs (Car::SetModel leaves the wheel numbers and aero levels 0, so
// Car::Refresh reads neither).
struct MadeUpSpecs {
  explicit MadeUpSpecs(Context& h, uint32_t first = 55, uint32_t count = 16)
      : block(64 + count * car_specs::kSpecBytes) {
    if (!block.address) {
      return;
    }
    params = block.address;
    const uint32_t specs = params + 64;
    WriteRandom(h.base, block.address, block.bytes / 4, h.random);
    guest::StoreU32(h.base, params + 8, specs);
    guest::StoreU32(h.base, params + 12, specs - car_specs::kSpecBytes * first);
    for (uint32_t k = 0; k < 9; k++) {
      guest::StoreU32(h.base, params + 16 + 4 * k, specs);
    }
    auto normal = [&](float typical) {
      return std::bit_cast<uint32_t>(typical * (0.5f + float(h.random.Below(1000)) / 1000.0f));
    };
    for (uint32_t n = 0; n < count; n++) {
      const uint32_t spec = specs + car_specs::kSpecBytes * n;
      guest::StoreU32(h.base, spec + 364, normal(0.5f));
      for (uint32_t offset = 372; offset <= 416; offset += 4) {
        guest::StoreU32(h.base, spec + offset, normal(3.0f));
      }
      guest::StoreU32(h.base, spec + 884, 1 + h.random.Below(5));
      guest::StoreU32(h.base, spec + 888, h.random.Below(5));
      guest::StoreU32(h.base, spec + 892, 1 + h.random.Below(5));
      guest::StoreU32(h.base, spec + 896, h.random.Below(5));
      for (uint32_t t = 0; t < 42; t++) {
        const uint32_t tire = spec + 4244 + 92 * t;
        guest::StoreU32(h.base, tire + 44, normal(0.3f));
        guest::StoreU32(h.base, tire + 48, normal(0.2f));
        guest::StoreU32(h.base, tire + 64, normal(1.0f));
        guest::StoreU32(h.base, tire + 80, normal(16.0f));
        guest::StoreU32(h.base, tire + 84, normal(0.6f));
      }
    }
  }
  Block block;
  uint32_t params = 0;
};

// The racer records at `records` for Racers::SetStart: the traffic's flag
// (+0x595E8) 0 or not (when the parameters are there: the traffic's cars are
// built from made-up specs), the count (at most 77 with the traffic), the
// records' flags 0x1, 0x2, 0x80, 0 or random.
void WriteStartRecords(uint8_t* base, uint32_t records, Random& r, bool params) {
  const bool traffic = params && r.Below(2) == 0;
  guest::StoreU32(base, records + race_setup::kRacersCount + 8,
                  traffic ? (r.Below(4) == 0 ? uint32_t(r.Next()) | 1 : 1) : 0);
  const uint32_t count = traffic ? r.Below(78) : r.Below(94);
  guest::StoreU32(base, records + race_setup::kRacersCount, count);
  for (uint32_t k = 0; k < 93; k++) {
    static constexpr uint32_t kFlags[] = {0, 1, 2, 0x80, 3, 0x82};
    guest::StoreU32(base, records + race_setup::kRacerRecordBytes * k + 3888,
                    r.Below(7) == 0 ? uint32_t(r.Next()) : kFlags[r.Below(6)]);
  }
}

// Racers::SetStart (the records in a block as big as them) and
// Race::PlaceRacers and RaceData::Reset and RaceData::ResetGlobal (a block as
// big as the race data, the global pointed at it). The rand seed set alike
// on both sides (and compared). Below the functions' frames not compared:
// their callees' frames, where the originals leave register saves the plain
// C++ doesn't write (Race::StartPosition's and the routes' copies are
// compared in their own cases).
void Start(Context& h) {
  const MadeUpSpecs specs(h);
  const bool params = specs.params != 0;
  if (!params) {
    ITC_LOG_ERROR("harness: race_setup: no block for the made-up parameters");
  }
  {
    const Block records(0x59600);
    if (!records.address) {
      ITC_LOG_ERROR("harness: race_setup: no block for the racer records");
      return;
    }
    Results results("Racers::SetStart (sub_82101D50)");
    for (int c = 0; c < 48; c++) {
      const uint32_t location = c % 6 == 5 ? uint32_t(h.random.Next()) : h.random.Below(53) - 2;
      const uint32_t type = c % 5 == 4 ? uint32_t(h.random.Next()) : h.random.Below(6);
      FramesCase(
          h, results, records, {{4096, 80}}, {race_setup::kRandSeed, car_specs::kParams},
          [&](Random& r) {
            guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(r.Next()));
            guest::StoreU32(h.base, car_specs::kParams, specs.params);
            WriteStartRecords(h.base, records.address, r, params);
          },
          [&](PPCContext& t) {
            t.r3.u64 = records.address;
            t.r4.u64 = location;
            t.r5.u64 = type;
          },
          __imp__sub_82101D50, hooks::RaceSetupSetStart, {},
          fmt::format("case {}: location {}, type {}", c, int32_t(location), int32_t(type)),
          false);
    }
    results.Report();
  }
  const Block data(0xA4000);
  if (!data.address) {
    ITC_LOG_ERROR("harness: race_setup: no block for the race data");
    return;
  }
  {
    Results results("Race::PlaceRacers (sub_82102048)");
    for (int c = 0; c < 16; c++) {
      static constexpr uint32_t kKinds[] = {2, 3, 1, 5, 0, 4};
      const uint32_t kind = c % 7 == 6 ? uint32_t(h.random.Next()) : kKinds[c % 6];
      FramesCase(
          h, results, data, {{128, 32}, {4096, 80}},
          {race_setup::kRandSeed, race_setup::kRaceData, car_specs::kParams},
          [&](Random& r) {
            guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(r.Next()));
            guest::StoreU32(h.base, car_specs::kParams, specs.params);
            guest::StoreU32(h.base, race_setup::kRaceData, data.address);
            WriteStartRecords(h.base, data.address + 48, r, params);
            guest::StoreU32(h.base, data.address + 0xA3AF4, kind);
            guest::StoreU32(h.base, data.address + 8,
                            r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(53) - 2);
            const uint32_t list = data.address + race_setup::kStageList;
            guest::StoreU32(h.base, list + 804, r.Below(10));
            for (uint32_t k = 0; k < 10; k++) {
              guest::StoreU32(h.base, list + 80 * k, r.Below(6));
            }
          },
          [](PPCContext&) {}, __imp__sub_82102048, hooks::RaceSetupPlaceRacers, {},
          fmt::format("case {}: kind {}", c, int32_t(kind)), false);
    }
    results.Report();
  }
  for (int global = 0; global < 2; global++) {
    Results results(global ? "RaceData::ResetGlobal (sub_821021A8)"
                           : "RaceData::Reset (sub_821020F0)");
    for (int c = 0; c < 3; c++) {
      GlobalsCase(
          h, results, data, {race_setup::kRaceData},
          [&](Random&) { guest::StoreU32(h.base, race_setup::kRaceData, data.address); },
          [&](PPCContext& t) { t.r3.u64 = 0x1234567800000000ull * uint64_t(c) + 0x9ABC; },
          global ? GuestFunction(__imp__sub_821021A8) : GuestFunction(__imp__sub_821020F0),
          global ? hooks::RaceSetupRaceDataResetGlobal : hooks::RaceSetupRaceDataReset, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The made-up items for Race::SetUpRival and Race::SetUp (see Rivals): the
// tables object at `items` (its +16 the items, from +64).
uint32_t NormalFloat(Random& r) {
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(20001)) - 10000) / 1000.0f);
}

void WriteItems(uint8_t* base, uint32_t items, Random& r, uint32_t rival, uint32_t kind) {
  guest::StoreU32(base, items + 16, items + 64);
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t item = items + 64 + 3008 * k;
    for (uint32_t w = 0; w < 3008 / 4; w++) {
      guest::StoreU32(base, item + 4 * w, NormalFloat(r));
    }
    for (uint32_t offset = 160 + 120; offset <= 160 + 268; offset += 4) {
      guest::StoreU32(base, item + offset, 0);
    }
    guest::StoreU32(base, item + 160 + 2788, 55 + r.Below(16));
    guest::StoreU16(base, item + 0, uint16_t(r.Below(8) == 0 ? r.Next() : k));
    guest::StoreU8(base, item + 36, uint8_t(r.Below(5) == 0 ? r.Next() : r.Below(10)));
    guest::StoreU8(base, item + 37, uint8_t(r.Next()));
    guest::StoreU8(base, item + 90, uint8_t(r.Below(4) == 0 ? r.Next() : r.Below(26)));
    guest::StoreU8(base, item + 91, uint8_t(r.Below(2)));
    guest::StoreU16(base, item + 88, uint16_t(k == rival ? kind : r.Below(7)));
    uint32_t id = k;
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t room = 7 - id;
      const bool end = room == 0 || r.Below(4) == 0;
      if (!end) {
        id += 1 + r.Below(room);
      }
      guest::StoreU16(base, item + 92 + 2 * i, end ? 0xFFFF : uint16_t(id));
      if (end) {
        id = 7;
      }
    }
  }
}

// Race::SetUpRival over made-up items (the data tables' global pointed at
// eight 3,008-byte items, ids 0 to 7) and the made-up specs: each item's
// words normal floats (its car build's floats, read and converted by
// Car::FromBuild, never NaN), its build's tuning words (+120 to +268) 0 (tire
// type 0, wheel 0, aero levels 0: in range of the made-up tables) and its car
// id one of the specs' (55 to 70); its id (+0), bytes +36 (the location's
// switch), +37, +90, the kind (+88: the rival's 0 to 6 or random) and the
// +92 list (ids above its own, increasing, or -1: the chains end). The race
// data's block compared whole, the frame of Race::SetUpRival (the ones below
// it not: its callees' register saves; the routes' copies are compared in
// their own cases), the rand seed alike. f1 compared only without `add`:
// with it, the original leaves the last CarDecal::FromBuild's float in f1 (a
// volatile register no caller reads), which Car::FromBuild's plain C++
// doesn't hand back (its own cases don't compare f1 either).
void Rivals(Context& h) {
  const MadeUpSpecs specs(h);
  const Block items(64 + 8 * 3008);
  const Block data(0xA4000);
  if (!specs.params || !items.address || !data.address) {
    ITC_LOG_ERROR("harness: race_setup: no blocks for Race::SetUpRival");
    return;
  }
  Results results("Race::SetUpRival (sub_82112DB0)");
  for (int c = 0; c < 96; c++) {
    const uint32_t rival = h.random.Below(4);
    const uint32_t kind = c % 16 == 15 ? uint32_t(h.random.Next()) : uint32_t(c % 7);
    const uint64_t location = c % 3 == 0 ? 48 : h.random.Below(50);
    const uint64_t add = c % 2 == 0 ? 1 : c % 4 == 1 ? 0 : (h.random.Next() << 32);
    FramesCase(
        h, results, data, {{4112, 88}},
        {race_setup::kRandSeed, race_setup::kRaceData, car_specs::kParams, data_tables::kTables},
        [&](Random& r) {
          guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(r.Next()));
          guest::StoreU32(h.base, race_setup::kRaceData, data.address);
          guest::StoreU32(h.base, car_specs::kParams, specs.params);
          guest::StoreU32(h.base, data_tables::kTables, items.address);
          WriteItems(h.base, items.address, r, rival, kind);
          const uint32_t records = data.address + 48;
          const uint32_t count = r.Below(21);
          guest::StoreU32(h.base, records + race_setup::kRacersCount, count);
          for (uint32_t k = 0; k < count + 2; k++) {
            guest::StoreU32(h.base, records + race_setup::kRacerRecordBytes * k + 3912,
                            r.Below(10));
          }
          const uint32_t list = data.address + race_setup::kStageList;
          guest::StoreU32(h.base, list + 800, r.Below(3));
          guest::StoreU32(h.base, list + 804, r.Below(3));
          for (uint32_t k = 0; k < 10; k++) {
            guest::StoreU32(h.base, list + 80 * k + 76, r.Below(3));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = location;
          t.r4.u64 = rival;
          t.r5.u64 = add;
        },
        __imp__sub_82112DB0, hooks::RaceSetupSetUpRival, {},
        fmt::format("case {}: rival {}, kind {}, location {}, add {:X}", c, rival, int32_t(kind),
                    location, add),
        false, int32_t(add) == 0);
  }
  results.Report();
}

// Race::SetUp over made-up data: the save data (the garage's list and
// cars: each car's id one of the made-up specs', its tuning words 0 and its
// float +1892 a normal value; the event 1, others, above 999 or random; the
// mode 0 to 3; the location 0 to 25 or 48; the counts by location, some at
// 9999), SaveData::Of's table pointed at it, the event (its entries' items the
// made-up ones), the rival -1 or an item, the made-up specs for ids 0 to 71
// (event 1's rival is car 11). The race data's and the save data's blocks
// compared whole, Race::SetUp's frame up to its register saves (the frames
// below it not: its callees' register saves; their copies are compared in
// their own cases), the rand seed alike. f1 isn't compared: the original
// leaves Car::FromBuild's or the garage car's float there (a volatile
// register no caller reads).
void SetUps(Context& h) {
  const MadeUpSpecs specs(h, 0, 72);
  const Block items(64 + 8 * 3008);
  const Block data(0xA4000);
  const Block save(0x20000);
  const Block event(0x200);
  if (!specs.params || !items.address || !data.address || !save.address || !event.address) {
    ITC_LOG_ERROR("harness: race_setup: no blocks for Race::SetUp");
    return;
  }
  constexpr uint32_t kSaveDataPointer = 0x826C5B58;
  Results results("Race::SetUp (sub_82115D98)");
  // Expected to differ: the traffic's racer records (the last 16 added, by
  // Racers::SetStart) hold the bytes of its stack record its callees
  // (RacerRecord::Init, Car::SetModel, Car::Refresh) never write (`holes`,
  // found by running them natively over two fills: such as the car
  // object's +34 and the record's last 8 bytes), copied from its frame as
  // the earlier calls left them: the originals' register saves there (Stack
  // leftovers), which the plain C++ doesn't write. Compared on their own,
  // left out of the case's other outputs.
  std::vector<uint32_t> holes;
  {
    const uint32_t params = guest::LoadU32(h.base, car_specs::kParams);
    const uint32_t seed = guest::LoadU32(h.base, race_setup::kRandSeed);
    guest::StoreU32(h.base, car_specs::kParams, specs.params);
    const Block record(race_setup::kRacerRecordBytes);
    std::vector<uint8_t> after[2];
    for (int f = 0; f < 2; f++) {
      guest::ForEachHostPart(h.base, record.address, record.bytes,
                             [f](uint8_t* host, uint32_t, uint32_t bytes) {
                               std::memset(host, f ? 0x55 : 0xAA, bytes);
                             });
      race_setup::RacerRecordInit(h.base, record.address);
      race_setup::SetModel(h.base, record.address, 55);
      uint32_t r3;
      car_specs::CarRefresh(h.base, record.address, &r3);
      after[f] = Read(h.base, record.address, record.bytes);
    }
    for (uint32_t i = 0; i < race_setup::kRacerRecordBytes; i++) {
      if (after[0][i] == 0xAA && after[1][i] == 0x55 && (i < 3904 || i >= 3912)) {
        holes.push_back(i);  // (+3904 to +3911 Racers::SetStart writes itself)
      }
    }
    guest::StoreU32(h.base, car_specs::kParams, params);
    guest::StoreU32(h.base, race_setup::kRandSeed, seed);
    ITC_LOG_INFO("harness: race_setup: Racers::SetStart's record leaves {} bytes unwritten",
                 holes.size());
  }
  Results leftovers("Race::SetUp, the traffic records' unwritten bytes (expected to differ)");
  for (int c = 0; c < 48; c++) {
    harness::Outputs tails[2];
    auto split = [&](int v) {
      const uint32_t records = data.address + 48;
      const uint32_t count = guest::LoadU32(h.base, records + race_setup::kRacersCount);
      for (uint32_t k = count >= 16 ? count - 16 : 0; k < count && k < 93; k++) {
        const uint32_t record = records + race_setup::kRacerRecordBytes * k;
        std::vector<uint8_t> bytes;
        for (uint32_t i : holes) {
          bytes.push_back(guest::LoadU8(h.base, record + i));
          guest::StoreU8(h.base, record + i, 0);
        }
        tails[v].Bytes(bytes);
      }
    };
    const uint64_t rival = c % 3 == 1 ? h.random.Below(4) : 0xFFFFFFFFFFFFFFFFull;
    const uint32_t kind = c % 16 == 15 ? uint32_t(h.random.Next()) : uint32_t(c % 7);
    FramesCase(
        h, results, data, {{11984, 120}},
        {race_setup::kRandSeed, race_setup::kRaceData, car_specs::kParams, data_tables::kTables,
         kSaveDataPointer, race_setup::kEvent},
        [&](Random& r) {
          guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(r.Next()));
          guest::StoreU32(h.base, race_setup::kRaceData, data.address);
          guest::StoreU32(h.base, car_specs::kParams, specs.params);
          guest::StoreU32(h.base, data_tables::kTables, items.address);
          guest::StoreU32(h.base, kSaveDataPointer, save.address);
          guest::StoreU32(h.base, race_setup::kEvent, event.address);
          WriteItems(h.base, items.address, r, uint32_t(rival), kind);
          // The save data.
          const uint32_t sd = save.address;
          WriteRandom(h.base, sd, save.bytes / 4, r);
          static constexpr uint32_t kEvents[] = {1, 1, 2, 5, 0, 1000};
          guest::StoreU32(h.base, sd + 6100,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : kEvents[r.Below(6)]);
          guest::StoreU32(h.base, sd + 6104, r.Below(4));
          guest::StoreU32(h.base, sd + 6108, r.Below(4) == 0 ? 48 : r.Below(26));
          guest::StoreU32(h.base, sd + 0x19920, r.Below(4));
          guest::StoreU16(h.base, sd + 6324, uint16_t(r.Below(50)));
          for (uint32_t k = 0; k < 50; k++) {
            guest::StoreU16(h.base, sd + 6120 + 4 * k, uint16_t(r.Below(50)));
            guest::StoreU16(h.base, sd + 6122 + 4 * k, uint16_t(r.Below(20)));
          }
          for (uint32_t k = 0; k < 20; k++) {
            const uint32_t car = sd + 6332 + 1968 * k;
            guest::StoreU32(h.base, car + 12, 55 + r.Below(16));
            for (uint32_t offset = 1656; offset <= 1812; offset += 4) {
              guest::StoreU32(h.base, car + offset, 0);
            }
            guest::StoreU32(h.base, car + 1892, NormalFloat(r));
          }
          for (uint32_t k = 0; k < 26; k++) {
            guest::StoreU16(h.base, sd + 5886 + 2 * k,
                            uint16_t(r.Below(4) == 0 ? 9999 : r.Below(9999)));
          }
          // The event: up to six entries of the made-up items.
          WriteRandom(h.base, event.address, event.bytes / 4, r);
          guest::StoreU32(h.base, event.address + 156, event.address + 0x100);
          guest::StoreU16(h.base, event.address + 20, uint16_t(r.Below(7)));
          for (uint32_t k = 0; k < 6; k++) {
            const uint32_t entry = event.address + 0x100 + 12 * k;
            guest::StoreU32(h.base, entry + 0, r.Below(8));
            guest::StoreU32(h.base, entry + 8, r.Below(2));
          }
          // The race data's stage list and racer records as Race::SetUp's
          // reset leaves them doesn't matter (it resets them first).
        },
        [&](PPCContext& t) {
          t.r3.u64 = save.address;
          t.r4.u64 = rival;
        },
        __imp__sub_82115D98, hooks::RaceSetupSetUp, {},
        fmt::format("case {}: rival {}, kind {}", c, int32_t(rival), int32_t(kind)), false, false,
        {&save}, split);
    leftovers.Check(tails[0], tails[1], fmt::format("case {}", c));
  }
  results.Report();
  leftovers.Report();
}

// Car::SetupVehicle's cases: Vehicle::SetupFromCar's (src/harnesses/
// car_physics.cpp: the same inputs and what's compared) with the car in r3
// and the car data in r4, the original run with its callees' native flags
// off.
void SetupVehicles(Context& h) {
  const Block block(0x1800);
  if (!block.address) {
    ITC_LOG_ERROR("harness: race_setup: no block from the allocation layer");
    return;
  }
  const uint32_t data = block.address, car = block.address + 0x800;
  const uint32_t r1 = CallContext(h).r1.u32;
  if (!SetupFromCarFits(h.base, car, r1)) {
    Results skipped("Car::SetupVehicle (sub_820CD5C8): the copy or the table doesn't fit");
    skipped.Check(std::vector<uint8_t>{0}, std::vector<uint8_t>{1}, "copy");
    skipped.Report();
    return;
  }
  const char* const kGroupNames[] = {"", ", one NaN", ", NaNs and infinities anywhere"};
  for (int group = 0; group < 3; group++) {
    Results results(std::string("Car::SetupVehicle (sub_820CD5C8)") + kGroupNames[group]);
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        WriteSetupFromCarCase(h.base, data, car, r1, h.data + kScratchBytes, seed * 3 + 1000,
                              group);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = car;
        t.r4.u64 = data;
        if (v == 0) {
          ITC_FLAG(native_race_setup) = false;
          RunSetupFromCarOriginal(__imp__sub_820CD5C8, t, h.base);
          ITC_FLAG(native_race_setup) = true;
        } else {
          hooks::RaceSetupSetupVehicle(t, h.base);
        }
        RecordSetupFromCar(out[v], h.base, data, car, r1, h.data, t);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

void RaceSetup(Context& h) {
  const Block block(0x1000);
  if (!block.address) {
    ITC_LOG_ERROR("harness: race_setup: no block from the allocation layer");
    return;
  }
  Parts(h, block);
  Decals(h, block);
  const Block cars(0x2000);
  if (!cars.address) {
    ITC_LOG_ERROR("harness: race_setup: no block from the allocation layer");
    return;
  }
  SetModels(h, cars);
  FromBuilds(h, cars);
  const Block leaves(0x4000);
  if (!leaves.address) {
    ITC_LOG_ERROR("harness: race_setup: no block from the allocation layer");
    return;
  }
  Leaves(h, leaves);
  CourseRoutes(h, block);
  Routes(h, block);
  OnePointer(h, block, "Stages::Reset (sub_82101860)", __imp__sub_82101860,
             hooks::RaceSetupStagesReset, 1224);
  OnePointer(h, block, "Stages::Clear (sub_82101900)", __imp__sub_82101900,
             hooks::RaceSetupStagesClear, 1224);
  OnePointer(h, cars, "RacerRecord::Init (sub_820FF808)", __imp__sub_820FF808,
             hooks::RaceSetupRacerRecordInit, 3936);
  RouteCallers(h, block, cars);
  Racers(h);
  Start(h);
  Rivals(h);
  SetUps(h);
  RaceDataConstructs(h);
  SetupVehicles(h);
}

}  // namespace harness
