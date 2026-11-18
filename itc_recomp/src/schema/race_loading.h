// The views of docs/functions/race-loading.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-loading.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CourseSetup
struct CourseSetup : View {
  using View::View;
  static constexpr uint32_t kSize = 9652;
  // 200 transitions of 48 bytes (StateTransition); 11 added by Init
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 at set-up (started), 3 waiting for the course, 4 for the archive and the fade, 7 the race, 32 restarting, 37 the end, 35 unloaded
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // cleared by Begin and WaitDraw; not read here
  uint32_t unknown_9608() const { return guest::LoadU32(base, address + 9608); }
  void set_unknown_9608(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t unknown_9608_address() const { return address + 9608; }
  // cleared by Begin; not read here
  uint32_t unknown_9616() const { return guest::LoadU32(base, address + 9616); }
  void set_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t unknown_9616_address() const { return address + 9616; }
  // cleared by Begin; not read here
  uint32_t unknown_9620() const { return guest::LoadU32(base, address + 9620); }
  void set_unknown_9620(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t unknown_9620_address() const { return address + 9620; }
  // cleared by Begin; not read here
  uint32_t unknown_9628() const { return guest::LoadU32(base, address + 9628); }
  void set_unknown_9628(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t unknown_9628_address() const { return address + 9628; }
  // cleared by Begin; not read here
  uint32_t unknown_9632() const { return guest::LoadU32(base, address + 9632); }
  void set_unknown_9632(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t unknown_9632_address() const { return address + 9632; }
  // cleared by Begin; not read here
  uint32_t unknown_9636() const { return guest::LoadU32(base, address + 9636); }
  void set_unknown_9636(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t unknown_9636_address() const { return address + 9636; }
  // cleared by OnCourseReady; not read here
  uint32_t unknown_9640() const { return guest::LoadU32(base, address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
  // the end's tick count: 0 at set-up, counted by EndTick (message 50 once above 2)
  uint32_t end_ticks() const { return guest::LoadU32(base, address + 9644); }
  void set_end_ticks(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t end_ticks_address() const { return address + 9644; }
  // WaitDraw's step: 0 before the fade-out, 1 fading; cleared by WaitCourse
  uint32_t draw_step() const { return guest::LoadU32(base, address + 9648); }
  void set_draw_step(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t draw_step_address() const { return address + 9648; }
};

// embedded; measure: sub_821511B8.r3, sub_821512F0.r3, sub_82151388.r3
struct RaceModeInterface32 : View {
  using View::View;
  static constexpr uint32_t kSize = 167232;
  // 0x82006D28 (the mode's +32)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the +848 machine's state (CourseSetup): 7 racing; 37 from slot 1
  uint32_t course_setup_state() const { return guest::LoadU32(base, address + 10420); }
  void set_course_setup_state(uint32_t v) const { guest::StoreU32(base, address + 10420, v); }
  uint32_t course_setup_state_address() const { return address + 10420; }
  // the camera director's machine's state (mode +10528's): 37 from slot 1
  uint32_t camera_director_state() const { return guest::LoadU32(base, address + 20100); }
  void set_camera_director_state(uint32_t v) const { guest::StoreU32(base, address + 20100, v); }
  uint32_t camera_director_state_address() const { return address + 20100; }
  // mode +87936's machine's state: 37 from slot 1
  uint32_t race_scene_state() const { return guest::LoadU32(base, address + 97508); }
  void set_race_scene_state(uint32_t v) const { guest::StoreU32(base, address + 97508, v); }
  uint32_t race_scene_state_address() const { return address + 97508; }
  // mode +97548's machine's state: 37 from slot 1
  uint32_t race_flow_state() const { return guest::LoadU32(base, address + 107120); }
  void set_race_flow_state(uint32_t v) const { guest::StoreU32(base, address + 107120, v); }
  uint32_t race_flow_state_address() const { return address + 107120; }
  // mode +107168's machine's state: 37 from slot 1
  uint32_t race_states_state() const { return guest::LoadU32(base, address + 116740); }
  void set_race_states_state(uint32_t v) const { guest::StoreU32(base, address + 116740, v); }
  uint32_t race_states_state_address() const { return address + 116740; }
  // mode +116812's machine's state: 37 from slot 1
  uint32_t race_world_state() const { return guest::LoadU32(base, address + 126384); }
  void set_race_world_state(uint32_t v) const { guest::StoreU32(base, address + 126384, v); }
  uint32_t race_world_state_address() const { return address + 126384; }
  // mode +128664's machine's state: 24 from slot 1
  uint32_t race_hud_machine_state() const { return guest::LoadU32(base, address + 138236); }
  void set_race_hud_machine_state(uint32_t v) const { guest::StoreU32(base, address + 138236, v); }
  uint32_t race_hud_machine_state_address() const { return address + 138236; }
  // mode +138284's machine's state: 37 from slot 1
  uint32_t race_sounds_state() const { return guest::LoadU32(base, address + 147856); }
  void set_race_sounds_state(uint32_t v) const { guest::StoreU32(base, address + 147856, v); }
  uint32_t race_sounds_state_address() const { return address + 147856; }
  // mode +147944's machine's state: 6 from slot 1
  uint32_t overlays_machine_state() const { return guest::LoadU32(base, address + 157516); }
  void set_overlays_machine_state(uint32_t v) const { guest::StoreU32(base, address + 157516, v); }
  uint32_t overlays_machine_state_address() const { return address + 157516; }
  // the mode's +167260: 0 holds the Update messages (slot 0), 1 again (slots 1 and 2)
  uint32_t running() const { return guest::LoadU32(base, address + 167228); }
  void set_running(uint32_t v) const { guest::StoreU32(base, address + 167228, v); }
  uint32_t running_address() const { return address + 167228; }
};

// embedded; measure: UiMovie.+16
struct UiMovieSceneChunk : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  // 'TXT', 'PRM', 'PIC', ... 'TRM' (the last) in its top three bytes
  uint32_t tag() const { return guest::LoadU32(base, address + 0); }
  void set_tag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tag_address() const { return address + 0; }
  // the chunk's size in bytes (the next chunk follows)
  uint32_t size() const { return guest::LoadU32(base, address + 4); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  // probably the frame: set by SetFrames for TXT (its text value), PRM and PIC chunks
  uint32_t frame() const { return guest::LoadU32(base, address + 72); }
  void set_frame(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t frame_address() const { return address + 72; }
  uint32_t unknown_80() const { return guest::LoadU32(base, address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
};

// The image words docs/functions/race-loading.md's code uses.
struct RaceLoadingGlobals {
  // 1 once LoadingScreen::Show ran
  static uint32_t loading_shown(uint8_t* base) { return guest::LoadU32(base, 0x82815E60u); }
  static void set_loading_shown(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815E60u, v); }
  static constexpr uint32_t loading_shown_address() { return 0x82815E60u; }
  // the loading screen's current movie (0 to 2), set by Show
  static uint32_t loading_current(uint8_t* base) { return guest::LoadU32(base, 0x82815E64u); }
  static void set_loading_current(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815E64u, v); }
  static constexpr uint32_t loading_current_address() { return 0x82815E64u; }
  // the loading screen's three UI movies (a word each)
  static constexpr uint32_t loading_movies_address() { return 0x82815E68u; }
  // the animation each loading screen movie plays (a word each: 0, 13 and 0 in the image)
  static constexpr uint32_t loading_animations_address() { return 0x8263799Cu; }
  // the network session (network-mode.toml's Session) sub_82293460 tests; 0 in play
  static uint32_t system_request(uint8_t* base) { return guest::LoadU32(base, 0x82816E3Cu); }
  static void set_system_request(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816E3Cu, v); }
  static constexpr uint32_t system_request_address() { return 0x82816E3Cu; }
  // CourseSetup::Start's ScreenFades::FadeIn passes (5)
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // CourseSetup::WaitDraw's ScreenFades::FadeOut passes (15)
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // the screen fades' layer 0 alpha (0 to 255)
  static uint32_t layer0_alpha(uint8_t* base) { return guest::LoadU32(base, 0x826A50D4u); }
  static void set_layer0_alpha(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50D4u, v); }
  static constexpr uint32_t layer0_alpha_address() { return 0x826A50D4u; }
  // layer 0's fade-out count (passes left)
  static uint32_t layer0_fade_out(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_layer0_fade_out(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t layer0_fade_out_address() { return 0x826A50E4u; }
  // layer 0's fade-in count (passes left)
  static uint32_t layer0_fade_in(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_layer0_fade_in(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t layer0_fade_in_address() { return 0x826A50DCu; }
  // the course's collision mesh's load state (the word +8 into 0x826C5B1C): 2 loaded
  static uint32_t collision_state(uint8_t* base) { return guest::LoadU32(base, 0x826C5B24u); }
  static void set_collision_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B24u, v); }
  static constexpr uint32_t collision_state_address() { return 0x826C5B24u; }
  // the course's route's load state (the word +8 into 0x826C5B10): 2 loaded
  static uint32_t route_state(uint8_t* base) { return guest::LoadU32(base, 0x826C5B18u); }
  static void set_route_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B18u, v); }
  static constexpr uint32_t route_state_address() { return 0x826C5B18u; }
};

}  // namespace schema
