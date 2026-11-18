// The race HUD object's gauge helpers and dashboard setters
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/car_physics.h"
#include "schema/race_hud_object.h"
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

using Hud = schema::RaceHud;

uint32_t Dashboard(uint8_t* base, uint32_t hud) { return Hud(base, hud).dashboard().address; }

}  // namespace

void CurveEval(uint8_t* base, uint32_t curve, double x) {
  ITC_NATIVE_RUNNING("sub_82212118 HudCurve::Eval");
  using Point = schema::HudCurvePoint;
  const schema::HudCurve c(base, curve);
  double f0 = double(c.max_x());
  c.set_input(float(x));
  double y = Lfs(base, kZero);
  if (x > f0) {
    c.set_input(float(f0));
    return;
  }
  f0 = double(c.min_x());
  if (x < f0) {
    c.set_input(float(f0));
    return;
  }
  if (int32_t(c.count()) - 1 > 0) {
    int32_t i = 0;
    while (true) {
      const uint32_t point = c.points_address() + 8 * uint32_t(i);
      if (!(x < double(Point(base, point).x())) && !(x > double(Point(base, point + 8).x()))) {
        const Point at(base, uint32_t(i) * 8 + c.points_address());
        const Point next(base, at.address + 8);
        double a = double(at.x());
        const double b = double(next.x());
        const double dx = Fsubs(x, a);
        a = Fsubs(a, b);
        const double ya = double(at.y());
        const double yb = double(next.y());
        const double dy = Fsubs(ya, yb);
        y = Fmadds(Fdivs(dx, a), dy, ya);
        break;
      }
      i++;
      if (!(i < int32_t(c.count()) - 1)) {
        break;
      }
    }
  }
  c.set_output(float(y));
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
  const schema::RaceHudBar b(base, bar);
  const double value = double(b.value());
  const double trail = double(b.trail());
  StoreS64(base, sp - 16, target);
  const double goal = Single(double(int64_t(target)));
  const double zero = Lfs(base, kZero);
  b.set_target(float(goal));
  double to_value = Fsubs(goal, value);
  double to_trail = Fsubs(goal, trail);
  const int32_t value_sign = to_value < zero ? -1 : 1;
  const int32_t trail_sign = to_trail < zero ? -1 : 1;
  const double value_step = double(b.value_step());
  StoreS64(base, sp - 16, value_sign);
  StoreS64(base, sp - 8, trail_sign);
  const double vs = Single(double(int64_t(value_sign)));
  const double ts = Single(double(int64_t(trail_sign)));
  to_value = Fmuls(vs, to_value);
  to_trail = Fmuls(ts, to_trail);
  b.set_value(float(to_value > value_step ? Fmadds(value_step, vs, value)
                                          : Fmadds(vs, to_value, value)));
  const double trail_step = double(b.trail_step());
  b.set_trail(float(to_trail > trail_step ? Fmadds(trail_step, ts, trail)
                                          : Fmadds(ts, to_trail, trail)));
  const double max = double(b.max());
  if (double(b.value()) > max) {
    b.set_value(float(max));
  }
  const double min = double(b.min());
  if (double(b.value()) < min) {
    b.set_value(float(min));
  }
  if (double(b.trail()) > max) {
    b.set_trail(float(max));
  }
  if (double(b.trail()) < min) {
    b.set_trail(float(min));
  }
  const double v = double(b.value());
  const double t = double(b.trail());
  const bool up = !(v < t) && !(goal < v) && !(trail_sign < 0);
  b.set_rising(up ? 1 : 0);
  if (v == t) {
    b.set_touched(0);
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
  const Hud h(base, hud);
  const double low = double(h.bar_min());
  h.set_bar_value(float(value));
  if (!(value > low)) {
    guest::StoreU32(base, node + 80, 0);
    return node;
  }
  guest::StoreU32(base, node + 80, 1);
  const double from = double(h.bar_min());
  const double range = Fsubs(double(h.bar_max()), from);
  Stfs(base, node + 28, Fdivs(double(h.bar_value()), range));
  return node;
}

void ResetBar(uint8_t* base, uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_82212668 RaceHud::ResetBar");
  const Hud h(base, hud);
  const uint32_t car = h.vehicle().address;
  h.set_bar_max(float(Lfs(base, car + 2420)));
  const double value = Lfs(base, car + 2420);
  const double zero = Lfs(base, kZero);
  h.set_bar_value(float(value));
  h.set_bar_min(float(zero));
}

void ClearWarning(uint8_t* base, uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_822122A0 RaceHud::ClearWarning");
  Hud(base, hud).set_warning_shown(0);
}

uint32_t SetPlayer(uint8_t* base, uint32_t hud, uint32_t player, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82212690 RaceHud::SetPlayer");
  Frame(base, sp, 96);
  const Hud h(base, hud);
  h.set_player(player);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, player);
  h.set_vehicle(vehicle);
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
