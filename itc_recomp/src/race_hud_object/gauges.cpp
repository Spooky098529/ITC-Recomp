// The race HUD object's gauge helpers and dashboard setters
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

// The original's frame: its back chain.
void Frame(uint8_t* base, uint32_t sp, uint32_t bytes) { guest::StoreU32(base, sp - bytes, sp); }

void StoreS64(uint8_t* base, uint32_t address, int64_t value) {
  guest::StoreU32(base, address, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(base, address + 4, uint32_t(uint64_t(value)));
}

uint32_t Dashboard(uint8_t* base, uint32_t hud) { return guest::LoadU32(base, hud + kHudDashboard); }

}  // namespace

void CurveEval(uint8_t* base, uint32_t curve, double x) {
  ITC_NATIVE_RUNNING("sub_82212118 HudCurve::Eval");
  double f0 = Lfs(base, curve + 88);
  Stfs(base, curve + 96, x);
  double y = Lfs(base, kZero);
  if (x > f0) {
    Stfs(base, curve + 96, f0);
    return;
  }
  f0 = Lfs(base, curve + 92);
  if (x < f0) {
    Stfs(base, curve + 96, f0);
    return;
  }
  if (int32_t(guest::LoadU32(base, curve + 108)) - 1 > 0) {
    int32_t i = 0;
    while (true) {
      const uint32_t point = curve + 8 * uint32_t(i);
      if (!(x < Lfs(base, point)) && !(x > Lfs(base, point + 8))) {
        const uint32_t at = uint32_t(i) * 8 + curve;
        double a = Lfs(base, at);
        const double b = Lfs(base, at + 8);
        const double dx = Fsubs(x, a);
        a = Fsubs(a, b);
        const double ya = Lfs(base, at + 4);
        const double yb = Lfs(base, at + 12);
        const double dy = Fsubs(ya, yb);
        y = Fmadds(Fdivs(dx, a), dy, ya);
        break;
      }
      i++;
      if (!(i < int32_t(guest::LoadU32(base, curve + 108)) - 1)) {
        break;
      }
    }
  }
  Stfs(base, curve + 100, y);
}

double PowInt(uint8_t* base, double x, int32_t n) {
  ITC_NATIVE_RUNNING("sub_82213C40 PowInt");
  double square = x;
  uint32_t bits = n < 0 ? uint32_t(-int64_t(n)) : uint32_t(n);
  const uint64_t one_bits =
      uint64_t(guest::LoadU32(base, 0x82002250)) << 32 | guest::LoadU32(base, 0x82002254);
  const double one = std::bit_cast<double>(one_bits);
  double result = one;
  while (true) {
    if ((bits & 1) != 0) {
      result = result * square;
    }
    bits >>= 1;
    if (bits == 0) {
      break;
    }
    square = square * square;
  }
  if (n < 0) {
    result = one / result;
  }
  return result;
}

void BarStep(uint8_t* base, uint32_t bar, int32_t target, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82211FB8 HudBar::Step");
  const double value = Lfs(base, bar + 0);
  const double trail = Lfs(base, bar + 8);
  StoreS64(base, sp - 16, target);
  const double goal = Single(double(int64_t(target)));
  const double zero = Lfs(base, kZero);
  Stfs(base, bar + 24, goal);
  double to_value = Fsubs(goal, value);
  double to_trail = Fsubs(goal, trail);
  const int32_t value_sign = to_value < zero ? -1 : 1;
  const int32_t trail_sign = to_trail < zero ? -1 : 1;
  const double value_step = Lfs(base, bar + 4);
  StoreS64(base, sp - 16, value_sign);
  StoreS64(base, sp - 8, trail_sign);
  const double vs = Single(double(int64_t(value_sign)));
  const double ts = Single(double(int64_t(trail_sign)));
  to_value = Fmuls(vs, to_value);
  to_trail = Fmuls(ts, to_trail);
  Stfs(base, bar + 0,
       to_value > value_step ? Fmadds(value_step, vs, value) : Fmadds(vs, to_value, value));
  const double trail_step = Lfs(base, bar + 12);
  Stfs(base, bar + 8,
       to_trail > trail_step ? Fmadds(trail_step, ts, trail) : Fmadds(ts, to_trail, trail));
  const double max = Lfs(base, bar + 20);
  if (Lfs(base, bar + 0) > max) {
    Stfs(base, bar + 0, max);
  }
  const double min = Lfs(base, bar + 16);
  if (Lfs(base, bar + 0) < min) {
    Stfs(base, bar + 0, min);
  }
  if (Lfs(base, bar + 8) > max) {
    Stfs(base, bar + 8, max);
  }
  if (Lfs(base, bar + 8) < min) {
    Stfs(base, bar + 8, min);
  }
  const double v = Lfs(base, bar + 0);
  const double t = Lfs(base, bar + 8);
  const bool up = !(v < t) && !(goal < v) && !(trail_sign < 0);
  guest::StoreU32(base, bar + 28, up ? 1 : 0);
  if (v == t) {
    guest::StoreU32(base, bar + 36, 0);
  }
}

uint32_t SetGear(uint8_t* base, uint32_t hud, int32_t gear, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212DB0 RaceHud::SetGear");
  Frame(base, sp, 96);
  const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), 0xFFFFFFFFu, 11);
  uint32_t picture;
  if (gear > 0) {
    picture = gear == 6 ? 7 : uint32_t(gear - 1);
  } else {
    picture = gear == 0 ? 5 : 6;
  }
  guest::StoreU32(base, node + 108, picture);
  return node;
}

uint32_t SetBar(uint8_t* base, uint32_t hud, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82214698 RaceHud::SetBar");
  Frame(base, sp, 112);
  const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), 0xFFFFFFFFu, 35);
  const double low = Lfs(base, hud + 412);
  Stfs(base, hud + 404, value);
  if (!(value > low)) {
    guest::StoreU32(base, node + 80, 0);
    return node;
  }
  guest::StoreU32(base, node + 80, 1);
  const double from = Lfs(base, hud + 412);
  const double range = Fsubs(Lfs(base, hud + 408), from);
  Stfs(base, node + 28, Fdivs(Lfs(base, hud + 404), range));
  return node;
}

void ResetBar(uint8_t* base, uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_82212668 RaceHud::ResetBar");
  const uint32_t car = guest::LoadU32(base, hud + kHudVehicle);
  Stfs(base, hud + 408, Lfs(base, car + 2420));
  const double value = Lfs(base, car + 2420);
  const double zero = Lfs(base, kZero);
  Stfs(base, hud + 404, value);
  Stfs(base, hud + 412, zero);
}

void ClearWarning(uint8_t* base, uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_822122A0 RaceHud::ClearWarning");
  guest::StoreU32(base, hud + 468, 0);
}

uint32_t SetPlayer(uint8_t* base, uint32_t hud, uint32_t player, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212690 RaceHud::SetPlayer");
  Frame(base, sp, 96);
  guest::StoreU32(base, hud + kHudPlayer, player);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, player);
  guest::StoreU32(base, hud + kHudVehicle, vehicle);
  return vehicle;
}

uint32_t Touching(uint8_t* base, uint32_t player, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82215820 RaceHud::Touching");
  Frame(base, sp, 96);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, player);
  const uint32_t object = guest::LoadU32(base, vehicle + 340);
  if (object == 0) {
    return 0;
  }
  return vehicle_accessors::CollisionObjectFirstContact(base, object, 0x07000000) != 0 ? 1 : 0;
}

}  // namespace race_hud_object
