// The car's part loads (see parts.h), call for call as the recompiled
// functions.

#include "car_models/parts.h"

#include "base/log.h"
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

}  // namespace car_models
