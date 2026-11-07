// The tuning screen's car stats (docs/functions/tuning-screen.md): see
// tuning_screen.h. The car data through the car makers' plain C++
// (TuningScreen::SetUpCarData), the power curve through the vehicle's
// (PowerCurve::FromEngine and ShiftPeak), the texts through UiMovie::SetText's
// override from the original's frame with its return addresses (the varargs
// it leaves unset passed as 0: the user's call, 2026-10-30). Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt): the conversions
// and products are the translated code's, rounded to single where it rounds.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "car_makers/car_makers.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "vehicle/vehicle.h"

#include <bit>
#include <climits>
#include <cmath>

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
double Lfs(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(Load(base, address)));
}
double Lfd(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(Load64(base, address));
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

// fctiwz as the translated code: NaN 0x80000000, saturated above (and below,
// as cvttsd answers).
int64_t Fctiwz(double value) {
  if (std::isnan(value)) {
    return int64_t(0x80000000u);
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value <= double(INT_MIN)) {
    return INT_MIN;
  }
  return int64_t(int32_t(value));
}

constexpr uint32_t kTwo = 0x820021E8;          // the axles' factor (2.0 probably)
constexpr uint32_t kTorqueScale = 0x8200ADA0;  // the torque shown
constexpr uint32_t kPounds = 0x82009F00;       // a double: the weight's unit
constexpr uint32_t kTorqueUnit = 0x82009F08;   // a double: child 12's

}  // namespace

uint64_t SetCarStats(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t screen,
                     uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_821DF788 TuningScreen::SetCarStats");
  const uint32_t frame = Frame(base, sp, 1200);
  const uint32_t s = screen;
  const uint32_t movie = Load(base, MovieEntry(base, frame, kMovieId));
  double data_f1 = 0.0;
  car_makers::TuningScreenSetUpCarData(base, calls, s, frame, &data_f1);
  const uint32_t curve = frame + 96;
  // FromEngine tail-calls PowerCurve::Build, whose frame is below this one.
  vehicle::PowerCurveFromEngine(base, curve, s + 20180, frame - 112);
  vehicle::PowerCurveShiftPeak(base, curve, 0, frame);

  const int64_t displacement = Fctiwz(Lfs(base, s + 20200));
  const uint32_t drive = Load(base, s + 20788);
  const int64_t power = Fctiwz(Lfs(base, curve + 1056));
  const double torque = Lfs(base, curve + 1048);
  Store(base, s + 20768, uint32_t(displacement));
  const double two = Lfs(base, kTwo);
  const double rear = Lfs(base, s + 19584);
  const double shown = double(float(torque * Lfs(base, kTorqueScale)));
  const double front = Lfs(base, s + 19312);
  const double axles = double(float(rear + front));
  const double body = Lfs(base, s + 18912);
  Store(base, s + 20780, std::bit_cast<uint32_t>(float(shown)));
  const double weight = double(float(std::fma(axles, two, body)));
  Store(base, s + 20772, uint32_t(Fctiwz(weight)));
  Store(base, s + 20776, uint32_t(power));

  uint64_t r3 = 0;
  auto text = [&](uint32_t lr, uint32_t child, uint32_t id, uint64_t argument,
                  const double* f = nullptr) {
    const uint64_t regs[8] = {movie, 18, child, id, argument, 0, 0, 0};
    r3 = calls.call(kSetText, regs, 8, f, f != nullptr ? 1 : 0, frame, lr, f1);
  };
  if (drive < 5) {
    text(0x821DF894, 5, 421 + drive, 0);
  }
  const uint32_t aspiration = Load(base, s + 20784);
  if (aspiration < 2) {
    text(0x821DF8C0, 6, 426 + aspiration, 0);
  }
  text(0x821DF8D8, 7, 8, Load(base, s + 20768));
  const int64_t w = int32_t(Load(base, s + 20772));
  Store64(base, frame + 80, uint64_t(w));
  const double single = double(float(double(w)));
  const double pounds = double(float(single * Lfd(base, kPounds)));
  Store(base, frame + 80, uint32_t(Fctiwz(pounds)));
  text(0x821DF920, 8, 8, Load(base, frame + 80));
  text(0x821DF938, 11, 8, Load(base, s + 20776));
  const double unit = double(float(Lfs(base, s + 20780) * Lfd(base, kTorqueUnit)));
  Store64(base, frame + 48, std::bit_cast<uint64_t>(unit));
  text(0x821DF968, 12, 436, Load64(base, frame + 48), &unit);
  text(0x821DF97C, 9, 438, 0);
  text(0x821DF990, 10, 440, 0);
  text(0x821DF9A4, 13, 437, 0);
  text(0x821DF9B8, 14, 439, 0);
  return r3;
}

}  // namespace tuning_screen
