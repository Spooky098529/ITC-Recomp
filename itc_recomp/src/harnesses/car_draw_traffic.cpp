// The traffic body (src/car_draw/traffic.cpp, docs/functions/car-draw.md:
// TrafficPoints::DrawBody and DrawWindow) against the recompiled functions,
// under the car_draw harness (its Case and model draws' world,
// car_draw_shared.h; the traffic's head and tail lights are in
// car_draw_lamps.cpp). The traffic body in an allocation of its own (the
// object at +0x100, its point block at the object + 3728: the body's records
// at +16 (+4 the model, the block's +0x100, or 0, +228 the shape; the plates'
// textures at +1456 and +1460), the headlights' group at +1472 and the tail
// lights' at +3152 (their glow nodes' positions at +144); the six part
// models at +9768, copies of the model draws' model at the allocation's
// +0x3000 with their shape counts; the scene at +9796), the command list's
// slots in the allocation. The scene at the race block's +0x10000
// ([0x826C2E44] too: its +4 the set, the glow model at +480, the pass
// description at +5072, the lighting block and cone CarLighting::Gather
// reads), the light sets' owner at the race block's +0x14000, the camera
// passes 0 to 3, the scene model's block at the block's +0xF380. The
// allocation's object and models and the scene are put back before each
// version and compared after it, with the frame's back chain, r3, r24, r28
// and what went to the render queue. The recompiled side runs with
// --native_car_draw off (its callees the originals); the native side the
// plain C++ through register steps of its own.

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
constexpr uint32_t kLightOwner = 0x826C551C;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kSceneBytes = 0x2000;
constexpr uint32_t kGlowModel = 480;
// The allocation: the traffic object, the part models, the slots.
constexpr uint32_t kObject = 0x100;
constexpr uint32_t kPoints = kObject + 3728;
constexpr uint32_t kModels = 0x3000;
constexpr uint32_t kModelBytes = 0x200;
constexpr uint32_t kRecordsEnd = kModels + 6 * kModelBytes;
constexpr uint32_t kSlots = 0x8000;
// Room for every draw (the lamps' glows and the shadows' depth draws; the
// originals write through a refused slot, address 0, when the list is full).
constexpr uint32_t kSlotCount = 24000;
constexpr uint32_t kTrafficBytes = kSlots + kSlotCount * 16;
// Below the shared Case's filled stack (its 2,048 bytes), filled too: the
// callees' frames reach further down.
constexpr uint32_t kDeepStackBytes = 8192;

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A matrix near a turn about y with a small translation, or anything finite.
void RandomMatrix(uint8_t* base, uint32_t at, Random& r) {
  if (r.Below(4) == 0) {
    FiniteFloats(base, at, 16, r);
    return;
  }
  const float c = Spread(r, 1.0f), s = Spread(r, 1.0f);
  StoreFloats(base, at, {c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, Spread(r, 0.5f), Spread(r, 0.5f),
                         Spread(r, 0.5f), 1});
}

uint32_t Fraction(Random& r) {
  return r.Below(6) == 0 ? RandomFloat(r)
                         : std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f);
}

// The world; see the top. `set` the scene's +4.
void TrafficWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t top,
                  uint32_t shape, uint32_t set) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  RandomWords(base, 0x8265ECE0, 64, r);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, top + kSlots);
  guest::StoreU32(base, list + 44, top + kSlots);
  // Every shape a copy of the drawn one; the materials' total (+0 of their
  // block) the two groups' counts.
  for (uint32_t k = 0; k < 4; k++) {
    if (k != shape) {
      const std::vector<uint8_t> bytes = Read(base, block + 0x2000 + 544 * shape, 544);
      Write(base, block + 0x2000 + 544 * k, bytes.data(), 544);
    }
  }
  const uint32_t materials = block + 0x6000;
  guest::StoreU32(base, materials,
                  guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12));
  // The object: zero, then the groups' records.
  const std::vector<uint8_t> zero(kRecordsEnd, 0);
  Write(base, top, zero.data(), kRecordsEnd);
  const uint32_t object = top + kObject, points = top + kPoints;
  const uint32_t model = block + 0x100;
  struct Group {
    uint32_t at, records;
  };
  for (const Group& g : {Group{16, 6}, Group{1472, 7}, Group{3152, 12}}) {
    for (uint32_t k = 0; k < g.records; k++) {
      const uint32_t record = points + g.at + 240 * k;
      guest::StoreU32(base, record + 4, r.Below(4) == 0 ? 0 : model);
      guest::StoreU32(base, record + 228, shape);
      if (g.at == 16) {
      } else if (r.Below(8) == 0) {
        RandomFloats(base, record + 144, 4, r);
      } else {
        StoreFloats(base, record + 144, {Spread(r, 1.0f), Spread(r, 1.0f), Spread(r, 1.0f),
                                         r.Below(4) ? 1.0f : 0.0f});
      }
    }
  }
  // The plates' textures.
  guest::StoreU32(base, points + 1456, uint32_t(r.Next()));
  guest::StoreU32(base, points + 1460, uint32_t(r.Next()));
  // The six part models: copies of the model, up to 4 shapes each.
  for (uint32_t m = 0; m < 6; m++) {
    const uint32_t copy = top + kModels + kModelBytes * m;
    const std::vector<uint8_t> bytes = Read(base, model, kModelBytes);
    Write(base, copy, bytes.data(), kModelBytes);
    guest::StoreU32(base, copy + 412, r.Below(4) == 0 ? 0 : 1 + r.Below(r.Below(4) ? 2 : 4));
    guest::StoreU32(base, object + 9768 + 4 * m, copy);
  }
  // The scene: the object's and [0x826C2E44].
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  guest::StoreU32(base, object + 9796, scene);
  guest::StoreU32(base, scene + 4, set);
  const std::vector<uint8_t> glow = Read(base, model, 0x200);
  Write(base, scene + kGlowModel, glow.data(), 0x200);
  guest::StoreU32(base, scene + kGlowModel + 412,
                  r.Below(8) == 0 ? 0 : r.Below(8) == 0 ? 4 : 1 + r.Below(2));
  // The pass description: the current camera pass (+0, kept enabled: a
  // disabled one reads address 144), the shadows' (+4, +8), the keys and
  // the bits.
  const uint32_t d = scene + 5072;
  const uint32_t current = r.Below(4);
  guest::StoreU32(base, d + 0, current);
  guest::StoreU32(base, d + 4, r.Below(4));
  guest::StoreU32(base, d + 8, r.Below(4));
  RandomWords(base, d + 32, 28, r);
  guest::StoreU32(base, d + 60, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(256));
  RandomWords(base, d + 76, 8, r);
  // The camera passes.
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t pass = kPasses + 592 * p;
    RandomMatrix(base, pass + 16, r);
    if (r.Below(6) == 0) {
      RandomFloats(base, pass + 144, 16, r);
    } else {
      StoreFloats(base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    if (r.Below(8) == 0) {
      RandomFloats(base, pass + 400, 3, r);
    } else {
      StoreFloats(base, pass + 400, {Spread(r, 6.0f), Spread(r, 6.0f), Spread(r, 6.0f)});
    }
    if (r.Below(8) == 0) {
      RandomFloats(base, pass + 464, 4, r);
    } else {
      StoreFloats(base, pass + 464, {Spread(r, 1.0f), Spread(r, 1.0f), Spread(r, 1.0f), 0.0f});
    }
    guest::StoreU32(base, pass + 588,
                    p != current && r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  }
  // The scene model's block: its bits, world and alphas.
  const uint32_t source = block + kSource;
  guest::StoreU32(base, source + 0, uint32_t(r.Next()));
  guest::StoreU32(base, source + 4, uint32_t(r.Next()));
  if (r.Below(3) == 0) {
    const float c = Spread(r, 1.0f), s = Spread(r, 1.0f);
    StoreFloats(base, source + 16, {c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, Spread(r, 4.0f),
                                    Spread(r, 1.0f), Spread(r, 4.0f), 1});
  } else {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, source + 16, world.data(), 64);
  }
  for (uint32_t at : {628u, 632u, 636u}) {
    guest::StoreU32(base, source + at, Fraction(r));
  }
  // The lighting CarLighting::Gather reads (a race's shadows) and the lights
  // near the car.
  const uint32_t lighting = scene + 5248;
  FiniteFloats(base, lighting + 96, 8, r);
  FiniteFloats(base, lighting + 240, 8, r);
  guest::StoreFloat(base, lighting + 272, Spread(r, 2));
  guest::StoreU32(base, scene + 5500,
                  r.Below(8) == 0 ? RandomFloat(r)
                                  : std::bit_cast<uint32_t>(float(r.Below(1800)) / 10.0f));
  guest::StoreU32(base, scene + 5052, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
  const uint32_t owner = race + 0x14000;
  guest::StoreU32(base, kLightOwner, owner);
  const float px = guest::LoadFloat(base, source + 64), py = guest::LoadFloat(base, source + 68),
              pz = guest::LoadFloat(base, source + 72);
  if (r.Below(16) == 0) {
    guest::StoreU32(base, owner + 216, 0);  // no sets
  } else {
    LightSets(base, owner, r.Below(8) == 0 ? 0 : r.Below(17), px, py, pz, r);
  }
}

// The register steps (without the flag): r3 the records or the group, r4 the
// block, r5 the pass.
void NativeBody(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, r1 = ctx.r1.u32;
  const uint32_t r24 = ctx.r24.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::TrafficDrawBody(base, calls, r1, r3, r4, r5, r24);
  });
}

void NativeWindow(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, r1 = ctx.r1.u32;
  const uint32_t r24 = ctx.r24.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::TrafficDrawWindow(base, calls, r1, r3, r4, r5, r24);
  });
}

// Which function a target runs on: the records (point block + 16) with a
// camera pass, or the point block, or the object.
enum class On { kRecords, kPoints, kObject };

struct Target {
  const char* name;
  GuestFunction recompiled, native;
  uint32_t frame;
  On on;
  int cases;
};

}  // namespace

void TrafficDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t allocation = memory::Alloc(kTrafficBytes + 16, 0);
  const uint32_t top = (allocation + 15) & ~15u;
  const uint32_t source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const Target targets[] = {
      {"TrafficPoints::DrawBody (sub_820E55A0)", __imp__sub_820E55A0, NativeBody,
       car_draw::kTrafficBodyFrame, On::kRecords, 256},
      {"TrafficPoints::DrawWindow (sub_820E5668)", __imp__sub_820E5668, NativeWindow,
       car_draw::kTrafficWindowFrame, On::kRecords, 256},
  };
  for (const Target& target : targets) {
    Results results(target.name);
    int races = 0, queued = 0;
    std::vector<uint8_t> scene_bytes, records;
    const std::vector<uint8_t> zero(kSlotCount * 16, 0);
    uint32_t sp = 0, set = 0;
    for (int c = 0; c < target.cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            set = r.Below(4) ? 1 : r.Below(2) ? 0 : 2 + r.Below(4);
            TrafficWorld(h, r, block, race, top, r.Below(4), set);
            scene_bytes = Read(h.base, scene, kSceneBytes);
            records = Read(h.base, top, kRecordsEnd);
            sp = t.r1.u32;
            switch (target.on) {
              case On::kRecords:
                t.r3.u64 = top + kPoints + 16;
                t.r5.u64 = kPasses + 592 * r.Below(4);
                break;
              case On::kPoints:
                t.r3.u64 = top + kPoints;
                t.r5.u64 = r.Next();
                break;
              case On::kObject:
                t.r3.u64 = top + kObject;
                t.r5.u64 = r.Next();
                break;
            }
            t.r4.u64 = source;
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            o.Word(guest::LoadU32(h.base, t.r1.u32 - target.frame));
            o.Bytes(Read(h.base, top, kRecordsEnd));
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t written = guest::LoadU32(h.base, block + 0x3000 + 44) - (top + kSlots);
            o.Word(written);
            // The slots as floats (the recorder's constants, NaN where their
            // inputs were); headers and pointers aren't NaN patterns, so they
            // still compare exactly.
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
              races += set == 1;
            }
            Write(h.base, scene, scene_bytes.data(), kSceneBytes);
            Write(h.base, top, records.data(), kRecordsEnd);
            Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
            Write(h.base, top + kSlots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    ITC_LOG_INFO("harness: {}: {} of {} cases in a race, {} ended handing an entry to the "
                 "render queue",
                 target.name, races, target.cases, queued / 2);
    results.Report();
  }
  memory::Free(allocation);
}

}  // namespace harness::car_draw_shared
