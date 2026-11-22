// The car models' load slots and the deferred releases (see load_slots.h).

#include "car_models/load_slots.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/last_error.h"
#include "memory/heap.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/scene_loaders.h"

namespace car_models {

void LoadSlotReset(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820D0730 LoadSlot::Reset");
  const schema::LoadSlot s(base, slot);
  s.set_id(0);
  s.set_request(0xFFFFFFFFu);
  s.set_size(0);
  s.set_buffer(0);
  s.set_shared(0);
  s.set_state(0);
}

std::optional<uint32_t> LoadSlotReleaseBuffer(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820D0758 LoadSlot::ReleaseBuffer");
  const schema::LoadSlot s(base, slot);
  uint32_t r3 = slot;
  if (s.shared() == 0) {
    const uint32_t buffer = s.buffer();
    if (buffer != 0) {
      if (!memory::IsPages(buffer)) {
        return std::nullopt;
      }
      memory::FreePages(buffer);
      r3 = 1;
      s.set_buffer(0);
    } else {
      r3 = 0;
    }
  }
  if (s.shared() != 0) {
    s.set_buffer(0);
  }
  return r3;
}

bool LoadSlotReleasable(uint8_t* base, uint32_t slot, uint32_t* freed) {
  const schema::LoadSlot s(base, slot);
  const uint32_t buffer = s.buffer();
  *freed = s.shared() == 0 ? buffer : 0;
  return *freed == 0 || memory::IsPages(buffer);
}

uint32_t LoadSlotLoadShared(uint8_t* base, uint32_t slot, uint32_t id, uint32_t context,
                            uint32_t callback, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D0860 LoadSlot::LoadShared");
  const schema::LoadSlot s(base, slot);
  const auto scene = schema::CarModelsGlobals::scene(base);
  const uint32_t capacity = scene.load_buffer_size();
  const uint32_t buffer = scene.load_buffer();
  s.set_id(id);
  const uint32_t size = archive::AssetSize(base, id);
  s.set_size(size);
  if (size == 0 || capacity <= size) {
    s.set_state(1);
    return 0;
  }
  s.set_buffer(buffer);
  const int64_t serial = load(s.id(), buffer, callback, context);
  s.set_request(uint32_t(serial));
  s.set_state(2);
  s.set_shared(1);
  return 1;
}

uint32_t LoadSlotLoad(uint8_t* base, uint32_t slot, uint32_t id, uint32_t context,
                      uint32_t callback, uint32_t pcr, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D07C8 LoadSlot::Load");
  const schema::LoadSlot s(base, slot);
  s.set_id(id);
  const uint32_t size = archive::AssetSize(base, id);
  s.set_size(size);
  if (size == 0) {
    s.set_state(1);
    return 0;
  }
  const uint32_t pages = memory::AllocPages(size, 4096);
  s.set_buffer(pages);
  if (pages == 0) {
    last_error::Get(base, pcr);  // as the original, its answer not read
  }
  const int64_t serial = load(s.id(), s.buffer(), callback, context);
  s.set_request(uint32_t(serial));
  s.set_state(2);
  return 1;
}

void DeferRelease(uint8_t* base, uint32_t set, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  ITC_NATIVE_RUNNING("sub_820C3F50 DeferRelease");
  const schema::ScenePools pools(base, set);
  const int32_t count = int32_t((pools.traffic_count() + pools.car_model_count()) << 2);
  const uint32_t records = pools.deferred_releases().address;
  using Record = schema::CarModelsDeferredRelease;
  // [set + 52] read again for each store, as the original.
  const auto record = [&](uint32_t at) { return Record(base, pools.deferred_releases().address + at); };
  for (int32_t i = 0; i < count; i++) {
    if (Record(base, records + Record::kSize * uint32_t(i)).state() != 0) {
      continue;
    }
    const uint32_t at = Record::kSize * uint32_t(i);
    record(at).set_first(a);
    record(at).set_first_data(b);
    record(at).set_second(c);
    record(at).set_second_data(d);
    record(at).set_state(2);
    return;
  }
}

std::optional<uint32_t> FreePagesAt(uint8_t* base, uint32_t at) {
  ITC_NATIVE_RUNNING("sub_820C3CE0 FreePagesAt");
  const uint32_t buffer = schema::CarModelsAddressWord(base, at).value();
  if (buffer == 0 || !memory::IsPages(buffer)) {
    return std::nullopt;
  }
  memory::FreePages(buffer);
  return 1;
}

}  // namespace car_models
