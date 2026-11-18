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
#include "schema/car_models.h"
#include "scene_loaders/objects.h"
#include "scene_models/scene_models.h"

#include <cstdlib>

namespace car_models {

namespace {

using schema::CarModel;
using G = schema::CarModelsGlobals;

// The node file's slot, the XPR2s' slots.
uint32_t Slot(const CarModel& car, uint32_t i) {
  return i == 0 ? car.node_file_slot_address() : car.xpr_slots_address() + 24 * (i - 1);
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

}  // namespace

bool CarModelReleasable(uint8_t* base, uint32_t car_model) {
  const CarModel car(base, car_model);
  uint32_t freed[3] = {};
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = Slot(car, i);
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
  const CarModel car(base, car_model);
  Require(LoadSlotReleaseBuffer(base, Slot(car, 0)).has_value(), "LoadSlot::ReleaseBuffer");
  Require(model_animation::SkeletonFreeNodeValues(base, car.skeleton_address()),
          "Skeleton::FreeNodeValues");
  Require(LoadSlotReleaseBuffer(base, Slot(car, 1)).has_value(), "LoadSlot::ReleaseBuffer");
  Require(LoadSlotReleaseBuffer(base, Slot(car, 2)).has_value(), "LoadSlot::ReleaseBuffer");
  {
    const uint32_t d = car.unknown_6696();
    const uint32_t c = car.unknown_6692();
    const uint32_t b = car.vinyl_sheet_data();
    const uint32_t a = car.vinyl_sheet();
    DeferRelease(base, car.first_set().address, a, b, c, d);
  }
  {
    const uint32_t d = car.plate_texture_b_data();
    const uint32_t c = car.plate_texture_b();
    const uint32_t b = car.plate_texture_a_data();
    const uint32_t a = car.plate_texture_a();
    DeferRelease(base, car.first_set().address, a, b, c, d);
  }
  const auto r3 = scene_loaders::CarModelReset(base, car_model);
  Require(r3.has_value(), "CarModel::Reset");
  return r3;
}

int64_t CarModelLoadSetA(uint8_t* base, uint32_t car_model, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D59D8 CarModel::LoadSetA");
  const CarModel car(base, car_model);
  car.set_node_file_id(car.block() * 226 + 893);
  car.set_node_file_size(archive::AssetSize(base, car.node_file_id()));
  int64_t r3 = scene_models::AllocPages(base, car.node_file_buffer_address(), car.node_file_size());
  if (car.node_file_size() > 0) {
    r3 = load(car.node_file_id(), car.node_file_buffer(), kOnFileLoaded, car_model);
    car.set_node_file_state(2);
    car.set_node_file_request(uint32_t(r3));
  }
  car.set_body_xpr_id(car.block() * 226 + 891);
  car.set_body_xpr_size(archive::AssetSize(base, car.body_xpr_id()));
  r3 = scene_models::AllocPages(base, car.body_xpr_buffer_address(), car.body_xpr_size());
  if (car.body_xpr_size() != 0) {
    r3 = load(car.body_xpr_id(), car.body_xpr_buffer(), kOnFileLoaded, car_model);
    car.set_body_xpr_state(2);
    car.set_body_xpr_request(uint32_t(r3));
  } else {
    car.set_body_xpr_state(1);
  }
  return r3;
}

int64_t CarModelLoadSetB(uint8_t* base, uint32_t car_model, uint32_t pcr,
                          const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D5AC0 CarModel::LoadSetB");
  const CarModel car(base, car_model);
  LoadSlotLoad(base, car.node_file_slot_address(), car.block() * 197 + 11063, car_model,
               kOnFileLoadedB, pcr, load);
  const uint32_t block = car.block();
  const uint32_t buffer =
      guest::LoadU32(base, G::scene<schema::View>(base).address + kSharedBuffer);
  const uint32_t id = block * 197 + 11061;
  car.set_body_xpr_id(id);
  car.set_body_xpr_size(archive::AssetSize(base, id));
  const int64_t serial = load(id, buffer, kOnFileLoadedB, car_model);
  car.set_body_xpr_request(uint32_t(serial));
  car.set_body_xpr_state(2);
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
  const CarModel car(base, car_model);
  bool asked = false;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t slot = car.layer_slots_address() + 48 * i;
    const int32_t index = int32_t(
        schema::CarModelDecalRecord(base, car.vinyl_layers_address() + 64 * i).index());
    if (index < 0) {
      schema::LoadSlot(base, slot).set_state(1);
      continue;
    }
    asked = true;
    LoadSlotLoadShared(base, slot, uint32_t(index + (i < 3 ? 163 : 189)), car_model,
                       kOnDecalLoaded, load);
  }
  static constexpr uint32_t kGroups[8] = {1360, 976, 1744, 2128, 3280, 2896, 2512, 592};
  for (int32_t j = 47; j >= 0; j--) {
    const schema::CarModelDecalRecord record(base,
                                             car_model + kGroups[j / 6] + 64 * uint32_t(j % 6));
    const uint32_t slot = car.sticker_slots_address() + 48 * uint32_t(j);
    const int32_t index = int32_t(record.index());
    if (index < 0) {
      schema::LoadSlot(base, slot).set_state(1);
      continue;
    }
    asked = true;
    const uint32_t id = record.region() == 9 ? uint32_t(index) : uint32_t(index + 383);
    LoadSlotLoadShared(base, slot, id, car_model, kOnDecalLoaded, load);
  }
  car.set_decals_state(2);
  if (!asked) {
    car.set_decals_state(3);
  }
  return 1;
}

std::optional<uint32_t> CarModelLoadExterior(uint8_t* base, uint32_t car_model, uint32_t setup,
                                             uint32_t stack, uint32_t pcr,
                                             const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D5EC8 CarModel::LoadExterior");
  const CarModel car(base, car_model);
  if (!native::Buffers_Memcpy(base, car.record_address(), setup, 3696, 0x820D5EF0)) {
    return std::nullopt;
  }
  TextureCreate(base, kSheetFormat, G::vinyl_sheet_width(base), G::vinyl_sheet_height(base),
                car.vinyl_sheet_address(), car.vinyl_sheet_data_address(), stack - 112);
  const uint32_t block = car.block();
  if (int32_t(block) >= 18) {
    const int32_t emblem = TeamEmblem(block);
    if (emblem != -1) {
      car.set_emblem_sticker(uint32_t(emblem));
      car.set_emblem_sticker_region(9);
    }
  }
  const uint32_t set = car.first_set().model_set();
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
  const CarModel car(base, car_model);
  DeferRelease(base, car.first_set().address, car.vinyl_sheet(), car.vinyl_sheet_data(),
               car.unknown_6692(), car.unknown_6696());
  car.set_decals_state(0);
  for (uint32_t k = 0; k < 3; k++) {
    LoadSlotReset(base, car.layer_slots_address() + 48 * k);
  }
  for (uint32_t k = 0; k < 6; k++) {
    LoadSlotReset(base, car.sticker_slots_address() + 48 * k);
  }
  TextureCreate(base, kSheetFormat, G::vinyl_sheet_width(base), G::vinyl_sheet_height(base),
                car.vinyl_sheet_address(), car.vinyl_sheet_data_address(), stack - 320);
  // (+3724 isn't 1: the adapter's check.)
  const uint32_t block = car.block();
  if (int32_t(block) >= 18) {
    const int32_t emblem = TeamEmblem(block);
    if (emblem != -1) {
      car.set_emblem_sticker(uint32_t(emblem));
      car.set_emblem_sticker_region(9);
    }
  }
  return CarModelLoadDecals(base, car_model, load);
}

uint32_t CarModelSetVinylTextures(uint8_t* base, uint32_t scene_model) {
  ITC_NATIVE_RUNNING("sub_820D5F88 CarModel::SetVinylTextures");
  uint32_t model = scene_model + 688;
  for (int k = 0; k < 6; k++, model += 464) {
    const CarModel car(base, guest::LoadU32(base, scene_model + 24));
    models::PartModelSetVinylTextures(base, model, car.vinyl_sheet(), car.unknown_6692());
  }
  return model - 464;
}

}  // namespace car_models
