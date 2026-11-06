// The car objects' makers (src/car_makers/, docs/functions/car-makers.md)
// against the recompiled functions. Each case fills a block of guest memory
// with random bytes, lays what the function reads there (a car object, a
// scene model record, a garage car), then runs the original and the
// register step (hooks/car_makers.h) from the same inputs and compares the
// block, the stack below the call (the frames' back chains and
// temporaries, the leaves' scratch; the top function's saved registers
// left out: the adapters don't save them), the argument area above it and
// r3. Floats are compared as floats where the originals compute or copy
// them with float instructions, everything else exactly. Most of these run
// in the front end, which attract races don't reach: the harness is their
// check.

#include "harness.h"

#include "hooks/car_makers.h"
#include "hooks/stand_models.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

// Vehicle::SetupFromCar's cases' writer (src/harnesses/car_physics.cpp).
bool SetupFromCarFits(uint8_t* base, uint32_t car, uint32_t r1);
void WriteSetupFromCarCase(uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                           uint32_t stack_end, uint64_t seed, int group);

namespace {

constexpr uint32_t kBlockBytes = 0x10000;
constexpr uint32_t kStackBytes = 2048;  // below the call's r1
constexpr uint32_t kArgBytes = 64;      // above it

// The layout in the block.
constexpr uint32_t kRecord = 0x0;     // a scene model record (4,244 bytes)
constexpr uint32_t kCar = 0x2000;     // a car object (3,936 bytes, block 2 at +1888)
constexpr uint32_t kBytes = 0x6000;   // the car's 20 bytes, or the choice's out
constexpr uint32_t kParams = 0x7000;  // a stand-in car parameters object
constexpr uint32_t kTables = 0x8000;  // its parts tables (to 0xCE00)
constexpr uint32_t kParamsPointer = 0x826C2E48;

// Absolute guest ranges compared as floats: (address, count).
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// Mostly a finite float a few hundred either way, now and then a NaN, an
// infinity, a zero, a denormal or one past the int range.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(14)) {
    case 0: return 0x7FC00000u | (r.Below(2) << 31) | r.Below(0x400000);
    case 1: return 0x7F800001u | (r.Below(2) << 31) | r.Below(0x3FFFFF);
    case 2: return 0x7F800000u | (r.Below(2) << 31);
    case 3: return r.Below(2) << 31;
    case 4: return r.Below(0x800000) | (r.Below(2) << 31);
    case 5: return std::bit_cast<uint32_t>(r.Below(2) ? 3.0e9f : -3.0e9f);
    default: {
      const float f = (float(r.Below(200000)) - 100000.0f) / 256.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

void AddRanges(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
               FloatRanges floats) {
  std::sort(floats.begin(), floats.end());
  uint32_t at = address;
  const uint32_t end = address + bytes;
  for (const auto& [start, count] : floats) {
    if (start < at || start + 4 * count > end) {
      continue;
    }
    if (start > at) {
      out.Bytes(Read(base, at, start - at));
    }
    out.Floats(base, start, count);
    at = start + 4 * count;
  }
  if (at < end) {
    out.Bytes(Read(base, at, end - at));
  }
}

// One case through both versions: the block filled at random, `setup(t)`
// laying the inputs and the registers (its inputs written back for the
// second run); the block, the stack below the call (the top `saves` bytes
// of the top function's frame left out) and the argument area above it
// compared, `floats` as floats, then r3; `skip` bytes of the stack below
// `skip_at` bytes under the call's r1 left out (the recompiled callees'
// saved registers).
// `more_skips` are more such (bytes under r1, size) pairs.
// `before` runs before each version (state outside the block put back),
// `extra` adds outputs after it; `stack_bytes` of the stack are compared.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, uint32_t saves, const FloatRanges& floats,
          const std::string& what, uint32_t skip_at = 0, uint32_t skip = 0,
          const std::function<void()>& before = {},
          const std::function<void(Outputs&, const PPCContext&)>& extra = {},
          uint32_t stack_bytes = kStackBytes, uint32_t block_bytes = kBlockBytes,
          const std::vector<std::pair<uint32_t, uint32_t>>& more_skips = {}) {
  RandomBytes(h.base, block, block_bytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, block_bytes);
  const uint32_t sp = start.r1.u32;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), block_bytes);
    Fill(h.base, sp - stack_bytes, stack_bytes + kArgBytes);
    if (before) {
      before();
    }
    PPCContext t = start;
    t.fpscr.disableFlushModeUnconditional();
    (v == 0 ? recompiled : native)(t, h.base);
    if (skip != 0) {
      Fill(h.base, sp - skip_at - skip, skip, 0);
    }
    for (const auto& [at, bytes] : more_skips) {
      Fill(h.base, sp - at - bytes, bytes, 0);
    }
    AddRanges(out[v], h.base, block, block_bytes, floats);
    out[v].Bytes(Read(h.base, sp - stack_bytes, stack_bytes - saves));
    out[v].Bytes(Read(h.base, sp, kArgBytes));
    if (extra) {
      extra(out[v], t);
    }
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// --- The record's setters ---

void BlockOf(Context& h, uint32_t block, Random& r) {
  Results results("CarModels::BlockOf (sub_8215AAD0)");
  for (int c = 0; c < 120; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          t.r3.u64 = c < 80 ? uint64_t(int64_t(c - 4)) : r.Next();
        },
        __imp__sub_8215AAD0, hooks::CarMakersBlockOf, 0, {}, fmt::format("case {}", c));
  }
  results.Report();
}

// A paint pick: -1 (the car's halfword +0, itself a small pick, -2 or any),
// -2, or a small index into the paint copy.
void SetPaint(Context& h, uint32_t block, Random& r) {
  Results results("SceneModelRecord::SetPaint (sub_8215AC70)");
  for (int c = 0; c < 300; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t record = block + kRecord, car = block + kCar;
          const uint32_t kinds[4] = {1, 2, 3, uint32_t(r.Next())};
          guest::StoreU32(h.base, record, kinds[r.Below(4)]);
          const uint16_t halves[4] = {uint16_t(r.Below(20)), 0xFFFE, 0xFFFF, uint16_t(r.Next())};
          guest::StoreU16(h.base, car + 0, halves[r.Below(r.Below(4) == 0 ? 4 : 3)]);
          const uint16_t sixes[4] = {0, 1, 2, uint16_t(r.Next())};
          guest::StoreU16(h.base, car + 6, sixes[r.Below(4)]);
          for (uint32_t at : {3284u, 3296u, 3332u, 3344u}) {
            guest::StoreU32(h.base, car + at, RandomFloat(r));
          }
          const uint64_t picks[4] = {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull,
                                     r.Below(20), (r.Next() << 32) | r.Below(20)};
          t.r3.u64 = record;
          t.r4.u64 = car;
          t.r5.u64 = picks[r.Below(4)];
          // The record's +0 also as a pick of -1 when the car's halfword
          // isn't a small one: keep it small then.
          if (int32_t(t.r5.u32) == -1 && guest::LoadU16(h.base, car) >= 20 &&
              guest::LoadU16(h.base, car) < 0xFFFE) {
            guest::StoreU16(h.base, car, uint16_t(r.Below(20)));
          }
        },
        __imp__sub_8215AC70, hooks::CarMakersSetPaint, 0,
        {{block + kRecord + 32, 36}, {block + kRecord + 4056, 2}, {block + kRecord + 4072, 2}},
        fmt::format("case {}", c));
  }
  results.Report();
}

void SetWheels(Context& h, uint32_t block, Random& r) {
  Results results("SceneModelRecord::SetWheels (sub_8215B238)");
  for (int c = 0; c < 200; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t car = block + kCar;
          guest::StoreU32(h.base, car + 1888 + 236, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
          for (uint32_t at : {3132u, 3164u, 3168u, 3224u, 3256u, 3260u}) {
            guest::StoreU32(h.base, car + at, RandomFloat(r));
          }
          t.r3.u64 = block + kRecord;
          t.r4.u64 = car;
        },
        __imp__sub_8215B238, hooks::CarMakersSetWheels, 24,
        {{block + kRecord + 4084, 2}, {block + kRecord + 4096, 2}}, fmt::format("case {}", c));
  }
  results.Report();
}

void SetChoice(Context& h, uint32_t block, Random& r) {
  Results results("SceneModelRecord::SetChoice (sub_8215B1A0)");
  for (int c = 0; c < 200; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t as[5] = {0, 1, 2, 3, uint32_t(r.Next())};
          const uint32_t bs[5] = {0, 1, 2, 3, uint32_t(r.Next())};
          const uint32_t cs[3] = {0, 1 + r.Below(4), uint32_t(r.Next())};
          t.r3.u64 = block + kBytes + 4 * r.Below(4);
          t.r4.u64 = (uint64_t(as[r.Below(5)]) << 32) | bs[r.Below(5)];
          t.r5.u64 = (uint64_t(cs[r.Below(3)]) << 32) | uint32_t(r.Next());
        },
        __imp__sub_8215B1A0, hooks::CarMakersSetChoice, 0, {}, fmt::format("case {}", c));
  }
  results.Report();
}

void SetBytes(Context& h, uint32_t block, Random& r) {
  Results results("CarRecord::SetBytes (sub_8215B0E0)");
  for (int c = 0; c < 100; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          t.r3.u64 = block + kRecord + 16;
          t.r4.u64 = block + kBytes;
        },
        __imp__sub_8215B0E0, hooks::CarMakersSetBytes, 32, {}, fmt::format("case {}", c));
  }
  results.Report();
}

// FromCar: the original calls the original setters (no adapters), whose
// saved registers (SetWheels' and SetBytes', the 32 bytes under its
// 256-byte frame, over SetPaint's scratch there) the plain C++ doesn't
// write: those 32 bytes are left out.
// The record's floats FromCar writes: SetPaint's and SetWheels', the
// decals' (+16, +20, +32, +48, +52 of each 64-byte entry) and pool 4's.
FloatRanges RecordFloats(uint32_t record) {
  FloatRanges floats = {{record + 32, 36},  {record + 4056, 2}, {record + 4072, 2},
                        {record + 4084, 2}, {record + 4096, 2}, {record + 4200, 3},
                        {record + 4216, 3}, {record + 4232, 3}};
  for (uint32_t e = 0; e < 46; e++) {
    const uint32_t r = record + 192 + 64 * e;
    floats.push_back({r + 16, 2});
    floats.push_back({r + 32, 1});
    floats.push_back({r + 48, 2});
  }
  return floats;
}

// A car object for FromCar: its flags (a player's, an opponent's, traffic's
// or any), a car id 1 to 71, the parts' and cockpit's words at their edges,
// the aero kit 0 to 2, block 2's +232, the tire grades 0 to 7, the doors.
void LayCar(uint8_t* base, uint32_t car, Random& r) {
  const uint32_t flags[6] = {0x1, 0x80, 0x2, 0x0, 0x81, uint32_t(r.Next())};
  guest::StoreU32(base, car + 3888, flags[r.Below(6)]);
  guest::StoreU32(base, car + 12, 1 + r.Below(71));
  const uint32_t stages[5] = {0, 2, 3, 7, uint32_t(r.Next())};
  guest::StoreU32(base, car + 1656, stages[r.Below(5)]);
  guest::StoreU32(base, car + 1708, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(9) - 1);
  guest::StoreU32(base, car + 1888 + 232, r.Below(3) == 0 ? 1 : r.Below(2) == 0 ? 0 : 2);
  guest::StoreU32(base, car + 1756, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(20));
  guest::StoreU32(base, car + 1764, r.Below(3));
  guest::StoreU32(base, car + 1784, r.Below(3));
  guest::StoreU32(base, car + 1816, r.Below(3) == 0 ? 3 : uint32_t(r.Next()));
  guest::StoreU32(base, car + 1716, r.Below(8));
  guest::StoreU32(base, car + 1724, r.Below(8));
  guest::StoreU16(base, car + 4, r.Below(4) == 0 ? uint16_t(r.Next()) : uint16_t(r.Below(7)));
  // The interior's choices: {a, b, c} with a and b small, c 0 or not.
  for (uint32_t at : {1824u, 1836u, 1848u, 1860u, 1872u}) {
    guest::StoreU32(base, car + at, r.Below(4));
    guest::StoreU32(base, car + at + 4, r.Below(4));
    guest::StoreU32(base, car + at + 8, r.Below(2) == 0 ? 0 : 1 + r.Below(4));
  }
  // SetPaint's picks and SetWheels' floats.
  guest::StoreU16(base, car + 0, r.Below(4) == 0 ? 0xFFFE : uint16_t(r.Below(20)));
  guest::StoreU16(base, car + 6, uint16_t(r.Below(3)));
  for (uint32_t at : {3132u, 3164u, 3168u, 3224u, 3256u, 3260u}) {
    guest::StoreU32(base, car + at, RandomFloat(r));
  }
}

}  // namespace

// For other harnesses' worlds (race_update's RaceScene::RemakeCarModel): a
// car object FromCar can take at `car` (LayCar's), and the stand-in car
// parameters at `params` whose nine parts tables all start at `tables`
// (the caller fills 0x4E00 bytes there and points [0x826C2E48] at
// `params`).
void LayFromCarWorld(uint8_t* base, uint32_t car, uint32_t params, uint32_t tables, Random& r) {
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, params + 16 + 4 * i, tables);
  }
  LayCar(base, car, r);
}

namespace {

void FromCar(Context& h, uint32_t block, Random& r) {
  Results results("SceneModelRecord::FromCar (sub_8215B878)");
  // The car parameters [0x826C2E48] (not loaded yet when the harnesses
  // run): a stand-in in the block whose nine parts tables (+16 to +48) all
  // start at kTables, random bytes (car ids 1 to 71 and kits 1 and 2 stay
  // inside the block).
  const uint32_t params_global = guest::LoadU32(h.base, kParamsPointer);
  guest::StoreU32(h.base, kParamsPointer, block + kParams);
  for (int c = 0; c < 300; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t car = block + kCar;
          for (uint32_t i = 0; i < 9; i++) {
            guest::StoreU32(h.base, block + kParams + 16 + 4 * i, block + kTables);
          }
          LayCar(h.base, car, r);
          const uint64_t picks[4] = {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull,
                                     r.Below(20), (r.Next() << 32) | r.Below(20)};
          t.r3.u64 = block + kRecord;
          t.r4.u64 = car;
          t.r5.u64 = picks[r.Below(4)];
        },
        __imp__sub_8215B878, hooks::CarMakersFromCar, 80, RecordFloats(block + kRecord),
        fmt::format("case {}", c), 256, 32);
  }
  guest::StoreU32(h.base, kParamsPointer, params_global);
  results.Report();
}

// --- The garage cars ---

constexpr uint32_t kGarageBytes = 0x30000;
constexpr uint32_t kGarageCar = 0x4000;    // the template car (4,096 bytes; the garage car)
constexpr uint32_t kCarData = 0x5000;      // the car data (1,856 bytes)
constexpr uint32_t kWriterStack = 0x9000;  // the case writer's r1 (its stack below)
constexpr uint32_t kGarageParams = 0x9100; // the stand-in parameters (468 bytes)
constexpr uint32_t kSpec = 0xA000;         // one car spec (8,432 bytes)
constexpr uint32_t kHolder = 0xC400;       // the stand's holder
constexpr uint32_t kSaveData = 0xD000;     // a save data's garage (to +0x19924)
constexpr uint32_t kCarId = 5;
constexpr uint32_t kSpecBytes = 8432;
constexpr uint32_t kRaceDataPointer = 0x826C5B2C;
constexpr uint32_t kRaceDataBytes = 0x5A000;
constexpr uint32_t kHolderPointer = 0x826D83E0;

// (The case writer's seed is the number of steps its generator is wound on:
// kept small.)
// A garage car Car::Refresh and Vehicle::SetupFromCar can take: the
// SetupFromCar cases' car object (and car data) at kGarageCar, its id
// kCarId, the engine stage 0 to 3, the tires' types 0 to 4 and grades 1 to
// 5, no aero parts or wheels of their own; the car spec Car::Refresh copies
// from made of that car's blocks (so the refreshed car is the case's), the
// parameters [0x826C2E48] pointing at it.
void LayGarage(uint8_t* base, uint32_t block, Random& r, int c) {
  const uint32_t car = block + kGarageCar, spec = block + kSpec;
  WriteSetupFromCarCase(base, block + kCarData, car, block + kWriterStack,
                        block + kWriterStack, uint64_t(c) * 7 + 3, c % 2);
  guest::StoreU32(base, car + 12, kCarId);
  const uint32_t stage = r.Below(4);
  guest::StoreU32(base, car + 1656, stage);
  uint32_t tire[2];
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t type = r.Below(5), grade = 1 + r.Below(5);
    guest::StoreU32(base, car + 1712 + 8 * k, type);
    guest::StoreU32(base, car + 1716 + 8 * k, grade);
    tire[k] = (type == 0 ? 0 : type * 5 + grade - 5) * 92 + spec + (k == 0 ? 4244 : 6176);
  }
  for (uint32_t at : {1744u, 1748u, 1752u, 1756u, 1760u, 1764u, 1768u, 1780u, 1784u, 1728u,
                      1736u}) {
    guest::StoreU32(base, car + at, 0);
  }
  auto copy = [&](uint32_t to, uint32_t from, uint32_t bytes) {
    Write(base, to, Read(base, from, bytes).data(), bytes);
  };
  copy(spec, car + 2240, 640);
  copy(spec + 640, car + 1888, 340);
  copy(spec + 980 + 204 * stage, car + 2880, 204);
  copy(spec + 8108, car + 3364, 272);
  copy(tire[0], car + 3084, 92);
  copy(tire[1], car + 3176, 92);
  copy(spec + 8380, car + 3268, 48);
  const uint32_t params = block + kGarageParams;
  guest::StoreU32(base, params + 12, spec - kSpecBytes * kCarId);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, params + 16 + 4 * i, spec);
  }
}

}  // namespace

// For load_poll's GarageStage::SetCar and SetCarClass cases: LayGarage's car
// in a block of its own (`block`, 0x10000 bytes: the car at +0x4000, the
// writer's stack and its parameters, the spec), which FitCar's
// Vehicle::SetupFromCar takes, given what SceneModelRecord::FromCar writes
// into the record the re-applies then read, as their world has it: its
// flags (a player's, an opponent's or, for `traffic`, none: kind 3), the
// parts' words small (+1756 -1 now and then: 13), the interior's +100 (car
// +1800) small and its gauges clear (the choices' c 0), the selected
// texture's index (+1812) below 6, the decals' ids (E +24 of the six
// entries at +96 and the forty at +288) small or none, as the re-applies'
// cases make them. LayGarage leaves +1764 0 (the car record's +3692 never
// 1) and no aero kit. Returns the car.
uint32_t LayStandCar(uint8_t* base, uint32_t block, Random& r, int c, bool traffic) {
  LayGarage(base, block, r, c);
  const uint32_t car = block + kGarageCar;
  const uint32_t flags[4] = {0x1, 0x80, 0x2, 0x81};
  guest::StoreU32(base, car + 3888, traffic ? 0 : flags[r.Below(4)]);
  for (uint32_t at : {1664u, 1668u, 1744u, 1748u, 1752u, 1760u, 1768u, 1772u, 1776u, 1780u}) {
    guest::StoreU32(base, car + at, r.Below(3));
  }
  guest::StoreU32(base, car + 1756, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(3));
  guest::StoreU32(base, car + 1800, r.Below(2) == 0 ? 0 : r.Below(4));
  guest::StoreU32(base, car + 1812, r.Below(6));
  for (uint32_t at : {1824u, 1836u, 1848u, 1860u, 1872u}) {
    guest::StoreU32(base, car + at + 8, 0);
  }
  auto id = [&r](uint32_t below) {
    return r.Below(3) == 0 ? uint32_t(-1 - int32_t(r.Below(4))) : r.Below(below);
  };
  for (uint32_t e = 0; e < 6; e++) {
    guest::StoreU32(base, car + 96 + 32 * e + 24, id(26));
  }
  for (uint32_t e = 0; e < 40; e++) {
    guest::StoreU32(base, car + 288 + 32 * e + 24, id(300));
  }
  return car;
}

// For load_poll's GarageStage::ShowCarById and ShowCarByIdWithClass cases:
// LayGarage's spec and parameters in `block` (as LayStandCar's), which
// Car::SetModel and Car::Refresh build the frame's car from (its id kCarId),
// the spec's block 2 words SetModel copies into the car given what
// Vehicle::SetupFromCar and the re-applies take: the case's engine as stage
// 0's (+980; SetModel's CarTuning::Reset leaves the stage 0), the tuning
// words (+772 to
// +792) 0, the tires (+884 to +896) LayGarage's types and grades (their
// copies in the spec), the five choices' c (+900 + 8k) 0 (the interior's
// gauges clear); the stand's car list (`list`: its +1136 array, the entry
// +48 + +52 one whose +8432 is kCarId) and the holder's word (`holder_word`
// holding `holder`). The caller points [0x826C2E48] at `params`,
// [0x826D83E0] at `holder_word` and [0x826D83F4] at `list`.
// (The same definition is in load_poll.cpp.)
struct StandWorld {
  uint32_t params, holder_word, list;
};
StandWorld LayShowCarWorld(uint8_t* base, uint32_t block, Random& r, int c, uint32_t holder) {
  LayGarage(base, block, r, c);
  const uint32_t car = block + kGarageCar, spec = block + kSpec;
  // The engine stage SetModel leaves (0): the case's engine there too.
  Write(base, spec + 980, Read(base, car + 2880, 204).data(), 204);
  for (uint32_t at = 772; at <= 792; at += 4) {
    guest::StoreU32(base, spec + at, 0);
  }
  guest::StoreU32(base, spec + 884, guest::LoadU32(base, car + 1712));
  guest::StoreU32(base, spec + 888, guest::LoadU32(base, car + 1716));
  guest::StoreU32(base, spec + 892, guest::LoadU32(base, car + 1720));
  guest::StoreU32(base, spec + 896, guest::LoadU32(base, car + 1724));
  for (uint32_t k = 0; k < 5; k++) {
    guest::StoreU32(base, spec + 900 + 8 * k, 0);
    guest::StoreU32(base, spec + 904 + 8 * k, r.Below(9));
  }
  StandWorld world;
  world.params = block + kGarageParams;
  world.holder_word = block + 0xCF00;
  world.list = block + 0xD000;
  guest::StoreU32(base, world.holder_word, holder);
  const uint32_t array = block + 0xD800, entry = block + 0xC400;
  const uint32_t a = r.Below(4), b = r.Below(4);
  guest::StoreU32(base, world.list + 48, a);
  guest::StoreU32(base, world.list + 52, b);
  guest::StoreU32(base, world.list + 1136, array);
  guest::StoreU32(base, array + 4 * (a + b), entry);
  guest::StoreU32(base, entry + 8432, kCarId);
  return world;
}

namespace {

// The race data (its own block): zeroed, a few racers already added.
struct RaceData {
  explicit RaceData(uint8_t* base)
      : base(base),
        global(guest::LoadU32(base, kRaceDataPointer)),
        params(guest::LoadU32(base, kParamsPointer)),
        holder(guest::LoadU32(base, kHolderPointer)),
        allocation(memory::Alloc(kRaceDataBytes, 0)) {}
  ~RaceData() {
    guest::StoreU32(base, kRaceDataPointer, global);
    guest::StoreU32(base, kParamsPointer, params);
    guest::StoreU32(base, kHolderPointer, holder);
    memory::Free(allocation);
  }
  void Reset(uint32_t count, uint32_t added) const {
    Fill(base, allocation, kRaceDataBytes, 0);
    guest::StoreU32(base, allocation + 48 + 0x595E0, count);
    guest::StoreU32(base, allocation + 48 + 0x595E4, added);
    guest::StoreU32(base, kRaceDataPointer, allocation);
  }
  uint8_t* base;
  uint32_t global, params, holder, allocation;
};

void Garage(Context& h, Random& r) {
  const uint32_t allocation = memory::Alloc(kGarageBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  const RaceData race(h.base);
  {
    Results results("GarageCar::Reset (sub_82110448)");
    for (int c = 0; c < 60; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) { t.r3.u64 = block + kGarageCar + 4 * r.Below(4); },
          __imp__sub_82110448, hooks::CarMakersGarageCarReset, 16, {},
          fmt::format("case {}", c), 0, 0, {}, {}, kStackBytes, kGarageBytes);
    }
    results.Report();
  }
  {
    // The two static garage cars, put back afterwards.
    constexpr uint32_t kStatic = 0x82814EB8, kStaticBytes = 2 * 1972;
    const std::vector<uint8_t> saved = Read(h.base, kStatic, kStaticBytes);
    Results results("GarageCars::StaticInit (sub_825D9EF0)");
    for (int c = 0; c < 8; c++) {
      std::vector<uint8_t> inputs(kStaticBytes);
      for (uint8_t& b : inputs) {
        b = uint8_t(r.Next());
      }
      Case(
          h, results, block, r, [](PPCContext&) {}, __imp__sub_825D9EF0,
          hooks::CarMakersStaticInit, 24, {}, fmt::format("case {}", c), 0, 0,
          [&] { Write(h.base, kStatic, inputs.data(), kStaticBytes); },
          [&](Outputs& out, const PPCContext&) { out.Bytes(Read(h.base, kStatic, kStaticBytes)); },
          kStackBytes, kGarageBytes);
    }
    Write(h.base, kStatic, saved.data(), kStaticBytes);
    results.Report();
  }
  auto race_outputs = [&](Outputs& out, const PPCContext& t) {
    out.Bytes(Read(h.base, race.allocation, kRaceDataBytes));
    out.Double(t.f1.u64);
  };
  {
    Results results("CarData::FromGarageCar (sub_82179A60)");
    for (int c = 0; c < 40; c++) {
      uint32_t count = 0, added = 0;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            LayGarage(h.base, block, r, c);
            guest::StoreU32(h.base, kParamsPointer, block + kGarageParams);
            count = r.Below(6);
            added = r.Below(3);
            t.r3.u64 = r.Next();
            t.r4.u64 = block + kGarageCar;
            t.r5.u64 = block + kCarData;
          },
          __imp__sub_82179A60, hooks::CarMakersFromGarageCar, 24, {},
          fmt::format("case {}", c), 0, 0, [&] { race.Reset(count, added); }, race_outputs,
          0x2000, kGarageBytes);
    }
    results.Report();
  }
  {
    Results results("TuningScreen::SetUpCarData (sub_821DF2F8)");
    for (int c = 0; c < 40; c++) {
      uint32_t count = 0, added = 0;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            LayGarage(h.base, block, r, c);
            guest::StoreU32(h.base, kParamsPointer, block + kGarageParams);
            guest::StoreU32(h.base, kHolderPointer, block + kHolder);
            count = r.Below(6);
            added = r.Below(3);
            // The screen where its +18912 is the case's car data: its save
            // data's garage list (nodes at +6120: halfwords next, slot), the
            // walk's length.
            const uint32_t screen = block + kCarData - 18912, save = block + kSaveData, nodes = save + 6120;
            guest::StoreU32(h.base, screen + 18904, save);
            const uint32_t steps = r.Below(4);
            guest::StoreU32(h.base, save + 0x19920, steps);
            uint16_t at = uint16_t(r.Below(40));
            guest::StoreU16(h.base, nodes + 204, at);
            for (uint32_t i = 0; i <= steps; i++) {
              const uint16_t next = uint16_t(r.Below(40));
              guest::StoreU16(h.base, nodes + 4 * at, next);
              if (i == steps) {
                const uint32_t slot = r.Below(3);
                guest::StoreU16(h.base, nodes + 4 * at + 2, uint16_t(slot));
                const uint32_t car = nodes + 212 + 1968 * slot;
                Write(h.base, car, Read(h.base, block + kGarageCar, 1884).data(), 1884);
              }
              at = next;
            }
            t.r3.u64 = screen;
          },
          // The top 24 bytes: the original CarData::FromGarageCar's register
          // saves (its tail call's; its adapter is gone, so the original
          // runs under the original SetUpCarData), which the native one
          // doesn't write.
          __imp__sub_821DF2F8, hooks::CarMakersSetUpCarData, 24, {},
          fmt::format("case {}", c), 0, 0, [&] { race.Reset(count, added); }, race_outputs,
          0x2000, kGarageBytes);
    }
    results.Report();
  }
  memory::Free(allocation);
}

// --- The garage stage's makers ---

constexpr uint32_t kStage = 0x9800;      // the stage (6,208 bytes; to 0xB040)
constexpr uint32_t kCarScene = 0x10000;  // the car scene (pool 7's list at +5028)
constexpr uint32_t kModels = 0x12000;    // two scene models, 0x1400 apart
constexpr uint32_t kOtherRecord = 0x15000;  // a record to copy from
constexpr uint32_t kScenePointer = 0x826C2E44;

void Stand(Context& h, Random& r) {
  const uint32_t allocation = memory::Alloc(kGarageBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  {
    Results results("GarageStage::FitCar (sub_82178C40)");
    const uint32_t stage = block + kStage;
    for (int c = 0; c < 40; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            LayGarage(h.base, block, r, c);
            guest::StoreU32(h.base, block + kHolder, stage);
            t.r3.u64 = block + kHolder;
            t.r4.u64 = block + kGarageCar;
          },
          __imp__sub_82178C40, hooks::CarMakersFitCar, 32, {{stage + 6096, 28}},
          fmt::format("case {}", c), 0, 0, {},
          [](Outputs& out, const PPCContext& t) { out.Double(t.f1.u64); }, 0x2000,
          kGarageBytes);
    }
    results.Report();
  }
  // GarageStage::FitAxles (stand_models/wheels.cpp, stand-models.md): FitCar's
  // world; the stage's axle records (+6176 to +6204) as floats, f1.
  {
    Results results("GarageStage::FitAxles (sub_82178B98)");
    const uint32_t stage = block + kStage;
    for (int c = 0; c < 40; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            LayGarage(h.base, block, r, c);
            guest::StoreU32(h.base, block + kHolder, stage);
            t.r3.u64 = block + kHolder;
            t.r4.u64 = block + kGarageCar;
          },
          __imp__sub_82178B98, hooks::StandModelsFitAxles, 24, {{stage + 6176, 8}},
          fmt::format("case {}", c), 0, 0, {},
          [](Outputs& out, const PPCContext& t) { out.Double(t.f1.u64); }, 0x2000,
          kGarageBytes);
    }
    results.Report();
  }
  {
    // The car scene's pool 7 (two models in use), the stage's index 0 or 1;
    // the record the stage's model's own +32 (as SetCar and SetCarClass pass
    // it: the copy onto itself) or another. Where SceneModelRecord::Copy's
    // original runs (the copy onto itself), it saves r31, which the original
    // CopyRecord holds the record in and the plain C++ doesn't (register
    // saves aren't reproduced): those 8 bytes (r1 - 112) are left out.
    const uint32_t scene_global = guest::LoadU32(h.base, kScenePointer);
    Results results("GarageStage::CopyRecord (sub_8217FC60)");
    for (int c = 0; c < 60; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t scene = block + kCarScene, stage = block + kStage;
            guest::StoreU32(h.base, scene + 5028, block + kModels);
            for (uint32_t i = 0; i < 2; i++) {
              const uint32_t model = block + kModels + 0x1400 * i;
              guest::StoreU32(h.base, model + 8, i == 0 ? model + 0x1400 : 0);
              guest::StoreU32(h.base, model + 16, 3);
            }
            const uint32_t index = r.Below(2);
            guest::StoreU32(h.base, stage + 8, index);
            guest::StoreU32(h.base, block + kHolder, stage);
            t.r3.u64 = block + kHolder;
            t.r4.u64 = r.Below(2) == 0 ? block + kModels + 0x1400 * index + 32
                                       : block + kOtherRecord;
          },
          __imp__sub_8217FC60, hooks::CarMakersCopyRecord, 16, {}, fmt::format("case {}", c),
          104, 8, [&] { guest::StoreU32(h.base, kScenePointer, block + kCarScene); }, {},
          kStackBytes, kGarageBytes);
    }
    guest::StoreU32(h.base, kScenePointer, scene_global);
    results.Report();
  }
  {
    // Movie 6 in the block: its lists set up, its first 128 named
    // animations over a root scene and one of 8 animations of no tracks;
    // the movie table at the one pair {6, the movie} (put back after).
    constexpr uint32_t kMovie = 0x0, kRoot = 0xA000, kAnimations = 0xB000, kPairs = 0xB400;
    const std::vector<uint8_t> table = Read(h.base, ui_movies::kMovieTable, 12);
    for (int which = 0; which < 2; which++) {
      Results results(which == 0 ? "sub_8219ED10 (the busy marker shown)"
                                 : "sub_8219EDB0 (the busy marker hidden)");
      for (int c = 0; c < 30; c++) {
        Case(
            h, results, block, r,
            [&](PPCContext&) {
              const uint32_t movie = block + kMovie, root = block + kRoot;
              ui_movies::InitAnimationLists(h.base, movie);
              guest::StoreU32(h.base, movie + 16, root);
              guest::StoreU32(h.base, movie + 20, root);
              guest::StoreU32(h.base, movie + 24, root);
              for (uint32_t k = 0; k < 128; k++) {
                const uint32_t named = movie + 176 + 36 * k;
                guest::StoreU32(h.base, named + 28, root);
                guest::StoreU32(h.base, named + 32, block + kAnimations + 64 * r.Below(8));
              }
              for (uint32_t k = 0; k < 8; k++) {
                const uint32_t animation = block + kAnimations + 64 * k;
                for (uint32_t i = 0; i < 64; i += 4) {
                  guest::StoreU32(h.base, animation + i, 0);
                }
                guest::StoreFloat(h.base, animation + 12, float(r.Below(100)) / 4.0f);
              }
              guest::StoreU32(h.base, root + 100, 0);
              guest::StoreU32(h.base, root + 104, 4);
              guest::StoreU32(h.base, root + 108, r.Below(4));
              guest::StoreU32(h.base, root + 116, 70);
              for (uint32_t i = 0; i < 94; i++) {
                guest::StoreU32(h.base, root + 128 + 8 * i, block + kAnimations + 64 * r.Below(8));
              }
              // Some calls before: the marker already shown or hidden.
              guest::StoreU32(h.base, block + kPairs, 6);
              guest::StoreU32(h.base, block + kPairs + 4, movie);
            },
            which == 0 ? __imp__sub_8219ED10 : __imp__sub_8219EDB0,
            which == 0 ? hooks::CarMakersShowBusyMarker : hooks::CarMakersHideBusyMarker, 16,
            {}, fmt::format("case {}", c), 0, 0,
            [&] {
              guest::StoreU32(h.base, ui_movies::kMovieTable, block + kPairs);
              guest::StoreU32(h.base, ui_movies::kMovieTable + 4, 8);
              guest::StoreU32(h.base, ui_movies::kMovieTable + 8, 1);
            },
            {}, kStackBytes, kGarageBytes);
      }
      results.Report();
    }
    Write(h.base, ui_movies::kMovieTable, table.data(), 12);
  }
  memory::Free(allocation);
}


// --- The wheels picked (stand_models/wheels.cpp, stand-models.md) ---

constexpr uint32_t kScreenSave = 0x2000;      // the tuning screen's +18904
constexpr uint32_t kHolderWord = 0xC480;      // [0x826D83E0] points here
constexpr uint32_t kSceneWheels = 0x16000;    // a pool 5 wheels object (its +28 the next)
constexpr uint32_t kFirstWheels = 0x16400;    // the first set's wheels (+28 to +184)
constexpr uint32_t kWheelRecords = 0x18000;   // table 73: 178 48-byte wheel records
constexpr uint32_t kSaveDataPointer = 0x826C5B58;  // SaveData::Of(0)

// The save data's garage list (nodes at +6120: halfwords next, slot) walked
// [+0x19920] steps to a slot 0 to 2, the template garage car copied there.
void LayGarageList(uint8_t* base, uint32_t block, Random& r) {
  const uint32_t save = block + kSaveData, nodes = save + 6120;
  const uint32_t steps = r.Below(4);
  guest::StoreU32(base, save + 0x19920, steps);
  uint16_t at = uint16_t(r.Below(40));
  guest::StoreU16(base, nodes + 204, at);
  for (uint32_t i = 0; i <= steps; i++) {
    const uint16_t next = uint16_t(r.Below(40));
    guest::StoreU16(base, nodes + 4 * at, next);
    if (i == steps) {
      const uint32_t slot = r.Below(3);
      guest::StoreU16(base, nodes + 4 * at + 2, uint16_t(slot));
      Write(base, nodes + 212 + 1968 * slot, Read(base, block + kGarageCar, 1884).data(), 1884);
    }
    at = next;
  }
}

// SetWheel's world on LayGarage's: the holder (its +0 the stage, its +8
// the model's index 0 or 1 in the car scene's pool 7, both in use), each
// model's +5100 the pool 5 wheels object (its +28 the first set's wheels),
// table 73 at the parameters' +8 (its records' +16 and +28 floats, NaNs and
// infinities among them), the spec's stock record's too, SaveData::Of(0)'s
// garage list ending at the template car (the car spec's id kCarId).
void LayWheelWorld(uint8_t* base, uint32_t block, Random& r, int c, bool tires) {
  LayGarage(base, block, r, c);
  const uint32_t car = block + kGarageCar, spec = block + kSpec;
  if (tires) {
    for (uint32_t at : {3132u, 3164u, 3168u, 3224u, 3256u, 3260u}) {
      guest::StoreU32(base, car + at, RandomFloat(r));
    }
  }
  for (uint32_t at : {16u, 28u}) {
    const uint32_t f = RandomFloat(r);
    guest::StoreU32(base, spec + 8380 + at, f);
    guest::StoreU32(base, car + 3268 + at, f);
  }
  for (uint32_t i = 0; i < 178; i++) {
    for (uint32_t at : {16u, 28u}) {
      guest::StoreU32(base, block + kWheelRecords + 48 * i + at, RandomFloat(r));
    }
  }
  guest::StoreU32(base, block + kGarageParams + 8, block + kWheelRecords);
  guest::StoreU32(base, kParamsPointer, block + kGarageParams);
  const uint32_t scene = block + kCarScene, stage = block + kStage;
  guest::StoreU32(base, scene + 5028, block + kModels);
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t model = block + kModels + 0x1400 * i;
    guest::StoreU32(base, model + 8, i == 0 ? model + 0x1400 : 0);
    guest::StoreU32(base, model + 16, 3);
    guest::StoreU32(base, model + 5100, block + kSceneWheels);
  }
  guest::StoreU32(base, block + kSceneWheels + 28, block + kFirstWheels);
  guest::StoreU32(base, stage + 8, r.Below(2));
  guest::StoreU32(base, block + kHolder, stage);
  guest::StoreU32(base, block + kHolderWord, block + kHolder);
  LayGarageList(base, block, r);
  guest::StoreU32(base, block + kScreenSave, block + kSaveData);
}

// A wheel number: 0 (the stock wheel), 1 to 178, above 178 (clamped), now
// and then with any high word.
uint64_t RandomWheelNumber(Random& r) {
  uint64_t n;
  switch (r.Below(6)) {
    case 0: n = 0; break;
    case 1: {
      constexpr uint32_t kAbove[] = {179, 180, 255, 1000, 0x7FFFFFFFu};
      n = kAbove[r.Below(5)];
      break;
    }
    case 2: n = r.Below(2) == 0 ? 1 : 178; break;
    default: n = 1 + r.Below(178); break;
  }
  if (r.Below(4) == 0) {
    n |= r.Next() << 32;
  }
  return n;
}

// Which: 0, 1, 2 mostly, other values now and then.
uint64_t RandomWhich(Random& r) {
  constexpr uint32_t kOthers[] = {3, 7, 0xFFFFFFFFu, 0x80000000u};
  return r.Below(5) == 0 ? kOthers[r.Below(4)] : r.Below(3);
}

FloatRanges WheelFloats(uint32_t block) {
  FloatRanges floats = {{block + kStage + 6176, 8}};
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t rec = block + kModels + 0x1400 * i + 32;
    for (uint32_t at : {80u, 96u, 112u, 128u, 144u, 160u}) {
      floats.push_back({rec + at, 3});
    }
    for (uint32_t at : {4056u, 4072u, 4084u, 4096u}) {
      floats.push_back({rec + at, 2});
    }
  }
  return floats;
}

void Wheels(Context& h, Random& r) {
  const uint32_t allocation = memory::Alloc(kGarageBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  const uint32_t globals[4] = {kScenePointer, kParamsPointer, kHolderPointer, kSaveDataPointer};
  uint32_t saved[4];
  for (int i = 0; i < 4; i++) {
    saved[i] = guest::LoadU32(h.base, globals[i]);
  }
  const auto before = [&] {
    guest::StoreU32(h.base, kScenePointer, block + kCarScene);
    guest::StoreU32(h.base, kHolderPointer, block + kHolderWord);
    guest::StoreU32(h.base, kSaveDataPointer, block + kSaveData);
  };
  const auto f1 = [](Outputs& out, const PPCContext& t) { out.Double(t.f1.u64); };
  {
    // The top 160 bytes: SetWheel's saved registers (f31, r14 to r31, lr);
    // the 24 under its frame: the original FitAxles' and SetSizes' (r30,
    // r31, lr), which the plain C++ doesn't write (register saves aren't
    // reproduced). Its whole frame (the table at +96) and FitAxles' below
    // it compared.
    Results results("GarageStage::SetWheel (sub_8217DD38)");
    for (int c = 0; c < 80; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            LayWheelWorld(h.base, block, r, c, r.Below(4) != 0);
            t.r3.u64 = block + kHolder;
            t.r4.u64 = RandomWheelNumber(r);
            t.r5.u64 = RandomWhich(r);
            t.r6.u64 = block + kGarageCar;
          },
          __imp__sub_8217DD38, hooks::StandModelsSetWheel, 160, WheelFloats(block),
          fmt::format("case {}", c), 3120, 24, before, f1, 0x2000, kGarageBytes);
    }
    results.Report();
  }
  {
    // PreviewWheel's saved registers (the top 32: r29 to r31, lr), and
    // SetWheel's (160) and FitAxles' (24) under it, left out.
    Results results("TuningScreen::PreviewWheel (sub_821DFA78)");
    for (int c = 0; c < 60; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            LayWheelWorld(h.base, block, r, c, false);
            t.r3.u64 = block + kScreenSave - 18904;
            t.r4.u64 = RandomWheelNumber(r);
            t.r5.u64 = RandomWhich(r);
          },
          __imp__sub_821DFA78, hooks::StandModelsPreviewWheel, 32, WheelFloats(block),
          fmt::format("case {}", c), 4016, 160, before, f1, 0x4000, kGarageBytes,
          {{4016 + 3120, 24}});
    }
    results.Report();
  }
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(h.base, globals[i], saved[i]);
  }
  memory::Free(allocation);
}

}  // namespace

void CarMakers(Context& h) {
  Random r(0x6361726d616b6572ull);  // "carmaker"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  BlockOf(h, block, r);
  SetPaint(h, block, r);
  SetWheels(h, block, r);
  SetChoice(h, block, r);
  SetBytes(h, block, r);
  FromCar(h, block, r);
  memory::Free(allocation);
  Garage(h, r);
  Stand(h, r);
  Wheels(h, r);
}

}  // namespace harness
