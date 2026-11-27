// The scene's loading objects' resets (see objects.h).

#include "scene_loaders/objects.h"

#include "accessories/accessories.h"
#include "base/log.h"
#include "car_models/car_model.h"
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
#include "scene_loaders/loads.h"
#include "scene_loaders/pools.h"
#include "scene_models/scene_models.h"
#include "schema/car_models.h"
#include "schema/model_animation.h"
#include "schema/models.h"
#include "schema/scene_loaders.h"
#include "schema/stand_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <array>
#include <bit>
#include <cstdlib>
#include <initializer_list>
#include <unordered_set>

namespace scene_loaders {

namespace {

using G = schema::SceneLoadersGlobals;

constexpr uint32_t kPhysical = 0xA0000000u;

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene loaders: {} refused after its checks", what);
    std::abort();
  }
}

// The identity matrix (the four rows the original builds on its frame,
// copied by memcpy).
void Identity(uint32_t matrix) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      Store(matrix + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
}

// A position (0, 0, 0, 1) at `at`, in the original's store order (+8, +4,
// +0, then w at +12).
void Position(uint32_t at, uint32_t zero, uint32_t one) {
  Store(at + 8, zero);
  Store(at + 4, zero);
  Store(at + 0, zero);
  Store(at + 12, one);
}

constexpr uint32_t kParts = 216;

// The texture providers' vtable's slot 2 (models.md's TextureProviderVtable).
uint32_t ProvidersClearAllSlot() {
  return schema::TextureProviderVtable(models::kTextureProviderVtable).clear_all();
}

// Whether `count` part models' Model::Release runs natively, all of them
// (the pending frees take every block they put off).
bool ModelsReleasable(uint32_t first, uint32_t count) {
  uint64_t nodes = 0;
  for (uint32_t i = 0; i < count; i++) {
    nodes += models::ModelReleaseNodes(first + models::kPartModelBytes * i);
  }
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(uint32_t(nodes));
}

// Whether LoadSlot::ReleaseBuffer runs natively on `count` slots from
// `first`, each freeing pages no other frees.
bool SlotsReleasable(uint32_t first, uint32_t count) {
  std::unordered_set<uint32_t> pages;
  for (uint32_t i = 0; i < count; i++) {
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(first + kSlotBytes * i, &freed) ||
        (freed != 0 && !pages.insert(freed).second)) {
      return false;
    }
  }
  return true;
}

// The constructors' Model::Release on fresh models: no block to put off, but
// XPR::Release's check for two nodes.
bool FreshReleasable() { return models::PendingFreesHaveRoom(2); }

}  // namespace

bool CarModelResettable(uint32_t car_model) {
  if (car_model >= kPhysical || kPhysical - car_model < kCarModelBytes) {
    return false;
  }
  return CarModelLoadsResettable(car_model + kCarModelLoads);
}

std::optional<uint32_t> CarModelReset(uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D3E98 CarModel::Reset");
  if (!CarModelResettable(car_model)) {
    return std::nullopt;
  }
  const schema::CarModel cm(car_model);
  cm.set_first_set(G::first_set().address);
  scene_models::CarRecordReset(cm.record_address());
  Require(CarModelLoadsReset(car_model + kCarModelLoads).has_value(),
          "CarModelLoads::Reset");
  for (uint32_t i = 0; i < 4; i++) {
    Identity(cm.tire_matrices_address() + 64 * i);
  }
  // The original loads both constants after the tires' matrices.
  const uint32_t one = G::one();
  const uint32_t zero = G::zero();
  for (uint32_t i = 0; i < 4; i++) {
    Position(cm.tire_positions_address() + 16 * i, zero, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Identity(cm.seat_matrices_address() + 64 * i);
    Position(cm.seat_positions_address() + 16 * i, zero, one);
  }
  for (uint32_t i = 0; i < 6; i++) {
    Store(cm.xpr_indices_address() + 4 * i, 0xFFFFFFFFu);
  }
  cm.set_accessory_matrix(0u);
  Store(cm.accessory_position_address() + 0, zero);
  Store(cm.accessory_position_address() + 4, zero);
  Store(cm.accessory_position_address() + 8, zero);
  Store(cm.accessory_position_address() + 12, one);
  return cm.seat_matrices_address() + 64;
}

// --- The car parts ---

bool CarPartsLoadsConstruct(uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6F88 CarPartsLoads::CarPartsLoads");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots)) {
    return false;
  }
  const schema::CarPartsLoads l(loads);
  for (uint32_t i = 0; i < kParts; i++) {
    Require(models::PartModelConstruct(l.part_models_address() + models::kPartModelBytes * i),
            "PartModel::PartModel");
  }
  const uint32_t one = G::one();
  for (uint32_t i = 0; i < 30; i++) {
    Store(l.points_address() + 16 * i + 12, one);
  }
  return true;
}

bool CarPartsConstruct(uint32_t parts, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D7428 CarParts::CarParts");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots) ||
      !FreshReleasable()) {
    return false;
  }
  const schema::CarParts p(parts);
  PoolNodeConstruct(parts, list);
  p.set_index(0xFFFFFFFFu);
  p.set_vtable(kCarPartsVtable);
  p.set_state(0);
  Require(CarPartsLoadsConstruct(p.loads_address()), "CarPartsLoads::CarPartsLoads");
  p.set_car_model(0);
  for (uint32_t i = 0; i < kParts; i++) {
    const uint32_t model = p.part_models_address() + kPartModelBytes * i;
    car_models::LoadSlotReset(p.slots_address() + kSlotBytes * i);
    Require(models::ModelRelease(model).has_value(), "Model::Release");
    models::PartModelClear(model);
  }
  scene_models::PartsRecordReset(p.record_address());
  p.set_first_set(G::first_set().address);
  return true;
}

std::optional<uint32_t> CarPartsRelease(uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D6D00 CarParts::Release");
  const schema::CarParts p(parts);
  if (!SlotsReleasable(p.slots_address(), kParts) ||
      !ModelsReleasable(p.part_models_address(), kParts)) {
    return std::nullopt;
  }
  uint32_t model = 0;
  for (uint32_t i = 0; i < kParts; i++) {
    model = p.part_models_address() + kPartModelBytes * i;
    Require(car_models::LoadSlotReleaseBuffer(p.slots_address() + kSlotBytes * i)
                .has_value(),
            "LoadSlot::ReleaseBuffer");
    Require(models::ModelRelease(model).has_value(), "Model::Release");
    models::PartModelClear(model);
  }
  return model;
}

std::optional<uint32_t> CarPartsLoadsDestruct(uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6FE8 CarPartsLoads::~CarPartsLoads");
  const uint32_t first = schema::CarPartsLoads(loads).part_models_address();
  for (uint32_t i = 0; i < kParts; i++) {
    if (!models::ModelMembersDestructible(first + models::kPartModelBytes * i)) {
      return std::nullopt;
    }
  }
  if (!ModelsReleasable(first, kParts)) {
    return std::nullopt;
  }
  std::optional<uint32_t> r3;
  for (uint32_t i = kParts; i-- > 0;) {
    r3 = models::PartModelDestruct(first + models::kPartModelBytes * i);
    Require(r3.has_value(), "PartModel::~PartModel");
  }
  return r3;
}

// --- The cockpit body and the interior ---

namespace {

// Whether every slot in `slots` (their addresses) is released natively,
// each freeing pages no other frees.
bool AllReleasableAt(std::initializer_list<uint32_t> slots) {
  std::unordered_set<uint32_t> pages;
  for (uint32_t slot : slots) {
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(slot, &freed) ||
        (freed != 0 && !pages.insert(freed).second)) {
      return false;
    }
  }
  return true;
}

// The same, the slots' addresses in an array.
template <size_t N>
bool AllReleasable(const std::array<uint32_t, N>& slots) {
  std::unordered_set<uint32_t> pages;
  for (uint32_t slot : slots) {
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(slot, &freed) ||
        (freed != 0 && !pages.insert(freed).second)) {
      return false;
    }
  }
  return true;
}

// The cockpit's +3472 to +3504 (the accessory node's matrix pointer, its
// position (0, 0, 0, 1) copied from the original's frame as doublewords,
// +3504).
void CockpitNode(uint32_t cockpit, uint32_t zero, uint32_t one) {
  const schema::Cockpit c(cockpit);
  c.set_accessory_node(0);
  Store(c.accessory_point_address() + 0, zero);
  Store(c.accessory_point_address() + 4, zero);
  Store(c.accessory_point_address() + 8, zero);
  c.set_accessory_point_w(std::bit_cast<float>(one));
  c.set_texture(0);
}

// The six parts' slots and models in the cockpit (+536 + 24i, +688 + 464i)
// and the interior (+160 + 24i, +304 + 464i): each slot reset,
// Model::Release and PartModel::Clear on its model, the checks made.
void ResetParts(uint32_t slots, uint32_t models_at) {
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = models_at + kPartModelBytes * i;
    car_models::LoadSlotReset(slots + kSlotBytes * i);
    Require(models::ModelRelease(model).has_value(), "Model::Release");
    models::PartModelClear(model);
  }
}

// The cockpit's two XPR2s (+400 + 24i slots, +448 + 24i XPRs, +496 + 20i
// providers): with `release`, each slot's buffer released first; the slot
// reset, XPR::Release, the provider's slot 2 (ClearAll), the checks made.
void ResetXprs(uint32_t cockpit, bool release) {
  const schema::Cockpit c(cockpit);
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t slot = c.xpr_slots_address() + kSlotBytes * i;
    if (release) {
      Require(car_models::LoadSlotReleaseBuffer(slot).has_value(),
              "LoadSlot::ReleaseBuffer");
    }
    car_models::LoadSlotReset(slot);
    Require(models::XprRelease(c.xprs_address() + kSlotBytes * i).has_value(),
            "XPR::Release");
    scenery_lists::TextureProviderClearAll(c.providers_address() + 20 * i);
  }
}

}  // namespace

bool CockpitConstruct(uint32_t cockpit, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D63A0 Cockpit::Cockpit");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots) ||
      ProvidersClearAllSlot() != kTextureProviderClearAll ||
      !models::PendingFreesHaveRoom(2)) {
    return false;
  }
  const schema::Cockpit c(cockpit);
  PoolNodeConstruct(cockpit, list);
  c.set_index(0xFFFFFFFFu);
  c.set_vtable(kCockpitVtable);
  c.set_state(0);
  Require(CockpitLoadsConstruct(c.loads_address()), "CockpitLoads::CockpitLoads");
  const uint32_t one = G::one();
  c.set_accessory_point_w(std::bit_cast<float>(one));
  const uint32_t set = G::first_set().address;
  c.set_car_model(0);
  c.set_first_set(set);
  Require(model_animation::SkeletonFreeNodeValues(c.skeleton_address()),
          "Skeleton::FreeNodeValues");
  scene_models::CockpitRecordReset(c.record_address());
  CockpitNode(cockpit, G::zero(), one);
  car_models::LoadSlotReset(c.node_file_slot_address());
  ResetXprs(cockpit, false);
  ResetParts(c.part_slots_address(), c.part_models_address());
  return true;
}

namespace {

// Cockpit::Release's checks (nothing changed), with `extra` nodes more for
// what follows it.
bool CockpitReleasable(uint32_t cockpit, uint64_t extra) {
  const schema::Cockpit c(cockpit);
  const uint32_t parts = c.part_slots_address();
  // The skeleton's node values (Skeleton's +176).
  const uint32_t values = schema::Skeleton(c.skeleton_address()).node_values();
  return AllReleasableAt({c.node_file_slot_address(), c.xpr_slots_address(),
                                c.xpr_slots_address() + kSlotBytes, parts, parts + kSlotBytes,
                                parts + 2 * kSlotBytes, parts + 3 * kSlotBytes,
                                parts + 4 * kSlotBytes, parts + 5 * kSlotBytes}) &&
         (values == 0 || memory::IsHeapBlock(values)) &&
         ProvidersClearAll(c.loads_address(), kCarModelMembers) &&
         4 + extra <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(uint32_t(4 + extra));
}

}  // namespace

std::optional<uint32_t> CockpitRelease(uint32_t cockpit) {
  ITC_NATIVE_RUNNING("sub_820D6238 Cockpit::Release");
  if (!CockpitReleasable(cockpit, 0)) {
    return std::nullopt;
  }
  const schema::Cockpit c(cockpit);
  Require(car_models::LoadSlotReleaseBuffer(c.node_file_slot_address()).has_value(),
          "LoadSlot::ReleaseBuffer");
  car_models::LoadSlotReset(c.node_file_slot_address());
  Require(model_animation::SkeletonFreeNodeValues(c.skeleton_address()),
          "Skeleton::FreeNodeValues");
  ResetXprs(cockpit, true);
  c.set_first_set(0);
  c.set_car_model(0);
  scene_models::CockpitRecordReset(c.record_address());
  const uint32_t zero = G::zero();
  CockpitNode(cockpit, zero, G::one());
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t slot = c.part_slots_address() + kSlotBytes * i;
    Require(car_models::LoadSlotReleaseBuffer(slot).has_value(), "LoadSlot::ReleaseBuffer");
    car_models::LoadSlotReset(slot);
  }
  return c.part_slots_address() + kSlotBytes * 5;
}

bool InteriorConstruct(uint32_t interior, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D78B8 Interior::Interior");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots) ||
      !models::PendingFreesHaveRoom(2)) {
    return false;
  }
  const schema::Interior in(interior);
  PoolNodeConstruct(interior, list);
  in.set_state(0);
  in.set_vtable(kInteriorVtable);
  in.set_index(0xFFFFFFFFu);
  for (uint32_t i = 0; i < 6; i++) {
    Require(models::PartModelConstruct(in.part_models_address() + kPartModelBytes * i),
            "PartModel::PartModel");
  }
  in.set_car_model(0);
  ResetParts(in.part_slots_address(), in.part_models_address());
  scene_models::InteriorRecordReset(in.record_address());
  const uint32_t set = G::first_set().address;
  in.set_cockpit(0);
  in.set_first_set(set);
  return true;
}

std::optional<uint32_t> InteriorOnPartLoaded(uint32_t request, uint32_t interior,
                                             const PartLoadedCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820D7578 Interior::OnPartLoaded");
  const schema::Interior in(interior);
  for (uint32_t i = 0; i < 6; i++) {
    if (Load(in.part_slot_request_address() + kSlotBytes * i) != request) {
      continue;
    }
    const uint32_t slot = in.part_slots_address() + kSlotBytes * i;
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(slot, &freed)) {
      return std::nullopt;
    }
    const uint32_t model = in.part_models_address() + kPartModelBytes * i;
    const uint32_t file = schema::LoadSlot(slot).buffer();
    memory::PhysicalTag tag("model vertices: scene objects");
    if (!models::ModelLoad(model, file, 0, calls.model_load)) {
      calls.load(calls.user, model, file);
    }
    const auto r3 = car_models::LoadSlotReleaseBuffer(slot);
    Require(r3.has_value(), "LoadSlot::ReleaseBuffer");
    schema::LoadSlot(slot).set_state(3);
    return r3;
  }
  return request;
}

// --- The wheels and the traffic body ---

namespace {

// WheelsLoads::Reset's pending frees' nodes (the XPRs' four and the six
// models'), or nullopt where a provider's slot 2 isn't ClearAll; nothing
// changed.
std::optional<uint64_t> WheelsResetNodes(uint32_t wheels) {
  const schema::WheelsLoads l(wheels + kWheelsLoads);
  uint64_t nodes = 4;
  for (uint32_t model : {l.tires_address(), l.tires_address() + 464, l.rims_address(),
                         l.rims_address() + 464, l.calipers_address(),
                         l.calipers_address() + 464}) {
    nodes += models::ModelReleaseNodes(model);
  }
  for (uint32_t i = 0; i < 2; i++) {
    // The provider's vtable's slot 2 (models.md's TextureProviderVtable).
    if (schema::TextureProvider(l.providers_address() + 20 * i).vtable().clear_all() !=
        kTextureProviderClearAll) {
      return std::nullopt;
    }
  }
  return nodes;
}

bool Room(uint64_t nodes) {
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(uint32_t(nodes));
}

}  // namespace

std::optional<uint32_t> WheelsReset(uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8B58 Wheels::Reset");
  const uint32_t loads = wheels + kWheelsLoads;
  // WheelsLoads::Reset's checks, before anything changes (the record's
  // defaults don't touch what they read).
  const auto nodes = WheelsResetNodes(wheels);
  if (!nodes || !Room(*nodes)) {
    return std::nullopt;
  }
  const schema::CarWheels w(wheels);
  w.set_car_model(0u);
  w.set_first_set(G::first_set().address);
  scene_models::WheelsRecordReset(w.setup_address());
  const auto r3 = WheelsLoadsReset(loads);
  Require(r3.has_value(), "WheelsLoads::Reset");
  return r3;
}

namespace {

// TrafficCar::Reset's pending frees' nodes (the XPRs' four and the six
// models'), or nullopt where the node values aren't the layer's or a
// provider's slot 2 isn't ClearAll; nothing changed.
std::optional<uint64_t> TrafficResetNodes(uint32_t traffic) {
  const uint32_t loads = traffic + kTrafficLoads;
  const uint32_t values = schema::Skeleton(loads + kTrafficMembers.skeleton).node_values();
  const uint32_t part_models = schema::TrafficCar(traffic).part_models_address();
  uint64_t nodes = 4;
  for (uint32_t i = 0; i < 6; i++) {
    nodes += models::ModelReleaseNodes(part_models + kPartModelBytes * i);
  }
  if ((values != 0 && !memory::IsHeapBlock(values)) ||
      !ProvidersClearAll(loads, kTrafficMembers)) {
    return std::nullopt;
  }
  return nodes;
}

}  // namespace

std::optional<uint32_t> TrafficReset(uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7AB0 TrafficCar::Reset");
  const uint32_t loads = traffic + kTrafficLoads;
  const auto nodes = TrafficResetNodes(traffic);
  if (!nodes || !Room(*nodes)) {
    return std::nullopt;
  }
  const schema::TrafficCar t(traffic);
  t.set_first_set(G::first_set().address);
  // The car record (its +0 the car number).
  scene_models::CarRecordReset(t.car_number_address());
  t.set_xpr_record(0xFFFFFFFFu);
  car_models::LoadSlotReset(t.file_slot_address());
  Require(model_animation::SkeletonFreeNodeValues(loads + kTrafficMembers.skeleton),
          "Skeleton::FreeNodeValues");
  for (uint32_t i = 0; i < 2; i++) {
    // TrafficLoads' XPR2 slots (its +240 + 24i).
    car_models::LoadSlotReset(traffic + 3968 + kSlotBytes * i);
    Require(models::XprRelease(loads + kTrafficMembers.xprs[i]).has_value(),
            "XPR::Release");
    scenery_lists::TextureProviderClearAll(loads + kTrafficMembers.providers[i]);
  }
  t.set_texture_word0(0);
  t.set_texture_word1(0);
  t.set_texture_word2(0);
  t.set_texture_word3(0);
  t.set_texture2_word0(0);
  t.set_texture2_word1(0);
  t.set_texture2_word2(0);
  t.set_texture2_word3(0);
  ResetParts(t.slots_address(), t.part_models_address());
  return t.part_models_address() + kPartModelBytes * 5;
}

// --- Pool 4's object ---

std::optional<uint32_t> Pool4Reset(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6948 Pool4Object::Reset");
  const schema::Pool4Object o(object);
  const uint32_t model = o.model_address();
  if (!ModelsReleasable(model, 1)) {
    return std::nullopt;
  }
  o.set_car_model(0);
  scene_models::Pool4RecordReset(o.record_address());
  car_models::LoadSlotReset(o.slot_address());
  Require(models::ModelRelease(model).has_value(), "Model::Release");
  models::PartModelClear(model);
  o.set_first_set(G::first_set().address);
  return model;
}

// --- The lifetimes ---

namespace {

// The pool node's part of a constructor: PoolNode::PoolNode, then +16 = 0,
// the vtable, +20 = -1 (the originals' order).
void ConstructNode(uint32_t object, uint32_t list, uint32_t vtable) {
  const schema::PoolNode node(object);
  PoolNodeConstruct(object, list);
  node.set_state(0);
  node.set_vtable(vtable);
  node.set_index(0xFFFFFFFFu);
}

// The end of every destructor: the base vtable, PoolNode::~PoolNode.
void DestructNode(uint32_t object) {
  schema::PoolNode(object).set_vtable(kPoolObjectVtable);
  PoolNodeDestruct(object);
}

// A fresh part model's Model::Release (after its constructor): the XPR's two
// nodes, the blocks all 0.
constexpr uint64_t kFreshNodes = 2;

// The wheels' eight slots in Wheels::Release's order (car-models.md's
// CarWheels: each kind's front slot, then each kind's rear).
std::array<uint32_t, 8> WheelsSlots(uint32_t wheels) {
  const schema::CarWheels w(wheels);
  return {w.tire_slots_address(),       w.rim_slots_address(),
          w.xpr_slots_address(),        w.caliper_slots_address(),
          w.tire_slots_address() + 24,  w.rim_slots_address() + 24,
          w.xpr_slots_address() + 24,   w.caliper_slots_address() + 24};
}

}  // namespace

bool Pool4Construct(uint32_t object, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D6A70 Pool4Object::Pool4Object");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots) ||
      !Room(kFreshNodes)) {
    return false;
  }
  ConstructNode(object, list, kPool4Vtable);
  Require(models::PartModelConstruct(schema::Pool4Object(object).model_address()),
          "PartModel::PartModel");
  Require(Pool4Reset(object).has_value(), "Pool4Object::Reset");
  return true;
}

namespace {

// Pool4Object::Release's checks (nothing changed), with `extra` nodes more
// for what follows it: the slot's buffer, then room for Model::Release's
// nodes and Pool4Object::Reset's (a released model's two).
bool Pool4Releasable(uint32_t object, uint64_t extra) {
  const schema::Pool4Object o(object);
  uint32_t freed = 0;
  return car_models::LoadSlotReleasable(o.slot_address(), &freed) &&
         Room(models::ModelReleaseNodes(o.model_address()) + kFreshNodes + extra);
}

// Pool4Object::Release's work, its checks made.
uint32_t Pool4ReleaseWork(uint32_t object) {
  const schema::Pool4Object o(object);
  Require(car_models::LoadSlotReleaseBuffer(o.slot_address()).has_value(),
          "LoadSlot::ReleaseBuffer");
  Require(models::ModelRelease(o.model_address()).has_value(), "Model::Release");
  const auto r3 = Pool4Reset(object);
  Require(r3.has_value(), "Pool4Object::Reset");
  return *r3;
}

}  // namespace

std::optional<uint32_t> Pool4Release(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6A20 Pool4Object::Release");
  if (!Pool4Releasable(object, 0)) {
    return std::nullopt;
  }
  return Pool4ReleaseWork(object);
}

bool Pool4Destruct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6AD0 Pool4Object::~Pool4Object");
  // The model's members aren't touched by its release; ~Model releases a
  // released model again (two nodes).
  const schema::Pool4Object o(object);
  if (!models::ModelMembersDestructible(o.model_address()) ||
      !Pool4Releasable(object, kFreshNodes)) {
    return false;
  }
  o.set_vtable(kPool4Vtable);
  Pool4ReleaseWork(object);
  Require(models::PartModelDestruct(o.model_address()).has_value(),
          "PartModel::~PartModel");
  DestructNode(object);
  return true;
}

bool Pool4DeletingDestruct(uint32_t object, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D6BC0 Pool4Object::DeletingDestructor");
  return (flags & 1) == 0 && Pool4Destruct(object);
}

bool WheelsConstruct(uint32_t wheels, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D9570 Wheels::Wheels");
  // WheelsLoads::WheelsLoads' check, and Wheels::Reset's on the fresh loads
  // (the providers' vtable's slot 2, the XPRs' four nodes and the six fresh
  // models').
  if (!models::CanAllocatePointers(models::kTextureProviderSlots) ||
      ProvidersClearAllSlot() != kTextureProviderClearAll ||
      !Room(4 + 6 * kFreshNodes)) {
    return false;
  }
  ConstructNode(wheels, list, kWheelsVtable);
  const uint32_t setup = schema::CarWheels(wheels).setup_address();
  scene_models::Record12Reset(setup + 44);
  scene_models::Record12Reset(setup + 56);
  Require(WheelsLoadsConstruct(wheels + kWheelsLoads), "WheelsLoads::WheelsLoads");
  Require(WheelsReset(wheels).has_value(), "Wheels::Reset");
  return true;
}

namespace {

// Wheels::Release's checks (nothing changed), with `extra` nodes more for
// what follows it.
bool WheelsReleasable(uint32_t wheels, uint64_t extra) {
  const auto nodes = WheelsResetNodes(wheels);
  return AllReleasable(WheelsSlots(wheels)) && nodes && Room(*nodes + extra);
}

uint32_t WheelsReleaseWork(uint32_t wheels) {
  for (uint32_t slot : WheelsSlots(wheels)) {
    Require(car_models::LoadSlotReleaseBuffer(slot).has_value(),
            "LoadSlot::ReleaseBuffer");
  }
  const auto r3 = WheelsReset(wheels);
  Require(r3.has_value(), "Wheels::Reset");
  return *r3;
}

}  // namespace

std::optional<uint32_t> WheelsRelease(uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8E68 Wheels::Release");
  if (!WheelsReleasable(wheels, 0)) {
    return std::nullopt;
  }
  return WheelsReleaseWork(wheels);
}

bool WheelsDestruct(uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D90B8 Wheels::~Wheels");
  // ~WheelsLoads after the reset: the XPRs' four nodes and the six released
  // models' two each; the members aren't touched by the release.
  if (!WheelsLoadsMembersDestructible(wheels + kWheelsLoads) ||
      !WheelsReleasable(wheels, 4 + 6 * kFreshNodes)) {
    return false;
  }
  schema::CarWheels(wheels).set_vtable(kWheelsVtable);
  WheelsReleaseWork(wheels);
  Require(WheelsLoadsDestruct(wheels + kWheelsLoads).has_value(),
          "WheelsLoads::~WheelsLoads");
  DestructNode(wheels);
  return true;
}

bool WheelsDeletingDestruct(uint32_t wheels, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D95E8 Wheels::DeletingDestructor");
  return (flags & 1) == 0 && WheelsDestruct(wheels);
}

// --- The car model, the car parts and the cockpit's destructors ---

namespace {

// The w (+12) of each of a matrix's four rows.
void MatrixW(uint32_t matrix, uint32_t one) {
  for (uint32_t row = 0; row < 4; row++) {
    Store(matrix + 16 * row + 12, one);
  }
}

}  // namespace

bool CarModelConstruct(uint32_t car_model, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D5890 CarModel::CarModel");
  // CarModelLoads::CarModelLoads' check, and CarModel::Reset's on the fresh
  // loads (no node values, the providers' vtable's slot 2, the XPRs' four
  // nodes).
  if (car_model >= kPhysical || kPhysical - car_model < kCarModelBytes ||
      !models::CanAllocatePointers(models::kTextureProviderSlots) ||
      ProvidersClearAllSlot() != kTextureProviderClearAll ||
      !models::PendingFreesHaveRoom(4)) {
    return false;
  }
  ConstructNode(car_model, list, kCarModelVtable);
  const schema::CarModel cm(car_model);
  scene_models::CarRecordSetW(cm.record_address());
  Require(CarModelLoadsConstruct(car_model + kCarModelLoads),
          "CarModelLoads::CarModelLoads");
  const uint32_t one = G::one();
  for (uint32_t i = 0; i < 4; i++) {
    MatrixW(cm.tire_matrices_address() + 64 * i, one);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Store(cm.tire_positions_address() + 16 * i + 12, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    MatrixW(cm.seat_matrices_address() + 64 * i, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Store(cm.seat_positions_address() + 16 * i + 12, one);
  }
  Store(cm.accessory_position_address() + 12, one);
  Require(CarModelReset(car_model).has_value(), "CarModel::Reset");
  return true;
}

bool CarModelDestruct(uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4750 CarModel::~CarModel");
  // The release's checks hold for ~CarModelLoads after it (the node values
  // it checks are freed by the release); the XPRs check for four nodes in
  // each.
  if (!car_models::CarModelReleasable(car_model) ||
      !CarModelLoadsDestructible(car_model + kCarModelLoads) ||
      !models::PendingFreesHaveRoom(8)) {
    return false;
  }
  schema::CarModel(car_model).set_vtable(kCarModelVtable);
  Require(car_models::CarModelRelease(car_model).has_value(), "CarModel::Release");
  Require(CarModelLoadsDestruct(car_model + kCarModelLoads), "CarModelLoads::~CarModelLoads");
  DestructNode(car_model);
  return true;
}

bool CarModelDeletingDestruct(uint32_t car_model, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D5988 CarModel::DeletingDestructor");
  return (flags & 1) == 0 && CarModelDestruct(car_model);
}

bool CarPartsDestruct(uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D7038 CarParts::~CarParts");
  // The release's checks, then ~CarPartsLoads' on the released models (their
  // members untouched, two nodes each).
  const schema::CarParts p(parts);
  const uint32_t first = p.part_models_address();
  uint64_t nodes = 0;
  for (uint32_t i = 0; i < kParts; i++) {
    const uint32_t model = first + kPartModelBytes * i;
    if (!models::ModelMembersDestructible(model)) {
      return false;
    }
    nodes += models::ModelReleaseNodes(model);
  }
  if (!SlotsReleasable(p.slots_address(), kParts) ||
      !Room(nodes + kFreshNodes * kParts)) {
    return false;
  }
  p.set_vtable(kCarPartsVtable);
  Require(CarPartsRelease(parts).has_value(), "CarParts::Release");
  Require(CarPartsLoadsDestruct(p.loads_address()).has_value(),
          "CarPartsLoads::~CarPartsLoads");
  DestructNode(parts);
  return true;
}

bool CarPartsDeletingDestruct(uint32_t parts, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D74C0 CarParts::DeletingDestructor");
  return (flags & 1) == 0 && CarPartsDestruct(parts);
}

bool CockpitDestruct(uint32_t cockpit) {
  ITC_NATIVE_RUNNING("sub_820D64D0 Cockpit::~Cockpit");
  // The release doesn't release the six part models: ~CockpitLoads' nodes
  // are theirs now, and the XPRs' four.
  const schema::Cockpit c(cockpit);
  const uint32_t loads = c.loads_address();
  uint64_t nodes = 4;
  for (uint32_t i = 0; i < 6; i++) {
    nodes += models::ModelReleaseNodes(c.part_models_address() + kPartModelBytes * i);
  }
  if (!CockpitLoadsMembersDestructible(loads) || !CockpitReleasable(cockpit, nodes)) {
    return false;
  }
  c.set_vtable(kCockpitVtable);
  Require(CockpitRelease(cockpit).has_value(), "Cockpit::Release");
  Require(CockpitLoadsDestruct(loads).has_value(), "CockpitLoads::~CockpitLoads");
  DestructNode(cockpit);
  return true;
}

bool CockpitDeletingDestruct(uint32_t cockpit, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D6798 Cockpit::DeletingDestructor");
  return (flags & 1) == 0 && CockpitDestruct(cockpit);
}

// --- The traffic body's lifetime ---

bool TrafficConstruct(uint32_t traffic, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D8908 TrafficCar::TrafficCar");
  // TrafficLoads::TrafficLoads' check, and TrafficCar::Reset's on the fresh
  // loads (no node values, the providers' vtable's slot 2, the XPRs' four
  // nodes and the six fresh models').
  if (!models::CanAllocatePointers(models::kTextureProviderSlots) ||
      ProvidersClearAllSlot() != kTextureProviderClearAll ||
      !Room(4 + 6 * kFreshNodes)) {
    return false;
  }
  ConstructNode(traffic, list, kTrafficVtable);
  const schema::TrafficCar t(traffic);
  scene_models::CarRecordSetW(t.car_number_address());
  Require(TrafficLoadsConstruct(traffic + kTrafficLoads), "TrafficLoads::TrafficLoads");
  const uint32_t one = G::one();
  // The tires' positions' w (+7072 + 16i + 12), then their matrices' rows'.
  for (uint32_t i = 0; i < 4; i++) {
    Store(t.tire_position0_w_address() + 16 * i, one);
  }
  for (uint32_t i = 0; i < 4; i++) {
    for (uint32_t row = 0; row < 4; row++) {
      Store(t.tire_matrix0_row0_w_address() + 64 * i + 16 * row, one);
    }
  }
  Require(TrafficReset(traffic).has_value(), "TrafficCar::Reset");
  return true;
}

namespace {

// TrafficCar::Release's checks (nothing changed), with `extra` nodes more
// for what follows it.
bool TrafficReleasable(uint32_t traffic, uint64_t extra) {
  uint32_t freed = 0;
  const auto nodes = TrafficResetNodes(traffic);
  return car_models::LoadSlotReleasable(schema::TrafficCar(traffic).file_slot_address(),
                                       &freed) &&
         nodes &&
         Room(*nodes + extra);
}

uint32_t TrafficReleaseWork(uint32_t traffic) {
  const schema::TrafficCar t(traffic);
  {
    const uint32_t d = t.texture_word3();
    const uint32_t c = t.texture_word2();
    const uint32_t b = t.texture_word1();
    const uint32_t a = t.texture_word0();
    car_models::DeferRelease(t.first_set().address, a, b, c, d);
  }
  {
    const uint32_t d = t.texture2_word3();
    const uint32_t c = t.texture2_word2();
    const uint32_t b = t.texture2_word1();
    const uint32_t a = t.texture2_word0();
    car_models::DeferRelease(t.first_set().address, a, b, c, d);
  }
  Require(car_models::LoadSlotReleaseBuffer(t.file_slot_address()).has_value(),
          "LoadSlot::ReleaseBuffer");
  const auto r3 = TrafficReset(traffic);
  Require(r3.has_value(), "TrafficCar::Reset");
  return *r3;
}

}  // namespace

std::optional<uint32_t> TrafficRelease(uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7B90 TrafficCar::Release");
  if (!TrafficReleasable(traffic, 0)) {
    return std::nullopt;
  }
  return TrafficReleaseWork(traffic);
}

bool TrafficDestruct(uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7E90 TrafficCar::~TrafficCar");
  // ~TrafficLoads after the reset: the six released models' two nodes each
  // and the XPRs' four; the members aren't touched by the release (the node
  // values are freed by it).
  if (!TrafficLoadsMembersDestructible(traffic + kTrafficLoads) ||
      !TrafficReleasable(traffic, 6 * kFreshNodes + 4)) {
    return false;
  }
  schema::TrafficCar(traffic).set_vtable(kTrafficVtable);
  TrafficReleaseWork(traffic);
  Require(TrafficLoadsDestruct(traffic + kTrafficLoads).has_value(),
          "TrafficLoads::~TrafficLoads");
  DestructNode(traffic);
  return true;
}

bool TrafficDeletingDestruct(uint32_t traffic, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D89B8 TrafficCar::DeletingDestructor");
  return (flags & 1) == 0 && TrafficDestruct(traffic);
}

// --- The interior's release and destructors, ScenePools::ReleaseA ---

namespace {

// Interior::Release's checks (nothing changed), with `extra` nodes more for
// what follows it: the six slots' buffers, the pending frees' room for the
// models' release, and the accessories' release by the model set (set a's
// three as ReleaseAll's check, the entries distinct).
bool InteriorReleasable(uint32_t interior, uint64_t extra) {
  const schema::Interior in(interior);
  uint64_t nodes = extra;
  for (uint32_t i = 0; i < 6; i++) {
    nodes += models::ModelReleaseNodes(in.part_models_address() + kPartModelBytes * i);
  }
  if (!SlotsReleasable(in.part_slots_address(), 6) || !Room(nodes)) {
    return false;
  }
  const uint32_t set = in.first_set().model_set();
  const uint32_t accessories = scene_models::Accessories();
  if (set == 0) {
    return accessories::SetReleasable(accessories);
  }
  if (set == 1) {
    return accessories::SetReleaseNative(accessories, int32_t(in.accessory()));
  }
  return true;
}

// Interior::Release's work, its checks made.
uint32_t InteriorReleaseWork(uint32_t interior) {
  const schema::Interior in(interior);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = in.part_models_address() + kPartModelBytes * i;
    Require(car_models::LoadSlotReleaseBuffer(in.part_slots_address() + kSlotBytes * i)
                .has_value(),
            "LoadSlot::ReleaseBuffer");
    Require(models::ModelRelease(model).has_value(), "Model::Release");
    models::PartModelClear(model);
    r3 = model;
  }
  const uint32_t set = in.first_set().model_set();
  if (set == 0) {
    for (int32_t i = 0; i < 3; i++) {
      const uint32_t accessories = scene_models::Accessories();
      r3 = accessories::SetRelease(accessories, i).value_or(accessories);
    }
  } else if (set == 1) {
    const int32_t index = int32_t(in.accessory());
    const uint32_t accessories = scene_models::Accessories();
    r3 = accessories::SetRelease(accessories, index).value_or(accessories);
  }
  in.set_cockpit(0);
  return r3;
}

}  // namespace

std::optional<uint32_t> InteriorRelease(uint32_t interior) {
  ITC_NATIVE_RUNNING("sub_820D75F8 Interior::Release");
  if (!InteriorReleasable(interior, 0)) {
    return std::nullopt;
  }
  return InteriorReleaseWork(interior);
}

bool InteriorDestruct(uint32_t interior) {
  ITC_NATIVE_RUNNING("sub_820D76D8 Interior::~Interior");
  // The release's checks, then ~PartModel's on the released models (their
  // members untouched, two nodes each).
  const schema::Interior in(interior);
  for (uint32_t i = 0; i < 6; i++) {
    if (!models::ModelMembersDestructible(in.part_models_address() + kPartModelBytes * i)) {
      return false;
    }
  }
  if (!InteriorReleasable(interior, 6 * kFreshNodes)) {
    return false;
  }
  in.set_vtable(kInteriorVtable);
  InteriorReleaseWork(interior);
  for (uint32_t i = 6; i-- > 0;) {
    Require(models::PartModelDestruct(in.part_models_address() + kPartModelBytes * i)
                .has_value(),
            "PartModel::~PartModel");
  }
  DestructNode(interior);
  return true;
}

bool InteriorDeletingDestruct(uint32_t interior, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D7960 Interior::DeletingDestructor");
  return (flags & 1) == 0 && InteriorDestruct(interior);
}

std::optional<uint32_t> ReleaseA(uint32_t set, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C4488 ScenePools::ReleaseA");
  const uint32_t object =
      vehicle_accessors::ListAt(schema::ScenePools(set).pools_address() + pool * 12,
                                index);
  std::optional<uint32_t> r3 = object;
  switch (pool) {
    case 0:
      r3 = car_models::CarModelRelease(object);
      break;
    case 1:
      r3 = CarPartsRelease(object);
      break;
    case 2:
      r3 = CockpitRelease(object);
      break;
    case 3:
      r3 = InteriorRelease(object);
      break;
    case 4:
      r3 = Pool4Release(object);
      break;
    case 5:
      r3 = WheelsRelease(object);
      break;
    case 6:
      r3 = TrafficRelease(object);
      break;
    default:
      break;
  }
  if (!r3) {
    return std::nullopt;
  }
  schema::PoolNode(object).set_state(0);
  return r3;
}

}  // namespace scene_loaders
