// The tuning screen's set-up's callees (docs/functions/tuning-machine.md):
// see tuning_machine.h. Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt): the vinyl layers' angles are the translated code's
// products rounded to single and truncated as fctiwz.

#include "tuning_machine/tuning_machine.h"

#include <climits>
#include <cmath>

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_makers.h"
#include "schema/car_specs.h"
#include "schema/career.h"
#include "schema/garage_screen.h"
#include "schema/race_event.h"
#include "schema/tuning_screen.h"
#include "tuning_pages/tuning_pages.h"
#include "tuning_screen/tuning_screen.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// fctiwz as the recompiled code does it: NaN 0x80000000, from INT_MAX
// saturated, below INT_MIN 0x80000000 (cvttsd2si).
inline int32_t TruncateWord(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value < -2147483648.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

constexpr uint32_t kDegrees = 0x82002244;  // the float the vinyl angles are scaled by

// SetUp's callees through the guest (their overrides).
constexpr uint32_t kCarInit = 0x820CD5D8;       // Car::Init(car)
constexpr uint32_t kCarRefresh = 0x820C4998;    // Car::Refresh(car)
constexpr uint32_t kMemcpy = 0x825A7410;        // memcpy(to, from, bytes)
constexpr uint32_t kGetCarParts = 0x820C9E00;   // Params::GetCarParts(out, car)
constexpr uint32_t kCountBodyKits = 0x820CBD20; // CountBodyKits(parts)

// The screen (tuning-screen.toml) and the selected garage car's car data
// (its +212; car-makers.toml's CarMakersCar, the head of a GarageCar).
using Screen = schema::TuningScreen;
using Car = schema::CarMakersCar;

}  // namespace

void LoadInteriorSlots(uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821DE280 TuningScreen::LoadInteriorSlots");
  const Car car(tuning_screen::SelectedGarageCar(screen) + 212);
  if (int32_t(car.car_id()) == 10) {
    Screen(screen).set_interior_slot_available_4(0);
  }
}

uint64_t LoadCurrentChoices(uint32_t screen, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821DA4B0 TuningScreen::LoadCurrentChoices");
  const Screen ts(screen);
  // The original's frame (its address only r3's leftover since E5 step 6:
  // its tables are the local `table` below, and no call is made with it).
  const uint32_t frame = uint32_t(r1) - kLoadCurrentChoicesFrame;
  // The frame's words +80 to +108 (the mirror table, its first four then
  // the sticker tens' 3, 2, 1, 0). An index past them (never in play)
  // reads the stack's word there, as the original.
  uint32_t table[8] = {};
  auto table_word = [&](uint32_t index) {
    return index < 8 ? table[index] : Load(frame + 80 + index * 4);
  };
  const Car car(tuning_screen::SelectedGarageCar(screen) + 212);
  // The garage car's words CarMakersCar doesn't name (car-makers.toml).
  auto at = [&](uint32_t offset) { return Load(car.address + offset); };
  const uint32_t tables = ts.part_tables_address();  // the screen's +34104
  // The power and power train tables' rows by the part set (read again
  // after the first store, as the original): +34304 and +34804.
  Store(tables + 200 + ts.price_class() * 4, car.engine_stage());
  Store(tables + 700 + ts.price_class() * 4, car.power_part());
  ts.set_power2_level(at(1672));
  ts.set_power2_type(car.cooling());
  ts.set_power3_level(at(1676));
  ts.set_train0_level(at(1680));
  ts.set_train1_level(at(1684));
  ts.set_train2_level(at(1688));
  ts.set_train3_level(car.power_train4());
  // The gear set: 0 3, 4 1, 5 4, 6 2, others 0.
  switch (int32_t(car.power_train3())) {
    case 0: ts.set_train3_type(3); break;
    case 4: ts.set_train3_type(1); break;
    case 5: ts.set_train3_type(4); break;
    case 6: ts.set_train3_type(2); break;
    default: ts.set_train3_type(0); break;
  }
  {
    const uint32_t low = at(1704);
    if (int32_t(low) == 0) {
      ts.set_train4_level(0);
    } else {
      ts.set_train4_level(at(1700) * 3 + low);
    }
  }
  ts.set_body_choice(car.body_level());
  ts.set_tire_both_choice(0);
  Store(tables + 1720, 0);  // +35824
  ts.set_tire_front_choice(car.front_tire_grade());
  if (int32_t(car.front_tire_grade()) == 0) {
    ts.set_tire_front_option(0);
  } else {
    ts.set_tire_front_option(at(1712) - 1);
  }
  ts.set_tire_rear_choice(car.rear_tire_grade());
  if (int32_t(car.rear_tire_grade()) == 0) {
    ts.set_tire_rear_option(0);
  } else {
    ts.set_tire_rear_option(at(1720) - 1);
  }
  {
    const bool plain = int32_t(ts.aero3_type_remap()) == 0;
    ts.set_aero0_type(car.aero_item0());
    ts.set_aero1_type(car.aero_item1());
    ts.set_aero2_type(car.aero_item2());
    const int32_t v = int32_t(car.aero_item3());
    if (plain) {
      ts.set_aero3_type(uint32_t(v));
    } else if (v == 0) {
      ts.set_aero3_type(0);
    } else if (v > 0 && v < 11) {
      ts.set_aero3_type(uint32_t(v + 1));
    } else {
      ts.set_aero3_type(1);
    }
  }
  ts.set_aero4_type(car.bonnet());
  switch (int32_t(car.bonnet_color_kind())) {
    case 0:
      ts.set_aero4_color_kind(0);
      ts.set_aero4_color(0);
      break;
    case 1:
      ts.set_aero4_color(0);
      ts.set_aero4_color_kind(1);
      break;
    case 2:
      ts.set_aero4_color(1);
      ts.set_aero4_color_kind(1);
      break;
    default: break;
  }
  ts.set_aero5_type(car.aero_item6());
  ts.set_aero6_type(car.aero_item7());
  ts.set_aero7_type(car.aero_item8());
  ts.set_aero8_type(car.aero_item9());
  ts.set_aero9_type(car.aero_kit());
  ts.set_saved_wheel_front(at(1728));
  ts.set_saved_wheel_4(at(1732));
  ts.set_saved_wheel_rear(at(1736));
  ts.set_saved_wheel_5(at(1740));
  ts.set_exterior1_choice(car.exterior_item1());
  ts.set_exterior2_choice(uint32_t(int32_t(int8_t(guest::LoadU8(car.setting_bytes_address())))));
  ts.set_exterior3_choice(at(1796));
  // The paint and lights: the car's own words when its paint is -2, else
  // the car spec's paint slot for it.
  const int32_t paint = int16_t(car.paint_pick());
  if (paint == -2) {
    ts.set_saved_paint_0(car.paint_hue());
    ts.set_saved_paint_1(car.paint_saturation());
    ts.set_saved_paint_2(car.paint_value());
    ts.set_saved_paint_3(uint32_t(int32_t(int16_t(car.paint_kind()))));
  } else {
    const schema::CarSpec spec(car_specs::GetCarSpec(car.car_id()));
    const schema::CarPaintSlot slot(spec.paints_address() + 8 + uint32_t(paint) * 20);
    ts.set_saved_paint_0(slot.hue0());
    ts.set_saved_paint_1(slot.saturation0());
    ts.set_saved_paint_2(slot.value0());
    ts.set_saved_paint_3(slot.type());
  }
  // The three vinyl layers (the car's 20-byte layers from +1588, the
  // screen's light kit words a = +30580 + 4i and b = +30660 + 4i).
  const double degrees = double(guest::LoadFloat(kDegrees));
  auto angle = [&](uint32_t field) {
    const double value = double(float(double(guest::LoadFloat(field)) * degrees));
    return uint32_t(TruncateWord(value));
  };
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t e = car.layer0_a_address() + 20 * i;
    const uint32_t a = ts.light_kit_a_address() + 4 * i;
    const uint32_t b = ts.light_kit_b_address() + 4 * i;
    const int32_t kind = int32_t(Load(e));
    if (kind == 0) {
      Store(a + 0, 0);
      Store(b + 0, 0);
      Store(a + 16, 180);
      Store(a + 32, 180);
      Store(a + 48, 180);
      Store(b + 16, 0);
      Store(b + 32, 180);
    } else if (kind == 1) {
      Store(a + 0, 1);
      Store(b + 0, 0);
      Store(a + 16, angle(e + 8));
      Store(a + 32, angle(e + 12));
      const double value = double(float(double(guest::LoadFloat(e + 16)) * degrees));
      Store(b + 16, 0);
      Store(b + 32, 180);
      Store(a + 48, uint32_t(TruncateWord(value)));
    } else if (kind >= 2 && kind <= 11) {
      Store(a + 0, 0);
      Store(b + 0, 1);
      Store(a + 16, 180);
      Store(a + 32, 180);
      Store(a + 48, 180);
      Store(a + 64, 0);
      Store(b + 16, Load(e) - 2);
      Store(b + 32, angle(e + 16));
      Store(b + 48, Load(e + 4));
    }
  }
  // The mirror table in the frame (+80 to +108), indexed unchecked by the
  // first layer's +4 (in play 0 to 7) for eight words.
  {
    static constexpr uint32_t kTable[][2] = {{80, 0}, {88, 4},  {92, 2},  {96, 1},
                                             {84, 6}, {100, 3}, {108, 5}, {104, 7}};
    for (const auto& [offset, value] : kTable) {
      table[(offset - 80) / 4] = value;
    }
    const uint32_t a = ts.light_kit_a_address(), b = ts.light_kit_b_address();
    // +30644 to +30656, +30708 to +30720.
    for (const uint32_t field : {a + 64, a + 68, a + 72, a + 76, b + 48,
                                 ts.unknown_30712_address(), ts.unknown_30716_address(),
                                 ts.unknown_30720_address()}) {
      Store(field, table_word(car.layer0_b()));
    }
  }
  // The neon: v 0 0; 1 and 2 by w 0 to 3: 3, 1, 0, 2; 3 by w 0 to 2: 2, 0,
  // 1; 4 by w 0 to 3: 2, 0, 1, 3; others nothing.
  ts.set_interior0_choice(car.interior_item0());
  {
    const uint32_t v = car.interior_item0();
    int64_t value = -1;
    if (v == 0) {
      value = 0;
    } else if (v <= 4) {
      const uint32_t w = car.interior_item1();
      if (v <= 2) {
        static constexpr int64_t kValues[4] = {3, 1, 0, 2};
        value = w < 4 ? kValues[w] : -1;
      } else if (v == 3) {
        static constexpr int64_t kValues[3] = {2, 0, 1};
        value = w < 3 ? kValues[w] : -1;
      } else {
        static constexpr int64_t kValues[4] = {2, 0, 1, 3};
        value = w < 4 ? kValues[w] : -1;
      }
    }
    if (value >= 0) {
      ts.set_interior0_level(uint32_t(value));
    }
  }
  ts.set_interior1_choice(car.interior_item2());
  ts.set_interior2_choice(car.interior_item3());
  {
    const uint32_t u = car.accessory();
    if (u < 4) {
      ts.set_interior_choice(3 - u);
    }
    const int32_t x = int32_t(ts.interior_choice());
    if (x == 0) {
      ts.set_interior_choice_word(0);
    } else {
      const uint32_t v = car.accessory_variant();
      if (x == 1 && int32_t(v) == 3) {
        ts.set_interior_choice_word(4);
      } else if (x == 1 && int32_t(v) == 4) {
        ts.set_interior_choice_word(3);
      } else {
        ts.set_interior_choice_word(v);
      }
    }
  }
  // The five sticker layers (the car's choices from +1824, the screen's
  // choice codes +33800 + 4i), the tens from the frame's table (+80 to
  // +92: 3, 2, 1, 0) indexed unchecked by the layer's +8 - 1 (reading the
  // mirror table's words past it).
  table[0] = 3;
  table[1] = 2;
  table[2] = 1;
  table[3] = 0;
  for (uint32_t i = 0; i < 5; i++) {
    const schema::CarMakersCarChoice choice(car.choice0_a_address() + 12 * i);
    const uint32_t layer = ts.choice_code_0_address() + 4 * i;
    const uint32_t v = choice.c();
    if (int32_t(v) == 0) {
      Store(layer, 0xFFFFFFFFu);
      continue;
    }
    auto tens = [&] { return table_word(v - 1) * 10; };
    const int32_t kind = int32_t(choice.a());
    const uint32_t unit = choice.b();
    if (kind == 1) {
      if (unit == 0) {
        Store(layer, tens() + 1);
      } else if (unit == 1) {
        Store(layer, tens());
      } else if (unit == 2) {
        Store(layer, tens() + 2);
      }
    } else if (kind == 2) {
      if (unit == 0) {
        Store(layer, tens() + 4);
      } else if (unit == 1) {
        Store(layer, tens() + 3);
      }
    }
  }
  // r3: the frame's +80 (r1's high word kept).
  return ((r1 & 0xFFFFFFFF00000000ull) | frame) + 80;
}

uint64_t SetUp(uint64_t r3, uint64_t r1, bool native_specs, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821E1B48 TuningScreen::SetUp");
  const uint32_t s = uint32_t(r3);
  const Screen ts(s);
  const uint32_t frame = Frame(uint32_t(r1), kSetUpFrame);
  const uint32_t car = frame + 176;
  // The save, the career's pick (s +8 to +27), the money and the day (at
  // most 999; its weekday by signed division).
  const schema::CareerSave save(vehicle_accessors::SaveDataOf(0));
  ts.set_save_data(save.address);
  for (uint32_t i = 0; i < 5; i++) {
    Store(ts.career_pick_address() + 4 * i, Load(save.pick_address() + 4 * i));
  }
  ts.set_money(save.money());
  const int32_t day = int32_t(save.day());
  ts.set_day(uint32_t(day > 999 ? 999 : day));
  ts.set_weekday(uint32_t(day - day / 7 * 7));
  // The selected garage car in the frame's car object.
  const Car garage(tuning_screen::SelectedGarageCar(s) + 212);
  calls.Call(kCarInit, frame, 0x821E1C08, car);
  if (!native::Buffers_Memcpy(car, garage.address, 1884, 0x821E1C18)) {
    calls.Call(kMemcpy, frame, 0x821E1C18, car, garage.address, 1884);
  }
  calls.Call(kCarRefresh, frame, 0x821E1C20, car);
  const schema::CarSpecCopies p(vehicle_accessors::ObjectRecord1888(car));
  ts.set_spec_byte_69(p.turbo());
  ts.set_spec_byte_130(p.unknown_130());
  // The price class.
  uint32_t set = 0;
  if (int32_t(garage.car_id()) == 8) {
    set = 4;
  } else {
    const uint32_t kind = p.turbo();
    if (kind == 1) {
      set = p.engine_type() == 7 ? 2 : 0;
    } else if (kind == 0) {
      set = p.engine_type() == 7 ? 3 : 1;
    }
  }
  ts.set_price_class(set);
  // The car's parts (the selected car found again) copied to frame + 80;
  // the body kits counted.
  const uint32_t id = Car(tuning_screen::SelectedGarageCar(s) + 212).car_id();
  const uint32_t parts_out = frame + 128, parts = frame + 80;
  uint32_t parts_at = parts_out;
  if (native_specs) {
    uint32_t words[9];
    car_specs::GetCarParts(id, words);
    for (uint32_t i = 0; i < 9; i++) {
      Store(parts_out + 4 * i, words[i]);
    }
  } else {
    parts_at = uint32_t(calls.Call(kGetCarParts, frame, 0x821E1D08, parts_out, id));
  }
  if (!native::Buffers_Memcpy(parts, parts_at, 36, 0x821E1D18)) {
    calls.Call(kMemcpy, frame, 0x821E1D18, parts, parts_at, 36);
  }
  const uint32_t kits = native_specs ? car_specs::CountBodyKits(parts)
                                     : uint32_t(calls.Call(kCountBodyKits, frame, 0x821E1D20, parts));
  ts.set_aero9_third_type(int32_t(kits) == 1 ? 0 : 1);
  ts.set_interior_slot_available_0(1);
  ts.set_interior_slot_available_1(1);
  ts.set_interior_slot_available_2(1);
  ts.set_interior_slot_available_3(1);
  ts.set_interior_slot_available_4(1);
  LoadInteriorSlots(s);
  ts.set_aero3_type_remap(p.spec_120() != 0 ? 1 : 0);
  LoadCurrentChoices(s, (r1 & 0xFFFFFFFF00000000ull) | frame);
  // The aero items available, then set by the body kit.
  for (uint32_t item = 0; item < 10; item++) {
    Store(ts.aero_available_0_address() + 4 * item, 1);
  }
  const tuning_screen::GuestCall call = [&calls](uint32_t function,
                                                 std::initializer_list<uint64_t> args,
                                                 uint32_t at, uint32_t lr) {
    uint64_t words[8] = {};
    uint32_t i = 0;
    for (const uint64_t arg : args) {
      if (i < 8) {
        words[i++] = arg;
      }
    }
    return calls.CallArgs(function, at, lr, words);
  };
  tuning_pages::SetAeroAvailability(call, s, frame, native_specs);
  LoadOwnedParts(s, frame);
  SetPrices(s, frame);
  LoadUnlockedParts(s);
  return r3;
}

}  // namespace tuning_machine
