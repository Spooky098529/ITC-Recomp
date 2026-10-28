// The car model's release (see car_model.h), call for call as the
// recompiled function.

#include "car_models/car_model.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "model_animation/model_animation.h"
#include "scene_loaders/objects.h"

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

std::optional<uint32_t> CarModelRelease(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4508 CarModel::Release");
  uint32_t freed[3] = {};
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = car_model + kSlots[i];
    if (!LoadSlotReleasable(base, slot, &freed[i])) {
      return std::nullopt;
    }
    // Two slots freeing the same pages: the second free would fail.
    for (uint32_t j = 0; j < i; j++) {
      if (freed[i] != 0 && freed[i] == freed[j]) {
        return std::nullopt;
      }
    }
  }
  // CarModel::Reset's checks hold after the frees as before them (the node
  // values it checks are the ones freed first).
  if (!scene_loaders::CarModelResettable(base, car_model)) {
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

}  // namespace car_models
