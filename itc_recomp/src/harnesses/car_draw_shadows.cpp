// The shadows (src/car_draw/shadows.cpp, docs/functions/car-draw.md:
// PartsPoints::DrawShadows) against the recompiled function, under the
// car_draw harness (its Case and model draws' world, car_draw_shared.h).
// The shadows group at the block's +0xC000 (record 0's model +4, the model
// draws' model or now and then 0, and shape +228; record 3's model word
// +724 and shape +948), the scene model's block at +0xF380 (its world at
// +16), the scene at the race block's +0x10000 (its +4: a race when 1; the
// lighting block at +5248 and the cone at +5500 CarLighting::Gather reads;
// the pass description at +5072 with its camera passes at +4 and +8, keys
// at +32 to +48, bits at +60, saved keys at +76 and +80), the light sets'
// owner at the race block's +0x14000 ([0x826C551C]), the camera passes 0 to
// 3, the command list's slots at the race block's +0x20000. Outside a race
// the gathered words are the filled stack's (the same for both versions),
// as the original reads them. The scene is put back before each version and
// compared after it, with the frame's temporaries, the gather's saved r3
// and r5, r3 and what went to the render queue. The recompiled side runs
// with --native_car_draw off (its Gather and DrawShapeDepth the originals or
// their own adapters), the native side the plain C++ through a register step
// of its own (the adapter is gone).

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
constexpr uint32_t kLightOwner = 0x826C551C;
constexpr uint32_t kGroup = 0xC000;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kSceneBytes = 0x2000;
constexpr uint32_t kSlots = 0x20000;
// Room for every draw (up to 20 depth draws).
constexpr uint32_t kSlotCount = 12000;
// Below the shared Case's filled stack (its 2,048 bytes), filled too:
// Gather's and FindNearest's frames reach further down.
constexpr uint32_t kDeepStackBytes = 4096;

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

void ShadowsWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t shape) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, race + kSlots);
  guest::StoreU32(base, list + 44, race + kSlots);
  // The group: records 0 and 3 (only their model words and shapes).
  const uint32_t group = block + kGroup;
  guest::StoreU32(base, group + 4, r.Below(8) == 0 ? 0 : block + 0x100);
  guest::StoreU32(base, group + 228, shape);
  guest::StoreU32(base, group + 724, r.Below(8) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  guest::StoreU32(base, group + 948, r.Below(4));
  // The block's world: the model draws' (near the origin), or a turn with a
  // translation.
  const uint32_t source = block + kSource;
  if (r.Below(3) == 0) {
    const float c = Spread(r, 1.0f), s = Spread(r, 1.0f);
    StoreFloats(base, source + 16, {c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, Spread(r, 4.0f),
                                    Spread(r, 1.0f), Spread(r, 4.0f), 1});
  } else {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, source + 16, world.data(), 64);
  }
  const float px = guest::LoadFloat(base, source + 64), py = guest::LoadFloat(base, source + 68),
              pz = guest::LoadFloat(base, source + 72);
  // The scene: a race (1) mostly, else the front end (0) or anything.
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  guest::StoreU32(base, scene + 4,
                  r.Below(4) ? 1 : r.Below(2) ? 0 : uint32_t(r.Next()));
  const uint32_t lighting = scene + 5248;
  FiniteFloats(base, lighting + 96, 8, r);
  FiniteFloats(base, lighting + 240, 8, r);
  guest::StoreFloat(base, lighting + 272, Spread(r, 2));
  guest::StoreU32(base, scene + 5500,
                  r.Below(8) == 0 ? RandomFloat(r)
                                  : std::bit_cast<uint32_t>(float(r.Below(1800)) / 10.0f));
  guest::StoreU32(base, scene + 5052, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
  // The lights near the car.
  const uint32_t owner = race + 0x14000;
  guest::StoreU32(base, kLightOwner, owner);
  if (r.Below(16) == 0) {
    guest::StoreU32(base, owner + 216, 0);  // no sets
  } else {
    LightSets(base, owner, r.Below(8) == 0 ? 0 : r.Below(17), px, py, pz, r);
  }
  // The pass description: its two camera passes, keys and bits.
  const uint32_t d = scene + 5072;
  guest::StoreU32(base, d + 4, r.Below(4));
  guest::StoreU32(base, d + 8, r.Below(4));
  RandomWords(base, d + 32, 20, r);
  guest::StoreU32(base, d + 60, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(256));
  RandomWords(base, d + 76, 8, r);
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t pass = kPasses + 592 * p;
    if (r.Below(6) == 0) {
      RandomFloats(base, pass + 144, 16, r);
    } else {
      StoreFloats(base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    guest::StoreU32(base, pass + 588, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  }
}

// The shadows' register steps (its adapter is gone: its caller, the race's
// parts' top, is plain C++): r3 the group, r4 the block.
void NativeShadows(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::PartsDrawShadows(base, calls, r1, r3, r4);
  });
}

}  // namespace

void ShadowsDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t group = block + kGroup, source = block + kSource;
  const uint32_t scene = race + 0x10000;
  Results results("PartsPoints::DrawShadows (sub_820E1D88)");
  const int cases = 768;
  int queued = 0, races = 0, recorded = 0, lit = 0;
  std::vector<uint8_t> scene_bytes;
  const std::vector<uint8_t> zero(kSlotCount * 16, 0);
  uint32_t sp = 0;
  for (int c = 0; c < cases; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          SetFlushOnEntry(r.Below(2) != 0);
          ShadowsWorld(h, r, block, race, r.Below(4));
          scene_bytes = Read(h.base, scene, kSceneBytes);
          sp = t.r1.u32;
          t.r3.u64 = group;
          t.r4.u64 = source;
          t.r5.u64 = kPasses + 592 * r.Below(4);
          t.r6.u64 = r.Below(2);
          t.r24.u64 = r.Next();
          t.r28.u64 = r.Next();
        },
        __imp__sub_820E1D88, NativeShadows, AlphaFloats(),
        fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& o) {
          const uint32_t frame = t.r1.u32 - car_draw::kPartsShadowsFrame;
          o.Word(guest::LoadU32(h.base, frame));
          // The gather's r3 and r5 saved in this frame.
          o.Word(guest::LoadU32(h.base, frame + 20));
          o.Word(guest::LoadU32(h.base, frame + 36));
          // The world, view-projections and products (+80 to +1040), the
          // gathered words (+1040 to +1088), the shadow matrices.
          o.Floats(h.base, frame + 80, (1040 - 80) / 4);
          o.Bytes(Read(h.base, frame + 1040, 48));
          o.Floats(h.base, frame + 1088, 640 / 4);
          o.Bytes(Read(h.base, scene, kSceneBytes));
          const uint32_t written =
              guest::LoadU32(h.base, block + 0x3000 + 44) - (race + kSlots);
          o.Word(written);
          if (written != 0) {
            recorded++;
          }
          // A light's shadow set drawn: its first product's w words are 1.0.
          for (uint32_t at : {592u + 12, 912u + 12, 400u + 12}) {
            if (guest::LoadU32(h.base, frame + at) == 0x3F800000u && written != 0) {
              lit++;
              break;
            }
          }
          // The slots as floats (the recorder's constants: the products,
          // NaN where their inputs were); headers and pointers aren't NaN
          // patterns, so they still compare exactly.
          o.Floats(h.base, race + kSlots, std::min(written, kSlotCount * 16) / 4);
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
          if (v == 0 && guest::LoadU32(h.base, scene + 4) == 1) {
            races++;
          }
          Write(h.base, scene, scene_bytes.data(), kSceneBytes);
          Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
          Write(h.base, race + kSlots, zero.data(), uint32_t(zero.size()));
        },
        true);
  }
  ITC_LOG_INFO("harness: PartsPoints::DrawShadows: {} of {} cases in a race, {} recorded "
               "depth draws, {} probably with a light's shadow, {} ended handing an entry to "
               "the render queue",
               races, cases, recorded / 2, lit / 2, queued / 2);
  results.Report();
}

}  // namespace harness::car_draw_shared
