// The views of docs/functions/race-hud-machine.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-hud-machine.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceHudMachine, PauseMenu4::Open.ret, AltHud::ToggleView.ret, RaceHud::RunMovie.ret
struct RaceHudMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateTransition, game-modes.toml); 65 added by RaceHudMachine::Init
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 at set-up; 1 waiting for the race world, 2 before the start, 3 a free run's intro, 4 a stage set up, 5 the countdown, 6 the HUD's movie +620, 7 the race, 8 the alternate HUD's view, 9 to 11 paused, 21 the results, 22 the menu after the race, 23 an abort, 24 and 25 the end (guesses)
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // the in-race screens' holder ([0x826D84F0], copied by CreateScreens)
  template <typename T = RaceScreens>
  T holder() const { return T(base, guest::LoadU32(base, address + 9608)); }
  void set_holder(const View& v) const { guest::StoreU32(base, address + 9608, v.address); }
  void set_holder(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t holder_address() const { return address + 9608; }
  // the race kind (the race data's +0xA3AF0, copied by CreateScreens): 1 to 3 and 6 the race HUD, 4 the holder's +656 object, 5 the alternate HUD, 0 the attract race's +12
  uint32_t kind() const { return guest::LoadU32(base, address + 9612); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t kind_address() const { return address + 9612; }
  // set by message 30 (OnMessage30), cleared by SetUp and ClosePause; read only by kind 4's pause menu
  uint32_t message30() const { return guest::LoadU32(base, address + 9616); }
  void set_message30(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t message30_address() const { return address + 9616; }
};

// embedded; measure: RaceScreens@+16.+436
struct RaceHud_436 : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  // measure fault: RaceHud::RunMovie's HUD +456 (its movie +604 run when not 0)
  uint32_t fault_hud_movie_running() const { return guest::LoadU32(base, address + 456); }
  void set_fault_hud_movie_running(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t fault_hud_movie_running_address() const { return address + 456; }
  // measure fault: RaceHud::RunMovie's HUD +604 (a UI movie)
  uint32_t fault_hud_movie() const { return guest::LoadU32(base, address + 604); }
  void set_fault_hud_movie(uint32_t v) const { guest::StoreU32(base, address + 604, v); }
  uint32_t fault_hud_movie_address() const { return address + 604; }
  // measure fault: the machine's +9608 (the holder), read by CountdownDraw and RaceDraw
  template <typename T = RaceScreens>
  T fault_machine_holder() const { return T(base, guest::LoadU32(base, address + 9608)); }
  void set_fault_machine_holder(const View& v) const { guest::StoreU32(base, address + 9608, v.address); }
  void set_fault_machine_holder(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t fault_machine_holder_address() const { return address + 9608; }
};

// allocated; measure: RaceScreens, PauseMenu4::Open.ret.+9608, AltHud::ToggleView.ret.+9608, RaceScreens@+16.+436.+9608
struct RaceScreens : View {
  using View::View;
  static constexpr uint32_t kSize = 1040;
  // 0 at set-up; bit 0 the replay bit (set when the menu after the race picks the replay: the alternate HUD drawn in place of the race HUD)
  uint32_t flags() const { return guest::LoadU32(base, address + 8); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t flags_address() const { return address + 8; }
  // the race HUD's +424 (signs on): 1 by CloseResults, 0 by OnRaceEnd
  uint32_t hud_signs_on() const { return guest::LoadU32(base, address + 440); }
  void set_hud_signs_on(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t hud_signs_on_address() const { return address + 440; }
  // the race HUD's +436: CountdownUpdate goes to the race once it's 0
  uint32_t hud_countdown_running() const { return guest::LoadU32(base, address + 452); }
  void set_hud_countdown_running(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t hud_countdown_running_address() const { return address + 452; }
  // the race HUD's +444: the race progress's ([0x826D82E8]) +2184, its results record (StartCountdown)
  template <typename T = RacerResultsRecord>
  T hud_results() const { return T(base, guest::LoadU32(base, address + 460)); }
  void set_hud_results(const View& v) const { guest::StoreU32(base, address + 460, v.address); }
  void set_hud_results(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t hud_results_address() const { return address + 460; }
  // measure fault: RaceUpdate's post, the race mode's dispatcher's ring count (+804)
  uint32_t fault_ring_count() const { return guest::LoadU32(base, address + 820); }
  void set_fault_ring_count(uint32_t v) const { guest::StoreU32(base, address + 820, v); }
  uint32_t fault_ring_count_address() const { return address + 820; }
  // measure fault: RaceUpdate's post, the dispatcher's ring start (+808)
  uint32_t fault_ring_start() const { return guest::LoadU32(base, address + 824); }
  void set_fault_ring_start(uint32_t v) const { guest::StoreU32(base, address + 824, v); }
  uint32_t fault_ring_start_address() const { return address + 824; }
  // the +656 object's +344: the race progress's +2184 (StartCountdown, kind 4)
  template <typename T = RacerResultsRecord>
  T split_results() const { return T(base, guest::LoadU32(base, address + 1000)); }
  void set_split_results(const View& v) const { guest::StoreU32(base, address + 1000, v.address); }
  void set_split_results(uint32_t v) const { guest::StoreU32(base, address + 1000, v); }
  uint32_t split_results_address() const { return address + 1000; }
  // the alternate HUD's holder (AltHudHolder, alt-hud.toml): the object and its menu list
  uint32_t alt_hud_address() const { return address + 1004; }
  // [0x826D84D0], the pause menu (the measure's PauseMenu4 ... new<4>@sub_8220D7E8; race-screens.md)
  View pause_menu() const { return View(base, guest::LoadU32(base, address + 1012)); }
  void set_pause_menu(uint32_t v) const { guest::StoreU32(base, address + 1012, v); }
  uint32_t pause_menu_address() const { return address + 1012; }
  // [0x826D84E0], kind 4's pause menu, probably (the measure's PauseMenu4)
  View kind4_pause_menu() const { return View(base, guest::LoadU32(base, address + 1016)); }
  void set_kind4_pause_menu(uint32_t v) const { guest::StoreU32(base, address + 1016, v); }
  uint32_t kind4_pause_menu_address() const { return address + 1016; }
  // [0x826D8510], the menu after the race (the measure's new<4>@sub_8221B1F0)
  View after_race_menu() const { return View(base, guest::LoadU32(base, address + 1020)); }
  void set_after_race_menu(uint32_t v) const { guest::StoreU32(base, address + 1020, v); }
  uint32_t after_race_menu_address() const { return address + 1020; }
  // [0x826D8500], the career's results (the measure's CareerResults, race-screens.md)
  View career_results() const { return View(base, guest::LoadU32(base, address + 1024)); }
  void set_career_results(uint32_t v) const { guest::StoreU32(base, address + 1024, v); }
  uint32_t career_results_address() const { return address + 1024; }
  // [0x82814E18], the results of kinds 1 (mode 0) and 2 (the measure's RaceResults)
  View race_results() const { return View(base, guest::LoadU32(base, address + 1028)); }
  void set_race_results(uint32_t v) const { guest::StoreU32(base, address + 1028, v); }
  uint32_t race_results_address() const { return address + 1028; }
  // [0x82814E1C], the results of kinds 4 and 6 (the measure's SplitResults)
  View split_results_screen() const { return View(base, guest::LoadU32(base, address + 1032)); }
  void set_split_results_screen(uint32_t v) const { guest::StoreU32(base, address + 1032, v); }
  uint32_t split_results_screen_address() const { return address + 1032; }
  // [0x82814E10], kind 6's screen after the race (the measure's Kind6Menu)
  View kind6_menu() const { return View(base, guest::LoadU32(base, address + 1036)); }
  void set_kind6_menu(uint32_t v) const { guest::StoreU32(base, address + 1036, v); }
  uint32_t kind6_menu_address() const { return address + 1036; }
};

// embedded; measure: RaceHudMachine::PauseUpdate.r4, RaceHudMachine::AfterRaceUpdate.r4, RaceHudMachine::ResultsUpdate.r4
struct RaceHudMachineDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 0; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 200); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t handler_count_address() const { return address + 200; }
  // 50 messages of 12 bytes (ModeMessage: +0 the message, +4 and +8 0)
  uint32_t ring_address() const { return address + 204; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
};

// embedded; measure: RacerRecords::At.ret, Vehicle::Get.ret.+5712, sub_82215A50.r3.+344, sub_82216908.r3.+344, sub_82216950.r3.+344, RaceScreens@+656.+344
struct RacerResultsRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  // the entries' count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // E +0: the racer record's index
  uint32_t e0_record_index() const { return guest::LoadU32(base, address + 4); }
  void set_e0_record_index(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t e0_record_index_address() const { return address + 4; }
  // E +4: its item (an id; AwardPrize's opponents, from entry 1)
  uint32_t e0_item() const { return guest::LoadU32(base, address + 8); }
  void set_e0_item(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t e0_item_address() const { return address + 8; }
  // E +8: its place (0 the leader)
  uint32_t e0_place() const { return guest::LoadU32(base, address + 12); }
  void set_e0_place(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t e0_place_address() const { return address + 12; }
  // E +12: its ticks raced
  uint32_t e0_ticks() const { return guest::LoadU32(base, address + 16); }
  void set_e0_ticks(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t e0_ticks_address() const { return address + 16; }
  // E +16: its time (RaceTime::FromTicks)
  uint32_t e0_time() const { return guest::LoadU32(base, address + 20); }
  void set_e0_time(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t e0_time_address() const { return address + 20; }
  // E +780: ticks with the vehicle's byte +2424 set
  uint32_t e0_flag_ticks() const { return guest::LoadU32(base, address + 784); }
  void set_e0_flag_ticks(uint32_t v) const { guest::StoreU32(base, address + 784, v); }
  uint32_t e0_flag_ticks_address() const { return address + 784; }
  // E +784: SP (probably spirit points; 0 is out)
  uint32_t e0_sp() const { return guest::LoadU32(base, address + 788); }
  void set_e0_sp(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t e0_sp_address() const { return address + 788; }
  // E +792: a running mean of the speed in km/h
  uint32_t e0_mean_speed() const { return guest::LoadU32(base, address + 796); }
  void set_e0_mean_speed(uint32_t v) const { guest::StoreU32(base, address + 796, v); }
  uint32_t e0_mean_speed_address() const { return address + 796; }
  // E +792 read as a doubleword
  uint64_t e0_mean_speed_pair() const { return guest::LoadU64(base, address + 796); }
  void set_e0_mean_speed_pair(uint64_t v) const { guest::StoreU64(base, address + 796, v); }
  uint32_t e0_mean_speed_pair_address() const { return address + 796; }
  uint32_t unknown_800() const { return guest::LoadU32(base, address + 800); }
  void set_unknown_800(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t unknown_800_address() const { return address + 800; }
  // E +800: the top speed
  float e0_top_speed() const { return guest::LoadFloat(base, address + 804); }
  void set_e0_top_speed(float v) const { guest::StoreFloat(base, address + 804, v); }
  uint32_t e0_top_speed_address() const { return address + 804; }
  // E +804: a hard hit (230.0 or more)
  uint32_t e0_hard_hit() const { return guest::LoadU32(base, address + 808); }
  void set_e0_hard_hit(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t e0_hard_hit_address() const { return address + 808; }
  // E +808
  uint32_t e0_track_hits() const { return guest::LoadU32(base, address + 812); }
  void set_e0_track_hits(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t e0_track_hits_address() const { return address + 812; }
  // E +812
  uint32_t e0_car_hits() const { return guest::LoadU32(base, address + 816); }
  void set_e0_car_hits(uint32_t v) const { guest::StoreU32(base, address + 816, v); }
  uint32_t e0_car_hits_address() const { return address + 816; }
  // E +816: the distance in km
  float e0_distance() const { return guest::LoadFloat(base, address + 820); }
  void set_e0_distance(float v) const { guest::StoreFloat(base, address + 820, v); }
  uint32_t e0_distance_address() const { return address + 820; }
  // E +824: the distance along the stage's route (the place's key); AwardPrize's prize from it
  float e0_route_distance() const { return guest::LoadFloat(base, address + 828); }
  void set_e0_route_distance(float v) const { guest::StoreFloat(base, address + 828, v); }
  uint32_t e0_route_distance_address() const { return address + 828; }
  // E +840: the first of 8-byte entries, a float each (AwardPrize's +24; their count +968)
  float e0_split_0() const { return guest::LoadFloat(base, address + 844); }
  void set_e0_split_0(float v) const { guest::StoreFloat(base, address + 844, v); }
  uint32_t e0_split_0_address() const { return address + 844; }
  // E +848: the second (AwardPrize's pairs from here, 8 bytes apart)
  float e0_split_1() const { return guest::LoadFloat(base, address + 852); }
  void set_e0_split_1(float v) const { guest::StoreFloat(base, address + 852, v); }
  uint32_t e0_split_1_address() const { return address + 852; }
  // E +964: the 8-byte entries' count
  uint16_t e0_split_count() const { return guest::LoadU16(base, address + 968); }
  void set_e0_split_count(uint16_t v) const { guest::StoreU16(base, address + 968, v); }
  uint32_t e0_split_count_address() const { return address + 968; }
  // E +968: 1 once the input's halfword +6 or +10 was nonzero
  uint32_t e0_input_seen() const { return guest::LoadU32(base, address + 972); }
  void set_e0_input_seen(uint32_t v) const { guest::StoreU32(base, address + 972, v); }
  uint32_t e0_input_seen_address() const { return address + 972; }
  // entry 1's item (AwardPrize walks +980 + 972i)
  uint32_t e1_item() const { return guest::LoadU32(base, address + 980); }
  void set_e1_item(uint32_t v) const { guest::StoreU32(base, address + 980, v); }
  uint32_t e1_item_address() const { return address + 980; }
  // entry 1's SP
  uint32_t e1_sp() const { return guest::LoadU32(base, address + 1760); }
  void set_e1_sp(uint32_t v) const { guest::StoreU32(base, address + 1760, v); }
  uint32_t e1_sp_address() const { return address + 1760; }
  // a 16-byte route's first word
  uint32_t route() const { return guest::LoadU32(base, address + 3892); }
  void set_route(uint32_t v) const { guest::StoreU32(base, address + 3892, v); }
  uint32_t route_address() const { return address + 3892; }
  // kind 6's result: 0 or 2 posts 40 after the race, 1 or 3 its own screen
  uint32_t kind6_result() const { return guest::LoadU32(base, address + 3916); }
  void set_kind6_result(uint32_t v) const { guest::StoreU32(base, address + 3916, v); }
  uint32_t kind6_result_address() const { return address + 3916; }
  // 1 finished (probably), 3 tested (the menu after the race's flag; AwardPrize's +0 2)
  uint32_t outcome() const { return guest::LoadU32(base, address + 3920); }
  void set_outcome(uint32_t v) const { guest::StoreU32(base, address + 3920, v); }
  uint32_t outcome_address() const { return address + 3920; }
  // 1 probably a win, 2 a loss, 3
  uint32_t finish() const { return guest::LoadU32(base, address + 3924); }
  void set_finish(uint32_t v) const { guest::StoreU32(base, address + 3924, v); }
  uint32_t finish_address() const { return address + 3924; }
  uint32_t unknown_3928() const { return guest::LoadU32(base, address + 3928); }
  void set_unknown_3928(uint32_t v) const { guest::StoreU32(base, address + 3928, v); }
  uint32_t unknown_3928_address() const { return address + 3928; }
  // probably the race's tick count (RaceHud::RunTimerMovie's FromTicks)
  uint32_t ticks() const { return guest::LoadU32(base, address + 3932); }
  void set_ticks(uint32_t v) const { guest::StoreU32(base, address + 3932, v); }
  uint32_t ticks_address() const { return address + 3932; }
};

// embedded; measure: sub_82215A50.r3, sub_82215D08.r3, sub_82215DA8.r3, sub_82216908.r3, sub_82216950.r3, sub_82216A40.r3
struct SplitHud : View {
  using View::View;
  static constexpr uint32_t kSize = 348;
  float unknown_248() const { return guest::LoadFloat(base, address + 248); }
  void set_unknown_248(float v) const { guest::StoreFloat(base, address + 248, v); }
  uint32_t unknown_248_address() const { return address + 248; }
  float unknown_252() const { return guest::LoadFloat(base, address + 252); }
  void set_unknown_252(float v) const { guest::StoreFloat(base, address + 252, v); }
  uint32_t unknown_252_address() const { return address + 252; }
  float unknown_256() const { return guest::LoadFloat(base, address + 256); }
  void set_unknown_256(float v) const { guest::StoreFloat(base, address + 256, v); }
  uint32_t unknown_256_address() const { return address + 256; }
  float unknown_260() const { return guest::LoadFloat(base, address + 260); }
  void set_unknown_260(float v) const { guest::StoreFloat(base, address + 260, v); }
  uint32_t unknown_260_address() const { return address + 260; }
  float unknown_264() const { return guest::LoadFloat(base, address + 264); }
  void set_unknown_264(float v) const { guest::StoreFloat(base, address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  float unknown_268() const { return guest::LoadFloat(base, address + 268); }
  void set_unknown_268(float v) const { guest::StoreFloat(base, address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  float unknown_272() const { return guest::LoadFloat(base, address + 272); }
  void set_unknown_272(float v) const { guest::StoreFloat(base, address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  uint32_t unknown_280() const { return guest::LoadU32(base, address + 280); }
  void set_unknown_280(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t unknown_280_address() const { return address + 280; }
  uint32_t unknown_284() const { return guest::LoadU32(base, address + 284); }
  void set_unknown_284(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
  // as the race HUD's +436, probably: CountdownUpdate's kind 4 goes to the race once it's 0; written by sub_822159F0 (the start), sub_82215D08 (the stop) and sub_82215DA8
  uint32_t countdown_running() const { return guest::LoadU32(base, address + 336); }
  void set_countdown_running(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t countdown_running_address() const { return address + 336; }
  // probably the mode sub_82216810 sets (StartCountdown's kind 4); read by sub_82216A40 and sub_82216830
  uint32_t mode() const { return guest::LoadU32(base, address + 340); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t mode_address() const { return address + 340; }
  // the race progress's +2184 (the holder's +1000, StartCountdown)
  template <typename T = RacerResultsRecord>
  T results() const { return T(base, guest::LoadU32(base, address + 344)); }
  void set_results(const View& v) const { guest::StoreU32(base, address + 344, v.address); }
  void set_results(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t results_address() const { return address + 344; }
};

// embedded; measure: sub_82216428.r5
struct SplitHudTime : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // probably a packed time: bits 16 to 30 capped at 59 (minutes or seconds)
  uint32_t packed() const { return guest::LoadU32(base, address + 0); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t packed_address() const { return address + 0; }
};

// embedded; measure: RaceMode@+128664
struct RaceHudMachineAt128664 : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
};

// The image words docs/functions/race-hud-machine.md's code uses.
struct RaceHudMachineGlobals {
  // the in-race screens' holder (PauseUpdate reads it in place of the machine's +9608)
  template <typename T = RaceScreens>
  static T holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F0u)); }
  static void set_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F0u, v.address); }
  static void set_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F0u, v); }
  static constexpr uint32_t holder_address() { return 0x826D84F0u; }
  // the race data (the measure's Racers): +0xA3AF0 the race kind, +0xA3AF4 the race mode, +0x59620 the stages, +0x59AE8 the racer records
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the racers' manager (the measure's Race): +2308 the player's MinimapRacerEntry
  template <typename T = RaceManager>
  static T racers_manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_racers_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // the race's progress: +2184 its results record (RacerResultsRecord)
  static View race_progress(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_race_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t race_progress_address() { return 0x826D82E8u; }
  // the network mode's session (0 in play): PauseUpdate's answer 9 with the mode 7 sets its +240
  template <typename T = Session>
  static T network_session(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816E3Cu)); }
  static void set_network_session(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816E3Cu, v.address); }
  static void set_network_session(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816E3Cu, v); }
  static constexpr uint32_t network_session_address() { return 0x82816E3Cu; }
  // 10.0 (AwardPrize: the route distance's factor)
  static float prize_distance_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_prize_distance_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t prize_distance_scale_address() { return 0x82001FD8u; }
  // AwardPrize's factor for a win streak of 20 and more (probably 2.0)
  static float streak_factor_20(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_streak_factor_20(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t streak_factor_20_address() { return 0x820021E8u; }
  // for 15 and more
  static float streak_factor_15(uint8_t* base) { return guest::LoadFloat(base, 0x82002CF0u); }
  static void set_streak_factor_15(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002CF0u, v); }
  static constexpr uint32_t streak_factor_15_address() { return 0x82002CF0u; }
  // for 10 and more
  static float streak_factor_10(uint8_t* base) { return guest::LoadFloat(base, 0x820027A8u); }
  static void set_streak_factor_10(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A8u, v); }
  static constexpr uint32_t streak_factor_10_address() { return 0x820027A8u; }
  // for 7 and more
  static float streak_factor_7(uint8_t* base) { return guest::LoadFloat(base, 0x820029F0u); }
  static void set_streak_factor_7(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029F0u, v); }
  static constexpr uint32_t streak_factor_7_address() { return 0x820029F0u; }
  // for 5 and more
  static float streak_factor_5(uint8_t* base) { return guest::LoadFloat(base, 0x820029D4u); }
  static void set_streak_factor_5(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D4u, v); }
  static constexpr uint32_t streak_factor_5_address() { return 0x820029D4u; }
  // for 3 and more
  static float streak_factor_3(uint8_t* base) { return guest::LoadFloat(base, 0x82002CB0u); }
  static void set_streak_factor_3(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002CB0u, v); }
  static constexpr uint32_t streak_factor_3_address() { return 0x82002CB0u; }
  // AwardPrize's splits' factor (probably -1.0, not read)
  static float split_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_split_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t split_scale_address() { return 0x82001BE0u; }
  // 1.0, probably (AwardPrize's factor below a streak of 3)
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
