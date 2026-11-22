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
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/tuning_machine.h"
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

using G = schema::TuningMachineGlobals;

}  // namespace

uint64_t Start(uint8_t* base, const tuning_screen::StandCalls& stand, const Calls& calls,
               uint64_t r3, uint64_t r1, std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E7368 TuningMachine::Start");
  const uint32_t s = uint32_t(r3);
  const schema::TuningMachineScreen sc(base, s);
  const uint32_t sp = uint32_t(r1);
  const uint32_t frame = sp - kStartFrame;
  Store(base, frame, sp);  // the back chain, as stwu
  // The screen's words, in the original's order.
  sc.set_money(9999999);
  sc.set_list_row(0);
  sc.set_list_previous_row(0);
  sc.set_refresh(1);
  sc.set_unknown_25124(0xFF);
  sc.set_bonnet_color_set(0);
  sc.set_command_first_run(1);
  sc.set_command_end_after_setup(0);
  sc.set_command_end_now(0);
  sc.set_rewrite_car(0);
  sc.set_pages_enabled(1);
  sc.set_camera_preset(1);
  sc.set_previewed_slot(0);
  sc.set_unknown_25080(4);
  sc.set_category(0);      // power
  sc.set_screen_state(3);  // TUNING SHOP MAIN
  sc.set_save_data(vehicle_accessors::SaveDataOf(base, 0));
  // The car data (its f1 as FromGarageCar leaves it), a power curve on the
  // frame (+96; FromEngine's tail call Build's frame below this one).
  double data_f1 = 0.0;
  car_makers::TuningScreenSetUpCarData(base, stand.calls, s, frame, &data_f1);
  *f1 = data_f1;
  const schema::StagePowerCurve curve(base, frame + 96);
  vehicle::PowerCurveFromEngine(base, curve.address, s + 20180, frame - 112);
  vehicle::PowerCurveShiftPeak(base, curve.address, 0, frame);
  // The stats (as SetCarStats): the displacement, the weight, the power
  // (words by fctiwz, stfiwx) and the torque shown (a float).
  const int32_t displacement = steps::Fctiwz(steps::Lfs(base, sc.car_displacement_address()));
  const int32_t power = steps::Fctiwz(steps::Lfs(base, curve.shifted_peak_power_address()));
  const double torque = steps::Lfs(base, curve.torque_scale_address());
  const double two = steps::Lfs(base, G::axle_factor_address());
  sc.set_stat_displacement(uint32_t(displacement));
  const double rear = steps::Lfs(base, sc.car_rear_weight_address());
  const double shown = steps::Single(torque * steps::Lfs(base, G::torque_scale_address()));
  const double front = steps::Lfs(base, sc.car_front_weight_address());
  const double axles = steps::Single(rear + front);
  const double body = steps::Lfs(base, sc.car_body_weight_address());
  Store(base, sc.stat_torque_address(), std::bit_cast<uint32_t>(float(shown)));
  const double weight = steps::Single(std::fma(axles, two, body));
  sc.set_stat_weight(uint32_t(steps::Fctiwz(weight)));
  sc.set_stat_power(uint32_t(power));
  SavePlaceholders(base, sc.decal_images_address(), frame);
  SetUp(base, r3, (r1 & 0xFFFFFFFF00000000ull) | frame, true, calls);
  // The busy icon: movie 2's nodes 172, 171 and 173 (its id also at the
  // frame's +80, UiMovies::Find's key), the movie's pose reset.
  sc.set_busy_shown(0);
  sc.set_busy_movie(kShopMovie);
  sc.set_busy_animation_a(172);
  sc.set_busy_animation_b(171);
  sc.set_busy_animation_c(173);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, kShopMovie));
  ui_movies::ResetPose(base, movie);
  tuning_screen::Refresh(base, stand, r3, frame, f1);
  // The command queue at its state; the category menu (6 rows, 8 lines).
  const uint32_t state = sc.screen_state();
  schema::PageObject queue = sc.queue();
  queue.set_next_page(state);
  queue.set_page(state);
  queue.set_previous_page(state);
  queue.set_unknown_20(0xFFFFFFFFu);
  queue.set_step(0xFFFFFFFFu);
  queue.set_step_started(0);
  const uint32_t category = sc.category();
  const schema::FrontEndMenu menu = sc.category_menu();
  menu.set_first_item(0);
  menu.set_last_item(6);
  menu.set_wraps(1);
  menu.set_moved(0);
  menu.set_cursor(category);
  menu.set_previous(category);
  menu.set_list_wrapped(0);
  menu.set_first_row(0);
  menu.set_rows_shown(8);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  // Command 0 queued (both read again).
  queue = sc.queue();
  const uint32_t page = sc.screen_state();
  const uint32_t current = queue.page();
  if (int32_t(page) != int32_t(current)) {
    queue.set_previous_page(current);
  }
  queue.set_step(0);
  queue.set_queued_step(0xFFFFFFFFu);
  queue.set_next_page(page);
  queue.set_page(0xFFFFFFFFu);
  const uint32_t passes = G::fade_in_passes(base);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  return passes;  // FadeIn leaves r3 (lwz: zero-extended)
}

}  // namespace tuning_machine
