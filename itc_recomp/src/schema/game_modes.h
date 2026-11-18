// The views of docs/functions/game-modes.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/game-modes.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: -
struct StateMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (key, functor, next state)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // set once a mode's movies are loaded (the modes' WaitForMovies)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
};

// embedded; measure: -
struct StateTransition : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // the key (TransitionKey): all four words compared
  uint32_t key_address() const { return address + 0; }
  // the state the transition is taken in
  uint32_t state() const { return guest::LoadU32(base, address + 0); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t state_address() const { return address + 0; }
  // the message id (0 Update, 1 Draw)
  uint32_t message() const { return guest::LoadU32(base, address + 4); }
  void set_message(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t message_address() const { return address + 4; }
  // the message's second word
  uint32_t arg0() const { return guest::LoadU32(base, address + 8); }
  void set_arg0(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t arg0_address() const { return address + 8; }
  // the message's third word
  uint32_t arg1() const { return guest::LoadU32(base, address + 12); }
  void set_arg1(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t arg1_address() const { return address + 12; }
  // a pointer to the functor's storage (+20): the functor part (StateFunctorPart) starts here
  uint32_t functor() const { return guest::LoadU32(base, address + 16); }
  void set_functor(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t functor_address() const { return address + 16; }
  // the functor's storage (StateFunctor)
  uint32_t storage_address() const { return address + 20; }
  // 0x82011918 a member function, 0x820069BC nothing
  uint32_t functor_vtable() const { return guest::LoadU32(base, address + 20); }
  void set_functor_vtable(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t functor_vtable_address() const { return address + 20; }
  // the object (the state machine)
  uint32_t functor_object() const { return guest::LoadU32(base, address + 24); }
  void set_functor_object(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t functor_object_address() const { return address + 24; }
  // the function
  uint32_t functor_function() const { return guest::LoadU32(base, address + 28); }
  void set_functor_function(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t functor_function_address() const { return address + 28; }
  // 0: stay
  uint32_t next_state() const { return guest::LoadU32(base, address + 44); }
  void set_next_state(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t next_state_address() const { return address + 44; }
};

// embedded; measure: StateKey, StateKey::Equals.r4
struct TransitionKey : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t state() const { return guest::LoadU32(base, address + 0); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t state_address() const { return address + 0; }
  // the message id
  uint32_t message() const { return guest::LoadU32(base, address + 4); }
  void set_message(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t message_address() const { return address + 4; }
  uint32_t arg0() const { return guest::LoadU32(base, address + 8); }
  void set_arg0(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t arg0_address() const { return address + 8; }
  uint32_t arg1() const { return guest::LoadU32(base, address + 12); }
  void set_arg1(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t arg1_address() const { return address + 12; }
};

// embedded; measure: StateMachine::Add.r5, StateMachine::Find.ret, StateMachine::Handle.ret
struct StateFunctorPart : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // a pointer to the storage (+4); GameMessages::Dispatch calls its slot 0
  uint32_t functor() const { return guest::LoadU32(base, address + 0); }
  void set_functor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t functor_address() const { return address + 0; }
  // the functor's storage (StateFunctor)
  uint32_t storage_address() const { return address + 4; }
  // 0x82011918 a member function, 0x820069BC nothing
  uint32_t functor_vtable() const { return guest::LoadU32(base, address + 4); }
  void set_functor_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t functor_vtable_address() const { return address + 4; }
  uint32_t functor_object() const { return guest::LoadU32(base, address + 8); }
  void set_functor_object(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t functor_object_address() const { return address + 8; }
  uint32_t functor_function() const { return guest::LoadU32(base, address + 12); }
  void set_functor_function(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t functor_function_address() const { return address + 12; }
  // 0: stay
  uint32_t next_state() const { return guest::LoadU32(base, address + 28); }
  void set_next_state(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t next_state_address() const { return address + 28; }
};

// embedded; measure: StateMachine::Handle.ret.+0
struct StateFunctor : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // 0x82011918 a member function, 0x820069BC nothing
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the state machine
  uint32_t object() const { return guest::LoadU32(base, address + 4); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_address() const { return address + 4; }
  uint32_t function() const { return guest::LoadU32(base, address + 8); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t function_address() const { return address + 8; }
};

// embedded; measure: StateMachine::Handle.r4
struct ModeMessage : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0 Update, 1 Draw
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  uint32_t arg0() const { return guest::LoadU32(base, address + 4); }
  void set_arg0(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t arg0_address() const { return address + 4; }
  uint32_t arg1() const { return guest::LoadU32(base, address + 8); }
  void set_arg1(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t arg1_address() const { return address + 8; }
};

// allocated; measure: Modes
struct ModeHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x82006AF0;
  // 0x82006AF0 (slot 0 sub_820ACCF0; 1 to 3 the empty sub_82106540)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the current mode, 0 until Modes::Tick makes one
  template <typename T = GameMode>
  T mode() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_mode(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t mode_address() const { return address + 4; }
};

// allocated; measure: RaceMode
struct RaceMode : View {
  using View::View;
  static constexpr uint32_t kSize = 167264;
  static constexpr uint32_t kVtable = 0x82006D34;
  // 0x82006D34 (the base's 0x820069EC before)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), not the mode's
  uint8_t answer_changed() const { return guest::LoadU8(base, address + 0); }
  void set_answer_changed(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t answer_changed_address() const { return address + 0; }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer), not the mode's
  uint64_t answer() const { return guest::LoadU64(base, address + 0); }
  void set_answer(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t answer_address() const { return address + 0; }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // 0x82006B20 the default; 0x82010FA0 after RaceMode::Init
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_address() const { return address + 8; }
  // the mode
  uint32_t delegate_object() const { return guest::LoadU32(base, address + 12); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_address() const { return address + 12; }
  // 0x82235C08 Mode::NextModeThunk
  uint32_t delegate_function() const { return guest::LoadU32(base, address + 16); }
  void set_delegate_function(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_adjustment() const { return guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  // 0x82006D28 (the base's 0x82006A0C before)
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher (+36): 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // the course set-up's state machine (CourseSetup::, race-loading.md)
  uint32_t course_setup_address() const { return address + 848; }
  uint32_t course_setup_count() const { return guest::LoadU32(base, address + 10448); }
  void set_course_setup_count(uint32_t v) const { guest::StoreU32(base, address + 10448, v); }
  uint32_t course_setup_count_address() const { return address + 10448; }
  uint32_t course_setup_state() const { return guest::LoadU32(base, address + 10452); }
  void set_course_setup_state(uint32_t v) const { guest::StoreU32(base, address + 10452, v); }
  uint32_t course_setup_state_address() const { return address + 10452; }
  // the camera director (RaceModeMember::RaceModeMember; camera-modes.toml's CameraDirector), its state machine at +10528
  uint32_t camera_director_address() const { return address + 10512; }
  // the race scene's state machine (race-scene.md)
  uint32_t race_scene_address() const { return address + 87936; }
  uint32_t race_scene_count() const { return guest::LoadU32(base, address + 97536); }
  void set_race_scene_count(uint32_t v) const { guest::StoreU32(base, address + 97536, v); }
  uint32_t race_scene_count_address() const { return address + 97536; }
  uint32_t race_scene_state() const { return guest::LoadU32(base, address + 97540); }
  void set_race_scene_state(uint32_t v) const { guest::StoreU32(base, address + 97540, v); }
  uint32_t race_scene_state_address() const { return address + 97540; }
  // the race flow's state machine (race-flow.md, racer-controllers.md)
  uint32_t race_flow_address() const { return address + 97548; }
  uint32_t race_flow_count() const { return guest::LoadU32(base, address + 107148); }
  void set_race_flow_count(uint32_t v) const { guest::StoreU32(base, address + 107148, v); }
  uint32_t race_flow_count_address() const { return address + 107148; }
  uint32_t race_flow_state() const { return guest::LoadU32(base, address + 107152); }
  void set_race_flow_state(uint32_t v) const { guest::StoreU32(base, address + 107152, v); }
  uint32_t race_flow_state_address() const { return address + 107152; }
  // the race states' state machine (race-states.md)
  uint32_t race_states_address() const { return address + 107168; }
  uint32_t race_states_count() const { return guest::LoadU32(base, address + 116768); }
  void set_race_states_count(uint32_t v) const { guest::StoreU32(base, address + 116768, v); }
  uint32_t race_states_count_address() const { return address + 116768; }
  uint32_t race_states_state() const { return guest::LoadU32(base, address + 116772); }
  void set_race_states_state(uint32_t v) const { guest::StoreU32(base, address + 116772, v); }
  uint32_t race_states_state_address() const { return address + 116772; }
  // the race world's state machine (race-world.md)
  uint32_t race_world_address() const { return address + 116812; }
  uint32_t race_world_count() const { return guest::LoadU32(base, address + 126412); }
  void set_race_world_count(uint32_t v) const { guest::StoreU32(base, address + 126412, v); }
  uint32_t race_world_count_address() const { return address + 126412; }
  uint32_t race_world_state() const { return guest::LoadU32(base, address + 126416); }
  void set_race_world_state(uint32_t v) const { guest::StoreU32(base, address + 126416, v); }
  uint32_t race_world_state_address() const { return address + 126416; }
  // the race HUD machine (race-hud-machine.md)
  uint32_t race_hud_machine_address() const { return address + 128664; }
  uint32_t race_hud_machine_count() const { return guest::LoadU32(base, address + 138264); }
  void set_race_hud_machine_count(uint32_t v) const { guest::StoreU32(base, address + 138264, v); }
  uint32_t race_hud_machine_count_address() const { return address + 138264; }
  uint32_t race_hud_machine_state() const { return guest::LoadU32(base, address + 138268); }
  void set_race_hud_machine_state(uint32_t v) const { guest::StoreU32(base, address + 138268, v); }
  uint32_t race_hud_machine_state_address() const { return address + 138268; }
  // the race sounds' state machine (race-sounds.md)
  uint32_t race_sounds_address() const { return address + 138284; }
  uint32_t race_sounds_count() const { return guest::LoadU32(base, address + 147884); }
  void set_race_sounds_count(uint32_t v) const { guest::StoreU32(base, address + 147884, v); }
  uint32_t race_sounds_count_address() const { return address + 147884; }
  uint32_t race_sounds_state() const { return guest::LoadU32(base, address + 147888); }
  void set_race_sounds_state(uint32_t v) const { guest::StoreU32(base, address + 147888, v); }
  uint32_t race_sounds_state_address() const { return address + 147888; }
  // an XPR (XPR::XPR; the race sounds' machine's +9608)
  uint32_t xpr_address() const { return address + 147892; }
  // the race overlays' state machine (RaceOverlaysMachine)
  uint32_t overlays_machine_address() const { return address + 147944; }
  uint32_t overlays_machine_count() const { return guest::LoadU32(base, address + 157544); }
  void set_overlays_machine_count(uint32_t v) const { guest::StoreU32(base, address + 157544, v); }
  uint32_t overlays_machine_count_address() const { return address + 157544; }
  uint32_t overlays_machine_state() const { return guest::LoadU32(base, address + 157548); }
  void set_overlays_machine_state(uint32_t v) const { guest::StoreU32(base, address + 157548, v); }
  uint32_t overlays_machine_state_address() const { return address + 157548; }
  // the race overlays' list (RaceOverlays::Clear, race-overlays.md)
  uint32_t overlays_address() const { return address + 157552; }
  // a state machine RaceMode::Init doesn't set up or add
  uint32_t machine_157652_address() const { return address + 157652; }
  uint32_t machine_157652_count() const { return guest::LoadU32(base, address + 167252); }
  void set_machine_157652_count(uint32_t v) const { guest::StoreU32(base, address + 167252, v); }
  uint32_t machine_157652_count_address() const { return address + 167252; }
  uint32_t machine_157652_state() const { return guest::LoadU32(base, address + 167256); }
  void set_machine_157652_state(uint32_t v) const { guest::StoreU32(base, address + 167256, v); }
  uint32_t machine_157652_state_address() const { return address + 167256; }
  // 1 after RaceMode::Init; RaceMode::NextMode posts Update only while it isn't 0 (race-loading.md)
  uint32_t running() const { return guest::LoadU32(base, address + 167260); }
  void set_running(uint32_t v) const { guest::StoreU32(base, address + 167260, v); }
  uint32_t running_address() const { return address + 167260; }
};

// embedded; measure: RaceOverlaysMachine
struct RaceOverlaysMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9708;
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 at set-up, 1 the overlays made, 3, 2 the views bound, 5 running, 6 probably paused, 7 and 8 the end
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 25 overlay pointers (race-overlays.md's CarOverlays; 0 none)
  uint32_t overlays_address() const { return address + 9608; }
};

// embedded; measure: Buffer28, CameraPath::OnLoaded.r6
struct CameraPathBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr uint32_t kVtable = 0x82009390;
  // 0x82009390
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the file's first 20-byte path (CameraPath::OnLoaded)
  uint32_t first_path() const { return guest::LoadU32(base, address + 4); }
  void set_first_path(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_path_address() const { return address + 4; }
  // its last path
  uint32_t last_path() const { return guest::LoadU32(base, address + 8); }
  void set_last_path(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_path_address() const { return address + 8; }
  // -1 at reset
  uint32_t index() const { return guest::LoadU32(base, address + 12); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t index_address() const { return address + 12; }
  // Archive::Load's answer; -1 at reset
  uint32_t serial() const { return guest::LoadU32(base, address + 16); }
  void set_serial(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t serial_address() const { return address + 16; }
  // the VirtualAlloc block the file is read into; 0 none
  uint32_t block() const { return guest::LoadU32(base, address + 20); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t block_address() const { return address + 20; }
  // 0 at reset, 1 allocating, 2 loading, 3 loaded
  uint32_t state() const { return guest::LoadU32(base, address + 24); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t state_address() const { return address + 24; }
};

// embedded; measure: XprHolder
struct XprHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // -1 in the rival logo's (RivalLogo.holder)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the XPR (XPR::XPR)
  uint32_t xpr_address() const { return address + 4; }
  // 2 at set-up in the rival logo's, bit 0x10 loaded
  uint32_t xpr_flags() const { return guest::LoadU32(base, address + 28); }
  void set_xpr_flags(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t xpr_flags_address() const { return address + 28; }
  // two 12-byte records, cleared at construction and destruction
  uint32_t records_address() const { return address + 32; }
  uint32_t record0_word0() const { return guest::LoadU32(base, address + 32); }
  void set_record0_word0(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t record0_word0_address() const { return address + 32; }
  uint32_t record0_word1() const { return guest::LoadU32(base, address + 36); }
  void set_record0_word1(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t record0_word1_address() const { return address + 36; }
  uint32_t record0_word2() const { return guest::LoadU32(base, address + 40); }
  void set_record0_word2(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t record0_word2_address() const { return address + 40; }
  uint32_t record1_word0() const { return guest::LoadU32(base, address + 44); }
  void set_record1_word0(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t record1_word0_address() const { return address + 44; }
  uint32_t record1_word1() const { return guest::LoadU32(base, address + 48); }
  void set_record1_word1(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t record1_word1_address() const { return address + 48; }
  uint32_t record1_word2() const { return guest::LoadU32(base, address + 52); }
  void set_record1_word2(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t record1_word2_address() const { return address + 52; }
};

// allocated; measure: ModeType6
struct GameModeType6 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr uint32_t kVtable = 0x82010A48;
  // 0x82010A48
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82010A3C
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // 0 at construction (before the vtables); not read here
  uint32_t word_848() const { return guest::LoadU32(base, address + 848); }
  void set_word_848(uint32_t v) const { guest::StoreU32(base, address + 848, v); }
  uint32_t word_848_address() const { return address + 848; }
};

// allocated; measure: ModeType7
struct GameModeType7 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr uint32_t kVtable = 0x82010FB0;
  // 0x82010FB0
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82010FA4
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
};

// The image words docs/functions/game-modes.md's code uses.
struct GameModesGlobals {
  // the current mode's type (1 the race)
  static uint32_t current_type(uint8_t* base) { return guest::LoadU32(base, 0x826032DCu); }
  static void set_current_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032DCu, v); }
  static constexpr uint32_t current_type_address() { return 0x826032DCu; }
  // the type before it
  static uint32_t previous_type(uint8_t* base) { return guest::LoadU32(base, 0x826032E0u); }
  static void set_previous_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032E0u, v); }
  static constexpr uint32_t previous_type_address() { return 0x826032E0u; }
  // the data load's done flag (data-load.md)
  static uint32_t data_loaded(uint8_t* base) { return guest::LoadU32(base, 0x826C5B8Cu); }
  static void set_data_loaded(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B8Cu, v); }
  static constexpr uint32_t data_loaded_address() { return 0x826C5B8Cu; }
  // the play time frame counter
  static uint32_t play_frames(uint8_t* base) { return guest::LoadU32(base, 0x826C5B90u); }
  static void set_play_frames(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B90u, v); }
  static constexpr uint32_t play_frames_address() { return 0x826C5B90u; }
  // StateMachine::Find's static default (a StateFunctorPart)
  static constexpr uint32_t default_transition_address() { return 0x826C5B68u; }
  // bit 0x1: the default made
  static uint32_t default_guard(uint8_t* base) { return guest::LoadU32(base, 0x826C5B88u); }
  static void set_default_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B88u, v); }
  static constexpr uint32_t default_guard_address() { return 0x826C5B88u; }
  // the sign-in watch's state (mode-tick.md)
  static uint32_t sign_in_state(uint8_t* base) { return guest::LoadU32(base, 0x826BAB3Cu); }
  static void set_sign_in_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB3Cu, v); }
  static constexpr uint32_t sign_in_state_address() { return 0x826BAB3Cu; }
  // the mode's seven system checks (mode-tick.md)
  static constexpr uint32_t mode_checks_address() { return 0x826BAB40u; }
  // 1 makes the next mode change remake player 0's save data (mode-tick.md)
  static uint32_t remake_save_data(uint8_t* base) { return guest::LoadU32(base, 0x826BAB70u); }
  static void set_remake_save_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB70u, v); }
  static constexpr uint32_t remake_save_data_address() { return 0x826BAB70u; }
  // 1.0 in .rdata (the camera objects' w words)
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // the word Member13360::Member13360 copies to the TV camera's +452 (its route ranges' count)
  static uint32_t tv_camera_range_count(uint8_t* base) { return guest::LoadU32(base, 0x82618BB4u); }
  static void set_tv_camera_range_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82618BB4u, v); }
  static constexpr uint32_t tv_camera_range_count_address() { return 0x82618BB4u; }
};

}  // namespace schema
