// The scene models' load poll (see load_poll.h), store for store as the
// recompiled functions.

#include "load_poll/load_poll.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "models/part_model.h"
#include "car_models/car_model.h"
#include "native/buffers.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/load_poll.h"
#include "schema/material_passes.h"
#include "schema/models.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
#include "schema/stand_models.h"
#include "scene_loaders/file_loaded.h"
#include "scene_loaders/pools.h"
#include "second_set/blocks.h"

namespace load_poll {

namespace {

namespace mp = material_passes;
namespace sl = scene_loaders;
namespace ss = second_set;

using schema::CarModel;
using schema::CarParts;
using schema::CarWheels;
using schema::Cockpit;
using schema::InteriorDials;
using schema::InteriorMeters;
using schema::LightKitRecord;
using schema::LoadPollGlobals;
using schema::Pool4Object;
using schema::SceneCockpit;
using schema::SceneInterior;
using schema::SceneLightKit;
using schema::SceneModel;
using schema::SceneParts;
using schema::SceneTraffic;
using schema::SceneWheels;
using schema::SecondSetInteriorPoints;
using schema::TrafficCar;

// A part model's Model words (models.toml's Model: +48 its texture provider)
// and its own (material-passes.toml's PartModel: +452 its category 3 texture).
schema::Model ModelAt(uint8_t* base, uint32_t model) {
  return schema::Model(base, model);
}
schema::PartModel PartModelAt(uint8_t* base, uint32_t model) {
  return schema::PartModel(base, model);
}

// [0x826C2E44] the scene's address.
uint32_t Scene(uint8_t* base) {
  return LoadPollGlobals::scene(base).address;
}

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
int32_t LoadS(uint8_t* base, uint32_t address) {
  return int32_t(guest::LoadU32(base, address));
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t sp = stack - bytes;
  Store(base, sp, stack);
  return sp;
}

// memcpy from the frame at `sp` (the call's return address `lr`): natively,
// else its original.
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr,
          Calls& calls) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

// The flush mode after a point set-up: its last float step's.
void AfterFind(Calls& calls) {
  calls.flush = calls.find.flush ? ss::Flush::kOn : ss::Flush::kOff;
}
void AfterWheels(Calls& calls) {
  if (calls.wheel.flush != ss::Flush::kUnchanged) {
    calls.flush = calls.wheel.flush;
  }
}

// sub_820D3658 (never seen in play; only InteriorMeters::SetGauge calls it):
// 0x82003600 + 16 x (2 x index + (value >= 3)), as the original's 64-bit
// sum with the base sign-extended.
uint64_t GaugeTableEntry(uint64_t index, int32_t value) {
  ITC_NATIVE_RUNNING("sub_820D3658 GaugeTableEntry");
  const uint64_t r9 = value >= 3 ? 1 : 0;
  const uint64_t r10 = uint64_t(uint32_t(uint32_t(index) << 1)) + r9;
  return uint64_t(uint32_t(uint32_t(r10) << 4)) + uint64_t(int64_t(-2113929216) + 13824);
}

}  // namespace

// --- The first set's wheels ---

uint64_t WheelsSetRecord(uint8_t* base, uint32_t wheels, uint32_t record, uint32_t stack,
                         Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D8EC0 Wheels::SetRecord");
  const uint32_t to = CarWheels(base, wheels).setup_address();
  // A tail call: memcpy from the caller's frame, its return address the
  // caller's (Wheels::SetRecord's own entry stands for it in memcpy's logs).
  Copy(base, to, record, 156, stack, 0x820D8EC0, calls);
  return uint64_t(wheels) + 28;
}

uint64_t WheelsSetCaliperTexture(uint8_t* base, uint32_t wheels, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D8ED0 Wheels::SetCaliperTexture");
  const uint32_t sp = Frame(base, stack, 96);
  const CarWheels w(base, wheels);
  mp::SetTexture(base, w.front_caliper_model_address(), w.caliper_texture(), -1, sp);
  return mp::SetTexture(base, w.rear_caliper_model_address(), w.caliper_texture(), -1, sp);
}

// --- The car model ---

uint64_t CarModelIsReady(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4048 CarModel::IsReady");
  const uint32_t set = CarModel(base, car_model).first_set().model_set();
  if (set > 1) {
    return 0;
  }
  return sl::CarModelIsLoaded(base, car_model);
}

uint64_t CarPartsSetVinylTextures(uint8_t* base, uint32_t parts, uint64_t r3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D72D8 CarParts::SetVinylTextures");
  Frame(base, stack, 128);
  const CarParts cp(base, parts);
  const uint32_t set = cp.first_set().model_set();
  const int32_t count = set < 1 ? 216 : (set == 1 ? 187 : 0);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t model = cp.part_models_address() + 464 * uint32_t(i);
    if (int32_t(cp.first_set().model_set()) == 1 && (i == 7 || i == 9 || i == 14)) {
      continue;
    }
    ModelAt(base, model).set_texture_provider(cp.car_model().texture_providers_address());
    const CarModel car = cp.car_model();
    const uint32_t b = car.unknown_6692();
    const uint32_t a = car.vinyl_sheet();
    models::PartModelSetVinylTextures(base, model, a, b);
    r3 = model;
  }
  return r3;
}

// --- The interior ---

uint64_t InteriorDialsSetBlock(uint8_t* base, uint32_t dials, uint64_t block) {
  ITC_NATIVE_RUNNING("sub_820E3040 InteriorDials::SetBlock");
  const InteriorDials d(base, dials);
  d.set_car_block(uint32_t(block));
  d.set_gauge_record(uint32_t(mp::RecordAt(block)));
  if (mp::IsBlockSetB(int32_t(d.car_block())) != 0) {
    d.set_digit_provider(Scene(base) + 1132);
    return 1;
  }
  if (mp::IsBlockSetA(int32_t(d.car_block())) != 0) {
    d.set_digit_provider(Scene(base) + 1064);
    return 1;
  }
  return 0;
}

uint64_t InteriorMetersSetGauge(uint8_t* base, uint32_t meters, uint64_t gauge, uint64_t entry,
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E32E8 InteriorMeters::SetGauge");
  const uint32_t sp = Frame(base, stack, 112);
  const InteriorMeters m(base, meters);
  Store(base, m.gauge0_provider_address() + (uint32_t(gauge) << 2), uint32_t(entry));
  // The gauge's point record: its part model and shape.
  using Word = uint32_t (InteriorMeters::*)() const;
  static constexpr Word kModels[7] = {
      &InteriorMeters::needle0_model, &InteriorMeters::needle1_model,
      &InteriorMeters::needle2_model, &InteriorMeters::needle3_model,
      &InteriorMeters::needle4_model, &InteriorMeters::needle5_model,
      &InteriorMeters::needle6_model};
  static constexpr Word kShapes[7] = {
      &InteriorMeters::needle0_shape, &InteriorMeters::needle1_shape,
      &InteriorMeters::needle2_shape, &InteriorMeters::needle3_shape,
      &InteriorMeters::needle4_shape, &InteriorMeters::needle5_shape,
      &InteriorMeters::needle6_shape};
  uint32_t model = 0;
  uint32_t shape = 0;
  if (uint32_t(gauge) < 7) {
    model = (m.*kModels[uint32_t(gauge)])();
    shape = (m.*kShapes[uint32_t(gauge)])();
  }
  const uint64_t offset = gauge * 12;
  const int32_t scene_flag = int32_t(LoadPollGlobals::scene(base).model_set());
  // The record's gauge entries (+8 + 12i): +12 the kind, +16 the texture index.
  int32_t index = LoadS(base, uint32_t(offset + m.record().unknown_16_address()));
  if (scene_flag != 0 && index > 2) {
    index = 3;
  }
  mp::SetTexture(base, model, uint32_t(index), int32_t(shape), sp);
  const schema::InteriorRecord record(base, uint32_t(offset + m.record().address));
  const int32_t value = int32_t(record.unknown_16());
  const uint64_t r3 = GaugeTableEntry(record.gauge0_kind(), value);
  Store(base, m.gauge0_angle_address() + (uint32_t(gauge) << 2), uint32_t(r3));
  return r3;
}

uint64_t SceneInteriorSetGauges(uint8_t* base, uint32_t interior, uint64_t r3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3938 SceneInterior::SetGauges");
  const uint32_t sp = Frame(base, stack, 112);
  const SceneInterior si(base, interior);
  const uint32_t meters = SecondSetInteriorPoints(base, si.points_address()).meters_address();
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t block = si.car_model().car_model().block();
    if (block == 5 && i == 1) {
      continue;
    }
    // The record's gauge i (+8 + 12i): present, then its kind.
    if (LoadS(base, si.gauge0_present_address() + 12 * i) == 0) {
      continue;
    }
    const uint64_t word = Load(base, si.gauge0_kind_address() + 12 * i);
    const uint64_t scene = si.scene().address;
    r3 = InteriorMetersSetGauge(base, meters, i, scene + word * 20 + 4864, sp);
  }
  return r3;
}

// --- The light kit and the traffic body ---

uint64_t SceneLightsFinish(uint8_t* base, uint32_t kit, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DDA60 SceneLights::Finish");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneLightKit k(base, kit);
  if (int32_t(k.finished()) != 0) {
    return 1;
  }
  const uint32_t loaded = sl::Pool4IsLoaded(base, k.first_set_object().address);
  if (loaded == 0) {
    return 0;
  }
  const Pool4Object object = k.first_set_object();
  const uint32_t texture = LightKitRecord(base, k.record_address()).texture();
  const uint32_t model = object.model_address();
  k.set_finished(1);
  k.set_model(model);
  mp::SetTexture(base, model, texture, -1, sp);
  const uint32_t scene = k.scene().address;
  ModelAt(base, k.model()).set_texture_provider(scene + 364);
  ss::LightsFind(base, k.points_address(), k.model(), sp, calls.find);
  AfterFind(calls);
  return 1;
}

uint64_t SceneLightsSetRecord(uint8_t* base, uint32_t kit, uint32_t record, uint32_t stack,
                              Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DCB70 SceneLights::SetRecord");
  const uint32_t sp = Frame(base, stack, 112);
  const SceneLightKit k(base, kit);
  Copy(base, k.record_address(), record, 52, sp, 0x820DCB98, calls);
  const uint32_t object = k.first_set_object().address;
  if (!sl::Pool4SetRecord(base, object, record)) {
    // Pool4Object::SetRecord's tail call to memcpy, from this frame.
    calls.memcpy(Pool4Object(base, object).record_address(), record, 52, sp, 0x820DCBA4);
  }
  return uint64_t(object) + 28;
}

uint64_t SceneLightsApplyRecord(uint8_t* base, uint32_t kit, uint32_t record, uint32_t stack,
                                Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DCBC0 SceneLights::ApplyRecord");
  const uint32_t sp = Frame(base, stack, 96);
  // The point block's four blink counters (LightPoints +8 to +32).
  const schema::LightPoints points(base, SceneLightKit(base, kit).points_address());
  points.set_blink_counter(0);
  points.set_front_counter(0);
  points.set_sides_counter(0);
  points.set_rear_counter(0);
  SceneLightsSetRecord(base, kit, record, sp, calls);
  const SceneLightKit k(base, kit);
  const uint32_t texture = LightKitRecord(base, k.record_address()).texture();
  return mp::SetTexture(base, k.model(), texture, -1, sp);
}

uint64_t SceneTrafficFinish(uint8_t* base, uint32_t body, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E5A68 SceneTraffic::Finish");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneTraffic t(base, body);
  if (int32_t(t.finished()) != 0) {
    return 1;
  }
  const uint32_t traffic = t.body();
  uint64_t bound;
  if (const auto r3 = sl::TrafficBindTextures(base, traffic, sp)) {
    bound = *r3;
  } else {
    bound = calls.traffic_bind(traffic, sp, 0x820E5A90);
  }
  if (int32_t(bound) == 0) {
    return bound;
  }
  t.set_skeleton(TrafficCar(base, t.body()).skeleton_address());
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, t.part_model_0_address() + 4 * i,
          TrafficCar(base, t.body()).part_models_address() + 464 * i);
  }
  const TrafficCar car(base, t.body());
  const uint32_t second = car.texture2_word2();
  const uint32_t first = car.texture2_word0();
  t.set_points_rear_plate_texture(second);
  t.set_points_front_plate_texture(first);
  ss::TrafficFind(base, t.points_address(), t.part_model_0_address(), sp, calls.find);
  AfterFind(calls);
  return 1;
}

// --- The paint ---

uint64_t ScenePartsApplyPaint(uint8_t* base, uint32_t parts, const uint64_t regs[6],
                              uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0908 SceneParts::ApplyPaint");
  const uint32_t first = SceneParts(base, parts).first_set_parts();
  return sl::PartsSetPaint(base, first, first, regs, stack);
}

uint64_t SceneInteriorApplyPaint(uint8_t* base, uint32_t interior, const uint64_t regs[6],
                                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4250 SceneInterior::ApplyPaint");
  const uint32_t first = SceneInterior(base, interior).first_set_interior().address;
  return sl::InteriorSetPaint(base, first, first, regs, stack);
}

uint64_t SceneCockpitApplyPaint(uint8_t* base, uint32_t cockpit, const uint64_t regs[6],
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC138 SceneCockpit::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 144);
  const SceneCockpit c(base, cockpit);
  const uint32_t body = c.body();
  sl::CockpitSetPaint(base, body, body, regs, sp);
  return SceneInteriorApplyPaint(base, c.interior().address, regs, sp);
}

uint64_t TrafficCarSetPaint(uint8_t* base, uint32_t traffic, uint64_t r3, const uint64_t regs[6],
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D7E18 TrafficCar::SetPaint");
  const uint32_t sp = Frame(base, stack, 160);
  const TrafficCar t(base, traffic);
  for (uint32_t i = 0; i < 6; i++) {
    if (LoadS(base, t.slot0_state_address() + 24 * i) == 3) {
      r3 = mp::SetPaint(base, t.part_models_address() + 464 * i, regs, sp);
    }
  }
  return r3;
}

uint64_t SceneTrafficApplyPaint(uint8_t* base, uint32_t body, const uint64_t regs[6],
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E61D0 SceneTraffic::ApplyPaint");
  const uint32_t traffic = SceneTraffic(base, body).body();
  return TrafficCarSetPaint(base, traffic, traffic, regs, stack);
}

uint64_t SceneModelApplyPaint(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9B68 SceneModel::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneModel m(base, model);
  if (int32_t(m.ready()) == 0) {
    return r3;
  }
  // The record's three colors, read again before each call.
  auto colors = [&](uint64_t regs[6]) {
    for (uint32_t i = 0; i < 6; i++) {
      regs[i] = Load64(base, m.paint_address() + 8 * i);
    }
  };
  uint64_t regs[6];
  if (int32_t(m.kind()) == 3) {
    colors(regs);
    return SceneTrafficApplyPaint(base, m.traffic_body(), regs, sp);
  }
  colors(regs);
  r3 = ScenePartsApplyPaint(base, m.parts().address, regs, sp);
  colors(regs);
  r3 = SceneInteriorApplyPaint(base, m.interior(), regs, sp);
  const uint32_t cockpit = m.cockpit();
  if (cockpit == 0) {
    return r3;
  }
  car_models::CarModelSetVinylTextures(base, SceneCockpit(base, cockpit).body());
  colors(regs);
  return SceneCockpitApplyPaint(base, m.cockpit(), regs, sp);
}

uint64_t WheelsApplyPaint(uint8_t* base, uint32_t wheels, const uint64_t regs[7],
                          uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D91E0 Wheels::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 192);
  const uint64_t v0[2] = {regs[0], regs[1]};
  const uint64_t v1[2] = {regs[2], regs[3]};
  const uint64_t v2[2] = {regs[4], regs[5]};
  auto paint = [&](uint32_t model, const uint64_t* a, const uint64_t* b, const uint64_t* c) {
    const uint64_t r[6] = {a[0], a[1], b[0], b[1], c[0], c[1]};
    return mp::SetPaintBody(base, model, r, sp);
  };
  const CarWheels w(base, wheels);
  paint(w.front_tire_model_address(), v1, v0, v2);
  // v3 (r10 and the frame's +88), v4 (+96) and v5 (+112), read after the
  // first call as the original does.
  const uint64_t v5[2] = {Load64(base, stack + 112), Load64(base, stack + 120)};
  const uint64_t v3[2] = {regs[6], Load64(base, stack + 88)};
  const uint64_t v4[2] = {Load64(base, stack + 96), Load64(base, stack + 104)};
  paint(w.rear_tire_model_address(), v4, v3, v5);
  paint(w.front_rim_model_address(), v0, v1, v2);
  return paint(w.rear_rim_model_address(), v3, v4, v5);
}

uint64_t SceneWheelsApplyPaint(uint8_t* base, uint32_t object, const uint64_t regs[7],
                               uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8A00 SceneWheels::ApplyPaint");
  const uint32_t sp = Frame(base, stack, 208);
  Copy(base, sp + 112, sp + 320, 16, sp, 0x820E8A3C, calls);
  Copy(base, sp + 96, sp + 304, 16, sp, 0x820E8A4C, calls);
  Store64(base, sp + 88, Load64(base, sp + 296));
  return WheelsApplyPaint(base, SceneWheels(base, object).first_set_wheels(), regs, sp);
}

uint64_t SceneModelApplyWheelPaint(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack,
                                   Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D9C50 SceneModel::ApplyWheelPaint");
  const uint32_t sp = Frame(base, stack, 160);
  const SceneModel m(base, model);
  if (int32_t(m.ready()) == 0 || int32_t(m.kind()) == 3 || int32_t(m.wheels_painted()) == 0) {
    return r3;
  }
  const uint32_t wheels = m.wheels();
  if (wheels == 0) {
    return r3;
  }
  // The six wheel rows (+112): r4 to r10 the first 56 bytes, the frame the
  // rest.
  const uint32_t rows = m.wheel_paint_address();
  Copy(base, sp + 112, rows + 80, 16, sp, 0x820D9CA8, calls);
  Copy(base, sp + 96, rows + 64, 16, sp, 0x820D9CB8, calls);
  const uint64_t last = Load64(base, rows + 56);
  uint64_t regs[7];
  for (uint32_t i = 0; i < 7; i++) {
    regs[i] = Load64(base, rows + 8 * i);
  }
  Store64(base, sp + 88, last);
  return SceneWheelsApplyPaint(base, wheels, regs, sp, calls);
}

// --- The finishers' builds ---

uint64_t SceneInteriorFinish(uint8_t* base, uint32_t interior, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E4140 SceneInterior::Finish");
  const uint32_t sp = Frame(base, stack, 128);
  const SceneInterior si(base, interior);
  if (int32_t(si.finished()) != 0) {
    return 1;
  }
  const uint64_t loaded = sl::InteriorIsLoaded(base, si.first_set_interior().address);
  if (int32_t(loaded) == 0) {
    return loaded;
  }
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = si.first_set_interior().part_models_address() + 464 * i;
    const uint32_t slot = si.part_model_0_address() + 4 * i;
    Store(base, slot, model);
    if (int32_t(model) == 0) {
      continue;
    }
    ModelAt(base, model).set_texture_provider(si.texture_set());
    if (int32_t(si.cockpit()) != 0) {
      const uint32_t at = Load(base, slot);
      PartModelAt(base, at).set_category3_texture(si.cockpit_texture());
    } else {
      const uint64_t texture =
          sl::CarModelSelectedTexture(base, si.car_model().car_model().address);
      PartModelAt(base, Load(base, slot)).set_category3_texture(uint32_t(texture));
    }
  }
  const uint32_t points = si.points_address();
  const uint32_t block = si.car_model().car_model().block();
  InteriorDialsSetBlock(base, SecondSetInteriorPoints(base, points).dials_address(), block);
  si.set_meters_car_block(block);
  const uint32_t provider = si.texture_set();
  const auto scene_model = si.car_model();
  si.set_meters_turbo_provider(provider);
  const CarModel car = scene_model.car_model();
  si.set_points_record(si.record_address());
  si.set_points_seat_position(car.seat_positions_address());
  si.set_points_skeleton(scene_model.car_model().skeleton_address());
  ss::InteriorFind(base, points, si.part_model_0_address(), sp, calls.find);
  AfterFind(calls);
  SceneInteriorSetGauges(base, interior, interior, sp);
  si.set_finished(1);
  return loaded;
}

uint64_t SceneWheelsSetUpPoints(uint8_t* base, uint32_t object, uint64_t r3, uint32_t stack,
                                Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8908 SceneWheels::SetUpPoints");
  const uint32_t sp = Frame(base, stack, 112);
  const SceneWheels o(base, object);
  if (int32_t(o.points_set_up()) != 0) {
    return r3;
  }
  const uint32_t car = o.car_model();
  if (car != 0) {
    o.set_points_car_model(car);
  } else {
    const uint32_t record = o.traffic_record();
    const uint32_t tires = o.tire_positions();
    o.set_points_car_record(record);
    o.set_points_matrix(tires);
  }
  const CarWheels wheels(base, o.first_set_wheels());
  o.set_points_record(o.record_address());
  o.set_points_tire_model(wheels.front_tire_model_address());
  o.set_points_rim_model(wheels.front_rim_model_address());
  o.set_points_brake_model(wheels.front_caliper_model_address());
  ss::WheelPointsSetUp(base, o.points_address(), sp, calls.wheel);
  AfterWheels(calls);
  WheelsSetRecord(base, o.first_set_wheels(), o.record_address(), sp, calls);
  const uint64_t left = WheelsSetCaliperTexture(base, o.first_set_wheels(), sp);
  o.set_points_set_up(1);
  return left;
}

uint64_t ScenePartsSetSelectedTexture(uint8_t* base, uint32_t parts, uint64_t r3,
                                      uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF250 SceneParts::SetSelectedTexture");
  Frame(base, stack, 112);
  const SceneParts p(base, parts);
  for (uint32_t k = 0; k < 17; k++) {
    const uint32_t slot = p.part_model_0_address() + 4 * k;
    if (Load(base, slot) == 0) {
      continue;
    }
    r3 = sl::CarModelSelectedTexture(base, p.scene_car_model().car_model().address);
    PartModelAt(base, Load(base, slot)).set_category3_texture(uint32_t(r3));
  }
  return r3;
}

uint64_t ScenePartsBuild(uint8_t* base, uint32_t parts, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E05E0 SceneParts::Build");
  const uint32_t sp = Frame(base, stack, 112);
  const SceneParts p(base, parts);
  // Each kind's record words (r5, r6): a word of the record, or none for
  // the constant 0.
  using Word = uint32_t (SceneParts::*)() const;
  struct Words {
    Word a, b;
  };
  static constexpr Words kWords[17] = {
      {&SceneParts::choice_0, nullptr},
      {&SceneParts::choice_1, &SceneParts::choice_0},
      {&SceneParts::choice_2, nullptr},
      {&SceneParts::choice_3, nullptr},
      {&SceneParts::choice_4, nullptr},
      {&SceneParts::choice_5, nullptr},
      {&SceneParts::choice_6, nullptr},
      {&SceneParts::choice_6, nullptr},
      {&SceneParts::choice_8, &SceneParts::choice_6},
      {&SceneParts::choice_8, &SceneParts::choice_6},
      {&SceneParts::choice_10, nullptr},
      {&SceneParts::choice_11, nullptr},
      {&SceneParts::choice_12, nullptr},
      {nullptr, nullptr},
      {&SceneParts::choice_14, nullptr},
      {&SceneParts::choice_15, nullptr},
      {nullptr, nullptr}};
  for (uint32_t k = 0; k < 17; k++) {
    const uint32_t a = kWords[k].a == nullptr ? 0 : (p.*kWords[k].a)();
    const uint32_t b = kWords[k].b == nullptr ? 0 : (p.*kWords[k].b)();
    if (k == 4 && int32_t(a) == 13) {
      p.set_part_model_4(0);
      continue;
    }
    const uint32_t model = sl::PartsPart(base, p.first_set_parts(), int32_t(k), a, b);
    Store(base, p.part_model_0_address() + 4 * k, model);
  }
  const uint32_t points = p.points_address();
  ss::PartsPointsReset(base, points, sp);
  const auto scene_model = p.scene_car_model();
  p.set_points_scene_car_model(scene_model.address);
  const CarModel car = scene_model.car_model();
  p.set_points_record(p.record_address());
  p.set_points_car_record(car.record_address());
  p.set_points_skeleton(scene_model.car_model().skeleton_address());
  ss::PartsFind(base, points, p.part_model_0_address(), sp, calls.find);
  AfterFind(calls);
  sl::PartsWalkMaterials(base, p.first_set_parts());
  return ScenePartsSetSelectedTexture(base, parts, parts, sp);
}

// --- The wheels' materials and the last finishers ---

namespace {

constexpr uint32_t kOne = LoadPollGlobals::one_address();    // 1.0
constexpr uint32_t kZero = LoadPollGlobals::zero_address();  // 0.0

// A float word copied with lfs and stfs, as the original does.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  camera_modes::steps::Stfs(base, to, camera_modes::steps::Lfs(base, from));
}

// PartModel::SetMaterials from the frame at `sp`: its plain C++, else its
// original. Returns its r3.
uint64_t SetMaterialsFrom(uint8_t* base, uint32_t model, const uint64_t regs[7], uint32_t sp,
                          uint32_t lr, Calls& calls) {
  std::optional<uint64_t> r3;
  if (!mp::SetMaterials(base, model, regs, sp, &r3)) {
    return calls.set_materials(model, regs, sp, lr);
  }
  return r3 ? *r3 : model;
}

// The frame's words SetMaterials takes in registers: r4 to r6 the flags
// (+160, +168, +176), r7 to r10 the first two float4s (+192 to +223).
void MaterialRegs(uint8_t* base, uint32_t sp, uint64_t regs[7]) {
  regs[0] = Load64(base, sp + 160);
  regs[1] = Load64(base, sp + 168);
  regs[2] = Load64(base, sp + 176);
  regs[3] = Load64(base, sp + 192);
  regs[4] = Load64(base, sp + 200);
  regs[5] = Load64(base, sp + 208);
  regs[6] = Load64(base, sp + 216);
}

// The shape, category and shared pair (+132, +140, +148) and the third
// float4's half and the 24 bytes (+224 copied to +96), before each call.
void MaterialArgs(uint8_t* base, uint32_t sp, uint32_t category, uint32_t lr, Calls& calls) {
  Store(base, sp + 148, 0xFFFFFFFFu);
  Store(base, sp + 140, category);
  Store(base, sp + 132, 0xFFFFFFFFu);
  Copy(base, sp + 96, sp + 224, 32, sp, lr, calls);
}

}  // namespace

uint64_t WheelsBindTextures(uint8_t* base, uint32_t wheels, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D9638 Wheels::BindTextures");
  const uint32_t sp = Frame(base, stack, 352);
  const CarWheels w(base, wheels);
  if (int32_t(w.rear_caliper_state()) == 2) {
    return 0;
  }
  const uint32_t texture = w.front_tire_texture();
  w.set_front_tire_model_provider(Scene(base) + 208);
  const uint32_t scene = Scene(base);
  // The wheels' own two texture providers (20 bytes each).
  w.set_front_rim_model_provider(w.texture_providers_address());
  w.set_rear_rim_model_provider(w.texture_providers_address() + 20);
  w.set_rear_tire_model_provider(scene + 208);
  w.set_front_caliper_model_provider(Scene(base) + 296);
  w.set_rear_caliper_model_provider(Scene(base) + 296);
  mp::SetTexture(base, w.front_tire_model_address(), texture, -1, sp);
  mp::SetTexture(base, w.rear_tire_model_address(), w.rear_tire_texture(), -1, sp);
  CopyFloat(base, sp + 204, kOne);
  Store(base, sp + 148, 0xFFFFFFFFu);
  CopyFloat(base, sp + 220, kOne);
  Store(base, sp + 140, 0xFFFFFFFFu);
  CopyFloat(base, sp + 236, kOne);
  Store(base, sp + 132, 0xFFFFFFFFu);
  CopyFloat(base, sp + 244, kZero);
  for (uint32_t at : {180u, 172u, 168u, 164u, 160u}) {
    Store(base, sp + at, 0);
  }
  Store(base, sp + 176, 1);
  Copy(base, sp + 96, sp + 224, 32, sp, 0x820D9728, calls);
  uint64_t regs[7];
  MaterialRegs(base, sp, regs);
  SetMaterialsFrom(base, w.front_tire_model_address(), regs, sp, 0x820D9768, calls);
  MaterialArgs(base, sp, 0xFFFFFFFFu, 0x820D9784, calls);
  SetMaterialsFrom(base, w.rear_tire_model_address(), regs, sp, 0x820D97A8, calls);
  return 1;
}

uint64_t SceneWheelsFinish(uint8_t* base, uint32_t object, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E7328 SceneWheels::Finish");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneWheels o(base, object);
  if (int32_t(o.textures_bound()) != 0) {
    return 1;
  }
  const uint64_t bound = WheelsBindTextures(base, o.first_set_wheels(), sp, calls);
  if (int32_t(bound) == 0) {
    return bound;
  }
  o.set_textures_bound(1);
  return 1;
}

uint64_t WheelsApplyPalette(uint8_t* base, uint32_t wheels, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D92A0 Wheels::ApplyPalette");
  const uint32_t sp = Frame(base, stack, 384);
  const CarWheels w(base, wheels);
  CopyFloat(base, sp + 204, kOne);
  CopyFloat(base, sp + 220, kOne);
  CopyFloat(base, sp + 236, kOne);
  for (uint32_t at : {180u, 168u, 164u, 160u}) {
    Store(base, sp + at, 0);
  }
  for (uint32_t m : {w.front_tire_model_address(), w.rear_tire_model_address(),
                     w.front_rim_model_address(), w.rear_rim_model_address(),
                     w.front_caliper_model_address(), w.rear_caliper_model_address()}) {
    mp::ApplyPalette(base, m, sp);
  }
  CopyFloat(base, sp + 240, w.unknown_48_address());
  CopyFloat(base, sp + 244, w.unknown_52_address());
  Store(base, sp + 176, 1);
  Store(base, sp + 148, 0xFFFFFFFFu);
  Store(base, sp + 140, 1);
  Store(base, sp + 172, 1);
  Store(base, sp + 132, 0xFFFFFFFFu);
  Copy(base, sp + 96, sp + 224, 32, sp, 0x820D9354, calls);
  uint64_t regs[7];
  MaterialRegs(base, sp, regs);
  SetMaterialsFrom(base, w.front_tire_model_address(), regs, sp, 0x820D9394, calls);
  MaterialArgs(base, sp, 2, 0x820D93B4, calls);
  SetMaterialsFrom(base, w.front_tire_model_address(), regs, sp, 0x820D93D8, calls);
  MaterialArgs(base, sp, 1, 0x820D93F4, calls);
  SetMaterialsFrom(base, w.front_rim_model_address(), regs, sp, 0x820D9418, calls);
  MaterialArgs(base, sp, 2, 0x820D9434, calls);
  SetMaterialsFrom(base, w.front_rim_model_address(), regs, sp, 0x820D9458, calls);
  CopyFloat(base, sp + 240, w.unknown_64_address());
  CopyFloat(base, sp + 244, w.unknown_68_address());
  MaterialArgs(base, sp, 1, 0x820D9484, calls);
  SetMaterialsFrom(base, w.rear_tire_model_address(), regs, sp, 0x820D94A8, calls);
  MaterialArgs(base, sp, 2, 0x820D94C4, calls);
  SetMaterialsFrom(base, w.rear_tire_model_address(), regs, sp, 0x820D94E8, calls);
  MaterialArgs(base, sp, 1, 0x820D9504, calls);
  SetMaterialsFrom(base, w.rear_rim_model_address(), regs, sp, 0x820D9528, calls);
  MaterialArgs(base, sp, 2, 0x820D9544, calls);
  return SetMaterialsFrom(base, w.rear_rim_model_address(), regs, sp, 0x820D9568, calls);
}

uint64_t ScenePartsFinish(uint8_t* base, uint32_t parts, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820E2E68 SceneParts::Finish");
  const uint32_t sp = Frame(base, stack, 112);
  const SceneParts p(base, parts);
  if (int32_t(p.loaded()) != 0) {
    return 1;
  }
  const uint64_t loaded = sl::PartsIsLoaded(base, p.first_set_parts());
  p.set_loaded(uint32_t(loaded));
  if (int32_t(loaded) == 0) {
    return loaded;
  }
  const uint32_t set = p.scene().model_set();
  if (set <= 1) {
    ScenePartsBuild(base, parts, sp, calls);
  }
  return loaded;
}

uint64_t SceneCockpitFinish(uint8_t* base, uint32_t cockpit, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DBFF8 SceneCockpit::Finish");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneCockpit c(base, cockpit);
  if (int32_t(c.finished()) != 0) {
    return 1;
  }
  const uint32_t body = c.body();
  uint64_t bound;
  if (const auto r3 = sl::CockpitBindTextures(base, body, sp)) {
    bound = *r3;
  } else {
    bound = calls.cockpit_bind(body, sp, 0x820DC020);
  }
  if (int32_t(bound) == 0) {
    return 0;
  }
  const Cockpit texture_of(base, c.body());
  const SceneInterior interior = c.interior();
  interior.set_cockpit_texture(texture_of.texture());
  if (int32_t(SceneInteriorFinish(base, c.interior().address, sp, calls)) == 0) {
    return 0;
  }
  c.set_finished(1);
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, c.part_model_0_address() + 4 * i,
          Cockpit(base, c.body()).part_models_address() + 464 * i);
  }
  ss::CockpitFind(base, c.points_address(), c.part_model_0_address(), sp, calls.find);
  AfterFind(calls);
  return 1;
}

// --- The poll ---

uint64_t SceneModelApplyPalettes(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack,
                                 Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820D9EA0 SceneModel::ApplyPalettes");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneModel m(base, model);
  if (int32_t(m.kind()) == 3) {
    const uint32_t body = m.traffic_body();
    if (body != 0) {
      const uint32_t traffic = SceneTraffic(base, body).body();
      r3 = sl::TrafficApplyPalette(base, traffic, traffic, sp);
    }
  } else {
    if (const uint32_t parts = m.parts().address; parts != 0) {
      const uint32_t first = SceneParts(base, parts).first_set_parts();
      r3 = sl::PartsApplyPalette(base, first, first, sp);
    }
    if (const uint32_t interior = m.interior(); interior != 0) {
      const uint32_t first = SceneInterior(base, interior).first_set_interior().address;
      r3 = sl::InteriorApplyPalette(base, first, first, sp);
    }
    if (const uint32_t cockpit = m.cockpit(); cockpit != 0) {
      const uint32_t body = SceneCockpit(base, cockpit).body();
      r3 = sl::CockpitApplyPalette(base, body, body, sp);
    }
  }
  if (const uint32_t wheels = m.wheels(); wheels != 0) {
    r3 = WheelsApplyPalette(base, SceneWheels(base, wheels).first_set_wheels(), sp, calls);
  }
  return r3;
}

uint64_t SceneModelPollLoad(uint8_t* base, uint32_t model, uint32_t stack, Calls& calls) {
  ITC_NATIVE_RUNNING("sub_820DA318 SceneModel::PollLoad");
  const uint32_t sp = Frame(base, stack, 96);
  const SceneModel m(base, model);
  auto set = [](uint32_t word) { return int32_t(word) != 0; };
  if (set(m.release_pending()) || set(m.build_pending())) {
    return 0;
  }
  if (int32_t(m.kind()) == 3) {
    const uint32_t body = m.traffic_body();
    if (body != 0 && !set(m.traffic_body_ready())) {
      if (int32_t(SceneTrafficFinish(base, body, sp, calls)) == 0) {
        return 0;
      }
      const uint32_t traffic_body = m.traffic_body();
      const uint32_t wheels = m.wheels();
      // The traffic body's tire positions (its +7072).
      SceneWheels(base, wheels).set_tire_positions(SceneTraffic(base, traffic_body).body() + 7072);
    }
  } else {
    if (const auto car = m.car_model(); car.address != 0) {
      if (int32_t(CarModelIsReady(base, car.car_model().address)) == 0) {
        return 0;
      }
      if (int32_t(m.car_model().car_model().decals_state()) != 3) {
        return 0;
      }
    }
    if (const uint32_t parts = m.parts().address; parts != 0) {
      if (int32_t(ScenePartsFinish(base, parts, sp, calls)) == 0) {
        return 0;
      }
    }
    if (const uint32_t interior = m.interior(); interior != 0) {
      if (int32_t(SceneInteriorFinish(base, interior, sp, calls)) == 0) {
        return 0;
      }
    }
    if (const uint32_t cockpit = m.cockpit(); cockpit != 0) {
      if (int32_t(SceneCockpitFinish(base, cockpit, sp, calls)) == 0) {
        return 0;
      }
    }
    if (const uint32_t kit = m.light_kit().address; kit != 0) {
      if (int32_t(SceneLightsFinish(base, kit, sp, calls)) == 0) {
        return 0;
      }
    }
  }
  if (const uint32_t wheels = m.wheels(); wheels != 0 && !set(m.wheels_ready())) {
    if (int32_t(SceneWheelsFinish(base, wheels, sp, calls)) == 0) {
      return 0;
    }
  }
  if (int32_t(m.kind()) == 3) {
    return 1;
  }
  if (m.car_model().address == 0 && m.parts().address == 0 && m.interior() == 0 &&
      m.wheels() == 0) {
    return 1;
  }
  // The ready flags: the cockpit's (+5080) only in a race (the scene's +4).
  const bool race = int32_t(m.scene().model_set()) != 0;
  bool ready = set(m.car_model_ready()) && set(m.parts_ready()) && set(m.interior_ready()) &&
               set(m.decals_reloaded());
  if (ready && race) {
    ready = set(m.cockpit_ready());
  }
  if (ready && set(m.wheels_ready()) && set(m.light_kit_ready())) {
    m.set_ready(1);
  }
  if (set(m.ready())) {
    return 1;
  }
  m.set_ready(1);
  m.set_light_kit_ready(1);
  m.set_cockpit_ready(1);
  m.set_interior_ready(1);
  m.set_parts_ready(1);
  m.set_decals_reloaded(1);
  m.set_car_model_ready(1);
  m.set_wheels_ready(1);
  SceneModelApplyPalettes(base, model, model, sp, calls);
  if (set(m.wheels_ready())) {
    if (const uint32_t wheels = m.wheels(); wheels != 0) {
      SceneWheelsSetUpPoints(base, wheels, wheels, sp, calls);
    }
  }
  if (const uint32_t parts = m.parts().address; parts != 0) {
    const uint32_t first = SceneParts(base, parts).first_set_parts();
    CarPartsSetVinylTextures(base, first, first, sp);
  }
  SceneModelApplyPaint(base, model, model, sp);
  if (set(m.ready()) && int32_t(m.kind()) != 3) {
    if (const uint32_t kit = m.light_kit().address; kit != 0) {
      SceneLightsApplyRecord(base, kit, m.light_kit_record_address(), sp, calls);
    }
  }
  SceneModelApplyWheelPaint(base, model, model, sp, calls);
  return 1;
}

}  // namespace load_poll
