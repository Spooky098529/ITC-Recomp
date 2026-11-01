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
#include "hooks/load_poll.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x48000;
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
          GuestFunction native, const std::string& what) {
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
  memory::Free(allocation);
}

}  // namespace harness
