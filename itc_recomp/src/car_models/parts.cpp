// The car's part loads (see parts.h), call for call as the recompiled
// functions.

#include "car_models/parts.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "native/buffers.h"

namespace car_models {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// One end of the rims or brakes: `n` the wheel number, `block` the car's;
// the part's id `part` for a block above 19, else 3 x block +
// `part_block_base`, or 3n + `part_wheel_base` for a wheel.
void WheelEnd(uint8_t* base, uint32_t xpr_at, uint32_t part_at, int32_t n, int32_t block,
              uint32_t part, uint32_t part_block_base, uint32_t part_wheel_base) {
  if (n == -1) {
    if (block > 19) {
      Store(base, part_at, part);
      Store(base, xpr_at, 695);
      return;
    }
    Store(base, xpr_at, 3 * uint32_t(block) + 587);
    Store(base, part_at, 3 * uint32_t(block) + part_block_base);
    return;
  }
  Store(base, xpr_at, 3 * uint32_t(n) + 695);
  Store(base, part_at, 3 * uint32_t(n) + part_wheel_base);
}

}  // namespace

void WheelsRims(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8BA8 Wheels::Rims");
  const int32_t block = int32_t(Load(base, Load(base, wheels + 24) + 32));
  WheelEnd(base, wheels + 2144, wheels + 1168, int32_t(Load(base, wheels + 44)), block, 696,
           588, 696);
  WheelEnd(base, wheels + 2168, wheels + 1192, int32_t(Load(base, wheels + 60)), block, 696,
           588, 696);
}

void WheelsBrakes(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8C50 Wheels::Brakes");
  if (Load(base, wheels + 28) == 3) {
    const uint32_t x = 3 * (Load(base, Load(base, wheels + 3264)) + 20);
    Store(base, wheels + 1168, x + 589);
    Store(base, wheels + 2144, x + 587);
    Store(base, wheels + 1192, x + 589);
    Store(base, wheels + 2168, x + 587);
    return;
  }
  const int32_t block = int32_t(Load(base, Load(base, wheels + 24) + 32));
  WheelEnd(base, wheels + 2144, wheels + 1168, int32_t(Load(base, wheels + 44)), block, 697,
           589, 697);
  WheelEnd(base, wheels + 2168, wheels + 1192, int32_t(Load(base, wheels + 60)), block, 697,
           589, 697);
}

std::optional<uint32_t> WheelsSetUp(uint8_t* base, uint32_t wheels, uint32_t setup,
                                    const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D9110 Wheels::SetUp");
  if (!native::Buffers_Memcpy(base, wheels + 28, setup, 156, 0x820D912C)) {
    return std::nullopt;
  }
  if (Load(base, Load(base, wheels + 3268) + 8) == 0) {
    WheelsRims(base, wheels);
  } else {
    WheelsBrakes(base, wheels);
  }
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t part = wheels + 1168 + 24 * i;
    const uint32_t tire = Load(base, wheels + 144 + 4 * i) == 0 ? 582 : 584;
    LoadSlotLoadShared(base, part - 976, tire, wheels, kOnWheelPartLoaded, load);
    LoadSlotLoadShared(base, part, Load(base, part), wheels, kOnWheelPartLoaded, load);
    LoadSlotLoadShared(base, part + 976, Load(base, part + 976), wheels, kOnWheelPartLoaded,
                       load);
    r3 = LoadSlotLoadShared(base, part + 1112, 887, wheels, kOnWheelPartLoaded, load);
  }
  return r3;
}

std::optional<uint32_t> CockpitLoadSetC(uint8_t* base, uint32_t object, uint32_t setup,
                                        uint32_t stack, uint32_t pcr,
                                        const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D67E8 CarInterior::LoadSetCBody");
  if (!native::Buffers_Memcpy(base, object + 28, setup, 144, 0x820D6808)) {
    return std::nullopt;
  }
  const uint32_t block = Load(base, Load(base, object + 24) + 32);
  const uint32_t n = block * 55;
  if (LoadSlotLoad(base, object + 176, n + 19928, object, kOnCockpitPartLoaded, pcr, load) == 0) {
    // sprintf's call from the original's 1,152-byte frame: the block spilled
    // into the frame's parameter area as sprintf spills r5 (r6 to r10, which
    // the call doesn't set, as 0: only dead frame words differ), its own
    // frames below. Its text, in the frame, is read by nothing.
    const uint32_t frame = stack - 1152;
    for (uint32_t i = 0; i < 6; i++) {
      Store(base, frame + 32 + 8 * i, 0);
      Store(base, frame + 36 + 8 * i, i == 0 ? block : 0);
    }
    int32_t written;
    c_runtime::Sprintf(base, frame + 80, kCockpitNodeFormat, frame + 32, frame - 144 - 1328,
                       &written);
  }
  const uint32_t body = n + 19926;
  const uint32_t buffer = Load(base, Load(base, kSceneSet) + kSharedBuffer);
  Store(base, object + 400, body);
  Store(base, object + 408, archive::AssetSize(base, body));
  const int64_t serial = load(body, buffer, kOnCockpitPartLoaded, object);
  Store(base, object + 404, uint32_t(serial));
  Store(base, object + 420, 2);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    uint32_t id;
    switch (i) {
      case 0: id = Load(base, object + 28) + body + 3; break;
      case 1: id = Load(base, object + 32) + body + 16; break;
      case 2: id = Load(base, object + 36) + body + 19; break;
      case 3: id = Load(base, object + 40) + body + 31; break;
      case 4: id = Load(base, object + 44) + body + 36; break;
      default: id = body + 35; break;
    }
    r3 = LoadSlotLoadShared(base, object + 536 + 24 * i, id, object, kOnCockpitPartLoaded, load);
  }
  return r3;
}

uint32_t InteriorLoadParts(uint8_t* base, uint32_t object, uint32_t first, uint32_t stride,
                           uint32_t offset, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7730 CarInterior::LoadParts");
  const int32_t block = int32_t(Load(base, Load(base, object + 24) + 32));
  const uint32_t id = uint32_t(int64_t(block) * int64_t(int32_t(stride))) + first + offset;
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    r3 = LoadSlotLoadShared(base, object + 160 + 24 * i, id + i, object, kOnInteriorPartLoaded,
                            load);
  }
  return r3;
}

}  // namespace car_models
