// The loading objects' embedded loads (see loads.h), member by member in the
// originals' order.

#include "scene_loaders/loads.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "scenery_lists/scenery_lists.h"
#include "schema/model_animation.h"
#include "schema/models.h"
#include "schema/scene_loaders.h"

#include <array>
#include <cstdlib>

namespace scene_loaders {

namespace {

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene loaders: {} refused after its checks", what);
    std::abort();
  }
}

// Whether TextureProvider::~TextureProvider frees the provider's storage
// natively (none, or a block the layer gave).
bool ProviderDestructible(uint8_t* base, uint32_t provider) {
  const uint32_t storage = schema::TextureProvider(base, provider).first().address;
  return storage == 0 || memory::IsHeapBlock(storage);
}

// The members CarModelLoads, CockpitLoads and TrafficLoads share (at the
// layout's offsets), made and destroyed with their checks made.
void ConstructMembers(uint8_t* base, uint32_t loads, const MembersLayout& m = kCarModelMembers);
void DestructMembers(uint8_t* base, uint32_t loads, const MembersLayout& m = kCarModelMembers);
// Whether DestructMembers runs natively, the pending frees apart.
bool MembersDestructible(uint8_t* base, uint32_t loads, const MembersLayout& m);

}  // namespace

bool CarModelLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3DE0 CarModelLoads::CarModelLoads");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  ConstructMembers(base, loads);
  return true;
}

namespace {

void ConstructMembers(uint8_t* base, uint32_t loads, const MembersLayout& m) {
  model_animation::SkeletonConstruct(base, loads + m.skeleton);
  for (uint32_t offset : m.xprs) {
    models::XprConstruct(base, loads + offset);
  }
  for (uint32_t offset : m.providers) {
    Require(models::TextureProviderConstruct(base, loads + offset, -1).has_value(),
            "TextureProvider::TextureProvider");
  }
}

}  // namespace

namespace {

bool MembersDestructible(uint8_t* base, uint32_t loads, const MembersLayout& m) {
  return ProviderDestructible(base, loads + m.providers[0]) &&
         ProviderDestructible(base, loads + m.providers[1]) &&
         model_animation::SkeletonDestructible(base, loads + m.skeleton);
}

}  // namespace

bool CarModelLoadsDestructible(uint8_t* base, uint32_t loads) {
  // XPR::Release checks for two nodes each time.
  return MembersDestructible(base, loads, kCarModelMembers) &&
         models::PendingFreesHaveRoom(base, 4);
}

bool ProvidersClearAll(uint8_t* base, uint32_t loads, const MembersLayout& m) {
  for (uint32_t offset : m.providers) {
    if (schema::TextureProvider(base, loads + offset).vtable().clear_all() !=
        kTextureProviderClearAll) {
      return false;
    }
  }
  return true;
}

bool CarModelLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3E40 CarModelLoads::~CarModelLoads");
  if (!CarModelLoadsDestructible(base, loads)) {
    return false;
  }
  DestructMembers(base, loads);
  return true;
}

namespace {

void DestructMembers(uint8_t* base, uint32_t loads, const MembersLayout& m) {
  Require(models::TextureProviderDestruct(base, loads + m.providers[1]).has_value(),
          "TextureProvider::~TextureProvider");
  Require(models::TextureProviderDestruct(base, loads + m.providers[0]).has_value(),
          "TextureProvider::~TextureProvider");
  Require(models::XprDestruct(base, loads + m.xprs[1]).has_value(), "XPR::~XPR");
  Require(models::XprDestruct(base, loads + m.xprs[0]).has_value(), "XPR::~XPR");
  Require(model_animation::SkeletonDestruct(base, loads + m.skeleton), "Skeleton::~Skeleton");
}

// Six part models from `first` (the cockpit's and the traffic body's): the
// pending frees' nodes their destructors may add, or nullopt where one's
// members wouldn't be destroyed natively.
std::optional<uint64_t> SixModelsNodes(uint8_t* base, uint32_t first) {
  uint64_t nodes = 0;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = first + models::kPartModelBytes * i;
    if (!models::ModelMembersDestructible(base, model)) {
      return std::nullopt;
    }
    nodes += models::ModelReleaseNodes(base, model);
  }
  return nodes;
}

bool Room(uint8_t* base, uint64_t nodes) {
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(base, uint32_t(nodes));
}

// Loads with members at `m` and six part models from +`parts`: constructed
// or destroyed, the originals' orders.
bool ConstructWithParts(uint8_t* base, uint32_t loads, const MembersLayout& m, uint32_t parts) {
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  ConstructMembers(base, loads, m);
  for (uint32_t i = 0; i < 6; i++) {
    Require(models::PartModelConstruct(base, loads + parts + models::kPartModelBytes * i),
            "PartModel::PartModel");
  }
  return true;
}

std::optional<uint32_t> DestructWithParts(uint8_t* base, uint32_t loads, const MembersLayout& m,
                                          uint32_t parts) {
  const auto nodes = SixModelsNodes(base, loads + parts);
  if (!nodes || !Room(base, *nodes + 4) || !MembersDestructible(base, loads, m)) {
    return std::nullopt;
  }
  for (uint32_t i = 6; i-- > 0;) {
    Require(models::PartModelDestruct(base, loads + parts + models::kPartModelBytes * i)
                .has_value(),
            "PartModel::~PartModel");
  }
  DestructMembers(base, loads, m);
  return loads + m.skeleton;
}

}  // namespace

bool CockpitLoadsMembersDestructible(uint8_t* base, uint32_t loads) {
  return SixModelsNodes(base, loads + kCockpitLoadsParts).has_value() &&
         MembersDestructible(base, loads, kCarModelMembers);
}

bool TrafficLoadsMembersDestructible(uint8_t* base, uint32_t loads) {
  return SixModelsNodes(base, loads + kTrafficLoadsParts).has_value() &&
         MembersDestructible(base, loads, kTrafficMembers);
}

bool CockpitLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6010 CockpitLoads::CockpitLoads");
  return ConstructWithParts(base, loads, kCarModelMembers, kCockpitLoadsParts);
}

std::optional<uint32_t> CockpitLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6090 CockpitLoads::~CockpitLoads");
  return DestructWithParts(base, loads, kCarModelMembers, kCockpitLoadsParts);
}

bool TrafficLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D7C40 TrafficLoads::TrafficLoads");
  return ConstructWithParts(base, loads, kTrafficMembers, kTrafficLoadsParts);
}

std::optional<uint32_t> TrafficLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D7CC0 TrafficLoads::~TrafficLoads");
  return DestructWithParts(base, loads, kTrafficMembers, kTrafficLoadsParts);
}

// --- The wheels' loads ---

bool WheelsLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D8F60 WheelsLoads::WheelsLoads");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  const schema::WheelsLoads l(base, loads);
  for (uint32_t first : {l.tires_address(), l.rims_address()}) {
    for (uint32_t i = 0; i < 2; i++) {
      Require(models::PartModelConstruct(base, first + models::kPartModelBytes * i),
              "PartModel::PartModel");
    }
  }
  for (uint32_t i = 0; i < 2; i++) {
    models::XprConstruct(base, l.xprs_address() + 24 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Require(models::TextureProviderConstruct(base, l.providers_address() + 20 * i, -1)
                .has_value(),
            "TextureProvider::TextureProvider");
  }
  for (uint32_t i = 0; i < 2; i++) {
    Require(models::PartModelConstruct(base, l.calipers_address() + models::kPartModelBytes * i),
            "PartModel::PartModel");
  }
  return true;
}

namespace {

// The wheels' six part models, front then rear of each kind.
std::array<uint32_t, 6> WheelsModels(const schema::WheelsLoads& l) {
  return {l.tires_address(), l.tires_address() + 464, l.rims_address(),
          l.rims_address() + 464, l.calipers_address(), l.calipers_address() + 464};
}

// Model::Release and PartModel::Clear on a part model, the checks made.
void ReleaseModel(uint8_t* base, uint32_t model) {
  Require(models::ModelRelease(base, model).has_value(), "Model::Release");
  models::PartModelClear(base, model);
}

}  // namespace

std::optional<uint32_t> WheelsLoadsReset(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D8AA8 WheelsLoads::Reset");
  const schema::WheelsLoads l(base, loads);
  uint64_t nodes = 4;  // the XPRs'
  for (uint32_t model : WheelsModels(l)) {
    nodes += models::ModelReleaseNodes(base, model);
  }
  for (uint32_t i = 0; i < 2; i++) {
    if (schema::TextureProvider(base, l.providers_address() + 20 * i).vtable().clear_all() !=
        kTextureProviderClearAll) {
      return std::nullopt;
    }
  }
  if (!Room(base, nodes)) {
    return std::nullopt;
  }
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    car_models::LoadSlotReset(base, l.tire_slots_address() + 24 * i);
    ReleaseModel(base, l.tires_address() + models::kPartModelBytes * i);
    car_models::LoadSlotReset(base, l.rim_slots_address() + 24 * i);
    ReleaseModel(base, l.rims_address() + models::kPartModelBytes * i);
    car_models::LoadSlotReset(base, l.xpr_slots_address() + 24 * i);
    Require(models::XprRelease(base, l.xprs_address() + 24 * i).has_value(), "XPR::Release");
    scenery_lists::TextureProviderClearAll(base, l.providers_address() + 20 * i);
    car_models::LoadSlotReset(base, l.caliper_slots_address() + 24 * i);
    r3 = l.calipers_address() + models::kPartModelBytes * i;
    ReleaseModel(base, r3);
  }
  return r3;
}

bool WheelsLoadsMembersDestructible(uint8_t* base, uint32_t loads) {
  const schema::WheelsLoads l(base, loads);
  for (uint32_t model : WheelsModels(l)) {
    if (!models::ModelMembersDestructible(base, model)) {
      return false;
    }
  }
  return ProviderDestructible(base, l.providers_address()) &&
         ProviderDestructible(base, l.providers_address() + 20);
}

std::optional<uint32_t> WheelsLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D9010 WheelsLoads::~WheelsLoads");
  const schema::WheelsLoads l(base, loads);
  uint64_t nodes = 4;
  for (uint32_t model : WheelsModels(l)) {
    if (!models::ModelMembersDestructible(base, model)) {
      return std::nullopt;
    }
    nodes += models::ModelReleaseNodes(base, model);
  }
  if (!Room(base, nodes) || !ProviderDestructible(base, l.providers_address()) ||
      !ProviderDestructible(base, l.providers_address() + 20)) {
    return std::nullopt;
  }
  auto destroy = [&](uint32_t model) {
    const auto r3 = models::PartModelDestruct(base, model);
    Require(r3.has_value(), "PartModel::~PartModel");
    return *r3;
  };
  destroy(l.calipers_address() + 464);
  destroy(l.calipers_address());
  for (uint32_t i = 2; i-- > 0;) {
    Require(models::TextureProviderDestruct(base, l.providers_address() + 20 * i).has_value(),
            "TextureProvider::~TextureProvider");
  }
  for (uint32_t i = 2; i-- > 0;) {
    Require(models::XprDestruct(base, l.xprs_address() + 24 * i).has_value(), "XPR::~XPR");
  }
  destroy(l.rims_address() + 464);
  destroy(l.rims_address());
  destroy(l.tires_address() + 464);
  return destroy(l.tires_address());
}

bool CarModelLoadsResettable(uint8_t* base, uint32_t loads) {
  // The skeleton's node values (Skeleton's +176).
  const uint32_t values =
      schema::Skeleton(base, schema::CarModelLoads(base, loads).skeleton_address()).node_values();
  if (values != 0 && !memory::IsHeapBlock(values)) {
    return false;
  }
  return ProvidersClearAll(base, loads, kCarModelMembers) && models::PendingFreesHaveRoom(base, 4);
}

std::optional<uint32_t> CarModelLoadsReset(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3B58 CarModelLoads::Reset");
  if (!CarModelLoadsResettable(base, loads)) {
    return std::nullopt;
  }
  const schema::CarModelLoads l(base, loads);
  car_models::LoadSlotReset(base, l.node_file_slot_address());
  Require(model_animation::SkeletonFreeNodeValues(base, l.skeleton_address()),
          "Skeleton::FreeNodeValues");
  for (uint32_t i = 0; i < 2; i++) {
    car_models::LoadSlotReset(base, l.xpr_slots_address() + 24 * i);
    Require(models::XprRelease(base, l.xprs_address() + 24 * i).has_value(), "XPR::Release");
    scenery_lists::TextureProviderClearAll(base, l.providers_address() + 20 * i);
  }
  l.set_decals_state(0);
  // Each layer's and sticker's two 24-byte LoadSlots, 48 bytes a record.
  for (uint32_t i = 0; i < 3; i++) {
    car_models::LoadSlotReset(base, l.layer_slots_address() + 48 * i);
    car_models::LoadSlotReset(base, l.layer_slots_address() + 24 + 48 * i);
  }
  for (uint32_t i = 0; i < 6; i++) {
    car_models::LoadSlotReset(base, l.sticker_slots_address() + 48 * i);
    car_models::LoadSlotReset(base, l.sticker_slots_address() + 24 + 48 * i);
  }
  l.set_vinyl_sheet(0);
  l.set_vinyl_sheet_data(0);
  l.set_plate_first(0);
  l.set_plate_first_data(0);
  l.set_plate_second(0);
  l.set_plate_second_data(0);
  return l.sticker_slots_address() + 24 + 48 * 5;
}

}  // namespace scene_loaders
