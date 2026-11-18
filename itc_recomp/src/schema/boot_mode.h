// The views of docs/functions/boot-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/boot-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Mode, BootMode, Mode::NextModeThunk.r4
struct GameMode : View {
  using View::View;
  static constexpr uint32_t kSize = 848;
  static constexpr uint32_t kVtable = 0x820069EC;
  // 0x820069EC the base's, 0x82006B70 the boot mode's, 0x82006B44 ModeType3's
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
  // the delegate (ModeDelegate): vtable 0x82010FA0
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_address() const { return address + 8; }
  // the delegate's object: the mode
  uint32_t delegate_object() const { return guest::LoadU32(base, address + 12); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_address() const { return address + 12; }
  // 0x82235C08 Mode::NextModeThunk for the boot and race modes
  uint32_t delegate_function() const { return guest::LoadU32(base, address + 16); }
  void set_delegate_function(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_adjustment() const { return guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  // the second vtable: 0x82006A0C the base's, 0x82006B64 the boot mode's
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // the dispatcher's ring: 50 messages of 12 bytes (id, two words)
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
};

// allocated; measure: BootMode::NextMode.r4
struct BootMode : View {
  using View::View;
  static constexpr uint32_t kSize = 10460;
  static constexpr uint32_t kVtable = 0x82006B70;
  // 0x82006B70
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // 0x82006B64
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // 50 messages of 12 bytes
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // the boot machine (BootMachine)
  uint32_t machine_address() const { return address + 848; }
  // +848's state: 0 before the movie, 1 playing, 2 ended
  uint32_t machine_state() const { return guest::LoadU32(base, address + 10452); }
  void set_machine_state(uint32_t v) const { guest::StoreU32(base, address + 10452, v); }
  uint32_t machine_state_address() const { return address + 10452; }
  // +848's +9608: 1 once Movies::Start was called
  uint32_t movie_started() const { return guest::LoadU32(base, address + 10456); }
  void set_movie_started(uint32_t v) const { guest::StoreU32(base, address + 10456, v); }
  uint32_t movie_started_address() const { return address + 10456; }
};

// embedded; measure: Boot, BootMachine
struct BootMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  // 200 transitions of 48 bytes
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 before the movie, 1 playing, 2 ended
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once Movies::Start was called (Boot::WaitForMovie); 0 from BootMachine::Init
  uint32_t movie_started() const { return guest::LoadU32(base, address + 9608); }
  void set_movie_started(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t movie_started_address() const { return address + 9608; }
};

// embedded; measure: Boot::WaitForMovie.r4
struct ModeDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 0; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 200); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t handler_count_address() const { return address + 200; }
  // 50 messages of 12 bytes (id, two words)
  uint32_t ring_address() const { return address + 204; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
};

// embedded; measure: sub_82107D80.r3
struct BootModeInterface32 : View {
  using View::View;
  static constexpr uint32_t kSize = 816;
  // 0x82006B64
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the mode's ring (+240)
  uint32_t ring_address() const { return address + 208; }
  // the mode's +840
  uint32_t ring_count() const { return guest::LoadU32(base, address + 808); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_count_address() const { return address + 808; }
  // the mode's +844
  uint32_t ring_start() const { return guest::LoadU32(base, address + 812); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t ring_start_address() const { return address + 812; }
};

// image; measure: Mode::NextModeThunk.r4.+0
struct GameModeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 0: BootMode::Init for the boot mode
  uint32_t init() const { return guest::LoadU32(base, address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  uint32_t slot1() const { return guest::LoadU32(base, address + 4); }
  void set_slot1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot1_address() const { return address + 4; }
  // slots 2 to 5: Mode::NoNextMode in every mode
  uint32_t slot2() const { return guest::LoadU32(base, address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
  uint32_t slot3() const { return guest::LoadU32(base, address + 12); }
  void set_slot3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot3_address() const { return address + 12; }
  uint32_t slot4() const { return guest::LoadU32(base, address + 16); }
  void set_slot4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot4_address() const { return address + 16; }
  uint32_t slot5() const { return guest::LoadU32(base, address + 20); }
  void set_slot5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot5_address() const { return address + 20; }
  // slot 6: the next-mode query (BootMode::NextMode)
  uint32_t next_mode() const { return guest::LoadU32(base, address + 24); }
  void set_next_mode(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t next_mode_address() const { return address + 24; }
  // slot 7
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 28); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t deleting_destructor_address() const { return address + 28; }
};

// embedded; measure: ModeDelegate::Invoke.r4
struct ModeDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 0x82010FA0
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the mode
  uint32_t object() const { return guest::LoadU32(base, address + 4); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_address() const { return address + 4; }
  // Mode::NextModeThunk (0x82235C08) for the boot and race modes
  uint32_t function() const { return guest::LoadU32(base, address + 8); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t function_address() const { return address + 8; }
  // added to the object
  uint32_t adjustment() const { return guest::LoadU32(base, address + 12); }
  void set_adjustment(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t adjustment_address() const { return address + 12; }
};

// embedded; measure: ModeDelegate
struct ModeAnswer : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 1 to change modes
  uint8_t changed() const { return guest::LoadU8(base, address + 0); }
  void set_changed(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t changed_address() const { return address + 0; }
  // the first word as copied: changed and three bytes the stack's
  uint32_t word0() const { return guest::LoadU32(base, address + 0); }
  void set_word0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word0_address() const { return address + 0; }
  // the next mode's type
  uint32_t next_type() const { return guest::LoadU32(base, address + 4); }
  void set_next_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_type_address() const { return address + 4; }
};

// allocated; measure: ModeType8
struct ModeType8 : View {
  using View::View;
  static constexpr uint32_t kSize = 87768;
  static constexpr uint32_t kVtable = 0x82006D60;
  // 0x82006D60
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // 0x82006D54
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // 50 messages of 12 bytes
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // ModeType8Member
  uint32_t member_address() const { return address + 848; }
};

// embedded; measure: ModeType8Member
struct ModeType8Member : View {
  using View::View;
  static constexpr uint32_t kSize = 86920;
  // Type8Screen0 (its machine +4)
  uint32_t screen0_address() const { return address + 0; }
  // Type8Dialog
  uint32_t dialog_address() const { return address + 9792; }
  // the list screen
  uint32_t list_screen_address() const { return address + 19544; }
  // the color list screen
  uint32_t color_list_screen_address() const { return address + 30912; }
  // the panel screen
  uint32_t panel_screen_address() const { return address + 42740; }
  // the options (a ScreenMachine)
  uint32_t options_address() const { return address + 57992; }
  // the controller pages (a ScreenMachine)
  uint32_t controller_pages_address() const { return address + 67632; }
  // the save screen (a ScreenMachine)
  uint32_t save_screen_address() const { return address + 77272; }
  // the mode's +87760: the 8-byte answer TearDown returns, its byte 1 to change modes; cleared by the constructor
  uint8_t answer_changed() const { return guest::LoadU8(base, address + 86912); }
  void set_answer_changed(uint8_t v) const { guest::StoreU8(base, address + 86912, v); }
  uint32_t answer_changed_address() const { return address + 86912; }
  // the answer's next mode (1 the race, 4 the type 4 front end, 12); cleared by the constructor
  uint32_t answer_next_type() const { return guest::LoadU32(base, address + 86916); }
  void set_answer_next_type(uint32_t v) const { guest::StoreU32(base, address + 86916, v); }
  uint32_t answer_next_type_address() const { return address + 86916; }
};

// The image words docs/functions/boot-mode.md's code uses.
struct BootModeGlobals {
  // the boot movie paused (1); BootMode::NextMode
  static uint32_t movie_paused(uint8_t* base) { return guest::LoadU32(base, 0x826C5BA4u); }
  static void set_movie_paused(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BA4u, v); }
  static constexpr uint32_t movie_paused_address() { return 0x826C5BA4u; }
  // the mode the boot mode is asked to go to (13 none)
  static uint32_t mode_asked(uint8_t* base) { return guest::LoadU32(base, 0x826C5BA8u); }
  static void set_mode_asked(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BA8u, v); }
  static constexpr uint32_t mode_asked_address() { return 0x826C5BA8u; }
  // up to five movie ids, 11 ending the list
  static constexpr uint32_t movie_ids_address() { return 0x826C5BACu; }
  // the mode type after the list (4 at boot)
  static uint32_t next_type(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC0u); }
  static void set_next_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC0u, v); }
  static constexpr uint32_t next_type_address() { return 0x826C5BC0u; }
  // a button skips the movies
  static uint32_t skippable(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC4u); }
  static void set_skippable(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC4u, v); }
  static constexpr uint32_t skippable_address() { return 0x826C5BC4u; }
  // the list's next entry
  static uint32_t movie_index(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC8u); }
  static void set_movie_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC8u, v); }
  static constexpr uint32_t movie_index_address() { return 0x826C5BC8u; }
  // the pad blocked (known-data.md): either word set
  static uint32_t pad_blocked0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_pad_blocked0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t pad_blocked0_address() { return 0x826C7960u; }
  static uint32_t pad_blocked1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_pad_blocked1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t pad_blocked1_address() { return 0x826C7964u; }
  // the passes word ScreenFades::FadeIn is given at a movie's start (5)
  static uint32_t start_fade_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_start_fade_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t start_fade_passes_address() { return 0x82001BECu; }
};

}  // namespace schema
