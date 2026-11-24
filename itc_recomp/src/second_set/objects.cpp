// The second set's objects' lifetimes (see objects.h), store for store as
// the recompiled functions.

#include "second_set/objects.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "memory/host_copy.h"
#include "native/buffers.h"
#include "scene/scene.h"
#include "scene_loaders/objects.h"
#include "scene_loaders/pools.h"
#include "scene_models/scene_models.h"
#include "schema/second_set.h"
#include "schema/car_draw.h"
#include "schema/load_poll.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "second_set/blocks.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cstdlib>

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;
using car_physics::ops::kZero;

using G = schema::SecondSetGlobals;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// memset (sub_825A71D0) with 0: the same bytes (a host object's through
// its handle).
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  if (memory::FillWithHandles(base, address, 0, bytes)) {
    return;
  }
  for (uint32_t i = 0; i < bytes; i++) {
    base[address + i] = 0;
  }
}

// The identity (vupkd3d128's rows, stored aligned) at `at`.
void Identity(uint8_t* base, uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
}

// [0x826C2E44] (the car scene) and [0x826C2E40] (the first set).
uint32_t Scene(uint8_t* base) { return G::scene<schema::View>(base).address; }
uint32_t FirstSet(uint8_t* base) { return G::first_set<schema::View>(base).address; }

void Memcpy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, caller)) {
    ITC_LOG_ERROR("Second set: an object's memcpy refused after its checks");
    std::abort();
  }
}

}  // namespace

std::optional<uint32_t> SceneCarModelRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB6C0 SceneCarModel::Release");
  (void)stack;
  const schema::SceneCarModel o(base, object);
  const schema::SecondSetFirstSetObject car_model = o.car_model();
  if (car_model) {
    if (!scene_loaders::ReleaseA(base, FirstSet(base), 0, car_model.index())) {
      return std::nullopt;
    }
  }
  const uint32_t r3 = scene_models::CarRecordReset(base, o.car_record_address());
  o.set_car_model(0u);
  o.set_scene(0u);
  return r3;
}

uint32_t ScenePartsReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0510 SceneParts::Reset");
  const uint32_t sp = stack - 96;
  const schema::SceneParts p(base, object);
  scene_models::PartsRecordReset(base, p.record_address());
  PartsPointsReset(base, p.points_address(), sp);
  p.set_scene_car_model(0u);
  p.set_scene(0u);
  p.set_skeleton(0);
  p.set_first_set_parts(0);
  p.set_unknown_37280(0);
  p.set_loaded(0);
  p.set_scene(Scene(base));
  Clear(base, p.part_model_0_address(), 68);
  return p.part_model_0_address();
}

uint32_t SceneCockpitReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBE20 SceneCockpit::Reset");
  const uint32_t sp = stack - 112;
  const schema::SceneCockpit c(base, object);
  c.set_car_model(0);
  c.set_interior(0u);
  c.set_body(0);
  c.set_finished(0);
  c.set_scene(Scene(base));
  scene_models::CockpitRecordReset(base, c.record_address());
  const uint32_t r3 = CockpitPointsReset(base, c.points_address(), sp);
  c.set_record_address(c.record_address());
  c.set_part_model_0(0);
  c.set_part_model_1(0);
  c.set_part_model_2(0);
  c.set_part_model_3(0);
  c.set_part_model_4(0);
  c.set_part_model_5(0);
  return r3;
}

uint32_t SceneInteriorReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3748 SceneInterior::Reset");
  const uint32_t sp = stack - 112;
  const schema::SceneInterior o(base, object);
  o.set_car_model(0u);
  o.set_skeleton(0);
  o.set_texture_set(0);
  o.set_first_set_interior(0u);
  o.set_cockpit(0);
  o.set_finished(0);
  o.set_cockpit_texture(0);
  scene_models::InteriorRecordReset(base, o.record_address());
  const uint32_t r3 = InteriorPointsReset(base, o.points_address(), sp);
  o.set_scene(Scene(base));
  o.set_part_model_0(0);
  o.set_part_model_1(0);
  o.set_part_model_2(0);
  o.set_part_model_3(0);
  o.set_part_model_4(0);
  o.set_part_model_5(0);
  return r3;
}

uint32_t SceneLightsReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DCA40 SceneLights::Reset");
  const uint32_t sp = stack - 112;
  const schema::SceneLightKit k(base, object);
  k.set_car_model(0u);
  k.set_first_set_object(0u);
  k.set_model(0);
  k.set_finished(0);
  scene_models::Pool4RecordReset(base, k.record_address());
  const uint32_t block = k.points_address();
  const uint32_t r3 = LightPointsReset(base, block, sp);
  k.set_scene(Scene(base));
  // The block's record, then its three groups' (+48, +4384, +8720).
  for (const uint32_t at : {0u, 48u, 4384u, 8720u}) {
    schema::LightPoints(base, block + at).set_record(k.record_address());
  }
  return r3;
}

uint32_t SceneTrafficReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5930 SceneTraffic::Reset");
  const uint32_t sp = stack - 96;
  const schema::SceneTraffic t(base, object);
  scene_models::CarRecordReset(base, t.car_record_address());
  const uint32_t r3 = TrafficPointsReset(base, t.points_address(), sp);
  t.set_skeleton(0);
  t.set_body(0);
  t.set_finished(0);
  t.set_part_model_0(0);
  t.set_part_model_1(0);
  t.set_part_model_2(0);
  t.set_part_model_3(0);
  t.set_part_model_4(0);
  t.set_part_model_5(0);
  return r3;
}

uint32_t SceneWheelsReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E88A8 SceneWheels::Reset");
  const uint32_t sp = stack - 96;
  const schema::SceneWheels w(base, object);
  w.set_car_model(0);
  w.set_first_set_wheels(0);
  w.set_textures_bound(0);
  w.set_points_set_up(0);
  w.set_traffic_record(0);
  w.set_tire_positions(0);
  w.set_scene(Scene(base));
  scene_models::WheelsRecordReset(base, w.record_address());
  return WheelPointsReset(base, w.points_address(), sp);
}

namespace {

// A release: ReleaseA on the first set's object in the word at `first` (its
// address), if any, then `reset`.
std::optional<uint32_t> Release(uint8_t* base, uint32_t object, uint32_t first, uint32_t pool,
                                uint32_t stack,
                                uint32_t (*reset)(uint8_t*, uint32_t, uint32_t)) {
  const uint32_t sp = stack - 96;
  const schema::SecondSetFirstSetObject held(base, Load(base, first));
  if (held) {
    if (!scene_loaders::ReleaseA(base, FirstSet(base), pool, held.index())) {
      return std::nullopt;
    }
  }
  return reset(base, object, sp);
}

// The constructors' start: PoolNode::PoolNode, +16 = 0, the vtable, +20 = -1.
void Node(uint8_t* base, uint32_t object, uint32_t list, uint32_t vtable) {
  const schema::PoolNode n(base, object);
  scene_loaders::PoolNodeConstruct(base, object, list);
  n.set_state(0);
  n.set_vtable(vtable);
  n.set_index(0xFFFFFFFFu);
}

}  // namespace

std::optional<uint32_t> ScenePartsRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0590 SceneParts::Release");
  return Release(base, object, schema::SceneParts(base, object).first_set_parts_address(), 1,
                 stack, ScenePartsReset);
}

std::optional<uint32_t> SceneInteriorRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3890 SceneInterior::Release");
  return Release(base, object, schema::SceneInterior(base, object).first_set_interior_address(), 3,
                 stack, SceneInteriorReset);
}

std::optional<uint32_t> SceneLightsRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DCB20 SceneLights::Release");
  return Release(base, object, schema::SceneLightKit(base, object).first_set_object_address(), 4,
                 stack, SceneLightsReset);
}

std::optional<uint32_t> SceneWheelsRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E89B0 SceneWheels::Release");
  return Release(base, object, schema::SceneWheels(base, object).first_set_wheels_address(), 5,
                 stack, SceneWheelsReset);
}

std::optional<uint32_t> SceneTrafficRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5A18 SceneTraffic::Release");
  return Release(base, object, schema::SceneTraffic(base, object).body_address(), 6, stack,
                 SceneTrafficReset);
}

uint64_t ReleaseB(uint8_t* base, uint32_t set, uint32_t pool, uint32_t index, uint32_t stack,
                  uint32_t lr, const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820C5C88 ScenePools::ReleaseB");
  if (pool <= 7 && !calls.native(pool)) {
    return calls.release_b(set, pool, index, stack, lr);
  }
  const uint32_t sp = stack - 112;
  // The scene's second set lists, 12 bytes a pool.
  const uint32_t list = schema::CarScene(base, set).car_models_first_address() + 12 * pool;
  const uint32_t object = vehicle_accessors::ListAt(base, list, index);
  if (schema::PoolNode(base, object).state() != 3) {
    return object;
  }
  std::optional<uint32_t> r3 = object;
  switch (pool) {
    case 0:
      r3 = SceneCarModelRelease(base, object, sp);
      break;
    case 1:
      r3 = ScenePartsRelease(base, object, sp);
      break;
    case 2:
      r3 = SceneCockpitRelease(base, object, sp, calls);
      break;
    case 3:
      r3 = SceneInteriorRelease(base, object, sp);
      break;
    case 4:
      r3 = SceneLightsRelease(base, object, sp);
      break;
    case 5:
      r3 = SceneWheelsRelease(base, object, sp);
      break;
    case 6:
      r3 = SceneTrafficRelease(base, object, sp);
      break;
    case 7:
      r3 = SceneModelRelease(base, object, sp, calls);
      break;
    default:
      break;
  }
  if (!r3) {
    return calls.release_b(set, pool, index, stack, lr);
  }
  schema::PoolNode(base, object).set_state(0);
  return *r3;
}

std::optional<uint32_t> SceneCockpitRelease(uint8_t* base, uint32_t object, uint32_t stack,
                                            const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DBF88 SceneCockpit::Release");
  const uint32_t sp = stack - 96;
  const schema::SceneCockpit c(base, object);
  const schema::SecondSetFirstSetObject body(base, c.body());
  if (body) {
    if (!scene_loaders::ReleaseA(base, FirstSet(base), 2, body.index())) {
      return std::nullopt;
    }
  }
  const schema::SceneInterior interior = c.interior();
  if (interior) {
    ReleaseB(base, c.scene<schema::View>().address, 3, interior.index(), sp, 0x820DBFD8, calls);
  }
  return SceneCockpitReset(base, object, sp);
}

uint32_t SceneModelRelease(uint8_t* base, uint32_t object, uint32_t stack,
                           const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DA5D0 SceneModel::Release");
  const uint32_t sp = stack - 96;
  const schema::SceneModel m(base, object);
  if (m.build_delay() == 0) {
    // The parts in the original's order: the word, the pool, the return
    // address after its call.
    struct Part {
      uint32_t (schema::SceneModel::*at)() const;
      uint32_t pool, lr;
    };
    static constexpr Part kParts[] = {
        {&schema::SceneModel::car_model_address, 0, 0x820DA618},
        {&schema::SceneModel::parts_address, 1, 0x820DA634},
        {&schema::SceneModel::interior_address, 3, 0x820DA650},
        {&schema::SceneModel::light_kit_address, 4, 0x820DA66C},
        {&schema::SceneModel::wheels_address, 5, 0x820DA688},
        {&schema::SceneModel::cockpit_address, 2, 0x820DA6A4},
        {&schema::SceneModel::traffic_body_address, 6, 0x820DA6C0}};
    for (const Part& part : kParts) {
      // Traffic (kind 3) has no car model to release.
      if (part.pool == 0 && m.kind() == 3) {
        continue;
      }
      const schema::SecondSetFirstSetObject held(base, Load(base, (m.*part.at)()));
      if (held) {
        ReleaseB(base, m.scene<schema::View>().address, part.pool, held.index(), sp, part.lr,
                 calls);
      }
    }
  }
  scene_models::SceneModelRecordReset(base, m.record_address());
  const uint32_t r3 = SceneModelBlockReset(base, m.draw_block_address(), sp);
  m.set_set_up(0);
  m.set_release_pending(0);
  m.set_second_wheels_claimed(0);
  m.set_ready(0);
  m.set_build_pending(0);
  m.set_car_model(0u);
  m.set_car_model_ready(0);
  m.set_decals_reloaded(0);
  m.set_parts(0u);
  m.set_parts_ready(0);
  m.set_interior(0);
  m.set_interior_ready(0);
  m.set_light_kit(0u);
  m.set_light_kit_ready(0);
  m.set_cockpit(0);
  m.set_cockpit_ready(0);
  m.set_traffic_body(0);
  m.set_traffic_body_ready(0);
  m.set_wheels(0);
  m.set_wheels_ready(0);
  m.set_second_wheels(0);
  m.set_second_wheels_ready(0);
  m.set_build_delay(3);
  m.set_scene(Scene(base));
  return r3;
}

uint32_t SceneCarModelConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB770 SceneCarModel::SceneCarModel");
  (void)stack;
  const schema::SceneCarModel o(base, object);
  scene_loaders::PoolNodeConstruct(base, object, list);
  o.set_index(0xFFFFFFFFu);
  o.set_vtable(schema::SceneCarModel::kVtable);
  o.set_state(0);
  scene_models::CarRecordSetW(base, o.car_record_address());
  scene_models::CarRecordReset(base, o.car_record_address());
  o.set_car_model(0u);
  o.set_scene(0u);
  return object;
}

uint32_t ScenePartsConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2F18 SceneParts::SceneParts");
  const uint32_t sp = stack - 96;
  Node(base, object, list, schema::SceneParts::kVtable);
  PartsPointsConstruct(base, schema::SceneParts(base, object).points_address(), sp);
  ScenePartsReset(base, object, sp);
  return object;
}

uint32_t SceneCockpitConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC3E8 SceneCockpit::SceneCockpit");
  const uint32_t sp = stack - 96;
  Node(base, object, list, schema::SceneCockpit::kVtable);
  CockpitPointsConstruct(base, schema::SceneCockpit(base, object).points_address(), sp);
  SceneCockpitReset(base, object, sp);
  return object;
}

uint32_t SceneInteriorConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5340 SceneInterior::SceneInterior");
  const uint32_t sp = stack - 96;
  Node(base, object, list, schema::SceneInterior::kVtable);
  InteriorPointsConstruct(base, schema::SceneInterior(base, object).points_address(), sp);
  SceneInteriorReset(base, object, sp);
  return object;
}

uint32_t SceneLightsConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE030 SceneLights::SceneLights");
  const uint32_t sp = stack - 96;
  Node(base, object, list, schema::SceneLightKit::kVtable);
  LightPointsConstruct(base, schema::SceneLightKit(base, object).points_address(), sp);
  SceneLightsReset(base, object, sp);
  return object;
}

uint32_t SceneWheelsConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECE40 SceneWheels::SceneWheels");
  const uint32_t sp = stack - 112;
  const schema::SceneWheels w(base, object);
  Node(base, object, list, schema::SceneWheels::kVtable);
  scene_models::Record12Reset(base, w.front_size_index_address());
  scene_models::Record12Reset(base, w.rear_size_index_address());
  WheelPointsConstruct(base, w.points_address(), sp);
  SceneWheelsReset(base, object, sp);
  return object;
}

uint32_t SceneTrafficConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E6E70 SceneTraffic::SceneTraffic");
  const uint32_t sp = stack - 96;
  const schema::SceneTraffic t(base, object);
  Node(base, object, list, schema::SceneTraffic::kVtable);
  scene_models::CarRecordSetW(base, t.car_record_address());
  TrafficPointsConstruct(base, t.points_address(), sp);
  SceneTrafficReset(base, object, sp);
  return object;
}

uint32_t SceneModelConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB330 SceneModel::SceneModel");
  const uint32_t sp = stack - 128;
  const schema::SceneModel m(base, object);
  scene_loaders::PoolNodeConstruct(base, object, list);
  m.set_index(0xFFFFFFFFu);
  m.set_vtable(schema::SceneModel::kVtable);
  m.set_state(0);
  scene_models::SceneModelRecordConstruct(base, m.record_address());
  SceneModelBlockConstruct(base, m.draw_block_address());
  scene_models::SceneModelRecordReset(base, m.record_address());
  SceneModelBlockReset(base, m.draw_block_address(), sp);
  m.set_set_up(0);
  m.set_release_pending(0);
  m.set_second_wheels_claimed(0);
  m.set_ready(0);
  m.set_build_pending(0);
  m.set_build_delay(0);
  m.set_car_model(0u);
  m.set_car_model_ready(0);
  m.set_decals_reloaded(0);
  m.set_parts(0u);
  m.set_parts_ready(0);
  m.set_interior(0);
  m.set_interior_ready(0);
  m.set_cockpit(0);
  m.set_cockpit_ready(0);
  m.set_light_kit(0u);
  m.set_light_kit_ready(0);
  m.set_traffic_body(0);
  m.set_traffic_body_ready(0);
  m.set_wheels(0);
  m.set_wheels_ready(0);
  m.set_second_wheels(0);
  m.set_second_wheels_ready(0);
  m.set_scene(Scene(base));
  return object;
}

namespace {

constexpr uint32_t kPoolObjectVtable = 0x82004F90;

// A destructor: the vtable, `release` (false where it declines), the point
// block's destructor (on `points`, the block's address), the base vtable,
// ~PoolNode.
bool Destruct(uint8_t* base, uint32_t object, uint32_t vtable, uint32_t stack,
              std::optional<uint32_t> (*release)(uint8_t*, uint32_t, uint32_t),
              uint32_t points, uint32_t (*destruct_points)(uint32_t)) {
  const uint32_t sp = stack - 96;
  schema::PoolNode(base, object).set_vtable(vtable);
  if (!release(base, object, sp)) {
    return false;
  }
  if (destruct_points != nullptr) {
    destruct_points(points);
  }
  schema::PoolNode(base, object).set_vtable(kPoolObjectVtable);
  scene_loaders::PoolNodeDestruct(base, object);
  return true;
}

// A deleting destructor: bit 0 (operator delete) left to the original.
bool DeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack,
                      bool (*destruct)(uint8_t*, uint32_t, uint32_t)) {
  return (flags & 1) == 0 && destruct(base, object, stack - 112);
}

}  // namespace

bool SceneCarModelDestruct(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB720 SceneCarModel::~SceneCarModel");
  return Destruct(base, object, schema::SceneCarModel::kVtable, stack, SceneCarModelRelease, 0,
                  nullptr);
}

bool ScenePartsDestruct(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2E10 SceneParts::~SceneParts");
  return Destruct(base, object, schema::SceneParts::kVtable, stack, ScenePartsRelease,
                  schema::SceneParts(base, object).points_address(),
                  PartsPointsDestruct);
}

bool SceneInteriorDestruct(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5178 SceneInterior::~SceneInterior");
  return Destruct(base, object, schema::SceneInterior::kVtable, stack, SceneInteriorRelease,
                  schema::SceneInterior(base, object).points_address(), InteriorPointsDestruct);
}

bool SceneLightsDestruct(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DDE00 SceneLights::~SceneLights");
  return Destruct(base, object, schema::SceneLightKit::kVtable, stack, SceneLightsRelease,
                  schema::SceneLightKit(base, object).points_address(),
                  LightPointsDestruct);
}

bool SceneWheelsDestruct(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECDD0 SceneWheels::~SceneWheels");
  return Destruct(base, object, schema::SceneWheels::kVtable, stack, SceneWheelsRelease,
                  schema::SceneWheels(base, object).points_address(), WheelPointsDestruct);
}

void SceneTrafficDestruct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820E69F0 SceneTraffic::~SceneTraffic");
  const schema::SceneTraffic t(base, object);
  t.set_vtable(schema::SceneTraffic::kVtable);
  TrafficPointsDestruct(t.points_address());
  t.set_vtable(kPoolObjectVtable);
  scene_loaders::PoolNodeDestruct(base, object);
}

bool SceneCarModelDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags,
                                   uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB7C8 SceneCarModel::DeletingDestructor");
  return DeletingDestruct(base, object, flags, stack, SceneCarModelDestruct);
}

bool ScenePartsDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2F78 SceneParts::DeletingDestructor");
  return DeletingDestruct(base, object, flags, stack, ScenePartsDestruct);
}

bool SceneInteriorDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags,
                                   uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E53A0 SceneInterior::DeletingDestructor");
  return DeletingDestruct(base, object, flags, stack, SceneInteriorDestruct);
}

bool SceneLightsDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE090 SceneLights::DeletingDestructor");
  return DeletingDestruct(base, object, flags, stack, SceneLightsDestruct);
}

bool SceneWheelsDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECEB8 SceneWheels::DeletingDestructor");
  return DeletingDestruct(base, object, flags, stack, SceneWheelsDestruct);
}

bool SceneTrafficDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags,
                                  uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E6ED8 SceneTraffic::DeletingDestructor");
  (void)stack;
  if ((flags & 1) != 0) {
    return false;
  }
  SceneTrafficDestruct(base, object);
  return true;
}

bool PoolObjectDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB400 PoolObject::DeletingDestructor");
  (void)stack;
  if ((flags & 1) != 0) {
    return false;
  }
  schema::PoolNode(base, object).set_vtable(kPoolObjectVtable);
  scene_loaders::PoolNodeDestruct(base, object);
  return true;
}

bool SceneCockpitDestruct(uint8_t* base, uint32_t object, uint32_t stack,
                          const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DC2F8 SceneCockpit::~SceneCockpit");
  const uint32_t sp = stack - 96;
  const schema::SceneCockpit c(base, object);
  c.set_vtable(schema::SceneCockpit::kVtable);
  if (!SceneCockpitRelease(base, object, sp, calls)) {
    return false;
  }
  CockpitPointsDestruct(c.points_address());
  c.set_vtable(kPoolObjectVtable);
  scene_loaders::PoolNodeDestruct(base, object);
  return true;
}

bool SceneCockpitDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack,
                                  const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DC448 SceneCockpit::DeletingDestructor");
  return (flags & 1) == 0 && SceneCockpitDestruct(base, object, stack - 112, calls);
}

void SceneModelBlockConstruct(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820DA108 SceneModelBlock::SceneModelBlock");
  const schema::CarDrawBlock b(base, block);
  for (uint32_t row = 0; row < 4; row++) {
    CopyWord(base, kOne, b.world_row0_w_address() + 16 * row);
  }
  for (const uint32_t from : {b.wheel_turns_address(), b.wheel_spins_address()}) {
    for (uint32_t m = 0; m < 4; m++) {
      for (uint32_t row = 0; row < 4; row++) {
        CopyWord(base, kOne, from + 64 * m + 16 * row + 12);
      }
    }
  }
  CopyWord(base, kOne, b.left_door_w_address());
  CopyWord(base, kOne, b.right_door_w_address());
}

uint32_t SceneModelBlockReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9F50 SceneModelBlock::Reset");
  const uint32_t f = stack - kSceneModelBlockResetFrameBytes;
  const schema::CarDrawBlock b(base, block);
  b.set_drawn(0);
  b.set_cockpit_only(0);
  Store(base, b.head_address(), 0);
  Store(base, b.head_address() + 4, 0);
  Identity(base, f + 80);
  Memcpy(base, b.world_address(), f + 80, 64, 0x820D9FC4);
  b.set_rear_second_matrix(0);
  uint32_t r3 = b.world_address();
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t matrix = b.wheel_turns_address() + 64 * k;
    Identity(base, f + 80);
    Memcpy(base, b.wheel_spins_address() + 64 * k, f + 80, 64, 0x820DA038);
    Identity(base, f + 144);
    Memcpy(base, matrix, f + 144, 64, 0x820DA068);
    r3 = matrix;
    CopyWord(base, kZero, b.brake_alphas_address() + 4 * k);
    Store(base, b.rim_looks_address() + 4 * k, 2);
  }
  CopyWord(base, kZero, b.bonnet_angle_address());
  CopyWord(base, 0x8200296C, b.window_alpha_02_address());
  CopyWord(base, 0x82002A10, b.window_alpha_01_address());
  CopyWord(base, 0x82002208, b.lights_alpha_address());
  // The doors' two float4s (+656 to +684).
  for (uint32_t w = 0; w < 8; w++) {
    CopyWord(base, kZero, b.left_door_x_address() + 4 * w);
  }
  b.set_popup_set(0);
  CopyWord(base, kZero, b.popup_angle_address());
  b.set_identity_world(0);
  CopyWord(base, kZero, b.speed_address());
  b.set_word_728(0);
  CopyWord(base, kZero, b.unknown_700_address());
  b.set_no_side_offset(1);
  return r3;
}

}  // namespace second_set
