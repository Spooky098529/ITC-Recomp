// The parts' top (src/car_draw/parts.cpp, docs/functions/car-draw.md:
// SceneParts::Draw with PartsPoints::DrawFrontEnd and DrawRace) against the
// recompiled function, under the car_draw harness (its Case and model draws'
// world, car_draw_shared.h). The parts object and its point block (pb, the
// object + 176) in an allocation of their own, with every group the two lists
// draw at its offset (records of 240 bytes: +4 the model, the block's +0x100,
// or 0, +228 the shape, +144 the lamps' glow nodes; the hinges' matrices; the
// doors' matrices +6016, car record +6080 and parts record +6084, set b's
// doors' parts record +5520; the bumpers' plate words; the headlights' pop-up
// matrix +3120 and angle +3184; the shadows' records 0 and 3), the scene
// model's record (pb + 37072, its kind), the car record (pb + 37076: +0 the
// headlights' color index, +3684 the plate, +3688 a scissor door) and the
// parts record (pb + 37080: +12, +28 the doors' designs, +32 the color
// sub-index, +56 the lights' choice) beside it, the object's scene (+37284)
// the scene at the race block's +0x10000 ([0x826C2E44] too: its +4 the set,
// 0 the front end, 1 the race; the XPR, the glow model at +480, the pass
// description at +5072, the lighting block and cone CarLighting::Gather
// reads), the light sets' owner at the race block's +0x14000, the camera
// passes 0 to 3 and the command list's slots in the allocation. The scene
// model's block at the block's +0xF380. The allocation's object and records
// and the scene are put back before each version and compared after it, with
// the frame's back chain and the mirrors' world (+80), r3, r24, r28 and what
// went to the render queue. The callees' frames below are their own cases'
// (car_draw_parts.cpp, car_draw_lamps.cpp, car_draw_shadows.cpp): the
// originals' saved registers sit between their temporaries. The recompiled
// side runs with --native_car_draw off (its callees the originals).

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
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kPlateFlag = 0x82816E3C;
constexpr uint32_t kPasses = 0x826BAC00;
constexpr uint32_t kLightOwner = 0x826C551C;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kSceneBytes = 0x2000;
constexpr uint32_t kGlowModel = 480;
// The allocation: the parts object, the records, the slots.
constexpr uint32_t kObject = 0x100;
constexpr uint32_t kPoints = kObject + 176;
constexpr uint32_t kCar = 0xA000;
constexpr uint32_t kPartsRecord = 0xB000;
constexpr uint32_t kSceneModelRecord = 0xB100;
constexpr uint32_t kRecordsEnd = 0xB200;
constexpr uint32_t kSlots = 0x10000;
// Room for every draw (both lists' parts and the lamps' glows; the
// originals write through a refused slot, address 0, when the list is full).
constexpr uint32_t kSlotCount = 24000;
constexpr uint32_t kTopBytes = kSlots + kSlotCount * 16;
// Below the shared Case's filled stack (its 2,048 bytes), filled too: the
// callees' frames reach further down.
constexpr uint32_t kDeepStackBytes = 8192;

// The groups: their offsets in pb and how many records each one's draws read.
struct Group {
  uint32_t at, records;
  bool glows;
};
constexpr Group kGroups[] = {
    {0, 11, false},      // the front bumper (its plate records 8 to 10)
    {2656, 1, false},    // the cooling
    {2896, 7, false},    // the rear bumper (record 6 its plate)
    {4592, 1, false},    // the side skirts
    {4832, 1, false},    // the spoiler
    {5072, 1, false},    // rs**_hb
    {5312, 1, false},    // rs**_hb_01
    {5552, 2, false},    // the bonnet
    {6032, 25, false},   // the left door
    {12128, 25, false},  // the right door
    {18224, 23, false},  // set b's doors
    {23760, 1, false},   // the left mirror
    {24000, 1, false},   // the right mirror
    {24240, 1, false},   // set b's mirror
    {24480, 13, true},   // the headlights (their pop-up matrix over record 13)
    {27680, 14, true},   // the tail lights
    {31040, 1, false},   // the tail lights' body
    {31280, 7, false},   // the fenders
    {32960, 4, false},   // the cabin's windows
    {33920, 1, false},   // the cabin's body
    {34160, 2, false},   // the engine
    {35856, 1, false},   // the muffler
};
constexpr uint32_t kShadows = 36096;
constexpr uint32_t kDoors[] = {6032, 12128};
constexpr uint32_t kHeadlights = 24480;

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

uint32_t Angle(Random& r) {
  return r.Below(6) == 0 ? RandomFloat(r) : std::bit_cast<uint32_t>(Spread(r, 4.0f) + 1.5f);
}

// The world; see the top. `set` the scene's +4.
void TopWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t top, uint32_t shape,
              uint32_t set) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  RandomWords(base, 0x8265ECE0, 64, r);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, top + kSlots);
  guest::StoreU32(base, list + 44, top + kSlots);
  // Every shape a copy of the drawn one (the glow model's shapes); the
  // materials' total (+0 of their block) the two groups' counts.
  for (uint32_t k = 0; k < 4; k++) {
    if (k != shape) {
      const std::vector<uint8_t> bytes = Read(base, block + 0x2000 + 544 * shape, 544);
      Write(base, block + 0x2000 + 544 * k, bytes.data(), 544);
    }
  }
  const uint32_t materials = block + 0x6000;
  guest::StoreU32(base, materials,
                  guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12));
  // The object and records: zero, then the groups' records.
  const std::vector<uint8_t> zero(kRecordsEnd, 0);
  Write(base, top, zero.data(), kRecordsEnd);
  const uint32_t points = top + kPoints;
  const uint32_t model = block + 0x100;
  for (const Group& g : kGroups) {
    for (uint32_t k = 0; k < g.records; k++) {
      const uint32_t record = points + g.at + 240 * k;
      guest::StoreU32(base, record + 4, r.Below(4) == 0 ? 0 : model);
      guest::StoreU32(base, record + 228, shape);
      if (!g.glows) {
      } else if (r.Below(8) == 0) {
        RandomFloats(base, record + 144, 4, r);
      } else {
        StoreFloats(base, record + 144, {Spread(r, 1.0f), Spread(r, 1.0f), Spread(r, 1.0f),
                                         r.Below(4) ? 1.0f : 0.0f});
      }
    }
  }
  // The hinges (record 0's +16 and +80 matrices, the scissor doors' record
  // 1's): near a turn, or anything finite.
  for (uint32_t at : {5552u, 6032u, 12128u, 24480u}) {
    for (uint32_t hinge : {16u, 256u}) {
      const uint32_t node = points + at + hinge;
      if (r.Below(3) == 0) {
        FiniteFloats(base, node, 32, r);
      } else {
        const float y = Spread(r, 1.0f), z = Spread(r, 1.0f);
        StoreFloats(base, node, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, y, z, 1});
        StoreFloats(base, node + 64, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -y, -z, 1});
      }
    }
  }
  // The car record (the doors' +6080, the front plate's +2648, pb +
  // 37076) and the parts record (the doors' +6084, set b's +5520, pb +
  // 37080).
  const uint32_t car = top + kCar, parts_record = top + kPartsRecord;
  guest::StoreU32(base, car + 0, r.Below(4));
  guest::StoreU32(base, car + 3684, r.Below(3));
  guest::StoreU32(base, car + 3688,
                  r.Below(3) == 0 ? (r.Below(4) == 0 ? uint32_t(r.Next()) | 1u : 1u) : 0);
  guest::StoreU32(base, parts_record + 12,
                  r.Below(4) == 0 ? uint32_t(int32_t(r.Below(33)) - 12) : r.Below(11));
  guest::StoreU32(base, parts_record + 28, r.Below(11));
  guest::StoreU32(base, parts_record + 32, r.Below(4));
  guest::StoreU32(base, parts_record + 56, r.Below(2) ? 0 : r.Below(4) == 0 ? r.Next() : 1);
  guest::StoreU32(base, top + kSceneModelRecord, r.Below(2) ? 2 : r.Below(8));
  guest::StoreU32(base, points + 37072, top + kSceneModelRecord);
  guest::StoreU32(base, points + 37076, car);
  guest::StoreU32(base, points + 37080, parts_record);
  for (uint32_t door : kDoors) {
    if (r.Below(6) == 0) {
      FiniteFloats(base, points + door + 6016, 16, r);
    } else {
      const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
      Write(base, points + door + 6016, world.data(), 64);
    }
    guest::StoreU32(base, points + door + 6080, car);
    guest::StoreU32(base, points + door + 6084, parts_record);
  }
  guest::StoreU32(base, points + 18224 + 5520, parts_record);
  // The plates: the front's flag, texture and car record; the rear's flag
  // and texture, and always a plate model (without one the original writes
  // guest words 456 and 432, unmapped here).
  guest::StoreU32(base, points + 2640, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  guest::StoreU32(base, points + 2644, uint32_t(r.Next()));
  guest::StoreU32(base, points + 2648, car);
  guest::StoreU32(base, points + 2896 + 1680, r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  guest::StoreU32(base, points + 2896 + 1684, uint32_t(r.Next()));
  guest::StoreU32(base, points + 2896 + 1444, model);
  // The headlights' pop-up matrix and its node's own angle.
  if (r.Below(2)) {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, points + kHeadlights + 3120, world.data(), 64);
  } else {
    RandomMatrix(base, points + kHeadlights + 3120, r);
  }
  guest::StoreU32(base, points + kHeadlights + 3184,
                  r.Below(6) == 0 ? RandomFloat(r)
                                  : std::bit_cast<uint32_t>(Spread(r, 1.0f) + 1.0f));
  // The shadows: records 0 and 3 (their model words and shapes).
  guest::StoreU32(base, points + kShadows + 4, r.Below(8) == 0 ? 0 : model);
  guest::StoreU32(base, points + kShadows + 228, shape);
  guest::StoreU32(base, points + kShadows + 724, r.Below(8) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  guest::StoreU32(base, points + kShadows + 948, r.Below(4));
  // The globals the plates read.
  guest::StoreU32(base, kPlateFlag, r.Below(3) == 0 ? 1 + r.Below(3) : 0);
  guest::StoreU32(base, kRaceData, race);
  guest::StoreU32(base, race + 0xA3AF4, r.Below(3) == 0 ? 7 + r.Below(2) : r.Below(10));
  // The scene: the object's and [0x826C2E44].
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  guest::StoreU32(base, top + kObject + 37284, scene);
  guest::StoreU32(base, scene + 4, set);
  const uint32_t xpr = scene + 4648;
  guest::StoreU32(base, xpr + 0, uint32_t(r.Next()));
  guest::StoreU32(base, xpr + 16, race + 0x11800);
  guest::StoreU32(base, xpr + 20, r.Below(4));
  RandomWords(base, race + 0x11800, 64, r);
  const std::vector<uint8_t> glow = Read(base, model, 0x200);
  Write(base, scene + kGlowModel, glow.data(), 0x200);
  guest::StoreU32(base, scene + kGlowModel + 412,
                  r.Below(8) == 0 ? 0 : r.Below(8) == 0 ? 4 : 1 + r.Below(2));
  // The pass description: the race's camera pass (+0, kept enabled: a
  // disabled one reads address 144), the shadows' (+4, +8), the mirrors'
  // (+16), the keys, the bits (the shadows' low byte, the mirrors' 0x100).
  const uint32_t d = scene + 5072;
  const uint32_t current = r.Below(4);
  guest::StoreU32(base, d + 0, current);
  guest::StoreU32(base, d + 4, r.Below(4));
  guest::StoreU32(base, d + 8, r.Below(4));
  guest::StoreU32(base, d + 16, r.Below(4));
  RandomWords(base, d + 32, 28, r);
  guest::StoreU32(base, d + 60, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(512));
  RandomWords(base, d + 76, 8, r);
  if (r.Below(3) == 0) {
    StoreFloats(base, d + 112, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
  } else {
    FiniteFloats(base, d + 112, 16, r);
  }
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
  // The scene model's block: its bits, world, alphas and angles.
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
  for (uint32_t at : {640u, 660u, 664u, 676u, 680u, 692u}) {
    guest::StoreU32(base, source + at, Angle(r));
  }
  guest::StoreU32(base, source + 688, r.Below(2) ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  guest::StoreU32(base, source + 728, r.Below(2) ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
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

}  // namespace

void TopDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t allocation = memory::Alloc(kTopBytes + 16, 0);
  const uint32_t top = (allocation + 15) & ~15u;
  const uint32_t source = block + kSource;
  const uint32_t scene = race + 0x10000;
  Results results("SceneParts::Draw (sub_820E2EE8)");
  const int cases = 384;
  int front_end = 0, races = 0, queued = 0;
  std::vector<uint8_t> scene_bytes, records;
  const std::vector<uint8_t> zero(kSlotCount * 16, 0);
  uint32_t sp = 0, set = 0;
  // The kept matrices (the doors' +6016, the headlights' +3120) as floats.
  const uint32_t points = top + kPoints;
  const uint32_t kept[] = {points + kDoors[0] + 6016, points + kDoors[1] + 6016,
                           points + kHeadlights + 3120};
  for (int c = 0; c < cases; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          SetFlushOnEntry(r.Below(2) != 0);
          set = r.Below(16) == 0 ? 2 + r.Below(4) : r.Below(2);
          TopWorld(h, r, block, race, top, r.Below(4), set);
          scene_bytes = Read(h.base, scene, kSceneBytes);
          records = Read(h.base, top, kRecordsEnd);
          sp = t.r1.u32;
          t.r3.u64 = top + kObject;
          t.r4.u64 = source;
          t.r5.u64 = r.Next();
          t.r6.u64 = r.Next();
          t.r24.u64 = r.Next();
          t.r28.u64 = r.Next();
        },
        __imp__sub_820E2EE8, hooks::CarDrawSceneParts, AlphaFloats(),
        fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& o) {
          if (set == 0) {
            const uint32_t frame = t.r1.u32 - car_draw::kPartsFrontEndFrame;
            o.Word(guest::LoadU32(h.base, frame));
            o.Floats(h.base, frame + 80, 16);  // the mirrors' world
          } else if (set == 1) {
            o.Word(guest::LoadU32(h.base, t.r1.u32 - car_draw::kPartsRaceFrame));
          }
          // The object and records, the kept matrices as floats.
          uint32_t at = top;
          for (uint32_t matrix : kept) {
            o.Bytes(Read(h.base, at, matrix - at));
            o.Floats(h.base, matrix, 16);
            at = matrix + 64;
          }
          o.Bytes(Read(h.base, at, top + kRecordsEnd - at));
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
            front_end += set == 0;
            races += set == 1;
          }
          Write(h.base, scene, scene_bytes.data(), kSceneBytes);
          Write(h.base, top, records.data(), kRecordsEnd);
          Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
          Write(h.base, top + kSlots, zero.data(), uint32_t(zero.size()));
        },
        true);
  }
  ITC_LOG_INFO("harness: SceneParts::Draw: {} of {} cases the front end's list, {} the race's, "
               "{} ended handing an entry to the render queue",
               front_end, cases, races, queued / 2);
  results.Report();
  memory::Free(allocation);
}

}  // namespace harness::car_draw_shared
