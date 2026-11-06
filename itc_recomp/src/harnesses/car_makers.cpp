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
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

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
// compared, `floats` as floats, then r3.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, uint32_t saves, const FloatRanges& floats,
          const std::string& what) {
  RandomBytes(h.base, block, kBlockBytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  const uint32_t sp = start.r1.u32;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    Fill(h.base, sp - kStackBytes, kStackBytes + kArgBytes);
    PPCContext t = start;
    t.fpscr.disableFlushModeUnconditional();
    (v == 0 ? recompiled : native)(t, h.base);
    AddRanges(out[v], h.base, block, kBlockBytes, floats);
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
    out[v].Bytes(Read(h.base, sp, kArgBytes));
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
        fmt::format("case {}", c));
  }
  guest::StoreU32(h.base, kParamsPointer, params_global);
  results.Report();
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
}

}  // namespace harness
