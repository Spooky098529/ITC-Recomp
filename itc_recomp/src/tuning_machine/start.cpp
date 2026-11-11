// TuningMachine::Start (sub_821E7368, docs/functions/tuning-machine.md): see
// tuning_machine.h. Its callees by their plain C++: SaveData::Of, the car
// makers' TuningScreen::SetUpCarData, the vehicle's PowerCurve::FromEngine
// and ShiftPeak, DecalImages::SavePlaceholders and TuningScreen::SetUp
// (here), UiMovies::Find and UiMovie::ResetPose, the tuning screen's
// refresh and ScreenFades::FadeIn; their guest calls through `stand` (the
// refresh's, the car makers') and `calls` (SetUp's). Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt): the stats'
// conversions and products are the translated code's, rounded to single
// where it rounds (as TuningScreen::SetCarStats').

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "car_makers/car_makers.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cmath>

namespace tuning_machine {

namespace {

namespace steps = camera_modes::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kTwo = 0x820021E8;          // the axles' factor (2.0 probably)
constexpr uint32_t kTorqueScale = 0x8200ADA0;  // the torque shown
constexpr uint32_t kFadePasses = 0x82001BEC;   // the fade in's passes (a word)

}  // namespace

uint64_t Start(uint8_t* base, const tuning_screen::StandCalls& stand, const Calls& calls,
               uint64_t r3, uint64_t r1, std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E7368 TuningMachine::Start");
  const uint32_t s = uint32_t(r3);
  const uint32_t sp = uint32_t(r1);
  const uint32_t frame = sp - kStartFrame;
  Store(base, frame, sp);  // the back chain, as stwu
  // The screen's words, in the original's order.
  Store(base, s + 25116, 9999999);
  Store(base, s + 25100, 0);
  Store(base, s + 25104, 0);
  Store(base, s + 9708, 1);
  guest::StoreU8(base, s + 25124, 0xFF);
  Store(base, s + 9712, 0);
  Store(base, s + 9716, 1);
  Store(base, s + 9720, 0);
  Store(base, s + 9724, 0);
  Store(base, s + 9732, 0);
  Store(base, s + 9728, 1);
  Store(base, s + 25072, 1);
  Store(base, s + 25076, 0);
  Store(base, s + 25080, 4);
  Store(base, s + 9656, 0);  // the category: power
  Store(base, s + 9652, 3);  // TUNING SHOP MAIN
  Store(base, s + tuning_screen::kSaveData, vehicle_accessors::SaveDataOf(base, 0));
  // The car data (its f1 as FromGarageCar leaves it), a power curve on the
  // frame (+96; FromEngine's tail call Build's frame below this one).
  double data_f1 = 0.0;
  car_makers::TuningScreenSetUpCarData(base, stand.calls, s, frame, &data_f1);
  *f1 = data_f1;
  const uint32_t curve = frame + 96;
  vehicle::PowerCurveFromEngine(base, curve, s + 20180, frame - 112);
  vehicle::PowerCurveShiftPeak(base, curve, 0, frame);
  // The stats (as SetCarStats): the displacement, the weight, the power
  // (words by fctiwz, stfiwx) and the torque shown (a float).
  const int32_t displacement = steps::Fctiwz(steps::Lfs(base, s + 20200));
  const int32_t power = steps::Fctiwz(steps::Lfs(base, curve + 1056));
  const double torque = steps::Lfs(base, curve + 1048);
  const double two = steps::Lfs(base, kTwo);
  Store(base, s + 20768, uint32_t(displacement));
  const double rear = steps::Lfs(base, s + 19584);
  const double shown = steps::Single(torque * steps::Lfs(base, kTorqueScale));
  const double front = steps::Lfs(base, s + 19312);
  const double axles = steps::Single(rear + front);
  const double body = steps::Lfs(base, s + 18912);
  Store(base, s + 20780, std::bit_cast<uint32_t>(float(shown)));
  const double weight = steps::Single(std::fma(axles, two, body));
  Store(base, s + 20772, uint32_t(steps::Fctiwz(weight)));
  Store(base, s + 20776, uint32_t(power));
  SavePlaceholders(base, s + kImages, frame);
  SetUp(base, r3, (r1 & 0xFFFFFFFF00000000ull) | frame, true, calls);
  // The busy icon: movie 2's nodes 172, 171 and 173 (its id also at the
  // frame's +80, UiMovies::Find's key), the movie's pose reset.
  Store(base, s + 10016, 0);
  Store(base, s + 10020, kShopMovie);
  Store(base, s + 10024, 172);
  Store(base, s + 10028, 171);
  Store(base, s + 10032, 173);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, kShopMovie));
  ui_movies::ResetPose(base, movie);
  tuning_screen::Refresh(base, stand, r3, frame, f1);
  // The command queue at its state; the category menu (6 rows, 8 lines).
  const uint32_t state = Load(base, s + 9652);
  uint32_t queue = Load(base, s + kQueue);
  Store(base, queue + 4, state);
  Store(base, queue + 0, state);
  Store(base, queue + 8, state);
  Store(base, queue + 20, 0xFFFFFFFFu);
  Store(base, queue + 12, 0xFFFFFFFFu);
  Store(base, queue + 24, 0);
  const uint32_t category = Load(base, s + 9656);
  const uint32_t menu = Load(base, s + kMenu);
  Store(base, menu + 12, 0);
  Store(base, menu + 16, 6);
  Store(base, menu + 20, 1);
  Store(base, menu + 8, 0);
  Store(base, menu + 0, category);
  Store(base, menu + 4, category);
  Store(base, menu + 44, 0);
  Store(base, menu + 56, 0);
  Store(base, menu + 60, 8);
  Store(base, menu + 36, 0);
  Store(base, menu + 64, 0);
  Store(base, menu + 24, 0);
  Store(base, menu + 28, 0);
  // Command 0 queued (both read again).
  queue = Load(base, s + kQueue);
  const uint32_t page = Load(base, s + 9652);
  const uint32_t current = Load(base, queue + 0);
  if (int32_t(page) != int32_t(current)) {
    Store(base, queue + 8, current);
  }
  Store(base, queue + 12, 0);
  Store(base, queue + 16, 0xFFFFFFFFu);
  Store(base, queue + 4, page);
  Store(base, queue + 0, 0xFFFFFFFFu);
  const uint32_t passes = Load(base, kFadePasses);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  return passes;  // FadeIn leaves r3 (lwz: zero-extended)
}

}  // namespace tuning_machine
