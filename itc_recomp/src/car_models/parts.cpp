// The car's part loads (see parts.h), call for call as the recompiled
// functions.

#include "car_models/parts.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "car_models/car_model.h"
#include "car_models/load_slots.h"
#include "car_models/vinyl.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/accessories.h"
#include "schema/car_draw.h"
#include "schema/car_makers.h"
#include "schema/car_models.h"
#include "schema/car_sounds.h"
#include "schema/game_modes.h"
#include "schema/load_poll.h"
#include "schema/material_passes.h"
#include "schema/race_sounds.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
#include "schema/stand_models.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cstring>

namespace car_models {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

using G = schema::CarModelsGlobals;
// The car scene ([0x826C2E44]) and the first set of pools ([0x826C2E40]).
uint32_t Scene() { return G::scene().address; }
uint32_t FirstSet() { return G::first_set().address; }
// The traffic car's index the word at `at` points to.
uint32_t TrafficIndex(uint32_t at) {
  return schema::CarModelsTrafficIndex(Load(at)).index();
}

// One end of the rims or brakes: `n` the wheel number, `block` the car's;
// the part's id `part` for a block above 19, else 3 x block +
// `part_block_base`, or 3n + `part_wheel_base` for a wheel.
void WheelEnd(uint32_t xpr_at, uint32_t part_at, int32_t n, int32_t block,
              uint32_t part, uint32_t part_block_base, uint32_t part_wheel_base) {
  const schema::LoadSlot xpr(xpr_at), slot(part_at);
  if (n == -1) {
    if (block > 19) {
      slot.set_id(part);
      xpr.set_id(695);
      return;
    }
    xpr.set_id(3 * uint32_t(block) + 587);
    slot.set_id(3 * uint32_t(block) + part_block_base);
    return;
  }
  xpr.set_id(3 * uint32_t(n) + 695);
  slot.set_id(3 * uint32_t(n) + part_wheel_base);
}

}  // namespace

void WheelsRims(uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8BA8 Wheels::Rims");
  const schema::CarWheels w(wheels);
  const int32_t block = int32_t(w.car_model().block());
  WheelEnd(w.front_xpr_id_address(), w.front_rim_id_address(), int32_t(w.front_wheel()),
           block, 696, 588, 696);
  WheelEnd(w.rear_xpr_id_address(), w.rear_rim_id_address(), int32_t(w.rear_wheel()),
           block, 696, 588, 696);
}

void WheelsBrakes(uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8C50 Wheels::Brakes");
  const schema::CarWheels w(wheels);
  if (w.type() == 3) {
    const uint32_t x = 3 * (w.traffic().index() + 20);
    w.set_front_rim_id(x + 589);
    w.set_front_xpr_id(x + 587);
    w.set_rear_rim_id(x + 589);
    w.set_rear_xpr_id(x + 587);
    return;
  }
  const int32_t block = int32_t(w.car_model().block());
  WheelEnd(w.front_xpr_id_address(), w.front_rim_id_address(), int32_t(w.front_wheel()),
           block, 697, 589, 697);
  WheelEnd(w.rear_xpr_id_address(), w.rear_rim_id_address(), int32_t(w.rear_wheel()),
           block, 697, 589, 697);
}

std::optional<uint32_t> WheelsSetUp(uint32_t wheels, uint32_t setup,
                                    const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D9110 Wheels::SetUp");
  const schema::CarWheels w(wheels);
  if (!native::Buffers_Memcpy(w.setup_address(), setup, 156, 0x820D912C)) {
    return std::nullopt;
  }
  if (w.first_set().model_set() == 0) {
    WheelsRims(wheels);
  } else {
    WheelsBrakes(wheels);
  }
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t part = w.rim_slots_address() + 24 * i;
    const uint32_t tire =
        Load(w.front_tire_aftermarket_address() + 4 * i) == 0 ? 582 : 584;
    LoadSlotLoadShared(part - 976, tire, wheels, kOnWheelPartLoaded, load);
    LoadSlotLoadShared(part, schema::LoadSlot(part).id(), wheels, kOnWheelPartLoaded,
                       load);
    LoadSlotLoadShared(part + 976, schema::LoadSlot(part + 976).id(), wheels,
                       kOnWheelPartLoaded, load);
    r3 = LoadSlotLoadShared(part + 1112, 887, wheels, kOnWheelPartLoaded, load);
  }
  return r3;
}

std::optional<uint32_t> CockpitLoadSetC(uint32_t object, uint32_t setup,
                                        uint32_t stack, uint32_t pcr,
                                        const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D67E8 CarInterior::LoadSetCBody");
  const schema::CarInterior cockpit(object);
  if (!native::Buffers_Memcpy(cockpit.setup_address(), setup, 144, 0x820D6808)) {
    return std::nullopt;
  }
  const uint32_t block = cockpit.car_model().block();
  const uint32_t n = block * 55;
  if (LoadSlotLoad(cockpit.cockpit_node_file_slot_address(), n + 19928, object,
                   kOnCockpitPartLoaded, pcr, load) == 0) {
    // sprintf's call from the original's 1,152-byte frame: the block spilled
    // into the frame's parameter area as sprintf spills r5 (r6 to r10, which
    // the call doesn't set, as 0: only dead frame words differ), its own
    // frames below. Its text, in the frame, is read by nothing.
    const uint32_t frame = stack - 1152;
    for (uint32_t i = 0; i < 6; i++) {
      Store(frame + 32 + 8 * i, 0);
      Store(frame + 36 + 8 * i, i == 0 ? block : 0);
    }
    int32_t written;
    c_runtime::Sprintf(frame + 80, kCockpitNodeFormat, frame + 32, frame - 144 - 1328,
                       &written);
  }
  const uint32_t body = n + 19926;
  const uint32_t buffer = G::scene().load_buffer();
  cockpit.set_cockpit_body_id(body);
  cockpit.set_cockpit_body_size(archive::AssetSize(body));
  const int64_t serial = load(body, buffer, kOnCockpitPartLoaded, object);
  cockpit.set_cockpit_body_request(uint32_t(serial));
  cockpit.set_cockpit_body_state(2);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    uint32_t id;
    switch (i) {
      case 0: id = cockpit.cockpit_bonnet() + body + 3; break;
      case 1: id = cockpit.cockpit_door() + body + 16; break;
      case 2: id = cockpit.cockpit_mirrors() + body + 19; break;
      case 3: id = cockpit.cockpit_headlights() + body + 31; break;
      case 4: id = cockpit.cockpit_fenders() + body + 36; break;
      default: id = body + 35; break;
    }
    r3 = LoadSlotLoadShared(cockpit.cockpit_part_slots_address() + 24 * i, id, object,
                            kOnCockpitPartLoaded, load);
  }
  return r3;
}

uint32_t InteriorLoadParts(uint32_t object, uint32_t first, uint32_t stride,
                           uint32_t offset, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7730 CarInterior::LoadParts");
  const schema::CarInterior interior(object);
  const int32_t block = int32_t(interior.car_model().block());
  const uint32_t id = uint32_t(int64_t(block) * int64_t(int32_t(stride))) + first + offset;
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    r3 = LoadSlotLoadShared(interior.part_slots_address() + 24 * i, id + i, object,
                            kOnInteriorPartLoaded, load);
  }
  return r3;
}

std::optional<uint32_t> InteriorLoad(uint32_t object, uint32_t setup,
                                     const archive::LoadCall& load,
                                     const AccessoryLoadCall& accessory_load) {
  ITC_NATIVE_RUNNING("sub_820D79B0 CarInterior::Load");
  const uint32_t accessories = scene_models::Accessories();
  const schema::CarInterior interior(object);
  if (!native::Buffers_Memcpy(interior.setup_address(), setup, 120, 0x820D79E0)) {
    return std::nullopt;
  }
  uint32_t r3 = interior.setup_address();  // memcpy's answer
  const uint32_t set = interior.first_set().model_set();
  const bool cockpit = interior.cockpit() != 0;
  if (set == 1) {
    if (cockpit) {
      InteriorLoadParts(object, 19926, 55, 49, load);
    } else {
      InteriorLoadParts(object, 11061, 197, 190, load);
    }
    r3 = accessory_load(accessories, interior.accessory(), interior.accessory_arg(), 0);
  } else if (set == 0) {
    if (cockpit) {
      InteriorLoadParts(object, 19926, 55, 49, load);
    } else {
      InteriorLoadParts(object, 891, 226, 219, load);
    }
    for (uint32_t i = 0; i < 3; i++) {
      r3 = accessory_load(accessories, i, 0, 1);
      schema::AccessorySkeleton(schema::Accessory(r3).skeleton_address())
          .set_swinging(0);
    }
  }
  return r3;
}

// --- The first set's pools' set-ups ---

void CarPartsLoadSetA(uint32_t object, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7090 CarParts::LoadSetA");
  const schema::CarParts parts(object);
  uint32_t id = parts.car_model().block() * 226 + 894;
  uint32_t slot = parts.slots_address();
  for (uint32_t i = 0; i < 216; i++, slot += 24, id++) {
    schema::LoadSlot(slot).set_id(id);
    LoadSlotLoadShared(slot, id, object, kOnPartsLoadedA, load);
  }
}

void CarPartsLoadSetB(uint32_t object, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D70F0 CarParts::LoadSetB");
  const schema::CarParts parts(object);
  const uint32_t first = parts.car_model().block() * 197 + 11061;
  // The record's words (the parts' +28 on).
  const auto field = [&](uint32_t offset) { return Load(parts.record_address() + offset); };
  for (uint32_t j = 0; j < 17; j++) {
    uint32_t id;
    switch (j) {
      case 0: id = field(0) + first + 3; break;
      case 1: id = field(0) * 7 + field(4) + first + 16; break;
      case 2: id = field(8) + first + 107; break;
      case 3: id = field(12) + first + 120; break;
      case 4: {
        const uint32_t value = field(16);
        if (int32_t(value) == 13) {
          continue;
        }
        id = value + first + 133;
        break;
      }
      case 5: id = field(20) + first + 146; break;
      case 6: id = field(24) + first + 154; break;
      case 8: id = field(28) + first + 157; break;
      case 10: id = field(32) + first + 169; break;
      case 11: id = field(36) + first + 173; break;
      case 12: id = field(40) + first + 178; break;
      case 13: id = first + 177; break;
      case 15: id = field(48) + first + 184; break;
      case 16: id = first + 189; break;
      default: continue;  // 7, 9 and 14
    }
    LoadSlotLoadShared(parts.slots_address() + 24 * j, id, object, kOnPartsLoadedB, load);
  }
}

bool CarPartsLoad(uint32_t object, uint32_t setup, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7510 CarParts::Load");
  const schema::CarParts parts(object);
  if (!native::Buffers_Memcpy(parts.record_address(), setup, 60, 0x820D7530)) {
    return false;
  }
  const uint32_t set = parts.first_set().model_set();
  if (set == 0) {
    CarPartsLoadSetA(object, load);
  } else if (set == 1) {
    CarPartsLoadSetB(object, load);
  }
  return true;
}

bool Pool4Load(uint32_t object, uint32_t record, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D6B48 Pool4Object::Load");
  const schema::Pool4Object pool4(object);
  if (!native::Buffers_Memcpy(pool4.record_address(), record, 52, 0x820D6B68)) {
    return false;
  }
  const schema::ScenePools scene_set = pool4.first_set();
  const schema::CarModel car = pool4.car_model();
  const uint32_t set = scene_set.model_set();
  const uint32_t block = car.block();
  const uint32_t id = set == 0 ? block * 226 + 1116 : block * 197 + 11257;
  LoadSlotLoadShared(pool4.slot_address(), id, object, kOnPool4Loaded, load);
  return true;
}

bool TrafficCarLoad(uint32_t object, uint32_t record, uint32_t stack, uint32_t pcr,
                    const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D8A08 TrafficCar::Load");
  const schema::TrafficCar traffic(object);
  if (!scene_models::CarRecordCopy(traffic.car_number_address(), record)) {
    return false;
  }
  const uint32_t frame = stack - 1136;
  const uint32_t number = traffic.car_number();
  traffic.set_first_set(FirstSet());
  if (LoadSlotLoad(traffic.file_slot_address(), number + 22529, object, kOnTrafficLoaded, pcr, load) ==
      0) {
    // sprintf's call from the original's frame: the number spilled into the
    // frame's parameter area as sprintf spills r5 (r6 to r10, which the
    // call doesn't set, as 0: only dead frame words differ), its own frames
    // below. Its text, in the frame, is read by nothing.
    for (uint32_t i = 0; i < 6; i++) {
      Store(frame + 32 + 8 * i, 0);
      Store(frame + 36 + 8 * i, i == 0 ? number : 0);
    }
    int32_t written;
    c_runtime::Sprintf(frame + 80, kTrafficFormat, frame + 32, frame - 144 - 1328,
                       &written);
  }
  TextureCreate(0x18280086, 16, 16, traffic.texture_word0_address(),
                traffic.texture_word1_address(), frame);
  return true;
}

uint32_t ScenePoolsSetUp(uint32_t pools, uint32_t pool, uint32_t index,
                         uint32_t record, uint32_t stack, uint32_t pcr,
                         const archive::LoadCall& load, const PoolSetUps& native,
                         const SetUpCall& call) {
  ITC_NATIVE_RUNNING("sub_820C4338 ScenePools::SetUp");
  const uint32_t object = vehicle_accessors::ListAt(schema::ScenePools(pools).pools_address() + pool * 12, index);
  if (int32_t(schema::PoolNode(object).state()) != 3) {
    return 0;
  }
  const uint32_t frame = stack - 112;
  // The set-up through the guest, from the frame (its back chain written).
  const auto guest = [&](uint32_t function, uint32_t lr) {
    Store(frame, stack);
    call(function, object, record, frame, lr);
  };
  switch (pool) {
    case 0:
      if (!(native.car_models && native.heap &&
            CarModelLoadExterior(object, record, frame, pcr, load))) {
        guest(0x820D5EC8, 0x820C4410);
      }
      break;
    case 1:
      if (!(native.loaders && native.car_models && CarPartsLoad(object, record, load))) {
        guest(0x820D7510, 0x820C4400);
      }
      break;
    case 2:
      if (!(native.car_models && native.heap &&
            CockpitLoadSetC(object, record, frame, pcr, load))) {
        guest(0x820D67E8, 0x820C43F0);
      }
      break;
    case 3:
      guest(0x820D79B0, 0x820C43E0);
      break;
    case 4:
      if (!(native.loaders && native.car_models && Pool4Load(object, record, load))) {
        guest(0x820D6B48, 0x820C43D0);
      }
      break;
    case 5:
      if (!(native.car_models && WheelsSetUp(object, record, load))) {
        guest(0x820D9110, 0x820C43C0);
      }
      break;
    case 6:
      if (!(native.loaders && native.car_models && native.heap &&
            TrafficCarLoad(object, record, frame, pcr, load))) {
        guest(0x820D8A08, 0x820C43B0);
      }
      break;
    default:
      break;
  }
  return object;
}

// --- The second set's set-ups ---

namespace {

// ScenePools::SetUp's plain C++ from a set-up's frame.
uint32_t SetUpFirst(uint32_t pools, uint32_t pool, uint32_t index,
                    uint32_t record, uint32_t frame, const SecondSetUps& setups) {
  return ScenePoolsSetUp(pools, pool, index, record, frame, setups.pcr, setups.load, setups.native,
      [&](uint32_t function, uint32_t r3, uint32_t r4, uint32_t r1, uint32_t lr) {
        return setups.guest(function, r3, r4, 0, r1, lr);
      });
}

// The claim and set-up the parts' and pool 4's set-ups share: pool `pool`
// claimed, its object (a `First`) into the word at `at`, its car model = the
// one of the second set's car model at `car_at`, set up with `copy`, the
// result into `at`.
template <typename First>
uint32_t ClaimLinkSetUp(uint32_t at, uint32_t car_at, uint32_t pool,
                        uint32_t copy, uint32_t frame, const SecondSetUps& setups) {
  namespace va = vehicle_accessors;
  const uint32_t pools = FirstSet();
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(pools, pool));
  const uint32_t first = va::ScenePoolsGetA(pools, pool, index);
  Store(at, first);
  First(first).set_car_model(
      schema::CarModelsSceneCarModel(Load(car_at)).car_model());
  const uint32_t r3 = SetUpFirst(pools, pool, index, copy, frame, setups);
  Store(at, r3);
  return r3;
}

}  // namespace

std::optional<uint32_t> SceneCarModelSetUp(uint32_t object, uint32_t record,
                                           uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DB818 SceneCarModel::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t frame = stack - 112;
  const uint32_t pools = FirstSet();
  const schema::SceneCarModel model(object);
  model.set_scene(Scene());
  if (!scene_models::CarRecordCopy(model.car_record_address(), record)) {
    return std::nullopt;
  }
  Store(frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(pools, 0));
  const uint32_t r3 = SetUpFirst(pools, 0, index, model.car_record_address(), frame, setups);
  model.set_car_model(r3);
  return r3;
}

std::optional<uint32_t> ScenePartsSetUp(uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DF1D0 SceneParts::SetUp");
  const schema::SceneParts parts(object);
  if (!native::Buffers_Memcpy(parts.record_address(), record, 60, 0x820DF1F8)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(frame, stack);
  return ClaimLinkSetUp<schema::CarParts>(parts.first_set_parts_address(),
                                          parts.scene_car_model_address(), 1,
                                          parts.record_address(), frame, setups);
}

std::optional<uint32_t> ScenePool4SetUp(uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DCAA0 ScenePool4::SetUp");
  const schema::SceneLightKit kit(object);
  if (!native::Buffers_Memcpy(kit.record_address(), record, 52, 0x820DCAC8)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(frame, stack);
  return ClaimLinkSetUp<schema::Pool4Object>(kit.first_set_object_address(),
                                             kit.car_model_address(), 4, kit.record_address(),
                                             frame, setups);
}

std::optional<uint32_t> SceneInteriorSetUp(uint32_t object, uint32_t record,
                                           uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820E37F0 SceneInterior::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t pools = FirstSet();
  const schema::SceneInterior interior(object);
  interior.set_scene(Scene());
  if (!native::Buffers_Memcpy(interior.record_address(), record, 120, 0x820E3824)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(pools, 3));
  const uint32_t first = va::ScenePoolsGetA(pools, 3, index);
  interior.set_first_set_interior(first);
  if (interior.cockpit() != 0) {
    schema::Interior(first).set_cockpit(1);
  }
  // The first set's interior read back before the car model's chain, as the
  // original.
  const schema::Interior first_interior = interior.first_set_interior();
  first_interior.set_car_model(interior.car_model().car_model());
  const uint32_t r3 =
      SetUpFirst(pools, 3, index, interior.record_address(), frame, setups);
  interior.set_first_set_interior(r3);
  return r3;
}

std::optional<uint32_t> SceneTrafficSetUp(uint32_t object, uint32_t record,
                                          uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820E5998 SceneTraffic::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t pools = FirstSet();
  const schema::SceneTraffic traffic(object);
  traffic.set_scene(Scene());
  if (!native::Buffers_Memcpy(traffic.car_record_address(), record, 3696, 0x820E59CC)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(pools, 6));
  traffic.set_body(va::ScenePoolsGetA(pools, 6, index));
  return SetUpFirst(pools, 6, index, record, frame, setups);
}

std::optional<uint32_t> SceneWheelsSetUp(uint32_t object, uint32_t record,
                                         uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820E70C0 SceneWheels::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t kTable = G::wheel_table_address();
  const uint32_t pools = FirstSet();
  const schema::SceneWheels w(object);
  const uint32_t copy = w.record_address();
  if (!native::Buffers_Memcpy(copy, record, 156, 0x820E70EC)) {
    return std::nullopt;
  }
  // lfs and stfs: through a double, as the original.
  const auto copy_float = [&](uint32_t to, uint32_t from) {
    const double d = double(std::bit_cast<float>(Load(from)));
    Store(to, std::bit_cast<uint32_t>(float(d)));
  };
  const bool alternate = w.brake_kind() != 0;
  uint32_t index;
  if (!alternate && int32_t(w.kind()) == 3) {
    index = TrafficIndex(w.traffic_record_address()) + 45;
  } else {
    index = schema::CarModelsSceneCarModel(w.car_model()).car_model().block();
  }
  // The table's 44-byte entry (an image table of words).
  const uint32_t entry = kTable + index * 44;
  w.set_table_112(Load(entry + 4));
  w.set_table_116(Load(entry + 8));
  if (!alternate) {
    w.set_caliper_texture(Load(entry + 12));
  }
  w.set_table_124(Load(entry + 16));
  w.set_table_128(Load(entry + 20));
  copy_float(w.front_brake_turn_address(), entry + 24);
  copy_float(w.rear_brake_turn_address(), entry + 28);
  w.set_front_brake_size(Load(entry + 32));
  w.set_rear_brake_size(Load(entry + 36));
  w.set_front_tire(Load(entry + 40));
  w.set_rear_tire(Load(entry + 40));
  const uint32_t frame = stack - 160;
  Store(frame, stack);
  if (!native::Buffers_Memcpy(record, copy, 156, 0x820E7238)) {
    setups.guest(0x825A7410, record, copy, 156, frame, 0x820E7238);
  }
  const uint32_t front = w.front_wheel_address(), rear = w.rear_wheel_address();
  if (int32_t(w.front_wheel()) == -1 && int32_t(w.rear_wheel()) == -1) {
    const uint32_t car = w.car_model();
    if (car != 0) {
      const uint32_t block = schema::CarModelsSceneCarModel(car).car_model().block();
      if (int32_t(block) >= 18) {
        setups.guest(0x820D39D8, block, front, rear, frame, 0x820E727C);
      }
    }
  }
  if (int32_t(w.front_wheel()) >= 0) {
    w.set_front_tire(w.front_wheel() == 190 ? 0 : 1);
  }
  if (int32_t(w.rear_wheel()) >= 0) {
    w.set_rear_tire(w.rear_wheel() == 190 ? 0 : 1);
  }
  const uint32_t claimed = uint32_t(va::ScenePoolsClaimA(pools, 5));
  const uint32_t wheels = va::ScenePoolsGetA(pools, 5, claimed);
  w.set_first_set_wheels(wheels);
  const uint32_t car = w.car_model();
  if (car != 0) {
    schema::CarWheels(wheels)
        .set_car_model(schema::CarModelsSceneCarModel(car).car_model());
  } else {
    schema::CarWheels(wheels).set_traffic(w.traffic_record());
  }
  return SetUpFirst(pools, 5, claimed, copy, frame, setups);
}

// --- ScenePools::SetUpB and the set-ups above the others ---

uint32_t ScenePoolsSetUpB(uint32_t scene, uint32_t pool, uint32_t index,
                          uint32_t record, uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820C5B28 ScenePools::SetUpB");
  const uint32_t object = vehicle_accessors::ListAt(schema::CarScene(scene).car_models_first_address() + pool * 12, index);
  if (int32_t(schema::PoolNode(object).state()) != 3) {
    return 0;
  }
  if (pool > 7) {
    return object;
  }
  const uint32_t frame = stack - 112;
  Store(frame, stack);
  using SetUp = std::optional<uint32_t> (*)(uint32_t, uint32_t, uint32_t,
                                            const SecondSetUps&);
  struct Case {
    SetUp setup;
    uint32_t function, lr;
  };
  // Pool 0's case is sub_820C5B98's call (merged), with its return address.
  static constexpr Case kCases[8] = {
      {SceneCarModelSetUp, 0x820DB818, 0x820C5BA4}, {ScenePartsSetUp, 0x820DF1D0, 0x820C5BB4},
      {SceneCockpitSetUp, 0x820DBE80, 0x820C5BD4},  {SceneInteriorSetUp, 0x820E37F0, 0x820C5BC4},
      {ScenePool4SetUp, 0x820DCAA0, 0x820C5BE4},    {SceneWheelsSetUp, 0x820E70C0, 0x820C5BF4},
      {SceneTrafficSetUp, 0x820E5998, 0x820C5C04},  {SceneModelSetUp, 0x820DB458, 0x820C5C14},
  };
  const Case& c = kCases[pool];
  if (!c.setup(object, record, frame, setups)) {
    setups.guest(c.function, object, record, 0, frame, c.lr);
  }
  return object;
}

std::optional<uint32_t> SceneCockpitSetUp(uint32_t object, uint32_t record,
                                          uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DBE80 SceneCockpit::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t pools = FirstSet();
  const schema::SceneCockpit cockpit(object);
  cockpit.set_scene(Scene());
  if (!native::Buffers_Memcpy(cockpit.record_address(), record, 144, 0x820DBEB4)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(pools, 2));
  const uint32_t first = va::ScenePoolsGetA(pools, 2, index);
  const uint32_t car_model = cockpit.car_model();
  cockpit.set_body(first);
  schema::Cockpit(first)
      .set_car_model(schema::CarModelsSceneCarModel(car_model).car_model());
  SetUpFirst(pools, 2, index, cockpit.record_address(), frame, setups);
  const uint32_t claimed = uint32_t(va::ScenePoolsClaimB(cockpit.scene().address, 3));
  const uint32_t interior = va::ScenePoolsGetB(cockpit.scene().address, 3, claimed);
  cockpit.set_interior(interior);
  schema::SceneInterior(interior).set_cockpit(1);
  scene_models::SetWord11452(schema::SceneInterior(interior).points_address(), 1);
  scene_models::InteriorLink(cockpit.interior().address, cockpit.car_model());
  cockpit.interior().set_skeleton(schema::Cockpit(cockpit.body()).skeleton_address());
  cockpit.interior().set_texture_set(schema::Cockpit(cockpit.body()).providers_address());
  // The record's +24, the interior's record.
  return ScenePoolsSetUpB(cockpit.scene().address, 3, claimed,
                          cockpit.record_address() + 24, frame, setups);
}

std::optional<uint32_t> SceneModelSetUp(uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups) {
  using Sounds = schema::RaceSoundsGlobals;
  using CarSounds = schema::CarSoundsGlobals;
  // 16-byte entries: +8, +12 the ids' arrays
  constexpr uint32_t kSoundEntries = CarSounds::sound_parameters_address();
  constexpr uint32_t kSoundBuffersA = Sounds::slot_buffers_1_address();
  constexpr uint32_t kSoundBuffersB = Sounds::slot_buffers_0_address();
  constexpr uint32_t kSoundState = Sounds::slot_asked_address();
  constexpr uint32_t kSoundContexts = Sounds::slot_contexts_address();
  constexpr uint32_t kSoundEntry = CarSounds::sound_picks_a_address();
  constexpr uint32_t kSoundIndex = CarSounds::sound_picks_b_address();
  constexpr uint32_t kOnSoundLoaded = 0x82175EE0;
  constexpr uint32_t kRecordBytes = scene_models::kSceneModelRecordBytes;
  // The original where the record overlaps the copy, or where the tail
  // sub_820DB53C would run (read from the record before its copy: its +0,
  // the car's block at +16, the parts record's +44 at +3756).
  const schema::SceneModel m(object);
  const schema::SceneModelRecord from(record);
  const uint32_t to = m.record_address();
  if (record < to + kRecordBytes && to < record + kRecordBytes) {
    return std::nullopt;
  }
  const schema::CarScene scene_now = m.scene();
  const int32_t kind_now = int32_t(from.kind());
  const uint32_t parts44 = from.parts_engine_stage();
  if (int32_t(scene_now.model_set()) == 1 &&
      int32_t(schema::GameModesGlobals::current_type()) != 5 &&
      (kind_now == 1 || kind_now == 2) && int32_t(from.block()) < 18 &&
      (parts44 == 3 || parts44 == 4)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820DB458 SceneModel::SetUp");
  const uint32_t frame = stack - 144;
  Store(frame, stack);
  m.set_set_up(1);
  uint32_t r3 = to;
  if (!scene_models::SceneModelRecordCopy(to, record)) {
    r3 = setups.guest(0x820DB290, to, record, 0, frame, 0x820DB47C);
  }
  const int32_t kind = int32_t(m.kind());
  const int32_t block = int32_t(m.block());
  m.set_wheels_kind(uint32_t(kind));
  if (block == 5) {
    m.set_interior_gauge1_present(1);
    m.set_interior_gauge1_kind(0);
  }
  const schema::CarScene scene = m.scene();
  m.set_windows_alpha(guest::LoadFloat(int32_t(scene.model_set()) == 0 ? schema::MaterialPassesGlobals::value_0_3_address()
                                            : G::float_tenth_address()));
  if (int32_t(scene.model_set()) == 1 &&
      int32_t(schema::GameModesGlobals::current_type()) != 5) {
    // The entry (r30) and the index into its arrays (r27).
    uint32_t entry = 0, at = 0;
    if (kind > 0) {
      if (kind <= 2) {
        entry = uint32_t(block);
        if (block < 18) {
          switch (m.parts_engine_stage()) {
            case 5:
            case 6:
              at = 2;
              break;
            case 7:
            case 8:
              at = 3;
              break;
            case 9:
              at = 4;
              break;
            default:
              break;
          }
        }
      } else if (kind == 3) {
        entry = 47;
      }
    }
    const auto slot = [&] { return m.index() << 2; };
    Store(slot() + Sounds::slot_arrived_address(), 0);
    Store(slot() + kSoundState, 1);
    guest::StoreU8(m.index() + Sounds::slot_files_arrived_address(), 0);
    const auto clear = [&](uint32_t address, uint32_t bytes) {
      guest::ForEachHostPart(address, bytes, [](uint8_t* host, uint32_t, uint32_t n) {
        std::memset(host, 0, n);
      });
    };
    clear(Load(slot() + kSoundBuffersA), 0x6B080);
    clear(Load(slot() + kSoundBuffersB), 0x5D2D0);
    const uint32_t ids_b = Load((entry << 4) + kSoundEntries + 8);
    const uint32_t ids_a = Load((entry << 4) + kSoundEntries + 12);
    const uint32_t id_b = Load(ids_b + (at << 2));
    const uint32_t id_a =
        Load(ids_a + (int32_t(m.block()) < 18 ? at << 2 : 0));
    Store(slot() + kSoundEntry, entry);
    Store(slot() + kSoundIndex, at);
    setups.load(id_a, Load(slot() + kSoundBuffersA), kOnSoundLoaded,
                slot() + kSoundContexts);
    const int64_t serial = setups.load(id_b, Load(slot() + kSoundBuffersB),
                                       kOnSoundLoaded, slot() + kSoundContexts);
    r3 = uint32_t(serial);
  }
  if (m.build_delay() != 0) {
    m.set_build_pending(1);
    return r3;
  }
  return SceneModelBuild(object, frame, setups);
}

uint32_t SceneModelBuild(uint32_t model, uint32_t stack,
                         const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820D9870 SceneModel::Build");
  namespace va = vehicle_accessors;
  namespace sm = scene_models;
  const uint32_t frame = stack - 128;
  Store(frame, stack);
  const schema::SceneModel m(model);
  const auto scene = [&] { return m.scene().address; };
  // A part: claimed in pool `pool`, its object into the word at `at` and
  // `link` called with it, then set up with `record` and the result stored
  // into `at`.
  const auto part = [&](uint32_t pool, uint32_t at, uint32_t record, const auto& link) {
    const uint32_t index = uint32_t(va::ScenePoolsClaimB(scene(), pool));
    const uint32_t object = va::ScenePoolsGetB(scene(), pool, index);
    Store(at, object);
    link(object);
    const uint32_t r3 = ScenePoolsSetUpB(scene(), pool, index, record, frame, setups);
    Store(at, r3);
    return r3;
  };
  const auto none = [](uint32_t) {};
  const auto car_model = [&] { return m.car_model().address; };
  if (int32_t(m.kind()) == 3) {
    m.set_car_model(0xFFFFFFFFu);
    m.set_cockpit_ready(1);
    part(6, m.traffic_body_address(), m.car_record_address(), none);
    return part(5, m.wheels_address(), m.wheels_record_address(), [&](uint32_t wheels) {
      sm::WheelsSetRecord(wheels, m.car_record_address());
    });
  }
  {
    const uint32_t index = uint32_t(va::ScenePoolsClaimB(scene(), 0));
    m.set_car_model(
        ScenePoolsSetUpB(scene(), 0, index, m.car_record_address(), frame, setups));
  }
  const uint32_t parts = part(1, m.parts_address(), m.parts_record_address(),
                              [&](uint32_t object) { sm::PartsLink(object, car_model()); });
  sm::PartsSetRecord(parts, m.record_address());
  part(3, m.interior_address(), m.interior_record_address(),
       [&](uint32_t object) { sm::InteriorLink(object, car_model()); });
  part(4, m.light_kit_address(), m.light_kit_record_address(),
       [&](uint32_t object) { sm::SetCarModel(object, car_model()); });
  uint32_t r3 = part(5, m.wheels_address(), m.wheels_record_address(),
                     [&](uint32_t object) { sm::SetCarModel(object, car_model()); });
  if (int32_t(m.kind()) != 2 || int32_t(m.scene().model_set()) != 1) {
    m.set_cockpit_ready(1);
    return r3;
  }
  // The cockpit's record from the interior's.
  const uint32_t pools = scene();
  int32_t seats = int32_t(m.parts_bonnet());
  const uint32_t w3768 = m.parts_aero_kit();
  const uint32_t w3772 = m.parts_aero6();
  if (seats >= 6) {
    seats += 5;
  }
  m.set_cockpit_bonnet(uint32_t(seats));
  int32_t w3784 = int32_t(m.parts_aero9());
  m.set_cockpit_aero_kit(w3768);
  m.set_cockpit_aero6(w3772);
  if (w3784 >= 4) {
    w3784 += 7;
  }
  m.set_cockpit_aero9(uint32_t(w3784));
  const uint32_t w3776 = m.parts_aero7();
  const uint32_t w3728 = m.selected_texture();
  m.set_cockpit_aero7(w3776);
  m.set_cockpit_selected_texture(w3728);
  // The interior's record into the cockpit record's +24.
  const uint32_t cockpit_interior = m.cockpit_record_address() + 24;
  if (!native::Buffers_Memcpy(cockpit_interior, m.interior_record_address(), 120,
                              0x820D9B08)) {
    setups.guest(0x825A7410, cockpit_interior, m.interior_record_address(), 120, frame,
                 0x820D9B08);
  }
  const uint32_t index = uint32_t(va::ScenePoolsClaimB(pools, 2));
  const uint32_t cockpit = va::ScenePoolsGetB(scene(), 2, index);
  const uint32_t linked = car_model();
  m.set_cockpit(cockpit);
  schema::SceneCockpit(cockpit).set_car_model(linked);
  r3 = ScenePoolsSetUpB(scene(), 2, index, m.cockpit_record_address(), frame, setups);
  m.set_cockpit(r3);
  return r3;
}

}  // namespace car_models

