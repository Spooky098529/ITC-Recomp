// The scene models' load poll (see load_poll.h), store for store as the
// recompiled functions.

#include "load_poll/load_poll.h"

#include "base/log.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "models/part_model.h"
#include "car_models/car_model.h"
#include "native/buffers.h"
#include "scene_loaders/file_loaded.h"
#include "scene_loaders/pools.h"
#include "second_set/blocks.h"

namespace load_poll {

namespace {

namespace mp = material_passes;
namespace sl = scene_loaders;
namespace ss = second_set;

constexpr uint32_t kScene = 0x826C2E44;  // [kScene] the scene

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
int32_t LoadS(uint8_t* base, uint32_t address) {
  return int32_t(guest::LoadU32(base, address));
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t sp = stack - bytes;
  Store(base, sp, stack);
  return sp;
}

// memcpy from the frame at `sp` (the call's return address `lr`): natively,
// else its original.
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr,
          Calls& calls) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

// The flush mode after a point set-up: its last float step's.
void AfterFind(Calls& calls) {
  calls.flush = calls.find.flush ? ss::Flush::kOn : ss::Flush::kOff;
}
void AfterWheels(Calls& calls) {
  if (calls.wheel.flush != ss::Flush::kUnchanged) {
    calls.flush = calls.wheel.flush;
  }
}

// sub_820D3658 (never seen in play; only InteriorMeters::SetGauge calls it):
// 0x82003600 + 16 x (2 x index + (value >= 3)), as the original's 64-bit
// sum with the base sign-extended.
uint64_t GaugeTableEntry(uint64_t index, int32_t value) {
  ITC_NATIVE_RUNNING("sub_820D3658 GaugeTableEntry");
  const uint64_t r9 = value >= 3 ? 1 : 0;
  const uint64_t r10 = uint64_t(uint32_t(uint32_t(index) << 1)) + r9;
  return uint64_t(uint32_t(uint32_t(r10) << 4)) + uint64_t(int64_t(-2113929216) + 13824);
}

}  // namespace

// --- The first set's wheels ---

uint64_t WheelsSetRecord(uint8_t* base, uint32_t wheels, uint32_t record, uint32_t stack,
                         Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D8EC0 Wheels::SetRecord");
  const uint32_t to = wheels + 28;
  // A tail call: memcpy from the caller's frame, its return address the
  // caller's (Wheels::SetRecord's own entry stands for it in memcpy's logs).
  Copy(base, to, record, 156, stack, 0x820D8EC0, calls);
  return uint64_t(wheels) + 28;
}

uint64_t WheelsSetCaliperTexture(uint8_t* base, uint32_t wheels, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D8ED0 Wheels::SetCaliperTexture");
  const uint32_t sp = Frame(base, stack, 96);
  mp::SetTexture(base, wheels + 2336, Load(base, wheels + 116), -1, sp);
  return mp::SetTexture(base, wheels + 2800, Load(base, wheels + 116), -1, sp);
}

// --- The car model ---

uint64_t CarModelIsReady(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4048 CarModel::IsReady");
  const uint32_t set = Load(base, Load(base, car_model + 7256) + 8);
  if (set > 1) {
    return 0;
  }
  return sl::CarModelIsLoaded(base, car_model);
}

uint64_t CarPartsSetVinylTextures(uint8_t* base, uint32_t parts, uint64_t r3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D72D8 CarParts::SetVinylTextures");
  Frame(base, stack, 128);
  const uint32_t set_at = parts + 131072 - 25072;
  const uint32_t set = Load(base, Load(base, set_at) + 8);
  const int32_t count = set < 1 ? 216 : (set == 1 ? 187 : 0);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t model = parts + 5280 + 464 * uint32_t(i);
    if (LoadS(base, Load(base, set_at) + 8) == 1 && (i == 7 || i == 9 || i == 14)) {
      continue;
    }
    Store(base, model + 48, Load(base, parts + 24) + 4048);
    const uint32_t car = Load(base, parts + 24);
    const uint32_t b = Load(base, car + 6692);
    const uint32_t a = Load(base, car + 6684);
    models::PartModelSetVinylTextures(base, model, a, b);
    r3 = model;
  }
  return r3;
}

// --- The interior ---

uint64_t InteriorDialsSetBlock(uint8_t* base, uint32_t dials, uint64_t block) {
  ITC_NATIVE_RUNNING("sub_820E3040 InteriorDials::SetBlock");
  Store(base, dials + 4, uint32_t(block));
  Store(base, dials + 2896, uint32_t(mp::RecordAt(block)));
  if (mp::IsBlockSetB(LoadS(base, dials + 4)) != 0) {
    Store(base, dials + 8, Load(base, kScene) + 1132);
    return 1;
  }
  if (mp::IsBlockSetA(LoadS(base, dials + 4)) != 0) {
    Store(base, dials + 8, Load(base, kScene) + 1064);
    return 1;
  }
  return 0;
}

uint64_t InteriorMetersSetGauge(uint8_t* base, uint32_t meters, uint64_t gauge, uint64_t entry,
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E32E8 InteriorMeters::SetGauge");
  const uint32_t sp = Frame(base, stack, 112);
  Store(base, uint32_t(uint32_t(gauge + 1324) << 2) + meters, uint32_t(entry));
  // The gauge's point record: its part model and shape.
  static constexpr uint32_t kRecords[7] = {1700, 1940, 2660, 2900, 3620, 3860, 4100};
  uint32_t model = 0;
  uint32_t shape = 0;
  if (uint32_t(gauge) < 7) {
    model = Load(base, meters + kRecords[uint32_t(gauge)]);
    shape = Load(base, meters + kRecords[uint32_t(gauge)] + 224);
  }
  const uint64_t offset = gauge * 12;
  const int32_t scene_flag = LoadS(base, Load(base, kScene) + 4);
  int32_t index = LoadS(base, uint32_t(offset + Load(base, meters)) + 16);
  if (scene_flag != 0 && index > 2) {
    index = 3;
  }
  mp::SetTexture(base, model, uint32_t(index), int32_t(shape), sp);
  const uint32_t at = uint32_t(offset + Load(base, meters));
  const int32_t value = LoadS(base, at + 16);
  const uint64_t r3 = GaugeTableEntry(Load(base, at + 12), value);
  Store(base, uint32_t(uint32_t(gauge + 1332) << 2) + meters, uint32_t(r3));
  return r3;
}

uint64_t SceneInteriorSetGauges(uint8_t* base, uint32_t interior, uint64_t r3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3938 SceneInterior::SetGauges");
  const uint32_t sp = Frame(base, stack, 112);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t block = Load(base, Load(base, Load(base, interior + 24) + 24) + 32);
    if (block == 5 && i == 1) {
      continue;
    }
    const uint32_t entry = interior + 52 + 12 * i;
    if (LoadS(base, entry - 4) == 0) {
      continue;
    }
    const uint64_t word = Load(base, entry);
    const uint64_t scene = Load(base, interior + 11668);
    r3 = InteriorMetersSetGauge(base, interior + 3312, i, scene + word * 20 + 4864, sp);
  }
  return r3;
}

// --- The light kit and the traffic body ---

uint64_t SceneLightsFinish(uint8_t* base, uint32_t kit, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DDA60 SceneLights::Finish");
  const uint32_t sp = Frame(base, stack, 96);
  if (LoadS(base, kit + 13156) != 0) {
    return 1;
  }
  const uint32_t loaded = sl::Pool4IsLoaded(base, Load(base, kit + 28));
  if (loaded == 0) {
    return 0;
  }
  const uint32_t object = Load(base, kit + 28);
  const uint32_t texture = Load(base, kit + 32);
  const uint32_t model = object + 112;
  Store(base, kit + 13156, 1);
  Store(base, kit + 13152, model);
  mp::SetTexture(base, model, texture, -1, sp);
  const uint32_t scene = Load(base, kit + 13160);
  Store(base, Load(base, kit + 13152) + 48, scene + 364);
  ss::LightsFind(base, kit + 96, Load(base, kit + 13152), sp, calls.find);
  AfterFind(calls);
  return 1;
}

uint64_t SceneLightsSetRecord(uint8_t* base, uint32_t kit, uint32_t record, uint32_t stack,
                              Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DCB70 SceneLights::SetRecord");
  const uint32_t sp = Frame(base, stack, 112);
  Copy(base, kit + 32, record, 52, sp, 0x820DCB98, calls);
  const uint32_t object = Load(base, kit + 28);
  if (!sl::Pool4SetRecord(base, object, record)) {
    // Pool4Object::SetRecord's tail call to memcpy, from this frame.
    calls.memcpy(object + 28, record, 52, sp, 0x820DCBA4);
  }
  return uint64_t(object) + 28;
}

uint64_t SceneLightsApplyRecord(uint8_t* base, uint32_t kit, uint32_t record, uint32_t stack,
                                Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DCBC0 SceneLights::ApplyRecord");
  const uint32_t sp = Frame(base, stack, 96);
  for (uint32_t at = 104; at < 136; at += 8) {
    Store64(base, kit + at, 0);
  }
  SceneLightsSetRecord(base, kit, record, sp, calls);
  const uint32_t texture = Load(base, kit + 32);
  return mp::SetTexture(base, Load(base, kit + 13152), texture, -1, sp);
}

uint64_t SceneTrafficFinish(uint8_t* base, uint32_t body, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E5A68 SceneTraffic::Finish");
  const uint32_t sp = Frame(base, stack, 96);
  if (LoadS(base, body + 9792) != 0) {
    return 1;
  }
  const uint32_t traffic = Load(base, body + 9760);
  uint64_t bound;
  if (const auto r3 = sl::TrafficBindTextures(base, traffic, sp)) {
    bound = *r3;
  } else {
    bound = calls.traffic_bind(traffic, sp, 0x820E5A90);
  }
  if (int32_t(bound) == 0) {
    return bound;
  }
  Store(base, body + 9764, Load(base, body + 9760) + 3776);
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, body + 9768 + 4 * i, Load(base, body + 9760) + 464 * i + 4256);
  }
  const uint32_t car = Load(base, body + 9760);
  const uint32_t second = Load(base, car + 7048);
  const uint32_t first = Load(base, car + 7040);
  Store(base, body + 3728 + 1460, second);
  Store(base, body + 3728 + 1456, first);
  ss::TrafficFind(base, body + 3728, body + 9768, sp, calls.find);
  AfterFind(calls);
  return 1;
}

// --- The paint ---

uint64_t ScenePartsApplyPaint(uint8_t* base, uint32_t parts, const uint64_t regs[6],
                              uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0908 SceneParts::ApplyPaint");
  const uint32_t first = Load(base, parts + 104);
  return sl::PartsSetPaint(base, first, first, regs, stack);
}

uint64_t SceneInteriorApplyPaint(uint8_t* base, uint32_t interior, const uint64_t regs[6],
                                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4250 SceneInterior::ApplyPaint");
  const uint32_t first = Load(base, interior + 36);
  return sl::InteriorSetPaint(base, first, first, regs, stack);
}

uint64_t SceneCockpitApplyPaint(uint8_t* base, uint32_t cockpit, const uint64_t regs[6],
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC138 SceneCockpit::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 144);
  const uint32_t body = Load(base, cockpit + 32);
  sl::CockpitSetPaint(base, body, body, regs, sp);
  return SceneInteriorApplyPaint(base, Load(base, cockpit + 28), regs, sp);
}

uint64_t TrafficCarSetPaint(uint8_t* base, uint32_t traffic, uint64_t r3, const uint64_t regs[6],
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D7E18 TrafficCar::SetPaint");
  const uint32_t sp = Frame(base, stack, 160);
  for (uint32_t i = 0; i < 6; i++) {
    if (LoadS(base, traffic + 4124 + 24 * i) == 3) {
      r3 = mp::SetPaint(base, traffic + 4256 + 464 * i, regs, sp);
    }
  }
  return r3;
}

uint64_t SceneTrafficApplyPaint(uint8_t* base, uint32_t body, const uint64_t regs[6],
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E61D0 SceneTraffic::ApplyPaint");
  const uint32_t traffic = Load(base, body + 9760);
  return TrafficCarSetPaint(base, traffic, traffic, regs, stack);
}

uint64_t SceneModelApplyPaint(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9B68 SceneModel::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 96);
  if (LoadS(base, model + 5036) == 0) {
    return r3;
  }
  // The record's three colors, read again before each call.
  auto colors = [&](uint64_t regs[6]) {
    for (uint32_t i = 0; i < 6; i++) {
      regs[i] = Load64(base, model + 64 + 8 * i);
    }
  };
  uint64_t regs[6];
  if (LoadS(base, model + 32) == 3) {
    colors(regs);
    return SceneTrafficApplyPaint(base, Load(base, model + 5092), regs, sp);
  }
  colors(regs);
  r3 = ScenePartsApplyPaint(base, Load(base, model + 5060), regs, sp);
  colors(regs);
  r3 = SceneInteriorApplyPaint(base, Load(base, model + 5068), regs, sp);
  const uint32_t cockpit = Load(base, model + 5076);
  if (cockpit == 0) {
    return r3;
  }
  car_models::CarModelSetVinylTextures(base, Load(base, cockpit + 32));
  colors(regs);
  return SceneCockpitApplyPaint(base, Load(base, model + 5076), regs, sp);
}

uint64_t WheelsApplyPaint(uint8_t* base, uint32_t wheels, const uint64_t regs[7],
                          uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D91E0 Wheels::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 192);
  const uint64_t v0[2] = {regs[0], regs[1]};
  const uint64_t v1[2] = {regs[2], regs[3]};
  const uint64_t v2[2] = {regs[4], regs[5]};
  auto paint = [&](uint32_t model, const uint64_t* a, const uint64_t* b, const uint64_t* c) {
    const uint64_t r[6] = {a[0], a[1], b[0], b[1], c[0], c[1]};
    return mp::SetPaintBody(base, model, r, sp);
  };
  paint(wheels + 240, v1, v0, v2);
  // v3 (r10 and the frame's +88), v4 (+96) and v5 (+112), read after the
  // first call as the original does.
  const uint64_t v5[2] = {Load64(base, stack + 112), Load64(base, stack + 120)};
  const uint64_t v3[2] = {regs[6], Load64(base, stack + 88)};
  const uint64_t v4[2] = {Load64(base, stack + 96), Load64(base, stack + 104)};
  paint(wheels + 704, v4, v3, v5);
  paint(wheels + 1216, v0, v1, v2);
  return paint(wheels + 1680, v3, v4, v5);
}

uint64_t SceneWheelsApplyPaint(uint8_t* base, uint32_t object, const uint64_t regs[7],
                               uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8A00 SceneWheels::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 208);
  Copy(base, sp + 112, sp + 320, 16, sp, 0x820E8A3C, calls);
  Copy(base, sp + 96, sp + 304, 16, sp, 0x820E8A4C, calls);
  Store64(base, sp + 88, Load64(base, sp + 296));
  return WheelsApplyPaint(base, Load(base, object + 28), regs, sp);
}

uint64_t SceneModelApplyWheelPaint(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack,
                                   Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D9C50 SceneModel::ApplyWheelPaint");
  const uint32_t sp = Frame(base, stack, 160);
  if (LoadS(base, model + 5036) == 0 || LoadS(base, model + 32) == 3 ||
      LoadS(base, model + 4072) == 0) {
    return r3;
  }
  const uint32_t wheels = Load(base, model + 5100);
  if (wheels == 0) {
    return r3;
  }
  Copy(base, sp + 112, model + 192, 16, sp, 0x820D9CA8, calls);
  Copy(base, sp + 96, model + 176, 16, sp, 0x820D9CB8, calls);
  const uint64_t last = Load64(base, model + 168);
  uint64_t regs[7];
  for (uint32_t i = 0; i < 7; i++) {
    regs[i] = Load64(base, model + 112 + 8 * i);
  }
  Store64(base, sp + 88, last);
  return SceneWheelsApplyPaint(base, wheels, regs, sp, calls);
}

// --- The finishers' builds ---

uint64_t SceneInteriorFinish(uint8_t* base, uint32_t interior, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E4140 SceneInterior::Finish");
  const uint32_t sp = Frame(base, stack, 128);
  if (LoadS(base, interior + 11660) != 0) {
    return 1;
  }
  const uint64_t loaded = sl::InteriorIsLoaded(base, Load(base, interior + 36));
  if (int32_t(loaded) == 0) {
    return loaded;
  }
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = Load(base, interior + 36) + 464 * i + 304;
    Store(base, interior + 11632 + 4 * i, model);
    if (int32_t(model) == 0) {
      continue;
    }
    Store(base, model + 48, Load(base, interior + 32));
    if (LoadS(base, interior + 11656) != 0) {
      const uint32_t at = Load(base, interior + 11632 + 4 * i);
      Store(base, at + 452, Load(base, interior + 11664));
    } else {
      const uint64_t texture =
          sl::CarModelSelectedTexture(base, Load(base, Load(base, interior + 24) + 24));
      Store(base, Load(base, interior + 11632 + 4 * i) + 452, uint32_t(texture));
    }
  }
  const uint32_t points = interior + 160;
  const uint32_t block = Load(base, Load(base, Load(base, interior + 24) + 24) + 32);
  InteriorDialsSetBlock(base, points + 240, block);
  Store(base, points + 3156, block);
  const uint32_t provider = Load(base, interior + 32);
  const uint32_t scene_model = Load(base, interior + 24);
  Store(base, interior + 8636, provider);
  const uint32_t car = Load(base, scene_model + 24);
  Store(base, interior + 11604, interior + 40);
  Store(base, interior + 11600, car + 7168);
  Store(base, interior + 11608, Load(base, scene_model + 24) + 3760);
  ss::InteriorFind(base, points, interior + 11632, sp, calls.find);
  AfterFind(calls);
  SceneInteriorSetGauges(base, interior, interior, sp);
  Store(base, interior + 11660, 1);
  return loaded;
}

uint64_t SceneWheelsSetUpPoints(uint8_t* base, uint32_t object, uint64_t r3, uint32_t stack,
                                Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8908 SceneWheels::SetUpPoints");
  const uint32_t sp = Frame(base, stack, 112);
  if (LoadS(base, object + 16772) != 0) {
    return r3;
  }
  const uint32_t car = Load(base, object + 24);
  if (car != 0) {
    Store(base, object + 192, car);
  } else {
    const uint32_t record = Load(base, object + 16776);
    const uint32_t tires = Load(base, object + 16780);
    Store(base, object + 196, record);
    Store(base, object + 200, tires);
  }
  const uint32_t wheels = Load(base, object + 28);
  Store(base, object + 204, object + 32);
  Store(base, object + 208, wheels + 240);
  Store(base, object + 212, wheels + 1216);
  Store(base, object + 216, wheels + 2336);
  ss::WheelPointsSetUp(base, object + 192, sp, calls.wheel);
  AfterWheels(calls);
  WheelsSetRecord(base, Load(base, object + 28), object + 32, sp, calls);
  const uint64_t left = WheelsSetCaliperTexture(base, Load(base, object + 28), sp);
  Store(base, object + 16772, 1);
  return left;
}

uint64_t ScenePartsSetSelectedTexture(uint8_t* base, uint32_t parts, uint64_t r3,
                                      uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF250 SceneParts::SetSelectedTexture");
  Frame(base, stack, 112);
  for (uint32_t k = 0; k < 17; k++) {
    if (Load(base, parts + 32 + 4 * k) == 0) {
      continue;
    }
    r3 = sl::CarModelSelectedTexture(base, Load(base, Load(base, parts + 24) + 24));
    Store(base, Load(base, parts + 32 + 4 * k) + 452, uint32_t(r3));
  }
  return r3;
}

uint64_t ScenePartsBuild(uint8_t* base, uint32_t parts, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E05E0 SceneParts::Build");
  const uint32_t sp = Frame(base, stack, 112);
  // Each kind's record words (r5, r6): an offset into the parts object, or
  // 0 for the constant 0.
  struct Words {
    uint32_t a, b;
  };
  static constexpr Words kWords[17] = {
      {108, 0},   {112, 108}, {116, 0},   {120, 0}, {124, 0},   {128, 0},
      {132, 0},   {132, 0},   {136, 132}, {136, 132}, {140, 0}, {144, 0},
      {148, 0},   {0, 0},     {152, 0},   {156, 0}, {0, 0}};
  for (uint32_t k = 0; k < 17; k++) {
    const uint32_t a = kWords[k].a == 0 ? 0 : Load(base, parts + kWords[k].a);
    const uint32_t b = kWords[k].b == 0 ? 0 : Load(base, parts + kWords[k].b);
    if (k == 4 && int32_t(a) == 13) {
      Store(base, parts + 48, 0);
      continue;
    }
    const uint32_t model = sl::PartsPart(base, Load(base, parts + 104), int32_t(k), a, b);
    Store(base, parts + 32 + 4 * k, model);
  }
  const uint32_t points = parts + 176;
  ss::PartsPointsReset(base, points, sp);
  const uint32_t scene_model = Load(base, parts + 24);
  Store(base, parts + 37260, scene_model);
  const uint32_t car = Load(base, scene_model + 24);
  Store(base, parts + 37256, parts + 108);
  Store(base, parts + 37252, car + 32);
  Store(base, parts + 37268, Load(base, scene_model + 24) + 3760);
  ss::PartsFind(base, points, parts + 32, sp, calls.find);
  AfterFind(calls);
  sl::PartsWalkMaterials(base, Load(base, parts + 104));
  return ScenePartsSetSelectedTexture(base, parts, parts, sp);
}

}  // namespace load_poll
