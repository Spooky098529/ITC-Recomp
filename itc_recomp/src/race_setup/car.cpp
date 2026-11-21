// The car object's parts as plain C++ (docs/functions/race-setup.md); see
// race_setup.h.

#include "race_setup/race_setup.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "race_overlays/race_overlays.h"
#include "schema/garage_screen.h"
#include "schema/race_setup.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <climits>
#include <cmath>

namespace race_setup {

namespace {

using G = schema::RaceSetupGlobals;

// The originals' lfs then stfs of one float.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}

// `count` decal entries from `decals`, as the originals' loops store them.
void ResetDecals(uint8_t* base, uint32_t decals, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    const schema::CarDecal entry(base, decals + 32 * i);
    entry.set_flag_scale(float(double(G::one(base))));
    entry.set_packed_high(0);
    entry.set_scale(float(double(G::one(base))));
    entry.set_packed_byte_22(0);
    entry.set_x(float(double(G::zero(base))));
    entry.set_packed_byte_23(0);
    entry.set_y(float(double(G::zero(base))));
    entry.set_mirrored(0);
    entry.set_angle(float(double(G::zero(base))));
    entry.set_image(0xFFFFFFFFu);
  }
}

// fctiwz as the translated code does it: NaN and values at or below
// -2^31 - 1 give INT32_MIN, values at or above INT32_MAX give INT32_MAX.
int32_t Fctiwz(double v) {
  if (std::isnan(v) || v <= -2147483649.0) {
    return INT32_MIN;
  }
  if (v >= 2147483647.0) {
    return INT32_MAX;
  }
  return int32_t(v);
}

// lwa, fcfid, frsp, stfs: a signed word as a float.
void WordToFloat(uint8_t* base, uint32_t to, uint32_t from) {
  const int32_t word = int32_t(guest::LoadU32(base, from));
  guest::StoreFloat(base, to, float(double(int64_t(word))));
}

// The three 20-byte entries at a car object's +1588 from `from`: +0, +4
// copied, +8 to +16 converted from integers (in the originals' orders: the
// build's +8, +16, +12, the spec's +8, +12, +16).
void CopyEntries1588(uint8_t* base, uint32_t car, uint32_t from, bool from_build) {
  for (uint32_t i = 0; i < 3; i++) {
    const schema::CarLayer to(base, schema::Car(base, car).layer0_a_address() + 20 * i);
    const uint32_t source = from + 20 * i;
    to.set_a(guest::LoadU32(base, source + 0));
    to.set_b(guest::LoadU32(base, source + 4));
    if (from_build) {
      WordToFloat(base, to.f8_address(), source + 8);
      WordToFloat(base, to.f16_address(), source + 16);
      WordToFloat(base, to.f12_address(), source + 12);
    } else {
      WordToFloat(base, to.f8_address(), source + 8);
      WordToFloat(base, to.f12_address(), source + 12);
      WordToFloat(base, to.f16_address(), source + 16);
    }
  }
}

// rand() % n, as divw's remainder (rand is 0 to 32767).
uint32_t RandBelow(uint8_t* base, uint32_t n, uint32_t& last) {
  last = race_overlays::Rand(base);
  return uint32_t(int32_t(last) % int32_t(n));
}

}  // namespace

void ResetDecals40(uint8_t* base, uint32_t decals) {
  ITC_NATIVE_RUNNING("sub_820CC4A0 Car::ResetDecals40");
  ResetDecals(base, decals, 40);
}

void ResetDecals6(uint8_t* base, uint32_t decals) {
  ITC_NATIVE_RUNNING("sub_820CCBF0 Car::ResetDecals6");
  ResetDecals(base, decals, 6);
}

void ResetSettings(uint8_t* base, uint32_t settings) {
  ITC_NATIVE_RUNNING("sub_820CD760 Car::ResetSettings");
  // `settings` is a car object's +16.
  const schema::Car car(base, settings - 16);
  car.set_setting_77(7);
  car.set_setting_72(2);
  car.set_setting_73(2);
  car.set_setting_74(2);
  car.set_setting_75(2);
  car.set_setting_76(3);
  // +78 to +92 (the setting bytes, the cambers among them) = 0.
  for (uint32_t i = 78; i <= 92; i++) {
    guest::StoreU8(base, car.address + i, 0);
  }
  car.set_rear_torque_share(10);
  car.set_setting_94(0);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU16(base, car.setting_half_0_address() + 2 * i, 0);
  }
}

void Reset1568(uint8_t* base, uint32_t bytes) {
  ITC_NATIVE_RUNNING("sub_820CCC70 Car::Reset1568");
  // `bytes` is a car object's +1568.
  const schema::Car car(base, bytes - 1568);
  car.set_extras_word_4(0);
  // The original's order: +0, +1, +12, then +2 to +11.
  car.set_exterior_item_2(0);
  car.set_unknown_1569(0);
  car.set_model_flag(0);
  car.set_unknown_1570(5);
  car.set_unknown_1571(0);
  car.set_unknown_1572(0);
  car.set_unknown_1573(1);
  car.set_unknown_1574(2);
  car.set_unknown_1575(1);
  car.set_unknown_1576(2);
  car.set_unknown_1577(2);
  car.set_unknown_1578(2);
  car.set_unknown_1579(2);
}

void TuningReset(uint8_t* base, uint32_t tuning, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_820CD618 CarTuning::Reset");
  const schema::CarTuning t(base, tuning);
  t.set_tuning_head(0);
  t.set_spec_flag(flag != 0 ? 1 : 0);
  // (Seven zero words go through the red zone first: not written.)
  const uint32_t word4 = t.spec_flag();
  t.set_engine_stage(0);
  t.set_power_part_a(0);
  t.set_spec_flag_copy(word4 == 1 ? 1 : 0);
  // +20 (the cooling) to +164 (interior item 3) = 0.
  for (uint32_t offset = 20; offset <= 164; offset += 4) {
    guest::StoreU32(base, tuning + offset, 0);
  }
  t.set_interior_item_4(3);
  t.set_interior_item_5(0);
  for (uint32_t i = 0; i < 5; i++) {
    const schema::CarChoice entry(base, t.choice0_a_address() + 12 * i);
    entry.set_c(0);
    entry.set_b(0);
    entry.set_a(1);
  }
}

void DecalFromBuild(uint8_t* base, uint32_t decal, uint32_t build) {
  ITC_NATIVE_RUNNING("sub_820CDC18 CarDecal::FromBuild");
  const uint32_t w20 = guest::LoadU32(base, build + 20);
  const uint32_t w24 = guest::LoadU32(base, build + 24);
  const uint32_t w28 = guest::LoadU32(base, build + 28);
  const schema::CarDecal d(base, decal);
  // sub_820CC470: +20.
  d.set_packed((w20 & 0xFFFF) << 16 | (w24 & 0xFF) << 8 | (w28 & 0xFF));
  // sub_820CC460, sub_820CC468: +8, +12.
  CopyFloat(base, d.x_address(), build + 8);
  CopyFloat(base, d.y_address(), build + 12);
  const double scale = double(guest::LoadFloat(base, build + 0));
  uint32_t mirrored = 0;
  if (scale < double(G::zero(base))) {
    // sub_820CC450 (never seen in play): +0 = -(build +0).
    d.set_flag_scale(float(-scale));
    mirrored = 1;
  }
  // sub_820CC478: +28.
  d.set_mirrored(mirrored);
  // sub_820CC458: +4.
  CopyFloat(base, d.scale_address(), build + 4);
  vehicle_accessors::Object344SetFloat16(base, decal, double(guest::LoadFloat(base, build + 16)));
  scene_models::SetCarModel(base, decal, guest::LoadU32(base, build + 32));
}

uint32_t CarClear(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820CD7F0 Car::Clear");
  const schema::Car c(base, car);
  c.set_car_id(0);
  TuningReset(base, c.tuning_address(), 0);
  c.set_paint_2(0);
  c.set_paint_pick(0);
  c.set_paint_kind(0);
  c.set_paint_hue(0);
  c.set_paint_saturation(0);
  c.set_body_class(0xFFFF);
  c.set_paint_value(0);
  ResetSettings(base, c.setting_half_0_address());
  ResetDecals40(base, c.decals40_address());
  ResetDecals6(base, c.decals6_address());
  Reset1568(base, c.extras_address());
  for (uint32_t i = 0; i < 3; i++) {
    const schema::CarLayer entry(base, c.layer0_a_address() + 20 * i);
    entry.set_f8(float(double(G::zero(base))));
    entry.set_a(0);
    entry.set_f12(float(double(G::zero(base))));
    entry.set_b(0);
    entry.set_f16(float(double(G::zero(base))));
  }
  return c.extras_address();
}

uint32_t CarInit(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820CD5D8 Car::Init");
  const uint32_t r3 = CarClear(base, car);
  const schema::Car c(base, car);
  c.set_flags(0);
  c.set_unknown_3892(1);
  return r3;
}

bool CarSetupVehicle(uint8_t* base, uint32_t car, uint32_t data, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820CD5C8 Car::SetupVehicle");
  return car_physics::VehicleSetupFromCar(base, data, car, frame, f1);
}

uint32_t SetModel(uint8_t* base, uint32_t car, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820CD8A8 Car::SetModel");
  const schema::Car c(base, car);
  c.set_car_id(id);
  if (car_specs::Exists(base) == 0) {
    return 0;
  }
  const schema::CarSpec spec(base, car_specs::GetCarSpec(base, c.car_id()));
  TuningReset(base, c.tuning_address(), spec.turbo() != 0 ? 1 : 0);
  CopyFloat(base, c.final_drive_address(), spec.final_drive_address());
  // The ratios from the spec's +388 (gear_ratios + 4).
  for (uint32_t i = 0; i < 8; i++) {
    CopyFloat(base, c.gear_ratio_0_address() + 4 * i, spec.gear_ratios_address() + 4 + 4 * i);
  }
  c.set_setting_76(uint8_t(spec.unknown_632()));
  c.set_setting_77(uint8_t(spec.unknown_636()));
  c.set_setting_74(uint8_t(spec.unknown_800()));
  c.set_setting_75(uint8_t(spec.unknown_808()));
  c.set_setting_72(uint8_t(spec.unknown_796()));
  // +72 again (the original's; +73 is left as it was).
  c.set_setting_72(uint8_t(spec.unknown_804()));
  const uint32_t drive = spec.drive_type();
  if (drive != 0 && drive != 1) {
    c.set_split_drive(1);
    namespace ops = car_physics::ops;
    double share = ops::Fsubs(ops::Lfs(base, G::one_address()), ops::Lfs(base, spec.front_torque_share_address()));
    share = ops::Fmuls(share, ops::Lfs(base, G::hundred_address()));
    share = ops::Fmuls(share, ops::Lfs(base, G::twenty_address()));
    share = ops::Fmuls(share, ops::Lfs(base, G::torque_scale_address()));
    // (The original converts through its frame's +80: not written.)
    c.set_rear_torque_share(uint8_t(uint32_t(Fctiwz(share))));
  } else {
    c.set_split_drive(0);
  }
  c.set_nitrous(spec.unknown_772());
  c.set_exterior_item_3(spec.unknown_776());
  c.set_drivetrain_36(spec.unknown_784());
  c.set_drivetrain_32(spec.unknown_780());
  c.set_lsd_a(spec.unknown_788());
  c.set_lsd_b(spec.unknown_792());
  CopyEntries1588(base, car, spec.unknown_812_address(), false);
  c.set_front_tire_grade(spec.unknown_888());
  c.set_front_tire_type(spec.unknown_884());
  c.set_rear_tire_grade(spec.unknown_896());
  c.set_rear_tire_type(spec.unknown_892());
  for (uint32_t i = 0; i < 5; i++) {
    const schema::CarChoice to(base, c.choice0_a_address() + 12 * i);
    const uint32_t from = spec.unknown_900_address() + 8 * i;
    to.set_c(guest::LoadU32(base, from));
    const int32_t next = int32_t(guest::LoadU32(base, from + 4));
    to.set_a(uint32_t(next / 3 + 1));
    const int32_t again = int32_t(guest::LoadU32(base, from + 4));
    to.set_b(uint32_t(again - (again / 3) * 3));
  }
  if (spec.displacement() <= 2000 && spec.unknown_716() <= 4700 && spec.unknown_720() <= 2000 &&
      spec.unknown_724() <= 1700) {
    c.set_unknown_1571(0);
    c.set_unknown_1570(5);
  } else {
    c.set_unknown_1570(3);
    c.set_unknown_1571(3);
  }
  c.set_unknown_1572(0);
  uint32_t last = 0;
  c.set_unknown_1569(uint8_t(RandBelow(base, 14, last)));
  // +1573: 1 to 10; +1574 and +1575: after a 10, 1 to 10, else 1 to 9.
  uint32_t value = RandBelow(base, 10, last);
  if (value == 0) {
    value = 10;
  }
  c.set_unknown_1573(uint8_t(value));
  for (uint32_t at : {c.unknown_1574_address(), c.unknown_1575_address()}) {
    if (int8_t(value) == 10) {
      value = RandBelow(base, 10, last);
      if (value == 0) {
        value = 10;
      }
    } else {
      value = RandBelow(base, 9, last) + 1;
    }
    guest::StoreU8(base, at, uint8_t(value));
    value &= 0xFF;
  }
  c.set_unknown_1576(uint8_t(RandBelow(base, 9, last) + 1));
  const int32_t car_id = int32_t(id);
  if (car_id == 56 || (car_id > 63 && car_id <= 68) || car_id == 70) {
    c.set_model_flag(1);
  }
  return last;
}

uint32_t FromBuild(uint8_t* base, uint32_t car, uint32_t build) {
  ITC_NATIVE_RUNNING("sub_820CDCE0 Car::FromBuild");
  CarClear(base, car);
  const schema::Car c(base, car);
  const uint32_t id = guest::LoadU32(base, build + 2788);
  c.set_car_id(id);
  SetModel(base, car, id);
  const uint16_t paint = uint16_t(guest::LoadU32(base, build + 2792));
  c.set_paint_pick(paint);
  c.set_paint_2(paint);
  c.set_paint_hue(uint16_t(guest::LoadU32(base, build + 2800)));
  c.set_paint_saturation(uint8_t(guest::LoadU32(base, build + 2804)));
  c.set_paint_value(uint8_t(guest::LoadU32(base, build + 2808)));
  c.set_paint_kind(uint16_t(guest::LoadU32(base, build + 2796)));
  for (uint32_t k = 0; k < 3; k++) {
    DecalFromBuild(base, c.decals6_address() + 32 * k, build + 392 + 36 * k);
    DecalFromBuild(base, c.decals6_address() + 96 + 32 * k, build + 500 + 36 * k);
  }
  static constexpr uint32_t kGroups[8] = {608, 968, 1328, 1688, 1868, 2048, 2408, 2588};
  for (uint32_t g = 0; g < 8; g++) {
    for (uint32_t k = 0; k < 5; k++) {
      DecalFromBuild(base, c.decals40_address() + 160 * g + 32 * k, build + kGroups[g] + 36 * k);
    }
  }
  namespace ops = car_physics::ops;
  const double one = ops::Lfs(base, G::one_address());
  const double scale = ops::Lfs(base, G::gear_scale_address());
  const double zero = ops::Lfs(base, G::zero_address());
  // The final drive and the first six gear ratios (+36 to +60).
  for (uint32_t i = 0; i < 7; i++) {
    const double factor = ops::Lfs(base, build + 4 * i);
    if (factor == zero) {
      continue;
    }
    const double ratio = ops::Fmadds(factor, scale, one);
    const uint32_t at = c.final_drive_address() + 4 * i;
    ops::Stfs(base, at, ops::Fmuls(ratio, ops::Lfs(base, at)));
  }
  // Setting bytes from build words +28 to +116.
  static constexpr uint8_t kSettings[23] = {72, 73, 74, 75, 80, 81, 78, 79, 82, 83, 84, 85,
                                            86, 87, 88, 89, 94, 92, 93, 90, 91, 76, 77};
  for (uint32_t i = 0; i < 23; i++) {
    guest::StoreU8(base, car + kSettings[i], uint8_t(guest::LoadU32(base, build + 28 + 4 * i)));
  }
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, c.extras_word_0_address() + 4 * i,
                    guest::LoadU32(base, build + 2768 + 4 * i));
  }
  CopyEntries1588(base, car, build + 332, true);
  // The tuning words: build +120 + 4i into car +kTuning[i].
  static constexpr uint16_t kTuning[38] = {
      1656, 1664, 1672, 1668, 1676, 1680, 1684, 1688, 1696, 1692, 1704, 1700, 1708,
      1716, 1712, 1724, 1720, 1744, 1748, 1752, 1756, 1764, 1760, 1768, 1772, 1776,
      1780, 1784, 1728, 1732, 1736, 1740, 1788, 1796, 1800, 1804, 1808, 1812};
  for (uint32_t i = 0; i < 38; i++) {
    guest::StoreU32(base, car + kTuning[i], guest::LoadU32(base, build + 120 + 4 * i));
  }
  uint32_t wheel = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const int32_t number = int32_t(guest::LoadU32(base, build + 232 + 8 * i));
    wheel = car_specs::FirstWheelOfModel(base, number / 10);
    guest::StoreU32(base, c.front_wheel_address() + 8 * i, wheel + uint32_t(number % 10));
    guest::StoreU32(base, c.front_inch_up_address() + 8 * i,
                    guest::LoadU32(base, build + 236 + 8 * i));
  }
  return wheel;
}

}  // namespace race_setup
