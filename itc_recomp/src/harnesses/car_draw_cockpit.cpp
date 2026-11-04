// The cockpit's draws (src/car_draw/cockpit.cpp, docs/functions/car-draw.md:
// CockpitPoints::DrawFenders, DrawCabinWindow, DrawDoorWindow,
// DrawHeadlights and Draw, SceneCockpit::DrawBody and Draw) against the
// recompiled functions, under the car_draw harness (its Case and model
// draws' world, car_draw_shared.h). The interior's world
// (SharedInteriorWorld, car_draw_interior.cpp) for the cockpit's own
// interior ([cockpit + 28]), and the cockpit beside it in the same
// allocation (at +0x6000): +4316 the scene, +192 its point block with every
// record the draws read (+4 the model draws' model or 0, +228 the shape);
// for DrawBody in a race CarLighting::Gather's world (the scene's lighting
// block at +5248, the cone at +5500, +5052, the light sets' owner at the
// race block's +0x14000) as the shadows' harness builds it. The allocation
// and the scene are put back before each version and compared after it,
// with the frame's back chain, DrawBody's record (+80 to +767: the words
// exactly, the matrices as floats) and Gather's saved r3 and r5 (+20, +36),
// r3, r24, r28 and what went to the render queue. The recompiled side runs
// with --native_car_draw off; the native side the plain C++ through
// register steps of its own (no adapters: only the cockpit's top reaches
// them), SceneCockpit::Draw through its register step.

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
constexpr uint32_t kCockpit = 0x6000;
constexpr uint32_t kPoints = kCockpit + 192;
constexpr uint32_t kEnd = 0x7100;
constexpr uint32_t kDeepStackBytes = 8192;
// Room for the interior's draws and the cockpit's (the interior's 8,000
// overflowed: a full list makes the original write through guest 0).
constexpr uint32_t kSlotCount = 24000;
constexpr uint32_t kBytes = kInteriorWorldSlots + kSlotCount * 16;
static_assert(kInteriorWorldEnd <= kCockpit && kEnd <= kInteriorWorldSlots);

// The point block's records the draws read.
constexpr uint32_t kRecords[] = {0,    240,  480,  720,  960,  1200, 1440,
                                 1680, 1920, 3120, 3360, 3600, 3840};
// The groups the leaves take.
constexpr uint32_t kFenders = 0, kCabin = 720, kDoorWindow = 1440, kHeadlights = 2160;

void CockpitWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t top,
                  uint32_t shape, uint32_t set) {
  uint8_t* base = h.base;
  SharedInteriorWorld(h, r, block, race, top, shape, set);
  guest::StoreU32(base, block + 0x3000 + 8, kSlotCount);
  const std::vector<uint8_t> zero(kEnd - kCockpit, 0);
  Write(base, top + kCockpit, zero.data(), uint32_t(zero.size()));
  const uint32_t scene = race + 0x10000, cockpit = top + kCockpit;
  guest::StoreU32(base, cockpit + 4316, scene);
  guest::StoreU32(base, cockpit + 28, top + kInteriorWorldInterior);
  for (uint32_t at : kRecords) {
    const uint32_t record = top + kPoints + at;
    guest::StoreU32(base, record + 4, r.Below(4) == 0 ? 0 : block + 0x100);
    guest::StoreU32(base, record + 228, shape);
  }
  // The block's flag bits (the headlights' 0x1 and 0x40) and the windows'
  // alpha.
  const uint32_t source = block + kSource;
  guest::StoreU32(base, source + 4, uint32_t(r.Next()));
  if (r.Below(4) != 0) {
    guest::StoreFloat(base, source + 628, Spread(r, 1.0f) + 0.5f);
  }
  // CarLighting::Gather's world (DrawBody in a race).
  const float px = guest::LoadFloat(base, source + 64), py = guest::LoadFloat(base, source + 68),
              pz = guest::LoadFloat(base, source + 72);
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
  if (r.Below(16) == 0) {
    guest::StoreU32(base, owner + 216, 0);  // no sets
  } else {
    LightSets(base, owner, r.Below(8) == 0 ? 0 : r.Below(17), px, py, pz, r);
  }
}

// What to run: the function, its register steps, r3 (a group's offset in
// the point block, the point block or the cockpit), its frame.
enum class Takes { kGroup, kPoints, kCockpit };
struct Target {
  const char* name;
  GuestFunction recompiled, native;
  Takes takes;
  uint32_t group;
  uint32_t frame;
  int cases;
};

// The leaves' register steps: r3 = the group, r4 = the block, r5 = the
// camera pass.
#define ITC_COCKPIT_STEP(name)                                                \
  void NativeCockpit##name(PPCContext& ctx, uint8_t* base) {                  \
    const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32;         \
    const uint32_t r1 = ctx.r1.u32, r24 = ctx.r24.u32;                        \
    NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {               \
      return ::car_draw::CockpitDraw##name(base, calls, r1, r3, r4, r5, r24); \
    });                                                                       \
  }
ITC_COCKPIT_STEP(Fenders)
ITC_COCKPIT_STEP(CabinWindow)
ITC_COCKPIT_STEP(DoorWindow)
ITC_COCKPIT_STEP(Headlights)
#undef ITC_COCKPIT_STEP

void NativeCockpitPoints(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32, r24 = ctx.r24.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::CockpitPointsDraw(base, calls, r1, r3, r4, r24);
  });
}
void NativeCockpitBody(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32;
  const uint32_t r24 = ctx.r24.u32, r28 = ctx.r28.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::SceneCockpitDrawBody(base, calls, r1, r3, r4, r24, r28);
  });
}

}  // namespace

void CockpitDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t allocation = memory::Alloc(kBytes + 16, 0);
  const uint32_t top = (allocation + 15) & ~15u;
  const uint32_t source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const uint32_t slots = top + kInteriorWorldSlots;
  const Target targets[] = {
      {"CockpitPoints::DrawFenders (sub_820DB8C8)", __imp__sub_820DB8C8, NativeCockpitFenders,
       Takes::kGroup, kFenders, car_draw::kCockpitSmallFrame, 128},
      {"CockpitPoints::DrawCabinWindow (sub_820DB980)", __imp__sub_820DB980,
       NativeCockpitCabinWindow, Takes::kGroup, kCabin, car_draw::kCockpitSmallFrame, 128},
      {"CockpitPoints::DrawDoorWindow (sub_820DBA38)", __imp__sub_820DBA38,
       NativeCockpitDoorWindow, Takes::kGroup, kDoorWindow, car_draw::kCockpitSmallFrame, 128},
      {"CockpitPoints::DrawHeadlights (sub_820DBB88)", __imp__sub_820DBB88,
       NativeCockpitHeadlights, Takes::kGroup, kHeadlights, car_draw::kCockpitSmallFrame, 128},
      {"CockpitPoints::Draw (sub_820DBD38)", __imp__sub_820DBD38, NativeCockpitPoints,
       Takes::kPoints, 0, car_draw::kCockpitPointsFrame, 192},
      {"SceneCockpit::DrawBody (sub_820DC350)", __imp__sub_820DC350, NativeCockpitBody,
       Takes::kCockpit, 0, car_draw::kCockpitBodyFrame, 256},
      {"SceneCockpit::Draw (sub_820DC498)", __imp__sub_820DC498, hooks::CarDrawSceneCockpit,
       Takes::kCockpit, 0, car_draw::kCockpitBodyFrame, 256},
  };
  std::vector<uint8_t> scene_bytes, records;
  const std::vector<uint8_t> zero(kSlotCount * 16, 0);
  for (const Target& target : targets) {
    Results results(target.name);
    uint32_t sp = 0;
    int queued = 0, races = 0;
    uint32_t most = 0;
    uint32_t set = 0;
    for (int c = 0; c < target.cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            // The scene's set: the cockpit's draw is a race's (1) mostly,
            // now and then the front end's (0) or above 1.
            set = r.Below(8) == 0 ? 2 + r.Below(4) : r.Below(4) == 0 ? 0 : 1;
            CockpitWorld(h, r, block, race, top, r.Below(4), set);
            if (set == 1) {
              races++;
            }
            scene_bytes = Read(h.base, scene, kSceneBytes);
            records = Read(h.base, top, kEnd);
            sp = t.r1.u32;
            t.r3.u64 = target.takes == Takes::kCockpit ? top + kCockpit
                       : target.takes == Takes::kPoints ? top + kPoints
                                                        : top + kPoints + target.group;
            t.r4.u64 = source;
            t.r5.u64 = kPasses + 592 * r.Below(4);
            t.r6.u64 = r.Next();
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - target.frame;
            o.Word(guest::LoadU32(h.base, frame));
            if (target.takes == Takes::kCockpit) {
              // Gather's saved r3 and r5, the record's words and matrices.
              o.Word(guest::LoadU32(h.base, frame + 20));
              o.Word(guest::LoadU32(h.base, frame + 36));
              o.Bytes(Read(h.base, frame + 80, 48));
              o.Floats(h.base, frame + 128, 640 / 4);
            }
            o.Bytes(Read(h.base, top, kEnd));
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t written = guest::LoadU32(h.base, block + 0x3000 + 44) - slots;
            o.Word(written);
            most = std::max(most, written / 16);
            o.Floats(h.base, slots, std::min(written, kSlotCount * 16) / 4);
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
            Write(h.base, scene, scene_bytes.data(), kSceneBytes);
            Write(h.base, top, records.data(), kEnd);
            Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
            Write(h.base, slots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    ITC_LOG_INFO("harness: {}: {} of {} cases in a race, {} ended handing an entry to the "
                 "render queue, at most {} slots written",
                 target.name, races, target.cases, queued / 2, most);
    results.Report();
  }
  memory::Free(allocation);
}

}  // namespace harness::car_draw_shared
