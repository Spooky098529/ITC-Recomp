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
#include "schema/car_physics.h"
#include "schema/vehicle.h"
#include "vehicle/vehicle.h"

#include <bit>
#include <climits>
#include <cmath>

namespace tuning_screen {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
uint64_t Load64(uint32_t address) {
  return (uint64_t(Load(address)) << 32) | Load(address + 4);
}
double Lfs(uint32_t address) {
  return double(std::bit_cast<float>(Load(address)));
}
double Lfd(uint32_t address) {
  return std::bit_cast<double>(Load64(address));
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
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

uint64_t SetCarStats(const front_end_car_draw::Calls& calls, uint32_t screen,
                     uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_821DF788 TuningScreen::SetCarStats");
  const uint32_t frame = Frame(sp, 1200);
  const uint32_t s = screen;
  const uint32_t movie = Load(MovieEntry(frame, kMovieId));
  double data_f1 = 0.0;
  car_makers::TuningScreenSetUpCarData(calls, s, frame, &data_f1);
  const uint32_t curve = frame + 96;
  // FromEngine tail-calls PowerCurve::Build, whose frame is below this one.
  vehicle::PowerCurveFromEngine(curve, TS(s).car_data_address() + 1268, frame - 112);
  vehicle::PowerCurveShiftPeak(curve, 0, frame);

  const schema::CarData car_data(TS(s).car_data_address());
  const int64_t displacement = Fctiwz(Lfs(car_data.displacement_address()));
  const uint32_t drive = TS(s).spec_byte_130();
  const int64_t power = Fctiwz(Lfs(schema::PowerCurve(curve).most_power_address()));
  const double torque = Lfs(schema::PowerCurve(curve).torque_scale_address());
  TS(s).set_stat_displacement(uint32_t(displacement));
  const double two = Lfs(kTwo);
  const double rear = Lfs(TS(s).car_data_address() + 672);
  const double shown = double(float(torque * Lfs(kTorqueScale)));
  const double front = Lfs(TS(s).car_data_address() + 400);
  const double axles = double(float(rear + front));
  const double body = Lfs(TS(s).car_data_address());
  TS(s).set_stat_torque(float(shown));
  const double weight = double(float(std::fma(axles, two, body)));
  TS(s).set_stat_weight(uint32_t(Fctiwz(weight)));
  TS(s).set_stat_peak_power(uint32_t(power));

  uint64_t r3 = 0;
  auto text = [&](uint32_t lr, uint32_t child, uint32_t id, uint64_t argument,
                  const double* f = nullptr) {
    const uint64_t regs[8] = {movie, 18, child, id, argument, 0, 0, 0};
    r3 = calls.call(kSetText, regs, 8, f, f != nullptr ? 1 : 0, frame, lr, f1);
  };
  if (drive < 5) {
    text(0x821DF894, 5, 421 + drive, 0);
  }
  const uint32_t aspiration = TS(s).spec_byte_69();
  if (aspiration < 2) {
    text(0x821DF8C0, 6, 426 + aspiration, 0);
  }
  text(0x821DF8D8, 7, 8, TS(s).stat_displacement());
  const int64_t w = int32_t(TS(s).stat_weight());
  Store64(frame + 80, uint64_t(w));
  const double single = double(float(double(w)));
  const double pounds = double(float(single * Lfd(kPounds)));
  Store(frame + 80, uint32_t(Fctiwz(pounds)));
  text(0x821DF920, 8, 8, Load(frame + 80));
  text(0x821DF938, 11, 8, TS(s).stat_peak_power());
  const double unit =
      double(float(Lfs(TS(s).stat_torque_address()) * Lfd(kTorqueUnit)));
  Store64(frame + 48, std::bit_cast<uint64_t>(unit));
  text(0x821DF968, 12, 436, Load64(frame + 48), &unit);
  text(0x821DF97C, 9, 438, 0);
  text(0x821DF990, 10, 440, 0);
  text(0x821DF9A4, 13, 437, 0);
  text(0x821DF9B8, 14, 439, 0);
  return r3;
}

}  // namespace tuning_screen
