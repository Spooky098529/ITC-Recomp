// The car models' load slots and the deferred releases (see load_slots.h).

#include "car_models/load_slots.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"

namespace car_models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

}  // namespace

void LoadSlotReset(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820D0730 LoadSlot::Reset");
  Store(base, slot + 0, 0);
  Store(base, slot + 4, 0xFFFFFFFFu);
  Store(base, slot + 8, 0);
  Store(base, slot + 16, 0);
  Store(base, slot + 12, 0);
  Store(base, slot + 20, 0);
}

std::optional<uint32_t> LoadSlotReleaseBuffer(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820D0758 LoadSlot::ReleaseBuffer");
  uint32_t r3 = slot;
  if (Load(base, slot + 12) == 0) {
    const uint32_t buffer = Load(base, slot + 16);
    if (buffer != 0) {
      if (!memory::IsPages(buffer)) {
        return std::nullopt;
      }
      memory::FreePages(buffer);
      r3 = 1;
      Store(base, slot + 16, 0);
    } else {
      r3 = 0;
    }
  }
  if (Load(base, slot + 12) != 0) {
    Store(base, slot + 16, 0);
  }
  return r3;
}

bool LoadSlotReleasable(uint8_t* base, uint32_t slot, uint32_t* freed) {
  const uint32_t buffer = Load(base, slot + 16);
  *freed = Load(base, slot + 12) == 0 ? buffer : 0;
  return *freed == 0 || memory::IsPages(buffer);
}

void DeferRelease(uint8_t* base, uint32_t set, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  ITC_NATIVE_RUNNING("sub_820C3F50 DeferRelease");
  const int32_t count = int32_t((Load(base, set + 40) + Load(base, set + 12)) << 2);
  const uint32_t records = Load(base, set + 52);
  for (int32_t i = 0; i < count; i++) {
    if (Load(base, records + 20 * uint32_t(i) + 16) != 0) {
      continue;
    }
    const uint32_t at = 20 * uint32_t(i);
    Store(base, Load(base, set + 52) + at, a);
    Store(base, Load(base, set + 52) + at + 4, b);
    Store(base, Load(base, set + 52) + at + 8, c);
    Store(base, Load(base, set + 52) + at + 12, d);
    Store(base, Load(base, set + 52) + at + 16, 2);
    return;
  }
}

}  // namespace car_models
