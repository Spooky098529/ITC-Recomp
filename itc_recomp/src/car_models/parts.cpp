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
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace car_models {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// One end of the rims or brakes: `n` the wheel number, `block` the car's;
// the part's id `part` for a block above 19, else 3 x block +
// `part_block_base`, or 3n + `part_wheel_base` for a wheel.
void WheelEnd(uint8_t* base, uint32_t xpr_at, uint32_t part_at, int32_t n, int32_t block,
              uint32_t part, uint32_t part_block_base, uint32_t part_wheel_base) {
  if (n == -1) {
    if (block > 19) {
      Store(base, part_at, part);
      Store(base, xpr_at, 695);
      return;
    }
    Store(base, xpr_at, 3 * uint32_t(block) + 587);
    Store(base, part_at, 3 * uint32_t(block) + part_block_base);
    return;
  }
  Store(base, xpr_at, 3 * uint32_t(n) + 695);
  Store(base, part_at, 3 * uint32_t(n) + part_wheel_base);
}

}  // namespace

void WheelsRims(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8BA8 Wheels::Rims");
  const int32_t block = int32_t(Load(base, Load(base, wheels + 24) + 32));
  WheelEnd(base, wheels + 2144, wheels + 1168, int32_t(Load(base, wheels + 44)), block, 696,
           588, 696);
  WheelEnd(base, wheels + 2168, wheels + 1192, int32_t(Load(base, wheels + 60)), block, 696,
           588, 696);
}

void WheelsBrakes(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8C50 Wheels::Brakes");
  if (Load(base, wheels + 28) == 3) {
    const uint32_t x = 3 * (Load(base, Load(base, wheels + 3264)) + 20);
    Store(base, wheels + 1168, x + 589);
    Store(base, wheels + 2144, x + 587);
    Store(base, wheels + 1192, x + 589);
    Store(base, wheels + 2168, x + 587);
    return;
  }
  const int32_t block = int32_t(Load(base, Load(base, wheels + 24) + 32));
  WheelEnd(base, wheels + 2144, wheels + 1168, int32_t(Load(base, wheels + 44)), block, 697,
           589, 697);
  WheelEnd(base, wheels + 2168, wheels + 1192, int32_t(Load(base, wheels + 60)), block, 697,
           589, 697);
}

std::optional<uint32_t> WheelsSetUp(uint8_t* base, uint32_t wheels, uint32_t setup,
                                    const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D9110 Wheels::SetUp");
  if (!native::Buffers_Memcpy(base, wheels + 28, setup, 156, 0x820D912C)) {
    return std::nullopt;
  }
  if (Load(base, Load(base, wheels + 3268) + 8) == 0) {
    WheelsRims(base, wheels);
  } else {
    WheelsBrakes(base, wheels);
  }
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t part = wheels + 1168 + 24 * i;
    const uint32_t tire = Load(base, wheels + 144 + 4 * i) == 0 ? 582 : 584;
    LoadSlotLoadShared(base, part - 976, tire, wheels, kOnWheelPartLoaded, load);
    LoadSlotLoadShared(base, part, Load(base, part), wheels, kOnWheelPartLoaded, load);
    LoadSlotLoadShared(base, part + 976, Load(base, part + 976), wheels, kOnWheelPartLoaded,
                       load);
    r3 = LoadSlotLoadShared(base, part + 1112, 887, wheels, kOnWheelPartLoaded, load);
  }
  return r3;
}

std::optional<uint32_t> CockpitLoadSetC(uint8_t* base, uint32_t object, uint32_t setup,
                                        uint32_t stack, uint32_t pcr,
                                        const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D67E8 CarInterior::LoadSetCBody");
  if (!native::Buffers_Memcpy(base, object + 28, setup, 144, 0x820D6808)) {
    return std::nullopt;
  }
  const uint32_t block = Load(base, Load(base, object + 24) + 32);
  const uint32_t n = block * 55;
  if (LoadSlotLoad(base, object + 176, n + 19928, object, kOnCockpitPartLoaded, pcr, load) == 0) {
    // sprintf's call from the original's 1,152-byte frame: the block spilled
    // into the frame's parameter area as sprintf spills r5 (r6 to r10, which
    // the call doesn't set, as 0: only dead frame words differ), its own
    // frames below. Its text, in the frame, is read by nothing.
    const uint32_t frame = stack - 1152;
    for (uint32_t i = 0; i < 6; i++) {
      Store(base, frame + 32 + 8 * i, 0);
      Store(base, frame + 36 + 8 * i, i == 0 ? block : 0);
    }
    int32_t written;
    c_runtime::Sprintf(base, frame + 80, kCockpitNodeFormat, frame + 32, frame - 144 - 1328,
                       &written);
  }
  const uint32_t body = n + 19926;
  const uint32_t buffer = Load(base, Load(base, kSceneSet) + kSharedBuffer);
  Store(base, object + 400, body);
  Store(base, object + 408, archive::AssetSize(base, body));
  const int64_t serial = load(body, buffer, kOnCockpitPartLoaded, object);
  Store(base, object + 404, uint32_t(serial));
  Store(base, object + 420, 2);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    uint32_t id;
    switch (i) {
      case 0: id = Load(base, object + 28) + body + 3; break;
      case 1: id = Load(base, object + 32) + body + 16; break;
      case 2: id = Load(base, object + 36) + body + 19; break;
      case 3: id = Load(base, object + 40) + body + 31; break;
      case 4: id = Load(base, object + 44) + body + 36; break;
      default: id = body + 35; break;
    }
    r3 = LoadSlotLoadShared(base, object + 536 + 24 * i, id, object, kOnCockpitPartLoaded, load);
  }
  return r3;
}

uint32_t InteriorLoadParts(uint8_t* base, uint32_t object, uint32_t first, uint32_t stride,
                           uint32_t offset, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7730 CarInterior::LoadParts");
  const int32_t block = int32_t(Load(base, Load(base, object + 24) + 32));
  const uint32_t id = uint32_t(int64_t(block) * int64_t(int32_t(stride))) + first + offset;
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    r3 = LoadSlotLoadShared(base, object + 160 + 24 * i, id + i, object, kOnInteriorPartLoaded,
                            load);
  }
  return r3;
}

std::optional<uint32_t> InteriorLoad(uint8_t* base, uint32_t object, uint32_t setup,
                                     const archive::LoadCall& load,
                                     const AccessoryLoadCall& accessory_load) {
  ITC_NATIVE_RUNNING("sub_820D79B0 CarInterior::Load");
  const uint32_t accessories = scene_models::Accessories(base);
  if (!native::Buffers_Memcpy(base, object + 28, setup, 120, 0x820D79E0)) {
    return std::nullopt;
  }
  uint32_t r3 = object + 28;  // memcpy's answer
  const uint32_t set = Load(base, Load(base, object + 3092) + 8);
  const bool cockpit = Load(base, object + 3088) != 0;
  if (set == 1) {
    if (cockpit) {
      InteriorLoadParts(base, object, 19926, 55, 49, load);
    } else {
      InteriorLoadParts(base, object, 11061, 197, 190, load);
    }
    r3 = accessory_load(accessories, Load(base, object + 140), Load(base, object + 144), 0);
  } else if (set == 0) {
    if (cockpit) {
      InteriorLoadParts(base, object, 19926, 55, 49, load);
    } else {
      InteriorLoadParts(base, object, 891, 226, 219, load);
    }
    for (uint32_t i = 0; i < 3; i++) {
      r3 = accessory_load(accessories, i, 0, 1);
      Store(base, r3 + 808, 0);
    }
  }
  return r3;
}

// --- The first set's pools' set-ups ---

void CarPartsLoadSetA(uint8_t* base, uint32_t object, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7090 CarParts::LoadSetA");
  uint32_t id = Load(base, Load(base, object + 24) + 32) * 226 + 894;
  uint32_t slot = object + 96;
  for (uint32_t i = 0; i < 216; i++, slot += 24, id++) {
    Store(base, slot, id);
    LoadSlotLoadShared(base, slot, id, object, kOnPartsLoadedA, load);
  }
}

void CarPartsLoadSetB(uint8_t* base, uint32_t object, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D70F0 CarParts::LoadSetB");
  const uint32_t first = Load(base, Load(base, object + 24) + 32) * 197 + 11061;
  const auto field = [&](uint32_t offset) { return Load(base, object + offset); };
  for (uint32_t j = 0; j < 17; j++) {
    uint32_t id;
    switch (j) {
      case 0: id = field(28) + first + 3; break;
      case 1: id = field(28) * 7 + field(32) + first + 16; break;
      case 2: id = field(36) + first + 107; break;
      case 3: id = field(40) + first + 120; break;
      case 4: {
        const uint32_t value = field(44);
        if (int32_t(value) == 13) {
          continue;
        }
        id = value + first + 133;
        break;
      }
      case 5: id = field(48) + first + 146; break;
      case 6: id = field(52) + first + 154; break;
      case 8: id = field(56) + first + 157; break;
      case 10: id = field(60) + first + 169; break;
      case 11: id = field(64) + first + 173; break;
      case 12: id = field(68) + first + 178; break;
      case 13: id = first + 177; break;
      case 15: id = field(76) + first + 184; break;
      case 16: id = first + 189; break;
      default: continue;  // 7, 9 and 14
    }
    LoadSlotLoadShared(base, object + 96 + 24 * j, id, object, kOnPartsLoadedB, load);
  }
}

bool CarPartsLoad(uint8_t* base, uint32_t object, uint32_t setup, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D7510 CarParts::Load");
  if (!native::Buffers_Memcpy(base, object + 28, setup, 60, 0x820D7530)) {
    return false;
  }
  const uint32_t set = Load(base, Load(base, object + 106000) + 8);
  if (set == 0) {
    CarPartsLoadSetA(base, object, load);
  } else if (set == 1) {
    CarPartsLoadSetB(base, object, load);
  }
  return true;
}

bool Pool4Load(uint8_t* base, uint32_t object, uint32_t record, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D6B48 Pool4Object::Load");
  if (!native::Buffers_Memcpy(base, object + 28, record, 52, 0x820D6B68)) {
    return false;
  }
  const uint32_t scene_set = Load(base, object + 576);
  const uint32_t car = Load(base, object + 24);
  const uint32_t set = Load(base, scene_set + 8);
  const uint32_t block = Load(base, car + 32);
  const uint32_t id = set == 0 ? block * 226 + 1116 : block * 197 + 11257;
  LoadSlotLoadShared(base, object + 80, id, object, kOnPool4Loaded, load);
  return true;
}

bool TrafficCarLoad(uint8_t* base, uint32_t object, uint32_t record, uint32_t stack, uint32_t pcr,
                    const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820D8A08 TrafficCar::Load");
  if (!scene_models::CarRecordCopy(base, object + 32, record)) {
    return false;
  }
  const uint32_t frame = stack - 1136;
  const uint32_t number = Load(base, object + 32);
  Store(base, object + 7392, Load(base, 0x826C2E40));
  if (LoadSlotLoad(base, object + 3744, number + 22529, object, kOnTrafficLoaded, pcr, load) ==
      0) {
    // sprintf's call from the original's frame: the number spilled into the
    // frame's parameter area as sprintf spills r5 (r6 to r10, which the
    // call doesn't set, as 0: only dead frame words differ), its own frames
    // below. Its text, in the frame, is read by nothing.
    for (uint32_t i = 0; i < 6; i++) {
      Store(base, frame + 32 + 8 * i, 0);
      Store(base, frame + 36 + 8 * i, i == 0 ? number : 0);
    }
    int32_t written;
    c_runtime::Sprintf(base, frame + 80, kTrafficFormat, frame + 32, frame - 144 - 1328,
                       &written);
  }
  TextureCreate(base, 0x18280086, 16, 16, object + 3728, object + 3732, frame);
  return true;
}

uint32_t ScenePoolsSetUp(uint8_t* base, uint32_t pools, uint32_t pool, uint32_t index,
                         uint32_t record, uint32_t stack, uint32_t pcr,
                         const archive::LoadCall& load, const PoolSetUps& native,
                         const SetUpCall& call) {
  ITC_NATIVE_RUNNING("sub_820C4338 ScenePools::SetUp");
  const uint32_t object = vehicle_accessors::ListAt(base, pool * 12 + pools + 56, index);
  if (int32_t(Load(base, object + 16)) != 3) {
    return 0;
  }
  const uint32_t frame = stack - 112;
  // The set-up through the guest, from the frame (its back chain written).
  const auto guest = [&](uint32_t function, uint32_t lr) {
    Store(base, frame, stack);
    call(function, object, record, frame, lr);
  };
  switch (pool) {
    case 0:
      if (!(native.car_models && native.heap &&
            CarModelLoadExterior(base, object, record, frame, pcr, load))) {
        guest(0x820D5EC8, 0x820C4410);
      }
      break;
    case 1:
      if (!(native.loaders && native.car_models && CarPartsLoad(base, object, record, load))) {
        guest(0x820D7510, 0x820C4400);
      }
      break;
    case 2:
      if (!(native.car_models && native.heap &&
            CockpitLoadSetC(base, object, record, frame, pcr, load))) {
        guest(0x820D67E8, 0x820C43F0);
      }
      break;
    case 3:
      guest(0x820D79B0, 0x820C43E0);
      break;
    case 4:
      if (!(native.loaders && native.car_models && Pool4Load(base, object, record, load))) {
        guest(0x820D6B48, 0x820C43D0);
      }
      break;
    case 5:
      if (!(native.car_models && WheelsSetUp(base, object, record, load))) {
        guest(0x820D9110, 0x820C43C0);
      }
      break;
    case 6:
      if (!(native.loaders && native.car_models && native.heap &&
            TrafficCarLoad(base, object, record, frame, pcr, load))) {
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
uint32_t SetUpFirst(uint8_t* base, uint32_t pools, uint32_t pool, uint32_t index,
                    uint32_t record, uint32_t frame, const SecondSetUps& setups) {
  return ScenePoolsSetUp(
      base, pools, pool, index, record, frame, setups.pcr, setups.load, setups.native,
      [&](uint32_t function, uint32_t r3, uint32_t r4, uint32_t r1, uint32_t lr) {
        return setups.guest(function, r3, r4, 0, r1, lr);
      });
}

// The claim and set-up the parts', pool 4's and the interior's set-ups
// share: pool `pool` claimed, its object into `at`, its +24 = the car
// model's, set up with `copy`, the result into `at`.
uint32_t ClaimLinkSetUp(uint8_t* base, uint32_t object, uint32_t pool, uint32_t at,
                        uint32_t copy, uint32_t frame, const SecondSetUps& setups) {
  namespace va = vehicle_accessors;
  const uint32_t pools = Load(base, kFirstSet);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(base, pools, pool));
  const uint32_t first = va::ScenePoolsGetA(base, pools, pool, index);
  Store(base, object + at, first);
  Store(base, first + 24, Load(base, Load(base, object + 24) + 24));
  const uint32_t r3 = SetUpFirst(base, pools, pool, index, copy, frame, setups);
  Store(base, object + at, r3);
  return r3;
}

}  // namespace

std::optional<uint32_t> SceneCarModelSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                           uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DB818 SceneCarModel::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t frame = stack - 112;
  const uint32_t pools = Load(base, kFirstSet);
  Store(base, object + 3728, Load(base, kSceneSet));
  if (!scene_models::CarRecordCopy(base, object + 32, record)) {
    return std::nullopt;
  }
  Store(base, frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(base, pools, 0));
  const uint32_t r3 = SetUpFirst(base, pools, 0, index, object + 32, frame, setups);
  Store(base, object + 24, r3);
  return r3;
}

std::optional<uint32_t> ScenePartsSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DF1D0 SceneParts::SetUp");
  if (!native::Buffers_Memcpy(base, object + 108, record, 60, 0x820DF1F8)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(base, frame, stack);
  return ClaimLinkSetUp(base, object, 1, 104, object + 108, frame, setups);
}

std::optional<uint32_t> ScenePool4SetUp(uint8_t* base, uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820DCAA0 ScenePool4::SetUp");
  if (!native::Buffers_Memcpy(base, object + 32, record, 52, 0x820DCAC8)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(base, frame, stack);
  return ClaimLinkSetUp(base, object, 4, 28, object + 32, frame, setups);
}

std::optional<uint32_t> SceneInteriorSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                           uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820E37F0 SceneInterior::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t pools = Load(base, kFirstSet);
  Store(base, object + 11668, Load(base, kSceneSet));
  if (!native::Buffers_Memcpy(base, object + 40, record, 120, 0x820E3824)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(base, frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(base, pools, 3));
  const uint32_t first = va::ScenePoolsGetA(base, pools, 3, index);
  Store(base, object + 36, first);
  if (Load(base, object + 11656) != 0) {
    Store(base, first + 3088, 1);
  }
  Store(base, Load(base, object + 36) + 24, Load(base, Load(base, object + 24) + 24));
  const uint32_t r3 = SetUpFirst(base, pools, 3, index, object + 40, frame, setups);
  Store(base, object + 36, r3);
  return r3;
}

std::optional<uint32_t> SceneTrafficSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                          uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820E5998 SceneTraffic::SetUp");
  namespace va = vehicle_accessors;
  const uint32_t pools = Load(base, kFirstSet);
  Store(base, object + 9796, Load(base, kSceneSet));
  if (!native::Buffers_Memcpy(base, object + 32, record, 3696, 0x820E59CC)) {
    return std::nullopt;
  }
  const uint32_t frame = stack - 128;
  Store(base, frame, stack);
  const uint32_t index = uint32_t(va::ScenePoolsClaimA(base, pools, 6));
  Store(base, object + 9760, va::ScenePoolsGetA(base, pools, 6, index));
  return SetUpFirst(base, pools, 6, index, record, frame, setups);
}

std::optional<uint32_t> SceneWheelsSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                         uint32_t stack, const SecondSetUps& setups) {
  ITC_NATIVE_RUNNING("sub_820E70C0 SceneWheels::SetUp");
  namespace va = vehicle_accessors;
  constexpr uint32_t kTable = 0x820044B0;
  const uint32_t pools = Load(base, kFirstSet);
  const uint32_t copy = object + 32;
  if (!native::Buffers_Memcpy(base, copy, record, 156, 0x820E70EC)) {
    return std::nullopt;
  }
  // lfs and stfs: through a double, as the original.
  const auto copy_float = [&](uint32_t to, uint32_t from) {
    const double d = double(std::bit_cast<float>(Load(base, from)));
    Store(base, to, std::bit_cast<uint32_t>(float(d)));
  };
  const bool alternate = Load(base, object + 100) != 0;
  uint32_t index;
  if (!alternate && int32_t(Load(base, copy)) == 3) {
    index = Load(base, Load(base, object + 16776)) + 45;
  } else {
    index = Load(base, Load(base, Load(base, object + 24) + 24) + 32);
  }
  const uint32_t entry = kTable + index * 44;
  Store(base, object + 112, Load(base, entry + 4));
  Store(base, object + 116, Load(base, entry + 8));
  if (!alternate) {
    Store(base, object + 120, Load(base, entry + 12));
  }
  Store(base, object + 124, Load(base, entry + 16));
  Store(base, object + 128, Load(base, entry + 20));
  copy_float(object + 132, entry + 24);
  copy_float(object + 136, entry + 28);
  Store(base, object + 140, Load(base, entry + 32));
  Store(base, object + 144, Load(base, entry + 36));
  Store(base, object + 148, Load(base, entry + 40));
  Store(base, object + 152, Load(base, entry + 40));
  const uint32_t frame = stack - 160;
  Store(base, frame, stack);
  if (!native::Buffers_Memcpy(base, record, copy, 156, 0x820E7238)) {
    setups.guest(0x825A7410, record, copy, 156, frame, 0x820E7238);
  }
  const uint32_t front = object + 48, rear = object + 64;
  if (int32_t(Load(base, front)) == -1 && int32_t(Load(base, rear)) == -1) {
    const uint32_t car = Load(base, object + 24);
    if (car != 0) {
      const uint32_t block = Load(base, Load(base, car + 24) + 32);
      if (int32_t(block) >= 18) {
        setups.guest(0x820D39D8, block, front, rear, frame, 0x820E727C);
      }
    }
  }
  if (int32_t(Load(base, front)) >= 0) {
    Store(base, object + 148, Load(base, front) == 190 ? 0 : 1);
  }
  if (int32_t(Load(base, rear)) >= 0) {
    Store(base, object + 152, Load(base, rear) == 190 ? 0 : 1);
  }
  const uint32_t claimed = uint32_t(va::ScenePoolsClaimA(base, pools, 5));
  const uint32_t wheels = va::ScenePoolsGetA(base, pools, 5, claimed);
  Store(base, object + 28, wheels);
  const uint32_t car = Load(base, object + 24);
  if (car != 0) {
    Store(base, wheels + 24, Load(base, car + 24));
  } else {
    Store(base, wheels + 3264, Load(base, object + 16776));
  }
  return SetUpFirst(base, pools, 5, claimed, copy, frame, setups);
}

}  // namespace car_models

