// The race HUD object's dashboard digits, needles and set-ups
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_hud_object {

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

void Frame(uint8_t* base, uint32_t sp, uint32_t bytes) { guest::StoreU32(base, sp - bytes, sp); }

uint32_t Dashboard(uint8_t* base, uint32_t hud) { return guest::LoadU32(base, hud + kHudDashboard); }
uint32_t Timer(uint8_t* base, uint32_t hud) { return guest::LoadU32(base, hud + kHudTimer); }

double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(uint64_t(guest::LoadU32(base, address)) << 32 |
                               guest::LoadU32(base, address + 4));
}

// The digits' 10.0 (the double at 0x8200F728) for PowInt.
constexpr uint32_t kTen = 0x8200F728;

// The digits' loop the speed and the distance share: node `node`'s
// picture (+108) for the digit of `value` (digit + 1), or 0 for a leading
// zero (`value` below 10 to the `place`, PowInt's result through fctiwz at
// frame + 80; the value then isn't divided). f1 is PowInt's when it ran.
uint32_t Digit(uint8_t* base, uint32_t node, int32_t& value, int32_t place, uint32_t frame,
               double* f1) {
  const int32_t digit = value - (value / 10) * 10;
  if (digit == 0 && place > 0) {
    *f1 = PowInt(base, LoadDouble(base, kTen), place);
    guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(*f1)));
    if (value < int32_t(guest::LoadU32(base, frame + 80))) {
      guest::StoreU32(base, node + 108, 0);
      return node;
    }
  }
  guest::StoreU32(base, node + 108, uint32_t(digit + 1));
  value /= 10;
  return node;
}

}  // namespace

uint32_t SetTach(uint8_t* base, uint32_t hud, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212D58 RaceHud::SetTach");
  Frame(base, sp, 96);
  CurveEval(base, hud + 20, value);
  const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 7);
  Stfs(base, node + 36, Fadds(Lfs(base, hud + 120), Lfs(base, 0x8200F720)));
  return node;
}

uint32_t SetBoost(uint8_t* base, uint32_t hud, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212E18 RaceHud::SetBoost");
  Frame(base, sp, 96);
  CurveEval(base, hud + 132, value);
  const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 5);
  Stfs(base, node + 36, Lfs(base, hud + 232));
  return node;
}

uint32_t SetSpeed(uint8_t* base, uint32_t hud, uint32_t speed, uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_82214260 RaceHud::SetSpeed");
  const uint32_t frame = sp - 160;
  Frame(base, sp, 160);
  if (speed > 999) {
    return hud;
  }
  const uint32_t digits = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 9);
  int32_t i = int32_t(guest::LoadU32(base, digits + 104)) - 1;
  if (i < 0) {
    return digits;
  }
  int32_t value = int32_t(speed);
  uint32_t node = 0;
  for (; i >= 0; i--) {
    node = ui_movies::FindNode(base, Dashboard(base, hud), kNone,
                               guest::LoadU32(base, digits + 100) + uint32_t(i));
    const int32_t place = int32_t(guest::LoadU32(base, digits + 104) - uint32_t(i)) - 1;
    Digit(base, node, value, place, frame, f1);
  }
  return node;
}

uint32_t SetTime(uint8_t* base, uint32_t hud, uint32_t time, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82213750 RaceHud::SetTime");
  Frame(base, sp, 144);
  uint32_t packed = guest::LoadU32(base, time);
  if ((packed & 0x7FFF0000) > 0x003B0000) {
    packed = 0x003BEFE7 | (packed & 0x80000000);
  }
  uint32_t node = 0;
  // Each part's digits from the last node down (r30 + 14), the value
  // divided by 10 after each.
  auto digits = [&](int32_t value, int32_t first, int32_t last) {
    for (int32_t k = first; k >= last; k--) {
      const int32_t digit = value - (value / 10) * 10;
      node = ui_movies::FindNode(base, Timer(base, hud), kNone, uint32_t(k + 14));
      value /= 10;
      guest::StoreU32(base, node + 108, uint32_t(digit + 1));
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

uint32_t SetDistance(uint8_t* base, uint32_t hud, uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_82214A50 RaceHud::SetDistance");
  const uint32_t frame = sp - 160;
  Frame(base, sp, 160);
  double value = *f1;
  double bound = Lfs(base, kZero);
  if (value < bound) {
    value = bound;
  }
  bound = Lfs(base, 0x8200F740);
  if (value > bound) {
    value = bound;
  }
  *f1 = value;
  const int32_t whole = Fctiwz(value);
  const double tenths = Fmuls(value, Lfs(base, 0x82001FD8));
  guest::StoreU32(base, frame + 84, uint32_t(whole));
  const double hundredths = Fmuls(value, Lfs(base, 0x82002A0C));
  guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(tenths)));
  const int32_t hundredths_bits = Fctiwz(hundredths);
  int32_t word = int32_t(guest::LoadU32(base, frame + 80));
  int32_t decimal = word - (word / 10) * 10;
  guest::StoreU32(base, frame + 80, uint32_t(hundredths_bits));
  word = int32_t(guest::LoadU32(base, frame + 80));
  int32_t rest;
  if (word - (word / 10) * 10 > 0 && ++decimal == 10) {
    decimal = 0;
    rest = int32_t(guest::LoadU32(base, frame + 84)) + 1;
  } else {
    rest = int32_t(guest::LoadU32(base, frame + 84));
  }
  uint32_t node = 0;
  for (int32_t k = 2; k >= 0; k--) {
    node = ui_movies::FindNode(base, Timer(base, hud), kNone, uint32_t(k + 9));
    if (k == 2) {
      guest::StoreU32(base, node + 108, uint32_t(decimal + 1));
      continue;
    }
    Digit(base, node, rest, 1 - k, frame, f1);
  }
  return node;
}

uint32_t Reset(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822121C0 RaceHud::Reset");
  Frame(base, sp, 128);
  const uint32_t group = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 1);
  const double zero = Lfs(base, kZero);
  if (int32_t(guest::LoadU32(base, group + 104)) > 0) {
    int32_t i = 0;
    do {
      const uint32_t child = ui_movies::FindNode(
          base, Dashboard(base, hud), kNone, guest::LoadU32(base, group + 100) + uint32_t(i));
      Stfs(base, child + 68, zero);
      i++;
    } while (i < int32_t(guest::LoadU32(base, group + 104)));
  }
  const uint32_t node5 = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 5);
  Stfs(base, node5 + 68, zero);
  guest::StoreU32(base, hud + 492, 0);
  guest::StoreU32(base, hud + 460, 0xFFFFFFFFu);
  const uint32_t mirror = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 19);
  guest::StoreU32(base, mirror + 80, 0);
  return mirror;
}

uint32_t ShowGauge(uint8_t* base, uint32_t hud, uint32_t slot, uint32_t shown, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212D08 RaceHud::ShowGauge");
  Frame(base, sp, 112);
  const uint32_t digits = ui_movies::FindNode(base, Dashboard(base, hud), 0, slot + 12);
  const uint32_t label = ui_movies::FindNode(base, Dashboard(base, hud), 0, slot + 9);
  guest::StoreU32(base, digits + 80, shown);
  guest::StoreU32(base, label + 80, shown);
  return label;
}

uint32_t SetUpExtraGauges(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82213DB0 RaceHud::SetUpExtraGauges");
  const uint32_t frame = sp - 144;
  Frame(base, sp, 144);
  const uint32_t car = guest::LoadU32(base, hud + kHudVehicle);
  uint32_t entry = guest::LoadU32(base, car + 5712) + 1824 + 8;
  guest::StoreU32(base, frame + 88, 0);
  guest::StoreU32(base, hud + 476, 0);
  guest::StoreU32(base, frame + 84, 0);
  guest::StoreU32(base, frame + 80, 0);
  for (int k = 0; k < 5; k++, entry += 12) {
    switch (int32_t(guest::LoadU32(base, entry))) {
      case 1:
        guest::StoreU32(base, hud + 476, 1);
        break;
      case 2:
        guest::StoreU32(base, frame + 80, 1);
        break;
      case 3:
        guest::StoreU32(base, frame + 84, 1);
        break;
      case 4:
        guest::StoreU32(base, frame + 88, 1);
        break;
      default:
        break;
    }
  }
  if (int32_t(guest::LoadU32(base, guest::LoadU32(base, car + 5712) + 12)) == 10) {
    guest::StoreU32(base, hud + 476, 1);
  }
  guest::StoreU32(base, frame + 96, frame + 80);
  guest::StoreU32(base, frame + 100, frame + 84);
  guest::StoreU32(base, frame + 104, frame + 88);
  // The slots from the last: the first flag set of the three (cleared), or -1.
  for (uint32_t slot = hud + 488; slot != hud + 476; slot -= 4) {
    guest::StoreU32(base, slot, 0xFFFFFFFFu);
    for (uint32_t i = 0; i < 3; i++) {
      if (int32_t(guest::LoadU32(base, guest::LoadU32(base, frame + 96 + 4 * i))) == 1) {
        guest::StoreU32(base, guest::LoadU32(base, frame + 96 + 4 * i), 0);
        guest::StoreU32(base, slot, i);
        break;
      }
    }
  }
  for (uint32_t i = 0; i < 3; i++) {
    ShowGauge(base, hud, i, guest::LoadU32(base, hud + 480 + 4 * i) < 3 ? 1 : 0, frame);
  }
  const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 1);
  guest::StoreU32(base, node + 80,
                  guest::LoadU32(base, hud + 8) & guest::LoadU32(base, hud + 476));
  return node;
}

uint32_t ApplyOptions(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212768 RaceHud::ApplyOptions");
  Frame(base, sp, 96);
  const uint32_t options = vehicle_accessors::SaveDataOf(base, 0) + 0x20000 - 1956;
  const bool mirror = guest::LoadU32(base, hud + 472) != 0;
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, hud + i, guest::LoadU32(base, options + i));
  }
  if (mirror) {
    const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 4);
    guest::StoreU32(base, node + 80, guest::LoadU32(base, hud + 12));
  }
  uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 3);
  guest::StoreU32(base, node + 80, guest::LoadU32(base, hud + 0));
  node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 3);
  guest::StoreU32(base, node + 80, guest::LoadU32(base, hud + 8));
  node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 4);
  guest::StoreU32(base, node + 80, guest::LoadU32(base, hud + 8));
  node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 1);
  guest::StoreU32(base, node + 80,
                  guest::LoadU32(base, hud + 476) & guest::LoadU32(base, hud + 8));
  node = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 1);
  guest::StoreU32(base, node + 80, guest::LoadU32(base, hud + 4));
  return node;
}

uint32_t UpdateView(uint8_t* base, uint32_t hud, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212B68 RaceHud::UpdateView");
  Frame(base, sp, 112);
  const uint32_t viewport = guest::LoadU32(base, guest::LoadU32(base, kViewports));
  guest::StoreU32(base, hud + 432, guest::LoadU32(base, viewport + 1860));
  const uint32_t full = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 0);
  const uint32_t cockpit = ui_movies::FindNode(base, Dashboard(base, hud), kNone, 4);
  const uint32_t view = guest::LoadU32(base, hud + 432);
  if (guest::LoadU32(base, hud + 12) != 0) {
    if (view >= 3) {
      return cockpit;
    }
    const uint32_t shown = view == 0 ? 0 : 1;
    if (view != 2) {
      guest::StoreU32(base, full + 80, view == 0 ? 1 : 0);
    } else {
      guest::StoreU32(base, full + 80, 1);
    }
    guest::StoreU32(base, cockpit + 80, shown);
    guest::StoreU32(base, hud + 472, shown);
    return cockpit;
  }
  guest::StoreU32(base, hud + 472, 0);
  guest::StoreU32(base, cockpit + 80, 0);
  const uint32_t view2 = guest::LoadU32(base, hud + 432);
  if (view2 < 3) {
    guest::StoreU32(base, full + 80, view2 == 1 ? 0 : 1);
  }
  return cockpit;
}

}  // namespace race_hud_object
