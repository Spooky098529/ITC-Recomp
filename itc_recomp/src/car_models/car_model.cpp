// The car model's release (see car_model.h), call for call as the
// recompiled function.

#include "car_models/car_model.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "model_animation/model_animation.h"
#include "scene_loaders/objects.h"
#include "scene_models/scene_models.h"

#include <cstdlib>

namespace car_models {

namespace {

constexpr uint32_t kSlots[] = {3728, 3952, 3976};  // the node file's, the XPR2s'
constexpr uint32_t kSkeleton = 3760;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

}  // namespace

bool CarModelReleasable(uint8_t* base, uint32_t car_model) {
  uint32_t freed[3] = {};
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = car_model + kSlots[i];
    if (!LoadSlotReleasable(base, slot, &freed[i])) {
      return false;
    }
    // Two slots freeing the same pages: the second free would fail.
    for (uint32_t j = 0; j < i; j++) {
      if (freed[i] != 0 && freed[i] == freed[j]) {
        return false;
      }
    }
  }
  // CarModel::Reset's checks hold after the frees as before them (the node
  // values it checks are the ones freed first).
  return scene_loaders::CarModelResettable(base, car_model);
}

std::optional<uint32_t> CarModelRelease(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4508 CarModel::Release");
  if (!CarModelReleasable(base, car_model)) {
    return std::nullopt;
  }
  Require(LoadSlotReleaseBuffer(base, car_model + kSlots[0]).has_value(),
          "LoadSlot::ReleaseBuffer");
  Require(model_animation::SkeletonFreeNodeValues(base, car_model + kSkeleton),
          "Skeleton::FreeNodeValues");
  Require(LoadSlotReleaseBuffer(base, car_model + kSlots[1]).has_value(),
          "LoadSlot::ReleaseBuffer");
  Require(LoadSlotReleaseBuffer(base, car_model + kSlots[2]).has_value(),
          "LoadSlot::ReleaseBuffer");
  for (uint32_t sheet : {6684u, 6700u}) {
    const uint32_t d = Load(base, car_model + sheet + 12);
    const uint32_t c = Load(base, car_model + sheet + 8);
    const uint32_t b = Load(base, car_model + sheet + 4);
    const uint32_t a = Load(base, car_model + sheet + 0);
    DeferRelease(base, Load(base, car_model + scene_loaders::kCarModelSet), a, b, c, d);
  }
  const auto r3 = scene_loaders::CarModelReset(base, car_model);
  Require(r3.has_value(), "CarModel::Reset");
  return r3;
}

int64_t CarModelLoadSetA(uint8_t* base, uint32_t car_model, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D59D8 CarModel::LoadSetA");
  const auto load_u32 = [base](uint32_t address) { return guest::LoadU32(base, address); };
  const auto store_u32 = [base](uint32_t address, uint32_t value) {
    guest::StoreU32(base, address, value);
  };
  store_u32(car_model + 3728, load_u32(car_model + 32) * 226 + 893);
  store_u32(car_model + 3736, archive::AssetSize(base, load_u32(car_model + 3728)));
  int64_t r3 = scene_models::AllocPages(base, car_model + 3744, load_u32(car_model + 3736));
  if (load_u32(car_model + 3736) > 0) {
    r3 = load(load_u32(car_model + 3728), load_u32(car_model + 3744), kOnFileLoaded, car_model);
    store_u32(car_model + 3748, 2);
    store_u32(car_model + 3732, uint32_t(r3));
  }
  store_u32(car_model + 3952, load_u32(car_model + 32) * 226 + 891);
  store_u32(car_model + 3960, archive::AssetSize(base, load_u32(car_model + 3952)));
  r3 = scene_models::AllocPages(base, car_model + 3968, load_u32(car_model + 3960));
  if (load_u32(car_model + 3960) != 0) {
    r3 = load(load_u32(car_model + 3952), load_u32(car_model + 3968), kOnFileLoaded, car_model);
    store_u32(car_model + 3972, 2);
    store_u32(car_model + 3956, uint32_t(r3));
  } else {
    store_u32(car_model + 3972, 1);
  }
  return r3;
}

int64_t CarModelLoadSetB(uint8_t* base, uint32_t car_model, uint32_t pcr,
                          const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D5AC0 CarModel::LoadSetB");
  LoadSlotLoad(base, car_model + 3728, guest::LoadU32(base, car_model + 32) * 197 + 11063,
               car_model, kOnFileLoadedB, pcr, load);
  const uint32_t block = guest::LoadU32(base, car_model + 32);
  const uint32_t buffer = guest::LoadU32(base, guest::LoadU32(base, kSceneSet) + kSharedBuffer);
  const uint32_t id = block * 197 + 11061;
  guest::StoreU32(base, car_model + 3952, id);
  guest::StoreU32(base, car_model + 3960, archive::AssetSize(base, id));
  const int64_t serial = load(id, buffer, kOnFileLoadedB, car_model);
  guest::StoreU32(base, car_model + 3956, uint32_t(serial));
  guest::StoreU32(base, car_model + 3972, 2);
  return serial;
}

}  // namespace car_models
