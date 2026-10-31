// The car model's release (see car_model.h), call for call as the
// recompiled function.

#include "car_models/car_model.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "car_models/raster.h"
#include "car_models/vinyl.h"
#include "model_animation/model_animation.h"
#include "models/part_model.h"
#include "native/buffers.h"
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

int32_t TeamEmblem(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820D3CF8 TeamEmblem");
  // Blocks 18 to 44; 19 to 23 and 26 have none.
  static constexpr int32_t kEmblems[27] = {179, -1,  -1,  -1,  -1,  -1,  168, 169, -1,
                                           170, 171, 172, 173, 174, 175, 176, 177, 178,
                                           184, 185, 186, 187, 188, 180, 181, 182, 183};
  const uint32_t index = block - 18;
  return index > 26 ? -1 : kEmblems[index];
}

uint32_t CarModelLoadDecals(uint8_t* base, uint32_t car_model, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D5B50 CarModel::LoadDecals");
  bool asked = false;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t slot = car_model + 4092 + 48 * i;
    const int32_t index = int32_t(guest::LoadU32(base, car_model + 208 + 64 * i));
    if (index < 0) {
      guest::StoreU32(base, slot + 20, 1);
      continue;
    }
    asked = true;
    LoadSlotLoadShared(base, slot, uint32_t(index + (i < 3 ? 163 : 189)), car_model,
                       kOnDecalLoaded, load);
  }
  static constexpr uint32_t kGroups[8] = {1360, 976, 1744, 2128, 3280, 2896, 2512, 592};
  for (int32_t j = 47; j >= 0; j--) {
    const uint32_t record = car_model + kGroups[j / 6] + 64 * uint32_t(j % 6);
    const uint32_t slot = car_model + 4380 + 48 * uint32_t(j);
    const int32_t index = int32_t(guest::LoadU32(base, record + 0));
    if (index < 0) {
      guest::StoreU32(base, slot + 20, 1);
      continue;
    }
    asked = true;
    const uint32_t id = guest::LoadU32(base, record + 8) == 9 ? uint32_t(index)
                                                              : uint32_t(index + 383);
    LoadSlotLoadShared(base, slot, id, car_model, kOnDecalLoaded, load);
  }
  guest::StoreU32(base, car_model + 4088, 2);
  if (!asked) {
    guest::StoreU32(base, car_model + 4088, 3);
  }
  return 1;
}

std::optional<uint32_t> CarModelLoadExterior(uint8_t* base, uint32_t car_model, uint32_t setup,
                                             uint32_t stack, uint32_t pcr,
                                             const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D5EC8 CarModel::LoadExterior");
  if (!native::Buffers_Memcpy(base, car_model + 32, setup, 3696, 0x820D5EF0)) {
    return std::nullopt;
  }
  TextureCreate(base, kSheetFormat, guest::LoadU32(base, kVinylSheetWidth + 0),
                guest::LoadU32(base, kVinylSheetWidth + 4), car_model + 6684, car_model + 6688,
                stack - 112);
  const uint32_t block = guest::LoadU32(base, car_model + 32);
  if (int32_t(block) >= 18) {
    const int32_t emblem = TeamEmblem(block);
    if (emblem != -1) {
      guest::StoreU32(base, car_model + 592, uint32_t(emblem));
      guest::StoreU32(base, car_model + 600, 9);
    }
  }
  const uint32_t set = guest::LoadU32(base, guest::LoadU32(base, car_model + 7256) + 8);
  if (set == 0) {
    CarModelLoadSetA(base, car_model, load);
  } else if (set == 1) {
    CarModelLoadSetB(base, car_model, pcr, load);
  }
  return CarModelLoadDecals(base, car_model, load);
}

uint32_t CarModelReloadDecals(uint8_t* base, uint32_t car_model, uint32_t stack,
                              const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D5D20 CarModel::ReloadDecals");
  DeferRelease(base, guest::LoadU32(base, car_model + 7256),
               guest::LoadU32(base, car_model + 6684), guest::LoadU32(base, car_model + 6688),
               guest::LoadU32(base, car_model + 6692), guest::LoadU32(base, car_model + 6696));
  guest::StoreU32(base, car_model + 4088, 0);
  for (uint32_t k = 0; k < 3; k++) {
    LoadSlotReset(base, car_model + 4092 + 48 * k);
  }
  for (uint32_t k = 0; k < 6; k++) {
    LoadSlotReset(base, car_model + 4380 + 48 * k);
  }
  TextureCreate(base, kSheetFormat, guest::LoadU32(base, kVinylSheetWidth + 0),
                guest::LoadU32(base, kVinylSheetWidth + 4), car_model + 6684, car_model + 6688,
                stack - 320);
  // (+3724 isn't 1: the adapter's check.)
  const uint32_t block = guest::LoadU32(base, car_model + 32);
  if (int32_t(block) >= 18) {
    const int32_t emblem = TeamEmblem(block);
    if (emblem != -1) {
      guest::StoreU32(base, car_model + 592, uint32_t(emblem));
      guest::StoreU32(base, car_model + 600, 9);
    }
  }
  return CarModelLoadDecals(base, car_model, load);
}

uint32_t CarModelSetVinylTextures(uint8_t* base, uint32_t scene_model) {
  ITC_NATIVE_RUNNING("sub_820D5F88 CarModel::SetVinylTextures");
  uint32_t model = scene_model + 688;
  for (int k = 0; k < 6; k++, model += 464) {
    const uint32_t car_model = guest::LoadU32(base, scene_model + 24);
    models::PartModelSetVinylTextures(base, model, guest::LoadU32(base, car_model + 6684),
                                      guest::LoadU32(base, car_model + 6692));
  }
  return model - 464;
}

}  // namespace car_models
