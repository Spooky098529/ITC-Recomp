// The scene model record's setters (docs/functions/car-makers.md), step for
// step as the translated code (see car_makers.h).

#include "car_makers/car_makers.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "scene_models/scene_models.h"
#include "schema/car_makers.h"
#include "schema/car_scene_draw.h"
#include "schema/car_specs.h"
#include "schema/race_event.h"
#include "schema/scene_models.h"
#include "strings_and_font/steps.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_makers {

namespace {

using namespace car_scene_draw::ops;

using G = schema::CarMakersGlobals;
using Record = schema::SceneModelRecord;
using Car = schema::CarMakersCar;

constexpr uint32_t kGet82 = 0x820CBC90;  // AeroParts::Get82

// An unsigned value to a float (the original's std, lfd, fcfid, frsp
// through a scratch doubleword below the stack, only it reading it: no
// longer stored since E5 step 6).
double Convert(uint64_t value) { return Frsp(double(int64_t(value))); }

// A halfword as a float into `to`, then two bytes x f0 into `to` + 4 and + 8.
void Triple(uint8_t* base, uint32_t from_half, uint32_t from_b0, uint32_t from_b1, uint32_t to,
            double f0) {
  Stfs(base, to, Convert(guest::LoadU16(base, from_half)));
  Stfs(base, to + 4, Fmuls(Convert(guest::LoadU8(base, from_b0)), f0));
  Stfs(base, to + 8, Fmuls(Convert(guest::LoadU8(base, from_b1)), f0));
}

// An lfs then stfs of one word: its bits (the translated code's float to
// double and back folds to a copy, keeping a signaling NaN).
void CopyFloat(uint8_t* base, uint32_t from, uint32_t to) {
  Store(base, to, Load(base, from));
}

// One decal entry of the car (32 bytes at `e`) into the record's 64-byte
// entry at `r` (its +0, +4 words, +16, +20, +32, +48, +52 floats), in the
// original's order.
void Decal(uint8_t* base, uint32_t r_at, uint32_t e_at) {
  const schema::CarMakersRecordDecal r(base, r_at);
  const schema::CarMakersCarDecal e(base, e_at);
  r.set_image(e.image());
  r.set_mirrored(e.mirrored());
  CopyFloat(base, e.x_address(), r.x_address());
  CopyFloat(base, e.y_address(), r.y_address());
  CopyFloat(base, e.angle_address(), r.angle_address());
  CopyFloat(base, e.flag_scale_address(), r.flag_scale_address());
  CopyFloat(base, e.scale_address(), r.scale_address());
}

// The record's interior choice `slot` (0 to 6, 12 bytes from +3780).
uint32_t ChoiceSlot(const Record& rec, uint32_t slot) {
  return rec.interior_choices_address() + 12 * slot;
}

// SetChoice with the car's 12-byte value at `from` (ld of its first
// doubleword; the word +8 rotated into r5's high half), from the frame.
void ChoiceFrom(uint8_t* base, uint32_t out, uint32_t from, uint32_t frame) {
  const schema::CarMakersCarChoice v(base, from);
  const uint64_t c = v.c();
  const uint64_t r4 = (uint64_t(v.a()) << 32) | v.b();
  SetChoice(base, out, r4, c << 32, frame);
}

}  // namespace

uint32_t BlockOf(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8215AAD0 CarModels::BlockOf");
  // By id - 1 (0 to 70); id 1's case is sub_8215AB04.
  static constexpr uint8_t kBlocks[71] = {
      1,  18, 19, 2,  4,  3,  10, 9,  21, 5,  8,  7,  6,  12, 11, 13, 14, 20,
      15, 22, 17, 16, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 41, 42, 43, 44, 36, 37, 38, 39,
      11, 12, 15, 13, 10, 5,  4,  6,  9,  1,  2,  3,  0,  14, 8,  7,  40};
  const uint32_t index = id - 1;
  return index > 70 ? 0 : kBlocks[index];
}

uint32_t SetPaint(uint8_t* base, uint32_t record, uint32_t car, uint32_t pick, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215AC70 SceneModelRecord::SetPaint");
  const Record rec(base, record);
  const Car c(base, car);
  int32_t p = int32_t(pick);
  if (p == -1) {
    p = int16_t(c.paint_pick());
  }
  const int32_t kind = int32_t(rec.kind());
  const double f0 = double(G::byte_scale(base));
  (void)stack;  // the scratch doublewords below it no longer written (E5 step 6)
  if (kind == 3) {
    rec.set_traffic_paint(uint32_t(p));
  } else if (p == -2) {
    for (uint32_t i = 0; i < 3; i++) {
      Triple(base, c.paint_copy0_hue0_address() + 4 * i, c.paint_copy0_saturation0_address() + 4 * i,
             c.paint_copy0_value0_address() + 4 * i, rec.paint0_hue_address() + 16 * i, f0);
    }
    rec.set_paint0_hue(float(Convert(c.paint_hue())));
    rec.set_paint0_saturation(float(Fmuls(Convert(c.paint_saturation()), f0)));
    rec.set_paint0_value(float(Fmuls(Convert(c.paint_value()), f0)));
    rec.set_paint1_hue(float(Convert(c.paint_hue())));
    rec.set_paint1_saturation(float(Fmuls(Convert(c.paint_saturation()), f0)));
    rec.set_paint1_value(float(Fmuls(Convert(c.paint_value()), f0)));
    const int32_t six = int16_t(c.paint_kind());
    if (six == 0) {
      const double zero = Lfs(base, schema::CarSceneDrawGlobals::zero_address());
      rec.set_paint2_hue(float(zero));
      rec.set_paint2_saturation(float(zero));
      CopyFloat(base, G::value72_address(), rec.paint2_value_address());
    } else if (six == 1) {
      const double value = double(G::value72_kind1(base));
      rec.set_paint2_hue(float(Convert(c.paint_hue())));
      const uint32_t b10 = c.paint_saturation();
      rec.set_paint2_value(float(value));
      const double f12 = Convert(b10);
      rec.set_paint2_saturation(float(Fmuls(Fmuls(f12, double(G::hue_scale(base))), f0)));
    }
  } else {
    const uint32_t from = uint32_t(int64_t(p) * 20) + c.paint_copy0_hue0_address();
    for (uint32_t i = 0; i < 3; i++) {
      Triple(base, from + 4 * i, from + 2 + 4 * i, from + 3 + 4 * i,
             rec.paint0_hue_address() + 16 * i, f0);
    }
  }
  rec.set_wheels_painted(1);
  Triple(base, c.front_wheel_color0_hue_address(), c.front_wheel_color0_saturation_address(),
         c.front_wheel_color0_value_address(), rec.front_wheel_paint0_hue_address(), f0);
  Triple(base, c.rear_wheel_color0_hue_address(), c.rear_wheel_color0_saturation_address(),
         c.rear_wheel_color0_value_address(), rec.rear_wheel_paint0_hue_address(), f0);
  Triple(base, c.front_wheel_color1_hue_address(), c.front_wheel_color1_saturation_address(),
         c.front_wheel_color1_value_address(), rec.front_wheel_paint1_hue_address(), f0);
  Triple(base, c.rear_wheel_color1_hue_address(), c.rear_wheel_color1_saturation_address(),
         c.rear_wheel_color1_value_address(), rec.rear_wheel_paint1_hue_address(), f0);
  Triple(base, c.front_wheel_color2_hue_address(), c.front_wheel_color2_saturation_address(),
         c.front_wheel_color2_value_address(), rec.front_wheel_paint2_hue_address(), f0);
  Triple(base, c.rear_wheel_color2_hue_address(), c.rear_wheel_color2_saturation_address(),
         c.rear_wheel_color2_value_address(), rec.rear_wheel_paint2_hue_address(), f0);
  CopyFloat(base, c.front_wheel_f16_address(), rec.front_wheel_f16_address());
  CopyFloat(base, c.rear_wheel_f16_address(), rec.rear_wheel_f16_address());
  CopyFloat(base, c.front_wheel_f28_address(), rec.front_wheel_f28_address());
  CopyFloat(base, c.rear_wheel_f28_address(), rec.rear_wheel_f28_address());
  return record;
}

uint32_t SetWheels(uint8_t* base, uint32_t record, uint32_t car, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B238 SceneModelRecord::SetWheels");
  (void)stack;  // no frame (E5 step 6: its back chain was all it held)
  const Record rec(base, record);
  const Car c(base, car);
  rec.set_front_wheel(c.front_wheel_number() - 1);
  rec.set_rear_wheel(c.rear_wheel_number() - 1);
  rec.set_wheel_extra(c.power_train4());
  rec.set_caliper_texture(c.power_train3());
  // The car's spec copies (race-event's CarSpecCopies, sub_820C4550's answer).
  uint32_t block2 = vehicle_accessors::ObjectRecord1888(car);
  if (schema::CarSpecCopies(base, block2).wheel_236() != 0) {
    block2 = vehicle_accessors::ObjectRecord1888(car);
    rec.set_caliper_texture(schema::CarSpecCopies(base, block2).wheel_236());
  }
  using strings_and_font::steps::Fctiwz;
  rec.set_front_size(uint32_t(Fctiwz(double(c.front_tire_80()))));
  const double scale = double(G::tire_scale(base));
  rec.set_front_size_a(float(Fmuls(double(c.front_tire_48()), scale)));
  const double hundred = double(G::hundred(base));
  rec.set_front_size_b(float(Fmuls(double(c.front_tire_84()), hundred)));
  rec.set_rear_size(uint32_t(Fctiwz(double(c.rear_tire_80()))));
  rec.set_rear_size_a(float(Fmuls(double(c.rear_tire_48()), scale)));
  rec.set_rear_size_b(float(Fmuls(double(c.rear_tire_84()), hundred)));
  return block2;
}

void SetChoice(uint8_t* base, uint32_t out, uint64_t r4, uint64_t r5, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B1A0 SceneModelRecord::SetChoice");
  // The value's words as registers (the original spilled r4 and r5 into the
  // caller's parameter area, +32 and +40, and read them back; E5 step 6).
  (void)stack;
  const schema::CarMakersRecordChoice choice(base, out);
  const uint32_t c = uint32_t(r5 >> 32);
  if (c == 0) {
    choice.set_set(c);
    return;
  }
  const uint32_t a = uint32_t(r4 >> 32);
  choice.set_index(c - 1);
  choice.set_set(1);
  uint32_t k = 0;
  if (int32_t(a) == 1) {
    const uint32_t b = uint32_t(r4);
    k = b < 1 ? 1 : b == 1 ? 2 : 0;
  } else if (int32_t(a) == 2) {
    const uint32_t b = uint32_t(r4);
    k = b == 1 ? 1 : 0;
  }
  choice.set_variant(uint32_t(uint64_t(a) * 3 + k - 3));
}

void ResetBytes(uint8_t* base, uint32_t at) {
  ITC_NATIVE_RUNNING("sub_820D0900 CarRecord::ResetBytes");
  Store(base, at + 0, 0);
  Store(base, at + 4, 0);
  Store(base, at + 8, 1);
  Store(base, at + 12, 2);
  Store(base, at + 16, 3);
  Store(base, at + 20, 29);
  Store(base, at + 24, 36);
  Store(base, at + 36, 37);
  Store(base, at + 40, 34);
  Store(base, at + 44, 35);
  Store(base, at + 28, 31);
  Store(base, at + 32, 32);
}

uint32_t SetBytes(uint8_t* base, uint32_t record, uint32_t bytes, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B0E0 CarRecord::SetBytes");
  (void)stack;  // no frame (E5 step 6: its back chain was all it held)
  const schema::CarRecord car_record(base, record);
  const uint32_t at = car_record.plate_address();
  ResetBytes(base, at);
  Store(base, at, 0);
  for (uint32_t i = 1; i <= 11; i++) {
    Store(base, at + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, bytes + i)))));
  }
  car_record.set_unknown_3684(uint32_t(int32_t(int8_t(guest::LoadU8(base, bytes + 0)))));
  return at;
}

uint32_t FromCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t record,
                 uint32_t car, uint64_t pick, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B878 SceneModelRecord::FromCar");
  const uint32_t frame = stack - 256;
  Store(base, frame, stack);
  const Record rec(base, record);
  const Car c(base, car);
  scene_models::SceneModelRecordReset(base, record);
  const uint32_t flags = c.flags();
  bool cars = true;  // r23
  uint32_t block;
  if ((flags & 0x81) != 0) {
    rec.set_kind(2);
    block = BlockOf(c.car_id());
  } else if ((flags & 0x2) != 0) {
    rec.set_kind(1);
    block = BlockOf(c.car_id());
  } else {
    rec.set_kind(3);
    block = BlockOf(c.car_id());
    cars = false;
  }
  const uint32_t car_record = rec.block_address();
  rec.set_block(block);
  SetWheels(base, record, car, frame);
  SetPaint(base, record, car, uint32_t(pick), frame);
  if (int32_t(rec.kind()) == 2) {
    bool both = false;  // r28
    scene_models::CockpitRecordReset(base, rec.cockpit_bonnet_address());
    rec.set_cockpit_bonnet(c.bonnet());
    rec.set_cockpit_bonnet_again(c.bonnet());
    rec.set_cockpit_aero9(c.aero_item9());
    rec.set_cockpit_aero7(c.aero_item7());
    rec.set_cockpit_aero6(c.aero_item6());
    rec.set_interior_item0(c.interior_item0());
    rec.set_interior_item1(c.interior_item1());
    rec.set_interior_item2(c.interior_item2());
    rec.set_interior_item3(c.interior_item3());
    const int32_t seats = int32_t(c.body_level());
    if (seats > 0 && seats <= 2) {
      rec.set_interior_has_seats(1);
      rec.set_interior_seats(0);
    } else if (seats > 2 && seats <= 5) {
      both = true;
      rec.set_interior_has_seats(1);
      rec.set_interior_seats(1);
    } else if (seats == 6) {
      both = true;
      rec.set_interior_has_seats(1);
      rec.set_interior_seats(2);
    } else {
      rec.set_interior_has_seats(0);
      rec.set_interior_seats(0);
    }
    bool second = false;
    if (int32_t(schema::CarSpecCopies(base, vehicle_accessors::ObjectRecord1888(car)).unknown_232()) == 1) {
      rec.set_interior_seats(1);
      second = true;
    } else {
      // The same word read again, as the original does.
      if (int32_t(schema::CarSpecCopies(base, vehicle_accessors::ObjectRecord1888(car)).unknown_232()) == 1) {
        both = true;
        rec.set_interior_seats(2);
      }
      second = both;
    }
    if (second) {
      ChoiceFrom(base, ChoiceSlot(rec, 2), c.choice3_address(), frame);
      ChoiceFrom(base, ChoiceSlot(rec, 3), c.choice4_address(), frame);
    } else {
      ChoiceFrom(base, ChoiceSlot(rec, 0), c.choice3_address(), frame);
      ChoiceFrom(base, ChoiceSlot(rec, 1), c.choice4_address(), frame);
    }
    ChoiceFrom(base, ChoiceSlot(rec, 6), c.choice0_address(), frame);
    ChoiceFrom(base, ChoiceSlot(rec, 5), c.choice1_address(), frame);
    ChoiceFrom(base, ChoiceSlot(rec, 4), c.choice2_address(), frame);
    const uint32_t light = c.accessory();
    if (int32_t(light) != 3) {
      rec.set_accessory(light);
      rec.set_accessory_variant(c.accessory_variant());
    }
  }
  if (cars) {
    rec.set_parts_engine_stage(c.engine_stage());
    if (int32_t(c.engine_stage()) >= 7) {
      rec.set_interior_engine_grade(2);
    } else if (int32_t(c.engine_stage()) >= 3) {
      rec.set_interior_engine_grade(1);
    } else {
      rec.set_interior_engine_grade(0);
    }
    rec.set_parts_power(c.power_part());
    rec.set_parts_cooling(c.cooling());
    rec.set_parts_aero6(c.aero_item6());
    rec.set_parts_aero7(c.aero_item7());
    rec.set_parts_aero8(c.aero_item8());
    if (c.aero_kit() == 0) {
      rec.set_parts_aero0(c.aero_item0());
      rec.set_parts_aero2(c.aero_item2());
      rec.set_parts_aero1(c.aero_item1());
      uint32_t v = c.aero_item3();
      rec.set_parts_aero3(int32_t(v) == -1 ? 13 : v);
      rec.set_parts_bonnet(c.bonnet());
      rec.set_bonnet_painted(int32_t(c.bonnet_color_kind()) == 1 ? 1 : 0);
      rec.set_parts_aero_kit(0);
      rec.set_parts_aero9(c.aero_item9());
    } else {
      const uint32_t out = frame + 128, parts_copy = frame + 80;
      uint32_t parts[9];
      car_specs::GetCarParts(base, c.car_id(), parts);
      for (uint32_t i = 0; i < 9; i++) {
        Store(base, out + 4 * i, parts[i]);
      }
      front_end_car_draw::Memcpy(base, calls, parts_copy, out, 36, frame, 0x8215BB98);
      const uint32_t k = c.aero_kit();
      const uint64_t k_less = uint64_t(k) - 1;  // r29
      for (uint32_t i = 0; i < 9; i++) {
        parts[i] = Load(base, parts_copy + 4 * i);
      }
      uint32_t kit = out;
      if (!car_specs::AeroPart82(base, out, parts, int32_t(k))) {
        const uint64_t regs[3] = {out, parts_copy, k};
        kit = uint32_t(calls.call(kGet82, regs, 3, nullptr, 0, frame, 0x8215BBAC, nullptr));
      }
      const uint32_t entry = uint32_t((uint64_t(rec.block() << 1) + k_less) * 24) +
                             G::aero_kits_address();
      const uint32_t kit_flags = schema::AeroPartRecord(base, kit).flags();
      front_end_car_draw::Memcpy(base, calls, parts_copy, entry, 24, frame, 0x8215BBDC);
      auto word = [&](uint32_t at) { return Load(base, frame + at); };
      if (int32_t(word(80)) != -1) {
        rec.set_parts_aero0(word(80));
      }
      if (int32_t(word(84)) != -1) {
        rec.set_parts_aero2(word(84));
      }
      if (int32_t(word(88)) != -1) {
        rec.set_parts_aero1(word(88));
      }
      if ((kit_flags & 0x2) != 0) {
        const uint32_t v = c.aero_item3();
        rec.set_parts_aero3(int32_t(v) == -1 ? 13 : v);
      } else if (int32_t(word(100)) != -1) {
        rec.set_parts_aero3(word(100));
      }
      if ((kit_flags & 0x1) != 0) {
        rec.set_parts_bonnet(c.bonnet());
        rec.set_bonnet_painted(int32_t(c.bonnet_color_kind()) == 1 ? 1 : 0);
      } else if (int32_t(word(96)) != -1) {
        rec.set_parts_bonnet(word(96));
      }
      rec.set_parts_aero_kit((kit_flags & 0x8) != 0 ? uint32_t(k_less + 1) : 0);
      if (int32_t(word(92)) != -1) {
        rec.set_parts_aero9(word(92));
      }
    }
  }
  rec.set_exterior_item1(c.exterior_item1());
  if (cars) {
    // Entries 2, 1, 0 of the car's six and 5, 4, 3 (their +96 bytes on) into
    // the record's first three and the next three (+192 bytes on).
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t r = rec.decals_address() + 64 * i, e = c.decals_address() + 64 - 32 * i;
      Decal(base, r, e);
      Decal(base, r + 192, e + 96);
    }
    // The car's forty from its +288 in runs of five, each into a group of
    // six slots of the record's (from its +576): the record's and the car's
    // offsets from their decals.
    static constexpr uint32_t kRuns[8][2] = {{384, 192},   {1152, 352},  {768, 512},
                                             {1536, 672},  {1920, 832},  {2304, 992},
                                             {3072, 1152}, {2688, 1312}};
    for (const auto& run : kRuns) {
      for (uint32_t i = 0; i < 5; i++) {
        Decal(base, rec.decals_address() + run[0] + 64 * i, c.decals_address() + run[1] + 32 * i);
      }
    }
  }
  const int32_t doors = int16_t(c.body_class());
  rec.set_parts_class((doors == 1 || doors == 2 || doors == 4) ? 1 : 0);
  if (cars) {
    // The tire grades' table {0, 1, 1, 2, 2} (the original's frame +80 to
    // +96, a local since E5 step 6), indexed unchecked: past it (never in
    // play) the stack's word there, as the original.
    static constexpr uint32_t kTable[5] = {0, 1, 1, 2, 2};
    const uint32_t table = frame + 80;
    auto grade = [&](uint32_t index) {
      return index < 5 ? kTable[index] : Load(base, table + (index << 2));
    };
    rec.set_layer0_a(c.layer0_a());
    CopyFloat(base, c.layer0_f8_address(), rec.layer0_f8_address());
    CopyFloat(base, c.layer0_f12_address(), rec.layer0_f12_address());
    CopyFloat(base, c.layer0_f16_address(), rec.layer0_f16_address());
    rec.set_layer1_a(c.layer1_a());
    CopyFloat(base, c.layer1_f8_address(), rec.layer1_f8_address());
    CopyFloat(base, c.layer1_f12_address(), rec.layer1_f12_address());
    CopyFloat(base, c.layer1_f16_address(), rec.layer1_f16_address());
    rec.set_layer2_a(c.layer2_a());
    CopyFloat(base, c.layer2_f8_address(), rec.layer2_f8_address());
    CopyFloat(base, c.layer2_f12_address(), rec.layer2_f12_address());
    CopyFloat(base, c.layer2_f16_address(), rec.layer2_f16_address());
    rec.set_layer0_b(c.layer0_b());
    rec.set_front_tire_grade(grade(c.front_tire_grade()));
    rec.set_rear_tire_grade(grade(c.rear_tire_grade()));
  }
  return SetBytes(base, car_record, c.setting_bytes_address(), frame);
}

}  // namespace car_makers
