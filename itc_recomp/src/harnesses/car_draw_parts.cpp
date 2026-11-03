// The parts' small draws (src/car_draw/parts.cpp, docs/functions/car-draw.md)
// against the recompiled functions, under the car_draw harness (its Case and
// model draws' world, car_draw_shared.h). The parts' group at the block's
// +0xC000 (records of 240 bytes: +4 the model, the block's +0x100, or 0, +228
// the shape; the door's matrix +6016, the parts record's pointer +6084 and
// +5520, the bumpers' plate words), the scene model's block at +0xF380 (its
// flag bits, its world at +16 the model draws' world, its alphas and the
// bonnet's angle), the parts record at +0xF700, the bumpers' car record's
// +3684 at +0xF800; the scene at the race block's +0x10000 (its +4, the
// pass description at +5072, the XPR at +4648 with its records at
// +0x11800), the race mode at the race block's +0xA3AF4, the command list's
// slots at the race block's +0x20000 (room for every draw: the originals
// write through a refused slot, address 0, when the list is full), the camera
// passes 0 to 3. The race block's scene and slots are put back before each
// version and compared after it, with the frames' temporaries, r3 and what
// went to the render queue. The recompiled side runs with --native_car_draw
// off (its CarPart::Draw calls the originals).

#include "harness.h"
#include "harnesses/car_draw_shared.h"

#include "base/log.h"
#include "hooks/car_draw.h"
#include "scenery_lists/scenery_lists.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <vector>

namespace harness::car_draw_shared {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kPlateFlag = 0x82816E3C;
constexpr uint32_t kPasses = 0x826BAC00;
constexpr uint32_t kGroup = 0xC000;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kRecord = 0xF700;
constexpr uint32_t kCarRecord = 0xF800;
constexpr uint32_t kSlots = 0x20000;
constexpr uint32_t kSlotCount = 4000;
constexpr uint32_t kSceneBytes = 0x2000;

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The parts' world; see the top (a quarter of the records without a model).
enum class Plate { kNone, kFront, kRear };
void PartsWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t shape,
                Plate plate) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  RandomWords(base, 0x8265ECE0, 64, r);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, race + kSlots);
  guest::StoreU32(base, list + 44, race + kSlots);
  const uint32_t group = block + kGroup, source = block + kSource;
  for (uint32_t k = 0; k < 25; k++) {
    const uint32_t part = group + 240 * k;
    guest::StoreU32(base, part + 4, r.Below(4) == 0 ? 0 : block + 0x100);
    guest::StoreU32(base, part + 228, shape);
  }
  // The door's matrix (the model draws' world, or anything).
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(base, group + 6016 + i, guest::LoadU32(base, block + 0x500 + i));
  }
  if (r.Below(6) == 0) {
    FiniteFloats(base, group + 6016, 16, r);
  }
  const uint32_t record = block + kRecord;
  guest::StoreU32(base, group + 6084, record);
  guest::StoreU32(base, group + 5520, record);
  guest::StoreU32(base, record + 28, r.Below(11));
  guest::StoreU32(base, record + 12,
                  r.Below(4) == 0 ? uint32_t(int32_t(r.Below(33)) - 12) : r.Below(11));
  // The bumpers' plates (over records 11 and 7, which the other groups
  // draw): the front's flag, texture and car record (its +3684 at
  // +0xF800); the rear's flag and texture, and always a plate model
  // (without one the original writes guest words 456 and 432, unmapped
  // here).
  if (plate == Plate::kFront) {
    guest::StoreU32(base, group + 2640, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
    guest::StoreU32(base, group + 2644, uint32_t(r.Next()));
    guest::StoreU32(base, group + 2648, block + kCarRecord - 3684);
    guest::StoreU32(base, block + kCarRecord, r.Below(3));
  } else if (plate == Plate::kRear) {
    guest::StoreU32(base, group + 1680, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
    guest::StoreU32(base, group + 1684, uint32_t(r.Next()));
    guest::StoreU32(base, group + 1444, block + 0x100);
  }
  guest::StoreU32(base, kPlateFlag, r.Below(3) == 0 ? 1 + r.Below(3) : 0);
  guest::StoreU32(base, kRaceData, race);
  guest::StoreU32(base, race + 0xA3AF4, r.Below(3) == 0 ? 7 + r.Below(2) : r.Below(10));
  // The scene: its +4 (the front end 0), the XPR, the pass description.
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  guest::StoreU32(base, scene + 4, r.Below(2) ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  const uint32_t xpr = scene + 4648;
  guest::StoreU32(base, xpr + 0, uint32_t(r.Next()));
  guest::StoreU32(base, xpr + 16, race + 0x11800);
  guest::StoreU32(base, xpr + 20, r.Below(4));
  RandomWords(base, race + 0x11800, 64, r);
  const uint32_t description = scene + 5072;
  guest::StoreU32(base, description + 16, r.Below(4));
  guest::StoreU32(base, description + 56, uint32_t(r.Next()));
  guest::StoreU32(base, description + 60,
                  r.Below(4) == 0 ? uint32_t(r.Next()) & ~0x100u : uint32_t(r.Next()) | 0x100u);
  guest::StoreU32(base, description + 76, uint32_t(r.Next()));
  guest::StoreU32(base, description + 80, uint32_t(r.Next()));
  if (r.Below(3) == 0) {
    StoreFloats(base, description + 112, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
  } else {
    FiniteFloats(base, description + 112, 16, r);
  }
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t pass = kPasses + 592 * p;
    if (r.Below(6) == 0) {
      RandomFloats(base, pass + 144, 16, r);
    } else {
      StoreFloats(base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    guest::StoreU32(base, pass + 588, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  }
  // The scene model's block: its bits, world, alphas and the bonnet's angle.
  guest::StoreU32(base, source + 0, uint32_t(r.Next()));
  guest::StoreU32(base, source + 4, uint32_t(r.Next()));
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(base, source + 16 + i, guest::LoadU32(base, block + 0x500 + i));
  }
  for (uint32_t at : {628u, 632u, 636u}) {
    guest::StoreU32(base, source + at,
                    r.Below(6) == 0 ? RandomFloat(r)
                                    : std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f));
  }
  guest::StoreU32(base, source + 640,
                  r.Below(6) == 0 ? RandomFloat(r)
                                  : std::bit_cast<uint32_t>(Spread(r, 4.0f) + 1.5f));
  // The bonnet's hinge (record 0's +16 and +80 matrices): near a turn, or
  // anything finite.
  if (r.Below(3) == 0) {
    FiniteFloats(base, group + 16, 32, r);
  } else {
    const float y = Spread(r, 1.0f), z = Spread(r, 1.0f);
    StoreFloats(base, group + 16, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, y, z, 1});
    StoreFloats(base, group + 80, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -y, -z, 1});
  }
}

struct Target {
  const char* name;
  GuestFunction recompiled, native;
  uint32_t frame;      // its frame's bytes
  uint32_t temporary;  // its frame's temporaries (from +80, as floats), bytes
  bool record;         // r3 a record (the mirrors), not a group
  int cases;
};

}  // namespace

void PartsDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t group = block + kGroup, source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const Target targets[] = {
      {"PartsPoints::DrawFrontBumperLights (sub_820DE1C8)", __imp__sub_820DE1C8,
       hooks::CarDrawPartsFrontBumperLights, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawRearBumperLights (sub_820DE3C0)", __imp__sub_820DE3C0,
       hooks::CarDrawPartsRearBumperLights, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawLeftDoorPanel (sub_820DE6E0)", __imp__sub_820DE6E0,
       hooks::CarDrawPartsLeftDoorPanel, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawDoorWindows02 (sub_820DE760)", __imp__sub_820DE760,
       hooks::CarDrawPartsDoorWindows02, car_draw::kPartsSmallFrame, 0, false, 256},
      {"PartsPoints::DrawRightDoorPanel (sub_820DE8C8)", __imp__sub_820DE8C8,
       hooks::CarDrawPartsRightDoorPanel, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawDoorWindows01 (sub_820DE948)", __imp__sub_820DE948,
       hooks::CarDrawPartsDoorWindows01, car_draw::kPartsSmallFrame, 0, false, 256},
      {"PartsPoints::DrawDoorsBody (sub_820DEA88)", __imp__sub_820DEA88,
       hooks::CarDrawPartsDoorsBody, car_draw::kPartsSmallFrame, 0, false, 256},
      {"PartsPoints::DrawDoorsWindows (sub_820DEAF0)", __imp__sub_820DEAF0,
       hooks::CarDrawPartsDoorsWindows, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawFendersBody (sub_820DED88)", __imp__sub_820DED88,
       hooks::CarDrawPartsFendersBody, car_draw::kPartsSmallFrame, 0, false, 256},
      {"PartsPoints::DrawFenderLights (sub_820DEDD8)", __imp__sub_820DEDD8,
       hooks::CarDrawPartsFenderLights, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawCabinWindow02 (sub_820DEF38)", __imp__sub_820DEF38,
       hooks::CarDrawPartsCabinWindow02, car_draw::kPartsSmallFrame, 0, false, 256},
      {"PartsPoints::DrawCabinWindow01 (sub_820DEFA8)", __imp__sub_820DEFA8,
       hooks::CarDrawPartsCabinWindow01, car_draw::kPartsFrame, 0, false, 256},
      {"PartsPoints::DrawEngine (sub_820DF070)", __imp__sub_820DF070, hooks::CarDrawPartsEngine,
       car_draw::kPartsSmallFrame, 0, false, 256},
      {"PartsPoints::DrawFrontBumper (sub_820DF308)", __imp__sub_820DF308,
       hooks::CarDrawPartsFrontBumper, car_draw::kPartsSmallFrame, 0, false, 384},
      {"PartsPoints::DrawRearBumper (sub_820DF458)", __imp__sub_820DF458,
       hooks::CarDrawPartsRearBumper, car_draw::kPartsSmallFrame, 0, false, 384},
      {"PartsPoints::DrawBonnet (sub_820E0998)", __imp__sub_820E0998, hooks::CarDrawPartsBonnet,
       car_draw::kPartsBonnetFrame, 400, false, 384},
      {"PartsPoints::DrawMirror (sub_820E1138)", __imp__sub_820E1138, hooks::CarDrawPartsMirror,
       car_draw::kPartsShadowFrame, 128, true, 384},
      {"PartsPoints::DrawWithShadow (sub_820E1260)", __imp__sub_820E1260,
       hooks::CarDrawPartsWithShadow, car_draw::kPartsShadowFrame, 128, true, 384},
  };
  for (const Target& target : targets) {
    Results results(target.name);
    const Plate plate = target.recompiled == __imp__sub_820DF308   ? Plate::kFront
                        : target.recompiled == __imp__sub_820DF458 ? Plate::kRear
                                                                   : Plate::kNone;
    int queued = 0;
    std::vector<uint8_t> scene_bytes;
    for (int c = 0; c < target.cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            PartsWorld(h, r, block, race, r.Below(4), plate);
            scene_bytes = Read(h.base, scene, kSceneBytes);
            t.r3.u64 = target.record ? group + 240 * r.Below(3) : group;
            t.r4.u64 = source;
            t.r5.u64 = kPasses + 592 * r.Below(4);
            // r6: the lights' choice (the parts record's +56), or the
            // mirror's world.
            if (target.recompiled == __imp__sub_820E1138) {
              t.r6.u64 = r.Below(2) ? source + 16 : group + 6016;
            } else {
              t.r6.u64 = r.Below(2) ? 0 : r.Below(4) == 0 ? r.Next() : 1;
            }
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - target.frame;
            o.Word(guest::LoadU32(h.base, frame));
            if (target.temporary != 0) {
              o.Floats(h.base, frame + 80, target.temporary / 4);
            }
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t list = block + 0x3000;
            const uint32_t written = guest::LoadU32(h.base, list + 44) - (race + kSlots);
            o.Word(written);
            o.Bytes(Read(h.base, race + kSlots, std::min(written, kSlotCount * 16)));
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
            std::vector<uint8_t> zero(kSlotCount * 16, 0);
            Write(h.base, race + kSlots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    ITC_LOG_INFO("harness: {}: {} of {} cases ended handing an entry to the render queue",
                 target.name, queued / 2, target.cases);
    results.Report();
  }
}

}  // namespace harness::car_draw_shared
