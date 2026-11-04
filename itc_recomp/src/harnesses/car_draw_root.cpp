// Car::DrawUpdate (src/car_draw/update.cpp, docs/functions/car-draw.md: the
// car draw's root) against the recompiled one, under the car_draw harness
// (its Case and model draws' world, car_draw_shared.h), with a random
// generator of its own (it runs last, so its cases shift no other file's
// inputs). The scene model sits at the block's +0xE2C0, so its block (+4288)
// is the scene model block the wheels' world builds at the block's +0xF380.
// The wheels drawn are the wheels' top's world (SharedWheelsWorld: the scene
// at the race block's +0x10000, its current camera pass enabled); the other
// second-set objects are built here, in an allocation of their own, so
// that their finishers answer from their finished flags (a loading slot
// otherwise: 0) and their apply steps walk no models (no arrived slots,
// models without shapes), and so that the draws the wheels' world doesn't
// cover draw nothing: the parts, the interior and the cockpit see a scene
// whose +4 is 2 or above (the draws' "nothing above set 1"), the light
// kit's groups modes that draw nothing (its blink counters and the copies
// still run), the traffic body records without models, part models without
// shapes and the block's bit 0x1 clear (no glows). The finishers' flags,
// the race flag ([+5116] + 4), the second wheels' claim and finish, the
// cockpit view's model, the cull (the camera pass's view-projection, the
// height cull's vehicles) and the light kit's pass vary by case. Compared:
// the block (the boost as a float), the globals (the light slots), the
// allocation, the wheels' leaves' records and object, the scene, the
// render queue's keys, the root's frame (its back chain and temporaries),
// what went to the command list, r3, r24 and r28. The recompiled side runs
// with --native_car_draw off (its callees the originals; the load poll's
// and ScenePools::ReleaseB's natively on both sides, as their wraps run
// them); the native side the register step hooks::CarDrawUpdate.

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
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kVehiclesGlobal = 0x826C2E34;
constexpr uint32_t kRacersGlobal = 0x826D82F4;
constexpr uint32_t kAccessoriesGlobal = 0x826C5510;
constexpr uint32_t kSecondPlayer = 0x826C7824;
constexpr uint32_t kKeys = 0x8296B900;  // the render queue's keys +0 and +12
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kModel = kSource - 4288;
constexpr uint32_t kSceneBytes = 0x2000;
constexpr uint32_t kDeepStackBytes = 8192;

// The allocation's areas.
constexpr uint32_t kPools = 0x0;              // [+5116]: +4 the race, +5004 pool 5's list
constexpr uint32_t kFirstWheels = 0x5800;     // the first set's wheels (+2324 loading)
constexpr uint32_t kSceneCar = 0x6800;        // [+5048]: +24 the car
constexpr uint32_t kCar = 0x6900;             // the first set's car model
constexpr uint32_t kSets = 0x8600;            // the model sets' words (+8)
constexpr uint32_t kInert = 0x8700;           // a scene with +4 at 2 or above
constexpr uint32_t kZeroModel = 0x8800;       // a part model without shapes
constexpr uint32_t kAccessories = 0x8A00;     // the accessories' set, empty
constexpr uint32_t kPool4 = 0x8B00;           // the light kit's pool 4 object
constexpr uint32_t kKitRecord = 0x8C00;       // the light kit's record
constexpr uint32_t kKitScene = 0x8D00;        // the light kit's scene (+4, +5048)
constexpr uint32_t kVehicles = 0xA100;        // two vehicles (+3312 the matrix)
constexpr uint32_t kRacerList = 0xD200;       // the racers' list
constexpr uint32_t kRacers = 0xDB40;          // two racers
constexpr uint32_t kParts = 0xDC00;           // [+5060]
constexpr uint32_t kInterior = 0x16E00;       // [+5068]
constexpr uint32_t kFirstInterior = 0x19C00;  // its +36
constexpr uint32_t kCockpit = 0x1A900;        // [+5076]
constexpr uint32_t kCockpitBody = 0x1BA00;    // its +32
constexpr uint32_t kKit = 0x1C800;            // [+5084]
constexpr uint32_t kTraffic = 0x1FC00;        // [+5092]
constexpr uint32_t kTrafficCar = 0x22300;     // its +9760
constexpr uint32_t kCarParts = 0x24000;       // the parts' +104 (its set at +106000)
// Two more wheels objects (16,784 bytes each: their finished and set-up
// flags at +16768 and +16772): the old ones (a node of the pools' list)
// and second wheels not yet finished.
constexpr uint32_t kOldWheels = 0x3E000;
constexpr uint32_t kNewWheels = 0x42200;
constexpr uint32_t kAreaBytes = 0x46400;

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A flag: set most of the time.
uint32_t Flag(Random& r) {
  return r.Below(4) == 0 ? 0 : (r.Below(8) == 0 ? uint32_t(r.Next()) | 1u : 1);
}

// The allocation's objects (zeroed first) and the scene model's fields;
// see the top. Returns the kind of case for the counts.
struct Shape {
  bool traffic, cockpit, swap, swap_done, race, near_cull, ready;
};

Shape RootWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t area,
                uint32_t wheels) {
  uint8_t* base = h.base;
  const std::vector<uint8_t> zero(kAreaBytes, 0);
  Write(base, area, zero.data(), kAreaBytes);
  Shape s{};
  const uint32_t model = block + kModel, source = block + kSource;
  const uint32_t kind = r.Below(4) == 0 ? 3 : r.Below(3);
  s.traffic = kind == 3;
  Put(base, model + 32, kind);
  Put(base, model + 4296, r.Below(10) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  Put(base, model + 4300, kind == 2 && r.Below(2) == 0 ? uint32_t(r.Next()) | 1u : 0);
  s.cockpit = kind == 2 && guest::LoadU32(base, model + 4300) != 0;
  Put(base, model + 4992, RandomFloat(r));
  Put(base, model + 5028, r.Below(10) == 0 ? 1 : 0);
  Put(base, model + 4072, r.Below(3));
  // The scene (the second set's pools: +4 the race flag; ReleaseB's list).
  const uint32_t pools = area + kPools;
  Put(base, model + 5116, pools);
  Put(base, pools + 4, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2));
  s.race = guest::LoadU32(base, pools + 4) != 0;
  Put(base, pools + 5004, area + kOldWheels);
  Put(base, area + kOldWheels + 16, r.Below(3));
  Put(base, area + kOldWheels + 20, r.Below(4));
  // The flags.
  for (uint32_t at : {5052u, 5056u, 5064u, 5072u, 5080u, 5088u, 5096u, 5104u}) {
    Put(base, model + at, Flag(r));
  }
  Put(base, model + 5036, r.Below(2) == 0 ? 1 : 0);
  s.ready = guest::LoadU32(base, model + 5036) != 0;
  // The first set's car (CarModel::IsReady, the decals' state, the vinyls).
  const uint32_t scene_car = area + kSceneCar, car = area + kCar, sets = area + kSets;
  Put(base, model + 5048, scene_car);
  Put(base, scene_car + 24, car);
  Put(base, car + 7256, sets);
  Put(base, sets + 8, r.Below(6) == 0 ? 2 : r.Below(2));
  for (uint32_t at : {3972u, 3996u, 3748u}) {
    Put(base, car + at, r.Below(6) == 0 ? 2 : 3);
  }
  Put(base, car + 4088, r.Below(4) == 0 ? 2 : 3);
  Put(base, car + 6684, uint32_t(r.Next()));
  Put(base, car + 6692, uint32_t(r.Next()));
  const uint32_t inert = area + kInert;
  Put(base, inert + 4, 2 + r.Below(4));
  const uint32_t zero_model = area + kZeroModel;
  Put(base, kAccessoriesGlobal, area + kAccessories);
  // The parts: their car parts' set 0 to 2 (no slot loading or arrived).
  const uint32_t parts = area + kParts, car_parts = area + kCarParts;
  Put(base, model + 5060, parts);
  Put(base, parts + 100, Flag(r));
  Put(base, parts + 104, car_parts);
  Put(base, parts + 37284, inert);
  Put(base, car_parts + 106000, sets + 0x20);
  Put(base, sets + 0x20 + 8, r.Below(3));
  Put(base, car_parts + 24, car);
  // The interior (its first set's model set 2: not loaded).
  const uint32_t interior = area + kInterior, first_interior = area + kFirstInterior;
  Put(base, model + 5068, interior);
  Put(base, interior + 36, first_interior);
  Put(base, interior + 11660, Flag(r));
  Put(base, interior + 11668, inert);
  Put(base, first_interior + 3092, sets + 0x40);
  Put(base, sets + 0x40 + 8, 2);
  // The cockpit (its body's first slot loading), or none (its flag set).
  const uint32_t cockpit = area + kCockpit, body = area + kCockpitBody;
  const bool has_cockpit = s.cockpit ? r.Below(8) != 0 : r.Below(2) == 0;
  Put(base, model + 5076, has_cockpit ? cockpit : 0);
  if (!has_cockpit) {
    Put(base, model + 5080, 1);  // its finisher is called unchecked
  }
  Put(base, cockpit + 4312, Flag(r));
  Put(base, cockpit + 32, body);
  Put(base, cockpit + 28, interior);
  Put(base, cockpit + 4316, inert);
  Put(base, body + 556, 2);
  Put(base, body + 24, car);
  // The light kit: its groups' and the blink's record (modes that draw
  // nothing), its pool 4 object loading, its scene.
  const uint32_t kit = area + kKit, record = area + kKitRecord, kit_scene = area + kKitScene;
  Put(base, model + 5084, kit);
  Put(base, kit + 28, area + kPool4);
  Put(base, area + kPool4 + 100, 2);
  Put(base, kit + 32, uint32_t(r.Next()));
  Put(base, kit + 96, record);
  for (uint32_t at = 104; at < 136; at += 4) {
    Put(base, kit + at, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(64));
  }
  for (uint32_t at : {144u, 4480u, 8816u}) {
    Put(base, kit + at, record);
  }
  for (uint32_t g = 0; g < 3; g++) {
    Put(base, record + 4 + 16 * g, r.Below(2) == 0 ? 0 : 12 + r.Below(100));
    guest::StoreFloat(base, record + 16 + 16 * g, float(r.Below(400)) / 100.0f);
  }
  Put(base, kit + 13152, zero_model);
  Put(base, kit + 13156, Flag(r));
  Put(base, kit + 13160, kit_scene);
  Put(base, kit_scene + 4, r.Below(2));
  Put(base, kit_scene + 5048, r.Below(2));
  // The traffic body (its car's slot loading): records without models, part
  // models without shapes, its scene inert; the block's bit 0x1 clear.
  const uint32_t traffic = area + kTraffic, traffic_car = area + kTrafficCar;
  Put(base, model + 5092, s.traffic ? traffic : 0);
  Put(base, traffic + 9760, traffic_car);
  Put(base, traffic + 9792, Flag(r));
  Put(base, traffic + 9796, inert);
  for (uint32_t m = 0; m < 6; m++) {
    Put(base, traffic + 9768 + 4 * m, zero_model);
  }
  Put(base, traffic_car + 3764, 2);
  if (s.traffic) {
    Put(base, source + 4, guest::LoadU32(base, source + 4) & ~1u);
  }
  // The wheels: the drawn ones (the wheels' world), and with the second
  // wheels claimed (+5032), the new ones: when finished the drawn ones
  // (the old a node of the pools' list), else ones that aren't.
  const uint32_t first_wheels = area + kFirstWheels, next = area + kNewWheels;
  Put(base, first_wheels + 2324, 2);
  for (uint32_t at : {48u, 52u, 64u, 68u}) {
    guest::StoreFloat(base, first_wheels + at, float(r.Below(200)) / 100.0f);
  }
  for (const uint32_t w : {wheels, next, area + kOldWheels}) {
    Put(base, w + 20, r.Below(4));
    Put(base, w + 28, first_wheels);
    Put(base, w + 16772, 1);
  }
  Put(base, wheels + 16768, Flag(r));
  Put(base, next + 16768, 0);
  s.swap = !s.traffic && r.Below(3) == 0;
  s.swap_done = s.swap && r.Below(2) == 0;
  if (s.swap_done) {
    Put(base, wheels + 16768, uint32_t(r.Next()) | 1u);  // the new ones finished
  }
  Put(base, model + 5032, s.swap ? uint32_t(r.Next()) | 1u : 0);
  Put(base, model + 5100, s.swap_done ? area + kOldWheels : wheels);
  Put(base, model + 5108, s.swap_done ? wheels : next);
  Put(base, model + 5112, r.Below(2));
  // The scene's fields the root reads: the light kit's pass (+5084) and key
  // (+5124), the keys' saves (+5136, +5140), the lighting's +176.
  const uint32_t scene = race + 0x10000;
  Put(base, scene + 5084, r.Below(4));
  Put(base, scene + 5124, uint32_t(r.Next()));
  Put(base, scene + 5136, uint32_t(r.Next()));
  Put(base, scene + 5140, uint32_t(r.Next()));
  RandomFloats(base, scene + 5248 + 176, 4, r);
  Put(base, kKeys, uint32_t(r.Next()));
  Put(base, kKeys + 12, uint32_t(r.Next()));
  // The height cull's vehicles (none now and then) and racers, as the
  // height cull's cases.
  const uint32_t vehicles = area + kVehicles;
  Put(base, kVehiclesGlobal, r.Below(4) == 0 ? 0 : vehicles);
  Put(base, kRaceData, race);
  Put(base, race + 0xA3AF0, r.Below(3) == 0 ? 6 : r.Below(8));
  Put(base, race + 0x59614, r.Below(2) ? 2 : r.Below(4));
  Put(base, kSecondPlayer, r.Below(2));
  const uint32_t list = area + kRacerList;
  Put(base, kRacersGlobal, list);
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t racer = area + kRacers + 0x40 * i;
    Put(base, list + (i + 577) * 4, r.Below(12) == 0 ? 0 : racer);
    Put(base, racer, r.Below(8) == 0 ? 0 : 0x40);
    Put(base, racer + 20, r.Below(2));
  }
  const float y = guest::LoadFloat(base, source + 16 + 52);
  for (uint32_t v = 0; v < 2; v++) {
    const uint32_t matrix = vehicles + 6272 * v + 3312;
    StoreFloats(base, matrix,
                {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 500),
                 r.Below(2) ? y + Spread(r, 2) : Spread(r, 40), Spread(r, 500), 1});
  }
  s.near_cull = guest::LoadU32(base, kVehiclesGlobal) != 0;
  return s;
}

}  // namespace

// Car::DrawUpdate against the recompiled one (run with --native_car_draw
// off), a random generator of its own.
void RootDraws(Context& h, uint32_t block, uint32_t race) {
  Random r(0x63617264726F6F74ull);  // "cardroot"
  const uint32_t leaves_allocation = memory::Alloc(kSharedWheelsLeavesBytes + 16, 0);
  const uint32_t top = (leaves_allocation + 15) & ~15u;
  const uint32_t top_allocation = memory::Alloc(kSharedWheelsTopBytes + 16, 0);
  const uint32_t mine = (top_allocation + 15) & ~15u;
  const uint32_t area_allocation = memory::Alloc(kAreaBytes + 16, 0);
  const uint32_t area = (area_allocation + 15) & ~15u;
  const uint32_t scene = race + 0x10000;
  const uint32_t wheels = mine + kSharedWheelsObject;
  const std::vector<uint8_t> saved_scene = Read(h.base, scene, kSceneBytes);
  const std::vector<uint8_t> saved_keys = Read(h.base, kKeys, 16);
  Results results("Car::DrawUpdate (sub_820DAC68)");
  constexpr int kCases = 512;
  int traffic = 0, cockpits = 0, swaps = 0, swapped = 0, races = 0, ready = 0, culled = 0,
      cleared = 0, lit = 0, drawn = 0;
  std::vector<uint8_t> scene_bytes, records, own, mine_bytes, keys;
  uint32_t mask_before = 0, saved_before = 0;
  const std::vector<uint8_t> zero(kSharedWheelsSlotCount * 16, 0);
  uint32_t sp = 0;
  Shape shape{};
  FloatRanges floats = AlphaFloats();
  floats.push_back({kSource + 704, 1});  // the boost
  for (int c = 0; c < kCases; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          SetFlushOnEntry(r.Below(2) != 0);
          const uint32_t set = r.Below(8) ? r.Below(2) : 2 + r.Below(4);
          SharedWheelsWorld(h, r, block, race, top, mine, set, r.Below(3) == 0);
          shape = RootWorld(h, r, block, race, area, wheels);
          traffic += shape.traffic;
          cockpits += shape.cockpit;
          swaps += shape.swap;
          swapped += shape.swap_done;
          races += shape.race;
          ready += shape.ready;
          scene_bytes = Read(h.base, scene, kSceneBytes);
          records = Read(h.base, top, kSharedWheelsLeavesEnd);
          mine_bytes = Read(h.base, mine, kSharedWheelsTopEnd);
          own = Read(h.base, area, kAreaBytes);
          keys = Read(h.base, kKeys, 16);
          mask_before = guest::LoadU32(h.base, scene + 5132);
          saved_before = guest::LoadU32(h.base, scene + 5136);
          sp = t.r1.u32;
          t.r3.u64 = block + kModel;
          t.r24.u64 = r.Next();
          t.r28.u64 = r.Next();
        },
        __imp__sub_820DAC68, hooks::CarDrawUpdate, floats, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& o) {
          const uint32_t frame = t.r1.u32 - car_draw::kCarDrawUpdateFrame;
          o.Word(guest::LoadU32(h.base, frame));
          o.Floats(h.base, frame + 80, (176 - 80) / 4);
          o.Floats(h.base, mine, kSharedWheelsTopEnd / 4);
          o.Floats(h.base, top, kSharedWheelsLeavesEnd / 4);
          o.Bytes(Read(h.base, area, kAreaBytes));
          o.Bytes(Read(h.base, scene, kSceneBytes));
          o.Bytes(Read(h.base, kKeys, 16));
          const uint32_t written =
              guest::LoadU32(h.base, block + 0x3000 + 44) - (mine + kSharedWheelsSlots);
          o.Word(written);
          o.Floats(h.base, mine + kSharedWheelsSlots,
                   std::min(written, kSharedWheelsSlotCount * 16) / 4);
          o.Word(uint32_t(t.r24.u64));
          o.Word(uint32_t(t.r28.u64));
          o.Word(uint32_t(t.r24.u64 >> 32));
          o.Word(uint32_t(t.r28.u64 >> 32));
          if (t.r3.u32 == frame + 160) {
            culled++;
          }
          if (written != 0) {
            drawn++;
          }
          if (guest::LoadU32(h.base, scene + 5136) != saved_before) {
            lit++;
          }
          if (guest::LoadU32(h.base, scene + 5132) != mask_before) {
            cleared++;
          }
        },
        [&](int) {
          Write(h.base, scene, scene_bytes.data(), kSceneBytes);
          Write(h.base, top, records.data(), kSharedWheelsLeavesEnd);
          Write(h.base, mine, mine_bytes.data(), kSharedWheelsTopEnd);
          Write(h.base, area, own.data(), kAreaBytes);
          Write(h.base, kKeys, keys.data(), 16);
          Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
          Write(h.base, mine + kSharedWheelsSlots, zero.data(), uint32_t(zero.size()));
        },
        true);
  }
  ITC_LOG_INFO("harness: Car::DrawUpdate: of {} cases {} traffic, {} cockpit views, {} with "
               "the second wheels claimed ({} finished), {} in a race, {} ready at entry; {} "
               "ended culled behind the camera, {} cleared the pass mask, {} drew the light "
               "kit, {} wrote to the command list",
               kCases, traffic, cockpits, swaps, swapped, races, ready, culled / 2, cleared / 2,
               lit / 2, drawn / 2);
  results.Report();
  Write(h.base, scene, saved_scene.data(), kSceneBytes);
  Write(h.base, kKeys, saved_keys.data(), 16);
  memory::Free(area_allocation);
  memory::Free(top_allocation);
  memory::Free(leaves_allocation);
}

}  // namespace harness::car_draw_shared
