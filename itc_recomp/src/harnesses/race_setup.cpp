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
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_setup);

namespace harness {

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
  RaceDataConstructs(h);
}

}  // namespace harness
