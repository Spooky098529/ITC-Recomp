// The tuning screen's writes into the selected garage car
// (docs/functions/tuning-screen.md, docs/known-data.md's "garage car"): see
// tuning_screen.h. Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt): the vinyl layers' angles are the translated code's
// int-to-float conversions and products rounded to single.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/race_event.h"

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store8(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU8(base, address, uint8_t(value));
}
void StoreS64(uint8_t* base, uint32_t address, int64_t value) {
  Store(base, address, uint32_t(uint64_t(value) >> 32));
  Store(base, address + 4, uint32_t(value));
}
// divw: 0 for a zero divisor or INT32_MIN / -1, as the translated code.
int32_t Divw(int32_t a, int32_t b) {
  return (b != 0 && !(a == INT32_MIN && b == -1)) ? a / b : 0;
}

constexpr uint32_t kZero = 0x82000A34;       // 0.0f
constexpr uint32_t kPerDegree = 0x820021FC;  // 1/360

}  // namespace

uint32_t SelectedGarageCar(uint8_t* base, uint32_t screen) {
  const schema::SaveData save(base, TS(base, screen).save_data().address);
  const uint32_t nodes = save.garage_address();
  uint32_t n = save.selected_car();
  uint32_t node =
      uint32_t(int32_t(int16_t(schema::CareerGarageList(base, nodes).head()))) * 4 + nodes;
  while (n != 0) {
    node = uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) * 4 + nodes;
    n--;
  }
  const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
  return uint32_t(uint64_t(slot) * 1968) + nodes;
}

uint64_t StoreToGarageCar(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DCC28 TuningScreen::StoreToGarageCar");
  const uint32_t s = uint32_t(r3);
  const uint32_t c = SelectedGarageCar(base, s) + 212;
  const schema::CarMakersCar car(base, c);
  car.set_engine_stage(Load(base, s + (TS(base, s).price_class() + 8576) * 4));
  car.set_power_part(Load(base, s + (TS(base, s).price_class() + 8701) * 4));
  Store(base, c + 1672, TS(base, s).power2_level());
  car.set_cooling(TS(base, s).power2_type());
  Store(base, c + 1676, TS(base, s).power3_level());
  Store(base, c + 1680, TS(base, s).train0_level());
  Store(base, c + 1684, TS(base, s).train1_level());
  Store(base, c + 1688, TS(base, s).train2_level());
  car.set_power_train4(TS(base, s).train3_level());
  // The gear set: 0 by the car's id.
  uint32_t gears = 0;
  switch (TS(base, s).train3_type()) {
    case 0: {
      const uint32_t id = schema::CarMakersCar(base, SelectedGarageCar(base, s) + 212).car_id();
      gears = id == 4 ? 6 : (id == 7 || id == 12 || id == 14) ? 5 : 4;
      break;
    }
    case 1: gears = 4; break;
    case 2: gears = 6; break;
    case 3: gears = 0; break;
    case 4: gears = 5; break;
    default: gears = 0; break;
  }
  car.set_power_train3(gears);
  if (int32_t(TS(base, s).train4_level()) == 0) {
    Store(base, c + 1704, 0);
    Store(base, c + 1700, 0);
  } else {
    const int32_t v = int32_t(TS(base, s).train4_level()) - 1;
    Store(base, c + 1704, uint32_t(v - Divw(v, 3) * 3 + 1));
    Store(base, c + 1700, uint32_t(Divw(int32_t(TS(base, s).train4_level()) - 1, 3)));
  }
  car.set_body_level(TS(base, s).body_choice());
  car.set_front_tire_grade(TS(base, s).tire_front_choice());
  Store(base, c + 1712,
        int32_t(TS(base, s).tire_front_choice()) == 0 ? 0 : TS(base, s).tire_front_option() + 1);
  car.set_rear_tire_grade(TS(base, s).tire_rear_choice());
  Store(base, c + 1720,
        int32_t(TS(base, s).tire_rear_choice()) == 0 ? 0 : TS(base, s).tire_rear_option() + 1);
  car.set_aero_item0(TS(base, s).aero0_type());
  car.set_aero_item1(TS(base, s).aero1_type());
  car.set_aero_item2(TS(base, s).aero2_type());
  {
    const bool plain = int32_t(TS(base, s).aero3_type_remap()) == 0;
    const int32_t v = int32_t(TS(base, s).aero3_type());
    if (plain) {
      car.set_aero_item3(uint32_t(v));
    } else if (v == 0) {
      car.set_aero_item3(0);
    } else if (v >= 2 && v <= 5) {
      car.set_aero_item3(uint32_t(v - 1));
    } else if (v == 1) {
      car.set_aero_item3(0xFFFFFFFFu);
    }
  }
  car.set_bonnet(TS(base, s).aero4_type());
  {
    const int32_t v = int32_t(TS(base, s).aero4_color_kind());
    if (v == 0) {
      car.set_bonnet_color_kind(0);
    } else if (v == 1) {
      const int32_t w = int32_t(TS(base, s).aero4_color());
      if (w == 0) {
        car.set_bonnet_color_kind(1);
      } else if (w == 1) {
        car.set_bonnet_color_kind(2);
      }
    }
  }
  car.set_aero_item6(TS(base, s).aero5_type());
  car.set_aero_item7(TS(base, s).aero6_type());
  car.set_aero_item8(TS(base, s).aero7_type());
  car.set_aero_item9(TS(base, s).aero8_type());
  car.set_aero_kit(TS(base, s).aero9_type());
  Store(base, c + 1728, TS(base, s).saved_wheel_front());
  Store(base, c + 1732, TS(base, s).saved_wheel_4());
  Store(base, c + 1736, TS(base, s).saved_wheel_rear());
  Store(base, c + 1740, TS(base, s).saved_wheel_5());
  car.set_exterior_item1(TS(base, s).exterior1_choice());
  Store8(base, car.setting_bytes_address(), TS(base, s).exterior2_choice());
  Store(base, c + 1796, TS(base, s).exterior3_choice());
  car.set_paint_hue(uint16_t(TS(base, s).saved_paint_0()));
  car.set_paint_saturation(uint8_t(TS(base, s).saved_paint_1()));
  car.set_paint_value(uint8_t(TS(base, s).saved_paint_2()));
  if (int32_t(TS(base, s).paint_kept()) != 0) {
    const uint32_t word = TS(base, s).saved_paint_3();
    const uint32_t old = car.body_class();
    car.set_paint_pick(uint16_t(0xFFFE));
    car.set_paint_kind(uint16_t(word));
    if (old == 0xFFFF) {
      car.set_body_class(uint16_t(0xFFFE));
    }
    car.set_unknown_2(uint16_t(0xFFFE));
    car.set_paint_pick(uint16_t(0xFFFE));
  }
  // The three vinyl layers (screen s + 30580 + 4i, car c + 1588 + 20i). The
  // original has no frame: its tables and conversion temporaries are below
  // `sp`, under its register saves (sp - 64 on).
  const float zero = guest::LoadFloat(base, kZero);
  const double per_degree = double(guest::LoadFloat(base, kPerDegree));
  auto angle = [&](uint32_t temp, uint32_t field) {
    const int64_t v = int32_t(Load(base, field));
    StoreS64(base, sp + temp, v);
    return float(double(float(double(v))) * per_degree);
  };
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t layer = TS(base, s).light_kit_a_address() + 4 * i;
    const uint32_t e = car.layer0_a_address() + 20 * i;
    const int32_t kind = int32_t(Load(base, layer));
    const uint32_t table[8] = {0, 4, 3, 5, 2, 7, 1, 6};
    for (uint32_t k = 0; k < 8; k++) {
      Store(base, sp - 96 + 4 * k, table[k]);
    }
    if (kind == 0 && int32_t(Load(base, layer + 80)) == 0) {
      guest::StoreFloat(base, e + 8, zero);
      Store(base, e + 0, 0);
      guest::StoreFloat(base, e + 12, zero);
      guest::StoreFloat(base, e + 16, zero);
    } else if (kind == 1 && int32_t(Load(base, layer + 80)) == 0) {
      Store(base, e + 0, 1);
      guest::StoreFloat(base, e + 8, angle(uint32_t(-144), layer + 16));
      guest::StoreFloat(base, e + 12, angle(uint32_t(-136), layer + 32));
      guest::StoreFloat(base, e + 16, angle(uint32_t(-128), layer + 48));
    } else if (kind == 0 && int32_t(Load(base, layer + 80)) == 1) {
      const uint32_t word = Load(base, layer + 96);
      guest::StoreFloat(base, e + 8, zero);
      guest::StoreFloat(base, e + 12, zero);
      Store(base, e + 0, word + 2);
      guest::StoreFloat(base, e + 16, angle(uint32_t(-112), layer + 112));
    }
    // The mirror table, indexed unchecked (in play 0 to 7).
    Store(base, e + 4, Load(base, sp - 96 + Load(base, layer + 64) * 4));
  }
  car.set_interior_item0(TS(base, s).interior0_choice());
  {
    const uint32_t v = TS(base, s).interior0_choice();
    const uint32_t w = TS(base, s).interior0_level();
    // 1 and 2: 2, 1, 3, 0 for w 0 to 3; 3: 1, 2, 0; 4: 1, 2, 0, 3; 0: 0.
    int64_t value = -1;
    if (v == 0) {
      value = 0;
    } else if (v == 1 || v == 2) {
      static constexpr int64_t kValues[4] = {2, 1, 3, 0};
      value = w < 4 ? kValues[w] : -1;
    } else if (v == 3) {
      static constexpr int64_t kValues[3] = {1, 2, 0};
      value = w < 3 ? kValues[w] : -1;
    } else if (v == 4) {
      static constexpr int64_t kValues[4] = {1, 2, 0, 3};
      value = w < 4 ? kValues[w] : -1;
    }
    if (value >= 0) {
      car.set_interior_item1(uint32_t(value));
    }
  }
  car.set_interior_item2(TS(base, s).interior1_choice());
  car.set_interior_item3(TS(base, s).interior2_choice());
  {
    const uint32_t u = TS(base, s).interior_choice();
    if (u < 4) {
      car.set_accessory(3 - u);
    }
    const int32_t x = int32_t(car.accessory());
    if (x == 3) {
      car.set_accessory_variant(0);
    } else {
      const uint32_t v = TS(base, s).interior_choice_word();
      if (x == 2 && int32_t(v) == 3) {
        car.set_accessory_variant(4);
      } else if (x == 2 && int32_t(v) == 4) {
        car.set_accessory_variant(3);
      } else {
        car.set_accessory_variant(v);
      }
    }
  }
  // The five sticker layers (screen +33800 + 4i, car c + 1824 + 12i), from
  // the tables below `sp`; the tens index those at sp - 128 and the units 3
  // to 9 those at sp - 144, both unchecked (reading the conversion
  // temporaries and words never written, as the original).
  const uint32_t kTables[9][2] = {{uint32_t(-128), 3}, {uint32_t(-124), 2}, {uint32_t(-120), 1},
                                  {uint32_t(-116), 0}, {uint32_t(-112), 1}, {uint32_t(-108), 0},
                                  {uint32_t(-104), 2}, {uint32_t(-144), 1}, {uint32_t(-140), 0}};
  for (const auto& [offset, value] : kTables) {
    Store(base, sp + offset, value);
  }
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t layer = TS(base, s).choice_code_0_address() + 4 * i;
    const uint32_t e = car.choice0_a_address() + 12 * i;
    const int32_t v = int32_t(Load(base, layer));
    if (v == -1) {
      Store(base, e + 0, 0);
      Store(base, e + 8, 0);
      continue;
    }
    const int32_t tens = Divw(v, 10);
    const int32_t d = v - tens * 10;
    // r3: the tens times 10, from the 32-bit quotient (all 64 bits).
    r3 = uint64_t(uint32_t(tens)) * 10;
    if (d < 0 || d > 2) {
      Store(base, e + 0, 2);
      Store(base, e + 4, Load(base, sp - 144 + uint32_t(d - 3) * 4));
    } else {
      Store(base, e + 0, 1);
      Store(base, e + 4, Load(base, sp - 112 + uint32_t(d) * 4));
    }
    Store(base, e + 8, Load(base, sp - 128 + uint32_t(tens) * 4) + 1);
  }
  return r3;
}

void EquipChoice(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821DF448 TuningScreen::EquipChoice");
  const uint32_t c = SelectedGarageCar(base, screen) + 212;
  const schema::CarMakersCar car(base, c);
  const int32_t state = int32_t(TS(base, screen).state());
  const int32_t m = int32_t(TS(base, screen).category());
  const uint32_t p = uint32_t(TS(base, screen).item());
  auto level = [&] { return uint32_t(uint32_t(TS(base, screen).level())); };
  auto type = [&] { return uint32_t(uint32_t(TS(base, screen).type())); };
  if (state == 5) {
    if (m == 0) {
      static constexpr uint32_t kFields[4] = {1656, 1664, 1672, 1676};
      if (p < 4) {
        Store(base, c + kFields[p], level());
      }
    } else if (m == 1) {
      static constexpr uint32_t kFields[4] = {1680, 1684, 1688, 1696};
      if (p < 4) {
        Store(base, c + kFields[p], level());
      } else if (p == 4) {
        if (level() == 0) {
          Store(base, c + 1704, 0);
          Store(base, c + 1700, 0);
        } else {
          const int32_t b = int32_t(level()) - 1;
          Store(base, c + 1704, uint32_t(b - Divw(b, 3) * 3 + 1));
          Store(base, c + 1700, uint32_t(Divw(int32_t(level()) - 1, 3)));
        }
      }
    } else if (m == 2) {
      car.set_body_level(level());
    } else if (m == 3) {
      if (p == 0) {
        car.set_front_tire_grade(level());
        car.set_rear_tire_grade(level());
      } else if (p == 1) {
        car.set_front_tire_grade(level());
      } else if (p == 2) {
        car.set_rear_tire_grade(level());
      }
    }
  } else if (state == 6) {
    if (m == 0) {
      if (p == 2) {
        car.set_cooling(type());
      }
    } else if (m == 4 && p <= 9) {
      static constexpr uint32_t kFields[10] = {1744, 1748, 1752, 0,    0,
                                               1768, 1772, 1776, 1780, 1784};
      if (p == 3) {
        const bool plain = int32_t(TS(base, screen).aero3_type_remap()) == 0;
        const uint32_t t = type();
        if (plain || t == 0) {
          car.set_aero_item3(t);
        } else if (t >= 2 && t <= 5) {
          car.set_aero_item3(t - 1);
        } else if (t == 1) {
          car.set_aero_item3(0xFFFFFFFFu);
        }
      } else if (p == 4) {
        car.set_bonnet(type());
        car.set_bonnet_color_kind(0);
      } else {
        Store(base, c + kFields[p], type());
      }
    }
  } else if (state == 7 || state == 8) {
    if (m == 4 && p == 4) {
      const uint32_t pick = state == 7 ? uint32_t(TS(base, screen).bonnet_choice())
                                       : uint32_t(TS(base, screen).option());
      if (pick == 0) {
        car.set_bonnet_color_kind(state == 7 ? 0 : 1);
      } else if (pick == 1) {
        car.set_bonnet_color_kind(state == 7 ? 1 : 2);
      }
    }
  }
}

}  // namespace tuning_screen
