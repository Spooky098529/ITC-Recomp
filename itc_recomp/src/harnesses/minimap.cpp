// The minimap's parts (src/minimap/, docs/functions/minimap.md) against the
// recompiled functions. Each case fills a block of guest memory with random
// bytes, writes its inputs there and points the globals the functions read
// at it, then runs the original and the register steps (hooks/minimap.h)
// from the same inputs, and compares the block, the globals and r3. The
// globals are put back after each case. Floats are compared as floats only
// where the originals copy or compute them with float instructions (lfs and
// stfs, Matrix::Copy, Matrix::Multiply); everything else exactly.

#include "harness.h"

#include "hooks/minimap.h"
#include "memory/heap.h"
#include "minimap/minimap.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace mm = minimap;

// The globals the functions read: the race manager, the vehicles, the
// route, the race data, the save data, the data tables and the transform's
// command list.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kRegions[] = {
    {0x826D82F0, 0x10}, {0x826C2E30, 0x10}, {0x826C5B10, 0x60},
    {0x826C5CA0, 0x10}, {0x826D83D0, 0x10},
};
constexpr uint32_t kVehicles = 0x826C2E34;
constexpr uint32_t kSaveData = 0x826C5B58;
constexpr uint32_t kTables = 0x826C5CA0;

constexpr uint32_t kBlockBytes = 0x4000;

// A float range [offset, offset + 4 x count) of the block.
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

// The block's bytes, with the float ranges compared as floats.
void AddBlock(Outputs& out, uint8_t* base, uint32_t block, FloatRanges floats) {
  std::sort(floats.begin(), floats.end());
  uint32_t at = 0;
  for (const auto& [offset, count] : floats) {
    if (offset > at) {
      out.Bytes(Read(base, block + at, offset - at));
    }
    out.Floats(base, block + offset, count);
    at = offset + 4 * count;
  }
  if (at < kBlockBytes) {
    out.Bytes(Read(base, block + at, kBlockBytes - at));
  }
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A float's bits: now and then a NaN (either sign, quiet or signaling), an
// infinity, a zero or a denormal; else a value of a few hundred.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | (r.Below(2) << 31) | r.Below(0x400000);
    case 1: return 0x7F800001u | (r.Below(2) << 31) | r.Below(0x3FFFFF);
    case 2: return 0x7F800000u | (r.Below(2) << 31);
    case 3: return r.Below(2) << 31;
    case 4: return r.Below(0x800000) | (r.Below(2) << 31);
    default: {
      const float f = (float(r.Below(200000)) - 100000.0f) / 256.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

void RandomFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, address + 4 * i, RandomFloat(r));
  }
}

// One case through both versions: the block filled at random and
// `setup(t)` writing the inputs and the registers (once: its inputs are
// written back for the second run); `floats` the block's float ranges,
// `extra(t, out)` outputs of its own.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what,
          const std::function<void(const PPCContext&, Outputs&)>& extra = {}) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Region& region : kRegions) {
    saved.push_back(Read(h.base, region.address, region.bytes));
  }
  RandomBytes(h.base, block, kBlockBytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  std::vector<std::vector<uint8_t>> globals;
  for (const Region& region : kRegions) {
    globals.push_back(Read(h.base, region.address, region.bytes));
  }
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    for (size_t i = 0; i < globals.size(); i++) {
      Write(h.base, kRegions[i].address, globals[i].data(), kRegions[i].bytes);
    }
    Fill(h.base, start.r1.u32 - 1024, 1024);  // the callee's frames
    PPCContext t = start;
    (v == 0 ? recompiled : native)(t, h.base);
    AddBlock(out[v], h.base, block, floats);
    for (const Region& region : kRegions) {
      out[v].Bytes(Read(h.base, region.address, region.bytes));
    }
    if (extra) {
      extra(t, out[v]);
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kRegions[i].address, saved[i].data(), kRegions[i].bytes);
  }
  results.Check(out[0], out[1], what);
}

// --- The race manager's lookups ---

void Lookups(Context& h, Random& r, uint32_t block) {
  {
    Results results("Race::FindPlayerEntry (sub_82170608)");
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            // The count: -1, 0, 1 to 80, or a random negative.
            const uint32_t count = c < 2 ? uint32_t(c - 1)
                                   : r.Below(16) == 0 ? 0x80000000u | uint32_t(r.Next())
                                                      : 1 + r.Below(80);
            guest::StoreU32(h.base, block + 2368, count);
            for (uint32_t i = 0; i < 80; i++) {
              // Flags with 0x4 or 0x8 now and then.
              const uint32_t flags = uint32_t(r.Next()) & ~0xCu;
              const uint32_t pick = r.Below(24);
              guest::StoreU32(h.base, block + 4 + 28 * i,
                              flags | (pick == 0 ? 0x4u : pick == 1 ? 0x8u : pick == 2 ? 0xCu : 0));
            }
            t.r3.u64 = block;
            t.r4.u64 = c % 3 == 2 ? r.Next() : uint64_t(c % 3);
          },
          __imp__sub_82170608, hooks::MinimapFindPlayerEntry, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Race::RacerVehicle (sub_82170578)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t racer = r.Below(16);
            guest::StoreU32(h.base, mm::kRaceManager, c < 4 ? 0 : block);
            guest::StoreU32(h.base, block + 2308 + 4 * racer, block + 0x1000 + 32 * racer);
            guest::StoreU32(h.base, block + 0x1000 + 32 * racer + 20,
                            r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8));
            guest::StoreU32(h.base, kVehicles, c % 8 == 5 ? 0 : 0x40000000u + r.Below(0x1000) * 16);
            t.r3.u64 = racer;
          },
          __imp__sub_82170578, hooks::MinimapRacerVehicle, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The course set's points ---

void Points(Context& h, Random& r, uint32_t block) {
  Results results("SetList::Point (sub_82147D18)");
  for (int c = 0; c < 128; c++) {
    const uint32_t out = block + 0x2000 + 16 * r.Below(4);
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t entry = r.Below(16), point = r.Below(4), part = r.Below(4);
          const uint32_t points = block + 0x800 + 0x100 * entry;
          guest::StoreU32(h.base, block + 528 + 4 * entry, points);
          RandomFloats(h.base, points, 64, r);
          t.r3.u64 = block;
          t.r4.u64 = out;
          t.r5.u64 = entry;
          t.r6.u64 = point;
          t.r7.u64 = part;
        },
        __imp__sub_82147D18, hooks::MinimapSetListPoint, {{out - block, 3}},
        fmt::format("case {}", c));
  }
  results.Report();
}

// --- Minimap::RacerMarkerType ---

void MarkerType(Context& h, Random& r, uint32_t block) {
  Results results("Minimap::RacerMarkerType (sub_820F2D28)");
  for (int c = 0; c < 1024; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t entry = block;
          // The flags (0x10 now and then), the racer record's index.
          guest::StoreU32(h.base, entry, uint32_t(r.Next()) & (r.Below(6) == 0 ? ~0u : ~0x10u));
          const uint32_t index = r.Below(4);
          guest::StoreU32(h.base, entry + 24, index);
          // The race data placed so the record's id is at +0x100.
          guest::StoreU32(h.base, mm::kRaceData, block + 0x100 - 48 - 3936 * index - 3912);
          const uint32_t id = r.Below(10) == 0 ? 0xFFFFFFFFu : r.Below(200);
          guest::StoreU32(h.base, block + 0x100, id);
          // The save data placed so the id's halfwords are at +0x200, the
          // item at +0x400 (its +88 never 3: that's sub_820F2CA8's chained
          // record, the original's in both versions, never seen in play, and
          // it follows the records until one ends, which made-up ones don't).
          guest::StoreU32(h.base, kSaveData, block + 0x200 - 104952 - 12 * id);
          for (uint32_t i = 0; i < 3; i++) {
            const uint32_t pick = r.Below(4);
            guest::StoreU16(h.base, block + 0x200 + 2 * i,
                            pick == 0 ? 0 : pick == 1 ? uint16_t(r.Next()) : uint16_t(r.Below(5)));
          }
          guest::StoreU32(h.base, kTables, block + 0x300);
          guest::StoreU32(h.base, block + 0x300 + 16, block + 0x400 - 3008 * id);
          const uint16_t kind = uint16_t(r.Next());
          guest::StoreU16(h.base, block + 0x400 + 88, kind == 3 ? 4 : kind);
          // The section and slice (negative now and then); the route's
          // sections with branch numbers 0 to 15 (or anything).
          guest::StoreU16(h.base, entry + 8,
                          r.Below(10) == 0 ? uint16_t(0x8000 | r.Next()) : r.Below(16));
          guest::StoreU16(h.base, entry + 10, r.Below(10) == 0 ? uint16_t(r.Next()) : r.Below(64));
          guest::StoreU32(h.base, mm::kRoute, block + 0x500);
          guest::StoreU32(h.base, block + 0x500 + 12, block + 0x600);
          for (uint32_t s = 0; s < 16; s++) {
            guest::StoreU32(h.base, block + 0x600 + 4 * s, block + 0x700 + 16 * s);
            guest::StoreU8(h.base, block + 0x700 + 16 * s + 3,
                           r.Below(8) == 0 ? uint8_t(r.Next()) : uint8_t(r.Below(16)));
          }
          t.r3.u64 = entry;
          // The player's branch: 0 to 15, -1 or anything.
          const uint32_t pick = r.Below(16);
          t.r4.u64 = pick == 0 ? ~0ull : pick == 1 ? r.Next() : r.Below(16);
        },
        __imp__sub_820F2D28, hooks::MinimapRacerMarkerType, {}, fmt::format("case {}", c));
  }
  results.Report();
}

// --- The markers' shapes ---

void Shapes(Context& h, Random& r, uint32_t block) {
  {
    Results results("Minimap::Scaling (sub_820F2EF8)");
    for (int c = 0; c < 64; c++) {
      const uint32_t out = block + 0x100 + 4 * r.Below(4);
      // The vector apart, or out itself (read after the identity).
      const uint32_t vector = c % 8 == 7 ? out : block + 0x800;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            RandomFloats(h.base, vector, 4, r);
            if (c < 4) {
              guest::StoreU32(h.base, vector + 0, 0x42C80000);  // 100.0, 1.0, 100.0
              guest::StoreU32(h.base, vector + 4, 0x3F800000);
              guest::StoreU32(h.base, vector + 8, 0x42C80000);
            }
            t.r3.u64 = out;
            t.r4.u64 = vector;
          },
          __imp__sub_820F2EF8, hooks::MinimapScaling,
          {{out - block, 1}, {out - block + 20, 1}, {out - block + 40, 1}},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Triangle::Init (sub_820F4040)");
    for (int c = 0; c < 16; c++) {
      // 16-byte aligned (the game's) and not (the stvx stores aligned).
      const uint32_t triangle = block + 0x100 + 4 * uint32_t(c % 4);
      Case(
          h, results, block, r, [&](PPCContext& t) { t.r3.u64 = triangle; },
          __imp__sub_820F4040, hooks::MinimapTriangleInit, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The 80-slot lists ---

std::vector<uint32_t> Permutation(Random& r, uint32_t n) {
  std::vector<uint32_t> p(n);
  for (uint32_t i = 0; i < n; i++) {
    p[i] = i;
  }
  for (uint32_t i = n; i > 1; i--) {
    std::swap(p[i - 1], p[r.Below(i)]);
  }
  return p;
}

// A valid 80-slot list of `count` nodes in random order and data slots, the
// rest on the free list; its data random floats.
void RandomList(uint8_t* base, uint32_t list, uint32_t count, Random& r) {
  const std::vector<uint32_t> order = Permutation(r, 80);
  const std::vector<uint32_t> slots = Permutation(r, 80);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t node = order[i];
    const bool last = i + 1 == count || i + 1 == 80;
    guest::StoreU16(base, list + 4 * node, last ? 0xFFFF : order[i + 1]);
    guest::StoreU16(base, list + 4 * node + 2, slots[node]);
  }
  guest::StoreU16(base, list + 320, count < 80 ? order[count] : 0xFFFF);
  guest::StoreU16(base, list + 324, count > 0 ? order[0] : 0xFFFF);
  guest::StoreU16(base, list + 328, count);
  RandomFloats(base, list + 336, 80 * 16, r);
}

void NativeLink(PPCContext& ctx, uint8_t* base) {
  mm::List80Link(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}

void Lists(Context& h, Random& r, uint32_t block) {
  const uint32_t list = block + 0x100;
  const uint32_t data = 0x100 + 336;
  {
    Results results("List80::Link (sub_820F40C0)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t count = r.Below(80);
            RandomList(h.base, list, count, r);
            t.r3.u64 = list;
            t.r4.u64 = (r.Below(4) == 0 ? r.Next() & ~0xFFFFull : 0) | r.Below(count + 1);
          },
          __imp__sub_820F40C0, NativeLink, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("List80::Insert (sub_820F5238)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t count = r.Below(80);
            RandomList(h.base, list, count, r);
            const uint32_t matrix = block + 0x3000;
            RandomFloats(h.base, matrix, 16, r);
            t.r3.u64 = list;
            t.r4.u64 = matrix;
            // The game's position is the count.
            t.r5.u64 = c % 2 == 0 ? count : r.Below(count + 1);
          },
          __imp__sub_820F5238, hooks::MinimapList80Insert, {{data, 80 * 16}},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("List80::Copy (sub_820F51A8)");
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t count = 1 + r.Below(80);
            RandomList(h.base, list, count, r);
            t.r3.u64 = block + 0x3000 + 4 * r.Below(4);
            t.r4.u64 = list;
            t.r5.u64 = r.Below(count);
          },
          __imp__sub_820F51A8, hooks::MinimapList80Copy, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- Primitive::RecordTransform ---

void Transform(Context& h, Random& r, uint32_t block) {
  Results results("Primitive::RecordTransform (sub_8217AEA8)");
  // A command list in the block: room for the five slots (a full list's
  // message and writes through 0 are left to the original, never seen).
  const uint32_t list = block + 0x100;
  const uint32_t slots = block + 0x1000;
  for (int c = 0; c < 128; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          const uint32_t used = r.Below(1000);
          guest::StoreU32(h.base, mm::kTransformList, list);
          guest::StoreU32(h.base, list + 8, used + 6 + r.Below(1000));
          guest::StoreU32(h.base, list + 12, used);
          guest::StoreU32(h.base, list + 44, slots);
          const uint32_t pass = block + 0x200;
          const uint32_t primitive = block + 0x400;
          RandomFloats(h.base, pass + 144, 16, r);
          RandomFloats(h.base, primitive, 16, r);
          t.r3.u64 = pass;
          t.r4.u64 = primitive;
        },
        __imp__sub_8217AEA8, hooks::MinimapRecordTransform,
        {{0x1000 + 16, 4}, {0x1000 + 32, 4}, {0x1000 + 48, 4}, {0x1000 + 64, 4}},
        fmt::format("case {}", c), [&](const PPCContext& t, Outputs& out) {
          out.Floats(h.base, t.r1.u32 - mm::kTransformProduct, 16);
        });
  }
  results.Report();
}

}  // namespace

void Minimap(Context& h) {
  Random r(0x6D696E696D6170ull);  // "minimap"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Lookups(h, r, block);
  Points(h, r, block);
  MarkerType(h, r, block);
  Shapes(h, r, block);
  Lists(h, r, block);
  Transform(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
