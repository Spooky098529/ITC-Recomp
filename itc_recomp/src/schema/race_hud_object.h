// The views of docs/functions/race-hud-object.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-hud-object.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceHud, sub_822155C0.r3, sub_82213860.r3, sub_82214C88.r3
struct RaceHud : View {
  using View::View;
  static constexpr uint32_t kSize = 640;
  // HUD option 0 (player 0's save data +0x1F85C): the dashboard's node 3 shown
  uint32_t option_node3() const { return guest::LoadU32(base, address + 0); }
  void set_option_node3(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t option_node3_address() const { return address + 0; }
  // HUD option 1: the dashboard's node 1 shown
  uint32_t option_node1() const { return guest::LoadU32(base, address + 4); }
  void set_option_node1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t option_node1_address() const { return address + 4; }
  // HUD option 2: nodes (0, 3) and (0, 4) shown, node (0, 1) with +476
  uint32_t option_gauges() const { return guest::LoadU32(base, address + 8); }
  void set_option_gauges(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t option_gauges_address() const { return address + 8; }
  // HUD option 3: the cockpit node 4 and the rear-view mirror by the view
  uint32_t option_mirror() const { return guest::LoadU32(base, address + 12); }
  void set_option_mirror(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t option_mirror_address() const { return address + 12; }
  // the player's index (RaceHud::SetPlayer)
  uint32_t player() const { return guest::LoadU32(base, address + 16); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t player_address() const { return address + 16; }
  // the tachometer's HudCurve (one of three at 0x8200D1F8)
  uint32_t tach_curve_address() const { return address + 20; }
  // the tachometer curve's output (its +100): the needle's rotation
  float tach_output() const { return guest::LoadFloat(base, address + 120); }
  void set_tach_output(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t tach_output_address() const { return address + 120; }
  // the boost's HudCurve (copied from 0x8200D348)
  uint32_t boost_curve_address() const { return address + 132; }
  // the boost curve's output (its +100): the needle's rotation
  float boost_output() const { return guest::LoadFloat(base, address + 232); }
  void set_boost_output(float v) const { guest::StoreFloat(base, address + 232, v); }
  uint32_t boost_output_address() const { return address + 232; }
  // the racers' bars: four RaceHudBar, 40 bytes each
  uint32_t bars_address() const { return address + 244; }
  // the bar at node 35: its value (the car's +2420)
  float bar_value() const { return guest::LoadFloat(base, address + 404); }
  void set_bar_value(float v) const { guest::StoreFloat(base, address + 404, v); }
  uint32_t bar_value_address() const { return address + 404; }
  // the bar at node 35: its top
  float bar_max() const { return guest::LoadFloat(base, address + 408); }
  void set_bar_max(float v) const { guest::StoreFloat(base, address + 408, v); }
  uint32_t bar_max_address() const { return address + 408; }
  // the bar at node 35: its bottom (0.0)
  float bar_min() const { return guest::LoadFloat(base, address + 412); }
  void set_bar_min(float v) const { guest::StoreFloat(base, address + 412, v); }
  uint32_t bar_min_address() const { return address + 412; }
  // the expressway sign's track zone (0-31, -1 none)
  uint32_t sign_zone() const { return guest::LoadU32(base, address + 416); }
  void set_sign_zone(uint32_t v) const { guest::StoreU32(base, address + 416, v); }
  uint32_t sign_zone_address() const { return address + 416; }
  // the course's sign flags (one of the 24 tables at 0x8200D9D0, by Race::CourseLayout)
  uint32_t sign_flags() const { return guest::LoadU32(base, address + 420); }
  void set_sign_flags(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t sign_flags_address() const { return address + 420; }
  // 1 after RaceHud::SetUpSigns
  uint32_t signs_on() const { return guest::LoadU32(base, address + 424); }
  void set_signs_on(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t signs_on_address() const { return address + 424; }
  // the place name's track zone (32-69, -1 none)
  uint32_t place_zone() const { return guest::LoadU32(base, address + 428); }
  void set_place_zone(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t place_zone_address() const { return address + 428; }
  // the camera view (viewport 0's +1860)
  uint32_t view() const { return guest::LoadU32(base, address + 432); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t view_address() const { return address + 432; }
  // 1 from StartCountdown, then UiMovie::Run's answer on the countdown
  uint32_t countdown_running() const { return guest::LoadU32(base, address + 436); }
  void set_countdown_running(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t countdown_running_address() const { return address + 436; }
  // Vehicle::Get(+16)
  template <typename T = Vehicle>
  T vehicle() const { return T(base, guest::LoadU32(base, address + 440)); }
  void set_vehicle(const View& v) const { guest::StoreU32(base, address + 440, v.address); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t vehicle_address() const { return address + 440; }
  // the race progress [0x826D82E8] + 2184 (RaceHudMachine::StartCountdown)
  template <typename T = RaceHudRacers>
  T racers() const { return T(base, guest::LoadU32(base, address + 444)); }
  void set_racers(const View& v) const { guest::StoreU32(base, address + 444, v.address); }
  void set_racers(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t racers_address() const { return address + 444; }
  // the panel (0 the racers' bars, 1 the timer, 2 to 4 the time trial's and others'; RaceHud::SetMode)
  uint32_t mode() const { return guest::LoadU32(base, address + 448); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t mode_address() const { return address + 448; }
  uint32_t racer_count() const { return guest::LoadU32(base, address + 452); }
  void set_racer_count(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t racer_count_address() const { return address + 452; }
  // 1 when a racer's bar fell below its warning level this tick
  uint32_t any_bar_low() const { return guest::LoadU32(base, address + 456); }
  void set_any_bar_low(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t any_bar_low_address() const { return address + 456; }
  // the item shown on the racers' movie (-1 none; the race progress's +7240)
  uint32_t item_shown() const { return guest::LoadU32(base, address + 460); }
  void set_item_shown(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t item_shown_address() const { return address + 460; }
  // the timer movie's named animation playing (the racers' +12)
  uint32_t timer_animation() const { return guest::LoadU32(base, address + 464); }
  void set_timer_animation(uint32_t v) const { guest::StoreU32(base, address + 464, v); }
  uint32_t timer_animation_address() const { return address + 464; }
  // the warning movie (+616) shown (the car's +6168, probably wrong way)
  uint32_t warning_shown() const { return guest::LoadU32(base, address + 468); }
  void set_warning_shown(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t warning_shown_address() const { return address + 468; }
  // the cockpit node 4 shown by the view (RaceHud::UpdateView)
  uint32_t view_dashboard() const { return guest::LoadU32(base, address + 472); }
  void set_view_dashboard(uint32_t v) const { guest::StoreU32(base, address + 472, v); }
  uint32_t view_dashboard_address() const { return address + 472; }
  // a build entry of kind 1 (or the build's +12 10): node (0, 1) shown with +8
  uint32_t part_fitted() const { return guest::LoadU32(base, address + 476); }
  void set_part_fitted(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t part_fitted_address() const { return address + 476; }
  // the extra gauges' slots: 0 a float, 1 and 2 integers, -1 none
  uint32_t gauge_slot0() const { return guest::LoadU32(base, address + 480); }
  void set_gauge_slot0(uint32_t v) const { guest::StoreU32(base, address + 480, v); }
  uint32_t gauge_slot0_address() const { return address + 480; }
  uint32_t gauge_slot1() const { return guest::LoadU32(base, address + 484); }
  void set_gauge_slot1(uint32_t v) const { guest::StoreU32(base, address + 484, v); }
  uint32_t gauge_slot1_address() const { return address + 484; }
  uint32_t gauge_slot2() const { return guest::LoadU32(base, address + 488); }
  void set_gauge_slot2(uint32_t v) const { guest::StoreU32(base, address + 488, v); }
  uint32_t gauge_slot2_address() const { return address + 488; }
  // the input recording's full notice shown
  uint32_t recording_full() const { return guest::LoadU32(base, address + 492); }
  void set_recording_full(uint32_t v) const { guest::StoreU32(base, address + 492, v); }
  uint32_t recording_full_address() const { return address + 492; }
  uint32_t unknown_496() const { return guest::LoadU32(base, address + 496); }
  void set_unknown_496(uint32_t v) const { guest::StoreU32(base, address + 496, v); }
  uint32_t unknown_496_address() const { return address + 496; }
  // the splits' (mode 2)
  uint32_t unknown_572() const { return guest::LoadU32(base, address + 572); }
  void set_unknown_572(uint32_t v) const { guest::StoreU32(base, address + 572, v); }
  uint32_t unknown_572_address() const { return address + 572; }
  // the splits' (mode 2)
  uint32_t unknown_576() const { return guest::LoadU32(base, address + 576); }
  void set_unknown_576(uint32_t v) const { guest::StoreU32(base, address + 576, v); }
  uint32_t unknown_576_address() const { return address + 576; }
  // the splits' (mode 2)
  uint32_t unknown_580() const { return guest::LoadU32(base, address + 580); }
  void set_unknown_580(uint32_t v) const { guest::StoreU32(base, address + 580, v); }
  uint32_t unknown_580_address() const { return address + 580; }
  // the splits' (mode 2)
  uint32_t unknown_584() const { return guest::LoadU32(base, address + 584); }
  void set_unknown_584(uint32_t v) const { guest::StoreU32(base, address + 584, v); }
  uint32_t unknown_584_address() const { return address + 584; }
  // the dashboard's texture 56 swapped out for the mirror (0 none)
  uint32_t swapped_texture() const { return guest::LoadU32(base, address + 588); }
  void set_swapped_texture(uint32_t v) const { guest::StoreU32(base, address + 588, v); }
  uint32_t swapped_texture_address() const { return address + 588; }
  // the mirror's texture record: render target 11 (CameraPasses::RenderTarget), then its width and height
  View mirror_texture() const { return View(base, guest::LoadU32(base, address + 592)); }
  void set_mirror_texture(uint32_t v) const { guest::StoreU32(base, address + 592, v); }
  uint32_t mirror_texture_address() const { return address + 592; }
  uint32_t mirror_width() const { return guest::LoadU32(base, address + 596); }
  void set_mirror_width(uint32_t v) const { guest::StoreU32(base, address + 596, v); }
  uint32_t mirror_width_address() const { return address + 596; }
  uint32_t mirror_height() const { return guest::LoadU32(base, address + 600); }
  void set_mirror_height(uint32_t v) const { guest::StoreU32(base, address + 600, v); }
  uint32_t mirror_height_address() const { return address + 600; }
  // UI movie id 10: the racers' gauges
  View racers_movie() const { return View(base, guest::LoadU32(base, address + 604)); }
  void set_racers_movie(uint32_t v) const { guest::StoreU32(base, address + 604, v); }
  uint32_t racers_movie_address() const { return address + 604; }
  // UI movie id 0: the dashboard
  View dashboard() const { return View(base, guest::LoadU32(base, address + 608)); }
  void set_dashboard(uint32_t v) const { guest::StoreU32(base, address + 608, v); }
  uint32_t dashboard_address() const { return address + 608; }
  // UI movie id 11: the start countdown
  View countdown() const { return View(base, guest::LoadU32(base, address + 612)); }
  void set_countdown(uint32_t v) const { guest::StoreU32(base, address + 612, v); }
  uint32_t countdown_address() const { return address + 612; }
  // UI movie id 3: the warning
  View warning() const { return View(base, guest::LoadU32(base, address + 616)); }
  void set_warning(uint32_t v) const { guest::StoreU32(base, address + 616, v); }
  uint32_t warning_address() const { return address + 616; }
  // UI movie id 2 (not read)
  View movie2() const { return View(base, guest::LoadU32(base, address + 620)); }
  void set_movie2(uint32_t v) const { guest::StoreU32(base, address + 620, v); }
  uint32_t movie2_address() const { return address + 620; }
  // UI movie id 13: the race timer
  View timer() const { return View(base, guest::LoadU32(base, address + 624)); }
  void set_timer(uint32_t v) const { guest::StoreU32(base, address + 624, v); }
  uint32_t timer_address() const { return address + 624; }
  // UI movie id 1 (not read)
  View movie1() const { return View(base, guest::LoadU32(base, address + 628)); }
  void set_movie1(uint32_t v) const { guest::StoreU32(base, address + 628, v); }
  uint32_t movie1_address() const { return address + 628; }
  // UI movie id 12: the time trial's splits
  View splits() const { return View(base, guest::LoadU32(base, address + 632)); }
  void set_splits(uint32_t v) const { guest::StoreU32(base, address + 632, v); }
  uint32_t splits_address() const { return address + 632; }
  // UI movie id 19 (never seen in play)
  View movie19() const { return View(base, guest::LoadU32(base, address + 636)); }
  void set_movie19(uint32_t v) const { guest::StoreU32(base, address + 636, v); }
  uint32_t movie19_address() const { return address + 636; }
};

// embedded; measure: RaceHud.+444, sub_822155C0.r3.+444, sub_82214BA8.r4
struct RaceHudRacers : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  // the racers' count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // record 0's item id (-1 none); record i's at +8 + 972 i
  uint32_t item0() const { return guest::LoadU32(base, address + 8); }
  void set_item0(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t item0_address() const { return address + 8; }
  // the timer movie's named animation (-1 the timer hidden)
  uint32_t timer_animation() const { return guest::LoadU32(base, address + 12); }
  void set_timer_animation(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t timer_animation_address() const { return address + 12; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  // record 0's bar top (an int); record i's at +788 + 972 i
  uint32_t bar_max0() const { return guest::LoadU32(base, address + 788); }
  void set_bar_max0(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t bar_max0_address() const { return address + 788; }
  // taken from the race progress's +7156 for the timer's distance
  float start_distance() const { return guest::LoadFloat(base, address + 828); }
  void set_start_distance(float v) const { guest::StoreFloat(base, address + 828, v); }
  uint32_t start_distance_address() const { return address + 828; }
  // the split (mode 4's rows at +24 + 76 x it)
  uint32_t split() const { return guest::LoadU32(base, address + 836); }
  void set_split(uint32_t v) const { guest::StoreU32(base, address + 836, v); }
  uint32_t split_address() const { return address + 836; }
  // record 1's item id
  uint32_t item1() const { return guest::LoadU32(base, address + 980); }
  void set_item1(uint32_t v) const { guest::StoreU32(base, address + 980, v); }
  uint32_t item1_address() const { return address + 980; }
  // the race time in ticks (RaceTime::FromTicks)
  uint32_t ticks() const { return guest::LoadU32(base, address + 3932); }
  void set_ticks(uint32_t v) const { guest::StoreU32(base, address + 3932, v); }
  uint32_t ticks_address() const { return address + 3932; }
};

// embedded; measure: HudBar
struct RaceHudBar : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
  // the value's most a step (the top times the float at 0x82002A60)
  float value_step() const { return guest::LoadFloat(base, address + 4); }
  void set_value_step(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t value_step_address() const { return address + 4; }
  float trail() const { return guest::LoadFloat(base, address + 8); }
  void set_trail(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t trail_address() const { return address + 8; }
  // the trail's most a step (the top times the float at 0x8200F724)
  float trail_step() const { return guest::LoadFloat(base, address + 12); }
  void set_trail_step(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t trail_step_address() const { return address + 12; }
  float min() const { return guest::LoadFloat(base, address + 16); }
  void set_min(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t min_address() const { return address + 16; }
  float max() const { return guest::LoadFloat(base, address + 20); }
  void set_max(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t max_address() const { return address + 20; }
  float target() const { return guest::LoadFloat(base, address + 24); }
  void set_target(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t target_address() const { return address + 24; }
  // set by HudBar::Step: the value at or above the trail and the target, moving up
  uint32_t rising() const { return guest::LoadU32(base, address + 28); }
  void set_rising(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t rising_address() const { return address + 28; }
  // the low bar's animation played
  uint32_t animation_played() const { return guest::LoadU32(base, address + 32); }
  void set_animation_played(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t animation_played_address() const { return address + 32; }
  // set when the racer touched something (the trail held), cleared when the value meets the trail
  uint32_t touched() const { return guest::LoadU32(base, address + 36); }
  void set_touched(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t touched_address() const { return address + 36; }
};

// embedded; measure: HudCurve
struct HudCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  // (x, y) floats, 8 bytes a point (HudCurvePoint)
  uint32_t points_address() const { return address + 0; }
  float max_x() const { return guest::LoadFloat(base, address + 88); }
  void set_max_x(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t max_x_address() const { return address + 88; }
  float min_x() const { return guest::LoadFloat(base, address + 92); }
  void set_min_x(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t min_x_address() const { return address + 92; }
  // the x evaluated (clamped to the bounds)
  float input() const { return guest::LoadFloat(base, address + 96); }
  void set_input(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t input_address() const { return address + 96; }
  // the y at it (left as it was when x was clamped)
  float output() const { return guest::LoadFloat(base, address + 100); }
  void set_output(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t output_address() const { return address + 100; }
  // the points' count
  uint32_t count() const { return guest::LoadU32(base, address + 108); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t count_address() const { return address + 108; }
};

// embedded; measure: -
struct HudCurvePoint : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
};

// embedded; measure: RaceHud::SetTime.r4, sub_820FF4D0.r3, sub_820FF628.r3, sub_820FF628.ret, sub_82213968.r4
struct RaceHudPackedTime : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t packed() const { return guest::LoadU32(base, address + 0); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t packed_address() const { return address + 0; }
  // the high half: bit 15 the sign, the minutes below
  uint16_t sign_minutes() const { return guest::LoadU16(base, address + 0); }
  void set_sign_minutes(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t sign_minutes_address() const { return address + 0; }
};

// embedded; measure: sub_820FF4D0.r4
struct RaceHudSplitTimes : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the first packed time; time i at +4 i
  uint32_t times() const { return guest::LoadU32(base, address + 0); }
  void set_times(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t times_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 60); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t count_address() const { return address + 60; }
};

// embedded; measure: UiMovie::FinishNamed.ret
struct RaceHudSignNode : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  // the node's picture
  uint32_t picture() const { return guest::LoadU32(base, address + 108); }
  void set_picture(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t picture_address() const { return address + 108; }
};

// embedded; measure: RaceScreens@+656
struct RaceHudAt656 : View {
  using View::View;
  static constexpr uint32_t kSize = 348;
  uint32_t unknown_328() const { return guest::LoadU32(base, address + 328); }
  void set_unknown_328(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  uint32_t unknown_336() const { return guest::LoadU32(base, address + 336); }
  void set_unknown_336(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  uint32_t unknown_340() const { return guest::LoadU32(base, address + 340); }
  void set_unknown_340(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  uint32_t unknown_344() const { return guest::LoadU32(base, address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
};

// The image words docs/functions/race-hud-object.md's code uses.
struct RaceHudObjectGlobals {
  // the race data (race-states.toml's RaceData; left untyped: alt_hud.cpp reads it without that header): +4 the course, the racers' records 3,936 bytes each from +0, +0xA3AF4 the race mode
  static View race_data(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's progress: +2184 the HUD's racers, +7156 a distance, +7240 the item shown
  template <typename T = RaceProgress>
  static T race_progress(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_race_progress(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82E8u, v.address); }
  static void set_race_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t race_progress_address() { return 0x826D82E8u; }
  // the viewports (camera-modes.toml's Viewports): +0 viewport 0 (+1860 its view)
  template <typename T = Viewports>
  static T viewports(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2CE8u, v.address); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the track zones' owner (track-zones.md)
  template <typename T = TrackZones>
  static T track_zones(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161ECu)); }
  static void set_track_zones(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161ECu, v.address); }
  static void set_track_zones(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161ECu, v); }
  static constexpr uint32_t track_zones_address() { return 0x828161ECu; }
  // the in-race screens' holder (race-hud-machine.toml's RaceScreens)
  template <typename T = RaceScreens>
  static T screens_holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F0u)); }
  static void set_screens_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F0u, v.address); }
  static void set_screens_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F0u, v); }
  static constexpr uint32_t screens_holder_address() { return 0x826D84F0u; }
  // the string table (Strings::Load and Release, 47)
  template <typename T = Strings>
  static T strings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C34u)); }
  static void set_strings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C34u, v.address); }
  static void set_strings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t strings_address() { return 0x826C5C34u; }
};

}  // namespace schema
