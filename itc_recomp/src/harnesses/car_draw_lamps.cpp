// The head and tail lights (src/car_draw/lamps.cpp, docs/functions/car-draw.md)
// against the recompiled functions, under the car_draw harness (its Case and
// model draws' world, car_draw_shared.h). The lights' group at the block's
// +0xC000 (records of 240 bytes: +4 the model, the block's +0x100, or 0,
// +228 the shape, +144 the glow nodes' positions; the headlights' pop-up
// matrix at +3120), the scene model's block at +0xF380 (its flag bits, its
// world at +16, the covers' alpha at +636), the scene at the race block's
// +0x10000 with the glow model at its +480 (a copy of the model draws'
// model, its +412 the shape count, every shape a copy of the drawn one), the
// camera passes 0 to 3 (their view at +16, view-projection at +144, eye at
// +400 and direction at +464). The scene is put back before each version and
// compared after it, with the frame's temporaries, r3, r24, r28 and what
// went to the render queue. The recompiled side runs with --native_car_draw
// off (its CarPart::Draw and Model::Draw calls run the originals).

#include "harness.h"
#include "harnesses/car_draw_shared.h"

#include "base/log.h"
#include "hooks/car_draw.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <vector>

namespace harness::car_draw_shared {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kPasses = 0x826BAC00;
constexpr uint32_t kGroup = 0xC000;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kSceneBytes = 0x2000;
constexpr uint32_t kGlowModel = 480;
constexpr uint32_t kSlots = 0x20000;
// Room for every draw (up to 24 glow draws of up to 7 materials; the
// originals write through a refused slot, address 0, when the list is full).
constexpr uint32_t kSlotCount = 12000;
// The frames' temporaries compared (from +80; the register saves above).
constexpr uint32_t kTemporariesEnd = 1424;
// Below the shared Case's filled stack (its 2,048 bytes), filled too: the
// projector's frames reach further down.
constexpr uint32_t kDeepStackBytes = 2048;

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

void LampsWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t shape,
                bool head) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  RandomWords(base, 0x8265ECE0, 64, r);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, race + kSlots);
  guest::StoreU32(base, list + 44, race + kSlots);
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
  // The group's records, the glow nodes' positions and the pop-up matrix.
  const uint32_t group = block + kGroup;
  for (uint32_t k = 0; k < 14; k++) {
    const uint32_t record = group + 240 * k;
    guest::StoreU32(base, record + 4, r.Below(4) == 0 ? 0 : block + 0x100);
    guest::StoreU32(base, record + 228, shape);
    if (r.Below(8) == 0) {
      RandomFloats(base, record + 144, 4, r);
    } else {
      StoreFloats(base, record + 144,
                  {Spread(r, 1.0f), Spread(r, 1.0f), Spread(r, 1.0f), r.Below(4) ? 1.0f : 0.0f});
    }
  }
  // The headlights' pop-up matrix (over record 13, the tail lights' cover).
  if (!head) {
  } else if (r.Below(2)) {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, group + 3120, world.data(), 64);
  } else {
    RandomMatrix(base, group + 3120, r);
  }
  // The scene and its glow model.
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  const std::vector<uint8_t> model = Read(base, block + 0x100, 0x200);
  Write(base, scene + kGlowModel, model.data(), 0x200);
  guest::StoreU32(base, scene + kGlowModel + 412,
                  r.Below(8) == 0 ? 0 : r.Below(8) == 0 ? 4 : 1 + r.Below(2));
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
  }
  // The scene model's block: its bits, world and the covers' alpha.
  const uint32_t source = block + kSource;
  guest::StoreU32(base, source + 0, uint32_t(r.Next()));
  guest::StoreU32(base, source + 4, uint32_t(r.Next()));
  if (r.Below(3) == 0) {
    RandomMatrix(base, source + 16, r);
  } else {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, source + 16, world.data(), 64);
  }
  guest::StoreU32(base, source + 636,
                  r.Below(6) == 0 ? RandomFloat(r)
                                  : std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f));
}

struct Target {
  const char* name;
  GuestFunction recompiled, native;
  uint32_t frame;
  bool head;
};

}  // namespace

void LampsDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t group = block + kGroup, source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const Target targets[] = {
      {"PartsPoints::DrawTailLights (sub_820DF8B0)", __imp__sub_820DF8B0,
       hooks::CarDrawPartsTailLights, car_draw::kPartsTailLightsFrame, false},
      {"PartsPoints::DrawHeadlights (sub_820E1690)", __imp__sub_820E1690,
       hooks::CarDrawPartsHeadlights, car_draw::kPartsHeadlightsFrame, true},
  };
  for (const Target& target : targets) {
    Results results(target.name);
    const int cases = 512;
    int queued = 0, glows = 0;
    std::vector<uint8_t> scene_bytes;
    const std::vector<uint8_t> zero(kSlotCount * 16, 0);
    uint32_t sp = 0;
    for (int c = 0; c < cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            LampsWorld(h, r, block, race, r.Below(4), target.head);
            scene_bytes = Read(h.base, scene, kSceneBytes);
            sp = t.r1.u32;
            t.r3.u64 = group;
            t.r4.u64 = source;
            t.r5.u64 = kPasses + 592 * r.Below(4);
            t.r6.u64 = r.Below(4);
            t.r7.u64 = r.Below(4);
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - target.frame;
            o.Word(guest::LoadU32(h.base, frame));
            o.Bytes(Read(h.base, frame + 80, 160 - 80));
            o.Floats(h.base, frame + 160, 8);
            o.Bytes(Read(h.base, frame + 192, 208 - 192));
            o.Floats(h.base, frame + 208, (kTemporariesEnd - 208) / 4);
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t written = guest::LoadU32(h.base, block + 0x3000 + 44) - (race + kSlots);
            o.Word(written);
            // The slots as floats: they hold the recorder's constants (the
            // glows' world, NaN where its inputs were: which NaN a product of
            // two keeps is the compiler's choice); the headers and pointers
            // around them are never NaN patterns, so they still compare
            // exactly.
            o.Floats(h.base, race + kSlots, std::min(written, kSlotCount * 16) / 4);
            o.Word(uint32_t(t.r24.u64));
            o.Word(uint32_t(t.r28.u64));
            if (guest::LoadU32(h.base, frame + 132) == 0xFFFFFFFFu) {
              glows++;
            }
            if (t.r3.u32 == block + 0x7C0) {
              queued++;
              o.Word(t.r4.u32);
              o.Word(t.r5.u32);
              o.Double(t.f1.u64);
            }
          },
          [&](int) {
            Write(h.base, scene, scene_bytes.data(), kSceneBytes);
            Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
            Write(h.base, race + kSlots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    ITC_LOG_INFO("harness: {}: {} of {} cases colored the glows, {} ended handing an entry to "
                 "the render queue",
                 target.name, glows / 2, cases, queued / 2);
    results.Report();
  }
}

}  // namespace harness::car_draw_shared
