// The scene models' load poll (see load_poll.h), store for store as the
// recompiled functions.

#include "load_poll/load_poll.h"

#include "base/log.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "models/part_model.h"
#include "native/buffers.h"
#include "scene_loaders/file_loaded.h"
#include "scene_loaders/pools.h"

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
  calls.find.flush = false;
  const uint32_t scene = Load(base, kit + 13160);
  Store(base, Load(base, kit + 13152) + 48, scene + 364);
  ss::LightsFind(base, kit + 96, Load(base, kit + 13152), sp, calls.find);
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
  return 1;
}

}  // namespace load_poll
