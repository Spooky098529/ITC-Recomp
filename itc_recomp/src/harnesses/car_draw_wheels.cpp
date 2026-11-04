// The wheels (src/car_draw/wheels.cpp, docs/functions/car-draw.md: the
// tires, set a's rims, plain and morphed, set b's rim and the brakes, plain and
// morphed (those from a generator of their own); the
// rims' world in RimAWorld, the brakes' in BrakesWorld) against the recompiled functions, under the car_draw harness (its
// Case and model draws' world, car_draw_shared.h), with a random generator
// of its own (so its cases don't shift the other files' inputs; it runs
// last). The wheels in an allocation of their own: the tire group at +0x100
// (+4 the car record, +8 the wheels record, +300 the front's flag, record 0 at
// +368: +4 the model, the block's +0x100, or 0, +228 the shape; the side
// float4s at +1808 and +1824), the wheels record at +0xA00 (+0 the kind, +56
// the second rear tire's count; for the morphed tire +44 and +56 the entry
// indices or -1, their floats, +72 and +76), the car record at +0xA80 (+0 the
// car, below 45), the morph's stream headers at +0xC00 and +0xE10 (every
// shape's +400 and +404), the pool maker at +0xB00 (its slot 1 the empty
// function: the "job" is the maker) or none, the work memory at +0x1000, the
// command list's slots from +0x4000. The scene at the race block's +0x10000 ([0x826C2E44]:
// its +4 the set, the pass description at +5072: +4 the depth pass, +32 the
// key, +60 the bits, +76 and +80 the keys saved), the camera passes 0 to 3,
// the scene model's block at the block's +0xF380 (+16 the world, +80 to +527
// the R and S matrices, +592 the second rear tire, +732 no translation). The
// allocation and the scene are put back before each version and compared
// after it, with the frame (its back chain and temporaries from +80), r3,
// r24, r28 and what went to the render queue. The recompiled side runs with
// --native_car_draw off (its callees the originals); the native side the
// adapter's register step.

#include "harness.h"
#include "harnesses/car_draw_shared.h"

#include "base/log.h"
#include "hooks/car_draw.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <vector>

namespace harness::car_draw_shared {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kPasses = 0x826BAC00;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kSceneBytes = 0x2000;
// The allocation.
constexpr uint32_t kGroup = 0x100;
constexpr uint32_t kWheels = 0xA00;
constexpr uint32_t kCar = 0xA80;
constexpr uint32_t kMaker = 0xB00;
constexpr uint32_t kStream0 = 0xC00;
constexpr uint32_t kStream1 = 0xE10;
constexpr uint32_t kWork = 0x1000;
// The brake group (twelve records and the side float4s at +2912 and +2928,
// past the work memory's reach).
constexpr uint32_t kBrakesGroup = 0x2800;
// The morphed brakes' side matrix ([+0] of the brake group).
constexpr uint32_t kBrakesSides = 0x3A00;
// The empty function the fake vtables' slots point at.
constexpr uint32_t kEmptyFunction = 0x82106540;
constexpr uint32_t kRecordsEnd = 0x4000;
constexpr uint32_t kSlots = 0x4000;
constexpr uint32_t kSlotCount = 4000;
constexpr uint32_t kWheelsBytes = kSlots + kSlotCount * 16;
// Below the shared Case's filled stack (its 2,048 bytes), filled too: the
// callees' frames reach further down.
constexpr uint32_t kDeepStackBytes = 8192;

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A matrix near a turn about y with a small translation, a scale, or
// anything finite.
void RandomMatrix(uint8_t* base, uint32_t at, Random& r) {
  switch (r.Below(5)) {
    case 0:
      FiniteFloats(base, at, 16, r);
      return;
    case 1: {
      const float s = 0.5f + float(r.Below(100)) / 100.0f;
      StoreFloats(base, at, {s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0, 0, 0, 0, 1});
      return;
    }
    default: {
      const float c = Spread(r, 1.0f), s = Spread(r, 1.0f);
      StoreFloats(base, at, {c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, Spread(r, 0.5f),
                             Spread(r, 0.5f), Spread(r, 0.5f), 1});
    }
  }
}

// A flag word: 0 or 1 mostly, now and then any word.
uint32_t Flag(Random& r) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2);
}

// The morph's world: for a tire (`tire`) the weights' inputs and the targets,
// then the shapes' streams, the work memory and the pool maker (the tire
// group at `at_group`, else top + kGroup). Returns whether there's a maker.
bool MorphWorld(Context& h, Random& r, uint32_t block, uint32_t top, bool tire,
                uint32_t at_group = 0) {
  uint8_t* base = h.base;
  const uint32_t group = at_group ? at_group : top + kGroup, wheels = top + kWheels;
  if (tire) {
    guest::StoreU32(base, group + 368 + 4, block + 0x100);
    guest::StoreU32(base, wheels + 0, r.Below(4) == 0 ? 3 : r.Below(3));
    for (const uint32_t at : {44u, 56u}) {
      guest::StoreU32(base, wheels + at, r.Below(2) == 0 ? 0xFFFFFFFFu : 12 + r.Below(12));
      guest::StoreU32(base, wheels + at + 4, RandomFloat(r));
      guest::StoreU32(base, wheels + at + 8, RandomFloat(r));
    }
    guest::StoreU32(base, wheels + 72, r.Below(3));
    guest::StoreU32(base, wheels + 76, r.Below(3));
    RandomFloats(base, group + 12, 288 / 4, r);
    for (uint32_t k = 0; k < 5; k++) {
      guest::StoreU32(base, group + 304 + 4 * k, r.Below(4));
    }
    RandomFloats(base, group + 328, 8, r);
  }
  // The streams: every shape's, shared (the same counts, no debug message).
  const uint32_t s0 = top + kStream0, s1 = top + kStream1;
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t shape = block + 0x2000 + 544 * k;
    guest::StoreU32(base, shape + 400, s0);
    guest::StoreU32(base, shape + 404, s1);
  }
  const uint32_t vertices = 1 + r.Below(16);
  guest::StoreU32(base, s0 + 0, vertices);
  guest::StoreU32(base, s0 + 12, 16 * vertices + (r.Below(4) == 0 ? 16 * r.Below(4) : 0));
  guest::StoreU32(base, s1 + 4, r.Below(3) == 0 ? 6 : 9);
  guest::StoreU32(base, s1 + 12, r.Below(3) == 0 ? 0 : 16 * vertices);
  for (uint32_t w = 0; w < 0x200 / 4; w++) {
    guest::StoreFloat(base, s0 + 16 + 4 * w, Spread(r, 4.0f));
    guest::StoreFloat(base, s1 + 16 + 4 * w, Spread(r, 4.0f));
  }
  // The work memory: buffer i current, its header count small (now and then
  // at the 1,280 limit), the bytes used and the capacity (now and then not
  // fitting).
  const uint32_t i = r.Below(2);
  guest::StoreU32(base, 0x8296B780, i);
  for (uint32_t b = 0; b < 2; b++) {
    guest::StoreU32(base, 0x8296B798 + 4 * b, r.Below(8) == 0 ? 1279 + r.Below(2) : r.Below(6));
    guest::StoreU32(base, 0x8296B7A0 + 4 * b, 128 * r.Below(8));
    guest::StoreU32(base, 0x8296B7A8 + 4 * b, top + kWork);
  }
  const uint32_t used = guest::LoadU32(base, 0x8296B7A0 + 4 * i);
  guest::StoreU32(base, 0x8296B7B0, r.Below(8) == 0 ? used + 16 * vertices : used + 0x800);
  // The pool maker.
  const bool maker = r.Below(3) != 0;
  guest::StoreU32(base, 0x8296B914, maker ? top + kMaker : 0);
  guest::StoreU32(base, top + kMaker, top + kMaker + 0x10);
  guest::StoreU32(base, top + kMaker + 0x10 + 4, kEmptyFunction);
  return maker;
}

// A tire group's fields at `group` (the car and wheels records at top +
// kCar and + kWheels): see the top.
void TireGroupWorld(uint8_t* base, Random& r, uint32_t block, uint32_t top, uint32_t group,
                    uint32_t shape) {
  guest::StoreU32(base, group + 4, top + kCar);
  guest::StoreU32(base, group + 8, top + kWheels);
  guest::StoreU32(base, group + 300, Flag(r));
  guest::StoreU32(base, group + 368 + 4, r.Below(8) == 0 ? 0 : block + 0x100);
  guest::StoreU32(base, group + 368 + 228, shape);
  for (uint32_t at : {1808u, 1824u}) {
    if (r.Below(6) == 0) {
      RandomFloats(base, group + at, 4, r);
    } else {
      FiniteFloats(base, group + at, 4, r);
    }
  }
}

// The world; see the top. `set` the scene's +4.
void WheelsWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t top,
                 uint32_t shape, uint32_t set) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, top + kSlots);
  guest::StoreU32(base, list + 44, top + kSlots);
  const std::vector<uint8_t> zero(kRecordsEnd, 0);
  Write(base, top, zero.data(), kRecordsEnd);
  // The tire group and its records.
  const uint32_t wheels = top + kWheels, car = top + kCar;
  TireGroupWorld(base, r, block, top, top + kGroup, shape);
  guest::StoreU32(base, wheels + 0, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(4));
  guest::StoreU32(base, wheels + 56,
                  r.Below(8) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(9)) - 2));
  guest::StoreU32(base, car + 0, r.Below(45));
  // The scene and its pass description.
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  guest::StoreU32(base, scene + 4, set);
  const uint32_t d = scene + 5072;
  guest::StoreU32(base, d + 4, r.Below(4));
  RandomWords(base, d + 32, 28, r);
  guest::StoreU32(base, d + 60, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(256));
  RandomWords(base, d + 76, 8, r);
  // The camera passes.
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t pass = kPasses + 592 * p;
    if (r.Below(6) == 0) {
      RandomFloats(base, pass + 144, 16, r);
    } else {
      StoreFloats(base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    guest::StoreU32(base, pass + 588, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  }
  // The scene model's block: its world, the R and S matrices, its flags.
  const uint32_t source = block + kSource;
  if (r.Below(3) == 0) {
    RandomMatrix(base, source + 16, r);
  } else {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, source + 16, world.data(), 64);
  }
  for (uint32_t m = 0; m < 8; m++) {
    if (r.Below(12) == 0) {
      RandomFloats(base, source + 80 + 64 * m, 16, r);
    } else {
      RandomMatrix(base, source + 80 + 64 * m, r);
    }
  }
  guest::StoreU32(base, source + 592, Flag(r));
  guest::StoreU32(base, source + 732, r.Below(4) == 0 ? Flag(r) : 0);
}

// A set a rim's world over the tire's (the rim group at the same +0x100):
// the front flag (+12), the weight (+16), the z offset (+20), the side
// float4s (+1712, +1728), records 0 to 6 at +32 + 240k (their models, +4,
// the block's +0x100 or now and then 0, record 0's always set when
// `morphed`: the original morphs on it unchecked; the drawn records' and the
// morphs' bases' shapes, +228, the case's, the targets' (records 1 and 3)
// any of the four; every shape a copy of the case's, the model's +412 shapes
// 0 to 4), and the materials' cull modes (+344) 2 or 6 now and then
// for the flips (the group at `at_group`, else top + kGroup).
void RimAWorld(Context& h, Random& r, uint32_t block, uint32_t top, uint32_t shape,
               bool morphed, uint32_t at_group = 0) {
  uint8_t* base = h.base;
  // Every shape a copy of the drawn one, as the interior's harness (the
  // flips walk all the model's shapes, its +412).
  for (uint32_t k = 0; k < 4; k++) {
    if (k != shape) {
      const std::vector<uint8_t> bytes = Read(base, block + 0x2000 + 544 * shape, 544);
      Write(base, block + 0x2000 + 544 * k, bytes.data(), 544);
    }
  }
  guest::StoreU32(base, block + 0x100 + 412, r.Below(8) == 0 ? 0 : 1 + r.Below(4));
  const uint32_t group = at_group ? at_group : top + kGroup;
  guest::StoreU32(base, group + 12, Flag(r));
  guest::StoreU32(base, group + 16, RandomFloat(r));
  if (r.Below(6) == 0) {
    RandomFloats(base, group + 20, 1, r);
  } else {
    guest::StoreFloat(base, group + 20, Spread(r, 0.2f));
  }
  for (uint32_t at : {1712u, 1728u}) {
    if (r.Below(6) == 0) {
      RandomFloats(base, group + at, 4, r);
    } else {
      FiniteFloats(base, group + at, 4, r);
    }
  }
  for (uint32_t k = 0; k < 7; k++) {
    const uint32_t record = group + 32 + 240 * k;
    const bool target = k == 1 || k == 3;
    const bool model = (morphed && k == 0) || r.Below(8) != 0;
    guest::StoreU32(base, record + 4, model ? block + 0x100 : 0);
    guest::StoreU32(base, record + 228, target ? r.Below(4) : shape);
  }
  const uint32_t materials = block + 0x6000;
  const uint32_t count = guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12);
  for (uint32_t m = 0; m < count && m < 16; m++) {
    if (r.Below(2) == 0) {
      guest::StoreU32(base, materials + 16 + 512 * m + 344, r.Below(2) ? 2 : 6);
    }
  }
}

// The brakes' world over the tire's (the brake group at +0x2800): the
// front flag (+12), the weight (+16), the side float4s (+2912, +2928), the
// wheels record's +8 (the morphs, now and then), +68 (the brake kind), +84,
// +88 and the angles +100 and +104, records 0 to 11 at +32 + 240k (their
// models the block's +0x100 or now and then 0, record 0's always set when
// morphing; the drawn records' and the morphs' bases' shapes the case's, the
// targets' (records 1, 3, 5, 7, 9) any of the four; every shape a copy of
// the case's), and the scene model block's alphas (+612 to +624). Returns whether it
// morphs; with the morphs, the morph's world (MorphWorld) and `makers`
// counted (the group at `at_group`, else top + kBrakesGroup).
bool BrakesWorld(Context& h, Random& r, uint32_t block, uint32_t source, uint32_t top,
                 uint32_t shape, int& makers, bool morphed, uint32_t at_group = 0) {
  uint8_t* base = h.base;
  for (uint32_t k = 0; k < 4; k++) {
    if (k != shape) {
      const std::vector<uint8_t> bytes = Read(base, block + 0x2000 + 544 * shape, 544);
      Write(base, block + 0x2000 + 544 * k, bytes.data(), 544);
    }
  }
  guest::StoreU32(base, block + 0x100 + 412, r.Below(8) == 0 ? 0 : 1 + r.Below(4));
  const uint32_t group = at_group ? at_group : top + kBrakesGroup, wheels = top + kWheels;
  const bool morphs = morphed ? r.Below(8) != 0 : r.Below(4) == 0;
  if (morphed) {
    // DrawBrakesMorphed's: the 64-byte matrix at [+0] its +2912 to +2943
    // come from, and the brakes' size words (+108, +112).
    guest::StoreU32(base, group, top + kBrakesSides);
    FiniteFloats(base, top + kBrakesSides, 16, r);
    for (uint32_t at : {108u, 112u}) {
      guest::StoreU32(base, wheels + at, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(19));
    }
  }
  guest::StoreU32(base, group + 8, wheels);
  guest::StoreU32(base, group + 12, Flag(r));
  guest::StoreU32(base, group + 16, RandomFloat(r));
  for (uint32_t at : {2912u, 2928u}) {
    if (r.Below(6) == 0) {
      RandomFloats(base, group + at, 4, r);
    } else {
      FiniteFloats(base, group + at, 4, r);
    }
  }
  guest::StoreU32(base, wheels + 8, morphs ? (r.Below(4) == 0 ? uint32_t(r.Next()) | 1u : 1) : 0);
  guest::StoreU32(base, wheels + 68,
                  r.Below(8) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(9)) - 1));
  guest::StoreU32(base, wheels + 84, Flag(r));
  guest::StoreU32(base, wheels + 88, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3));
  for (uint32_t at : {100u, 104u}) {
    if (r.Below(6) == 0) {
      RandomFloats(base, wheels + at, 1, r);
    } else {
      guest::StoreFloat(base, wheels + at, Spread(r, 360.0f));
    }
  }
  for (uint32_t k = 0; k < 12; k++) {
    const uint32_t record = group + 32 + 240 * k;
    const bool target = k < 10 && (k & 1) != 0;
    const bool model = (morphs && k == 0) || r.Below(8) != 0;
    guest::StoreU32(base, record + 4, model ? block + 0x100 : 0);
    guest::StoreU32(base, record + 228, target ? r.Below(4) : shape);
  }
  for (uint32_t at = 612; at <= 624; at += 4) {
    if (r.Below(6) == 0) {
      RandomFloats(base, source + at, 1, r);
    } else {
      guest::StoreFloat(base, source + at, float(r.Below(101)) / 100.0f);
    }
  }
  if (morphs) {
    makers += MorphWorld(h, r, block, top, false);
  }
  return morphs;
}

enum class Kind { kTire, kTireMorphed, kRimA, kRimAMorphed, kRimB, kBrakes, kBrakesMorphed };

struct Target {
  const char* name;
  GuestFunction recompiled, native;
  uint32_t frame;
  uint32_t temporaries_end;  // below the saved registers
  int cases;
  Kind kind;
};

// --- The wheels' top ---

// The wheels' top's allocation: the object at +0x100 (+40 its wheels
// record's +8, the dispatch), its point block at +192 (+12 the wheels record
// at the leaves' top + kWheels; the groups at their offsets), the command
// list's slots from +0x4400 (up to six leaves' draws a case).
constexpr uint32_t kTopObject = 0x100;
constexpr uint32_t kTopPointsBytes = 0x4100;
constexpr uint32_t kTopEnd = kTopObject + 192 + kTopPointsBytes;
constexpr uint32_t kTopSlots = 0x4400;
constexpr uint32_t kTopSlotCount = 24000;
constexpr uint32_t kTopBytes = kTopSlots + kTopSlotCount * 16;

// The point block's world at `points` over the leaves' (`top`, WheelsWorld
// already run): the tires (+32, +1872), set a's rims (+3712, +5456), set
// b's (+7200, +8944) and the brakes (+10688, +13632), each group's world as
// its leaf's harness builds it; `morphed` the morphed leaves' (the tires'
// weights, the rims' morphs, the brakes' side matrix). With the wheels
// record's +8 left set, every brake group's record 0 model set (the
// brakes morph on it unchecked). Returns whether the brakes morph.
bool TopPointsWorld(Context& h, Random& r, uint32_t block, uint32_t source, uint32_t top,
                    uint32_t points, uint32_t shape, bool morphed, int& makers) {
  uint8_t* base = h.base;
  const std::vector<uint8_t> zero(kTopPointsBytes, 0);
  Write(base, points, zero.data(), kTopPointsBytes);
  guest::StoreU32(base, points + 12, top + kWheels);
  for (const uint32_t at : {32u, 1872u}) {
    TireGroupWorld(base, r, block, top, points + at, shape);
    if (morphed) {
      makers += MorphWorld(h, r, block, top, true, points + at);
    }
    // The floats the lists copy into the rims (the last pass's weights'
    // floats for the morphed tires).
    RandomFloats(base, points + at + 352, 2, r);
  }
  for (const uint32_t at : {3712u, 5456u}) {
    TireGroupWorld(base, r, block, top, points + at, shape);
    RimAWorld(h, r, block, top, shape, morphed, points + at);
    if (morphed) {
      makers += MorphWorld(h, r, block, top, false);
    }
  }
  for (const uint32_t at : {7200u, 8944u}) {
    TireGroupWorld(base, r, block, top, points + at, shape);
    RimAWorld(h, r, block, top, shape, false, points + at);
  }
  for (uint32_t at = 596; at <= 608; at += 4) {
    guest::StoreU32(base, source + at, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(4));
  }
  bool morphs = false;
  for (const uint32_t at : {10688u, 13632u}) {
    morphs = BrakesWorld(h, r, block, source, top, shape, makers, morphed, points + at);
  }
  if (guest::LoadU32(base, top + kWheels + 8) != 0) {
    for (const uint32_t at : {10688u, 13632u}) {
      guest::StoreU32(base, points + at + 36, block + 0x100);
    }
  }
  return morphs;
}

enum class TopKind { kScene, kDraw, kDrawMorphed };

struct TopTarget {
  const char* name;
  GuestFunction recompiled, native;
  int cases;
  TopKind kind;
};

void NativeWheelsPointsDraw(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32;
  const uint64_t r24 = ctx.r24.u64;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::WheelsPointsDraw(base, calls, r1, r3, r4, r24);
  });
}

void NativeWheelsPointsDrawMorphed(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32;
  const uint64_t r24 = ctx.r24.u64;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::WheelsPointsDrawMorphed(base, calls, r1, r3, r4, r24);
  });
}

// SceneWheels::Draw and the point block's two draws against the recompiled
// ones (run with --native_car_draw off), a random generator of their own:
// the frame's back chain, the last tire's temporaries, the object, the
// leaves' records, the scene, the command list and r3, r24, r28 compared.
void WheelsTopDraws(Context& h, uint32_t block, uint32_t race, uint32_t top) {
  Random r(0x7768656C746F70ull);  // "wheltop"
  const uint32_t allocation = memory::Alloc(kTopBytes + 16, 0);
  const uint32_t mine = (allocation + 15) & ~15u;
  const uint32_t object = mine + kTopObject, points = object + 192;
  const uint32_t source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const TopTarget targets[] = {
      {"SceneWheels::Draw (sub_820ECE28)", __imp__sub_820ECE28, hooks::CarDrawSceneWheels, 192,
       TopKind::kScene},
      {"WheelPoints::Draw (sub_820ECA30)", __imp__sub_820ECA30, NativeWheelsPointsDraw, 128,
       TopKind::kDraw},
      {"WheelPoints::DrawMorphed (sub_820ECB68)", __imp__sub_820ECB68,
       NativeWheelsPointsDrawMorphed, 128, TopKind::kDrawMorphed},
  };
  for (const TopTarget& target : targets) {
    Results results(target.name);
    int morphed_cases = 0, front_end = 0, copies = 0, brake_morphs = 0, makers = 0, queued = 0;
    std::vector<uint8_t> scene_bytes, records, own;
    const std::vector<uint8_t> zero(kTopSlotCount * 16, 0);
    uint32_t sp = 0;
    bool morphed = false;
    for (int c = 0; c < target.cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            const uint32_t set = r.Below(8) ? r.Below(2) : 2 + r.Below(4);
            const uint32_t shape = r.Below(4);
            WheelsWorld(h, r, block, race, top, shape, set);
            morphed = target.kind == TopKind::kDrawMorphed ||
                      (target.kind == TopKind::kScene && r.Below(2) == 0);
            brake_morphs +=
                TopPointsWorld(h, r, block, source, top, points, shape, morphed, makers);
            guest::StoreU32(h.base, object + 40,
                            morphed ? (r.Below(4) == 0 ? uint32_t(r.Next()) | 1u : 1) : 0);
            // The command list's slots in this allocation; the scene's
            // current camera pass (+5072) enabled (a disabled one makes the
            // leaves read address 144).
            const uint32_t list = block + 0x3000;
            guest::StoreU32(h.base, list + 8, kTopSlotCount);
            guest::StoreU32(h.base, list + 32, mine + kTopSlots);
            guest::StoreU32(h.base, list + 44, mine + kTopSlots);
            const uint32_t index = r.Below(4);
            guest::StoreU32(h.base, scene + 5072, index);
            guest::StoreU32(h.base, kPasses + 592 * index + 588, uint32_t(r.Next()) | 1u);
            morphed_cases += morphed;
            front_end += set == 0;
            copies += !morphed && guest::LoadU32(h.base, top + kWheels + 8) != 0;
            scene_bytes = Read(h.base, scene, kSceneBytes);
            records = Read(h.base, top, kRecordsEnd);
            own = Read(h.base, mine, kTopEnd);
            sp = t.r1.u32;
            t.r3.u64 = target.kind == TopKind::kScene ? object : points;
            t.r4.u64 = source;
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - car_draw::kWheelsPointsFrame;
            o.Word(guest::LoadU32(h.base, frame));
            // The last tire's frame below it: its back chain and
            // temporaries (above them its saved registers).
            const uint32_t tire_frame =
                morphed ? car_draw::kWheelsTireMorphedFrame : car_draw::kWheelsTireFrame;
            const uint32_t tire_end = morphed ? 1520 : 1888;
            o.Word(guest::LoadU32(h.base, frame - tire_frame));
            o.Floats(h.base, frame - tire_frame + 80, (tire_end - 80) / 4);
            o.Floats(h.base, mine, kTopEnd / 4);
            o.Floats(h.base, top, kRecordsEnd / 4);
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t written =
                guest::LoadU32(h.base, block + 0x3000 + 44) - (mine + kTopSlots);
            o.Word(written);
            o.Floats(h.base, mine + kTopSlots, std::min(written, kTopSlotCount * 16) / 4);
            o.Word(uint32_t(t.r24.u64));
            o.Word(uint32_t(t.r28.u64));
            if (t.r3.u32 == block + 0x7C0) {
              queued++;
              o.Word(t.r4.u32);
              o.Word(t.r5.u32);
              o.Double(t.f1.u64);
            }
          },
          [&](int) {
            Write(h.base, scene, scene_bytes.data(), kSceneBytes);
            Write(h.base, top, records.data(), kRecordsEnd);
            Write(h.base, mine, own.data(), kTopEnd);
            Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
            Write(h.base, mine + kTopSlots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    ITC_LOG_INFO("harness: {}: {} of {} cases morphed ({} with the brakes' morphs, {} pool "
                 "makers), {} in the front end, {} copied the tires' floats unmorphed, {} ended "
                 "handing an entry to the render queue",
                 target.name, morphed_cases, target.cases, brake_morphs, makers, front_end,
                 copies, queued / 2);
    results.Report();
  }
  memory::Free(allocation);
}

}  // namespace

void WheelsDraws(Context& h, uint32_t block, uint32_t race) {
  Random wheels_random(0x776865656C73ull);  // "wheels"
  const uint32_t allocation = memory::Alloc(kWheelsBytes + 16, 0);
  const uint32_t top = (allocation + 15) & ~15u;
  const uint32_t source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const Target targets[] = {
      {"WheelPoints::DrawTire (sub_820E8AE8)", __imp__sub_820E8AE8,
       hooks::CarDrawWheelsDrawTire, car_draw::kWheelsTireFrame, 1888, 384, Kind::kTire},
      {"WheelPoints::DrawTireMorphed (sub_820E9318)", __imp__sub_820E9318,
       hooks::CarDrawWheelsDrawTireMorphed, car_draw::kWheelsTireMorphedFrame, 1520, 384,
       Kind::kTireMorphed},
      {"WheelPoints::DrawRimA (sub_820E9A28)", __imp__sub_820E9A28,
       hooks::CarDrawWheelsDrawRimA, car_draw::kWheelsRimAFrame, 1904, 256, Kind::kRimA},
      {"WheelPoints::DrawRimAMorphed (sub_820EA148)", __imp__sub_820EA148,
       hooks::CarDrawWheelsDrawRimAMorphed, car_draw::kWheelsRimAMorphedFrame, 2128, 256,
       Kind::kRimAMorphed},
      {"WheelPoints::DrawRimB (sub_820EA950)", __imp__sub_820EA950,
       hooks::CarDrawWheelsDrawRimB, car_draw::kWheelsRimBFrame, 2400, 256, Kind::kRimB},
      {"WheelPoints::DrawBrakes (sub_820EB388)", __imp__sub_820EB388,
       hooks::CarDrawWheelsDrawBrakes, car_draw::kWheelsBrakesFrame, 2464, 256, Kind::kBrakes},
      {"WheelPoints::DrawBrakesMorphed (sub_820EBEB8)", __imp__sub_820EBEB8,
       hooks::CarDrawWheelsDrawBrakesMorphed, car_draw::kWheelsBrakesMorphedFrame, 2464, 256,
       Kind::kBrakesMorphed},
  };
  // The morphed brakes' cases from a generator of their own.
  Random morphed_brakes(0x6D6F72706862726Bull);  // "morphbrk"
  const auto brakes = [](Kind kind) {
    return kind == Kind::kBrakes || kind == Kind::kBrakesMorphed;
  };
  for (const Target& target : targets) {
    Random& r = target.kind == Kind::kBrakesMorphed ? morphed_brakes : wheels_random;
    Results results(target.name);
    int queued = 0, depth = 0, weights = 0, makers = 0, flips = 0, seconds = 0, morphs = 0;
    std::vector<uint8_t> scene_bytes, records;
    const std::vector<uint8_t> zero(kSlotCount * 16, 0);
    uint32_t sp = 0;
    for (int c = 0; c < target.cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            const uint32_t set = r.Below(4) ? 1 : r.Below(2) ? 0 : 2 + r.Below(4);
            const uint32_t shape = r.Below(4);
            WheelsWorld(h, r, block, race, top, shape, set);
            const bool morphed = target.kind == Kind::kTireMorphed ||
                                 target.kind == Kind::kRimAMorphed;
            if (morphed) {
              makers += MorphWorld(h, r, block, top, target.kind == Kind::kTireMorphed);
              weights += guest::LoadU32(h.base, top + kWheels) != 3;
            }
            if (target.kind == Kind::kRimA || target.kind == Kind::kRimAMorphed) {
              RimAWorld(h, r, block, top, shape, morphed);
              flips += guest::LoadU32(h.base, top + kGroup + 36) != 0;
            }
            if (target.kind == Kind::kRimB) {
              // Set b's records are set a's layout; the looks by the
              // block's +596 to +608 (1, 2 or another word).
              RimAWorld(h, r, block, top, shape, false);
              for (uint32_t at = 596; at <= 608; at += 4) {
                guest::StoreU32(h.base, source + at,
                                r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(4));
              }
              seconds += guest::LoadU32(h.base, source + 592) != 0;
            }
            if (brakes(target.kind)) {
              morphs += BrakesWorld(h, r, block, source, top, shape, makers,
                                    target.kind == Kind::kBrakesMorphed);
            }
            scene_bytes = Read(h.base, scene, kSceneBytes);
            records = Read(h.base, top, kRecordsEnd);
            sp = t.r1.u32;
            t.r3.u64 = top + (brakes(target.kind) ? kBrakesGroup : kGroup);
            t.r4.u64 = kPasses + 592 * r.Below(4);
            t.r5.u64 = source;
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - target.frame;
            o.Word(guest::LoadU32(h.base, frame));
            o.Floats(h.base, frame + 80, (target.temporaries_end - 80) / 4);
            o.Floats(h.base, top, kRecordsEnd / 4);
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t written = guest::LoadU32(h.base, block + 0x3000 + 44) - (top + kSlots);
            o.Word(written);
            o.Floats(h.base, top + kSlots, std::min(written, kSlotCount * 16) / 4);
            o.Word(uint32_t(t.r24.u64));
            o.Word(uint32_t(t.r28.u64));
            if (t.r3.u32 == block + 0x7C0) {
              queued++;
              o.Word(t.r4.u32);
              o.Word(t.r5.u32);
              o.Double(t.f1.u64);
            }
          },
          [&](int v) {
            if (v == 0) {
              const uint32_t d = scene + 5072;
              const uint32_t pass = kPasses + 592 * guest::LoadU32(h.base, d + 4);
              depth += guest::LoadU32(h.base, top + kWheels) != 2 &&
                       guest::LoadU32(h.base, pass + 588) != 0 &&
                       (guest::LoadU32(h.base, d + 60) & 0x4) != 0;
            }
            Write(h.base, scene, scene_bytes.data(), kSceneBytes);
            Write(h.base, top, records.data(), kRecordsEnd);
            Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
            Write(h.base, top + kSlots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    const bool tire = target.kind == Kind::kTire || target.kind == Kind::kTireMorphed;
    if (tire) {
      ITC_LOG_INFO("harness: {}: {} of {} cases with a depth pass, {} ended handing an entry to "
                   "the render queue",
                   target.name, depth, target.cases, queued / 2);
    } else if (brakes(target.kind)) {
      ITC_LOG_INFO("harness: {}: {} of {} cases morphed ({} with the pool maker), {} ended "
                   "handing an entry to the render queue",
                   target.name, morphs, target.cases, makers, queued / 2);
    } else if (target.kind == Kind::kRimB) {
      ITC_LOG_INFO("harness: {}: {} of {} cases with the second matrix, {} ended handing an "
                   "entry to the render queue",
                   target.name, seconds, target.cases, queued / 2);
    } else {
      ITC_LOG_INFO("harness: {}: {} of {} cases with a rim model to flip, {} ended handing an "
                   "entry to the render queue",
                   target.name, flips, target.cases, queued / 2);
    }
    if (target.kind == Kind::kTireMorphed) {
      ITC_LOG_INFO("harness: {}: {} of {} cases worked the weights out, {} with the pool maker",
                   target.name, weights, target.cases, makers);
    } else if (target.kind == Kind::kRimAMorphed) {
      ITC_LOG_INFO("harness: {}: {} of {} cases with the pool maker", target.name, makers,
                   target.cases);
    }
    results.Report();
  }
  WheelsTopDraws(h, block, race, top);
  memory::Free(allocation);
}

}  // namespace harness::car_draw_shared
