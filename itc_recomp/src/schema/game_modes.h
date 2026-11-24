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
#pragma pack(push, 1)
struct ModeHolderData {
  uint32_t vtable = 0;  // +0
  uint32_t mode = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(ModeHolderData) == 8);
inline constexpr memory::LayoutField kModeHolderFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kModeHolderLayout = {kModeHolderFields, 2};

struct ModeHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82006AF0;
  ModeHolderData* data() const { return static_cast<ModeHolderData*>(memory::HandleObject(address)); }
  static ModeHolder New(uint8_t* base) {
    return ModeHolder(base, memory::MakeHandle(new ModeHolderData(), sizeof(ModeHolderData), "ModeHolder", &kModeHolderLayout));
  }
  template <uint32_t Stride = kSize>
  static ModeHolder NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeHolderData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeHolderData*>(::operator new(sizeof(ModeHolderData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeHolderData();
    return ModeHolder(base, memory::MakeHandle(p, sizeof(ModeHolderData) * count, "ModeHolder", &kModeHolderLayout, sizeof(ModeHolderData)));
  }
  ModeHolder at(uint32_t i) const { return ModeHolder(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeHolderLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeHolderLayout, offset, width, v); }
  // 0x82006AF0 (slot 0 sub_820ACCF0; 1 to 3 the empty sub_82106540)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the current mode, 0 until Modes::Tick makes one
  uint32_t mode_address() const { return address + 4; }
  template <typename T = GameMode>
  T mode() const { return T(base, data()->mode); }
  void set_mode(const View& w) const { const uint32_t v = w.address; data()->mode = v; }
  void set_mode(uint32_t v) const { data()->mode = v; }
  uint32_t mode_bits() const { return data()->mode; }
  void set_mode_bits(uint32_t v) const { data()->mode = v; }
};

// allocated; measure: RaceMode
#pragma pack(push, 1)
struct RaceModeData {
  uint32_t vtable = 0;  // +0
  uint32_t delegate_ptr = 0;  // +4
  uint32_t delegate_vtable = 0;  // +8
  uint32_t delegate_object = 0;  // +12
  uint32_t delegate_function = 0;  // +16
  uint32_t delegate_adjustment = 0;  // +20
  uint8_t pad_24[8] = {};
  uint32_t vtable32 = 0;  // +32
  uint8_t handlers[200] = {};  // +36, guest order
  uint32_t handler_count = 0;  // +236
  uint8_t ring[600] = {};  // +240, guest order
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
  uint8_t course_setup[9600] = {};  // +848, guest order
  uint32_t course_setup_count = 0;  // +10448
  uint32_t course_setup_state = 0;  // +10452
  uint8_t pad_10456[56] = {};
  uint8_t camera_director[77392] = {};  // +10512, guest order
  uint8_t pad_87904[32] = {};
  uint8_t race_scene[9600] = {};  // +87936, guest order
  uint32_t race_scene_count = 0;  // +97536
  uint32_t race_scene_state = 0;  // +97540
  uint8_t pad_97544[4] = {};
  uint8_t race_flow[9600] = {};  // +97548, guest order
  uint32_t race_flow_count = 0;  // +107148
  uint32_t race_flow_state = 0;  // +107152
  uint8_t pad_107156[12] = {};
  uint8_t race_states[9600] = {};  // +107168, guest order
  uint32_t race_states_count = 0;  // +116768
  uint32_t race_states_state = 0;  // +116772
  uint8_t pad_116776[36] = {};
  uint8_t race_world[9600] = {};  // +116812, guest order
  uint32_t race_world_count = 0;  // +126412
  uint32_t race_world_state = 0;  // +126416
  uint8_t pad_126420[2244] = {};
  uint8_t race_hud_machine[9600] = {};  // +128664, guest order
  uint32_t race_hud_machine_count = 0;  // +138264
  uint32_t race_hud_machine_state = 0;  // +138268
  uint8_t pad_138272[12] = {};
  uint8_t race_sounds[9600] = {};  // +138284, guest order
  uint32_t race_sounds_count = 0;  // +147884
  uint32_t race_sounds_state = 0;  // +147888
  uint8_t xpr[24] = {};  // +147892, guest order
  uint8_t pad_147916[28] = {};
  uint8_t overlays_machine[9600] = {};  // +147944, guest order
  uint32_t overlays_machine_count = 0;  // +157544
  uint32_t overlays_machine_state = 0;  // +157548
  uint8_t overlays[100] = {};  // +157552, guest order
  uint8_t machine_157652[9600] = {};  // +157652, guest order
  uint32_t machine_157652_count = 0;  // +167252
  uint32_t machine_157652_state = 0;  // +167256
  uint32_t running = 0;  // +167260
};
#pragma pack(pop)
static_assert(sizeof(RaceModeData) == 167264);
inline constexpr memory::LayoutField kRaceModeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {840, 4}, {844, 4}, {10448, 4}, {10452, 4}, {97536, 4}, {97540, 4}, {107148, 4}, {107152, 4}, {116768, 4}, {116772, 4}, {126412, 4}, {126416, 4}, {138264, 4}, {138268, 4}, {147884, 4}, {147888, 4}, {157544, 4}, {157548, 4}, {167252, 4}, {167256, 4}, {167260, 4}};
inline constexpr memory::Layout kRaceModeLayout = {kRaceModeFields, 29};

struct RaceMode : View {
  using View::View;
  static constexpr uint32_t kSize = 167264;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82006D34;
  RaceModeData* data() const { return static_cast<RaceModeData*>(memory::HandleObject(address)); }
  static RaceMode New(uint8_t* base) {
    return RaceMode(base, memory::MakeHandle(new RaceModeData(), sizeof(RaceModeData), "RaceMode", &kRaceModeLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceModeData*>(::operator new(sizeof(RaceModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceModeData();
    return RaceMode(base, memory::MakeHandle(p, sizeof(RaceModeData) * count, "RaceMode", &kRaceModeLayout, sizeof(RaceModeData)));
  }
  RaceMode at(uint32_t i) const { return RaceMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kRaceModeLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceModeLayout, offset, width, v); }
  // 0x82006D34 (the base's 0x820069EC before)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kRaceModeLayout, 0, 1)); }
  void set_answer_changed_bits(uint8_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceModeLayout, 0, 1, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer), not the mode's
  uint32_t answer_address() const { return address + 0; }
  uint64_t answer_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kRaceModeLayout, 0, 8)); }
  void set_answer_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRaceModeLayout, 0, 8, v); }
  uint64_t answer() const { return uint64_t(answer_bits()); }
  void set_answer(uint64_t v) const { set_answer_bits(uint64_t(v)); }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
  uint32_t delegate_ptr_address() const { return address + 4; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, data()->delegate_ptr); }
  void set_delegate_ptr(const View& w) const { const uint32_t v = w.address; data()->delegate_ptr = v; }
  void set_delegate_ptr(uint32_t v) const { data()->delegate_ptr = v; }
  uint32_t delegate_ptr_bits() const { return data()->delegate_ptr; }
  void set_delegate_ptr_bits(uint32_t v) const { data()->delegate_ptr = v; }
  // 0x82006B20 the default; 0x82010FA0 after RaceMode::Init
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable() const { return data()->delegate_vtable; }
  void set_delegate_vtable(uint32_t v) const { data()->delegate_vtable = v; }
  uint32_t delegate_vtable_bits() const { return data()->delegate_vtable; }
  void set_delegate_vtable_bits(uint32_t v) const { data()->delegate_vtable = v; }
  // the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object() const { return data()->delegate_object; }
  void set_delegate_object(uint32_t v) const { data()->delegate_object = v; }
  uint32_t delegate_object_bits() const { return data()->delegate_object; }
  void set_delegate_object_bits(uint32_t v) const { data()->delegate_object = v; }
  // 0x82235C08 Mode::NextModeThunk
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function() const { return data()->delegate_function; }
  void set_delegate_function(uint32_t v) const { data()->delegate_function = v; }
  uint32_t delegate_function_bits() const { return data()->delegate_function; }
  void set_delegate_function_bits(uint32_t v) const { data()->delegate_function = v; }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment() const { return data()->delegate_adjustment; }
  void set_delegate_adjustment(uint32_t v) const { data()->delegate_adjustment = v; }
  uint32_t delegate_adjustment_bits() const { return data()->delegate_adjustment; }
  void set_delegate_adjustment_bits(uint32_t v) const { data()->delegate_adjustment = v; }
  // 0x82006D28 (the base's 0x82006A0C before)
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32() const { return data()->vtable32; }
  void set_vtable32(uint32_t v) const { data()->vtable32 = v; }
  uint32_t vtable32_bits() const { return data()->vtable32; }
  void set_vtable32_bits(uint32_t v) const { data()->vtable32 = v; }
  // the dispatcher (+36): 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return reinterpret_cast<uint8_t*>(data()) + 36; }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count() const { return data()->handler_count; }
  void set_handler_count(uint32_t v) const { data()->handler_count = v; }
  uint32_t handler_count_bits() const { return data()->handler_count; }
  void set_handler_count_bits(uint32_t v) const { data()->handler_count = v; }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return reinterpret_cast<uint8_t*>(data()) + 240; }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count() const { return data()->ring_count; }
  void set_ring_count(uint32_t v) const { data()->ring_count = v; }
  uint32_t ring_count_bits() const { return data()->ring_count; }
  void set_ring_count_bits(uint32_t v) const { data()->ring_count = v; }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start() const { return data()->ring_start; }
  void set_ring_start(uint32_t v) const { data()->ring_start = v; }
  uint32_t ring_start_bits() const { return data()->ring_start; }
  void set_ring_start_bits(uint32_t v) const { data()->ring_start = v; }
  // the course set-up's state machine (CourseSetup::, race-loading.md)
  uint32_t course_setup_address() const { return address + 848; }
  uint8_t* course_setup() const { return reinterpret_cast<uint8_t*>(data()) + 848; }
  uint32_t course_setup_count_address() const { return address + 10448; }
  uint32_t course_setup_count() const { return data()->course_setup_count; }
  void set_course_setup_count(uint32_t v) const { data()->course_setup_count = v; }
  uint32_t course_setup_count_bits() const { return data()->course_setup_count; }
  void set_course_setup_count_bits(uint32_t v) const { data()->course_setup_count = v; }
  uint32_t course_setup_state_address() const { return address + 10452; }
  uint32_t course_setup_state() const { return data()->course_setup_state; }
  void set_course_setup_state(uint32_t v) const { data()->course_setup_state = v; }
  uint32_t course_setup_state_bits() const { return data()->course_setup_state; }
  void set_course_setup_state_bits(uint32_t v) const { data()->course_setup_state = v; }
  // the camera director (RaceModeMember::RaceModeMember; camera-modes.toml's CameraDirector), its state machine at +10528
  uint32_t camera_director_address() const { return address + 10512; }
  uint8_t* camera_director() const { return reinterpret_cast<uint8_t*>(data()) + 10512; }
  // the race scene's state machine (race-scene.md)
  uint32_t race_scene_address() const { return address + 87936; }
  uint8_t* race_scene() const { return reinterpret_cast<uint8_t*>(data()) + 87936; }
  uint32_t race_scene_count_address() const { return address + 97536; }
  uint32_t race_scene_count() const { return data()->race_scene_count; }
  void set_race_scene_count(uint32_t v) const { data()->race_scene_count = v; }
  uint32_t race_scene_count_bits() const { return data()->race_scene_count; }
  void set_race_scene_count_bits(uint32_t v) const { data()->race_scene_count = v; }
  uint32_t race_scene_state_address() const { return address + 97540; }
  uint32_t race_scene_state() const { return data()->race_scene_state; }
  void set_race_scene_state(uint32_t v) const { data()->race_scene_state = v; }
  uint32_t race_scene_state_bits() const { return data()->race_scene_state; }
  void set_race_scene_state_bits(uint32_t v) const { data()->race_scene_state = v; }
  // the race flow's state machine (race-flow.md, racer-controllers.md)
  uint32_t race_flow_address() const { return address + 97548; }
  uint8_t* race_flow() const { return reinterpret_cast<uint8_t*>(data()) + 97548; }
  uint32_t race_flow_count_address() const { return address + 107148; }
  uint32_t race_flow_count() const { return data()->race_flow_count; }
  void set_race_flow_count(uint32_t v) const { data()->race_flow_count = v; }
  uint32_t race_flow_count_bits() const { return data()->race_flow_count; }
  void set_race_flow_count_bits(uint32_t v) const { data()->race_flow_count = v; }
  uint32_t race_flow_state_address() const { return address + 107152; }
  uint32_t race_flow_state() const { return data()->race_flow_state; }
  void set_race_flow_state(uint32_t v) const { data()->race_flow_state = v; }
  uint32_t race_flow_state_bits() const { return data()->race_flow_state; }
  void set_race_flow_state_bits(uint32_t v) const { data()->race_flow_state = v; }
  // the race states' state machine (race-states.md)
  uint32_t race_states_address() const { return address + 107168; }
  uint8_t* race_states() const { return reinterpret_cast<uint8_t*>(data()) + 107168; }
  uint32_t race_states_count_address() const { return address + 116768; }
  uint32_t race_states_count() const { return data()->race_states_count; }
  void set_race_states_count(uint32_t v) const { data()->race_states_count = v; }
  uint32_t race_states_count_bits() const { return data()->race_states_count; }
  void set_race_states_count_bits(uint32_t v) const { data()->race_states_count = v; }
  uint32_t race_states_state_address() const { return address + 116772; }
  uint32_t race_states_state() const { return data()->race_states_state; }
  void set_race_states_state(uint32_t v) const { data()->race_states_state = v; }
  uint32_t race_states_state_bits() const { return data()->race_states_state; }
  void set_race_states_state_bits(uint32_t v) const { data()->race_states_state = v; }
  // the race world's state machine (race-world.md)
  uint32_t race_world_address() const { return address + 116812; }
  uint8_t* race_world() const { return reinterpret_cast<uint8_t*>(data()) + 116812; }
  uint32_t race_world_count_address() const { return address + 126412; }
  uint32_t race_world_count() const { return data()->race_world_count; }
  void set_race_world_count(uint32_t v) const { data()->race_world_count = v; }
  uint32_t race_world_count_bits() const { return data()->race_world_count; }
  void set_race_world_count_bits(uint32_t v) const { data()->race_world_count = v; }
  uint32_t race_world_state_address() const { return address + 126416; }
  uint32_t race_world_state() const { return data()->race_world_state; }
  void set_race_world_state(uint32_t v) const { data()->race_world_state = v; }
  uint32_t race_world_state_bits() const { return data()->race_world_state; }
  void set_race_world_state_bits(uint32_t v) const { data()->race_world_state = v; }
  // the race HUD machine (race-hud-machine.md)
  uint32_t race_hud_machine_address() const { return address + 128664; }
  uint8_t* race_hud_machine() const { return reinterpret_cast<uint8_t*>(data()) + 128664; }
  uint32_t race_hud_machine_count_address() const { return address + 138264; }
  uint32_t race_hud_machine_count() const { return data()->race_hud_machine_count; }
  void set_race_hud_machine_count(uint32_t v) const { data()->race_hud_machine_count = v; }
  uint32_t race_hud_machine_count_bits() const { return data()->race_hud_machine_count; }
  void set_race_hud_machine_count_bits(uint32_t v) const { data()->race_hud_machine_count = v; }
  uint32_t race_hud_machine_state_address() const { return address + 138268; }
  uint32_t race_hud_machine_state() const { return data()->race_hud_machine_state; }
  void set_race_hud_machine_state(uint32_t v) const { data()->race_hud_machine_state = v; }
  uint32_t race_hud_machine_state_bits() const { return data()->race_hud_machine_state; }
  void set_race_hud_machine_state_bits(uint32_t v) const { data()->race_hud_machine_state = v; }
  // the race sounds' state machine (race-sounds.md)
  uint32_t race_sounds_address() const { return address + 138284; }
  uint8_t* race_sounds() const { return reinterpret_cast<uint8_t*>(data()) + 138284; }
  uint32_t race_sounds_count_address() const { return address + 147884; }
  uint32_t race_sounds_count() const { return data()->race_sounds_count; }
  void set_race_sounds_count(uint32_t v) const { data()->race_sounds_count = v; }
  uint32_t race_sounds_count_bits() const { return data()->race_sounds_count; }
  void set_race_sounds_count_bits(uint32_t v) const { data()->race_sounds_count = v; }
  uint32_t race_sounds_state_address() const { return address + 147888; }
  uint32_t race_sounds_state() const { return data()->race_sounds_state; }
  void set_race_sounds_state(uint32_t v) const { data()->race_sounds_state = v; }
  uint32_t race_sounds_state_bits() const { return data()->race_sounds_state; }
  void set_race_sounds_state_bits(uint32_t v) const { data()->race_sounds_state = v; }
  // an XPR (XPR::XPR; the race sounds' machine's +9608)
  uint32_t xpr_address() const { return address + 147892; }
  uint8_t* xpr() const { return reinterpret_cast<uint8_t*>(data()) + 147892; }
  // the race overlays' state machine (RaceOverlaysMachine)
  uint32_t overlays_machine_address() const { return address + 147944; }
  uint8_t* overlays_machine() const { return reinterpret_cast<uint8_t*>(data()) + 147944; }
  uint32_t overlays_machine_count_address() const { return address + 157544; }
  uint32_t overlays_machine_count() const { return data()->overlays_machine_count; }
  void set_overlays_machine_count(uint32_t v) const { data()->overlays_machine_count = v; }
  uint32_t overlays_machine_count_bits() const { return data()->overlays_machine_count; }
  void set_overlays_machine_count_bits(uint32_t v) const { data()->overlays_machine_count = v; }
  uint32_t overlays_machine_state_address() const { return address + 157548; }
  uint32_t overlays_machine_state() const { return data()->overlays_machine_state; }
  void set_overlays_machine_state(uint32_t v) const { data()->overlays_machine_state = v; }
  uint32_t overlays_machine_state_bits() const { return data()->overlays_machine_state; }
  void set_overlays_machine_state_bits(uint32_t v) const { data()->overlays_machine_state = v; }
  // the race overlays' list (RaceOverlays::Clear, race-overlays.md)
  uint32_t overlays_address() const { return address + 157552; }
  uint8_t* overlays() const { return reinterpret_cast<uint8_t*>(data()) + 157552; }
  // a state machine RaceMode::Init doesn't set up or add
  uint32_t machine_157652_address() const { return address + 157652; }
  uint8_t* machine_157652() const { return reinterpret_cast<uint8_t*>(data()) + 157652; }
  uint32_t machine_157652_count_address() const { return address + 167252; }
  uint32_t machine_157652_count() const { return data()->machine_157652_count; }
  void set_machine_157652_count(uint32_t v) const { data()->machine_157652_count = v; }
  uint32_t machine_157652_count_bits() const { return data()->machine_157652_count; }
  void set_machine_157652_count_bits(uint32_t v) const { data()->machine_157652_count = v; }
  uint32_t machine_157652_state_address() const { return address + 167256; }
  uint32_t machine_157652_state() const { return data()->machine_157652_state; }
  void set_machine_157652_state(uint32_t v) const { data()->machine_157652_state = v; }
  uint32_t machine_157652_state_bits() const { return data()->machine_157652_state; }
  void set_machine_157652_state_bits(uint32_t v) const { data()->machine_157652_state = v; }
  // 1 after RaceMode::Init; RaceMode::NextMode posts Update only while it isn't 0 (race-loading.md)
  uint32_t running_address() const { return address + 167260; }
  uint32_t running() const { return data()->running; }
  void set_running(uint32_t v) const { data()->running = v; }
  uint32_t running_bits() const { return data()->running; }
  void set_running_bits(uint32_t v) const { data()->running = v; }
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

// embedded; measure: Buffer28, CameraPath::OnLoaded.r6, Buffer28::Reset.r3
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

// embedded; measure: XprHolder::XprHolder.r3
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
#pragma pack(push, 1)
struct GameModeType6Data {
  uint32_t vtable = 0;  // +0
  uint32_t delegate_ptr = 0;  // +4
  uint32_t delegate_vtable = 0;  // +8
  uint32_t delegate_object = 0;  // +12
  uint32_t delegate_function = 0;  // +16
  uint32_t delegate_adjustment = 0;  // +20
  uint8_t pad_24[8] = {};
  uint32_t vtable32 = 0;  // +32
  uint8_t handlers[200] = {};  // +36, guest order
  uint32_t handler_count = 0;  // +236
  uint8_t ring[600] = {};  // +240, guest order
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
  uint32_t word_848 = 0;  // +848
};
#pragma pack(pop)
static_assert(sizeof(GameModeType6Data) == 852);
inline constexpr memory::LayoutField kGameModeType6Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kGameModeType6Layout = {kGameModeType6Fields, 11};

struct GameModeType6 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82010A48;
  GameModeType6Data* data() const { return static_cast<GameModeType6Data*>(memory::HandleObject(address)); }
  static GameModeType6 New(uint8_t* base) {
    return GameModeType6(base, memory::MakeHandle(new GameModeType6Data(), sizeof(GameModeType6Data), "GameModeType6", &kGameModeType6Layout));
  }
  template <uint32_t Stride = kSize>
  static GameModeType6 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(GameModeType6Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<GameModeType6Data*>(::operator new(sizeof(GameModeType6Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) GameModeType6Data();
    return GameModeType6(base, memory::MakeHandle(p, sizeof(GameModeType6Data) * count, "GameModeType6", &kGameModeType6Layout, sizeof(GameModeType6Data)));
  }
  GameModeType6 at(uint32_t i) const { return GameModeType6(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kGameModeType6Layout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kGameModeType6Layout, offset, width, v); }
  // 0x82010A48
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the delegate at +8 (GameMode's)
  uint32_t delegate_ptr_address() const { return address + 4; }
  uint32_t delegate_ptr() const { return data()->delegate_ptr; }
  void set_delegate_ptr(uint32_t v) const { data()->delegate_ptr = v; }
  uint32_t delegate_ptr_bits() const { return data()->delegate_ptr; }
  void set_delegate_ptr_bits(uint32_t v) const { data()->delegate_ptr = v; }
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable() const { return data()->delegate_vtable; }
  void set_delegate_vtable(uint32_t v) const { data()->delegate_vtable = v; }
  uint32_t delegate_vtable_bits() const { return data()->delegate_vtable; }
  void set_delegate_vtable_bits(uint32_t v) const { data()->delegate_vtable = v; }
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object() const { return data()->delegate_object; }
  void set_delegate_object(uint32_t v) const { data()->delegate_object = v; }
  uint32_t delegate_object_bits() const { return data()->delegate_object; }
  void set_delegate_object_bits(uint32_t v) const { data()->delegate_object = v; }
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function() const { return data()->delegate_function; }
  void set_delegate_function(uint32_t v) const { data()->delegate_function = v; }
  uint32_t delegate_function_bits() const { return data()->delegate_function; }
  void set_delegate_function_bits(uint32_t v) const { data()->delegate_function = v; }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment() const { return data()->delegate_adjustment; }
  void set_delegate_adjustment(uint32_t v) const { data()->delegate_adjustment = v; }
  uint32_t delegate_adjustment_bits() const { return data()->delegate_adjustment; }
  void set_delegate_adjustment_bits(uint32_t v) const { data()->delegate_adjustment = v; }
  // 0x82010A3C
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32() const { return data()->vtable32; }
  void set_vtable32(uint32_t v) const { data()->vtable32 = v; }
  uint32_t vtable32_bits() const { return data()->vtable32; }
  void set_vtable32_bits(uint32_t v) const { data()->vtable32 = v; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return reinterpret_cast<uint8_t*>(data()) + 36; }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count() const { return data()->handler_count; }
  void set_handler_count(uint32_t v) const { data()->handler_count = v; }
  uint32_t handler_count_bits() const { return data()->handler_count; }
  void set_handler_count_bits(uint32_t v) const { data()->handler_count = v; }
  // 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return reinterpret_cast<uint8_t*>(data()) + 240; }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count() const { return data()->ring_count; }
  void set_ring_count(uint32_t v) const { data()->ring_count = v; }
  uint32_t ring_count_bits() const { return data()->ring_count; }
  void set_ring_count_bits(uint32_t v) const { data()->ring_count = v; }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start() const { return data()->ring_start; }
  void set_ring_start(uint32_t v) const { data()->ring_start = v; }
  uint32_t ring_start_bits() const { return data()->ring_start; }
  void set_ring_start_bits(uint32_t v) const { data()->ring_start = v; }
  // 0 at construction (before the vtables); not read here
  uint32_t word_848_address() const { return address + 848; }
  uint32_t word_848() const { return data()->word_848; }
  void set_word_848(uint32_t v) const { data()->word_848 = v; }
  uint32_t word_848_bits() const { return data()->word_848; }
  void set_word_848_bits(uint32_t v) const { data()->word_848 = v; }
};

// allocated; measure: ModeType7
#pragma pack(push, 1)
struct GameModeType7Data {
  uint32_t vtable = 0;  // +0
  uint32_t delegate_ptr = 0;  // +4
  uint32_t delegate_vtable = 0;  // +8
  uint32_t delegate_object = 0;  // +12
  uint32_t delegate_function = 0;  // +16
  uint32_t delegate_adjustment = 0;  // +20
  uint8_t pad_24[8] = {};
  uint32_t vtable32 = 0;  // +32
  uint8_t handlers[200] = {};  // +36, guest order
  uint32_t handler_count = 0;  // +236
  uint8_t ring[600] = {};  // +240, guest order
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
  uint32_t block = 0;  // +848
};
#pragma pack(pop)
static_assert(sizeof(GameModeType7Data) == 852);
inline constexpr memory::LayoutField kGameModeType7Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kGameModeType7Layout = {kGameModeType7Fields, 11};

struct GameModeType7 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82010FB0;
  GameModeType7Data* data() const { return static_cast<GameModeType7Data*>(memory::HandleObject(address)); }
  static GameModeType7 New(uint8_t* base) {
    return GameModeType7(base, memory::MakeHandle(new GameModeType7Data(), sizeof(GameModeType7Data), "GameModeType7", &kGameModeType7Layout));
  }
  template <uint32_t Stride = kSize>
  static GameModeType7 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(GameModeType7Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<GameModeType7Data*>(::operator new(sizeof(GameModeType7Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) GameModeType7Data();
    return GameModeType7(base, memory::MakeHandle(p, sizeof(GameModeType7Data) * count, "GameModeType7", &kGameModeType7Layout, sizeof(GameModeType7Data)));
  }
  GameModeType7 at(uint32_t i) const { return GameModeType7(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kGameModeType7Layout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kGameModeType7Layout, offset, width, v); }
  // 0x82010FB0
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the delegate at +8 (GameMode's)
  uint32_t delegate_ptr_address() const { return address + 4; }
  uint32_t delegate_ptr() const { return data()->delegate_ptr; }
  void set_delegate_ptr(uint32_t v) const { data()->delegate_ptr = v; }
  uint32_t delegate_ptr_bits() const { return data()->delegate_ptr; }
  void set_delegate_ptr_bits(uint32_t v) const { data()->delegate_ptr = v; }
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable() const { return data()->delegate_vtable; }
  void set_delegate_vtable(uint32_t v) const { data()->delegate_vtable = v; }
  uint32_t delegate_vtable_bits() const { return data()->delegate_vtable; }
  void set_delegate_vtable_bits(uint32_t v) const { data()->delegate_vtable = v; }
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object() const { return data()->delegate_object; }
  void set_delegate_object(uint32_t v) const { data()->delegate_object = v; }
  uint32_t delegate_object_bits() const { return data()->delegate_object; }
  void set_delegate_object_bits(uint32_t v) const { data()->delegate_object = v; }
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function() const { return data()->delegate_function; }
  void set_delegate_function(uint32_t v) const { data()->delegate_function = v; }
  uint32_t delegate_function_bits() const { return data()->delegate_function; }
  void set_delegate_function_bits(uint32_t v) const { data()->delegate_function = v; }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment() const { return data()->delegate_adjustment; }
  void set_delegate_adjustment(uint32_t v) const { data()->delegate_adjustment = v; }
  uint32_t delegate_adjustment_bits() const { return data()->delegate_adjustment; }
  void set_delegate_adjustment_bits(uint32_t v) const { data()->delegate_adjustment = v; }
  // 0x82010FA4
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32() const { return data()->vtable32; }
  void set_vtable32(uint32_t v) const { data()->vtable32 = v; }
  uint32_t vtable32_bits() const { return data()->vtable32; }
  void set_vtable32_bits(uint32_t v) const { data()->vtable32 = v; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return reinterpret_cast<uint8_t*>(data()) + 36; }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count() const { return data()->handler_count; }
  void set_handler_count(uint32_t v) const { data()->handler_count = v; }
  uint32_t handler_count_bits() const { return data()->handler_count; }
  void set_handler_count_bits(uint32_t v) const { data()->handler_count = v; }
  // 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return reinterpret_cast<uint8_t*>(data()) + 240; }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count() const { return data()->ring_count; }
  void set_ring_count(uint32_t v) const { data()->ring_count = v; }
  uint32_t ring_count_bits() const { return data()->ring_count; }
  void set_ring_count_bits(uint32_t v) const { data()->ring_count = v; }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start() const { return data()->ring_start; }
  void set_ring_start(uint32_t v) const { data()->ring_start = v; }
  uint32_t ring_start_bits() const { return data()->ring_start; }
  void set_ring_start_bits(uint32_t v) const { data()->ring_start = v; }
  // the screen block (type6-type7-modes.toml's Type7Mode.block)
  uint32_t block_address() const { return address + 848; }
  uint32_t block() const { return data()->block; }
  void set_block(uint32_t v) const { data()->block = v; }
  uint32_t block_bits() const { return data()->block; }
  void set_block_bits(uint32_t v) const { data()->block = v; }
};

// embedded; measure: Buffers28::~Buffers28.r3@+43100
struct CameraPathBufferAt43100 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: RaceMode@+147944
struct RaceOverlaysMachineAt147944 : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
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
