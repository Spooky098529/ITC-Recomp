// The race HUD object's dashboard digits, needles and set-ups
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/car_physics.h"
#include "schema/race_event.h"
#include "schema/race_hud_object.h"
#include "schema/race_setup.h"
#include "schema/camera_modes.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_hud_object {

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

using Hud = schema::RaceHud;
using G = schema::RaceHudObjectGlobals;

uint32_t Dashboard(uint32_t hud) { return Hud(hud).dashboard().address; }
uint32_t Timer(uint32_t hud) { return Hud(hud).timer().address; }

double LoadDouble(uint32_t address) {
  return std::bit_cast<double>(uint64_t(guest::LoadU32(address)) << 32 |
                               guest::LoadU32(address + 4));
}

// The digits' 10.0 (the double at 0x8200F728) for PowInt.
constexpr uint32_t kTen = 0x8200F728;

// The digits' loop the speed and the distance share: node `node`'s
// picture (+108) for the digit of `value` (digit + 1), or 0 for a leading
// zero (`value` below 10 to the `place`, PowInt's result through fctiwz,
// the original's frame + 80 a local here; the value then isn't divided).
// f1 is PowInt's when it ran.
uint32_t Digit(uint32_t node, int32_t& value, int32_t place, double* f1) {
  const int32_t digit = value - (value / 10) * 10;
  if (digit == 0 && place > 0) {
    *f1 = PowInt(LoadDouble(kTen), place);
    const int32_t bound = Fctiwz(*f1);
    if (value < bound) {
      guest::StoreU32(node + 108, 0);
      return node;
    }
  }
  guest::StoreU32(node + 108, uint32_t(digit + 1));
  value /= 10;
  return node;
}

}  // namespace

uint32_t SetTach(uint32_t hud, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212D58 RaceHud::SetTach");
  const Hud h(hud);
  CurveEval(h.tach_curve_address(), value);
  const uint32_t node = ui_movies::FindNode(Dashboard(hud), 0, 7);
  Stfs(node + 36, Fadds(double(h.tach_output()), Lfs(0x8200F720)));
  return node;
}

uint32_t SetBoost(uint32_t hud, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212E18 RaceHud::SetBoost");
  const Hud h(hud);
  CurveEval(h.boost_curve_address(), value);
  const uint32_t node = ui_movies::FindNode(Dashboard(hud), 0, 5);
  Stfs(node + 36, double(h.boost_output()));
  return node;
}

uint32_t SetSpeed(uint32_t hud, uint32_t speed, uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_82214260 RaceHud::SetSpeed");
  if (speed > 999) {
    return hud;
  }
  const uint32_t digits = ui_movies::FindNode(Dashboard(hud), kNone, 9);
  int32_t i = int32_t(guest::LoadU32(digits + 104)) - 1;
  if (i < 0) {
    return digits;
  }
  int32_t value = int32_t(speed);
  uint32_t node = 0;
  for (; i >= 0; i--) {
    node = ui_movies::FindNode(Dashboard(hud), kNone,
                               guest::LoadU32(digits + 100) + uint32_t(i));
    const int32_t place = int32_t(guest::LoadU32(digits + 104) - uint32_t(i)) - 1;
    Digit(node, value, place, f1);
  }
  return node;
}

uint32_t SetTime(uint32_t hud, uint32_t time, uint32_t sp) {
  return SetTimeValue(hud, guest::LoadU32(time));
}

uint32_t SetTimeValue(uint32_t hud, uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_82213750 RaceHud::SetTime");
  if ((packed & 0x7FFF0000) > 0x003B0000) {
    packed = 0x003BEFE7 | (packed & 0x80000000);
  }
  uint32_t node = 0;
  // Each part's digits from the last node down (r30 + 14), the value
  // divided by 10 after each.
  auto digits = [&](int32_t value, int32_t first, int32_t last) {
    for (int32_t k = first; k >= last; k--) {
      const int32_t digit = value - (value / 10) * 10;
      node = ui_movies::FindNode(Timer(hud), kNone, uint32_t(k + 14));
      value /= 10;
      guest::StoreU32(node + 108, uint32_t(digit + 1));
    }
  };
  int32_t minutes = int32_t((packed >> 16) & 0x7FFF);
  digits(minutes > 59 ? 59 : minutes, 1, 0);
  int32_t seconds = int32_t((packed >> 10) & 0x3F);
  digits(seconds > 59 ? 59 : seconds, 3, 2);
  int32_t milliseconds = int32_t(packed & 0x3FF);
  digits(milliseconds > 999 ? 999 : milliseconds, 6, 4);
  return node;
}

uint32_t SetDistance(uint32_t hud, uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_82214A50 RaceHud::SetDistance");
  double value = *f1;
  double bound = Lfs(kZero);
  if (value < bound) {
    value = bound;
  }
  bound = Lfs(0x8200F740);
  if (value > bound) {
    value = bound;
  }
  *f1 = value;
  const int32_t whole = Fctiwz(value);
  const double tenths = Fmuls(value, Lfs(0x82001FD8));
  // The original's frame + 84 (the whole) and + 80 (the tenths, then the
  // hundredths) as locals.
  const double hundredths = Fmuls(value, Lfs(0x82002A0C));
  const int32_t tenths_word = Fctiwz(tenths);
  const int32_t hundredths_word = Fctiwz(hundredths);
  int32_t decimal = tenths_word - (tenths_word / 10) * 10;
  int32_t rest;
  if (hundredths_word - (hundredths_word / 10) * 10 > 0 && ++decimal == 10) {
    decimal = 0;
    rest = whole + 1;
  } else {
    rest = whole;
  }
  uint32_t node = 0;
  for (int32_t k = 2; k >= 0; k--) {
    node = ui_movies::FindNode(Timer(hud), kNone, uint32_t(k + 9));
    if (k == 2) {
      guest::StoreU32(node + 108, uint32_t(decimal + 1));
      continue;
    }
    Digit(node, rest, 1 - k, f1);
  }
  return node;
}

uint32_t Reset(uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822121C0 RaceHud::Reset");
  const uint32_t group = ui_movies::FindNode(Dashboard(hud), kNone, 1);
  const double zero = Lfs(kZero);
  if (int32_t(guest::LoadU32(group + 104)) > 0) {
    int32_t i = 0;
    do {
      const uint32_t child = ui_movies::FindNode(Dashboard(hud), kNone, guest::LoadU32(group + 100) + uint32_t(i));
      Stfs(child + 68, zero);
      i++;
    } while (i < int32_t(guest::LoadU32(group + 104)));
  }
  const uint32_t node5 = ui_movies::FindNode(Dashboard(hud), kNone, 5);
  Stfs(node5 + 68, zero);
  const Hud h(hud);
  h.set_recording_full(0);
  h.set_item_shown(0xFFFFFFFFu);
  const uint32_t mirror = ui_movies::FindNode(Dashboard(hud), kNone, 19);
  guest::StoreU32(mirror + 80, 0);
  return mirror;
}

uint32_t ShowGauge(uint32_t hud, uint32_t slot, uint32_t shown, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212D08 RaceHud::ShowGauge");
  const uint32_t digits = ui_movies::FindNode(Dashboard(hud), 0, slot + 12);
  const uint32_t label = ui_movies::FindNode(Dashboard(hud), 0, slot + 9);
  guest::StoreU32(digits + 80, shown);
  guest::StoreU32(label + 80, shown);
  return label;
}

uint32_t SetUpExtraGauges(uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82213DB0 RaceHud::SetUpExtraGauges");
  // The original's three flags (frame + 80 to + 88) and the pointers to
  // them (frame + 96 to + 104) as a local array, read by index.
  int32_t flags[3] = {};
  const Hud h(hud);
  const schema::Vehicle car = h.vehicle();
  // The car's five choices' third words (CarChoice.c), 12 bytes apart.
  uint32_t entry = schema::Car(car.car()).choice0_a_address();
  h.set_part_fitted(0);
  for (int k = 0; k < 5; k++, entry += schema::CarChoice::kSize) {
    switch (int32_t(schema::CarChoice(entry).c())) {
      case 1:
        h.set_part_fitted(1);
        break;
      case 2:
        flags[0] = 1;
        break;
      case 3:
        flags[1] = 1;
        break;
      case 4:
        flags[2] = 1;
        break;
      default:
        break;
    }
  }
  if (int32_t(schema::Car(car.car()).car_id()) == 10) {
    h.set_part_fitted(1);
  }
  // The slots from the last: the first flag set of the three (cleared), or -1.
  for (uint32_t slot = h.gauge_slot2_address(); slot != h.part_fitted_address(); slot -= 4) {
    guest::StoreU32(slot, 0xFFFFFFFFu);
    for (uint32_t i = 0; i < 3; i++) {
      if (flags[i] == 1) {
        flags[i] = 0;
        guest::StoreU32(slot, i);
        break;
      }
    }
  }
  const uint32_t frame = sp - 144;  // ShowGauge's stack pointer (it makes no frame)
  for (uint32_t i = 0; i < 3; i++) {
    ShowGauge(hud, i, guest::LoadU32(h.gauge_slot0_address() + 4 * i) < 3 ? 1 : 0,
              frame);
  }
  const uint32_t node = ui_movies::FindNode(Dashboard(hud), 0, 1);
  guest::StoreU32(node + 80, h.option_gauges() & h.part_fitted());
  return node;
}

uint32_t ApplyOptions(uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212768 RaceHud::ApplyOptions");
  const schema::SaveData options(vehicle_accessors::SaveDataOf(0));
  const Hud h(hud);
  const bool mirror = h.view_dashboard() != 0;
  // The four HUD options (the save data's +0x1F85C to +0x1F868), word by word.
  h.set_option_node3(options.hud_option_0());
  h.set_option_node1(options.hud_option_1());
  h.set_option_gauges(options.hud_option_2());
  h.set_option_mirror(options.hud_option_3());
  if (mirror) {
    const uint32_t node = ui_movies::FindNode(Dashboard(hud), kNone, 4);
    guest::StoreU32(node + 80, h.option_mirror());
  }
  uint32_t node = ui_movies::FindNode(Dashboard(hud), kNone, 3);
  guest::StoreU32(node + 80, h.option_node3());
  node = ui_movies::FindNode(Dashboard(hud), 0, 3);
  guest::StoreU32(node + 80, h.option_gauges());
  node = ui_movies::FindNode(Dashboard(hud), 0, 4);
  guest::StoreU32(node + 80, h.option_gauges());
  node = ui_movies::FindNode(Dashboard(hud), 0, 1);
  {
    const uint32_t part = h.part_fitted();
    guest::StoreU32(node + 80, part & h.option_gauges());
  }
  node = ui_movies::FindNode(Dashboard(hud), kNone, 1);
  guest::StoreU32(node + 80, h.option_node1());
  return node;
}

uint32_t UpdateView(uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212B68 RaceHud::UpdateView");
  const Hud h(hud);
  const schema::Viewport viewport = G::viewports().first();
  h.set_view(viewport.view_mode());
  const uint32_t full = ui_movies::FindNode(Dashboard(hud), kNone, 0);
  const uint32_t cockpit = ui_movies::FindNode(Dashboard(hud), kNone, 4);
  const uint32_t view = h.view();
  if (h.option_mirror() != 0) {
    if (view >= 3) {
      return cockpit;
    }
    const uint32_t shown = view == 0 ? 0 : 1;
    if (view != 2) {
      guest::StoreU32(full + 80, view == 0 ? 1 : 0);
    } else {
      guest::StoreU32(full + 80, 1);
    }
    guest::StoreU32(cockpit + 80, shown);
    h.set_view_dashboard(shown);
    return cockpit;
  }
  h.set_view_dashboard(0);
  guest::StoreU32(cockpit + 80, 0);
  const uint32_t view2 = h.view();
  if (view2 < 3) {
    guest::StoreU32(full + 80, view2 == 1 ? 0 : 1);
  }
  return cockpit;
}

}  // namespace race_hud_object
