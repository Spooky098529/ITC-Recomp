// The scene models' load poll (src/load_poll/, docs/functions/load-poll.md)
// against the recompiled functions. Each case fills a block of guest memory
// and the stack below the call's r1 with random bytes, writes the objects'
// pointers (the part models with no nodes, so the point set-ups find
// nothing, and zero to two shapes of up to two materials each, so the
// material passes walk real materials), points the scene ([0x826C2E44]) and
// the accessories' set ([0x826C5510]) at the block for the case (put back
// after), runs the original and the
// register step (hooks/load_poll.h) from the same inputs, and compares the
// block and r3. The callees run as their adapters run them on both sides
// (their own harnesses check them). The functions here copy words without
// float math (the point set-ups' identity matrices are copied), so the
// block is compared exactly; the frames aren't (the plain C++ doesn't write
// the saved registers' slots).

#include "harness.h"

#include "base/log.h"
#include "hooks/car_makers.h"
#include "graphics/resources.h"
#include "hooks/load_poll.h"
#include "hooks/scene_models.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x4A000;
constexpr uint32_t kStackBytes = 1024;  // below the call's r1
constexpr uint32_t kArgBytes = 256;     // above it (the parameter area)
constexpr uint32_t kSceneGlobal = 0x826C2E44;
constexpr uint32_t kAccessoriesGlobal = 0x826C5510;  // the accessories' set

// The block's areas.
constexpr uint32_t kObject = 0x0;       // the object (the car parts' 106,016 bytes)
constexpr uint32_t kCar = 0x1C000;      // a first-set car model (7,264 bytes)
constexpr uint32_t kOther = 0x1E000;    // a first-set object (up to 7,408 bytes)
constexpr uint32_t kSmall = 0x20000;    // records, sets, tables
constexpr uint32_t kScene = 0x20800;    // the scene (its +4 the race flag)
constexpr uint32_t kShapes = 0x21000;   // 8 shape areas of 0x1000
constexpr uint32_t kModels = 0x29000;   // 8 free part models of 0x200
constexpr uint32_t kSubs = 0x2A000;     // 4 small objects of 0x400
constexpr uint32_t kInterior = 0x2B000; // a first-set interior (3,104 bytes)
constexpr uint32_t kParts = 0x2C000;    // the first set's car parts (106,016 bytes)
constexpr uint32_t kOther2 = 0x48000;   // a second first-set object (the cockpit body)
constexpr uint32_t kTrafficBody = 0x1400;  // a scene traffic body beside a scene model

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A part model at `model`: no nodes (+408), zero to two shapes (+412) in
// shape area `area`, each with a material block of up to one material a
// group.
void MakeModel(uint8_t* base, uint32_t block, uint32_t model, uint32_t area, Random& r) {
  const uint32_t shapes = block + kShapes + 0x1000 * (area % 8);
  Put(base, model + 408, 0);
  Put(base, model + 412, r.Below(3));
  Put(base, model + 40, shapes);
  for (uint32_t s = 0; s < 2; s++) {
    const uint32_t materials = shapes + 0x480 + 0x600 * s;
    Put(base, shapes + 544 * s + 396, materials);
    const uint32_t first = r.Below(2), second = r.Below(2);
    Put(base, materials + 0, first + second);  // Model::MaterialTotal's
    Put(base, materials + 8, first);
    Put(base, materials + 12, second);
  }
}

// A model word: one of the free part models, or 0 now and then.
uint32_t ModelWord(uint8_t* base, uint32_t block, Random& r, bool maybe_none) {
  if (maybe_none && r.Below(4) == 0) {
    return 0;
  }
  return block + kModels + 0x200 * r.Below(8);
}

// One case through both versions: the block and the stack filled at random,
// `setup(t)` writing the inputs and the registers (once: they're written
// back for the second run).
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const std::string& what,
          const std::function<void(const PPCContext&)>& after = {}) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  RandomBytes(h.base, block, kBlockBytes, r);
  RandomBytes(h.base, stack, kStackBytes + kArgBytes, r);
  for (uint32_t m = 0; m < 8; m++) {
    MakeModel(h.base, block, block + kModels + 0x200 * m, m, r);
  }
  Put(h.base, block + kScene + 4, r.Below(2));
  // The accessories' set (Interior::IsLoaded): no entries, or one loaded or
  // loading.
  const uint32_t accessories = block + kSmall + 0x780;
  for (uint32_t i = 0; i < 3; i++) {
    Put(h.base, accessories + 4 + 4 * i, 0);
  }
  if (r.Below(3) == 0) {
    Put(h.base, accessories + 4, block + kSmall + 0x7C0 - 1096);
    Put(h.base, block + kSmall + 0x7C0, r.Below(2));
  }
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes + kArgBytes);
  const uint32_t saved_scene = guest::LoadU32(h.base, kSceneGlobal);
  const uint32_t saved_accessories = guest::LoadU32(h.base, kAccessoriesGlobal);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    Write(h.base, stack, frames.data(), kStackBytes + kArgBytes);
    Put(h.base, kSceneGlobal, block + kScene);
    Put(h.base, kAccessoriesGlobal, accessories);
    PPCContext t = start;
    (v == 0 ? recompiled : native)(t, h.base);
    Put(h.base, kSceneGlobal, saved_scene);
    Put(h.base, kAccessoriesGlobal, saved_accessories);
    if (after && v == 0) {
      after(t);
    }
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// A load slot's state: loading (2), arrived (3) or another.
uint32_t SlotState(Random& r) {
  static const uint32_t kStates[] = {2, 3, 3, 0, 1};
  return kStates[r.Below(5)];
}

// --- The first set's wheels ---

void Wheels(Context& h, Random& r, uint32_t block) {
  {
    Results results("Wheels::SetRecord (sub_820D8EC0)");
    for (int c = 0; c < 8; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             t.r3.u64 = block + kOther;
             t.r4.u64 = block + kSmall + 4 * r.Below(16);
           },
           __imp__sub_820D8EC0, hooks::LoadPollWheelsSetRecord, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Wheels::SetCaliperTexture (sub_820D8ED0)");
    for (int c = 0; c < 8; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t wheels = block + kOther;
             MakeModel(h.base, block, wheels + 2336, 0, r);
             MakeModel(h.base, block, wheels + 2800, 1, r);
             t.r3.u64 = wheels;
           },
           __imp__sub_820D8ED0, hooks::LoadPollWheelsSetCaliperTexture,
           fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The car model and the car parts ---

void CarModels(Context& h, Random& r, uint32_t block) {
  {
    Results results("CarModel::IsReady (sub_820D4048)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t car = block + kCar;
             const uint32_t set = block + kSmall;
             Put(h.base, car + 7256, set);
             static const uint32_t kSets[] = {0, 1, 2, 0xFFFFFFFFu};
             Put(h.base, set + 8, kSets[r.Below(4)]);
             for (uint32_t i = 0; i < 2; i++) {
               Put(h.base, car + 3972 + 24 * i, SlotState(r));
             }
             Put(h.base, car + 3748, SlotState(r));
             t.r3.u64 = car;
           },
           __imp__sub_820D4048, hooks::LoadPollCarModelIsReady, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarParts::SetVinylTextures (sub_820D72D8)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t parts = block + kObject;
             const uint32_t set = block + kSmall;
             Put(h.base, parts + 106000, set);
             static const uint32_t kSets[] = {0, 1, 2, 0xFFFFFFFFu};
             Put(h.base, set + 8, kSets[c % 4]);
             Put(h.base, parts + 24, block + kCar);
             t.r3.u64 = parts;
           },
           __imp__sub_820D72D8, hooks::LoadPollCarPartsSetVinylTextures,
           fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The interior ---

void Interiors(Context& h, Random& r, uint32_t block) {
  static const uint32_t kBlocks[] = {1, 10, 26, 28, 33, 36, 37, 5, 0, 0xFFFFFFFFu};
  {
    Results results("InteriorDials::SetBlock (sub_820E3040)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             t.r3.u64 = block + kObject + 400;
             t.r4.u64 = c < 10 ? kBlocks[c] : uint32_t(r.Next());
           },
           __imp__sub_820E3040, hooks::LoadPollInteriorDialsSetBlock,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  // The interior: [[+24] + 24] + 32 the car's block, the record's seven
  // 12-byte entries (+48), the meters (+3312) with their table ([+0]) and
  // the gauges' point records' models and shapes.
  auto interior = [&](PPCContext& t, bool any) {
    const uint32_t object = block + kObject;
    const uint32_t scene_model = block + kSmall;
    Put(h.base, object + 24, scene_model);
    Put(h.base, scene_model + 24, block + kCar);
    Put(h.base, block + kCar + 32, r.Below(3) == 0 ? 5 : r.Below(40));
    for (uint32_t i = 0; i < 7; i++) {
      Put(h.base, object + 48 + 12 * i, any && r.Below(3) == 0 ? 1 + r.Below(4) : 0);
      Put(h.base, object + 52 + 12 * i, r.Below(8));
    }
    const uint32_t meters = object + 3312;
    const uint32_t table = block + kSmall + 0x100;
    Put(h.base, meters, table);
    for (uint32_t g = 0; g < 7; g++) {
      Put(h.base, table + 12 * g + 12, r.Below(6));
      Put(h.base, table + 12 * g + 16, r.Below(7) - 1);
    }
    static const uint32_t kRecords[7] = {1700, 1940, 2660, 2900, 3620, 3860, 4100};
    for (uint32_t record : kRecords) {
      Put(h.base, meters + record, ModelWord(h.base, block, r, true));
      Put(h.base, meters + record + 224, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(2));
    }
    Put(h.base, object + 11668, block + kScene);
    t.r3.u64 = object;
  };
  {
    Results results("SceneInterior::SetGauges (sub_820E3938)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r, [&](PPCContext& t) { interior(t, c % 4 != 0); },
           __imp__sub_820E3938, hooks::LoadPollSceneInteriorSetGauges,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("InteriorMeters::SetGauge (sub_820E32E8)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             interior(t, false);
             t.r3.u64 = block + kObject + 3312;
             t.r4.u64 = c < 14 ? uint32_t(c % 7) : 7 + r.Below(4);
             t.r5.u64 = r.Next();
           },
           __imp__sub_820E32E8, hooks::LoadPollInteriorMetersSetGauge,
           fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The light kit and the traffic body ---

void LightsAndTraffic(Context& h, Random& r, uint32_t block) {
  // The light kit: +28 pool 4's object (its slot state +100, its part
  // model +112), +32 the texture, +13152 the model, +13156 finished,
  // +13160 the scene.
  auto kit = [&](PPCContext& t, bool finished) {
    const uint32_t object = block + kObject;
    const uint32_t pool4 = block + kOther;
    Put(h.base, object + 28, pool4);
    Put(h.base, pool4 + 100, SlotState(r));
    MakeModel(h.base, block, pool4 + 112, 2, r);
    Put(h.base, object + 13152, ModelWord(h.base, block, r, true));
    Put(h.base, object + 13156, finished ? 1 + r.Below(3) : 0);
    Put(h.base, object + 13160, block + kScene);
    t.r3.u64 = object;
  };
  {
    Results results("SceneLights::Finish (sub_820DDA60)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r, [&](PPCContext& t) { kit(t, c % 5 == 0); },
           __imp__sub_820DDA60, hooks::LoadPollSceneLightsFinish, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneLights::SetRecord (sub_820DCB70)");
    for (int c = 0; c < 8; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             kit(t, false);
             t.r4.u64 = block + kSmall + 4 * r.Below(32);
           },
           __imp__sub_820DCB70, hooks::LoadPollSceneLightsSetRecord, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneLights::ApplyRecord (sub_820DCBC0)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             kit(t, false);
             t.r4.u64 = block + kSmall + 4 * r.Below(32);
           },
           __imp__sub_820DCBC0, hooks::LoadPollSceneLightsApplyRecord,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The traffic body: +9760 the first set's traffic car (its file slot
    // +3764, +32 its kind, +192 0 so no texture is looked up by name, its
    // six part models +4256), +9792 finished, the point block +3728.
    Results results("SceneTraffic::Finish (sub_820E5A68)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t body = block + kObject;
             const uint32_t traffic = block + kOther;
             Put(h.base, body + 9760, traffic);
             Put(h.base, body + 9792, c % 5 == 0 ? 1 : 0);
             Put(h.base, traffic + 3764, SlotState(r));
             Put(h.base, traffic + 32, r.Below(4));
             Put(h.base, traffic + 192, 0);
             for (uint32_t i = 0; i < 6; i++) {
               MakeModel(h.base, block, traffic + 4256 + 464 * i, i, r);
             }
             t.r3.u64 = body;
           },
           __imp__sub_820E5A68, hooks::LoadPollSceneTrafficFinish, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The paint ---

// A color as the record keeps them: hue, saturation, value, 1.0 (a hue
// below 0 now and then).
void PutColor(uint8_t* base, uint32_t at, Random& r) {
  const float hue = float(int32_t(r.Below(400)) - 20);
  const float sat = float(r.Below(101)) / 100.0f;
  const float value = float(r.Below(101)) / 100.0f;
  const float w = 1.0f;
  for (uint32_t i = 0; i < 4; i++) {
    const float f = i == 0 ? hue : i == 1 ? sat : i == 2 ? value : w;
    guest::StoreU32(base, at + 4 * i, std::bit_cast<uint32_t>(f));
  }
}

// `count` part models from `models` (464 bytes apart) and their slot states
// from `states` (24 bytes apart).
void MakeModels(uint8_t* base, uint32_t block, uint32_t models, uint32_t states, uint32_t count,
                Random& r) {
  for (uint32_t i = 0; i < count; i++) {
    MakeModel(base, block, models + 464 * i, i, r);
    Put(base, states + 24 * i, SlotState(r));
  }
}

// A scene model's parts: the scene parts (+5060, its +104 the car parts
// with 216 slots and the model set), the interior (+5068, its +36 the
// interior with six slots), the cockpit (+5076 or 0: its +32 the body with
// six slots and its car model at +24, its +28 the interior), the traffic
// body (+5092, its +9760 the traffic car with six slots) and the wheels
// (+5100, its +28 the wheels with their four painted models).
void PaintWorld(uint8_t* base, uint32_t block, uint32_t model, Random& r, bool traffic) {
  const uint32_t parts = block + kSubs, interior = block + kSubs + 0x400,
                 cockpit = block + kSubs + 0x800, wheels = block + kSubs + 0xC00;
  Put(base, model + 5060, parts);
  Put(base, parts + 104, block + kParts);
  const uint32_t set = block + kSmall + 0x400;
  Put(base, block + kParts + 106000, set);
  Put(base, set + 8, r.Below(3));
  MakeModels(base, block, block + kParts + 5280, block + kParts + 116, 216, r);
  Put(base, model + 5068, interior);
  Put(base, interior + 36, block + kInterior);
  MakeModels(base, block, block + kInterior + 304, block + kInterior + 180, 6, r);
  Put(base, model + 5076, r.Below(4) == 0 ? 0 : cockpit);
  Put(base, cockpit + 28, interior);
  Put(base, model + 5100, wheels);
  Put(base, wheels + 28, block + kOther);
  if (traffic) {
    const uint32_t body = block + kTrafficBody;
    Put(base, model + 5092, body);
    Put(base, body + 9760, block + kOther);
    MakeModels(base, block, block + kOther + 4256, block + kOther + 4124, 6, r);
  } else {
    Put(base, cockpit + 32, block + kOther);
    Put(base, block + kOther + 24, block + kCar);
    MakeModels(base, block, block + kOther + 688, block + kOther + 556, 6, r);
    for (uint32_t m : {240u, 704u, 1216u, 1680u}) {
      // The wheels' models over the cockpit body's (the cases use one or
      // the other; both stay valid part models).
      MakeModel(base, block, block + kOther + m, m / 240, r);
    }
  }
  for (uint32_t c = 0; c < 9; c++) {
    PutColor(base, model + 64 + 16 * c, r);
  }
}

void Paint(Context& h, Random& r, uint32_t block) {
  {
    Results results("SceneModel::ApplyPaint (sub_820D9B68)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t model = block + kObject;
             const bool traffic = c % 4 == 1;
             PaintWorld(h.base, block, model, r, traffic);
             Put(h.base, model + 5036, c % 8 == 0 ? 0 : 1);
             Put(h.base, model + 32, traffic ? 3 : r.Below(3));
             t.r3.u64 = model;
           },
           __imp__sub_820D9B68, hooks::LoadPollSceneModelApplyPaint, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneModel::ApplyWheelPaint (sub_820D9C50)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t model = block + kObject;
             PaintWorld(h.base, block, model, r, false);
             Put(h.base, model + 5036, c % 8 == 0 ? 0 : 1);
             Put(h.base, model + 32, c % 8 == 1 ? 3 : r.Below(3));
             Put(h.base, model + 4072, c % 8 == 2 ? 0 : 1 + r.Below(3));
             if (c % 8 == 3) {
               Put(h.base, model + 5100, 0);
             }
             t.r3.u64 = model;
           },
           __imp__sub_820D9C50, hooks::LoadPollSceneModelApplyWheelPaint,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  // The steps under them, r4 to r10 the record's colors.
  auto colors = [&](PPCContext& t, uint32_t model) {
    const uint32_t at = model + 112;
    uint64_t* regs[7] = {&t.r4.u64, &t.r5.u64, &t.r6.u64, &t.r7.u64,
                         &t.r8.u64, &t.r9.u64, &t.r10.u64};
    for (uint32_t i = 0; i < 7; i++) {
      *regs[i] = (uint64_t(guest::LoadU32(h.base, at + 8 * i)) << 32) |
                 guest::LoadU32(h.base, at + 8 * i + 4);
    }
    // v3's second half, v4 and v5 in the caller's parameter area.
    for (uint32_t i = 0; i < 2; i++) {
      Put(h.base, t.r1.u32 + 88 + 4 * i, guest::LoadU32(h.base, at + 56 + 4 * i));
    }
    for (uint32_t i = 0; i < 8; i++) {
      Put(h.base, t.r1.u32 + 96 + 4 * i, guest::LoadU32(h.base, at + 64 + 4 * i));
    }
  };
  struct Step {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t object;  // r3: 0 the scene parts, 1 the interior, 2 the cockpit, 3 the
                      // traffic body, 4 the traffic car, 5 the scene wheels, 6 the wheels
  };
  const Step steps[] = {
      {"SceneParts::ApplyPaint (sub_820E0908)", __imp__sub_820E0908,
       hooks::LoadPollScenePartsApplyPaint, 0},
      {"SceneInterior::ApplyPaint (sub_820E4250)", __imp__sub_820E4250,
       hooks::LoadPollSceneInteriorApplyPaint, 1},
      {"SceneCockpit::ApplyPaint (sub_820DC138)", __imp__sub_820DC138,
       hooks::LoadPollSceneCockpitApplyPaint, 2},
      {"SceneTraffic::ApplyPaint (sub_820E61D0)", __imp__sub_820E61D0,
       hooks::LoadPollSceneTrafficApplyPaint, 3},
      {"TrafficCar::SetPaint (sub_820D7E18)", __imp__sub_820D7E18,
       hooks::LoadPollTrafficCarSetPaint, 4},
      {"SceneWheels::ApplyPaint (sub_820E8A00)", __imp__sub_820E8A00,
       hooks::LoadPollSceneWheelsApplyPaint, 5},
      {"Wheels::ApplyPaint (sub_820D91E0)", __imp__sub_820D91E0,
       hooks::LoadPollWheelsApplyPaint, 6},
  };
  for (const Step& step : steps) {
    Results results(step.name);
    for (int c = 0; c < 8; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t model = block + kObject;
             const bool traffic = step.object == 3 || step.object == 4;
             PaintWorld(h.base, block, model, r, traffic);
             if (step.object == 2) {
               Put(h.base, model + 5076, block + kSubs + 0x800);
             }
             static const uint32_t kObjects[7] = {kSubs, kSubs + 0x400, kSubs + 0x800,
                                                  kTrafficBody, kOther, kSubs + 0xC00, kOther};
             t.r3.u64 = block + kObjects[step.object];
             colors(t, model);
           },
           step.recompiled, step.native, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The finishers' builds ---

// A small float (eighths from -4 to 4).
uint32_t SmallFloat(Random& r) {
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(65)) - 32) / 8.0f);
}

// The first-set car model (kCar): its car record's +0 (+32: a block below
// 40, an index into the image's tables), its selected texture (+3712, the
// words at +7232; -1 gives none) from its XPR (+4000: +16 the records, +20
// the count) and its matrix (+6976).
void MakeCar(uint8_t* base, uint32_t block, Random& r) {
  const uint32_t car = block + kCar;
  Put(base, car + 32, r.Below(3) == 0 ? 5 : r.Below(40));
  Put(base, car + 3712, r.Below(6));
  for (uint32_t i = 0; i < 6; i++) {
    Put(base, car + 7232 + 4 * i, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(6));
  }
  Put(base, car + 4000 + 16, block + kSmall + 0x600);
  Put(base, car + 4000 + 20, 4);
  for (uint32_t w = 0; w < 16; w++) {
    Put(base, car + 6976 + 4 * w, SmallFloat(r));
  }
}

// A scene interior at `object` (as SetGauges' cases build it, its gauges'
// entries clear): the car at kCar (through [+24] + 24), the first-set
// interior (+36: six slots, the model set at [+3092] + 8), not finished.
void SceneInteriorWorld(uint8_t* base, uint32_t block, uint32_t object, Random& r) {
  const uint32_t scene_model = block + kSmall;
  Put(base, object + 24, scene_model);
  Put(base, scene_model + 24, block + kCar);
  MakeCar(base, block, r);
  for (uint32_t i = 0; i < 7; i++) {
    Put(base, object + 48 + 12 * i, 0);
  }
  Put(base, object + 11660, 0);
  Put(base, object + 36, block + kInterior);
  const uint32_t set = block + kSmall + 0x400;
  Put(base, block + kInterior + 3092, set);
  Put(base, set + 8, r.Below(3));
  MakeModels(base, block, block + kInterior + 304, block + kInterior + 180, 6, r);
}

void Builds(Context& h, Random& r, uint32_t block) {
  // The interior as SetGauges' cases build it, its first-set interior
  // (+36: six slots, the model set at [+3092] + 8), its finished flag
  // (+11660), the cockpit's flag and texture (+11656, +11664).
  {
    Results results("SceneInterior::Finish (sub_820E4140)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t object = block + kObject;
             SceneInteriorWorld(h.base, block, object, r);
             Put(h.base, object + 11660, c % 6 == 0 ? 1 : 0);
             Put(h.base, object + 11656, r.Below(2));
             t.r3.u64 = object;
           },
           __imp__sub_820E4140, hooks::LoadPollSceneInteriorFinish, fmt::format("case {}", c));
    }
    results.Report();
  }
  // The wheels object: +24 the car model (second set, its +24 the first
  // set's) or 0 (then +16776 a traffic car record and +16780 the tire
  // positions), +28 the first set's wheels (the models at +240, +1216,
  // +2336 and the rears 464 after), +32 the wheels record (+8 set: the
  // morph left out; see the second_set harness for it), +16772 done.
  {
    Results results("SceneWheels::SetUpPoints (sub_820E8908)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t object = block + kObject;
             const uint32_t scene_car = block + kSmall + 0x200;
             MakeCar(h.base, block, r);
             const bool traffic = c % 4 == 1;
             Put(h.base, object + 24, traffic ? 0 : scene_car);
             Put(h.base, object + 192, 0);  // the point block's car model, as reset
             Put(h.base, scene_car + 24, block + kCar);
             const uint32_t record = block + kSmall + 0x700;
             Put(h.base, record, r.Below(4));
             Put(h.base, object + 16776, record);
             Put(h.base, object + 16780, block + kCar + 6976);
             Put(h.base, object + 16772, c % 7 == 0 ? 1 : 0);
             const uint32_t wheels = block + kOther;
             Put(h.base, object + 28, wheels);
             for (uint32_t m : {240u, 704u, 1216u, 1680u, 2336u, 2800u}) {
               MakeModel(h.base, block, wheels + m, m / 464, r);
             }
             const uint32_t w = object + 32;
             Put(h.base, w + 0, traffic ? 3 : r.Below(7));
             Put(h.base, w + 8, 1 + r.Below(3));
             for (const uint32_t at : {44u, 56u}) {
               Put(h.base, w + at, 0xFFFFFFFFu);
             }
             Put(h.base, w + 68, r.Below(8));
             Put(h.base, w + 72, r.Below(3));
             Put(h.base, w + 76, r.Below(3));
             Put(h.base, w + 108, 9 + r.Below(12));
             Put(h.base, w + 112, 9 + r.Below(12));
             if (traffic) {
               Put(h.base, block + kCar + 32, r.Below(4));
             }
             t.r3.u64 = object;
           },
           __imp__sub_820E8908, hooks::LoadPollSceneWheelsSetUpPoints,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  // The parts object: +104 the first set's car parts (216 slots, the model
  // set), the record's words at +108 to +156 (small slot numbers; +124 13
  // now and then), +24 the car model (second set; its +24 the first set's).
  auto parts = [&](PPCContext& t) {
    const uint32_t object = block + kObject;
    const uint32_t scene_car = block + kSmall + 0x200;
    MakeCar(h.base, block, r);
    Put(h.base, object + 24, scene_car);
    Put(h.base, scene_car + 24, block + kCar);
    Put(h.base, object + 104, block + kParts);
    const uint32_t set = block + kSmall + 0x400;
    Put(h.base, block + kParts + 106000, set);
    Put(h.base, set + 8, r.Below(4) == 0 ? 2 : r.Below(2));
    MakeModels(h.base, block, block + kParts + 5280, block + kParts + 116, 216, r);
    for (uint32_t at = 108; at <= 156; at += 4) {
      Put(h.base, object + at, r.Below(3));
    }
    if (r.Below(3) == 0) {
      Put(h.base, object + 124, 13);
    }
    t.r3.u64 = object;
  };
  {
    Results results("SceneParts::Build (sub_820E05E0)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r, parts, __imp__sub_820E05E0, hooks::LoadPollScenePartsBuild,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneParts::SetSelectedTexture (sub_820DF250)");
    for (int c = 0; c < 8; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             parts(t);
             for (uint32_t k = 0; k < 17; k++) {
               Put(h.base, block + kObject + 32 + 4 * k, ModelWord(h.base, block, r, true));
             }
           },
           __imp__sub_820DF250, hooks::LoadPollScenePartsSetSelectedTexture,
           fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The wheels' materials and the last finishers ---

void Finishers(Context& h, Random& r, uint32_t block) {
  // The first set's wheels (kOther): the calipers' slot (+2324), the six
  // models, the textures (+40, +56) and the record's floats (+48 to +68).
  auto wheels = [&](PPCContext& t) {
    const uint32_t w = block + kOther;
    Put(h.base, w + 2324, SlotState(r));
    for (uint32_t m : {240u, 704u, 1216u, 1680u, 2336u, 2800u}) {
      MakeModel(h.base, block, w + m, m / 464, r);
    }
    for (uint32_t at = 48; at < 72; at += 4) {
      Put(h.base, w + at, SmallFloat(r));
    }
    t.r3.u64 = w;
  };
  {
    Results results("Wheels::BindTextures (sub_820D9638)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r, wheels, __imp__sub_820D9638, hooks::LoadPollWheelsBindTextures,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneWheels::Finish (sub_820E7328)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             wheels(t);
             const uint32_t object = block + kObject;
             Put(h.base, object + 28, block + kOther);
             Put(h.base, object + 16768, c % 5 == 0 ? 1 : 0);
             t.r3.u64 = object;
           },
           __imp__sub_820E7328, hooks::LoadPollSceneWheelsFinish, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Wheels::ApplyPalette (sub_820D92A0)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r, wheels, __imp__sub_820D92A0, hooks::LoadPollWheelsApplyPalette,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The parts object as Build's cases build it, +100 loaded and +37284
    // a scene object whose +4 is 0, 1 or 2.
    Results results("SceneParts::Finish (sub_820E2E68)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t object = block + kObject;
             const uint32_t scene_car = block + kSmall + 0x200;
             MakeCar(h.base, block, r);
             Put(h.base, object + 24, scene_car);
             Put(h.base, scene_car + 24, block + kCar);
             Put(h.base, object + 104, block + kParts);
             const uint32_t set = block + kSmall + 0x400;
             Put(h.base, block + kParts + 106000, set);
             Put(h.base, set + 8, r.Below(4) == 0 ? 2 : r.Below(2));
             MakeModels(h.base, block, block + kParts + 5280, block + kParts + 116, 216, r);
             for (uint32_t at = 108; at <= 156; at += 4) {
               Put(h.base, object + at, r.Below(3));
             }
             Put(h.base, object + 100, c % 5 == 0 ? 1 : 0);
             const uint32_t scene = block + kSmall + 0x7E0;
             Put(h.base, object + 37284, scene);
             Put(h.base, scene + 4, r.Below(3));
             t.r3.u64 = object;
           },
           __imp__sub_820E2E68, hooks::LoadPollScenePartsFinish, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The cockpit (kObject): +32 the body (kOther: its slots +556, +420,
    // +196, its six models +688, its car model at +24 with a block of 18 or
    // more, so the name is printed but not looked up), +28 a scene
    // interior (kObject + 0x1400), +4312 finished, the point block +192.
    Results results("SceneCockpit::Finish (sub_820DBFF8)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t cockpit = block + kObject;
             const uint32_t interior = block + kObject + 0x1400;
             SceneInteriorWorld(h.base, block, interior, r);
             Put(h.base, block + kCar + 32, 18 + r.Below(22));
             const uint32_t body = block + kOther;
             Put(h.base, cockpit + 32, body);
             Put(h.base, cockpit + 28, interior);
             Put(h.base, cockpit + 4312, c % 5 == 0 ? 1 : 0);
             Put(h.base, body + 24, block + kCar);
             for (uint32_t i = 0; i < 6; i++) {
               Put(h.base, body + 556 + 24 * i, r.Below(5) == 0 ? 2 : 3);
             }
             for (uint32_t i = 0; i < 2; i++) {
               Put(h.base, body + 420 + 24 * i, r.Below(8) == 0 ? 2 : 3);
             }
             Put(h.base, body + 196, r.Below(8) == 0 ? 2 : 3);
             Put(h.base, body + 48, r.Below(3));
             for (uint32_t i = 0; i < 6; i++) {
               MakeModel(h.base, block, body + 688 + 464 * i, i, r);
             }
             t.r3.u64 = cockpit;
           },
           __imp__sub_820DBFF8, hooks::LoadPollSceneCockpitFinish, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The poll ---

// The poll's scene model (kObject) and its parts' second-set objects, each
// present or not, finished or not (an unfinished one's first-set object
// still loading, so its finisher answers 0 before building anything; the
// builds have their own cases): the car model (+5048, kSubs: its +24 the
// car at kCar with its model set and slots, its decals' state +4088), the
// parts (+5060, +100 finished, +104 the car parts), the interior (+5068,
// +11660, +36), the cockpit (+5076, +4312, +32 the body at kOther2, +28
// the interior), the light kit (+5084, +13156, +28 pool 4's object), the
// wheels (+5100, +16768, +28 the wheels; +16772 set: the point set-up has
// its own cases) and the traffic body (+5092, +9792, +9760 the traffic
// car); the ready flags (+5036 to +5104), the scene (+5116, its +4) and the
// record's colors and light kit record (+4224).
void PollWorld(uint8_t* base, uint32_t block, uint32_t model, Random& r, bool traffic) {
  const uint32_t parts = model + 0x1400, interior = model + 0xA800, cockpit = model + 0xD800,
                 kit = model + 0xEA00, wheels = model + 0x12000, body = model + 0x16400;
  const uint32_t scene_car = block + kSubs;
  const uint32_t car = block + kCar, w = block + kOther, cb = block + kOther2;
  const uint32_t pool4 = block + kOther + 0x1000;
  MakeCar(base, block, r);
  auto present = [&](uint32_t at, uint32_t object) {
    Put(base, model + at, r.Below(5) == 0 ? 0 : object);
  };
  auto finished = [&](uint32_t at) {
    const bool done = r.Below(6) != 0;
    Put(base, at, done ? 1 : 0);
    return done;
  };
  // The car model.
  present(5048, scene_car);
  Put(base, scene_car + 24, car);
  const uint32_t car_set = block + kSmall + 0x500;
  Put(base, car + 7256, car_set);
  Put(base, car_set + 8, r.Below(6) == 0 ? 2 : r.Below(2));
  for (uint32_t i = 0; i < 2; i++) {
    Put(base, car + 3972 + 24 * i, r.Below(6) == 0 ? 2 : 3);
  }
  Put(base, car + 3748, r.Below(6) == 0 ? 2 : 3);
  Put(base, car + 4088, r.Below(6) == 0 ? 2 : 3);
  // The parts: their car parts as the paint's cases build them.
  present(5060, parts);
  Put(base, parts + 104, block + kParts);
  const uint32_t set = block + kSmall + 0x400;
  Put(base, block + kParts + 106000, set);
  Put(base, set + 8, r.Below(2));
  MakeModels(base, block, block + kParts + 5280, block + kParts + 116, 216, r);
  if (!finished(parts + 100)) {
    Put(base, block + kParts + 116, 2);
  }
  // The interior.
  present(5068, interior);
  Put(base, interior + 36, block + kInterior);
  Put(base, block + kInterior + 3092, block + kSmall + 0x580);
  Put(base, block + kSmall + 0x580 + 8, r.Below(2));
  MakeModels(base, block, block + kInterior + 304, block + kInterior + 180, 6, r);
  if (!finished(interior + 11660)) {
    Put(base, block + kInterior + 180, 2);
  }
  // The cockpit.
  present(5076, cockpit);
  Put(base, cockpit + 32, cb);
  Put(base, cockpit + 28, interior);
  Put(base, cb + 24, car);
  MakeModels(base, block, cb + 688, cb + 556, 6, r);
  if (!finished(cockpit + 4312)) {
    Put(base, cb + 556, 2);
  }
  // The light kit.
  present(5084, kit);
  Put(base, kit + 28, pool4);
  MakeModel(base, block, pool4 + 112, 3, r);
  Put(base, kit + 13152, pool4 + 112);
  Put(base, kit + 13160, block + kScene);
  if (!finished(kit + 13156)) {
    Put(base, pool4 + 100, 2);
  }
  // The wheels.
  present(5100, wheels);
  if (traffic) {
    // The poll writes the traffic body's tire positions into the wheels.
    Put(base, model + 5100, wheels);
  }
  Put(base, wheels + 28, w);
  for (uint32_t m : {240u, 704u, 1216u, 1680u, 2336u, 2800u}) {
    MakeModel(base, block, w + m, m / 464, r);
  }
  for (uint32_t at = 48; at < 72; at += 4) {
    Put(base, w + at, SmallFloat(r));
  }
  Put(base, wheels + 16772, 1);
  if (!finished(wheels + 16768)) {
    Put(base, w + 2324, 2);
  }
  // The traffic body.
  Put(base, model + 5092, traffic && r.Below(5) != 0 ? body : 0);
  Put(base, model + 5096, r.Below(4) == 0 ? 1 : 0);
  Put(base, body + 9760, w);
  MakeModels(base, block, w + 4256, w + 4124, 6, r);
  Put(base, w + 192, 0);
  Put(base, w + 32, r.Below(4));
  if (!finished(body + 9792)) {
    Put(base, w + 3764, 2);
  }
  // The flags, the scene, the record.
  Put(base, model + 5028, r.Below(8) == 0 ? 1 : 0);
  Put(base, model + 5040, r.Below(8) == 0 ? 1 : 0);
  Put(base, model + 32, traffic ? 3 : r.Below(3));
  for (uint32_t at = 5036; at <= 5104; at += 4) {
    if (at != 5048 && at != 5060 && at != 5068 && at != 5076 && at != 5084 && at != 5092 &&
        at != 5100 && at != 5096) {
      Put(base, model + at, r.Below(4) == 0 ? 0 : 1);
    }
  }
  if (r.Below(2) == 0) {
    Put(base, model + 5036, 0);
  }
  const uint32_t scene = block + kSmall + 0x7E0;
  Put(base, model + 5116, scene);
  Put(base, scene + 4, r.Below(2));
  for (uint32_t c = 0; c < 9; c++) {
    PutColor(base, model + 64 + 16 * c, r);
  }
  Put(base, model + 4072, r.Below(3));
}

void Poll(Context& h, Random& r, uint32_t block) {
  {
    Results results("SceneModel::ApplyPalettes (sub_820D9EA0)");
    for (int c = 0; c < 12; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t model = block + kObject;
             PollWorld(h.base, block, model, r, c % 4 == 1);
             t.r3.u64 = model;
           },
           __imp__sub_820D9EA0, hooks::LoadPollSceneModelApplyPalettes,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneModel::PollLoad (sub_820DA318)");
    for (int c = 0; c < 48; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t model = block + kObject;
             PollWorld(h.base, block, model, r, c % 4 == 1);
             t.r3.u64 = model;
           },
           __imp__sub_820DA318, hooks::LoadPollSceneModelPollLoad, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The record re-applies (scene_models/apply.cpp, the stand's setters'
// callees: scene-models.md) ---

// A 60-byte parts record of small slot numbers (+16 13 now and then), as
// SceneParts::Build's cases make them.
void PartsRecord(uint8_t* base, uint32_t record, Random& r) {
  for (uint32_t at = 0; at < 60; at += 4) {
    Put(base, record + at, r.Below(3));
  }
  if (r.Below(3) == 0) {
    Put(base, record + 16, 13);
  }
}

// A 120-byte interior record: its gauges' entries clear (SetGauges' callee
// never ran in play), its +100 (the scene's part model) 0 or small.
void InteriorRecord(uint8_t* base, uint32_t record, Random& r) {
  for (uint32_t i = 0; i < 7; i++) {
    Put(base, record + 8 + 12 * i, 0);
  }
  Put(base, record + 100, r.Below(2) == 0 ? 0 : r.Below(4));
}

// An interior's extras for SceneInterior::Apply: the scene (+11668) and its
// six parts (+11632, 0x200 apart beside the cockpit body).
void InteriorParts(uint8_t* base, uint32_t block, uint32_t interior) {
  Put(base, interior + 11668, block + kScene);
  for (uint32_t i = 0; i < 6; i++) {
    Put(base, interior + 11632 + 4 * i, block + kOther2 + 0x1000 + 0x200 * i);
  }
}

void Applies(Context& h, Random& r, uint32_t block) {
  {
    Results results("SceneParts::Apply (sub_820E08B0)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t object = block + kObject;
             const uint32_t scene_car = block + kSmall + 0x200;
             MakeCar(h.base, block, r);
             Put(h.base, object + 24, scene_car);
             Put(h.base, scene_car + 24, block + kCar);
             Put(h.base, object + 104, block + kParts);
             const uint32_t set = block + kSmall + 0x400;
             Put(h.base, block + kParts + 106000, set);
             Put(h.base, set + 8, r.Below(4) == 0 ? 2 : r.Below(2));
             MakeModels(h.base, block, block + kParts + 5280, block + kParts + 116, 216, r);
             // The parts' scene: the case's (its +4 0 or 1: the race's skips).
             Put(h.base, object + 37284, block + kScene);
             const uint32_t record = block + kSmall + 0x100;
             PartsRecord(h.base, record, r);
             t.r3.u64 = object;
             t.r4.u64 = record;
           },
           __imp__sub_820E08B0, hooks::SceneModelsPartsApply, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneInterior::Apply (sub_820E4258)");
    for (int c = 0; c < 16; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t object = block + kObject;
             SceneInteriorWorld(h.base, block, object, r);
             InteriorParts(h.base, block, object);
             const uint32_t record = block + kSmall + 0x100;
             InteriorRecord(h.base, record, r);
             t.r3.u64 = object;
             t.r4.u64 = record;
           },
           __imp__sub_820E4258, hooks::SceneModelsInteriorApply, fmt::format("case {}", c));
    }
    results.Report();
  }
  // SceneModel::ApplyRecord: the poll's world (a car, its car model, parts
  // and interior present), the scene's +4 0 (the front end's: the race's
  // path is the original's on both sides), the parts and interior given
  // what the applies read (as their cases above), the scene model's car
  // record (+48: its block, 5 now and then, and its +3680 the selected
  // texture's index, both copied into the car model), parts record (+3744)
  // and interior record (+3804).
  {
    Results results("SceneModel::ApplyRecord (sub_820DA758)");
    int applied = 0;
    for (int c = 0; c < 48; c++) {
      Case(h, results, block, r,
           [&](PPCContext& t) {
             const uint32_t model = block + kObject;
             PollWorld(h.base, block, model, r, false);
             const uint32_t parts = model + 0x1400, interior = model + 0xA800;
             if (r.Below(4) != 0) {
               // Mostly loaded: nothing pending, every part finished.
               Put(h.base, model + 5028, 0);
               Put(h.base, model + 5040, 0);
               Put(h.base, block + kSmall + 0x500 + 8, r.Below(2));
               for (uint32_t i = 0; i < 2; i++) {
                 Put(h.base, block + kCar + 3972 + 24 * i, 3);
               }
               Put(h.base, block + kCar + 3748, 3);
               Put(h.base, block + kCar + 4088, 3);
               for (const uint32_t at : {0x1400u + 100, 0xA800u + 11660, 0xD800u + 4312,
                                         0xEA00u + 13156, 0x12000u + 16768}) {
                 Put(h.base, model + at, 1);
               }
             }
             Put(h.base, model + 5048, block + kSubs);
             Put(h.base, model + 5060, parts);
             Put(h.base, model + 5068, interior);
             Put(h.base, block + kSmall + 0x7E0 + 4, 0);
             Put(h.base, parts + 24, block + kSubs);
             Put(h.base, parts + 37284, block + kScene);
             Put(h.base, interior + 24, block + kSubs);
             InteriorParts(h.base, block, interior);
             Put(h.base, model + 48, r.Below(3) == 0 ? 5 : r.Below(40));
             Put(h.base, model + 48 + 3680, r.Below(6));
             PartsRecord(h.base, model + 3744, r);
             InteriorRecord(h.base, model + 3804, r);
             t.r3.u64 = model;
           },
           __imp__sub_820DA758, hooks::SceneModelsApplyRecord, fmt::format("case {}", c),
           [&](const PPCContext& t) { applied += t.r3.u32 != 0 ? 1 : 0; });
    }
    ITC_LOG_INFO("harness: load_poll: ApplyRecord got past the poll in {} of 48 cases", applied);
    results.Report();
  }
}

// The garage stage's re-applies (car_makers/stand.cpp, car-makers.md), in
// ApplyRecord's world above (the poll's world: its paints' and light kit's
// objects; the parts and interior given what the applies read), with the
// stage's holder and stage (its +8 the model's index, 0), pool 7's list of
// one node in the case's scene ([0x826C2E44]), the scene model at kObject
// in use (+16 3), ready (+5036) or not, traffic now and then. For
// ReloadDecals' tail, the car model (kCar) as SceneModel::ReloadDecals'
// cases lay it (car_models.cpp): the record's decals (+176, +560: ids small
// or none, copied over the car model's +208 and +592), its +3692 (the car's
// +3724) never 1, the car's first pool set (+7256) with a record list,
// [0x826C2E44]'s shared buffer, and Archive::Load's calls recorded
// (harness::ArchiveLoads). The block (the new vinyl sheet's header and data
// by their offsets in them), the sheet's header and data, the loads asked
// for and r3. Only in the front end: the harness is their check.
// GarageStage::SetCar and SetCarClass, their callers in the same world: a
// car object for SceneModelRecord::FromCar first (car_makers.cpp's
// LayStandCar, in a block of its own, compared too) and the paint pick;
// SetCar's stage sizes (FitCar's) as floats. GarageStage::ShowCarById and
// ShowCarByIdWithClass, theirs: the car made in the frame from its id
// (car_makers.cpp's LayShowCarWorld: the spec Car::SetModel and
// Car::Refresh read, the stand's car list [0x826D83F4], the holder's word
// [0x826D83E0], the parameters [0x826C2E48], all put back after), the rand
// seed (SetModel's) and the frame's car (but the original's register
// saves at its end) compared.
void DecalIds(uint8_t* base, uint32_t records, Random& r) {
  for (uint32_t i = 0; i < 6; i++) {
    Put(base, records + 64 * i, r.Below(3) == 0 ? uint32_t(-1 - int32_t(r.Below(4))) : r.Below(26));
  }
  for (uint32_t k = 0; k < 48; k++) {
    const uint32_t record = records + 384 + 64 * k;
    Put(base, record, r.Below(2) == 0 ? uint32_t(-1 - int32_t(r.Below(4))) : r.Below(300));
    if (r.Below(3) == 0) {
      Put(base, record + 8, 9);
    }
  }
}

struct Known {
  uint32_t address, bytes;
};

void Address(Outputs& out, uint32_t value, const std::vector<Known>& known) {
  for (uint32_t i = 0; i < known.size(); i++) {
    if (value - known[i].address < known[i].bytes) {
      out.Word(1 + i);
      out.Word(value - known[i].address);
      return;
    }
  }
  out.Word(0);
  out.Word(value);
}

void Addresses(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
               const std::vector<Known>& known) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    Address(out, guest::LoadU32(base, address + i), known);
  }
}

}  // namespace

uint32_t LayStandCar(uint8_t* base, uint32_t block, Random& r, int c, bool traffic);
// (The same definition is in car_makers.cpp.)
struct StandWorld {
  uint32_t params, holder_word, list;
};
StandWorld LayShowCarWorld(uint8_t* base, uint32_t block, Random& r, int c, uint32_t holder);

namespace {

void Reapplies(Context& h, Random& r, uint32_t block) {
  constexpr uint32_t kHolder = 0x1A000, kStage = 0x1A040;
  constexpr uint32_t kSharedBuffer = 5056, kSharedBufferSize = 5060;  // car_models/load_slots.h
  constexpr uint32_t kSheetFormat = 0x18280086;                      // car_models/vinyl.h
  constexpr uint32_t kSheetWidth = 0x82602570;                       // car_models/raster.h
  struct Reapply {
    const char* name;
    GuestFunction recompiled, native;
    bool set_car;  // SetCar or SetCarClass: a car object for FromCar first
    bool show_car = false;  // ShowCarById or WithClass: the car made from its id
    bool with_class = false;
  };
  const Reapply reapplies[] = {
      {"GarageStage::ReapplyRecord (sub_82178DF8)", __imp__sub_82178DF8,
       hooks::CarMakersReapplyRecord, false},
      {"GarageStage::ReapplyRecordNoDecals (sub_82178E60)", __imp__sub_82178E60,
       hooks::CarMakersReapplyRecordNoDecals, false},
      {"GarageStage::SetCar (sub_8217FD58)", __imp__sub_8217FD58, hooks::CarMakersSetCar, true},
      {"GarageStage::SetCarClass (sub_8217FDC8)", __imp__sub_8217FDC8,
       hooks::CarMakersSetCarClass, true, false, true},
      {"GarageStage::ShowCarById (sub_82182C90)", __imp__sub_82182C90,
       hooks::CarMakersShowCarById, false, true, false},
      {"GarageStage::ShowCarByIdWithClass (sub_82182D30)", __imp__sub_82182D30,
       hooks::CarMakersShowCarByIdWithClass, false, true, true},
  };
  constexpr uint32_t kParamsGlobal = 0x826C2E48, kHolderGlobal = 0x826D83E0,
                     kCarListGlobal = 0x826D83F4, kRandSeed = 0x82638188;
  const uint32_t model = block + kObject, car = block + kCar;
  // SetCar's and SetCarClass' car object, in a block of its own
  // (car_makers.cpp's LayStandCar), and FitCar's deeper frames.
  constexpr uint32_t kCarBlockBytes = 0x10000, kSetCarStackBytes = 0x3000;
  constexpr uint32_t kShowCarStackBytes = 0x4000;
  const uint32_t car_allocation = memory::Alloc(kCarBlockBytes + 16, 0);
  const uint32_t car_block = (car_allocation + 15) & ~15u;
  for (const Reapply& reapply : reapplies) {
    const bool callees =
        reapply.show_car ? hooks::CarMakersShowCarByIdCalleesNative(reapply.with_class)
        : reapply.set_car
            ? (reapply.with_class ? hooks::CarMakersSetCarClassCalleesNative()
                                  : hooks::CarMakersSetCarCalleesNative())
            : true;
    if (!callees) {
      ITC_LOG_WARN("harness: load_poll: {} skipped (its callees' plain C++ isn't native)",
                   reapply.name);
      continue;
    }
    Results results(reapply.name);
    int ready = 0, sheets = 0;
    const bool car_world = reapply.set_car || reapply.show_car;
    const uint32_t stack_bytes = reapply.show_car ? kShowCarStackBytes
                                 : reapply.set_car ? kSetCarStackBytes
                                                   : kStackBytes;
    for (int c = 0; c < 32; c++) {
      PPCContext start = CallContext(h);
      start.r1.u64 &= ~0xFull;
      const uint32_t stack = start.r1.u32 - stack_bytes;
      RandomBytes(h.base, block, kBlockBytes, r);
      RandomBytes(h.base, stack, stack_bytes + kArgBytes, r);
      for (uint32_t m = 0; m < 8; m++) {
        MakeModel(h.base, block, block + kModels + 0x200 * m, m, r);
      }
      Put(h.base, block + kScene + 4, r.Below(2));
      const uint32_t accessories = block + kSmall + 0x780;
      for (uint32_t i = 0; i < 3; i++) {
        Put(h.base, accessories + 4 + 4 * i, 0);
      }
      if (r.Below(3) == 0) {
        Put(h.base, accessories + 4, block + kSmall + 0x7C0 - 1096);
        Put(h.base, block + kSmall + 0x7C0, r.Below(2));
      }
      const bool traffic = c % 8 == 7;
      PollWorld(h.base, block, model, r, traffic);
      if (traffic) {
        Put(h.base, model + 5092, model + 0x16400);  // the traffic body, always there
      }
      const uint32_t parts = model + 0x1400, interior = model + 0xA800;
      if (r.Below(4) != 0) {
        Put(h.base, model + 5028, 0);
        Put(h.base, model + 5040, 0);
        Put(h.base, block + kSmall + 0x500 + 8, r.Below(2));
        for (uint32_t i = 0; i < 2; i++) {
          Put(h.base, car + 3972 + 24 * i, 3);
        }
        Put(h.base, car + 3748, 3);
        Put(h.base, car + 4088, 3);
        for (const uint32_t at : {0x1400u + 100, 0xA800u + 11660, 0xD800u + 4312,
                                  0xEA00u + 13156, 0x12000u + 16768}) {
          Put(h.base, model + at, 1);
        }
      }
      Put(h.base, model + 5048, block + kSubs);
      Put(h.base, model + 5060, parts);
      Put(h.base, model + 5068, interior);
      Put(h.base, block + kSmall + 0x7E0 + 4, 0);
      Put(h.base, parts + 24, block + kSubs);
      Put(h.base, parts + 37284, block + kScene);
      Put(h.base, interior + 24, block + kSubs);
      InteriorParts(h.base, block, interior);
      Put(h.base, model + 48, r.Below(3) == 0 ? 5 : r.Below(40));
      DecalIds(h.base, model + 48 + 176, r);
      DecalIds(h.base, model + 48 + 560, r);
      Put(h.base, model + 48 + 3692, r.Below(2) == 0 ? 0 : 2 + r.Below(4));
      Put(h.base, model + 48 + 3680, r.Below(6));
      PartsRecord(h.base, model + 3744, r);
      InteriorRecord(h.base, model + 3804, r);
      // Ready at entry in half the cases; the poll readies others.
      if (c % 2 == 0) {
        Put(h.base, model + 5036, 0);
      } else {
        Put(h.base, model + 5036, 1);
      }
      // The car's first pool set and the shared buffer (ReloadDecals').
      const uint32_t first_set = block + kSmall + 0x500, records = block + kSmall + 0x300;
      Put(h.base, car + 7256, first_set);
      Put(h.base, first_set + 40, 1);
      Put(h.base, first_set + 12, r.Below(3));
      Put(h.base, first_set + 52, records);
      for (uint32_t i = 0; i < 12; i++) {
        Put(h.base, records + 20 * i + 16, r.Below(3) == 0 ? 0 : 2);
      }
      Put(h.base, block + kScene + kSharedBuffer, uint32_t(r.Next()));
      Put(h.base, block + kScene + kSharedBufferSize, 0x200000);
      // The holder, the stage and pool 7's list.
      Put(h.base, block + kHolder, block + kStage);
      Put(h.base, block + kStage + 8, 0);
      const uint32_t list = block + kScene + 12 * (7 + 412);
      Put(h.base, list + 0, model);
      Put(h.base, list + 8, 1);
      Put(h.base, model + 8, 0);
      Put(h.base, model + 16, 3);
      start.r3.u64 = block + kHolder;
      std::vector<uint8_t> car_inputs;
      StandWorld show{};
      if (car_world) {
        RandomBytes(h.base, car_block, kCarBlockBytes, r);
        const uint64_t picks[] = {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull, r.Below(6),
                                  r.Below(6), (r.Next() << 32) | r.Below(6)};
        if (reapply.show_car) {
          show = LayShowCarWorld(h.base, car_block, r, c, block + kHolder);
          start.r3.u64 = picks[r.Below(5)];
        } else {
          start.r4.u64 = LayStandCar(h.base, car_block, r, c, traffic);
          start.r5.u64 = picks[r.Below(5)];
        }
        car_inputs = Read(h.base, car_block, kCarBlockBytes);
      }
      const uint32_t saved_params = guest::LoadU32(h.base, kParamsGlobal);
      const uint32_t saved_holder = guest::LoadU32(h.base, kHolderGlobal);
      const uint32_t saved_list = guest::LoadU32(h.base, kCarListGlobal);
      const uint32_t saved_seed = guest::LoadU32(h.base, kRandSeed);

      const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
      const std::vector<uint8_t> frames = Read(h.base, stack, stack_bytes + kArgBytes);
      const uint32_t saved_scene = guest::LoadU32(h.base, kSceneGlobal);
      const uint32_t saved_accessories = guest::LoadU32(h.base, kAccessoriesGlobal);
      const uint32_t old_header = guest::LoadU32(h.base, car + 6684);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), kBlockBytes);
        Write(h.base, stack, frames.data(), stack_bytes + kArgBytes);
        if (car_world) {
          Write(h.base, car_block, car_inputs.data(), kCarBlockBytes);
        }
        if (reapply.show_car) {
          Put(h.base, kParamsGlobal, show.params);
          Put(h.base, kHolderGlobal, show.holder_word);
          Put(h.base, kCarListGlobal, show.list);
          Put(h.base, kRandSeed, saved_seed);
        }
        Put(h.base, kSceneGlobal, block + kScene);
        Put(h.base, kAccessoriesGlobal, accessories);
        std::optional<ArchiveLoads> loads;
        loads.emplace(std::vector<int64_t>{1, 2, -1, 4});
        PPCContext t = start;
        (v == 0 ? reapply.recompiled : reapply.native)(t, h.base);
        Put(h.base, kSceneGlobal, saved_scene);
        Put(h.base, kAccessoriesGlobal, saved_accessories);
        const uint32_t seed_left = guest::LoadU32(h.base, kRandSeed);
        if (reapply.show_car) {
          Put(h.base, kParamsGlobal, saved_params);
          Put(h.base, kHolderGlobal, saved_holder);
          Put(h.base, kCarListGlobal, saved_list);
          Put(h.base, kRandSeed, saved_seed);
        }
        const uint32_t header = guest::LoadU32(h.base, car + 6684);
        const bool made = header != old_header;
        const uint32_t data = guest::LoadU32(h.base, car + 6688);
        const uint32_t bytes = graphics::XGSetTextureHeader(
            h.base, guest::LoadU32(h.base, kSheetWidth), guest::LoadU32(h.base, kSheetWidth + 4),
            1, 2, kSheetFormat, 0, 0, 0, 0, 0, 0);
        std::vector<Known> known;
        if (made) {
          known = {{header, 52}, {data, bytes}};
        }
        if (v == 0) {
          ready += guest::LoadU32(h.base, model + 5036) != 0 || made ? 1 : 0;
          sheets += made ? 1 : 0;
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        if (reapply.set_car || (reapply.show_car && !reapply.with_class)) {
          // FitCar's stage sizes (+6096 to +6204) as floats.
          const uint32_t sizes = block + kStage + 6096;
          Addresses(out[v], h.base, block, sizes - block, known);
          out[v].Floats(h.base, sizes, 28);
          Addresses(out[v], h.base, sizes + 112, block + kBlockBytes - sizes - 112, known);
        } else {
          Addresses(out[v], h.base, block, kBlockBytes, known);
        }
        if (car_world) {
          // The car object's block (FitCar's SetupFromCar may write the car).
          out[v].Bytes(Read(h.base, car_block, kCarBlockBytes));
        }
        if (reapply.show_car) {
          // The rand seed Car::SetModel moves; the frame's car (frame + 80)
          // up to the original's register saves (its last 32 bytes, r1 - 32
          // to r1, which the native version doesn't write), its float blocks
          // (+1888 to +3884) as floats, the rest exactly.
          out[v].Word(seed_left);
          const uint32_t frame_car = start.r1.u32 - 4016 + 80;
          out[v].Bytes(Read(h.base, frame_car, 1888));
          out[v].Floats(h.base, frame_car + 1888, (3884 - 1888) / 4);
          out[v].Bytes(Read(h.base, frame_car + 3884, 4016 - 80 - 32 - 3884));
        }
        if (made) {
          Addresses(out[v], h.base, header, 52, known);
          out[v].Bytes(Read(h.base, data, bytes));
        }
        out[v].Word(uint32_t(loads->calls().size()));
        for (const uint32_t word : loads->calls()) {
          Address(out[v], word, known);
        }
        loads.reset();
        if (made) {
          memory::FreePages(header);
          memory::FreePhysical(data);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    ITC_LOG_INFO("harness: load_poll: {}: ready in {} of 32 cases, a new sheet in {}",
                 reapply.name, ready, sheets);
    results.Report();
  }
  memory::Free(car_allocation);
}

}  // namespace

void LoadPoll(Context& h) {
  if (!hooks::LoadPollCalleesNative()) {
    ITC_LOG_WARN("harness: load_poll skipped (its callees' plain C++ isn't native)");
    return;
  }
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Wheels(h, h.random, block);
  CarModels(h, h.random, block);
  Interiors(h, h.random, block);
  LightsAndTraffic(h, h.random, block);
  Paint(h, h.random, block);
  Builds(h, h.random, block);
  Finishers(h, h.random, block);
  Poll(h, h.random, block);
  Applies(h, h.random, block);
  if (hooks::CarMakersReapplyCalleesNative()) {
    Reapplies(h, h.random, block);
  } else {
    ITC_LOG_WARN("harness: load_poll: the garage stage's re-applies skipped (their callees' "
                 "plain C++ isn't native)");
  }
  memory::Free(allocation);
}

}  // namespace harness
